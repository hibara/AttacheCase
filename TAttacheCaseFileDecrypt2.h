﻿//===========================================================================
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
#ifndef TAttacheCaseFileDecrypt2H
#define TAttacheCaseFileDecrypt2H
//---------------------------------------------------------------------------
#include <Classes.hpp>

#include "MsgDecrypt.hpp"

#include "rijndael.h"
#include "zlib.h"
#include "isaac.h"

//バッファサイズ
#define BUF_SIZE 32
#define LARGE_BUF_SIZE 1024
#define MARGIN_BUF_SIZE 256
#define FOOTER_BUF_SIZE 16
#define PASSWORD_BUF_SIZE 264 //MyPasscode + "_atc"

//制限サイズ4GB (4294967296-230=4294967066)
#define SIZE_4GB 4294967066

//-----------------------------------
//データバージョン（ここをいじると
// 下位バージョンで復号できないエラーが発生する）
#define ATC_DATA_FILE_VERSION 105
//-----------------------------------
//データサブバージョン
#define ATC_DATA_SUB_VERSION 6     //ver.2.70～


//アルゴリズムタイプ
#define TYPE_ALGORISM_BLOWFISH 0  // Blowfish
#define TYPE_ALGORISM_RIJNDAEL 1  // Rijndael


//---------------------------------------------------------------------------
class TAttacheCaseFileDecrypt2 : public TThread
{

private:

	//パスワードキー
	char key[32];

	//メッセージダイアログ
	String MsgText;
	TMsgDlgType MsgType;
	// --
	// mtWarning
	// mtError
	// mtInformation
	// mtConfirmation
	// mtCustom
	TMsgDlgButtons MsgButtons;
	TMsgDlgBtn MsgDefaultButton;
	int MsgReturnVal;
	String MsgReturnPath;

	//「復号したファイルを関連付けされたソフトで開く」一時的な設定
	bool fTempOpenFile;
	//最上位のフォルダーを開いたか
	bool fOpenFolderOnce;

	//バッファの入力
	__int64 __fastcall InputBuffer(
		int &buff_size, char *source_buffer, char *chain_buffer,
		TFileStream *fsIn, bool &fOpen, __int64 TotalSize );
	//バッファの出力
	int __fastcall OutputBuffer
		(char *output_buffer, int buff_size,
		TFileStream *&fsOut, bool &fOpen,
		TStringList *FileList, int &FileIndex,
		__int64 *FileSizeList, int *FileAttrList,
		int *FileDtChangeList, int *FileTmChangeList,
		int *FileDtCreateList, int *FileTmCreateList);

	//ファイル/ディレクトリのタイムスタンプを設定する
	void __fastcall FileSetTimeStamp
		(String FilePath,
		 int tsChangeDateNum, int tsChangeTimeNum, int tsCreateDateNum, int tsCreateTimeNum,
		 bool fNow, bool fDir);
	//乱数データの生成
	void fillrand(char *buf, const int len);
	//ディスクの空き容量を調べる
	__int64 __fastcall GetDiskFreeSpaceNum(String FilePath);
	//メインフォームに確認メッセージを投げて処理を中断する
	void __fastcall PostConfirmMessageForm();
	//メインフォームに上書きの確認メッセージを投げて処理を中断する
	void __fastcall PostConfirmOverwriteMessageForm();



protected:
	void __fastcall Execute();


public:
	__fastcall TAttacheCaseFileDecrypt2(bool CreateSuspended);
	__fastcall ~TAttacheCaseFileDecrypt2();

	//-----------------------------------
	// 変数
	//-----------------------------------

	int StatusNum;                     //ステータス
	String MsgAppendString;            //メッセージボックスに表示する付加テキスト
	String StatusMsg;                  //追加ステータス内容（ファイル名など翻訳不要のもの）

  __int64 AllTotalSize;              //復号処理する合計サイズ

	int ProgressPercentNum;            //進捗パーセント
	String ProgressPercentNumText;     //進捗パーセント（小数点第一位までの文字列）
	String ProgressStatusText;         //進捗ステータス
	String ProgressMsgText;            //進捗メッセージ

	bool fMemoryStoredFile;            // データをメモリ上に展開するか

	bool fCompare;                     //コンペア
	TStringList *CompareFileList;      //コンペアする元ファイルリスト

	bool fOpenFolder;                  //フォルダの場合に復号後に開くか
	bool fOpenFile;                    //復号したファイルを関連付けされたソフトで開く
	bool fConfirmOverwirte;            //同名ファイルの上書きを確認するか
	bool fOverwirteYesToAll;           //同名ファイルはすべて上書きして暗号化する（ダイアログで「すべてはい」を選択 = true）

	String AppExeFilePath;	           //アタッシェケース本体の場所
	String AtcFilePath;                //入力する暗号化ファイルパス
	String OutDirPath;                 //出力するディレクトリ

	int NumOfTrials;                   //試行回数
	int TypeLimits;                    //ミスタイプ回数 0～10
	bool fDestroy;                     //破壊するか否か 0 or 1

	//-----------------------------------
	// 関数
	//-----------------------------------

	//パスワード文字列をセットする（ver.1.* ～）
	//void __fastcall SetPasswordString(AnsiString Password);

	//パスワードにバイナリ値をセットする
	void __fastcall SetPasswordBinary(char *password);



};
//---------------------------------------------------------------------------
#endif	/* TAttacheCaseFileDecryptH */

