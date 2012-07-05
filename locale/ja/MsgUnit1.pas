unit MsgUnit1;

interface

resourcestring

  //パネル
  DRAG_AND_DROP_HERE                        = '暗号化/復号するファイルかフォルダを'+#13+'ここへドラッグ＆ドロップしてください。';
  INPUT_PASSWORD                            = 'ファイル/フォルダを暗号化します。パスワードを入力してください：';
  CONFIRM_PASSWORD                          = '確認のため、もう一度パスワードを入力してください：';
  INPUT_DECRYPT_PASSWORD                    = '復号して元のファイル/フォルダに戻します。パスワードを入力してください：';

	//ダイアログ
	DIALOG_SELECT_DIRECTORY_TEXT              = '暗号化するディレクトリ（フォルダ）を選択してください。';

	DIALOG_OPEN_FILES_FOR_ENCRYPTION_TITLE    = '暗号化するファイルを開く';
	DIALOG_OPEN_FILES_FOR_ENCRYPTION_FILTER   = 'すべてのファイル(*.*)|*.*';
	DIALOG_OPEN_FILES_FOR_DECRYPTION_TITLE    = '復号するアタッシェケースファイルを開く';
	DIALOG_OPEN_FILES_FOR_DECRYPTION_FINTER   = 'アタッシェケースファイル(*.atc)|*.atc|すべてのファイル(*.*)|*.*';
	DIALOG_SAVE_FILES_FOR_ENCRYPTION_TITLE    = '暗号化されたアタッシェケースファイル名の入力';
	DIALOG_SAVE_FILES_FOR_ENCRYPTION_FILTER   = 'アタッシェケースファイル(*.atc)|*.atc|実行形式ファイル(*.exe)|(*.exe)|すべてのファイル(*.*)|*.*';

	//ダイアログキャプション
	DIALOG_SAVE_AS_FOLDER_CAPTION             = '復号フォルダーの保存';
	DIALOG_SAVE_AS_FILE_CAPTION               = '復号ファイルの保存';
	//ダイアログフィルター
	DIALOG_SAVE_AS_FILTER                     = 'ファイルフォルダー|*';
	//ダイアログボタンキャプション
	DIALOG_BUTTON_NAME_OVERWITE_YES_TO_ALL    = '全て上書き(&W)'; // YesToAll
	DIALOG_BUTTON_NAME_OVERWITE_YES           = '上書き(&O)';     // Yes
	DIALOG_BUTTON_NAME_OVERWITE_NO            = '別名保存(&A)';   // No
	DIALOG_BUTTON_NAME_OVERWITE_CANCEL        = 'キャンセル(&C)'; // Cancel

	//チェックボックス
	CHECK_BOX_EXEFILE_OUT                     = '実行形式出力(&X)';

	//メッセージキャプション
	MSG_CAPTION_ERROR                         = 'エラー';
	MSG_CAPTION_CONFIRMATION                  = '確認';

  //メッセージ
  MSG_BROKEN_ATC_FILE                       = 'この暗号化ファイルはパスワード入力に失敗して破壊されているようです。';
  MSG_FILE_OPEN_ERROR                       = 'ファイルを開けません。他のアプリケーションで使用中の可能性があります。';
	MSG_CONFIRM_ENCRYPT_OR_DECRYPT            = 'ドラッグ＆ドロップされたファイルまたはフォルダーをどのように処理しますか？';
	MSG_ERROR_SAVE_ENC_TO_FOLDER_NOT_EXISTS   = '保存する先のフォルダーが見つかりません。保存設定を再確認してください。'+#13+
	                                            '暗号化を中止します。';
	MSG_ERROR_SAVE_DEC_TO_FOLDER_NOT_EXISTS   = '保存する先のフォルダーが見つかりません。保存設定を再確認してください。'+#13+
	                                            '復号処理を中止します。';
	MSG_ERROR_NOT_FOUND_ENC_PASSWORD_FILE     = '動作設定で指定された暗号化するパスワードファイルが見つかりません。'+#13+
	                                            '設定を確認してください。';
	MSG_ERROR_NOT_FOUND_DEC_PASSWORD_FILE     = '動作設定で指定された復号するパスワードファイルが見つかりません。'+#13+
	                                            '設定を確認してください。';
	MSG_ERROR_OPEN_PASSWORD_FILE              = 'パスワードファイルを開けません。他のアプリケーションで使用中の可能性があります。';
	MSG_ERROR_MISMATCH_PASSWORD_FILE          = '先に入力されたパスワードファイルとちがいます。SHA-1ハッシュ値が一致しませんでした。'+#13+
	                                            '暗号化することができません。';
	MSG_ERROR_NOT_EXISTS_COMPAER_FIlE         = 'コンペアする暗号化ファイルまたはフォルダーが見つかりません。'+#13+
	                                            'コンペアに失敗しました。';

  //ラベル
  LABEL_STATUS_TITLE_ENCRYPTING             = '暗号化しています...';
  LABEL_STATUS_TITLE_DECRYPTING             = '復号しています...';
  LABEL_STATUS_TITLE_LISTING                = 'ファイルリストの生成';
  LABEL_STATUS_TITLE_ERROR                  = 'エラー';
  LABEL_STATUS_TITLE_USER_CANCEL            = 'キャンセル';
  LABEL_STATUS_TITLE_COMPLETE               = '完了';

  LABEL_STATUS_DETAIL_PREPARING_ENCRYPT     = '暗号化するための準備をしています...';
  LABEL_STATUS_DETAIL_FAILED_ENCRYPT        = '暗号化に失敗しました。';
  LABEL_STATUS_DETAIL_STOPPED_ENCRYPT       = '暗号化が中止されました。';
  LABEL_STATUS_DETAIL_PREPARING_DECRYPT     = '復号するための準備をしています...';
  LABEL_STATUS_DETAIL_FAILED_DECRYPT        = '復号に失敗しました。';
  LABEL_STATUS_DETAIL_STOPPED_DECRYPT       = '復号処理が中止されました。';



	//ボタン
	BUTTON_CAPTION_ENCRYPTION                 = '暗号化する(&E)';
	BUTTON_CAPTION_DECRYPTION                 = '復号する(&D)';
	BUTTON_CAPTION_CANCEL                     = 'キャンセル(&C)';

	//バルーンヒント
	BALLOON_HINT_PASSWOED_MISMATCH            = '先に入力されたパスワードとちがいます。';
	BALLOON_HINT_PASSWOED_CONFIRMATION        = '全角・半角のちがい、CAPSロックがないか確認してください。';
	BALLOON_HINT_PASSWOED_LENGTH_LONG         = 'パスワードが長すぎます。';
	BALLOON_HINT_PASSWOED_LENGTH_SESCRIPTION  = 'パスワードに使用できる文字数は半角32文字（全角16文字）以内です。';
	BALLOON_HINT_PASSWORD_HASH_TITLE          = 'パスワードに以下のファイルのハッシュ値が入力されました。';



implementation

initialization

	//Delphiコンパイラによる最適化を防ぐ

	DRAG_AND_DROP_HERE;
	INPUT_PASSWORD;
	CONFIRM_PASSWORD;
	INPUT_DECRYPT_PASSWORD;

	DIALOG_SELECT_DIRECTORY_TEXT;

	DIALOG_OPEN_FILES_FOR_ENCRYPTION_TITLE;
	DIALOG_OPEN_FILES_FOR_ENCRYPTION_FILTER;
	DIALOG_OPEN_FILES_FOR_DECRYPTION_TITLE;
	DIALOG_OPEN_FILES_FOR_DECRYPTION_FINTER;
	DIALOG_SAVE_FILES_FOR_ENCRYPTION_TITLE;
	DIALOG_SAVE_FILES_FOR_ENCRYPTION_FILTER;

	DIALOG_SAVE_AS_FOLDER_CAPTION;
	DIALOG_SAVE_AS_FILE_CAPTION;
	DIALOG_SAVE_AS_FILTER;
	DIALOG_BUTTON_NAME_OVERWITE_YES_TO_ALL;
	DIALOG_BUTTON_NAME_OVERWITE_YES;
	DIALOG_BUTTON_NAME_OVERWITE_NO;
	DIALOG_BUTTON_NAME_OVERWITE_CANCEL;

	CHECK_BOX_EXEFILE_OUT;

	MSG_CAPTION_ERROR;
	MSG_CAPTION_CONFIRMATION;

	MSG_BROKEN_ATC_FILE;
	MSG_FILE_OPEN_ERROR;
	MSG_CONFIRM_ENCRYPT_OR_DECRYPT;
	MSG_ERROR_SAVE_ENC_TO_FOLDER_NOT_EXISTS;
	MSG_ERROR_SAVE_DEC_TO_FOLDER_NOT_EXISTS;
	MSG_ERROR_NOT_FOUND_ENC_PASSWORD_FILE;
	MSG_ERROR_NOT_FOUND_DEC_PASSWORD_FILE;
	MSG_ERROR_OPEN_PASSWORD_FILE;
	MSG_ERROR_MISMATCH_PASSWORD_FILE;

	LABEL_STATUS_TITLE_ENCRYPTING;
	LABEL_STATUS_TITLE_DECRYPTING;
	LABEL_STATUS_TITLE_LISTING;
	LABEL_STATUS_TITLE_ERROR;
	LABEL_STATUS_TITLE_USER_CANCEL;
	LABEL_STATUS_TITLE_COMPLETE;

	LABEL_STATUS_DETAIL_PREPARING_ENCRYPT;
	LABEL_STATUS_DETAIL_FAILED_ENCRYPT;
	LABEL_STATUS_DETAIL_STOPPED_ENCRYPT;
	LABEL_STATUS_DETAIL_PREPARING_DECRYPT;
	LABEL_STATUS_DETAIL_FAILED_DECRYPT;
	LABEL_STATUS_DETAIL_STOPPED_DECRYPT;



	BUTTON_CAPTION_ENCRYPTION;
	BUTTON_CAPTION_DECRYPTION;
	BUTTON_CAPTION_CANCEL;

	BALLOON_HINT_PASSWOED_MISMATCH;
	BALLOON_HINT_PASSWOED_CONFIRMATION;
	BALLOON_HINT_PASSWOED_LENGTH_LONG;
	BALLOON_HINT_PASSWOED_LENGTH_SESCRIPTION;
	BALLOON_HINT_PASSWORD_HASH_TITLE;




end.

