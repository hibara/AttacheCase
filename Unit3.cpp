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

#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#if __CODEGEARC__ < 0x640
#pragma link "HTMLHelpViewer"
#else
#pragma link "Vcl.HTMLHelpViewer"
#endif

TForm3 *Form3;
TDragAndDrop *DragAndDropOptionTarget;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner, TAttacheCaseOptionHandle *opt)
	: TForm(Owner)
{

int i;

//設定クラスのインスタンス
pOpt = opt;

//-----------------------------------
//すべてのタブを消去
PageControl1->Align = alClient;
for ( i = 0; i < PageControl1->PageCount; i++) {
	PageControl1->Pages[i]->TabVisible = false;
}

//-----------------------------------
//サイドバーメニュー
//-----------------------------------

//メニューキャンバスの設定値
bmpSideMenu = new Graphics::TBitmap;
bmpSideMenu->Width = PaintBoxMenu->Width;
bmpSideMenu->Height = PaintBoxMenu->Height;
bmpSideMenu->Canvas->Brush->Style = bsClear;
bmpSideMenu->Canvas->Font->Color = clWhite;

//メニューアイコンの位置
ptSideMenu[0] = TPoint(-1, -1);
ptSideMenu[1] = TPoint(22, 16);
ptSideMenu[2] = TPoint(22, 88);
ptSideMenu[3] = TPoint(22,160);
ptSideMenu[4] = TPoint(22,232);
ptSideMenu[5] = TPoint(22,304);
ptSideMenu[6] = TPoint(22,376);

//ラベル文字
SideMenuLabelCaption[0] = "";
SideMenuLabelCaption[1] = LoadResourceString(&Msgunit3::_LABEL_BASIC);
SideMenuLabelCaption[2] = LoadResourceString(&Msgunit3::_LABEL_SAVE);
SideMenuLabelCaption[3] = LoadResourceString(&Msgunit3::_LABEL_DELETE);
SideMenuLabelCaption[4] = LoadResourceString(&Msgunit3::_LABEL_MOVEMENT);
SideMenuLabelCaption[5] = LoadResourceString(&Msgunit3::_LABEL_SYSTEM);
SideMenuLabelCaption[6] = LoadResourceString(&Msgunit3::_LABEL_ADVANCED);

//ラベル文字の配置
SideMenuLabelCaptionPosX[0] = 0;
for (i = 1; i < 7; i++) {
	SideMenuLabelCaptionPosX[i] = ptSideMenu[i].x+24 - bmpSideMenu->Canvas->TextWidth(SideMenuLabelCaption[i])/2;
}


//-----------------------------------
//【基本設定】タブ
//-----------------------------------
PanelBasicCaption->Caption = LoadResourceString(&Msgunit3::_PANEL_BASIC_CAPTION);
chkMyEncPasswordKeep->Caption = LoadResourceString(&Msgunit3::_BASIC_PANEL_CHECKBOX_MY_ENC_PASSWORD_KEEP);
chkMyDecPasswordKeep->Caption = LoadResourceString(&Msgunit3::_BASIC_PANEL_CHECKBOX_MY_DEC_PASSWORD_KEEP);
chkMyPasswordExe->Caption = LoadResourceString(&Msgunit3::_BASIC_PANEL_CHECKBOX_MY_PASSWORD_EXE);

chkWindowMinimize->Caption = LoadResourceString(&Msgunit3::_BASIC_PANEL_CHECKBOX_WINDOW_MINIMAIZE);
chkTaskBarHide->Caption = LoadResourceString(&Msgunit3::_BASIC_PANEL_CHECKBOX_TASKBAR_HIDE);
chkTaskTrayIcon->Caption = LoadResourceString(&Msgunit3::_BASIC_PANEL_CHECKBOX_TASKTRAY_ICON);
chkOpenFolder->Caption = LoadResourceString(&Msgunit3::_BASIC_PANEL_CHECKBOX_OPEN_FOLDER);
chkOpenFile->Caption = LoadResourceString(&Msgunit3::_BASIC_PANEL_CHECKBOX_OPEN_FILE);
chkEndToExit->Caption = LoadResourceString(&Msgunit3::_BASIC_PANEL_CHECKBOX_END_TO_EXIT);
chkWindowForeground->Caption = LoadResourceString(&Msgunit3::_BASIC_PANEL_CHECKBOX_WINDOW_FOREGROUND);
chkNoHidePassword->Caption = LoadResourceString(&Msgunit3::_BASIC_PANEL_CHECKBOX_NO_HIDE_PASSWORD);
chkSaveToExeout->Caption = LoadResourceString(&Msgunit3::_BASIC_PANEL_CHECKBOX_SAVE_TO_EXEOUT);
chkShowExeoutChkBox->Caption = LoadResourceString(&Msgunit3::_BASIC_PANEL_CHECKBOX_SHOW_EXEOUT_CHKBOX);
chkAskEncDecode->Caption = LoadResourceString(&Msgunit3::_BASIC_PANEL_CHECKBOX_ASK_ENC_DECODE);
chkNoMultipleInstance->Caption = LoadResourceString(&Msgunit3::_BASIC_PANEL_CHECKBOX_NO_MULTIPLE_INSTANCE);

//-----------------------------------
//【保存設定】タブ
//-----------------------------------
PanelSaveCaption->Caption = LoadResourceString(&Msgunit3::_PANEL_SAVE_CAPTION);
chkSaveToSameFldr->Caption = LoadResourceString(&Msgunit3::_SAVE_PANEL_CHECKBOX_SAVE_TO_SAME_FOLDER);
chkDecodeToSameFldr->Caption = LoadResourceString(&Msgunit3::_SAVE_PANEL_CHECKBOX_DECODE_TO_SAME_FOLDER);
chkConfirmOverwirte->Caption = LoadResourceString(&Msgunit3::_SAVE_PANEL_CHECKBOX_CONFIRM_OVERWRITE);
//複数ファイルがあるときの処理
radiogrpMultipleFiles->Caption = LoadResourceString(&Msgunit3::_SAVE_PANEL_RADIO_GROUP_MULTIPLE_FILES);
radiogrpMultipleFiles->Items->Add(LoadResourceString(&Msgunit3::_SAVE_PANEL_RADIO_GROUP_NO_ACTION));
radiogrpMultipleFiles->Items->Add(LoadResourceString(&Msgunit3::_SAVE_PANEL_RADIO_GROUP_ALL_FILES_PACK));
radiogrpMultipleFiles->Items->Add(LoadResourceString(&Msgunit3::_SAVE_PANEL_RADIO_GROUP_FILES_ONE_BY_ONE));
chkNoParentFldr->Caption = LoadResourceString(&Msgunit3::_SAVE_PANEL_CHECKBOX_NO_PARENT_FOLDER);
chkKeepTimeStamp->Caption = LoadResourceString(&Msgunit3::_SAVE_PANEL_CHECKBOX_KEEP_TIME_STAMP);
chkSameTimeStamp->Caption = LoadResourceString(&Msgunit3::_SAVE_PANEL_CHECKBOX_SAME_TIME_STAMP);
chkExtInAtcFileName->Caption = LoadResourceString(&Msgunit3::_SAVE_PANEL_CHECKBOX_EXT_IN_ATC_FILE_NAME);
chkAutoName->Caption = LoadResourceString(&Msgunit3::_SAVE_PANEL_CHECKBOX_AUTO_NAME);

//ポップアップメニュー
pmnuInsertFileName->Caption = LoadResourceString(&Msgunit3::_POPUP_MENU_ITEM_INSERT_FILENAME_FORMAT);
pmnuInsertFileExt->Caption = LoadResourceString(&Msgunit3::_POPUP_MENU_ITEM_INSERT_EXTENSION_FORMAT);
//--
pmnuInsertDatetime->Caption = LoadResourceString(&Msgunit3::_POPUP_MENU_ITEM_INSERT_DATETIME_FORMAT);
pmnuInsertSerialNum->Caption = LoadResourceString(&Msgunit3::_POPUP_MENU_ITEM_INSERT_SERIAL_NUM__FORMAT);
pmnuInsertRandomText->Caption = LoadResourceString(&Msgunit3::_POPUP_MENU_ITEM_INSERT_RANDOM_TEXT_FORMAT);
pmnuInsertFileNameHead->Caption = LoadResourceString(&Msgunit3::_POPUP_MENU_ITEM_INSERT_FILENAME_HEAD_FORMAT);
pmnuInsertFileNameEnd->Caption = LoadResourceString(&Msgunit3::_POPUP_MENU_ITEM_INSERT_FILENAME_END_FORMAT);

//-----------------------------------
//【削除設定】タブ
//-----------------------------------
PanelDeleteCaption->Caption = LoadResourceString(&Msgunit3::_PANEL_DELETE_CAPTION);
chkDelOrgFile->Caption = LoadResourceString(&Msgunit3::_DELETE_PANEL_CHECKBOX_DEL_ORG_FILE);
chkDelEncFile->Caption = LoadResourceString(&Msgunit3::_DELETE_PANEL_CHECKBOX_DEL_ENC_FILE);
chkShowDeleteChkBox->Caption = LoadResourceString(&Msgunit3::_DELETE_PANEL_CHECKBOX_SHOW_DELETE_CHKBOX);
//削除の詳細設定
gbDeleteOption->Caption = LoadResourceString(&Msgunit3::_DELETE_PANEL_RADIO_GROUP_DELETE_OPTION);
optNormalDelete->Caption = LoadResourceString(&Msgunit3::_DELETE_PANEL_RADIO_GROUP_NORMAL_DELETE);
optGoToTrash->Caption = LoadResourceString(&Msgunit3::_DELETE_PANEL_RADIO_GROUP_GO_TO_TRASH);
optCompleteDelete->Caption = LoadResourceString(&Msgunit3::_DELETE_PANEL_RADIO_GROUP_COMPLETE_DELETE);
txtDelRandNum->EditLabel->Caption = LoadResourceString(&Msgunit3::_DELETE_PANEL_DEL_RAND_NUM);
txtDelZeroNum->EditLabel->Caption = LoadResourceString(&Msgunit3::_DELETE_PANEL_DEL_ZERO_NUM);
//矢印アイコン
imgArrowDiabled->BoundsRect = imgArrowEnabled->BoundsRect;

//-----------------------------------
//【動作設定】タブ
//-----------------------------------
PanelMovementCaption->Caption = LoadResourceString(&Msgunit3::_PANEL_MOVEMENT_CAPTION);
//圧縮率設定
gbCompressRate->Caption = LoadResourceString(&Msgunit3::_MOVEMENT_PANEL_RADIO_GROUP_COMPRESS_RATE);
chkCompress->Caption = LoadResourceString(&Msgunit3::_MOVEMENT_PANEL_CHECKBOX_COMPRESS_RATE);
lblCompressRate->Caption = LoadResourceString(&Msgunit3::_MOVEMENT_PANEL_LABEL_COMPRESS_RATE_6);
lblCompressRateDetail->Caption = LoadResourceString(&Msgunit3::_MOVEMENT_PANEL_LABEL_COMPRESS_RATE_DETAIL);
//コンペア
gbCompare->Caption = LoadResourceString(&Msgunit3::_MOVEMENT_PANEL_RADIO_GROUP_COMPARE);
chkCompareFile->Caption = LoadResourceString(&Msgunit3::_MOVEMENT_PANEL_CHECKBOX_COMPARE_FILE);

//-----------------------------------
//【システム設定】タブ
//-----------------------------------
PanelSystemCaption->Caption = LoadResourceString(&Msgunit3::_PANEL_SYSTEM_CAPTION);
//Windowsシステムの設定
gbWindowsSystemOption->Caption = LoadResourceString(&Msgunit3::_SYSTEM_PANEL_RADIO_GROUP_WINDOWS_SYS_OPT);
cmdAssociateAtcFile->Caption = LoadResourceString(&Msgunit3::_SYSTEM_PANEL_BUTTON_ASSOCIATE_ATC_FILE);
cmdUnAssociateAtcFile->Caption = LoadResourceString(&Msgunit3::_SYSTEM_PANEL_BUTTON_UN_ASSOCIATE_ATC_FILE);
cmdShortCutToSendToFileDir->Caption = LoadResourceString(&Msgunit3::_SYSTEM_PANEL_BUTTON_SHORTCUT_TO_SENDTOFILE_DIR);
//ファイルアイコンの変更
gbChangeFileIcon->Caption = LoadResourceString(&Msgunit3::_SYSTEM_PANEL_RADIO_GROUP_CHANGE_FILE_ICON);
cmdAddFileIcon->Caption = LoadResourceString(&Msgunit3::_SYSTEM_PANEL_BUTTON_ADD_FILE_ICON);
//動作設定の出力
gbOutputOptionData->Caption = LoadResourceString(&Msgunit3::_SYSTEM_PANEL_RADIO_GROUP_OUTPUT_OPTION_DATA);
cmdOutputOptionData->Caption = LoadResourceString(&Msgunit3::_SYSTEM_PANEL_BUTTON_OUTPUT_OPTION_DATA);
cmdChangeTempOptionData->Caption = LoadResourceString(&Msgunit3::_SYSTEM_PANEL_BUTTON_CHANGE_TEMP_OPTION_DATA);

if ( pOpt->OptType == 1 ) {
	cmdChangeTempOptionData->Enabled = true;
}
else{
	cmdChangeTempOptionData->Enabled = false;
}


//-----------------------------------
//【高度設定】タブ
//-----------------------------------
PanelAdvancedCaption->Caption = LoadResourceString(&Msgunit3::_PANEL_ADVANCED_CAPTION);
//パスワードファイル
lblAdvancedOptionAlertMsg->Caption = LoadResourceString(&Msgunit3::_ADVANCED_PANEL_LABEL_ADVANCED_OPTION_ALERT_MSG);
gbPasswordFile->Caption = LoadResourceString(&Msgunit3::_ADVANCED_PANEL_RADIO_GROUP_PASSWORD_FILE);
chkAllowPassFile->Caption = LoadResourceString(&Msgunit3::_ADVANCED_PANEL_CHECKBOX_ALLOW_PASS_FILE);
chkCheckPassFile->Caption = LoadResourceString(&Msgunit3::_ADVANCED_PANEL_CHECKBOX_PASS_FILE);
chkCheckPassFileDecrypt->Caption = LoadResourceString(&Msgunit3::_ADVANCED_PANEL_CHECKBOX_PASS_FILE_DECRYPT);
chkNoErrMsgOnPassFile->Caption = LoadResourceString(&Msgunit3::_ADVANCED_PANEL_CHECKBOX_NO_ERR_MSG_ON_PASS_FILE);
//拡張子偽装
gbCamoExt->Caption = LoadResourceString(&Msgunit3::_ADVANCED_PANEL_RADIO_GROUP_CAMO_EXT);
chkAddCamoExt->Caption = LoadResourceString(&Msgunit3::_ADVANCED_PANEL_CHECKBOX_ADD_CAMO_EXT);
lblCamoExtDetail->Caption = LoadResourceString(&Msgunit3::_ADVANCED_PANEL_LABEL_CAMO_EXT_DETAIL);
//パスワード入力回数制限
gbTypeLimit->Caption = LoadResourceString(&Msgunit3::_ADVANCED_PANEL_RADIO_GROUP_TYPE_LIMIT);
lblTypeLimitAlertMsg->Caption = LoadResourceString(&Msgunit3::_ADVANCED_PANEL_LABEL_TYPE_LIMIT_ALERT_MSG);
lblMissTypeLimitsNum->Caption = LoadResourceString(&Msgunit3::_ADVANCED_PANEL_LABEL_TYPE_LIMIT_NUM);
chkBroken->Caption = LoadResourceString(&Msgunit3::_ADVANCED_PANEL_CHECKBOX_BROKEN);

//-----------------------------------
//【デバッグ】タブ
//-----------------------------------
PanelDebugCaption->Caption = LoadResourceString(&Msgunit3::_PANEL_DEBUG_CAPTION);
ValueListEditor1->Align = alClient;


//-----------------------------------
// 設定値をフォームに反映する
FormShowOptionData();


//-----------------------------------
// 開いていたタブページ
int ActiveTabNum = pOpt->ActiveTabNum;

if (ActiveTabNum > -1 &&  ActiveTabNum < 7) {
	PageControl1->ActivePageIndex = ActiveTabNum;
}
else{
	PageControl1->ActivePageIndex = 0;
}

//-----------------------------------
// サイドメニューを描画する
PaintSideMenu();

fChangeRegData = false;

}
//---------------------------------------------------------------------------
void __fastcall TForm3::FormCreate(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TForm3::FormDestroy(TObject *Sender)
{

pOpt->ActiveTabNum = PageControl1->ActivePageIndex;

//-----------------------------------
//OLEドラッグ＆ドロップの後始末
//-----------------------------------
RevokeDragDrop(Form3->Handle);
DragAndDropOptionTarget->Release();
OleUninitialize();


}
//---------------------------------------------------------------------------
void __fastcall TForm3::FormShow(TObject *Sender)
{

//-----------------------------------
//ドラッグ＆ドロップクラス
//-----------------------------------
OleInitialize(NULL);
//FormをCreateしてからでないと呼び出せない
DragAndDropOptionTarget = new TDragAndDrop(Form3);
DragAndDropOptionTarget->fMultipleFilesOk = false;	//複数ファイルドロップ不可
DragAndDropOptionTarget->FilesDragOver = FilesDragOver;
DragAndDropOptionTarget->FilesDragLeave = FilesDragLeave;
DragAndDropOptionTarget->FilesDragEnd = FilesDragEnd;
RegisterDragDrop(Form3->Handle, (IDropTarget*)DragAndDropOptionTarget);

//-----------------------------------
//読み込み先をフォームキャプションに表示

// INIファイル
if (pOpt->OptType == 1) {
	Form3->Icon = imgIni->Picture->Icon;
	//"INIファイル"
	Form3->Caption = LoadResourceString(&Msgunit3::_FORM_CAPTION_LOAD_INI_FILE) + pOpt->OptionPath;
}
// コマンドライン引数
else if (pOpt->OptType == 1) {
	Form3->Icon = imgBat->Picture->Icon;
	//"コマンドライン"
	Form3->Caption = LoadResourceString(&Msgunit3::_FORM_CAPTION_LOAD_COMMANDLINE) + pOpt->OptionPath;
}
// レジストリ
else{
	Form3->Icon = imgReg->Picture->Icon;
	//"レジストリ"
	Form3->Caption = LoadResourceString(&Msgunit3::_FORM_CAPTION_LOAD_REGISTRY) + pOpt->OptionPath;
}


}
//---------------------------------------------------------------------------
void __fastcall TForm3::FormResize(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TForm3::cmdCancelClick(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::cmdOKClick(TObject *Sender)
{

if (fChangeRegData == true) {
	//関連付け設定を更新
	RunAsAdmin(0);
}

//設定値を動作設定フォームに反映表示する
FormSaveOptionData();

Close();

}
//===========================================================================
// 設定値を動作設定フォームに反映表示する
//===========================================================================
void __fastcall TForm3::FormShowOptionData(void)
{

//----------------------------------------------------------------------
//【基本設定】タブ
//----------------------------------------------------------------------
//文字数分だけ"*"を表示する
int pNum;
chkMyEncPasswordKeep->Checked = pOpt->fMyEncodePasswordKeep;
if ( (pNum = pOpt->MyEncodePassword.Length()) > 32 ){
	pNum = 32;
}
btneditMyEncPassword->Text = String::StringOfChar('*', pNum);

chkMyDecPasswordKeep->Checked = pOpt->fMyDecodePasswordKeep;
if ( (pNum = pOpt->MyDecodePassword.Length()) > 32 ){
	pNum = 32;
}
btneditMyDecPassword->Text = String::StringOfChar('*', pNum);

chkMyPasswordExe->Checked = pOpt->fMemPasswordExe;
chkWindowMinimize->Checked = pOpt->fMainWindowMinimize;
chkTaskBarHide->Checked = pOpt->fTaskBarHide;
chkTaskTrayIcon->Checked = pOpt->fTaskTrayIcon;
chkOpenFolder->Checked = pOpt->fOpenFolder;
chkOpenFile->Checked = pOpt->fOpenFile;
chkEndToExit->Checked = pOpt->fEndToExit;
chkWindowForeground->Checked = pOpt->fWindowForeground;
chkNoHidePassword->Checked = pOpt->fNoHidePassword;
chkSaveToExeout->Checked = pOpt->fSaveToExeout;
chkShowExeoutChkBox->Checked = pOpt->fShowExeoutChkBox;
chkAskEncDecode->Checked = pOpt->fAskEncDecode;
chkNoMultipleInstance->Checked = pOpt->fNoMultipleInstance;

//----------------------------------------------------------------------
//【保存設定】タブ
//----------------------------------------------------------------------
chkSaveToSameFldr->Checked = pOpt->fSaveToSameFldr;
btneditSaveToSameFldrPath->Text = pOpt->SaveToSameFldrPath;
if (chkSaveToSameFldr->Checked == true) {
	btneditSaveToSameFldrPath->Enabled = true;
	btneditSaveToSameFldrPath->Color = clWindow;
}
else{
	btneditSaveToSameFldrPath->Enabled = false;
	btneditSaveToSameFldrPath->Color = clBtnFace;
}

chkDecodeToSameFldr->Checked = pOpt->fDecodeToSameFldr;
btneditDecodeToSameFldrPath->Text = pOpt->DecodeToSameFldrPath;
if (chkDecodeToSameFldr->Checked == true) {
	btneditDecodeToSameFldrPath->Enabled = true;
	btneditDecodeToSameFldrPath->Color = clWindow;
}
else{
	btneditDecodeToSameFldrPath->Enabled = false;
	btneditDecodeToSameFldrPath->Color = clBtnFace;
}

chkConfirmOverwirte->Checked = pOpt->fConfirmOverwirte;

//複数ファイルがあるときの処理
if ( pOpt->fAllFilePack == true ) {
	//複数のファイルを暗号化する際は一つにまとめる
	radiogrpMultipleFiles->ItemIndex = 1;
}
else if ( pOpt->fFilesOneByOne == true ) {
	//フォルダ内のファイルは個別に暗号化/復号する
	radiogrpMultipleFiles->ItemIndex = 2;
}
else{
	//何もしない
	radiogrpMultipleFiles->ItemIndex = 0;
}

chkNoParentFldr->Checked = pOpt->fNoParentFldr;
chkKeepTimeStamp->Checked = pOpt->fKeepTimeStamp;
chkSameTimeStamp->Checked = pOpt->fSameTimeStamp;
chkExtInAtcFileName->Checked = pOpt->fExtInAtcFileName;
chkAutoName->Checked = pOpt->fAutoName;
btneditAutoNameFormatText->Text = pOpt->AutoNameFormatText;


//----------------------------------------------------------------------
//【削除設定】タブ
//----------------------------------------------------------------------
chkDelOrgFile->Checked = pOpt->fDelOrgFile;
chkDelEncFile->Checked = pOpt->fDelEncFile;
chkShowDeleteChkBox->Checked = pOpt->fShowDeleteChkBox;

//削除回数
UpDownDelRand->Position = pOpt->DelRandNum;
UpDownDelZero->Position = pOpt->DelZeroNum;

//削除の詳細設定
if (pOpt->fCompleteDelete == 1) {      //完全削除
	optNormalDelete->Checked = false;
	optGoToTrash->Checked = false;
	optCompleteDelete->Checked = true;
}
else if (pOpt->fCompleteDelete == 2) { //ごみ箱へ移動
	optNormalDelete->Checked = false;
	optGoToTrash->Checked = true;
	optCompleteDelete->Checked = false;
}
else{                                  //通常削除
	optNormalDelete->Checked = true;
	optGoToTrash->Checked = false;
	optCompleteDelete->Checked = false;
}


//----------------------------------------------------------------------
//【動作設定】タブ
//----------------------------------------------------------------------
//圧縮率設定
if ( pOpt->CompressRate == 0 ) {
	chkCompress->Checked = false;
}
else{
	chkCompress->Checked = true;
}
TrackBar1->Position = pOpt->CompressRate;

//コンペア
chkCompareFile->Checked = pOpt->fCompareFile;


//----------------------------------------------------------------------
//【システム設定】タブ
//----------------------------------------------------------------------

//'「送る」フォルダーにアタッシェケースを追加(&S)';
String SendToFilePath = pOpt->CreateSendToFolderAppFullPath();

if ( FileExists(SendToFilePath) == true) {
	//'「送る」フォルダーからアタッシェケースを削除(&S)'
	cmdShortCutToSendToFileDir->Caption =
		LoadResourceString(&Msgunit3::_SYSTEM_PANEL_BUTTON_DELETE_FROM_SENDTOFILE_DIR);
}
else{
	//'「送る」フォルダーにアタッシェケースを追加(&S)';
	cmdShortCutToSendToFileDir->Caption =
		LoadResourceString(&Msgunit3::_SYSTEM_PANEL_BUTTON_SHORTCUT_TO_SENDTOFILE_DIR);
}

//ファイルアイコンの登録
RegIconFileToComboBox();

//動作設定の出力
bool fArgs = false;

//引数起動なら設定は保存できない
if ( ParamCount() > 0) {
	cmdOutputOptionData->Enabled = false;
	fArgs = true;
}

//引数起動か、またはiniファイルから
if ( fArgs == true || pOpt->OptType == 1){
	cmdChangeTempOptionData->Enabled = true; //一時設定を現在の設定に置き換える
}
else{
	cmdChangeTempOptionData->Enabled = false;
}


//----------------------------------------------------------------------
//【高度設定】タブ
//----------------------------------------------------------------------
//パスワードファイル
chkAllowPassFile->Checked = pOpt->fAllowPassFile;

//-----------------------------------
//暗号時にパスワードファイルを自動チェックする
if ( pOpt->fCheckPassFile == true &&	chkAllowPassFile->Checked == true ){
	chkCheckPassFile->Checked = true;
	btneditPassFilePath->Enabled = true;
	btneditPassFilePath->Color = clWindow;
}
else{
	chkCheckPassFile->Checked = false;
	btneditPassFilePath->Enabled = false;
	btneditPassFilePath->Color = clBtnFace;
}
btneditPassFilePath->Text = pOpt->PassFilePath;
btneditPassFilePath->SelStart = btneditPassFilePath->Text.Length(); //パス末尾の方を表示

//-----------------------------------
//復号時にパスワードファイルを自動チェックする
if ( pOpt->fCheckPassFileDecrypt == true && chkAllowPassFile->Checked == true ){
	chkCheckPassFileDecrypt->Checked = true;
	btneditPassFilePathDecrypt->Enabled = true;
	btneditPassFilePathDecrypt->Color = clWindow;
}
else{
	chkCheckPassFileDecrypt->Checked = false;
	btneditPassFilePathDecrypt->Enabled = false;
	btneditPassFilePathDecrypt->Color = clBtnFace;
}
btneditPassFilePathDecrypt->Text = pOpt->PassFilePathDecrypt;
btneditPassFilePathDecrypt->SelStart = btneditPassFilePathDecrypt->Text.Length(); //パス末尾の方を表示

chkNoErrMsgOnPassFile->Checked = pOpt->fNoErrMsgOnPassFile;

//拡張子偽装
if ( pOpt->fAddCamoExt == true ){
	chkAddCamoExt->Checked = true;
	txtCamoExt->Enabled = true;
	txtCamoExt->Color = clWindow;
}
else{
	chkAddCamoExt->Checked = false;
	txtCamoExt->Enabled = false;
	txtCamoExt->Color = clBtnFace;
}

txtCamoExt->Text = pOpt->CamoExt;

//パスワード入力回数制限
cmbMissTypeLimitsNum->ItemIndex = pOpt->MissTypeLimitsNum-1;
chkBroken->Checked = pOpt->fBroken;


}
//===========================================================================
// 動作設定フォームから設定値に反映する（レジストリ or INIファイルへの保存も行う）
//===========================================================================
void __fastcall TForm3::FormSaveOptionData(void)
{

//----------------------------------------------------------------------
//【基本設定】タブ
//----------------------------------------------------------------------

//暗号化パスワードを記憶する
pOpt->fMyEncodePasswordKeep = chkMyEncPasswordKeep->Checked;
if (TempMyEncodePassword != "" && pOpt->fMyEncodePasswordKeep == true) {
	pOpt->MyEncodePassword = TempMyEncodePassword;
	TempMyEncodePassword = "";
}

if ( chkMyEncPasswordKeep->Checked == false ){
	pOpt->MyEncodePassword = "";	//記憶パスワードのクリア
}

//復号パスワードを記憶する
pOpt->fMyDecodePasswordKeep = chkMyDecPasswordKeep->Checked;
if ( TempMyDecodePassword != "" && pOpt->fMyDecodePasswordKeep == true ) {
	pOpt->MyDecodePassword = TempMyDecodePassword;
	TempMyDecodePassword = "";
}

if ( chkMyDecPasswordKeep->Checked == false ){
	pOpt->MyDecodePassword = "";	//記憶パスワードのクリア
}

pOpt->fMainWindowMinimize = chkWindowMinimize->Checked;
pOpt->fTaskBarHide = chkTaskBarHide->Checked;
pOpt->fTaskTrayIcon = chkTaskTrayIcon->Checked;
pOpt->fMemPasswordExe = chkMyPasswordExe->Checked;
pOpt->fOpenFolder = chkOpenFolder->Checked;
pOpt->fOpenFile = chkOpenFile->Checked;
pOpt->fEndToExit = chkEndToExit->Checked;
pOpt->fWindowForeground = chkWindowForeground->Checked;
pOpt->fNoHidePassword = chkNoHidePassword->Checked;

pOpt->fSaveToExeout = chkSaveToExeout->Checked;
pOpt->fShowExeoutChkBox = chkShowExeoutChkBox->Checked;
pOpt->fAskEncDecode = chkAskEncDecode->Checked;
pOpt->fNoMultipleInstance = chkNoMultipleInstance->Checked;


//----------------------------------------------------------------------
//【保存設定】タブ
//----------------------------------------------------------------------
pOpt->fSaveToSameFldr = chkSaveToSameFldr->Checked;
pOpt->SaveToSameFldrPath = btneditSaveToSameFldrPath->Text;
pOpt->fDecodeToSameFldr = chkDecodeToSameFldr->Checked;
pOpt->DecodeToSameFldrPath = btneditDecodeToSameFldrPath->Text;

pOpt->fConfirmOverwirte = chkConfirmOverwirte->Checked;

//複数のファイルを暗号化する際は一つにまとめる
if ( radiogrpMultipleFiles->ItemIndex == 1 ){
	pOpt->fAllFilePack = true;
	pOpt->fFilesOneByOne = false;
}
//フォルダ内のファイルは個別に暗号化/復号する
else if ( radiogrpMultipleFiles->ItemIndex == 2 ){
	pOpt->fAllFilePack = false;
	pOpt->fFilesOneByOne = true;
}
else{
	//何もしない
	pOpt->fAllFilePack = false;
	pOpt->fFilesOneByOne = false;
}

pOpt->fNoParentFldr = chkNoParentFldr->Checked;
pOpt->fKeepTimeStamp = chkKeepTimeStamp->Checked;
pOpt->fSameTimeStamp = chkSameTimeStamp->Checked;
pOpt->fExtInAtcFileName = chkExtInAtcFileName->Checked;
pOpt->fAutoName = chkAutoName->Checked;
pOpt->AutoNameFormatText = btneditAutoNameFormatText->Text;


//----------------------------------------------------------------------
//【削除設定】タブ
//----------------------------------------------------------------------
pOpt->fDelOrgFile = chkDelOrgFile->Checked;
pOpt->fDelEncFile = chkDelEncFile->Checked;
pOpt->fShowDeleteChkBox = chkShowDeleteChkBox->Checked;

//削除の詳細設定
if ( optCompleteDelete->Checked == true ) { //完全削除
	pOpt->fCompleteDelete = 1;
}
else if ( optGoToTrash->Checked == true ) { //ごみ箱へ移動
	pOpt->fCompleteDelete = 2;
}
else{
	pOpt->fCompleteDelete = 0;                //通常削除
}

//削除回数
pOpt->DelRandNum = UpDownDelRand->Position;
pOpt->DelZeroNum = UpDownDelZero->Position;


//----------------------------------------------------------------------
//【動作設定】タブ
//----------------------------------------------------------------------
//圧縮率設定
pOpt->CompressRate = TrackBar1->Position;
//コンペア
pOpt->fCompareFile = chkCompareFile->Checked;


//----------------------------------------------------------------------
//【システム設定】タブ
//----------------------------------------------------------------------
//関連づけファイルアイコン番号
pOpt->AtcsFileIconIndex = comboDataIcon->ItemIndex + 1;
//ユーザー登録アイコンファイルのパス
pOpt->UserRegIconFilePath = TempUserRegIconFilePath;


//----------------------------------------------------------------------
//【高度設定】タブ
//----------------------------------------------------------------------
//パスワードファイル
pOpt->fAllowPassFile = chkAllowPassFile->Checked;

//-----------------------------------
//暗号時にパスワードファイルを自動チェックする
pOpt->fCheckPassFile = chkCheckPassFile->Checked;
pOpt->PassFilePath = btneditPassFilePath->Text;

//復号時にパスワードファイルを自動チェックする
pOpt->fCheckPassFileDecrypt = chkCheckPassFileDecrypt->Checked;
pOpt->PassFilePathDecrypt = btneditPassFilePathDecrypt->Text;

pOpt->fNoErrMsgOnPassFile = chkNoErrMsgOnPassFile->Checked;

//拡張子偽装
pOpt->fAddCamoExt = chkAddCamoExt->Checked;
pOpt->CamoExt = txtCamoExt->Text;

//パスワード入力回数制限
pOpt->MissTypeLimitsNum = cmbMissTypeLimitsNum->ItemIndex + 1;
pOpt->fBroken = chkBroken->Checked;


//----------------------------------------------------------------------
// 設定値をレジストリへ保存
//----------------------------------------------------------------------
pOpt->SaveOptionData();


}
//---------------------------------------------------------------------------
void __fastcall TForm3::TrackBar1Change(TObject *Sender)
{

//圧縮率の変更
switch(TrackBar1->Position){
case 0:
	lblCompressRate->Caption =
		LoadResourceString(&Msgunit3::_MOVEMENT_PANEL_LABEL_COMPRESS_RATE_0);
	break;
case 1:
	lblCompressRate->Caption =
		LoadResourceString(&Msgunit3::_MOVEMENT_PANEL_LABEL_COMPRESS_RATE_1);
	break;
case 2:
	lblCompressRate->Caption =
		LoadResourceString(&Msgunit3::_MOVEMENT_PANEL_LABEL_COMPRESS_RATE_2);
	break;
case 3:
	lblCompressRate->Caption =
		LoadResourceString(&Msgunit3::_MOVEMENT_PANEL_LABEL_COMPRESS_RATE_3);
	break;
case 4:
	lblCompressRate->Caption =
		LoadResourceString(&Msgunit3::_MOVEMENT_PANEL_LABEL_COMPRESS_RATE_4);
	break;
case 5:
	lblCompressRate->Caption =
		LoadResourceString(&Msgunit3::_MOVEMENT_PANEL_LABEL_COMPRESS_RATE_5);
	break;
case 7:
	lblCompressRate->Caption =
		LoadResourceString(&Msgunit3::_MOVEMENT_PANEL_LABEL_COMPRESS_RATE_7);
	break;
case 8:
	lblCompressRate->Caption =
		LoadResourceString(&Msgunit3::_MOVEMENT_PANEL_LABEL_COMPRESS_RATE_8);
	break;
case 9:
	lblCompressRate->Caption =
		LoadResourceString(&Msgunit3::_MOVEMENT_PANEL_LABEL_COMPRESS_RATE_9);
	break;
default:
	// 6: 標準圧縮
	lblCompressRate->Caption =
		LoadResourceString(&Msgunit3::_MOVEMENT_PANEL_LABEL_COMPRESS_RATE_6);
	break;
}

if ( TrackBar1->Position == 0 ) {
	chkCompress->Checked = false;
}
else{
	chkCompress->Checked = true;
}


}
//----------------------------------------------------------------------
//ファイルアイコンをコンボボックスへ登録する
void __fastcall TForm3::RegIconFileToComboBox(void)
{

//ファイルアイコンのロード

int i;
String FilePath;
String IconName;
TIcon *icon;

try{

	icon= new TIcon;
	TComboExItem *pItem;

	imlAssociateFilesIcon->Clear();
	comboDataIcon->Items->Clear();

	//リソースからアイコンファイルを登録
	for ( i = 0; i < FILE_ICON_NUM; i++ ){

		IconName = "SUBICON" + IntToStr(i);
		icon->LoadFromResourceName((int)HInstance, IconName);

		if(icon->Handle){
			imlAssociateFilesIcon->AddIcon(icon);
			pItem = comboDataIcon->ItemsEx->Add();
			pItem->ImageIndex = i;
			pItem->Caption = "Icon "+IntToStr(i+1);
		}

	}

	//ユーザー指定のアイコンファイルを登録
	if ( TempUserRegIconFilePath != "" ){
		FilePath = TempUserRegIconFilePath;
	}
	else{
		FilePath = pOpt->UserRegIconFilePath;
	}

	if ( FileExists(FilePath) == true ){
		icon->LoadFromFile(FilePath);
		imlAssociateFilesIcon->AddIcon(icon);
		pItem = comboDataIcon->ItemsEx->Add();
		pItem->ImageIndex = imlAssociateFilesIcon->Count-1;
		pItem->Caption = LoadResourceString(&Msgunit3::_SYSTEM_PANEL_COMBO_USER_REG_FILE_ICON);
		pOpt->AtcsFileIconIndex = comboDataIcon->ItemsEx->Count;
	}

	comboDataIcon->ItemIndex = pOpt->AtcsFileIconIndex-1;

}
__finally{

	delete icon;

}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::optNormalDeleteClick(TObject *Sender)
{
//'通常の削除を行う(&N)
imgArrowEnabled->Visible = false;
imgArrowDiabled->Visible = true;
UpDownDelRand->Enabled = false;
UpDownDelZero->Enabled = false;
txtDelRandNum->Color = clBtnFace;
txtDelZeroNum->Color = clBtnFace;
}
//---------------------------------------------------------------------------
void __fastcall TForm3::optGoToTrashClick(TObject *Sender)
{
//'ごみ箱への削除を行う(&T)'
imgArrowEnabled->Visible = false;
imgArrowDiabled->Visible = true;
UpDownDelRand->Enabled = false;
UpDownDelZero->Enabled = false;
txtDelRandNum->Color = clBtnFace;
txtDelZeroNum->Color = clBtnFace;
}
//---------------------------------------------------------------------------
void __fastcall TForm3::optCompleteDeleteClick(TObject *Sender)
{
//'完全削除を行う(&C)'
imgArrowEnabled->Visible = true;
imgArrowDiabled->Visible = false;
UpDownDelRand->Enabled = true;
UpDownDelZero->Enabled = true;
txtDelRandNum->Color = clWindow;
txtDelZeroNum->Color = clWindow;
}
//---------------------------------------------------------------------------
void __fastcall TForm3::btneditMyEncPasswordLeftButtonClick(TObject *Sender)
{
//暗号化の記憶パスワード入力パネルを表示
Form4 = new TForm4(this, 0);
Form4->ShowModal();
Form4->Release();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::btneditMyDecPasswordLeftButtonClick(TObject *Sender)
{
//復号する記憶パスワード入力パネルを表示
Form4 = new TForm4(this, 1);
Form4->ShowModal();
Form4->Release();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::btneditSaveToSameFldrPathLeftButtonClick(TObject *Sender)
{

String DirPath;
TSelectDirExtOpts opt =
	TSelectDirExtOpts() << sdNewUI << sdNewFolder << sdShowEdit << sdValidateDir;

// '暗号化ファイルを常に保存したいフォルダーを選択してください。'
if (SelectDirectory(
	LoadResourceString(
		&Msgunit3::_DIALOG_MSG_SELECT_SAVE_ATC_FILE_TO_DIR_PATH),
		L"", DirPath, opt, this) == true){

	btneditSaveToSameFldrPath->Text = DirPath;

}
else{
	chkSaveToSameFldr->Checked = false;

}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::btneditDecodeToSameFldrPathLeftButtonClick(TObject *Sender)
{

String DirPath;
TSelectDirExtOpts opt =
	TSelectDirExtOpts() << sdNewUI << sdNewFolder << sdShowEdit << sdValidateDir;

// '復号したファイルを常に保存したいフォルダーを選択してください。'
if (SelectDirectory(
	LoadResourceString(&Msgunit3::_DIALOG_MSG_SELECT_DECODE_TO_SAME_DIR_PATH),
		L"", DirPath, opt, this) == true){
	btneditDecodeToSameFldrPath->Text = DirPath;
}
else{
	chkDecodeToSameFldr->Checked = false;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::chkAllowPassFileClick(TObject *Sender)
{

//'パスワードファイルにファイルの指定を許可する(&D)'
if ( chkAllowPassFile->Checked == true ) {
	chkCheckPassFile->Enabled = true;
	btneditPassFilePath->Enabled = true;
	btneditPassFilePath->Color = clWindow;
	chkCheckPassFileDecrypt->Enabled = true;
	btneditPassFilePathDecrypt->Enabled = true;
	btneditPassFilePathDecrypt->Color = clWindow;
}
else{
	chkCheckPassFile->Enabled = false;
	btneditPassFilePath->Enabled = false;
	btneditPassFilePath->Color = clBtnFace;
	chkCheckPassFileDecrypt->Enabled = false;
	btneditPassFilePathDecrypt->Enabled = false;
	btneditPassFilePathDecrypt->Color = clBtnFace;
}

}
//---------------------------------------------------------------------------
//暗号時にパスワードファイルを自動チェックする
void __fastcall TForm3::chkCheckPassFileClick(TObject *Sender)
{

if ( this->Visible == false ) {
	return;
}

if ( chkCheckPassFile->Checked == true ){
	btneditPassFilePath->Enabled = true;
	btneditPassFilePath->Color = clWindow;
	if (btneditPassFilePath->Text == "") {
		btneditPassFilePathLeftButtonClick(Sender);
	}
}
else{
	btneditPassFilePath->Enabled = false;
	btneditPassFilePath->Color = clBtnFace;
}

}
//---------------------------------------------------------------------------
//復号時にパスワードファイルを自動チェックする
void __fastcall TForm3::chkCheckPassFileDecryptClick(TObject *Sender)
{

if ( this->Visible == false ) {
	return;
}

if ( chkCheckPassFileDecrypt->Checked == true ){
	btneditPassFilePathDecrypt->Enabled = true;
	btneditPassFilePathDecrypt->Color = clWindow;
	if (btneditPassFilePathDecrypt->Text == "") {
		btneditPassFilePathDecryptLeftButtonClick(Sender);
	}
}
else{
	btneditPassFilePathDecrypt->Enabled = false;
	btneditPassFilePathDecrypt->Color = clBtnFace;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::btneditPassFilePathLeftButtonClick(TObject *Sender)
{

//'暗号化するときのパスワードファイル選択'
OpenDialog1->Title = LoadResourceString(&Msgunit3::_DIALOG_CAPTION_SELECT_PASSWORD_FILE_ENCRYPT);
//'すべてのファイル(*.*)|*.*'
OpenDialog1->Filter = LoadResourceString(&Msgunit3::_DIALOG_SELECT_PASSWORD_FILE_FILTER_TEXT);

if ( OpenDialog1->Execute() == true ) {
	btneditPassFilePath->Text = OpenDialog1->FileName;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::btneditPassFilePathDecryptLeftButtonClick(TObject *Sender)
{

//'復号するときのパスワードファイルの選択'
OpenDialog1->Title = LoadResourceString(&Msgunit3::_DIALOG_MSG_SELECT_PASSWORD_FILE_DECRYPT);
//'すべてのファイル(*.*)|*.*'
OpenDialog1->Filter = LoadResourceString(&Msgunit3::_DIALOG_SELECT_PASSWORD_FILE_FILTER_TEXT);

if ( OpenDialog1->Execute() == true ) {
	btneditPassFilePathDecrypt->Text = OpenDialog1->FileName;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::chkAddCamoExtClick(TObject *Sender)
{

if ( chkAddCamoExt->Checked == true ) {
	txtCamoExt->Enabled = true;
	txtCamoExt->Color = clWindow;
}
else{
	txtCamoExt->Enabled = false;
	txtCamoExt->Color = clBtnFace;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::pmnuInsertFileNameClick(TObject *Sender)
{
//ファイル名タグを挿入
btneditAutoNameFormatText->SelText = "<filename>";
}
//---------------------------------------------------------------------------
void __fastcall TForm3::pmnuInsertFileExtClick(TObject *Sender)
{
//拡張子タグを挿入
btneditAutoNameFormatText->SelText = "<ext>";
}
//---------------------------------------------------------------------------
void __fastcall TForm3::pmnuInsertDatetimeClick(TObject *Sender)
{
Form4 = new TForm4(this, 2);	//日時挿入
Form4->ShowModal();
Form4->Release();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::pmnuInsertSerialNumClick(TObject *Sender)
{
Form4 = new TForm4(this, 3);	//連番挿入
Form4->ShowModal();
Form4->Release();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::pmnuInsertRandomTextClick(TObject *Sender)
{
Form4 = new TForm4(this, 4);	//ランダム文字列の挿入
Form4->ShowModal();
Form4->Release();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::pmnuInsertFileNameHeadClick(TObject *Sender)
{
Form4 = new TForm4(this, 5);	//ファイル名の先頭文字数の挿入
Form4->ShowModal();
Form4->Release();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::pmnuInsertFileNameEndClick(TObject *Sender)
{
Form4 = new TForm4(this, 6);	//ファイル名の末尾文字数の挿入
Form4->ShowModal();
Form4->Release();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::chkMyEncPasswordKeepClick(TObject *Sender)
{

//フォームをつくったときにクリックイベントが起きてしまうので、
//フォームが表示されているときのみ左ボタンイベントを呼ぶ
if ( this->Visible == true && chkMyEncPasswordKeep->Checked == true ) {
	btneditMyEncPasswordLeftButtonClick(Sender);
	if ( TempMyEncodePassword == "") {
		chkMyEncPasswordKeep->Checked = false;
	}
}

if ( chkMyEncPasswordKeep->Checked == true ) {
	btneditMyEncPassword->Enabled = true;
	btneditMyEncPassword->Color = clWindow;
}
else{
	btneditMyEncPassword->Enabled = false;
	btneditMyEncPassword->Color = clBtnFace;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::chkMyDecPasswordKeepClick(TObject *Sender)
{

if ( this->Visible == true && chkMyDecPasswordKeep->Checked == true ) {
	btneditMyDecPasswordLeftButtonClick(Sender);
	if ( TempMyDecodePassword == "") {
		chkMyDecPasswordKeep->Checked = false;
	}
}

if ( chkMyDecPasswordKeep->Checked == true ) {
	btneditMyDecPassword->Enabled = true;
	btneditMyDecPassword->Color = clWindow;
}
else{
	btneditMyDecPassword->Enabled = false;
	btneditMyDecPassword->Color = clBtnFace;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::chkSaveToSameFldrClick(TObject *Sender)
{

if ( chkSaveToSameFldr->Checked == true ) {
	btneditSaveToSameFldrPath->Enabled = true;
	btneditSaveToSameFldrPath->Color = clWindow;

	if (btneditSaveToSameFldrPath->Text == "") {
		btneditSaveToSameFldrPathLeftButtonClick(Sender);
	}

}
else{
	btneditSaveToSameFldrPath->Enabled = false;
	btneditSaveToSameFldrPath->Color = clBtnFace;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::chkDecodeToSameFldrClick(TObject *Sender)
{

if ( chkDecodeToSameFldr->Checked == true ) {
	btneditDecodeToSameFldrPath->Enabled = true;
	btneditDecodeToSameFldrPath->Color = clWindow;

	if (btneditDecodeToSameFldrPath->Text == "") {
		btneditDecodeToSameFldrPathLeftButtonClick(Sender);
	}

}
else{
	btneditDecodeToSameFldrPath->Enabled = false;
	btneditDecodeToSameFldrPath->Color = clBtnFace;
}


}
//---------------------------------------------------------------------------
void __fastcall TForm3::cmdAssociateAtcFileClick(TObject *Sender)
{
//'暗号化ファイル(*.atc)の関連付け設定(&A)';
RunAsAdmin(0);
}
//---------------------------------------------------------------------------
void __fastcall TForm3::cmdUnAssociateAtcFileClick(TObject *Sender)
{

//'暗号化ファイル(*.atc)の関連付け解除(&F)';
RunAsAdmin(1);
}
//---------------------------------------------------------------------------
void __fastcall TForm3::cmdShortCutToSendToFileDirClick(TObject *Sender)
{

//'「送る」フォルダーにアタッシェケースを追加(&S)';
String SendToFilePath = pOpt->CreateSendToFolderAppFullPath();

//ShowMessage(SendToFilePath);
//return;

if ( FileExists(SendToFilePath) == true) {
	//「送る」にあるショートカットファイルの削除
	DeleteFile(SendToFilePath);
	//'「送る」フォルダーにアタッシェケースを追加(&S)';
	cmdShortCutToSendToFileDir->Caption =
		LoadResourceString(&Msgunit3::_SYSTEM_PANEL_BUTTON_SHORTCUT_TO_SENDTOFILE_DIR);
}
else{
	// 「送る」にショートカットファイルを作成する
	if ( pOpt->CreateShortCutFile(SendToFilePath, Application->ExeName, "", "", 0 ) == false ){
		//エラー
		ShowMessage("エラー");
	}

	//'「送る」フォルダーからアタッシェケースを削除(&S)'
	cmdShortCutToSendToFileDir->Caption =
		LoadResourceString(&Msgunit3::_SYSTEM_PANEL_BUTTON_DELETE_FROM_SENDTOFILE_DIR);
}

}
//===========================================================================
//UACエレベーションを要求して関連付けプログラムを実行
//===========================================================================
bool __fastcall TForm3::RunAsAdmin(int Option)
{

// Option : 0  atcファイルの関連付け設定を行う
//          1  関連付け設定を解除する

String Parameters;
String MsgErrorString;
String AtcSetupExePath = ExtractFileDir(Application->ExeName)+"\\AtcSetup.exe";

//-----------------------------------
//関連付けプログラムの指定
if ( FileExists(AtcSetupExePath) == false ){
	//'アタッシェケースに付属する関連付けアプリケーション(AtcSetup.exe)が見つかりません。'
	MsgErrorString = LoadResourceString(&Msgunit3::_DIALOG_MSG_NOT_FOUND_ATC_SETUP_EXE);
	MessageDlg(MsgErrorString, mtError, TMsgDlgButtons() << mbOK, 0);
	return(false);
}

//-----------------------------------
//パラメータの設定
Parameters = IntToStr(Option);


//-----------------------------------
//UACエレベーションを要求して関連付けプログラムを呼び出す

TShellExecuteInfoW sei = {0};

if ( Win32MajorVersion >= 6 ) {
	sei.lpVerb = L"runas";	//Vista
}
else{
	sei.lpVerb = L"open";	//その他OS
}

sei.cbSize = sizeof(sei);
sei.hwnd = Application->Handle;
sei.fMask = SEE_MASK_FLAG_DDEWAIT | SEE_MASK_FLAG_NO_UI;
sei.lpFile = AtcSetupExePath.c_str();
sei.lpParameters = Parameters.c_str();
sei.nShow = SW_SHOWNOACTIVATE;

if ( ShellExecuteExW(&sei) == false){
		//RaiseLastOSError();
		return(false);
}

return(true);

}
//---------------------------------------------------------------------------
void __fastcall TForm3::chkAutoNameClick(TObject *Sender)
{

// '自動で暗号化ファイル名を付加する(&R)';
if ( chkAutoName->Checked == true ) {
	btneditAutoNameFormatText->Enabled = true;
	btneditAutoNameFormatText->Color = clWindow;
	btneditAutoNameFormatText->RightButton->Enabled = true;
	if (this->Visible == true) {
		btneditAutoNameFormatText->SetFocus();
	}
}
else{
	btneditAutoNameFormatText->Enabled = false;
	btneditAutoNameFormatText->Color = clBtnFace;
	btneditAutoNameFormatText->RightButton->Enabled = false;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::chkDelOrgFileClick(TObject *Sender)
{

//「削除の詳細設定」のグループボックスの有効無効
if ( chkDelOrgFile->Checked == true || chkDelEncFile->Checked == true ) {
	chkShowDeleteChkBox->Enabled = true;
	gbDeleteOption->Enabled = true;
	optNormalDelete->Enabled = true;
	optGoToTrash->Enabled = true;
	optCompleteDelete->Enabled = true;
}
else{
	chkShowDeleteChkBox->Enabled = false;
	gbDeleteOption->Enabled = false;
	optNormalDelete->Enabled = false;
	optGoToTrash->Enabled = false;
	optCompleteDelete->Enabled = false;
}

//グループボックス内の有効無効状態を変更
if (gbDeleteOption->Enabled == true) {

	if ( optCompleteDelete->Checked == true ){
		txtDelRandNum->Color = clWindow;
		txtDelZeroNum->Color = clWindow;
		imgArrowEnabled->Visible = true;
		imgArrowDiabled->Visible = false;
	}
	else{
		txtDelRandNum->Color = clBtnFace;
		txtDelZeroNum->Color = clBtnFace;
		imgArrowEnabled->Visible = false;
		imgArrowDiabled->Visible = true;
	}
}
else{
		txtDelRandNum->Color = clBtnFace;
		txtDelZeroNum->Color = clBtnFace;
		imgArrowEnabled->Visible = false;
		imgArrowDiabled->Visible = true;
}

}
//---------------------------------------------------------------------------
// DragOver イベントのコールバック関数
//---------------------------------------------------------------------------
void __fastcall TForm3::FilesDragOver(POINTL ptl)
{

int px, py, pr, pb;

TRect rcResult;
TRect rcMouse = TRect(ptl.x-1, ptl.y-1, ptl.x+1, ptl.y+1);   //マウス位置
TRect rcTarget;

//-----------------------------------
// 【基本設定】
//-----------------------------------
if ( PageControl1->ActivePage == TabBasic ){

	//-----------------------------------
	//暗号化パスワードを記憶するか（チェックボックス、テキストボックスにかかっているか）
	px = chkMyEncPasswordKeep->Left;
	py = chkMyEncPasswordKeep->Top;
	pr = btneditMyEncPassword->BoundsRect.Right;
	pb = btneditMyEncPassword->BoundsRect.Bottom;
	rcTarget = Rect(ClientToScreen(TPoint(px, py)), ClientToScreen(TPoint(pr, pb)));

	if (IntersectRect(rcResult, rcMouse, rcTarget) == true ) {
		btneditMyEncPassword->Color = TColor(0xDBEBF7);  //オレンジ色
	}
	else{
		btneditMyEncPassword->Color = clBtnFace;
	}

	//-----------------------------------
	//復号パスワードを記憶するか（チェックボックス、テキストボックスにかかっているか）
	px = chkMyDecPasswordKeep->Left;
	py = chkMyDecPasswordKeep->Top;
	pr = btneditMyDecPassword->BoundsRect.Right;
	pb = btneditMyDecPassword->BoundsRect.Bottom;
	rcTarget = Rect(ClientToScreen(TPoint(px, py)), ClientToScreen(TPoint(pr, pb)));

	if (IntersectRect(rcResult, rcMouse, rcTarget) == true ) {
		btneditMyDecPassword->Color = TColor(0xDBEBF7);  //オレンジ色
	}
	else{
		btneditMyDecPassword->Color = clBtnFace;
	}

}
//-----------------------------------
// 【保存設定】
//-----------------------------------
else if ( PageControl1->ActivePage == TabSave ){

	//-----------------------------------
	//'常に同じ場所へ暗号化ファイルを保存する(&C)'
	px = chkSaveToSameFldr->Left;
	py = chkSaveToSameFldr->Top;
	pr = btneditSaveToSameFldrPath->BoundsRect.Right;
	pb = btneditSaveToSameFldrPath->BoundsRect.Bottom;
	rcTarget = Rect(ClientToScreen(TPoint(px, py)), ClientToScreen(TPoint(pr, pb)));

	if (IntersectRect(rcResult, rcMouse, rcTarget) == true ) {
		btneditSaveToSameFldrPath->Color = TColor(0xDBEBF7);  //オレンジ色
	}
	else{
		if ( chkSaveToSameFldr->Checked == true ) {
			btneditSaveToSameFldrPath->Color = clWindow;
		}
		else{
			btneditSaveToSameFldrPath->Color = clBtnFace;
		}
	}

	//-----------------------------------
	//'常に同じ場所へファイルを復号する(&D)'
	px = chkDecodeToSameFldr->Left;
	py = chkDecodeToSameFldr->Top;
	pr = btneditDecodeToSameFldrPath->BoundsRect.Right;
	pb = btneditDecodeToSameFldrPath->BoundsRect.Bottom;
	rcTarget = Rect(ClientToScreen(TPoint(px, py)), ClientToScreen(TPoint(pr, pb)));

	if (IntersectRect(rcResult, rcMouse, rcTarget) == true ) {
		btneditDecodeToSameFldrPath->Color = TColor(0xDBEBF7);  //オレンジ色
	}
	else{
		if ( chkDecodeToSameFldr->Checked == true ) {
			btneditDecodeToSameFldrPath->Color = clWindow;
		}
		else{
			btneditDecodeToSameFldrPath->Color = clBtnFace;
		}
	}

}
//-----------------------------------
// 【高度な設定】
//-----------------------------------
else if ( PageControl1->ActivePage == TabAdvanced ){

	//-----------------------------------
	//'暗号化時にパスワードファイルを自動チェックする(&H)'
	px = gbPasswordFile->Left + chkCheckPassFile->Left;
	py = gbPasswordFile->Top + chkCheckPassFile->Top;
	pr = gbPasswordFile->Left + btneditDecodeToSameFldrPath->BoundsRect.Right;
	pb = gbPasswordFile->Top + btneditDecodeToSameFldrPath->BoundsRect.Bottom;
	rcTarget = Rect(ClientToScreen(TPoint(px, py)), ClientToScreen(TPoint(pr, pb)));

	if (IntersectRect(rcResult, rcMouse, rcTarget) == true ) {
		btneditPassFilePath->Color = TColor(0xDBEBF7);  //オレンジ色
	}
	else{
		if ( chkCheckPassFile->Checked == true ) {
			btneditPassFilePath->Color = clWindow;
		}
		else{
			btneditPassFilePath->Color = clBtnFace;
		}
	}

	//-----------------------------------
	//'復号時にパスワードファイルを自動チェックする(&K)';
	px = gbPasswordFile->Left + chkCheckPassFileDecrypt->Left;
	py = gbPasswordFile->Top + chkCheckPassFileDecrypt->Top;
	pr = gbPasswordFile->Left + btneditPassFilePathDecrypt->BoundsRect.Right;
	pb = gbPasswordFile->Top + btneditPassFilePathDecrypt->BoundsRect.Bottom;
	rcTarget = Rect(ClientToScreen(TPoint(px, py)), ClientToScreen(TPoint(pr, pb)));

	if (IntersectRect(rcResult, rcMouse, rcTarget) == true ) {
		btneditPassFilePathDecrypt->Color = TColor(0xDBEBF7);  //オレンジ色
	}
	else{
		if ( chkCheckPassFileDecrypt->Checked == true ) {
			btneditPassFilePathDecrypt->Color = clWindow;
		}
		else{
			btneditPassFilePathDecrypt->Color = clBtnFace;
		}
	}
	//-----------------------------------

}


}
//---------------------------------------------------------------------------
// DragLeave イベントのコールバック関数
//---------------------------------------------------------------------------
void __fastcall TForm3::FilesDragLeave(void)
{

//-----------------------------------
// 【基本設定】
//-----------------------------------
if ( PageControl1->ActivePage == TabBasic ){

	if ( chkMyEncPasswordKeep->Checked == true ) {
		btneditMyEncPassword->Enabled = true;
		btneditMyEncPassword->Color = clWindow;
	}
	else{
		btneditMyEncPassword->Enabled = false;
		btneditMyEncPassword->Color = clBtnFace;
	}

	if ( chkMyDecPasswordKeep->Checked == true ) {
		btneditMyDecPassword->Enabled = true;
		btneditMyDecPassword->Color = clWindow;
	}
	else{
		btneditMyDecPassword->Enabled = false;
		btneditMyDecPassword->Color = clBtnFace;
	}

}
//-----------------------------------
// 【保存設定】
//-----------------------------------
else if ( PageControl1->ActivePage == TabSave ){

	if ( chkSaveToSameFldr->Checked == true ) {
		btneditSaveToSameFldrPath->Enabled = true;
		btneditSaveToSameFldrPath->Color = clWindow;
	}
	else{
		btneditSaveToSameFldrPath->Enabled = false;
		btneditSaveToSameFldrPath->Color = clBtnFace;
	}

	if ( chkDecodeToSameFldr->Checked == true ) {
		btneditDecodeToSameFldrPath->Enabled = true;
		btneditDecodeToSameFldrPath->Color = clWindow;
	}
	else{
		btneditDecodeToSameFldrPath->Enabled = false;
		btneditDecodeToSameFldrPath->Color = clBtnFace;
	}

}
//-----------------------------------
// 【高度な設定】
//-----------------------------------
else if ( PageControl1->ActivePage == TabAdvanced ){

	//暗号時にパスワードファイルを自動チェックする
	if ( chkCheckPassFile->Checked == true ){
		btneditPassFilePath->Enabled = true;
		btneditPassFilePath->Color = clWindow;
	}
	else{
		btneditPassFilePath->Enabled = false;
		btneditPassFilePath->Color = clBtnFace;
	}

	//復号時にパスワードファイルを自動チェックする
	if ( chkCheckPassFileDecrypt->Checked == true ){
		btneditPassFilePathDecrypt->Enabled = true;
		btneditPassFilePathDecrypt->Color = clWindow;
	}
	else{
		btneditPassFilePathDecrypt->Enabled = false;
		btneditPassFilePathDecrypt->Color = clBtnFace;
	}

}


}
//---------------------------------------------------------------------------
// DragEnd イベントのコールバック関数
//---------------------------------------------------------------------------
void __fastcall TForm3::FilesDragEnd(POINTL ptl, String FileListText)
{

int res;
String MsgText;
AnsiString SHA1HashPassword;

TColor tmpColor;
TForm* dialog;

int px, py, pr, pb;

TRect rcResult;
TRect rcMouse = TRect(ptl.x-1, ptl.y-1, ptl.x+1, ptl.y+1); //マウス位置（少し緩めに設定）
TRect rcTarget;

String FilePath;
TStringList *FileList;

try{

	FileList = new TStringList;
	FileList->Text = FileListText;

	//-----------------------------------
	//複数ファイルのドロップを許可しない
	if ( FileList->Count > 0 ) {
		FilePath = FileList->Strings[0];
	}
	else{
		return;
	}

	//-----------------------------------
	// 【基本設定】
	//-----------------------------------
	if ( PageControl1->ActivePage == TabBasic ){

		//-----------------------------------
		//暗号化パスワードを記憶するか（チェックボックス、テキストボックスにかかっているか）
		px = chkMyEncPasswordKeep->Left;
		py = chkMyEncPasswordKeep->Top;
		pr = btneditMyEncPassword->BoundsRect.Right;
		pb = btneditMyEncPassword->BoundsRect.Bottom;
		rcTarget = Rect(ClientToScreen(TPoint(px, py)), ClientToScreen(TPoint(pr, pb)));

		if (IntersectRect(rcResult, rcMouse, rcTarget) == true ) {

			if ( chkAllowPassFile->Checked == false ) {
				//'パスワードファイルを使用するには【高度設定】タブの'+#13+
				//'「パスワードファイルにファイルの指定を許可する(&D)」を有効にしてください。';
				MsgText = LoadResourceString(&Msgunit3::_DIALOG_MSG_ALLOW_PASSWORD_FILE_OPTION);
				MessageDlg(MsgText, mtConfirmation, TMsgDlgButtons() << mbOK, 0);
				return;
			}

			tmpColor = btneditMyEncPassword->Color;
			btneditMyEncPassword->Color = TColor(0xDBEBF7);  //オレンジ色にして強調

			// 'ドラッグ＆ドロップされたファイルのSHA-1ハッシュ値を'+#13+
			//'暗号化パスワードとして記憶します。よろしいですか？';
			MsgText = LoadResourceString(&Msgunit3::_DIALOG_MSG_MEM_ENCRYPT_PASSWORD_HASH)+"\n"+
								FilePath;
			try{
				dialog = CreateMessageDialog(MsgText, mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo, mbNo);
				dialog->Caption = LoadResourceString(&Msgunit3::_MSG_CAPTION_CONFIRMATION);	//'確認'

				if (dialog->ShowModal() == ID_NO){
					btneditMyEncPassword->Color = tmpColor;        //元の色に戻す
					return;
				}
			}
			__finally{
				delete dialog;
			}

			//SHA-1ハッシュを求める
			if ( pOpt->GetSHA1HashFromFile(FilePath, SHA1HashPassword, (AnsiString&)"") == true ){
				//仮の記憶暗号化パスワード
				TempMyEncodePassword = SHA1HashPassword;
				btneditMyEncPassword->Text = String::StringOfChar('*', 32);
				chkMyEncPasswordKeep->OnClick = NULL;
				chkMyEncPasswordKeep->Checked = true;
				chkMyEncPasswordKeep->OnClick = chkMyEncPasswordKeepClick;
			}

		}

		//-----------------------------------
		//復号パスワードを記憶するか（チェックボックス、テキストボックスにかかっているか）
		//-----------------------------------
		px = chkMyDecPasswordKeep->Left;
		py = chkMyDecPasswordKeep->Top;
		pr = btneditMyDecPassword->BoundsRect.Right;
		pb = btneditMyDecPassword->BoundsRect.Bottom;
		rcTarget = Rect(ClientToScreen(TPoint(px, py)), ClientToScreen(TPoint(pr, pb)));

		if (IntersectRect(rcResult, rcMouse, rcTarget) == true ) {

			if ( chkAllowPassFile->Checked == false ) {
				//'パスワードファイルを使用するには【高度設定】タブの'+#13+
				//'「パスワードファイルにファイルの指定を許可する(&D)」を有効にしてください。';
				MsgText = LoadResourceString(&Msgunit3::_DIALOG_MSG_ALLOW_PASSWORD_FILE_OPTION);
				MessageDlg(MsgText, mtConfirmation, TMsgDlgButtons()<<mbOK, 0);
				return;
			}

			tmpColor = btneditMyDecPassword->Color;
			btneditMyDecPassword->Color = TColor(0xDBEBF7);  //オレンジ色にして強調

			//'ドラッグ＆ドロップされたファイルのSHA-1ハッシュ値を'+#13+
			//'復号するパスワードとして記憶します。よろしいですか？';
			MsgText = LoadResourceString(&Msgunit3::_DIALOG_MSG_MEM_DECRYPT_PASSWORD_HASH)+"\n"+
								FilePath;
			try{
				dialog = CreateMessageDialog(MsgText, mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo, mbNo);
				dialog->Caption = LoadResourceString(&Msgunit3::_MSG_CAPTION_CONFIRMATION);	//'確認'

				if (dialog->ShowModal() == ID_NO){
					btneditMyDecPassword->Color = tmpColor;        //元の色に戻す
					return;
				}
			}
			__finally{
        delete dialog;
      }
			//SHA-1ハッシュを求める
			if ( pOpt->GetSHA1HashFromFile(FilePath, SHA1HashPassword, (AnsiString&)"") == true ){
				//仮の記憶暗号化パスワード
				TempMyDecodePassword = SHA1HashPassword;
				btneditMyDecPassword->Text = String::StringOfChar('*', 32);
				chkMyDecPasswordKeep->OnClick = NULL;
				chkMyDecPasswordKeep->Checked = true;
				chkMyDecPasswordKeep->OnClick = chkMyDecPasswordKeepClick;
			}

		}
	}
	//-----------------------------------
	// 【保存設定】
	//-----------------------------------
	else if ( PageControl1->ActivePage == TabSave ){

		//-----------------------------------
		//'常に同じ場所へ暗号化ファイルを保存する(&C)'
		px = chkSaveToSameFldr->Left;
		py = chkSaveToSameFldr->Top;
		pr = btneditSaveToSameFldrPath->BoundsRect.Right;
		pb = btneditSaveToSameFldrPath->BoundsRect.Bottom;
		rcTarget = Rect(ClientToScreen(TPoint(px, py)), ClientToScreen(TPoint(pr, pb)));

		if ( IntersectRect(rcResult, rcMouse, rcTarget) == true ) {

			if (FileExists(FilePath) == true ) {
				chkSaveToSameFldr->Checked = true;
				btneditSaveToSameFldrPath->Text = ExtractFileDir(FilePath);
				btneditSaveToSameFldrPath->SetFocus();
			}
			else if ( DirectoryExists(FilePath) == true ){
				chkSaveToSameFldr->Checked = true;
				btneditSaveToSameFldrPath->Text = FilePath;
				btneditSaveToSameFldrPath->SetFocus();
			}

		}

		//-----------------------------------
		//'常に同じ場所へファイルを復号する(&D)'
		px = chkDecodeToSameFldr->Left;
		py = chkDecodeToSameFldr->Top;
		pr = btneditDecodeToSameFldrPath->BoundsRect.Right;
		pb = btneditDecodeToSameFldrPath->BoundsRect.Bottom;
		rcTarget = Rect(ClientToScreen(TPoint(px, py)), ClientToScreen(TPoint(pr, pb)));

		if ( IntersectRect(rcResult, rcMouse, rcTarget) == true ) {

			if ( FileExists(FilePath) == true ) {
				chkDecodeToSameFldr->Checked = true;
				btneditDecodeToSameFldrPath->Text = ExtractFileDir(FilePath);
				btneditDecodeToSameFldrPath->SetFocus();
			}
			else if ( DirectoryExists(FilePath) == true ){
				chkDecodeToSameFldr->Checked = true;
				btneditDecodeToSameFldrPath->Text = FilePath;
				btneditDecodeToSameFldrPath->SetFocus();
			}

		}

	}
	//-----------------------------------
	// 【高度な設定】
	//-----------------------------------
	else if ( PageControl1->ActivePage == TabAdvanced ){

		//-----------------------------------
		//'暗号化時にパスワードファイルを自動チェックする(&H)'
		px = gbPasswordFile->Left + chkCheckPassFile->Left;
		py = gbPasswordFile->Top + chkCheckPassFile->Top;
		pr = gbPasswordFile->Left + btneditPassFilePath->BoundsRect.Right;
		pb = gbPasswordFile->Top + btneditPassFilePath->BoundsRect.Bottom;
		rcTarget = Rect(ClientToScreen(TPoint(px, py)), ClientToScreen(TPoint(pr, pb)));

		if ( IntersectRect(rcResult, rcMouse, rcTarget) == true ) {

			if ( chkAllowPassFile->Checked == false || chkCheckPassFile->Checked == false) {

				tmpColor = btneditMyDecPassword->Color;
				btneditPassFilePath->Color = TColor(0xDBEBF7);  //オレンジ色にして強調

				//'パスワードファイル欄にファイルがドラッグ＆ドロップされました。'+#13+
				//'暗号化時に使うパスワードファイルとして登録して有効にしますか？';
				MsgText = LoadResourceString(&Msgunit3::_DIALOG_MSG_ALLOW_ENCRYPT_PASSWORD_FILE_ENABLED);
				res = MessageDlg(MsgText, mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0);

				if ( res == mrNo ) {
					btneditPassFilePath->Color = tmpColor;        //元の色に戻す
					return;
				}
			}

			if ( FileExists(FilePath) == true ) {
				btneditPassFilePath->Text = FilePath;
				chkAllowPassFile->Checked = true;
				chkCheckPassFile->OnClick = NULL;
				chkCheckPassFile->Checked = true;
				chkCheckPassFile->OnClick = chkCheckPassFileClick;
				btneditPassFilePath->SetFocus();
			}
			else{
				//'有効なファイル（たとえばディレクトリなど）以外は登録することができません。'
				MsgText = LoadResourceString(&Msgunit3::_DIALOG_MSG_ERROR_PASSWORD_FILE_WRONG)+"\n"+
									FilePath;
				MessageDlg(MsgText, mtError, TMsgDlgButtons() << mbOK, 0);
				btneditPassFilePath->Color = tmpColor;        //元の色に戻す
				return;
			}

		}

		//-----------------------------------
		//'復号時にパスワードファイルを自動チェックする(&K)';
		px = gbPasswordFile->Left + chkCheckPassFileDecrypt->Left;
		py = gbPasswordFile->Top + chkCheckPassFileDecrypt->Top;
		pr = gbPasswordFile->Left + btneditPassFilePathDecrypt->BoundsRect.Right;
		pb = gbPasswordFile->Top + btneditPassFilePathDecrypt->BoundsRect.Bottom;
		rcTarget = Rect(ClientToScreen(TPoint(px, py)), ClientToScreen(TPoint(pr, pb)));

		if (IntersectRect(rcResult, rcMouse, rcTarget) == true ) {

			if ( chkAllowPassFile->Checked == false || chkCheckPassFileDecrypt->Checked == false) {

				tmpColor = btneditPassFilePathDecrypt->Color;
				btneditPassFilePathDecrypt->Color = TColor(0xDBEBF7);  //オレンジ色にして強調

				//'パスワードファイル欄にファイルがドラッグ＆ドロップされました。'+#13+
				//'復号時に使うパスワードファイルとして登録して有効にしますか？';
				MsgText = LoadResourceString(&Msgunit3::_DIALOG_MSG_ALLOW_DECRYPT_PASSWORD_FILE_ENABLED);
				res = MessageDlg(MsgText, mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0);

				if ( res == mrNo ) {
					btneditPassFilePathDecrypt->Color = tmpColor;        //元の色に戻す
					return;
				}
			}

			if ( FileExists(FilePath) == true ) {
				btneditPassFilePathDecrypt->Text = FilePath;
				chkAllowPassFile->Checked = true;
				chkCheckPassFileDecrypt->OnClick = NULL;
				chkCheckPassFileDecrypt->Checked = true;
				chkCheckPassFileDecrypt->OnClick = chkCheckPassFileDecryptClick;
				btneditPassFilePathDecrypt->SetFocus();
			}
			else{
				//'有効なファイル（たとえばディレクトリなど）以外は登録することができません。'
				MsgText = LoadResourceString(&Msgunit3::_DIALOG_MSG_ERROR_PASSWORD_FILE_WRONG)+"\n"+
									FilePath;
				MessageDlg(MsgText, mtError, TMsgDlgButtons() << mbOK, 0);
				btneditPassFilePathDecrypt->Color = tmpColor;        //元の色に戻す
				return;
			}

		}
		//-----------------------------------

	}
}
__finally{

	delete FileList;

}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::cmdHelpClick(TObject *Sender)
{

//開いているページに応じたヘルプファイルのページを表示

if ( PageControl1->ActivePage == TabBasic ) {
	Application->HelpContext(IDH_BASIC_OPTION);
}
else if ( PageControl1->ActivePage == TabSave ) {
	Application->HelpContext(IDH_SAVE_OPTION);
}
else if ( PageControl1->ActivePage == TabDelete ) {
	Application->HelpContext(IDH_DELETE_OPTION);
}
else if ( PageControl1->ActivePage == TabMovement ) {
	Application->HelpContext(IDH_MOVEMENT_OPTION);
}
else if ( PageControl1->ActivePage == TabSystem ) {
	Application->HelpContext(IDH_SYSTEM_OPTION);
}
else if ( PageControl1->ActivePage == TabAdvanced ) {
	Application->HelpContext(IDH_ADVANCED_OPTION);
}
else {

}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::btneditAutoNameFormatTextChange(TObject *Sender)
{

//「自動で暗号化ファイル名を付加する」が変更されたとき
String FilePath =
	pOpt->InterpretFormatTextToFilePath("c:\\sample.atc", btneditAutoNameFormatText->Text);
lblAutoFormatExample->Caption = "ex)." + ExtractFileName(FilePath);

}
//---------------------------------------------------------------------------
//「現在の動作設定を出力する」
void __fastcall TForm3::cmdOutputOptionDataClick(TObject *Sender)
{

int OptType = pOpt->OptType; //現在の保存形式
// 'INIファイル(*.ini)|*.ini|すべてのファイル(*.*)|*.*';
SaveDialog1->Filter = LoadResourceString(&Msgunit3::_DIALOG_SAVE_TO_INIFILE_FILTER_TEXT);
SaveDialog1->FilterIndex = 0;
SaveDialog1->FileName = "_AtcCase.ini";
//'現在の動作設定をINIファイルに保存';
SaveDialog1->Title = LoadResourceString(&Msgunit3::_DIALOG_SAVE_TO_INIFILE_TITLE_TEXT);

if ( SaveDialog1->Execute() == true ) {
	pOpt->OptType = 1;
	pOpt->OptionPath = SaveDialog1->FileName;
	//ファイルを新規に作成してから
	TFileStream *fsIni = new TFileStream(SaveDialog1->FileName, fmCreate);
	delete fsIni;
	pOpt->SaveOptionData();
	pOpt->OptType = OptType;	//保存形式を戻す
}

}
//---------------------------------------------------------------------------
//「一時設定をメイン設定と置き換える」
void __fastcall TForm3::cmdChangeTempOptionDataClick(TObject *Sender)
{

//'現在、読み込まれている以下のINIファイル設定をレジストリへ反映しますか？';
String MsgText = LoadResourceString(&Msgunit3::_DIALOG_MSG_CONFIRM_OVERWRITE_SETTINGS);

int ret;
TForm *dlg;

try{

	dlg = CreateMessageDialog(MsgText, mtConfirmation, mbOKCancel, mbCancel);
	dlg->ParentWindow = this->Handle;
	//メッセージダイアログの表示
	ret = dlg->ShowModal();

	if ( ret == mrYes ) {
		pOpt->OptType = 0;			//保存先をレジストリに変更して保存
		pOpt->SaveOptionData();
		pOpt->OptionPath = "";
		cmdChangeTempOptionData->Enabled = false;
	}

}
__finally{

	delete dlg;

}

}
//---------------------------------------------------------------------------
// サイドメニューを描画する
//---------------------------------------------------------------------------
void __fastcall TForm3::PaintSideMenu(void)
{

int i;
Graphics::TIcon *icon;

//背景を敷き詰める
for (int PosY = 0; PosY < bmpSideMenu->Height; PosY+=imgMenuBackground->Height) {
	bmpSideMenu->Canvas->Draw(0, PosY, imgMenuBackground->Picture->Icon);
}

try{

for (i = 1; i < 7; i++) {

	icon = new Graphics::TIcon;
	if (PageControl1->ActivePageIndex+1 == i || optSelectedMenu == i) {
		imlSideMenuIconOn->GetIcon(i, icon);
		bmpSideMenu->Canvas->Font->Color = clWhite;
	}
	else{
		imlSideMenuIconOff->GetIcon(i, icon);
		bmpSideMenu->Canvas->Font->Color = TColor(RGB(160,160,160)); //ラベル文字列を暗い色に
	}
	bmpSideMenu->Canvas->Draw(ptSideMenu[i].x, ptSideMenu[i].y, icon);
	bmpSideMenu->Canvas->TextOut(SideMenuLabelCaptionPosX[i], ptSideMenu[i].y+48, SideMenuLabelCaption[i]);

}

PaintBoxMenu->Canvas->Draw(0, 0, bmpSideMenu);

}
__finally{
	delete icon;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::PaintBoxMenuPaint(TObject *Sender)
{

PaintBoxMenu->Canvas->Draw(0, 0, bmpSideMenu);

}
//---------------------------------------------------------------------------
void __fastcall TForm3::PaintBoxMenuMouseDown(TObject *Sender, TMouseButton Button,
					TShiftState Shift, int X, int Y)
{

int i;

//カーソルの位置にアイコンがあるか
for ( i = 6; i > 0; i--) {
	if ( Y > ptSideMenu[i].y) {
		if (Y < ptSideMenu[i].y+48) {
			if (X > ptSideMenu[i].x && X < ptSideMenu[i].x+48) {
				optSelectedMenu = i;
				break;
			}
		}
	}
}
if ( i > 0) {
	PageControl1->ActivePageIndex = optSelectedMenu-1;
	PaintSideMenu();
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::PaintBoxMenuMouseMove(TObject *Sender, TShiftState Shift,
					int X, int Y)
{

int i;

optSelectedMenu = 0;
//カーソルの位置にアイコンがあるか
for ( i = 6; i > 0; i--) {
	if ( Y > ptSideMenu[i].y) {
		if (Y < ptSideMenu[i].y+48) {
			if (X > ptSideMenu[i].x && X < ptSideMenu[i].x+48) {
				optSelectedMenu = i;
				break;
			}
		}
	}
}

if ( i > 0) {
	PaintSideMenu();
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::PaintBoxMenuDblClick(TObject *Sender)
{

TStringList *KeyValueList;

try{

	KeyValueList = new TStringList();
	pOpt->GetKeyValueData(KeyValueList);

	//設定値をまとめてデバッグ表示する
	for (int i = 0; i < KeyValueList->Count; i++) {
		ValueListEditor1->InsertRow(KeyValueList->Names[i], KeyValueList->ValueFromIndex[i], true);
	}

	//設定値のデバッグ一覧表示
	PageControl1->ActivePageIndex = 6;

}
__finally{

	delete KeyValueList;

}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::txtDelRandNumKeyPress(TObject *Sender, wchar_t &Key)

{

//数値のみの入力に抑制
if ( Key != '0' &&
		 Key != '1' &&
		 Key != '2' &&
		 Key != '3' &&
		 Key != '4' &&
		 Key != '5' &&
		 Key != '6' &&
		 Key != '7' &&
		 Key != '8' &&
		 Key != '9' &&
		 Key != '\b' &&
		 Key != VK_DELETE ){
			Key = 0;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::txtDelRandNumChange(TObject *Sender)
{

if ((txtDelRandNum->Text == "" || txtDelRandNum->Text == "0" ) &&
		(txtDelZeroNum->Text == "" || txtDelZeroNum->Text == "0")) {
	optNormalDelete->Checked = true;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::txtDelRandNumExit(TObject *Sender)
{

if (txtDelRandNum->Text == "" ){
	txtDelRandNum->Text == "0";
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::txtDelZeroNumKeyPress(TObject *Sender, wchar_t &Key)

{

//数値のみの入力に抑制
if ( Key != '0' &&
		 Key != '1' &&
		 Key != '2' &&
		 Key != '3' &&
		 Key != '4' &&
		 Key != '5' &&
		 Key != '6' &&
		 Key != '7' &&
		 Key != '8' &&
		 Key != '9' &&
		 Key != '\b' &&
		 Key != VK_DELETE ){
			Key = 0;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::txtDelZeroNumChange(TObject *Sender)
{

if ((txtDelRandNum->Text == "" || txtDelRandNum->Text == "0" ) &&
		(txtDelZeroNum->Text == "" || txtDelZeroNum->Text == "0")) {
	optNormalDelete->Checked = true;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::txtDelZeroNumExit(TObject *Sender)
{

if (txtDelRandNum->Text == "" ){
	txtDelRandNum->Text == "0";
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::comboDataIconChange(TObject *Sender)
{

//関連付けアイコンの変更を行った
fChangeRegData = true;

}
//---------------------------------------------------------------------------
//「ウィンドウを最小化したときタスクバーに表示しない(&H)」
void __fastcall TForm3::chkTaskBarHideClick(TObject *Sender)
{

if (chkTaskBarHide->Checked == true && chkTaskTrayIcon->Checked == false) {
	chkTaskTrayIcon->Checked = true;
}

}
//---------------------------------------------------------------------------
//「タスクトレイにアイコンを表示する(&C)」
void __fastcall TForm3::chkTaskTrayIconClick(TObject *Sender)
{

if (chkTaskTrayIcon->Checked == false && chkTaskBarHide->Checked == true) {
	chkTaskBarHide->Checked = false;
}

}
//---------------------------------------------------------------------------

