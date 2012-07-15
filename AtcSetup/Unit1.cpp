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

int opt;
String MsgText;

//引数が指定されてきた場合のみ起動～
if ( ParamCount() > 0 ){

	TRegIniFile *pReg = new TRegIniFile("Software\\Hibara");

	//アタッシェケース本体のパス
	AtcExeFilePath = pReg->ReadString( "AttacheCase\\AppInfo", "AppPath", "");

	if ( AtcExeFilePath == "" ){
		AtcExeFilePath = ExtractFileDir(ExpandUNCFileName(Application->ExeName))+"\\AtchCase.exe";
	}

	if ( FileExists(AtcExeFilePath) == false ){
		delete pReg;
		exit(0);
	}

	//ファイルアイコン番号
	AtcsFileIconIndex = pReg->ReadInteger( "AttacheCase\\Option", "AtcsFileIconIndex", 2);
	//ユーザー指定のファイルアイコンパス
	UserRegIconFilePath = pReg->ReadString( "AttacheCase\\Option", "UserRegIconFilePath", "");

	pReg->CloseKey();
	delete pReg;

	//-----------------------------------

	opt = StrToIntDef(ParamStr(1), 0);

	if ( opt == 0 ){
		//関連付け設定
		RegistDataFileAssociation();
	}
	else{
		//関連付け解除
		DeleteDataFileAssociation();
	}

}
//引数無し起動の場合はメッセージで通知
else{
	//'このプログラムは単体では動作しません。'+#13+
	//'アタッシェケースからファイルの関連付け設定で呼び出されるときのみ使用されます。';
	MsgText = LoadResourceString(&Msgmain::_MSG_ERROR_THIS_APP_DOES_NOT_EXECUTE_ALONE);
	MessageDlg(MsgText, mtWarning   , TMsgDlgButtons() << mbOK, 0);
}

//Application->Terminate();
exit(0);


}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
//データファイルをシステムに関連付け
//---------------------------------------------------------------------------
bool __fastcall TForm1::RegistDataFileAssociation(void)
{

TRegIniFile *Reg = new TRegIniFile("");

//登録内容
String RegData = "\"" + AtcExeFilePath + "\" \"%1\"";
String RegIconData;

//-----------------------------------
//ルートキー指定
Reg->RootKey = HKEY_CLASSES_ROOT;
Reg->Access = KEY_ALL_ACCESS;

//-----------------------------------
//コンテキストメニュー登録

//一回開いてみて、レジストリが開けないようならエラー
//（※Win2000/XPなどで制限ユーザーである可能性）
if ( Reg->OpenKey( "AttacheCase.DataFile\\Shell", true)){

	//openコマンド
	//古いキーがあるなら削除（ver.2.21～）
	if ( Reg->KeyExists( "open"))  Reg->EraseSection( "open");

	Reg->OpenKey("open\\command", true);
	Reg->WriteExpandString( "", RegData);
	Reg->CloseKey();

	//decodeコマンド
	Reg->RootKey = HKEY_CLASSES_ROOT;
	Reg->Access = KEY_ALL_ACCESS;
	Reg->OpenKey( "AttacheCase.DataFile\\Shell", true);
	Reg->WriteExpandString("", "");  // Shell直下をクリア（前のバージョンで残る場合が有り）

	if ( !Reg->KeyExists( "decode")){

		Reg->OpenKey("decode", true);
		//'アタッシェケースファイルを復号する'
		Reg->WriteExpandString( "", LoadResourceString(&Msgmain::_SYSTEM_CONTEXT_MENU_DECYPTION));

		if ( !Reg->KeyExists( "command")){
			Reg->OpenKey("command", true);
			Reg->WriteExpandString( "", RegData);
		}

	}
	else{

		//レジストリの登録がちがう
		if (Reg->ReadString("decode\\command","","") != RegData ){
			Reg->OpenKey("decode\\command", true);
			Reg->WriteExpandString( "", RegData);
		}

	}

	Reg->CloseKey();

}
else{

	//レジストリが読み出せない？
	delete Reg;
	return(false);

}

//-----------------------------------
//関連付けアイコンの設定
if ( FileExists(UserRegIconFilePath)){  //ユーザー指定
	RegIconData = "\""+UserRegIconFilePath+"\"";
}
else{ //既存アイコン
	RegIconData = "\""+AtcExeFilePath+"\","+IntToStr(AtcsFileIconIndex);
}

if ( Reg->OpenKey("AttacheCase.DataFile\\DefaultIcon", true) == true ){
	Reg->WriteExpandString( "", RegIconData);
	Reg->CloseKey();
}
else{
	//レジストリが読み出せない？
	delete Reg;
	return(false);
}

//-----------------------------------
// .atc拡張子の関連付け

if ( Reg->OpenKey(".atc", true)){
	Reg->WriteExpandString( "", "AttacheCase.DataFile");
	Reg->CloseKey();
}
else{
	delete Reg;
	return(false);	//失敗
}

delete Reg;

//-----------------------------------
//システムからアイコンの表示更新
SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);


return(true);


}//end RegistDataFileAssociation;
//---------------------------------------------------------------------------
//データファイルの関連付け設定の削除処理
//---------------------------------------------------------------------------
bool __fastcall TForm1::DeleteDataFileAssociation(void)
{

TRegIniFile *Reg = new TRegIniFile("");

//登録内容
String RegData = "\"" + AtcExeFilePath + "\" \"%1\"";
//AnsiString RegIconData = "\""+AtcExeFilePath+"\",1";

//-----------------------------------
//ルートキー指定
Reg->RootKey = HKEY_CLASSES_ROOT;
Reg->Access = KEY_ALL_ACCESS;

//-----------------------------------
//レジストリの登録があるか？

if (Reg->ReadString("AttacheCase.DataFile\\Shell\\open\\Command","","") != RegData ){

	//delete Reg;
	//return(false);

}
else{

	//一回開いてみて、レジストリが開けないようならエラー
	//※Win2000/XPなどで制限ユーザーである可能性
	if ( !Reg->OpenKey("AttacheCase.DataFile\\Shell\\open\\Command", true)){
		delete Reg;
		return(false);
	}

}

delete Reg;

//-----------------------------------
//レジストリ削除

TRegIniFile *DelReg = new TRegIniFile("");

//ルートキー指定
DelReg->RootKey = HKEY_CLASSES_ROOT;
DelReg->EraseSection(".atc");
DelReg->EraseSection("AttacheCase.DataFile");

delete DelReg;

//-----------------------------------
//アタッシェケース各設定の削除
//※アンインストーラーがきれいにしてくれるのでここではやらない。
/*
DelReg = new TRegIniFile("Software\\Hibara");
//レジストリ削除
DelReg->EraseSection("AttacheCase");

delete DelReg;
*/

SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, 0, 0);

return(true);

}//end DeleteDataFileAssociation;
//---------------------------------------------------------------------------

