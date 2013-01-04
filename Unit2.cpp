//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{

/*
//バージョン情報取得
pAppInfoString->Comments;
pAppInfoString->InternalName;
pAppInfoString->ProductName;
pAppInfoString->CompanyName;
pAppInfoString->LegalCopyright;
pAppInfoString->ProductVersion;
pAppInfoString->FileDescription;
pAppInfoString->LegalTrademarks;
pAppInfoString->PrivateBuild;
pAppInfoString->FileVersion;
pAppInfoString->OriginalFilename;
pAppInfoString->SpecialBuild;
*/

TGetAppInfoString *pAppInfoString = new TGetAppInfoString();
//アプリケーション名
lblAppName->Caption = Application->Title;
//バージョン情報
VersionString = pAppInfoString->FileVersion;
lblVersion->Caption = "ver."+ VersionString;
//zlibバージョン
String ZlibVersion = ZLIB_VERSION;
lblZlibVersion->Caption = "(zlib ver."+ ZlibVersion + ")";
//ライセンス表示
lblLisence->Caption = "The BSD 3-Clause License";
//著作権表示
lblCopyright->Caption = pAppInfoString->LegalCopyright;
//サイト表示
lblHomePage->Caption = "http://hibara.org/software/";

delete pAppInfoString;

}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormCreate(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TForm2::cmdOKClick(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::CMMouseLeave( TMessage &Msg )
{

if (Msg.LParam == long(lblLisence)){
	lblLisence->Font->Color = TColor(TColor(0x00FF5A5A));
	lblLisence->Color = clBtnFace;

}
else if(Msg.LParam == long(lblHomePage)){
	lblHomePage->Font->Color = TColor(TColor(0x00FF5A5A));
	lblHomePage->Color = clBtnFace;

}//end if;

}
//---------------------------------------------------------------------------
void __fastcall TForm2::lblLisenceClick(TObject *Sender)
{

//ブラウザ起動
String Link = "http://opensource.org/licenses/BSD-3-Clause";
ShellExecuteW(NULL, L"open", Link.c_str(), NULL, NULL, SW_NORMAL);
Close();

}
//---------------------------------------------------------------------------
void __fastcall TForm2::lblLisenceMouseMove(TObject *Sender, TShiftState Shift, int X,
					int Y)
{
//文字フォントのカラー変更
lblLisence->Font->Color = clBlue;
lblLisence->Update();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::lblHomePageMouseMove(TObject *Sender, TShiftState Shift, int X,
					int Y)
{
//文字フォントのカラー変更
lblHomePage->Font->Color = clBlue;
lblHomePage->Update();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::lblHomePageClick(TObject *Sender)
{

//ピリオド抜きのバージョン番号を取得する
String VerString = StringReplace(VersionString, ".", "", TReplaceFlags()<<rfReplaceAll);
//ブラウザ起動
String Link = "http://hibara.org/software/?s=atc&v="+VerString;
ShellExecuteW(NULL, L"open", Link.c_str(), NULL, NULL, SW_NORMAL);
Close();

}
//---------------------------------------------------------------------------
void __fastcall TForm2::lblLisenceMouseEnter(TObject *Sender)
{

BalloonHint1->Title = "http://opensource.org/licenses/BSD-3-Clause";
//BalloonHint1->Description = "クリックするとブラウザが起動します。";
BalloonHint1->Description = LoadResourceString(&Msgunit2::_BALLOON_HINT_MSG);
BalloonHint1->ShowHint(lblLisence->ClientToScreen(CenterPoint(lblLisence->ClientRect)));

}
//---------------------------------------------------------------------------
void __fastcall TForm2::lblHomePageMouseEnter(TObject *Sender)
{

BalloonHint1->Title = "http://hibara.org/software/";
//BalloonHint1->Description = "クリックするとブラウザが起動します。";
BalloonHint1->Description = LoadResourceString(&Msgunit2::_BALLOON_HINT_MSG);
BalloonHint1->ShowHint(lblHomePage->ClientToScreen(CenterPoint(lblHomePage->ClientRect)));

}
//---------------------------------------------------------------------------



