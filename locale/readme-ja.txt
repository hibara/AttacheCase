﻿多言語（国際化）対応について
===================================

以下のディレクトリに分けたファイルを翻訳していただけると、各言語に対応することが可能です。  

現状「ja」というディレクトリに日本語ファイルが入っています。  

*.pas  

の拡張子がついたファイルをテキストファイルなどで開いていただければ、そこにズラッと文字列変数が並んでいるのが確認できます。  

resourcestring宣言で、各メッセージ文字列の宣言を行って、initialization宣言で、前のresourcestringでDelphiによる最適化を避けるため、列挙した変数名をここにリストアップします。  

ただ、これを手動でやるには、なかなか面倒なので、自前で簡易ツールも用意してあります。  

詳しくはこちらのディレクトリを参照ください。  

AttacheCaseProject\tools\init_lang  

同様な構造で（つまりは、jaディレクトリのように）、別言語（たとえばenとか作って）を構築していただければ、多言語でアタッシェケースをビルドすることができます。ただし、当たり前ですが、ビルドには、C++Builder2010以降のバージョンが必要です。  

もしもこれらのメッセージ文字列を翻訳していただける方がいたら、  

http://hibara.org/  

までご一報いただけると嬉しいです。  

その際には、ヘルプファイルも翻訳していただけると助かります。  
けっこうな量なので、大変かもしれませが・・・（^^;  


以上

