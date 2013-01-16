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
#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <FileCtrl.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <Graphics.hpp>
#include <GIFImg.hpp>
#include <Buttons.hpp>
#include "shlobj.h"

#include "TAttacheCaseFileEncrypt.h"
#include "TAttacheCaseFileDecrypt2.h"
#include "TAttacheCaseDelete.h"
#include "TAttacheCaseOptionHandle.h"

#include "MsgUnit1.hpp"
#include "TDragAndDrop.h"
#include "TMemoryStoredFiles.h"

#include "Unit2.h"
#include "Unit3.h"

#define INI_FILE_NAME "_AtcCase.ini"

#define TYPE_CRYPT_ENCRYPT 1	//暗号化
#define TYPE_CRYPT_DECRYPT 2  //復号
#define TYPE_CRYPT_ERROR -1   //エラー


//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE 管理のコンポーネント

	TStatusBar *StatusBar1;
	TTimer *TimerEncrypt;
	TMainMenu *MainMenu1;
	TMenuItem *mnuFile;
	TMenuItem *mnuHelp;
	TMenuItem *mnuContents;
	TMenuItem *N1;
	TMenuItem *mnuAbout;
	TMenuItem *mnuOption;
	TMenuItem *mnuSetting;
	TMenuItem *mnuEncryptFiles;
	TMenuItem *mnuEncryptDir;
	TMenuItem *mnuDecrypt;
	TMenuItem *N3;
	TMenuItem *mnuExit;
	TOpenDialog *OpenDialogEncrypt;
	TOpenDialog *OpenDialogDecrypt;
	TTimer *TimerDecrypt;
	TBalloonHint *BalloonHint1;
	TTimer *TimerDelete;
	TMenuItem *E1;
	TPageControl *PageControl1;
	TTabSheet *TabSheetMain;
	TTabSheet *TabSheetInputEncPass;
	TTabSheet *TabSheetInputEncPassConfirm;
	TTabSheet *TabSheetInputDecPass;
	TTabSheet *TabSheetExecute;
	TLabel *lblMain;
	TLabeledEdit *txtEncryptPassword;
	TButton *cmdEncryptPasswordOK;
	TButton *cmdEncryptPasswordCancel;
	TLabeledEdit *txtPasswordConfirm;
	TButton *cmdConfirmOK;
	TButton *cmdConfirmCancel;
	TLabeledEdit *txtDecryptPassword;
	TButton *cmdDecryptPasswordOK;
	TButton *cmdDecryptPasswordCancel;
	TLabel *lblStatus;
	TLabel *lblMsg;
	TProgressBar *ProgressBar1;
	TLabel *lblProgressPercentNum;
	TButton *cmdCancel;
	TCheckBox *chkExeFileOut;
	TCheckBox *chkExeFileOutConf;
	TSaveDialog *SaveDialog1;
	TPaintBox *PaintBoxMain;
	TPaintBox *PaintBoxEncrypt;
	TPaintBox *PaintBoxConfirm;
	TPaintBox *PaintBoxDecrypt;
	TImage *imgBackFromInputEncPass;
	TImage *imgBackNormal;
	TImage *imgBackActive;
	TImage *imgBackFromInputEncPassConfirm;
	TImage *imgBackFromInputDecPass;
	TImage *imgBackFromExecute;
	TImage *imgMenuBackground;
	TPaintBox *PaintBoxMenu;
	TImage *imgMenuEncryptOff;
	TImage *imgMenuEncryptOn;
	TImage *imgMenuDecryptOff;
	TImage *imgMenuDecryptOn;
	TImage *imgMenuOptionOff;
	TImage *imgMenuOptionOn;
	TImage *imgMenuHorizontalLine;
	TImage *imgDropFileIn;
	TCheckBox *chkDeleteAtcData;
	TCheckBox *chkDeleteSourceData;
	TCheckBox *chkDeleteSourceDataConf;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall EncryptThreadTerminated(TObject *Sender);
	void __fastcall DecryptThreadTerminated(TObject *Sender);
	void __fastcall DeleteThreadTerminated(TObject *Sender);
	void __fastcall TimerEncryptTimer(TObject *Sender);
	void __fastcall mnuEncryptFilesClick(TObject *Sender);
	void __fastcall mnuEncryptDirClick(TObject *Sender);
	void __fastcall mnuDecryptClick(TObject *Sender);
	void __fastcall mnuExitClick(TObject *Sender);
	void __fastcall mnuSettingClick(TObject *Sender);
	void __fastcall mnuContentsClick(TObject *Sender);
	void __fastcall mnuAboutClick(TObject *Sender);
	void __fastcall TimerDecryptTimer(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall cmdEncryptPasswordOKClick(TObject *Sender);
	void __fastcall cmdEncryptPasswordCancelClick(TObject *Sender);
	void __fastcall cmdConfirmOKClick(TObject *Sender);
	void __fastcall cmdConfirmCancelClick(TObject *Sender);
	void __fastcall txtEncryptPasswordKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall txtPasswordConfirmKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall txtDecryptPasswordKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall cmdCancelClick(TObject *Sender);
	void __fastcall TimerDeleteTimer(TObject *Sender);
	void __fastcall cmdDecryptPasswordOKClick(TObject *Sender);
	void __fastcall mnuFileClick(TObject *Sender);
	void __fastcall imgBackMouseEnter(TObject *Sender);
	void __fastcall imgBackMouseLeave(TObject *Sender);
	void __fastcall PaintBoxMainMouseEnter(TObject *Sender);
	void __fastcall PaintBoxMainMouseLeave(TObject *Sender);
	void __fastcall PageControl1Change(TObject *Sender);
	void __fastcall txtDecryptPasswordChange(TObject *Sender);
	void __fastcall txtEncryptPasswordChange(TObject *Sender);
	void __fastcall txtPasswordConfirmChange(TObject *Sender);
	void __fastcall PaintBoxMenuPaint(TObject *Sender);
	void __fastcall PaintBoxMenuMouseMove(TObject *Sender, TShiftState Shift, int X,
					int Y);
	void __fastcall PaintBoxMenuMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
					int X, int Y);
	void __fastcall FormGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
					bool &Handled);
	void __fastcall PaintBoxMainMouseDown(TObject *Sender, TMouseButton Button,
					TShiftState Shift, int X, int Y);


private:	// ユーザー宣言

	//-----------------------------------
	// 変数
	//-----------------------------------
	TAttacheCaseFileEncrypt *encrypt;
	TAttacheCaseFileDecrypt2 *decrypt;
	TAttacheCaseDelete *cmpdel;

	//サイドメニューグラフィック
	Graphics::TBitmap *bmpSideMenu;
	TPoint ptSideMenu[5];
	String SideBarCaption[5];
	int SideBarCaptionPosX[5];

	bool fEncryptMenu;
	bool fDecryptMenu;
	bool fOptionMenu;

	//タッチイベントのウィンドウリサイズで使うサイズ格納
	TPoint TouchFormStartPos;
	int TouchFormWidth;
	int TouchFormHeight;

	//-----------------------------------
	// 現在の処理内容
	//-----------------------------------
	//  0:処理待ち,
	//  1:暗号化(TYPE_CRYPT_ENCRYPT),
	//  2:復号(TYPE_CRYPT_DECRYPT)
	// -1:エラー（TYPE_CRYPT_ERROR）
	int CryptTypeNum;

	//先に起動しているアタッシェケースからのメッセージ受け取り
	void __fastcall EvWmCOPYDATA(TMessage message);
	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(WM_COPYDATA, TMessage, EvWmCOPYDATA);
	END_MESSAGE_MAP(TForm);

	ITaskbarList3 *tskpbr;           //タスクバーにプログレスバー表示 （Win7）

	TStringList *FileList;           //投げ込まれたファイルリスト
	int FileListPosition;            //ファイルがどこまで処理されたか

  __int64 DecryptAllTotalSize;     //復号されたファイルの合計サイズ

	String RetryAtcFilePath;         //パスワードの再入力
	int RetryNum;

	String PasswordFilePath;         //パスワードファイルパス
	String ConfirmPasswordFilePath;

	//-----------------------------------
	// 関数
	//-----------------------------------

	// ファイル/フォルダの暗号化処理
	void __fastcall FileEncrypt(void);
	// ファイルを復号する処理
	void __fastcall FileDecrypt(void);
	// ファイルをコンペアする処理（とは言っても実際は復号処理）
	void __fastcall FileCompare(void);

	// サイドメニューを描画する
	void __fastcall PaintSideMenu(void);
	// タブシートの選択
	void __fastcall PageControlActiveSheet(TTabSheet *tb);

	// ファイルの判別を行い処理を分けて実行
	void __fastcall DoExecute(TStringList *ExeFileList);
	// 完全削除処理実行
	void __fastcall DoDeleteFile(String DelFileListString, __int64 DelTotalSize);

	// 暗号化ファイルのヘッダ判定（すべてATCファイルなら真を返す）
	bool __fastcall CheckAtcFileHeader(TStringList *AtcFileList);
	// 暗号化ファイルを破壊する
	bool __fastcall DestroyAtcFile(String AtcFilePath);

	// ファイルリストにあるフォルダからすべてをファイルパスに展開する
	void __fastcall ExpandFileList(TStringList *ExpandFileList);
	// ディレクトリ内にあるファイルリストを取得する
	void __fastcall GetFileListInDirectroy(String DirPath, TStringList *ResultList);

	// フォーム内コンポーネントを配置する
	void __fastcall SetFormComponent(TObject *Sender);



public:		// ユーザー宣言
	__fastcall TForm1(TComponent* Owner);


	//動作設定のインスタンス
	TAttacheCaseOptionHandle *opthdl;

	//OLEドラッグ＆ドロップのコールバック関数
	void __fastcall FilesDragOver(POINTL ptl);
	void __fastcall FilesDragLeave(void);
	void __fastcall FilesDragEnd(POINTL ptl, String FileListText);

	//メッセージダイアログの表示（スレッドオブジェクトから呼ばれる）
	int __fastcall ShowConfirmMassageForm
		(String MsgText, TMsgDlgType MsgType, TMsgDlgButtons MsgButtons, TMsgDlgBtn MsgDefaultButton);
	//上書き確認メッセージダイアログの表示（復号スレッドオブジェクトから呼ばれる）
	int __fastcall ShowConfirmOverwriteMassageForm(String MsgText, String &Path);



};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
