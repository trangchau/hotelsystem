// BookingStatusView.cpp : CBookingStatusView クラスの実装
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#include ".\fig\fig_ope.h"
#include ".\fnc\viw_cmn.h"

#include "BookingStatusDoc.h"
#include "BookingStatusView.h"
#include ".\fnc\bst_fnc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	TEV_INI			1			// 初期化タイマーイベント
#define	TIM_INI			10			// 初期化タイマーインターバル


// CBookingStatusView

IMPLEMENT_DYNCREATE(CBookingStatusView, CView)

BEGIN_MESSAGE_MAP(CBookingStatusView, CView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_GRAPHNOTIFY()
	ON_WM_VIEWNOTIFY()
	ON_WM_MOUSEWHEEL()
//	ON_WM_MOUSEHWHEEL()
END_MESSAGE_MAP()

// CBookingStatusView コンストラクション/デストラクション

CBookingStatusView::CBookingStatusView()
{
	m_Bst_mng = NULL;
	m_Rdn_sts = NULL;
	m_Rdn_rii = 0;
	m_Rdn_rdi = 0;
	m_Ope_mod = OPE_CHG|OPE_CRT;
}

CBookingStatusView::~CBookingStatusView()
{
	if ( m_Bst_mng ){
		if ( m_Bst_mng->ghd )	Fig_Trm( m_Bst_mng->ghd );
		if ( m_Bst_mng->hnd )	Fig_Trm( m_Bst_mng->hnd );
		Dst_Gph( m_Bst_mng->grt );
		Dst_Gph( m_Bst_mng->rot );
		Bst_Dps_Dat( m_Bst_mng->rom );
		free( m_Bst_mng );
	}
}

BOOL CBookingStatusView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~WS_SYSMENU;
	return CView::PreCreateWindow(cs);
}

// CBookingStatusView 描画

void CBookingStatusView::OnDraw(CDC* /*pDC*/)
{
	CBookingStatusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CDC		*pDC = GetDC();
	if ( m_Bst_mng )	Drw_Rif( pDC, this, m_Bst_mng );
	ReleaseDC( pDC );

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}


// CBookingStatusView 印刷

BOOL CBookingStatusView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CBookingStatusView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CBookingStatusView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}


// CBookingStatusView 診断

#ifdef _DEBUG
void CBookingStatusView::AssertValid() const
{
	CView::AssertValid();
}

void CBookingStatusView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBookingStatusDoc* CBookingStatusView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBookingStatusDoc)));
	return (CBookingStatusDoc*)m_pDocument;
}
#endif //_DEBUG


// CBookingStatusView メッセージ ハンドラ

// Create Event
BOOL CBookingStatusView::Create(					// ウィンドウ生成イベント
LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CWnd::Create(lpszClassName, lpszWindowName,
		dwStyle|WS_HSCROLL|WS_VSCROLL, rect, pParentWnd, nID, pContext);
}

// 初期化
void CBookingStatusView::OnInitialUpdate()
{
	((CChildFrame *)GetParent())->m_Viw_knd = VWK_BST;
	CView::OnInitialUpdate();

	GetMainFrm()->m_Bst_viw = this;

	m_pPms_mng = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->m_Dbs_obj;

	((CChildFrame *)GetParent())->ModifyStyle( WS_MAXIMIZEBOX, 0, TRUE );

    CMenu*  pSysMenu = ((CChildFrame *)GetParent())->GetSystemMenu(FALSE);
    pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);

	SetTimer( TEV_INI, TIM_INI, NULL );
}

// タイマーイベント
void CBookingStatusView::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer( nIDEvent );

	CDC		*pDC = GetDC();
	RECT	rct[1];

	if ( m_Bst_mng = (BST_MNG *)calloc( 1, sizeof(BST_MNG) ) ){
		m_Bst_mng->fnt = Bst_Cre_Fnt( _T("MS UI Gothic"), 10, FALSE, FALSE, pDC->m_hDC );
		m_Bst_mng->fhg = Bst_Get_Fsz( 10, pDC->m_hDC );
		m_Bst_mng->rnw = m_Bst_mng->fhg * 16 + 1;
		m_Bst_mng->rtw = m_Bst_mng->fhg * 26 + 1;
		m_Bst_mng->par = m_hWnd;
		m_Bst_mng->mng = m_pPms_mng;
		*m_Bst_mng->std = *m_Bst_mng->edd = *(m_pPms_mng->bdt);
		Add_Dte( m_Bst_mng->edd, 30 );
		if ( Bst_Get_Rom( m_Bst_mng->rom, this ) ){
			Bst_Mak_Sts( this, m_Bst_mng );
		}
		Mak_Gph( m_Bst_mng, AfxGetApp()->m_hInstance, m_hWnd, Cnt_Lnk(m_Bst_mng->rom) );
		GetClientRect( rct );
		SetScrollRenge( rct->right, rct->bottom );
	}

	Drw_Rif( pDC, this, m_Bst_mng );
	ReleaseDC( pDC );
	CView::OnTimer(nIDEvent);
}

void CBookingStatusView::OnSize(					// サイズ変更イベント
UINT nType, int cx, int cy) 
{
	SCROLLINFO	sci[1];

	CView::OnSize(nType, cx, cy);

	if ( m_Bst_mng && nType == SIZE_RESTORED ){
		SetScrollRenge( cx, cy );
		Siz_Gph( m_Bst_mng, cx, cy, Cnt_Lnk(m_Bst_mng->rom), TRUE );
//		CalcScrollRenge( cy );
//		m_Bst_mng->sys->top = -( GetScrollPos( SB_VERT ) * ( GPH_HIT + 1 ) );
		GetScrollInfo( SB_VERT, sci );
		m_Bst_mng->sys->top = -( sci->nPos * ( GPH_HIT + 1 ) );
		GetScrollInfo( SB_HORZ, sci );
		m_Bst_mng->sys->lft = sci->nPos * ( GPH_UNT + 1 );
		Fig_Set_Swn( m_Bst_mng->hnd, m_Bst_mng->sys, TRUE );
	}
}


void CBookingStatusView::OnVScroll(				// 垂直スクロール
UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int			dy;
	SCROLLINFO	sci[1];

	sci->cbSize = sizeof(sci);
	GetScrollInfo( SB_VERT, sci );

	switch( nSBCode ){
	case SB_PAGEUP:
	case SB_LINEUP:			dy = nSBCode==SB_LINEUP ? -1: -2;
							dy = (sci->nPos + dy) < 0 ? 0: dy;
							break;
	case SB_PAGEDOWN:
	case SB_LINEDOWN:		dy = nSBCode==SB_LINEDOWN ? 1: 2;
							dy = ( sci->nPos + dy ) > ( sci->nMax - sci->nPage + 1 ) ? 0: dy;
							break;
//	case SB_BOTTOM:			dy = ;
//							break;
//	case SB_TOP:			m_Bst_mng->sys->top = -( max * ( GPH_HIT + 1 ) );
//							nPos = max;
//							break;
	case SB_THUMBTRACK:	
	case SB_THUMBPOSITION:	dy = nPos - sci->nPos;
						break;
	}
	if ( nSBCode != SB_ENDSCROLL ){
		dy = max(-1 * sci->nPos, min(dy, sci->nMax - sci->nPos));
		if ( dy != 0 ){
			m_Bst_mng->sys->top += dy * ( GPH_HIT + 1) * -1;
			sci->nPos += dy ;
			sci->fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
			SetScrollInfo( SB_VERT, sci );
			Fig_Set_Swn( m_Bst_mng->hnd, m_Bst_mng->sys, TRUE );
			CDC		*pDC = GetDC();
			Drw_Rif( pDC, this, m_Bst_mng );
			ReleaseDC( pDC );
		}
	}
//	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CBookingStatusView::OnHScroll(				// 水平スクロール処理
UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int			dx;
	SCROLLINFO	sci[1];

	sci->cbSize = sizeof(sci);
	GetScrollInfo( SB_HORZ, sci );

	switch( nSBCode ){
	case SB_PAGELEFT:
	case SB_LINELEFT:		dx = nSBCode==SB_LINELEFT ? -1: -2;
							dx = (sci->nPos + dx) < 0 ? 0: dx;
							break;
	case SB_PAGERIGHT:
	case SB_LINERIGHT:		dx = nSBCode==SB_LINERIGHT ? 1: 2;
							dx = ( sci->nPos + dx ) > ( sci->nMax - sci->nPage + 1 ) ? 0: dx;
							break;
	//case SB_RIGHT:		m_Bst_mng->gsy->lft = nPos = 0;
	//						break;
	//case SB_LEFT:			m_Bst_mng->gsy->lft = nPos	= (int)(GPH_WID(m_Bst_mng->dys+1) - m_Bst_mng->gdv->wid);
	//						break;
	case SB_THUMBTRACK:	
	case SB_THUMBPOSITION:	dx = nPos - sci->nPos;
	}

	if ( nSBCode != SB_ENDSCROLL ){
		dx = max(-1 * sci->nPos, min(dx, sci->nMax - sci->nPos));
		if ( dx != 0 ){
			m_Bst_mng->sys->lft += dx * ( GPH_UNT + 1);
			m_Bst_mng->gsy->lft = m_Bst_mng->sys->lft;
			sci->nPos += dx ;
			sci->fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
			SetScrollInfo( SB_HORZ, sci );
			Fig_Set_Swn( m_Bst_mng->hnd, m_Bst_mng->sys, TRUE );
			Fig_Set_Swn( m_Bst_mng->ghd, m_Bst_mng->gsy, TRUE );
		}
	}
//	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CBookingStatusView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if ( nFlags & MK_SHIFT )	OnHScroll( zDelta<0 ? SB_LINERIGHT: SB_LINELEFT, 0, NULL );
	else						OnVScroll( zDelta<0 ? SB_LINEDOWN: SB_LINEUP, 0, NULL );

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CBookingStatusView::OnEraseBkgnd(			// 背景描画処理
CDC* pDC) 
{
	return CView::OnEraseBkgnd(pDC);
//	return( 0 );
}

// グラフイベント
LRESULT CBookingStatusView::OnGraphNotify( WPARAM wParam, LPARAM lParam )
{
	GPH_NTF			*ntf = (GPH_NTF *)wParam;
	GPH_RDY			*rdy = (GPH_RDY *)lParam;
	Dpn				*dpn = (Dpn *)lParam;
	LRESULT			rsl=FALSE;
	WCHAR			buf[64], edd[40];
	
	CMainFrame		*mfm = GetMainFrm();

	if ( ntf ){
		switch( ntf->ntf ){
		case GPN_DRAG:			if ( lParam ){
									if ( rdy->ned->yer > 0 ){
										swprintf_s( edd, memcnt(edd), _T(" - %d/%02d"), rdy->ned->mth, rdy->ned->day );
									}
									else{
										edd[0] = 0;
									}
									swprintf_s( buf, memcnt(buf), _T("%s: %d/%02d%s"),
										Fld_Ptr(rdy->rom->cod, FLD_WST), rdy->nsd->mth, rdy->nsd->day, edd );
									mfm->SetBookingStsPanel( buf );
									free( rdy );
								}
								break;
		case GPN_CREATE:		if ( lParam ){
									Bst_Rch_Crv( rdy );
									free( rdy );
								}
								break;
		case GPN_CHANGE:		if ( lParam ){
									Bst_Rsv_Chg( (BST_ROM_STS *)ntf->usr, rdy, this, m_pDbs_obj, m_Bst_mng );
									free( rdy );
								}
								break;
		case GPN_RBUTTONDOWN:	if ( lParam ){
									m_Rdn_rii = ntf->rii;
									m_Rdn_rdi = ntf->rdi;
									m_Rdn_sts = (BST_ROM_STS *)ntf->usr;
									if ( !!m_Rdn_rii )	Bst_Ctx_Mnu( this, ntf->msx, ntf->msy, ntf->usr, m_pPms_mng );
									free( dpn );
								}
								break;
		}
		free( ntf );
	}

	return( rsl );
}

// Notify from each view
LRESULT CBookingStatusView::OnViewNotify( WPARAM wParam, LPARAM lParam )
{
	VIW_NTF			*ntf = (VIW_NTF *)wParam;

	if ( ntf ){
		switch( ntf->ntf ){
		case VWN_RSV_CREATED:
		case VWN_RSV_CHANGED:	Bst_Upd_Gph( (Csr)(ntf->usr), this, m_Bst_mng );
								break;
		case VWN_RSV_DELETED:	break;
		case VWN_STS_CHANGED:	Bst_Upd_Sts( (int)(LONG_PTR)(ntf->usr), (int)lParam, this, m_Bst_mng );
								break;
		}
		free( ntf );
	}
	return( 0 );
}

// Windows command operation
BOOL CBookingStatusView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (pHandlerInfo == NULL){
		if (nCode == CN_COMMAND){
			switch ( nID ){
			case ID_BOOKINGSTATUS_RESERVATIONVIEW:
			case ID_BOOKINGSTATUS_CHECKIN:
			case ID_BOOKINGSTATUS_INHOUSESTATUS:
			case ID_BOOKINGSTATUS_CHECKOUT:					Bst_Rch_Viw( nID, m_Rdn_rii, m_Rdn_rdi );
															break;
			case ID_BOOKINGSTATUS_CHECKINDIRECT:			Bst_Cin_Dir( m_Rdn_sts, this, m_pDbs_obj, m_Bst_mng );
															break;
			case ID_BOOKINGSTATUS_CANCELRESERVATIONDIRECT:	Bst_Cxl_Dir( m_Rdn_sts, this, m_pDbs_obj, m_Bst_mng );
															break;
			}
		}
	}

	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// Non client area destroy
void CBookingStatusView::PostNcDestroy()			// 非クライアント領域破棄
{
	CView::PostNcDestroy();
}

/////////////////////////////////////////////////////////////////////////////
// CGrphView オペレーション
void CBookingStatusView::SetScrollRenge( int cx, int cy )	// setting scroll range
{
	SCROLLINFO	sci[1];

	sci->cbSize = sizeof(sci);
	sci->fMask= SIF_RANGE | SIF_PAGE;
	sci->nMin = 0;
	sci->nMax = Cnt_Lnk( m_Bst_mng->rom ) + 1;
	sci->nPage = cy / GPH_HIT;
	SetScrollInfo( SB_VERT, sci );
	sci->nMin = 0;
	sci->nMax = m_Bst_mng->dys * 2;
	sci->nPage = ( cx - ( m_Bst_mng->rnw+m_Bst_mng->rtw ) ) / ( GPH_UNT + 1 );
	SetScrollInfo( SB_HORZ, sci );
}

void CBookingStatusView::ReDraw( void )				// Re draw graphic
{
	Fig_Rdw( m_Bst_mng->hnd );							// 全再描画処理
}


void CBookingStatusView::OnPeriodChange(			// displaying prtiod changed
WCHAR	*fbf,										// period start date ( text )
WCHAR	*tbf )										// period endt date ( text )
{
	Day		pfr[1], pto[1];
	WCHAR	buf[80], *itm=NULL, *err=NULL;
	RECT	rct[1];

	if ( !!fbf && !!tbf ){
		if ( Str_Dte( pfr, fbf ) ){
			if ( Str_Dte( pto, tbf ) ){
				if ( Clc_Dys( pfr, pto ) >= 10 ){
					if ( Cmp_Dte(pfr,m_Bst_mng->std) != 0 || Cmp_Dte(pto,m_Bst_mng->edd) != 0 ){
						*m_Bst_mng->std = *pfr;
						*m_Bst_mng->edd = *pto;
						Bst_Pod_Chg( this, m_Bst_mng );
						GetClientRect( rct );
						SetScrollRenge( rct->right, rct->bottom );
					}
				}
				else{
					err = _T("Period end date should be more than 10 days from Period start date");
				}
			}
			else{
				itm = _T("Period end");
			}
		}
		else{
			itm = _T("Period start");
		}
		if ( !err && !!itm ){
			swprintf_s( buf, memcnt(buf), _T("%s date is wrong"), itm );
			err = buf;
		}
		if ( err ){
			MessageBox( err, NULL, MB_ICONEXCLAMATION|MB_OK );
		}
	}
	else{
		GetMainFrm()->SetBookingStsPeriod( m_Bst_mng->std, m_Bst_mng->edd );
	}
}
