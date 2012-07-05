//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <FileCtrl.hpp>

#include "Unit2.h"
#include "MsgExeOut.hpp"
#include "TAttacheCaseFileDecrypt2.h"
#include "TDragAndDrop.h"
#include "sha1.h"


//SHA-1ハッシュサイズ（160bit）
//#define BUF_SHA1_SIZE 20
#define BUF_SHA1_SIZE 32
//ハッシュ値を計算するときの読み込みバッファ
#define READ_FILE_BUF_SIZE 2048


//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE 管理のコンポーネント
	TEdit *txtInputPassword;
	TButton *cmdExit;
	TLabel *lblAbout;
	TProgressBar *ProgressBar1;
	TTimer *TimerDecrypt;
	TButton *cmdOK;
	TLabel *lblMsgInputPassword;
	TCheckBox *chkSaveToOtherDirectory;
	TImage *imgKeyIcon;
	TLabel *lblProgressPercentNum;
	TLabel *lblMsg;
	TLabel *lblStatus;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall cmdExitClick(TObject *Sender);
	void __fastcall lblAboutMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall lblAboutMouseLeave(TObject *Sender);
	void __fastcall lblAboutClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall DecryptThreadTerminated(TObject *Sender);
	void __fastcall cmdOKClick(TObject *Sender);
	void __fastcall TimerDecryptTimer(TObject *Sender);


private:	// ユーザー宣言

	//タスクバーにプログレスバー表示 （Win7）
	ITaskbarList3* ptl;

	TAttacheCaseFileDecrypt2 *decrypt; // 復号クラスのインスタンス
	unsigned char password_hash[32];   // パスワードハッシュ
	int TypeLimits;                    // ミスタイプ回数 0〜10
	bool fDestroy;                     // 破壊するか否か 0 or 1

	// フォーム状態の切り替え
	void __fastcall ChangeFormStatus(int opt);	// 0:メイン, 1:実行中
	// ファイルを復号する処理
	void __fastcall FileDecrypt(void);
	// ファイルからSHA-1ハッシュ値を取得する
	bool __fastcall GetSHA1HashFromFile(String FilePath, unsigned char *sha1buffer);



public:		// ユーザー宣言
	__fastcall TForm1(TComponent* Owner);

	//OLEドラッグ＆ドロップのコールバック関数
	void __fastcall FilesDragOver(POINTL ptl);
	void __fastcall FilesDragLeave(void);
	void __fastcall FilesDragEnd(POINTL ptl, String FileListText);

	//メッセージダイアログの表示（スレッドオブジェクトから呼ばれる）
	int __fastcall ShowConfirmMassageForm(String MsgText, TMsgDlgType MsgType, TMsgDlgButtons MsgButtons, TMsgDlgBtn MsgDefaultButton);
	//上書き確認メッセージダイアログの表示（復号スレッドオブジェクトから呼ばれる）
	int __fastcall ShowConfirmOverwriteMassageForm(String MsgText, String &Path);


};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
