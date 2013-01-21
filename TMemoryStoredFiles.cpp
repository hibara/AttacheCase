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
#include <vcl.h>
#pragma hdrstop
#include "TMemoryStoredFiles.h"
//----------------------------------------------------------------------
__fastcall TMemoryStoredFiles::TMemoryStoredFiles(int FileCount) : TObject()
{

FilePathList = new TStringList;         // 0: ファイル名
FileSizeList = new __int64[FileCount];  // 1: ファイルサイズ（フォルダは-1）
FileAttrList = new int[FileCount];      // 2: 属性
FileDtChangeList = new int[FileCount];  // 3: 更新日
FileTmChangeList = new int[FileCount];  // 4: 更新時
FileDtCreateList = new int[FileCount];  // 5: 作成日
FileTmCreateList = new int[FileCount];  // 6: 作成時

ms = new TMemoryStream();

}
//----------------------------------------------------------------------
__fastcall TMemoryStoredFiles::~TMemoryStoredFiles(void)
{

delete FilePathList;         // 0: ファイル名
delete [] FileSizeList;      // 1: ファイルサイズ（フォルダは-1）
delete [] FileAttrList;      // 2: 属性
delete [] FileDtChangeList;  // 3: 更新日
delete [] FileTmChangeList;  // 4: 更新時
delete [] FileDtCreateList;  // 5: 作成日
delete [] FileTmCreateList;  // 6: 作成時

delete ms;

}
//----------------------------------------------------------------------

