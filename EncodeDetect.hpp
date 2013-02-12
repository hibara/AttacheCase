// CodeGear C++Builder
// Copyright (c) 1995, 2009 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Encodedetect.pas' rev: 21.00

#ifndef EncodedetectHPP
#define EncodedetectHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member functions
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <Sysinit.hpp>	// Pascal unit
#include <Sysutils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Encodedetect
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TBOMType { btNone, btUTF8, bt16LE, bt16BE, bt32LE, bt32BE };
#pragma option pop

#pragma option push -b-
enum TIgnoreEncode { ieSJIS, ieEUC, ieUTF8, ieJIS, ieBOM, ieUTF7 };
#pragma option pop

typedef Set<TIgnoreEncode, ieSJIS, ieUTF7>  TIgnoreEncodes;

//-- var, const, procedure ---------------------------------------------------
static const ShortInt ENC_UNKNOWN = -1;
static const ShortInt ENC_SJIS = 0x0;
static const ShortInt ENC_EUC = 0x1;
static const ShortInt ENC_JIS = 0x2;
static const ShortInt ENC_UTF16LE = 0x3;
static const ShortInt ENC_UTF16BE = 0x4;
static const ShortInt ENC_UTF8 = 0x5;
static const ShortInt ENC_UTF8BOM = 0x6;
static const ShortInt ENC_UTF32LE = 0x7;
static const ShortInt ENC_UTF32BE = 0x8;
static const ShortInt ENC_UTF7 = 0x9;
extern PACKAGE TBOMType __fastcall BOMCheck(System::Byte const *B, const int B_Size, TIgnoreEncodes IgnoreEncodes);
extern PACKAGE bool __fastcall IsASCII(const System::Byte B);
extern PACKAGE bool __fastcall IsUTF8(System::Byte const *B, const int B_Size, int &BCUTF8, TIgnoreEncodes IgnoreEncodes);
extern PACKAGE bool __fastcall IsSJIS(System::Byte const *B, const int B_Size, int &BCSJIS, TIgnoreEncodes IgnoreEncodes);
extern PACKAGE bool __fastcall IsEUC(System::Byte const *B, const int B_Size, int &BCEUC, TIgnoreEncodes IgnoreEncodes);
extern PACKAGE bool __fastcall IsJIS(System::Byte const *B, const int B_Size, TIgnoreEncodes IgnoreEncodes);
extern PACKAGE bool __fastcall IsUTF7(System::Byte const *B, const int B_Size, int &BCUTF7, TIgnoreEncodes IgnoreEncodes);
extern PACKAGE int __fastcall CheckEncoding(System::Byte *BS, const int BS_Size, TIgnoreEncodes IgnoreEncodes = TIgnoreEncodes() )/* overload */;
extern PACKAGE int __fastcall CheckEncoding(Classes::TMemoryStream* MS, int CheckSize = 0x400, TIgnoreEncodes IgnoreEncodes = TIgnoreEncodes() )/* overload */;
extern PACKAGE int __fastcall CheckEncoding(Classes::TFileStream* FS, int CheckSize = 0x400, TIgnoreEncodes IgnoreEncodes = TIgnoreEncodes() )/* overload */;

}	/* namespace Encodedetect */
using namespace Encodedetect;
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// EncodedetectHPP
