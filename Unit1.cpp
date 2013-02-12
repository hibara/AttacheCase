//===========================================================================
/*

アタッシェケース（AttachéCase）
Copyright (c) 2002-2013, Mitsuhiro Hibara ( http://hibara.org )
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


#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#if __CODEGEARC__ < 0x640
#pragma link "HTMLHelpViewer"
#else
#pragma link "Vcl.HTMLHelpViewer"
#endif

TForm1 *Form1;
TDragAndDrop *DragAndDropTarget;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{

int i;
//サイドメニューグラフィック
bmpSideMenu = new Graphics::TBitmap;
bmpSideMenu->Canvas->Brush->Style = bsClear;

//アプリケーションタイトル
this->Caption = Application->Title;

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

//動作設定のINIファイルパス
String IniFilePath = "";
//処理するファイルリスト
FileList = new TStringList;

lblMain->Caption = LoadResourceString(&Msgunit1::_DRAG_AND_DROP_HERE);
txtEncryptPassword->EditLabel->Caption = LoadResourceString(&Msgunit1::_INPUT_PASSWORD);
chkExeFileOut->Caption = LoadResourceString(&Msgunit1::_CHECK_BOX_EXEFILE_OUT);
txtPasswordConfirm->EditLabel->Caption = LoadResourceString(&Msgunit1::_CONFIRM_PASSWORD);
chkExeFileOutConf->Caption = LoadResourceString(&Msgunit1::_CHECK_BOX_EXEFILE_OUT);
txtDecryptPassword->EditLabel->Caption = LoadResourceString(&Msgunit1::_INPUT_DECRYPT_PASSWORD);
chkDeleteSourceData->Caption = LoadResourceString(&Msgunit1::_CHECK_BOX_DELETE_SOURCE_DATA);
chkDeleteSourceDataConf->Caption = chkDeleteSourceData->Caption;
chkDeleteAtcData->Caption = LoadResourceString(&Msgunit1::_CHECK_BOX_DELETE_ATC_DATA);
lblStatus->Caption = "-";
lblMsg->Caption = "-";
lblProgressPercentNum->Caption = "- %";

//サイドバーメニューキャプション
SideBarCaption[1] = LoadResourceString(&Msgunit1::_SIDEBAR_CAPTION_ENCRYPT);
SideBarCaption[2] = LoadResourceString(&Msgunit1::_SIDEBAR_CAPTION_DECRYPT);
SideBarCaption[4] = LoadResourceString(&Msgunit1::_SIDEBAR_CAPTION_OPTION);
SideBarCaptionPosX[1] = PaintBoxMenu->Width/2 - PaintBoxMenu->Canvas->TextWidth(SideBarCaption[1])/2;
SideBarCaptionPosX[2] = PaintBoxMenu->Width/2 - PaintBoxMenu->Canvas->TextWidth(SideBarCaption[2])/2;
SideBarCaptionPosX[4] = PaintBoxMenu->Width/2 - PaintBoxMenu->Canvas->TextWidth(SideBarCaption[4])/2;

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
	for (i = 0; i < FileList->Count; i++) {

		if ( DirectoryExists(FileList->Strings[i]) == true ){
			//フォルダならその中にあるファイル
			IniFilePath = IncludeTrailingPathDelimiter(FileList->Strings[i])+INI_FILE_NAME;
		}
		else {
			//ファイルなら同じディレクトリから
			IniFilePath = IncludeTrailingPathDelimiter(ExtractFileDir(FileList->Strings[i]))+INI_FILE_NAME;
		}

		if (FileExists(IniFilePath) == true) {
			break;
		}

	}

	if (FileExists(IniFilePath) == false ) {
		//カレントディレクトリに設定INIファイルがあるか？
		IniFilePath = IncludeTrailingPathDelimiter(GetCurrentDir())+INI_FILE_NAME;
		if ( FileExists(IniFilePath) == false ){
			//なければ本体ディレクトリから、なければレジストリから読み込む
			IniFilePath = IncludeTrailingPathDelimiter(ExtractFileDir(Application->ExeName))+INI_FILE_NAME;
			opthdl->LoadOptionData(IniFilePath);
		}
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
	IniFilePath = IncludeTrailingPathDelimiter(GetCurrentDir())+INI_FILE_NAME;

	if ( FileExists(IniFilePath) == true ){
		opthdl->LoadOptionData(IniFilePath);
	}
	else{
		//なければ本体ディレクトリから、なければレジストリから読み込む
		IniFilePath = IncludeTrailingPathDelimiter(ExtractFileDir(Application->ExeName))+INI_FILE_NAME;
		opthdl->LoadOptionData(IniFilePath);
	}

}

//-----------------------------------
//各コンポーネントの配置
//-----------------------------------
SetFormComponent(NULL);

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

//-----------------------------------
//サイドメニューを描画する
//-----------------------------------
PaintSideMenu();


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

//全設定を保存する
opthdl->SaveOptionData();

delete opthdl;

delete FileList;    //処理するファイルリスト


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

	//フォルダ内のファイルは個別に暗号化/復号する
	if (opthdl->fFilesOneByOne == true ) {
		ExpandFileList(FileList);
	}

	//暗号化/復号処理を自動判定し実行する
	DoExecute(FileList);

}
else{
	//メインパネルを通常表示
	PageControlActiveSheet(TabSheetMain);

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

//-----------------------------------
//各コンポーネントの配置
//-----------------------------------
SetFormComponent(Sender);

//-----------------------------------
//各メニューアイコンの位置・描画
//-----------------------------------

const int posX = 22;

//「オプション」
ptSideMenu[4].x = posX;
ptSideMenu[4].y = PaintBoxMenu->Height - 72;
// 分割線
ptSideMenu[3].x = posX;
ptSideMenu[3].y = ptSideMenu[4].y - 30;


// 中央配置
ptSideMenu[0].x = posX;
ptSideMenu[0].y = PaintBoxMenu->Height/2 - 36;

//「暗号化」
ptSideMenu[1].x = posX;
ptSideMenu[1].y = 16;

//「復号する」
ptSideMenu[2].x = posX;
ptSideMenu[2].y = ptSideMenu[1].y + 90;

//サイドメニューを再描画する
PaintSideMenu();

}
//---------------------------------------------------------------------------
//ジェスチャ（タッチ）イベント
//---------------------------------------------------------------------------
void __fastcall TForm1::FormGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
					bool &Handled)
{

int PosX, PosY;

switch(EventInfo.GestureID){

//igZoom
case 259:

	Handled = true;

	if(EventInfo.Flags.Contains(gfBegin) == true){	        //ズーム開始

		//基準座標を保存
		TouchFormStartPos.x = (int)EventInfo.Location.x;
		TouchFormStartPos.y = (int)EventInfo.Location.y;
		TouchFormWidth = Form1->Width;
		TouchFormHeight = Form1->Height;

	}
	else if (EventInfo.Flags.Contains(gfInertia) == true ){ //イナーシャ中

	}
	else if ( EventInfo.Flags.Contains(gfEnd) == true  ){   //ズーム終了

		TouchFormStartPos = TPoint(0,0);
		TouchFormWidth = Form1->Width;
		TouchFormHeight = Form1->Height;

	}
	else{

		PosX = TouchFormStartPos.x - (int)EventInfo.Location.x;
		PosY = TouchFormStartPos.y - (int)EventInfo.Location.y;

		//フォームをそれに伴って拡大縮小する
		Form1->Width = TouchFormWidth - PosX*2;
		Form1->Height = TouchFormHeight - PosY*2;

	}

	break;


default:
	break;


}//switch;


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
lblMain->Left = TabSheetMain->Width/2 - lblMain->Width/2;
lblMain->Top = TabSheetMain->Height/2 - lblMain->Height/2;

//-----------------------------------
//暗号化パスワード入力パネル
//-----------------------------------
txtEncryptPassword->Left = 24;
txtEncryptPassword->Top = TabSheetInputEncPass->Height/2 - txtEncryptPassword->Height;
txtEncryptPassword->Width = TabSheetInputEncPass->Width - txtEncryptPassword->Left*2;

//チェックボックス
chkExeFileOut->Top = txtEncryptPassword->BoundsRect.Bottom + 8;
chkExeFileOut->Left = txtEncryptPassword->Left;
chkExeFileOut->Width = txtEncryptPassword->Width;
chkDeleteSourceData->Left = chkExeFileOut->Left;
chkDeleteSourceData->Top = chkExeFileOut->BoundsRect.Bottom + 8;

//各ボタン
cmdEncryptPasswordCancel->Top = chkDeleteSourceData->BoundsRect.Bottom + 8;
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

//チェックボックス
chkExeFileOutConf->Top = txtPasswordConfirm->BoundsRect.Bottom + 8;
chkExeFileOutConf->Left = txtPasswordConfirm->Left;
chkExeFileOutConf->Width = txtPasswordConfirm->Width;
chkDeleteSourceDataConf->Left = chkExeFileOutConf->Left;
chkDeleteSourceDataConf->Top = chkExeFileOutConf->BoundsRect.Bottom + 8;

//各ボタン
cmdConfirmCancel->Top = chkDeleteSourceDataConf->BoundsRect.Bottom + 8;
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

//「暗号化ファイルを削除する」チェックボックス
chkDeleteAtcData->Top = txtDecryptPassword->BoundsRect.Bottom + 8;
chkDeleteAtcData->Left = txtDecryptPassword->Left;
chkDeleteAtcData->Width = txtDecryptPassword->Width;

//各ボタン
cmdDecryptPasswordCancel->Top = chkDeleteAtcData->BoundsRect.Bottom + 8;
cmdDecryptPasswordCancel->Left = txtDecryptPassword->BoundsRect.Right - cmdDecryptPasswordCancel->Width;
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

if (TimerEncrypt->Enabled == false && TimerDecrypt->Enabled == false && TimerDelete->Enabled == false) {
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

#define DOT_HEIGHT 4
#define DOT_WIDTH 4

int PosX, PosY;

this->Canvas->Pen->Color = TColor(RGB(200,200,200));
this->Canvas->Brush->Color = TColor(RGB(200,200,200));

//水平線を描画
PosY = PageControl1->BoundsRect.bottom;
for (PosX = (PageControl1->BoundsRect.left - DOT_WIDTH); PosX < this->Width; PosX+=24) {
	this->Canvas->Rectangle(PosX,    0, PosX+16, DOT_HEIGHT);         //Top
	this->Canvas->Rectangle(PosX, PosY, PosX+16, PosY+DOT_HEIGHT);    //Bottom
}

//垂直線を描画
for (PosY = 0; PosY < this->Height; PosY+=24) {
	this->Canvas->Rectangle(PageControl1->Left-DOT_WIDTH,   PosY, PageControl1->Left+DOT_WIDTH, PosY+16);
	this->Canvas->Rectangle(PageControl1->BoundsRect.right, PosY, PosX+DOT_WIDTH,               PosY+16);
}

//ドロップ矢印
PaintBoxMain->Canvas->Draw(
	PaintBoxMain->Width/2-imgDropFileIn->Width/2, lblMain->Top-imgDropFileIn->Height,
	imgDropFileIn->Picture->Icon);


}
//---------------------------------------------------------------------------
void __fastcall TForm1::PaintBoxMainMouseLeave(TObject *Sender)
{

//点線枠をクリア
this->Canvas->Brush->Color = clBtnFace;
this->Canvas->FillRect(Rect(0, 0, this->Width, this->Height));

//ドロップ矢印をクリア
TRect rc = TRect(lblMain->Left, lblMain->Top-imgDropFileIn->Height,
				lblMain->Left + lblMain->Width, lblMain->Top);
PaintBoxMain->Canvas->Brush->Color = clWhite;
PaintBoxMain->Canvas->FillRect(rc);



//サイドバーメニューを再描画しておく
PaintBoxMenuPaint(Sender);

}
//---------------------------------------------------------------------------
// PaintBoxをドラッグしてフォームを移動する
//---------------------------------------------------------------------------
void __fastcall TForm1::PaintBoxMainMouseDown(TObject *Sender, TMouseButton Button,
					TShiftState Shift, int X, int Y)
{

ReleaseCapture();
SendMessage(this->Handle, WM_SYSCOMMAND, SC_MOVE | 2, 0);

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
		Form1->Caption = ExtractFileName(FileList->Strings[0]) + " - " + Application->Title;
		//フォルダ内のファイルは個別に暗号化/復号する
		if (opthdl->fFilesOneByOne == true ) {
			ExpandFileList(FileList);
		}
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

			PasswordFilePath = DropFileList->Strings[0];
			txtEncryptPassword->Text = PasswordFilePath;

			//'パスワードに以下のファイルのハッシュ値が入力されました'
			BalloonHint1->Title = LoadResourceString(&Msgunit1::_BALLOON_HINT_PASSWORD_HASH_TITLE);
			//そのファイルパスを表示する
			BalloonHint1->Description = DropFileList->Strings[0];
			BalloonHint1->ShowHint(txtEncryptPassword->ClientToScreen(CenterPoint(txtEncryptPassword->ClientRect)));
			txtEncryptPassword->SetFocus();
			txtEncryptPassword->SelectAll();

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

			ConfirmPasswordFilePath = DropFileList->Strings[0];
			txtPasswordConfirm->Text = ConfirmPasswordFilePath;
			cmdConfirmOKClick((TObject*)0);
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
			//パスワードファイル
			PasswordFilePath = DropFileList->Strings[0];
			txtDecryptPassword->Text = PasswordFilePath;
			cmdDecryptPasswordOKClick((TObject*)0);
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
//ファイルの判別を行った後に処理を分けて実行
//---------------------------------------------------------------------------
void __fastcall TForm1::DoExecute(TStringList *ExeFileList)
{

int i;
String MsgText;
String FilePath;
int FileVersion;

//-----------------------------------
// 処理内容
//-----------------------------------
//  1:暗号化(TYPE_CRYPT_ENCRYPT),
//  2:復号(TYPE_CRYPT_DECRYPT)
// -1:エラー（TYPE_CRYPT_ERROR）
// int CryptTypeNum = 0;

int res;
TForm* dlgconf;
TButton *btn;

TFileStream *fs;
char token[17];
ZeroMemory(token, 17);

FileListPosition = 0;

RetryAtcFilePath = "";
RetryNum = 0;

//-----------------------------------
//暗号/復号処理かを問い合わせる
//-----------------------------------
if ( opthdl->fAskEncDecode == true ) {

	//'ドラッグ＆ドロップされたファイルまたはフォルダーをどのように処理しますか？'
	MsgText = LoadResourceString(&Msgunit1::_MSG_CONFIRM_ENCRYPT_OR_DECRYPT)+"\n"+
									 ExeFileList->Strings[0];

	if ( ExeFileList->Count > 1) {
		MsgText = MsgText + "\n..." + IntToStr(ExeFileList->Count) + " files.";
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
		ExeFileList->Clear();
		return;
	}

}
//-------------------------------------
//ファイルヘッダを自動判定する
//-------------------------------------
else{

	//判定するファイルリストがすべてATCファイル形式であれば「復号」
	for (i = 0; i < ExeFileList->Count; i++) {
		CryptTypeNum = CheckAtcFileHeader(ExeFileList->Strings[i], FileVersion);
		if (CryptTypeNum == TYPE_CRYPT_ERROR) {
			//エラー
			break;
		}
		else if (CryptTypeNum == TYPE_CRYPT_ENCRYPT) {
			//一つでも暗号化するべきファイルがあれば抜ける
			break;
		}
		else{
			// TYPE_CRYPT_DECRYPT
		}
	}

}
//-----------------------------------
// 暗号化/復号それぞれの処理実行
//-----------------------------------

//-----------------------------------
// 暗号化
//-----------------------------------
if ( CryptTypeNum == TYPE_CRYPT_ENCRYPT ) {

	//実行形式で出力するか
	chkExeFileOut->Checked = opthdl->fSaveToExeout;
	chkExeFileOutConf->Checked = chkExeFileOut->Checked;

	//元ファイルを削除するか
	chkDeleteSourceData->Checked = opthdl->fDelOrgFile;
	chkDeleteSourceDataConf->Checked = chkDeleteSourceData->Checked;

	//コマンドラインにパスワードが設定されているようだ
	if (opthdl->fArgPassword == true) {
		//実行パネル表示
		PageControlActiveSheet(TabSheetExecute);
		//再確認テキストボックスに入れてしまう
		txtPasswordConfirm->Text = opthdl->MyPassword;
		//暗号化開始
		FileEncrypt();
		return;
	}
	//記憶パスワードで即座に実行する
	else if ( opthdl->fMemPasswordExe == true && opthdl->fMyEncodePasswordKeep == true) {
		PageControlActiveSheet(TabSheetExecute);
		txtPasswordConfirm->Text = opthdl->MyEncodePassword;
		FileEncrypt();
		return;
	}
	//暗号時にパスワードファイルを自動チェックする
	else if ( opthdl->fAllowPassFile == true && opthdl->fCheckPassFile == true ) {

		if ( FileExists(opthdl->PassFilePath) == false ) {
			//パスワードファイルがない場合エラーを出さない？（オプション）
			if ( opthdl->fNoErrMsgOnPassFile == true ) {
				//メッセージを出さずにパスワード入力パネルへ
				PageControlActiveSheet(TabSheetInputEncPass);
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
		else{
			PasswordFilePath = opthdl->PassFilePath;
			txtEncryptPassword->Text = PasswordFilePath;
			//実行パネル表示
			PageControlActiveSheet(TabSheetExecute);
			//そのまま暗号化へ
			FileEncrypt();
		}

	}
	else{
		//パスワード記憶が行われているのなら
		if (opthdl->fMyEncodePasswordKeep == true) {
			//パスワード再確認パネルまで進む
			PageControlActiveSheet(TabSheetInputEncPassConfirm);
			txtEncryptPassword->Text = opthdl->MyEncodePassword;
			txtPasswordConfirm->Text = opthdl->MyEncodePassword;
			txtPasswordConfirm->SetFocus();
		}
		else{
			//パスワード入力パネルへ進む
			PageControlActiveSheet(TabSheetInputEncPass);
			txtEncryptPassword->SetFocus();
			return;
		}

	}

}
//-----------------------------------
// 復号
//-----------------------------------
else if ( CryptTypeNum == TYPE_CRYPT_DECRYPT) {

	//復号されるファイルの合計サイズ
	DecryptAllTotalSize = 0;

	//暗号化ファイルを削除するか
	chkDeleteAtcData->Checked = opthdl->fDelEncFile;

	//コマンドラインにパスワードが設定されているようだ
	if (opthdl->fArgPassword == true) {
		PageControlActiveSheet(TabSheetExecute);
		txtDecryptPassword->Text = opthdl->MyPassword;
		FileDecrypt();
		return;
	}
	//記憶パスワードで即座に実行する
	else if ( opthdl->fMemPasswordExe == true && opthdl->fMyDecodePasswordKeep == true) {
		//実行パネル表示
		PageControlActiveSheet(TabSheetExecute);
		txtDecryptPassword->Text = opthdl->MyDecodePassword;
		FileDecrypt();
		return;
	}
	//復号時にパスワードファイルを自動チェックする
	else if ( opthdl->fAllowPassFile == true && opthdl->fCheckPassFileDecrypt == true ) {

		if ( FileExists(opthdl->PassFilePathDecrypt) == false ) {
			//パスワードファイルがない場合エラーを出さない
			if ( opthdl->fNoErrMsgOnPassFile == true ) {
				//メッセージを出さずにパスワード入力パネルへ
				PageControlActiveSheet(TabSheetInputDecPass);
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
		else{

			//パスワードファイル
			PasswordFilePath = opthdl->PassFilePathDecrypt;
			txtPasswordConfirm->Text = PasswordFilePath;

			//実行パネル表示
			PageControlActiveSheet(TabSheetExecute);
			//そのまま暗号化へ
			FileDecrypt();

		}

	}
	else{
		//パスワード記憶設定が行われているのなら値を入力しておく
		if (opthdl->fMyDecodePasswordKeep == true) {
			txtDecryptPassword->Text = opthdl->MyDecodePassword;
		}
		//パスワード入力パネルへ進む
		PageControlActiveSheet(TabSheetInputDecPass);
		txtDecryptPassword->SetFocus();
		return;
	}

}
//-----------------------------------
// エラー
//-----------------------------------
else {
	//メインへ戻る
	PageControlActiveSheet(TabSheetMain);
	ExeFileList->Clear();
	return;
}


}
//---------------------------------------------------------------------------
//完全削除処理実行
//---------------------------------------------------------------------------
void __fastcall TForm1::DoDeleteFile(String DelFileListString, __int64 DelTotalSize)
{

//実行パネル表示
PageControlActiveSheet(TabSheetExecute);

//完全削除インスタンスの作成
cmpdel = new TAttacheCaseDelete(true);
cmpdel->OnTerminate = DeleteThreadTerminated;
cmpdel->FreeOnTerminate = true;

cmpdel->DeleteFileList->Text = DelFileListString;
cmpdel->TotalFileSize = DelTotalSize;
cmpdel->Opt = opthdl->fCompleteDelete;     // 0:通常削除, 1:完全削除, 2:ゴミ箱
cmpdel->RandClearNum = opthdl->DelRandNum; //完全削除（乱数書き込み回数）
cmpdel->ZeroClearNum = opthdl->DelZeroNum; //完全削除（NULL書き込み回数）

//削除の実行
cmpdel->Start();

//タスクバー進捗表示（Win7）
if(CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_ALL, IID_ITaskbarList3, (void**)&tskpbr) == S_OK) {
	tskpbr->SetProgressState(this->Handle, TBPF_NORMAL);
}

//進捗をTimerで監視
TimerDelete->Enabled = true;


}
//---------------------------------------------------------------------------
// ファイル/フォルダの暗号化処理
//---------------------------------------------------------------------------
void __fastcall TForm1::FileEncrypt(void)
{

int i;

AnsiString Password;
char password[32];

for (i = 0; i < 32; i++) {
	password[i] = NULL;
}

AnsiString PasswordFileHash, PasswordFileHeader;

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
	DirPath = ExtractFileDir(FileList->Strings[FileListPosition]);
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

FileName = ExtractFileName(FileList->Strings[FileListPosition]);

//複数のファイルを暗号化する際は一つにまとめる
if ( opthdl->fAllFilePack == true && FileList->Count > 1){

	if ( opthdl->fAutoName == false) {

		SaveDialog1->InitialDir = DirPath;
		SaveDialog1->FileName = ChangeFileExt(FileName, "");
		Extension = ExtractFileExt(FileList->Strings[FileListPosition]);
		// FilterIndex
		if ( chkExeFileOutConf->Checked == true ){
			SaveDialog1->FilterIndex = 2;  //".EXE"
		}
		else{
			SaveDialog1->FilterIndex = 1;	 //".ATC"
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
	FileListPosition = FileList->Count-1;
	InputFileList = FileList;

}
//-----------------------------------
//個別にファイルを暗号化する
//-----------------------------------
else{

	InputFileList->Add(FileList->Strings[FileListPosition]);

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
encrypt->InputFileList->Text = InputFileList->Text;                //暗号化する元ファイルリスト
encrypt->CompressRateNum = opthdl->CompressRate;                   //圧縮率
encrypt->fOver4gbOk = opthdl->fOver4GBok;                          //4GB超を許可
encrypt->fExeOutputOption = chkExeFileOutConf->Checked;            //実行形式出力
encrypt->fOptBrokenFileOption = opthdl->fBroken;                   //ミスタイプでファイルを破壊するか否か
encrypt->fConfirmOverwirte = opthdl->fConfirmOverwirte;            //同名ファイルがあるときは上書きの確認をする
encrypt->intOptMissTypeLimitsNumOption = opthdl->MissTypeLimitsNum;//タイプミスできる回数
encrypt->AppExeFilePath = Application->ExeName;	                   //アタッシェケース本体の場所（実行形式出力のときに参照する）

Form1->Caption = ExtractFileName(FilePath) + " - " + Application->Title;

//パスワードファイルを使用するか
if ( PasswordFilePath != "") {

	//SHA-1ハッシュを求める
	if ( opthdl->GetSHA1HashFromFile(PasswordFilePath, PasswordFileHash, PasswordFileHeader ) == true ){
		StrLCopy(password, PasswordFileHash.c_str(), 32);
		encrypt->SetPasswordBinary(password);
	}
	else{
		//オプションでパスワードファイルがない場合エラーを出すように設定している
		if ( opthdl->fNoErrMsgOnPassFile == false ) {
			//'パスワードファイルを開けません。他のアプリケーションで使用中の可能性があります。';
			MsgText = LoadResourceString(&Msgunit1::_MSG_ERROR_OPEN_PASSWORD_FILE)+"\n"+
								opthdl->PassFilePathDecrypt;
			MessageDlg(MsgText, mtError, TMsgDlgButtons() << mbOK, 0);
		}
		return;
	}

}
else{
	//テキストボックスの内容をパスワードにする
	Password = (AnsiString)txtPasswordConfirm->Text;
	StrLCopy(password, Password.c_str(), 32);
	encrypt->SetPasswordBinary(password);
}

//タスクバー進捗表示（Win7）失敗した場合は無視
if(CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_ALL, IID_ITaskbarList3, (void**)&tskpbr) == S_OK) {
	tskpbr->SetProgressState(this->Handle, TBPF_NORMAL);
}

//進捗をTimerで監視
TimerEncrypt->Enabled = true;

//暗号化の実行
encrypt->Start();


}
//---------------------------------------------------------------------------
// ファイルを復号する処理
//---------------------------------------------------------------------------
void __fastcall TForm1::FileDecrypt(void)
{

int i;

String MsgText;

int FileVersion;
AnsiString Password;
char password[32];

for (i = 0; i < 32; i++) {
	password[i] = NULL;
}

AnsiString PasswordFileHash, PasswordFileHeader;

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

if ( FileList->Count > 0) {

	//-----------------------------------
	//復号処理インスタンスの作成
	//-----------------------------------

	AtcFilePath = FileList->Strings[FileListPosition];

	CheckAtcFileHeader(AtcFilePath, FileVersion);

	if (FileVersion < 104 ) {
		decrypt1 = new TAttacheCaseFileDecrypt1(true);         //旧バージョンファイル（ver.1）
		decrypt1->OnTerminate = DecryptThreadTerminated;
		decrypt1->FreeOnTerminate = false;
		decrypt1->AppExeFilePath = Application->ExeName;         //アタッシェケース本体の場所（実行形式出力のときに参照する）
		decrypt1->AtcFilePath = AtcFilePath;                     //入力する暗号化ファイルパス
		decrypt1->OutDirPath = OutDirPath;                       //出力するディレクトリ

		decrypt1->fOpenFolder = opthdl->fOpenFolder;             //フォルダの場合に復号後に開くか
		decrypt1->fOpenFile = opthdl->fOpenFile;                 //復号したファイルを関連付けされたソフトで開く
		decrypt1->fConfirmOverwirte = opthdl->fConfirmOverwirte; //同名ファイルの上書きを確認するか

		if (AtcFilePath.Compare(RetryAtcFilePath) == 0) {
			decrypt1->NumOfTrials = RetryNum;
		}
	}
	else{
		decrypt2 = new TAttacheCaseFileDecrypt2(true);
		decrypt2->OnTerminate = DecryptThreadTerminated;
		decrypt2->FreeOnTerminate = false;
		decrypt2->AppExeFilePath = Application->ExeName;         //アタッシェケース本体の場所（実行形式出力のときに参照する）
		decrypt2->AtcFilePath = AtcFilePath;                     //入力する暗号化ファイルパス
		decrypt2->OutDirPath = OutDirPath;                       //出力するディレクトリ

		decrypt2->fOpenFolder = opthdl->fOpenFolder;             //フォルダの場合に復号後に開くか
		decrypt2->fOpenFile = opthdl->fOpenFile;                 //復号したファイルを関連付けされたソフトで開く
		decrypt2->fConfirmOverwirte = opthdl->fConfirmOverwirte; //同名ファイルの上書きを確認するか

		if (AtcFilePath.Compare(RetryAtcFilePath) == 0) {
			decrypt2->NumOfTrials = RetryNum;
		}
	}

	Form1->Caption = ExtractFileName(AtcFilePath) + " - " + Application->Title;

	//-----------------------------------
	//パスワードのセット
	//-----------------------------------

	if (FileVersion > 103 ) {
		//パスワードファイルを使用するか
		if ( PasswordFilePath != "") {
			//SHA-1ハッシュを求める
			if ( opthdl->GetSHA1HashFromFile(PasswordFilePath, PasswordFileHash, PasswordFileHeader ) == true ){
				StrLCopy(password, PasswordFileHash.c_str(), 32);
				decrypt2->SetPasswordBinary(password);
			}
			else{
				//オプションでパスワードファイルがない場合エラーを出すように設定している
				if ( opthdl->fNoErrMsgOnPassFile == false ) {
					//'パスワードファイルを開けません。他のアプリケーションで使用中の可能性があります。';
					MsgText = LoadResourceString(&Msgunit1::_MSG_ERROR_OPEN_PASSWORD_FILE)+"\n"+
										opthdl->PassFilePathDecrypt;
					MessageDlg(MsgText, mtError, TMsgDlgButtons() << mbOK, 0);
				}
			}
		}
		else{
			//テキストボックスの内容をパスワードにする
			Password = (AnsiString)txtDecryptPassword->Text;
			StrLCopy(password, Password.c_str(), 32);
			decrypt2->SetPasswordBinary(password);
		}
	}
	else{ 	//旧バージョン（ver.1～）
		if ( PasswordFilePath != "") {
			//先頭文字列32文字を取得する
			if ( opthdl->GetSHA1HashFromFile(PasswordFilePath, PasswordFileHash, PasswordFileHeader ) == true ){
				decrypt1->SetPasswordString(PasswordFileHeader);
			}
			else{
				//オプションでパスワードファイルがない場合エラーを出すように設定している
				if ( opthdl->fNoErrMsgOnPassFile == false ) {
					//'パスワードファイルを開けません。他のアプリケーションで使用中の可能性があります。';
					MsgText = LoadResourceString(&Msgunit1::_MSG_ERROR_OPEN_PASSWORD_FILE)+"\n"+
										opthdl->PassFilePathDecrypt;
					MessageDlg(MsgText, mtError, TMsgDlgButtons() << mbOK, 0);
				}
			}
		}
		else{
			//テキストボックスの内容をパスワードにする
			Password = (AnsiString)txtDecryptPassword->Text;
			decrypt1->SetPasswordString(Password);
		}
	}

	//-----------------------------------
	//復号の実行
	//-----------------------------------
	if (FileVersion < 104 ) {
		decrypt1->Start();
	}
	else{
		decrypt2->Start();
	}

	//タスクバー進捗表示（Win7）
	if(CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_ALL, IID_ITaskbarList3, (void**)&tskpbr) != S_OK) {
		//失敗
	}
	if (tskpbr){
		tskpbr->SetProgressState(this->Handle, TBPF_NORMAL);
	}
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
char password[32];

for (i = 0; i < 32; i++) {
	password[i] = NULL;
}

String MsgText;

String AtcFilePath;
String OutDirPath;

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
else{
	AtcFilePath = encrypt->OutFilePath;
}

//復号処理インスタンスの作成
decrypt2 = new TAttacheCaseFileDecrypt2(true);
decrypt2->fCompare = true;	//コンペアON
decrypt2->CompareFileList->Text = encrypt->FilePathList->Text;
decrypt2->OnTerminate = DecryptThreadTerminated;
decrypt2->FreeOnTerminate = true;
decrypt2->AppExeFilePath = Application->ExeName;    //アタッシェケース本体の場所（実行形式出力のときに参照する）
decrypt2->AtcFilePath = AtcFilePath;                //入力する暗号化ファイルパス
decrypt2->OutDirPath = ExtractFileDir(AtcFilePath); //出力するディレクトリ

//-----------------------------------
//パスワードのセット
//-----------------------------------
encrypt->GetPasswordBinary(password);
decrypt2->SetPasswordBinary(password);

encrypt = NULL;

//コンペア（復号）の実行
decrypt2->Start();

//タスクバー進捗表示（Win7）
if(CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_ALL, IID_ITaskbarList3, (void**)&tskpbr) != S_OK) {
	//失敗
}
if (tskpbr){
	tskpbr->SetProgressState(this->Handle, TBPF_NORMAL);
}

//進捗をTimerで監視
TimerDecrypt->Enabled = true;


}
//---------------------------------------------------------------------------
//暗号化処理スレッドの終了
//---------------------------------------------------------------------------
void __fastcall TForm1::EncryptThreadTerminated(TObject *Sender)
{

//削除機能
String DelFileListString;
__int64 DelFileTotalSize;

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

if(tskpbr){
	tskpbr->SetProgressValue(this->Handle, 100, 100);
	tskpbr->Release();
	tskpbr = NULL;
}

//暗号化成功
if ( encrypt->StatusNum > 0 ) {

	//暗号化処理終了
	TimerEncrypt->Enabled = false;
	if ( chkDeleteSourceDataConf->Checked == true ) {
		DelFileTotalSize = encrypt->AllTotalSize;
		DelFileListString = encrypt->FilePathList->Text;	//処理したファイルリストを削除リストに
	}

	FileListPosition++;

	//コンペア
	if ( opthdl->fCompareFile == true ){
		FileCompare();
		return;
	}

	encrypt = NULL;

	//個別に暗号化するオプションでまだ処理するファイルが残っている
	if (FileListPosition < FileList->Count) {
		FileEncrypt();
		return;
	}

	//元ファイルの削除処理
	if ( chkDeleteSourceDataConf->Checked == true ) {
		DoDeleteFile(DelFileListString, DelFileTotalSize);
		return;
	}

	//処理後に終了する
	if ( opthdl->fEndToExit == true ) {
		Application->Terminate();
	}

	// "Cancel" → "OK"
	cmdCancel->Caption = "&OK";

}
else{
	//エラーで終了してきた
	TimerEncrypt->Enabled = false;
	encrypt = NULL;
}


}
//---------------------------------------------------------------------------
//復号処理スレッドの終了
//---------------------------------------------------------------------------
void __fastcall TForm1::DecryptThreadTerminated(TObject *Sender)
{

int ret;
String AtcFilePath;
int TypeLimits;
int AllTotalSize;

ProgressBar1->Style = pbstNormal;

if (decrypt1 == NULL) {
	ProgressBar1->Position = decrypt2->ProgressPercentNum;
	lblProgressPercentNum->Caption = IntToStr(decrypt2->ProgressPercentNum)+"%";
	lblStatus->Caption = decrypt2->ProgressStatusText;
	lblMsg->Caption = decrypt2->ProgressMsgText;
}
else{
	ProgressBar1->Position = decrypt1->ProgressPercentNum;
	lblProgressPercentNum->Caption = IntToStr(decrypt1->ProgressPercentNum)+"%";
	lblStatus->Caption = decrypt1->ProgressStatusText;
	lblMsg->Caption = decrypt1->ProgressMsgText;
}

if(tskpbr){
	tskpbr->SetProgressValue(this->Handle, 100, 100);
	tskpbr->Release();
	tskpbr = NULL;
}

TimerDecrypt->Enabled = false;

if (decrypt1 == NULL) {
	ret = decrypt2->StatusNum;
	AtcFilePath = decrypt2->AtcFilePath;
	TypeLimits = decrypt2->TypeLimits;
	AllTotalSize = decrypt2->AllTotalSize;
}
else{
	ret = decrypt1->StatusNum;
	AtcFilePath = decrypt1->AtcFilePath;
	TypeLimits = 3;
	AllTotalSize = decrypt1->AllTotalSize;
}

//-----------------------------------
//復号に成功
if ( ret > 0 ) {

	//復号処理終了
	TimerDecrypt->Enabled = false;
	DecryptAllTotalSize += AllTotalSize;

	//コンペアしてきた
	if (decrypt2 != NULL && decrypt2->fCompare == true) {
		//個別に暗号化するオプションでまだ処理するファイルが残っている
		if (FileListPosition < FileList->Count) {
			FileEncrypt();
			return;
		}
	}

	FileListPosition++;

	decrypt1 = NULL;
	decrypt2 = NULL;

	//個別に暗号化するオプションでまだ処理するファイルが残っている
	if (FileListPosition < FileList->Count) {
		FileDecrypt();
		return;
	}

	//暗号化ファイルの削除処理
	if ( chkDeleteAtcData->Checked == true ) {
		DoDeleteFile(FileList->Text, DecryptAllTotalSize);
		return;
	}

	//処理後に終了する
	if ( opthdl->fEndToExit == true ) {
		Application->Terminate();
	}
	else{
		// "Cancel" → "OK"
		cmdCancel->Caption = "&OK";
	}

}
//-----------------------------------
//復号に失敗
else{

	//パスワード入力エラーで抜けてきた
	if ( ret == -1 ) {
		//パスワード入力パネルへ戻る
		PageControlActiveSheet(TabSheetInputDecPass);
		txtDecryptPassword->SetFocus();
		txtDecryptPassword->SelectAll();

		RetryNum++;

		if ( RetryAtcFilePath.Compare(AtcFilePath) == 0 ) {
			//パスワード入力回数制限を超えた
			if ( RetryNum > TypeLimits ) {
				//破壊するオプション
				if ( decrypt2 != NULL && decrypt2->fDestroy == true ) {
					//破壊する
					DestroyAtcFile(RetryAtcFilePath);
				}
				else{
					decrypt1 = NULL;
					decrypt2 = NULL;
					TimerDecrypt->Enabled = false;
					Application->Terminate();
				}
			}
		}
		else{
			RetryAtcFilePath = AtcFilePath;
		}

	}
	//エラー
	else if ( ret == -2 ) {

	}
	//ユーザーキャンセル
	else{

	}

	// "Cancel" → "OK"
	cmdCancel->Caption = "&OK";

	//エラーで終了してきた
	TimerDecrypt->Enabled = false;
	decrypt1 = NULL;
	decrypt2 = NULL;

}

}
//---------------------------------------------------------------------------
//完全削除処理スレッドの終了
//---------------------------------------------------------------------------
void __fastcall TForm1::DeleteThreadTerminated(TObject *Sender)
{

String MsgText;

ProgressBar1->Style = pbstNormal;
ProgressBar1->Position = cmpdel->ProgressPercentNum;
	lblProgressPercentNum->Caption = IntToStr(cmpdel->ProgressPercentNum)+"%";

lblStatus->Caption = cmpdel->ProgressStatusText;
lblMsg->Caption = cmpdel->ProgressMsgText;

if(tskpbr){
	tskpbr->SetProgressValue(this->Handle, 100, 100);
	tskpbr->Release();
	tskpbr = NULL;
}

TimerDelete->Enabled = false;

//-----------------------------------
//削除処理に失敗
if ( cmpdel->StatusNum < 0 ) {
	//'ファイル/フォルダー削除処理に失敗しました。'+#13+
	//'一部のファイル/フォルダーが削除できずに残ってしまった可能性があります。';
	MsgText = LoadResourceString(&Msgunit1::_MSG_ERROR_DELETE_FIlES_FAILED);
	MessageDlg(MsgText, mtError, TMsgDlgButtons() << mbOK, 0);
}

//処理後に終了する
if ( opthdl->fEndToExit == true && cmpdel->StatusNum > -1) {
	//削除処理の終了
	cmpdel = NULL;
	Application->Terminate();
}
else{
	// "Cancel" → "OK"
	cmdCancel->Caption = "&OK";
	cmpdel = NULL;
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
		if (tskpbr) {
			tskpbr->SetProgressState(this->Handle, TBPF_INDETERMINATE);
		}
	}
	else{
		ProgressBar1->Style = pbstNormal;
		ProgressBar1->Position = encrypt->ProgressPercentNum;
		lblProgressPercentNum->Caption = encrypt->ProgressPercentNumText;
		//タスクバー進捗表示（Win7）
		if(tskpbr){
			tskpbr->SetProgressValue(this->Handle, encrypt->ProgressPercentNum, 100);
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

if (decrypt1 == NULL && decrypt2 == NULL) {
	TimerDecrypt->Enabled = false;
}
else{

	if (decrypt1 == NULL) {
		ProgressBar1->Position = decrypt2->ProgressPercentNum;
		lblProgressPercentNum->Caption = decrypt2->ProgressPercentNumText;

		lblStatus->Caption = decrypt2->ProgressStatusText;
		lblMsg->Caption = decrypt2->ProgressMsgText;

		//タスクバー進捗表示（Win7）
		if(tskpbr){
			tskpbr->SetProgressValue(this->Handle, decrypt2->ProgressPercentNum, 100);
		}
	}
	else{
		ProgressBar1->Position = decrypt1->ProgressPercentNum;
		lblProgressPercentNum->Caption = decrypt1->ProgressPercentNumText;

		lblStatus->Caption = decrypt1->ProgressStatusText;
		lblMsg->Caption = decrypt1->ProgressMsgText;

		//タスクバー進捗表示（Win7）
		if(tskpbr){
			tskpbr->SetProgressValue(this->Handle, decrypt1->ProgressPercentNum, 100);
		}
	}
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
		if (tskpbr) {
			tskpbr->SetProgressState(this->Handle, TBPF_INDETERMINATE);
		}
	}
	else{
		ProgressBar1->Style = pbstNormal;
		ProgressBar1->Position = cmpdel->ProgressPercentNum;
		lblProgressPercentNum->Caption = cmpdel->ProgressPercentNumText;
		//タスクバー進捗表示（Win7）
		if(tskpbr){
			tskpbr->SetProgressValue(this->Handle, cmpdel->ProgressPercentNum, 100);
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
// 暗号化ファイルのヘッダ判定
//---------------------------------------------------------------------------
int __fastcall TForm1::CheckAtcFileHeader(String AtcFilePath, int &Version)
{

//返値：
//#define TYPE_CRYPT_ENCRYPT 1	//暗号化
//#define TYPE_CRYPT_DECRYPT 2  //復号
//#define TYPE_CRYPT_ERROR  -1  //エラー

int i;
String MsgText;

int PlainHeaderSize;                                  //平文のヘッダサイズ
__int64 AllTotalSize;                                 //復号処理する合計サイズ

const char charTokenString[17] = "_AttacheCaseData";  //暗号化ファイルを示すトークン
const char charBrokenToken[17] = "_Atc_Broken_Data";  //ファイルが破壊されていることを示すトークン

TFileStream *fs;
char token[17];
ZeroMemory(token, 17);

Version = 0;

//-----------------------------------
//ファイル
//-----------------------------------
if ( FileExists(AtcFilePath) == true ) {

	try{
		fs = new TFileStream(AtcFilePath, fmOpenRead | fmShareDenyNone);
	}
	catch(...){
		//ファイルを開けない（エラー）
		//'ファイルを開けません。他のアプリケーションで使用中の可能性があります。';
		MsgText = LoadResourceString(&Msgunit1::_MSG_FILE_OPEN_ERROR) + "\n" + AtcFilePath;
		MessageDlg(MsgText, mtError, TMsgDlgButtons() << mbOK, 0);
		return(TYPE_CRYPT_ERROR);
	}

	fs->Seek((__int64)4, TSeekOrigin::soBeginning);
	fs->Read(token, 16);

	if ( StrComp(token, charBrokenToken) == 0 ) {
		// 'この暗号化ファイルはパスワード入力に失敗して破壊されているようです。復号できません。'
		MsgText = LoadResourceString(&Msgunit1::_MSG_BROKEN_ATC_FILE) + "\n" + AtcFilePath;
		MessageDlg(MsgText, mtInformation, TMsgDlgButtons() << mbOK, 0);
		delete fs;
		return(TYPE_CRYPT_ERROR);
	}
	else if ( StrComp(token, charTokenString) == 0 ) {
		fs->Read(&Version, sizeof(int));	//ファイルバージョン
		delete fs;
		return(TYPE_CRYPT_DECRYPT);	    //復号
	}
	else{
		//実は自己実行形式ファイル？（拡張子偽装されている場合もある）
		fs->Seek(-(__int64)sizeof(__int64), TSeekOrigin::soEnd);         // サイズを再取得
		fs->Read(&AllTotalSize, sizeof(__int64));
		fs->Seek(-(AllTotalSize + sizeof(__int64)), TSeekOrigin::soEnd); // 位置を戻す
		fs->Read(&PlainHeaderSize, sizeof(int));                         // もう一度、平文ヘッダサイズを読み込む
		fs->Read(token, 16);                                             // もう一度、トークンを取得

		if (StrComp(token, charTokenString) == 0 ) {                     // トークンを再チェック
			fs->Read(&Version, sizeof(int));	//ファイルバージョン
			delete fs;
			return(TYPE_CRYPT_DECRYPT);	    //復号
		}
		else{
			if ( StrComp(token, charBrokenToken) == 0 ) {
				// 'この暗号化ファイルはパスワード入力に失敗して破壊されているようです。復号できません。'
				MsgText = LoadResourceString(&Msgunit1::_MSG_BROKEN_ATC_FILE) + "\n" + AtcFilePath;
				MessageDlg(MsgText, mtInformation, TMsgDlgButtons() << mbOK, 0);
				delete fs;
				return(TYPE_CRYPT_ERROR);   //破壊されたファイル（エラー）
			}
			else{
				delete fs;
				return(TYPE_CRYPT_ENCRYPT); //暗号化
			}
		}
	}

}
//-----------------------------------
//ディレクトリ
//-----------------------------------
else{
	//暗号化処理へ
	return(TYPE_CRYPT_ENCRYPT);
}

}
//---------------------------------------------------------------------------
// ファイルリストにあるフォルダからすべてをファイルパスに展開する
//---------------------------------------------------------------------------
void __fastcall TForm1::ExpandFileList(TStringList *ExpandFileList)
{

String FilePath;
TStringList *ResultList = new TStringList;

for (int i = 0; i < ExpandFileList->Count; i++) {

	FilePath = ExpandFileList->Strings[i];

	if (FileExists(FilePath) == true ) {
		ResultList->Add(FilePath);
	}
	else if (DirectoryExists(FilePath) == true ){
		//ディレクトリパスをファイルパスリストに置き換える
		GetFileListInDirectroy(FilePath, ResultList);
	}
}

ExpandFileList->Text = ResultList->Text;

delete ResultList;

}
//---------------------------------------------------------------------------
// ディレクトリ内にあるファイルリストを取得する
//---------------------------------------------------------------------------
void __fastcall TForm1::GetFileListInDirectroy(String DirPath, TStringList *ResultList)
{

int ret;
TSearchRec sr;
String FilePath;

ret = FindFirst(IncludeTrailingPathDelimiter(DirPath)+"*", faAnyFile, sr);

while (ret == 0) {

	if (sr.Name != "." && sr.Name != "..") {

		FilePath = IncludeTrailingPathDelimiter(DirPath) + sr.Name;

		//-----------------------------------
		//ディレクトリ
		if (sr.Attr & faDirectory) {
			//再帰呼び出し
			GetFileListInDirectroy(FilePath, ResultList);
		}
		//-----------------------------------
		//ファイル
		else{
			ResultList->Add(FilePath);
		}
		//-----------------------------------

	}//end if;

	ret = FindNext(sr);

}//while;

FindClose(sr);


}
//---------------------------------------------------------------------------
void __fastcall TForm1::mnuFileClick(TObject *Sender)
{

//

}
//---------------------------------------------------------------------------
void __fastcall TForm1::mnuEncryptFilesClick(TObject *Sender)
{

String MsgText;

if (OpenDialogEncrypt->Execute() == true) {

	FileList->Text = OpenDialogEncrypt->Files->Text;  //暗号化する元ファイルリスト

	//記憶パスワードで即座に実行する
	if ( opthdl->fMemPasswordExe == true && opthdl->fMyEncodePasswordKeep == true) {
		//実行パネル表示
		PageControlActiveSheet(TabSheetExecute);
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
				PageControlActiveSheet(TabSheetInputEncPass);
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
		PageControlActiveSheet(TabSheetInputEncPass);
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

	FileList->Text = DirPath;

		//記憶パスワードで即座に実行する
	if ( opthdl->fMemPasswordExe == true && opthdl->fMyEncodePasswordKeep == true) {
		//実行パネル表示
		PageControlActiveSheet(TabSheetExecute);
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
				PageControlActiveSheet(TabSheetInputEncPass);
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
		PageControlActiveSheet(TabSheetInputEncPass);
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
    txtDecryptPassword->Text = opthdl->MyDecodePassword;
		//実行パネル表示
		PageControlActiveSheet(TabSheetExecute);
		FileDecrypt();
		return;
	}
	//復号時にパスワードファイルを自動チェックする
	else if ( opthdl->fAllowPassFile == true && opthdl->fCheckPassFileDecrypt == true ) {

		if ( FileExists(opthdl->PassFilePathDecrypt) == false ) {
			//パスワードファイルがない場合エラーを出さない
			if ( opthdl->fNoErrMsgOnPassFile == true ) {
				//メッセージを出さずにパスワード入力パネルへ
				PageControlActiveSheet(TabSheetInputDecPass);
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
		PageControlActiveSheet(TabSheetInputDecPass);
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
void __fastcall TForm1::mnuSettingClick(TObject *Sender)
{

//オプションパネルの表示
Form3 = new TForm3(this, opthdl);
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

if (decrypt1 != NULL) {
	decrypt1->Terminate();
	CanClose = false;
}

if (decrypt2 != NULL) {
	decrypt2->Terminate();
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
PageControlActiveSheet(TabSheetInputEncPassConfirm);
txtPasswordConfirm->SetFocus();

}
//---------------------------------------------------------------------------
void __fastcall TForm1::cmdEncryptPasswordCancelClick(TObject *Sender)
{

//メインパネルへ戻る
PageControlActiveSheet(TabSheetMain);

}
//---------------------------------------------------------------------------
void __fastcall TForm1::cmdConfirmOKClick(TObject *Sender)
{

String MsgText;

AnsiString PasswordFileHash[2] = {"", ""};
AnsiString PasswordFileHeader[2] = {"", ""};


//-----------------------------------
//パスワードファイルが使われている
if ( PasswordFilePath != "" ){

	bool fCheckPasswordFile = false;

	//最初に投げ込まれたファイルのSHA-1ハッシュを求める
	if ( opthdl->GetSHA1HashFromFile(PasswordFilePath, PasswordFileHash[0], PasswordFileHeader[0] ) == true ){
		//２番目のファイル
		if ( opthdl->GetSHA1HashFromFile(ConfirmPasswordFilePath, PasswordFileHash[1], PasswordFileHeader[1] ) == true ){

			if (PasswordFileHash[0].LowerCase() == PasswordFileHash[1].LowerCase()) {
				//同一のハッシュ値のようだ
				fCheckPasswordFile = true;
			}
		}
	}

	if ( fCheckPasswordFile == false ) {
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
PageControlActiveSheet(TabSheetExecute);
//暗号化開始
FileEncrypt();


}
//---------------------------------------------------------------------------
void __fastcall TForm1::cmdConfirmCancelClick(TObject *Sender)
{

//確認のテキストボックス・パスワードファイルパスをクリア
txtPasswordConfirm->Text = "";
ConfirmPasswordFilePath = "";

//パスワード入力パネルへ戻る
PageControlActiveSheet(TabSheetInputEncPass);
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
PageControlActiveSheet(TabSheetExecute);
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

int ret;
TForm *dlgconf;

if (encrypt != NULL) {        //暗号化キャンセル
	encrypt->Terminate();
}
else if (decrypt1 != NULL) {   //復号キャンセル
	decrypt1->Terminate();
}
else if (decrypt2 != NULL) {
	decrypt2->Terminate();
}
else if (cmpdel != NULL) {    //完全削除キャンセル

	//'中止すると完全に削除されない可能性があります。'+#13+
	//'それでも中止しますか？';
	String MsgText = LoadResourceString(&Msgunit1::_MSG_CONFIRM_COMPLETE_DELETE_STOP);
	dlgconf = CreateMessageDialog(MsgText, mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo, mbNo);
	dlgconf->Caption = LoadResourceString(&Msgunit1::_MSG_CAPTION_CONFIRMATION);	//'確認'
	ret = dlgconf->ShowModal();
	delete dlgconf;

	if (ret == mrYes) {
		cmpdel->Terminate();  //完全削除処理中止
	}

}
else{
	//メインパネルへ戻る
	PageControlActiveSheet(TabSheetMain);
}

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

			if ( OneLine.Compare(FileList->Strings[c]) == 0 ){
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

//フォルダ内のファイルは個別に暗号化/復号する
if (opthdl->fFilesOneByOne == true ) {
	ExpandFileList(FileList);
}

DoExecute(FileList);


}//end EvWmCOPYDATA;
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
//暗号化ファイルを破壊する
//---------------------------------------------------------------------------
bool __fastcall TForm1::DestroyAtcFile(String AtcFilePath)
{

int i;
int fh;

String MsgText;

char buffer[BUF_SIZE];

for ( i = 0; i < BUF_SIZE; i++ ){
	buffer[i]=NULL;
}

TFileStream *fsIn;

char token[16];
const char charTokenString[16] = "_AttacheCaseData";         //復号の正否に使う
const char charDestroyTokenString[16] = "_Atc_Broken_Data";  //破壊されているとき
String AtcFileTokenString;                                   //暗号化ファイルのトークン（文字列）
String AtcFileCreateDateString;                              //暗号化ファイルの生成日時（文字列）

__int64 AllTotalSize;
int PlaneHeaderSize;
int HeaderBufSize;

try {
#ifdef EXE_OUT //自己実行形式（自身を開く）
	fsIn = new TFileStream(AtcFilePath, fmOpenReadWrite | fmShareDenyNone);
#else
	fsIn = new TFileStream(AtcFilePath, fmOpenReadWrite | fmShareDenyNone);
#endif
}
catch(...) {
	//'ファイルを開けません。他のアプリケーションで使用中の可能性があります。'
	MsgText = LoadResourceString(&Msgunit1::_MSG_FILE_OPEN_ERROR);
	ShowConfirmMassageForm(MsgText, mtError, TMsgDlgButtons()<<mbOK, mbOK);
	return(false);
}

//総サイズ取得
AllTotalSize = fsIn->Seek((__int64)0, TSeekOrigin::soEnd);
fsIn->Seek((__int64)0, TSeekOrigin::soBeginning);

//-----------------------------------
//ヘッダ情報のチェック
//-----------------------------------

// 平文ヘッダサイズを取得
fsIn->Read(&PlaneHeaderSize, sizeof(int));
// トークンを取得
fsIn->Read(token, 16);

if (memcmp(token, charTokenString, 16) != 0 ) {

	//--------------------------------------------------------
	//実は自己実行形式ファイル？（拡張子偽装されている場合も）
	//--------------------------------------------------------
	// サイズを再取得
	fsIn->Seek(-(__int64)sizeof(__int64), TSeekOrigin::soEnd);
	fsIn->Read(&AllTotalSize, sizeof(__int64));
	// 位置を戻す
	fsIn->Seek(-(AllTotalSize + sizeof(__int64)), TSeekOrigin::soEnd);
	// もう一度、平文ヘッダサイズを読み込む
	fsIn->Read(&PlaneHeaderSize, sizeof(int));
	// もう一度、トークンを取得
	fsIn->Read(token, 16);

	// トークンを再チェック
	if (memcmp(token, charTokenString, 16) != 0 ) {
		//すでに壊れている？　サイレントに処理終了
		delete fsIn;
		return(true);
	}
	else{
		fsIn->Seek((__int64)-16, TSeekOrigin::soCurrent);
		//"_Atc_Broken_Data"を書き込む
		fsIn->Write(charDestroyTokenString, 16);
	}

}
else{
	fsIn->Seek((__int64)-16, TSeekOrigin::soCurrent);
	//"_Atc_Broken_Data"を書き込む
	fsIn->Write(charDestroyTokenString, 16);
}

//「データバージョン」「アルゴリズムの種類」分だけ進める
fsIn->Seek((__int64)(sizeof(int)*2), TSeekOrigin::soCurrent);
//暗号部ヘッダサイズを取得する
fsIn->Read(&HeaderBufSize, sizeof(int));
//暗号部ヘッダのIVを書き換えて破壊する
fsIn->Write(buffer, BUF_SIZE);
//「暗号部ヘッダサイズ」分だけ進める
fsIn->Seek((__int64)(HeaderBufSize-BUF_SIZE), TSeekOrigin::soCurrent);
// IV部分を書き換えて破壊する
fsIn->Write(buffer, BUF_SIZE);

delete fsIn;

return(true);


}
//---------------------------------------------------------------------------
//タブシートの選択
void __fastcall TForm1::PageControlActiveSheet(TTabSheet *tb)
{

TObject *Sender = NULL;
PageControl1->ActivePage = tb;
PageControl1Change(Sender);    // OnChangeイベント

}
//---------------------------------------------------------------------------
void __fastcall TForm1::PageControl1Change(TObject *Sender)
{

//-----------------------------------
// メインパネル
//-----------------------------------
if ( PageControl1->ActivePage == TabSheetMain ){

	mnuEncryptFiles->Enabled = true;
	mnuEncryptDir->Enabled = true;
	mnuDecrypt->Enabled = true;
	mnuSetting->Enabled = true;

	txtEncryptPassword->Text = "";
	txtPasswordConfirm->Text = "";
	txtDecryptPassword->Text = "";

	//パスワードファイルの変数を初期化する
	PasswordFilePath = "";
	ConfirmPasswordFilePath = "";

	//実行中パネルのボタンを「キャンセル」に戻す
	cmdCancel->Caption = "&Cancel";

	this->Caption = Application->Title;

	CryptTypeNum = 0;

}

//-----------------------------------
// 暗号化パスワード入力パネル
//-----------------------------------
else if ( PageControl1->ActivePage == TabSheetInputEncPass ){

	mnuEncryptFiles->Enabled = true;
	mnuEncryptDir->Enabled = true;
	mnuDecrypt->Enabled = false;
	mnuSetting->Enabled = false;

	chkExeFileOut->Checked = opthdl->fSaveToExeout;           //常に自己実行形式で出力する
	chkExeFileOut->Visible = opthdl->fShowExeoutChkBox;       //メインフォームにチェックボックスを表示する
	chkDeleteSourceData->Visible = opthdl->fShowDeleteChkBox; //「元ファイルを削除する」チェックボックス
	chkDeleteSourceData->Checked = opthdl->fDelOrgFile;

	//「*」で隠さずパスワードを確認しながら入力する
	if ( opthdl->fNoHidePassword == true ){
		txtEncryptPassword->PasswordChar = NULL;
		txtPasswordConfirm->PasswordChar = NULL;
	}
	else{
		txtEncryptPassword->PasswordChar = '*';
		txtPasswordConfirm->PasswordChar = '*';
	}

	//「元ファイルを削除する」チェックボックスの表示
	if ( chkDeleteSourceData->Visible == true ) {
		chkDeleteSourceData->Left = chkExeFileOut->Left;
		if ( chkExeFileOut->Visible == true ) {
			chkDeleteSourceData->Top = chkExeFileOut->BoundsRect.Bottom + 8;
		}
		else{
			chkDeleteSourceData->Top = chkExeFileOut->Top;
		}
	}

}
//-----------------------------------
// 暗号化パスワード再確認パネル
//-----------------------------------
else if ( PageControl1->ActivePage == TabSheetInputEncPassConfirm ){

	mnuEncryptFiles->Enabled = false;
	mnuEncryptDir->Enabled = false;
	mnuDecrypt->Enabled = false;
	mnuSetting->Enabled = false;

	chkExeFileOutConf->Visible = chkExeFileOut->Visible;
	chkExeFileOutConf->Checked = chkExeFileOut->Checked;
	chkDeleteSourceDataConf->Visible = chkDeleteSourceData->Visible;
	chkDeleteSourceDataConf->Checked = chkDeleteSourceData->Checked;

}
//-----------------------------------
// 復号パスワード入力パネル
//-----------------------------------
else if ( PageControl1->ActivePage == TabSheetInputDecPass ){
	mnuEncryptFiles->Enabled = false;
	mnuEncryptDir->Enabled = false;
	mnuDecrypt->Enabled = true;
	mnuSetting->Enabled = false;

	//「*」で隠さずパスワードを確認しながら入力する
	if ( opthdl->fNoHidePassword == true ){
		txtDecryptPassword->PasswordChar = NULL;
	}
	else{
		txtDecryptPassword->PasswordChar = '*';
	}

	chkDeleteAtcData->Visible = opthdl->fShowDeleteChkBox;
	chkDeleteAtcData->Checked = opthdl->fDelEncFile;

}
//-----------------------------------
// 実行パネル
//-----------------------------------
else if ( PageControl1->ActivePage == TabSheetExecute ){
	mnuEncryptFiles->Enabled = false;
	mnuEncryptDir->Enabled = false;
	mnuDecrypt->Enabled = false;
	mnuSetting->Enabled = false;

}

//サイドメニューを描画する
PaintSideMenu();


}
//---------------------------------------------------------------------------
void __fastcall TForm1::txtDecryptPasswordChange(TObject *Sender)
{

if (PasswordFilePath == "") {
	return;
}

//復号パスワードの再入力があったときはパスワードファイルパスをクリアする
if (txtDecryptPassword->Text != PasswordFilePath) {
	PasswordFilePath = "";
}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::txtEncryptPasswordChange(TObject *Sender)
{

if (PasswordFilePath == "") {
	return;
}

//暗号化パスワードの再入力があったときはパスワードファイルパスをクリアする
if (txtEncryptPassword->Text != PasswordFilePath) {
	PasswordFilePath = "";
}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::txtPasswordConfirmChange(TObject *Sender)
{

if (ConfirmPasswordFilePath == "") {
	return;
}

if (txtPasswordConfirm->Text != ConfirmPasswordFilePath) {
	ConfirmPasswordFilePath = "";
}

}
//---------------------------------------------------------------------------
// サイドメニューを描画する
//---------------------------------------------------------------------------
void __fastcall TForm1::PaintSideMenu(void)
{

bmpSideMenu->Width = PaintBoxMenu->Width;
bmpSideMenu->Height = PaintBoxMenu->Height;

//背景を敷き詰める
for (int PosY = 0; PosY < bmpSideMenu->Height; PosY+=imgMenuBackground->Height) {
	bmpSideMenu->Canvas->Draw(0, PosY, imgMenuBackground->Picture->Icon);
}

//-----------------------------------
//スタート・ウィンドウ
if ( PageControl1->ActivePage == TabSheetMain){

	//暗号化
	if ( fEncryptMenu == true ) {
		bmpSideMenu->Canvas->Draw(ptSideMenu[1].x, ptSideMenu[1].y, imgMenuEncryptOn->Picture->Icon);
		bmpSideMenu->Canvas->Font->Color = clWhite;	//キャプション
	}
	else{
		bmpSideMenu->Canvas->Draw(ptSideMenu[1].x, ptSideMenu[1].y, imgMenuEncryptOff->Picture->Icon);
		bmpSideMenu->Canvas->Font->Color = TColor(RGB(160,160,160)); //ラベル文字列を暗い色に
	}
	bmpSideMenu->Canvas->TextOut(SideBarCaptionPosX[1], ptSideMenu[1].y+48, SideBarCaption[1]);

	//復号する
	if ( fDecryptMenu == true ) {
		bmpSideMenu->Canvas->Draw(ptSideMenu[2].x, ptSideMenu[2].y, imgMenuDecryptOn->Picture->Icon);
		bmpSideMenu->Canvas->Font->Color = clWhite;
	}
	else{
		bmpSideMenu->Canvas->Draw(ptSideMenu[2].x, ptSideMenu[2].y, imgMenuDecryptOff->Picture->Icon);
		bmpSideMenu->Canvas->Font->Color = TColor(RGB(160,160,160));
	}
	bmpSideMenu->Canvas->TextOut(SideBarCaptionPosX[2], ptSideMenu[2].y+48, SideBarCaption[2]);

	//水平線
	bmpSideMenu->Canvas->Draw(ptSideMenu[3].x, ptSideMenu[3].y, imgMenuHorizontalLine->Picture->Icon);

	//オプション
	//復号する
	if ( fOptionMenu == true ) {
		bmpSideMenu->Canvas->Draw(ptSideMenu[4].x, ptSideMenu[4].y, imgMenuOptionOn->Picture->Icon);
		bmpSideMenu->Canvas->Font->Color = clWhite;
	}
	else{
		bmpSideMenu->Canvas->Draw(ptSideMenu[4].x, ptSideMenu[4].y, imgMenuOptionOff->Picture->Icon);
		bmpSideMenu->Canvas->Font->Color = TColor(RGB(160,160,160));
	}
	bmpSideMenu->Canvas->TextOut(SideBarCaptionPosX[4], ptSideMenu[4].y+48, SideBarCaption[4]);


}
//-----------------------------------
//暗号化ウィンドウ
else if ( PageControl1->ActivePage == TabSheetInputEncPass ||
		 PageControl1->ActivePage == TabSheetInputEncPassConfirm ){
	bmpSideMenu->Canvas->Draw(ptSideMenu[0].x, ptSideMenu[0].y, imgMenuEncryptOn->Picture->Icon);
	bmpSideMenu->Canvas->Font->Color = clWhite;
	bmpSideMenu->Canvas->TextOut(SideBarCaptionPosX[1], ptSideMenu[0].y+48, SideBarCaption[1]);

}
//-----------------------------------
//復号ウィンドウ
else if ( PageControl1->ActivePage == TabSheetInputDecPass ){
	bmpSideMenu->Canvas->Draw(ptSideMenu[0].x, ptSideMenu[0].y, imgMenuDecryptOn->Picture->Icon);
	bmpSideMenu->Canvas->Font->Color = clWhite;
	bmpSideMenu->Canvas->TextOut(SideBarCaptionPosX[2], ptSideMenu[0].y+48, SideBarCaption[2]);

}
//-----------------------------------
//実行ウィンドウ
else if ( PageControl1->ActivePage == TabSheetExecute ){

	bmpSideMenu->Canvas->Font->Color = clWhite;

	if (CryptTypeNum == 1 ) {   //暗号化
		bmpSideMenu->Canvas->Draw(ptSideMenu[0].x, ptSideMenu[0].y, imgMenuEncryptOn->Picture->Icon);
		bmpSideMenu->Canvas->TextOut(SideBarCaptionPosX[1], ptSideMenu[0].y+48, SideBarCaption[1]);
	}
	else{
		bmpSideMenu->Canvas->Draw(ptSideMenu[0].x, ptSideMenu[0].y, imgMenuDecryptOn->Picture->Icon);
		bmpSideMenu->Canvas->TextOut(SideBarCaptionPosX[2], ptSideMenu[0].y+48, SideBarCaption[2]);
	}
}

PaintBoxMenu->Canvas->Draw(0, 0, bmpSideMenu);

}
//---------------------------------------------------------------------------
void __fastcall TForm1::PaintBoxMenuPaint(TObject *Sender)
{

PaintBoxMenu->Canvas->Draw(0, 0, bmpSideMenu);

}
//---------------------------------------------------------------------------
void __fastcall TForm1::PaintBoxMenuMouseMove(TObject *Sender, TShiftState Shift,
					int X, int Y)
{

int ptIndex;

if ( PageControl1->ActivePage != TabSheetMain ) {
	return;
}

//カーソルの位置にアイコンがあるか
for (ptIndex = 4; ptIndex > 0; ptIndex--) {
	if ( Y > ptSideMenu[ptIndex].y) {
		if (Y < ptSideMenu[ptIndex].y+48) {
			if (X > ptSideMenu[ptIndex].x && X < ptSideMenu[ptIndex].x+48) {
				break;
			}
		}
	}
}

//どのアイコンが点灯中か
switch(ptIndex){
case 1:
	fEncryptMenu = true;
	fDecryptMenu = false;
	fOptionMenu = false;
	break;

case 2:
	fEncryptMenu = false;
	fDecryptMenu = true;
	fOptionMenu = false;
	break;

case 4:
	fEncryptMenu = false;
	fDecryptMenu = false;
	fOptionMenu = true;
	break;

default:
	fEncryptMenu = false;
	fDecryptMenu = false;
	fOptionMenu = false;
	break;
}

//メニューを再描画
PaintSideMenu();


}
//---------------------------------------------------------------------------
void __fastcall TForm1::PaintBoxMenuMouseDown(TObject *Sender, TMouseButton Button,
					TShiftState Shift, int X, int Y)
{

int ptIndex;

//カーソルの位置にアイコンがあるか
for (ptIndex = 4; ptIndex > 0; ptIndex--) {
	if ( Y > ptSideMenu[ptIndex].y) {
		if (Y < ptSideMenu[ptIndex].y+64) {
			if (X > ptSideMenu[ptIndex].x && X < ptSideMenu[ptIndex].x+64) {
				break;
			}
		}
	}
}

switch(ptIndex){
case 1:	//暗号化
	if (mnuEncryptFiles->Enabled == true) {
		mnuEncryptFilesClick(Sender);
		fEncryptMenu = false;
	}
	break;

case 2:	//復号
	if (mnuDecrypt->Enabled == true) {
		mnuDecryptClick(Sender);
		fDecryptMenu = false;
	}
	break;

case 4:	//オプション
	if (mnuSetting->Enabled == true) {
		mnuSettingClick(Sender);
		fOptionMenu = false;
	}
	break;

default:
	break;

}

SetFormComponent(Sender);

//メニューを再描画
PaintSideMenu();


}
//---------------------------------------------------------------------------

