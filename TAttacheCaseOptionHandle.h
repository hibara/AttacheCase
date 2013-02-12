//===========================================================================
/*

アタッシェケース（AttachéCase）
Copyright (c) 2002-2013, Mitsuhiro Hibara ( http://hibara.org )
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
#ifndef TAttacheCaseOptionHandleH
#define TAttacheCaseOptionHandleH


#include <System.hpp>
#include <SysUtils.hpp>
#include <IniFiles.hpp>     //INIファイル操作用
#include <Registry.hpp>     //レジストリ操作用
#include <Math.hpp>

#include <shlobj.h>
#include <Shobjidl.h>

#include "sha1.h"
#include "blowfish.h"

#include "MsgOption.hpp"

#include "TGetAppInfoString.h"
#include "EncodeDetect.hpp"

//レジストリの場所
#define HIBARA_REGISTRY_PATH "Software\\Hibara"
#define ATTACHE_CASE_REGISTRY_PATH "Software\\Hibara\\AttacheCase"
#define ATTACHE_CASE_BAK_REGISTRY_PATH "Software\\Hibara\\AttacheCase.BAK"

#define TYPE_ENCODE_FILE 0 //暗号化
#define TYPE_DECODE_FILE 1 //復号化

//256 + 8（パスワード＋トークン）
#define BUF_PASSWORD_SIZE 264


//SHA-1ハッシュ計算（160bit＝20byte）
#define BUF_SHA1_SIZE 20
#define BUF_HASH_DATA 32


//----------------------------------------------------------------------
class TAttacheCaseOptionHandle : public TObject
{
private:


	//古いバージョンを併用しているか
	bool fOldVersionExists;
	//レジストリのリストア
	bool fRestoreRegistory;

	//記憶パスワードをレジストリまたはINIファイルから読み出す
	AnsiString __fastcall ReadMyPasswordFromRegistry(int Type);
	//記憶パスワードをレジストリまたはINIファイル保存する
	bool __fastcall SaveMyPasswordToRegistry(String Password, int Type);

	//ハードディスクのボリュームラベルIDを取得する
	String __fastcall GetVolumeLabelIDNumber(void);
	// ロングファイル名を取得する
	String __fastcall GetLongFilePath(String FilePath);
	// Windowsが許さないファイル名文字列の検査関数
	bool __fastcall ChkFileNameChr(String ChkText);

	// SaveShellLink
	bool __fastcall SaveShellLink(IShellLinkW *psl, String LinkName, BOOL bRemember);

	// テキストファイルを読み込んで文字エンコーディングを判定して返す
	TEncoding* __fastcall GetCharEncoding(String FilePath);



protected:

public:

	__fastcall TAttacheCaseOptionHandle();
	__fastcall ~TAttacheCaseOptionHandle();

	//===================================
	// 変数
	//===================================

	// 読み込んだ先のパス（INIファイルパス）
	String OptionPath;
	int OptType;                    // 0:レジストリ, 1:INIファイル, 2:コマンドライン引数

	//アプリケーション情報
	String AppPath;                 //本体がインストールされたフルパス
	int VersionNum;                 //バージョン

	//フォームポジション
	int FormTop;
	int FormLeft;
	int FormWidth;
	int FormHeight;
	int WinStyle;

	int ActiveTabNum;               //開いていたタブ

	//【基本設定】
	String MyPassword;              //記憶しているパスワード

	bool fArg;                      //実行引数処理をするか
	bool fArgPassword;              //実行引数にパスワードを指定

	bool fMyEncodePasswordKeep;     //暗号化パスワードを記憶するか
	bool fMyDecodePasswordKeep;     //復号パスワードを記憶するか
	AnsiString MyEncodePassword;    //記憶暗号化パスワード
	AnsiString MyDecodePassword;    //記憶復号化パスワード

	bool fMemPasswordExe;           //記憶パスワードで即座に実行する
	bool fOpenFolder;               //フォルダの場合に復号後に開くか
	bool fOpenFile;                 //復号したファイルを関連付けされたソフトで開く
	bool fEndToExit;                //処理後に終了するか
	bool fWindowForeground;         //デスクトップで最前面にウィンドウを表示する
	bool fNoHidePassword;           //「*」で隠さずパスワードを確認しながら入力する
	bool fSaveToExeout;             //常に自己実行形式で出力する
	bool fShowExeoutChkBox;         //メインフォームにチェックボックスを表示する
	bool fAskEncDecode;             //暗号/復号処理かを問い合わせる
	int ProcTypeWithoutAsk;         //暗号/復号処理か（動作設定にはない。コマンドラインのみ）
	bool fNoMultipleInstance;       //複数起動しない

	//【保存設定】
	bool fSaveToSameFldr;           //暗号化ファイルを常に同じ場所に保存するか
	String SaveToSameFldrPath;      //その保存場所
	bool fDecodeToSameFldr;         //常に同じ場所へファイルを復号するか
	String DecodeToSameFldrPath;    //その保存場所
	bool fConfirmOverwirte;         //同名ファイルの上書きを確認するか
	bool fKeepTimeStamp;            //暗号化ファイルのタイムスタンプを元ファイルに合わせる
	bool fSameTimeStamp;            //復号したファイルのタイムスタンプを生成日時にする
	bool fAllFilePack;              //複数のファイルを暗号化する際は一つにまとめる
	bool fFilesOneByOne;            //フォルダ内のファイルは個別に暗号化/復号する
	bool fNoParentFldr;             //復号するときに親フォルダを生成しない
	bool fExtInAtcFileName;         //暗号化ファイル名に拡張子を含める
	bool fAutoName;                 //自動で暗号化ファイル名を付加する
	String AutoNameFormatText;	    //自動で付加するファイル名書式

	//【削除設定】
	bool fDelOrgFile;               //暗号化した後、元ファイルを削除する
	bool fDelEncFile;               //復号化した後、暗号化ファイルを削除する
	bool fShowDeleteChkBox;         //メインフォームにチェックボックスを表示する
	int fCompleteDelete;            //完全削除を行うか(0:通常，1:完全削除，2:ごみ箱）
	int DelRandNum;                 //乱数を何回書き込み消去するか
	int DelZeroNum;                 //NULLを何回書き込み消去するか

	//【動作設定】
	int CompressRate;               //圧縮率
	bool fCompareFile;              //暗号処理後にファイルコンペアを行うか

	//【システム】
	bool fAssociationFile;          //関連付け設定を保持するか
	int AtcsFileIconIndex;          //ファイルアイコン番号
	String UserRegIconFilePath;     //ユーザー指定のファイルアイコンパス

	//【高度な設定】
	bool fAllowPassFile;            //パスワードファイルを許可する
	bool fCheckPassFile;            //暗号時にパスワードファイルを自動チェックする
	String PassFilePath;            //暗号時のパスワードファイルパス
	bool fCheckPassFileDecrypt;     //復号時にパスワードファイルを自動チェックする
	String PassFilePathDecrypt;     //復号時のパスワードファイルパス
	bool fNoErrMsgOnPassFile;       //パスワードファイルがない場合エラーを出さない
	bool fAddCamoExt;               //暗号化ファイルの拡張子を偽装する
	String CamoExt;                 //その拡張子
	int MissTypeLimitsNum;          //パスワードのタイプミス制限回数（ver.2.70～）
	bool fBroken;                   //タイプミス回数を超えたときにファイルを破壊するか否か（ver.2.70～）

	//【コマンドラインからのみ指定】
	bool fOver4GBok;                //4GB超えを容認
	bool fHideMainForm;             //メインフォームを非表示
	//int WinStyle;                 //ウィンドウの状態(0: wsNormal, 1: wsMinimized, 2: wsMaximized）
	bool fNoErrorMsg;               //エラーメッセージ表示の抑制


	//===================================
	// 関数
	//===================================

	//動作設定の読み込み
	bool __fastcall LoadOptionData(String IniFilePath);
	//動作設定を記録する
	bool __fastcall SaveOptionData(void);
	//コマンドラインオプションからの設定読み込み
	bool __fastcall LoadOptionDataFromParamString(TStringList *FileList);

	//動作設定をKeyValue形式のTStringListで取得する
	void __fastcall GetKeyValueData(TStringList *KeyValueList);

	//カスタマイズされた文字列を解釈してファイル名を置き換える
	String __fastcall InterpretFormatText(String FileName, String InputText, int Num);
	//実際にカスタマイズ文字列を解釈して重複のない(連番の)ファイルパスを返す
	String __fastcall InterpretFormatTextToFilePath(String FilePath, String InputText);
	//指定のランダム文字列を生成
	String __fastcall MakeRandomFileName(int MojiNum, bool fNum, bool fKigo);
	//ファイルからSHA-1ハッシュ値を取得する
	bool __fastcall GetSHA1HashFromFile(
		String FilePath,              //パスワードファイルパス
		AnsiString &HashDataVer2,     //ver.2.*  ～：SHA-1ハッシュ値（足りないデータを補填した不完全な32byte）
		AnsiString &HashDataVer1 );   //ver.1.*  ～：ヘッダデータ（先頭文字列32文字）

	//「送る」フォルダーまでのアプリケーションフルパスを生成する
	String __fastcall CreateSendToFolderAppFullPath(void);
	// ショートカットファイルを作成する
	bool __fastcall CreateShortCutFile
		( String LinkFilePath,  String TargetFilePath, String Arguments, String WorkDir, int ShowCmd );

};


#endif

