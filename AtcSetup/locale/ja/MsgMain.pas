unit MsgMain;

interface

resourcestring


	SYSTEM_CONTEXT_MENU_DECYPTION             = 'アタッシェケースファイルを復号する';

  MSG_ERROR_DO_NOT_READ_REGISTRY            = '理由はわかりませんが、レジストリからの読み込みに失敗しました。';
  MSG_ERROR_THIS_APP_DOES_NOT_EXECUTE_ALONE = 'このプログラムは単体では動作しません。'+#13+
                                              'アタッシェケースからファイルの関連付け設定で呼び出されるときのみ使用されます。';



implementation

initialization

	//Delphiコンパイラによる最適化を防ぐ
	SYSTEM_CONTEXT_MENU_DECYPTION;
	MSG_ERROR_DO_NOT_READ_REGISTRY;
	MSG_ERROR_THIS_APP_DOES_NOT_EXECUTE_ALONE;




end.

