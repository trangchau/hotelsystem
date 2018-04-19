// CurrencyRateDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MainFrm.h"
#include "CurrencyRateDlg.h"
#include ".\fnc\csh_sql.h"
#include ".\fnc\viw_cmn.h"


// CCurrencyRateDlg ダイアログ

static	WCHAR	*Grd_ttl[] =	{	{ _T("CurrencyName")},
									{ _T("Code")	},
									{ _T("Rate")	}};

static	GRD_CLM	Grd_clm[] =		{	{ 148, GAL_LFT, GCT_NML,  12 },		// Currency
									{  80, GAL_LFT, GCT_NML,  12 },		// Code
									{ 160, GAL_LFT, GCT_NML,  14 },};	// Rate

#define	IDC_GRD_CRC		(8192)	// Copy destination selecting grid

#define	CLM_DAT			0		// Data keeping column

#define	CLM_NAM			0		// Currency
#define	CLM_COD			1		// Code
#define	CLM_RTE			2		// Rate

IMPLEMENT_DYNAMIC(CCurrencyRateDlg, CDialog)

CCurrencyRateDlg::CCurrencyRateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCurrencyRateDlg::IDD, pParent)
{
	m_Ccv_grd = NULL;
	m_pPms_mng = NULL;
	m_pDbs_obj = NULL;
	m_Ope_cod = NULL;
	memset( m_Ccv_rot, 0, sizeof(m_Ccv_rot) );
}

CCurrencyRateDlg::~CCurrencyRateDlg()
{
	if ( m_Ccv_grd )	delete m_Ccv_grd;
	Del_All( m_Ccv_rot );
}

void CCurrencyRateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CCurrencyRateDlg::SetupComponent()
{
	int				idx;
	WINDOWPLACEMENT	wpc[1];
	//RECT			rct[1];
	GRD_DEF			gdf[1];
/*
	struct	{
		CWnd	*wnd;
		int		lmt;
		DWORD	cid;
	}Cpn_[] = {
		{ NULL,       30, IDC_EDIT_CATEGORYNAME	},
		{ NULL,        3, IDC_EDIT_CURRENCYCODE	},
		{ NULL,       14, IDC_EDIT_EXCHANGERATE	},
		{ NULL,        2, IDC_EDIT_ADPD			}};

	for ( idx=0; idx<memcnt(Cpn_); idx++ ){
		if ( !!Cpn_[idx].wnd ){
			Cpn_[idx].wnd->SubclassDlgItem(Cpn_[idx].cid, this);
		}
		if ( Cpn_[idx].lmt!=-1 )	((CEdit *)GetDlgItem(Cpn_[idx].cid))->SetLimitText( Cpn_[idx].lmt );
	}
*/
	memset( gdf, 0, sizeof(gdf) );

	GetDlgItem(IDC_STATIC_BASECURRENCY)->GetWindowPlacement( wpc );
	wpc->rcNormalPosition.top += 30;

	gdf->lnh = 20;
	gdf->vlc = 6;
	gdf->vcc = memcnt(Grd_clm);
	gdf->hbc = RGB(128,255,255);
	gdf->hfc = RGB(0,0,0);
	gdf->dbc = RGB(255,255,255);
	gdf->dfc = RGB(0,0,0);
	gdf->sbc = RGB(0,0,0);
	gdf->sfc = RGB(255,255,255);
	gdf->cct = gdf->vcc;
	gdf->clm = Grd_clm;
	gdf->sel = GSL_CLM;
	gdf->hdr = TRUE;
	gdf->fsa = TRUE;

	m_Ccv_grd = new CSimpleGrid();
	m_Ccv_grd->Create( gdf, wpc->rcNormalPosition, IDC_GRD_CRC, this );
	for ( idx=0; idx<memcnt(Grd_ttl); idx++ ){
		m_Ccv_grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}
}

int	 CCurrencyRateDlg::InitData( WCHAR *opc, WCHAR *bcd, WCHAR *fcd )
{
	int			chk=TRUE, row=1;
	Csr			csr=NULL;
	CRC_CVS		*ccv, key[1];
	WCHAR		*erm=NULL, buf[1024];

	m_Ope_cod = opc;
	m_Brc_cod = bcd;
	m_Fcl_cod = fcd;

	Del_All( m_Ccv_rot );
	m_Ccv_grd->ResetContent();

	memset( key, 0, sizeof(key) );
	DBS_WST_SET(key->opc, opc);
	DBS_WST_SET(key->brc, bcd);
	DBS_WST_SET(key->fcc, fcd);

	if ( chk = Csh_Get_Ccv( m_Ccv_rot, key, &erm, m_pDbs_obj, m_pPms_mng ) ){
		m_Ccv_grd->AddRow( Cnt_Lnk( m_Ccv_rot ) - 1 );
		while ( csr=Nxt_Lnk( m_Ccv_rot, csr ) ){
			ccv = Rec_Of(CRC_CVS, csr);
			if ( !DBS_INT_VAL(ccv->isb) ){
				m_Ccv_grd->SetCellText( CLM_NAM, row, EDT_DBF_WST(ccv->nam,_T("")) );
				m_Ccv_grd->SetCellText( CLM_COD, row, EDT_DBF_WST(ccv->crc,_T("")) );
				swprintf_s( buf, memcnt(buf), _T("%.12f"), DBS_DBL_VAL(ccv->rte) );
				m_Ccv_grd->SetCellText( CLM_RTE, row, buf );
				m_Ccv_grd->SetData( CLM_DAT, row++, (int)(LONG_PTR)ccv );
			}
			else{
				swprintf_s( buf, memcnt(buf), _T("%s(%s)"), EDT_DBF_WST(ccv->nam,_T("")), EDT_DBF_WST(ccv->crc,_T("")) );
				GetDlgItem(IDC_STATIC_BASECURRENCY)->SetWindowText( buf );
			}
			Set_Nul( ccv->mdt, FLD_DTM, TRUE );
		}
		m_Ccv_grd->InputEnable( TRUE );
	}
	else{
		if ( erm ){
			MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
	}

	return( chk );
}


BEGIN_MESSAGE_MAP(CCurrencyRateDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_BN_CLICKED(IDCANCEL, &CCurrencyRateDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CCurrencyRateDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCurrencyRateDlg メッセージ ハンドラ
BOOL CCurrencyRateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pPms_mng = ((CMainFrame *)GetParent())->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)GetParent())->m_Dbs_obj;

	SetupComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// グリッドイベント
LRESULT CCurrencyRateDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;
	int				row;
	double			fvl;
	WCHAR			*txt;
	CRC_CVS			*ccv;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:	row = ntf->row;
								//if ( row > 0 ){
								//	ccv = (CRC_CVS *)(LONG_PTR)m_Crc_grd->GetData( CLM_DAT, row );
								//	if ( ccv ){
								//		SetEditData( ccv );
								//	}
								//}
								break;
		case GN_EDITCOMPLETE:	txt = (WCHAR *)lParam;
								if ( ccv = (CRC_CVS *)(LONG_PTR)m_Ccv_grd->GetData( CLM_DAT, ntf->row ) ){
									fvl = _wtof( txt );
									*DBS_DBL_PTR(ccv->rte) = fvl;
									Set_Nul( ccv->mdt, FLD_DTM, FALSE );
									m_Ccv_grd->SetCellText( ntf->clm, ntf->row, txt );
								}
								free( txt );
								break;
		case GN_QUERYINPUTTYPE:	rsl = ntf->clm==CLM_RTE ? GCT_EDT: GCT_NML;
								break;
		}
		free( ntf );
	}

	return( rsl );
}

// close button clicjk
void CCurrencyRateDlg::OnBnClickedCancel()
{
	DWORD		ans=IDYES;
	Csr			csr=NULL;

	while ( csr=Nxt_Lnk( m_Ccv_rot, csr ) ){
		if ( !Nul_Fld(Rec_Of(CRC_CVS, csr)->mdt, FLD_DTM) )	break;
	}
	if ( !!csr ){
		ans = Viw_Cfm_Dcd( this );
	}
	if ( ans==IDYES )	OnCancel();
}

// close with save button clicjk
void CCurrencyRateDlg::OnBnClickedOk()
{
	WCHAR		*erm=NULL;
	Day			dte[1];

	Get_Dte( dte );
	if ( Csh_Sav_Ccv( m_Ccv_rot, dte, &erm, m_pDbs_obj, m_pPms_mng ) ){
		OnOK();
	}
	else{
		if ( erm ){
			MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
	}
}

BOOL CCurrencyRateDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}
