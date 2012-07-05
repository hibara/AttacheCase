//======================================================================
//
//	TGetAppInfoStringƒNƒ‰ƒX     Ver.1.00      Copyright(C) 2009 M.Hibara
//
//======================================================================

//----------------------------------------------------------------------
#ifndef TGetAppInfoStringH
#define TGetAppInfoStringH

//----------------------------------------------------------------------
class TGetAppInfoString : public TObject
{
private:

protected:

public:

	LPVOID* pVersionInfo;

	__fastcall TGetAppInfoString();
	__fastcall ~TGetAppInfoString();

	String Comments;
	String InternalName;
	String ProductName;
	String CompanyName;
	String LegalCopyright;
	String ProductVersion;
	String FileDescription;
	String LegalTrademarks;
	String PrivateBuild;
	String FileVersion;
	String OriginalFilename;
	String SpecialBuild;

};


#endif

