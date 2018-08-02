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

//バッファサイズ
#define BUF_SIZE 32

//SHA-1ハッシュ計算（160bit＝20byte）
#define BUF_SHA1_SIZE 20
#define BUF_HASH_DATA 32


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
	void __fastcall txtInputPasswordChange(TObject *Sender);


private:	// ユーザー宣言

	//タスクバーにプログレスバー表示 （Win7）
	ITaskbarList3* ptl;

	TAttacheCaseFileDecrypt2 *decrypt; // 復号クラスのインスタンス

  String PasswordFilePath;           // パスワードファイルパス

	int TypeLimits;                    // ミスタイプ回数 0～10
	bool fDestroy;                     // 破壊するか否か 0 or 1

	//パスワードの再入力回数
	int RetryNum;

	// フォーム状態の切り替え
	void __fastcall ChangeFormStatus(int opt);	// 0:メイン, 1:実行中
	// ファイルを復号する処理
	void __fastcall FileDecrypt(void);
	// ファイルからSHA-1ハッシュ値を取得する
	bool __fastcall GetSHA1HashFromFile(
		String FilePath,              // パスワードファイルパス
		AnsiString &HashDataVer2,     // ver.2.*  ～：SHA-1ハッシュ値（20byte + 12byte）
		AnsiString &HashDataVer1 );   // ver.1.*  ～：ヘッダデータ（先頭文字列32文字）
	//暗号化ファイルを破壊する
	bool __fastcall DestroyAtcFile(void);


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
