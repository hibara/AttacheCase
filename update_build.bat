@echo OFF
@echo. ======================================================================
@echo. このバッチファイルでは、必要なファイルを各所からコピーしてきて、
@echo. 最終的な製品として 「アタッシェケース」をパックし、インストーラーを
@echo. 作成するところまで行います。この処理には、あらかじめインストールして
@echo. おくべきツールがいくつかあります。
@echo. 
@echo. 以下のツールが所定の場所にインストールされていることを確認してから
@echo. 実行してください。
@echo. 
@echo. ■Inno Setup（インストーラー作成ソフト）
@echo. ■UPX（実行ファイル圧縮ソフト）
@echo. 
@echo. ======================================================================

set /p NUM="ビルドを開始します。Enterキーを押してください。>"

@echo. 
@echo. -----------------------------------
@echo. HTML Help Workshopファイルの生成
@echo. -----------------------------------

if "%PROCESSOR_ARCHITECTURE%" == "AMD64" (
"%ProgramFiles(x86)%\HTML Help Workshop\hhc.exe" help\AttacheCase.hhp
) else (
"%ProgramFiles%\HTML Help Workshop\hhc.exe" help\AttacheCase.hhp
)

copy help\AttacheCase.chm AttacheCase.chm


@echo 
@echo -----------------------------------
@echo 必要ファイルのコピー
@echo -----------------------------------

mkdir installer\bin

@rem  必要ファイルをinstallerへコピー
copy readme\readme.txt installer\bin\readme.txt
copy help\AttacheCase.chm installer\bin\AttacheCase.chm
copy AtcSetup\AtcSetup.exe installer\bin\AtcSetup.exe
copy exeout\exeout.exe installer\bin\exeout.exe
copy AttacheCase.exe installer\bin\AttacheCase.exe


@echo. 
@echo. -----------------------------------
@echo. UPXでの処理
@echo. -----------------------------------

upx308w\upx --compress-icons=0 -6 installer\bin\AttacheCase.exe
upx308w\upx --compress-resources=0 --best installer\bin\exeout.exe


@echo. 
@echo. -----------------------------------
@echo. 自己実行ファイル形式のマージ
@echo. -----------------------------------

tools\merge\merge.exe installer\bin\AttacheCase.exe installer\bin\exeout.exe


@echo. 
@echo. -----------------------------------
@echo. インストーラパッケージの生成
@echo. -----------------------------------

if "%PROCESSOR_ARCHITECTURE%" == "AMD64" (
"%ProgramFiles(x86)%\Inno Setup 5\ISCC.exe" installer\AttacheCase.iss
) else (
"%ProgramFiles%\Inno Setup 5\ISCC.exe" installer\AttacheCase.iss
)

@echo. 
@echo. -----------------------------------
@echo. ハッシュ値をテキストファイル保存
@echo. -----------------------------------

tools\gethash\gethash.exe installer\*.exe


@echo. 
@echo. -----------------------------------
@echo. binディレクトリの削除
@echo. -----------------------------------

rd /s /q "installer\bin"

:END

@echo 
@echo **********************************************************************
@echo 処理が完了しました。
@echo **********************************************************************
@echo 


pause

