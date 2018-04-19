// SimpleGrid.cpp : 実装ファイル
//

#include "afxwin.h"
#include "SimpleGrid.h"

#define	MAX_DGT		256			// １カラム最大文字数
#define	CBX_SIZ		14			// チェックボックスのサイズ
#define	PAG_UNT		4			// １ページスクロール単位

#define	EDF_EDT		0x1000		// エディットボックス表示中
#define	EDF_CMB		0x2000		// コンボボックス表示中

#define		wsizeof(x)	((x)*sizeof(WCHAR))
#define		memcnt(x)	((sizeof(x))/(sizeof(x[0])))	// 配列個数取得

// CSimpleGrid

#define		wsizeof(x)	((x)*sizeof(WCHAR))

IMPLEMENT_DYNAMIC(CSimpleGrid, CWnd)

CSimpleGrid::CSimpleGrid( /*RECT *rct, CWnd *pParent*/ )
{
	memset( m_Grd_def, 0, sizeof(m_Grd_def) );
	m_Font = NULL;
	m_Str_tbl = NULL;
	m_Val_tbl = NULL;
	m_Enb_tbl = NULL;
	m_Fcl_tbl = NULL;
	m_Bcl_tbl = NULL;
	m_LineCount = 0;
	m_TopRow = 1;
	m_LeftColumn = 0;
	m_VScroll = m_HScroll = FALSE;
	m_SelRow = m_SelColumn = -1;
	m_EditBox = NULL;
	m_DateEditBox = NULL;
	m_TimeEditBox = NULL;
	m_CommaEditBox = NULL;
	m_ComboBox = NULL;
	m_InputEnable = FALSE;
	m_Fst_flg = TRUE;
	m_EditFlg = 0;
	m_InputChar = 0;
}

CSimpleGrid::~CSimpleGrid()
{
	if ( m_EditBox )		delete m_EditBox;
	if ( m_DateEditBox )	delete m_DateEditBox;
	if ( m_TimeEditBox )	delete m_TimeEditBox;
	if ( m_CommaEditBox )	delete m_CommaEditBox;
	if ( m_ComboBox )		delete m_ComboBox;
}


BEGIN_MESSAGE_MAP(CSimpleGrid, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_VSCROLL()
	ON_WM_RBUTTONUP()
	ON_WM_EDITNOTIFY()
	ON_WM_COMBONOTIFY()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// ローカル関数郡
static	WORD	Chk_mrk[] = {
	0xff, 0xfe, 0xfc, 0x38, 0x11, 0x83, 0xc7, 0xef 
};


static	void	drw_cbx( HDC hdc, RECT *rct, int chk )
{
	int			top, lft, tcl;
	HDC			src;
	HBRUSH		brs;
	HBITMAP		bmp;
	HGDIOBJ		obm, obs;

	brs = ::CreateSolidBrush( RGB(255,255,255) );
	obs = ::SelectObject( hdc, brs );
	top = rct->top + ( ( rct->bottom - rct->top + 1 ) - CBX_SIZ ) / 2;
	lft = rct->left + 4;
	Rectangle( hdc, lft, top, lft+CBX_SIZ, top+CBX_SIZ );
	if ( chk ){
		src = ::CreateCompatibleDC( hdc );
		bmp = ::CreateBitmap( 8, 8, 1, 1, Chk_mrk );
		obm = (HBITMAP)::SelectObject( src, bmp );
		tcl = ::SetTextColor( hdc, RGB(0,0,0) );
		::BitBlt( hdc, lft+3, top+3, CBX_SIZ-2, CBX_SIZ-2, src, 0, 0,SRCCOPY );
		::SelectObject( src, obm );
		::DeleteObject( bmp );
		::DeleteDC( src );
		::SetTextColor( hdc, tcl );
	}
	::SelectObject( hdc, obs );
	::DeleteObject( brs );
}

// CSimpleGrid オペレーション
// 非公開オペレーション
void CSimpleGrid::CalcCellRect( RECT *rct, int clm, int row )
{
	int			idx, lft=0, top=0, mgn=m_Grd_def->hdr ? 1: 0;

	memset( rct, 0, sizeof(rct) );

	if ( clm < m_Grd_def->cct && row < m_LineCount ){
		rct->top = ( m_Grd_def->lnh - 1 ) * ( row - m_TopRow + mgn );
		rct->bottom = rct->top + m_Grd_def->lnh;
		for ( idx=0; idx<m_LeftColumn; idx++ ){
			lft -= (m_Grd_def->clm+idx)->wid - 1;
		}
		if ( clm != -1 ){
			for ( idx=0; idx<clm; idx++ ){
				rct->left += (m_Grd_def->clm+idx)->wid - 1;
			}
			rct->left += lft;
			rct->right = rct->left + (m_Grd_def->clm+idx)->wid;
		}
		else{
			rct->left = rct->right = 0;
			for ( idx=0; idx<m_Grd_def->cct; idx++ ){
				rct->right += (m_Grd_def->clm+idx)->wid - 1;
			}
			rct->left += lft;
			rct->right += lft;
		}
	}
}

int CSimpleGrid::MPointToCell( int *clm, int *row, int msx, int msy )
{
	RECT		rct[1];
	int			hit=FALSE, idx, x;

	GetClientRect( rct );

	hit  = msx>=rct->left && msx<=rct->right;
	hit &= msy>=rct->top && msy<=rct->bottom;

	if ( hit ){
//		*row = msy / m_Grd_def->lnh + ( ( msy % m_Grd_def->lnh ) ? 1: 0 );
		*row = msy / ( m_Grd_def->lnh - 1 );
		for ( idx=x=m_LeftColumn; idx<m_Grd_def->cct; idx++ ){
			if ( msx>=x && msx<=x+(m_Grd_def->clm+idx)->wid-1 ){
//				*clm = idx;
				*clm = idx - m_LeftColumn;
				break;
			}
			x += (m_Grd_def->clm+idx)->wid-1;
		}
		hit = idx<m_Grd_def->cct;
	}
	return( hit );
}

LRESULT CSimpleGrid::SendNotify( int ncd, int msx, int msy, int clm, int row, LPARAM lpr )
{
	GRD_NTF			*ntf;
	WINDOWPLACEMENT	wpl[1];
	LRESULT			rsl=-1;

	GetWindowPlacement( wpl );

	ntf = (GRD_NTF *)calloc( 1, sizeof(GRD_NTF) );
	if ( ntf ){
		ntf->ntf = ncd;
		ntf->cid = GetWindowLong( m_hWnd, GWL_ID );
		ntf->msx = wpl->rcNormalPosition.left + msx;
		ntf->msy = wpl->rcNormalPosition.top + msy;
		ntf->clm = clm;
		ntf->row = row;
		ntf->grd = this;
		rsl = ::SendMessage( m_pParent->m_hWnd, WM_GRIDNOTIFY, (WPARAM)ntf, lpr );
//		rsl = ::PostMessage( m_pParent->m_hWnd, WM_GRIDNOTIFY, (WPARAM)ntf, lpr );
	}
	return( rsl );
}

LRESULT	CSimpleGrid::PostNotify( int ncd, int msx, int msy, int clm, int row, LPARAM lpr )
{
	GRD_NTF			*ntf;
	WINDOWPLACEMENT	wpl[1];
	LRESULT			rsl=-1;

	GetWindowPlacement( wpl );

	ntf = (GRD_NTF *)calloc( 1, sizeof(GRD_NTF) );
	if ( ntf ){
		ntf->ntf = ncd;
		ntf->cid = GetWindowLong( m_hWnd, GWL_ID );
		ntf->msx = wpl->rcNormalPosition.left + msx;
		ntf->msy = wpl->rcNormalPosition.top + msy;
		ntf->clm = clm;
		ntf->row = row;
		ntf->grd = this;
		rsl = ::PostMessage( m_pParent->m_hWnd, WM_GRIDNOTIFY, (WPARAM)ntf, lpr );
	}
	return( rsl );
}

void CSimpleGrid::SetVScrollBar()
{
	RECT			rct[1];
	int				vsc;
	WINDOWPLACEMENT	wpc[1];
	SCROLLINFO		sci[1];

	vsc = m_LineCount > m_Grd_def->vlc;
	if ( vsc != m_VScroll ){
		m_VScroll = vsc;
		GetWindowPlacement( wpc );
		*rct = wpc->rcNormalPosition;
		if ( vsc ){
			ModifyStyle( 0, WS_VSCROLL, TRUE );
			ShowScrollBar( SB_VERT, TRUE );
			sci->cbSize = sizeof(sci);
			sci->fMask=SIF_PAGE|SIF_POS|SIF_RANGE;
			GetScrollInfo( SB_VERT, sci );
			sci->nMin = m_Grd_def->hdr ? 1: 0;
			sci->nMax = m_LineCount - ( m_Grd_def->hdr ? 0: 1 );
			sci->nPage = m_Grd_def->vlc;
			sci->nPos = m_Grd_def->hdr ? 1: 0;
			SetScrollInfo( SB_VERT, sci );
			rct->right += GetSystemMetrics(SM_CYHSCROLL);
		}
		else{
			ModifyStyle( WS_VSCROLL, 0, TRUE );
			rct->right -= GetSystemMetrics(SM_CYHSCROLL);
		}
		MoveWindow( rct, TRUE );
	}
}

void CSimpleGrid::SetHScrollBar()
{
	RECT			rct[1];
	int				cct=0, idx, hsc, wid, rwd;
	WINDOWPLACEMENT	wpc[1];
	SCROLLINFO		sci[1];

	GetClientRect( rct );
	wid = rct->right - rct->left;
	for ( idx=rwd=0; idx<m_Grd_def->cct; idx++ ){
		rwd += (m_Grd_def->clm+idx)->wid - 1;
//		if ( (rwd>wid) && !cct)	cct = idx+1;
	}
	rwd += /*GetSystemMetrics( SM_CYEDGE ) * 2*/ + 1;

	hsc = rwd > wid;
	if ( hsc != m_HScroll ){
		m_HScroll = hsc;
		GetWindowPlacement( wpc );
		*rct = wpc->rcNormalPosition;
		if ( hsc ){
			ModifyStyle( 0, WS_HSCROLL, TRUE );
			//ShowScrollBar( SB_HORZ, TRUE );
			sci->cbSize = sizeof(sci);
			sci->fMask=SIF_PAGE|SIF_POS|SIF_RANGE;
			GetScrollInfo( SB_HORZ, sci );
			sci->nMin = 0;
			sci->nMax = m_Grd_def->cct + ( m_Grd_def->cct % m_Grd_def->vcc );
//			sci->nPage = cct;
			sci->nPage = m_Grd_def->vcc;
			sci->nPos = 0;
			SetScrollInfo( SB_HORZ, sci );
			rct->bottom += GetSystemMetrics(SM_CXVSCROLL);
		}
		else{
			ModifyStyle( WS_HSCROLL, 0, TRUE );
			rct->bottom -= GetSystemMetrics(SM_CXVSCROLL);
		}
		MoveWindow( rct, TRUE );
		if ( hsc )	ShowScrollBar( SB_HORZ, TRUE );
	}
}

// 公開オペレーション
void CSimpleGrid::SetCellText( int clm, int row, WCHAR *dat )
{
	WCHAR		*ptr;
	RECT		rct[1];

	if ( m_Str_tbl ){
		if ( clm < m_Grd_def->cct && row < m_LineCount ){
			ptr = m_Str_tbl + MAX_DGT * m_Grd_def->cct * row + MAX_DGT * clm; 
			wcsncpy_s( ptr, MAX_DGT, dat, MAX_DGT );
			CalcCellRect( rct, clm, row );
			InvalidateRect( rct, FALSE );
		}
	}
}

void CSimpleGrid::GetCellText( WCHAR *dst, int siz, int clm, int row )
{
	WCHAR		*ptr;

	if ( m_Str_tbl ){
		if ( clm < m_Grd_def->cct && row < m_LineCount ){
			ptr = m_Str_tbl + MAX_DGT * m_Grd_def->cct * row + MAX_DGT * clm; 
			wcsncpy_s( dst, siz, ptr, siz );
		}
	}
}

void CSimpleGrid::SetCellColor( int clm, int row, COLORREF fcl, COLORREF bcl )
{
	RECT		rct[1];
	int			idx;

	if ( m_Fcl_tbl && m_Bcl_tbl && row < m_LineCount ){
		CalcCellRect( rct, 0, row );
		for ( idx=0; idx<m_Grd_def->cct; idx++ ){
			if ( clm < m_Grd_def->cct ){
				if ( clm<0 || clm==idx ){
					*(m_Fcl_tbl + m_Grd_def->cct * row + idx) = fcl; 
					*(m_Bcl_tbl + m_Grd_def->cct * row + idx) = bcl;
					if ( clm<0 ){
						rct->right+=m_Grd_def->clm->wid;
					}
					else{
						CalcCellRect( rct, clm, row );
						break;
					}
				}
			}
		}
		InvalidateRect( rct, FALSE );
	}
}

int CSimpleGrid::AddRow( int nCount )
{
	WCHAR		*str;
	char		*enb;
	int			*val, lct=m_LineCount+nCount, siz, idx;
	COLORREF	*fcl, *bcl;

	if ( lct ){
		str = (WCHAR *)calloc(m_Grd_def->cct*lct, wsizeof(MAX_DGT) );
		enb = (char *)calloc( m_Grd_def->cct*lct, sizeof(char) );
		val = (int *)calloc( m_Grd_def->cct*lct, sizeof(int) );
		fcl = (COLORREF *)calloc( m_Grd_def->cct*lct, sizeof(COLORREF) );
		bcl = (COLORREF *)calloc( m_Grd_def->cct*lct, sizeof(COLORREF) );

		if ( val && str && enb && fcl && bcl ){
			if ( m_Str_tbl ){
				siz = wsizeof(m_Grd_def->cct * m_LineCount * MAX_DGT);
				memcpy( str, m_Str_tbl, siz );
			}
			if ( m_Val_tbl ){
				siz = m_Grd_def->cct * m_LineCount * sizeof(int);
				memcpy( val, m_Val_tbl, siz );
			}
			if ( m_Enb_tbl ){
				siz = m_Grd_def->cct * m_LineCount * sizeof(char);
				memcpy( enb, m_Enb_tbl, siz );
			}
			if ( m_Fcl_tbl ){
				siz = m_Grd_def->cct * m_LineCount * sizeof(COLORREF);
				memcpy( fcl, m_Fcl_tbl, siz );
			}
			if ( m_Bcl_tbl ){
				siz = m_Grd_def->cct * m_LineCount * sizeof(COLORREF);
				memcpy( bcl, m_Bcl_tbl, siz );
			}
			if ( nCount ){
				memset( enb+m_Grd_def->cct*m_LineCount*sizeof(char),
					TRUE, nCount*m_Grd_def->cct );
				siz = m_Grd_def->cct * m_LineCount;
				for ( idx=0; idx<m_Grd_def->cct*nCount; idx++ ){
					*(fcl+siz+idx) = m_Grd_def->dfc;
					*(bcl+siz+idx) = m_Grd_def->dbc;
				}
			}
			m_LineCount+=nCount;
			if ( m_Str_tbl )	free( m_Str_tbl );
			if ( m_Val_tbl )	free( m_Val_tbl );
			if ( m_Enb_tbl )	free( m_Enb_tbl );
			if ( m_Fcl_tbl )	free( m_Fcl_tbl );
			if ( m_Bcl_tbl )	free( m_Bcl_tbl );
			m_Str_tbl = str;
			m_Val_tbl = val;
			m_Enb_tbl = enb;
			m_Fcl_tbl = fcl;
			m_Bcl_tbl = bcl;
			SetVScrollBar();
		}
		else{
			if ( str )	free( str );
			if ( val )	free( val );
			if ( enb )	free( enb );
			if ( fcl )	free( fcl );
			if ( bcl )	free( bcl );
			lct = 0;
		}
	}
	return( lct- ( m_Grd_def->hdr ? 1: 0 ) );
}

int CSimpleGrid::DelRow( int nRow )
{
	WCHAR		*str;
	char		*enb;
	int			*val, cct, sct, lct=m_LineCount-1;
	COLORREF	*fcl, *bcl;

	nRow += m_Grd_def->hdr ? 1: 0;

	if ( nRow <= m_LineCount && !( !!m_Grd_def->hdr && nRow==0 ) ){
		str = (WCHAR *)calloc( m_Grd_def->cct*lct, wsizeof(MAX_DGT) );
		enb = (char *)calloc( m_Grd_def->cct*lct, sizeof(char) );
		val = (int *)calloc( m_Grd_def->cct*lct, sizeof(int) );
		fcl = (COLORREF *)calloc( m_Grd_def->cct*lct, sizeof(COLORREF) );
		bcl = (COLORREF *)calloc( m_Grd_def->cct*lct, sizeof(COLORREF) );

		if ( val && str && enb && fcl && bcl ){
			cct = (nRow-1) * m_Grd_def->cct;	sct= nRow * m_Grd_def->cct;
			if ( m_Str_tbl ){
				memcpy( str, m_Str_tbl, cct * wsizeof(MAX_DGT) );
				if ( nRow < m_LineCount ){
					memcpy( str+(cct*MAX_DGT), m_Str_tbl+(sct*MAX_DGT), (m_LineCount-nRow) * m_Grd_def->cct * wsizeof(MAX_DGT) );
				}
			}
			if ( m_Val_tbl ){
				memcpy( val, m_Val_tbl, cct * sizeof(int) );
				if ( nRow < m_LineCount ){
					memcpy( val+cct, m_Val_tbl+sct, (m_LineCount-nRow) * m_Grd_def->cct * sizeof(int) );
				}
			}
			if ( m_Enb_tbl ){
				memcpy( enb, m_Enb_tbl, cct * sizeof(char) );
				if ( nRow < m_LineCount ){
					memcpy( enb+cct, m_Enb_tbl+sct,  (m_LineCount-nRow) * m_Grd_def->cct * sizeof(char) );
				}
			}
			if ( m_Fcl_tbl ){
				memcpy( fcl, m_Fcl_tbl, cct * sizeof(COLORREF) );
				if ( nRow < m_LineCount ){
					memcpy( fcl+cct, m_Fcl_tbl+sct, (m_LineCount-nRow) * m_Grd_def->cct * sizeof(COLORREF) );
				}
			}
			if ( m_Bcl_tbl ){
				memcpy( bcl, m_Bcl_tbl, cct * sizeof(COLORREF) );
				if ( nRow < m_LineCount ){
					memcpy( bcl+cct, m_Bcl_tbl+sct, (m_LineCount-nRow) * m_Grd_def->cct * sizeof(COLORREF) );
				}
			}
			m_LineCount -= 1;
			if ( m_LineCount <= m_Grd_def->vlc ){
				m_TopRow = 1;
			}

			if ( m_Str_tbl )	free( m_Str_tbl );
			if ( m_Val_tbl )	free( m_Val_tbl );
			if ( m_Enb_tbl )	free( m_Enb_tbl );
			if ( m_Fcl_tbl )	free( m_Fcl_tbl );
			if ( m_Bcl_tbl )	free( m_Bcl_tbl );
			m_Str_tbl = str;
			m_Val_tbl = val;
			m_Enb_tbl = enb;
			m_Fcl_tbl = fcl;
			m_Bcl_tbl = bcl;
			SetVScrollBar();
		}
		else{
			if ( str )	free( str );
			if ( val )	free( val );
			if ( enb )	free( enb );
			if ( fcl )	free( fcl );
			if ( bcl )	free( bcl );
			lct = 0;
		}
	}
	return( nRow );
}

void CSimpleGrid::ResetContent()
{
	WCHAR		*str;
	char		*enb;
	int			*val, siz;
	COLORREF	*fcl, *bcl;
	RECT		rct[1];

	if ( m_Grd_def->hdr ){
		str = (WCHAR *)calloc( m_Grd_def->cct, wsizeof(MAX_DGT) );
		enb = (char *)calloc( m_Grd_def->cct, sizeof(char) );
		val = (int *)calloc( m_Grd_def->cct, sizeof(int) );
		fcl = (COLORREF *)calloc( m_Grd_def->cct, sizeof(COLORREF) );
		bcl = (COLORREF *)calloc( m_Grd_def->cct, sizeof(COLORREF) );

		if ( val && str && enb && fcl && bcl ){
			m_LineCount = 1;
			if ( m_Str_tbl ){
				siz = wsizeof(m_Grd_def->cct * m_LineCount * MAX_DGT);
				memcpy( str, m_Str_tbl, siz );
			}
			if ( m_Val_tbl ){
				siz = m_Grd_def->cct * m_LineCount * sizeof(int);
				memcpy( val, m_Val_tbl, siz );
			}
			if ( m_Enb_tbl ){
				siz = m_Grd_def->cct * m_LineCount * sizeof(char);
				memcpy( enb, m_Enb_tbl, siz );
			}
			if ( m_Fcl_tbl ){
				siz = m_Grd_def->cct * m_LineCount * sizeof(COLORREF);
				memcpy( fcl, m_Fcl_tbl, siz );
			}
			if ( m_Bcl_tbl ){
				siz = m_Grd_def->cct * m_LineCount * sizeof(COLORREF);
				memcpy( bcl, m_Bcl_tbl, siz );
			}

			if ( m_Str_tbl )	free( m_Str_tbl );
			if ( m_Val_tbl )	free( m_Val_tbl );
			if ( m_Enb_tbl )	free( m_Enb_tbl );
			if ( m_Fcl_tbl )	free( m_Fcl_tbl );
			if ( m_Bcl_tbl )	free( m_Bcl_tbl );
			m_Str_tbl = str;
			m_Val_tbl = val;
			m_Enb_tbl = enb;
			m_Fcl_tbl = fcl;
			m_Bcl_tbl = bcl;
		}
		else{
			if ( str )	free( str );
			if ( val )	free( val );
			if ( enb )	free( enb );
			if ( fcl )	free( fcl );
			if ( bcl )	free( bcl );
		}
	}
	else{
		m_LineCount = 0;
		if ( m_Str_tbl )	free( m_Str_tbl );
		if ( m_Val_tbl )	free( m_Val_tbl );
		if ( m_Enb_tbl )	free( m_Enb_tbl );
		if ( m_Fcl_tbl )	free( m_Fcl_tbl );
		if ( m_Bcl_tbl )	free( m_Bcl_tbl );

		m_Str_tbl = NULL;
		m_Val_tbl = NULL;
		m_Enb_tbl = NULL;
		m_Fcl_tbl = NULL;
		m_Bcl_tbl = NULL;
	}
	m_SelRow = m_SelColumn = -1;
	m_TopRow = 1;
	m_LeftColumn = 0;

	SetVScrollBar();
	InputEnable( FALSE );
	if ( m_EditBox )		m_EditBox->ShowWindow( SW_HIDE );
	if ( m_DateEditBox )	m_DateEditBox->ShowWindow( SW_HIDE );
	if ( m_TimeEditBox )	m_TimeEditBox->ShowWindow( SW_HIDE );
	if ( m_CommaEditBox )	m_CommaEditBox->ShowWindow( SW_HIDE );

	GetClientRect( rct );
	HBRUSH	brs = ::CreateSolidBrush( GetSysColor(COLOR_3DFACE) );
	HDC		hdc = ::GetDC( m_hWnd );
	::FillRect( hdc, rct, brs );
	::DeleteObject( brs );
	::ReleaseDC( m_hWnd, hdc );
	::InvalidateRect( m_hWnd, NULL, FALSE );
}

int CSimpleGrid::SetCheck( int clm, int row, int chk )
{
	int			*ptr;
	int			dat=-1, old=0;
	RECT		rct[1];

	if ( m_Str_tbl ){
		if ( clm < m_Grd_def->cct && row < m_LineCount ){
			ptr = m_Val_tbl + m_Grd_def->cct * row + clm; 
			old = *ptr;
			*ptr = chk;
			CalcCellRect( rct, clm, row-m_TopRow+1 );
			InvalidateRect( rct, FALSE );
		}
	}
	return( old );
}

int CSimpleGrid::GetCheck( int clm, int row )
{
	int			*ptr;
	int			dat=-1;

	if ( m_Str_tbl ){
	if ( clm < m_Grd_def->cct && row < m_LineCount ){
			ptr = m_Val_tbl + m_Grd_def->cct * row + clm; 
			dat = *ptr;
		}
	}
	return( dat );
}

int CSimpleGrid::SetData( int clm, int row, int dat )
{
	int			*ptr;
	int			old=0;

	if ( m_Str_tbl ){
		if ( clm < m_Grd_def->cct && row < m_LineCount ){
			if ( !( (m_Grd_def->clm+clm)->typ & GCT_CHK ) ){
				ptr = m_Val_tbl + m_Grd_def->cct * row + clm;
				old = *ptr;
				*ptr = dat;
			}
		}
	}
	return( old );
}

int CSimpleGrid::GetData( int clm, int row )
{
	int			*ptr;
	int			dat=0;

	if ( m_Str_tbl ){
		if ( clm < m_Grd_def->cct && row < m_LineCount ){
			if ( !( (m_Grd_def->clm+clm)->typ & GCT_CHK ) ){
				ptr = m_Val_tbl + m_Grd_def->cct * row + clm; 
				dat = *ptr;
			}
		}
	}
	return( dat );
}

int CSimpleGrid::GetSelRow()
{
	int			sel=-1;

	if ( !!m_Grd_def->sel ){
		sel = m_SelRow;
	}
	return( sel );
}

int CSimpleGrid::SetEnable( int clm, int row, int enb )
{
	int			old=FALSE, idx;
	char		*ptr;
	RECT		rct[1];

	if ( m_Str_tbl ){
		if ( clm < 0 ){
			if ( row < m_LineCount ){
				ptr = m_Enb_tbl + m_Grd_def->cct * row;
				old = *ptr;
				for ( idx=0; idx<m_Grd_def->cct; idx++, ptr++ ){
					if ( *ptr != enb ){
						*ptr = enb;
						CalcCellRect( rct, idx, row );
						InvalidateRect( rct, TRUE );
					}
				}
			}
		}
		else  if ( clm < m_Grd_def->cct && row < m_LineCount ){
			ptr = m_Enb_tbl + m_Grd_def->cct * row + clm;
			old = *ptr;
			if ( old != enb ){
				CalcCellRect( rct, clm, row );
				*ptr = enb;
				InvalidateRect( rct, TRUE );
			}
		}
	}
	return( old );
}

int CSimpleGrid::GetSelCol()
{
	int			sel=-1;

	if ( m_Grd_def->sel==GSL_CLM ){
		sel = m_SelColumn;
	}
	return( sel );
}

int CSimpleGrid::IsHeader()
{
	return( m_Grd_def->hdr );
}

int CSimpleGrid::InputEnable( int enb )
{
	int			old = m_InputEnable;

	if ( m_InputEnable!=enb ){
		m_InputEnable=enb;
		if ( !enb && !!m_EditBox ){
			m_EditBox->ShowWindow( SW_HIDE );
		}
		if ( !enb && !!m_DateEditBox ){
			m_DateEditBox->ShowWindow( SW_HIDE );
		}
		if ( !enb && !!m_TimeEditBox ){
			m_TimeEditBox->ShowWindow( SW_HIDE );
		}
		if ( !enb && !!m_CommaEditBox ){
			m_CommaEditBox->ShowWindow( SW_HIDE );
		}
	}
	return( old );
}

void CSimpleGrid::SetSelectCell( int clm, int row )
{
	RECT		rct[1];

	if ( ( clm>=0 ) && ( clm<m_Grd_def->cct ) &&
		( row>0 ) && (row<m_LineCount ) ){
		ChangeSelect( clm, row );
	}
	else{
		CalcCellRect( rct, -1, m_SelRow );
		m_SelColumn = m_SelRow = -1;
		InvalidateRect( rct, TRUE );
	}
}

int CSimpleGrid::GetLineCount( int IncludingHeader )
{
	return( m_LineCount - ( IncludingHeader ? 0: ( m_Grd_def->hdr ? 1: 0 ) ) );
}

void CSimpleGrid::Redraw()
{
	InvalidateRect( NULL, TRUE );
}

// CSimpleGrid メッセージ ハンドラ

void CSimpleGrid::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	// 描画メッセージで CWnd::OnPaint() を呼び出さないでください。

	HPEN		pen/*, spn*/;
	HBRUSH		brs, sbs;
	HGDIOBJ		opn, obs, ofn;
	int			mod, idy, idx, *val, sel;
	WCHAR		*ptr;
	char		*enb;
	COLORREF	*fcl, *bcl, txc;
	RECT		rct[1], drc[1];

	if ( m_Fst_flg ){
		m_Fst_flg = FALSE;
		SetHScrollBar();
	}
	if ( m_Grd_def ){
		mod = ::SetBkMode( dc.m_hDC, TRANSPARENT );
		pen = ::CreatePen( PS_SOLID, 1, RGB(128,128,128) );	// 2008.12.2変更
		sbs = ::CreateSolidBrush( RGB( 0, 96, 192 ) );
//		spn = ::CreatePen( PS_SOLID, 2, m_Grd_def->sbc );	// 特殊仕様（選択項目は矩形で囲む）
		if ( m_Font )	ofn = ::SelectObject( dc.m_hDC, m_Font );
		brs = ::CreateSolidBrush( m_Grd_def->hbc );
		opn = ::SelectObject( dc.m_hDC, pen );
		obs = ::SelectObject( dc.m_hDC, brs );
		// ヘッダー描画
		if ( m_Grd_def->hdr ){
			::SetTextColor( dc.m_hDC, m_Grd_def->hfc );
			rct->left = rct->top = 0;
			rct->bottom = m_Grd_def->lnh;
			ptr = m_Str_tbl;
			val = m_Val_tbl;
			for ( idx=0; idx<m_Grd_def->cct; idx++ ){
				if ( idx >= m_LeftColumn ){
					rct->right = rct->left + (m_Grd_def->clm+idx)->wid;
					::Rectangle( dc.m_hDC, rct->left, rct->top, rct->right, rct->bottom );
					if ( !!ptr ){
						::DrawText( dc.m_hDC, ptr, (int)wcslen(ptr), rct,
							DT_SINGLELINE|DT_CENTER|DT_VCENTER );
					}
					rct->left += (m_Grd_def->clm+idx)->wid - 1;
					rct->right += (m_Grd_def->clm+idx)->wid - 1;
				}
				if ( val )	val++;
				if ( ptr )	ptr += MAX_DGT;
			}
		}
		::SelectObject( dc.m_hDC, obs );
		::DeleteObject( brs );
		brs = ::CreateSolidBrush( GetSysColor( COLOR_3DFACE ) );
		GetClientRect( drc );
		drc->left = rct->left + 1;
		FillRect( dc.m_hDC, drc, brs );
		::DeleteObject( brs );
		// データ部描画
		rct->top = m_Grd_def->hdr ? m_Grd_def->lnh - 1: 0;
		rct->bottom = rct->top + m_Grd_def->lnh;
		if ( m_Str_tbl && m_Val_tbl && m_Enb_tbl){
			ptr = m_Str_tbl + m_Grd_def->cct * m_TopRow * MAX_DGT;
			val = m_Val_tbl + m_Grd_def->cct * m_TopRow;
			enb = m_Enb_tbl + m_Grd_def->cct * m_TopRow;
			fcl = m_Fcl_tbl + m_Grd_def->cct * m_TopRow;
			bcl = m_Bcl_tbl + m_Grd_def->cct * m_TopRow;
		}
		else{
			ptr = NULL;
			val = NULL;
			enb = NULL;
			fcl = NULL;
			bcl = NULL;
		}
		for ( idy=0; idy<m_Grd_def->vlc; idy++ ){
			if ( m_TopRow+idy > m_LineCount-1 )	break;
			rct->left = 0;
			for ( idx=0; idx<m_Grd_def->cct; idx++ ){
				if ( idx >= m_LeftColumn ){
					if ( sel = !!m_Grd_def->sel ){
						sel  = m_TopRow+idy==m_SelRow;
						sel &= m_Grd_def->sel==GSL_ROW || idx==m_SelColumn; 
					}
					if ( !!enb && !!(*enb) ){
						if ( sel ){
							brs = ::CreateSolidBrush( m_Grd_def->sbc );
							txc = m_Grd_def->sfc;
						}
						else{
							if ( bcl )	brs = ::CreateSolidBrush( *bcl );
							else		brs = ::CreateSolidBrush( m_Grd_def->dbc );
							if ( fcl )	txc = *fcl;
							else		txc = m_Grd_def->dfc;
						}
					}
					else{
						brs = ::CreateSolidBrush( RGB(248,248,248) );
						txc = RGB(0, 0, 0);
					}
					::SelectObject( dc.m_hDC, sel ? sbs: brs );
					::SetTextColor( dc.m_hDC, (!!enb && !!(*enb)) ? txc: ( !!sel ? RGB(255,255,255): txc ) );
					rct->right = rct->left + (m_Grd_def->clm+idx)->wid;
					::Rectangle( dc.m_hDC, rct->left, rct->top, rct->right, rct->bottom );
					//::FillRect( dc.m_hDC, rct, sbs );

/*
					if ( sel ){				// 特殊仕様（選択項目は矩形で囲む）
						::SelectObject( dc.m_hDC, spn );
						::MoveToEx( dc.m_hDC, rct->left+1, rct->top+1, NULL );
						::LineTo( dc.m_hDC, rct->right-1, rct->top+1 );
						::MoveToEx( dc.m_hDC, rct->left+1, rct->bottom-1, NULL );
						::LineTo( dc.m_hDC, rct->right-1, rct->bottom-1 );
						if ( m_Grd_def->sel==GSL_CLM || idx==m_Grd_def->cct-1 ){
							::LineTo( dc.m_hDC, rct->right-1, rct->top+1 );
						}
						if ( m_Grd_def->sel==GSL_CLM || idx==0 ){
							::MoveToEx( dc.m_hDC, rct->left+1, rct->top+1, NULL );
							::LineTo( dc.m_hDC, rct->left+1, rct->bottom-1 );
						}
						::SelectObject( dc.m_hDC, pen );
					}
*/
					*drc = *rct;
					if ( !!ptr ){
						if ( !!( (m_Grd_def->clm+idx)->typ & GCT_CHK ) ){
							drw_cbx( dc.m_hDC, drc, val ? *val: 0 );
							drc->left += CBX_SIZ+8;
						}
						else{
							drc->left+=3;
						}
						drc->right-=3;
						::DrawText( dc.m_hDC, ptr, (int)wcslen(ptr), drc,
							(m_Grd_def->clm+idx)->alg );
					}
					rct->left += (m_Grd_def->clm+idx)->wid - 1;
					rct->right += (m_Grd_def->clm+idx)->wid - 1;
					::SelectObject( dc.m_hDC, obs );
					::DeleteObject( brs );
				}
				if ( val )	val++;
				if ( enb )	enb++;
				if ( fcl )	fcl++;
				if ( bcl )	bcl++;
				if ( ptr )	ptr += MAX_DGT;
			}
			rct->top += m_Grd_def->lnh - 1;
			rct->bottom += m_Grd_def->lnh - 1;
		}

		brs = ::CreateSolidBrush( GetSysColor( COLOR_3DFACE ) );
		GetClientRect( rct );
		rct->top += (m_Grd_def->lnh - 1) * ( idy + 1 ) + 1;
		FillRect( dc.m_hDC, rct, brs );
		::DeleteObject( brs );

		::SetBkMode( dc.m_hDC, mod );
		::SelectObject( dc.m_hDC, opn );
		::SelectObject( dc.m_hDC, obs );
		if ( m_Font )	::SelectObject( dc.m_hDC, ofn );
		::DeleteObject( sbs );
//		::DeleteObject( spn );	// 特殊仕様（選択項目は矩形で囲む）
		::DeleteObject( pen );
	}
}

BOOL CSimpleGrid::PreCreateWindow(CREATESTRUCT& cs)
{
//	cs.style |= WS_VSCROLL;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	return CWnd::PreCreateWindow(cs);
}

BOOL CSimpleGrid::Create(GRD_DEF *grd, const RECT& rect, UINT nID, CWnd* pParentWnd)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	GRD_CLM		*clm;
	int			/*siz, */idx, wid=0, flw;
	RECT		rct[1];
	LOGFONT		fnt[1];

	m_pParent = pParentWnd;
	memset( fnt, 0, sizeof(fnt) );
	fnt->lfCharSet = DEFAULT_CHARSET;				// SHIFTJIS_CHARSET;
	wcscpy_s( fnt->lfFaceName, sizeof(fnt->lfFaceName)/wsizeof(1), _T("MS UI Gothic") );	// フォント名
	if ( grd->fsa ){
		fnt->lfHeight = ( grd->lnh - 4 ) * -1;	// フォントサイズ
		fnt->lfWeight = 400;//bld ? 700: 400;			// スタイル～ボールド（400,700)
	}
	else{
		fnt->lfHeight = 18 * -1;	// フォントサイズ
		fnt->lfWeight = 700;//bld ? 700: 400;			// スタイル～ボールド（400,700)
	}
	fnt->lfQuality = PROOF_QUALITY;					// 文字品質
	m_Font = CreateFontIndirect( fnt );				// フォント作成

	m_TopRow = grd->hdr ? 1: 0;

	*m_Grd_def = *grd;
	m_LineCount = grd->hdr ? 1: 0;
	m_Grd_def->clm = clm = (GRD_CLM *)calloc( grd->cct, sizeof(GRD_CLM) );
	if ( clm ){
		if ( m_LineCount ){
			m_Str_tbl = (WCHAR *)calloc( wsizeof(grd->cct*m_LineCount), wsizeof(MAX_DGT) );
			m_Enb_tbl = (char *)calloc( grd->cct*m_LineCount, sizeof(char) );
			m_Val_tbl = (int *)calloc( grd->cct*m_LineCount, sizeof(int) );
			m_Fcl_tbl = (COLORREF *)calloc( grd->cct*m_LineCount, sizeof(COLORREF) );
			m_Bcl_tbl = (COLORREF *)calloc( grd->cct*m_LineCount, sizeof(COLORREF) );
		}
		if ( !m_LineCount || ( !!m_Str_tbl && !!m_Val_tbl && !!m_Enb_tbl && !!m_Fcl_tbl && !!m_Bcl_tbl ) ){
			memset( m_Enb_tbl, TRUE, grd->cct*m_LineCount );
			for ( idx=0; idx<grd->cct*m_LineCount; idx++ ){
				*(m_Fcl_tbl+idx) = m_Grd_def->hfc;
				*(m_Bcl_tbl+idx) = m_Grd_def->hbc;
			}
			for ( idx=0; idx<grd->cct; idx++ ){
//				siz = (int)strlen( (grd->clm+idx)->ttl ) + 8;
//				clm->ttl = (char *)malloc( siz );
//				if ( clm->ttl )	strcpy_s( clm->ttl, siz, (grd->clm+idx)->ttl );
//				else			break;
				clm->wid = (grd->clm+idx)->wid;
				clm->alg = (grd->clm+idx)->alg;
				clm->typ = (grd->clm+idx)->typ;
				if ( idx<grd->vcc )	wid += (grd->clm+idx)->wid - 1;
				clm++;
			}
			if ( idx<grd->cct ){		// メモリー確保エラー
//				do{
//					free( (m_Grd_def->clm+idx)->ttl );
//				}while( idx-- );
				free( m_Grd_def->clm );
				m_Grd_def->clm = NULL;
			}
		}
		else{
			if ( !!m_Val_tbl )	free( m_Val_tbl );
			if ( !!m_Str_tbl )	free( m_Str_tbl );
			if ( !!m_Enb_tbl )	free( m_Enb_tbl );
			if ( !!m_Fcl_tbl )	free( m_Fcl_tbl );
			if ( !!m_Bcl_tbl )	free( m_Bcl_tbl );
			free( clm );
			m_Grd_def->clm = NULL;
		}
	}

	*rct = rect;
	flw = GetSystemMetrics( SM_CXEDGE ) * 2;
//	if ( rct->right==-1 ){
		rct->right = rct->left + wid + flw + 1;
//	}
	flw = GetSystemMetrics( SM_CYEDGE ) * 2;
//	if ( rct->bottom==-1 ){
		rct->bottom = rct->top +
			( m_Grd_def->lnh -1 ) * m_Grd_def->vlc + flw + 1;
//	}

	return CWnd::Create(NULL, _T(""), WS_CHILD|WS_VISIBLE|WS_TABSTOP, rct[0], pParentWnd, nID, NULL);
}

void CSimpleGrid::PostNcDestroy()
{
	int			idx;

	if ( m_Grd_def ){
		if ( m_Grd_def->clm ){
			for ( idx=0; idx<m_Grd_def->cct; idx++ ){
//				if ( (m_Grd_def->clm+idx)->ttl )	free( (m_Grd_def->clm+idx)->ttl );
			}
			free( m_Grd_def->clm );
		}
	}
	if ( m_Str_tbl )	free( m_Str_tbl );
	if ( m_Val_tbl )	free( m_Val_tbl );
	if ( m_Enb_tbl )	free( m_Enb_tbl );
	if ( !!m_Fcl_tbl )	free( m_Fcl_tbl );
	if ( !!m_Bcl_tbl )	free( m_Bcl_tbl );
	if ( m_Font )	::DeleteObject( m_Font );

	CWnd::PostNcDestroy();
}

void CSimpleGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
}

void CSimpleGrid::SelectCell( int clm, int row )
{
	int			*ptr, *val, typ, cre, cmp;
	COLORREF	clr;
	RECT		rct[1];
	WCHAR		buf[MAX_DGT];
	char		enb;
	GRD_CLM		*cdf;
	CCustomEdit	*edt;

	if ( row - m_TopRow >= 0 ){
		if ( m_Grd_def->sel ){
			if ( m_SelColumn>-1 && m_SelRow>-1 ){
				CalcCellRect( rct,
					m_Grd_def->sel==GSL_ROW ? -1: m_SelColumn, m_SelRow );
				InvalidateRect( rct, FALSE );
			}
			m_SelColumn = clm;
			m_SelRow = row;
		}
		typ = (int)SendNotify( GN_QUERYINPUTTYPE, 0, 0, clm, row, 0 );
		cdf = m_Grd_def->clm+clm;
		if ( typ==GCT_CDF )	typ = cdf->typ;
		enb  = !!(*( m_Enb_tbl + m_Grd_def->cct * row + clm ) );
		enb &= m_InputEnable && row>0;

		if ( !!( typ & GCT_CHK ) && enb ){
			ptr = m_Val_tbl+m_Grd_def->cct*row+clm;
			*ptr = !(*ptr);
		}
		CalcCellRect( rct, m_Grd_def->sel==GSL_ROW ? -1: clm, row );

		if ( !!( typ & (GCT_EDT|GCT_DED|GCT_TED|GCT_CED) ) && enb ){
			if ( m_InputChar ){
				*buf = m_InputChar;
				*(buf+1) = '\0';
			}
			else{
				GetCellText( buf, memcnt(buf), clm, row );
			}
			CalcCellRect( rct, clm, row );
			switch( typ&(GCT_EDT|GCT_DED|GCT_TED|GCT_CED) ){
			case GCT_EDT:	if ( !m_EditBox ){
								if ( cre = !!( m_EditBox = new CCustomEdit() ) ){
									m_EditBox->Create( WS_VISIBLE, rct[0], this, 100 );
									m_EditBox->ModifyStyle( 0, ES_UPPERCASE );
									m_EditBox->SetFont( _T("MS UI Gothic"), ( m_Grd_def->lnh - 4 ) * -1, RGB(0,0,0) );
								}
							}
							else{
								m_EditBox->ShowWindow( SW_HIDE );
							}
							edt = m_EditBox;
							break;
			case GCT_DED:	if ( !m_DateEditBox ){
								if ( cre = !!( m_DateEditBox = new CDateEdit() ) ){
									m_DateEditBox->Create( WS_VISIBLE, rct[0], this, 101 );
									m_DateEditBox->ModifyStyle( 0, ES_UPPERCASE );
									m_DateEditBox->SetFont( _T("MS UI Gothic"), ( m_Grd_def->lnh - 4 ) * -1, RGB(0,0,0) );
								}
							}
							else{
								m_DateEditBox->ShowWindow( SW_HIDE );
							}
							edt = (CCustomEdit *)m_DateEditBox;
							break;
			case GCT_TED:	if ( !m_TimeEditBox ){
								if ( cre = !!( m_TimeEditBox = new CTimeEdit() ) ){
									m_TimeEditBox->Create( WS_VISIBLE, rct[0], this, 102 );
									m_TimeEditBox->ModifyStyle( 0, ES_UPPERCASE );
									m_TimeEditBox->SetFont( _T("MS UI Gothic"), ( m_Grd_def->lnh - 4 ) * -1, RGB(0,0,0) );
								}
							}
							else{
								m_TimeEditBox->ShowWindow( SW_HIDE );
							}
							edt = (CCustomEdit *)m_TimeEditBox;
							break;
			case GCT_CED:	if ( !m_CommaEditBox ){
								if ( cre = !!( m_CommaEditBox = new CCommaEdit() ) ){
									m_CommaEditBox->Create( WS_VISIBLE, rct[0], this, 102 );
									m_CommaEditBox->ModifyStyle( 0, ES_UPPERCASE );
									m_CommaEditBox->SetFont( _T("MS UI Gothic"), ( m_Grd_def->lnh - 4 ) * -1, RGB(0,0,0) );
								}
							}
							else{
								m_CommaEditBox->ShowWindow( SW_HIDE );
							}
							cmp = (int)SendNotify( GN_QUERYCMEPARAM, 0, 0, clm, row, 0 );
							m_CommaEditBox->DecimalPlacess = HIWORD( cmp );
							m_CommaEditBox->RoundMethod = LOWORD( cmp );
							edt = (CCustomEdit *)m_CommaEditBox;
							break;
			}
			if ( cre ){
				if ( (typ&GCT_EDT) ){
					edt->LimitText( cdf->dgt );
					if ( typ & GCT_NMO )	edt->ModifyStyle( 0, ES_NUMBER );
					else					edt->ModifyStyle( ES_NUMBER, 0 );
				}
				rct->top++; rct->left++;
				rct->bottom--; rct->right--;
				edt->SetWindowText( buf );
				edt->MoveWindow( rct );
				//::SetWindowLong( edt->m_hWnd, GWL_USERDATA, (LONG)MAKELPARAM(clm, row) );
				edt->SetUserData((LONG)MAKELPARAM(clm, row) );
				edt->ShowWindow( SW_NORMAL );
				if ( m_InputChar )	edt->SetSel( 1,  1, TRUE );
				else				edt->SetSel( 0, -1, TRUE );
				edt->SetFocus();
				m_EditFlg |= (EDF_EDT|typ);
			}
		}
		else{
			switch( typ&(GCT_EDT|GCT_DED|GCT_TED|GCT_CED) ){
			case GCT_EDT:	edt = m_EditBox;					break;
			case GCT_DED:	edt = (CCustomEdit *)m_DateEditBox;	break;
			case GCT_TED:	edt = (CCustomEdit *)m_TimeEditBox;	break;
			case GCT_CED:	edt = (CCustomEdit *)m_CommaEditBox;break;
			default:		edt = NULL;							break;
			}
			if ( edt ){
				m_EditFlg &= ((~EDF_EDT)&0xffff0000);
				edt->ShowWindow( SW_HIDE );
			}
		}
		if ( ( ( typ & GCT_CMB ) || ( typ & GCT_PNS ) ) && enb){
			val = m_Val_tbl + m_Grd_def->cct * row + clm;
			CalcCellRect( rct, clm, row );
			rct->bottom += 640;
			if ( !m_ComboBox ){
				m_ComboBox = new CCustomCombo();
				m_ComboBox->Create( CBS_DROPDOWNLIST, rct[0], this, 110 );
				m_ComboBox->SetFont( _T("MS UI Gothic"), ( m_Grd_def->lnh - 8 ) * -1, RGB(0,0,0) );
				//m_ComboBox->ModifyStyle( WS_DLGFRAME, WS_BORDER );
				//m_ComboBox->SetItemHeight( -1, 14 );
			}
			else{
				m_ComboBox->ShowWindow( SW_HIDE );
			}
			if ( m_ComboBox ){
				rct->top++; rct->left++;
				rct->bottom-=2; rct->right-=2;
				m_ComboBox->MoveWindow( rct );
				::SetWindowLong( m_ComboBox->m_hWnd, GWL_USERDATA, (LONG)MAKELPARAM(clm, row) );
				m_ComboBox->ResetContent();
				m_ComboBox->ShowWindow( SW_NORMAL );
				SendNotify( GN_QUERYCOMBODATA, 0, 0, clm, row, (LPARAM)m_ComboBox );
				WCHAR *ptr = m_Str_tbl + MAX_DGT * m_Grd_def->cct * row + MAX_DGT * clm; 
				int idx = m_ComboBox->FindString(0, ptr);
				if ( idx!=CB_ERR )	m_ComboBox->SetCurSel( idx );
				//m_ComboBox->SetCurSel( *val );
				m_ComboBox->SetFocus();
				m_EditFlg |= EDF_CMB;
			}
		}
		else{
			if ( m_ComboBox ){
				m_EditFlg &= ~EDF_CMB;
				m_ComboBox->ShowWindow( SW_HIDE );
			}
		}
		if ( !!( typ & GCT_CLR ) && enb ){
			GetCellText( buf, sizeof(buf), clm, row );
			CalcCellRect( rct, clm, row );
			if ( SendNotify( GN_QUERYCOLORDATA, 0, 0, clm, row, (LPARAM)&clr ) ){
				SetCellColor( clm, row, RGB(0,0,0), clr );
				//InvalidateRect( rct, TRUE );
			}
		}
		else{
		}
		InvalidateRect( rct, FALSE );
	}
}

void CSimpleGrid::OnLButtonUp(UINT nFlags, CPoint point)
{
	int		clm, row, mgn;

	mgn = m_Grd_def->hdr ? 1: 0;
	SetFocus();
	if ( MPointToCell( &clm, &row, point.x, point.y ) ){
		clm += m_LeftColumn;
		row += m_TopRow-mgn;
		if ( ( clm>=0 ) && ( clm<m_Grd_def->cct ) && ( row>=0 ) && (row<m_LineCount ) ){
			if ( PostNotify( GN_LBUTTONCLICK, point.x, point.y, clm, row, 0 )==TRUE ){
				if ( !m_Grd_def->hdr || row>0 )	SelectCell( clm, row );
			}
		}
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CSimpleGrid::OnRButtonUp(UINT nFlags, CPoint point)
{
	int		clm, row, mgn;

	mgn = m_Grd_def->hdr ? 1: 0;
	SetFocus();
	if ( MPointToCell( &clm, &row, point.x, point.y ) ){
		clm += m_LeftColumn;
		row += m_TopRow-mgn;
//		SendNotify( GN_RBUTTONCLICK, point.x, point.y, clm+m_LeftColumn, row+m_TopRow-1, 0 );
		if ( ( clm>=0 ) && ( clm<m_Grd_def->cct ) && ( row>=0 ) && (row<m_LineCount ) ){
			if ( PostNotify( GN_RBUTTONCLICK, point.x, point.y, clm, row, 0 )==TRUE ){
				if ( !m_Grd_def->hdr || row>0 )	SelectCell( clm, row );
			}
		}
	}

	CWnd::OnRButtonUp(nFlags, point);
}

void CSimpleGrid::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int		clm, row;

	if ( MPointToCell( &clm, &row, point.x, point.y ) ){
		SendNotify( GN_LBUTTONDBLCLICK, point.x, point.y, clm+m_LeftColumn, row+m_TopRow-1, 0 );
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CSimpleGrid::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int			dif=0, scl, dlt=1, mgn;
	RECT		rct[1];
	SCROLLINFO	sci[1];

	mgn = m_Grd_def->hdr ? 1: 0;

	switch( nSBCode ){
	case SB_PAGEUP:			
	case SB_LINEUP:			dlt = nSBCode==SB_PAGEUP ? PAG_UNT: 1;
							dlt = m_TopRow-dlt < (mgn+1) ? dlt-(mgn-(m_TopRow-dlt)): dlt;
							m_TopRow -= dlt;
							dif = -dlt;
							break;
	case SB_PAGEDOWN:			
	case SB_LINEDOWN:		dlt = nSBCode==SB_PAGEDOWN ? PAG_UNT: 1;
							if ( m_TopRow+dlt+(m_Grd_def->vlc-mgn) > m_LineCount ){
								dlt = ( m_LineCount - ( m_Grd_def->vlc - mgn ) ) - m_TopRow;
							}
							m_TopRow += dlt;
							dif = dlt;
							break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:	dif = nPos - m_TopRow;
							m_TopRow = nPos;
							break;
	}
	if ( dif ){
		GetClientRect( rct );
		rct->top += m_Grd_def->hdr ? m_Grd_def->lnh: 0;
		scl = abs(dif)*(m_Grd_def->lnh-1);
		scl *= dif < 0 ? 1: -1;
		ScrollWindow( 0, scl, NULL, rct );
		if ( dif < 0 ){
			rct->bottom = rct->top+( m_Grd_def->hdr ? m_Grd_def->lnh - 1: 0 ) * ( dif * -1 );
		}
		else{
			rct->top = rct->bottom- ( m_Grd_def->lnh - 1) * dif;
		}
		InvalidateRect( rct, TRUE );
		sci->cbSize = sizeof(sci);
		sci->fMask = SIF_POS;
		sci->nPos = m_TopRow;
		SetScrollInfo( SB_VERT, sci );
	}
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CSimpleGrid::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int			dif=0, scl, dlt=1, idx, lft=m_LeftColumn;
	RECT		rct[1];
	SCROLLINFO	sci[1];

	switch( nSBCode ){
	case SB_PAGEUP:			
	case SB_LINEUP:			dlt = nSBCode==SB_PAGEUP ? PAG_UNT: 1;
							dlt = m_LeftColumn-dlt <= 0 ? dlt+(m_LeftColumn-dlt): dlt;
							m_LeftColumn -= dlt;
							dif = -dlt;
							break;
/*
	case SB_PAGEDOWN:			
	case SB_LINEDOWN:		dlt = nSBCode==SB_PAGEDOWN ? PAG_UNT: 1;
							if ( m_LeftColumn+dlt+(m_Grd_def->vcc) > m_Grd_def->cct ){
								dlt = ( m_Grd_def->cct - ( m_Grd_def->vcc ) ) - m_LeftColumn;
							}
							m_LeftColumn += dlt;
							dif = dlt;
							break;
*/
	case SB_PAGEDOWN:			
	case SB_LINEDOWN:		dlt = nSBCode==SB_PAGEDOWN ? PAG_UNT: 1;
							if ( m_LeftColumn+dlt > m_Grd_def->cct-1 ){
								dlt = ( (m_Grd_def->cct-1) ) - m_LeftColumn;
							}
							m_LeftColumn += dlt;
							dif = dlt;
							break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:	dif = nPos - m_LeftColumn;
							m_LeftColumn = nPos;
							break;
	}
	if ( dif ){
		GetClientRect( rct );
		scl = 0;
		if ( dif>0 ){
			for ( idx=0; idx<dif; idx++ ){
				scl += (m_Grd_def->clm+lft+idx)->wid - 1;
			}
			scl *= -1;
		}
		else{
			for ( idx=-1; idx>=dif; idx-- ){
				scl += (m_Grd_def->clm+lft+idx)->wid - 1;
			}
		}
		ScrollWindow( scl, 0, NULL, rct );
		if ( dif>0 ){
			rct->left = rct->right-scl;
		}
		else{
			rct->right = rct->left+scl;
		}
		InvalidateRect( rct, TRUE );
		sci->cbSize = sizeof(sci);
		sci->fMask = SIF_POS;
		sci->nPos = m_LeftColumn;
		SetScrollInfo( SB_HORZ, sci );
	}

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CSimpleGrid::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int			min, max;

	GetScrollRange( nFlags & MK_SHIFT ? SB_HORZ: SB_VERT, &min, &max );

	if ( max ){
		if ( nFlags & MK_SHIFT )	OnHScroll( zDelta<0 ? SB_LINERIGHT: SB_LINELEFT, 0, NULL );
		else						OnVScroll( zDelta<0 ? SB_LINEDOWN: SB_LINEUP, 0, NULL );
	}
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

// エディットボックス通知メッセージ
LRESULT CSimpleGrid::OnEditNotify( WPARAM wParam, LPARAM lParam )
{
	int			siz;
	DWORD		dat;
	WCHAR		*txt;
	DWORD		cid;
	CCustomEdit	*edt;

	if ( LOWORD(wParam) == WM_KILLFOCUS ){
		cid = HIWORD(wParam);
		edt = (CCustomEdit *)GetDlgItem(cid);
		if ( m_EditFlg & EDF_EDT ){
			//if ( txt = (WCHAR *)malloc( wsizeof(MAX_DGT) ) ){
			siz = (int)wcslen(edt->m_Txt_dat) + 1;

			if ( txt = (WCHAR *)malloc( wsizeof( siz ) ) ){
				//GetDlgItem(cid)->GetWindowText( txt, MAX_DGT );
				wcscpy_s( txt, siz, edt->m_Txt_dat );
				dat = (DWORD)lParam;//::GetWindowLong( m_EditBox->m_hWnd, GWL_USERDATA );
				SendNotify( GN_EDITCOMPLETE, 0, 0, LOWORD(dat), HIWORD(dat), (LPARAM)txt );
			}
		}
		GetDlgItem(HIWORD(wParam))->ShowWindow( SW_HIDE );
		m_EditFlg = 0;
	}
	return( TRUE );
}

// コンボボックス通知メッセージ
LRESULT CSimpleGrid::OnComboNotify( WPARAM wParam, LPARAM lParam )
{
	DWORD		dat;
//	char		*txt;
	int			idx;

	dat = ::GetWindowLong( m_ComboBox->m_hWnd, GWL_USERDATA );
	if ( LOWORD(wParam) == WM_KILLFOCUS ){
//		SendNotify( GN_EDITCOMPLETE, 0, 0, LOWORD(dat), HIWORD(dat), (LPARAM)txt );
		GetDlgItem(HIWORD(wParam))->ShowWindow( SW_HIDE );
	}
	else if ( LOWORD(wParam) == CN_CHANGED) {
		idx = m_ComboBox->GetCurSel();
		SendNotify( GN_COMBOCHANGED, 0, 0, LOWORD(dat), HIWORD(dat), (LPARAM)idx );
	}
	return( TRUE );
}

void CSimpleGrid::ChangeSelect(	int newColumn, int newRow )		// 選択カラム（ロー）変更
{
	RECT		rct[1];
	int			inr, vwd, cwd, idx, pos, dif, clm=m_SelColumn, row=m_SelRow;

	// 現在の選択状態を消去
	CalcCellRect( rct, m_Grd_def->sel==GSL_CLM ? clm: -1, row );
	InvalidateRect( rct, TRUE );
	GetClientRect( rct );
	vwd = rct->right - rct->left + 1;
	m_SelColumn = newColumn;
	m_SelRow = newRow;
	//表示範囲チェック
	if ( m_Grd_def->sel==GSL_CLM ){
		if ( !( inr = newColumn >= m_LeftColumn ) ){
			pos = m_SelColumn;
		}
		else{
			for ( cwd=0, idx=m_LeftColumn; idx<=newColumn; idx++ ){
				cwd += (m_Grd_def->clm+idx)->wid - 1;
			}
			inr = cwd < vwd;
			if ( !inr ){
				dif = cwd - vwd;
				pos = m_SelColumn;
				for ( vwd=0, idx=m_LeftColumn; idx<=m_Grd_def->cct; idx++ ){
					vwd += (m_Grd_def->clm+idx)->wid - 1;
					if ( vwd >= dif ){
						pos = idx+1;
						break;
					}
				}
			}
		}
		if ( !inr ){
			OnHScroll(SB_THUMBPOSITION, (UINT)pos, NULL );
		}
	}
	if ( !( inr = (m_SelRow-m_TopRow+1) > 1 ) ){
		pos = m_SelRow;
	}
	else{
		if ( !( inr &= (m_SelRow-m_TopRow+1) < m_Grd_def->vlc ) ){
			pos = m_SelRow - m_Grd_def->vlc + 2; 
		}
	}
	if ( !inr ){
		OnVScroll(SB_THUMBPOSITION, (UINT)pos, NULL );
	}
	CalcCellRect( rct, m_Grd_def->sel==GSL_CLM ? newColumn: -1, newRow );
	InvalidateRect( rct, TRUE );
	PostNotify( GN_SELECTCHANGE, 0, 0, newColumn, newRow, (LPARAM)0 );
}

void CSimpleGrid::OnArrowKey(UINT nKeyCode)
{
	int			upd=FALSE;
	int			clm=m_SelColumn, row=m_SelRow;

	if ( m_EditFlg ){
		//m_EditFlg = 0;
		SetFocus();
	}

	if ( m_Grd_def->sel==GSL_CLM ){
		if ( row!=-1 && clm!=-1 ){
			switch(nKeyCode){
			case VK_RIGHT:	clm++;
							upd = clm < m_Grd_def->cct;
							break;
			case VK_LEFT:	upd = --clm >= 0;
							break;
			}
		}
	}
	if ( m_Grd_def->sel!=GSL_NON ){
		if ( row!=-1 ){
			switch(nKeyCode){
			case VK_UP:		upd = --row > 0;
							break;
			case VK_DOWN:	upd = ++row < m_LineCount;
							break;
			}
		}
	}

	if ( upd )	ChangeSelect( clm, row );
}

void CSimpleGrid::OnSpaceKey()
{
	if ( m_Grd_def->sel==GSL_CLM ){
		if ( m_SelColumn!=-1 && m_SelRow!=-1 ){
			if ( m_EditFlg ){
				m_EditFlg = 0;
				SetFocus();
			}
			if ( !!( m_Grd_def->clm[m_SelColumn].typ & GCT_CHK ) ){
				SelectCell( m_SelColumn, m_SelRow );
			}
		}
	}
}

void CSimpleGrid::OnEnterKey( char chr )
{
	if ( m_Grd_def->sel==GSL_CLM ){
		if ( m_SelColumn!=-1 && m_SelRow!=-1 ){
			m_InputChar = chr;
			if ( m_EditFlg ){
				//m_EditFlg = 0;
				SetFocus();
				if ( !chr )	OnArrowKey(VK_DOWN);
			}
			else{
				SelectCell( m_SelColumn, m_SelRow );
				if ( !chr )	OnArrowKey(VK_DOWN);
			}
		}
	}
	else if ( m_Grd_def->sel==GSL_NON ){
		m_InputChar = chr;
		if ( m_EditFlg ){
			SetFocus();
		}
	}
}

BOOL CSimpleGrid::PreTranslateMessage(MSG* pMsg)
{
	BOOL			ret=TRUE;
//	unsigned char	chr;

	switch( pMsg->message ){
	case WM_KEYUP:
	case WM_CHAR:
	case WM_KEYDOWN:	if ( !m_EditFlg ){
							switch( pMsg->wParam ){
							case VK_ESCAPE:	
											break;
							case VK_RETURN:	if ( pMsg->message==WM_KEYDOWN ){
												OnEnterKey( 0 );
											}
											break;
							case VK_SPACE:	if ( pMsg->message==WM_KEYDOWN ){
												OnSpaceKey();
											}
											break;
							case VK_LEFT:	
							case VK_RIGHT:	
							case VK_UP:	
							case VK_DOWN:	if ( pMsg->message==WM_KEYDOWN ){
												OnArrowKey( (UINT)pMsg->wParam );
											}
											break;
							default:		if ( pMsg->message==WM_CHAR ){
												OnEnterKey(  (char)pMsg->wParam );
											}
											else{
												ret = FALSE;
											}
											break;
							}
						}
						else{
							ret = pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN;
							if ( ret )	OnEnterKey( 0 );
						}
						break;
	default:			ret = FALSE;
						break;
	}
	if ( !ret ){
		ret = CWnd::PreTranslateMessage(pMsg);
	}
	return( ret );

	return CWnd::PreTranslateMessage(pMsg);
}
