//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit4.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner, int Type)
	: TForm(Owner)
{

int i;

InputType = Type;

//日時のサンプル書式
String SampleText;
String SampleDateTime[8] = {
	"dddddd(dddd)",
	"yymmdd",
	"yyyy_m_d",
	"h'時'n'分'n'秒'",
	"",
	"",
	"",
	""
};

PageControl1->Align = alClient;
for (int i = 0; i < PageControl1->PageCount; i++) {
	PageControl1->Pages[i]->TabVisible = false;
}

switch(InputType){

case 0: //記憶する暗号化パスワードの入力
	this->Caption = LoadResourceString(&Msgunit4::_FORM_CAPTION_INPUT_ENCRYPT_MEM_PASSWORD);
	cmdOK->Caption = LoadResourceString(&Msgunit4::_BUTTON_CAPTION_OK);
	cmdOK->Enabled = false;
	txtInputMemPassword->EditLabel->Caption =
		LoadResourceString(&Msgunit4::_LABEL_INPUT_MEM_NEW_PASSWORD);
	txtInputMemConfirmPassword->EditLabel->Caption =
		LoadResourceString(&Msgunit4::_LABEL_INPUT_MEM_NEW_PASSWORD_CONFIRM);

	PageControl1->ActivePageIndex = 0;

	break;

case 1: //記憶する復号パスワードの入力
	this->Caption = LoadResourceString(&Msgunit4::_FORM_CAPTION_INSERT_DECRYPT_MEM_PASSWORD);
	cmdOK->Caption = LoadResourceString(&Msgunit4::_BUTTON_CAPTION_OK);
	cmdOK->Enabled = false;
	txtInputMemPassword->EditLabel->Caption =
		LoadResourceString(&Msgunit4::_LABEL_INPUT_MEM_NEW_PASSWORD);
	txtInputMemConfirmPassword->EditLabel->Caption =
		LoadResourceString(&Msgunit4::_LABEL_INPUT_MEM_NEW_PASSWORD_CONFIRM);

	PageControl1->ActivePageIndex = 0;

	break;

case 2:	//日付書式の挿入
	this->Caption = LoadResourceString(&Msgunit4::_FORM_CAPTION_INSERT_DATETIME_FORMAT);
	cmdOK->Caption = LoadResourceString(&Msgunit4::_BUTTON_CAPTION_INSERT);
	lblExampleDateTime->Caption = LoadResourceString(&Msgunit4::_LABEL_INSERT_DATETIME);

	PageControl1->ActivePageIndex = 1;

	for ( i = 0; i < 8; i++ ){
		if ( SampleDateTime[i] != "" ) {
			ComboDateTime->Items->Add(SampleDateTime[i]);
		}
	}

	ComboDateTime->ItemIndex = 0;

	//表示をサンプル反映
	SampleText = Now().FormatString(SampleDateTime[ComboDateTime->ItemIndex]);
	lblExampleDateTime->Caption = "ex). "+SampleText;

	break;

case 3:	//連番書式の挿入
	this->Caption = LoadResourceString(&Msgunit4::_FORM_CAPTION_INSERT_SERIAL_NUM_FORMAT);
	cmdOK->Caption = LoadResourceString(&Msgunit4::_BUTTON_CAPTION_INSERT);
	txtNum->EditLabel->Caption = LoadResourceString(&Msgunit4::_LABEL_INSERT_SERIAL_NUM);
	imgInsertNum->Picture = imgSerialNum->Picture;

	PageControl1->ActivePageIndex = 2;

	break;

case 4:	//ランダム文字列数の挿入
	this->Caption = LoadResourceString(&Msgunit4::_FORM_CAPTION_INSERT_RANDOM_TEXT_NUM_FORMAT);
	cmdOK->Caption = LoadResourceString(&Msgunit4::_BUTTON_CAPTION_INSERT);
	txtNum->EditLabel->Caption = LoadResourceString(&Msgunit4::_LABEL_INSERT_RANDOM_TEXT);
	imgInsertNum->Picture = imgRandomText->Picture;

	PageControl1->ActivePageIndex = 2;

	break;

case 5:	//ファイル名の先頭文字数の挿入
	this->Caption = LoadResourceString(&Msgunit4::_FORM_CAPTION_INSERT_FILE_NAME_HEAD_NUM_FORMAT);
	cmdOK->Caption = LoadResourceString(&Msgunit4::_BUTTON_CAPTION_INSERT);
	txtNum->EditLabel->Caption = LoadResourceString(&Msgunit4::_LABEL_INSERT_FILE_NAME_HEAD);

	imgInsertNum->Picture = imgFileNameHead->Picture;

	PageControl1->ActivePageIndex = 2;

	break;

case 6:	//ファイル名の末尾文字数の挿入
	this->Caption = LoadResourceString(&Msgunit4::_FORM_CAPTION_INSERT_FILE_NAME_END_NUM_FORMAT);
	cmdOK->Caption = LoadResourceString(&Msgunit4::_BUTTON_CAPTION_INSERT);
	txtNum->EditLabel->Caption = LoadResourceString(&Msgunit4::_LABEL_INSERT_FILE_NAME_END);

	imgInsertNum->Picture = imgFileNameEnd->Picture;

	PageControl1->ActivePageIndex = 2;

	break;

default:
	//エラー
	PageControl1->Visible = false;
	break;

}

//キャンセルボタン
cmdCancel->Caption = LoadResourceString(&Msgunit4::_BUTTON_CAPTION_CANCEL);


}
//---------------------------------------------------------------------------
void __fastcall TForm4::FormCreate(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TForm4::FormDestroy(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TForm4::FormShow(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TForm4::FormResize(TObject *Sender)
{

//-----------------------------------
//パスワード入力パネル
//-----------------------------------
if (PageControl1->ActivePage == TabSheetInputPassword ) {

	txtInputMemPassword->Width = TabSheetInputPassword->Width - 16;
	txtInputMemPassword->Left = TabSheetInputPassword->Width/2 - txtInputMemPassword->Width/2;
	txtInputMemPassword->Top = TabSheetInputPassword->Height/2 - txtInputMemPassword->Height/2 - 16;

	txtInputMemConfirmPassword->Width = txtInputMemPassword->Width;
	txtInputMemConfirmPassword->Left = txtInputMemPassword->Left;
	txtInputMemConfirmPassword->Top = TabSheetInputPassword->Height/2 + 16;

	txtInputMemPassword->SetFocus();

}
//-----------------------------------
//日時入力パネル
//-----------------------------------
else if (PageControl1->ActivePage == TabSheetInsertDate ) {

	imgDateTime->Left = 16;
	imgDateTime->Top = TabSheetInsertDate->Height/2 - imgDateTime->Height/2;

	ComboDateTime->Left = imgDateTime->BoundsRect.Right + 8;
	ComboDateTime->Top = TabSheetInsertDate->Height/2 - ComboDateTime->Height/2;
	ComboDateTime->Width = TabSheetInsertDate->Width - ComboDateTime->Left - 16;

	lblInsertDateTime->Top = ComboDateTime->Top - lblInsertDateTime->Height - 8;
	lblInsertDateTime->Left = ComboDateTime->Left;
	lblInsertDateTime->Width = ComboDateTime->Width;

	lblExampleDateTime->Left = ComboDateTime->Left;
	lblExampleDateTime->Top = ComboDateTime->BoundsRect.Bottom + 8;
	lblExampleDateTime->Width = ComboDateTime->Width;

}
//-----------------------------------
//数値入力パネル
//-----------------------------------
else{

	imgInsertNum->Left = 16;
	imgInsertNum->Top = TabSheetInsertNum->Height/2 - imgInsertNum->Height/2;
	UpDown1->Left = TabSheetInsertNum->Width - UpDown1->Width - 16;
	UpDown1->Top = TabSheetInsertNum->Height/2 - UpDown1->Height/2;
	txtNum->Top = TabSheetInsertNum->Height/2 - txtNum->Height/2;
	txtNum->Left = imgInsertNum->BoundsRect.Right + 8;
	txtNum->Width = UpDown1->Left - txtNum->Left - 2;

}

cmdOK->Left = Panel1->Width/2 - cmdOK->Width - 8;
cmdCancel->Left = Panel1->Width/2 + 8;


}
//---------------------------------------------------------------------------
void __fastcall TForm4::cmdCancelClick(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm4::cmdOKClick(TObject *Sender)
{

String InsFormatText = "";
String MsgErrorString;

//-----------------------------------
//記憶パスワードの入力タブシート
//-----------------------------------
if ( PageControl1->ActivePageIndex == 0 ) {

	if ( AnsiString(txtInputMemConfirmPassword->Text).Length() > 56 ){
		//'パスワードに使用できる文字数は半角56文字（全角28文字）以内です。'
		MsgErrorString = LoadResourceString(&Msgunit4::_MSG_ERROR_PASSWORD_MAX_NUM);
		MessageDlg(MsgErrorString, mtError, TMsgDlgButtons() << mbOK, 0);
		txtInputMemConfirmPassword->SetFocus();
		txtInputMemConfirmPassword->SelectAll();
		return;
	}

	if ( (AnsiString)txtInputMemPassword->Text != (AnsiString)txtInputMemConfirmPassword->Text ){
		//'入力されたパスワードとちがいます。'+#13+
		//'再度パスワードを入力してください。'
		MsgErrorString = LoadResourceString(&Msgunit4::_MSG_ERROR_INPUT_PASSWORD_MISMATCH);
		MessageDlg(MsgErrorString, mtError, TMsgDlgButtons() << mbOK, 0);

		txtInputMemConfirmPassword->SetFocus();
		txtInputMemConfirmPassword->SelectAll();
		return;
	}

	if ( InputType == 0 ){         //暗号化パスワードの記録

		//OKボタンが押されるまではまだ仮
		Form3->TempMyEncodePassword = (AnsiString)txtInputMemConfirmPassword->Text;
		//文字数分だけ"*"を表示
		Form3->btneditMyEncPassword->Text =
			String::StringOfChar('*', Form3->TempMyEncodePassword.Length());
		Form3->chkMyEncPasswordKeep->Checked = true;

	}
	else if ( InputType == 1 ){    //復号パスワードの記録

		//OKボタンが押されるまではまだ仮
		Form3->TempMyDecodePassword = (AnsiString)txtInputMemConfirmPassword->Text;
		//文字数分だけ"*"を表示
		Form3->btneditMyDecPassword->Text =
			String::StringOfChar('*', Form3->TempMyDecodePassword.Length());
		Form3->chkMyDecPasswordKeep->Checked = true;

	}//end if;

	Close();

}
//-----------------------------------
// 書式入力タブシート
//-----------------------------------
else {

	switch(InputType){

	case 2:	//日付書式の挿入
		InsFormatText = "<date:"+ComboDateTime->Items->Strings[ComboDateTime->ItemIndex]+">";
		break;

	case 3:	//連番書式の挿入
		InsFormatText = "<number:" + IntToStr(UpDown1->Position) + ">";
		break;

	case 4:	//ランダム文字列数の挿入
		InsFormatText = "<fend:" + IntToStr(UpDown1->Position) + ">";
		break;

	case 5:	//ファイル名の先頭文字数の挿入
		InsFormatText = "<fhead:" + IntToStr(UpDown1->Position) + ">";
		break;

	case 6:	//ファイル名の末尾文字数の挿入
		InsFormatText = "<fend:" + IntToStr(UpDown1->Position) + ">";
		break;

	default:
		break;
	}

	Form3->btneditAutoNameFormatText->SelText = InsFormatText;
	Close();

}

}
//---------------------------------------------------------------------------
void __fastcall TForm4::txtInputMemPasswordChange(TObject *Sender)
{

if ( txtInputMemPassword->Text.Length() > 0 ){
	txtInputMemConfirmPassword->Enabled = true;
	txtInputMemConfirmPassword->Color = clWindow;
	cmdOK->Enabled = true;
}
else{
	txtInputMemConfirmPassword->Enabled = false;
	txtInputMemConfirmPassword->Color = clBtnFace;
	cmdOK->Enabled = false;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm4::txtInputMemPasswordKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{

if ( Key == VK_RETURN ){
	if ( txtInputMemConfirmPassword->Enabled == true ){
		txtInputMemConfirmPassword->SetFocus();
	}
}

}
//---------------------------------------------------------------------------
void __fastcall TForm4::txtInputMemConfirmPasswordKeyDown(TObject *Sender, WORD &Key,
					TShiftState Shift)
{

if ( Key == VK_RETURN ){
	cmdOKClick(Sender);
}

}
//---------------------------------------------------------------------------

