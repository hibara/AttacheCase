#ifndef TDragAndDropH
#define TDragAndDropH


#include <windows.h>
#include <oleidl.h>
#include <vcl.h>

#include "shlobj.h"
//---------------------------------------------------------------------------
class TDragAndDrop : public IDropTarget
{

public:
	//コンストラクタ
	__fastcall TDragAndDrop::TDragAndDrop(TForm *_Form);
	//デストラクタ
	__fastcall ~TDragAndDrop(void);


	//複数ファイルの受け入れを許可するか
	bool fMultipleFilesOk;
	//ドロップイメージタイプ
	DROPIMAGETYPE DropImageType;

	/*
	typedef enum {
		DROPIMAGE_INVALID = -1,
		DROPIMAGE_NONE = 0,
		DROPIMAGE_COPY = DROPEFFECT_COPY,
		DROPIMAGE_MOVE = DROPEFFECT_MOVE,
		DROPIMAGE_LINK = DROPEFFECT_LINK,
		DROPIMAGE_LABEL = 6,
		DROPIMAGE_WARNING = 7
	} DROPIMAGETYPE;
	*/

	//OLEドロップするフォームへのポインタ
	TForm *Form;
	//仮想関数
	virtual HRESULT __stdcall QueryInterface(const IID& iid, void** ppv);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();
	virtual HRESULT __stdcall DragEnter(IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
	virtual HRESULT __stdcall DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
	virtual HRESULT __stdcall DragLeave();
	virtual HRESULT __stdcall Drop(IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect);
	//Formへのコールバック関数（登録するフォーム側にもつくる必要あり）
	void __fastcall (__closure *FilesDragOver)(POINTL ptl);
	void __fastcall (__closure *FilesDragLeave)(void);
	void __fastcall (__closure *FilesDragEnd)(POINTL ptl, String FileListText);

private:
	LONG _RefCount;
	IDataObject *_DataObject;

	int FileCount;


};
//---------------------------------------------------------------------------

#endif


