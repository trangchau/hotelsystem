/*
	テキストファイルアクセス関数定義
*/

#ifndef Txt_FilH
#define Txt_FilH

#define	TXT_RED		1
#define	TXT_WRT		2
#define	TXT_APD		3
#define	TOP_ERR		((HANDLE)(LONG_PTR)0xffffffff)

int		Txt_Red( WCHAR *, int, HANDLE );	// １行読み込み
int		Txt_Wrt( WCHAR *, int, HANDLE );	// １行書き込み
HANDLE	Txt_Opn( WCHAR *, int );			// ファイルオープン
void	Txt_Cls( HANDLE );					// ファイルクローズ

#endif
