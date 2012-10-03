unit MsgDelete;

interface

resourcestring

	//メッセージキャプション
	MSG_CAPTION_ERROR                        = 'エラー';
	MSG_CAPTION_CONFIRMATION                 = '確認';

  //メッセージダイアログ
  MSG_ERROR_NOT_FILE_OPEN                  = 'ファイル削除中にエラーが発生しました。ファイルを開くことができません。';
  MSG_ERROR_GO_TO_TRASH_FAILED             = 'ごみ箱への削除に失敗しました。'+#13+
                                             '一部のファイル/フォルダが削除できずに残ってしまった可能性があります。';

  //ラベル
  LABEL_STATUS_TITLE_LISTING               = 'ファイルリストの生成';
  LABEL_STATUS_TITLE_DELETING              = '削除しています...';
  LABEL_STATUS_TITLE_COMPLETE_DELETING     = '完全削除しています...';
  LABEL_STATUS_TITLE_GO_TO_TRASH           = 'ごみ箱へ移動しています...';
  LABEL_STATUS_TITLE_ERROR                 = 'エラー';
  LABEL_STATUS_TITLE_USER_CANCEL           = 'キャンセル';
  LABEL_STATUS_TITLE_COMPLETE              = '完了';

  //ラベル（詳細説明）
  LABEL_STATUS_DETAIL_PREPARING            = '削除するための準備をしています...';
  LABEL_STATUS_DETAIL_WRITE_RAND           = '乱数を書き込み、完全削除を行っています...(Step:%d/%d)';
  LABEL_STATUS_DETAIL_WRITE_ZERO           = 'ゼロ(NULL)を書き込み、完全削除を行っています...(Step:%d/%d)';
  LABEL_STATUS_DETAIL_FAILED               = '削除に失敗しました。';
  LABEL_STATUS_DETAIL_STOPPED              = '削除が中止されました。';
  LABEL_STATUS_DETAIL_COMPLETE             = '削除が正常に完了しました。';



implementation

initialization

	//Delphiコンパイラによる最適化を防ぐ

	MSG_CAPTION_ERROR;
	MSG_CAPTION_CONFIRMATION;

	MSG_ERROR_NOT_FILE_OPEN;
	MSG_ERROR_GO_TO_TRASH_FAILED;

	LABEL_STATUS_TITLE_LISTING;
	LABEL_STATUS_TITLE_DELETING;
	LABEL_STATUS_TITLE_COMPLETE_DELETING;
	LABEL_STATUS_TITLE_GO_TO_TRASH;
	LABEL_STATUS_TITLE_ERROR;
	LABEL_STATUS_TITLE_USER_CANCEL;
	LABEL_STATUS_TITLE_COMPLETE;

	LABEL_STATUS_DETAIL_PREPARING;
	LABEL_STATUS_DETAIL_WRITE_RAND;
	LABEL_STATUS_DETAIL_WRITE_ZERO;
	LABEL_STATUS_DETAIL_FAILED;
	LABEL_STATUS_DETAIL_STOPPED;
	LABEL_STATUS_DETAIL_COMPLETE;




end.

