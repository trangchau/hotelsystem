// PaymentDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "PaymentDlg.h"
#include "CashierDlg.h"
#include ".\fnc\csh_fnc.h"
#include ".\fnc\viw_cmn.h"


// CPaymentDlg ダイアログ

static	WCHAR	*Tax_ttl[] =	{	{ _T("Description")	},
									{ _T("Rate")		},
									{ _T("Amount")		},
									{ _T("Amount")		}};

static	GRD_CLM	Tax_clm[] =		{	{ 128, GAL_RGT, GCT_NML,  12 },		// Description
									{  64, GAL_RGT, GCT_NML,  12 },		// Rate
									{ 100, GAL_RGT, GCT_NML,  12 },		// Amount
									{ 100, GAL_RGT, GCT_NML,  12 } };	// Amount


static	WCHAR	*Ttl_ttl[] =	{	{ _T("Currency")	},
									{ _T("PayMethod")	},
									{ _T("Balance")	},
									{ _T("Balance")	},
									{ _T("Payment")		}};

static	GRD_CLM	Ttl_clm[] =		{	{  76, GAL_CTR, GCT_NML,  12 },		// Currency
									{  98, GAL_LFT, GCT_NML,  12 },		// PayMethod
									{ 100, GAL_RGT, GCT_NML,  12 },		// Balance(Currency)
									{ 100, GAL_RGT, GCT_NML,  12 },		// Balance(PaymentCurrency)
									{ 100, GAL_RGT, GCT_CED,  12 } };	// Payment

#define	CLM_DAT					1		// Data keeping column


static	double	clc_bls( double *rfd, double ttl, double csh, double cdt, double cpn, double otr, double dps )
{
	double	tmp=0.0;

	tmp = csh - ( ttl - ( cdt + cpn + otr + dps ) );
	tmp = tmp < 0.0 ? 0.0: tmp;
	if ( rfd )	*rfd = tmp;
	return( ttl - ( ( csh- tmp ) + cdt + cpn + otr + dps  ) );
}

static	void	set_abr( CSimpleGrid *grd, CRC_CVS *ccv )
{
	WCHAR		buf[40];

	*buf = 0;
	if ( ccv )	swprintf_s( buf, memcnt(buf), _T("Amount(%s)"), DBS_WST_PTR(ccv->crc) );
	grd->SetCellText( 3, 0, buf );
}

IMPLEMENT_DYNAMIC(CPaymentDlg, CDialog)

CPaymentDlg::CPaymentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPaymentDlg::IDD, pParent)
{
	m_Tax_grd = NULL;
	m_Ttl_grd = NULL;
	m_Tax_rot = NULL;
	m_Crc_cvs = NULL;
	m_Crc_bse = NULL;
	m_Pay_rot = NULL;
	m_Ccv_rot = NULL;
	m_Dps_rot = NULL;
	m_Dpd_rot = NULL;
	m_Ttl_amt = 0;
	m_Csh_amt = 0;
	m_Cdt_amt = 0;
	m_Cpn_amt = 0;
	m_Otr_amt = 0;
	m_Dps_amt = 0;
	m_Chk_out = FALSE;
	memset( m_Dps_pay, 0, sizeof(m_Dps_pay) );
}

CPaymentDlg::~CPaymentDlg()
{
	if ( m_Tax_grd )	delete	m_Tax_grd;
	if ( m_Ttl_grd )	delete	m_Ttl_grd;
	Del_All( m_Dps_pay );
}

void CPaymentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPaymentDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_CBN_SELCHANGE(IDC_COMBO_CURRENCY, &CPaymentDlg::OnCbnSelchangeComboCurrency)
	ON_BN_CLICKED(IDOK, &CPaymentDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPaymentDlg::OnBnClickedCancel)
	ON_EN_KILLFOCUS(IDC_EDIT_CASH, &CPaymentDlg::OnEnKillfocusEditCash)
	ON_EN_KILLFOCUS(IDC_EDIT_CREDITCARD, &CPaymentDlg::OnEnKillfocusEditCash)
	ON_EN_KILLFOCUS(IDC_EDIT_COUPON, &CPaymentDlg::OnEnKillfocusEditCash)
	ON_EN_KILLFOCUS(IDC_EDIT_OTHER, &CPaymentDlg::OnEnKillfocusEditCash)
END_MESSAGE_MAP()

void CPaymentDlg::SetupComponent()
{
	int				idx;
	RECT			rct[1];
	GRD_DEF			gdf[1];
	WINDOWPLACEMENT	wpc[1];

	struct	{
		CWnd	*wnd;
		int		lmt;
		DWORD	cid;
	}Cpn_[] = {
		{ m_Edt_csh,  14, IDC_EDIT_CASH			},
		{ m_Edt_cdt,  14, IDC_EDIT_CREDITCARD	},
		{ m_Edt_cpn,  14, IDC_EDIT_COUPON		},
		{ m_Edt_otr,  14, IDC_EDIT_OTHER		}
	};

	for ( idx=0; idx<memcnt(Cpn_); idx++ ){
		if ( !!Cpn_[idx].wnd ){
			Cpn_[idx].wnd->SubclassDlgItem(Cpn_[idx].cid, this);
		}
		if ( Cpn_[idx].lmt!=-1 )	((CEdit *)GetDlgItem(Cpn_[idx].cid))->SetLimitText( Cpn_[idx].lmt );
	}
	//GRD_DEF			gdf[1];
	//int				idx;
	//RECT			rct[1];

	memset( gdf, 0, sizeof(gdf) );

	gdf->lnh = 20;
	gdf->vlc = 11;
	gdf->vcc = memcnt(Tax_clm);
	gdf->hbc = RGB(128,255,255);
	gdf->hfc = RGB(0,0,0);
	gdf->dbc = RGB(255,255,255);
	gdf->dfc = RGB(0,0,0);
	gdf->sbc = RGB(0,0,0);
	gdf->sfc = RGB(255,255,255);
	gdf->cct = gdf->vcc;
	gdf->clm = Tax_clm;
	gdf->sel = GSL_NON;//id==IDC_GRD_CSH ? GSL_ROW: GSL_NON;
	gdf->hdr = TRUE;
	gdf->fsa = TRUE;

	rct->left = 20;
	rct->top = 12;
	m_Tax_grd = new CSimpleGrid();
	m_Tax_grd->Create( gdf, *rct, IDC_GRD_CTX, this );
	for ( idx=0; idx<memcnt(Tax_ttl); idx++ ){
		m_Tax_grd->SetCellText( idx, 0, Tax_ttl[idx] );
	}

	GetDlgItem(IDC_STATIC_DEPOSIT)->GetWindowPlacement( wpc );

	gdf->vlc = 9;
	gdf->vcc = memcnt(Ttl_clm);
	gdf->cct = gdf->vcc;
	gdf->clm = Ttl_clm;
	gdf->sel = GSL_NON;//id==IDC_GRD_CSH ? GSL_ROW: GSL_NON;
	gdf->hdr = TRUE;
	gdf->fsa = TRUE;

	wpc->rcNormalPosition.top += 12;
	m_Ttl_grd = new CSimpleGrid();
	m_Ttl_grd->Create( gdf, wpc->rcNormalPosition, IDC_GRD_DPS, this );
	for ( idx=0; idx<memcnt(Ttl_ttl); idx++ ){
		m_Ttl_grd->SetCellText( idx, 0, Ttl_ttl[idx] );
	}
	if ( m_Chk_out ){
		SetWindowText( _T("Checkout payment") );
		GetDlgItem(IDOK)->SetWindowTextW( _T("Execute Checkout") );
	}
}


// CPaymentDlg メッセージ ハンドラ

BOOL CPaymentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetupComponent();
	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// Notify from grid
LRESULT CPaymentDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;
	int				row;
	WCHAR			*txt;
	CSH_TAX_DAT		*tax;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:	row = ntf->row;
								if ( row > 0 ){
								}
								break;
		case GN_QUERYINPUTTYPE:	if ( ntf->cid==IDC_GRD_DPS ){
									rsl = ntf->clm==CLM_PMT ? GCT_CED: GCT_NML;
								}
								else{
									rsl=GCT_NML; 
									if ( tax = (CSH_TAX_DAT *)(LONG_PTR)m_Tax_grd->GetData( CLM_DAT, ntf->row ) ){
										if ( DBS_INT_VAL(tax->tax->dkd)==DRK_DISCOUNT ){
											if ( ( DBS_INT_VAL(tax->tax->prk)==PRK_DISC_AMOUNT && ntf->clm==CLM_TXP )
												|| ( DBS_INT_VAL(tax->tax->prk)==PRK_DISC_RATE && ntf->clm==CLM_RTE ) ){
												rsl=GCT_CED;
											}
										}
									}
								}
								break;
		case GN_QUERYCMEPARAM:	rsl = MAKELPARAM(CEDT_ROUND_UP, DBS_INT_VAL(m_Crc_cvs->adp) );
								break;
		case GN_EDITCOMPLETE:	if ( !!( txt = (WCHAR *)lParam ) ){
									if ( ntf->row > 0 ){
										if ( ntf->cid==IDC_GRD_DPS ){
											if ( Csh_Use_Dps( m_Ttl_grd, txt, ntf->clm, ntf->row, this, m_pPms_mng ) ){
												SetBalance();
											}
										}
										else{
											Csh_Ecp_Ttl( ntf->row, m_Pay_rot, txt, m_Tax_grd, this, m_pPms_mng );
											Csh_Dsp_Ttl( m_Tax_grd, m_Pay_rot, m_Tax_rot, m_Dps_rot, m_Crc_cvs, m_pPms_mng );
										}
									}
									free( txt );
								}
								break;
		}
		free( ntf );
	}

	return( rsl );
}

// payment currency select change
void CPaymentDlg::OnCbnSelchangeComboCurrency()
{
	int			idx;
	CRC_CVS		*ccv;
	CComboBox	*cbx = (CComboBox *)GetDlgItem(IDC_COMBO_CURRENCY);

	if ( ( idx = cbx->GetCurSel() ) != -1 ){
		ccv = (CRC_CVS *)(LONG_PTR)cbx->GetItemData( idx );
		if ( m_Crc_cvs != ccv ){
			m_Crc_cvs = ccv;
			set_abr( m_Tax_grd, ccv );
			Csh_Dsp_Crt( ccv, GetDlgItem(IDC_STATIC_RATE) );
			Csh_Dsp_Tax( m_Tax_grd, m_Tax_rot, m_Dps_rot, TRUE, m_pPms_mng );
			/*m_Ttl_amt = */Csh_Dsp_Ttl( m_Tax_grd, m_Pay_rot, m_Tax_rot, m_Dps_rot, m_Crc_cvs, m_pPms_mng );
			SetData();
		}
	}
}

// cash editbox killfocus 
void CPaymentDlg::OnEnKillfocusEditCash()
{
	double		val;

	m_Csh_amt = !m_Edt_csh->GetValue( &val ) ? val: 0;	
	m_Cdt_amt = !m_Edt_cdt->GetValue( &val ) ? val: 0;	
	m_Cpn_amt = !m_Edt_cpn->GetValue( &val ) ? val: 0;	
	m_Otr_amt = !m_Edt_otr->GetValue( &val ) ? val: 0;	
	SetBalance();
}

BOOL CPaymentDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}

void CPaymentDlg::OnBnClickedOk()
{
	int			row;
	Csr			csr;
	CSH_DPS_DAT	*dps;

	if ( clc_bls( NULL, m_Ttl_amt, m_Csh_amt, m_Cdt_amt, m_Cpn_amt, m_Otr_amt, m_Dps_amt )==0.0 ){
		for ( row=1; row<=m_Ttl_grd->GetLineCount(FALSE); row++ ){
			dps = (CSH_DPS_DAT* )(LONG_PTR)m_Ttl_grd->GetData( CLM_DAT, row );
			if ( !( csr = Add_Lnk( m_Dps_pay, &dps, sizeof(CSH_DPS_DAT *) ) ) ){
				MessageBox( _T("System error! Insufficient memory."), NULL, MB_ICONEXCLAMATION|MB_OK );
				OnCancel();
				break;
			}
		}
		OnOK();
	}
	else{
		MessageBox( _T("Payment amount is insufficient."), NULL, MB_ICONEXCLAMATION|MB_OK );
	}
}

void CPaymentDlg::OnBnClickedCancel()
{
	OnCancel();
}

// Operation
void CPaymentDlg::InitData()
{
	int			idx;
	Csr			csr=NULL;
	CRC_CVS		*ccv;
	CComboBox	*cbx = (CComboBox *)GetDlgItem(IDC_COMBO_CURRENCY);

	m_Edt_csh->RoundMethod = CEDT_ROUND_UP;
	m_Edt_cdt->RoundMethod = CEDT_ROUND_UP;
	m_Edt_cpn->RoundMethod = CEDT_ROUND_UP;
	m_Edt_otr->RoundMethod = CEDT_ROUND_UP;

	cbx->ResetContent();

	while( csr=Nxt_Lnk( m_Ccv_rot, csr ) ){
		ccv = Rec_Of(CRC_CVS, csr);
		cbx->SetItemData( idx=cbx->InsertString( -1, EDT_DBF_WST(ccv->crc, _T("")) ), (LONG_PTR)ccv );
		if ( !!DBS_INT_VAL(ccv->isb) ){
			cbx->SetCurSel( idx );
			Csh_Dsp_Crt( ccv, GetDlgItem(IDC_STATIC_RATE) );
			m_Crc_cvs = ccv;
			m_Crc_bse = ccv;
		}
		set_abr( m_Tax_grd, m_Crc_cvs );
	}
	Csh_Dsp_Tax( m_Tax_grd, m_Tax_rot, m_Dps_rot, TRUE, m_pPms_mng );
	/*m_Ttl_amt = */Csh_Dsp_Ttl( m_Tax_grd, m_Pay_rot, m_Tax_rot, m_Dps_rot, m_Crc_cvs, m_pPms_mng );
	SetData();
}

void CPaymentDlg::SetData()
{
	int				idx, div;
	WCHAR			buf[40];
	WINDOWPLACEMENT	wpc[1], cpn[1];
	CComboBox	*cbx = (CComboBox *)GetDlgItem(IDC_COMBO_CURRENCY);

	m_Csh_amt = m_Cdt_amt = m_Cpn_amt = m_Otr_amt = 0;
	m_Edt_csh->SetValue( m_Csh_amt, TRUE );
	m_Edt_csh->SetValue( m_Cdt_amt, TRUE );
	m_Edt_csh->SetValue( m_Cpn_amt, TRUE );
	m_Edt_otr->SetValue( m_Otr_amt, TRUE );
	m_Edt_csh->DecimalPlacess = DBS_INT_VAL(m_Crc_cvs->adp);
	m_Edt_cdt->DecimalPlacess = DBS_INT_VAL(m_Crc_cvs->adp);
	m_Edt_cpn->DecimalPlacess = DBS_INT_VAL(m_Crc_cvs->adp);
	m_Edt_otr->DecimalPlacess = DBS_INT_VAL(m_Crc_cvs->adp);

	if ( ( idx = cbx->GetCurSel() ) != -1 ){
		m_Tax_grd->GetCellText( buf, memcnt(buf), 3, m_Tax_grd->GetLineCount(FALSE) );
		Prg_Cma( buf );
		m_Ttl_amt = _wtof( buf );
		SetBalance();
	}
	Csh_Dsp_Dps( m_Ttl_grd, m_Dpd_rot, TRUE, m_Crc_cvs, m_Crc_bse );
	if ( !Cnt_Lnk( m_Dpd_rot ) ){
		GetWindowPlacement( wpc );
		GetDlgItem(IDC_COMBO_CURRENCY)->GetWindowPlacement( cpn );
		div = wpc->rcNormalPosition.right - ( cpn->rcNormalPosition.right + 48 );
		wpc->rcNormalPosition.right -= div;
		SetWindowPlacement( wpc );
		GetDlgItem(IDCANCEL)->GetWindowPlacement( cpn );
		cpn->rcNormalPosition.left -= div;
		cpn->rcNormalPosition.right -= div;
		GetDlgItem(IDCANCEL)->SetWindowPlacement( cpn );
		GetDlgItem(IDOK)->GetWindowPlacement( cpn );
		cpn->rcNormalPosition.left -= div;
		cpn->rcNormalPosition.right -= div;
		GetDlgItem(IDOK)->SetWindowPlacement( cpn );
	}
	m_Ttl_grd->InputEnable(TRUE);
}

void CPaymentDlg::SetBalance()
{
	double		bls, rfd;
	WCHAR		*ptr, *msg;

	bls = clc_bls( &rfd, m_Ttl_amt, m_Csh_amt, m_Cdt_amt, m_Cpn_amt, m_Otr_amt, m_Dps_amt );
	if ( !( msg = ptr = Cma_Edt( rfd, DBS_INT_VAL(m_Crc_cvs->adp) ) ) ){
		msg = _T("Err!!");
	}
	GetDlgItem(IDC_STATIC_REFUND)->SetWindowTextW( msg );
	if ( ptr )	free( ptr );
	if ( !( msg = ptr = Cma_Edt( bls, DBS_INT_VAL(m_Crc_cvs->adp) ) ) ){
		msg = _T("Err!!");
	}
	GetDlgItem(IDC_STATIC_BALANCE)->SetWindowTextW( msg );
	if ( ptr )	free( ptr );
}
