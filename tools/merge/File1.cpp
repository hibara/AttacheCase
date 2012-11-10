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

int i;

//ロケールの設定
_tsetlocale(LC_ALL, _T(""));

//-----------------------------------
//パラメータチェック
//-----------------------------------

String mFilePath;
String FilePath = ExpandFileName(String(argv[1]));  //本体となる実行ファイル

TStringList *FileList = new TStringList;
for (i = 2; i < argc; i++) {
	mFilePath = ExpandFileName(String(argv[i]));
	if (FileExists(mFilePath) == true ) {
		FileList->Add(mFilePath);
	}
}

if ( FileExists(FilePath) == false ){
	wprintf(L"\"%s\" - マージする本体のファイルが見つかりません。\n", FilePath.c_str());
	delete FileList;
	return 1;
}

if ( FileList->Count == 0 ){
	wprintf(L"マージするファイルが一つも見つかりません。\n");
	delete FileList;
	return 1;
}

//-----------------------------------
// マージ
//-----------------------------------

int fh, mfh;
int pos;
int bufsize;
char buffer[BUF_SIZE];

fh = FileOpen(FilePath, fmOpenReadWrite);


for (i = 0; i < FileList->Count; i++) {

	mFilePath = FileList->Strings[i];

	//本体へマージする追加ファイルを開く
	mfh = FileOpen(mFilePath, fmOpenRead);

	if (fh == -1 || mfh == -1) {
		wprintf(L"%s\n%s\n - 本体、またはマージするファイルが開けません。", FilePath.c_str(), mFilePath.c_str());
		FileClose(fh);
		FileClose(mfh);
		delete FileList;
		return 1;
	}

	//マージするファイルのサイズ取得
	__int64 size = FileSeek(mfh, (__int64)0, 2);
	FileSeek(mfh, 0, 0);

	//本体末尾へポイント
	FileSeek(fh, 0, 2);

	while ((pos = FileRead( mfh, buffer, BUF_SIZE )) != 0 ){

		if ( FileWrite( fh, buffer, pos) == -1 ){
			wprintf(L"%s - 書き込みで失敗しました。", FilePath.c_str());
			FileClose(fh);
			FileClose(mfh);
			delete FileList;
			return 1;
		}

		for ( i = 0; i < BUF_SIZE; i++ ){
			buffer[i] = NULL;
		}

	}

	//お尻にマージしたファイルサイズを埋め込む
	FileWrite( fh, &size, sizeof(__int64));
	wprintf(L"%s - マージしました。\n", mFilePath.c_str());
	FileClose(mfh);

}

FileClose(fh);
wprintf(L"%s - マージ完了しました。\n", FilePath.c_str());

delete FileList;

return 0;

}
//---------------------------------------------------------------------------
