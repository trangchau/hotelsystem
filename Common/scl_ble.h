/*
		データ暗号化・復号化関数定義
*/
#ifndef	Scl_BleH
#define	Scl_BleH

int		Ecd_Siz( int );										// 暗号化データサイズ計算
void	Ecd_Dat( unsigned long *, void *, int, char * );	// データ暗号化
void	Dcd_Dat( void *, unsigned long *, int, char * );	// データ復号化
int		Pop_Str( WCHAR *, WCHAR *);							// pop from stack
int		Psh_Str( WCHAR *, int, WCHAR *);					// push to stack

#endif
