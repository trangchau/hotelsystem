/*
	アプリケーション二重起動禁止処理定義
*/

#ifndef	App_DupH
#define	App_DupH

HANDLE		Etr_Exc( HWND, char * );			// 二重起動防止
void		Lev_Exc( HWND, char *, HANDLE );	// アプリケーション終了処理

#endif
