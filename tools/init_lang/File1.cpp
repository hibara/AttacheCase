//---------------------------------------------------------------------------

#include <vcl.h>





#pragma hdrstop

#include <tchar.h>

// フォルダー内にあるpasファイルを検索してまとめて処理する
bool SearchFolderForPasFiles(String DirPath);
// 宣言されたメッセージ定数名を initialization に書き込む
bool WriteToInitializationSection(String FilePath);

//---------------------------------------------------------------------------
#pragma argsused
int _tmain(int argc, _TCHAR* argv[])
{

int i;
String FilePath;
String MsgErrorString;

for (i = 1; i < argc; i++) {

	FilePath = argv[i];

	//ファイル
	if (FileExists(FilePath) == true) {

		WriteToInitializationSection(FilePath);

	}
	//ディレクトリ
	else{

		if (DirectoryExists(FilePath) == true) {
			SearchFolderForPasFiles(FilePath);
		}
		else{
			MsgErrorString = "指定されたファイル/フォルダーが見つかりません。\n処理を中止します。\n"+FilePath;
			MessageDlg(MsgErrorString, mtConfirmation, TMsgDlgButtons()<<mbOK, 0);
			return 1;
		}

	}

}

return 0;


}
//===========================================================================
// フォルダー内にあるpasファイルを検索してまとめて処理する
//===========================================================================
bool SearchFolderForPasFiles(String DirPath)
{

TSearchRec sr;
String FilePath;
int ret  = FindFirst(DirPath + "*.pas", faAnyFile, sr);

while (ret == 0) {

	if (sr.Name != "." && sr.Name != "..") {

		FilePath = IncludeTrailingPathDelimiter(DirPath) + sr.Name;

		if (sr.Attr & faDirectory) {
			//ディレクトリは無視
		}
		else{
			//ファイル
			WriteToInitializationSection(FilePath);
		}
	}
	ret = FindNext(sr);
}

return(true);

}
//===========================================================================
// 宣言されたメッセージ定数名を initialization に書き込む
//===========================================================================
bool WriteToInitializationSection(String FilePath)
{

int i;
String fSection = "";

TStringList *FileList = new TStringList;
FileList->LoadFromFile(FilePath);

TStringList *OutFileList = new TStringList;

//列挙する定数リストを挿入する
TStringList *InitList = new TStringList;
InitList->Add("\n\t//Delphiコンパイラによる最適化を防ぐ");

// '='区切りの文字列を分ける
TStringList* tsv = new TStringList();
tsv->StrictDelimiter = true;
tsv->Delimiter = '=';

for (i = 0; i < FileList->Count; i++) {

	if ( Trim(FileList->Strings[i]) == "resourcestring" ) {
		fSection = "resourcestring";
		OutFileList->Add(FileList->Strings[i]);
	}
	else if ( Trim(FileList->Strings[i]) == "implementation" ) {
		fSection = "implementation";
		OutFileList->Add(FileList->Strings[i]);
	}
	else if ( Trim(FileList->Strings[i]) == "initialization" ) {
		fSection = "initialization";
		OutFileList->Add(FileList->Strings[i]);
		//貯めていた内容を吐き出す
		OutFileList->Add(InitList->Text);
	}
	else if ( Trim(FileList->Strings[i]) == "end." ) {
		fSection = "end.";
		OutFileList->Add(FileList->Strings[i]);
	}
	else{
		if ( fSection == "resourcestring" ) {   // resourcestring部を精査中...
			if (FileList->Strings[i].Pos("=") > 0) {
				tsv->DelimitedText = FileList->Strings[i];
				InitList->Add("\t" + Trim(tsv->Strings[0]) + ";");  //定数名だけ抜いておく
			}
			else if (Trim(FileList->Strings[i]) == "") {          //空行も一応再現
				InitList->Add("");
			}
			OutFileList->Add(FileList->Strings[i]);
		}
		else if ( fSection == "initialization" ) {
			// "initialization" セクションはすでに書き出したので無視
		}
		else{
			OutFileList->Add(FileList->Strings[i]);
		}
	}
}

OutFileList->SaveToFile(FilePath);

delete OutFileList;

delete InitList;
delete tsv;
delete FileList;

return(true);

}
//===========================================================================

