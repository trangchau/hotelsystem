/*
	アプリケーション二重起動禁止処理
*/

#include <windows.h>

static	HANDLE	app_dup(		// アプリケーション二重起動検査
LPCWSTR		unq )				// プログラム識別名
{
	HANDLE		mtx;
	HWND		wnd;

	mtx = CreateMutex( NULL, TRUE, unq );

	if ( GetLastError() == ERROR_ALREADY_EXISTS ){
		wnd = GetDesktopWindow();
		if ( wnd )	wnd = GetWindow( wnd, GW_CHILD );
		while( wnd ){
			if ( ::GetProp( wnd, unq ) ){
				if ( IsIconic( wnd ) ){ 
					ShowWindow( wnd, SW_RESTORE );
				}
				SetForegroundWindow( wnd );
				if ( wnd = GetLastActivePopup( wnd ) ){
					SetForegroundWindow( wnd );
				}
				break;
			}
			wnd = GetWindow( wnd, GW_HWNDNEXT );
		}
		mtx = NULL;
	}
  
	return( mtx );
}

HANDLE		Etr_Exc(				// 二重起動防止
HWND		hWnd,					// メインウィンドウハンドル
LPCWSTR		unq )					// 識別名称
{
	HANDLE		mtx;

	if ( !!( mtx = app_dup( unq ) ) ){
		::SetProp( hWnd, unq, (HANDLE)1 );
	}
	return( mtx );
}

void		Lev_Exc(				// アプリケーション終了処理
HWND		hWnd,					// メインウィンドウハンドル
LPCWSTR		unq,					// 識別名称
HANDLE		mtx )					// ミューテックスハンドル
{
	RemoveProp( hWnd, unq );
	::CloseHandle( mtx );
}
