// GrphView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "..\Common\bsc_def.h"
//#include "..\Common\dbs_def.h"
#include "..\Graph\fig_mnp.h"
#include "..\Graph\gph_dsp.h"
#include ".\fnc\sdl_fnc.h"
#include ".\fnc\gph_sub.h"
#include ".\fnc\viw_cmn.h"
//#include "kosu.h"
#include "GrphView.h"
#include "MainFrm.h"
#include "HotelPMSDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrphView

IMPLEMENT_DYNCREATE(CGrphView, CView)

CGrphView::CGrphView()
{
	CMainFrame	*frm = GetMainFrm();

	m_Bst_mng = NULL;
	m_OpeMode = OPE_CHG|OPE_CRT;
//	m_Mng_dat = frm->m_Mng_dat;
}

CGrphView::~CGrphView()
{
}

BEGIN_MESSAGE_MAP(CGrphView, CView)
	//{{AFX_MSG_MAP(CGrphView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrphView 描画

void CGrphView::OnDraw(CDC* pDC)
{
	CDocument*	pDoc = GetDocument();
	RECT		rct[1];

	if ( !m_Bst_mng ){
		if ( m_Bst_mng = (BST_MNG *)calloc( 1, sizeof(BST_MNG) ) ){
			m_Bst_mng->fnt = Cre_Fnt( _T("ＭＳ ゴシック"), 10, FALSE, FALSE, pDC->m_hDC );
			m_Bst_mng->fhg = Get_Fsz( 10, pDC->m_hDC );
			m_Bst_mng->rnw = m_Bst_mng->fhg * 30 + 1;
			m_Bst_mng->rtw = 8 * m_Bst_mng->fhg * 2 + 1;
// 2018/01/01			m_Bst_mng->mng = m_Mng_dat;
//			Mak_Gph( m_Bst_mng, AfxGetApp()->m_hInstance, m_hWnd, Cnt_Lnk(m_Mng_dat->mdt->dtl) );
			Tim_Gph( m_Bst_mng );
			GetClientRect( rct );
			CalcScrollRenge( rct->bottom - rct->top + 1 );
		}
	}
	Drw_Rif( pDC, this, m_Bst_mng );
}

/////////////////////////////////////////////////////////////////////////////
// CGrphView 診断

#ifdef _DEBUG
void CGrphView::AssertValid() const
{
	CView::AssertValid();
}

void CGrphView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGrphView オペレーション

void CGrphView::Refresh( int bReset )			// 表示内容一新
{
	if ( bReset )	Rst_Gph( m_hWnd, m_Bst_mng );
	else			Tim_Gph( m_Bst_mng );
	InvalidateRect( NULL, TRUE );
//	2018/01/01
//	((CKosuDoc *)GetDocument())->SetCustomTitle( GPH_WIN );
}

void CGrphView::CalcScrollRenge(			// スクロール状態設定
int			cy )							// 高さ
{
	int		vsc, dx;

	if ( m_Bst_mng ){
		SetScrollRange( SB_HORZ, 0, (int)(GPH_WID - m_Bst_mng->dvc->wid), TRUE );
// 2018/01/01
//		dx = Cnt_Lnk( m_Bst_mng->mng->mdt->dtl ) * ( GPH_HIT + 1 );
		vsc = dx - ( cy - GAG_HGT );
		if ( vsc > 0 )	vsc = vsc / ( GPH_HIT+1 ) + ( vsc % ( GPH_HIT+1 ) ? 1: 0 ); 
		else			vsc = 0;
		SetScrollRange( SB_VERT, 0, vsc, TRUE );
	}
}


/////////////////////////////////////////////////////////////////////////////
// CGrphView メッセージ ハンドラ

BOOL CGrphView::Create(					// ウィンドウ生成イベント
LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	CHotelPMSDoc	*pDoc = (CHotelPMSDoc*)pContext->m_pCurrentDoc;

// 2018/01/01
//	if (pDoc)	pDoc->m_Vew_Typ = VIEW_TYPE_GRAPH;
	return CWnd::Create(lpszClassName, lpszWindowName,
		dwStyle|WS_HSCROLL|WS_VSCROLL, rect, pParentWnd, nID, pContext);
}

void CGrphView::OnSize(					// サイズ変更イベント
UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if ( m_Bst_mng && nType == SIZE_RESTORED ){
// 2018/01/01
//		Siz_Gph( m_Bst_mng, cx, cy, Cnt_Lnk(m_Mng_dat->mdt->dtl), TRUE );
		CalcScrollRenge( cy );
		m_Bst_mng->sys->top = -( GetScrollPos( SB_VERT ) * ( GPH_HIT + 1 ) );
		Chg_Pos( m_Bst_mng->hnd, m_Bst_mng->sys );
	}
}

void CGrphView::OnVScroll(				// 垂直スクロール
UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int			min, max, mov;

	if ( nSBCode != SB_THUMBTRACK )	nPos = GetScrollPos( SB_VERT );

	GetScrollRange( SB_VERT, &min, &max );

	switch( nSBCode ){
	case SB_PAGELEFT:
	case SB_LINELEFT:	mov = nSBCode==SB_LINELEFT ? 1: 4;
						if ( (int)nPos >= mov ){
							m_Bst_mng->sys->top -= -( mov * ( GPH_HIT + 1 ) );
							nPos -= mov;
						}
						else{
							m_Bst_mng->sys->top = nPos = 0;
						}
						break;
	case SB_PAGERIGHT:
	case SB_LINERIGHT:	mov = nSBCode==SB_LINERIGHT ? 1: 4;
						if ( (int)nPos+mov <= max ){
							m_Bst_mng->sys->top += -( mov * ( GPH_HIT + 1 ) );
							nPos += mov;
						}
						else{
							m_Bst_mng->sys->top = -( max * ( GPH_HIT + 1 ) );
							nPos = max;
						}
						break;
	case SB_RIGHT:		m_Bst_mng->sys->top = nPos = 0;
						break;
	case SB_LEFT:		m_Bst_mng->sys->top = -( max * ( GPH_HIT + 1 ) );
						nPos = max;
						break;
	case SB_THUMBTRACK:	m_Bst_mng->sys->top = -( (int)nPos * ( GPH_HIT + 1 ) );
						break;
	}
	Chg_Pos( m_Bst_mng->hnd, m_Bst_mng->sys );
	SetScrollPos( SB_VERT, nPos, TRUE );
	InvalidateRect( NULL, FALSE );
//	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CGrphView::OnHScroll(				// 水平スクロール処理
UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int			max, mov;

	if ( nSBCode != SB_THUMBTRACK )	nPos = GetScrollPos( SB_HORZ );

	switch( nSBCode ){
	case SB_PAGELEFT:
	case SB_LINELEFT:	mov = GPH_UNT * ( nSBCode==SB_LINELEFT ? 1: 4 );
						if ( m_Bst_mng->gsy->lft > mov ){
							m_Bst_mng->gsy->lft -= mov, nPos -= mov;
						}
						else{
							m_Bst_mng->gsy->lft = nPos = 0;
						}
						break;
	case SB_PAGERIGHT:
	case SB_LINERIGHT:	mov = GPH_UNT * ( nSBCode==SB_LINERIGHT ? 1: 4 );
						max = (int)(GPH_WID - m_Bst_mng->gdv->wid);
						if ( m_Bst_mng->gsy->lft < max-mov ){
							m_Bst_mng->gsy->lft += mov, nPos += mov;
						}
						else{
							m_Bst_mng->gsy->lft = max;
							nPos = (int)m_Bst_mng->gsy->lft;
						}
						break;
	case SB_RIGHT:		m_Bst_mng->gsy->lft = nPos = 0;
						break;
	case SB_LEFT:		m_Bst_mng->gsy->lft = nPos	= (int)(GPH_WID - m_Bst_mng->gdv->wid);
						break;
	case SB_THUMBTRACK:	m_Bst_mng->gsy->lft = nPos;
						break;
	}
	m_Bst_mng->sys->lft = m_Bst_mng->gsy->lft;
	Chg_Pos( m_Bst_mng->ghd, m_Bst_mng->gsy );
	Chg_Pos( m_Bst_mng->hnd, m_Bst_mng->sys );
	SetScrollPos( SB_HORZ, nPos, TRUE );
//	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CGrphView::OnEraseBkgnd(			// 背景描画処理
CDC* pDC) 
{
//	return CView::OnEraseBkgnd(pDC);
	return( 0 );
}

BOOL CGrphView::OnCmdMsg(				// コマンドメッセージ処理
UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	int		exc = FALSE, ope = OPE_CHG;
	static	int	fst=TRUE;

	if (pHandlerInfo == NULL){
		if (nCode == CN_COMMAND){
/*	2018/01/01
			switch ( nID ){
			case IDI_GRPINP:	ope |= OPE_CRT;		exc = TRUE;	break;
			case IDI_GRPCUT:	ope |= OPE_CUT;		exc = TRUE;	break;
			case IDI_GRPSEL:	ope |= OPE_MOV;		exc = TRUE;	break;
			case IDI_GRPDEL:	Ers_Tim(m_Bst_mng);	exc = TRUE;	break;
			}
*/
			if ( ope != OPE_CHG )	Chg_Mod( ope, RBR_LIN );
			m_OpeMode = ope;
		}
		else if (nCode == CN_UPDATE_COMMAND_UI){
/*	2018/01/01
			if ( nID==IDI_GRPINP || nID==IDI_GRPCUT || nID==IDI_GRPSEL || nID==IDI_GRPDEL ){
				((CCmdUI*)pExtra)->Enable( TRUE );
			}
			if ( nID==IDI_GRPINP )	((CCmdUI*)pExtra)->SetCheck( !!(m_OpeMode&OPE_CRT) );
			if ( nID==IDI_GRPCUT )	((CCmdUI*)pExtra)->SetCheck( !!(m_OpeMode&OPE_CUT) );
			if ( nID==IDI_GRPSEL )	((CCmdUI*)pExtra)->SetCheck( !!(m_OpeMode&OPE_MOV) );
*/
			exc = TRUE;
		}
	}
	return ( exc ? exc: CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) );
}

void CGrphView::PostNcDestroy()			// 非クライアント領域破棄
{
	if ( m_Bst_mng ){
		if ( m_Bst_mng->ghd )	Trm_Dsp( m_Bst_mng->ghd );
		if ( m_Bst_mng->hnd )	Trm_Dsp( m_Bst_mng->hnd );
		Del_All( m_Bst_mng->grt );
		Del_All( m_Bst_mng->rot );
		free( m_Bst_mng );
	}
	CView::PostNcDestroy();
}
