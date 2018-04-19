// AddDepositDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "AddDepositDlg.h"
#include ".\fnc\viw_cmn.h"
#include ".\fnc\csh_fnc.h"


// CAddDepositDlg ダイアログ

static	WCHAR	*Dps_ttl[] =	{	{ _T("Date")	},
									{ _T("PayMethod")},
									{ _T("Payment")	},
									{ _T("Kind")	},
									{ _T("Amount")	}};

static	GRD_CLM	Dps_clm[] =		{	{  90, GAL_CTR, GCT_NML,  12 },		// Date
									{  80, GAL_CTR, GCT_NML,  12 },		// Currency
									{  98, GAL_LFT, GCT_NML,  12 },		// Payment
									{  64, GAL_CTR, GCT_NML,  12 }, 	// Kind
									{ 110, GAL_RGT, GCT_NML,  12 } };	// Amount

static	WCHAR	*Ttl_ttl[] =	{	{ _T("Currency")	},
									{ _T("PayMethod")	},
									{ _T("Balance")		}};

static	GRD_CLM	Ttl_clm[] =		{	{  76, GAL_CTR, GCT_NML,  12 },		// Currency
									{  98, GAL_LFT, GCT_NML,  12 },		// Paymen
									{ 100, GAL_RGT, GCT_NML,  12 } };	// Amount



//#define	IDC_GRD_DPS			8192
#define	IDC_GRD_TTL			8193

#define	CLM_DAT				0

#define	CLM_DTE				0
//#define	CLM_CRC				1
//#define	CLM_MTD				2
#define	CLM_KND				3
#define	CLM_AMT				4

IMPLEMENT_DYNAMIC(CAddDepositDlg, CDialog)

static	void	dsp_dps(
CSimpleGrid		*grd,
int				row,
CSH_DPS_DAT		*dps )
{
	WCHAR		buf[256], *ptr;

	grd->SetCellText( CLM_DTE, row, EDT_DBF_DAY(buf, memcnt(buf), dps->dtl->sld) );
	grd->SetCellText( CLM_CRC, row, EDT_DBF_WST(dps->crc,_T("")) );
	ptr = Viw_Get_Dsn( DBS_INT_VAL(dps->pmt->mtd), DSC_PMT );
	grd->SetCellText( CLM_MTD, row, ptr ? ptr: _T("Unknown") );
	ptr = Viw_Get_Dsn( DBS_INT_VAL(dps->pmt->dpk), DSC_DPK );
	grd->SetCellText( CLM_KND, row, ptr ? ptr: _T("Unknown") );
	ptr = Cma_Edt( DBS_DBL_VAL(dps->pmt->amt), DBS_INT_VAL(dps->adp) );
	grd->SetCellText( CLM_AMT, row, ptr ? ptr: _T("Unknown") );
	if ( ptr )	free( ptr );
}

static	CSH_DPS_DAT	*add_dps(		// adding a deposit into inhouse data
CSH_INH_DAT	*inh,					// inhouse data
CSimpleGrid	*grd,					// displaying grid
double		amt,					// deposit amount
int			pmt,					// payment method
int			knd,					// deposit kind
CRC_CVS		*ccv,					// currency data
CSH_TAX_DAT	*dsc,					// discount master
WCHAR		*opc,					// operator_code
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	double		fvl;
	CSH_DPS_DAT	*dps;
	Csr			csr;
	
	amt *= knd==DPK_DEPOSIT ? 1.0: -1.0;
	if ( csr = Add_Lnk( inh->dps, NULL, sizeof(CSH_DPS_DAT) ) ){
		dps = Rec_Of(CSH_DPS_DAT, csr);
		DBS_INT_CPY(dps->rii, inh->rii);
		DBS_INT_CPY(dps->rdi, inh->rdi);
		DBS_WST_CPY(dps->crc, ccv->crc);
		DBS_INT_CPY(dps->adp, ccv->adp);
		DBS_INT_CPY(dps->isb, ccv->isb);
		DBS_DBL_CPY(dps->rte, ccv->rte);
		DBS_WST_SET(dps->dtl->opc, opc);
		DBS_INT_CPY(dps->dtl->sii, inh->sal->sal->id);
		DBS_DAY_SET(dps->dtl->sld, mng->bdt);
		DBS_INT_CPY(dps->dtl->dsi, dsc->tax->id);
		DBS_DBL_SET(dps->dtl->qty, &(fvl=0));
		DBS_DBL_SET(dps->dtl->nmp, &(fvl=0));
		DBS_DBL_SET(dps->dtl->slp, &(fvl=0));
		DBS_INT_SET(dps->dtl->asi, (int *)NULL);
		DBS_INT_SET(dps->dtl->eqi, (int *)NULL);
		DBS_WST_SET(dps->pmt->opc, opc);
		DBS_INT_CPY(dps->pmt->sii, inh->sal->sal->id);
		DBS_DAY_SET(dps->pmt->pmd, mng->bdt);
		DBS_INT_CPY(dps->pmt->cci, ccv->id);
		DBS_DBL_CPY(dps->pmt->rte, ccv->rte);
		DBS_DBL_SET(dps->pmt->amt, &amt);
		DBS_INT_SET(dps->pmt->mtd, &pmt);
		DBS_INT_SET(dps->pmt->dpk, &knd);
		//Sot_Lnk( inh->dps, dps_cmp );
		grd->AddRow( 1 );
		dsp_dps( grd, grd->GetLineCount(FALSE), dps );
		grd->Redraw();
	}
	return( !!csr ? dps: NULL );
}

CAddDepositDlg::CAddDepositDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddDepositDlg::IDD, pParent)
{
	m_Dps_grd = NULL;
	m_Ttl_grd = NULL;
	m_Ccv_rot = NULL;
	m_Crc_cvs = NULL;
	m_Dps_mst = NULL;
	m_Csh_amt = 0;
	m_Pay_mtd = 0;
	m_Dir_ect = FALSE;
	m_Inh_dat = NULL;
}

CAddDepositDlg::~CAddDepositDlg()
{
	if ( m_Dps_grd )	delete m_Dps_grd;
	if ( m_Ttl_grd )	delete m_Ttl_grd;
}

void CAddDepositDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAddDepositDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_CBN_SELCHANGE(IDC_COMBO_CURRENCY, &CAddDepositDlg::OnCbnSelchangeComboCurrency)
	ON_BN_CLICKED(IDOK, &CAddDepositDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAddDepositDlg::OnBnClickedCancel)
	ON_EN_KILLFOCUS(IDC_EDIT_CASH, &CAddDepositDlg::OnEnKillfocusEditCash)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CAddDepositDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REFUND, &CAddDepositDlg::OnBnClickedButtonRefund)
END_MESSAGE_MAP()


void CAddDepositDlg::SetupComponent()
{
	int				idx, cnt;
	GRD_DEF			gdf[1];
	WINDOWPLACEMENT	wpc[1];
	DSC_CDN			*cdn;
	CComboBox		*cbx = (CComboBox *)GetDlgItem(IDC_COMBO_PAYMENTMETHOD);

	struct	{
		CWnd	*wnd;
		int		lmt;
		DWORD	cid;
	}Cpn_[] = {
		{ m_Edt_csh,  14, IDC_EDIT_CASH			}
	};

	for ( idx=0; idx<memcnt(Cpn_); idx++ ){
		if ( !!Cpn_[idx].wnd ){
			Cpn_[idx].wnd->SubclassDlgItem(Cpn_[idx].cid, this);
		}
		if ( Cpn_[idx].lmt!=-1 )	((CEdit *)GetDlgItem(Cpn_[idx].cid))->SetLimitText( Cpn_[idx].lmt );
	}

	GetDlgItem(IDC_STATIC_ROOMNUMBER)->GetWindowPlacement( wpc );

	memset( gdf, 0, sizeof(gdf) );

	gdf->lnh = 20;
	gdf->vlc = 11;
	gdf->vcc = memcnt(Dps_clm);
	gdf->hbc = RGB(128,255,255);
	gdf->hfc = RGB(0,0,0);
	gdf->dbc = RGB(255,255,255);
	gdf->dfc = RGB(0,0,0);
	gdf->sbc = RGB(0,0,0);
	gdf->sfc = RGB(255,255,255);
	gdf->cct = gdf->vcc;
	gdf->clm = Dps_clm;
	gdf->sel = GSL_NON;//GSL_ROW;
	gdf->hdr = TRUE;
	gdf->fsa = TRUE;

	GetDlgItem(IDC_STATIC_HISTORY)->GetWindowPlacement( wpc );
	wpc->rcNormalPosition.top += 12;
	m_Dps_grd = new CSimpleGrid();
	m_Dps_grd->Create( gdf, wpc->rcNormalPosition, IDC_GRD_DPS, this );
	for ( idx=0; idx<memcnt(Dps_ttl); idx++ ){
		m_Dps_grd->SetCellText( idx, 0, Dps_ttl[idx] );
	}

	GetDlgItem(IDC_STATIC_TOTAL)->GetWindowPlacement( wpc );

	gdf->vlc = 6;
	gdf->vcc = memcnt(Ttl_clm);
	gdf->cct = gdf->vcc;
	gdf->clm = Ttl_clm;
	gdf->sel = GSL_NON;
	gdf->hdr = TRUE;
	gdf->fsa = TRUE;

	wpc->rcNormalPosition.top += 12;
	m_Ttl_grd = new CSimpleGrid();
	m_Ttl_grd->Create( gdf, wpc->rcNormalPosition, IDC_GRD_TTL, this );
	for ( idx=0; idx<memcnt(Ttl_ttl); idx++ ){
		m_Ttl_grd->SetCellText( idx, 0, Ttl_ttl[idx] );
	}

	cnt = Viw_Get_Dsc( &cdn, DSC_PMT );
	for ( idx=0; idx<cnt; idx++ ){
		cbx->SetItemData( cbx->InsertString( -1, (cdn+idx)->nam ), (LONG_PTR)(cdn+idx)->cod );
	}

	if ( m_Dir_ect ){
		GetDlgItem(IDCANCEL)->ShowWindow( SW_HIDE );
		GetDlgItem(IDOK)->SetWindowText( _T("Close") );
	}
}


// CAddDepositDlg メッセージ ハンドラ

BOOL CAddDepositDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetupComponent();
	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// Notify from grid
LRESULT CAddDepositDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;
	int				row;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:	row = ntf->row;
								if ( row > 0 ){
								}
								break;
		}
		free( ntf );
	}

	return( rsl );
}

// payment currency select change
void CAddDepositDlg::OnCbnSelchangeComboCurrency()
{
	int			idx;
	CRC_CVS		*ccv;
	CComboBox	*cbx = (CComboBox *)GetDlgItem(IDC_COMBO_CURRENCY);

	if ( ( idx = cbx->GetCurSel() ) != -1 ){
		ccv = (CRC_CVS *)(LONG_PTR)cbx->GetItemData( idx );
		if ( m_Crc_cvs != ccv ){
			m_Crc_cvs = ccv;
			m_Edt_csh->SetValue( 0, TRUE );
			m_Edt_csh->DecimalPlacess = DBS_INT_VAL(ccv->adp);
			m_Edt_csh->RoundMethod = CEDT_ROUND_UP;
		}
	}
}

// cash editbox killfocus 
void CAddDepositDlg::OnEnKillfocusEditCash()
{
	double		val;

	m_Csh_amt = !m_Edt_csh->GetValue( &val ) ? val: 0;	
}

// Add button click
void CAddDepositDlg::OnBnClickedButtonAdd()
{
	if ( GetData() ){
		if ( !!add_dps( m_Inh_dat, m_Dps_grd, m_Csh_amt, m_Pay_mtd, DPK_DEPOSIT, m_Crc_cvs, m_Dps_mst, m_Ope_cod, m_pPms_mng ) ){
//			m_Dps_grd->AddRow( 1 );
//			dsp_dps( m_Dps_grd, m_Dps_grd->GetLineCount(FALSE), dps );
//			m_Dps_grd->Redraw();
			Csh_Dsp_Dps( m_Ttl_grd, m_Inh_dat->dps, TRUE, NULL, NULL );
		}
	}
}

// Refund button click
void CAddDepositDlg::OnBnClickedButtonRefund()
{
	Csr			csr=NULL;
	double		max=0.0;
	CSH_DPS_DAT	*dps;

	if ( GetData() ){
		while( csr=Nxt_Lnk( m_Inh_dat->dps, csr ) ){
			dps = Rec_Of(CSH_DPS_DAT, csr);
			if ( DBS_INT_VAL(dps->pmt->cci)==DBS_INT_VAL(m_Crc_cvs->id) ){
				if ( DBS_INT_VAL(dps->pmt->mtd)==m_Pay_mtd ){
					max += DBS_DBL_VAL(dps->pmt->amt); 
				}
			}
		}
		if ( m_Csh_amt <= max ){
			if ( !!add_dps( m_Inh_dat, m_Dps_grd, m_Csh_amt, m_Pay_mtd, DPK_REFUND, m_Crc_cvs, m_Dps_mst, m_Ope_cod, m_pPms_mng ) ){
				Csh_Dsp_Dps( m_Ttl_grd, m_Inh_dat->dps, TRUE, NULL, NULL );
			}
		}
		else{
			MessageBox( _T("Can not refund the amount that more than depositting."), NULL, MB_ICONEXCLAMATION|MB_OK );
		}
	}
}

BOOL CAddDepositDlg::PreTranslateMessage(MSG* pMsg)
{
	int		ret=TRUE;

	// To prevent that dialog is closed when 2 keys which below pushed
	if( WM_KEYDOWN == pMsg->message ){
		switch( pMsg->wParam ){
		case VK_RETURN:
		case VK_ESCAPE:	ret=FALSE;	break;
		default:		break;
		}
	}
	if ( ret )	return( CDialog::PreTranslateMessage(pMsg) );
	else		return( FALSE );
}

void CAddDepositDlg::OnBnClickedOk()
{
	OnOK();
}

void CAddDepositDlg::OnBnClickedCancel()
{
	OnCancel();
}

// Operation
void CAddDepositDlg::InitData()
{
	int			idx;
	Csr			csr=NULL;
	CRC_CVS		*ccv;
	CComboBox	*cbx = (CComboBox *)GetDlgItem(IDC_COMBO_CURRENCY);
	WCHAR		buf[530];

	swprintf_s( buf, memcnt(buf), _T("%s%s"), EDT_DBF_WST(m_Inh_dat->fsn,_T("")), EDT_DBF_WST(m_Inh_dat->lsn,_T("")) );
	GetDlgItem(IDC_STATIC_NAME)->SetWindowTextW( buf );
	GetDlgItem(IDC_STATIC_ROOMNUMBER)->SetWindowTextW( EDT_DBF_WST(m_Inh_dat->rmn,_T("")) );
	m_Edt_csh->SetFocus();

	while( csr=Nxt_Lnk( m_Ccv_rot, csr ) ){
		ccv = Rec_Of(CRC_CVS, csr);
		cbx->SetItemData( idx=cbx->InsertString(-1, EDT_DBF_WST(ccv->crc,_T(""))), (LONG_PTR)ccv );
		if ( !!DBS_INT_VAL(ccv->isb) ){
			cbx->SetCurSel(idx);
			m_Crc_cvs = ccv;
		}
	}

	SetData();
}

void CAddDepositDlg::SetData()
{
	int			row=0;
	Csr			csr=NULL;
	CSH_DPS_DAT	*dps;

	m_Dps_grd->ResetContent();
	m_Dps_grd->AddRow( Cnt_Lnk( m_Inh_dat->dps ) );

	while ( csr=Nxt_Lnk( m_Inh_dat->dps, csr ) ){
		dps = Rec_Of(CSH_DPS_DAT, csr);
		dsp_dps( m_Dps_grd, ++row, dps );
		m_Dps_grd->SetData( CLM_DAT, row, (int)(LONG_PTR)dps );
	}
	Csh_Dsp_Dps( m_Ttl_grd, m_Inh_dat->dps, TRUE, NULL, NULL );
}

int		CAddDepositDlg::GetData()
{
	int			chk, idx;
	CComboBox	*cbx = (CComboBox *)GetDlgItem(IDC_COMBO_PAYMENTMETHOD);

	if ( chk = !m_Edt_csh->GetValue( &m_Csh_amt ) ){
		if ( chk = !!m_Crc_cvs ){
			if ( chk = ( idx = cbx->GetCurSel() ) != -1 ){
				m_Pay_mtd = (int)(LONG_PTR)cbx->GetItemData( idx );
			}
			else{
				MessageBox( _T("PaymentMethod is not selected."), NULL, MB_ICONEXCLAMATION|MB_OK );
			}
		}
		else{
			MessageBox( _T("PaymentCurrency is not selected."), NULL, MB_ICONEXCLAMATION|MB_OK );
		}
	}
	else{
		MessageBox( _T("Amount is not inputted."), NULL, MB_ICONEXCLAMATION|MB_OK );
	}
	return( chk );
}
