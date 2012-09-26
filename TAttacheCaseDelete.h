//---------------------------------------------------------------------------
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

	// ディレクトリ/ファイルを再帰的に削除する
	int __fastcall DeleteDirAndFiles(String DirPath, int &FileCount, int TotalFileCount,
			__int64 &CountFileSize, __int64 TotalFileSize);
	//ファイルの完全削除
	int __fastcall CompleteDeleteFile(String FilePath, __int64 &CountFileSize, __int64 TotalFileSize);
	//ごみ箱への移動
	bool __fastcall GoToTrash(String Path);
	// 削除するファイルリスト情報（ファイル数、合計サイズ）を収集する
	int __fastcall GetDeleteFileListInfo
		(String DirPath, int &TotalFileCount, __int64 &TotalFileSize);
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

	TStringList *FileList;
	int Opt;                           // 0:通常削除, 1:完全削除, 2:ゴミ箱へ
	int RandClearNum;                  // 完全削除時の乱数書き込み回数
	int ZeroClearNum;                  // 完全削除時のNULL書き込み回数

	int ProgressPercentNum;            // 進捗パーセント
	String ProgressStatusText;         // 進捗ステータス
	String ProgressMsgText;            // 進捗メッセージ

};
//---------------------------------------------------------------------------
#endif	/* TAttacheCaseDeleteH */

