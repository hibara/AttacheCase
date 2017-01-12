//======================================================================
//
//	TGetAppInfoStringクラス     Ver.1.00      Copyright(C) 2017 M.Hibara
//
//======================================================================

//----------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TGetAppInfoString.h"
//----------------------------------------------------------------------
__fastcall TGetAppInfoString::TGetAppInfoString() : TObject()
{

//初期化
Comments = "";
InternalName = "";
ProductName = "";
CompanyName = "";
LegalCopyright = "";
ProductVersion = "";
FileDescription = "";
LegalTrademarks = "";
PrivateBuild = "";
FileVersion = "";
OriginalFilename = "";
SpecialBuild = "";

LPVOID lpLang = NULL;
LPVOID lpInfoItem = NULL;

WORD* lpTranslate;
DWORD dwHandleTemp = NULL;
UINT ret;
UINT dwLength;

String SubBlock;
String sInfo;
String Result="";

//バージョン情報のサイズを取得する
DWORD dwInfoSize = GetFileVersionInfoSizeW(Application->ExeName.c_str(), &dwHandleTemp);
pVersionInfo = new LPVOID[dwInfoSize];


if(GetFileVersionInfoW(Application->ExeName.c_str(), dwHandleTemp, dwInfoSize, pVersionInfo)){

	//言語IDとキャラクタIDを取得する
	if (VerQueryValue(pVersionInfo, TEXT("\\VarFileInfo\\Translation"), &lpLang, &dwLength)){

		lpTranslate = (WORD*)lpLang;

		//コメント
		SubBlock.sprintf(L"\\StringFileInfo\\%04x%04x\\Comments", *lpTranslate, *(lpTranslate + 1));
		if ( VerQueryValueW(pVersionInfo, SubBlock.w_str(), &lpInfoItem, &dwLength) ){
			Result.sprintf(L"%s", lpInfoItem);
			Comments = Result;
		}

		//内部名
		SubBlock.sprintf(L"\\StringFileInfo\\%04x%04x\\InternalName", *lpTranslate, *(lpTranslate + 1));
		if ( VerQueryValueW(pVersionInfo, SubBlock.w_str(), &lpInfoItem, &dwLength) ){
			Result.sprintf(L"%s", lpInfoItem);
			InternalName = Result;
		}

		//製品名
		SubBlock.sprintf(L"\\StringFileInfo\\%04x%04x\\ProductName", *lpTranslate, *(lpTranslate + 1));
		if ( VerQueryValueW(pVersionInfo, SubBlock.w_str(), &lpInfoItem, &dwLength) ){
			Result.sprintf(L"%s", lpInfoItem);
			ProductName = Result;
		}

		//会社名
		SubBlock.sprintf(L"\\StringFileInfo\\%04x%04x\\CompanyName", *lpTranslate, *(lpTranslate + 1));
		if ( VerQueryValueW(pVersionInfo, SubBlock.w_str(), &lpInfoItem, &dwLength) ){
			Result.sprintf(L"%s", lpInfoItem);
			CompanyName = Result;
		}

		//著作権
		SubBlock.sprintf(L"\\StringFileInfo\\%04x%04x\\LegalCopyright", *lpTranslate, *(lpTranslate + 1));
		if ( VerQueryValueW(pVersionInfo, SubBlock.w_str(), &lpInfoItem, &dwLength) ){
			Result.sprintf(L"%s", lpInfoItem);
			LegalCopyright = Result;
		}

		//製品バージョン
		SubBlock.sprintf(L"\\StringFileInfo\\%04x%04x\\ProductVersion", *lpTranslate, *(lpTranslate + 1));
		if ( VerQueryValueW(pVersionInfo, SubBlock.w_str(), &lpInfoItem, &dwLength) ){
			Result.sprintf(L"%s", lpInfoItem);
			ProductVersion = Result;
		}

		//説明
		SubBlock.sprintf(L"\\StringFileInfo\\%04x%04x\\FileDescription", *lpTranslate, *(lpTranslate + 1));
		if ( VerQueryValueW(pVersionInfo, SubBlock.w_str(), &lpInfoItem, &dwLength) ){
			Result.sprintf(L"%s", lpInfoItem);
			FileDescription = Result;
		}

		//商標
		SubBlock.sprintf(L"\\StringFileInfo\\%04x%04x\\LegalTrademarks", *lpTranslate, *(lpTranslate + 1));
		if ( VerQueryValueW(pVersionInfo, SubBlock.w_str(), &lpInfoItem, &dwLength) ){
			Result.sprintf(L"%s", lpInfoItem);
			LegalTrademarks = Result;
		}

		//PrivateBuild
		SubBlock.sprintf(L"\\StringFileInfo\\%04x%04x\\PrivateBuild", *lpTranslate, *(lpTranslate + 1));
		if ( VerQueryValueW(pVersionInfo, SubBlock.w_str(), &lpInfoItem, &dwLength) ){
			Result.sprintf(L"%s", lpInfoItem);
			PrivateBuild = Result;
		}

		//ファイルバージョン
		SubBlock.sprintf(L"\\StringFileInfo\\%04x%04x\\FileVersion", *lpTranslate, *(lpTranslate + 1));
		if ( VerQueryValueW(pVersionInfo, SubBlock.w_str(), &lpInfoItem, &dwLength) ){
			Result.sprintf(L"%s", lpInfoItem);
			FileVersion = Result;
		}

		//正式ファイル名
		SubBlock.sprintf(L"\\StringFileInfo\\%04x%04x\\OriginalFilename", *lpTranslate, *(lpTranslate + 1));
		if ( VerQueryValueW(pVersionInfo, SubBlock.w_str(), &lpInfoItem, &dwLength) ){
			Result.sprintf(L"%s", lpInfoItem);
			OriginalFilename = Result;
		}

		//SpecialBuild
		SubBlock.sprintf(L"\\StringFileInfo\\%04x%04x\\SpecialBuild", *lpTranslate, *(lpTranslate + 1));
		if ( VerQueryValueW(pVersionInfo, SubBlock.w_str(), &lpInfoItem, &dwLength) ){
			Result.sprintf(L"%s", lpInfoItem);
			SpecialBuild = Result;
		}

	}

}

}
//----------------------------------------------------------------------
__fastcall TGetAppInfoString::~TGetAppInfoString()
{

delete[] pVersionInfo;

}
//----------------------------------------------------------------------

