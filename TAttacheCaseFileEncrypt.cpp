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

#include <vcl.h>

#pragma hdrstop

#include "Unit1.h"
#include "TAttacheCaseFileEncrypt.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------

// 注意：異なるスレッドが所有する VCL のメソッド/関数/プロパティを別の
// レッド所有のオブジェクトに対しては Synchronize を使用できます。
//
//      Synchronize(&UpdateCaption);
//
// 例えば UpdateCaption を以下のように定義し
//
//      void __fastcall TAttacheCaseFileEncode::UpdateCaption()
//      {
//        Form1->Caption = "スレッドから書き換えました";
//      }
//---------------------------------------------------------------------------
__fastcall TAttacheCaseFileEncrypt::TAttacheCaseFileEncrypt
	(bool CreateSuspended) : TThread(CreateSuspended)
{

	int i;

	//オプション（デフォルト値）
	CompressRateNum = Z_DEFAULT_COMPRESSION; //圧縮率
	fOver4gbOk = true;                       //4GB超を許可
	fExeOutputOption = false;                //実行形式出力
	fOptBrokenFileOption = false;            //ミスタイプでファイルを破壊するか否か
	intOptMissTypeLimitsNumOption = 3;       //タイプミスできる回数
	fConfirmOverwirte = true;                //同名ファイルがあるときは上書きの確認をする
	fOverwirteYesToAll = false;              //同名ファイルはすべて上書きして暗号化する（ダイアログで「すべてはい」を選択 = true）

	ProgressPercentNum = -1;                 //進捗パーセント
	ProgressStatusText = "";                 //進捗ステータス
	ProgressMsgText = "";                    //進捗メッセージ

	AppExeFilePath = "";                     //アタッシェケース本体の場所

	OutFilePath = "";                        //出力する暗号化ファイル
	InputFileList = new TStringList;         //暗号化する元ファイルリスト
	FilePathList = new TStringList;          //ヘッダ生成時に展開されるファイルリスト

	for (i = 0; i < 32; i++) {
		key[i] = 0;
	}

	StatusNum = 0;                           //ステータス表示内容番号
	MsgErrorString = "";                     //エラーメッセージ


}
//===========================================================================
//デストラクタ
//===========================================================================
__fastcall TAttacheCaseFileEncrypt::~TAttacheCaseFileEncrypt(void)
{

delete InputFileList;
delete FilePathList;

}
//===========================================================================
// スレッド実行
//===========================================================================
void __fastcall TAttacheCaseFileEncrypt::Execute()
{

int i, c;
int res;

float ProgressPercentNumF;  //進捗パーセンテージ（浮動小数点）

z_stream z;                 // zlibライブラリとやりとりするための構造体
int flush, status;          // zlib

//出力する暗号化ファイルのタイムスタンプを元ファイルに合わせる
HANDLE hFile;
//_WIN32_FIND_DATAW first_fd;
ZeroMemory(&first_fd, sizeof(_WIN32_FIND_DATAW));

int len, pos;
int FileIndex;
String FilePath;

int HeaderSize;                          //ヘッダデータサイズ
__int64 CurrentDriveFreeSpaceSize = -1;  //保存するドライブの空き容量

//実行可能形式出力ファイルのデータサイズ
__int64 ExeAllSize = 0;
__int64 ExeSize    = 0;

//全体のファイルサイズ
AllTotalSize = 0;
__int64 TotalSize = 0;

//バッファ
char source_buffer[BUF_SIZE];
char read_buffer[BUF_SIZE];
char out_buffer[BUF_SIZE];
char chain_buffer[BUF_SIZE]; // IVなどを格納するチェインバッファ
char margin_buffer[BUF_SIZE];

//ファイルストリーム
TFileStream *fsIn;
TFileStream *fsOut;
TFileStream *fsExe;

//オープン中か
bool fOpenIn;
bool fOpenOut;
//メモリストリーム
TMemoryStream *pms = new TMemoryStream;

// マージンバッファサイズ
int MarginBufSize = MARGIN_BUF_SIZE;

// PKCS #7 Pading num.
unsigned char paddingNum = 0;

//---------------------------------------
// 同名ファイルがあるのでダイアログ表示
//---------------------------------------
if ( fConfirmOverwirte == true && fOverwirteYesToAll == false ) {

	if (FileExists(OutFilePath) == true) {
		//同名ファイルの上書き確認メッセージダイアログ
		MsgText = LoadResourceString(&Msgencrypt::_MSG_CONFIRM_OVER_WRITE_SAME_FILE)+"\n"+OutFilePath;
		Synchronize(&PostConfirmOverwriteMessageForm);
		if ( MsgReturnVal == mrYes ) {
			//上書きOKなのでFilePathはそのまま
		}
		else if ( MsgReturnVal == mrNo ) {
			//別名保存でFilePath文字列が書き換えられてきている
			OutFilePath = MsgReturnPath;
		}
		else if ( MsgReturnVal == mrYesToAll ) {
			//すべて上書き（YesToAll）
			fOverwirteYesToAll = true;
		}
		else if ( MsgReturnVal == mrCancel ) {
			//キャンセル
			delete pms;
			goto LabelStop;
		}
	}

}

//---------------------------------------
// ヘッダ情報の生成＆ファイル総サイズ取得
//---------------------------------------

//'暗号化するファイルリストの生成中...'
ProgressStatusText = LoadResourceString(&Msgencrypt::_LABEL_STATUS_TITLE_LISTING);

if ( CreateHeaderData( pms, InputFileList, FilePathList, AllTotalSize) == false ){
	if (Terminated == true) {
		//ユーザーキャンセルで抜けてきた
		delete pms;
		goto LabelStop;
	}
	else{
		//'暗号化するファイルを開けません。他のアプリケーションが使用中の可能性があります。'
		MsgText = LoadResourceString(&Msgencrypt::_MSG_ERROR_FILE_OPEN);
		MsgType = mtError;
		MsgButtons = TMsgDlgButtons() << mbOK;
		MsgDefaultButton = mbOK;
		Synchronize(&PostConfirmMessageForm);
		delete pms;
		goto LabelError;
	}

}

//-----------------------------------
// ディスクの空き容量チェック
//-----------------------------------

CurrentDriveFreeSpaceSize = GetDiskFreeSpaceNum(OutFilePath);

if (CurrentDriveFreeSpaceSize > -1 ) {
	if ( AllTotalSize > CurrentDriveFreeSpaceSize ) {
		//"ディスクの空き容量が足りません！　暗号化ファイルを保存できません。\n
		//暗号化を中止します。;"
		MsgText = LoadResourceString(&Msgencrypt::_MSG_ERROR_NO_DISK_FREE_SPACE);
		MsgType = mtError;
		MsgButtons = TMsgDlgButtons() << mbOK;
		MsgDefaultButton = mbOK;
		Synchronize(&PostConfirmMessageForm);
		delete pms;
		goto LabelError;
	}
}
else{
	// -1はネットワークドライブの可能性があるので無視
	//(万が一、別のエラーの場合、実際書き込みに移行したときエラーが発生する)
}

//-----------------------------------
// 実行可能形式でかつ
// 合計バイト数が4GBを越えたときのエラー
//-----------------------------------
if ( fExeOutputOption == true && fOver4gbOk == false && AllTotalSize > SIZE_4GB ){

	//実行形式ファイルのサイズが4GBを超えてしまう可能性があります！\n
	//Win32アプリケーションとして実行できなくなるかもしれませんがよろしいですか？';
	MsgText = LoadResourceString(&Msgencrypt::_MSG_ERROR_OVER_4GB_EXE);
	MsgType = mtError;
	MsgButtons = TMsgDlgButtons() << mbYes << mbNo;
	MsgDefaultButton = mbNo;
	Synchronize(&PostConfirmMessageForm);

	if ( MsgReturnVal == mbNo) {
		//キャンセル
		delete pms;
		goto LabelStop;
	}

}

//-----------------------------------
// 暗号化ファイルの生成開始
//-----------------------------------
//'暗号化しています...'
ProgressStatusText = LoadResourceString(&Msgencrypt::_LABEL_STATUS_TITLE_ENCRYPTING);
ProgressMsgText = ExtractFileName(OutFilePath);

TotalSize = 0;

try{
	fsOut = new TFileStream(OutFilePath, fmCreate);
	fOpenOut = true;
}
catch(...){
	//'保存する先のファイルが開けません。他のアプリケーションが使用中の可能性があります。'
	MsgText = LoadResourceString(&Msgencrypt::_MSG_ERROR_OUT_FILE_OPEN) + "\n" + OutFilePath;
	MsgType = mtError;
	MsgButtons = TMsgDlgButtons() << mbOK;
	MsgDefaultButton = mbOK;
	Synchronize(&PostConfirmMessageForm);
	delete pms;
	goto LabelError;
}

//-----------------------------------
// 実行可能形式の出力
//-----------------------------------
if ( fExeOutputOption == true ){

	//-----------------------------------
	// 自分のお尻から実行データを抽出
	//-----------------------------------

	//自分自身の実行ファイルを開く
	try{
		fsExe = new TFileStream(Application->ExeName, fmShareDenyNone);
	}
	catch(...){
		//'実行可能形式出力に失敗しました。暗号化処理を中止します。'
		MsgText = LoadResourceString(&Msgencrypt::_MSG_ERROR_EXEOUT_FAILED);
		MsgType = mtError;
		MsgButtons = TMsgDlgButtons() << mbOK;
		MsgDefaultButton = mbOK;
		Synchronize(&PostConfirmMessageForm);
		delete pms;
		goto LabelError;
	}

	//切り出すサイズを取得
	fsExe->Seek(-(__int64)sizeof(__int64), TSeekOrigin::soEnd);
	fsExe->Read(&ExeAllSize, sizeof(__int64));

	//処理する合計サイズに実行形式分を加える
	AllTotalSize += ExeAllSize;

	//自己実行可能形式データの境界へ
	fsExe->Seek(-(__int64)ExeAllSize-sizeof(__int64), TSeekOrigin::soEnd);

	while(fsExe->Read(read_buffer, BUF_SIZE) != 0 ){
		ExeSize+=BUF_SIZE;
		//書き込む
		if ( ExeSize < ExeAllSize ){
			fsOut->Write(read_buffer, BUF_SIZE);
			TotalSize += BUF_SIZE;
		}
		else{
			fsOut->Write(read_buffer, ExeSize-ExeAllSize);
			TotalSize += (ExeSize-ExeAllSize);
		}
		//進捗表示
		ProgressPercentNumF = (float)TotalSize/AllTotalSize;
		ProgressPercentNum = (int)(ProgressPercentNumF*100);
		if (AllTotalSize < 104857600) {	// 100MB未満
			ProgressPercentNumText = IntToStr(ProgressPercentNum)+"%";
		}
		else{
			ProgressPercentNumText = FloatToStrF(ProgressPercentNumF*100, ffNumber, 4, 1)+"%";
		}
	}
	//自分自身を閉じる
	delete fsExe;
}

//-----------------------------------
// ヘッダ情報の描き込み
//-----------------------------------

pms->SaveToStream(fsOut);	//fsOutに追記
delete pms;


//-----------------------------------
// Rijndaelの初期化
//-----------------------------------

gentables();

gkey( 8, 8, key);

// 初期化ベクトルを生成して先頭に書き込む
fillrand(chain_buffer, BUF_SIZE);

if ( fsOut->Write(chain_buffer, BUF_SIZE) < BUF_SIZE ){
	//''保存先に指定された暗号化ファイルに書き込めません。
	MsgText = LoadResourceString(&Msgencrypt::_MSG_ERROR_OUT_FILE_WRITE) + "\n" + OutFilePath;
	MsgType = mtError;
	MsgButtons = TMsgDlgButtons() << mbOK;
	MsgDefaultButton = mbOK;
	Synchronize(&PostConfirmMessageForm);
	goto LabelError;
}

//-----------------------------------
// zlib 初期化（圧縮においてすべてのメモリ管理をライブラリに任せる）
z.zalloc = Z_NULL;
z.zfree  = Z_NULL;
z.opaque = Z_NULL;
//z.next_in = Z_NULL;

// 第2引数は圧縮の度合。0～9 の範囲の整数で，0 は無圧縮
// Z_DEFAULT_COMPRESSION (= 6) が標準

if (deflateInit(&z, CompressRateNum) != Z_OK){
	//zlibエラー表示はラベル先で
	goto LabelError;
}

//出力バッファの初期化
for(i = 0; i < BUF_SIZE; i++){
	out_buffer[i] = 0;
}

// zlibに入出力バッファをセットする
z.avail_in  = 0;                    // 入力バッファ中のデータのバイト数
z.next_out  = out_buffer;           // 出力バッファ残量
z.avail_out = BUF_SIZE;             // 出力ポインタ

// 通常は deflate() の第2引数は Z_NO_FLUSH にして呼び出す
flush = Z_NO_FLUSH;

FileIndex = 0;

while(!Terminated) {

	//-----------------------------------
	//入力
	//-----------------------------------
	if ( z.avail_in == 0 && flush != Z_FINISH){

		pos = 0;

		for(i = 0; i < BUF_SIZE; i++){
			source_buffer[i] = 0;
			read_buffer[i] = 0;
		}

		while ( pos < BUF_SIZE ){

			//オープン中のファイルがあればそこから読む
			if ( fOpenIn == true ) {

				if (pos < BUF_SIZE) {

					len = fsIn->Read(read_buffer, BUF_SIZE - pos);
					TotalSize+=len;

					for (i = 0; i < len; i++) {
						source_buffer[pos+i] = read_buffer[i];
					}

					if (len < BUF_SIZE - pos) {
						fOpenIn = false; //ファイルを閉じる
						delete fsIn;
					}

				}

				pos += len;

			}
			//ファイルを開く
			else{
				if (FileIndex < FilePathList->Count) {
					while(FileIndex < FilePathList->Count){
						if (FilePathList->Strings[FileIndex] != "") {
							try{
								fsIn = new TFileStream(FilePathList->Strings[FileIndex], fmOpenRead);
								fOpenIn = true;
								FileIndex++;
								break;
							}
							catch(...){
								//'暗号化するファイルを開けません。他のアプリケーションが使用中の可能性があります。'
								MsgText = LoadResourceString(&Msgencrypt::_MSG_ERROR_FILE_OPEN);
								MsgType = mtError;
								MsgButtons = TMsgDlgButtons() << mbOK;
								MsgDefaultButton = mbOK;
								Synchronize(&PostConfirmMessageForm);
								fOpenIn = false;
								goto LabelError;
							}
						}
						FileIndex++;
					}
				}
				else{

					//読み込むファイルがなくなったので、
					//お尻にダミーのマージンデータを挿入する
					//
					//【補足】
					// 本来はここにあるマージンデータ挿入処理は不要ですが、
					// 昔に作った際に復号の際に圧縮データ境界のチェックを
					// 怠っていたため、このように余分なデータを
					// 入れておくという力業を使っています（すみません...）
					fillrand(margin_buffer, BUF_SIZE);

					for (i = pos; i < BUF_SIZE; i++) {
						source_buffer[i] = margin_buffer[i];
					}

					pos = BUF_SIZE;
					MarginBufSize -= BUF_SIZE;

				}//end if (FileIndex < FilePathList->Count);

			}//end if ( fOpenIn == true );

		}//while ( pos < BUF_SIZE && 0 < MarginBufSize );

		if (MarginBufSize < 1) {
			flush = Z_FINISH;	//入力バッファはこれが最後
		}

		z.next_in = source_buffer;
		z.avail_in = pos;

	}//end if ( z.avail_in == 0 );

	//-----------------------------------
	//圧縮
	//-----------------------------------
	if ( z.avail_out > 0 ){
		status = deflate(&z, flush);
	}
	if (status == Z_STREAM_END){
			break;
	}
	if (status != Z_OK ){
		//#define Z_OK              0
		//#define Z_STREAM_END      1
		//#define Z_NEED_DICT       2
		//#define Z_ERRNO         (-1)
		//#define Z_STREAM_ERROR  (-2)
		//#define Z_DATA_ERROR    (-3)
		//#define Z_MEM_ERROR     (-4)
		//#define Z_BUF_ERROR     (-5)
		//#define Z_VERSION_ERROR (-6)
		goto LabelError;
	}
	//-----------------------------------
	//出力
	//-----------------------------------
	if ( z.avail_out == 0 ){

		// CBC - xor the file bytes with the IV bytes
		for(i = 0; i < BUF_SIZE; i++){
			out_buffer[i] ^= chain_buffer[i];
		}

		//encrypt!
		rijndael_encrypt(out_buffer);

		len = fsOut->Write(out_buffer, BUF_SIZE);

		if (len < BUF_SIZE) {
			//'保存先に指定された暗号化ファイルに書き込めません。
			MsgText = LoadResourceString(&Msgencrypt::_MSG_ERROR_OUT_FILE_WRITE) + "\n" + OutFilePath;
			MsgType = mtError;
			MsgButtons = TMsgDlgButtons() << mbOK;
			MsgDefaultButton = mbOK;
			Synchronize(&PostConfirmMessageForm);
			goto LabelError;
		}

		for(i = 0; i < BUF_SIZE; i++){
			chain_buffer[i] = out_buffer[i];
			out_buffer[i] = 0;
		}

		z.next_out = out_buffer;    // 出力バッファ残量を元に戻す
		z.avail_out = BUF_SIZE;     // 出力ポインタを元に戻す

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

	if ( fOpenIn == true ){
		ProgressMsgText = ExtractFileName(fsIn->FileName);
	}
	else{
		ProgressMsgText = ExtractFileName(OutFilePath);
	}


}//while(!Terminated);

if (Terminated == true) {
	//ユーザーキャンセルで抜けてきた
	goto LabelStop;
}

//残りのバッファ
if (z.avail_out > 0) {

		// PKCS #7 パディング
		len = BUF_SIZE - z.avail_out;

		paddingNum = (char)z.avail_out;
		for(i = len; i < BUF_SIZE; i++){
			out_buffer[i] = paddingNum;
		}

		// CBC - xor the file bytes with the IV bytes
		for(i = 0; i < BUF_SIZE; i++){
			out_buffer[i] ^= chain_buffer[i];
		}

		//encrypt!
		rijndael_encrypt(out_buffer);

		if ((len = fsOut->Write(out_buffer, BUF_SIZE)) != BUF_SIZE){
			//'保存先に指定された暗号化ファイルに書き込めません。
			MsgText = LoadResourceString(&Msgencrypt::_MSG_ERROR_OUT_FILE_WRITE) + "\n" + OutFilePath;
			MsgType = mtError;
			MsgButtons = TMsgDlgButtons() << mbOK;
			MsgDefaultButton = mbOK;
			Synchronize(&PostConfirmMessageForm);
			goto LabelError;
		}

}

if (deflateEnd(&z) != Z_OK){
	//zlibエラー
	goto LabelError;
}


//-----------------------------------
// 実行可能形式ファイルは
// 末尾へ暗号化データサイズを書き込む
//-----------------------------------
if ( fExeOutputOption == true ){
	ExeSize = fsOut->Seek((__int64)0, TSeekOrigin::soEnd);
	ExeSize = ExeSize-ExeAllSize;
	fsOut->Write(&ExeSize, sizeof(__int64));
}

//-----------------------------------
// 完了
//-----------------------------------
ProgressPercentNum = 100;
//'完了'
ProgressStatusText = LoadResourceString(&Msgencrypt::_LABEL_STATUS_TITLE_COMPLETE);
ProgressMsgText = ExtractFileName(OutFilePath);

if (fOpenIn == true) {
	delete fsIn;
}
if (fOpenOut == true) {
	delete fsOut;
}

//出力する暗号化ファイルのタイムスタンプを元ファイルに合わせる
if ( fKeepTimeStamp == true && first_fd.cFileName[0] != NULL ) {

	hFile = CreateFileW(FilePath.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE) {
		SetFileTime( hFile, &first_fd.ftCreationTime, &first_fd.ftLastAccessTime, &first_fd.ftLastWriteTime);
		CloseHandle(hFile);
	}
}

StatusNum = 1;
return;

//-----------------------------------
// エラーの後始末
//-----------------------------------
LabelError:

	ProgressPercentNum = 0;

	if ( status < 0 ){
		//'zlibライブラリからエラーを返されました。'
		//'エラー番号：'
		MsgText = LoadResourceString(&Msgencrypt::_MSG_ERROR_ZLIB) + IntToStr(status) + "\n" + z.msg;
		MsgType = mtError;
		MsgButtons = TMsgDlgButtons() << mbOK;
		MsgDefaultButton = mbOK;
		Synchronize(&PostConfirmMessageForm);
	}

	//'エラー'
	ProgressStatusText = LoadResourceString(&Msgencrypt::_LABEL_STATUS_TITLE_ERROR);
	//'暗号化に失敗しました。'
	ProgressMsgText = LoadResourceString(&Msgencrypt::_LABEL_STATUS_DETAIL_FAILED);

	if (fOpenIn == true) {
		delete fsIn;
	}
	if (fOpenOut == true) {
		delete fsOut;
	}

	StatusNum = -1;

	return;


//-----------------------------------
// ユーザーキャンセルの後始末
//-----------------------------------
LabelStop:

	ProgressPercentNum = 0;
	//'キャンセル'
	ProgressStatusText = LoadResourceString(&Msgencrypt::_LABEL_STATUS_TITLE_USER_CANCEL);
	//'暗号化が中止されました。'
	ProgressMsgText = LoadResourceString(&Msgencrypt::_LABEL_STATUS_DETAIL_STOPPED);

	if (fOpenIn == true) {
		delete fsIn;
	}
	if (fOpenOut == true) {
		delete fsOut;
	}

	StatusNum = -2;

	return;

}
//---------------------------------------------------------------------------
// 初期化ベクトル（IV）の生成
//---------------------------------------------------------------------------
void TAttacheCaseFileEncrypt::fillrand(char *buf, const int len)
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
// ヘッダ情報を生成する
//===========================================================================
bool __fastcall TAttacheCaseFileEncrypt::CreateHeaderData
		 ( TMemoryStream *pms,
			 TStringList   *FileList,
			 TStringList   *FilePathList,
			 __int64       &AllTotalFileSize
		 )
{

int i, c;

int ret;
int Index = 0;
int HeaderSizeAddress = 0;
TSearchRec sr;
String OneLine;
String DirPath, FileName;

String MsgText;

//暗号部トークン
const AnsiString Passcode_AttacheCase = "Passcode:AttacheCase\n";
//暗号化ファイルの作成日
AnsiString LastDateTimeString = "LastDateTime:" + DateTimeToStr(Now()) + "\n";

//旧ヘッダーテキストすべて
AnsiString Fn_HeaderText;
//Unicode用ヘッダーテキストすべて
String U_HeaderText;

int EncryptHeaderSize = 0;  //暗号部ヘッダサイズ

char buffer[BUF_SIZE];
char chain_buffer[BUF_SIZE];

TStringList *HeaderDataList;
TMemoryStream* tpms;          //テンポラリメモリストリーム

//-----------------------------------
// ヘッダ情報（平文）
//-----------------------------------
const char charReservedValue[4] = { 0, 0, 0, 0 };

const char charDataSubVersion = ATC_DATA_SUB_VERSION;
const char charOptMissTypeLimitsNumOption = intOptMissTypeLimitsNumOption;
const char charOptBrokenFileOption = (fOptBrokenFileOption > 0 ? 1 : 0);
const char charTokenString[16] = "_AttacheCaseData";
const int DataFileVersion = ATC_DATA_FILE_VERSION;
const int AlgorismType = TYPE_ALGORISM_RIJNDAEL;

//データサブバージョン                              : 1byte
pms->Write(&charDataSubVersion, sizeof(char));
//予約データ(reserved）                             : 1byte
pms->Write(&charReservedValue, sizeof(char));
//ミスタイプ回数                                    : 1byte
pms->Write(&charOptMissTypeLimitsNumOption, sizeof(char));
//破壊するか否か                                    : 1byte
pms->Write(&charOptBrokenFileOption, sizeof(char));
//トークン                                          : 16byte
pms->Write(&charTokenString, 16);
//データファイルバージョン                          : 4byte
pms->Write(&DataFileVersion, sizeof(int));
//アルゴリズムタイプ                                : 4byte
pms->Write(&AlgorismType, sizeof(int));
//暗号化部分のヘッダデータサイズ（先に確保しておく）：4byte
HeaderSizeAddress = pms->Position;
pms->Write(&EncryptHeaderSize, sizeof(int));

//-----------------------------------
// ヘッダ情報（暗号化部分）
//-----------------------------------

//進捗状況表示
ProgressPercentNum = -1;
//'暗号化するファイルリストの生成中...'
ProgressStatusText = LoadResourceString(&Msgencrypt::_LABEL_STATUS_TITLE_LISTING);

//ヘッダデータリスト（文字列）
HeaderDataList = new TStringList;
//パスワード
HeaderDataList->Add(Passcode_AttacheCase);
//作成日
HeaderDataList->Add(LastDateTimeString);

for ( i = 0; i < FileList->Count; i++ ){
	//ファイル
	if (FileExists(FileList->Strings[i]) == true) {
		DirPath = ExtractFileDir(FileList->Strings[i]);
		FileName = ExtractFileName(FileList->Strings[i]);
		ProgressMsgText = FileName;      //処理中のファイル名
		AllTotalFileSize +=
			GetFileInfoList(Index, DirPath, FileName, FileList->Strings[i], FilePathList, HeaderDataList);
	}
	//ディレクトリ
	else{
		DirPath = ExtractFileDir(FileList->Strings[i]);
		FileName = ExtractFileName(FileList->Strings[i]);
		ProgressMsgText = FileName;      //処理中のファイル名
		//トップディレクトリ
		GetFileInfoList(Index, DirPath, FileName, FileList->Strings[i], FilePathList, HeaderDataList);
		//その配下
		AllTotalFileSize +=
			GetFileInfoList(Index, FileList->Strings[i], "", FileList->Strings[i], FilePathList, HeaderDataList);
	}

	//ユーザーキャンセル
	if (Terminated == true) {
		delete HeaderDataList;
		return(false);
	}

}// end for;

//進捗状況表示
ProgressPercentNum = -1;
//'ヘッダデータを書き込んでいます...'
ProgressStatusText = LoadResourceString(&Msgencrypt::_LABEL_STATUS_TITLE_ENCRYPTING_LIST);
ProgressMsgText = "";

//メモリストリームへ書き込み
tpms = new TMemoryStream;

//------------------------------------------------
// TODO: 暗号化時にヘッダデータの互換性維持
// 旧バージョンで暗号化されたファイルの下位互換性と、
// 新バージョンによるUnicodeファイル名に対応するため、
// 以下のようにファイルリスト情報を二重持ちしています。
// 他に良い方法があれば作者までご提案いただけると
// うれしいです。
//---------------------------------------------------
HeaderDataList->SaveToStream(tpms, TEncoding::GetEncoding(932));
//新バージョン（ver.2.8.0～）用（UTF-8）に保存
for (i = 0; i < HeaderDataList->Count; i++) {
	HeaderDataList->Strings[i] = StringReplace(HeaderDataList->Strings[i],"Fn_","U_",TReplaceFlags()<<rfIgnoreCase );
}
HeaderDataList->SaveToStream(tpms, TEncoding::UTF8);

delete HeaderDataList;

//-----------------------------------
//ヘッダ情報の暗号化
//-----------------------------------

//暗号化の準備
gentables();
//キー入力
gkey( 8, 8, key);

for (i = 0; i < BUF_SIZE; i++) {
	buffer[i] = 0;
}

//初期化ベクトル（IV）を生成
fillrand(chain_buffer, BUF_SIZE);
pms->Write(chain_buffer, BUF_SIZE);

//先頭にポインタを戻す
tpms->Seek((__int64)0, TSeekOrigin::soBeginning);
EncryptHeaderSize = 0;

//CBCモードで書き込む
while (tpms->Read( buffer, BUF_SIZE ) != NULL){

	EncryptHeaderSize += BUF_SIZE;

	// xor
	for ( i = 0; i < BUF_SIZE; i++ ){
		buffer[i] ^= chain_buffer[i];
	}

	// rijndael
	rijndael_encrypt(buffer);

	pms->Write(buffer, BUF_SIZE);

	//CBC＆バッファの初期化
	for ( i = 0; i < BUF_SIZE; i++ ){
		chain_buffer[i] = buffer[i];
		buffer[i] = 0;
	}

	//ユーザーキャンセル
	if (Terminated == true) {
		delete tpms;
		return(false);
	}

}//loop;

delete tpms;

//暗号化部分のヘッダデータサイズ（確保しておいた場所へ改めて書き込む）
pms->Position = HeaderSizeAddress;
pms->Write(&EncryptHeaderSize, sizeof(int));
//先頭にポインタを戻す
pms->Seek((__int64)0, TSeekOrigin::soBeginning);

return(true);


}//end CreateHeaderData;
//===========================================================================
// 暗号化するファイルリストとファイル情報のリストを同時生成する
//===========================================================================
__int64 __fastcall TAttacheCaseFileEncrypt::GetFileInfoList
	( int &Index,
		String DirPath,
		String FileName,
		String BasePath,
		TStringList *FileList,
		TStringList *DataList
	)
{

/* ------------------------------------------------
 ファイルリストのファイル番号の頭に「Fn_*:」と
 なぜ重複するような記号が挿入されているかと言いますと
 あまり意味はございません・・・お恥ずかしいかぎり・・・
 すみません、これもver.1からの仕様を引きずっているのと、
 習作時代にやっちゃった無意味なデータ仕様の一つです。
--------------------------------------------------- */

int ret;
__int64 TotalSize = 0;

bool fParent = false;
String OneLine;
String FilePath;
String FileNameString;

TSearchRec sr;
//_WIN32_FIND_DATAW fd;

DirPath = IncludeTrailingPathDelimiter(DirPath);

if (FileName == "") {  //ディレクトリ
	FileName = "*.*";
}
else{
	fParent = true;
}

ret = FindFirst(DirPath + FileName, faAnyFile, sr);

while (ret == 0) {

	if (sr.Name != "." && sr.Name != "..") {

		FilePath = DirPath + sr.Name;
		FileNameString = ExtractRelativePath(BasePath, FilePath);

		//-----------------------------------
		//ディレクトリ
		if (sr.Attr & faDirectory) {

			// Win95/98系（非対応だが一応）
			if ( Win32Platform == VER_PLATFORM_WIN32_WINDOWS ){
				OneLine =
					"Fn_" + IntToStr((int)Index) + ":" +                 //インデックス
					FileNameString + "\\\t" +                            //ディレクトリ名
					"*\t16\t0\t0\t0\t0";                                 //残りは0
			}
			else{
				// _WIN32_FIND_DATAW 構造体
				//fd = sr.FindData;
				OneLine =
					"Fn_" + IntToStr((int)Index) + ":" +                 //インデックス
					FileNameString + "\\\t" +                            //ディレクトリの相対パス
					"*\t" +                                              //ファイルサイズ(=0)
					IntToStr(sr.Attr) + "\t" +                           //属性
					TimeStampToString(sr.FindData.ftLastWriteTime)+"\t"+ //更新日時
					TimeStampToString(sr.FindData.ftCreationTime);       //作成日時

					//出力する暗号化ファイルのタイムスタンプを元ファイルに合わせるため保持
					if ( fKeepTimeStamp == true && first_fd.cFileName[0] == NULL ) {
						first_fd = sr.FindData;
					}

			}

			FileList->Add("");      //ディレクトリは空行
			DataList->Add(OneLine);
			Index++;

			if (fParent == false) {
				//再帰呼び出し
				TotalSize += GetFileInfoList(Index, FilePath, "", BasePath, FileList, DataList);
			}

		}
		//-----------------------------------
		//ファイル
		else{
			OneLine =
				"Fn_" + IntToStr((int)Index) + ":" +                   //インデックス
				FileNameString + "\t" +                                //ファイルの相対パス
				IntToStr(sr.Size) + "\t" +                             //ファイルサイズ
				IntToStr(sr.Attr) + "\t" +                             //属性
				TimeStampToString(sr.FindData.ftLastWriteTime)+"\t"+   //更新日時
				TimeStampToString(sr.FindData.ftCreationTime);         //作成日時

			//出力する暗号化ファイルのタイムスタンプを元ファイルに合わせるため保持
			if ( fKeepTimeStamp == true && first_fd.cFileName[0] == NULL ) {
				first_fd = sr.FindData;
			}

			if (sr.Size > 0) {
				FileList->Add(FilePath);
			}
			else{
				FileList->Add("");      //空ファイルは加えない
			}

			DataList->Add(OneLine);
			Index++;

			//ファイル総計
			TotalSize += sr.Size;

		}
		//-----------------------------------

	}//end if;

	ret = FindNext(sr);

}//while;

FindClose(sr);

return(TotalSize);


}//end GetFileInfoList;
//===========================================================================
//FILETIME構造体をTTimeStampに変換して文字列として取得する
//===========================================================================
String __fastcall TAttacheCaseFileEncrypt::TimeStampToString(FILETIME ft)
{

SYSTEMTIME st;
TDateTime dt;
TTimeStamp ts;

//FileTime → SystemFileTime
FileTimeToSystemTime(&ft, &st);
//SystemTime → TDateTime
dt = SystemTimeToDateTime(st);
//TDateTime → TimeStamp
ts = DateTimeToTimeStamp(dt);

/*
//ファイル/ディレクトリの日時をなぜ
//わざわざサイズが大きいTTimeStampに変換して格納したのか？
//ごめんなさい、意味はありません(笑)。

struct TTimeStamp
{
	int Time;
	int Date;
};
*/

return(IntToStr(ts.Date)+"\t"+IntToStr(ts.Time));

}
//===========================================================================
//ディスクの空き容量を調べる
//===========================================================================
__int64 __fastcall TAttacheCaseFileEncrypt::GetDiskFreeSpaceNum(String FilePath)
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
//パスワード文字列をセットする
//===========================================================================
/*
void __fastcall TAttacheCaseFileEncrypt::SetPasswordString(AnsiString Password)
{

for (int i = 0; i < 32; i++) {
	key[i] = 0;
}

StrCopy(key, Password.c_str());

}
*/
//===========================================================================
//パスワードにバイナリ値をセットする
//===========================================================================
void __fastcall TAttacheCaseFileEncrypt::SetPasswordBinary(char *password)
{

for (int i = 0; i < 32; i++) {
	key[i] = 0;
}

memcpy(key, password, 32);

}
//===========================================================================
//現在設定されているパスワードを取得する
//===========================================================================
void __fastcall TAttacheCaseFileEncrypt::GetPasswordBinary(char *password)
{

memcpy(password, key, 32);

}
//===========================================================================
//メインフォームに確認メッセージを投げて処理を中断する
//===========================================================================
void __fastcall TAttacheCaseFileEncrypt::PostConfirmMessageForm()
{

//※以下が、グローバル変数(private)として定義してある
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
void __fastcall TAttacheCaseFileEncrypt::PostConfirmOverwriteMessageForm()
{

//グローバル変数(private)として定義してある

//String MsgText;
//String MsgReturnPath;

MsgReturnVal = Form1->ShowConfirmOverwriteMassageForm(MsgText, MsgReturnPath);

}
//===========================================================================

