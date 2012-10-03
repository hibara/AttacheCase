//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
TDragAndDrop *DragAndDropTarget;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{

txtInputPassword->Text = "";

this->Caption = ExtractFileName(Application->ExeName);

lblMsgInputPassword->Caption = LoadResourceString(&Msgexeout::_LABEL_CAPTION_INPUT_PASSWORD);
lblAbout->Caption = LoadResourceString(&Msgexeout::_LABEL_CAPTION_ABOUT);

chkSaveToOtherDirectory->Caption = LoadResourceString(&Msgexeout::_CHECK_BOX_SAVE_TO_OTHER_DIRECTORY);

cmdOK->Caption = LoadResourceString(&Msgexeout::_BUTTON_CAPTION_DECRYOTION);
cmdExit->Caption = LoadResourceString(&Msgexeout::_BUTTON_CAPTION_CANCEL);

RetryNum = 0;

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

}
//---------------------------------------------------------------------------
//フォームのリサイズイベント
//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{

txtInputPassword->Top = this->ClientHeight/2 - txtInputPassword->Height;

ProgressBar1->Top = txtInputPassword->Top;
lblProgressPercentNum->Top = ProgressBar1->BoundsRect.Bottom + 1;
lblStatus->Top = ProgressBar1->Top - lblStatus->Height - 1;
lblMsg->Top = lblStatus->Top - lblMsg->Height - 1;

imgKeyIcon->Top = txtInputPassword->Top + txtInputPassword->Height/2 - imgKeyIcon->Height/2;
chkSaveToOtherDirectory->Top = ProgressBar1->BoundsRect.Bottom + 4;
lblMsgInputPassword->Top = txtInputPassword->Top - lblMsgInputPassword->Height - 8;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{

if ( Key == VK_ESCAPE ) {
	Close();
}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::cmdExitClick(TObject *Sender)
{

if (ProgressBar1->Visible == true) {
	if (ProgressBar1->Position == 100 || ProgressBar1->Position == 0) {
		ChangeFormStatus(0);  //メインフォームへ戻る
		return;
	}
	else{
		if (decrypt != NULL) {
			decrypt->Terminate();
			return;
		}
		else{
			ChangeFormStatus(0);  //メインフォームへ戻る
			return;
		}
	}
}
else{
	Close();
}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::lblAboutMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
lblAbout->Font->Color = TColor(0x00CC0000);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::lblAboutMouseLeave(TObject *Sender)
{
lblAbout->Font->Color = TColor(0x00FF9999);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::lblAboutClick(TObject *Sender)
{

//バージョン情報の表示
Form2 = new TForm2(this);
Form2->PopupParent = Form1;
Form2->ShowModal();
Form2->Release();

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

DragAndDropTarget->fMultipleFilesOk = false;
DragAndDropTarget->DropImageType = DROPIMAGE_COPY;

//パスワード入力ボックス
px = txtInputPassword->Left;
py = txtInputPassword->Top;
pr = txtInputPassword->BoundsRect.Right;
pb = txtInputPassword->BoundsRect.Bottom;
rcTarget = Rect(ClientToScreen(TPoint(px, py)), ClientToScreen(TPoint(pr, pb)));

if (IntersectRect(rcResult, rcMouse, rcTarget) == true ) {
	txtInputPassword->Color = TColor(0xDBEBF7);  //オレンジ色
}
else{
	txtInputPassword->Color = clWindow;
}


}
//---------------------------------------------------------------------------
// DragLeave イベントのコールバック関数
//---------------------------------------------------------------------------
void __fastcall TForm1::FilesDragLeave(void)
{

txtInputPassword->Color = clWindow;

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
unsigned char *sha1hash;

String SHA1HashPassword = "";
TStringList *DropFileList = new TStringList;
DropFileList->Text = FileListText;

DragAndDropTarget->fMultipleFilesOk = false;
DragAndDropTarget->DropImageType =DROPIMAGE_COPY;

//パスワード入力ボックス
px = txtInputPassword->Left;
py = txtInputPassword->Top;
pr = txtInputPassword->BoundsRect.Right;
pb = txtInputPassword->BoundsRect.Bottom;
rcTarget = Rect(ClientToScreen(TPoint(px, py)), ClientToScreen(TPoint(pr, pb)));

if (IntersectRect(rcResult, rcMouse, rcTarget) == true && DropFileList->Count > 0) {

	txtInputPassword->Color = TColor(0xDBEBF7);  //オレンジ色

	PasswordFilePath = DropFileList->Strings[0];
	txtInputPassword->Text = PasswordFilePath;

	FileDecrypt();

}

txtInputPassword->Color = clWindow;
delete DropFileList;

}
//---------------------------------------------------------------------------
// フォーム状態の切り替え
//---------------------------------------------------------------------------
void __fastcall TForm1::ChangeFormStatus(int opt)
{

if (opt == 1) {                        //実行中
	//フォームを実行状態にする
	lblMsgInputPassword->Visible = false;
	txtInputPassword->Visible = false;
	imgKeyIcon->Visible = false;
	chkSaveToOtherDirectory->Visible = false;
	//プログレスバー関連の表示
	ProgressBar1->Visible = true;
	lblProgressPercentNum->Visible = true;
	lblStatus->Visible = true;
	lblMsg->Visible = true;
}
else{                                  //メイン表示
	//パスワード入力ボックスの表示
	lblMsgInputPassword->Visible = true;
	txtInputPassword->Visible = true;
	imgKeyIcon->Visible = true;
	chkSaveToOtherDirectory->Visible = true;
	//プログレスバー関連の表示を消す
	ProgressBar1->Visible = false;
	lblProgressPercentNum->Visible = false;
	lblStatus->Visible = false;
	lblMsg->Visible = false;
}

}
//---------------------------------------------------------------------------
// ファイルを復号する処理
//---------------------------------------------------------------------------
void __fastcall TForm1::FileDecrypt(void)
{

int i;
String MsgText;

char password[32];
for (i = 0; i < 32; i++) {
	password[i] = NULL;
}

AnsiString Password;
AnsiString PasswordFileHash, PasswordFileHeader;


String AtcFilePath;
String OutDirPath;
String RootDirPath = ExtractFileDir(Application->ExeName);

TSelectDirExtOpts opt;

//フォームを実行中状態にする
ChangeFormStatus(1);

//親フォルダを生成しない
//if ( fNoParentFldr == true  )

if ( chkSaveToOtherDirectory->Checked == true ) {

	opt = TSelectDirExtOpts() << sdShowShares << sdNewUI << sdNewFolder << sdShowEdit << sdValidateDir;
	// '復号する先のフォルダーを指定してください。'
	if (SelectDirectory(LoadResourceString(&Msgexeout::_DIALOG_MSG_SELECT_SAVE_TO_FOLDER),
			RootDirPath, OutDirPath, opt, this) == false){
		return;
	}

}
else{
	OutDirPath = ExtractFileDir(Application->ExeName);
}


if ( DirectoryExists(OutDirPath) == false ) {
	//'保存する先のフォルダーが見つかりません。保存設定を再確認してください。'+#13+
	//'復号処理化を中止します。';
	MsgText = LoadResourceString(&Msgexeout::_MSG_ERROR_SAVE_DEC_TO_FOLDER_NOT_EXISTS)+"\n"+
						OutDirPath;
	MessageDlg(MsgText, mtError, TMsgDlgButtons() << mbOK, 0);
	//エラー終了表示
	ProgressBar1->Position = 0;
	lblProgressPercentNum->Caption = " - %";
	//'キャンセル'
	lblStatus->Caption = LoadResourceString(&Msgexeout::_LABEL_STATUS_TITLE_USER_CANCEL);
	//'復号が中止されました。'
	lblMsg->Caption = LoadResourceString(&Msgexeout::_LABEL_STATUS_DETAIL_STOPPED);
	return;
}

//-----------------------------------
// 復号処理の開始
//-----------------------------------

//復号処理インスタンスの作成
decrypt = new TAttacheCaseFileDecrypt2(true);
decrypt->OnTerminate = DecryptThreadTerminated;
decrypt->FreeOnTerminate = true;
decrypt->AppExeFilePath = Application->ExeName;  //アタッシェケース本体の場所（実行形式出力のときに参照する）
decrypt->AtcFilePath = Application->ExeName;     //入力する暗号化ファイルパス（自分自身）
decrypt->OutDirPath = OutDirPath;                //出力するディレクトリ
decrypt->fConfirmOverwirte = true;               //上書きの確認

//-----------------------------------
//パスワードのセット
//-----------------------------------

//パスワードファイルを使用する
if (PasswordFilePath != "") {

	//SHA-1ハッシュを求める
	if ( GetSHA1HashFromFile(PasswordFilePath, PasswordFileHash, PasswordFileHeader) == true ){
		StrLCopy(password, PasswordFileHash.c_str(), 32);
		decrypt->SetPasswordBinary(password);
		FileDecrypt();
	}
	else{
		//'パスワードファイルを開けません。他のアプリケーションで使用中の可能性があります。';
		MsgText = LoadResourceString(&Msgexeout::_MSG_ERROR_OPEN_PASSWORD_FILE)+"\n"+PasswordFilePath;
		MessageDlg(MsgText, mtError, TMsgDlgButtons() << mbOK, 0);
	}

}
else{
	//文字列をセット
	Password = (AnsiString)txtInputPassword->Text;
	StrLCopy(password, Password.c_str(), 32);
	decrypt->SetPasswordBinary(password);
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
//===========================================================================
// ファイルからSHA-1ハッシュ値を取得する
//===========================================================================
bool __fastcall TForm1::GetSHA1HashFromFile(
	String FilePath,              // パスワードファイルパス
	AnsiString &HashDataVer2,     // ver.2.*  ～：SHA-1ハッシュ値（20byte + 12byte）
	AnsiString &HashDataVer1 )    // ver.1.*  ～：ヘッダデータ（先頭文字列32文字）
{

int i;

int fh;
int bytes, rest;
char buffer[255];	                               //読み込みバッファ
char sha1_hash_data_mixed_padding[BUF_HASH_DATA];// 32byte

for (i = 0; i < BUF_HASH_DATA; i++) {
	sha1_hash_data_mixed_padding[i] = NULL;
}

if ( !FileExists(FilePath) ) return(false);

try{

	if ( (fh = FileOpen(FilePath, fmShareDenyNone)) == -1 ){
		//パスワードファイルが開けない？
		return(false);
	}

	//ヘッダ先頭の255byteを読む（ver.1.* ～）
	FileRead(fh, buffer, 255);
	HashDataVer1 = (AnsiString)buffer;

	//SHA-1ハッシュの計算
	FileSeek(fh, 0, 0);
	SHA1Context sha;
	unsigned char Message_Digest[20];
	ZeroMemory(Message_Digest, 20);

	//初期化（リセット）
	if ( SHA1Reset(&sha)){
		return(false);
	}

	//ファイルを読み出してSHA-1へ入力していく
	while ((bytes = FileRead (fh, buffer, 255)) != 0){
		rest = bytes;
		if ( SHA1Input(&sha, (const unsigned char *)buffer, bytes) ){
			return(false);
		}
	}

	if ( FileSeek(fh, -rest, 2) > 0 ){
		FileRead(fh, buffer, 255);
	}
	else{
		FileSeek(fh, 0, 0);
		FileRead(fh, buffer, 255);
	}

	//出力
	if(SHA1Result(&sha, Message_Digest)){
		return(false);
	}
	for (i = 0; i < BUF_SHA1_SIZE; i++){   // 20byte
		sha1_hash_data_mixed_padding[i] = Message_Digest[i];
	}

	//残りの12byteを補填
	for (i = 0; i < 12; i++) {             // 12byte
		 sha1_hash_data_mixed_padding[BUF_SHA1_SIZE+i] = buffer[i];
	}

	HashDataVer2 = "";
	for (i = 0; i < BUF_HASH_DATA; i++){   // 32byte
		//適切にキャストされるように1byteずつ代入
		HashDataVer2 += (AnsiString)sha1_hash_data_mixed_padding[i];
	}
	HashDataVer2.SetLength(32);

}
__finally{

	FileClose(fh);

}
return(true);


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

TimerDecrypt->Enabled = false;

//復号に成功
if ( decrypt->StatusNum > 0 ) {

	decrypt = NULL;

}
//復号エラー
else{

	//パスワード入力エラーで抜けてきた
	if ( decrypt->StatusNum == -1 ) {

		//メインパネルへ戻る
		ChangeFormStatus(0);

		RetryNum++;

		//パスワード入力回数制限を超えた
		if ( RetryNum > decrypt->TypeLimits ) {
			//実行ファイルは自身を書き換えられないので終了するだけ
			decrypt = NULL;
			Application->Terminate();
		}

	}
	//エラー
	else if ( decrypt->StatusNum == -2 ) {

	}
	//ユーザーキャンセル
	else{

	}

	//エラーで終了してきた
	decrypt = NULL;

}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::cmdOKClick(TObject *Sender)
{

//復号開始
FileDecrypt();

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
//メッセージダイアログの表示（スレッドオブジェクトから呼ばれる）
//---------------------------------------------------------------------------
int __fastcall TForm1::ShowConfirmMassageForm
	(String MsgText, TMsgDlgType MsgType, TMsgDlgButtons MsgButtons, TMsgDlgBtn MsgDefaultButton)
{

TForm *dlg = CreateMessageDialog(MsgText, MsgType, MsgButtons, MsgDefaultButton);
dlg->ParentWindow = Form1->Handle;

if ( MsgType == mtError) {
	dlg->Caption = LoadResourceString(&Msgexeout::_MSG_CAPTION_ERROR);
}
else {
	dlg->Caption = LoadResourceString(&Msgexeout::_MSG_CAPTION_CONFIRMATION);
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
btn->Caption = LoadResourceString(&Msgexeout::_DIALOG_BUTTON_NAME_OVERWITE_YES_TO_ALL);
btn = (TButton *)dlg->FindComponent("Yes");        //'上書き(&O)'
btn->Caption = LoadResourceString(&Msgexeout::_DIALOG_BUTTON_NAME_OVERWITE_YES);
btn = (TButton *)dlg->FindComponent("No");         //'別名保存(&A)'
btn->Caption = LoadResourceString(&Msgexeout::_DIALOG_BUTTON_NAME_OVERWITE_NO);
btn = (TButton *)dlg->FindComponent("Cancel");     //'キャンセル(&C)'
btn->Caption = LoadResourceString(&Msgexeout::_DIALOG_BUTTON_NAME_OVERWITE_CANCEL);

dlg->Caption = LoadResourceString(&Msgexeout::_MSG_CAPTION_CONFIRMATION);
dlg->ParentWindow = Form1->Handle;

//メッセージダイアログの表示
int ret = dlg->ShowModal();

if (ret == mrNo) { //別名で保存する（いいえ）

	TSaveDialog *dlgsave = new TSaveDialog(dlg);
	//'復号ファイルの保存'
	dlgsave->Title = LoadResourceString(&Msgexeout::_DIALOG_SAVE_AS_FILE_CAPTION);
	dlgsave->InitialDir = ExtractFileDir(ExcludeTrailingPathDelimiter(Path));
	dlgsave->FileName = ExtractFileName(ExcludeTrailingPathDelimiter(Path));
	//'ファイルフォルダー|*'
	dlgsave->Filter = LoadResourceString(&Msgexeout::_DIALOG_SAVE_AS_FILTER);
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
bool __fastcall TForm1::DestroyAtcFile(void)
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
	fsIn = new TFileStream(Application->ExeName, fmShareDenyNone);
}
catch(...) {
	//'ファイルを開けません。他のアプリケーションで使用中の可能性があります。'
	MsgText = LoadResourceString(&Msgexeout::_MSG_FILE_OPEN_ERROR);
	ShowConfirmMassageForm(MsgText, mtError, TMsgDlgButtons()<<mbOK, mbOK);
	return(false);
}

//総サイズ取得
AllTotalSize = fsIn->Seek((__int64)0, TSeekOrigin::soEnd);
fsIn->Seek((__int64)0, TSeekOrigin::soBeginning);

//-----------------------------------
//ヘッダ情報のチェック
//-----------------------------------

// サイズを取得
fsIn->Seek(-(__int64)sizeof(__int64), TSeekOrigin::soEnd);
fsIn->Read(&AllTotalSize, sizeof(__int64));
// 位置を戻す
fsIn->Seek(-(AllTotalSize + sizeof(__int64)), TSeekOrigin::soEnd);
// 平文ヘッダサイズを読み込む
fsIn->Read(&PlaneHeaderSize, sizeof(int));
// トークンを取得
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
void __fastcall TForm1::txtInputPasswordChange(TObject *Sender)
{

if (PasswordFilePath == "") {
	return;
}

//暗号化パスワードの再入力があったときはパスワードファイルパスをクリアする
if (txtInputPassword->Text != PasswordFilePath) {
	PasswordFilePath = "";
}

}
//---------------------------------------------------------------------------

