/*
	ＷＩＮ３２ 印刷 ルーチン
	印刷用紙情報取得関数ヘッダーファイル
	Copyright(C) Arsnova Corp. 2003
*/
#ifndef ppr_infH
#define ppr_infH

int Get_Ppr_Nbr( short *nbr, WCHAR *, WCHAR *ppr );	// 用紙名から用紙番号取得
int Get_Ppr_Nam( WCHAR *, int, short, WCHAR * );	// 用紙番号から用紙名取得
int Get_Ppr_Siz( SIZEL	*, WCHAR	*, WCHAR	* );// 用紙サイズ取得

#endif
