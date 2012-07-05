//---------------------------------------------------------------------------

#include <vcl.h>

//printf
#include <stdio.h>

#pragma hdrstop

#define BUF_SIZE 256

#include <tchar.h>
//---------------------------------------------------------------------------

#pragma argsused
int _tmain(int argc, _TCHAR* argv[])
{

//ロケールの設定
_tsetlocale(LC_ALL, _T(""));

//-----------------------------------
//パラメータチェック
//-----------------------------------
String FilePath = ExpandFileName(String(argv[1]));  //アタッシェケース本体
String mFilePath = ExpandFileName(String(argv[2])); //Mergeする自己実行ファイル

if ( FileExists(FilePath) == false ){
	wprintf(L"\"%s\" - マージする本体のファイルが見つかりません。\n", FilePath.c_str());
	return 1;
}

if ( FileExists(mFilePath) == false ){
	wprintf(L"\"%s\" - マージするファイルが見つかりません。\n", mFilePath.c_str());
	return 1;
}

//-----------------------------------
// マージ
//-----------------------------------

int i;

int pos;
int bufsize;
char buffer[BUF_SIZE];

//追加ファイルを開く
int fh = FileOpen(FilePath, fmOpenReadWrite);
int mfh = FileOpen(mFilePath, fmOpenRead);

if (fh == -1 || mfh == -1) {
	wprintf(L"%s - マージするファイルが開けません。", FilePath.c_str());
	FileClose(fh);
	FileClose(mfh);
	return 1;
}

//マージファイルのサイズ取得
__int64 size = FileSeek(mfh, (__int64)0, 2);
FileSeek(mfh, 0, 0);

//本体末尾へポイント
FileSeek(fh, 0, 2);

while ((pos = FileRead( mfh, buffer, BUF_SIZE )) != 0 ){

	if ( FileWrite( fh, buffer, pos) == -1 ){
		wprintf(L"%s - 書き込みで失敗しました。", FilePath.c_str());
		FileClose(fh);
		FileClose(mfh);
		return 1;
	}

	for ( i = 0; i < BUF_SIZE; i++ ){
		buffer[i] = NULL;
	}

}

//お尻にマージしたファイルサイズを埋め込む
FileWrite( fh, &size, sizeof(__int64));

FileClose(fh);
FileClose(mfh);

wprintf(L"%s - マージ完了しました。\n", FilePath.c_str());

return 0;

}
//---------------------------------------------------------------------------
