/*
	バイナリ－ファイルアクセス関数定義
*/
#ifndef Bin_FilH
#define Bin_FilH

#define	BIN_RED		1
#define	BIN_WRT		2
#define	BIN_ERR		-1
#define	BOP_ERR		((HANDLE)(LONG_PTR)0xffffffff)

int		Bin_Red( void *, int, HANDLE );		// ブロック読み込み
int		Bin_Wrt( void *, int, HANDLE );		// ブロック書き込み
HANDLE	Bin_Opn( WCHAR *, int );			// ファイルオープン
void	Bin_Cls( HANDLE );					// ファイルクローズ

#endif
