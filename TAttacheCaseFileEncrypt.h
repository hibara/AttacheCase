//---------------------------------------------------------------------------
#ifndef TAttacheCaseFileEncryptH
#define TAttacheCaseFileEncryptH
//---------------------------------------------------------------------------
#include <Classes.hpp>

#include "MsgEncrypt.hpp"

#include "rijndael.h"
#include "zlib.h"
#include "isaac.h"

//バッファサイズ
#define BUF_SIZE 32
#define LARGE_BUF_SIZE 1024
#define MARGIN_BUF_SIZE 256
#define FOOTER_BUF_SIZE 16
#define PASSWORD_BUF_SIZE 264 //MyPasscode + "_atc"

//-----------------------------------
//データバージョン（ここをいじると
// 下位バージョンで復号できないエラーが発生する）
#define ATC_DATA_FILE_VERSION 105
//-----------------------------------

//データサブバージョン
#define ATC_DATA_SUB_VERSION 6     //ver.2.70～

//制限サイズ4GB (4294967296-230=4294967066)
#define SIZE_4GB 4294967066

//アルゴリズムタイプ
#define TYPE_ALGORISM_BLOWFISH 0  // Blowfish
#define TYPE_ALGORISM_RIJNDAEL 1  // Rijndael


//---------------------------------------------------------------------------
class TAttacheCaseFileEncrypt : public TThread
{


private:

	//-----------------------------------
	// 変数
	//-----------------------------------

	//パスワードキー
	char key[32];

	//出力する暗号化ファイルのタイムスタンプを元ファイルに合わせるために格納する構造体
	_WIN32_FIND_DATAW first_fd;

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
	String MsgReturnPath;

	//同名ファイルはすべて上書きして暗号化する
	//（ユーザーがダイアログで「すべてはい」を選択したとき = true ）
	bool fOverwirteYesToAll;


	//-----------------------------------
	// 関数
	//-----------------------------------

	//ヘッダ情報を生成する
	bool __fastcall CreateHeaderData
		(TMemoryStream *pms,
		 TStringList *FileList, TStringList *FilePathList,
		 __int64 &AllTotalFileSize);
	//暗号化するファイルリストとファイル情報のリストを生成する
	__int64 __fastcall GetFileInfoList
		( int &Index, String DirPath, String FileName, String BasePath,
			TStringList *FileList, TStringList *DataList);
	// FILETIME構造体をTTimeStampに変換して文字列として取得する
	String __fastcall TimeStampToString(FILETIME ft);
	//ディスクの空き容量を調べる
	__int64 __fastcall GetDiskFreeSpaceNum(String FilePath);
	//初期化ベクトル（IV）の生成
	void fillrand(char *buf, const int len);
	//メインフォームに確認メッセージを投げて処理を中断する
	void __fastcall PostConfirmMessageForm();
	//メインフォームに上書きの確認メッセージを投げて処理を中断する
	void __fastcall PostConfirmOverwriteMessageForm();



protected:
	void __fastcall Execute();



public:
	__fastcall TAttacheCaseFileEncrypt(bool CreateSuspended);
	__fastcall ~TAttacheCaseFileEncrypt();


	//-----------------------------------
	// 変数
	//-----------------------------------

	int StatusNum;                     //ステータスメッセージ番号（エラー内容も含む）

	String MsgErrorString;             //エラーメッセージ

	int ProgressPercentNum;            //進捗パーセント
	String ProgressStatusText;         //進捗ステータス
	String ProgressMsgText;            //進捗メッセージ

	int CompressRateNum;               //圧縮率
	bool fOver4gbOk;                   //4GB超を許可
	bool fKeepTimeStamp;               //暗号化ファイルのタイムスタンプを元ファイルに合わせる
	int fAllFilesPackOption;           //すべてのファイルを１つにまとめる
	bool fExeOutputOption;             //実行形式出力
	bool fOptBrokenFileOption;         //ミスタイプでファイルを破壊するか否か
	int intOptMissTypeLimitsNumOption; //タイプミスできる回数
	bool fConfirmOverwirte;            //同名ファイルがあるときは上書きの確認をする

	String AppExeFilePath;	           //アタッシェケース本体の場所
	String OutFilePath;                //出力する暗号化ファイル
	TStringList *InputFileList;        //暗号化する元ファイルリスト

	//-----------------------------------
	// 関数
	//-----------------------------------

	//パスワード文字列をセットする
	//void __fastcall SetPasswordString(AnsiString Password);
	//パスワードにバイナリ値をセットする
	void __fastcall SetPasswordBinary(char *password);
  //現在設定されているパスワードを取得する
	void __fastcall GetPasswordBinary(char *password);



};
//---------------------------------------------------------------------------
#endif /* TAttacheCaseFileEncryptH */

