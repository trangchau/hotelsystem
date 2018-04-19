// CommaEdit.cpp : 実装ファイル
//

#include <afxwin.h>
#include <math.h>
#include "CommaEdit.h"

#include "..\Common\bsc_def.h"
#include "..\Common\dte_mng.h"

#define		wsizeof(x)	((x)*sizeof(WCHAR))


// CCommaEdit
#define ON_WM_SETTEXT() \
	{ WM_SETTEXT, 0, 0, 0, AfxSig_lwl, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< LRESULT (AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM) > \
		( &ThisClass :: OnSetText)) },

IMPLEMENT_DYNAMIC(CCommaEdit, CCustomEdit)

CCommaEdit::CCommaEdit()
{
	DecimalPlacess = 0;
	RoundMethod = CEDT_ROUND_UP;
}

CCommaEdit::~CCommaEdit()
{
}

BEGIN_MESSAGE_MAP(CCommaEdit, CCustomEdit)
	ON_WM_KILLFOCUS()
	ON_WM_RBUTTONUP()
	ON_WM_SETFOCUS()
	ON_WM_CHAR()
//	ON_WM_SETTEXT()
END_MESSAGE_MAP()



// CCommaEdit メッセージ ハンドラ

#define	is_delimitter(x,y)		((x)==_T(',')||((y)>0&&(x)==_T('.')))

static	int	is_numeric( WCHAR *str )
{
	while ( *str ){
		if ( *str<_T('0') || *str>_T('9') )	break;
		str++;
	}
	return( *str==0 );
}

/*
static	WCHAR	*cma_edt(				// コンマ編集
WCHAR			*src,					// 元データ(Only numeric character)
int				dps,					// 小数点以下桁数
int				rmt )					// 丸め方式
{
	int			siz, cct, cma, idx, pfx;
	WCHAR		*rsl, *spt, *dpt, buf[40];
	double		val;

	val = _wtof(src);
	if ( rmt==CEDT_ROUND_TRUNCATION ){
		val = floor( val * powf(10.0,(float)dps) ) / powf(10.0,(float)dps);
	}
	swprintf_s( buf, memcnt(buf), _T("%.*f"), dps, val );

	pfx = dps>0 ? dps+1: 0;
	siz = (int)wcslen( buf );
	cct = ( (siz-pfx) / 3 - 1 ) + ( ( (siz-pfx) % 3 ) ? 1:0 );
	if ( !!( rsl = (WCHAR *)malloc( wsizeof(siz+cct+1) ) ) ){
		dpt = rsl + siz + cct;
		*dpt-- = 0;
		spt = buf + siz - 1;
		while( pfx-- )	*dpt-- = *spt--, siz--;
		for ( idx=0, cma=0; idx<siz; idx++ ){
			*dpt-- = *spt--;
			if ( cct && ++cma==3 ){
				*dpt-- = _T(',');
				cma = 0;
				cct--;
			}
		}
	}

	return( rsl );
}
*/

void CCommaEdit::OnKillFocus(CWnd* pNewWnd)
{
	ModifyStyle(ES_LEFT,ES_RIGHT);

	WCHAR		src[128], *rsl;

	GetWindowText( src, memcnt(src) );
	if ( !!wcslen( src ) ){
		if ( rsl = Cma_Edt( _wtof(src), DecimalPlacess ) ){
			CCustomEdit::SetWindowText( rsl );
			free( rsl );
		}
	}

	CCustomEdit::OnKillFocus(pNewWnd);

}

void CCommaEdit::OnRButtonUp(UINT nFlags, CPoint point)
{
	//


	CCustomEdit::OnRButtonUp(nFlags, point);
}

void CCommaEdit::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
{
	//SetLimitText( 10 );

	CCustomEdit::CalcWindowRect(lpClientRect, nAdjustType);
}

void CCommaEdit::OnSetFocus(CWnd* pOldWnd)
{
	//SetLimitText( 10 );
	ModifyStyle(ES_RIGHT,ES_LEFT);

	CCustomEdit::OnSetFocus(pOldWnd);

	WCHAR			buf[128];
	GetWindowText( buf, memcnt(buf) );
	CCustomEdit::SetWindowText( Prg_Cma(buf) );
	SetSel( 0, -1 );


	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CCommaEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ( ( nChar>=_T('0') && nChar<=_T('9') ) || is_delimitter(nChar,DecimalPlacess) || nChar==VK_DELETE || nChar==VK_BACK ){
		CCustomEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else{
		nChar = 0;
	}
}

void	CCommaEdit::SetValue( double dValue, int bNull )
{
	WCHAR			*buf=NULL;

	ModifyStyle(ES_LEFT,ES_RIGHT);
	if ( !!bNull || !!( buf = Cma_Edt( dValue, DecimalPlacess ) ) ){
		SetWindowText( bNull ? _T(""): buf );
		if ( buf )	free( buf );
	}
}


int		CCommaEdit::GetValue( double *dValue )
{
	WCHAR			buf[128];

	GetWindowText( buf, memcnt(buf) );
	Prg_Cma(buf);

	*dValue = _wtof( buf );

	return( !wcslen(buf) );		// null status
}

/*
LRESULT CCommaEdit::OnSetText( WPARAM wParam, LPARAM lParam )
{
	WCHAR			*buf;

	if ( buf = cma_edt( (WCHAR *)lParam, DecimalPlacess, RoundMethod ) ){
//		CCustomEdit::SetWindowTextW( buf );

		DefWindowProc( WM_SETTEXT, wParam, (LPARAM)buf );
		free( buf );
	}

	return( 0 );
}
*/