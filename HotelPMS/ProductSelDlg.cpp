// ProductSelDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "ProductSelDlg.h"
#include "MainFrm.h"
#include ".\fnc\mfm_sql.h"
#include ".\fnc\viw_cmn.h"


// CProductSelDlg ダイアログ

IMPLEMENT_DYNAMIC(CProductSelDlg, CDialog)

static	WCHAR	*Grd_ttl[] =	{	{ _T("Sel")			},
									{ _T("Description")	},
									{ _T("Unit")		},
									{ _T("Qty")			},
									{ _T("Amount")		} };

static	GRD_CLM	Grd_clm[] =		{	{  24, GAL_LFT, GCT_CHK,  12 },		// Select
									{ 186, GAL_LFT, GCT_NML,  12 },		// Description
									{  80, GAL_RGT, GCT_NML,  12 },		// Unit
									{  80, GAL_RGT, GCT_EDT/*GCT_CED*/,  12 },		// Qty
									{ 128, GAL_RGT, GCT_NML,  12 } };	// Amount

#define	IDC_GRD_PDT		(8192)	// Copy destination selecting grid

#define	CLM_DAT			1		// Data keeping column

#define	CLM_SEL			0		// Select
#define	CLM_DSC			1		// Description
#define	CLM_UNT			2		// Unit
#define	CLM_QTY			3		// Qty
#define	CLM_AMT			4		// Amount

CProductSelDlg::CProductSelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProductSelDlg::IDD, pParent)
{
	m_Ope_cod = NULL;
	m_Brc_cod = NULL;
	m_Fcl_cod = NULL;

	m_Pdt_grd = NULL;
	m_Pdt_rot = NULL;
	m_Cat_cnt = 0;
	m_Tab_idx = -1;
	m_Sel_rot = NULL;
}

CProductSelDlg::~CProductSelDlg()
{
	DisposeProductList();
	Del_All( m_Sel_rot );
	if ( m_Pdt_grd )	delete	m_Pdt_grd;
}

void CProductSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void	CProductSelDlg::DisposeProductList( void )
{
	int		idx;

	if ( m_Pdt_rot ){
		for ( idx=0; idx<m_Cat_cnt; idx++ ){
			Del_All( m_Pdt_rot+idx );
		}
		free( m_Pdt_rot );
	}
	m_Pdt_rot = NULL;
	m_Cat_cnt = 0;
}

int	 CProductSelDlg::InitData( WCHAR *opc, WCHAR *brc, WCHAR *fcc )
{
	m_Ope_cod = opc;
	m_Brc_cod = brc;
	m_Fcl_cod = fcc;

	return( TRUE );
}

void CProductSelDlg::DispProduct( CSH_PDT_LST *pdt, int row )
{
	double		amt;
	WCHAR		buf[40], *ptr;

	m_Pdt_grd->SetCheck( CLM_SEL, row, pdt->qty>0.0 );
	m_Pdt_grd->SetCellText( CLM_DSC, row, EDT_DBF_WST(pdt->pdt->nam,_T("")) );
	amt = !Nul_Fld(pdt->pdt->prc,FLD_DBL) ? DBS_DBL_VAL(pdt->pdt->prc): 0.0;
	Viw_Prc_Edt( buf, memcnt(buf), amt, FALSE, m_pPms_mng );
	m_Pdt_grd->SetCellText( CLM_UNT, row, buf );
	ptr = Cma_Edt( pdt->qty, 0 );
	m_Pdt_grd->SetCellText( CLM_QTY, row, !!ptr ? ptr: _T("") );
	if ( ptr )	free( ptr );
	amt = amt * pdt->qty;
	Viw_Prc_Edt( buf, memcnt(buf), amt, FALSE, m_pPms_mng );
	m_Pdt_grd->SetCellText( CLM_AMT, row, buf );
}

void CProductSelDlg::DispProductList()
{
	Csr			csr=NULL;
	CSH_PDT_LST	*pdt;
	int			row=1;

	m_Pdt_grd->ResetContent();
	m_Pdt_grd->AddRow( Cnt_Lnk( m_Pdt_rot+m_Tab_idx ) );
	m_Pdt_grd->InputEnable( TRUE );
	while ( csr=Nxt_Lnk( m_Pdt_rot+m_Tab_idx, csr ) ){
		pdt = Rec_Of(CSH_PDT_LST, csr);
		DispProduct( pdt, row );
		m_Pdt_grd->SetEnable( CLM_QTY, row, TRUE );
		m_Pdt_grd->SetData( CLM_DAT, row++, (int)(LONG_PTR)pdt );
	}
}


int	 CProductSelDlg::MakeProductList()
{
	int			chk=TRUE, cat=0, idx=0;
	Lnk			rot[1], *csr=NULL;
	CSH_PDT_LST	*pdt;
	WCHAR		*erm=NULL;
	CTabCtrl	*tab = (CTabCtrl *)GetDlgItem(IDC_TAB_CATEGORY);

	DisposeProductList();

	memset( rot, 0, sizeof(rot) );
	tab->DeleteAllItems();

	if ( chk = Csh_Get_Pdt( rot, m_Ope_cod, m_Brc_cod, m_Fcl_cod, &erm, m_pDbs_obj, m_pPms_mng ) ){
		while ( csr = Nxt_Lnk( rot, csr ) ){
			pdt = Rec_Of(CSH_PDT_LST, csr);
			if ( cat != DBS_INT_VAL(pdt->dci) ){
				cat = DBS_INT_VAL(pdt->dci);
				tab->InsertItem( m_Cat_cnt, EDT_DBF_WST(pdt->dcn,_T("")) );
				m_Cat_cnt++;
			}
		}
		if ( chk = !!( m_Pdt_rot = (Csr)calloc( m_Cat_cnt, sizeof(Lnk) ) ) ){
			cat = 0;
			while( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
				pdt = Rec_Of(CSH_PDT_LST, csr);
				if ( cat != DBS_INT_VAL(pdt->dci)){
					if ( !!cat )	idx++;
					cat = DBS_INT_VAL(pdt->dci);
				}
				chk = !!Add_Lnk( m_Pdt_rot+idx, pdt, sizeof(CSH_PDT_LST) );
			}
		}
	}
	if ( chk ){
		m_Tab_idx = 0;
		DispProductList();
	}
	else{
		if ( erm ){
			MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
		DisposeProductList();
	}

	Del_All( rot );
	return( chk );
}


BEGIN_MESSAGE_MAP(CProductSelDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CATEGORY, &CProductSelDlg::OnTcnSelchangeTabCategory)
	ON_BN_CLICKED(IDCANCEL, &CProductSelDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CProductSelDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CProductSelDlg メッセージ ハンドラ

BOOL CProductSelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GRD_DEF			gdf[1];
	int				idx;
	WINDOWPLACEMENT	wpc[1];

	m_pPms_mng = ((CMainFrame *)GetParent())->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)GetParent())->m_Dbs_obj;

	memset( gdf, 0, sizeof(gdf) );

	GetDlgItem(IDC_TAB_CATEGORY)->GetWindowPlacement( wpc );
	wpc->rcNormalPosition.left += 2;
	wpc->rcNormalPosition.top += 24;

	gdf->lnh = 20;
	gdf->vlc = 15;
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

	m_Pdt_grd = new CSimpleGrid();
	m_Pdt_grd->Create( gdf, wpc->rcNormalPosition, IDC_GRD_PDT, this );
	for ( idx=0; idx<memcnt(Grd_ttl); idx++ ){
		m_Pdt_grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}

	MakeProductList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// グリッドイベント
LRESULT CProductSelDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;
	int				row;
	double			qty;
	CSH_PDT_LST		*pdt=NULL;
	WCHAR			buf[50], *txt;

	if ( ntf ){
		if ( ( row = ntf->row ) > 0 ){
			pdt = (CSH_PDT_LST *)(LONG_PTR)m_Pdt_grd->GetData( CLM_DAT, row );
		}
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:
		case GN_RBUTTONCLICK:	row = ntf->row;
								if ( pdt && ntf->clm!=CLM_QTY ){
									m_Pdt_grd->GetCellText( buf, memcnt(buf), CLM_QTY, row );
									Prg_Cma( buf );
									qty = _wtof( buf );
									qty += ( ntf->ntf==GN_LBUTTONCLICK )? 1: -1;
									if ( qty >= 0.0 ){
										if ( txt = Cma_Edt( qty, 0 ) ){
											m_Pdt_grd->SetCellText( CLM_QTY, row, txt );
											free( txt );
											pdt->qty = qty;
											DispProduct( pdt, row );
										}
									}
								}
								break;
		case GN_EDITCOMPLETE:	if ( !!( txt = (WCHAR *)lParam) ){
									if ( pdt ){
										if ( ntf->clm==CLM_QTY ){
											Prg_Cma( txt );
											qty = _wtof( txt );
											if ( qty >= 0.0 ){
												pdt->qty = qty;
												DispProduct( pdt, row );
											}
										}
										free( txt );
									}
								}
								break;
		case GN_QUERYINPUTTYPE:	switch( ntf->clm ){
								case CLM_SEL:	rsl = GCT_CHK;	break;
								case CLM_QTY:	rsl = GCT_EDT/*GCT_CED*/;	break;
								default:		rsl = GCT_NML;	break;
								}
								break;
		}
		free( ntf );
	}

	return( rsl );
}

// Tab select change event
void CProductSelDlg::OnTcnSelchangeTabCategory(NMHDR *pNMHDR, LRESULT *pResult)
{
    int        idx;
    
    idx = ((CTabCtrl *)GetDlgItem(IDC_TAB_CATEGORY))->GetCurSel();
	if ( m_Tab_idx != idx ){
		m_Tab_idx = idx;
		DispProductList();
	}

	*pResult = 0;
}

// Close  button click
void CProductSelDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OnCancel();
}

// Close with reflect button click
void CProductSelDlg::OnBnClickedOk()
{
	int				idx;
	Csr				csr=NULL;
	CSH_PDT_LST		*pdt;

	if ( !!m_Sel_rot ){
		Del_All( m_Sel_rot );
		for ( idx=0; idx<m_Cat_cnt; idx++ ){
			while ( csr=Nxt_Lnk( m_Pdt_rot+idx, csr ) ){
				pdt = Rec_Of(CSH_PDT_LST, csr);
				if ( pdt->qty> 0.0 ){
					if ( !Add_Lnk( m_Sel_rot, pdt, sizeof(CSH_PDT_LST) ) ){
						break;
					}
				}
			}
			if ( !!csr )	break;
		}
		if ( !!csr )	Del_All( m_Sel_rot );
	}
	OnOK();
}

BOOL CProductSelDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}
