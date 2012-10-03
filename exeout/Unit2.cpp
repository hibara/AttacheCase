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
lblAppName->Caption = pAppInfoString->ProductName;
//バージョン情報
lblVersion->Caption = "ver."+ pAppInfoString->FileVersion;
//著作権表示
lblCopyright->Caption = pAppInfoString->LegalCopyright;
//サイト表示
lblWebSite->Caption = pAppInfoString->CompanyName;

delete pAppInfoString;

imgMainIcon->Picture->Icon = Application->Icon;

//'バージョン情報'
this->Caption = LoadResourceString(&Msgexeout::_FORM_ABOUT_CAPTION);

//'このファイルは、フリーで、かつオープンソースで開発されている、';
//'ソフトウェアで暗号化されています。';
//'詳しい情報は以下のサイトをご覧ください。';
//'http://hibara.org/';
lblDescription01->Caption = LoadResourceString(&Msgexeout::_FORM_ABOUT_LABEL_CAPTION_DESCRIPTION_01);
lblDescription02->Caption = LoadResourceString(&Msgexeout::_FORM_ABOUT_LABEL_CAPTION_DESCRIPTION_02);
lblDescription03->Caption = LoadResourceString(&Msgexeout::_FORM_ABOUT_LABEL_CAPTION_DESCRIPTION_03);

}
//---------------------------------------------------------------------------
void __fastcall TForm2::cmdExitClick(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::lblWebSiteMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
lblWebSite->Font->Color = TColor(0x00CC0000);
}
//---------------------------------------------------------------------------
void __fastcall TForm2::lblWebSiteMouseLeave(TObject *Sender)
{
lblWebSite->Font->Color = TColor(0x00FF9999);
}
//---------------------------------------------------------------------------
void __fastcall TForm2::lblWebSiteClick(TObject *Sender)
{

//ブラウザ関連付けオープン
ShellExecuteW(NULL, L"open", lblWebSite->Caption.c_str(), NULL, NULL, SW_NORMAL);
Close();

}
//---------------------------------------------------------------------------

