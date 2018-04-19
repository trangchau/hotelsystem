/*
	部品管理システム共通関数 定義
*/

#ifndef	tri_cmnH
#define	tri_cmnH

int		Axn_Chk( char * );				// 車軸管理番号正当性チェック
int		Whl_Chk( char * );				// 車輪管理番号正当性チェック
int		Bkd_Chk( char * );				// ブレーキディスク管理番号正当性チェック
int		Brg_Chk( char * );				// 内輪管理番号正当性チェック
int		Tri_S2D( char *, int );			// 入力した日付文字列を正規変換
void	edt_dte( char *, int, int );	// 日付データを可視データへ変換

#endif
