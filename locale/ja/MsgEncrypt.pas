unit MsgEncrypt;

interface

resourcestring

	//メッセージキャプション
	MSG_CAPTION_ERROR                  = 'エラー';
	MSG_CAPTION_CONFIRMATION           = '確認';

	//メッセージ
	MSG_ERROR_FILE_OPEN                = '暗号化するファイルを開けません。他のアプリケーションが使用中の可能性があります。';
	MSG_ERROR_OUT_FILE_OPEN            = '保存する先のファイルが開けません。他のアプリケーションが使用中の可能性があります。';
	MSG_ERROR_OUT_FILE_WRITE           = '保存先に指定された暗号化ファイルに書き込めません。';
	MSG_ERROR_EXEOUT_FAILED            = '実行可能形式出力に失敗しました。暗号化処理を中止します。';

	MSG_ERROR_NO_DISK_FREE_SPACE       = 'ディスクの空き容量が足りません。暗号化ファイルを保存できません。'+#13+
	                                     '暗号化を中止します。';
	MSG_ERROR_OVER_4GB_EXE             = '実行形式ファイルのサイズが4GBを超えてしまう可能性があります！'+#13+
	                                     'Win32アプリケーションとして実行できなくなるかもしれませんがよろしいですか？';
	MSG_ERROR_ZLIB                     = 'zlibライブラリからエラーを返されました。'+#13+'エラー番号：';

	MSG_CONFIRM_OVER_WRITE_SAME_FILE  = '同名のファイルが既に存在しています。上書きして暗号化しますか？';

	//ラベル
	LABEL_STATUS_TITLE_ENCRYPTING      = '暗号化しています...';
	LABEL_STATUS_TITLE_LISTING         = 'ファイルリストの生成';
	LABEL_STATUS_TITLE_ERROR           = 'エラー';
	LABEL_STATUS_TITLE_USER_CANCEL     = 'キャンセル';
	LABEL_STATUS_TITLE_COMPLETE        = '完了';

	LABEL_STATUS_DETAIL_PREPARING      = '暗号化するための準備をしています...';
	LABEL_STATUS_DETAIL_FAILED         = '暗号化に失敗しました。';
	LABEL_STATUS_DETAIL_STOPPED        = '暗号化が中止されました。';






implementation

initialization

	//Delphiコンパイラによる最適化を防ぐ

	MSG_CAPTION_ERROR;
	MSG_CAPTION_CONFIRMATION;

	MSG_ERROR_FILE_OPEN;
	MSG_ERROR_OUT_FILE_OPEN;
	MSG_ERROR_OUT_FILE_WRITE;


	MSG_ERROR_NO_DISK_FREE_SPACE;
	MSG_ERROR_OVER_4GB_EXE;
	MSG_ERROR_ZLIB;


	LABEL_STATUS_TITLE_ENCRYPTING;
	LABEL_STATUS_TITLE_LISTING;
	LABEL_STATUS_TITLE_ERROR;
	LABEL_STATUS_TITLE_USER_CANCEL;
	LABEL_STATUS_TITLE_COMPLETE;

	LABEL_STATUS_DETAIL_PREPARING;
	LABEL_STATUS_DETAIL_FAILED;
	LABEL_STATUS_DETAIL_STOPPED;







end.

