unit MsgExeOut;

interface

resourcestring

	//アプリケーション名
	APP_NAME                                  = 'アタッシェケース（暗号化ソフトウェア）';

	//フォームボタン
	BUTTON_CAPTION_DECRYOTION                 = '復号する(&D)';
	BUTTON_CAPTION_CANCEL                     = 'キャンセル(&C)';
	BUTTON_CAPTION_EXIT                       = '終了(&X)';
	BUTTON_CAPTION_HELP                       = 'ヘルプ(&H)';

	//チェックボックス
	CHECK_BOX_SAVE_TO_OTHER_DIRECTORY         = 'このファイルがあるフォルダーとは別の場所に保存する(&T)';
	//フォルダー選択ダイアログメッセージ
	DIALOG_MSG_SELECT_SAVE_TO_FOLDER          = '復号する先のフォルダーを指定してください。';

	//フォームラベル
	LABEL_CAPTION_ABOUT                       = 'このファイルについて';
	LABEL_CAPTION_SELECT_DECRYPTION_FOLDER    = '復号先フォルダの指定：';
	LABEL_CAPTION_INPUT_PASSWORD              = 'このファイルは暗号化されています。パスワードを入力してください...';

	//システムメニュー
	SYSTEM_CONTEXT_MENU_DECYPTION             = 'アタッシェケースファイルを復号する';

	//エラーメッセージ
	MSG_ERROR_DO_NOT_READ_REGISTRY            = '理由はわかりませんが、レジストリからの読み込みに失敗しました。';
	MSG_ERROR_THIS_APP_DOES_NOT_EXECUTE_ALONE = 'このプログラムは単体では動作しません。'+#13+
                                              'アタッシェケースからファイルの関連付け設定で呼び出されるときのみ使用されます。';
	MSG_ERROR_OPEN_PASSWORD_FILE              = 'パスワードファイルを開けません。他のアプリケーションで使用中の可能性があります。';
	MSG_ERROR_SAVE_DEC_TO_FOLDER_NOT_EXISTS   = '保存する先のフォルダーが見つかりません。保存設定を再確認してください。'+#13+
                                              '復号処理化を中止します。';
	//メッセージキャプション
	MSG_CAPTION_ERROR                         = 'エラー';
	MSG_CAPTION_CONFIRMATION                  = '確認';
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

	//ラベル
	LABEL_STATUS_TITLE_DECRYPTING     = '復号しています...';
	LABEL_STATUS_TITLE_LISTING        = 'ファイルリストの生成';
	LABEL_STATUS_TITLE_ERROR          = 'エラー';
	LABEL_STATUS_TITLE_USER_CANCEL    = 'キャンセル';
	LABEL_STATUS_TITLE_COMPLETE       = '完了';

	LABEL_STATUS_DETAIL_PREPARING     = '復号するための準備をしています...';
	LABEL_STATUS_DETAIL_FAILED        = '復号に失敗しました。';
	LABEL_STATUS_DETAIL_STOPPED       = '復号が中止されました。';


	//フォーム（バージョン情報）ラベル
	FORM_ABOUT_CAPTION                        = 'バージョン情報';
	FORM_ABOUT_LABEL_CAPTION_DESCRIPTION_01   = 'このファイルは、フリーで、かつオープンソースで開発されている、';
	FORM_ABOUT_LABEL_CAPTION_DESCRIPTION_02   = 'ソフトウェアで暗号化されています。';
	FORM_ABOUT_LABEL_CAPTION_DESCRIPTION_03   = '詳しい情報は以下のサイトをご覧ください。';
	FORM_ABOUT_LABEL_WEB_SITE_URL             = 'http://hibara.org/';



implementation



initialization

	//Delphiコンパイラによる最適化を防ぐ
	SYSTEM_CONTEXT_MENU_DECYPTION;
	MSG_ERROR_DO_NOT_READ_REGISTRY;
	MSG_ERROR_THIS_APP_DOES_NOT_EXECUTE_ALONE;




end.

