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


