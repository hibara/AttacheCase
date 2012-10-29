//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{

int opt;        // 0: 関連付け設定, 1: 関連付け解除

String MsgText;
TRegistryIniFile *pReg;

//引数が指定されてきた場合のみ起動～
if ( ParamCount() > 0 ){
	opt = StrToIntDef(ParamStr(1), 0);
}
else{
	//'このプログラムは単体では動作しません。'+#13+
	//'アタッシェケースからファイルの関連付け設定で呼び出されるときのみ使用されます。';
	MsgText = LoadResourceString(&Msgmain::_MSG_ERROR_THIS_APP_DOES_NOT_EXECUTE_ALONE);
	MessageDlg(MsgText, mtWarning   , TMsgDlgButtons() << mbOK, 0);
	//Application->Terminate();
	exit(0);
}

try{

	pReg = new TRegistryIniFile ("Software\\Hibara");

	//アタッシェケース本体のパス
	AtcExeFilePath = pReg->ReadString( "AttacheCase\\AppInfo", "AppPath", "");

	if ( AtcExeFilePath == "" ){
		AtcExeFilePath = ExtractFileDir(ExpandUNCFileName(Application->ExeName))+"\\AtchCase.exe";
	}

	if ( FileExists(AtcExeFilePath) == true ){

		//ファイルアイコン番号
		AtcsFileIconIndex = pReg->ReadInteger( "AttacheCase\\Option", "AtcsFileIconIndex", 1);
		//ユーザー指定のファイルアイコンパス
		UserRegIconFilePath = pReg->ReadString( "AttacheCase\\Option", "UserRegIconFilePath", "");

		//-----------------------------------
		if ( opt == 0 ){
			//関連付け設定
			RegistDataFileAssociation();
		}
		else{
			//関連付け解除
			DeleteDataFileAssociation();
		}
		//-----------------------------------

	}

}
__finally{

	delete pReg;

}

//Application->Terminate();
exit(0);


}
//---------------------------------------------------------------------------
//データファイルをシステムに関連付け
//---------------------------------------------------------------------------
bool __fastcall TForm1::RegistDataFileAssociation(void)
{


String MsgText;
TRegistry *pReg;


try{

	pReg = new TRegistry();

	//登録内容
	String RegData = "\"" + AtcExeFilePath + "\" \"%1\"";
	String RegIconData;

	//-----------------------------------
	//ルートキー指定
	pReg->RootKey = HKEY_CLASSES_ROOT;
	pReg->Access = KEY_ALL_ACCESS;

	//-----------------------------------
	//コンテキストメニュー登録
	//-----------------------------------

	//いったんクリア
	if ( pReg->KeyExists("\\AttacheCase.DataFile") == true ){
		pReg->DeleteKey("\\AttacheCase.DataFile");
	}

	//一回開いてみて、レジストリが開けないようならエラー
	//（※Win2000/XPなどで制限ユーザーである可能性）
	if ( pReg->OpenKey("\\AttacheCase.DataFile\\Shell\\open\\command", true) == true ){
		// open コマンド
		pReg->WriteExpandString("", RegData);
		// decode コマンド
		pReg->OpenKey( "\\AttacheCase.DataFile\\Shell\\decode", true);
		//'アタッシェケースファイルを復号する'
		pReg->WriteExpandString("", LoadResourceString(&Msgmain::_SYSTEM_CONTEXT_MENU_DECYPTION));
		pReg->OpenKey( "\\AttacheCase.DataFile\\Shell\\decode\\command", true);
		pReg->WriteExpandString("", RegData);

	}
	else{
		//'理由はわかりませんが、レジストリからの読み込みに失敗しました。'+#13+
		//'制限ユーザーである可能性があります。管理者権限での実行が必要です。';
		MsgText = LoadResourceString(&Msgmain::_MSG_ERROR_DO_NOT_READ_REGISTRY);
		MessageDlg(MsgText, mtWarning, TMsgDlgButtons() << mbOK, 0);
		return(false);

	}

	//-----------------------------------
	//関連付けアイコンの設定
	//-----------------------------------

	if ( FileExists(UserRegIconFilePath)){          //ユーザー指定
		RegIconData = "\""+UserRegIconFilePath+"\"";
	}
	else{                                           //既存アイコン
		RegIconData = "\""+AtcExeFilePath+"\","+IntToStr(AtcsFileIconIndex);
	}

	if ( pReg->OpenKey("\\AttacheCase.DataFile\\DefaultIcon", true) == true ){
		pReg->WriteExpandString("", RegIconData);
	}
	else{
		return(false);
	}

	//-----------------------------------
	// .atc拡張子の関連付け
	//-----------------------------------

	if ( pReg->OpenKey("\\.atc", true)){
		pReg->WriteExpandString( "", "AttacheCase.DataFile");
	}
	else{
		return(false);
	}


	pReg->CloseKey();


	//-----------------------------------
	//システムからアイコンの表示更新
	//-----------------------------------
	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);

}
__finally{

	delete pReg;

}

return(true);


}//end RegistDataFileAssociation;
//---------------------------------------------------------------------------
//データファイルの関連付け設定の削除処理
//---------------------------------------------------------------------------
bool __fastcall TForm1::DeleteDataFileAssociation(void)
{


String MsgText;
TRegistry *pReg;

try{

	pReg = new TRegistry();

	//-----------------------------------
	//ルートキー指定
	//-----------------------------------
	pReg->RootKey = HKEY_CLASSES_ROOT;
	pReg->Access = KEY_ALL_ACCESS;

	//-----------------------------------
	//そもそもレジストリが開けるか
	//-----------------------------------

	if ( pReg->OpenKey("\\AttacheCase.DataFile\\Shell\\open\\command", true) == false ){
		//'理由はわかりませんが、レジストリからの読み込みに失敗しました。'+#13+
		//'制限ユーザーである可能性があります。管理者権限での実行が必要です。';
		MsgText = LoadResourceString(&Msgmain::_MSG_ERROR_DO_NOT_READ_REGISTRY);
		MessageDlg(MsgText, mtWarning, TMsgDlgButtons() << mbOK, 0);
		return(false);
	}

	//-----------------------------------
	//各キーがあれば削除
	//-----------------------------------

	if ( pReg->KeyExists("\\AttacheCase.DataFile") == true ){
		pReg->DeleteKey("\\AttacheCase.DataFile");
	}

	if ( pReg->KeyExists("\\.atc") == true ){
		pReg->DeleteKey("\\.atc");
	}

}
__finally{
	delete pReg;

}

//-----------------------------------
//システムからアイコンの表示更新
//-----------------------------------
SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);

return(true);

}//end DeleteDataFileAssociation;
//---------------------------------------------------------------------------

