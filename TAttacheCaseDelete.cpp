//===========================================================================
/*

アタッシェケース（AttachéCase）
Copyright (c) 2002-2017, Mitsuhiro Hibara ( http://hibara.org )
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
#include "TAttacheCaseDelete.h"

#pragma package(smart_init)
// ---------------------------------------------------------------------------
// 注意：異なるスレッドが所有する VCL のメソッド/関数/プロパティを別の
// レッド所有のオブジェクトに対しては Synchronize を使用できます。
//
// Synchronize(&UpdateCaption);
//
// 例えば UpdateCaption を以下のように定義し
//
// void __fastcall TAttacheCaseDelete::UpdateCaption()
// {
// Form1->Caption = "スレッドから書き換えました";
// }
// ---------------------------------------------------------------------------
__fastcall TAttacheCaseDelete::TAttacheCaseDelete
(bool CreateSuspended) : TThread(CreateSuspended)
{

Opt = 0;	                     //通常削除
RandClearNum = 0;              //乱数書き込み回数
ZeroClearNum = 0;              //ゼロ書き込み回数

ProgressPercentNum = -1;       //進捗パーセント
ProgressStatusText = "";       //進捗ステータス
ProgressMsgText = "";          //進捗メッセージ

StatusNum = 0;                 //ステータス表示内容番号
MsgErrorString = "";           //エラーメッセージ

DeleteFileList = new TStringList; //削除ファイルリスト

}
//===========================================================================
//デストラクタ
//===========================================================================
__fastcall TAttacheCaseDelete::~TAttacheCaseDelete(void)
{

delete DeleteFileList;

}
//===========================================================================
//スレッド実行
//===========================================================================
void __fastcall TAttacheCaseDelete::Execute()
{

int i;
int FileCount = 0;
__int64 CountFileSize = 0;

int ret;
int ErrorNum;
TSearchRec sr;
int Attrs;

String FilePath;

//===================================
//デバッグ
//ShowMessage(DeleteFileList->Text);
//===================================

//-----------------------------------
// ゴミ箱の場合はファイル/フォルダ毎
// そのまま移動
//-----------------------------------
if (Opt == 2) {
	for (i = 0; i < DeleteFileList->Count; i++) {
		if (Terminated == true) {
			goto LabelStop;
		}

		if ( GoToTrash(DeleteFileList->Strings[i]) == false ){
			//エラー
			goto LabelStop;
		}
	}

	ProgressPercentNum = 100;
	//'完了'
	ProgressStatusText = LoadResourceString(&Msgdelete::_LABEL_STATUS_TITLE_COMPLETE);
	//'削除が正常に完了しました。'
	ProgressMsgText = LoadResourceString(&Msgdelete::_LABEL_STATUS_DETAIL_COMPLETE);
	return;

}

//----------------------------------------------------
// 削除ファイルリスト情報（ファイル数、サイズ）を取得
//----------------------------------------------------
ProgressPercentNum = -1;
//'ファイルリストの生成'
ProgressStatusText = LoadResourceString(&Msgdelete::_LABEL_STATUS_TITLE_LISTING);
//'削除するための準備をしています...'
ProgressMsgText = LoadResourceString(&Msgdelete::_LABEL_STATUS_DETAIL_PREPARING);

//-----------------------------------
// 削除、または完全削除
//-----------------------------------
if ( Opt == 0){
	//'削除しています...'
	ProgressStatusText = LoadResourceString(&Msgdelete::_LABEL_STATUS_TITLE_DELETING);
}
else if ( Opt == 1 ) {
	//'完全削除しています...'
	ProgressStatusText = LoadResourceString(&Msgdelete::_LABEL_STATUS_TITLE_COMPLETE_DELETING);
	//完全削除回数分×合計サイズが増える
	TotalFileSize = TotalFileSize * (RandClearNum + ZeroClearNum > 0 ? RandClearNum + ZeroClearNum : 1 );
}


//-----------------------------------
// 削除実行
//-----------------------------------
for (i = DeleteFileList->Count-1; i > -1; i--) {

	FilePath = DeleteFileList->Strings[i];

	//-----------------------------------
	// ファイル
	//-----------------------------------
	if (FileExists(FilePath) == true) {

		//読み取り専用なら外してから
		if ( FileIsReadOnly(FilePath) == true ){
			FileSetReadOnly(FilePath, false);
		}
		//-----------------------------------
		// 通常削除
		//-----------------------------------
		if ( Opt == 0){
			DeleteFile(FilePath);
			FileCount++;
			//ファイル数でプログレス表示
			ProgressPercentNum = ((float)FileCount/DeleteFileList->Count)*100;
		}
		//-----------------------------------
		// 完全削除
		//-----------------------------------
		else if ( Opt == 1 ) {
			//処理サイズでプログレス表示（「完全削除」関数内で処理）
			if ( CompleteDeleteFile(FilePath, CountFileSize, TotalFileSize) == false ){
				goto LabelError;
			}
		}
	}
	//-----------------------------------
	// ディレクトリ
	//-----------------------------------
	else{
		//ディレクトリは空行が入っているので
		//前に削除したファイルの親ディレクトリを取得
		FilePath = ExtractFileDir(DeleteFileList->Strings[i+1]);
		if ( RemoveDir(FilePath) == false ){
			//削除に失敗したときは属性を変更
			Attrs = FileGetAttr(FilePath);
			if (Attrs & faHidden){   //隠しファイル属性のときは外す
				FileSetAttr( FilePath, Attrs & !faHidden);
			}
			if (Attrs & faReadOnly){ //読み取り専用属性のときも外す
				FileSetAttr( FilePath, Attrs & !faReadOnly	);
			}
			RemoveDir(FilePath);     //再チャレンジ
		}
	}

	if (Terminated == true) {
		goto LabelStop;
	}

}//end for (i = DeleteList->Count-1; i > -1; i--);

ProgressPercentNum = 100;
//'完了'
ProgressStatusText = LoadResourceString(&Msgdelete::_LABEL_STATUS_TITLE_COMPLETE);
//'削除が正常に完了しました。'
ProgressMsgText = LoadResourceString(&Msgdelete::_LABEL_STATUS_DETAIL_COMPLETE);

StatusNum = 1;

return;

//-----------------------------------
// エラー
//-----------------------------------
LabelError:

	ProgressPercentNum = 0;
	//'エラー'
	ProgressStatusText = LoadResourceString(&Msgencrypt::_LABEL_STATUS_TITLE_ERROR);
	//'削除に失敗しました。'
	ProgressMsgText = LoadResourceString(&Msgencrypt::_LABEL_STATUS_DETAIL_FAILED);

	StatusNum = -1;

	return;


//-----------------------------------
// ユーザーキャンセル
//-----------------------------------
LabelStop:

	ProgressPercentNum = 0;
	//'キャンセル'
	ProgressStatusText = LoadResourceString(&Msgdelete::_LABEL_STATUS_TITLE_USER_CANCEL);
	//'削除が中止されました。'
	ProgressMsgText = LoadResourceString(&Msgdelete::_LABEL_STATUS_DETAIL_STOPPED);

	StatusNum = -2;

	return;


}
//===========================================================================
//ファイルの完全削除
//===========================================================================
int __fastcall TAttacheCaseDelete::
	CompleteDeleteFile(String FilePath, __int64 &CountSize, __int64 TotalFileSize)
{

int i, c;
int res;
char buffer[LARGE_BUF];

float ProgressPercentNumF;

HANDLE  hFile;
DWORD uniWriteByte;

int fh = FileOpen(FilePath, fmShareDenyNone);
if ( fh == -1 )return(false);

__int64 uiWriteSize;
__int64 uiFileSize = FileSeek(fh,(__int64)0, 2);
FileClose(fh);

//読み取り専用なら外しておく
if ( FileIsReadOnly(FilePath) == true ){
	FileSetReadOnly(FilePath, false);
}

//-----------------------------------
//ランダムな値で指定回数書き込みを行う
//-----------------------------------
for ( i = 0; i < RandClearNum; i++ ){

	//'乱数を書き込み、完全削除を行っています...(Step:%d/%d)'
	ProgressMsgText =
		String().Format(LoadResourceString(&Msgdelete::_LABEL_STATUS_DETAIL_WRITE_RAND),
										 ARRAYOFCONST((i + 1, RandClearNum+ZeroClearNum))) + "\n" + FilePath;

	//キャッシュを直接ディスクへ書き込む
	hFile = CreateFileW(
		FilePath.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ,
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);

	if ( hFile != INVALID_HANDLE_VALUE ){

		uiWriteSize = 0;

		while ( uiWriteSize < uiFileSize ){

			fillrand(buffer, LARGE_BUF);

			//ランダムの値で埋め尽くす
			if ( WriteFile( hFile, buffer, LARGE_BUF, &uniWriteByte, NULL) == 0 ){
				break;
			}

			uiWriteSize+=uniWriteByte;
			CountSize+=uniWriteByte;

			//途中キャンセルされたときの処理
			if ( Terminated == true ){
				CloseHandle( hFile );
				DeleteFile(FilePath);  //ふつうに削除する
				return(-2);
			}

			//プログレス表示
			ProgressPercentNumF = (float)CountSize/TotalFileSize;
			ProgressPercentNum = (int)(ProgressPercentNumF*100);
			if (TotalFileSize < 104857600) {	// 100MB未満
				ProgressPercentNumText = IntToStr(ProgressPercentNum)+"%";
			}
			else{
				ProgressPercentNumText = FloatToStrF(ProgressPercentNumF*100, ffNumber, 4, 1)+"%";
			}

		}//while;

	}
	else{
		//'ファイル削除中にエラーが発生しました。ファイルを開くことができません。'
		MsgText = LoadResourceString(&Msgdelete::_MSG_ERROR_NOT_FILE_OPEN) + "\n" + FilePath;
		MsgType = mtError;
		MsgButtons = TMsgDlgButtons() << mbOK;
		MsgDefaultButton = mbOK;
		Synchronize(&PostConfirmMessageForm);
		CloseHandle( hFile );
		return(-1);

	}//end if;

	CloseHandle( hFile );

}//loop;

//-----------------------------------
//NULL値で指定回数書き込む
//-----------------------------------

for ( c = 0; c < LARGE_BUF; c++ ){
	buffer[c] = NULL;
}

for ( i = 0; i < ZeroClearNum; i++ ){

	//'ゼロ(NULL)を書き込み、完全削除を行っています...(Step:%d/%d)'
	ProgressMsgText =
		String().Format(LoadResourceString(&Msgdelete::_LABEL_STATUS_DETAIL_WRITE_ZERO),
										 ARRAYOFCONST((i + 1 + RandClearNum, RandClearNum+ZeroClearNum))) +
										 "\n" + FilePath;

	//キャッシュを直接ディスクへ書き込む
	hFile = CreateFileW(
		FilePath.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);

	if ( hFile != INVALID_HANDLE_VALUE ){

		uiWriteSize = 0;

		while ( uiWriteSize < uiFileSize ){

			//NULL値で埋め尽くす
			if ( WriteFile( hFile, buffer, LARGE_BUF, &uniWriteByte, NULL) == 0 ){
				break;
			}
			uiWriteSize+=uniWriteByte;
			CountSize+=uniWriteByte;

			//途中キャンセルされたときの処理
			if ( Terminated == true ){
				CloseHandle( hFile );
				DeleteFile(FilePath);  //ふつうに削除する
				return(-2);
			}

			//プログレス表示
			ProgressPercentNumF = (float)CountSize/TotalFileSize;
			ProgressPercentNum = (int)(ProgressPercentNumF*100);
			if (TotalFileSize < 104857600) {	// 100MB未満
				ProgressPercentNumText = IntToStr(ProgressPercentNum)+"%";
			}
			else{
				ProgressPercentNumText = FloatToStrF(ProgressPercentNumF*100, ffNumber, 4, 1)+"%";
			}

		}//while;

	}
	else{
		//'ファイル削除中にエラーが発生しました。ファイルを開くことができません。'
		MsgText = LoadResourceString(&Msgdelete::_MSG_ERROR_NOT_FILE_OPEN) + "\n" + FilePath;
		MsgType = mtError;
		MsgButtons = TMsgDlgButtons() << mbOK;
		MsgDefaultButton = mbOK;
		Synchronize(&PostConfirmMessageForm);
		CloseHandle( hFile );
		return(-1);

	}//end if;

	CloseHandle( hFile );

}//loop;

//-----------------------------------
//さらにファイルサイズを0バイトにする

hFile = CreateFileW(
	FilePath.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ,
	NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING, NULL);//FILE_FLAG_WRITE_THROUGH

CloseHandle( hFile );

//-----------------------------------
//ようやくここで削除

DeleteFile(FilePath);

return(1);


}//end CompleteDeleteFile;
//===========================================================================
//ごみ箱への移動
//===========================================================================
bool __fastcall TAttacheCaseDelete::GoToTrash(String Path)
{

ProgressPercentNum = -1;
//'ごみ箱へ移動しています...'
ProgressStatusText = LoadResourceString(&Msgdelete::_LABEL_STATUS_TITLE_GO_TO_TRASH);
ProgressMsgText = Path;

if (FileExists(Path) == true) {
}
else{
	if (DirectoryExists(Path) == true) {
		Path = ExcludeTrailingPathDelimiter(Path);
	}
	else{
		//'ごみ箱への削除に失敗しました。'+#13+
		//'一部のファイル/フォルダが削除できずに残ってしまった可能性があります。';
		MsgText = LoadResourceString(&Msgdelete::_MSG_ERROR_GO_TO_TRASH_FAILED);
		MsgType = mtError;
		MsgButtons = TMsgDlgButtons() << mbOK;
		MsgDefaultButton = mbOK;
		Synchronize(&PostConfirmMessageForm);
		return(false);
	}
}

const int len = Path.Length() + 2;  //終端\0\0
wchar_t* from = new wchar_t[len];
wcscpy(from, Path.c_str());
from[len-1] = 0;

SHFILEOPSTRUCTW sh;
ZeroMemory(&sh, sizeof(SHFILEOPSTRUCT));
sh.hwnd = Application->Handle;
sh.wFunc = FO_DELETE;              // ゴミ箱へ捨てる
sh.pFrom = from;                   // ファイルのポインタ
sh.pTo = NULL;
// Undo可, 確認なし（※でも結果的にUndoは不可能になるみたい）
sh.fFlags= FOF_ALLOWUNDO + FOF_NOCONFIRMATION;

if ( SHFileOperationW(&sh) != 0 ){
	//'ごみ箱への削除に失敗しました。'+#13+
	//'一部のファイル/フォルダが削除できずに残ってしまった可能性があります。';
	MsgText = LoadResourceString(&Msgdelete::_MSG_ERROR_GO_TO_TRASH_FAILED);
	MsgType = mtError;
	MsgButtons = TMsgDlgButtons() << mbOK;
	MsgDefaultButton = mbOK;
	Synchronize(&PostConfirmMessageForm);
	return(false);
}

delete[] from;

//フォルダ内の表示更新
SHChangeNotify(SHCNE_UPDATEDIR, SHCNF_PATH, Path.c_str(), 0);

return(true);


}//end GotoTrashOneFile;
//===========================================================================
// ランダム値の生成（CRNG）
//===========================================================================
void TAttacheCaseDelete::fillrand(char *buf, const int len)
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
//メインフォームに確認メッセージを投げて処理を中断する
//===========================================================================
void __fastcall TAttacheCaseDelete::PostConfirmMessageForm()
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




