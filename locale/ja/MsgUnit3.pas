unit MsgUnit3;

interface

resourcestring

  //フォームキャプション
  FORM_CAPTION_LOAD_INI_FILE                      = 'INIファイル';
  FORM_CAPTION_LOAD_COMMANDLINE                   = 'コマンドライン';
  FORM_CAPTION_LOAD_REGISTRY                      = 'レジストリ';

	//メッセージキャプション
	MSG_CAPTION_ERROR                               = 'エラー';
	MSG_CAPTION_CONFIRMATION                        = '確認';

	//サイドバーメニュー
	LABEL_BASIC                                     = '基本設定';
	LABEL_SAVE                                      = '保存設定';
	LABEL_DELETE                                    = '削除設定';
	LABEL_MOVEMENT                                  = '動作設定';
	LABEL_SYSTEM                                    = 'システム設定';
	LABEL_ADVANCED                                  = '高度な設定';

	//基本設定タブ
	PANEL_BASIC_CAPTION                             = '基本設定 - アタッシェケースの基本的な動作設定を行います。';
	BASIC_PANEL_CHECKBOX_MY_ENC_PASSWORD_KEEP       = '暗号化パスワードを記憶する(&E)';
	BASIC_PANEL_CHECKBOX_MY_DEC_PASSWORD_KEEP       = '復号のパスワードを記憶する(&D)';
	BASIC_PANEL_CHECKBOX_MY_PASSWORD_EXE            = '記憶パスワードで確認せずに実行する(&M)';
	BASIC_PANEL_CHECKBOX_OPEN_FOLDER                = 'フォルダーの場合、復号後に開く(&O)';
	BASIC_PANEL_CHECKBOX_OPEN_FILE                  = '復号したファイルを関連付けられたソフトで開く(&A)';
	BASIC_PANEL_CHECKBOX_END_TO_EXIT                = '処理後にアタッシェケースを終了する(&X)';
	BASIC_PANEL_CHECKBOX_WINDOW_FOREGROUND          = 'デスクトップで最前面にウィンドウを表示する(&F) ※アタッシェケースの再起動が必要です。';
	BASIC_PANEL_CHECKBOX_NO_HIDE_PASSWORD           = 'パスワードを表示しながら入力する(&N)';
	BASIC_PANEL_CHECKBOX_SAVE_TO_EXEOUT             = '常に自己実行形式で出力する(&T)';
	BASIC_PANEL_CHECKBOX_SHOW_EXEOUT_CHKBOX         = 'メインフォームにチェックボックスを表示する(&I)';
	BASIC_PANEL_CHECKBOX_ASK_ENC_DECODE             = '暗号/復号処理かを問い合わせる(&K)';
	BASIC_PANEL_CHECKBOX_NO_MULTIPLE_INSTANCE       = '複数起動しない(&S)';

	DIALOG_MSG_MEM_ENCRYPT_PASSWORD_HASH            = 'ドラッグ＆ドロップされたファイルのSHA-1ハッシュ値を'+#13+
                                                    '暗号化パスワードとして記憶します。よろしいですか？';
	DIALOG_MSG_MEM_DECRYPT_PASSWORD_HASH            = 'ドラッグ＆ドロップされたファイルのSHA-1ハッシュ値を'+#13+
                                                    '復号するパスワードとして記憶します。よろしいですか？';
	DIALOG_MSG_ALLOW_PASSWORD_FILE_OPTION           = 'パスワードファイルを使用するには【高度設定】タブの'+#13+
                                                    '「パスワードファイルにファイルの指定を許可する(&D)」を有効にしてください。';

	//保存設定タブ
	PANEL_SAVE_CAPTION                              = '保存設定 - 出力するファイル/フォルダの細かい設定を行います。';
	SAVE_PANEL_CHECKBOX_SAVE_TO_SAME_FOLDER         = '常に同じ場所へ暗号化ファイルを保存する(&C)';
	SAVE_PANEL_CHECKBOX_DECODE_TO_SAME_FOLDER       = '常に同じ場所へファイルを復号する(&D)';
	SAVE_PANEL_CHECKBOX_CONFIRM_OVERWRITE           = '同名ファイルがある場合は上書きの確認をする(&O)';
	SAVE_PANEL_RADIO_GROUP_MULTIPLE_FILES           = '複数ファイルがあるときの処理：';
	SAVE_PANEL_RADIO_GROUP_NO_ACTION                = '何もしない(&N)';
	SAVE_PANEL_RADIO_GROUP_ALL_FILES_PACK           = '複数ファイルは一つの暗号化ファイルにまとめる(&L)';
	SAVE_PANEL_RADIO_GROUP_FILES_ONE_BY_ONE         = 'フォルダ内のファイルは個別に暗号化/復号する(&P)';
	SAVE_PANEL_CHECKBOX_NO_PARENT_FOLDER            = '復号するとき親フォルダーを生成しない(&M)';
	SAVE_PANEL_CHECKBOX_KEEP_TIME_STAMP             = '暗号化ファイルのタイムスタンプを元ファイルに合わせる(&G)';
	SAVE_PANEL_CHECKBOX_SAME_TIME_STAMP             = 'タイムスタンプを復号した日時に合わせる(&P)';
	SAVE_PANEL_CHECKBOX_EXT_IN_ATC_FILE_NAME        = '暗号化ファイル名に拡張子を含める(&I)';
	SAVE_PANEL_CHECKBOX_AUTO_NAME                   = '自動で暗号化ファイル名を付加する(&R)';

	POPUP_MENU_ITEM_INSERT_FILENAME_FORMAT          = 'ファイル名（拡張子を除く）(&F)';
	POPUP_MENU_ITEM_INSERT_EXTENSION_FORMAT         = '拡張子（ピリオドを含む）(&X)';
	POPUP_MENU_ITEM_INSERT_DATETIME_FORMAT          = '日時(&D)...';
	POPUP_MENU_ITEM_INSERT_SERIAL_NUM__FORMAT       = '連番(&S)...';
	POPUP_MENU_ITEM_INSERT_RANDOM_TEXT_FORMAT       = 'ランダムな文字列(&R)...';
	POPUP_MENU_ITEM_INSERT_FILENAME_HEAD_FORMAT     = 'ファイル名先頭の文字列(&T)...';
	POPUP_MENU_ITEM_INSERT_FILENAME_END_FORMAT      = 'ファイル名末尾の文字列(&E)...';

  DIALOG_MSG_SELECT_SAVE_ATC_FILE_TO_DIR_PATH     = '暗号化ファイルを常に保存したいフォルダーを選択してください。';
  DIALOG_MSG_SELECT_DECODE_TO_SAME_DIR_PATH       = '復号したファイルを常に保存したいフォルダーを選択してください。';


	//削除設定タブ
	PANEL_DELETE_CAPTION                            = '削除設定 - 処理後のファイル/フォルダーの削除方法などを設定します。';
	DELETE_PANEL_CHECKBOX_DEL_ORG_FILE              = '暗号化した後、元ファイル/フォルダーを削除する(&D)';
	DELETE_PANEL_CHECKBOX_DEL_ENC_FILE              = '復号した後、暗号化ファイルを削除する(&E)';
	DELETE_PANEL_CHECKBOX_SHOW_DELETE_CHKBOX        = 'メインフォームにチェックボックスを表示する(&I)';
	DELETE_PANEL_RADIO_GROUP_DELETE_OPTION          = '削除の詳細設定：';
	DELETE_PANEL_RADIO_GROUP_NORMAL_DELETE          = '通常の削除を行う(&N)';
	DELETE_PANEL_RADIO_GROUP_GO_TO_TRASH            = 'ごみ箱への削除を行う(&T)';
	DELETE_PANEL_RADIO_GROUP_COMPLETE_DELETE        = '完全削除を行う(&C)';
	DELETE_PANEL_DEL_RAND_NUM                       = '乱数の書き込み回数：';
	DELETE_PANEL_DEL_ZERO_NUM                       = 'ゼロ(NULL)の書き込み回数：';

	//動作設定タブ
	PANEL_MOVEMENT_CAPTION                          = '動作設定 - 圧縮率の設定、コンペアを行うかを設定します。';
	MOVEMENT_PANEL_RADIO_GROUP_COMPRESS_RATE        = '圧縮率設定：';
	MOVEMENT_PANEL_CHECKBOX_COMPRESS_RATE           = '圧縮する(&P)';
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_0            = '< 0 > 圧縮しない';
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_1            = '< 1 > 圧縮';
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_2            = '< 2 > 圧縮';
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_3            = '< 3 > 圧縮';
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_4            = '< 4 > 圧縮';
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_5            = '< 5 > 圧縮';
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_6            = '< 6 > 標準圧縮';
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_7            = '< 7 > 圧縮';
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_8            = '< 8 > 圧縮';
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_9            = '< 9 > 最大圧縮';
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_DETAIL       = '暗号化ファイルの圧縮率を設定できます。'+#13+
	                                                  '無圧縮 < 0 > ～ 最大 < 9 > までの指定が可能で、'+#13+
	                                                  '圧縮率は高いほど処理に時間がかかります。';
	MOVEMENT_PANEL_RADIO_GROUP_COMPARE              = 'コンペア：';
	MOVEMENT_PANEL_CHECKBOX_COMPARE_FILE            = '暗号化した後にデータのコンペアを行う(&P)';

	//システム設定タブ
	PANEL_SYSTEM_CAPTION                            = 'システム設定 - Windowsシステムに関係する設定を行います。';
	SYSTEM_PANEL_RADIO_GROUP_WINDOWS_SYS_OPT        = 'Windowsシステムの設定：';
	SYSTEM_PANEL_BUTTON_ASSOCIATE_ATC_FILE          = '暗号化ファイル(*.atc)の関連付け設定(&A)';
	SYSTEM_PANEL_BUTTON_UN_ASSOCIATE_ATC_FILE       = '暗号化ファイル(*.atc)の関連付け解除(&F)';
	SYSTEM_PANEL_BUTTON_SHORTCUT_TO_SENDTOFILE_DIR  = '「送る」フォルダーにアタッシェケースを追加(&S)';
	SYSTEM_PANEL_BUTTON_DELETE_FROM_SENDTOFILE_DIR  = '「送る」フォルダーからアタッシェケースを削除(&S)';
	SYSTEM_PANEL_RADIO_GROUP_CHANGE_FILE_ICON       = 'ファイルアイコンの変更：';
	SYSTEM_PANEL_BUTTON_ADD_FILE_ICON               = 'アイコンファイルの登録(&R)';
	SYSTEM_PANEL_RADIO_GROUP_OUTPUT_OPTION_DATA     = '動作設定の出力：';
	SYSTEM_PANEL_BUTTON_OUTPUT_OPTION_DATA          = '現在の動作設定を出力する(&O)';
	SYSTEM_PANEL_BUTTON_CHANGE_TEMP_OPTION_DATA     = '一時設定をメイン設定に置き換える(&P)';

  SYSTEM_PANEL_COMBO_USER_REG_FILE_ICON           = 'ユーザー登録';

  DIALOG_MSG_NOT_FOUND_ATC_SETUP_EXE              = 'アタッシェケースに付属する関連付けアプリケーション(AtcSetup.exe)が見つかりません。';
	DIALOG_SAVE_TO_INIFILE_FILTER_TEXT              = 'INIファイル(*.ini)|*.ini|すべてのファイル(*.*)|*.*';
	DIALOG_SAVE_TO_INIFILE_TITLE_TEXT               = '現在の動作設定をINIファイルに保存';

  DIALOG_MSG_CONFIRM_OVERWRITE_SETTINGS           = '現在、読み込まれている以下のINIファイル設定をレジストリへ反映しますか？';


	//高度設定タブ
	PANEL_ADVANCED_CAPTION                          = '高度な設定 - 上級者向けの設定を行います。';
	ADVANCED_PANEL_LABEL_ADVANCED_OPTION_ALERT_MSG  = 'ここでの設定を誤ると、あなたの大切なデータを失う恐れがあります。'+#13+
	                                                  '必ずヘルプファイルを参照し、十分注意してから行ってください。';
	ADVANCED_PANEL_RADIO_GROUP_PASSWORD_FILE        = 'パスワードファイルの設定：';
	ADVANCED_PANEL_CHECKBOX_ALLOW_PASS_FILE         = 'パスワードに「ファイル」の指定を許可する(&D)';
	ADVANCED_PANEL_CHECKBOX_PASS_FILE               = '暗号化時にパスワードファイルを自動チェックする(&H)';
	ADVANCED_PANEL_CHECKBOX_PASS_FILE_DECRYPT       = '復号時にパスワードファイルを自動チェックする(&K)';
	ADVANCED_PANEL_CHECKBOX_NO_ERR_MSG_ON_PASS_FILE = 'ファイルがない場合、エラーメッセージを表示しない(&N)';
	ADVANCED_PANEL_RADIO_GROUP_CAMO_EXT             = '拡張子偽装設定：';
	ADVANCED_PANEL_CHECKBOX_ADD_CAMO_EXT            = '暗号化ファイルの拡張子を偽装する(&E)';
	ADVANCED_PANEL_LABEL_CAMO_EXT_DETAIL            = '※空白にすると拡張子無し';
	ADVANCED_PANEL_RADIO_GROUP_TYPE_LIMIT           = 'パスワード入力回数の設定：';
	ADVANCED_PANEL_LABEL_TYPE_LIMIT_ALERT_MSG       = 'この回数設定はアタッシェケース本体にではなく個々のファイルに設定されます。';
	ADVANCED_PANEL_LABEL_TYPE_LIMIT_NUM             = '暗号化ファイルに入力回数を設定する：';
	ADVANCED_PANEL_CHECKBOX_BROKEN                  = 'その回数を間違えて入力があった場合、暗号化ファイルを破壊する(&X)';

	DIALOG_CAPTION_SELECT_PASSWORD_FILE_ENCRYPT     = '暗号化するときのパスワードファイル選択';
	DIALOG_MSG_SELECT_PASSWORD_FILE_DECRYPT         = '復号するときのパスワードファイルの選択';
	DIALOG_SELECT_PASSWORD_FILE_FILTER_TEXT         = 'すべてのファイル(*.*)|*.*';
	DIALOG_MSG_ALLOW_PASSWORD_FILE_ENABLED          = 'パスワードファイルを使用するには「パスワードファイルにファイルの指定を許可する(&D)」を'+#13+
	                                                  '有効にする必要があります。有効にしますか？';
	DIALOG_MSG_ALLOW_ENCRYPT_PASSWORD_FILE_ENABLED  = 'パスワードファイル欄にファイルがドラッグ＆ドロップされました。'+#13+
	                                                  '暗号化時に使うパスワードファイルとして登録して有効にしますか？';
	DIALOG_MSG_ALLOW_DECRYPT_PASSWORD_FILE_ENABLED  = 'パスワードファイル欄にファイルがドラッグ＆ドロップされました。'+#13+
	                                                  '復号時に使うパスワードファイルとして登録して有効にしますか？';
	DIALOG_MSG_ERROR_PASSWORD_FILE_WRONG            = '有効なファイル（たとえばディレクトリなど）以外は登録することができません。';


	//デバッグ（隠し）タブ
	PANEL_DEBUG_CAPTION                             = 'デバッグページ - アタッシェケースの全設定値を表示します。';



implementation

initialization

	//Delphiコンパイラによる最適化を防ぐ

  FORM_CAPTION_LOAD_INI_FILE;
  FORM_CAPTION_LOAD_COMMANDLINE;
  FORM_CAPTION_LOAD_REGISTRY;

	MSG_CAPTION_ERROR;
	MSG_CAPTION_CONFIRMATION;

	LABEL_BASIC;
	LABEL_SAVE;
	LABEL_DELETE;
	LABEL_MOVEMENT;
	LABEL_SYSTEM;
	LABEL_ADVANCED;

	PANEL_BASIC_CAPTION;
	BASIC_PANEL_CHECKBOX_MY_ENC_PASSWORD_KEEP;
	BASIC_PANEL_CHECKBOX_MY_DEC_PASSWORD_KEEP;
	BASIC_PANEL_CHECKBOX_MY_PASSWORD_EXE;
	BASIC_PANEL_CHECKBOX_OPEN_FOLDER;
	BASIC_PANEL_CHECKBOX_OPEN_FILE;
	BASIC_PANEL_CHECKBOX_END_TO_EXIT;
	BASIC_PANEL_CHECKBOX_WINDOW_FOREGROUND;
	BASIC_PANEL_CHECKBOX_NO_HIDE_PASSWORD;
	BASIC_PANEL_CHECKBOX_SAVE_TO_EXEOUT;
	BASIC_PANEL_CHECKBOX_SHOW_EXEOUT_CHKBOX;
	BASIC_PANEL_CHECKBOX_ASK_ENC_DECODE;
	BASIC_PANEL_CHECKBOX_NO_MULTIPLE_INSTANCE;

	DIALOG_MSG_MEM_ENCRYPT_PASSWORD_HASH;
	DIALOG_MSG_MEM_DECRYPT_PASSWORD_HASH;
	DIALOG_MSG_ALLOW_PASSWORD_FILE_OPTION;

	PANEL_SAVE_CAPTION;
	SAVE_PANEL_CHECKBOX_SAVE_TO_SAME_FOLDER;
	SAVE_PANEL_CHECKBOX_DECODE_TO_SAME_FOLDER;
	SAVE_PANEL_CHECKBOX_CONFIRM_OVERWRITE;
	SAVE_PANEL_RADIO_GROUP_MULTIPLE_FILES;
	SAVE_PANEL_RADIO_GROUP_NO_ACTION;
	SAVE_PANEL_RADIO_GROUP_ALL_FILES_PACK;
	SAVE_PANEL_RADIO_GROUP_FILES_ONE_BY_ONE;
	SAVE_PANEL_CHECKBOX_NO_PARENT_FOLDER;
	SAVE_PANEL_CHECKBOX_KEEP_TIME_STAMP;
	SAVE_PANEL_CHECKBOX_SAME_TIME_STAMP;
	SAVE_PANEL_CHECKBOX_EXT_IN_ATC_FILE_NAME;
	SAVE_PANEL_CHECKBOX_AUTO_NAME;

	POPUP_MENU_ITEM_INSERT_FILENAME_FORMAT;
	POPUP_MENU_ITEM_INSERT_EXTENSION_FORMAT;
	POPUP_MENU_ITEM_INSERT_DATETIME_FORMAT;
	POPUP_MENU_ITEM_INSERT_SERIAL_NUM__FORMAT;
	POPUP_MENU_ITEM_INSERT_RANDOM_TEXT_FORMAT;
	POPUP_MENU_ITEM_INSERT_FILENAME_HEAD_FORMAT;
	POPUP_MENU_ITEM_INSERT_FILENAME_END_FORMAT;

	DIALOG_MSG_SELECT_SAVE_ATC_FILE_TO_DIR_PATH;
	DIALOG_MSG_SELECT_DECODE_TO_SAME_DIR_PATH;


	PANEL_DELETE_CAPTION;
	DELETE_PANEL_CHECKBOX_DEL_ORG_FILE;
	DELETE_PANEL_CHECKBOX_DEL_ENC_FILE;
	DELETE_PANEL_CHECKBOX_SHOW_DELETE_CHKBOX;
	DELETE_PANEL_RADIO_GROUP_DELETE_OPTION;
	DELETE_PANEL_RADIO_GROUP_NORMAL_DELETE;
	DELETE_PANEL_RADIO_GROUP_GO_TO_TRASH;
	DELETE_PANEL_RADIO_GROUP_COMPLETE_DELETE;
	DELETE_PANEL_DEL_RAND_NUM;
	DELETE_PANEL_DEL_ZERO_NUM;

	PANEL_MOVEMENT_CAPTION;
	MOVEMENT_PANEL_RADIO_GROUP_COMPRESS_RATE;
	MOVEMENT_PANEL_CHECKBOX_COMPRESS_RATE;
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_0;
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_1;
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_2;
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_3;
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_4;
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_5;
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_6;
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_7;
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_8;
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_9;
	MOVEMENT_PANEL_LABEL_COMPRESS_RATE_DETAIL;
	MOVEMENT_PANEL_RADIO_GROUP_COMPARE;
	MOVEMENT_PANEL_CHECKBOX_COMPARE_FILE;

	PANEL_SYSTEM_CAPTION;
	SYSTEM_PANEL_RADIO_GROUP_WINDOWS_SYS_OPT;
	SYSTEM_PANEL_BUTTON_ASSOCIATE_ATC_FILE;
	SYSTEM_PANEL_BUTTON_UN_ASSOCIATE_ATC_FILE;
	SYSTEM_PANEL_BUTTON_SHORTCUT_TO_SENDTOFILE_DIR;
	SYSTEM_PANEL_BUTTON_DELETE_FROM_SENDTOFILE_DIR;
	SYSTEM_PANEL_RADIO_GROUP_CHANGE_FILE_ICON;
	SYSTEM_PANEL_BUTTON_ADD_FILE_ICON;
	SYSTEM_PANEL_RADIO_GROUP_OUTPUT_OPTION_DATA;
	SYSTEM_PANEL_BUTTON_OUTPUT_OPTION_DATA;
	SYSTEM_PANEL_BUTTON_CHANGE_TEMP_OPTION_DATA;

	SYSTEM_PANEL_COMBO_USER_REG_FILE_ICON;

	DIALOG_MSG_NOT_FOUND_ATC_SETUP_EXE;
	DIALOG_SAVE_TO_INIFILE_FILTER_TEXT;
	DIALOG_SAVE_TO_INIFILE_TITLE_TEXT;

	DIALOG_MSG_CONFIRM_OVERWRITE_SETTINGS;




	PANEL_ADVANCED_CAPTION;
	ADVANCED_PANEL_LABEL_ADVANCED_OPTION_ALERT_MSG;
	ADVANCED_PANEL_RADIO_GROUP_PASSWORD_FILE;
	ADVANCED_PANEL_CHECKBOX_ALLOW_PASS_FILE;
	ADVANCED_PANEL_CHECKBOX_PASS_FILE;
	ADVANCED_PANEL_CHECKBOX_PASS_FILE_DECRYPT;
	ADVANCED_PANEL_CHECKBOX_NO_ERR_MSG_ON_PASS_FILE;
	ADVANCED_PANEL_RADIO_GROUP_CAMO_EXT;
	ADVANCED_PANEL_CHECKBOX_ADD_CAMO_EXT;
	ADVANCED_PANEL_LABEL_CAMO_EXT_DETAIL;
	ADVANCED_PANEL_RADIO_GROUP_TYPE_LIMIT;
	ADVANCED_PANEL_LABEL_TYPE_LIMIT_ALERT_MSG;
	ADVANCED_PANEL_LABEL_TYPE_LIMIT_NUM;
	ADVANCED_PANEL_CHECKBOX_BROKEN;

	DIALOG_CAPTION_SELECT_PASSWORD_FILE_ENCRYPT;
	DIALOG_MSG_SELECT_PASSWORD_FILE_DECRYPT;
	DIALOG_SELECT_PASSWORD_FILE_FILTER_TEXT;
	DIALOG_MSG_ALLOW_PASSWORD_FILE_ENABLED;
	DIALOG_MSG_ALLOW_ENCRYPT_PASSWORD_FILE_ENABLED;
	DIALOG_MSG_ALLOW_DECRYPT_PASSWORD_FILE_ENABLED;
	DIALOG_MSG_ERROR_PASSWORD_FILE_WRONG;


	PANEL_DEBUG_CAPTION;




end.

