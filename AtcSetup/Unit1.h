//---------------------------------------------------------------------------

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
	void __fastcall FormCreate(TObject *Sender);


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
