/*

'AttacheCase' - file encryption software for Windows.

TAttacheCaseFileDecrypt2 Class file.

Copyright (C) 2012 M.Hibara, All rights reserved.
http://hibara.org/

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see

http://www.gnu.org/licenses/

*/
//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "Unit1.h"
#include "TAttacheCaseFileDecrypt2.h"

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
__fastcall TAttacheCaseFileDecrypt2::TAttacheCaseFileDecrypt2
(bool CreateSuspended) : TThread(CreateSuspended)
{

for (int i = 0; i < 32; i++) {
	key[i] = 0;
}

fCompare = false;        // コンペア

ProgressPercentNum = -1; // 進捗パーセント
ProgressStatusText = ""; // 進捗ステータス内容

AtcFilePath = "";        // 入力する暗号化ファイル
OutDirPath = "";         // 出力するディレクトリ

NumOfTrials = 1;         // パスワード入力試行回数

StatusNum = -1;          // 処理結果ステータス
												 //   1: 復号成功
												 //   0: ユーザーキャンセル
												 //  -1: パスワード入力エラー
												 //  -2: 復号エラー

}
//===========================================================================
//デストラクタ
//===========================================================================
__fastcall TAttacheCaseFileDecrypt2::~TAttacheCaseFileDecrypt2(void)
{

//

}
//===========================================================================
//スレッド実行
//===========================================================================
void __fastcall TAttacheCaseFileDecrypt2::Execute()
{

int i, len;

// バッファ
char source_buffer[BUF_SIZE];
char temp_buffer[BUF_SIZE];
char chain_buffer[BUF_SIZE];
char output_buffer[LARGE_BUF_SIZE];
char *headerbuffer;

String FilePath, FileName;

// ファイルストリーム
TFileStream *fsIn;
TFileStream *fsOut;
bool fInputFileOpen = false;
bool fOutputFileOpen = false;

float free_space_required;
__int64 TotalSize, AllTotalSize;
__int64 CurrentDriveFreeSpaceSize;

int ret;	//バッファ出力の返値
int FileIndex = 0;

char token[16];
const char charTokenString[16] = "_AttacheCaseData";         //復号の正否に使う
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

// 平文ヘッダサイズ（データサブバージョン、破壊設定など）
int PlaneHeaderSize = 0;
// 暗号化部分のヘッダサイズ
int EncryptHeaderSize = 0;

int DataVersion;      // ver.2.00～は "5", ver.2.70～は "6"
int AlgorismType;

char reserved;        // 0
//int TypeLimits;     // ミスタイプ回数 0～10  （グローバル:public宣言とした）
//bool fDestroy;      // 破壊するか否か 0 or 1 （グローバル:public宣言とした）

String PrefixString;	//ファイルリストの接頭辞（Fn_*, U_*）

int flush, status;    // zlib
z_stream z;           // zlibライブラリとやりとりするための構造体

//ヘッダデータから必要情報を取り出すための
TMemoryStream *pms;   // メモリストリーム

int idx;
TStringList *DataList;
TStringList *tsv;

TStringList *FileList = new TStringList();  // 0: ファイル名
__int64 *FileSizeList = 0;               // 1: ファイルサイズ（フォルダは-1）
int *FileAttrList     = 0;               // 2: 属性
int *FileDtChangeList = 0;               // 3: 更新日
int *FileTmChangeList = 0;               // 4: 更新時
int *FileDtCreateList = 0;               // 5: 作成日
int *FileTmCreateList = 0;               // 6: 作成時

int rest;
int buf_size;

//-----------------------------------
// 平文のヘッダ内容チェック

try {
#ifdef EXE_OUT //自己実行形式（自身を開く）
	fsIn = new TFileStream(AtcFilePath, fmShareDenyNone);
#else
	fsIn = new TFileStream(AtcFilePath, fmOpenRead);
#endif
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

// 平文ヘッダサイズを取得
fsIn->Read(&PlaneHeaderSize, sizeof(int));
// トークンを取得
fsIn->Read(token, 16);

if (memcmp(token, charTokenString, 16) != 0 ) {

	//--------------------------------------------------------
	//実は自己実行形式ファイル？（拡張子偽装されている場合も）
	//--------------------------------------------------------
	// サイズを再取得
	fsIn->Seek(-(__int64)sizeof(__int64), TSeekOrigin::soEnd);
	fsIn->Read(&AllTotalSize, sizeof(__int64));
	// 位置を戻す
	fsIn->Seek(-(AllTotalSize + sizeof(__int64)), TSeekOrigin::soEnd);
	// もう一度、平文ヘッダサイズを読み込む
	fsIn->Read(&PlaneHeaderSize, sizeof(int));
	// もう一度、トークンを取得
	fsIn->Read(token, 16);

	// トークンを再チェック
	if (memcmp(token, charTokenString, 16) != 0 ) {
		if (token == "_Atc_Broken_Data") {
			//'復号するファイルを開けません。この暗号化ファイルは破壊されています。'
			MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_FILE_BROKEN);
			MsgType = mtError;
			MsgButtons = TMsgDlgButtons() << mbOK;
			MsgDefaultButton = mbOK;
			Synchronize(&PostConfirmMessageForm);
		}
		else {
			// '暗号化ファイルではありません。アタッシェケースによって暗号化されたファイルでないか、'+#13+
			// 'ファイルが壊れている可能性があります。'
			MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_FILE_NOT_ATC);
			MsgType = mtError;
			MsgButtons = TMsgDlgButtons() << mbOK;
			MsgDefaultButton = mbOK;
			Synchronize(&PostConfirmMessageForm);
		}
		goto LabelError;
	}

}

//-----------------------------------
// データバージョンチェック
//-----------------------------------
DataVersion = -1;
fsIn->Read(&DataVersion, sizeof(int));

if (DataVersion > ATC_DATA_FILE_VERSION && DataVersion < 200) {
	//'データバージョンがちがいます。復号できません。'+#13+
	//'ファイルは上位バージョンのアタッシェケースで暗号化されています。'+#13+
	//'最新版を入手して復号を試みてください。';
	MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_FILE_HIGHER_VERSION);
	MsgType = mtError;
	MsgButtons = TMsgDlgButtons() << mbOK;
	MsgDefaultButton = mbOK;
	Synchronize(&PostConfirmMessageForm);
	goto LabelError;
}
else if (DataVersion <= 103) {

	// Blowfishで暗号化されたファイル
	// 正式版では正常に復号されるが、ここではエラーとする

	//'データバージョンがちがいます。復号できません。'+#13+
	//'ファイルはver.1のアタッシェケースで暗号化されています。'+#13+
	//'古いバージョン（ver.2.75以前）か正式版を入手して復号を試みてください。';
	MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_FILE_LOWER_VERSION);
	MsgType = mtError;
	MsgButtons = TMsgDlgButtons() << mbOK;
	MsgDefaultButton = mbOK;
	Synchronize(&PostConfirmMessageForm);
	goto LabelError;

}
else {

	// 104 ～
	// Rijndaelで暗号化されている

	// アルゴリズムタイプ
	fsIn->Read(&AlgorismType, sizeof(int));
	// ヘッダサイズを読み込む
	fsIn->Read(&EncryptHeaderSize, sizeof(int));
	// int型からポインタキャストでchar配列を取り出す
	headerbuffer = (char*) & PlaneHeaderSize;
	// データサブバージョンチェック（ver.2.70～）
	if (headerbuffer[0] == 6) {
		TypeLimits = (int)headerbuffer[2];
		fDestroy = (bool)headerbuffer[3];
		// 有効範囲（1～10）かチェック
		if (TypeLimits < 1 || 10 < TypeLimits) {
			TypeLimits = 3; // デフォルト
		}
	}
	else { // headerbuffer[0] == 5            //旧バージョン
		TypeLimits = 3;
		fDestroy = false;
	}
}

//-----------------------------------
// 復号の準備
//-----------------------------------

// テーブル生成
gentables();

//パスワードセット
if (DataVersion == 105) {  //ver.2.75

	if ( old_key[0] != 0) {  //古い形式のパスワードファイルを使っている
		gkey(8, 8, old_key);
	}
	else{
		gkey(8, 8, key);
	}

}
else if( DataVersion == 106 ){
	gkey(8, 8, key);
}

//-----------------------------------
// 暗号部ヘッダの復号
//-----------------------------------

pms = new TMemoryStream;

// IVの読み込み
fsIn->Read(chain_buffer, BUF_SIZE);

len = 0;

while (len < EncryptHeaderSize) {

	// 読み出しバッファ
	for (i = 0; i < BUF_SIZE; i++) {
		source_buffer[i] = 0;
	}
	// 暗号化されたデータブロックの読み出し
	len += fsIn->Read(source_buffer, BUF_SIZE);

	for (i = 0; i < BUF_SIZE; i++) {
		// あとのxorのためによけておく
		temp_buffer[i] = source_buffer[i];
	}

	// 復号処理
	rijndael_decrypt(source_buffer);

	// xor
	for (i = 0; i < BUF_SIZE; i++) {
		source_buffer[i] ^= chain_buffer[i];
		chain_buffer[i] = temp_buffer[i]; // CBC
	}

	pms->Write(source_buffer, BUF_SIZE);


}

pms->Seek((__int64)0, TSeekOrigin::soBeginning);  //ポインタを先頭へ戻す
DataList = new TStringList;

DataList->LoadFromStream(pms);                                 // default encoding

//DataList->LoadFromStream(pms, TEncoding::UTF8);              // TEncoding::UTF8

//DataList->LoadFromStream(pms, TEncoding::GetEncoding(932));  // shift-jis

delete pms;

//-----------------------------------
// 復号正否（復号できたか）
//-----------------------------------
if (DataList->Count == 0 || DataList->Strings[0].Pos("AttacheCase") == 0) {

	//'パスワードがちがいます。復号できません。'+#13+
	//'場合によってはファイルが壊れている可能性もあります。';
	MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_PASSWORD_WRONG);
	if ( fCompare == true ) {
		//メッセージに'コンペアに失敗しました。'を追加
		MsgText += "\n" + LoadResourceString(&Msgdecrypt::_MSG_ERROR_COMPARE_FILE);
	}
	MsgType = mtError;
	MsgButtons = TMsgDlgButtons() << mbOK;
	MsgDefaultButton = mbOK;
	Synchronize(&PostConfirmMessageForm);
	delete DataList;
	goto LabelTypeMiss;

}

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
FileDtChangeList = new int[DataList->Count];  // 3: 更新日
FileTmChangeList = new int[DataList->Count];  // 4: 更新時
FileDtCreateList = new int[DataList->Count];  // 5: 作成日
FileTmCreateList = new int[DataList->Count];  // 6: 作成時

DataList->NameValueSeparator = ':';

PrefixString = "Fn_";
for (i = 0; i < DataList->Count; i++) {
	if ( DataList->IndexOfName("U_"+IntToStr(i)) > -1){
		PrefixString = "U_";	//新バージョン（ver.2.8.0～）で暗号化されているようだ
		break;
	}
}

tsv = new TStringList;
tsv->Delimiter = '\t';
tsv->StrictDelimiter = true;

AllTotalSize = 0;
for (i = 0; i < DataList->Count; i++) {

	idx = DataList->IndexOfName(PrefixString+IntToStr(i));

	if (idx > 0) {
		tsv->DelimitedText = DataList->ValueFromIndex[idx];
		FileList->Add(tsv->Strings[0]);
		FileSizeList[i] = StrToIntDef(tsv->Strings[1], -1);    // 1: ファイルサイズ（フォルダは-1）
		if (FileSizeList[i] > 0) {
			AllTotalSize += FileSizeList[i];
		}
		FileAttrList[i] = StrToIntDef(tsv->Strings[2], -1);    // 2: 属性
		FileDtChangeList[i] = StrToIntDef(tsv->Strings[3], -1);// 3: 更新日
		FileTmChangeList[i] = StrToIntDef(tsv->Strings[4], -1);// 4: 更新時
		FileDtCreateList[i] = StrToIntDef(tsv->Strings[5], -1);// 5: 作成日
		FileTmCreateList[i] = StrToIntDef(tsv->Strings[6], -1);// 6: 作成時
	}

}

delete tsv;
delete DataList;

//-----------------------------------
//ディスクの空き容量チェック
//-----------------------------------

CurrentDriveFreeSpaceSize = GetDiskFreeSpaceNum(OutDirPath);

if (CurrentDriveFreeSpaceSize > -1 && fCompare == false) {
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
if ( fTempOpenFile == true && FileList->Count > 4 && fCompare == false) {

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

// ファイルサイズを取得する
AllTotalSize = fsIn->Size - fsIn->Position;
// 初期化ベクトルの読み出し
TotalSize = fsIn->Read(chain_buffer, BUF_SIZE);

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

		// 入力バッファ
		for (i = 0; i < BUF_SIZE; i++) {
			source_buffer[i] = 0;
		}

		TotalSize =
			InputBuffer(
				len, source_buffer, chain_buffer,
				fsIn, fInputFileOpen,
				TotalSize, AllTotalSize
			);

		z.avail_in = len;
		z.next_in = source_buffer;

		if ( len < BUF_SIZE ) {
			status = Z_FINISH;
		}

	}

	//-----------------------------------
	// 展開
	//-----------------------------------
	status = inflate(&z, flush);

	if (status == Z_STREAM_END) {
		break;
	}

	//-----------------------------------
	// 処理ステータス
	//-----------------------------------
	if ( status == Z_OK ){
	}
	else if ( status == Z_BUF_ERROR ) { //入力が尽きた可能性
		//出力バッファをクリアにして継続させる
		len = LARGE_BUF_SIZE - z.avail_out;
		ret = OutputBuffer(output_buffer, len,
											 fsOut, fOutputFileOpen,
											 FileList, FileIndex,
											 FileSizeList, FileAttrList,
											 FileDtChangeList, FileTmChangeList,
											 FileDtCreateList, FileTmCreateList);
		if (ret == 0) {
			z.next_out = output_buffer;
			z.avail_out = LARGE_BUF_SIZE;
		}
		else if ( ret == 1) {
			break;
		}
		else{
			if ( ret == -2) {
				goto LabelStop;
			}
			else {
				goto LabelError;
			}
		}

	}
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

	// -----------------------------------
	// 出力
	// -----------------------------------
	if ( z.avail_out == 0 ) {

		ret = OutputBuffer(output_buffer, LARGE_BUF_SIZE,
											 fsOut, fOutputFileOpen,
											 FileList, FileIndex,
											 FileSizeList, FileAttrList,
											 FileDtChangeList, FileTmChangeList,
											 FileDtCreateList, FileTmCreateList);

		if ( ret < 0) {      //エラー
			if (ret == -2) {
				goto LabelStop;
			}
			else{
				goto LabelError;
			}
		}
		else{

			for (i = 0; i < LARGE_BUF_SIZE; i++) {
				output_buffer[i] = 0;
			}
			z.next_out = output_buffer;
			z.avail_out = LARGE_BUF_SIZE;
		}


	}//end if (z.avail_out == 0);

	//-----------------------------------
	//進捗状況表示
	ProgressPercentNum = ((float)TotalSize/AllTotalSize)*100;
	//'復号しています...'
	ProgressStatusText = LoadResourceString(&Msgdecrypt::_LABEL_STATUS_TITLE_DECRYPTING);
	ProgressMsgText = ExtractFileName(AtcFilePath);


}//while (!Terminated);

if (Terminated == true) {
	//ユーザーキャンセルで抜けてきた
	goto LabelStop;
}

//----------------------------------------------------------------------
// 万が一、出力バッファに余りがある場合
//----------------------------------------------------------------------
if (z.avail_out > 0) {

		len = LARGE_BUF_SIZE - z.avail_out;
		ret = OutputBuffer(output_buffer, len,
										 fsOut, fOutputFileOpen,
										 FileList, FileIndex,
										 FileSizeList, FileAttrList,
										 FileDtChangeList, FileTmChangeList,
										 FileDtCreateList, FileTmCreateList);
	if ( ret < 0 ) {
		if ( ret == -2 ){
			goto LabelStop;
		}
		else{
			goto LabelError;
		}
	}

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
delete [] FileDtChangeList;  // 3: 更新日
delete [] FileTmChangeList;  // 4: 更新時
delete [] FileDtCreateList;  // 5: 作成日
delete [] FileTmCreateList;  // 6: 作成時

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
	delete [] FileDtChangeList;
	delete [] FileTmChangeList;
	delete [] FileDtCreateList;
	delete [] FileTmCreateList;

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
		if ( fCompare == true ) {
			//メッセージに'コンペアに失敗しました。'を追加
			MsgText += "\n" + LoadResourceString(&Msgdecrypt::_MSG_ERROR_COMPARE_FILE);
		}
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
	delete [] FileDtChangeList;
	delete [] FileTmChangeList;
	delete [] FileDtCreateList;
	delete [] FileTmCreateList;

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
	delete [] FileDtChangeList;
	delete [] FileTmChangeList;
	delete [] FileDtCreateList;
	delete [] FileTmCreateList;

	StatusNum = 0;
	return;

}
//===========================================================================
// バッファの入力
//===========================================================================
__int64 __fastcall TAttacheCaseFileDecrypt2::InputBuffer
	(int &buff_size, char *source_buffer, char *chain_buffer,
	 TFileStream *fsIn, bool &fOpen,
	 __int64 TotalSize, __int64 AllTotalSize )
{

int i;

bool fPKCS;
int paddingNum;
char temp_buffer[BUF_SIZE];

// 読み出しバッファの初期化
for (i = 0; i < BUF_SIZE; i++) {
	source_buffer[i] = 0;
}

// 入力ファイルが開かれていない
if ( fOpen == false ) {
	buff_size = 0;
	return(TotalSize);
}

// 最後のブロックを越えている
if (TotalSize >= AllTotalSize) {
	buff_size = 0;
	return(TotalSize);
}

// 暗号化されたデータブロックの読み出し
TotalSize += fsIn->Read(source_buffer, BUF_SIZE);

for (i = 0; i < BUF_SIZE; i++) {
	// あとのxorのためによけておく
	temp_buffer[i] = source_buffer[i];
}

// 復号処理
rijndael_decrypt(source_buffer);

// xor
for (i = 0; i < BUF_SIZE; i++) {
	source_buffer[i] ^= chain_buffer[i];
	chain_buffer[i] = temp_buffer[i]; // CBC
}

// 最後のブロックの境界...
if (TotalSize >= AllTotalSize) {

	// Check PKCS #7 pading num
	fPKCS = false;
	paddingNum = source_buffer[BUF_SIZE - 1];
	for (i = 0; i < paddingNum; i++) {
		if (source_buffer[BUF_SIZE - 1 - i] == paddingNum) {
			fPKCS = true;
		}
		else {
			fPKCS = false;
			break;
		}
	}
	if (fPKCS == true) {
		buff_size = BUF_SIZE - paddingNum;
	}
	else {
		buff_size = BUF_SIZE;
	}

}
else {
	buff_size = BUF_SIZE;
}

return(TotalSize);

}
//===========================================================================
// バッファの出力
//===========================================================================
int __fastcall TAttacheCaseFileDecrypt2::OutputBuffer
	(char *output_buffer, int buff_size,
	TFileStream *&fsOut, bool &fOpen,
	TStringList *FileList, int &FileIndex,
	__int64 *FileSizeList, int *FileAttrList,
	int *FileDtChangeList, int *FileTmChangeList,
	int *FileDtCreateList, int *FileTmCreateList)
{

//返値　0=<:成功, -1:エラー, -2:ユーザーキャンセル

int i;
int res;

int rest;
String FileName, FilePath;
char read_buffer[LARGE_BUF_SIZE];      //コンペア用

// 解凍されたバッファがすべて書き込まれるまでループ
while (buff_size > 0 && !Terminated) {

	// ファイルが開かれていないならオープンする
	if ( fOpen == false ) {

		for (i = FileIndex; i < FileList->Count; i++) {

			FileName = FileList->Strings[i];
			FilePath = OutDirPath + FileName;

			//-----------------------------------
			// ディレクトリ
			//-----------------------------------
			if (FileName.IsPathDelimiter(FileList->Strings[i].Length()) == true) {

				//コンペア
				if (fCompare == true) {
					if (FileName == ExtractRelativePath(OutDirPath, CompareFileList->Strings[i])) {
						continue;
					}
					else{
						//'コンペアに失敗しました。'
						MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_COMPARE_FILE_MISMATCH)+"\n";
						//'次のファイルまたはフォルダの内容が異なるようです。'
						MsgText += LoadResourceString(&Msgdecrypt::_MSG_ERROR_COMPARE_FILE_MISMATCH)+"\n";
						// '暗号化:'+CompareFileList->Strings[i];
						MsgText += LoadResourceString(&Msgdecrypt::_MSG_ERROR_COMPARE_ENCRYPT_FILE)+"\n"+CompareFileList->Strings[i];
						// '復号  :'+FilePath;
						MsgText += LoadResourceString(&Msgdecrypt::_MSG_ERROR_COMPARE_DECRYPT_FILE)+"\n"+FilePath;
						MsgType = mtError;
						MsgButtons = TMsgDlgButtons() << mbOK;
						MsgDefaultButton = mbOK;
						Synchronize(&PostConfirmMessageForm);
						goto LabelError;
					}
				}


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
				if ( FilePath == AtcFilePath ) {
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
				FileSetTimeStamp(
					FilePath,
					FileDtChangeList[i], FileTmChangeList[i],
					FileDtCreateList[i], FileTmCreateList[i],
					false, true);
				//属性のセット
				FileSetAttr(FilePath, FileAttrList[i]);

				if ( fOpenFolder == true && fOpenFolderOnce == false ) {
					ShellExecuteW(NULL, L"open", L"EXPLORER.EXE", FilePath.c_str(), FilePath.c_str(), SW_NORMAL);  //脆弱性対策（ver.2.70）
					//ShellExecuteW(NULL, L"explore", FilePath.c_str(), NULL, NULL, SW_NORMAL);	//こちらでも正常動作したようだ
					fOpenFolderOnce = true;
				}

				continue;

			}
			//-----------------------------------
			// ファイル
			//-----------------------------------
			else {

				if ( fCompare == false && fConfirmOverwirte == true && fOverwirteYesToAll == false ) {

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
				if ( fCompare == false && FilePath == AtcFilePath ) {
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

					if (fCompare == true) {
						//コンペアは元ファイルを開く
						fsOut = new TFileStream(CompareFileList->Strings[i], fmOpenRead);
					}
					else{
						//出力するファイルを開く
						fsOut = new TFileStream(FilePath, fmCreate);
					}

				}
				catch(...) {

					//保存する先のファイルが開けません。他のアプリケーションが使用中の可能性があります。
					MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_OUT_FILE_OPEN)+"\n"+FilePath;
					if ( fCompare == true ) {
						//メッセージに'コンペアに失敗しました。'を追加
						MsgText += "\n" + LoadResourceString(&Msgdecrypt::_MSG_ERROR_COMPARE_FILE);
					}
					MsgType = mtError;
					MsgButtons = TMsgDlgButtons() << mbOK;
					MsgDefaultButton = mbOK;
					Synchronize(&PostConfirmMessageForm);
					goto LabelError;

				}

				fOpen = true;

				if (FileSizeList[i] == 0) { // 0バイトファイル

					delete fsOut;

					if (fCompare == true) {
						//コンペアの場合は抜ける
						continue;
					}

					fOpen = false;

					FileSetTimeStamp(FilePath,
						(int)FileDtChangeList[i], (int)FileTmChangeList[i],
						(int)FileDtCreateList[i], (int)FileTmCreateList[i],	false, true);

					FileSetAttr(FilePath, (int)FileAttrList[i]);

					//関連付けられたアプリケーションでファイルを開く
					if ( fTempOpenFile == true ) {
						ShellExecuteW(NULL, L"open", FilePath.c_str(), NULL, NULL, SW_NORMAL);
					}
				}
				else{
					break;
				}

			}

		} // end for;

		FileIndex = i;

	} // end if (fOpen == false);

	//-----------------------------------
	// 開いたファイルに書き込む
	//-----------------------------------
	if ( fOpen == true ) {

		//-----------------------------------
		// コンペア
		//-----------------------------------
		if ( fCompare == true ) {

			if (fsOut->Position + LARGE_BUF_SIZE < FileSizeList[FileIndex]) {

				if (fsOut->Read(read_buffer, LARGE_BUF_SIZE) != LARGE_BUF_SIZE) {
					//読み込みエラー
					goto LabelReadWriteError;
				}
				else{
					//データのコンペア
					if ( CompareMem(read_buffer, output_buffer, LARGE_BUF_SIZE) == false ){
						//'データの一致しないファイルが見つかりました。'
						MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_COMPARE_DATA_NOT_MATCH)+"\n"+fsOut->FileName;
						//'コンペアに失敗しました。';
						MsgText += LoadResourceString(&Msgdecrypt::_MSG_ERROR_COMPARE_FILE);
						MsgType = mtError;
						MsgButtons = TMsgDlgButtons() << mbOK;
						MsgDefaultButton = mbOK;
						Synchronize(&PostConfirmMessageForm);
						goto LabelError;
					}
				}

			}
			else{
				rest = FileSizeList[FileIndex] - fsOut->Position;
				if (fsOut->Read(read_buffer, rest) != rest) {
					//読み込みエラー
					goto LabelReadWriteError;
				}
				else {
					buff_size -= rest;
					delete fsOut;
					fOpen = false;

					FileSetTimeStamp(
						FilePath,
						FileDtChangeList[FileIndex], FileTmChangeList[FileIndex],
						FileDtCreateList[FileIndex], FileTmCreateList[FileIndex],
						false, true);
					FileSetAttr(FilePath, FileAttrList[FileIndex]);

					//関連付けられたアプリケーションでファイルを開く
					if ( fTempOpenFile == true ) {
						ShellExecuteW(NULL, L"open", FilePath.c_str(), NULL, NULL, SW_NORMAL);
					}

					FileIndex++;

				}//end if (fsOut->Write(output_buffer, rest) != rest);

			}

		}
		//-----------------------------------
		// 通常の復号処理
		//-----------------------------------
		else{

			if (fsOut->Position + LARGE_BUF_SIZE < FileSizeList[FileIndex]) {
				if (fsOut->Write(output_buffer, LARGE_BUF_SIZE) != LARGE_BUF_SIZE) {
					goto LabelReadWriteError;
				}
				else {
					buff_size -= LARGE_BUF_SIZE;
				}
			}
			else {
				rest = FileSizeList[FileIndex] - fsOut->Position;
				if (fsOut->Write(output_buffer, rest) != rest) {
					goto LabelReadWriteError;
				}
				else {
					buff_size -= rest;
					delete fsOut;
					fOpen = false;

					FileSetTimeStamp(
						FilePath,
						FileDtChangeList[FileIndex], FileTmChangeList[FileIndex],
						FileDtCreateList[FileIndex], FileTmCreateList[FileIndex],
						false, true);
					FileSetAttr(FilePath, FileAttrList[FileIndex]);

					//関連付けられたアプリケーションでファイルを開く
					if ( fTempOpenFile == true ) {
						ShellExecuteW(NULL, L"open", FilePath.c_str(), NULL, NULL, SW_NORMAL);
					}

					FileIndex++;

				}

			}

		}//end if ( fCompare == true );

	}
	else {
		//もう開くファイルがない
		break;

	}//end if ( fOpen == true );
	//-----------------------------------

}//end while ( buf_size < 0 );

if ( Terminated == true ) {
	goto LabelStop;
}

//成功
return(0);

//-----------------------------------
//読み書きエラー
LabelReadWriteError:

	if ( fCompare == true ) {
		//'コンペアを行う元のファイルを読み込むことができません。'
		MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_COMPARE_NOT_READ_SOURCE)+"\n"+fsOut->FileName;
		//'コンペアに失敗しました。';
		MsgText += LoadResourceString(&Msgdecrypt::_MSG_ERROR_COMPARE_FILE);
		MsgType = mtError;
		MsgButtons = TMsgDlgButtons() << mbOK;
		MsgDefaultButton = mbOK;
		Synchronize(&PostConfirmMessageForm);
	}
	else{
		//'保存する先のファイルに書き込めません。復号できません。'
		MsgText = LoadResourceString(&Msgdecrypt::_MSG_ERROR_OUT_FILE_WRITE)+"\n"+fsOut->FileName;
		MsgType = mtError;
		MsgButtons = TMsgDlgButtons() << mbOK;
		MsgDefaultButton = mbOK;
		Synchronize(&PostConfirmMessageForm);
	}

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
// ファイル/ディレクトリのタイムスタンプを設定する
//===========================================================================
void __fastcall TAttacheCaseFileDecrypt2::FileSetTimeStamp(String FilePath, // パス
	int tsChangeDateNum, // 更新日 （※Win98の場合はここにFileAgeが入る）
	int tsChangeTimeNum, // 更新時
	int tsCreateDateNum, // 作成日
	int tsCreateTimeNum, // 作成時
	bool fNow,           // trueなら他の引数に関係なく更新日時を現在に設定
	bool fDir            // ディレクトリ
		)
{

	HANDLE hFile;

	FILETIME ChangeFileTime, CreateFileTime;
	SYSTEMTIME sysChangeFileTime, sysCreateFileTime;

	TDateTime dtChangeDate, dtCreateDate;
	TTimeStamp tsChangeDate, tsCreateDate;

	// -----------------------------------
	// 2000, XP系

	// Win95/98→Win2000/XPへ復号するときに不正なタイムスタンプが
	// 渡ることがあるので、以下に例外処理を施す
	try {

		// TTimeStampへ代入
		tsChangeDate.Date = tsChangeDateNum; // 更新日時
		tsChangeDate.Time = tsChangeTimeNum;
		tsCreateDate.Date = tsCreateDateNum; // 作成日時
		tsCreateDate.Time = tsCreateTimeNum;

		// 現在日時を代入するなら
		if (fNow == true) {
			dtChangeDate = Now();
			dtCreateDate = Now();
		}
		else {
			// TimeStampからTDateTimeへ
			dtChangeDate = TimeStampToDateTime(tsChangeDate);
			dtCreateDate = TimeStampToDateTime(tsCreateDate);
		}

	}
	catch(Exception & e) {

		return; // タイムスタンプは現在日時に設定する

	}

	// 更新日時
	DateTimeToSystemTime(dtChangeDate, sysChangeFileTime);
	// TDateTime → SystemTime
	SystemTimeToFileTime(&sysChangeFileTime, &ChangeFileTime);
	// SystemTime → FILETIME
	// 作成日時
	DateTimeToSystemTime(dtCreateDate, sysCreateFileTime);
	// TDateTime → SystemTime
	SystemTimeToFileTime(&sysCreateFileTime, &CreateFileTime);
	// SystemTime → FILETIME

	// ローカルのファイル時刻を、世界協定時刻（UTC）に基づくファイル時刻へ変換する
	if (fNow == true) {
		LocalFileTimeToFileTime(&ChangeFileTime, &ChangeFileTime);
		LocalFileTimeToFileTime(&CreateFileTime, &CreateFileTime);
	}

	// -----------------------------------
	// ディレクトリ
	if (fDir == true) {
		// ディレクトリハンドルを得る（※NT/2000系のみ：FILE_FLAG_BACKUP_SEMANTICS）
		hFile = CreateFileW(FilePath.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	}
	// -----------------------------------
	// ファイルを開く（※「GENERIC_WRITE」にしておかないと変更できないよう）
	else {
		hFile = CreateFileW(FilePath.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	// -----------------------------------
	// ファイルがオープンされている
	if (hFile != INVALID_HANDLE_VALUE) {

		// 作成日が0になっているなら
		if (tsCreateDateNum == 0) {
			SetFileTime(hFile, NULL, NULL, &ChangeFileTime); // タイムスタンプのセット
		}
		else {
			SetFileTime(hFile, &CreateFileTime, NULL, &ChangeFileTime); // タイムスタンプのセット
		}

	}

	CloseHandle(hFile);

}
// end SetFileAgeStr;
//===========================================================================
//ディスクの空き容量を調べる
//===========================================================================
__int64 __fastcall TAttacheCaseFileDecrypt2::GetDiskFreeSpaceNum(String FilePath)
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
// 乱数データの生成
//===========================================================================
void TAttacheCaseFileDecrypt2::fillrand(char *buf, const int len)
{

static unsigned long count = 4;
static char          r[4];
int                  i;

// ISAAC ( Cryptographic Random Number Generator )
randctx ctx;

// init
randinit(&ctx, 1);

for(i = 0; i < len; ++i){
	if(count == 4){
		*(unsigned long*)r = rand(&ctx);
		count = 0;
	}
	buf[i] = r[count++];
}

}
//===========================================================================
//パスワード文字列をセットする
//===========================================================================
void __fastcall TAttacheCaseFileDecrypt2::SetPasswordString(AnsiString Password)
{

for ( int i = 0; i < 32; i++){
	key[i]=0;
}
StrCopy(key, Password.c_str());


}
//===========================================================================
//パスワードにバイナリ値をセットする（ver.2.80～）
//===========================================================================
void __fastcall TAttacheCaseFileDecrypt2::SetPasswordBinary(unsigned char *password)
{

for ( int i = 0; i < 32; i++){
	key[i]=0;
}
memcpy(key, password, 32);

}
//===========================================================================
//パスワード文字列からバイナリ値をセットする（下位互換）
//===========================================================================
void __fastcall TAttacheCaseFileDecrypt2::SetPasswordStringToBinary(AnsiString Password)
{

//つまりは先頭の32バイトしかpasswordに代入されない
for ( int i = 0; i < 32; i++){
	old_key[i]=0;
}
strcpy( old_key, Password.c_str() );

}
//===========================================================================
//メインフォームに確認メッセージを投げて処理を中断する
//===========================================================================
void __fastcall TAttacheCaseFileDecrypt2::PostConfirmMessageForm()
{

//グローバル変数(private)として定義してある

//String MsgText;
//TMsgDlgType MsgType;
//TMsgDlgButtons MsgButtons;
//TMsgDlgBtn MsgDefaultButton;

MsgReturnVal =
	Form1->ShowConfirmMassageForm(MsgText, MsgType, MsgButtons, MsgDefaultButton);


}
//===========================================================================
//メインフォームに上書きの確認メッセージを投げて処理を中断する
//===========================================================================
void __fastcall TAttacheCaseFileDecrypt2::PostConfirmOverwriteMessageForm()
{

//グローバル変数(private)として定義してある

//String MsgText;
//String MsgReturnPath;

MsgReturnVal = Form1->ShowConfirmOverwriteMassageForm(MsgText, MsgReturnPath);

}
//===========================================================================

