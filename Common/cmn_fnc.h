/*
	各種雑多関数群
*/

#ifndef	cmn_fncH
#define	cmn_fncH

#include "rng_buf.h"

#define	MKD_WRN						1		// 警告メッセージ
#define	MKD_FTL						2		// エラーメッセージ
#define	MKD_INF						3		// 案内メッセージ

#define	CEDT_ROUND_UP				1
#define	CEDT_ROUND_TRUNCATION		2

//void	Get_Vsn( HWND, GAZS_MNG * );								// プログラムバージョンの取得
void	DBG_OUT( char *, int, char *, ... );						// デバッグ出力
int		Fil_Exs( char * );											// ファイル存在検定
void	WStr_NCpy( WCHAR *, WCHAR *, int );							// 最大サイズ指定の文字列複写
WCHAR	*Add_Str( WCHAR *, WCHAR * );								// 文字列追加
int		xtoi( char * );												// １６進文字列をバイナリー変換
WORD	Bin_BCD( int );												// バイナリ→BCD変換
int		BCD_Bin( WORD );											// BCD→バイナリ変換
WCHAR	*trim( WCHAR * );											// 文字列トリミング
void	Get_Dir( char * );											// ディレクトリ部分のみ取り出し
int		Str_UCmp( char *, char * );									// 文字列の大文字小文字無視比較
double	Rnd_Dec( double, int, int );								// rounding to ordering digit
WCHAR	*Cma_Edt( double, int );									// コンマ編集
WCHAR	*Prg_Cma( WCHAR *);											// コンマを削除

#endif
