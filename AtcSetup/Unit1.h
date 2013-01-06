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

#include "MsgMain.hpp"

#include <Registry.hpp>     //レジストリ操作用
#include <shlobj.h>	        //表示アイコンをシステム更新

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE 管理のコンポーネント


private:	// ユーザー宣言

	String AtcExeFilePath;
	String UserRegIconFilePath;
	int AtcsFileIconIndex;


	//データファイルをシステムに関連付け
	bool __fastcall RegistDataFileAssociation(void);
	//データファイルの関連付け設定の削除処理
	bool __fastcall DeleteDataFileAssociation(void);



public:		// ユーザー宣言
	__fastcall TForm1(TComponent* Owner);



};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
