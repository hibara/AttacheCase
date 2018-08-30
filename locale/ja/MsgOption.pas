unit MsgOption;

interface

resourcestring


	APPLICATION_NAME	        = 'アタッシェケース';

	MSG_CAPTION_CONFIRMATION  = '確認';

  MSG_CONFIRM_OPEN_INI_FILE = '以下にある設定ファイル（_AtcCase.ini）を読み込もうとしています。'+#13+
                              '覚えのない設定ファイルを読み込むとセキュリティリスクが生じる可能性があります。'+#13+
                              'それでも読み込みますか？';




implementation

initialization

	//Delphiコンパイラによる最適化を防ぐ

	APPLICATION_NAME;
  MSG_CAPTION_CONFIRMATION;
  MSG_CONFIRM_OPEN_INI_FILE;



end.

