//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
TDragAndDrop *DragAndDropOptionTarget;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{

TRegIniFile *pOpt;

//-----------------------------------
//すべてのタブを消去
PageControl1->Align = alClient;
for (int i = 0; i < PageControl1->PageCount; i++) {
	PageControl1->Pages[i]->TabVisible = false;
}

//-----------------------------------
//サイドバーメニュー
//-----------------------------------
lblBasic->Caption = LoadResourceString(&Msgunit3::_LABEL_BASIC);
lblSave->Caption = LoadResourceString(&Msgunit3::_LABEL_SAVE);
lblDelete->Caption = LoadResourceString(&Msgunit3::_LABEL_DELETE);
lblMovement->Caption = LoadResourceString(&Msgunit3::_LABEL_MOVEMENT);
lblSystem->Caption = LoadResourceString(&Msgunit3::_LABEL_SYSTEM);
lblAdvanced->Caption = LoadResourceString(&Msgunit3::_LABEL_ADVANCED);

//-----------------------------------
//【基本設定】タブ
//-----------------------------------
PanelBasicCaption->Caption = LoadResourceString(&Msgunit3::_PANEL_BASIC_CAPTION);
chkMyEncPasswordKeep->Caption = LoadResourceString(&Msgunit3::_BASIC_PANEL_CHECKBOX_MY_ENC_PASSWORD_KEEP);
chkMyDecPasswordKeep->Caption = LoadResourceString(&Msgunit3::_BASIC_PANEL_CHECKBOX_MY_DEC_PASSWORD_KEEP);
chkMyPasswordExe->Caption = LoadResourceString(&Msgunit3::_BASIC_PANEL_CHECKBOX_MY_PASSWORD_EXE);
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
//設定値をフォームに反映する
ShowOptionData();


//-----------------------------------
//アクティブタブ（これはオプションクラスとは別読み）
if ( FileExists(Form1->opthdl->OptionPath) == true ){
	//通常なら読み込んだ先へ保存する
	pOpt = new TRegIniFile(Form1->opthdl->OptionPath);
}
else{
	//なければレジストリへ
	pOpt = new TRegIniFile("Software\\Hibara\\AttacheCase");
}

int ActiveTabNum = pOpt->ReadInteger( "WindowPos", "ActiveTabNum", 0);
delete pOpt;

if (ActiveTabNum > -1 &&  ActiveTabNum < 7) {
	PageControl1->ActivePageIndex = ActiveTabNum;
}
else{
	PageControl1->ActivePageIndex = 0;
}

//-----------------------------------
switch(PageControl1->ActivePageIndex){
case 1:
	imgFocusLight->BoundsRect = imgSave->BoundsRect;
	break;
case 2:
	imgFocusLight->BoundsRect = imgDelete->BoundsRect;
	break;
case 3:
	imgFocusLight->BoundsRect = imgMovement->BoundsRect;
	break;
case 4:
	imgFocusLight->BoundsRect = imgSystem->BoundsRect;
	break;
case 5:
	imgFocusLight->BoundsRect = imgAdvanced->BoundsRect;
	break;
default:
	imgFocusLight->BoundsRect = imgBasic->BoundsRect;
	break;
}

//-----------------------------------
//表示反映
PanelMenuRefresh();


}
//---------------------------------------------------------------------------
void __fastcall TForm3::FormCreate(TObject *Sender)
{

//

}
//---------------------------------------------------------------------------
void __fastcall TForm3::FormDestroy(TObject *Sender)
{

TRegIniFile *pOpt;
//アクティブタブ（これはオプションクラスとは別保存）
if ( FileExists(Form1->opthdl->OptionPath) == true ){
	//通常なら読み込んだ先へ保存する
	pOpt = new TRegIniFile(Form1->opthdl->OptionPath);
}
else{
	//なければレジストリへ
	pOpt = new TRegIniFile("Software\\Hibara\\AttacheCase");
}
pOpt->WriteInteger( "WindowPos", "ActiveTabNum", PageControl1->ActivePageIndex);
delete pOpt;

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
if (Form1->opthdl->OptType == 1) {
	Form3->Icon = imgIni->Picture->Icon;
	Form3->Caption = "INIファイル：" + Form1->opthdl->OptionPath;
}
// コマンドライン引数
else if (Form1->opthdl->OptType == 1) {
	Form3->Icon = imgBat->Picture->Icon;
	Form3->Caption = "コマンドライン：" + Form1->opthdl->OptionPath;
}
// レジストリ
else{
	Form3->Icon = imgReg->Picture->Icon;
	Form3->Caption = "レジストリ：" + Form1->opthdl->OptionPath;
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

//設定値を動作設定フォームに反映表示する
SaveOptionData();

Close();

}
//---------------------------------------------------------------------------
void __fastcall TForm3::imgBasicMouseEnter(TObject *Sender)
{
imgFocusLight->BoundsRect = imgBasic->BoundsRect;
}
//---------------------------------------------------------------------------
void __fastcall TForm3::imgSaveMouseEnter(TObject *Sender)
{
imgFocusLight->BoundsRect = imgSave->BoundsRect;
}
//---------------------------------------------------------------------------
void __fastcall TForm3::imgDeleteMouseEnter(TObject *Sender)
{
imgFocusLight->BoundsRect = imgDelete->BoundsRect;
}
//---------------------------------------------------------------------------
void __fastcall TForm3::imgMovementMouseEnter(TObject *Sender)
{
imgFocusLight->BoundsRect = imgMovement->BoundsRect;
}
//---------------------------------------------------------------------------
void __fastcall TForm3::imgSystemMouseEnter(TObject *Sender)
{
imgFocusLight->BoundsRect = imgSystem->BoundsRect;
}
//---------------------------------------------------------------------------
void __fastcall TForm3::imgAdvancedMouseEnter(TObject *Sender)
{
imgFocusLight->BoundsRect = imgAdvanced->BoundsRect;
}
//---------------------------------------------------------------------------
void __fastcall TForm3::imgBasicClick(TObject *Sender)
{
PageControl1->ActivePageIndex = 0;
PanelMenuRefresh();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::imgSaveClick(TObject *Sender)
{
PageControl1->ActivePageIndex = 1;
PanelMenuRefresh();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::imgDeleteClick(TObject *Sender)
{
PageControl1->ActivePageIndex = 2;
PanelMenuRefresh();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::imgMovementClick(TObject *Sender)
{
PageControl1->ActivePageIndex = 3;
PanelMenuRefresh();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::imgSystemClick(TObject *Sender)
{
PageControl1->ActivePageIndex = 4;
PanelMenuRefresh();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::imgAdvancedClick(TObject *Sender)
{
PageControl1->ActivePageIndex = 5;
PanelMenuRefresh();
}
//---------------------------------------------------------------------------
//パネルメニュー表示のリフレッシュを行う
void __fastcall TForm3::PanelMenuRefresh(void)
{

switch(PageControl1->ActivePageIndex){
case 1:
	imgSelectCursor->Top = imgSave->Top;
	imgSelectCursor->Left = PanelMenu->Width - imgSelectCursor->Width - 4;
	break;
case 2:
	imgSelectCursor->Top = imgDelete->Top;
	imgSelectCursor->Left = PanelMenu->Width - imgSelectCursor->Width - 4;
	break;
case 3:
	imgSelectCursor->Top = imgMovement->Top;
	imgSelectCursor->Left = PanelMenu->Width - imgSelectCursor->Width - 4;
	break;
case 4:
	imgSelectCursor->Top = imgSystem->Top;
	imgSelectCursor->Left = PanelMenu->Width - imgSelectCursor->Width - 4;
	break;
case 5:
	imgSelectCursor->Top = imgAdvanced->Top;
	imgSelectCursor->Left = PanelMenu->Width - imgSelectCursor->Width - 4;
	break;
default:	// 0
	imgSelectCursor->Top = imgBasic->Top;
	imgSelectCursor->Left = PanelMenu->Width - imgSelectCursor->Width - 4;
	break;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::PanelMenuDblClick(TObject *Sender)
{

TStringList *KeyValueList = new TStringList();
Form1->opthdl->GetKeyValueData(KeyValueList);

//設定値をまとめてデバッグ表示する
for (int i = 0; i < KeyValueList->Count; i++) {
	ValueListEditor1->InsertRow(KeyValueList->Names[i], KeyValueList->ValueFromIndex[i], true);
}

delete KeyValueList;

//設定値のデバッグ一覧表示
PageControl1->ActivePageIndex = 6;

}
//===========================================================================
// 設定値を動作設定フォームに反映表示する
//===========================================================================
void __fastcall TForm3::ShowOptionData(void)
{

//-----------------------------------
//【基本設定】タブ
//-----------------------------------
//文字数分だけ"*"を表示する
int pNum;
chkMyEncPasswordKeep->Checked = Form1->opthdl->fMyEncodePasswordKeep;
if ( (pNum = Form1->opthdl->MyEncodePassword.Length()) > 32 ){
	pNum = 32;
}
btneditMyEncPassword->Text = String::StringOfChar('*', pNum);

chkMyDecPasswordKeep->Checked = Form1->opthdl->fMyDecodePasswordKeep;
if ( (pNum = Form1->opthdl->MyDecodePassword.Length()) > 32 ){
	pNum = 32;
}
btneditMyDecPassword->Text = String::StringOfChar('*', pNum);

chkMyPasswordExe->Checked = Form1->opthdl->fMemPasswordExe;
chkOpenFolder->Checked = Form1->opthdl->fOpenFolder;
chkOpenFile->Checked = Form1->opthdl->fOpenFile;
chkEndToExit->Checked = Form1->opthdl->fEndToExit;
chkWindowForeground->Checked = Form1->opthdl->fWindowForeground;
chkNoHidePassword->Checked = Form1->opthdl->fNoHidePassword;
chkSaveToExeout->Checked = Form1->opthdl->fSaveToExeout;
chkShowExeoutChkBox->Checked = Form1->opthdl->fShowExeoutChkBox;
chkAskEncDecode->Checked = Form1->opthdl->fAskEncDecode;
chkNoMultipleInstance->Checked = Form1->opthdl->fNoMultipleInstance;

//-----------------------------------
//【保存設定】タブ
//-----------------------------------
chkSaveToSameFldr->Checked = Form1->opthdl->fSaveToSameFldr;
btneditSaveToSameFldrPath->Text = Form1->opthdl->SaveToSameFldrPath;
if (chkSaveToSameFldr->Checked == true) {
	btneditSaveToSameFldrPath->Enabled = true;
	btneditSaveToSameFldrPath->Color = clWindow;
}
else{
	btneditSaveToSameFldrPath->Enabled = false;
	btneditSaveToSameFldrPath->Color = clBtnFace;
}

chkDecodeToSameFldr->Checked = Form1->opthdl->fDecodeToSameFldr;
btneditDecodeToSameFldrPath->Text = Form1->opthdl->DecodeToSameFldrPath;
if (chkDecodeToSameFldr->Checked == true) {
	btneditDecodeToSameFldrPath->Enabled = true;
	btneditDecodeToSameFldrPath->Color = clWindow;
}
else{
	btneditDecodeToSameFldrPath->Enabled = false;
	btneditDecodeToSameFldrPath->Color = clBtnFace;
}

chkConfirmOverwirte->Checked = Form1->opthdl->fConfirmOverwirte;

//複数ファイルがあるときの処理
if ( Form1->opthdl->fAllFilePack == true ) {
	//複数のファイルを暗号化する際は一つにまとめる
	radiogrpMultipleFiles->ItemIndex = 1;
}
else if ( Form1->opthdl->fFilesOneByOne == true ) {
	//フォルダ内のファイルは個別に暗号化/復号する
	radiogrpMultipleFiles->ItemIndex = 2;
}
else{
	//何もしない
	radiogrpMultipleFiles->ItemIndex = 0;
}

chkNoParentFldr->Checked = Form1->opthdl->fNoParentFldr;
chkKeepTimeStamp->Checked = Form1->opthdl->fKeepTimeStamp;
chkSameTimeStamp->Checked = Form1->opthdl->fSameTimeStamp;
chkExtInAtcFileName->Checked = Form1->opthdl->fExtInAtcFileName;
chkAutoName->Checked = Form1->opthdl->fAutoName;
btneditAutoNameFormatText->Text = Form1->opthdl->AutoNameFormatText;


//-----------------------------------
//【削除設定】タブ
//-----------------------------------
chkDelOrgFile->Checked = Form1->opthdl->fDelOrgFile;
chkDelEncFile->Checked = Form1->opthdl->fDelEncFile;
chkShowDeleteChkBox->Checked = Form1->opthdl->fShowDeleteChkBox;

//削除の詳細設定
if (Form1->opthdl->fCompleteDelete == 1) {      //完全削除
	optCompleteDelete->Checked = true;
	imgArrowEnabled->Visible = true;
	imgArrowDiabled->Visible = false;
	UpDownDelRand->Enabled = true;
	UpDownDelZero->Enabled = true;
	txtDelRandNum->Color = clWindow;
	txtDelZeroNum->Color = clWindow;
}
else if (Form1->opthdl->fCompleteDelete == 2) { //ごみ箱へ移動
	optGoToTrash->Checked = true;
	imgArrowEnabled->Visible = false;
	imgArrowDiabled->Visible = true;
	UpDownDelRand->Enabled = false;
	UpDownDelZero->Enabled = false;
	txtDelRandNum->Color = clBtnFace;
	txtDelZeroNum->Color = clBtnFace;
}
else{                                           //通常削除
	optNormalDelete->Checked = true;
	imgArrowEnabled->Visible = false;
	imgArrowDiabled->Visible = true;
	UpDownDelRand->Enabled = false;
	UpDownDelZero->Enabled = false;
	txtDelRandNum->Color = clBtnFace;
	txtDelZeroNum->Color = clBtnFace;
}
//削除回数
UpDownDelRand->Position = Form1->opthdl->DelRandNum;
UpDownDelZero->Position = Form1->opthdl->DelZeroNum;

//「削除の詳細設定」グループボックス内の有効/無効を制御する
DeleteGroupEnabled();

//-----------------------------------
//【動作設定】タブ
//-----------------------------------
//圧縮率設定
if ( Form1->opthdl->CompressRate == 0 ) {
	chkCompress->Checked = false;
}
else{
	chkCompress->Checked = true;
}
TrackBar1->Position = Form1->opthdl->CompressRate;

//コンペア
chkCompareFile->Checked = Form1->opthdl->fCompareFile;

//-----------------------------------
//【システム設定】タブ
//-----------------------------------

//'「送る」フォルダーにアタッシェケースを追加(&S)';
String SendToFilePath = Form1->opthdl->CreateSendToFolderAppFullPath();

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
if ( fArgs == true || Form1->opthdl->OptType == 1){
	cmdChangeTempOptionData->Enabled = true; //一時設定を現在の設定に置き換える
}
else{
	cmdChangeTempOptionData->Enabled = false;
}

//-----------------------------------
//【高度設定】タブ
//-----------------------------------
//パスワードファイル
chkAllowPassFile->Checked = Form1->opthdl->fAllowPassFile;

//-----------------------------------
//暗号時にパスワードファイルを自動チェックする
if ( Form1->opthdl->fCheckPassFile == true &&	chkAllowPassFile->Checked == true ){
	chkCheckPassFile->Checked = true;
	//cmdSelectPassFolder->Glyph = imgFolder->Picture->Bitmap;
	btneditPassFilePath->ReadOnly = false;
	btneditPassFilePath->Color = clWindow;
}
else{
	chkCheckPassFile->Checked = false;
	//cmdSelectPassFolder->Glyph = imgNoFolder->Picture->Bitmap;
	btneditPassFilePath->ReadOnly = true;
	btneditPassFilePath->Color = clBtnFace;
}
btneditPassFilePath->Text = Form1->opthdl->PassFilePath;
btneditPassFilePath->SelStart = btneditPassFilePath->Text.Length(); //パス末尾の方を表示

//-----------------------------------
//復号時にパスワードファイルを自動チェックする
if ( Form1->opthdl->fCheckPassFileDecrypt == true && chkAllowPassFile->Checked == true ){
	chkCheckPassFileDecrypt->Checked = true;
	//cmdSelectPassFolderDecrypt->Glyph = imgFolder->Picture->Bitmap;
	btneditPassFilePathDecrypt->ReadOnly = false;
	btneditPassFilePathDecrypt->Color = clWindow;
}
else{
	chkCheckPassFileDecrypt->Checked = false;
	//cmdSelectPassFolderDecrypt->Glyph = imgNoFolder->Picture->Bitmap;
	btneditPassFilePathDecrypt->ReadOnly = true;
	btneditPassFilePathDecrypt->Color = clBtnFace;
}
btneditPassFilePathDecrypt->Text = Form1->opthdl->PassFilePathDecrypt;
btneditPassFilePathDecrypt->SelStart = btneditPassFilePathDecrypt->Text.Length(); //パス末尾の方を表示

chkNoErrMsgOnPassFile->Checked = Form1->opthdl->fNoErrMsgOnPassFile;

//拡張子偽装
if ( Form1->opthdl->fAddCamoExt == true ){
	chkAddCamoExt->Checked = true;
	txtCamoExt->Enabled = true;
	txtCamoExt->Color = clWindow;
}
else{
	chkAddCamoExt->Checked = false;
	txtCamoExt->Enabled = false;
	txtCamoExt->Color = clBtnFace;
}

txtCamoExt->Text = Form1->opthdl->CamoExt;

//パスワード入力回数制限
cmbMissTypeLimitsNum->ItemIndex = Form1->opthdl->MissTypeLimitsNum-1;
chkBroken->Checked = Form1->opthdl->fBroken;


}
//===========================================================================
// 動作設定フォームから設定値に反映する（レジストリ or INIファイルへの保存も行う）
//===========================================================================
void __fastcall TForm3::SaveOptionData(void)
{

//-----------------------------------
//【基本設定】タブ
//-----------------------------------

//暗号化パスワードを記憶する
Form1->opthdl->fMyEncodePasswordKeep = chkMyEncPasswordKeep->Checked;
if (TempMyEncodePassword != "" && Form1->opthdl->fMyEncodePasswordKeep == true) {
	Form1->opthdl->MyEncodePassword = TempMyEncodePassword;
	TempMyEncodePassword = "";
}

if ( chkMyEncPasswordKeep->Checked == false ){
	Form1->opthdl->MyEncodePassword = "";	//記憶パスワードのクリア
}

//復号パスワードを記憶する
Form1->opthdl->fMyDecodePasswordKeep = chkMyDecPasswordKeep->Checked;
if ( TempMyDecodePassword != "" && Form1->opthdl->fMyDecodePasswordKeep == true ) {
	Form1->opthdl->MyDecodePassword = TempMyDecodePassword;
	TempMyDecodePassword = "";
}

if ( chkMyDecPasswordKeep->Checked == false ){
	Form1->opthdl->MyDecodePassword = "";	//記憶パスワードのクリア
}

Form1->opthdl->fMemPasswordExe = chkMyPasswordExe->Checked;
Form1->opthdl->fOpenFolder = chkOpenFolder->Checked;
Form1->opthdl->fOpenFile = chkOpenFile->Checked;
Form1->opthdl->fEndToExit = chkEndToExit->Checked;
Form1->opthdl->fWindowForeground = chkWindowForeground->Checked;
Form1->opthdl->fNoHidePassword = chkNoHidePassword->Checked;
Form1->opthdl->fSaveToExeout = chkSaveToExeout->Checked;
Form1->opthdl->fShowExeoutChkBox = chkShowExeoutChkBox->Checked;
Form1->opthdl->fAskEncDecode = chkAskEncDecode->Checked;
Form1->opthdl->fNoMultipleInstance = chkNoMultipleInstance->Checked;

//-----------------------------------
//【保存設定】タブ
//-----------------------------------
Form1->opthdl->fSaveToSameFldr = chkSaveToSameFldr->Checked;
Form1->opthdl->SaveToSameFldrPath = btneditSaveToSameFldrPath->Text;
Form1->opthdl->fDecodeToSameFldr = chkDecodeToSameFldr->Checked;
Form1->opthdl->DecodeToSameFldrPath = btneditDecodeToSameFldrPath->Text;

Form1->opthdl->fConfirmOverwirte = chkConfirmOverwirte->Checked;

//複数のファイルを暗号化する際は一つにまとめる
if ( radiogrpMultipleFiles->ItemIndex == 1 ){
	Form1->opthdl->fAllFilePack = true;
	Form1->opthdl->fFilesOneByOne = false;
}
//フォルダ内のファイルは個別に暗号化/復号する
else if ( radiogrpMultipleFiles->ItemIndex == 2 ){
	Form1->opthdl->fAllFilePack = false;
	Form1->opthdl->fFilesOneByOne = true;
}
else{
	//何もしない
	Form1->opthdl->fAllFilePack = false;
	Form1->opthdl->fFilesOneByOne = false;
}

Form1->opthdl->fNoParentFldr = chkNoParentFldr->Checked;
Form1->opthdl->fKeepTimeStamp = chkKeepTimeStamp->Checked;
Form1->opthdl->fSameTimeStamp = chkSameTimeStamp->Checked;
Form1->opthdl->fExtInAtcFileName = chkExtInAtcFileName->Checked;
Form1->opthdl->fAutoName = chkAutoName->Checked;
Form1->opthdl->AutoNameFormatText = btneditAutoNameFormatText->Text;

//-----------------------------------
//【削除設定】タブ
//-----------------------------------
Form1->opthdl->fDelOrgFile = chkDelOrgFile->Checked;
Form1->opthdl->fDelEncFile = chkDelEncFile->Checked;
Form1->opthdl->fShowDeleteChkBox = chkShowDeleteChkBox->Checked;

//削除の詳細設定
if ( optCompleteDelete->Checked == true ) {     //完全削除
	Form1->opthdl->fCompleteDelete = 1;
}
else if ( optGoToTrash->Checked == true ) {     //ごみ箱へ移動
	Form1->opthdl->fCompleteDelete = 2;
}
else{
	Form1->opthdl->fCompleteDelete = 3;           //通常削除
}

//削除回数
Form1->opthdl->DelRandNum = UpDownDelRand->Position;
Form1->opthdl->DelZeroNum = UpDownDelZero->Position;


//-----------------------------------
//【動作設定】タブ
//-----------------------------------
//圧縮率設定
Form1->opthdl->CompressRate = TrackBar1->Position;
//コンペア
Form1->opthdl->fCompareFile = chkCompareFile->Checked;

//-----------------------------------
//【システム設定】タブ
//-----------------------------------
//関連づけファイルアイコン番号
Form1->opthdl->AtcsFileIconIndex = comboDataIcon->ItemIndex + 1;
//ユーザー登録アイコンファイルのパス
Form1->opthdl->UserRegIconFilePath = TempUserRegIconFilePath;

//-----------------------------------
//【高度設定】タブ
//-----------------------------------
//パスワードファイル
Form1->opthdl->fAllowPassFile = chkAllowPassFile->Checked;

//-----------------------------------
//暗号時にパスワードファイルを自動チェックする
Form1->opthdl->fCheckPassFile = chkCheckPassFile->Checked;
Form1->opthdl->PassFilePath = btneditPassFilePath->Text;

//復号時にパスワードファイルを自動チェックする
Form1->opthdl->fCheckPassFileDecrypt = chkCheckPassFileDecrypt->Checked;
Form1->opthdl->PassFilePathDecrypt = btneditPassFilePathDecrypt->Text;

Form1->opthdl->fNoErrMsgOnPassFile = chkNoErrMsgOnPassFile->Checked;

//拡張子偽装
Form1->opthdl->fAddCamoExt = chkAddCamoExt->Checked;
Form1->opthdl->CamoExt = txtCamoExt->Text;

//パスワード入力回数制限
Form1->opthdl->MissTypeLimitsNum = cmbMissTypeLimitsNum->ItemIndex + 1;
Form1->opthdl->fBroken = chkBroken->Checked;

//-----------------------------------
// 設定値を保存
//-----------------------------------
Form1->opthdl->SaveOptionData();


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

TIcon *icon= new TIcon;
TComboExItem *pItem;

ImageList2->Clear();
comboDataIcon->Items->Clear();

//リソースからアイコンファイルを登録
for ( i = 0; i < FILE_ICON_NUM; i++ ){

	IconName = "SUBICON" + IntToStr(i);
	icon->LoadFromResourceName((int)HInstance, IconName);

	if(icon->Handle){
		ImageList2->AddIcon(icon);
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
	FilePath = Form1->opthdl->UserRegIconFilePath;
}

if ( FileExists(FilePath) == true ){
	icon->LoadFromFile(FilePath);
	ImageList2->AddIcon(icon);
	pItem = comboDataIcon->ItemsEx->Add();
	pItem->ImageIndex = ImageList2->Count-1;
	pItem->Caption = LoadResourceString(&Msgunit3::_SYSTEM_PANEL_COMBO_USER_REG_FILE_ICON);
	Form1->opthdl->AtcsFileIconIndex = comboDataIcon->ItemsEx->Count;
}

comboDataIcon->ItemIndex = Form1->opthdl->AtcsFileIconIndex-1;

delete icon;

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
// 暗号化/復号パスワード記憶のテキストボックスにファイルをドラッグ＆ドロップ

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
		L"",
		DirPath, opt) == true){

	btneditSaveToSameFldrPath->Text = DirPath;
	chkSaveToSameFldr->Checked = true;

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
	LoadResourceString(
		&Msgunit3::_DIALOG_MSG_SELECT_DECODE_TO_SAME_DIR_PATH),
		L"",
		DirPath, opt) == true){

	btneditSaveToSameFldrPath->Text = DirPath;
	chkDecodeToSameFldr->Checked = true;

}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::chkAllowPassFileClick(TObject *Sender)
{

//'パスワードファイルにファイルの指定を許可する(&D)'
if ( chkAllowPassFile->Checked == true ) {
	chkCheckPassFile->Enabled = true;
	btneditPassFilePath->ReadOnly = false;
	btneditPassFilePath->Color = clWindow;
	chkCheckPassFileDecrypt->Enabled = true;
	btneditPassFilePathDecrypt->ReadOnly = false;
	btneditPassFilePathDecrypt->Color = clWindow;
}
else{
	chkCheckPassFile->Enabled = false;
	btneditPassFilePath->ReadOnly = true;
	btneditPassFilePath->Color = clBtnFace;
	chkCheckPassFileDecrypt->Enabled = false;
	btneditPassFilePathDecrypt->ReadOnly = true;
	btneditPassFilePathDecrypt->Color = clBtnFace;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::chkCheckPassFileClick(TObject *Sender)
{

//'暗号化時にパスワードファイルを自動チェックする(&H)'
if ( chkCheckPassFile->Checked == true) {
	btneditPassFilePathLeftButtonClick(Sender);
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::chkCheckPassFileDecryptClick(TObject *Sender)
{

//'復号時にパスワードファイルを自動チェックする(&K)';
if ( chkCheckPassFileDecrypt->Checked == true) {
	btneditPassFilePathDecryptLeftButtonClick(Sender);
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
	chkCheckPassFile->Checked = true;
}
else{
	chkCheckPassFile->Checked = false;
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
	chkCheckPassFileDecrypt->Checked = true;
}
else{
	chkCheckPassFileDecrypt->Checked = false;
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
//
}
//---------------------------------------------------------------------------
void __fastcall TForm3::pmnuInsertFileExtClick(TObject *Sender)
{
//
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
	btneditMyEncPassword->ReadOnly = false;
	btneditMyEncPassword->Color = clWindow;
}
else{
	btneditMyEncPassword->ReadOnly = true;
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
String SendToFilePath = Form1->opthdl->CreateSendToFolderAppFullPath();

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
	if ( Form1->opthdl->CreateShortCutFile(SendToFilePath, Application->ExeName, "", "", 0 ) == false ){

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
	btneditAutoNameFormatText->ReadOnly = false;
	btneditAutoNameFormatText->Color = clWindow;
	btneditAutoNameFormatText->RightButton->Enabled = true;
	btneditAutoNameFormatText->SetFocus();
}
else{
	btneditAutoNameFormatText->ReadOnly = true;
	btneditAutoNameFormatText->Color = clBtnFace;
	btneditAutoNameFormatText->RightButton->Enabled = false;
}

}
//---------------------------------------------------------------------------
void __fastcall TForm3::chkDelOrgFileClick(TObject *Sender)
{

//'暗号化した後、元ファイル/フォルダーを削除する(&D)';
DeleteGroupEnabled();

}
//---------------------------------------------------------------------------
void __fastcall TForm3::chkDelEncFileClick(TObject *Sender)
{

//'復号した後、暗号化ファイルを削除する(&E)';
DeleteGroupEnabled();

}
//---------------------------------------------------------------------------
void __fastcall TForm3::chkShowDeleteChkBoxClick(TObject *Sender)
{

//'メインフォームにチェックボックスを表示する(&I)';
DeleteGroupEnabled();

}
//---------------------------------------------------------------------------
//「'削除の詳細設定：'」グループボックス内の有効・無効を制御する
bool __fastcall TForm3::DeleteGroupEnabled(void)
{

if ( chkDelOrgFile->Checked == true || chkDelEncFile->Checked == true || chkDelEncFile->Checked == true ) {

	gbDeleteOption->Enabled = true;
	optNormalDelete->Enabled = true;
	optGoToTrash->Enabled = true;
	optCompleteDelete->Enabled = true;
	txtDelRandNum->Color = clWindow;
	txtDelZeroNum->Color = clWindow;
	imgArrowEnabled->Visible = true;
	imgArrowDiabled->Visible = false;
	return(true);
}
else{
	gbDeleteOption->Enabled = false;
	optNormalDelete->Enabled = false;
	optGoToTrash->Enabled = false;
	optCompleteDelete->Enabled = false;
	txtDelRandNum->Color = clBtnFace;
	txtDelZeroNum->Color = clBtnFace;
	imgArrowEnabled->Visible = false;
	imgArrowDiabled->Visible = true;
	return(false);
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
		btneditMyEncPassword->ReadOnly = false;
		btneditMyEncPassword->Color = clWindow;
	}
	else{
		btneditMyEncPassword->ReadOnly = true;
		btneditMyEncPassword->Color = clBtnFace;
	}

	if ( chkMyDecPasswordKeep->Checked == true ) {
		btneditMyDecPassword->ReadOnly = false;
		btneditMyDecPassword->Color = clWindow;
	}
	else{
		btneditMyDecPassword->ReadOnly = true;
		btneditMyDecPassword->Color = clBtnFace;
	}

}
//-----------------------------------
// 【保存設定】
//-----------------------------------
else if ( PageControl1->ActivePage == TabSave ){

	if ( chkSaveToSameFldr->Checked == true ) {
		btneditSaveToSameFldrPath->ReadOnly = false;
		btneditSaveToSameFldrPath->Color = clWindow;
	}
	else{
		btneditSaveToSameFldrPath->ReadOnly = true;
		btneditSaveToSameFldrPath->Color = clBtnFace;
	}

	if ( chkDecodeToSameFldr->Checked == true ) {
		btneditDecodeToSameFldrPath->ReadOnly = false;
		btneditDecodeToSameFldrPath->Color = clWindow;
	}
	else{
		btneditDecodeToSameFldrPath->ReadOnly = true;
		btneditDecodeToSameFldrPath->Color = clBtnFace;
	}

}
//-----------------------------------
// 【高度な設定】
//-----------------------------------
else if ( PageControl1->ActivePage == TabAdvanced ){

	//暗号時にパスワードファイルを自動チェックする
	if ( chkCheckPassFile->Checked == true ){
		//cmdSelectPassFolder->Glyph = imgFolder->Picture->Bitmap;
		btneditPassFilePath->ReadOnly = false;
		btneditPassFilePath->Color = clWindow;
	}
	else{
		//cmdSelectPassFolder->Glyph = imgNoFolder->Picture->Bitmap;
		btneditPassFilePath->ReadOnly = true;
		btneditPassFilePath->Color = clBtnFace;
	}

	//復号時にパスワードファイルを自動チェックする
	if ( chkCheckPassFileDecrypt->Checked == true ){
		//cmdSelectPassFolderDecrypt->Glyph = imgFolder->Picture->Bitmap;
		btneditPassFilePathDecrypt->ReadOnly = false;
		btneditPassFilePathDecrypt->Color = clWindow;
	}
	else{
		//cmdSelectPassFolderDecrypt->Glyph = imgNoFolder->Picture->Bitmap;
		btneditPassFilePathDecrypt->ReadOnly = true;
		btneditPassFilePathDecrypt->Color = clBtnFace;
	}

}


}
//---------------------------------------------------------------------------
// DragEnd イベントのコールバック関数
//---------------------------------------------------------------------------
void __fastcall TForm3::FilesDragEnd(POINTL ptl, String FileListText)
{

TStringList *FileList = new TStringList;
FileList->Text = FileListText;

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
		dialog = CreateMessageDialog(MsgText, mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo, mbNo);
		dialog->Caption = LoadResourceString(&Msgunit3::_MSG_CAPTION_CONFIRMATION);	//'確認'

		if (dialog->ShowModal() == ID_NO){
			btneditMyEncPassword->Color = tmpColor;        //元の色に戻す
			delete dialog;
			return;
		}
		else{
			delete dialog;
		}

		//SHA-1ハッシュを求める
		if ( Form1->opthdl->GetSHA1HashFromFile(
			FilePath, (unsigned char*)NULL, SHA1HashPassword, (AnsiString)NULL) == true ){
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
		dialog = CreateMessageDialog(MsgText, mtConfirmation, TMsgDlgButtons()<<mbYes<<mbNo, mbNo);
		dialog->Caption = LoadResourceString(&Msgunit3::_MSG_CAPTION_CONFIRMATION);	//'確認'


		if (dialog->ShowModal() == ID_NO){
			btneditMyDecPassword->Color = tmpColor;        //元の色に戻す
			delete dialog;
			return;
		}
		else{
			delete dialog;
		}

		//SHA-1ハッシュを求める
		if ( Form1->opthdl->GetSHA1HashFromFile(
			FilePath, (unsigned char*)NULL, SHA1HashPassword, (AnsiString)NULL) == true ){
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

