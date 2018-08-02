//===========================================================================
/*

アタッシェケース（AttachéCase）
Copyright (c) 2002-2018, Mitsuhiro Hibara ( http://hibara.org )
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

