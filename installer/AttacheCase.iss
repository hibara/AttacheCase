#define MyAppVer GetFileVersion("..\AttacheCase.exe")
#define MyAppVerNum StringChange(MyAppVer, ".", "")
#define BetaVerString "β"
#define BetaVerNum "b"

[Setup]
AppName=アタッシェケース
AppVersion={#MyAppVer}
AppVerName=アタッシェケース ver.{#MyAppVer}{#BetaVerString}
DefaultGroupName=アタッシェケース
OutputBaseFilename=atcs{#MyAppVerNum}{#BetaVerNum}
DefaultDirName={pf}\AttacheCase
UsePreviousAppDir=yes
AppendDefaultDirName=yes 
OutputDir=.\

;-----------------------------------
;インストーラプログラム
;-----------------------------------
VersionInfoVersion={#MyAppVer}
VersionInfoDescription=暗号化ソフトウェア「アタッシェケース」をセットアップするプログラム
AppCopyright=Copyright(C) 2012 M.Hibara, All rights reserved.
;SetupIconFile=icon\main_icon.ico
;ウィザードページに表示されるグラフィック（*.bmp: 164 x 314）
WizardImageFile=bmp\installer_pic_01.bmp
;ウィザードページに表示されるグラフィックが拡大されない
WizardImageStretch=no
;その隙間色
WizardImageBackColor=$ffffff 
;ウィザードページの右上部分のグラフィック（*.bmp: 55 x 58）
WizardSmallImageFile=bmp\installer_pic_02.bmp
;進捗表示？
ShowTasksTreeLines=yes

;------------------------------------------
;「プログラムの追加と削除」ダイアログ情報
;------------------------------------------
;配布元
AppPublisher=Hibara, Mitsuhiro
;アプリケーション配布元 Webサイトの URL
AppPublisherURL=http://hibara.org/
;連絡先
AppContact=m@hibara.org
;サポートサイトURL
AppSupportURL=http://hibara.org/software/
;ReadMeファイルパス
AppReadmeFile="{app}\AttacheCase\readme.txt"
;製品更新先のURL
AppUpdatesURL=http://hibara.org/software/attachecase/
;アプリケーションの説明
AppComments=ファイル/フォルダー暗号化ソフトウェア

[Files]
Source: "bin\AttacheCase.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\AtcSetup.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\AttacheCase.chm"; DestDir: "{app}"; Flags: ignoreversion
Source: "bin\readme.txt"; DestDir: "{app}"; Flags: isreadme  ignoreversion

[Languages]
Name: japanese; MessagesFile: compiler:Languages\Japanese.isl

[Icons]
Name: "{group}\アタッシェケース"; Filename: "{app}\AttacheCase.exe"; WorkingDir: "{app}"
Name: "{group}\ヘルプファイル"; Filename: "{app}\AttacheCase.chm"; WorkingDir: "{app}"
Name: "{group}\アンインストール"; Filename: "{uninstallexe}"
Name: "{commondesktop}\アタッシェケース"; Filename: "{app}\AttacheCase.exe"; WorkingDir: "{app}"; Tasks: desktopicon

[Tasks]
Name: desktopicon; Description: "デスクトップにショートカットアイコンを作成する"
Name: association; Description: "*.ATCファイルをアタッシェケースに関連付けする";

[Run]
Filename: "{app}\AtcSetup.exe"; Parameters: "0"; Tasks: association; Flags: nowait skipifsilent runascurrentuser

[Registry]
;（アンインストール時に）アタッシェケースの動作設定を削除
Root: HKCU; Subkey: "Software\Hibara\AttacheCase"; Flags: uninsdeletekey
;（アンインストール時に）関連付け設定を削除
Root: HKCR; Subkey: "AttacheCase.DataFile"; Flags: uninsdeletekey
Root: HKCR; Subkey: ".atc"; Flags: uninsdeletekey


