//===========================================================================
/*

アタッシェケース（AttachéCase）
Copyright (c) 2002-2018, Mitsuhiro Hibara ( http://hibara.org )
All rights reserved.

Redistribution and use in source and binary forms,
with or without modification, are permitted provided that the following
conditions are met:

・Redistributions of source code must retain the above copyright
	notice, this list of conditions and the following disclaimer.
・Redistributions in binary form must reproduce the above copyright
	notice, this list of conditions and the following disclaimer
	in the documentation and/or other materials provided with the
	distribution.
・Neither the name of the "HIBARA.ORG" nor the names of its
	contributors  may be used to endorse or promote products derived
	from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
//===========================================================================
#include <vcl.h>

#pragma hdrstop

#include "TAttacheCaseOptionHandle.h"
//===========================================================================
//コンストラクタ
//===========================================================================
__fastcall TAttacheCaseOptionHandle::TAttacheCaseOptionHandle() : TObject()
{

	OptionPath = "";                // 読み込んだ先のパス（INIファイルパス）
	OptType = 0;                    // 0:レジストリ, 1:INIファイル, 2:コマンドライン引数

	fOldVersionExists = false;      // 古いバージョンを併用しているか
	fRestoreRegistory = false;      // レジストリのリストア

	//アプリケーション情報
	String AppPath = "";            // 本体がインストールされたフルパス
	VersionNum = 0;                 // バージョン

	//フォームポジション
	FormTop = 0;
	FormLeft = 0;
	FormWidth = 420;
	FormHeight = 360;
	WinStyle = 0;

	ActiveTabNum = 0;               // 開いていたタブ

	//【基本設定】
	MyPassword = "";                // 記憶しているパスワード

	fArg = false;                   // 実行引数処理をするか
	fArgPassword = false;           // 実行引数にパスワードを指定

	fMyEncodePasswordKeep = false;  // 暗号化パスワードを記憶するか
	fMyDecodePasswordKeep = false;  // 復号パスワードを記憶するか
	MyEncodePassword = "";          // 記憶暗号化パスワード
	MyDecodePassword = "";          // 記憶復号化パスワード

	fMainWindowMinimize = false;    // 常にウィンドウを最小化して処理する
	fTaskBarHide = false;           // タスクバーに表示しない
	fTaskTrayIcon = false;          // タスクトレイにアイコンを表示する
	fMemPasswordExe = false;        // 記憶パスワードで即座に実行する
	fOpenFolder = false;            // フォルダの場合に復号後に開くか
	fOpenFile = false;              // 復号したファイルを関連付けされたソフトで開く
	fEndToExit = false;             // 処理後に終了するか
	fWindowForeground = true;       // デスクトップで最前面にウィンドウを表示する
	fNoHidePassword = false;        //「*」で隠さずパスワードを確認しながら入力する
	fSaveToExeout = false;          // 常に自己実行形式で出力する
	fShowExeoutChkBox = true;       // メインフォームにチェックボックスを表示する
	fAskEncDecode = false;          // 暗号/復号処理かを問い合わせる
	ProcTypeWithoutAsk = 0;         // 暗号/復号処理か（動作設定にはない。コマンドラインのみ）
	fNoMultipleInstance = false;    // 複数起動しない

	//【保存設定】
	fSaveToSameFldr = false;        // 暗号化ファイルを常に同じ場所に保存するか
	SaveToSameFldrPath = "";        // その保存場所
	fDecodeToSameFldr = false;      // 常に同じ場所へファイルを復号するか
	DecodeToSameFldrPath = "";      // その保存場所
	fConfirmOverwirte = true;       // 同名ファイルの上書きを確認するか
	fKeepTimeStamp = false;         // 暗号化ファイルのタイムスタンプを元ファイルに合わせる
	fSameTimeStamp = false;         // 復号したファイルのタイムスタンプを生成日時にする
	fAllFilePack = false;           // 複数のファイルを暗号化する際は一つにまとめる
	fFilesOneByOne = false;         // フォルダ内のファイルは個別に暗号化/復号する
	fNoParentFldr = false;          // 復号するときに親フォルダを生成しない
	fExtInAtcFileName = false;      // 暗号化ファイル名に拡張子を含める
	fAutoName = false;              // 自動で暗号化ファイル名を付加する
	AutoNameFormatText = "";	      // 自動で付加するファイル名書式

	//【削除設定】
	fDelOrgFile = false;            // 暗号化した後、元ファイルを削除する
	fDelEncFile = false;            // 復号化した後、暗号化ファイルを削除する
	fShowDeleteChkBox = false;      // メインフォームにチェックボックスを表示する
	fCompleteDelete = 0;            // 完全削除を行うか(0:通常，1:完全削除，2:ごみ箱）
	DelRandNum = 0;                 // 乱数を何回書き込み消去するか
	DelZeroNum = 0;                 // NULLを何回書き込み消去するか

	//【動作設定】
	CompressRate = 6;               // 圧縮率
	fCompareFile = false;           // 暗号処理後にファイルコンペアを行うか

	//【システム】
	fAssociationFile = true;        // 関連付け設定を保持するか
	AtcsFileIconIndex = 1;          // ファイルアイコン番号
	UserRegIconFilePath = "";       // ユーザー指定のファイルアイコンパス

	//【高度な設定】
	fAllowPassFile = false;         // パスワードファイルを許可する
	fCheckPassFile = false;         // 暗号時にパスワードファイルを自動チェックする
	PassFilePath = "";              // 暗号時のパスワードファイルパス
	fCheckPassFileDecrypt = false;  // 復号時にパスワードファイルを自動チェックする
	PassFilePathDecrypt = "";       // 復号時のパスワードファイルパス
	fNoErrMsgOnPassFile = false;    // パスワードファイルがない場合エラーを出さない
	fAddCamoExt = false;            // 暗号化ファイルの拡張子を偽装する
	CamoExt = "";                   // その拡張子
	MissTypeLimitsNum = 3;          // パスワードのタイプミス制限回数（ver.2.70～）
	fBroken = false;                // タイプミス回数を超えたときにファイルを破壊するか否か（ver.2.70～）

	//【コマンドラインからのみ指定】
	fOver4GBok = false;             // 4GB超えを容認
	fHideMainForm = false;          // メインフォームを非表示
	//int WinStyle;                 // ウィンドウの状態(0: wsNormal, 1: wsMinimized, 2: wsMaximized）
	fNoErrorMsg = false;            // エラーメッセージ表示の抑制

}
//===========================================================================
//デストラクタ
//===========================================================================
__fastcall TAttacheCaseOptionHandle::~TAttacheCaseOptionHandle()
{

TCustomIniFile *pOpt;
TRegistry *reg;

if (OptType == 2) {	//コマンドラインからの起動は記憶しない
	return;
}

try{

	reg = new TRegistry();

	if ( FileExists(OptionPath) == true ){
		//通常なら読み込んだ先へ保存する
		pOpt = new TIniFile(OptionPath);
	}
	else{
		//なければレジストリへ
		pOpt = new TRegistryIniFile(ATTACHE_CASE_REGISTRY_PATH);
	}

	//-----------------------------------
	// フォームポジションだけは記憶する
	//-----------------------------------
	pOpt->WriteInteger( "WindowPos", "WindowWidth", FormWidth);
	pOpt->WriteInteger( "WindowPos", "WindowHeight", FormHeight);
	pOpt->WriteInteger( "WindowPos", "WindowTop", FormTop);
	pOpt->WriteInteger( "WindowPos", "WindowLeft", FormLeft);
	pOpt->WriteInteger( "WindowPos", "FormStyle", WinStyle);

	//-----------------------------------
	// バックアップされたレジストリからリストアされたので
	// バックアップ先を消去
	//-----------------------------------
	if (fRestoreRegistory == true) {
		pOpt = new TRegistryIniFile(HIBARA_REGISTRY_PATH);
		pOpt->EraseSection("AttacheCase.BAK");
	}

	//-----------------------------------
	// 旧バージョンと併用しているのならレジストリバックアップ
	//-----------------------------------
	if (fOldVersionExists == true || reg->OpenKey(ATTACHE_CASE_BAK_REGISTRY_PATH, false) == true) {
		OptType = -1;
		SaveOptionData();
	}

}
__finally{

	delete reg;
	delete pOpt;

}


}
//===========================================================================
//動作設定の読み込み
//===========================================================================
bool __fastcall TAttacheCaseOptionHandle::LoadOptionData(String IniFilePath)
{

int CurrentVersion;

TRegistry *reg;
TCustomIniFile *pOpt;
TGetAppInfoString *pAppInfoString;

try{

	if ( FileExists(IniFilePath) == true ){
		// INIファイルから読み込み
		OptionPath = IniFilePath;
		pOpt = new TIniFile(OptionPath);
		OptType = 1;
	}
	else{
		// レジストリから読み込み
		OptionPath = ATTACHE_CASE_REGISTRY_PATH;	//"Software\\Hibara\\AttacheCase"

		reg = new TRegistry();
		if (reg->OpenKey(OptionPath, false) == false) {
			//メインレジストリがなく、バックアップのみ残っている
			if ( reg->OpenKey(ATTACHE_CASE_BAK_REGISTRY_PATH, false) == true ){
				OptionPath = ATTACHE_CASE_BAK_REGISTRY_PATH;
				fRestoreRegistory = true;	//リストア
			}
		}
		pOpt = new TRegistryIniFile(OptionPath);
		OptType = 0;
	}

	//-----------------------------------
	//アプリケーション情報
	//-----------------------------------

	//レジストリに記録されているバージョン番号
	CurrentVersion = pOpt->ReadInteger( "AppInfo", "AppVersion", -1);

	if ( CurrentVersion < 2800 ) { //古いバージョンを併用中
		fOldVersionExists = true;
	}
	else{
		fOldVersionExists = false;
	}

	AppPath = Application->ExeName;
	//バージョン番号
	pAppInfoString = new TGetAppInfoString();
	VersionNum = StrToIntDef(StringReplace(pAppInfoString->FileVersion, ".", "", TReplaceFlags()<<rfReplaceAll), -1);

	//-----------------------------------
	// フォームポジションなど
	//-----------------------------------
	FormWidth = pOpt->ReadInteger( "WindowPos", "WindowWidth", 420);
	FormHeight = pOpt->ReadInteger( "WindowPos", "WindowHeight", 360);
	FormTop = pOpt->ReadInteger( "WindowPos", "WindowTop", Screen->Height/2-FormHeight/2);
	FormLeft = pOpt->ReadInteger( "WindowPos", "WindowLeft", Screen->Width/2-FormWidth/2);
	WinStyle = pOpt->ReadInteger( "WindowPos", "FormStyle", 0);

	//----------------------------------------------------------------------
	// 基本設定
	//----------------------------------------------------------------------
	//記憶復号化パスワード
	fMyEncodePasswordKeep = pOpt->ReadBool( "MyKey", "fKeep", false);
	fMyDecodePasswordKeep = pOpt->ReadBool( "MyKey", "fKeep01", false);
	MyEncodePassword = ReadMyPasswordFromRegistry(TYPE_ENCODE_FILE);
	MyDecodePassword = ReadMyPasswordFromRegistry(TYPE_DECODE_FILE);

	//記憶するにチェックが入っているのにパスワードが空文字の場合は
	//非チェックにする
	if ( fMyEncodePasswordKeep == true ) {
		if (MyEncodePassword == "") {
			fMyEncodePasswordKeep = false;
		}
	}
	if ( fMyDecodePasswordKeep == true ) {
		if (MyDecodePassword == "") {
			fMyDecodePasswordKeep = false;
		}
	}

	fMainWindowMinimize = pOpt->ReadBool( "Option", "fMainWindowMinimize", false);//常にウィンドウを最小化して処理する
	fTaskBarHide = pOpt->ReadBool( "Option", "fTaskBarHide", false);              //タスクバーに表示しない
	fTaskTrayIcon = pOpt->ReadBool( "Option", "fTaskTrayIcon", false);            //タスクトレイにアイコンを表示する
	fMemPasswordExe = pOpt->ReadBool( "Option", "fMemPasswordExe", true);         //記憶パスワードで確認なく実行する
	fOpenFolder = pOpt->ReadBool( "Option", "fOpenFolder", false);                //フォルダを開く
	fOpenFile = pOpt->ReadBool( "Option", "fOpenFile", false);                    //復号したファイルを関連付けされたソフトで開く
	fEndToExit = pOpt->ReadBool( "Option", "fEndToExit", false);                  //処理後、アプリを終了する
	fWindowForeground = pOpt->ReadBool( "Option", "fWindowForeground", true);     //デスクトップで最前面にウィンドウを表示する
	fNoHidePassword = pOpt->ReadBool( "Option", "fNoHidePassword", false);        //「*」で隠さずパスワードを確認しながら入力する
	fSaveToExeout = pOpt->ReadBool( "Option", "fSaveToExeout", false);            //常に自己実行形式で出力する
	fShowExeoutChkBox = pOpt->ReadBool( "Option", "fShowExeoutChkBox", true);     //メインフォームにチェックボックスを表示する
	fAskEncDecode = pOpt->ReadBool( "Option", "fAskEncDecode", false);            //暗号/復号処理かを問い合わせる
	fNoMultipleInstance = pOpt->ReadBool( "Option", "fNoMultipleInstance", true); //複数起動しない

	ProcTypeWithoutAsk = -1;     //明示的な暗号/復号処理か（コマンドラインからのみ）

	//----------------------------------------------------------------------
	// 保存設定
	//----------------------------------------------------------------------
	fSaveToSameFldr = pOpt->ReadBool( "Option", "fSaveToSameFldr", false);          //常に同じ場所に保存するか
	SaveToSameFldrPath = pOpt->ReadString( "Option", "SaveToSameFldrPath", "");
	fDecodeToSameFldr = pOpt->ReadInteger( "Option", "fDecodeToSameFldr", false);   //常に同じ場所へファイルを復号化するか
	DecodeToSameFldrPath = pOpt->ReadString( "Option", "DecodeToSameFldrPath", ""); //その保存場所
	fConfirmOverwirte = pOpt->ReadBool( "Option", "ConfirmOverwite", true);         //同名ファイルの上書きを確認するか
	fKeepTimeStamp = pOpt->ReadBool( "Option", "fKeepTimeStamp", false);            //暗号化ファイルのタイムスタンプを元ファイルに合わせる
	fSameTimeStamp = pOpt->ReadBool( "Option", "fSameTimeStamp", false);            //復号したファイルのタイムスタンプを生成日時にする
	fAllFilePack = pOpt->ReadBool( "Option", "fAllFilePack", false);                //複数のファイルを暗号化する際は一つにまとめる
	fFilesOneByOne = pOpt->ReadBool( "Option", "fFilesOneByOne", false);            //フォルダ内のファイルは個別に暗号化/復号する
	fNoParentFldr = pOpt->ReadBool( "Option", "fNoParentFldr", false);              //復号するときに親フォルダを生成しない
	fExtInAtcFileName = pOpt->ReadBool( "Option", "fExtInAtcFileName", false);      //暗号化ファイル名に拡張子を含める
	fAutoName = pOpt->ReadBool( "Option", "fAutoName", false);                      //自動で暗号化ファイル名を付加する
	AutoNameFormatText = pOpt->ReadString("Option", "AutoNameFormatText", "<filename>_<date:yyyy_mm_dd><ext>");//自動で付加するファイル名書式

	//----------------------------------------------------------------------
	// 削除設定
	//----------------------------------------------------------------------
	fDelOrgFile = pOpt->ReadBool( "Option", "fDelOrgFile", false);        //元ファイルを削除する
	fDelEncFile = pOpt->ReadBool( "Option", "fDelEncFile", false);        //暗号化ファイルを削除する
	fShowDeleteChkBox = pOpt->ReadBool( "Option", "fShowDeleteChkBox", false); //メインフォームにチェックボックスを表示する
	fCompleteDelete = pOpt->ReadInteger( "Option", "fCompleteDelFile", 1);//完全の方法(0:通常，1:完全削除，2:ごみ箱）
	DelRandNum = pOpt->ReadInteger( "Option", "DelRandNum", 0);           //乱数の書き込み回数
	DelZeroNum = pOpt->ReadInteger( "Option", "DelZeroNum", 1);           //ゼロ書き込み回数

	//----------------------------------------------------------------------
	// 動作設定
	//----------------------------------------------------------------------
	CompressRate = pOpt->ReadInteger( "Option", "CompressRate", 6); //圧縮率
	fCompareFile = pOpt->ReadInteger( "Option", "fCompareFile", 0); //暗号処理後にファイルコンペアを行うか

	//----------------------------------------------------------------------
	// システム
	//----------------------------------------------------------------------
	fAssociationFile = pOpt->ReadInteger( "Option", "fAssociationFile", 1);      //関連付け設定
	AtcsFileIconIndex = pOpt->ReadInteger( "Option", "AtcsFileIconIndex", 1);    //ファイルアイコン番号
	UserRegIconFilePath = pOpt->ReadString( "Option", "UserRegIconFilePath", "");//ユーザー指定のファイルアイコンパス

	//----------------------------------------------------------------------
	// 高度設定
	//----------------------------------------------------------------------
	fAllowPassFile = pOpt->ReadBool( "Option", "fAllowPassFile", false);               //パスワードファイルを許可する
	fCheckPassFile = pOpt->ReadBool( "Option", "fCheckPassFile", false);               //暗号時にパスワードファイルを自動チェックする
	PassFilePath = pOpt->ReadString( "Option", "PassFilePath", "");                    //暗号時のパスワードファイルパス
	fCheckPassFileDecrypt = pOpt->ReadBool( "Option", "fCheckPassFileDecrypt", false); //復号時にパスワードファイルを自動チェックする
	PassFilePathDecrypt = pOpt->ReadString( "Option", "PassFilePathDecrypt", "");      //復号時のパスワードファイルパス
	fNoErrMsgOnPassFile = pOpt->ReadBool( "Option", "fNoErrMsgOnPassFile", false);     //パスワードファイルがない場合エラーを出さない
	fAddCamoExt = pOpt->ReadBool( "Option", "fAddCamoExt", false);                     //暗号化ファイルの拡張子を偽装する
	CamoExt = pOpt->ReadString( "Option", "CamoExt", ".jpg");                          //その拡張子
	MissTypeLimitsNum = pOpt->ReadInteger( "Option", "MissTypeLimitsNum", 3);          //パスワードのタイプミス制限回数（ver.2.70～）
	fBroken = pOpt->ReadBool( "Option", "fBroken", false);                             //タイプミス回数を超えたときにファイルを破壊するか否か（ver.2.70～）

	//----------------------------------------------------------------------
	// その他（コマンドラインからのみ）
	//----------------------------------------------------------------------
	fOver4GBok = false;     //4GB超えを容認


}
__finally{

	delete pOpt;
	delete pAppInfoString;

}

return(true);

}
//===========================================================================
//動作設定をKeyValue形式のTStringListで取得する
//===========================================================================
void __fastcall TAttacheCaseOptionHandle::GetKeyValueData(TStringList *KeyValueList)
{

TCustomIniFile *pOpt;
TGetAppInfoString *pAppInfoString;

try{

	if ( FileExists(OptionPath) == true ){
		// INIファイルから読み込み
		pOpt = new TIniFile(OptionPath);
		OptType = 1;
	}
	else{
		// レジストリから読み込み
		pOpt = new TRegistryIniFile(ATTACHE_CASE_REGISTRY_PATH);
		OptType = 0;
	}

	KeyValueList->Clear();

	//-----------------------------------
	//アプリケーション情報
	//-----------------------------------
	AppPath = Application->ExeName;

	//バージョン番号
	pAppInfoString = new TGetAppInfoString();
	VersionNum = StrToIntDef(StringReplace(pAppInfoString->FileVersion, ".", "", TReplaceFlags()<<rfReplaceAll), -1);

	KeyValueList->Add(String().Format("%s=%s", ARRAYOFCONST(("AppPath", AppPath))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("VersionNum", VersionNum))));

	//-----------------------------------
	// フォームポジションなど
	//-----------------------------------
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("FormWidth", FormWidth))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("FormHeight", FormHeight))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("FormTop", FormTop))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("FormLeft", FormLeft))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("WinStyle", WinStyle))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("ActiveTabNum", ActiveTabNum))));

	//-----------------------------------
	// 基本設定
	//-----------------------------------
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fMyEncodePasswordKeep", (int)fMyEncodePasswordKeep))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fMyDecodePasswordKeep", (int)fMyDecodePasswordKeep))));
	KeyValueList->Add(String().Format("%s=%s", ARRAYOFCONST(("MyEncodePassword", MyEncodePassword))));
	KeyValueList->Add(String().Format("%s=%s", ARRAYOFCONST(("MyDecodePassword", MyDecodePassword))));

	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fMainWindowMinimize", (int)fMainWindowMinimize))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fTaskBarHide", (int)fTaskBarHide))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fTaskTrayIcon", (int)fTaskTrayIcon))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fMemPasswordExe", (int)fMemPasswordExe))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fOpenFolder", (int)fOpenFolder))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fOpenFile", (int)fOpenFile))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fEndToExit", (int)fEndToExit))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fWindowForeground", (int)fWindowForeground))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fNoHidePassword", (int)fNoHidePassword))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fSaveToExeout", (int)fSaveToExeout))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fShowExeoutChkBox", (int)fShowExeoutChkBox))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fAskEncDecode", (int)fAskEncDecode))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fNoMultipleInstance", (int)fNoMultipleInstance))));

	//-----------------------------------
	// 保存設定
	//-----------------------------------
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fSaveToSameFldr", (int)fSaveToSameFldr))));
	KeyValueList->Add(String().Format("%s=%s", ARRAYOFCONST(("SaveToSameFldrPath", SaveToSameFldrPath))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fDecodeToSameFldr", (int)fDecodeToSameFldr))));
	KeyValueList->Add(String().Format("%s=%s", ARRAYOFCONST(("DecodeToSameFldrPath", DecodeToSameFldrPath))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fConfirmOverwirte", (int)fConfirmOverwirte))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fKeepTimeStamp", (int)fKeepTimeStamp))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fSameTimeStamp", (int)fSameTimeStamp))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fAllFilePack", (int)fAllFilePack))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fFilesOneByOne", (int)fFilesOneByOne))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fNoParentFldr", (int)fNoParentFldr))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fExtInAtcFileName", (int)fExtInAtcFileName))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fAutoName", (int)fAutoName))));
	KeyValueList->Add(String().Format("%s=%s", ARRAYOFCONST(("AutoNameFormatText", AutoNameFormatText))));

	//-----------------------------------
	// 削除設定
	//-----------------------------------
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fDelOrgFile", (int)fDelOrgFile))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fDelEncFile", (int)fDelEncFile))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fShowDeleteChkBox", (int)fShowDeleteChkBox))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fCompleteDelete", (int)fCompleteDelete))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("DelRandNum", DelRandNum))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("DelZeroNum", DelZeroNum))));

	//-----------------------------------
	// 動作設定
	//-----------------------------------
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("CompressRate", CompressRate))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fCompareFile", (int)fCompareFile))));

	//-----------------------------------
	// システム
	//-----------------------------------
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fAssociationFile", (int)fAssociationFile))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("AtcsFileIconIndex", AtcsFileIconIndex))));
	KeyValueList->Add(String().Format("%s=%s", ARRAYOFCONST(("UserRegIconFilePath", UserRegIconFilePath))));

	//-----------------------------------
	// 高度設定
	//-----------------------------------
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fAllowPassFile", (int)fAllowPassFile))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fCheckPassFile", (int)fCheckPassFile))));
	KeyValueList->Add(String().Format("%s=%s", ARRAYOFCONST(("PassFilePath", PassFilePath))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fCheckPassFileDecrypt", (int)fCheckPassFileDecrypt))));
	KeyValueList->Add(String().Format("%s=%s", ARRAYOFCONST(("PassFilePathDecrypt", PassFilePathDecrypt))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fNoErrMsgOnPassFile", (int)fNoErrMsgOnPassFile))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fAddCamoExt", (int)fAddCamoExt))));
	KeyValueList->Add(String().Format("%s=%s", ARRAYOFCONST(("CamoExt", CamoExt))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("MissTypeLimitsNum", MissTypeLimitsNum))));
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fBroken", (int)fBroken))));

	//-----------------------------------
	// その他（コマンドラインからのみ）
	//-----------------------------------
	KeyValueList->Add(String().Format("%s=%d", ARRAYOFCONST(("fOver4GBok", (int)fOver4GBok))));

}
__finally{

	delete pAppInfoString;
	delete pOpt;

}


}
//===========================================================================
//動作設定を保存する
//===========================================================================
bool __fastcall TAttacheCaseOptionHandle::SaveOptionData(void)
{

TCustomIniFile *pOpt;
TGetAppInfoString *pAppInfoString;

try{

	if ( OptType == 0 ) {
		pOpt = new TRegistryIniFile(ATTACHE_CASE_REGISTRY_PATH);    //レジストリ
	}
	else if ( OptType == -1 ) {
		pOpt = new TRegistryIniFile(ATTACHE_CASE_BAK_REGISTRY_PATH);//レジストリバックアップ
	}
	else{
		if (FileExists(OptionPath) == true) {
			pOpt = new TIniFile(OptionPath);                          //INIファイル
		}
		else{
			return(true);	                                            //保存しない（コマンドラインオプションからなどは）
		}
	}

	//-----------------------------------
	//アプリケーション情報
	//-----------------------------------
	//実行ファイルパス
	pOpt->WriteString( "AppInfo", "AppPath", ExpandUNCFileName(Application->ExeName));

	//バージョン番号
	pAppInfoString = new TGetAppInfoString();
	VersionNum = StrToIntDef(StringReplace(pAppInfoString->FileVersion, ".", "", TReplaceFlags()<<rfReplaceAll), -1);
	pOpt->WriteInteger( "AppInfo", "AppVersion", VersionNum);

	//-----------------------------------
	// フォームポジションなど
	//（デストラクタで記録しているのでここではやらない）
	//-----------------------------------
	pOpt->WriteString( "WindowPos", "WindowWidth", IntToStr(FormWidth));
	pOpt->WriteString( "WindowPos", "WindowHeight", IntToStr(FormHeight));
	pOpt->WriteString( "WindowPos", "WindowTop", IntToStr(FormTop));
	pOpt->WriteString( "WindowPos", "WindowLeft", IntToStr(FormLeft));
	pOpt->WriteString( "WindowPos", "FormStyle", IntToStr(WinStyle));

	//-----------------------------------
	// 基本設定
	//-----------------------------------
	pOpt->WriteString( "MyKey", "fKeep", fMyEncodePasswordKeep==true ? "1" : "0");   //パスワードを記録するか
	pOpt->WriteString( "MyKey", "fKeep01", fMyDecodePasswordKeep==true ? "1" : "0");
	//記憶するパスワードを記録する
	SaveMyPasswordToRegistry(MyEncodePassword, 0);	//暗号化
	SaveMyPasswordToRegistry(MyDecodePassword, 1);	//復号

	pOpt->WriteString( "Option", "fMainWindowMinimize", fMainWindowMinimize==true ? "1" : "0");//常にウィンドウを最小化して処理する
	pOpt->WriteString( "Option", "fTaskBarHide", fTaskBarHide==true ? "1" : "0");              //タスクバーに表示しない
	pOpt->WriteString( "Option", "fTaskTrayIcon", fTaskTrayIcon==true ? "1" : "0");            //タスクトレイにアイコンを表示する
	pOpt->WriteString( "Option", "fMemPasswordExe", fMemPasswordExe==true ? "1" : "0");        //記憶パスワードで確認なく実行する
	pOpt->WriteString( "Option", "fOpenFolder", fOpenFolder==true ? "1" : "0");                //フォルダを開く
	pOpt->WriteString( "Option", "fOpenFile", fOpenFile==true ? "1" : "0");                    //復号したファイルを関連付けされたソフトで開く
	pOpt->WriteString( "Option", "fEndToExit", fEndToExit==true ? "1" : "0");                  //処理後、アタッシェケースを終了する
	pOpt->WriteString( "Option", "fWindowForeground", fWindowForeground==true ? "1" : "0");    //デスクトップで最前面にウィンドウを表示する
	pOpt->WriteString( "Option", "fNoHidePassword", fNoHidePassword==true ? "1" : "0");        //「*」で隠さずパスワードを確認しながら入力する
	pOpt->WriteString( "Option", "fSaveToExeout", fSaveToExeout==true ? "1" : "0");            //常に自己実行形式で出力する
	pOpt->WriteString( "Option", "fShowExeoutChkBox", fShowExeoutChkBox==true ? "1" : "0");    //メインフォームにチェックボックスを表示する
	pOpt->WriteString( "Option", "fAskEncDecode", fAskEncDecode==true ? "1" : "0");            //暗号/復号処理かを問い合わせる
	pOpt->WriteString( "Option", "fNoMultipleInstance", fNoMultipleInstance==true ? "1" : "0");//複数起動しない

	//-----------------------------------
	// 保存設定
	//-----------------------------------
	pOpt->WriteString( "Option", "fSaveToSameFldr", fSaveToSameFldr==true ? "1" : "0");     //常に同じ場所に保存するか
	pOpt->WriteString( "Option", "SaveToSameFldrPath", SaveToSameFldrPath);                 //その保存場所
	pOpt->WriteString( "Option", "fDecodeToSameFldr", fDecodeToSameFldr==true ? "1" : "0"); //常に同じ場所へファイルを復号化するか
	pOpt->WriteString( "Option", "DecodeToSameFldrPath", DecodeToSameFldrPath);             //その保存場所
	pOpt->WriteString( "Option", "ConfirmOverwite", fConfirmOverwirte==true ? "1" : "0");   //同名ファイルの上書きを確認するか
	pOpt->WriteString( "Option", "fKeepTimeStamp", fKeepTimeStamp==true ? "1" : "0");       //暗号化ファイルのタイムスタンプを元ファイルに合わせる
	pOpt->WriteString( "Option", "fSameTimeStamp", fSameTimeStamp==true ? "1" : "0");       //復号したファイルのタイムスタンプを生成日時にする
	pOpt->WriteString( "Option", "fAllFilePack", fAllFilePack==true ? "1" : "0");           //複数のファイルを暗号化する際は一つにまとめる
	pOpt->WriteString( "Option", "fFilesOneByOne", fFilesOneByOne==true ? "1" : "0");       //フォルダ内のファイルは個別に暗号化/復号する
	pOpt->WriteString( "Option", "fNoParentFldr", fNoParentFldr==true ? "1" : "0");         //復号するときに親フォルダを生成しない
	pOpt->WriteString( "Option", "fExtInAtcFileName", fExtInAtcFileName==true ? "1" : "0"); //暗号化ファイル名に拡張子を含める
	pOpt->WriteString( "Option", "fAutoName", fAutoName==true ? "1" : "0");                 //自動で暗号化ファイル名を付加する
	pOpt->WriteString( "Option", "AutoNameFormatText", AutoNameFormatText);                 //自動で付加するファイル名書式

	//-----------------------------------
	// 削除設定
	//-----------------------------------
	pOpt->WriteString( "Option", "fDelOrgFile", fDelOrgFile==true ? "1" : "0");             //元ファイルを削除する
	pOpt->WriteString( "Option", "fDelEncFile", fDelEncFile==true ? "1" : "0");             //暗号化ファイルを削除する
	pOpt->WriteString( "Option", "fShowDeleteChkBox", fShowDeleteChkBox==true ? "1" : "0"); //メインフォームにチェックボックスを表示する
	pOpt->WriteString( "Option", "fCompleteDelFile", fCompleteDelete==true ? "1" : "0");    //削除の方法（ 0: 通常削除、1: 完全削除、2: ごみ箱へ）
	pOpt->WriteString( "Option", "DelRandNum", IntToStr(DelRandNum));                       //乱数の書き込み回数
	pOpt->WriteString( "Option", "DelZeroNum", IntToStr(DelZeroNum));                       //ゼロ書き込み回数

	//-----------------------------------
	// 動作設定
	//-----------------------------------
	pOpt->WriteString( "Option", "CompressRate", IntToStr(CompressRate));                   //圧縮率
	pOpt->WriteString( "Option", "fCompareFile", fCompareFile==true ? "1" : "0");           //暗号処理後にファイルコンペアを行うか

	//-----------------------------------
	// システム
	//-----------------------------------
	pOpt->WriteString( "Option", "fAssociationFile", fAssociationFile==true ? "1" : "0");   //関連付け設定
	pOpt->WriteString( "Option", "AtcsFileIconIndex", IntToStr(AtcsFileIconIndex));         //ファイルアイコン番号
	pOpt->WriteString( "Option", "UserRegIconFilePath", UserRegIconFilePath);               //ユーザー指定のファイルアイコンパス

	//-----------------------------------
	// 高度設定
	//-----------------------------------
	pOpt->WriteString( "Option", "fAllowPassFile", fAllowPassFile==true ? "1" : "0");               //パスワードファイルを許可する
	pOpt->WriteString( "Option", "fCheckPassFile", fCheckPassFile==true ? "1" : "0");               //暗号時にパスワードファイルを自動チェックする
	pOpt->WriteString( "Option", "PassFilePath", PassFilePath);                                     //暗号時のパスワードファイルパス
	pOpt->WriteString( "Option", "fCheckPassFileDecrypt", fCheckPassFileDecrypt==true ? "1" : "0"); //復号時にパスワードファイルを自動チェックする
	pOpt->WriteString( "Option", "PassFilePathDecrypt", PassFilePathDecrypt);                       //復号時のパスワードファイルパス
	pOpt->WriteString( "Option", "fNoErrMsgOnPassFile", fNoErrMsgOnPassFile==true ? "1" : "0");     //パスワードファイルがない場合エラーを出さない
	pOpt->WriteString( "Option", "fAddCamoExt", fAddCamoExt==true ? "1" : "0");                     //暗号化ファイルの拡張子を偽装する
	pOpt->WriteString( "Option", "CamoExt", CamoExt);                                               //その拡張子
	pOpt->WriteString( "Option", "MissTypeLimitsNum", IntToStr(MissTypeLimitsNum));                 //パスワードのタイプミス制限回数（ver.2.70～）
	pOpt->WriteString( "Option", "fBroken", fBroken==true ? "1" : "0");                             //タイプミス回数を超えたときにファイルを破壊するか否か（ver.2.70～）

	delete pAppInfoString;
	delete pOpt;

}
catch(...){
	delete pAppInfoString;
	delete pOpt;

}

return(true);

}
//===========================================================================
//コマンドラインオプションからの設定読み込み
//===========================================================================
bool __fastcall TAttacheCaseOptionHandle::
	LoadOptionDataFromParamString(TStringList *FileList)
{

int i, c, L;
int intvalue = 0;
String strvalue = "";

String CmdStr;
String FilePath, FileName;
fArg = false;       //引数はない
OptType = 0;        //ファイル以外のパラメータがあった場合は "OptType=2"とする
FileList->Clear();  //ファイルリストクリア

//-----------------------------------

//引数を吸い出す
for ( i = 1; i < ParamCount()+1 ; i++){

	//引数
	if ( ParamStr(i).SubString(1,1) == "/" ){

		fArg = true;//実行引数処理（グローバル）

		CmdStr = ParamStr(i).LowerCase();
		CmdStr = CmdStr.SubString(2, ParamStr(i).Pos("=")-2);

		// "="後ろを抜き出す
		L = ParamStr(i).Pos("=");
		if ( L > 0 ){
			strvalue = ParamStr(i).SubString(L+1,ParamStr(i).Length()-L);
		}
		else{
			strvalue = "";
		}

		//基本設定-----------------------------------
		if (CmdStr == "p"){
			//パスワード
			MyPassword = strvalue;
			fArgPassword = true;  //実行引数にパスワード
		}
		else if (CmdStr == "mempexe"){
			//記憶パスワードで確認なく実行する
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fMemPasswordExe = true;
			}
			else{
				fMemPasswordExe = false;
			}
		}
		else if (CmdStr == "wmin"){
			//常にウィンドウを最小化して処理する
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fMainWindowMinimize = true;
			}
			else{
				fMainWindowMinimize = false;
			}
		}
		else if (CmdStr == "tskb"){
			//タスクバーに表示しない
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fTaskBarHide = true;
			}
			else{
				fTaskBarHide = false;
			}
		}
		else if (CmdStr == "tsktr"){
			//タスクトレイにアイコンを表示する
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fTaskTrayIcon = true;
			}
			else{
				fTaskTrayIcon = false;
			}
		}
		else if (CmdStr == "opd"){
			//フォルダの場合に復号後に開くか
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fOpenFolder = true;
			}
			else{
				fOpenFolder = false;
			}
		}
		else if (CmdStr == "opf"){
			//復号したファイルを関連付けされたソフトで開く
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fOpenFile = true;
			}
			else{
				fOpenFile = false;
			}
		}
		else if (CmdStr == "exit"){
			//処理後に終了するか
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fEndToExit = true;
			}
			else{
				fEndToExit = false;
			}
		}
		else if (CmdStr == "front"){
			//デスクトップで最前面にウィンドウを表示する
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fWindowForeground = true;
			}
			else{
				fWindowForeground = false;
			}
		}
		else if (CmdStr == "nohide"){
			//「*」で隠さずパスワードを確認しながら入力する
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fNoHidePassword = true;//ここは逆
			}
			else{
				fNoHidePassword = false;
			}
		}
		else if (CmdStr == "exeout"){
			//常に自己実行形式で出力する
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fSaveToExeout = true;
			}
			else{
				fSaveToExeout = false;
			}
		}
		else if (CmdStr == "chkexeout"){
			//メインフォームにチェックボックスを表示する
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fShowExeoutChkBox = true;
			}
			else{
				fShowExeoutChkBox = false;
			}
		}
		else if (CmdStr == "askende"){
			//暗号/復号処理かを問い合わせる
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fAskEncDecode = true;
			}
			else{
				fAskEncDecode = false;
			}
		}
		else if (CmdStr == "en"){
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				ProcTypeWithoutAsk = 0; //明示的な暗号処理
			}
		}
		else if (CmdStr == "de" ){
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				ProcTypeWithoutAsk = 1; //明示的な復号処理
			}
		}
		else if (CmdStr == "nomulti" ){
			//複数起動しない
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fNoMultipleInstance = true;
			}
			else{
				fNoMultipleInstance = false;
			}
		}
		//保存設定-----------------------------------
		else if (CmdStr == "saveto" ){
			//常に同じ場所へ暗号化ファイルを保存する
			fSaveToSameFldr = true;
			SaveToSameFldrPath = GetLongFilePath(ExpandUNCFileName(strvalue));
		}
		else if (CmdStr == "dsaveto"){
			//常に同じ場所へファイルを復号化する
			fDecodeToSameFldr = true;
			DecodeToSameFldrPath = GetLongFilePath(ExpandUNCFileName(strvalue));
		}
		else if (CmdStr == "ow"){
			//同名ファイルの上書きを確認するか（確認無で上書きするか？）
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fConfirmOverwirte = false;//ここは逆
			}
			else{
				fConfirmOverwirte = true;
			}
		}
		else if (CmdStr == "orgdt"){
			//暗号化ファイルのタイムスタンプを元ファイルに合わせる
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fKeepTimeStamp = true;
			}
			else{
				fKeepTimeStamp = false;
			}
		}
		else if (CmdStr == "now"){
			//復号したファイルのタイムスタンプを生成日時にする
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fSameTimeStamp = true;
			}
			else{
				fSameTimeStamp = false;
			}
		}
		else if (CmdStr == "allpack"){
			//複数のファイルを暗号化する際は一つにまとめる
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fAllFilePack = true;
			}
			else{
				fAllFilePack = false;
			}
		}
		else if (CmdStr == "oneby"){
			//フォルダ内のファイルは個別に暗号化/復号する
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fFilesOneByOne = true;
			}
			else{
				fFilesOneByOne = false;
			}
		}
		else if (CmdStr == "nopfldr"){
			//復号するときに親フォルダを生成しない
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fNoParentFldr = true;
			}
			else{
				fNoParentFldr = false;
			}
		}
		else if (CmdStr == "withext"){
			//暗号化ファイル名に拡張子を含める
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fExtInAtcFileName = true;
			}
			else{
				fExtInAtcFileName = false;
			}
		}
		else if (CmdStr == "autoname"){
			//自動で暗号化ファイル名を付加する
			FileName = ExtractFileName(InterpretFormatTextToFilePath("sample.atc", strvalue)); //試しに解釈してみる
			if ( strvalue  != "" && ChkFileNameChr(FileName) == false){  //Windowsの禁止文字？
				fAutoName = true;
				AutoNameFormatText = strvalue;
			}
			else{
				fAutoName = false;
			}
		}
		//削除設定-----------------------------------
		else if (CmdStr == "del"){
			//元ファイルの完全削除を行うか
			//(0:削除しない, 1:通常，2:完全削除 3:ごみ箱）
			intvalue = StrToIntDef(strvalue.Trim(),-1);
			if ( 0 < intvalue && intvalue < 4 ){
				fDelOrgFile = true;
				fCompleteDelete = intvalue-1;
			}
			else{
				fDelOrgFile = false;
			}
		}
		else if (CmdStr == "delenc"){
			//暗号化ファイルの完全削除を行うか
			//(0:削除しない, 1:通常，2:ごみ箱，3:完全削除）
			intvalue = StrToIntDef(strvalue.Trim(),-1);
			if ( 0 < intvalue && intvalue < 4 ){
				fDelEncFile = true;
				fCompleteDelete = intvalue-1;
			}
			else{
				fDelEncFile = false;
			}
		}
		else if (CmdStr == "chkdel"){
			//メインフォームにチェックボックスを表示する
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fShowDeleteChkBox = true;
			}
			else{
				fShowDeleteChkBox = false;
			}
		}
		else if (CmdStr == "delrand"){
			//乱数を何回書き込み消去するか
			intvalue = StrToIntDef(strvalue.Trim(),-1);
			if ( intvalue > 0 ){ //マイナス値は無視
				if ( intvalue > 99 ) intvalue = 99;
				DelRandNum = intvalue;
			}
		}
		else if (CmdStr == "delnull"){
			//NULLを何回書き込み消去するか
			intvalue = StrToIntDef(strvalue.Trim(),-1);
			if ( intvalue > 0 ){ //マイナス値は無視
				if ( intvalue > 99 ) intvalue = 99;
				DelZeroNum = intvalue;
				i++;
			}
		}
		//動作設定-----------------------------------
		else if (CmdStr == "comprate"){
			//圧縮率
			intvalue = StrToIntDef(strvalue.Trim(),-1);
			if ( -1 < intvalue && intvalue < 10 ){
				CompressRate = intvalue;
			}
			else{
				CompressRate = 0; //無圧縮
			}
		}
		else if (CmdStr == "cmp"){
			//暗号化した後にデータのコンペアを行う
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fCompareFile = true;
			}
			else{
				fCompareFile = false;
			}
		}
		//高度設定-----------------------------------
		else if (CmdStr == "pf"){
			//パスワードにファイルの指定を許可する
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fAllowPassFile = true;
			}
			else{
				fAllowPassFile = false;
			}
		}
		else if (CmdStr == "pfile"){
			//パスワードファイルパス
			if ( strvalue != "" ){
				fAllowPassFile = true;
				fCheckPassFile = true;
				PassFilePath = GetLongFilePath(ExpandUNCFileName(strvalue)); //ロングパスに変換
			}
			else{
				fCheckPassFile = false;
				if ( fCheckPassFileDecrypt == false )
					fAllowPassFile = false;
			}
		}
		else if (CmdStr == "dpfile"){
			//復号時のパスワードファイルパス
			if ( strvalue != "" ){
				fAllowPassFile = true;
				fCheckPassFileDecrypt = true;
				PassFilePathDecrypt = GetLongFilePath(ExpandUNCFileName(strvalue)); //ロングパスに変換
			}
			else{
				fCheckPassFileDecrypt = false;
				if ( fCheckPassFile == false )
					fAllowPassFile = false;
			}
		}
		else if (CmdStr == "nomsgp"){
			//パスワードファイルがない場合エラーを出さない
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fNoErrMsgOnPassFile = true;
			}
			else{
				fNoErrMsgOnPassFile = false;
			}
		}
		else if (CmdStr == "camoext"){
			//暗号化ファイルの拡張子を偽装する
			if ( ChkFileNameChr(strvalue) == true ){ //禁止文字を使っている
				fAddCamoExt = false;
				CamoExt = "";
			}
			else if ( strvalue.LowerCase() == "null" ){ //OFF
				fAddCamoExt = false;
			}
			else{
				fAddCamoExt = true;
				CamoExt = strvalue;
			}
		}
		//-----------------------------------
		//その他（コマンドラインからのみ）
		//-----------------------------------

		//4GB超えを容認
		else if (CmdStr == "4gbok"){
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fOver4GBok = true;
			}
			else{
				fOver4GBok = false;
			}
		}
		//ウィンドウの状態
		//(0: Normal, 1: Minimize，2: Maximize）
		else if (CmdStr == "ws"){
			intvalue = StrToIntDef(strvalue.Trim(),-1);
			if (intvalue == 1 || intvalue == 2) {
				WinStyle = intvalue;
			}
			else{
				WinStyle = 0;
			}
		}
		//エラーメッセージ表示の抑制
		else if (CmdStr == "noerr"){
			if ( StrToIntDef(strvalue.Trim(), -1) == 1 ){
				fNoErrorMsg = true;
			}
			else{
				fNoErrorMsg = false;
			}
		}

		//-----------------------------------
		//TODO: 指定のファイルリストを読み込む
		//-----------------------------------
		else if (CmdStr == "list"){
			if ( strvalue != "" ){
				String ListFilePath = GetLongFilePath(ExpandUNCFileName(strvalue)); //ロングパスに変換
				if ( FileExists(ListFilePath) == true ){
					TStringList *ListFileList = new TStringList;
					ListFileList->LoadFromFile(ListFilePath);
					//１行１ファイルパスとして取り出してファイルリストへ格納
					for ( c = 0; c < ListFileList->Count; c++ ){
						if ( ListFileList->Strings[c] != "" ){
							FileList->Add(GetLongFilePath(ExpandUNCFileName(ListFileList->Strings[c])));
						}
					}
					delete ListFileList;
				}
			}
		}

		OptType = 2;	//コマンドライン引数

	}
	//ファイルパス
	else{
		String temp = ExpandUNCFileName(ParamStr(i));
		FileList->Add(GetLongFilePath(ExpandUNCFileName(ParamStr(i))));
	}

}//loop;

//w_char filepath[];
//size = GetFileTitle("新しい~1.txt",NULL,0);

//-----------------------------------
//デバッグ
//ShowMessage(FileList->Text);
//-----------------------------------

return(true);

}
//===========================================================================
// ロングファイル名を取得する
//===========================================================================
String __fastcall TAttacheCaseOptionHandle::GetLongFilePath(String FilePath)
{

String FileName;
TSearchRec sr;

while(FindFirst(FilePath, faAnyFile, sr) == 0){
	if (sr.Name != "." && sr.Name != "..") {
		FileName = sr.FindData.cFileName;
		FilePath = IncludeTrailingPathDelimiter(ExtractFileDir(FilePath)) + FileName;
		return(FilePath);
	}
}

//エラー
return("");

}
//===========================================================================
//記憶パスワードをレジストリまたはINIファイルから読み出す
//===========================================================================
AnsiString __fastcall TAttacheCaseOptionHandle::ReadMyPasswordFromRegistry(int Type)
{

int i;

//取得したパスワードが正しいかチェックするための変数
AnsiString TokenString;
AnsiString ResultString;
AnsiString Password;

char buffer[BUF_PASSWORD_SIZE];
char newbuffer[BUF_PASSWORD_SIZE];

TMemoryStream *ms;
CBlowFish *bf;
TCustomIniFile *pOpt;

try{

	ms = new TMemoryStream();
	bf = new CBlowFish;

	if ( OptType == 0 ) {
		pOpt = new TRegistryIniFile(ATTACHE_CASE_REGISTRY_PATH);    //レジストリ
	}
	else if ( OptType == -1 ) {
		pOpt = new TRegistryIniFile(ATTACHE_CASE_BAK_REGISTRY_PATH);//レジストリバックアップ
	}
	else{
		if (FileExists(OptionPath) == true) {
			pOpt = new TIniFile(OptionPath);                          //INIファイル
		}
		else{
			return("");	                                              //保存しない（コマンドラインオプションからなどは）
		}
	}

	//-----------------------------------
	//固定パスワードの生成
	AnsiString MyKeyPass;

	//コンピュータ名を取得
	char UserName[255];
	DWORD  BufferLen = sizeof(UserName)-1;
	GetComputerName(UserName, &BufferLen);

	//「ドライブのボリュームラベルID + コンピュータ名」をパスワードに設定
	MyKeyPass = (AnsiString)GetVolumeLabelIDNumber() + "_" + (AnsiString)UserName;
	// ex).  818980454_HIBARA

	//バッファの初期化
	for ( i = 0; i < BUF_PASSWORD_SIZE; i++ ){
		buffer[i] = NULL;
		newbuffer[i] = NULL;
	}

	//暗号化パスワード
	if ( Type == TYPE_ENCODE_FILE ){
		pOpt->ReadBinaryStream("MyKey", "Passcode", ms);
	}
	//復号パスワード
	else if ( Type == TYPE_DECODE_FILE ){
		pOpt->ReadBinaryStream( "MyKey", "Passcode01", ms);
	}

	ms->Position = 0;
	ms->Read(buffer, BUF_PASSWORD_SIZE);


	//-----------------------------------
	//
	//復号
	//
	//※実はver.1の仕様を引きずっていてBlowfishで暗号化されています。
	//

	bf->Initialize( MyKeyPass.c_str(), MyKeyPass.Length() );   //初期化
	bf->Decode( buffer, newbuffer, BUF_PASSWORD_SIZE);         //復号

	//取得したパスワードが正しいかチェック
	ResultString = (AnsiString)newbuffer;

	//トークンを抜き出す
	if ( ResultString.Length() > 4 ){  //パスコードが巧く抜き出せなかったときのエラー防止

		TokenString = ResultString.SubString( ResultString.Length()-3, 4 );

		if ( TokenString == "_atc" ){
			Password = ResultString.SubString( 1, ResultString.Length()-4);
		}
		else{
			//読み出せないときはキーを削除しておく
			SaveMyPasswordToRegistry("", Type);
		}

	}
	else{

		//読み出せないときはキーを削除しておく
		SaveMyPasswordToRegistry("", Type);

	}

}
__finally	 {
	delete bf;
	delete ms;
	delete pOpt;

}

//===================================
//デバッグ
//ShowMessage(Password);
//===================================

//保存のパスワードを返す
return(Password);


}//end GetMyKeyCode;
//===========================================================================
//記憶パスワードをレジストリまたはINIファイルに保存する
//===========================================================================
bool __fastcall TAttacheCaseOptionHandle::
	SaveMyPasswordToRegistry(String Password, int Type)
{

// Type:
// #define TYPE_ENCODE_FILE 0 //暗号化
// #define TYPE_DECODE_FILE 1 //復号化

//ヘッダに 256 + 8（パス＋トークン）分を定義
//#define BUF_PASSWORD_SIZE 264

int i;
char buffer[BUF_PASSWORD_SIZE];
char newbuffer[BUF_PASSWORD_SIZE];

for (i = 0; i < BUF_PASSWORD_SIZE; i++) {
	buffer[i] = 0;
	newbuffer[i] = 0;
}

TCustomIniFile *pOpt;
CBlowFish *bf = new CBlowFish;
TMemoryStream *ms = new TMemoryStream();

try{

	if ( OptType == 0 ) {
		pOpt = new TRegistryIniFile(ATTACHE_CASE_REGISTRY_PATH);    //レジストリ
	}
	else if ( OptType == -1 ) {
		pOpt = new TRegistryIniFile(ATTACHE_CASE_BAK_REGISTRY_PATH);//レジストリバックアップ
	}
	else{
		if (FileExists(OptionPath) == true) {
			pOpt = new TIniFile(OptionPath);                          //INIファイル
		}
		else{
			return(true);	                                            //保存しない（コマンドラインオプションからなどは）
		}
	}

	//パスコードをクリアする
	if ( Password == "" ){

		if ( Type == TYPE_ENCODE_FILE){
			pOpt->DeleteKey( "MyKey", "Passcode");
		}
		else if ( Type == TYPE_DECODE_FILE){
			pOpt->DeleteKey( "MyKey", "Passcode01");
		}
		else{
			return(false);
		}

		return(true);

	}

	//長すぎるときはBlowfishパスワード文字列MAXに切る
	if ( Password.Length() > 32 ){
		Password.SetLength(32);
	}

	//-----------------------------------
	//パスワードトークンと連結
	String PassToken = "_atc";
	Password = Password + PassToken;

	//-----------------------------------
	//固定パスワードの生成
	AnsiString MyKeyPass;

	//保存する先のコンピュータ名を取得
	char UserName[255];
	DWORD  BufferLen = sizeof(UserName)-1;
	GetComputerName(UserName, &BufferLen);

	//「ドライブのボリュームラベルID + コンピュータ名」をパスワードに設定
	MyKeyPass = (AnsiString)GetVolumeLabelIDNumber() + "_" + (AnsiString)UserName;
	// ex).  818980454_HIBARA

	//-----------------------------------
	//
	//暗号化
	//
	//※実はver.1の仕様を引きずっていてBlowfishで暗号化されています。
	//

	//バッファの初期化
	for ( i = 0; i < BUF_PASSWORD_SIZE; i++ ){
		buffer[i] = NULL;
		newbuffer[i] = NULL;
	}

	bf->Initialize( MyKeyPass.c_str(), MyKeyPass.Length() );   //初期化
	StrCopy( buffer, AnsiString(Password).c_str());  	 //パスワードをバッファに
	bf->Encode( buffer, newbuffer, BUF_PASSWORD_SIZE); //暗号化

	if ( Type == TYPE_ENCODE_FILE){
		ms->Write(newbuffer, BUF_PASSWORD_SIZE);
		ms->Position = 0;
		pOpt->WriteBinaryStream( "MyKey", "Passcode", ms);
	}
	else if ( Type == TYPE_DECODE_FILE){
		ms->Write(newbuffer, BUF_PASSWORD_SIZE);
		ms->Position = 0;
		pOpt->WriteBinaryStream( "MyKey", "Passcode01", ms);
	}
	else{
		return(false);
	}

}
__finally{

	delete bf;
	delete ms;
	delete pOpt;

}

return(true);

}
//===========================================================================
// ハードディスクのボリュームラベルIDを取得する
//===========================================================================
String __fastcall TAttacheCaseOptionHandle::GetVolumeLabelIDNumber(void)
{

//アプリのあるドライブ名
String DriveName = ExtractFileDrive(Application->ExeName);
DriveName = IncludeTrailingPathDelimiter(DriveName);

const wchar_t *pszDrv = DriveName.c_str();
wchar_t buf[MAX_PATH * 6];
wchar_t szVolName[MAX_PATH];
wchar_t szFileSystem[16];
DWORD dwVolId, dwMaxCmpLen, dwFSFlags;

GetVolumeInformationW(
		pszDrv,        // ドライブ
		szVolName,     // ボリュームラベル
		MAX_PATH,      // ボリュームラベルのバッファ長(適当)
		&dwVolId,      // ボリュームID
		&dwMaxCmpLen,  // 最大コンポーネント長
		&dwFSFlags,    // ファイルシステム属性
		szFileSystem,  // ファイルシステム名
		16);           // ファイルシステム名のバッファ長(適当)

return(String(dwVolId));

}//end GetVolumeLabelIDNumber;
//======================================================================
// Windowsが許さないファイル名文字列の検査関数
//======================================================================
bool __fastcall TAttacheCaseOptionHandle::ChkFileNameChr(String ChkText) {

	// ChkText :チェックするテキスト
	// 返り値  :禁止文字が見つかった場合は、Trueを返す

	int i, L;
	String ChkOneWord;                   // チェックする禁止文字
	String DontUseText = "\\/:,*?\"<>|"; // 禁止文字列リスト

	//禁止する文字列数を取得
	L = DontUseText.Length();

	for (i = 1; i < L + 1; i++) {
		// チェックする文字を禁止文字列から抜き出す
		ChkOneWord = DontUseText[i];
		if (ChkText.Pos(ChkOneWord) > 0) {
			// 禁止文字が見つかった
			return(true);
		}
	}

	return(false);

}// end ChkFileNameChr;
//===========================================================================
//カスタマイズされた文字列を解釈してファイル名を置き換える
//===========================================================================
String __fastcall TAttacheCaseOptionHandle::
	InterpretFormatText(String FileName, String InputText, int Num)
{

int i;
bool fScript = false;

int RandNum;      //ランダム文字数
int KetaNum;     	//連番桁数

String DateText;

int HeadNameNum, EndNameNum;

String strNumber = "";

String OneChar;
String FormatText;
String ReturnText;

String BaseFileName = ChangeFileExt(FileName, "");  //拡張子抜きのファイル名
String FileExt = ExtractFileExt(FileName);          //拡張子

ReturnText = "";

for ( i = 1; i < InputText.Length()+1; i++ ){

	OneChar = InputText.SubString(i,1);

	if ( OneChar == "<" ){
		fScript = true;
		FormatText = FormatText + OneChar;
	}
	else if ( OneChar == ">" ){

		FormatText = FormatText + OneChar;

		//-----------------------------------
		//ファイル名
		//-----------------------------------
		if ( FormatText.LowerCase() == "<filename>"){
			ReturnText = ReturnText + BaseFileName;
		}
		//-----------------------------------
		//拡張子
		//-----------------------------------
		else if ( FormatText.LowerCase() == "<ext>"){
			ReturnText = ReturnText + FileExt;
		}
		//-----------------------------------
		//ファイル名の先頭
		//-----------------------------------
		else if ( FormatText.SubString(1,7).LowerCase() == "<fhead:" ){
			//先頭ファイル名何文字か
			HeadNameNum = StrToIntDef(FormatText.SubString(8,FormatText.Length()-8), -1);
			if ( HeadNameNum > -1 ){
				ReturnText = ReturnText + BaseFileName.SubString(0, HeadNameNum);
			}
		}
		//-----------------------------------
		//ファイル名の末尾
		//-----------------------------------
		else if ( FormatText.SubString(1,6).LowerCase() == "<fend:" ){
			//末尾ファイル名何文字か
			EndNameNum = StrToIntDef(FormatText.SubString(7,FormatText.Length()-7), -1);
			if ( EndNameNum > -1 ){
				ReturnText = ReturnText +
				BaseFileName.SubString(BaseFileName.Length()-EndNameNum+1, BaseFileName.Length());
			}
		}
		//-----------------------------------
		//ランダム文字列
		//-----------------------------------
		else if ( FormatText.SubString(1,8).LowerCase() == "<random:" ){
			//ランダム文字数
			RandNum = StrToIntDef(FormatText.SubString(9,FormatText.Length()-9), -1);
			if ( RandNum > -1 ){
				//ランダム文字生成
				ReturnText = ReturnText + MakeRandomFileName(RandNum, true, true);
			}
		}
		//-----------------------------------
		//ナンバリング
		//-----------------------------------
		else if ( FormatText.SubString(1,8).LowerCase() == "<number:" ){
			//連番桁数
			KetaNum = StrToIntDef(FormatText.SubString(9,FormatText.Length()-9), -1);
			if ( KetaNum > -1 ){
				strNumber = IntToStr(Num);
				//桁数合わせ
				while( strNumber.Length() < KetaNum ){
					strNumber = "0" + strNumber;
				}
				ReturnText = ReturnText + strNumber;
			}
		}
		//-----------------------------------
		//日付として解釈
		//-----------------------------------
		else if ( FormatText.SubString(1,6).LowerCase() == "<date:" ){
			//日付の書式指定だけ抜き出す（※末尾の>は除く）
			DateText = FormatText.SubString(7,FormatText.Length()-7);
			if ( DateText != "" ){
				ReturnText = ReturnText + Now().FormatString(DateText);
			}
		}//end if;

		//-----------------------------------

		FormatText = "";	//クリア

		fScript = false;

	}
	//-----------------------------------
	//通常文字列
	//-----------------------------------
	else{
		if ( fScript == true ){
			FormatText = FormatText + OneChar;
		}
		else{
			ReturnText = ReturnText + OneChar;
		}

	}

}//end for ( i = 1; i < InputText.Length()+1; i++ );

//括弧が開いたまま
if ( fScript == true ){
	ReturnText = ReturnText + FormatText;
}

return(ReturnText);


}//end InterpretFormatText;
//===========================================================================
//実際にカスタマイズ文字列を解釈して重複のない(連番の)ファイルパスを返す
//===========================================================================
String __fastcall TAttacheCaseOptionHandle::
	InterpretFormatTextToFilePath(String FilePath, String InputText)
{

int c = 0;

String ReturnText = "";
String DirPath = ExtractFileDir(FilePath);          //フォルダ名
String FileName = ExtractFileName(FilePath);        //ファイル名

do{
	FileName = InterpretFormatText(FileName, InputText, c); //解釈
	ReturnText = IncludeTrailingPathDelimiter(DirPath) + FileName;
	c++;
}while(FileExists(ReturnText));

return(ReturnText);

}
//===========================================================================
//指定のランダム文字列を生成
//===========================================================================
String __fastcall TAttacheCaseOptionHandle::
	MakeRandomFileName(int MojiNum, bool fNum, bool fKigo)
{

int i;
int RandNum;
String OneChar;
String ReturnText;

String FileNameCharList;
String CharAlphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
String CharNumber = "0123456789";
String CharKigo = "=-+!_#$%&()[]{}~^`'@";

//アルファベットはデフォルト
FileNameCharList = CharAlphabet;

//数字も含める
if ( fNum == true )
	FileNameCharList = FileNameCharList + CharNumber;

//記号も含める
if ( fKigo == true )
	FileNameCharList = FileNameCharList + CharKigo;

//指定の文字数分ランダム文字列を生成
for ( i = 0; i < MojiNum; i++ ){

	RandNum = RandomRange( 1 ,FileNameCharList.Length());
	OneChar = FileNameCharList.SubString( RandNum, 1);
	ReturnText = ReturnText + OneChar;

}//loop;

return(ReturnText);

}//end MakeRandomFileName;
//===========================================================================
// ファイルからSHA-1ハッシュ値を取得する
//===========================================================================
bool __fastcall TAttacheCaseOptionHandle::GetSHA1HashFromFile(
	String FilePath,              // パスワードファイルパス
	AnsiString &HashDataVer2,     // ver.2.*  ～：SHA-1ハッシュ値（20byte + 12byte）
	AnsiString &HashDataVer1 )    // ver.1.*  ～：ヘッダデータ（先頭文字列32文字）
{

/*
 * パスワードファイルからSHA-1を計算して、パスワードの値にセットしていますが、
 * SHA-1の計算結果は20バイト（160ビット）で出力される仕様のため、Rijndaelの
 * パスワードサイズ（32バイト）と合いません。
 * そのため、ここでは強引にファイルから任意の12バイトを追加して32バイトとしています。
 *
 * 今年(2012年)中には次世代の「SHA-3」が選定されますので、それに合わせて、
 * 32バイト値をセットする仕様にバージョンアップしようと考えています。
 *
 * なお、SHA-3の策定状況につきましては、下記を参照ください。
 *
 * http://csrc.nist.gov/groups/ST/hash/sha-3/index.html
 *
*/

int i;

int fh;
int bytes, rest;
char buffer[255];	                               //読み込みバッファ
char sha1_hash_data_mixed_padding[BUF_HASH_DATA];// 32byte

for (i = 0; i < BUF_HASH_DATA; i++) {
	sha1_hash_data_mixed_padding[i] = NULL;
}

if ( !FileExists(FilePath) ) return(false);

try{

	if ( (fh = FileOpen(FilePath, fmShareDenyNone)) == -1 ){
		//パスワードファイルが開けない？
		return(false);
	}

	//ヘッダ先頭の255byteを読む（ver.1.* ～）
	FileRead(fh, buffer, 255);
	HashDataVer1 = (AnsiString)buffer;

	//SHA-1ハッシュの計算
	FileSeek(fh, 0, 0);
	SHA1Context sha;
	unsigned char Message_Digest[20];
	ZeroMemory(Message_Digest, 20);

	//初期化（リセット）
	if ( SHA1Reset(&sha)){
		return(false);
	}

	//ファイルを読み出してSHA-1へ入力していく
	while ((bytes = FileRead (fh, buffer, 255)) != 0){
		rest = bytes;
		if ( SHA1Input(&sha, (const unsigned char *)buffer, bytes) ){
			return(false);
		}
	}

	if ( FileSeek(fh, -rest, 2) > 0 ){
		FileRead(fh, buffer, 255);
	}
	else{
		FileSeek(fh, 0, 0);
		FileRead(fh, buffer, 255);
	}

	//出力
	if(SHA1Result(&sha, Message_Digest)){
		return(false);
	}
	for (i = 0; i < BUF_SHA1_SIZE; i++){   // 20byte
		sha1_hash_data_mixed_padding[i] = Message_Digest[i];
	}

	//残りの12byteを補填
	for (i = 0; i < 12; i++) {             // 12byte
		 sha1_hash_data_mixed_padding[BUF_SHA1_SIZE+i] = buffer[i];
	}

	HashDataVer2 = "";
	for (i = 0; i < BUF_HASH_DATA; i++){   // 32byte
		//適切にキャストされるように1byteずつ代入
		HashDataVer2 += (AnsiString)sha1_hash_data_mixed_padding[i];
	}
	HashDataVer2.SetLength(32);

}
__finally{

	FileClose(fh);

}
return(true);


}
//===========================================================================
//「送る」フォルダーまでのアプリケーションフルパスを生成する
//===========================================================================
String __fastcall TAttacheCaseOptionHandle::CreateSendToFolderAppFullPath(void)
{

//'アタッシェケース'
String ApplicationName = LoadResourceString(&Msgoption::_APPLICATION_NAME);
String SendToFolderPath;
String ShortCutFilePath;
wchar_t path[MAX_PATH];

LPITEMIDLIST PidList;

SHGetSpecialFolderLocation(Application->Handle, CSIDL_SENDTO, &PidList );
SHGetPathFromIDListW( PidList, path );

SendToFolderPath = path;
ShortCutFilePath = IncludeTrailingPathDelimiter(SendToFolderPath) + ApplicationName + ".lnk";

return(ShortCutFilePath);


}
//===========================================================================
// ショートカットファイルを作成する
//===========================================================================
bool __fastcall TAttacheCaseOptionHandle::CreateShortCutFile
( String LinkFilePath, String TargetFilePath, String Arguments, String WorkDir, int ShowCmd )
{

// LinkFile   : ショートカットファイルの名前(*.lnk)
// TargetFile : リンク先
// Arguments  : コマンドライン引数
// WorkDir    : 作業ディレクトリ
// ShowCmd    : 実行時の大きさ

IShellLinkW *psl;

CoInitialize( NULL );

if ( CoCreateInstance
		( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW, (void **)&psl ) == S_OK){

	psl->SetPath(TargetFilePath.c_str());          // リンク先を設定

	if ( Arguments != "" ){
		psl->SetArguments( Arguments.c_str() );      // コマンドライン引数を設定
	}
	if ( WorkDir != "" ){                          // 作業フォルダを設定
		psl->SetWorkingDirectory( WorkDir.c_str() );
	}
	psl->SetShowCmd( ShowCmd );                    // 実行時の大きさを設定

	// ショートカットの保存
	if ( SaveShellLink( psl, LinkFilePath, TRUE ) == true){
		psl->Release();
		CoUninitialize();
		return(true);
	}
	else{
		psl->Release();
	}

}

CoUninitialize();
return(false);


}//CreateShellLink
//===========================================================================
// SaveShellLink
//===========================================================================
bool __fastcall TAttacheCaseOptionHandle::SaveShellLink
	( IShellLinkW *psl, String LinkName, BOOL bRemember )
{

bool fResult;

IPersistFile *ppf;

if ( psl->QueryInterface( IID_IPersistFile, (void **)&ppf ) != S_OK ){
	return(false);
}

if ( ppf->Save( LinkName.c_str(), bRemember ) == S_OK ){
	fResult = true;
}
else{
	fResult = false;
}

ppf->Release();
return(fResult);

}//SaveShellLink;
//===========================================================================

