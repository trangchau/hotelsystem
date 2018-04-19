// CopySelectDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "CopySelectDlg.h"
#include "..\Common\bsc_def.h"
#include ".\fnc\viw_cmn.h"



// CCopySelectDlg ダイアログ

static	WCHAR	*Grd_ttl[] =	{	{ _T("Cpy")			},
									{ _T("Destination")	} };

static	GRD_CLM	Grd_clm[] =		{	{  24, GAL_CTR, GCT_CHK,  12 },		// Cpy
									{ 192, GAL_LFT, GCT_NML,  12 } };	// Destination

#define	IDC_GRD_DTL		(8192)	// Copy destination selecting grid

static	Dst_	Fix_[] = {
	{ _T("Direct"),	VIA_DIRECT,	0	},	// Resrevation via direct WebSite
	{ _T("Web"),	VIA_WEBSITE,0	}	// Reservation via tel.
};

IMPLEMENT_DYNAMIC(CCopySelectDlg, CDialog)

CCopySelectDlg::CCopySelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCopySelectDlg::IDD, pParent)
{
	m_Sel_grd = NULL;
	m_pPms_mng = NULL;
	m_Dst_lst = NULL;
	m_Result = FALSE;
	m_Omt_bkp = 0;
	m_Omt_aii = 0;

}

CCopySelectDlg::~CCopySelectDlg()
{
	if ( m_Sel_grd )	delete	m_Sel_grd;
	if ( m_Dst_lst )	free( m_Dst_lst );
}

void CCopySelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCopySelectDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_BN_CLICKED(IDCANCEL, &CCopySelectDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CCopySelectDlg::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCopySelectDlg メッセージ ハンドラ

BOOL CCopySelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GRD_DEF			gdf[1];
	int				idx;
	RECT			rct[1];

	memset( gdf, 0, sizeof(gdf) );
	rct->left = 10;
	rct->top = 10;
	rct->right = 0;
	rct->bottom = 0;

	gdf->lnh = 20;
	gdf->vlc = 8;
	gdf->vcc = memcnt(Grd_clm);
	gdf->hbc = RGB(128,255,255);
	gdf->hfc = RGB(0,0,0);
	gdf->dbc = RGB(255,255,255);
	gdf->dfc = RGB(0,0,0);
	gdf->sbc = RGB(0,0,0);
	gdf->sfc = RGB(255,255,255);
	gdf->cct = gdf->vcc;
	gdf->clm = Grd_clm;
	gdf->sel = GSL_ROW;
	gdf->hdr = TRUE;
	gdf->fsa = TRUE;

	m_Sel_grd = new CSimpleGrid();
	m_Sel_grd->Create( gdf, *rct, IDC_GRD_DTL, this );
	for ( idx=0; idx<memcnt(Grd_ttl); idx++ ){
		m_Sel_grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}

	SetTimer( 1, 10, NULL );
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CCopySelectDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer( nIDEvent );

	Csr				csr=NULL;
	AGT_INF			*agt;
	int				idx, row=1;

	if ( m_Dst_lst = (Dst_ *)calloc( memcnt(Fix_)+Cnt_Lnk(m_pPms_mng->agl), sizeof(Dst_) ) ){
		for ( idx=0; idx<memcnt(Fix_); idx++ ){
			if ( ( (Fix_+idx)->bkp != m_Omt_bkp ) || ( (Fix_+idx)->aii != m_Omt_aii ) ){
				*(m_Dst_lst+row-1) = Fix_[idx];
				m_Sel_grd->AddRow(1);
				m_Sel_grd->SetCellText( 1, row, (Fix_+idx)->nam);
				m_Sel_grd->SetData( 1, row, (int)(LONG_PTR)(m_Dst_lst+row-1) );
				row++;
			}
		}
		for( idx=row-1; csr=Nxt_Lnk( m_pPms_mng->agl, csr ); ){
			agt = Rec_Of(AGT_INF, csr);
			if ( ( VIA_AGENT != m_Omt_bkp ) || ( DBS_INT_VAL(agt->id) != m_Omt_aii ) ){
				m_Sel_grd->AddRow(1);
				(m_Dst_lst+idx)->nam = DBS_WST_PTR(agt->nam);
				(m_Dst_lst+idx)->bkp = VIA_AGENT;
				(m_Dst_lst+idx)->aii = DBS_INT_VAL(agt->id);
				m_Sel_grd->SetCellText( 1, row, (m_Dst_lst+idx)->nam);
				m_Sel_grd->SetData( 1, row++, (int)(LONG_PTR)(m_Dst_lst+idx) );
				idx++;
			}
		}
		m_Sel_grd->Redraw();
	}
	else{
		m_Result = FALSE;
		PostMessage( WM_CLOSE, 0, 0 );
	}

	CDialog::OnTimer(nIDEvent);
}

LRESULT CCopySelectDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	WCHAR			*txt;
	int				chk;
	LRESULT			rsl=TRUE;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_EDITCOMPLETE:	if ( !!( txt = (WCHAR *)lParam) ){
									free( txt );
								}
								break;
		case GN_SELECTCHANGE:	break;
		case GN_LBUTTONCLICK:	if ( ntf->row>0 ){
									chk = m_Sel_grd->GetCheck(0, ntf->row);
									m_Sel_grd->SetCheck(0, ntf->row,!chk);
									(m_Dst_lst+ntf->row-1)->sel = !chk;
								}
								break;
		case GN_LBUTTONDBLCLICK:break;						// 左ボタンクリック
		case GN_QUERYCOMBODATA:	break;
		case GN_COMBOCHANGED:	break;
		case GN_QUERYINPUTTYPE:	if ( ntf->clm == 0 )	rsl=GCT_CHK;
								break;
		case GN_QUERYCOLORDATA:	break;
		}
		free( ntf );
	}

	return( rsl );
}

void CCopySelectDlg::OnBnClickedOk()
{
	Dst_		*dst;
	Csr			csr;
	int			idx, cnt=0;

	for ( idx=0; idx<m_Sel_grd->GetLineCount(FALSE); idx++ ){
		dst = (Dst_ *)(LONG_PTR)m_Sel_grd->GetData( 1, idx+1 );
		if ( dst->sel ){
			if ( csr = Add_Lnk( m_Sel_lst, NULL, sizeof(int) * 2 ) ){
				*Rec_Of(int, csr) = dst->bkp;
				*(Rec_Of(int, csr)+1) = dst->aii;
				m_Result = TRUE;
				++cnt;
			}
			else{
				m_Result = FALSE;
				Del_All( m_Sel_lst );
				break;
			}
		}
	}
	if ( !cnt ){
		MessageBox( _T("No selected"), NULL, MB_ICONEXCLAMATION|MB_OK );
	}
	else{
		OnOK();
	}
}

void CCopySelectDlg::OnBnClickedCancel()
{
	m_Result = FALSE;
	OnCancel();
}

BOOL CCopySelectDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}
