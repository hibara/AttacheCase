unit MsgUnit4;

interface

resourcestring

	//メッセージキャプション
	MSG_CAPTION_ERROR                             = 'エラー';
	MSG_CAPTION_CONFIRMATION                      = '確認';

	FORM_CAPTION_INPUT_ENCRYPT_MEM_PASSWORD       = '記憶する暗号化パスワードの入力';
	FORM_CAPTION_INSERT_DECRYPT_MEM_PASSWORD      = '記憶する復号パスワードの入力';
	FORM_CAPTION_INSERT_DATETIME_FORMAT           = '日時書式の挿入';
	FORM_CAPTION_INSERT_SERIAL_NUM_FORMAT         = '連番書式の挿入';
	FORM_CAPTION_INSERT_RANDOM_TEXT_NUM_FORMAT    = 'ランダム文字列数の挿入';
	FORM_CAPTION_INSERT_FILE_NAME_HEAD_NUM_FORMAT = 'ファイル名の先頭文字数の挿入';
	FORM_CAPTION_INSERT_FILE_NAME_END_NUM_FORMAT  = 'ファイル名の末尾文字数の挿入';

	BUTTON_CAPTION_OK                             = '&OK';
	BUTTON_CAPTION_INSERT                         = '挿入(&I)';
	BUTTON_CAPTION_CANCEL                         = 'キャンセル(&C)';

	LABEL_INPUT_MEM_NEW_PASSWORD                  = '記憶する新しいパスワードを入力してください...';
	LABEL_INPUT_MEM_NEW_PASSWORD_CONFIRM          = '確認するため、もう一度同じパスワード入力してください...';

	LABEL_INSERT_DATETIME                         = '挿入する日時の書式：';
	LABEL_INSERT_SERIAL_NUM                       = '連番の桁数：';
	LABEL_INSERT_RANDOM_TEXT                      = 'ランダム文字列の数：';
	LABEL_INSERT_FILE_NAME_HEAD                   = 'ファイル名の先頭文字数：';
	LABEL_INSERT_FILE_NAME_END                    = 'ファイル名の末尾文字数：';


	MSG_ERROR_PASSWORD_MAX_NUM                    = 'パスワードに使用できる文字数は半角56文字（全角28文字）以内です。';
	MSG_ERROR_INPUT_PASSWORD_MISMATCH             = '入力されたパスワードとちがいます。'+#13+'再度パスワードを入力してください。';



implementation

initialization

	//Delphiコンパイラによる最適化を防ぐ

	MSG_CAPTION_ERROR;
	MSG_CAPTION_CONFIRMATION;

	FORM_CAPTION_INPUT_ENCRYPT_MEM_PASSWORD;
	FORM_CAPTION_INSERT_DECRYPT_MEM_PASSWORD;
	FORM_CAPTION_INSERT_DATETIME_FORMAT;
	FORM_CAPTION_INSERT_SERIAL_NUM_FORMAT;
	FORM_CAPTION_INSERT_RANDOM_TEXT_NUM_FORMAT;
	FORM_CAPTION_INSERT_FILE_NAME_HEAD_NUM_FORMAT;
	FORM_CAPTION_INSERT_FILE_NAME_END_NUM_FORMAT;

	BUTTON_CAPTION_OK;
	BUTTON_CAPTION_INSERT;
	BUTTON_CAPTION_CANCEL;

	LABEL_INPUT_MEM_NEW_PASSWORD;
	LABEL_INPUT_MEM_NEW_PASSWORD_CONFIRM;

	LABEL_INSERT_DATETIME;
	LABEL_INSERT_SERIAL_NUM;
	LABEL_INSERT_RANDOM_TEXT;
	LABEL_INSERT_FILE_NAME_HEAD;
	LABEL_INSERT_FILE_NAME_END;


	MSG_ERROR_PASSWORD_MAX_NUM;
	MSG_ERROR_INPUT_PASSWORD_MISMATCH;




end.

