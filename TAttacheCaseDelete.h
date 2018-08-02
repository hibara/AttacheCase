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
#ifndef TAttacheCaseDeleteH
#define TAttacheCaseDeleteH
//---------------------------------------------------------------------------
#include <Classes.hpp>

#include "MsgDelete.hpp"

#define LARGE_BUF 1024

#include "shlobj.h"
#include "isaac.h"

//---------------------------------------------------------------------------
class TAttacheCaseDelete : public TThread
{

private:

	//メッセージダイアログ
	String MsgText;
	TMsgDlgType MsgType;
	// --
	// mtWarning
	// mtError
	// mtInformation
	// mtConfirmation
	// mtCustom
	TMsgDlgButtons MsgButtons;
	TMsgDlgBtn MsgDefaultButton;
	int MsgReturnVal;

	//ファイルの完全削除
	int __fastcall CompleteDeleteFile(String FilePath, __int64 &CountFileSize, __int64 TotalFileSize);
	//ごみ箱への移動
	bool __fastcall GoToTrash(String Path);

	//ランダム値の生成（CRNG）
	void fillrand(char *buf, const int len);
	//メインフォームに確認メッセージを投げて処理を中断する
	void __fastcall PostConfirmMessageForm();


protected:
	void __fastcall Execute();


public:
	__fastcall TAttacheCaseDelete(bool CreateSuspended);
	__fastcall ~TAttacheCaseDelete();

	int StatusNum;                     //ステータスメッセージ番号（エラー内容も含む）
	String MsgErrorString;             //エラーメッセージ

	TStringList *DeleteFileList;       // 削除処理するファイルリスト
	__int64 TotalFileSize;             // 削除するファイルの合計サイズ
	int Opt;                           // 0:通常削除, 1:完全削除, 2:ゴミ箱へ
	int RandClearNum;                  // 完全削除時の乱数書き込み回数
	int ZeroClearNum;                  // 完全削除時のNULL書き込み回数

	int ProgressPercentNum;            // 進捗パーセント
	String ProgressPercentNumText;     // 進捗パーセント（小数点第一位までの文字列）
	String ProgressStatusText;         // 進捗ステータス
	String ProgressMsgText;            // 進捗メッセージ

};
//---------------------------------------------------------------------------
#endif	/* TAttacheCaseDeleteH */

