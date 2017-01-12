//===========================================================================
/*

アタッシェケース（AttachéCase）
Copyright (c) 2002-2017, Mitsuhiro Hibara ( http://hibara.org )
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

#include "TDragAndDrop.h"
#include <System.hpp>

//===========================================================================
//コンストラクタ
//===========================================================================
__fastcall TDragAndDrop::TDragAndDrop(TForm *_Form): _RefCount(1), Form(_Form)
{

//複数ファイルの受け入れを許可するか
fMultipleFilesOk = true;

//ドロップイメージタイプ（デフォルト＝コピー）
DropImageType = DROPIMAGE_COPY;


}
//===========================================================================
//デストラクタ
//===========================================================================
__fastcall TDragAndDrop::~TDragAndDrop(void)
{
//
}
//===========================================================================
HRESULT __stdcall TDragAndDrop::QueryInterface(const IID& iid, void** ppv)
{

HRESULT hr;

if(iid == IID_IDropTarget || iid == IID_IUnknown){
	hr = S_OK;
	*ppv = (void*)this;
	AddRef();
}
else{
	hr = E_NOINTERFACE;
	*ppv = 0;
}
return hr;

}
//---------------------------------------------------------------------------
ULONG __stdcall TDragAndDrop::AddRef()
{

InterlockedIncrement(&_RefCount);
return (ULONG)_RefCount;

}
//---------------------------------------------------------------------------
ULONG __stdcall TDragAndDrop::Release()
{

ULONG ret = (ULONG)InterlockedDecrement(&_RefCount);
if(ret == 0){
	delete this;
}
return (ULONG)_RefCount;

}
//---------------------------------------------------------------------------
HRESULT __stdcall TDragAndDrop::
	DragEnter(IDataObject* pDataObject, DWORD grfKeyState, POINTL ptl, DWORD* pdwEffect)
{

_DataObject = pDataObject;


// IDataObject → HDROP の値を取得していく
STGMEDIUM td = { TYMED_HGLOBAL };
FORMATETC fr = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };

HRESULT hr = _DataObject->GetData(&fr, &td);

if ( FAILED(hr) == true ){
	return E_FAIL;
}

HDROP hDrop = static_cast<HDROP>(GlobalLock(td.hGlobal));

if (!hDrop){
	return E_INVALIDARG;
}

//いつものドロップされたファイル数を取得
FileCount = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);

// Windowsを最前面に
SetForegroundWindow(Form->Handle);
SetCapture(Form->Handle);
Form->Cursor = crDefault;


if ( fMultipleFilesOk == true ) {
	*pdwEffect = DROPIMAGE_COPY;
}
else{
	if (FileCount == 1 ) {
		*pdwEffect = DROPIMAGE_NONE;
	}
	else{
		*pdwEffect = DROPIMAGE_COPY;
	}
}

//デバッグ
*pdwEffect = DropImageType;


return DragOver(grfKeyState, ptl, pdwEffect);

}
//---------------------------------------------------------------------------
HRESULT __stdcall TDragAndDrop::DragOver(DWORD grfKeyState, POINTL ptl, DWORD* pdwEffect)
{

if ( fMultipleFilesOk == true ) {
	*pdwEffect = DROPIMAGE_COPY;
}
else{
	if (FileCount == 1 ) {
		*pdwEffect = DROPIMAGE_NONE;
	}
	else{
		*pdwEffect = DROPIMAGE_COPY;
	}
}

//デバッグ
*pdwEffect = DropImageType;

FilesDragOver(ptl);	         //コールバック関数

return S_OK;

}
//---------------------------------------------------------------------------
HRESULT __stdcall TDragAndDrop::DragLeave()
{

Form->Cursor = crDefault;

FilesDragLeave();	//コールバック関数
ReleaseCapture();

_DataObject = NULL;

return S_OK;

}
//---------------------------------------------------------------------------
HRESULT __stdcall TDragAndDrop::Drop
	(IDataObject* pDataObject, DWORD grfKeyState, POINTL ptl, DWORD* pdwEffect)
{

int i;
wchar_t filename[MAX_PATH];

TStringList *FileList = new TStringList;

// IDataObject → HDROP の値を取得していく
STGMEDIUM td = { TYMED_HGLOBAL };
FORMATETC fr = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };

HRESULT hr = pDataObject->GetData(&fr, &td);

if ( FAILED(hr) == true ){
	return E_FAIL;
}

HDROP hDrop = static_cast<HDROP>(GlobalLock(td.hGlobal));

if (!hDrop){
	return E_INVALIDARG;
}

//いつものドロップされたファイル数を取得
int FileCount = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);

for (i = 0; i < FileCount; i++) {
	//ファイル名を取得する
	DragQueryFileW(hDrop, i, filename, MAX_PATH);
	FileList->Add(filename);
}

//コールバック関数
FilesDragEnd(ptl, FileList->Text);

//後始末
GlobalUnlock(td.hGlobal);
ReleaseStgMedium(&td);
Form->Cursor = crDefault;
*pdwEffect = DROPEFFECT_NONE;
ReleaseCapture();
delete FileList;

return S_OK;

}
//---------------------------------------------------------------------------


