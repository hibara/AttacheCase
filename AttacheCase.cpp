//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

#include "Unit1.h"
#include "MsgWinMain.hpp"

//---------------------------------------------------------------------------
USEFORM("Unit1.cpp", Form1);
USEFORM("Unit2.cpp", Form2);
USEFORM("Unit3.cpp", Form3);
USEFORM("Unit4.cpp", Form4);
//---------------------------------------------------------------------------
HANDLE mx;
void SendToMsgData(HWND handle);

//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{

//'アタッシェケース'
String ApplicationTitle = LoadResourceString(&Msgwinmain::_TITLE_APP_NAME);
HWND handle = NULL;

try{

	mx = CreateMutex( NULL, true, "AtacheCase2" );

	if( GetLastError() ){
		//すでに起動中のアタッシェケースのウィンドウハンドルを取得
		handle = ::FindWindowW(NULL, ApplicationTitle.c_str());
	}

	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->Title = ApplicationTitle;
		Application->HelpFile = "AttacheCase.chm";
		Application->CreateForm(__classid(TForm1), &Form1);
		if ( handle != NULL && Form1->opthdl->fNoMultipleInstance == true){
			//すでに起動中のアタッシェケースへメッセージを送る
			SendToMsgData(handle);
		}
		else{
			Application->Run();
		}

	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}

}
__finally{
	ReleaseMutex(mx);

}

Application->Terminate();
return 0;

}
//----------------------------------------------------------------------
//すでに起動中のアタッシェケースへファイルリストをメッセージとして送る
//----------------------------------------------------------------------
void SendToMsgData(HWND handle)
{

int i;

TStringList *SendFileList;


try{

SendFileList = new TStringList;

WPARAM wParam;
LPARAM lParam;

COPYDATASTRUCT cds;

wParam = (WPARAM)Application->Handle;
lParam = (LPARAM)&cds;

//コマンドライン引数をまとめる
for ( i = 1; i < ParamCount()+1 ; i++){
	if ( FileExists(ParamStr(i)) || DirectoryExists(ParamStr(i))){
		SendFileList->Add(ExpandFileName(ParamStr(i)));
	}
}

if ( SendFileList->Count > 0 ){

	// データサイズ（UnicodeString）
	int size = SendFileList->Text.Length()*SendFileList->Text.ElementSize() + 1;
	wchar_t *buffer = new wchar_t[size];
	ZeroMemory( buffer, size );                          // 初期化
	StrLCopy( buffer, SendFileList->Text.c_str(), size); // バッファへコピー

	cds.dwData = 1;
	cds.cbData = size;
	cds.lpData = (LPVOID)buffer;

	//送りつける
	::SendMessage(handle, WM_COPYDATA, wParam, lParam);

}

//ウィンドウを前面に
SetForegroundWindow(handle);

}
__finally{
	delete SendFileList;

}

}//end SendToMsgData;
//---------------------------------------------------------------------------




