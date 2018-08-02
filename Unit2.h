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

#include "zlib-1.2.11\zlib.h"
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
