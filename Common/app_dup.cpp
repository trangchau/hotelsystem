/*
	�A�v���P�[�V������d�N���֎~����
*/

#include <windows.h>

static	HANDLE	app_dup(		// �A�v���P�[�V������d�N������
LPCWSTR		unq )				// �v���O�������ʖ�
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

HANDLE		Etr_Exc(				// ��d�N���h�~
HWND		hWnd,					// ���C���E�B���h�E�n���h��
LPCWSTR		unq )					// ���ʖ���
{
	HANDLE		mtx;

	if ( !!( mtx = app_dup( unq ) ) ){
		::SetProp( hWnd, unq, (HANDLE)1 );
	}
	return( mtx );
}

void		Lev_Exc(				// �A�v���P�[�V�����I������
HWND		hWnd,					// ���C���E�B���h�E�n���h��
LPCWSTR		unq,					// ���ʖ���
HANDLE		mtx )					// �~���[�e�b�N�X�n���h��
{
	RemoveProp( hWnd, unq );
	::CloseHandle( mtx );
}
