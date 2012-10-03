unit MsgDecrypt;

interface

resourcestring

	//メッセージキャプション
	MSG_CAPTION_ERROR                 = 'エラー';
	MSG_CAPTION_CONFIRMATION          = '確認';

	//エラーメッセージ
	MSG_ERROR_FILE_OPEN               = '復号するファイルを開けません。他のアプリケーションが使用中の可能性があります。';
	MSG_ERROR_FILE_BROKEN             = '復号するファイルを開けません。この暗号化ファイルは破壊されています。';
	MSG_ERROR_FILE_NOT_ATC            = '暗号化ファイルではありません。アタッシェケースによって暗号化されたファイルでないか、'+#13+
	                                    'ファイルが壊れている可能性があります。';
	MSG_ERROR_FILE_HIGHER_VERSION     = 'データバージョンがちがいます。復号できません。'+#13+
	                                    'ファイルは上位バージョンのアタッシェケースで暗号化されています。'+#13+
	                                    '最新版を入手して復号を試みてください。';
	MSG_ERROR_FILE_LOWER_VERSION      = 'データバージョンがちがいます。復号できません。'+#13+
	                                    'ファイルはver.1のアタッシェケースで暗号化されています。'+#13+
	                                    '古いバージョン（ver.2.75以前）か正式版を入手して復号を試みてください。';
	MSG_ERROR_PASSWORD_WRONG          = 'パスワードがちがいます。復号できません。'+#13+
	                                    '場合によってはファイルが壊れている可能性もあります。';
	MSG_ERROR_OUT_FILE_OPEN           = '保存する先のファイルが開けません。他のアプリケーションが使用中の可能性があります。';
	MSG_ERROR_OUT_FILE_WRITE          = '保存する先のファイルに書き込めません。復号できません。';
	MSG_ERROR_ZLIB                    = 'zlibライブラリからエラーを返されました。'+#13+'エラー番号：';
	MSG_ERROR_COMPARE_FILE            = 'コンペアに失敗しました。';
	MSG_ERROR_COMPARE_FILE_MISMATCH   = '次のファイルまたはフォルダの内容が異なるようです。';
	MSG_ERROR_COMPARE_ENCRYPT_FILE    = '暗号化:';
	MSG_ERROR_COMPARE_DECRYPT_FILE    = '復号  :';
	MSG_ERROR_COMPARE_NOT_READ_SOURCE = 'コンペアを行う元のファイルを読み込むことができません。';
	MSG_ERROR_COMPARE_DATA_NOT_MATCH  = 'データの一致しないファイルが見つかりました。';

	
	MSG_ERROR_DRIVE_NO_FREE_SPACE     = '復号する先のドライブの空き容量が足りません。'+#13+
	                                    '復号するには、約%.2fMBの空き容量が必要です。復号処理を中止します。';
	MSG_CONFIRM_OVER_WRITE_SAME_FILE  = '同名のファイルが既に存在しています。上書きして復号しますか？';
	MSG_CONFIRM_OVER_WRITE_SAME_DIR   = '同名のフォルダーが既に存在しています。上書きして復号しますか？';
	MSG_ERROR_NOT_OVERWRITE_MYSELF    = '暗号化ファイル自身にファイルまたはフォルダーを復号することはできません。'+#13+
	                                    '復号処理を中止します。';
	MSG_CONFIRM_OPEN_DECRYPTED_FILES  = '5つ以上のファイルを復号後に開こうとしていますが、処理を続けますか？'+#13+
	                                    '「いいえ」を選択すると、開かず復号します。';
	//ダイアログ
	DIALOG_SAVE_AS_FOLDER_CAPTION          = '復号フォルダーの保存';
	DIALOG_SAVE_AS_FILE_CAPTION            = '復号ファイルの保存';
	DIALOG_SAVE_AS_FILTER                  = 'ファイルフォルダー|*';
	DIALOG_BUTTON_NAME_OVERWITE_YES_TO_ALL = '全て上書き(&W)'; // YesToAll
	DIALOG_BUTTON_NAME_OVERWITE_YES        = '上書き(&O)';     // Yes
	DIALOG_BUTTON_NAME_OVERWITE_NO         = '別名保存(&A)';   // No
	DIALOG_BUTTON_NAME_OVERWITE_CANCEL     = 'キャンセル(&C)'; // Cancel

  //ラベル
	LABEL_STATUS_TITLE_DECRYPTING     = '復号しています...';
	LABEL_STATUS_TITLE_COMPARING      = 'コンペアしています...';
	LABEL_STATUS_TITLE_LISTING        = 'ファイルリストの生成';
	LABEL_STATUS_TITLE_ERROR          = 'エラー';
	LABEL_STATUS_TITLE_USER_CANCEL    = 'キャンセル';
	LABEL_STATUS_TITLE_COMPLETE       = '完了';

	LABEL_STATUS_DETAIL_PREPARING     = '復号するための準備をしています...';
	LABEL_STATUS_DETAIL_FAILED        = '復号に失敗しました。';
	LABEL_STATUS_DETAIL_STOPPED       = '復号が中止されました。';



implementation

initialization

	//Delphiコンパイラによる最適化を防ぐ

	MSG_CAPTION_ERROR;
	MSG_CAPTION_CONFIRMATION;

	MSG_ERROR_FILE_OPEN;
	MSG_ERROR_FILE_BROKEN;
	MSG_ERROR_FILE_NOT_ATC;
	MSG_ERROR_FILE_HIGHER_VERSION;
	MSG_ERROR_FILE_LOWER_VERSION;
	MSG_ERROR_PASSWORD_WRONG;
	MSG_ERROR_OUT_FILE_OPEN;
	MSG_ERROR_OUT_FILE_WRITE;
	MSG_ERROR_ZLIB;
	MSG_ERROR_COMPARE_FILE;
	MSG_ERROR_COMPARE_FILE_MISMATCH;
	MSG_ERROR_COMPARE_ENCRYPT_FILE;
	MSG_ERROR_COMPARE_DECRYPT_FILE;
	MSG_ERROR_COMPARE_NOT_READ_SOURCE;
	MSG_ERROR_COMPARE_DATA_NOT_MATCH;


	MSG_ERROR_DRIVE_NO_FREE_SPACE;
	MSG_CONFIRM_OVER_WRITE_SAME_FILE;
	MSG_CONFIRM_OVER_WRITE_SAME_DIR;
	MSG_ERROR_NOT_OVERWRITE_MYSELF;
	MSG_CONFIRM_OPEN_DECRYPTED_FILES;
	DIALOG_SAVE_AS_FOLDER_CAPTION;
	DIALOG_SAVE_AS_FILE_CAPTION;
	DIALOG_SAVE_AS_FILTER;
	DIALOG_BUTTON_NAME_OVERWITE_YES_TO_ALL;
	DIALOG_BUTTON_NAME_OVERWITE_YES;
	DIALOG_BUTTON_NAME_OVERWITE_NO;
	DIALOG_BUTTON_NAME_OVERWITE_CANCEL;

	LABEL_STATUS_TITLE_DECRYPTING;
	LABEL_STATUS_TITLE_COMPARING;
	LABEL_STATUS_TITLE_LISTING;
	LABEL_STATUS_TITLE_ERROR;
	LABEL_STATUS_TITLE_USER_CANCEL;
	LABEL_STATUS_TITLE_COMPLETE;

	LABEL_STATUS_DETAIL_PREPARING;
	LABEL_STATUS_DETAIL_FAILED;
	LABEL_STATUS_DETAIL_STOPPED;




end.

