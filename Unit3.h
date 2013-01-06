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
#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <ValEdit.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <Buttons.hpp>
#include <FileCtrl.hpp>

#include "Unit4.h"

#include "help.h"
#include "Windowsx.h"
#include "MsgUnit3.hpp"

#include "TDragAndDrop.h"
#include "TAttacheCaseOptionHandle.h"


#define FILE_ICON_NUM 4

//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE 管理のコンポーネント
	TPanel *Panel1;
	TButton *cmdOK;
	TButton *cmdCancel;
	TStatusBar *StatusBar1;
	TImage *imgBat;
	TImage *imgReg;
	TImage *imgIni;
	TPageControl *PageControl1;
	TTabSheet *TabBasic;
	TTabSheet *TabSave;
	TTabSheet *TabDelete;
	TTabSheet *TabMovement;
	TTabSheet *TabSystem;
	TTabSheet *TabAdvanced;
	TPanel *PanelBasicCaption;
	TPanel *PanelSaveCaption;
	TPanel *PanelDeleteCaption;
	TPanel *PanelMovementCaption;
	TPanel *PanelSystemCaption;
	TPanel *PanelAdvancedCaption;
	TCheckBox *chkNoMultipleInstance;
	TCheckBox *chkMyEncPasswordKeep;
	TButtonedEdit *btneditMyEncPassword;
	TCheckBox *chkMyDecPasswordKeep;
	TButtonedEdit *btneditMyDecPassword;
	TCheckBox *chkMyPasswordExe;
	TCheckBox *chkOpenFolder;
	TCheckBox *chkOpenFile;
	TCheckBox *chkEndToExit;
	TCheckBox *chkWindowForeground;
	TCheckBox *chkNoHidePassword;
	TCheckBox *chkSaveToExeout;
	TCheckBox *chkShowExeoutChkBox;
	TCheckBox *chkAskEncDecode;
	TCheckBox *chkSaveToSameFldr;
	TButtonedEdit *btneditSaveToSameFldrPath;
	TImageList *ImageList1;
	TCheckBox *chkDecodeToSameFldr;
	TButtonedEdit *btneditDecodeToSameFldrPath;
	TCheckBox *chkConfirmOverwirte;
	TRadioGroup *radiogrpMultipleFiles;
	TCheckBox *chkNoParentFldr;
	TCheckBox *chkKeepTimeStamp;
	TCheckBox *chkSameTimeStamp;
	TCheckBox *chkExtInAtcFileName;
	TCheckBox *chkAutoName;
	TButtonedEdit *btneditAutoNameFormatText;
	TLabel *lblAutoFormatExample;
	TCheckBox *chkDelOrgFile;
	TCheckBox *chkDelEncFile;
	TCheckBox *chkShowDeleteChkBox;
	TGroupBox *gbDeleteOption;
	TRadioButton *optNormalDelete;
	TRadioButton *optGoToTrash;
	TRadioButton *optCompleteDelete;
	TLabeledEdit *txtDelRandNum;
	TUpDown *UpDownDelRand;
	TImage *imgArrowEnabled;
	TImage *imgArrowDiabled;
	TLabeledEdit *txtDelZeroNum;
	TUpDown *UpDownDelZero;
	TGroupBox *gbCompressRate;
	TCheckBox *chkCompress;
	TTrackBar *TrackBar1;
	TLabel *lblCompressRateDetail;
	TLabel *lblCompressRate;
	TGroupBox *gbCompare;
	TCheckBox *chkCompareFile;
	TGroupBox *gbWindowsSystemOption;
	TButton *cmdAssociateAtcFile;
	TButton *cmdUnAssociateAtcFile;
	TButton *cmdShortCutToSendToFileDir;
	TGroupBox *gbChangeFileIcon;
	TButton *cmdAddFileIcon;
	TGroupBox *gbOutputOptionData;
	TButton *cmdOutputOptionData;
	TButton *cmdChangeTempOptionData;
	TGroupBox *gbPasswordFile;
	TCheckBox *chkAllowPassFile;
	TCheckBox *chkCheckPassFile;
	TButtonedEdit *btneditPassFilePath;
	TCheckBox *chkCheckPassFileDecrypt;
	TButtonedEdit *btneditPassFilePathDecrypt;
	TCheckBox *chkNoErrMsgOnPassFile;
	TGroupBox *gbCamoExt;
	TCheckBox *chkAddCamoExt;
	TEdit *txtCamoExt;
	TLabel *lblCamoExtDetail;
	TGroupBox *gbTypeLimit;
	TComboBox *cmbMissTypeLimitsNum;
	TLabel *lblMissTypeLimitsNum;
	TCheckBox *chkBroken;
	TLabel *lblTypeLimitAlertMsg;
	TLabel *lblAdvancedOptionAlertMsg;
	TTabSheet *TabDebug;
	TValueListEditor *ValueListEditor1;
	TPanel *PanelDebugCaption;
	TComboBoxEx *comboDataIcon;
	TOpenDialog *OpenDialog1;
	TImage *Image1;
	TImage *Image2;
	TPopupMenu *PopupMenu1;
	TImageList *ImageList3;
	TMenuItem *pmnuInsertFileName;
	TMenuItem *pmnuInsertFileExt;
	TMenuItem *pmnuSeparator;
	TMenuItem *pmnuInsertDatetime;
	TMenuItem *pmnuInsertSerialNum;
	TMenuItem *pmnuInsertRandomText;
	TMenuItem *pmnuInsertFileNameHead;
	TMenuItem *pmnuInsertFileNameEnd;
	TMemo *Memo1;
	TBalloonHint *BalloonHint1;
	TButton *cmdHelp;
	TSaveDialog *SaveDialog1;
	TPaintBox *PaintBoxMenu;
	TImageList *imlSideMenuIconOff;
	TImageList *imlSideMenuIconOn;
	TImage *imgMenuBackground;
	TImageList *imlAssociateFilesIcon;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall cmdCancelClick(TObject *Sender);
	void __fastcall cmdOKClick(TObject *Sender);
	void __fastcall TrackBar1Change(TObject *Sender);
	void __fastcall optNormalDeleteClick(TObject *Sender);
	void __fastcall optGoToTrashClick(TObject *Sender);
	void __fastcall optCompleteDeleteClick(TObject *Sender);
	void __fastcall btneditMyEncPasswordLeftButtonClick(TObject *Sender);
	void __fastcall btneditMyDecPasswordLeftButtonClick(TObject *Sender);
	void __fastcall btneditSaveToSameFldrPathLeftButtonClick(TObject *Sender);
	void __fastcall btneditDecodeToSameFldrPathLeftButtonClick(TObject *Sender);
	void __fastcall btneditPassFilePathLeftButtonClick(TObject *Sender);
	void __fastcall btneditPassFilePathDecryptLeftButtonClick(TObject *Sender);
	void __fastcall chkAllowPassFileClick(TObject *Sender);
	void __fastcall chkCheckPassFileClick(TObject *Sender);
	void __fastcall chkCheckPassFileDecryptClick(TObject *Sender);
	void __fastcall chkAddCamoExtClick(TObject *Sender);
	void __fastcall pmnuInsertFileNameClick(TObject *Sender);
	void __fastcall pmnuInsertFileExtClick(TObject *Sender);
	void __fastcall pmnuInsertDatetimeClick(TObject *Sender);
	void __fastcall pmnuInsertSerialNumClick(TObject *Sender);
	void __fastcall pmnuInsertRandomTextClick(TObject *Sender);
	void __fastcall pmnuInsertFileNameHeadClick(TObject *Sender);
	void __fastcall pmnuInsertFileNameEndClick(TObject *Sender);
	void __fastcall chkMyEncPasswordKeepClick(TObject *Sender);
	void __fastcall chkMyDecPasswordKeepClick(TObject *Sender);
	void __fastcall chkSaveToSameFldrClick(TObject *Sender);
	void __fastcall chkDecodeToSameFldrClick(TObject *Sender);
	void __fastcall cmdAssociateAtcFileClick(TObject *Sender);
	void __fastcall cmdUnAssociateAtcFileClick(TObject *Sender);
	void __fastcall cmdShortCutToSendToFileDirClick(TObject *Sender);
	void __fastcall chkAutoNameClick(TObject *Sender);
	void __fastcall chkDelOrgFileClick(TObject *Sender);
	void __fastcall cmdHelpClick(TObject *Sender);
	void __fastcall btneditAutoNameFormatTextChange(TObject *Sender);
	void __fastcall cmdOutputOptionDataClick(TObject *Sender);
	void __fastcall cmdChangeTempOptionDataClick(TObject *Sender);
	void __fastcall PaintBoxMenuMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall PaintBoxMenuDblClick(TObject *Sender);
	void __fastcall PaintBoxMenuPaint(TObject *Sender);
	void __fastcall PaintBoxMenuMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall txtDelRandNumKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall txtDelZeroNumKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall txtDelRandNumChange(TObject *Sender);
	void __fastcall txtDelZeroNumChange(TObject *Sender);
	void __fastcall txtDelRandNumExit(TObject *Sender);
	void __fastcall txtDelZeroNumExit(TObject *Sender);

private:	// ユーザー宣言

	//サイドメニューグラフィック
	Graphics::TBitmap *bmpSideMenu;
	TPoint ptSideMenu[7];
	String SideMenuLabelCaption[7];
	int SideMenuLabelCaptionPosX[7];
	int optSelectedMenu;


	//ユーザー指定のアイコンファイルパス（OKボタンが押されるまでのテンポラリ）
	String TempUserRegIconFilePath;

	//設定値を動作設定フォームに反映表示する
	void __fastcall FormShowOptionData(void);
	//動作設定フォームから設定値に反映する（各メディアへ保存も行う）
	void __fastcall FormSaveOptionData(void);

	//ファイルアイコンをコンボボックスへ登録する
	void __fastcall RegIconFileToComboBox(void);

	//UACエレベーションを要求して関連付けプログラムを実行
	bool __fastcall RunAsAdmin(int Option);

	// サイドメニューを描画する
	void __fastcall PaintSideMenu(void);



public:		// ユーザー宣言
	__fastcall TForm3(TComponent* Owner, TAttacheCaseOptionHandle *opt);

	TAttacheCaseOptionHandle *pOpt;

	//記憶パスワードが変更されたときに格納する
	AnsiString TempMyEncodePassword;
	AnsiString TempMyDecodePassword;

	//OLEドラッグ＆ドロップのコールバック関数
	void __fastcall FilesDragOver(POINTL ptl);
	void __fastcall FilesDragLeave(void);
	void __fastcall FilesDragEnd(POINTL ptl, String FileListText);


};
//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif
