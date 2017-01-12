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
#include <tchar.h>

#include "Unit1.h"
#include "MsgWinMain.hpp"

//---------------------------------------------------------------------------
USEFORM("Unit3.cpp", Form3);
USEFORM("Unit2.cpp", Form2);
USEFORM("Unit4.cpp", Form4);
USEFORM("Unit1.cpp", Form1);
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
		Application->Title = "アタッシェケース";
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




