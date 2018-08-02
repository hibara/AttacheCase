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

#include <vcl.h>

#pragma hdrstop

#include "Unit1.h"
#include "TAttacheCaseFileDecrypt1.h"
#include <RegularExpressions.hpp>

#pragma package(smart_init)
// ---------------------------------------------------------------------------
// 注意：異なるスレッドが所有する VCL のメソッド/関数/プロパティを別の
// レッド所有のオブジェクトに対しては Synchronize を使用できます。
//
// Synchronize(&UpdateCaption);
//
// 例えば UpdateCaption を以下のように定義し
//
// void __fastcall TAttacheCaseFileEncode::UpdateCaption()
// {
// Form1->Caption = "スレッドから書き換えました";
// }
// ---------------------------------------------------------------------------
__fastcall TAttacheCaseFileDecrypt1::TAttacheCaseFileDecrypt1
(bool CreateSuspended) : TThread(CreateSuspended)
{

KeyString = "";                    // パスワード

ProgressPercentNum = -1;           // 進捗パーセント
ProgressStatusText = "";           // 進捗ステータス
ProgressMsgText = "";              // 進捗メッセージ

AtcFilePath = "";                  // 入力する暗号化ファイル
OutDirPath = "";                   // 出力するディレクトリ

fOverwirteYesToAll = false;        //同名ファイルはすべて上書きして暗号化する（ダイアログで「すべてはい」を選択 = true）

NumOfTrials = 1;                   // パスワード入力試行回数

StatusNum = -1;                    // 処理結果ステータス
                                   //   1: 復号成功
                                   //   0: ユーザーキャンセル
                                   //  -1: パスワード入力エラー
																	 //  -2: 復号エラー

}
//===========================================================================
//デストラクタ
//===========================================================================
__fastcall TAttacheCaseFileDecrypt1::~TAttacheCaseFileDecrypt1(void)
{

//

}
//===========================================================================
//スレッド実行
//===========================================================================
void __fastcall TAttacheCaseFileDecrypt1::Execute()
{

int i, c, len;
float ProgressPercentNumF;  //進捗パーセンテージ（浮動小数点）

// バッファ
char buffer[BUF_SIZE];
char source_buffer[BUF_SIZE];
char temp_buffer[BUF_SIZE];
char output_buffer[LARGE_BUF_SIZE];
char *headerbuffer;

//パスワード
bool fPasswordOk = false;

String FilePath, FileName;

// ファイルストリーム
TFileStream *fsIn;
TFileStream *fsOut;
bool fInputFileOpen = false;
bool fOutputFileOpen = false;

float free_space_required;
__int64 CurrentPos, TotalSize;
__int64 CurrentDriveFreeSpaceSize;

//処理する合計サイズ
AllTotalSize = 0;

int ret;	//バッファ出力の返値
int FileIndex = 0;

//Blowfish
CBlowFish *bf;

char token[17] = {0, };
const String PrefixString      = "Fn_";
const char charTokenString[17] = "_AttacheCaseData";         //復号の正否に使う
String AtcFileTokenString;                                   //暗号化ファイルのトークン（文字列）
String AtcFileCreateDateString;                              //暗号化ファイルの生成日時（文字列）

//同名ファイル/フォルダーはすべて上書きして復号する
//（ユーザーがダイアログで「すべてはい」を選択したとき = true ）
fOverwirteYesToAll = false;
//「復号したファイルを関連付けされたソフトで開く」一時的な設定
fTempOpenFile = fOpenFile;
//フォルダーを一度開いたか（深いフォルダーすべてを開かないように）
fOpenFolderOnce = false;

// 出力するディレクトリ
OutDirPath = IncludeTrailingPathDelimiter(OutDirPath);

String TempRelativePath;


// 暗号化部分のヘッダサイズ
int EncryptHeaderSize = 0;

int DataVersion;        // ver.2.00～は "5", ver.2.70～は "6"

int flush, status;      // zlib
z_stream z;             // zlibライブラリとやりとりするための構造体
bool fInputEnd = false; // 入力ストリームの終了

//ヘッダデータから必要情報を取り出すための
TMemoryStream *pms;     // メモリストリーム

int idx;
TStringList *DataList;
TStringList *tsv;

TStringList *FileList = new TStringList();  // 0: ファイル名
__int64 *FileSizeList = 0;                  // 1: ファイルサイズ（フォルダは-1）
int *FileAttrList     = 0;                  // 2: 属性
int *FileAgeList      = 0;                  // 3: タイムスタンプ

int rest;
int buf_size;

//----------------------------------------------------------------------
// 平文のヘッダ内容チェック
try {
	fsIn = new TFileStream(AtcFilePath, fmOpenRead | fmShareDenyNone);
}
catch(...) {
	//'復号するファイルを開けません。他のアプリケーションが使用中の可能性があります。'
	MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_FILE_OPEN);
	MsgType = mtError;
	MsgButtons = TMsgDlgButtons() << mbOK;
	MsgDefaultButton = mbOK;
	Synchronize(&PostConfirmMessageForm);
	goto LabelError;
}

fInputFileOpen = true;

// 暗号部ヘッダサイズを取得
fsIn->Read(&EncryptHeaderSize, sizeof(int));
// トークンを取得
fsIn->Read(token, 16);

if (StrComp(token, charTokenString) != 0 ) {
	//--------------------------------------------------------
	//実は自己実行形式ファイル？（拡張子偽装されている場合も）
	//--------------------------------------------------------
	// サイズを再取得
	fsIn->Seek(-(__int64)sizeof(__int64), TSeekOrigin::soEnd);
	fsIn->Read(&AllTotalSize, sizeof(__int64));
	// 位置を戻す
	fsIn->Seek(-(AllTotalSize + sizeof(__int64)), TSeekOrigin::soEnd);
	// もう一度、暗号部ヘッダサイズを取得
	fsIn->Read(&EncryptHeaderSize, sizeof(int));
	// もう一度、トークンを取得
	fsIn->Read(token, 16);

	// トークンを再チェック
	if (StrComp(token, charTokenString) != 0 ) {
		// '暗号化ファイルではありません。アタッシェケースによって暗号化されたファイルでないか、'+#13+
		// 'ファイルが壊れている可能性があります。'
		MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_FILE_NOT_ATC);
		MsgType = mtError;
		MsgButtons = TMsgDlgButtons() << mbOK;
		MsgDefaultButton = mbOK;
		Synchronize(&PostConfirmMessageForm);
		goto LabelError;
	}

}
else{
	AllTotalSize = fsIn->Size;
}

//-----------------------------------
// データバージョンチェック
//-----------------------------------
DataVersion = -1;
fsIn->Read(&DataVersion, sizeof(int));

if (DataVersion <= 103) {

	// Blowfishで暗号化されたファイル

}
else{
	//'バージョンがちがいます。復号できません。'+#13+
	//'ファイルはver.1のアタッシェケースで暗号化されていません。';
	MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_NOT_BLOWFISH_ENCRYPTION);
	MsgType = mtError;
	MsgButtons = TMsgDlgButtons() << mbOK;
	MsgDefaultButton = mbOK;
	Synchronize(&PostConfirmMessageForm);
	goto LabelError;
}

//-----------------------------------
// 復号の準備
//-----------------------------------

bf = new CBlowFish;
bf->Initialize( KeyString.c_str(), KeyString.Length() );   //初期化

//-----------------------------------
// 暗号部ヘッダの復号（ECBモード）
//-----------------------------------

pms = new TMemoryStream;

len = 0;
while (len < EncryptHeaderSize) {
	// 読み出しバッファ
	for (c = 0; c < BUF_SIZE; c++) {
		source_buffer[c] = 0;
	}
	// 暗号化されたデータブロックの読み出し
	len += fsIn->Read(source_buffer, BUF_SIZE);

	// 復号処理
	bf->Decode( source_buffer, buffer, BUF_SIZE);
	pms->Write(buffer, BUF_SIZE);
}

pms->Seek((__int64)0, TSeekOrigin::soBeginning);             //ポインタを先頭へ戻す
DataList = new TStringList;
DataList->LoadFromStream(pms, TEncoding::GetEncoding(932));  // shift-jis


//-----------------------------------
// 復号正否（復号できたか）
//-----------------------------------
if (DataList->Count == 0 || DataList->Strings[0].Pos("AttacheCase") == 0) {
	fPasswordOk = false;
}
else{
	fPasswordOk = true;   //パスワード合致
}

if ( fPasswordOk == false ) {
	//'パスワードがちがいます。復号できません。'+#13+
	//'場合によってはファイルが壊れている可能性もあります。';
	MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_PASSWORD_WRONG);
	MsgType = mtError;
	MsgButtons = TMsgDlgButtons() << mbOK;
	MsgDefaultButton = mbOK;
	Synchronize(&PostConfirmMessageForm);
	delete DataList;
	goto LabelTypeMiss;
}

//-----------------------------------
// 復号時のエンコーディング判定
//-----------------------------------
pms->Position = 0;
DataList->LoadFromStream(pms, TEncoding::GetEncoding(932));  // shift-jis


//===================================
// デバッグ
//ShowMessage(DataList->Text);
//===================================

delete pms;


//-----------------------------------
// 暗号化ファイルの生成日時
//-----------------------------------
//※特に今は使用していないが、将来的に
//  期限付きでファイルを復号できなくなる
//  などの機能を追加しても良いかも。
//-----------------------------------
AtcFileCreateDateString = DataList->Strings[1];

//-----------------------------------
// ヘッダデータからファイルリストや
// ファイル情報などを各変数を動的確保
//-----------------------------------

FileSizeList = new __int64[DataList->Count];  // 1: ファイルサイズ（フォルダは-1）
FileAttrList = new int[DataList->Count];      // 2: 属性
FileAgeList = new int[DataList->Count];       // 3: タイムスタンプ

DataList->NameValueSeparator = ':';

tsv = new TStringList;
tsv->Delimiter = '\t';
tsv->StrictDelimiter = true;

for (i = 0; i < DataList->Count; i++) {
	idx = DataList->IndexOfName(PrefixString+IntToStr(i));
	if (idx > 0) {
		tsv->DelimitedText = DataList->ValueFromIndex[idx];

		// ディレクトリトラバーサル対策（ver.2.8.4.0～）
		if (TRegEx::IsMatch(tsv->Strings[0], "^[a-zA-Z]:|\.\.\s*[\\/]|\\\\")){
			//'不正なファイルパスです。復号できません。';
			MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_INVALID_FILE_PATH);
			MsgType = mtError;
			MsgButtons = TMsgDlgButtons() << mbOK;
			MsgDefaultButton = mbOK;
			Synchronize(&PostConfirmMessageForm);
			delete DataList;
			goto LabelTypeMiss;
		}

		FileList->Add(tsv->Strings[0]);                        // 0: ファイルパス
		FileSizeList[i] = StrToIntDef(tsv->Strings[1], -1);    // 1: ファイルサイズ（フォルダは-1）
		FileAttrList[i] = StrToIntDef(tsv->Strings[2], -1);    // 2: 属性
		FileAgeList[i] = StrToIntDef(tsv->Strings[3], -1);     // 3: タイムスタンプ
	}
}

delete tsv;
delete DataList;


//-----------------------------------
//ディスクの空き容量チェック
//-----------------------------------
CurrentDriveFreeSpaceSize = GetDiskFreeSpaceNum(OutDirPath);

if (CurrentDriveFreeSpaceSize > -1) {
	if ( AllTotalSize > CurrentDriveFreeSpaceSize ) {
		//'復号する先のドライブの空き容量が足りません。'+#13+
		//'復号するには、約%dMBの空き容量が必要です。復号処理を中止します。';
		MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_DRIVE_NO_FREE_SPACE);
		free_space_required = (float)AllTotalSize/1024/1024;  // MB
		MsgText = String().sprintf(MsgText.c_str(), free_space_required);
		MsgType = mtError;
		MsgButtons = TMsgDlgButtons() << mbOK;
		MsgDefaultButton = mbOK;
		Synchronize(&PostConfirmMessageForm);
		goto LabelError;
	}
}
else{
	// OK!
	//
}

//-----------------------------------
//複数フォルダ/ファイルを開こうとしているので確認
//-----------------------------------

//復号したファイルを関連付けされたソフトで開くか
if ( fTempOpenFile == true && FileList->Count > 4) {

	//'5つ以上のファイルを復号後に開こうとしていますが、処理を続けますか？'+#13+
	//'「いいえ」を選択すると、開かず復号します。';
	MsgText = LoadResourceString(&Msgdecrypt::_MSG_CONFIRM_OPEN_DECRYPTED_FILES);
	MsgType = mtConfirmation;
	MsgButtons = TMsgDlgButtons() << mbYes << mbNo << mbCancel;
	MsgDefaultButton = mbCancel;
	Synchronize(&PostConfirmMessageForm);

	if ( MsgReturnVal == mrCancel ) {
		goto LabelStop;
	}
	else if ( MsgReturnVal == mrNo ) {
		fTempOpenFile = true;
	}

}

//-----------------------------------
// 復号開始
//-----------------------------------

//'復号しています...'
ProgressStatusText = LoadResourceString(&Msgdecrypt::_LABEL_STATUS_TITLE_DECRYPTING);
ProgressMsgText = ExtractFileName(AtcFilePath);

// ファイル（データ本体）サイズを取得する
AllTotalSize = fsIn->Size - fsIn->Position + 1;

// zlib 前準備
// 圧縮においてすべてのメモリ管理をライブラリに任せる
z.zalloc = Z_NULL;
z.zfree = Z_NULL;
z.opaque = Z_NULL;

if (inflateInit(&z) != Z_OK) {
	// エラー
	// zlibエラーは最後でまとめてメッセージ処理
	goto LabelError;
}

// 通常は deflate() の第2引数は Z_NO_FLUSH にして呼び出す
flush = Z_NO_FLUSH;

// 入出力バッファ
for (i = 0; i < LARGE_BUF_SIZE; i++) {
	output_buffer[i] = 0;
}

z.avail_in  = 0;               // 入力バッファ中のデータのバイト数
z.next_in   = Z_NULL;          // 入力バッファ中のデータのバイト数
z.next_out  = output_buffer;   // 出力バッファ残量
z.avail_out = LARGE_BUF_SIZE;  // 出力ポインタ（展開するので大きめに）
status      = Z_OK;

while (Terminated == false) {

	//-----------------------------------
	// 入力
	//-----------------------------------
	if (z.avail_in == 0) {

		TotalSize = InputBuffer(bf, len, source_buffer, fsIn, fInputFileOpen, TotalSize);
		z.avail_in = len;
		z.next_in = source_buffer;

		if ( len == 0 ) {  //入力ストリーム終了
			fInputEnd = true;
		}

	}

	//-----------------------------------
	// 展開
	//-----------------------------------
	status = inflate(&z, flush);

	//-----------------------------------
	// 処理ステータス
	//-----------------------------------
	if ( status == Z_OK ){

		if ( z.avail_out == 0 ) {

			ret = OutputBuffer(output_buffer, LARGE_BUF_SIZE,
												 fsOut, fOutputFileOpen,
												 FileList, FileIndex,
												 FileSizeList, FileAttrList, FileAgeList);

			if ( ret == 0) {
				z.next_out = output_buffer;
				z.avail_out = LARGE_BUF_SIZE;
			}
			else if (ret == -1) {
				goto LabelError;
			}
			else{
				goto LabelStop;
			}

		}//end if (z.avail_out == 0);

	}
	//-----------------------------------
	// バッファエラー
	//-----------------------------------
	else if ( status == Z_BUF_ERROR ) { //出力バッファがいっぱいの可能性

		//出力バッファをクリアにして継続させる
		len = LARGE_BUF_SIZE - z.avail_out;
		ret = OutputBuffer(output_buffer, len,
											 fsOut, fOutputFileOpen,
											 FileList, FileIndex,
											 FileSizeList, FileAttrList, FileAgeList);
		if (ret == 0) {
			z.next_out = output_buffer;
			z.avail_out = LARGE_BUF_SIZE;
		}
		else if ( ret == -1) {
			goto LabelError;
		}
		else{
			goto LabelStop;
		}

	}
	//-----------------------------------
	// 終了
	//-----------------------------------
	else if (status == Z_STREAM_END) {
		break;
	}
	//-----------------------------------
	// エラー
	//-----------------------------------
	else{
		// #define Z_OK              0
		// #define Z_STREAM_END      1
		// #define Z_NEED_DICT       2
		// #define Z_ERRNO         (-1)
		// #define Z_STREAM_ERROR  (-2)
		// #define Z_DATA_ERROR    (-3)
		// #define Z_MEM_ERROR     (-4)
		// #define Z_BUF_ERROR     (-5)
		// #define Z_VERSION_ERROR (-6)
		goto LabelError;
	}


	//-----------------------------------
	//進捗状況表示
	ProgressPercentNumF = (float)TotalSize/AllTotalSize;
	ProgressPercentNum = (int)(ProgressPercentNumF*100);
	if (AllTotalSize < 104857600) {	// 100MB未満
		ProgressPercentNumText = IntToStr(ProgressPercentNum)+"%";
	}
	else{
		ProgressPercentNumText = FloatToStrF(ProgressPercentNumF*100, ffNumber, 4, 1)+"%";
	}

	if ( fInputEnd == true) {
		break;
	}

}//while (!Terminated);

if (Terminated == true) {
	//ユーザーキャンセルで抜けてきた
	goto LabelStop;
}

//----------------------------------------------------------------------
// 万が一、出力バッファに余りがある場合
//----------------------------------------------------------------------

len = LARGE_BUF_SIZE - z.avail_out;
ret = OutputBuffer(output_buffer, len,
									 fsOut, fOutputFileOpen,
									 FileList, FileIndex,
									 FileSizeList, FileAttrList, FileAgeList);
if ( ret == 0 ) {
}
else if ( ret == -1 ){
	goto LabelError;
}
else{
	goto LabelStop;
}


if (inflateEnd(&z) != Z_OK) {
	goto LabelError;
}

ProgressPercentNum = 100;
//'完了'
ProgressStatusText = LoadResourceString(&Msgdecrypt::_LABEL_STATUS_TITLE_COMPLETE);
ProgressMsgText = ExtractFileName(AtcFilePath);

if ( fInputFileOpen == true ) {
	delete fsIn;
	fInputFileOpen = false;
}
if ( fOutputFileOpen == true ) {
	delete fsOut;
	fOutputFileOpen = false;
}

delete FileList;
delete [] FileSizeList;      // 1: ファイルサイズ（フォルダは-1）
delete [] FileAttrList;      // 2: 属性
delete [] FileAgeList;       // 3: 更新日

//復号成功
StatusNum = 1;
return;


//-----------------------------------
//パスワード入力ミスの後始末
//-----------------------------------
LabelTypeMiss:

	ProgressPercentNum = 0;

	//'エラー'
	ProgressStatusText = LoadResourceString(&Msgdecrypt::_LABEL_STATUS_TITLE_ERROR);
	//'復号に失敗しました。'
	ProgressMsgText = LoadResourceString(&Msgdecrypt::_LABEL_STATUS_DETAIL_FAILED);

	if ( fInputFileOpen == true ) {
		delete fsIn;
		fInputFileOpen = false;
	}
	if ( fOutputFileOpen == true ) {
		delete fsOut;
		fOutputFileOpen = false;
	}

	delete FileList;
	delete [] FileSizeList;
	delete [] FileAttrList;
	delete [] FileAgeList;

	StatusNum = -1;
	return;


//-----------------------------------
//エラーの後始末
//-----------------------------------
LabelError:

	ProgressPercentNum = 0;

	if ( status < 0 ){
		//'zlibライブラリからエラーを返されました。'
		//'エラー番号：'
		MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_ZLIB) + IntToStr(status) + "\n" + z.msg;
		MsgType = mtError;
		MsgButtons = TMsgDlgButtons() << mbOK;
		MsgDefaultButton = mbOK;
		Synchronize(&PostConfirmMessageForm);
	}
	//'エラー'
	ProgressStatusText = LoadResourceString(&Msgdecrypt::_LABEL_STATUS_TITLE_ERROR);
	//'復号に失敗しました。'
	ProgressMsgText = LoadResourceString(&Msgdecrypt::_LABEL_STATUS_DETAIL_FAILED);

	if ( fInputFileOpen == true ) {
		delete fsIn;
		fInputFileOpen = false;
	}
	if ( fOutputFileOpen == true ) {
		delete fsOut;
		fOutputFileOpen = false;
	}

	delete FileList;
	delete [] FileSizeList;
	delete [] FileAttrList;
	delete [] FileAgeList;

	StatusNum = -2;
	return;


//-----------------------------------
//ユーザーキャンセルの後始末
//-----------------------------------
LabelStop:

	ProgressPercentNum = 0;
	//'キャンセル'
	ProgressStatusText = LoadResourceString(&Msgdecrypt::_LABEL_STATUS_TITLE_USER_CANCEL);
	//'復号が中止されました。'
	ProgressMsgText = LoadResourceString(&Msgdecrypt::_LABEL_STATUS_DETAIL_STOPPED);

	if ( fInputFileOpen == true ) {
		delete fsIn;
		fInputFileOpen = false;
	}
	if ( fOutputFileOpen == true ) {
		delete fsOut;
		fOutputFileOpen = false;
	}

	delete FileList;
	delete [] FileSizeList;
	delete [] FileAttrList;
	delete [] FileAgeList;

	StatusNum = 0;
	return;

}
//===========================================================================
// バッファの入力
//===========================================================================
__int64 __fastcall TAttacheCaseFileDecrypt1::InputBuffer
	(CBlowFish *bf, int &buff_size, char *source_buffer,
	 TFileStream *fsIn, bool &fOpen, __int64 TotalSize)
{

char i;
int len;

char buffer[BUF_SIZE];

// 入力ファイルが開かれていない
if ( fOpen == false ) {
	buff_size = 0;
	return(TotalSize);
}

// 読み出しバッファの初期化
for (i = 0; i < BUF_SIZE; i++) {
	source_buffer[i] = 0;
}

// 暗号化されたデータブロックの読み出し
len = fsIn->Read(buffer, BUF_SIZE);
if ( len == 0 ) {
	buff_size = 0;
	return(TotalSize);
}
else{
	TotalSize += len;
}

//-----------------------------------
//ECBモードで暗号化されている
//-----------------------------------

// 復号処理
bf->Decode( buffer, source_buffer, len);

buff_size = len;
return(TotalSize);

}
//===========================================================================
// バッファの出力
//===========================================================================
int __fastcall TAttacheCaseFileDecrypt1::OutputBuffer
	(char *output_buffer, int buff_size,
	TFileStream *&fsOut, bool &fOpen,
	TStringList *FileList, int &FileIndex,
	__int64 *FileSizeList, int *FileAttrList, int *FileAgeList)
{

//返値　0=<:成功, -1:エラー, -2:ユーザーキャンセル

int res;

int rest;
String FileName, FilePath, DirPath;
char read_buffer[LARGE_BUF_SIZE];      //コンペア用
char temp_buffer[LARGE_BUF_SIZE];      //データ詰める用

//----------------------------------------------------------------------
// 解凍されたバッファがすべて書き込まれるまでループ
//----------------------------------------------------------------------

// ※0バイトファイルなどの場合もあるので
// 　一度はループに入れるようにした。

//while (buff_size > 0 && !Terminated) {
do{

	// ファイルが開かれていないならオープンする
	if ( fOpen == false ) {

		for (FileIndex; FileIndex < FileList->Count; FileIndex++) {

			FileName = FileList->Strings[FileIndex];
			FilePath = OutDirPath + FileName;

			//----------------------------------------------------------------------
			// ディレクトリ
			//----------------------------------------------------------------------
			if (FileName.IsPathDelimiter(FileList->Strings[FileIndex].Length()) == true) {

				if ( fConfirmOverwirte == true && fOverwirteYesToAll == false ) {

					if (DirectoryExists(FilePath) == true) {

						//同名フォルダーの上書き確認メッセージダイアログ
						MsgText = LoadResourceString(&Msgdecrypt::_MSG_CONFIRM_OVER_WRITE_SAME_DIR)+"\n"+FilePath;
						Synchronize(&PostConfirmOverwriteMessageForm);

						if ( MsgReturnVal == mrYes ) {
							//上書きOKなのでFilePathはそのまま
						}
						else if ( MsgReturnVal == mrNo ) {
							//別名保存でFilePath文字列が書き換えられてきている
							FilePath = MsgReturnPath;
						}
						else if ( MsgReturnVal == mrYesToAll ) {
							//すべて上書き（YesToAll）
							fOverwirteYesToAll = true;
						}
						else if ( MsgReturnVal == mrCancel ) {
							//キャンセル
							goto LabelStop;
						}
					}

				}

				//-----------------------------------
				//暗号化ファイル自身への復号はNG
				//（ex: ..\hoge -> ..\hoge）
				//-----------------------------------
				if ( FilePath.CompareIC(AtcFilePath) == 0 ) {
					//'暗号化ファイル自身にファイルまたはフォルダーを復号することはできません。'+#13+
					//'復号処理を中止します。';
					MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_NOT_OVERWRITE_MYSELF)+"\n"+FilePath;
					MsgType = mtError;
					MsgButtons = TMsgDlgButtons() << mbOK;
					MsgDefaultButton = mbOK;
					Synchronize(&PostConfirmMessageForm);
					//エラー
					goto LabelError;
				}

				//フォルダの生成
				ForceDirectories(FilePath);
				//タイムスタンプのセット
				//FileSetDate(FilePath, FileAge[i]);  //フォルダはタイムスタンプをセットしない
				//属性のセット
				FileSetAttr(FilePath, FileAttrList[FileIndex]);

				if ( fOpenFolder == true && fOpenFolderOnce == false ) {
					ShellExecuteW(NULL, L"open", L"EXPLORER.EXE", FilePath.c_str(), FilePath.c_str(), SW_NORMAL);  //脆弱性対策（ver.2.70）
					//ShellExecuteW(NULL, L"explore", FilePath.c_str(), NULL, NULL, SW_NORMAL);	//こちらでも正常動作したようだ
					fOpenFolderOnce = true;
				}

				continue;

			}
			//----------------------------------------------------------------------
			// ファイル
			//----------------------------------------------------------------------
			else {

				if (fConfirmOverwirte == true && fOverwirteYesToAll == false ) {

					DirPath = ExtractFileDir(FilePath);
					if (DirectoryExists(DirPath) == false) {
						ForceDirectories(DirPath);
					}

					if (FileExists(FilePath) == true) {
						//同名ファイルの上書き確認メッセージダイアログ
						MsgText = LoadResourceString(&Msgdecrypt::_MSG_CONFIRM_OVER_WRITE_SAME_FILE)+"\n"+FilePath;
						Synchronize(&PostConfirmOverwriteMessageForm);

						if ( MsgReturnVal == mrYes ) {
							//上書きOKなのでFilePathはそのまま
						}
						else if ( MsgReturnVal == mrNo ) {
							//別名保存でFilePath文字列が書き換えられてきている
							FilePath = MsgReturnPath;
						}
						else if ( MsgReturnVal == mrYesToAll ) {
							//すべて上書き（YesToAll）
							fOverwirteYesToAll = true;
						}
						else if ( MsgReturnVal == mrCancel ) {
							//キャンセル
							goto LabelStop;
						}
					}

				}

				//-----------------------------------
				//暗号化ファイル自身への復号はNG
				//（ex: ..\hoge -> ..\hoge）
				//-----------------------------------
				if ( FilePath.CompareIC(AtcFilePath) == 0) {
					//'暗号化ファイル自身にファイルまたはフォルダーを復号することはできません。'+#13+
					//'復号処理を中止します。';
					MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_NOT_OVERWRITE_MYSELF)+"\n"+FilePath;
					MsgType = mtError;
					MsgButtons = TMsgDlgButtons() << mbOK;
					MsgDefaultButton = mbOK;
					Synchronize(&PostConfirmMessageForm);
					//エラー
					goto LabelError;
				}

				try {
					// TODO: ver.2.75以前で「複数ファイルは一つの暗号化ファイルにまとめる」に
					// チェックして暗号化されていた場合の対処
					DirPath = ExtractFileDir(FilePath);
					if (DirectoryExists(DirPath) == false) {
						ForceDirectories(DirPath);	//親フォルダーがなければ強制的に生成する
					}
					//出力するファイルを開く
					fsOut = new TFileStream(FilePath, fmCreate);
				}
				catch(...) {
					//保存する先のファイルが開けません。他のアプリケーションが使用中の可能性があります。
					MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_OUT_FILE_OPEN)+"\n"+FilePath;
					MsgType = mtError;
					MsgButtons = TMsgDlgButtons() << mbOK;
					MsgDefaultButton = mbOK;
					Synchronize(&PostConfirmMessageForm);
					goto LabelError;

				}

				fOpen = true;

				if (FileSizeList[FileIndex] == 0) { // 0バイトファイル？

					delete fsOut;
					fOpen = false;

					FileSetAttr(FilePath, (int)FileAttrList[FileIndex]); //属性のセット
					FileSetDate(FilePath, (int)FileAgeList[FileIndex]);  //タイムスタンプのセット

					//関連付けられたアプリケーションでファイルを開く
					if ( fTempOpenFile == true ) {
						ShellExecuteW(NULL, L"open", FilePath.c_str(), NULL, NULL, SW_NORMAL);
					}
				}
				else{
					break;
				}

			}

		}//end for;


	}//end if (fOpen == false);

	//-----------------------------------
	// 開いたファイルに書き込む
	//-----------------------------------
	if ( fOpen == true ) {

		//-----------------------------------
		// 通常書き込み
		//-----------------------------------

		if (fsOut->Size + buff_size < FileSizeList[FileIndex]) {

			if (fsOut->Write(output_buffer, buff_size) != buff_size) {
				goto LabelReadWriteError;
			}
			else {
				buff_size = 0;
			}

		}
		else {
			//クローズするファイルパス（書き込んでいたファイルパス）
			FileName = FileList->Strings[FileIndex];
			FilePath = OutDirPath + FileName;

			rest = FileSizeList[FileIndex] - fsOut->Size;

			if (fsOut->Write(output_buffer, rest) != rest) {
				goto LabelReadWriteError;
			}
			else {

				buff_size -= rest;

				//残ったバッファを前に詰める
				for (int i = 0; i < LARGE_BUF_SIZE; i++) {
					if (i < buff_size) {
						temp_buffer[i] = output_buffer[rest+i];
					}
					else{
						temp_buffer[i] = 0;
					}
				}
				memcpy(output_buffer, temp_buffer, LARGE_BUF_SIZE);

				delete fsOut;
				fOpen = false;

				FileSetAttr(FilePath, (int)FileAttrList[FileIndex]); //属性のセット
				FileSetDate(FilePath, (int)FileAgeList[FileIndex]);  //タイムスタンプのセット

				//関連付けられたアプリケーションでファイルを開く
				if ( fTempOpenFile == true ) {
					ShellExecuteW(NULL, L"open", FilePath.c_str(), NULL, NULL, SW_NORMAL);
				}
				FileIndex++;
			}

		}

	}
	else {
		//もう開くファイルがない
		break;

	//-----------------------------------
	}//end if ( fOpen == true );
	//-----------------------------------


}while (buff_size > 0 && !Terminated);//end while ( buf_size > 0 );
//----------------------------------------------------------------------

//出力バッファはきれいにしておく
for (int i = 0; i < LARGE_BUF_SIZE; i++) {
	output_buffer[i] = 0;
}

if ( Terminated == true ) {
	goto LabelStop;
}

//成功
return(0);

//-----------------------------------
//読み書きエラー
LabelReadWriteError:

	//'保存する先のファイルに書き込めません。復号できません。'
	MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_OUT_FILE_WRITE)+"\n"+fsOut->FileName;
	MsgType = mtError;
	MsgButtons = TMsgDlgButtons() << mbOK;
	MsgDefaultButton = mbOK;
	Synchronize(&PostConfirmMessageForm);

	if ( fOpen == true ) {
		delete fsOut;
		fOpen = false;
	}

	return(-1);


//-----------------------------------
//エラー
LabelError:

	//ここのエラーは直前でメッセージを提示してから来ている

	if ( fOpen == true ) {
		delete fsOut;
		fOpen = false;
	}

	return(-1);

//-----------------------------------
//ユーザーキャンセル
LabelStop:

	if ( fOpen == true ) {
		delete fsOut;
		fOpen = false;
	}

	return(-2);


}
//===========================================================================
//ディスクの空き容量を調べる
//===========================================================================
__int64 __fastcall TAttacheCaseFileDecrypt1::GetDiskFreeSpaceNum(String FilePath)
{

int i;
int flag;

__int64 FreeSpace;

String DriveName = ExtractFileDrive(FilePath)+":";
String DirPath = IncludeTrailingPathDelimiter(ExtractFileDir(FilePath));

OSVERSIONINFO ovi;   // バージョン情報を格納する構造体
ovi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );

// バージョン情報取得
GetVersionEx( (LPOSVERSIONINFO)&ovi );

// Windows95 OSR2以前
if( ( ovi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
		&& ( int( ovi.dwBuildNumber & 0xffff ) <= 1000 ) ){

	DWORD SCluster,BSector,FCluster,TCluster;

	if ( GetDiskFreeSpaceW(String(DriveName).c_str(), &SCluster, &BSector, &FCluster, &TCluster) > 0){
		FreeSpace = SCluster*BSector*FCluster;
	}
	else{
		//ネットワークサーバ上は取得できずエラーとなる
		FreeSpace = -1;
	}

}
// OSR2以降～
else{

	ULARGE_INTEGER pqwFreeCaller;
	ULARGE_INTEGER pqwTot;
	ULARGE_INTEGER pqwFree;

	if(::GetDiskFreeSpaceExW(String(DirPath).c_str(), &pqwFreeCaller, &pqwTot, &pqwFree)){
		//64bit Integerで返す
		FreeSpace = pqwFreeCaller.QuadPart;
	}
	else{
		FreeSpace = -1;
	}

}

return(FreeSpace);

}//end GetDiskFreeSpaceNum;
//===========================================================================
//パスワードに文字列をセットする
//===========================================================================
void __fastcall TAttacheCaseFileDecrypt1::SetPasswordString(AnsiString PasswordText)
{

//#define PASS_FOOTER "_AttacheCase-M.Hibara"
KeyString = (AnsiString)PasswordText + PASS_FOOTER;

}
//===========================================================================
//メインフォームに確認メッセージを投げて処理を中断する
//===========================================================================
void __fastcall TAttacheCaseFileDecrypt1::PostConfirmMessageForm()
{

//グローバル変数(private)として定義してある

//String MsgText;
//TMsgDlgType MsgType;
//TMsgDlgButtons MsgButtons;
//TMsgDlgBtn MsgDefaultButton;
MsgReturnVal = Form1->ShowConfirmMassageForm(MsgText, MsgType, MsgButtons, MsgDefaultButton);


}
//===========================================================================
//メインフォームに上書きの確認メッセージを投げて処理を中断する
//===========================================================================
void __fastcall TAttacheCaseFileDecrypt1::PostConfirmOverwriteMessageForm()
{

//グローバル変数(private)として定義してある

//String MsgText;
//String MsgReturnPath;
MsgReturnVal = Form1->ShowConfirmOverwriteMassageForm(MsgText, MsgReturnPath);

}
//===========================================================================

