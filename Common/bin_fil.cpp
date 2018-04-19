#include <stdio.h>
#include <windows.h>

#include "Bin_Fil.h"

int			Bin_Red(					// ブロック読み込み
void		*buf,			   			// 読み込みバッファ
int			siz,			   			// バッファサイズ
HANDLE		fil )						// ファイルハンドル
{
	ULONG			len;
	int				chk, err;

	chk = ReadFile( fil, buf, siz, &len, NULL );
	if ( !chk )	err = GetLastError();
	return( ( chk && ( !len || (ULONG)siz==len ) ) ? len: BIN_ERR );
}

int			Bin_Wrt(					// ブロック書き込み
void		*buf,			   			// 書き込みバッファ
int			siz,			   			// バッファサイズ
HANDLE		fil )						// ファイルハンドル
{
	ULONG			len;
	int				chk;

	chk = WriteFile( fil, buf, siz, &len, NULL );
	return( ( chk && (ULONG)siz==len ) ? len: BIN_ERR );
}

HANDLE		Bin_Opn(					// ファイルオープン
WCHAR	*fnm,				   			// ファイル名
int		mod )				   			// オープンモード
{
	HANDLE				fil;
	int					opn, cre;

	opn = mod == BIN_RED ? GENERIC_READ: GENERIC_WRITE;
	cre = mod == BIN_RED ? OPEN_EXISTING: CREATE_ALWAYS;

	fil = CreateFile( fnm, opn, 0, NULL, cre, FILE_ATTRIBUTE_ARCHIVE, NULL );
	return( fil );
}

void		Bin_Cls(					// ファイルクローズ
HANDLE		fil )						// ファイルハンドル
{
	CloseHandle( fil );
}
