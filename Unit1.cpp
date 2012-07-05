//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop



#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#pragma link "HTMLHelpViewer"

TForm1 *Form1;
TDragAndDrop *DragAndDropTarget;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{

int i;

//ページコントロールのタブを非表示に
PageControl1->Align = alClient;
for (i = 0; i < PageControl1->PageCount; i++) {
	PageControl1->Pages[i]->TabVisible = false;
}
//メインパネル表示
PageControl1->ActivePageIndex = 0;

PaintBoxMain->Align = alClient;
PaintBoxEncrypt->Align = alClient;
PaintBoxConfirm->Align = alClient;
PaintBoxDecrypt->Align = alClient;

//動作設定インスタンスの作成
opthdl = new TAttacheCaseOptionHandle();

//パスワードファイル関係
PasswordFileHeader = "";
PasswordFileHash = "";
for (i = 0; i < 32; i++) {
	password_hash[i] = 0;
	temp_password_hash[i] = 0;
}

//各コンポーネントの配置
SetFormComponent(NULL);

String IniFilePath = "";
FileList = new TStringList;

lblMain->Caption = LoadResourceString(&Msgunit1::_DRAG_AND_DROP_HERE);
txtEncryptPassword->EditLabel->Caption = LoadResourceString(&Msgunit1::_INPUT_PASSWORD);
chkExeFileOut->Caption = LoadResourceString(&Msgunit1::_CHECK_BOX_EXEFILE_OUT);
txtPasswordConfirm->EditLabel->Caption = LoadResourceString(&Msgunit1::_CONFIRM_PASSWORD);
chkExeFileOutConf->Caption = LoadResourceString(&Msgunit1::_CHECK_BOX_EXEFILE_OUT);
txtDecryptPassword->EditLabel->Caption = LoadResourceString(&Msgunit1::_INPUT_DECRYPT_PASSWORD);

//-----------------------------------
//ダイアログ周り
//-----------------------------------

//'暗号化するファイルを開く'
OpenDialogEncrypt->Title = LoadResourceString(&Msgunit1::_DIALOG_OPEN_FILES_FOR_ENCRYPTION_TITLE);
//'すべてのファイル(*.*)|*.*'
OpenDialogEncrypt->Filter = LoadResourceString(&Msgunit1::_DIALOG_OPEN_FILES_FOR_ENCRYPTION_FILTER);
//'復号するアタッシェケースファイルを開く'
OpenDialogDecrypt->Title = LoadResourceString(&Msgunit1::_DIALOG_OPEN_FILES_FOR_DECRYPTION_TITLE);
//'アタッシェケースファイル(*.atc)|*.atc|すべてのファイル(*.*)|*.*';
OpenDialogDecrypt->Filter = LoadResourceString(&Msgunit1::_DIALOG_OPEN_FILES_FOR_DECRYPTION_FINTER);
//'暗号化されたアタッシェケースファイル名の入力';
SaveDialog1->Title = LoadResourceString(&Msgunit1::_DIALOG_SAVE_FILES_FOR_ENCRYPTION_TITLE);
//'アタッシェケースファイル(*.atc)|*.atc|実行形式ファイル(*.exe)|(*.exe)|すべてのファイル(*.*)|*.*';
SaveDialog1->Filter = LoadResourceString(&Msgunit1::_DIALOG_SAVE_FILES_FOR_ENCRYPTION_FILTER);

//-----------------------------------
//動作設定を読み込む
//-----------------------------------

// コマンドライン引数がある
if ( ParamCount() > 0){

	opthdl->LoadOptionDataFromParamString(FileList);

	//ファイルリストがあれば、その同ディレクトリにINIファイルがあるか？
	if ( FileList->Count > 0 ){

		if ( DirectoryExists(FileList->Strings[0]) == true ){
			//フォルダならその中にあるファイル
			IniFilePath = IncludeTrailingPathDelimiter(FileList->Strings[0])+INI_FILE_NAME;
		}
		else {
			//ファイルなら同じディレクトリから
			IniFilePath = IncludeTrailingPathDelimiter(ExtractFileDir(FileList->Strings[0]))+INI_FILE_NAME;
		}

		// INIファイルがあればそこから、なければレジストリから読み込む
		opthdl->LoadOptionData(IniFilePath);

	}
	//ファイルリストはない
	else{
		// レジストリから読み込む
		opthdl->LoadOptionData("");
	}

	//もう一度、その上からコマンドライン引数を上書きする
	opthdl->LoadOptionDataFromParamString(FileList);

}
else{

	//-----------------------------------
	// INIファイルがあればそこから、
	// なければレジストリから読み込む
	//-----------------------------------

	//カレントディレクトリに設定INIファイルがあるか？
	String IniFilePath = IncludeTrailingPathDelimiter(GetCurrentDir())+INI_FILE_NAME;

	if ( FileExists(IniFilePath) == false ){
		//なければ本体ディレクトリから
		IniFilePath = IncludeTrailingPathDelimiter(ExtractFileDir(Application->ExeName))+INI_FILE_NAME;
	}

	opthdl->LoadOptionData(IniFilePath);

}

//-----------------------------------
//ウィンドウポジション
//-----------------------------------
this->Width = opthdl->FormWidth;
this->Height = opthdl->FormHeight;
this->Top = opthdl->FormTop;
this->Left = opthdl->FormLeft;
if ( opthdl->WinStyle == 1 ) {     	//最小化
	Application->Minimize();
}
else if ( opthdl->WinStyle == 2 ) {	//最大化
	this->WindowState = wsMaximized;
}
else{
	this->WindowState = wsNormal;
}


}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{

//-----------------------------------
//ウィンドウポジションなどを記憶する
//-----------------------------------
opthdl->FormWidth = this->Width;
opthdl->FormHeight = this->Height;
opthdl->FormTop = this->Top;
opthdl->FormLeft = this->Left;
if ( this->WindowState == wsMinimized ){       //最小化
	opthdl->WinStyle = 1;
}
else if ( Form1->WindowState == wsMaximized ){ //最大化
	opthdl->WinStyle = 2;
}
else{
	opthdl->WinStyle = 0;
}

delete opthdl;

delete FileList;  //処理するファイルリスト

//-----------------------------------
//OLEドラッグ＆ドロップの後始末
//-----------------------------------
RevokeDragDrop(Form1->Handle);
DragAndDropTarget->Release();
OleUninitialize();

}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormShow(TObject *Sender)
{

//-----------------------------------
//ドラッグ＆ドロップクラス
//-----------------------------------
OleInitialize(NULL);
//FormをCreateしてからでないと呼び出せない
DragAndDropTarget = new TDragAndDrop(Form1);
DragAndDropTarget->FilesDragOver = FilesDragOver;
DragAndDropTarget->FilesDragLeave = FilesDragLeave;
DragAndDropTarget->FilesDragEnd = FilesDragEnd;
RegisterDragDrop(Form1->Handle, (IDropTarget*)DragAndDropTarget);

//コマンドライン引数にファイルが投げ込まれてきている
if ( FileList->Count > 0 ) {
	//暗号化/復号処理を自動判定し実行する
	DoExecute(FileList);
}
else{
	//メインパネルを通常表示
	PageControl1->ActivePage = TabSheetMain;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{

if (Key == VK_ESCAPE) {
	Close();
}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{

SetFormComponent(Sender);

}
//---------------------------------------------------------------------------
//フォーム内コンポーネントを配置する
//---------------------------------------------------------------------------
void __fastcall TForm1::SetFormComponent(TObject *Sender)
{

//-----------------------------------
//デスクトップで最前面にウィンドウを表示するか
//-----------------------------------
if ( opthdl->fWindowForeground == true ){
	this->FormStyle = fsStayOnTop;
}
else{
	this->FormStyle = fsNormal;
}

//-----------------------------------
//メインパネル
//-----------------------------------
lblMain->Left = Form1->Width/2 - lblMain->Width/2;
lblMain->Top = TabSheetMain->Height/2 - lblMain->Height/2;

imgOptionPanel->Picture = imgOptionIcon->Picture;

//-----------------------------------
//暗号化パスワード入力パネル
//-----------------------------------
txtEncryptPassword->Left = 24;
txtEncryptPassword->Top = TabSheetInputEncPass->Height/2 - txtEncryptPassword->Height;
txtEncryptPassword->Width = TabSheetInputEncPass->Width - txtEncryptPassword->Left*2;

//「*」で隠さずパスワードを確認しながら入力する
if ( opthdl->fNoHidePassword == true ){
	txtEncryptPassword->PasswordChar = NULL;
}
else{
	txtEncryptPassword->PasswordChar = '*';
}

chkExeFileOut->Top = txtEncryptPassword->BoundsRect.Bottom + 8;
chkExeFileOut->Left = txtEncryptPassword->Left;
chkExeFileOut->Width = txtEncryptPassword->Width;
if ( Sender == NULL) {
	//リサイズイベントでは変更しない
	chkExeFileOut->Checked = opthdl->fSaveToExeout;       //常に自己実行形式で出力する
}
chkExeFileOut->Visible = opthdl->fShowExeoutChkBox;   //メインフォームにチェックボックスを表示する

cmdEncryptPasswordCancel->Top = chkExeFileOut->BoundsRect.Bottom + 8;
cmdEncryptPasswordCancel->Left = chkExeFileOut->BoundsRect.Right - cmdEncryptPasswordCancel->Width;

cmdEncryptPasswordOK->Top = cmdEncryptPasswordCancel->Top;
cmdEncryptPasswordOK->Left = cmdEncryptPasswordCancel->Left - cmdEncryptPasswordOK->Width - 8;

//戻るボタン
imgBackFromInputEncPass->Picture = imgBackNormal->Picture;

//-----------------------------------
//暗号化パスワード再確認パネル
//-----------------------------------
txtPasswordConfirm->Left = 24;
txtPasswordConfirm->Top = TabSheetInputEncPassConfirm->Height/2 - txtPasswordConfirm->Height;
txtPasswordConfirm->Width = TabSheetInputEncPassConfirm->Width - txtPasswordConfirm->Left*2;

//「*」で隠さずパスワードを確認しながら入力する
if ( opthdl->fNoHidePassword == true ){
	txtPasswordConfirm->PasswordChar = NULL;
}
else{
	txtPasswordConfirm->PasswordChar = '*';
}

chkExeFileOutConf->Top = txtPasswordConfirm->BoundsRect.Bottom + 8;
chkExeFileOutConf->Left = txtPasswordConfirm->Left;
chkExeFileOutConf->Width = txtPasswordConfirm->Width;
//chkExeFileOut->Checked = true;	//chkExeFileOutのクリックイベントで制御する
chkExeFileOutConf->Visible = opthdl->fShowExeoutChkBox;   //メインフォームにチェックボックスを表示する

cmdConfirmCancel->Top = chkExeFileOutConf->BoundsRect.Bottom + 8;
cmdConfirmCancel->Left = chkExeFileOutConf->BoundsRect.Right - cmdConfirmCancel->Width;

cmdConfirmOK->Top = cmdConfirmCancel->Top;
cmdConfirmOK->Left = cmdConfirmCancel->Left - cmdConfirmOK->Width - 8;

//戻るボタン
imgBackFromInputEncPassConfirm->Picture = imgBackNormal->Picture;


//-----------------------------------
//復号パスワード入力パネル
//-----------------------------------
txtDecryptPassword->Left = 24;
txtDecryptPassword->Top = TabSheetInputDecPass->Height/2 - txtDecryptPassword->Height;
txtDecryptPassword->Width = TabSheetInputDecPass->Width - txtDecryptPassword->Left*2;

//「*」で隠さずパスワードを確認しながら入力する
if ( opthdl->fNoHidePassword == true ){
	txtDecryptPassword->PasswordChar = NULL;
}
else{
	txtDecryptPassword->PasswordChar = '*';
}

cmdDecryptPasswordCancel->Top = txtDecryptPassword->BoundsRect.Bottom + 8;
cmdDecryptPasswordCancel->Left =
	txtDecryptPassword->BoundsRect.Right - cmdDecryptPasswordCancel->Width;

cmdDecryptPasswordOK->Top = cmdDecryptPasswordCancel->Top;
cmdDecryptPasswordOK->Left = cmdDecryptPasswordCancel->Left - cmdDecryptPasswordOK->Width - 8;

//戻るボタン
imgBackFromInputDecPass->Picture = imgBackNormal->Picture;


//-----------------------------------
//実行パネル
//-----------------------------------
ProgressBar1->Left = 24;
ProgressBar1->Top = TabSheetExecute->Height/2 - ProgressBar1->Height;
ProgressBar1->Width =TabSheetExecute->Width - ProgressBar1->Left*2;

lblMsg->Left = ProgressBar1->Left;
lblMsg->Top = ProgressBar1->Top - lblMsg->Height*2 - 8;

lblProgressPercentNum->Top = ProgressBar1->Top + ProgressBar1->Height + 8;
lblProgressPercentNum->Left =
	ProgressBar1->Left + ProgressBar1->Width/2 - lblProgressPercentNum->Width/2;

lblStatus->Left = ProgressBar1->Left;
lblStatus->Top = lblMsg->Top - lblStatus->Height - 8;

cmdCancel->Top = ProgressBar1->BoundsRect.Bottom + 8;
cmdCancel->Left = ProgressBar1->BoundsRect.Right - cmdCancel->Width;

//戻るボタン
imgBackFromExecute->Picture = imgBackNormal->Picture;

if (TimerEncrypt->Enabled == false &&
		TimerDecrypt->Enabled == false && TimerDelete->Enabled == false) {
	imgBackFromExecute->Visible = true;
}
else{
	imgBackFromExecute->Visible = false;
}


}
//---------------------------------------------------------------------------
void __fastcall TForm1::PaintBoxMainMouseEnter(TObject *Sender)
{

//点線枠を表示する

#define DOT_HEIGHT 8
#define DOT_WIDTH 8

int PosX, PosY;

this->Canvas->Pen->Color = TColor(RGB(200,200,200));
this->Canvas->Brush->Color = TColor(RGB(200,200,200));

PosY = PageControl1->BoundsRect.Bottom;
for (PosX = 0; PosX < this->Width; PosX+=24) {
	this->Canvas->Rectangle(PosX, 0, PosX+16, DOT_HEIGHT);
	this->Canvas->Rectangle(PosX, PosY, PosX+16, PosY+DOT_HEIGHT);
}

PosX = PageControl1->BoundsRect.Right;
for (PosY = 0; PosY < this->Height; PosY+=24) {
	this->Canvas->Rectangle(0, PosY, DOT_WIDTH, PosY+16);
	this->Canvas->Rectangle(PosX, PosY, PosX+DOT_WIDTH, PosY+16);
}


}
//---------------------------------------------------------------------------
void __fastcall TForm1::PaintBoxMainMouseLeave(TObject *Sender)
{

//点線枠をクリア
this->Canvas->Brush->Color = clBtnFace;
this->Canvas->FillRect(Rect(0, 0, this->Width, this->Height));

}
//---------------------------------------------------------------------------
// DragOver イベントのコールバック関数
//---------------------------------------------------------------------------
void __fastcall TForm1::FilesDragOver(POINTL ptl)
{

int px, py, pr, pb;
TRect rcResult;
TRect rcMouse = TRect(ptl.x-1, ptl.y-1, ptl.x+1, ptl.y+1);   //マウス位置
TRect rcTarget;

//-----------------------------------
// メインパネル
//-----------------------------------
if ( PageControl1->ActivePage == TabSheetMain ) {

	this->Color = TColor(RGB(80,80,80));
	DragAndDropTarget->DropImageType = DROPIMAGE_COPY;

}
//-----------------------------------
// パスワード入力パネル
//-----------------------------------
else if ( PageControl1->ActivePage == TabSheetInputEncPass ) {

	//パスワードファイルを許可する
	if ( opthdl->fAllowPassFile == true ) {

		DragAndDropTarget->fMultipleFilesOk = false;
		DragAndDropTarget->DropImageType = DROPIMAGE_COPY;

		//パスワード入力ボックス
		px = txtEncryptPassword->Left;
		py = txtEncryptPassword->Top;
		pr = txtEncryptPassword->BoundsRect.Right;
		pb = txtEncryptPassword->BoundsRect.Bottom;
		rcTarget = Rect(ClientToScreen(TPoint(px, py)), ClientToScreen(TPoint(pr, pb)));

		if (IntersectRect(rcResult, rcMouse, rcTarget) == true ) {
			txtEncryptPassword->Color = TColor(0xDBEBF7);  //オレンジ色
		}
		else{
			txtEncryptPassword->Color = clBtnFace;
		}

	}
	else{
		DragAndDropTarget->DropImageType = DROPIMAGE_NONE;
	}

}
//-----------------------------------
// パスワード再確認パネル
//-----------------------------------
else if ( PageControl1->ActivePage == TabSheetInputEncPassConfirm ) {

	//パスワードファイルを許可する
	if ( opthdl->fAllowPassFile == true ) {

		DragAndDropTarget->fMultipleFilesOk = false;
		DragAndDropTarget->DropImageType = DROPIMAGE_COPY;

		//パスワード入力ボックス
		px = txtPasswordConfirm->Left;
		py = txtPasswordConfirm->Top;
		pr = txtPasswordConfirm->BoundsRect.Right;
		pb = txtPasswordConfirm->BoundsRect.Bottom;
		rcTarget = Rect(ClientToScreen(TPoint(px, py)), ClientToScreen(TPoint(pr, pb)));

		if (IntersectRect(rcResult, rcMouse, rcTarget) == true ) {
			txtPasswordConfirm->Color = TColor(0x99ccff);  //より濃いオレンジ色
		}
		else{
			txtPasswordConfirm->Color = TColor(0xDBEBF7);  //オレンジ色に戻す
		}

	}
	else{
		DragAndDropTarget->DropImageType = DROPIMAGE_NONE;
	}

}
//-----------------------------------
// 復号パスワード入力パネル
//-----------------------------------
else if ( PageControl1->ActivePage == TabSheetInputDecPass ) {

	//パスワードファイルを許可する
	if ( opthdl->fAllowPassFile == true ) {

		DragAndDropTarget->fMultipleFilesOk = false;
		DragAndDropTarget->DropImageType = DROPIMAGE_COPY;

		//パスワード入力ボックス
		px = txtDecryptPassword->Left;
		py = txtDecryptPassword->Top;
		pr = txtDecryptPassword->BoundsRect.Right;
		pb = txtDecryptPassword->BoundsRect.Bottom;
		rcTarget = Rect(ClientToScreen(TPoint(px, py)), ClientToScreen(TPoint(pr, pb)));

		if (IntersectRect(rcResult, rcMouse, rcTarget) == true ) {
			txtDecryptPassword->Color = TColor(0xDBEBF7);  //オレンジ色
		}
		else{
			txtDecryptPassword->Color = clBtnFace;
		}

	}
	else{
		DragAndDropTarget->DropImageType = DROPIMAGE_NONE;
	}

}
//-----------------------------------
// 実行中パネル
//-----------------------------------
else if ( PageControl1->ActivePage == TabSheetExecute ) {
	//実行中はドロップを受け付けない
	DragAndDropTarget->DropImageType = DROPIMAGE_NONE;

}

}
//---------------------------------------------------------------------------
// DragLeave イベントのコールバック関数
//---------------------------------------------------------------------------
void __fastcall TForm1::FilesDragLeave(void)
{

//-----------------------------------
// メインパネル
//-----------------------------------
if ( PageControl1->ActivePage == TabSheetMain ) {
	//メインフォーム
	this->Color = clBtnFace;
}
//-----------------------------------
// パスワード入力パネル
//-----------------------------------
else if ( PageControl1->ActivePage == TabSheetInputEncPass ) {
	txtEncryptPassword->Color = clBtnFace;
}
//-----------------------------------
// パスワード再確認パネル
//-----------------------------------
else if ( PageControl1->ActivePage == TabSheetInputEncPassConfirm ) {
	txtPasswordConfirm->Color = TColor(0xDBEBF7);  //オレンジ色に戻す
}
//-----------------------------------
// 復号パスワード入力パネル
//-----------------------------------
else if ( PageControl1->ActivePage == TabSheetInputDecPass ) {
	txtDecryptPassword->Color = clBtnFace;
}
//-----------------------------------
// 実行中パネル
//-----------------------------------
else if ( PageControl1->ActivePage == TabSheetExecute ) {

}

}
//---------------------------------------------------------------------------
// DragEnd イベントのコールバック関数
//---------------------------------------------------------------------------
void __fastcall TForm1::FilesDragEnd(POINTL ptl, String FileListText)
{

int px, py, pr, pb;
TRect rcResult;
TRect rcMouse = TRect(ptl.x-1, ptl.y-1, ptl.x+1, ptl.y+1);   //マウス位置
TRect rcTarget;

String MsgText;

String SHA1HashPassword = "";
TStringList *DropFileList = new TStringList;
DropFileList->Text = FileListText;

//再確認用のパスワードハッシュ
AnsiString TempPasswordFileHeader;
AnsiString TempPasswordFileHash;

//-----------------------------------
// メインパネル
//-----------------------------------
if ( PageControl1->ActivePage == TabSheetMain ) {

	//メインフォーム
	this->Color = clBtnFace;

	//ドラッグ＆ドロップされたファイルリストを取得する
	FileList->Text = FileListText;

	//-----------------------------------
	//暗号化/復号処理を自動判定し実行する
	//-----------------------------------
	if (FileList->Count > 0) {
		DoExecute(FileList);
	}

}
//-----------------------------------
// パスワード入力パネル
//-----------------------------------
else if ( PageControl1->ActivePage == TabSheetInputEncPass ) {

	txtEncryptPassword->Color = clBtnFace;

	//パスワードファイルを許可する
	if ( opthdl->fAllowPassFile == true ) {

		DragAndDropTarget->fMultipleFilesOk = false;
		DragAndDropTarget->DropImageType = DROPIMAGE_COPY;

		//パスワード入力ボックス
		px = txtEncryptPassword->Left;
		py = txtEncryptPassword->Top;
		pr = txtEncryptPassword->BoundsRect.Right;
		pb = txtEncryptPassword->BoundsRect.Bottom;
		rcTarget = Rect(ClientToScreen(TPoint(px, py)), ClientToScreen(TPoint(pr, pb)));

		if (IntersectRect(rcResult, rcMouse, rcTarget) == true && DropFileList->Count > 0) {

			//SHA-1ハッシュを求める
			if ( opthdl->GetSHA1HashFromFile(
				DropFileList->Strings[0], password_hash, PasswordFileHeader, PasswordFileHash ) == true ){
				txtEncryptPassword->Text = PasswordFileHash.SetLength(32);
				//'パスワードに以下のファイルのハッシュ値が入力されました'
				BalloonHint1->Title = LoadResourceString(&Msgunit1::_BALLOON_HINT_PASSWORD_HASH_TITLE);
				//そのファイルパスを表示する
				BalloonHint1->Description = DropFileList->Strings[0];
				BalloonHint1->ShowHint(txtEncryptPassword->ClientToScreen(CenterPoint(txtEncryptPassword->ClientRect)));
				txtEncryptPassword->SetFocus();
				txtEncryptPassword->SelectAll();
			}
			else{
				//'パスワードファイルを開けません。他のアプリケーションで使用中の可能性があります。';
				MsgText = LoadResourceString(&Msgunit1::_MSG_ERROR_OPEN_PASSWORD_FILE)+"\n"+
																		 DropFileList->Strings[0];
				MessageDlg(MsgText, mtError, TMsgDlgButtons() << mbOK, 0);
			}
		}

	}
	else{
		DragAndDropTarget->DropImageType = DROPIMAGE_NONE;
	}

}
//-----------------------------------
// パスワード再確認パネル
//-----------------------------------
else if ( PageControl1->ActivePage == TabSheetInputEncPassConfirm ) {

	txtPasswordConfirm->Color = TColor(0xDBEBF7);  //オレンジ色に戻す

	//パスワードファイルを許可する
	if ( opthdl->fAllowPassFile == true ) {

		DragAndDropTarget->fMultipleFilesOk = false;
		DragAndDropTarget->DropImageType = DROPIMAGE_COPY;

		//パスワード入力ボックス
		px = txtPasswordConfirm->Left;
		py = txtPasswordConfirm->Top;
		pr = txtPasswordConfirm->BoundsRect.Right;
		pb = txtPasswordConfirm->BoundsRect.Bottom;
		rcTarget = Rect(ClientToScreen(TPoint(px, py)), ClientToScreen(TPoint(pr, pb)));

		if (IntersectRect(rcResult, rcMouse, rcTarget) == true && DropFileList->Count > 0) {
			//SHA-1ハッシュを求める
			if ( opthdl->GetSHA1HashFromFile(
				DropFileList->Strings[0], temp_password_hash, TempPasswordFileHeader, TempPasswordFileHash ) == true ){
				txtPasswordConfirm->Text = TempPasswordFileHash.SetLength(32);
				//即座に実行（実行先でパスワードの正否チェックが行われる）
				cmdConfirmOKClick((TObject*)0);
			}
			else{
				//'パスワードファイルを開けません。他のアプリケーションで使用中の可能性があります。';
				MsgText = LoadResourceString(&Msgunit1::_MSG_ERROR_OPEN_PASSWORD_FILE)+"\n"+
																		 DropFileList->Strings[0];
				MessageDlg(MsgText, mtError, TMsgDlgButtons() << mbOK, 0);
			}
		}

	}
	else{
		DragAndDropTarget->DropImageType = DROPIMAGE_NONE;
	}

}
//-----------------------------------
// 復号パスワード入力パネル
//-----------------------------------
else if ( PageControl1->ActivePage == TabSheetInputDecPass ) {

	txtDecryptPassword->Color = clBtnFace;

	//パスワードファイルを許可する
	if ( opthdl->fAllowPassFile == true ) {

		DragAndDropTarget->fMultipleFilesOk = false;
		DragAndDropTarget->DropImageType = DROPIMAGE_COPY;

		//パスワード入力ボックス
		px = txtDecryptPassword->Left;
		py = txtDecryptPassword->Top;
		pr = txtDecryptPassword->BoundsRect.Right;
		pb = txtDecryptPassword->BoundsRect.Bottom;
		rcTarget = Rect(ClientToScreen(TPoint(px, py)), ClientToScreen(TPoint(pr, pb)));

		if (IntersectRect(rcResult, rcMouse, rcTarget) == true && DropFileList->Count > 0) {
			//SHA-1ハッシュを求める
			if ( opthdl->GetSHA1HashFromFile(
				DropFileList->Strings[0], password_hash, PasswordFileHeader, PasswordFileHash ) == true ){
				txtDecryptPassword->Text = PasswordFileHash;
				cmdDecryptPasswordOKClick((TObject*)0);
			}
			else{
				//'パスワードファイルを開けません。他のアプリケーションで使用中の可能性があります。';
				MsgText = LoadResourceString(&Msgunit1::_MSG_ERROR_OPEN_PASSWORD_FILE)+"\n"+
																		 DropFileList->Strings[0];
				MessageDlg(MsgText, mtError, TMsgDlgButtons() << mbOK, 0);
			}
		}

	}
	else{
		DragAndDropTarget->DropImageType = DROPIMAGE_NONE;
	}

}
//-----------------------------------
// 実行中パネル
//-----------------------------------
else if ( PageControl1->ActivePage == TabSheetExecute ) {

}

delete DropFileList;


}
//---------------------------------------------------------------------------
// 暗号化ファイルのヘッダ判定
//---------------------------------------------------------------------------
bool __fastcall TForm1::CheckAtcFileHeader(TStringList *FileList)
{

int i;
String FilePath;

TFileStream *fs;
char token[17];
ZeroMemory(token, 17);

//判定するファイルリストがすべてATCファイル形式であれば「復号＝true」を返す
bool fDecrypt = true;

//-----------------------------------
// ファイルヘッダを判定
//-----------------------------------
for (i = 0; i < FileList->Count; i++) {

	FilePath = FileList->Strings[i];

	if ( FileExists(FilePath) == true ) {

		try{
			fs = new TFileStream(FilePath, fmOpenRead);
		}
		catch(...){
			//ファイルが開けない場合は無視
			return(false);
		}

		fs->Seek((__int64)4, TSeekOrigin::soBeginning);
		fs->Read(token, 16);
		delete fs;

		//暗号化ファイル
		if ( StrComp(token, "_AttacheCaseData") == 0 ) {

		}
		//破壊されたファイル
		else if ( StrComp(token, "_Atc_Broken_Data") == 0 ) {
			return(true);	//復号にして実行処理させてエラーをはかせるため
		}
		//通常のファイル
		else{
			fDecrypt = false;	//1つでもatcファイル以外があれば「暗号化」処理
		}

	}
	else{
		//ディレクトリ
		fDecrypt = false;
	}

}//end for (i = 0; i < FileList->Count; i++);

return(fDecrypt);

}
//---------------------------------------------------------------------------
//ファイルの判別を行った後に処理を分けて実行
//---------------------------------------------------------------------------
void __fastcall TForm1::DoExecute(TStringList *FileList)
{

int i;
String MsgText;
String FilePath;

int CryptTypeNum = 0;	// 1:暗号化(TYPE_CRYPT_ENCRYPT), 2:復号(TYPE_CRYPT_DECRYPT)

int res;
TForm* dlgconf;
TButton *btn;

TFileStream *fs;
char token[17];
ZeroMemory(token, 17);

FileListPosition = 0;

//-----------------------------------
//暗号/復号処理かを問い合わせる
//-----------------------------------
if ( opthdl->fAskEncDecode == true ) {

	//'ドラッグ＆ドロップされたファイルまたはフォルダーをどのように処理しますか？'
	MsgText = LoadResourceString(&Msgunit1::_MSG_CONFIRM_ENCRYPT_OR_DECRYPT)+"\n"+
									 FileList->Strings[0];

	if ( FileList->Count > 1) {
		MsgText = MsgText + "\n..." + IntToStr(FileList->Count) + " files.";
	}

	dlgconf = CreateMessageDialog(MsgText, mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo<<mbCancel, mbCancel);
	dlgconf->Caption = LoadResourceString(&Msgunit1::_MSG_CAPTION_CONFIRMATION);	//'確認'
	//ボタン名をそれぞれ書き換える
	btn = (TButton*)dlgconf->FindComponent("Yes");        //'暗号化する(&E)'
	btn->Caption = LoadResourceString(&Msgunit1::_BUTTON_CAPTION_ENCRYPTION);
	btn = (TButton*)dlgconf->FindComponent("No");         //'復号する(&D)'
	btn->Caption = LoadResourceString(&Msgunit1::_BUTTON_CAPTION_DECRYPTION);
	btn = (TButton*)dlgconf->FindComponent("Cancel");     //'キャンセル(&C)'
	btn->Caption = LoadResourceString(&Msgunit1::_BUTTON_CAPTION_CANCEL);
	dlgconf->ParentWindow = Form1->Handle;
	res = dlgconf->ShowModal();
	delete dlgconf;

	//-----------------------------------
	//暗号化
	if (res == mrYes) {
		CryptTypeNum = TYPE_CRYPT_ENCRYPT;
	}
	//-----------------------------------
	//復号
	else if (res == mrNo) {
		CryptTypeNum = TYPE_CRYPT_DECRYPT;
	}
	//-----------------------------------
	//キャンセル
	else {
		FileList->Clear();
		return;
	}

}
//-------------------------------------
//暗号/復号処理かを問い合わせず判定する
//-------------------------------------
else{

	CryptTypeNum = TYPE_CRYPT_DECRYPT;

	//-----------------------------------
	// ファイルヘッダを判定
	//-----------------------------------
	for (i = 0; i < FileList->Count; i++) {

		FilePath = FileList->Strings[i];

		if ( FileExists(FilePath) == true ) {

			try{
				fs = new TFileStream(FilePath, fmOpenRead);
			}
			catch(...){
				//'ファイルを開けません。他のアプリケーションで使用中の可能性があります。';
				MsgText = LoadResourceString(&Msgunit1::_MSG_FILE_OPEN_ERROR) + "\n" + FilePath;
				MessageDlg(MsgText, mtError, TMsgDlgButtons() << mbOK, 0);
				break;
			}

			fs->Seek((__int64)4, TSeekOrigin::soBeginning);
			fs->Read(token, 16);
			delete fs;

			//暗号化ファイル
			if ( StrComp(token, "_AttacheCaseData") == 0 ) {

			}
			//破壊されたファイル
			else if ( StrComp(token, "_Atc_Broken_Data") == 0 ) {
				// 'この暗号化ファイルはパスワード入力に失敗して破壊されているようです。'
				MsgText = LoadResourceString(&Msgunit1::_MSG_BROKEN_ATC_FILE) + "\n" + FilePath;
				MessageDlg(MsgText, mtInformation, TMsgDlgButtons() << mbOK, 0);
				break;
			}
			//通常のファイル
			else{
				//1つでもatcファイル以外があれば「暗号化」処理
				CryptTypeNum = TYPE_CRYPT_ENCRYPT;
			}

		}
		else{
			//ディレクトリ（暗号処理）
			CryptTypeNum = TYPE_CRYPT_ENCRYPT;
		}

	}//end for (i = 0; i < FileList->Count; i++);

}//end if ( opthdl->fAskEncDecode == true );

//-----------------------------------
// 暗号化/復号それぞれの処理実行
//-----------------------------------
//暗号化
if ( CryptTypeNum == TYPE_CRYPT_ENCRYPT ) {

	//記憶パスワードで即座に実行する
	if ( opthdl->fMemPasswordExe == true && opthdl->fMyEncodePasswordKeep == true) {
		//実行パネル表示
		PageControl1->ActivePage = TabSheetExecute;
		//再確認テキストボックスに入れてしまう
		txtPasswordConfirm->Text = opthdl->MyEncodePassword;
		//暗号化開始
		FileEncrypt();
		return;
	}
	//暗号時にパスワードファイルを自動チェックする
	else if ( opthdl->fAllowPassFile == true && opthdl->fCheckPassFile == true ) {

		if ( FileExists(opthdl->PassFilePath) == false ) {
			//パスワードファイルがない場合エラーを出さない
			if ( opthdl->fNoErrMsgOnPassFile == true ) {
				//メッセージを出さずにパスワード入力パネルへ
				PageControl1->ActivePage = TabSheetInputEncPass;
				txtEncryptPassword->SetFocus();
				return;
			}
			else{
				//'動作設定で指定された暗号化するためのパスワードファイルが見つかりません。
				//'設定を確認してください。'
				MsgText = LoadResourceString(&Msgunit1::_MSG_ERROR_NOT_FOUND_ENC_PASSWORD_FILE)+"\n"+
									opthdl->PassFilePath;
				MessageDlg(MsgText, mtInformation, TMsgDlgButtons() << mbOK, 0);
				return;
			}

		}

	}
	else{
		//パスワード入力パネルへ進む
		PageControl1->ActivePage = TabSheetInputEncPass;
		txtEncryptPassword->SetFocus();
		return;
	}

}
//-----------------------------------
//復号
else if ( CryptTypeNum == TYPE_CRYPT_DECRYPT) {

	//記憶パスワードで即座に実行する
	if ( opthdl->fMemPasswordExe == true && opthdl->fMyDecodePasswordKeep == true) {
		//実行パネル表示
		PageControl1->ActivePage = TabSheetExecute;
		FileDecrypt();
		return;
	}
	//復号時にパスワードファイルを自動チェックする
	else if ( opthdl->fAllowPassFile == true && opthdl->fCheckPassFileDecrypt == true ) {

		if ( FileExists(opthdl->PassFilePathDecrypt) == false ) {
			//パスワードファイルがない場合エラーを出さない
			if ( opthdl->fNoErrMsgOnPassFile == true ) {
				//メッセージを出さずにパスワード入力パネルへ
				PageControl1->ActivePage = TabSheetInputDecPass;
				txtDecryptPassword->SetFocus();
				return;
			}
			else{
				//'動作設定で指定された復号するパスワードファイルが見つかりません。'+#13+
				//'設定を確認してください。';
				MsgText = LoadResourceString(&Msgunit1::_MSG_ERROR_NOT_FOUND_DEC_PASSWORD_FILE)+"\n"+
									opthdl->PassFilePath;
				MessageDlg(MsgText, mtInformation, TMsgDlgButtons() << mbOK, 0);
				return;
			}

		}

	}
	else{
		//パスワード入力パネルへ進む
		PageControl1->ActivePage = TabSheetInputDecPass;
		txtDecryptPassword->SetFocus();
		return;
	}

}


}
//---------------------------------------------------------------------------
//完全削除処理実行
//---------------------------------------------------------------------------
void __fastcall TForm1::DoDeleteFile(TStringList *FileList)
{

//実行パネル表示
PageControl1->ActivePage = TabSheetExecute;

//完全削除インスタンスの作成
cmpdel = new TAttacheCaseDelete(true);
cmpdel->FileList = FileList;
cmpdel->Opt = opthdl->fCompleteDelete;	// 0:通常削除, 1:完全削除, 2:ゴミ箱
cmpdel->OnTerminate = DeleteThreadTerminated;
cmpdel->FreeOnTerminate = true;

//削除の実行
cmpdel->Start();

//タスクバー進捗表示（Win7）
if(CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_ALL, IID_ITaskbarList3, (void**)&ptl) != S_OK) {
	//失敗
}
if (ptl) ptl->SetProgressState(Application->Handle, TBPF_NORMAL);

//進捗をTimerで監視
TimerDelete->Enabled = true;


}
//---------------------------------------------------------------------------
//暗号化処理スレッドの終了
//---------------------------------------------------------------------------
void __fastcall TForm1::EncryptThreadTerminated(TObject *Sender)
{

ProgressBar1->Style = pbstNormal;

if (encrypt->ProgressPercentNum < 0) {
	ProgressBar1->Position = 0;
	lblProgressPercentNum->Caption = " - %";
}
else{
	ProgressBar1->Position = encrypt->ProgressPercentNum;
	lblProgressPercentNum->Caption = IntToStr(encrypt->ProgressPercentNum)+"%";
}

lblStatus->Caption = encrypt->ProgressStatusText;
lblMsg->Caption = encrypt->ProgressMsgText;

if(ptl){
	ptl->SetProgressValue(Application->Handle, 100, 100);
	ptl->Release();
	ptl = NULL;
}

//暗号化成功
if ( encrypt->StatusNum > 0 ) {

	//コンペア
	if ( opthdl->fCompareFile == true && FileListPosition < FileList->Count ){
		FileCompare();
		return;
	}

	//個別に暗号化するオプションでまだ処理するファイルが残っている
	if (FileListPosition < FileList->Count) {
		FileEncrypt();
		return;
	}

	TimerEncrypt->Enabled = false;

	//デバッグメッセージ
	//ShowMessage("暗号化スレッドが終了しました。");
	encrypt = NULL;

	//元ファイルの削除処理
	if ( opthdl->fDelOrgFile == true ) {
		DoDeleteFile(FileList);
		return;
	}

	//処理後に終了する
	if ( opthdl->fEndToExit == true ) {
		Application->Terminate();
	}

}
else{
	//エラーで終了してきた
	encrypt = NULL;
}


}
//---------------------------------------------------------------------------
//復号処理スレッドの終了
//---------------------------------------------------------------------------
void __fastcall TForm1::DecryptThreadTerminated(TObject *Sender)
{

ProgressBar1->Style = pbstNormal;
ProgressBar1->Position = decrypt->ProgressPercentNum;
	lblProgressPercentNum->Caption = IntToStr(decrypt->ProgressPercentNum)+"%";
lblStatus->Caption = decrypt->ProgressStatusText;
lblMsg->Caption = decrypt->ProgressMsgText;

if(ptl){
	ptl->SetProgressValue(Application->Handle, 100, 100);
	ptl->Release();
	ptl = NULL;
}

//復号に成功
if ( decrypt->StatusNum > 0 ) {

	//コンペアしてきた
	if (decrypt->fCompare == true) {
		//デバッグメッセージ
		//ShowMessage("コンペア処理スレッドが終了しました。");
		decrypt = NULL;
		//元の暗号化処理スレッドへ戻る
		EncryptThreadTerminated(Sender);
		return;
	}

	//個別に暗号化するオプションでまだ処理するファイルが残っている
	if (FileListPosition < FileList->Count) {
		FileDecrypt();
		return;
	}

	TimerDecrypt->Enabled = false;

	//デバッグメッセージ
	//ShowMessage("復号処理スレッドが終了しました。");
	decrypt = NULL;

	//暗号化ファイルの削除処理
	if ( opthdl->fDelEncFile == true ) {
		DoDeleteFile(FileList);
		return;
	}

	//処理後に終了する
	if ( opthdl->fEndToExit == true ) {
		Application->Terminate();
	}

}
else{
	//エラーで終了してきた
	decrypt = NULL;
}

}
//---------------------------------------------------------------------------
//完全削除処理スレッドの終了
//---------------------------------------------------------------------------
void __fastcall TForm1::DeleteThreadTerminated(TObject *Sender)
{

ProgressBar1->Style = pbstNormal;
ProgressBar1->Position = cmpdel->ProgressPercentNum;
	lblProgressPercentNum->Caption = IntToStr(cmpdel->ProgressPercentNum)+"%";

lblStatus->Caption = cmpdel->ProgressStatusText;
lblMsg->Caption = cmpdel->ProgressMsgText;

if(ptl){
	ptl->SetProgressValue(Application->Handle, 100, 100);
	ptl->Release();
	ptl = NULL;
}

TimerDelete->Enabled = false;

//デバッグメッセージ
//ShowMessage("完全削除処理スレッドが終了しました。");
cmpdel = NULL;

FileList->Clear();

//処理後に終了する
if ( opthdl->fEndToExit == true ) {
	Application->Terminate();
}


}
//---------------------------------------------------------------------------
void __fastcall TForm1::TimerEncryptTimer(TObject *Sender)
{

//暗号化の進捗状況を確認する

//【補足】
// TThreadクラスの Synchronize() 、Queue()
// を使うとマルチスレッドの恩恵に預かれず、極端に処理速度が低下するため
// メインスレッドからはTTimerで見張るようにしています。

if (encrypt != NULL) {

	if (encrypt->ProgressPercentNum == -1) {
		//マーキー表示
		ProgressBar1->Style = pbstMarquee;
		lblProgressPercentNum->Caption = " - %";
		if (ptl) {
			ptl->SetProgressState(Application->Handle, TBPF_INDETERMINATE);
		}

	}
	else{
		ProgressBar1->Style = pbstNormal;
		ProgressBar1->Position = encrypt->ProgressPercentNum;
		lblProgressPercentNum->Caption = IntToStr(encrypt->ProgressPercentNum)+"%";
		//タスクバー進捗表示（Win7）
		if(ptl){
			ptl->SetProgressValue(Application->Handle, encrypt->ProgressPercentNum, 100);
		}
	}
	lblStatus->Caption = encrypt->ProgressStatusText;
	lblMsg->Caption = encrypt->ProgressMsgText;

}
else{
	TimerEncrypt->Enabled = false;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::TimerDecryptTimer(TObject *Sender)
{

//復号の進捗状況を確認する

if (decrypt != NULL) {
	ProgressBar1->Position = decrypt->ProgressPercentNum;
	lblProgressPercentNum->Caption = IntToStr(decrypt->ProgressPercentNum)+"%";

	lblStatus->Caption = decrypt->ProgressStatusText;
	lblMsg->Caption = decrypt->ProgressMsgText;

	//タスクバー進捗表示（Win7）
	if(ptl){
		ptl->SetProgressValue(Application->Handle, decrypt->ProgressPercentNum, 100);
	}
}
else{
	TimerDecrypt->Enabled = false;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::TimerDeleteTimer(TObject *Sender)
{

//完全削除処理の進捗状況を確認する

if (cmpdel != NULL) {
	if (cmpdel->ProgressPercentNum == -1) {
		//マーキー表示
		ProgressBar1->Style = pbstMarquee;
		lblProgressPercentNum->Caption = " - %";
		if (ptl) {
			ptl->SetProgressState(Application->Handle, TBPF_INDETERMINATE);
		}
	}
	else{
		ProgressBar1->Style = pbstNormal;
		ProgressBar1->Position = cmpdel->ProgressPercentNum;
		lblProgressPercentNum->Caption = IntToStr(cmpdel->ProgressPercentNum)+"%";
		//タスクバー進捗表示（Win7）
		if(ptl){
			ptl->SetProgressValue(Application->Handle, cmpdel->ProgressPercentNum, 100);
		}
	}
	lblStatus->Caption = cmpdel->ProgressStatusText;
	lblMsg->Caption = cmpdel->ProgressMsgText;
}
else{
	TimerDelete->Enabled = false;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::mnuFileClick(TObject *Sender)
{

if ( PageControl1->ActivePage == TabSheetMain ){
	mnuEncryptFiles->Enabled = true;
	mnuEncryptDir->Enabled = true;
	mnuDecrypt->Enabled = true;
}
else if ( PageControl1->ActivePage == TabSheetInputEncPass ){
	mnuEncryptFiles->Enabled = true;
	mnuEncryptDir->Enabled = true;
	mnuDecrypt->Enabled = false;
}
else if ( PageControl1->ActivePage == TabSheetInputEncPassConfirm ){
	mnuEncryptFiles->Enabled = false;
	mnuEncryptDir->Enabled = false;
	mnuDecrypt->Enabled = false;
}
else if ( PageControl1->ActivePage == TabSheetInputDecPass ){
	mnuEncryptFiles->Enabled = false;
	mnuEncryptDir->Enabled = false;
	mnuDecrypt->Enabled = true;
}
else if ( PageControl1->ActivePage == TabSheetExecute ){
	mnuEncryptFiles->Enabled = false;
	mnuEncryptDir->Enabled = false;
	mnuDecrypt->Enabled = false;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::mnuEncryptFilesClick(TObject *Sender)
{

String MsgText;

if (OpenDialogEncrypt->Execute() == true) {

	FileList->Clear();
	FileList->Text = OpenDialogEncrypt->Files->Text;  //暗号化する元ファイルリスト

	//記憶パスワードで即座に実行する
	if ( opthdl->fMemPasswordExe == true && opthdl->fMyEncodePasswordKeep == true) {
		//実行パネル表示
		PageControl1->ActivePage = TabSheetExecute;
		//再確認テキストボックスに入れてしまう
		txtPasswordConfirm->Text = opthdl->MyEncodePassword;
		//暗号化開始
		FileEncrypt();
		return;
	}
	//暗号時にパスワードファイルを自動チェックする
	else if ( opthdl->fAllowPassFile == true && opthdl->fCheckPassFile == true ) {

		if ( FileExists(opthdl->PassFilePath) == false ) {
			//パスワードファイルがない場合エラーを出さない
			if ( opthdl->fNoErrMsgOnPassFile == true ) {
				//メッセージを出さずにパスワード入力パネルへ
				PageControl1->ActivePage = TabSheetInputEncPass;
				txtEncryptPassword->SetFocus();
				return;
			}
			else{
				//'動作設定で指定された暗号化するためのパスワードファイルが見つかりません。
				//'設定を確認してください。'
				MsgText = LoadResourceString(&Msgunit1::_MSG_ERROR_NOT_FOUND_ENC_PASSWORD_FILE)+"\n"+
									opthdl->PassFilePath;
				MessageDlg(MsgText, mtInformation, TMsgDlgButtons() << mbOK, 0);
				return;
			}

		}

	}
	else{
		//パスワード入力パネルへ進む
		PageControl1->ActivePage = TabSheetInputEncPass;
		txtEncryptPassword->SetFocus();
		return;
	}

}//end if (OpenDialogEncrypt->Execute() == true);

}
//---------------------------------------------------------------------------
void __fastcall TForm1::mnuEncryptDirClick(TObject *Sender)
{

String MsgText;

String DirPath;

TSelectDirExtOpts opt =
	TSelectDirExtOpts() << sdNewUI << sdNewFolder << sdShowEdit << sdValidateDir;

// "暗号化するディレクトリ（フォルダ）を選択してください。"
if (SelectDirectory(LoadResourceString(&Msgunit1::_DIALOG_SELECT_DIRECTORY_TEXT), L"", DirPath, opt) == true){

	FileList->Clear();
	FileList->Text = DirPath;

		//記憶パスワードで即座に実行する
	if ( opthdl->fMemPasswordExe == true && opthdl->fMyEncodePasswordKeep == true) {
		//実行パネル表示
		PageControl1->ActivePage = TabSheetExecute;
		//再確認テキストボックスに入れてしまう
		txtPasswordConfirm->Text = opthdl->MyEncodePassword;
		//暗号化開始
		FileEncrypt();
		return;
	}
	//暗号時にパスワードファイルを自動チェックする
	else if ( opthdl->fAllowPassFile == true && opthdl->fCheckPassFile == true ) {

		if ( FileExists(opthdl->PassFilePath) == false ) {
			//パスワードファイルがない場合エラーを出さない
			if ( opthdl->fNoErrMsgOnPassFile == true ) {
				//メッセージを出さずにパスワード入力パネルへ
				PageControl1->ActivePage = TabSheetInputEncPass;
				txtEncryptPassword->SetFocus();
				return;
			}
			else{
				//'動作設定で指定された暗号化するためのパスワードファイルが見つかりません。
				//'設定を確認してください。'
				MsgText = LoadResourceString(&Msgunit1::_MSG_ERROR_NOT_FOUND_ENC_PASSWORD_FILE)+"\n"+
									opthdl->PassFilePath;
				MessageDlg(MsgText, mtInformation, TMsgDlgButtons() << mbOK, 0);
				return;
			}

		}

	}
	else{
		//パスワード入力パネルへ進む
		PageControl1->ActivePage = TabSheetInputEncPass;
		txtEncryptPassword->SetFocus();
		return;
	}

}//end if (SelectDirectory);

}
//---------------------------------------------------------------------------
void __fastcall TForm1::mnuDecryptClick(TObject *Sender)
{

String MsgText;

if (OpenDialogDecrypt->Execute() == true) {

	FileList->Clear();
	FileList->Text = OpenDialogDecrypt->Files->Text;  //復号する元ファイルリスト

	//記憶パスワードで即座に実行する
	if ( opthdl->fMemPasswordExe == true && opthdl->fMyDecodePasswordKeep == true) {
		//実行パネル表示
		PageControl1->ActivePage = TabSheetExecute;
		FileDecrypt();
		return;
	}
	//復号時にパスワードファイルを自動チェックする
	else if ( opthdl->fAllowPassFile == true && opthdl->fCheckPassFileDecrypt == true ) {

		if ( FileExists(opthdl->PassFilePathDecrypt) == false ) {
			//パスワードファイルがない場合エラーを出さない
			if ( opthdl->fNoErrMsgOnPassFile == true ) {
				//メッセージを出さずにパスワード入力パネルへ
				PageControl1->ActivePage = TabSheetInputDecPass;
				txtDecryptPassword->SetFocus();
				return;
			}
			else{
				//'動作設定で指定された復号するパスワードファイルが見つかりません。'+#13+
				//'設定を確認してください。';
				MsgText = LoadResourceString(&Msgunit1::_MSG_ERROR_NOT_FOUND_DEC_PASSWORD_FILE)+"\n"+
									opthdl->PassFilePath;
				MessageDlg(MsgText, mtInformation, TMsgDlgButtons() << mbOK, 0);
				return;
			}

		}

	}
	else{
		//パスワード入力パネルへ進む
		PageControl1->ActivePage = TabSheetInputDecPass;
		txtDecryptPassword->SetFocus();
		return;
	}

}//end if (OpenDialogDecrypt->Execute() == true)

}
//---------------------------------------------------------------------------
void __fastcall TForm1::mnuExitClick(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::mnuOptionClick(TObject *Sender)
{

if ( PageControl1->ActivePage == TabSheetMain ){
	mnuSetting->Enabled = true;
}
else{
	mnuSetting->Enabled = false;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::mnuSettingClick(TObject *Sender)
{
//オプションパネルの表示
Form3 = new TForm3(this);
Form3->PopupParent = Screen->ActiveForm;
Form3->ShowModal();
Form3->Release();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::mnuContentsClick(TObject *Sender)
{

//ヘルプファイルの目次を開く
Application->HelpShowTableOfContents();

}
//---------------------------------------------------------------------------
void __fastcall TForm1::mnuAboutClick(TObject *Sender)
{
//バージョン情報の表示
Form2 = new TForm2(this);
Form2->PopupParent = Screen->ActiveForm;
Form2->ShowModal();
Form2->Release();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCloseQuery(TObject *Sender, bool &CanClose)
{

if (encrypt != NULL) {
	encrypt->Terminate();
	CanClose = false;
}

if (decrypt != NULL) {
	decrypt->Terminate();
	CanClose = false;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::cmdEncryptPasswordOKClick(TObject *Sender)
{

if (AnsiString(txtEncryptPassword->Text).Length() > 32) {
	//'入力されたパスワードが長すぎます。'
	BalloonHint1->Title = LoadResourceString(&Msgunit1::_BALLOON_HINT_PASSWOED_LENGTH_LONG);
	//'パスワードに使用できる文字数は半角32文字（全角16文字）以内です'
	BalloonHint1->Description = LoadResourceString(&Msgunit1::_BALLOON_HINT_PASSWOED_LENGTH_SESCRIPTION);
	BalloonHint1->ShowHint(txtEncryptPassword->ClientToScreen(CenterPoint(txtEncryptPassword->ClientRect)));
	txtEncryptPassword->SetFocus();
	txtEncryptPassword->SelectAll();
	return;
}

//実行可能形式出力のチェックボックス
chkExeFileOutConf->Checked = chkExeFileOut->Checked;

//パスワード再確認パネルへ進む
PageControl1->ActivePage = TabSheetInputEncPassConfirm;
txtPasswordConfirm->SetFocus();

}
//---------------------------------------------------------------------------
void __fastcall TForm1::cmdEncryptPasswordCancelClick(TObject *Sender)
{

//メインパネルへ戻る
PageControl1->ActivePage = TabSheetMain;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::cmdConfirmOKClick(TObject *Sender)
{

String MsgText;

//-----------------------------------
//パスワードファイルが使われている
if (password_hash[0] != 0 ){

	if ( memcmp( password_hash, temp_password_hash, 32) != 0) {
		//'先に入力されたパスワードファイルとちがいます。SHA-1ハッシュ値が一致しませんでした。'+#13+
		//'暗号化することができません。';
		MsgText = LoadResourceString(&Msgunit1::_MSG_ERROR_MISMATCH_PASSWORD_FILE);
		MessageDlg(MsgText, mtError, TMsgDlgButtons() << mbOK, 0);
		txtPasswordConfirm->SelectAll();
		return;
	}

}
//-----------------------------------
//前に入力されたパスワードと不一致
else if (txtEncryptPassword->Text != txtPasswordConfirm->Text){
	txtPasswordConfirm->SelectAll();
	//'先に入力されたパスワードとちがいます。'
	BalloonHint1->Title = LoadResourceString(&Msgunit1::_BALLOON_HINT_PASSWOED_MISMATCH);
	//'全角・半角のちがい、CAPSロックがないか確認してください。'
	BalloonHint1->Description = LoadResourceString(&Msgunit1::_BALLOON_HINT_PASSWOED_CONFIRMATION);
	BalloonHint1->ShowHint(
		txtPasswordConfirm->ClientToScreen(CenterPoint(txtPasswordConfirm->ClientRect)));
	return;

}

//-----------------------------------
//暗号化
//-----------------------------------

//実行パネル表示
PageControl1->ActivePage = TabSheetExecute;
//暗号化開始
FileEncrypt();


}
//---------------------------------------------------------------------------
// ファイル/フォルダの暗号化処理
//---------------------------------------------------------------------------
void __fastcall TForm1::FileEncrypt(void)
{

AnsiString Password;
String MsgText;
String DirPath, FilePath, FileName, Extension;
TStringList *InputFileList = new TStringList;

//-----------------------------------
// ディレクトリ
//-----------------------------------
if ( opthdl->fSaveToSameFldr == true ) {
	//暗号化ファイルを常に同じ場所に保存するか
	DirPath = opthdl->SaveToSameFldrPath;
}
else{
	DirPath = ExtractFileDir(FileList->Strings[0]);
}

if ( DirectoryExists(DirPath) == false ) {
	//'保存する先のフォルダーが見つかりません。保存設定を再確認してください。'+#13+
	//'暗号化を中止します。';
	MsgText = LoadResourceString(&Msgunit1::_MSG_ERROR_SAVE_ENC_TO_FOLDER_NOT_EXISTS)+"\n"+
						DirPath;
	MessageDlg(MsgText, mtError, TMsgDlgButtons() << mbOK, 0);
	//エラー終了表示
	ProgressBar1->Position = 0;
	lblProgressPercentNum->Caption = " - %";
	//'キャンセル'
	lblStatus->Caption = LoadResourceString(&Msgunit1::_LABEL_STATUS_TITLE_USER_CANCEL);
	//'暗号化が中止されました。'
	lblMsg->Caption = LoadResourceString(&Msgunit1::_LABEL_STATUS_DETAIL_STOPPED_ENCRYPT);
	delete InputFileList;
	return;
}

//-----------------------------------
// ファイル名
//-----------------------------------

FileName = ExtractFileName(FileList->Strings[0]);

//複数のファイルを暗号化する際は一つにまとめる
if ( opthdl->fAllFilePack == true && FileList->Count > 1){

	if ( opthdl->fAutoName == false) {

		SaveDialog1->InitialDir = DirPath;
		SaveDialog1->FileName = FileName;
		Extension = ExtractFileExt(FileList->Strings[0]);
		// FilterIndex
		if ( Extension.UpperCase() == ".ATC" ){
			SaveDialog1->FilterIndex = 1;
		}
		else if ( Extension.UpperCase() == ".EXE" ){
			SaveDialog1->FilterIndex = 2;
		}
		else{
			SaveDialog1->FilterIndex = 3;
		}

		//保存ダイアログ表示
		if ( SaveDialog1->Execute() == true ){
			FilePath = SaveDialog1->FileName;
			DirPath = ExtractFileDir(FilePath);
			FileName = ExtractFileName(FilePath);
		}
		else{
			//キャンセル終了表示
			ProgressBar1->Position = 0;
			lblProgressPercentNum->Caption = " - %";
			//'キャンセル'
			lblStatus->Caption = LoadResourceString(&Msgunit1::_LABEL_STATUS_TITLE_USER_CANCEL);
			//'暗号化が中止されました。'
			lblMsg->Caption = LoadResourceString(&Msgunit1::_LABEL_STATUS_DETAIL_STOPPED_ENCRYPT);
			delete InputFileList;
			return;
		}

	}

	//まとめるのでそのままファイルリストを暗号化処理につっこむ
	FileListPosition = FileList->Count;
	InputFileList = FileList;

}
//-----------------------------------
//個別にファイルを暗号化する
//-----------------------------------
else{

	InputFileList->Add(FileList->Strings[FileListPosition]);
	FileListPosition++;

}

//-----------------------------------
//出力ファイル（暗号化ファイル）パスをつくる
//-----------------------------------

//暗号化ファイルの拡張子を偽装する
if ( opthdl->fAddCamoExt == true ){
	Extension = opthdl->CamoExt;
}
//実行可能形式
else if ( chkExeFileOutConf->Checked == true ){
	Extension = ".exe";
}
//通常の暗号化ファイル
else{
	Extension = ".atc";
}

//-----------------------------------
//暗号化ファイル名に拡張子を含める
//-----------------------------------
if ( opthdl->fExtInAtcFileName == true ) {
	FilePath = IncludeTrailingPathDelimiter(DirPath) + FileName + Extension;
}
else{
	FilePath = IncludeTrailingPathDelimiter(DirPath) + ChangeFileExt(FileName, Extension);
}

//-----------------------------------
//自動で暗号化ファイル名を付加する
//-----------------------------------
if ( opthdl->fAutoName == true ) {
	FilePath = opthdl->InterpretFormatTextToFilePath(FilePath, opthdl->AutoNameFormatText);
}

//-----------------------------------
//暗号化インスタンスの作成
//-----------------------------------
encrypt = new TAttacheCaseFileEncrypt(true);

encrypt->OnTerminate = EncryptThreadTerminated;
encrypt->FreeOnTerminate = true;
//暗号化オプションをそれぞれセットする
encrypt->OutFilePath = FilePath;                                   //出力する暗号化ファイル
encrypt->InputFileList = InputFileList;                            //暗号化する元ファイルリスト
encrypt->CompressRateNum = opthdl->CompressRate;                   //圧縮率
encrypt->fOver4gbOk = opthdl->fOver4GBok;                          //4GB超を許可
encrypt->fAllFilesPackOption = opthdl->fAllFilePack;               //すべてのファイルを１つにまとめる
encrypt->fExeOutputOption = chkExeFileOutConf->Checked;            //実行形式出力
encrypt->fOptBrokenFileOption = opthdl->fBroken;                   //ミスタイプでファイルを破壊するか否か
encrypt->fConfirmOverwirte = opthdl->fConfirmOverwirte;            //同名ファイルがあるときは上書きの確認をする
encrypt->intOptMissTypeLimitsNumOption = opthdl->MissTypeLimitsNum;//タイプミスできる回数
encrypt->AppExeFilePath = Application->ExeName;	                   //アタッシェケース本体の場所（実行形式出力のときに参照する）

//パスワード
if ( password_hash[0] == 0) {
	//パスワードに文字列をセット
	Password = AnsiString(txtPasswordConfirm->Text);
	encrypt->SetPasswordString(Password);;
}
else{
	//パスワードファイルのSHA-1値をセット
	encrypt->SetPasswordBinary(password_hash);
}

//暗号化の実行
encrypt->Start();

//タスクバー進捗表示（Win7）
if(CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_ALL, IID_ITaskbarList3, (void**)&ptl) != S_OK) {
	//失敗した場合は無視
}
if (ptl) ptl->SetProgressState(Application->Handle, TBPF_NORMAL);

//進捗をTimerで監視
TimerEncrypt->Enabled = true;

}
//---------------------------------------------------------------------------
// ファイルを復号する処理
//---------------------------------------------------------------------------
void __fastcall TForm1::FileDecrypt(void)
{

int i;

String MsgText;

String AtcFilePath;
String OutDirPath;


//親フォルダを生成しない
//if ( fNoParentFldr == true  )

//-----------------------------------
//保存先のディレクトリ
//-----------------------------------
if ( opthdl->fDecodeToSameFldr == true ) {
	//常に同じ場所に復号するか
	OutDirPath = opthdl->DecodeToSameFldrPath;
}
else{
	OutDirPath = ExtractFileDir(FileList->Strings[0]);
}

if ( DirectoryExists(OutDirPath) == false ) {
	//'保存する先のフォルダーが見つかりません。保存設定を再確認してください。'+#13+
	//'復号処理化を中止します。';
	MsgText = LoadResourceString(&Msgunit1::_MSG_ERROR_SAVE_DEC_TO_FOLDER_NOT_EXISTS)+"\n"+
						OutDirPath;
	MessageDlg(MsgText, mtError, TMsgDlgButtons() << mbOK, 0);
	//エラー終了表示
	ProgressBar1->Position = 0;
	lblProgressPercentNum->Caption = " - %";
	//'キャンセル'
	lblStatus->Caption = LoadResourceString(&Msgunit1::_LABEL_STATUS_TITLE_USER_CANCEL);
	//'復号が中止されました。'
	lblMsg->Caption = LoadResourceString(&Msgunit1::_LABEL_STATUS_DETAIL_STOPPED_DECRYPT);
	return;

}

//-----------------------------------
// 復号処理の開始
//-----------------------------------

FileListPosition = 0;

if ( FileList->Count > 0) {

	AtcFilePath = FileList->Strings[FileListPosition];

	//復号処理インスタンスの作成
	decrypt = new TAttacheCaseFileDecrypt2(true);
	decrypt->OnTerminate = DecryptThreadTerminated;
	decrypt->FreeOnTerminate = true;
	decrypt->AppExeFilePath = Application->ExeName;  //アタッシェケース本体の場所（実行形式出力のときに参照する）
	decrypt->AtcFilePath = AtcFilePath;              //入力する暗号化ファイルパス
	decrypt->OutDirPath = OutDirPath;                //出力するディレクトリ

	decrypt->fOpenFolder = opthdl->fOpenFolder;             //フォルダの場合に復号後に開くか
	decrypt->fOpenFile = opthdl->fOpenFile;                 //復号したファイルを関連付けされたソフトで開く
	decrypt->fConfirmOverwirte = opthdl->fConfirmOverwirte; //同名ファイルの上書きを確認するか

	//-----------------------------------
	//パスワードのセット
	//-----------------------------------

	//パスワードファイルを使用する
	if ( opthdl->fCheckPassFileDecrypt == true &&
			 FileExists(opthdl->PassFilePathDecrypt) == true) {
		//パスワードをバイナリ値でセット
		decrypt->SetPasswordBinary(password_hash);
		//パスワード文字列をバイナリ値でセット（ver.2.75以前）
		decrypt->SetPasswordStringToBinary(PasswordFileHash);
	}
	else{
		decrypt->SetPasswordString(txtDecryptPassword->Text);
	}

	//復号の実行
	decrypt->Start();

	//タスクバー進捗表示（Win7）
	if(CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_ALL, IID_ITaskbarList3, (void**)&ptl) != S_OK) {
		//失敗
	}
	if (ptl) ptl->SetProgressState(Application->Handle, TBPF_NORMAL);

	//進捗をTimerで監視
	TimerDecrypt->Enabled = true;

}


}
//---------------------------------------------------------------------------
// ファイルをコンペアする処理（とは言っても実際は復号処理）
//---------------------------------------------------------------------------
void __fastcall TForm1::FileCompare(void)
{

int i;

String MsgText;

String AtcFilePath;
String OutDirPath;

//-----------------------------------
// コンペア処理の開始
//-----------------------------------
if (FileListPosition > FileList->Count - 1) {
	return;
}

//-----------------------------------
// コンペア処理の開始
//-----------------------------------

// この処理の前に暗号化処理を行っているか
//（暗号化インスタンスがあるか）
if ( encrypt == NULL) {
	//'コンペアする暗号化ファイルまたはフォルダーが見つかりません。'+#13+
	//'コンペアに失敗しました。';
	MsgText = LoadResourceString(&Msgunit1::_MSG_ERROR_NOT_EXISTS_COMPAER_FIlE);
	MessageDlg(MsgText, mtError, TMsgDlgButtons() << mbOK, 0);
	return;
}

AtcFilePath = FileList->Strings[FileListPosition];

//復号処理インスタンスの作成
decrypt = new TAttacheCaseFileDecrypt2(true);

decrypt->fCompare = true;	//コンペア
decrypt->CompareFileList = encrypt->InputFileList;

decrypt->OnTerminate = DecryptThreadTerminated;
decrypt->FreeOnTerminate = true;
decrypt->AppExeFilePath = Application->ExeName;  //アタッシェケース本体の場所（実行形式出力のときに参照する）
decrypt->AtcFilePath = AtcFilePath;              //入力する暗号化ファイルパス
decrypt->OutDirPath = "";                        //出力するディレクトリ

//-----------------------------------
//パスワードのセット
//-----------------------------------
memcpy(decrypt->key, encrypt->key, 32);

//コンペア（復号）の実行
decrypt->Start();

//タスクバー進捗表示（Win7）
if(CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_ALL, IID_ITaskbarList3, (void**)&ptl) != S_OK) {
	//失敗
}
if (ptl) ptl->SetProgressState(Application->Handle, TBPF_NORMAL);

//進捗をTimerで監視
TimerDecrypt->Enabled = true;



}
//---------------------------------------------------------------------------
void __fastcall TForm1::cmdConfirmCancelClick(TObject *Sender)
{

//パスワード入力パネルへ戻る
PageControl1->ActivePage = TabSheetInputEncPass;
txtEncryptPassword->SetFocus();
txtEncryptPassword->SelectAll();

}
//---------------------------------------------------------------------------
void __fastcall TForm1::cmdDecryptPasswordOKClick(TObject *Sender)
{

//-----------------------------------
//復号する
//-----------------------------------

//実行パネル表示
PageControl1->ActivePage = TabSheetExecute;
//復号開始
FileDecrypt();


}
//---------------------------------------------------------------------------
void __fastcall TForm1::txtEncryptPasswordKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
if (Key == VK_RETURN) {
	cmdEncryptPasswordOKClick(Sender);
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::txtPasswordConfirmKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
if (Key == VK_RETURN) {
	cmdConfirmOKClick(Sender);
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::txtDecryptPasswordKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
if (Key == VK_RETURN) {
	cmdDecryptPasswordOKClick(Sender);
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::cmdCancelClick(TObject *Sender)
{

//暗号化（復号）処理のキャンセル

if (encrypt != NULL) {
	encrypt->Terminate();
}
else if (decrypt != NULL) {
	decrypt->Terminate();
}
else{
	//メインパネルへ戻る
	PageControl1->ActivePage = TabSheetMain;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::StatusBar1DblClick(TObject *Sender)
{

//デバッグ

//オプションパネルの表示
Form3 = new TForm3(Application);
Form3->PopupParent = Screen->ActiveForm;
Form3->ShowModal();
Form3->Release();

}
//---------------------------------------------------------------------------
void __fastcall TForm1::chkExeFileOutClick(TObject *Sender)
{

//「暗号化パスワードの入力再確認」パネルのチェックボックスも変更する
chkExeFileOutConf->Checked = chkExeFileOut->Checked;

}
//---------------------------------------------------------------------------
// 二重起動した先からのメッセージ受け取り
//---------------------------------------------------------------------------
void __fastcall TForm1::EvWmCOPYDATA(TMessage message)
{

int i, c;

bool fChk = false;
String OneLine;

//メッセージ内容を取り出す
COPYDATASTRUCT *cds;
cds = (COPYDATASTRUCT *)message.LParam;

if (cds == NULL) {
	return;
}

wchar_t *buffer;
buffer = new wchar_t[cds->cbData];
ZeroMemory(buffer, cds->cbData);
memmove_s(buffer, cds->cbData, cds->lpData, cds->cbData);
//char *buffer;
//buffer = new char[cds->cbData];
//ZeroMemory(buffer, cds->cbData);
//memmove_s(buffer, cds->cbData, cds->lpData, cds->cbData);

TStringList *TempList = new TStringList;
TempList->Text = buffer;

if (TempList->Count > 0) {
	delete[] buffer;
	delete TempList;
	return;
}

// すでに格納されているFileListに追加するという実装は
// どうなのか？　要検討事項。

for ( i = 0; i < TempList->Count; i++){

	OneLine = TempList->Strings[i];

	if ( OneLine != "" ){

		for ( c = 0; c < FileList->Count; c++){

			if ( OneLine == FileList->Strings[c] ){
				//すでにファイルリストに存在するファイルは無視
				fChk = true;
				break;
			}

		}

		if ( fChk == true ){
			fChk = false;
		}
		else{
			FileList->Add(OneLine);
		}

	}

}

delete[] buffer;
delete TempList;

//-----------------------------------
//処理開始
//-----------------------------------
DoExecute(FileList);


}//end EvWmCOPYDATA;
//---------------------------------------------------------------------------
void __fastcall TForm1::PaintBoxMainPaint(TObject *Sender)
{

//中央のグラフィック描画
int posx = 0;
int posy = TabSheetMain->Height - imgMain->Height;
PaintBoxMain->Canvas->Draw( posx, posy,	imgMain->Picture->Bitmap);
PaintBoxEncrypt->Canvas->Draw( posx, posy,	imgEncrypt->Picture->Bitmap);
PaintBoxConfirm->Canvas->Draw( posx, posy,	imgEncrypt->Picture->Bitmap);
PaintBoxDecrypt->Canvas->Draw( posx, posy,	imgDecrypt->Picture->Bitmap);

}
//---------------------------------------------------------------------------
void __fastcall TForm1::imgOptionPanelClick(TObject *Sender)
{

//オプションパネルの表示
Form3 = new TForm3(this);
Form3->PopupParent = Screen->ActiveForm;
Form3->ShowModal();
Form3->Release();

}
//---------------------------------------------------------------------------
void __fastcall TForm1::imgOptionPanelMouseEnter(TObject *Sender)
{

imgOptionPanel->Picture = imgOptionIconSelect->Picture;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::imgOptionPanelMouseLeave(TObject *Sender)
{

imgOptionPanel->Picture = imgOptionIcon->Picture;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::imgBackMouseEnter(TObject *Sender)
{

TImage *img = dynamic_cast<TImage *>(Sender);
img->Picture = imgBackActive->Picture;


}
//---------------------------------------------------------------------------
void __fastcall TForm1::imgBackMouseLeave(TObject *Sender)
{

TImage *img = dynamic_cast<TImage *>(Sender);
img->Picture = imgBackNormal->Picture;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::imgBackOnClick(TObject *Sender)
{

TImage *img = dynamic_cast<TImage *>(Sender);

if ( img->Name == "imgBackFromInputEncPass") {
	PageControl1->ActivePage = TabSheetMain;
}
else if ( img->Name == "imgBackFromInputEncPassConfirm") {
	PageControl1->ActivePage = TabSheetInputEncPass;
}
else if ( img->Name == "imgBackFromInputDecPass") {
	PageControl1->ActivePage = TabSheetMain;
}
else if ( img->Name == "imgBackFromExecute") {
	PageControl1->ActivePage = TabSheetMain;
}

}
//---------------------------------------------------------------------------
//メッセージダイアログの表示（スレッドオブジェクトから呼ばれる）
//---------------------------------------------------------------------------
int __fastcall TForm1::ShowConfirmMassageForm
	(String MsgText, TMsgDlgType MsgType, TMsgDlgButtons MsgButtons, TMsgDlgBtn MsgDefaultButton)
{

TForm *dlg = CreateMessageDialog(MsgText, MsgType, MsgButtons, MsgDefaultButton);
dlg->ParentWindow = Form1->Handle;

if ( MsgType == mtError) {
	dlg->Caption = LoadResourceString(&Msgunit1::_MSG_CAPTION_ERROR);
}
else {
	dlg->Caption = LoadResourceString(&Msgunit1::_MSG_CAPTION_CONFIRMATION);
}

//メッセージダイアログの表示
int ret = dlg->ShowModal();
delete dlg;
return(ret);

}
//---------------------------------------------------------------------------
//上書き確認メッセージダイアログの表示（復号スレッドオブジェクトから呼ばれる）
//---------------------------------------------------------------------------
int __fastcall TForm1::ShowConfirmOverwriteMassageForm(String MsgText, String &Path)
{

//ボタンをカスタマイズするので上記関数と分けて作成

TButton *btn;
TMsgDlgButtons MsgButtons = (TMsgDlgButtons() << mbYesToAll << mbYes << mbNo << mbCancel);
TForm *dlg = CreateMessageDialog(MsgText, mtConfirmation, MsgButtons, mbCancel);

//ボタン名をそれぞれ書き換える
btn = (TButton *)dlg->FindComponent("YesToAll");   //'全て上書き(&W)'
btn->Caption = LoadResourceString(&Msgunit1::_DIALOG_BUTTON_NAME_OVERWITE_YES_TO_ALL);
btn = (TButton *)dlg->FindComponent("Yes");        //'上書き(&O)'
btn->Caption = LoadResourceString(&Msgunit1::_DIALOG_BUTTON_NAME_OVERWITE_YES);
btn = (TButton *)dlg->FindComponent("No");         //'別名保存(&A)'
btn->Caption = LoadResourceString(&Msgunit1::_DIALOG_BUTTON_NAME_OVERWITE_NO);
btn = (TButton *)dlg->FindComponent("Cancel");     //'キャンセル(&C)'
btn->Caption = LoadResourceString(&Msgunit1::_DIALOG_BUTTON_NAME_OVERWITE_CANCEL);

dlg->Caption = LoadResourceString(&Msgunit1::_MSG_CAPTION_CONFIRMATION);
dlg->ParentWindow = Form1->Handle;

//メッセージダイアログの表示
int ret = dlg->ShowModal();

if (ret == mrNo) { //別名で保存する（いいえ）

	TSaveDialog *dlgsave = new TSaveDialog(dlg);
	//'復号ファイルの保存'
	dlgsave->Title = LoadResourceString(&Msgunit1::_DIALOG_SAVE_AS_FILE_CAPTION);
	dlgsave->InitialDir = ExtractFileDir(ExcludeTrailingPathDelimiter(Path));
	dlgsave->FileName = ExtractFileName(ExcludeTrailingPathDelimiter(Path));
	//'ファイルフォルダー|*'
	dlgsave->Filter = LoadResourceString(&Msgunit1::_DIALOG_SAVE_AS_FILTER);
	//別名保存ダイアログ
	if ( dlgsave->Execute() == true ){
		Path = dlgsave->FileName;
	}
	else{
		//キャンセル
		ret = mrCancel;
	}
	delete dlgsave;
}

delete dlg;
return(ret);

}
//---------------------------------------------------------------------------

