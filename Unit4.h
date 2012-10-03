//---------------------------------------------------------------------------

#ifndef Unit4H
#define Unit4H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>

#include "Unit3.h"

#include "MsgUnit4.hpp"

//---------------------------------------------------------------------------
class TForm4 : public TForm
{
__published:	// IDE 管理のコンポーネント
	TPanel *Panel1;
	TButton *cmdCancel;
	TButton *cmdOK;
	TPageControl *PageControl1;
	TTabSheet *TabSheetInputPassword;
	TTabSheet *TabSheetInsertDate;
	TTabSheet *TabSheetInsertNum;
	TLabeledEdit *txtInputMemPassword;
	TLabeledEdit *txtInputMemConfirmPassword;
	TComboBox *ComboDateTime;
	TLabel *lblExampleDateTime;
	TImage *imgDateTime;
	TImage *imgRandomText;
	TImage *imgSerialNum;
	TImage *imgFileNameHead;
	TImage *imgFileNameEnd;
	TImage *imgInsertNum;
	TUpDown *UpDown1;
	TLabel *lblInsertDateTime;
	TLabeledEdit *txtNum;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall cmdCancelClick(TObject *Sender);
	void __fastcall cmdOKClick(TObject *Sender);
	void __fastcall txtInputMemPasswordChange(TObject *Sender);
	void __fastcall txtInputMemPasswordKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall txtInputMemConfirmPasswordKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormResize(TObject *Sender);



private:	// ユーザー宣言

	int InputType;


public:		// ユーザー宣言

	__fastcall TForm4(TComponent* Owner, int InputType);




};
//---------------------------------------------------------------------------
extern PACKAGE TForm4 *Form4;
//---------------------------------------------------------------------------
#endif
