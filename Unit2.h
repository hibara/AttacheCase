//---------------------------------------------------------------------------
#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>

#include "MsgUnit2.hpp"

#include "zlib.h"
#include "Unit1.h"

#include "TGetAppInfoString.h"


//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE 管理のコンポーネント
	TImage *Image1;
	TLabel *lblAppName;
	TButton *cmdOK;
	TLabel *lblVersion;
	TLabel *lblZlibVersion;
	TBalloonHint *BalloonHint1;
	TLabel *lblLisence;
	TLabel *lblCopyright;
	TLabel *lblHomePage;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall cmdOKClick(TObject *Sender);
	void __fastcall lblLisenceClick(TObject *Sender);
	void __fastcall lblLisenceMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall lblHomePageMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall lblHomePageClick(TObject *Sender);
	void __fastcall lblLisenceMouseEnter(TObject *Sender);
	void __fastcall lblHomePageMouseEnter(TObject *Sender);









private:	// ユーザー宣言

  void __fastcall CMMouseLeave( TMessage &Msg );
  BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER( CM_MOUSELEAVE, TMessage, CMMouseLeave )
	END_MESSAGE_MAP( TForm )

  String VersionString;

public:		// ユーザー宣言
	__fastcall TForm2(TComponent* Owner);





};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
