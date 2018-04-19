/*
	Graphic object make and displaying window
*/

#include <afx.h>
#include "fig_mnp.h"
#include "fig_drw.h"
#include "fig_ope.h"
#include "fig_win.h"
#include "rbr_bnd.h"

#define	CLS_NAM		_T("Figure_Manipuration_Class_")
#define	WM_UPAINT	WM_USER+1024

static	void	cal_dif_(			// 始点との差を計算
Fig_mng		*mng,					// 図形管理構造
Fig_elm		*elm )					// 対象図形
{
	Dpn			dvc[2];

	switch( elm->typ ){
	case ELM_LIN:	Dpn_Cnv( &dvc[0], elm->elm.lin.sp, 1, mng->s2d );
					Dpn_Cnv( &dvc[1], elm->elm.lin.ep, 1, mng->s2d );
					break;
	case ELM_RCT:	Dpn_Cnv( &dvc[0], elm->elm.rct.sp, 1, mng->s2d );
					Dpn_Cnv( &dvc[1], elm->elm.rct.ep, 1, mng->s2d );
					break;
	}
	mng->dif.sp.x = dvc[0].x - (double)mng->mss.x;
	mng->dif.sp.y = dvc[0].y - (double)mng->mss.y;
	mng->dif.ep.x = dvc[1].x - (double)mng->mss.x;
	mng->dif.ep.y = dvc[1].y - (double)mng->mss.y;
}

static	int		sta_ope_(			// 図形操作処理開始
HWND	wnd,						// ウィンドウハンドル
UINT	msg,						// ウィンドウメッセージ
UINT	wpr,						// パラメータ（ＷＯＲＤ）
LONG	lpr )						// パラメータ（ＬＯＮＧ）
{
	Fig_mng			*mng;
	RECT			rct;

	mng = (Fig_mng *)(LONG_PTR)GetWindowLong( wnd, GWL_USERDATA );
	if ( mng ){
		mng->mss.x = LOWORD(lpr);
		mng->mss.y = HIWORD(lpr);
		if ( mng->sts )	mng->hit = mng->fnd;
		else			mng->hit = Hit_Tst( mng );
		if ( mng->hit )	cal_dif_( mng, mng->hit );
		mng->drg = DRG_STT;
		mng->dtl = mng->sts;
		mng->dir = mng->dst;
		GetWindowRect( wnd, &rct );
		SetCapture( wnd );
		ClipCursor( &rct );
	}
	return( !!mng );
}

static	int		mov_ope_(			// 選択範囲移動
HWND	wnd,						// ウィンドウハンドル
UINT	msg,						// ウィンドウメッセージ
UINT	wpr,						// パラメータ（ＷＯＲＤ）
LONG	lpr )						// パラメータ（ＬＯＮＧ）
{
	Fig_mng			*mng = (Fig_mng *)(LONG_PTR)GetWindowLong( wnd, GWL_USERDATA );
	HDC				hdc;
	double			dtc, dtx, dty, sqrt( double );
	int				chk = TRUE;

	if ( msg == WM_MOUSEMOVE ){
		Set_Ope( mng, LOWORD(lpr), HIWORD(lpr), wpr );
	}

	if ( mng && mng->drg ){
		hdc = GetDC( wnd );
		if ( mng->rbr )		Rbr_Bnd( hdc, mng );
		mng->mse.x = LOWORD(lpr);
		mng->mse.y = HIWORD(lpr);
		if ( mng->mse.x > mng->dvc.wid )	mng->mse.x = 0;
		if ( mng->mse.y > mng->dvc.hit )	mng->mse.y = 0;
		dtx = (double)( mng->mss.x - mng->mse.x );
		dty = (double)( mng->mss.y - mng->mse.y );
		dtc = sqrt( dtx*dtx + dty*dty );
		if ( msg == WM_MOUSEMOVE ){
			if ( dtc > 3.0 ){
				if ( mng->drg == DRG_STT ){
					if ( ( chk = Ope_Stt( mng ) ) == TRUE ){
						mng->drg = DRG_ING;
					}
				}
				if ( chk ){
					Rbr_Bnd( hdc, mng );
					mng->rbr = TRUE;
				}
			}
		}
		else{
			Ope_End( dtc, mng );
			chk = FALSE;
		}
		if ( !chk ){
			mng->rbr = FALSE;
			mng->drg = DRG_NON;
			ReleaseCapture();
			ClipCursor( NULL );
		}
		ReleaseDC( wnd, hdc );
	}
	return( !!mng );
}

static	int		rdn_ope_(			// 選択範囲移動
HWND	wnd,						// ウィンドウハンドル
UINT	msg,						// ウィンドウメッセージ
UINT	wpr,						// パラメータ（ＷＯＲＤ）
LONG	lpr )						// パラメータ（ＬＯＮＧ）
{
	Fig_mng			*mng;
	Dpn				wld[1], dvc[1];
	int				chk;

	mng = (Fig_mng *)(LONG_PTR)GetWindowLong( wnd, GWL_USERDATA );
	if ( chk = !!mng ){
		mng->mss.x = LOWORD(lpr);
		mng->mss.y = HIWORD(lpr);
		if ( !!( mng->rht = Hit_Tst( mng ) ) ){
		//	cal_dif_( mng, mng->rht );
		}
		dvc->x = LOWORD(lpr);
		dvc->y = HIWORD(lpr);
		Dpn_Cnv( wld, dvc, 1, mng->s2d );
		chk = mng->ocb( EVT_RDN, (int)dvc->x, (int)dvc->y, wld, NULL, NULL, mng->usr );
	}
	return( !!mng );
}

static	LONG APIENTRY	win_prc_(	// ウィンドウプロシージャ
HWND	wnd,						// ウィンドウハンドル
UINT	msg,						// ウィンドウメッセージ
UINT	wpr,						// パラメータ（ＷＯＲＤ）
LONG	lpr )						// パラメータ（ＬＯＮＧ）
{
	LONG			ret;
	PAINTSTRUCT		ps;
	HDC				hdc;

	switch( msg ){
	case WM_CREATE:		PostMessage( wnd, WM_UPAINT, 0, 0 );
						ret = 0;
						break;
	case WM_UPAINT:		hdc = GetDC( wnd );
						Fig_Drw( hdc, wnd );
						//drw_all_( hdc, wnd );
						ReleaseDC( wnd, hdc );
						ret = 0;
						break;
	case WM_PAINT:		hdc = BeginPaint( wnd, &ps );
						Fig_Drw( hdc, wnd );
						//drw_all_( hdc, wnd );
						ret = 0;
						break;
	case WM_LBUTTONDOWN:sta_ope_( wnd, msg, wpr, lpr );
						ret = 0;
						break;
	case WM_RBUTTONDOWN:rdn_ope_( wnd, msg, wpr, lpr );
						ret = 0;
						break;
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:	mov_ope_( wnd, msg, wpr, lpr );
						ret = 0;
						break;
	default:			ret = (LONG)(LONG_PTR)DefWindowProc( wnd, msg, wpr, lpr );
						break;
	}
	return( ret );
}

static	int		reg_cls_(			// ウィンドウクラスの登録
HINSTANCE		ins )				// プロセスインスタンス
{
	WNDCLASS		wcl;
	int				chk;

	if ( ( chk = GetClassInfo( ins, CLS_NAM, &wcl ) ) == FALSE ){
		wcl.style         = CS_HREDRAW | CS_VREDRAW;
		wcl.lpfnWndProc   = win_prc_;
		wcl.cbClsExtra    = wcl.cbWndExtra = 0;
		wcl.hInstance     = ins;
		wcl.hIcon         = LoadIcon( NULL, IDI_APPLICATION );
		wcl.hCursor       = LoadCursor( NULL,IDC_ARROW );
		wcl.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
		wcl.lpszMenuName  = NULL;
		wcl.lpszClassName = CLS_NAM;

		chk = RegisterClass( &wcl );
	}
	return( chk );
}

static	HWND	cre_win_(			// ウィンド生成
HINSTANCE	ins,					// 親アプリケーションインスタンス
Fig_mng		*mng )					// 描画管理構造
{
	HWND		wnd;
	
	mng->ins = ins;
	if ( reg_cls_( ins ) ){
		wnd = CreateWindow( CLS_NAM, _T("NoName"), WS_CHILD|WS_BORDER,
				(int)mng->dvc.lft, (int)mng->dvc.top, (int)mng->dvc.wid, (int)mng->dvc.hit,
												mng->par, NULL, ins, NULL );
	}

	return ( wnd );
}

HWND	Crt_Win(					// 描画ウィンドウ生成
HINSTANCE	ins,					// 親アプリケーションインスタンス
Fig_mng		*mng )					// 描画管理構造
{
	HWND			wnd;

	if ( ( wnd = cre_win_( ins, mng ) ) != NULL ){
		mng->wnd = wnd;
		SetWindowLong( wnd, GWL_USERDATA, (LONG)(LONG_PTR)mng );
		ShowWindow( wnd, SW_SHOW );
		UpdateWindow( wnd );
	}

	return ( wnd );
}

