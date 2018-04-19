// ChargeDetailDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "ChargeDetailDlg.h"

#include "..\Common\dte_mng.h"
#include "..\Common\cmn_fnc.h"
#include ".\fnc\viw_cmn.h"

// CChargeDetailDlg ダイアログ
static	WCHAR	*Grd_ttl[] =	{	{ _T("Date")		},
									{ _T("Room Type")	},
									{ _T("Room Nuner")	},
									{ _T("List price")	},
									{ _T("Actual price")} };

static	GRD_CLM	Grd_clm[] =		{	{ 128, GAL_LFT, GCT_NML,  12 },		// Cpy
									{ 144, GAL_LFT, GCT_NML,  12 },		// List price
									{  96, GAL_LFT, GCT_NML,  12 },		// List price
									{  88, GAL_RGT, GCT_NML,  12 },		// List price
									{  88, GAL_RGT, GCT_NML,  12 } };	// Actual price

#define	IDC_GRD_DTL		(8192)	// Copy destination selecting grid

#define	CLM_DAT			0		// Data keeping column

#define	CLM_DTE			0		// Cpy
#define	CLM_RTP			1		// List price
#define	CLM_RNM			2		// List price
#define	CLM_LPR			3		// List price
#define	CLM_APR			4		// Actual price


IMPLEMENT_DYNAMIC(CChargeDetailDlg, CDialog)

CChargeDetailDlg::CChargeDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChargeDetailDlg::IDD, pParent)
{
	m_Dtl_grd = NULL;
	m_pPms_mng = NULL;
	m_Dtl_lst = NULL;
	m_Rcg_dat = NULL;
	memset( m_Rom_typ, 0, sizeof(m_Rom_typ) );
	memset( m_Pln_nam, 0, sizeof(m_Pln_nam) );
}

CChargeDetailDlg::~CChargeDetailDlg()
{
	if ( m_Dtl_grd )	delete m_Dtl_grd;
	if ( m_Rcg_dat )	free( m_Rcg_dat );
}

void CChargeDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChargeDetailDlg, CDialog)
	ON_WM_TIMER()
	ON_WM_GRIDNOTIFY()
	ON_BN_CLICKED(IDC_BUTTON_COPYALL, &CChargeDetailDlg::OnBnClickedButtonCopyall)
	ON_BN_CLICKED(IDOK, &CChargeDetailDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CChargeDetailDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CChargeDetailDlg メッセージ ハンドラ

BOOL CChargeDetailDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GRD_DEF			gdf[1];
	int				idx;
	RECT			rct[1];
	WINDOWPLACEMENT	wpc[1];

	memset( gdf, 0, sizeof(gdf) );

	GetDlgItem(IDC_STATIC_REQUESTEDROOMTYPE)->GetWindowPlacement( wpc );
	*rct = wpc->rcNormalPosition;
	//rct->left = 10;
	rct->top += 30;
	rct->right = 0;
	rct->bottom = 0;

	gdf->lnh = 20;
	gdf->vlc = 16;
	gdf->vcc = memcnt(Grd_clm);
	gdf->hbc = RGB(128,255,255);
	gdf->hfc = RGB(0,0,0);
	gdf->dbc = RGB(255,255,255);
	gdf->dfc = RGB(0,0,0);
	gdf->sbc = RGB(0,0,0);
	gdf->sfc = RGB(255,255,255);
	gdf->cct = gdf->vcc;
	gdf->clm = Grd_clm;
	gdf->sel = GSL_NON;
	gdf->hdr = TRUE;
	gdf->fsa = TRUE;

	m_Dtl_grd = new CSimpleGrid();
	m_Dtl_grd->Create( gdf, *rct, IDC_GRD_DTL, this );
	for ( idx=0; idx<memcnt(Grd_ttl); idx++ ){
		m_Dtl_grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}

	GetDlgItem(IDC_STATIC_REQUESTEDROOMTYPE)->SetWindowText( m_Rom_typ );
	GetDlgItem(IDC_STATIC_RATEPLANNAME)->SetWindowText( m_Pln_nam );
	SetTimer( 1, 10, NULL );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

static	void	dsp_row(
CSimpleGrid		*grd,
int				row,
RSV_RCG_DAT		*rcg,
PMS_MNG			*mng )
{
	WCHAR		buf[64], dbf[40], *ptr=NULL;
	WCHAR		*dow[]={ _T("Sun"), _T("Mon"), _T("Tue"), _T("Wed"), _T("Thu"), _T("Fri"), _T("Sat") };

	swprintf_s( buf, memcnt(buf), _T("%s(%s)"),
			Edt_Dte( dbf, memcnt(dbf), DBS_DAY_PTR(rcg->rcg->udt)),
			dow[Get_Dow(DBS_DAY_PTR(rcg->rcg->udt))] );
	grd->SetCellText( CLM_DTE, row, buf );
	grd->SetCellText( CLM_RTP, row, DBS_WST_PTR(rcg->etn) );
	grd->SetCellText( CLM_RNM, row, DBS_WST_PTR(rcg->eqc) );
	if ( !Nul_Fld(rcg->prc, FLD_DBL) ){
		ptr = Cma_Edt( DBS_DBL_VAL(rcg->prc), mng->bsc->adp );
		grd->SetCellText( CLM_LPR, row, ptr ? ptr: _T("memerr") );
	}
	else{
		grd->SetCellText( CLM_LPR, row, _T("") );
	}
	if ( !Nul_Fld(rcg->rcg->amt, FLD_DBL) ){
		if ( ptr )	free( ptr );
		ptr = Cma_Edt( DBS_DBL_VAL(rcg->rcg->amt), mng->bsc->adp );
		grd->SetCellText( CLM_APR, row, ptr ? ptr: _T("memerr") );
		if ( ptr )		free( ptr );
	}
	else{
		grd->SetCellText( CLM_APR, row, ptr ? ptr: _T("") );
		if ( ptr )		free( ptr );
	}
	grd->SetData( CLM_DAT, row, (int)(LONG_PTR)rcg );
	grd->SetEnable( -1, row, TRUE );
}

void CChargeDetailDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer( nIDEvent );

	Csr			csr=NULL;
	RSV_RCG_DAT	*rcg;
	int			row=1;

	m_Rcg_dat = (RSV_RCG_DAT *)calloc(Cnt_Lnk( m_Dtl_lst ), sizeof(RSV_RCG_DAT));

	if ( m_Rcg_dat ){
		m_Dtl_grd->AddRow( Cnt_Lnk( m_Dtl_lst ) );
		m_Dtl_grd->InputEnable( TRUE );

		while( csr = Nxt_Lnk( m_Dtl_lst, csr ) ){
			*(rcg = (m_Rcg_dat+row-1) ) = *Rec_Of(RSV_RCG_DAT, csr);
			dsp_row( m_Dtl_grd, row++, rcg, m_pPms_mng );
		}
	}
	else{
		MessageBox( _T("Insuffcient memory"), NULL, MB_ICONSTOP|MB_OK );
		OnClose();
	}
}

// Grid operation notify
LRESULT CChargeDetailDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	WCHAR			*txt, *ptr;
	LRESULT			rsl=TRUE;
	RSV_RCG_DAT		*rcg;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_EDITCOMPLETE:	if ( !!( txt = (WCHAR *)lParam) ){
									if ( ntf->clm==CLM_APR ){
										rcg = (RSV_RCG_DAT *)(LONG_PTR)m_Dtl_grd->GetData( CLM_DAT, ntf->row );
										if ( rcg ){
											m_Dtl_grd->SetCellText( CLM_APR, ntf->row, txt );
											if ( !!wcslen(txt) && ( DBS_DBL_VAL(rcg->prc) != _wtof( Prg_Cma( txt ) ) ) ){
												Set_Nul(rcg->rcg->amt, FLD_DBL, FALSE);
												*DBS_DBL_PTR(rcg->rcg->amt) = _wtof( Prg_Cma( txt ) );
											}
											else{
												if ( ptr = Cma_Edt( DBS_DBL_VAL(rcg->prc), m_pPms_mng->bsc->adp ) ){
													m_Dtl_grd->SetCellText( CLM_APR, ntf->row, ptr );
													free( ptr );
												}
												Set_Nul(rcg->rcg->amt, FLD_DBL, TRUE);
											}
										}
									}
									free( txt );
								}
								break;
		case GN_LBUTTONCLICK:	if ( ntf->cid==IDC_GRD_DTL ){
								}
								break;
		case GN_LBUTTONDBLCLICK:if ( ntf->cid == IDC_GRD_DTL ){
								}
								break;
		case GN_RBUTTONCLICK:	break;
		case GN_QUERYCOMBODATA:	break;
		case GN_COMBOCHANGED:	break;
		case GN_QUERYINPUTTYPE:	switch( ntf->clm ){
								case CLM_APR:	rsl = GCT_CED;	break;
								default:		rsl = GCT_NML;	break;
								}
								break;
		case GN_QUERYCOLORDATA:	break;
		case GN_QUERYCMEPARAM:	rsl = MAKELPARAM(CEDT_ROUND_UP, m_pPms_mng->bsc->adp);
								break;
		}
		free( ntf );
	}

	return( rsl );
}

// CopyAll button click
void CChargeDetailDlg::OnBnClickedButtonCopyall()
{
	RSV_RCG_DAT	*src, *rcg;
	int			row=2;

	src = (RSV_RCG_DAT *)(LONG_PTR)m_Dtl_grd->GetData( CLM_DAT, 1 );
	for ( row=2; row<=m_Dtl_grd->GetLineCount(FALSE); row++ ){
		rcg = (RSV_RCG_DAT *)(LONG_PTR)m_Dtl_grd->GetData( CLM_DAT, row );
		DBS_DBL_CPY(rcg->rcg->amt, src->rcg->amt);
		dsp_row( m_Dtl_grd, row, rcg, m_pPms_mng );
	}
}

static	int		chg_exs(			// confirmation or copy changed
Csr			org,					// original data
RSV_RCG_DAT	*rcg,					// editting data
int			cpy )					// copy data
{
	Csr			csr=NULL;

	for( ; csr=Nxt_Lnk( org, csr ); rcg++ ){
		if ( cpy ){
			DBS_DBL_CPY(Rec_Of(RSV_RCG_DAT, csr)->rcg->amt, rcg->rcg->amt);
		}
		else{
			if ( Nul_Fld(Rec_Of(RSV_RCG_DAT, csr)->rcg->amt, FLD_DBL) != Nul_Fld(rcg->rcg->amt, FLD_DBL) ){
				break;
			}
			else if ( !Rec_Of(RSV_RCG_DAT, csr)->rcg->amt, FLD_DBL ){
				if ( DBS_DBL_VAL(Rec_Of(RSV_RCG_DAT, csr)->rcg->amt) != DBS_DBL_VAL(rcg->rcg->amt) ){
					break;
				}
			}
		}
	}

	return( !!csr );
}

// Close with save button click
void CChargeDetailDlg::OnBnClickedOk()
{
	chg_exs( m_Dtl_lst, m_Rcg_dat, TRUE );
	OnOK();
}

// close button click
void CChargeDetailDlg::OnBnClickedCancel()
{
	DWORD		ans=IDYES;

	if ( chg_exs( m_Dtl_lst, m_Rcg_dat, FALSE ) ){
		ans = MessageBox( _T("Are you sure you want to discard your changes?"), NULL, MB_ICONQUESTION|MB_YESNO );
	}
	if ( ans==IDYES )	OnCancel();
}

BOOL CChargeDetailDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}
