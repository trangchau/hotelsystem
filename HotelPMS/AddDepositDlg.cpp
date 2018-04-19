// AddDepositDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "AddDepositDlg.h"
#include ".\fnc\viw_cmn.h"
#include ".\fnc\csh_fnc.h"
#include ".\fnc\dsc_sql.h"


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

CSH_DPS_DAT	*CAddDepositDlg::AddDeposit(		// adding a deposit into inhouse data
int			knd )					// deposit kind
{
	double		fvl;
	CSH_DPS_DAT	*dps;
	Csr			csr;
	
	m_Csh_amt *= knd==DPK_DEPOSIT ? 1.0: -1.0;
	if ( csr = Add_Lnk( m_Dps_rot, NULL, sizeof(CSH_DPS_DAT) ) ){
		dps = Rec_Of(CSH_DPS_DAT, csr);
		DBS_INT_SET(dps->rii, &m_Rsv_iid);
		DBS_INT_SET(dps->rdi, &m_Rsv_did);
		DBS_WST_CPY(dps->crc, m_Crc_cvs->crc);
		DBS_INT_CPY(dps->adp, m_Crc_cvs->adp);
		DBS_INT_CPY(dps->isb, m_Crc_cvs->isb);
		DBS_DBL_CPY(dps->rte, m_Crc_cvs->rte);
		DBS_WST_SET(dps->dtl->opc, m_Ope_cod);
		DBS_INT_SET(dps->dtl->sii, &m_Sal_iid);
		DBS_DAY_SET(dps->dtl->sld, m_pPms_mng->bdt);
		DBS_INT_CPY(dps->dtl->dsi, m_Dps_mst->tax->id);
		DBS_DBL_SET(dps->dtl->qty, &(fvl=0));
		DBS_DBL_SET(dps->dtl->nmp, &(fvl=0));
		DBS_DBL_SET(dps->dtl->slp, &(fvl=0));
		DBS_INT_SET(dps->dtl->asi, (int *)NULL);
		DBS_INT_SET(dps->dtl->eqi, (int *)NULL);
		DBS_WST_SET(dps->pmt->opc, m_Ope_cod);
		DBS_INT_SET(dps->pmt->sii, &m_Sal_iid);
		DBS_DAY_SET(dps->pmt->pmd, m_pPms_mng->bdt);
		DBS_INT_CPY(dps->pmt->cci, m_Crc_cvs->id);
		DBS_DBL_CPY(dps->pmt->rte, m_Crc_cvs->rte);
		DBS_DBL_SET(dps->pmt->amt, &m_Csh_amt);
		DBS_INT_SET(dps->pmt->mtd, &m_Pay_mtd);
		DBS_INT_SET(dps->pmt->dpk, &knd);
		//Sot_Lnk( inh->dps, dps_cmp );
		m_Dps_grd->AddRow( 1 );
		dsp_dps( m_Dps_grd, m_Dps_grd->GetLineCount(FALSE), dps );
		m_Dps_grd->Redraw();
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
	m_Rsv_iid = 0;
	m_Rsv_did = 0;
	m_Sal_iid = 0;
	m_Eqp_iid = 0;
	m_Dir_ect = FALSE;
//	m_Inh_dat = NULL;
	m_Dps_rot = NULL;
	m_Fst_nam = _T("");
	m_Lst_nam = _T("");
	m_Rom_nbr = _T("");
	memset( m_Ccv_lnk, 0, sizeof(m_Ccv_lnk) );
	memset( m_Dps_lnk, 0, sizeof(m_Ccv_lnk) );
	memset( m_Dpo_sit, 0, sizeof(m_Dpo_sit) );
}

CAddDepositDlg::~CAddDepositDlg()
{
	if ( m_Dps_grd )	delete m_Dps_grd;
	if ( m_Ttl_grd )	delete m_Ttl_grd;
	Del_All( m_Ccv_lnk );
	Del_All( m_Dps_lnk );
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

	if ( !m_Dir_ect ){
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
		if ( !!AddDeposit( DPK_DEPOSIT ) ){
//			m_Dps_grd->AddRow( 1 );
//			dsp_dps( m_Dps_grd, m_Dps_grd->GetLineCount(FALSE), dps );
//			m_Dps_grd->Redraw();
			Csh_Dsp_Dps( m_Ttl_grd, m_Dps_rot, TRUE, NULL, NULL );
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
		while( csr=Nxt_Lnk( m_Dps_rot, csr ) ){
			dps = Rec_Of(CSH_DPS_DAT, csr);
			if ( DBS_INT_VAL(dps->pmt->cci)==DBS_INT_VAL(m_Crc_cvs->id) ){
				if ( DBS_INT_VAL(dps->pmt->mtd)==m_Pay_mtd ){
					max += DBS_DBL_VAL(dps->pmt->amt); 
				}
			}
		}
		if ( m_Csh_amt <= max ){
			if ( !!AddDeposit( DPK_REFUND ) ){
				Csh_Dsp_Dps( m_Ttl_grd, m_Dps_rot, TRUE, NULL, NULL );
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
	int			chk;
	Day			dte[1];
	WCHAR		*erm=NULL;

	if ( !( chk = !m_Dir_ect ) ){
		if ( !( chk = !m_Dps_rot ) ){
			Get_Dte( dte );
			if ( !( chk = Csh_Sav_Dps( m_Dps_rot, dte, &erm, m_pDbs_obj, m_pPms_mng ) ) ){
				if ( erm ){
					MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
					free( erm );
				}
			}
		}
	}
	if ( chk )	OnOK();
}

void CAddDepositDlg::OnBnClickedCancel()
{
	Csr			csr=NULL;
	CSH_DPS_DAT	*dps;
	DWORD		ans = IDYES;

	if ( m_Dir_ect && m_Dps_rot ){
		while( csr=Nxt_Lnk( m_Dps_rot, csr ) ){
			dps = Rec_Of(CSH_DPS_DAT, csr);
			if ( Nul_Fld(dps->pmt->id, FLD_INT) || !Nul_Fld(dps->pmt->mdt, FLD_DTM) || !Nul_Fld(dps->pmt->ddt, FLD_DTM) ){
				break;
			}
		}
	}
	if ( m_Dir_ect && !!csr ){
		ans = Viw_Cfm_Dcd( this );
	}
	if ( ans == IDYES )	OnCancel();
}

// Operation
void CAddDepositDlg::InitData()
{
	Csr			csr=NULL;
	WCHAR		buf[530];

	swprintf_s( buf, memcnt(buf), _T("%s %s"), m_Fst_nam, m_Lst_nam );
	GetDlgItem(IDC_STATIC_NAME)->SetWindowTextW( buf );
	GetDlgItem(IDC_STATIC_ROOMNUMBER)->SetWindowTextW( m_Rom_nbr );
	m_Edt_csh->SetFocus();

	SetData();
}

void CAddDepositDlg::SetData()
{
	int			chk=TRUE, row=0, idx;
	WCHAR		*erm=NULL;
	CSH_DPS_DAT	*dps;
	CSH_TAX_DAT	*dpm;
	CRC_CVS		cky[1], *ccv;
	DSC_RPT		dky[1];
	SAL_INF		sal[1];
	Lnk			rot[1], *csr=NULL;;
	CComboBox	*cbx = (CComboBox *)GetDlgItem(IDC_COMBO_CURRENCY);

	if ( m_Dir_ect ){
		memset( rot, 0, sizeof(rot) );
		memset( cky, 0, sizeof(cky) );
		memset( dky, 0, sizeof(dky) );
		memset( sal, 0, sizeof(sal) );
		DBS_WST_SET(cky->opc, m_Ope_cod);
		DBS_WST_SET(cky->brc, m_Brc_cod);
		DBS_WST_SET(cky->fcc, m_Fcl_cod);
		DBS_WST_SET(dky->opc, m_Ope_cod );
		DBS_WST_SET(dky->brc, m_Brc_cod );
		DBS_WST_SET(dky->fcc, m_Fcl_cod );
		chk = Csh_Get_Ccv( m_Ccv_lnk, cky, &erm, m_pDbs_obj, m_pPms_mng );
		if ( chk ){
			m_Ccv_rot = m_Ccv_lnk;
			chk = Dsc_Get_Dsc( rot, dky, TRUE, &erm, m_pDbs_obj, m_pPms_mng );
			if ( chk ){
				while( csr=Nxt_Lnk( rot, csr ) ){
					dpm = Rec_Of(CSH_TAX_DAT, csr);
					if ( DBS_INT_VAL(dpm->tax->dkd)==DRK_DEPOSIT ){
						*m_Dpo_sit = *dpm;
						m_Dps_mst = m_Dpo_sit;
						break;
					}
				}
				if ( chk = !!csr ){
					DBS_WST_SET(sal->opc, m_Ope_cod);
					DBS_INT_SET(sal->rii, &m_Rsv_iid);
					DBS_INT_SET(sal->rdi, &m_Rsv_did);
					DBS_INT_SET(sal->eii, &m_Eqp_iid);
					DBS_WST_SET(sal->brc, m_Brc_cod);
					DBS_WST_SET(sal->fcc, m_Fcl_cod);
					DBS_INT_SET(sal->ivn, (int *)0);
					DBS_INT_SET(sal->flg, &(idx=0));	///--------->>>>>>>>>>> 暫定
					chk = ( m_Sal_iid = Csh_Get_Sii( m_Rsv_did, sal, &erm, m_pDbs_obj, m_pPms_mng ) ) != 0;
				}
				else{
					MessageBox( _T("Deposit is not registerd on master."), NULL, MB_ICONEXCLAMATION|MB_OK );
				}
			}
		}

		if ( chk ){
			if ( chk = Csh_Get_Dps( m_Dps_lnk, m_Rsv_did, &erm, m_pDbs_obj, m_pPms_mng ) ){
				m_Dps_rot = m_Dps_lnk;
			}
		}
		Del_All( rot );

		if ( erm ){
			MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
	}

	if ( chk ){
		for ( csr=NULL; csr=Nxt_Lnk( m_Ccv_rot, csr ); ){
			ccv = Rec_Of(CRC_CVS, csr);
			cbx->SetItemData( idx=cbx->InsertString(-1, EDT_DBF_WST(ccv->crc,_T(""))), (LONG_PTR)ccv );
			if ( !!DBS_INT_VAL(ccv->isb) ){
				cbx->SetCurSel(idx);
				m_Crc_cvs = ccv;
			}
		}

		if ( m_Dps_rot ){
			m_Dps_grd->ResetContent();
			m_Dps_grd->AddRow( Cnt_Lnk( m_Dps_rot ) );
			while ( csr=Nxt_Lnk( m_Dps_rot, csr ) ){
				dps = Rec_Of(CSH_DPS_DAT, csr);
				if ( m_Dir_ect ){
					DBS_DTM_SET(dps->dtl->mdt, (Day *)0 );
					DBS_DTM_SET(dps->pmt->mdt, (Day *)0 );
				}
				dsp_dps( m_Dps_grd, ++row, dps );
				m_Dps_grd->SetData( CLM_DAT, row, (int)(LONG_PTR)dps );
			}
			Csh_Dsp_Dps( m_Ttl_grd, m_Dps_rot, TRUE, NULL, NULL );
		}
	}
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
