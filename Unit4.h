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
