// CurrencyCvsDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "CurrencyCvsDlg.h"
#include "MainFrm.h"
#include ".\fnc\mfm_sql.h"
//#include ".\fnc\dsc_sql.h"
#include ".\fnc\viw_cmn.h"

static	WCHAR	*Grd_ttl[] =	{	{ _T("CurrencyName")},
									{ _T("Code")	},
									{ _T("Rate")	},
									{ _T("Base")	},
									{ _T("Scond")	},
									{ _T("ADPD")	} };

static	GRD_CLM	Grd_clm[] =		{	{ 176, GAL_LFT, GCT_NML,  12 },		// Currency
									{  80, GAL_LFT, GCT_NML,  12 },		// Code
									{ 116, GAL_LFT, GCT_NML,  12 },		// Rate
									{  52, GAL_LFT, GCT_CHK,  12 },		// Base
									{  52, GAL_LFT, GCT_CHK,  12 },		// Scond
									{  48, GAL_LFT, GCT_NML,  12 } };	// ADPD

#define	IDC_GRD_CRC		(8192)	// Copy destination selecting grid

#define	CLM_DAT			0		// Data keeping column

#define	CLM_NAM			0		// Currency
#define	CLM_COD			1		// Code
#define	CLM_RTE			2		// Rate
#define	CLM_BSE			3		// Base
#define	CLM_SCD			4		// Scond
#define	CLM_ADP			5		// ADPD


// CCurrencyCvsDlg ダイアログ

IMPLEMENT_DYNAMIC(CCurrencyCvsDlg, CDialog)

CCurrencyCvsDlg::CCurrencyCvsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCurrencyCvsDlg::IDD, pParent)
{
	m_Crc_grd = NULL;
	m_pPms_mng = NULL;
	m_pDbs_obj = NULL;
	m_Ope_cod = NULL;
	m_Brc_idx = -1;
	m_Fcl_idx = -1;
	memset( m_Brc_rot, 0, sizeof(m_Brc_rot) );
	memset( m_Fcl_rot, 0, sizeof(m_Fcl_rot) );
	memset( m_Crc_rot, 0, sizeof(m_Crc_rot) );
}

CCurrencyCvsDlg::~CCurrencyCvsDlg()
{
	if ( m_Crc_grd )	delete m_Crc_grd;
	Del_All( m_Brc_rot );
	Del_All( m_Fcl_rot );
	Del_All( m_Crc_rot );
}

void CCurrencyCvsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CCurrencyCvsDlg::SetupComponent()
{
	int				idx;
	WINDOWPLACEMENT	wpc[1];
	RECT			rct[1];
	GRD_DEF			gdf[1];

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

	memset( gdf, 0, sizeof(gdf) );

	GetDlgItem(IDC_COMBO_BRANCH)->GetWindowPlacement( wpc );
	*rct = wpc->rcNormalPosition;
	//rct->left = 10;
	rct->top += 30;
	rct->right = 0;
	rct->bottom = 0;

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
	gdf->sel = GSL_ROW;
	gdf->hdr = TRUE;
	gdf->fsa = TRUE;

	m_Crc_grd = new CSimpleGrid();
	m_Crc_grd->Create( gdf, *rct, IDC_GRD_CRC, this );
	for ( idx=0; idx<memcnt(Grd_ttl); idx++ ){
		m_Crc_grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}
}


int	 CCurrencyCvsDlg::InitData( WCHAR *opc, WCHAR *bcd, WCHAR *fcd )
{
	int			chk;
	Csr			csr=NULL;
	WCHAR		*erm=NULL;
	BRC_INF		*brc;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);

	m_Ope_cod = opc;
	m_Brc_cod = bcd;
	m_Fcl_cod = fcd;

	m_Brc_idx = -1;
	m_Fcl_idx = -1;
	Del_All( m_Brc_rot );
	Del_All( m_Fcl_rot );
	Del_All( m_Crc_rot );
	bcx->ResetContent();
	fcx->ResetContent();
	m_Crc_grd->ResetContent();

	SetEditData( NULL );

	if ( chk = Get_Mst( m_Brc_rot, GTM_BRC_INF, NULL, NULL, &erm, m_pDbs_obj, m_pPms_mng ) ){
		bcx->InsertString(-1, _T("") );
		while( csr = Nxt_Lnk( m_Brc_rot, csr ) ){
			brc = Rec_Of(BRC_INF, csr);
			bcx->SetItemData( bcx->InsertString(-1, EDT_DBF_WST(brc->nam, _T("")) ), (DWORD)(LONG_PTR)brc );
		}
	}
	bcx->SetCurSel(0);
	OnCbnSelchangeComboBranch();

	return( chk );
}

void	CCurrencyCvsDlg::DispData( CRC_CVS *ccv, int row )
{
	WCHAR		buf[40];

	m_Crc_grd->SetCellText( CLM_NAM, row, EDT_DBF_WST(ccv->nam, _T("")) );
	m_Crc_grd->SetCellText( CLM_COD, row, EDT_DBF_WST(ccv->crc, _T("")) );
	m_Crc_grd->SetCellText( CLM_RTE, row, EDT_DBF_DBL(buf, memcnt(buf), ccv->rte, _T("%.12f"), Edt_Dbl_Nml) );
	m_Crc_grd->SetCheck( CLM_BSE, row, !!DBS_INT_VAL(ccv->isb) );
	m_Crc_grd->SetCheck( CLM_SCD, row, !!DBS_INT_VAL(ccv->iss) );
	m_Crc_grd->SetCellText( CLM_ADP, row, EDT_DBF_INT(buf, memcnt(buf), ccv->adp, Edt_Num_Nml) );
	m_Crc_grd->SetData( CLM_DAT, row, (int)(LONG_PTR)ccv );
}

static	void	set_enb(
CWnd			*tgt,
DWORD			cid,			
Csr				rot,
CRC_CVS			*ccv )
{
	int			chk, enb, fnd=FALSE;
	Csr			csr=NULL;
	CRC_CVS		*otr;

	if ( !(enb = !ccv) ){
		while ( csr = Nxt_Lnk( rot, csr ) ){
			otr = Rec_Of(CRC_CVS, csr);
			chk = cid==IDC_CHECK_BASE ? !!DBS_INT_VAL(otr->isb): !!DBS_INT_VAL(otr->iss);
			if ( chk ){
				if ( DBS_INT_VAL(otr->id) == DBS_INT_VAL(ccv->id) ){
					enb = TRUE;
				}
				else{
					fnd = TRUE;
				}
				break;
			}
		}
	}
	tgt->EnableWindow( !!ccv && ( !fnd || enb ) );
}

void	CCurrencyCvsDlg::SetEditData( CRC_CVS *ccv )
{
	WCHAR		buf[40];

	GetDlgItem(IDC_EDIT_CATEGORYNAME)->SetWindowTextW( !!ccv ? EDT_DBF_WST(ccv->nam, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_CURRENCYCODE)->SetWindowTextW( !!ccv ? EDT_DBF_WST(ccv->crc, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_EXCHANGERATE)->SetWindowTextW( !!ccv ? EDT_DBF_DBL(buf, memcnt(buf), ccv->rte, _T("%.12f"), Edt_Dbl_Nml): _T("") );
	GetDlgItem(IDC_EDIT_ADPD)->SetWindowTextW( !!ccv ? EDT_DBF_INT(buf, memcnt(buf), ccv->adp, Edt_Num_Nml): _T("") );
	((CButton *)GetDlgItem(IDC_CHECK_BASE))->SetCheck( !!ccv && DBS_INT_VAL(ccv->isb) );
	((CButton *)GetDlgItem(IDC_CHECK_SECONDALY))->SetCheck( !!ccv && DBS_INT_VAL(ccv->iss) );
	set_enb( GetDlgItem(IDC_CHECK_BASE), IDC_CHECK_BASE, m_Crc_rot, ccv );
	set_enb( GetDlgItem(IDC_CHECK_SECONDALY), IDC_CHECK_SECONDALY, m_Crc_rot, ccv );

	ButtonEnable();
}

void	CCurrencyCvsDlg::ButtonEnable()	
{
	int			enb, sel;
	enb  = ((CComboBox *)GetDlgItem(IDC_COMBO_BRANCH))->GetCurSel() != -1;
	enb &= ((CComboBox *)GetDlgItem(IDC_COMBO_FACILITY))->GetCurSel() != -1;

	sel = m_Crc_grd->GetSelRow() > 0;

	GetDlgItem(IDC_EDIT_CATEGORYNAME)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_CURRENCYCODE)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_EXCHANGERATE)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ADPD)->EnableWindow( enb );
	//GetDlgItem(IDC_CHECK_BASE)->EnableWindow( enb );
	//GetDlgItem(IDC_CHECK_SECONDALY)->EnableWindow( enb );
	GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow( sel && enb );
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow( enb);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow( sel && enb );
}

int CCurrencyCvsDlg::GetData( CRC_CVS * dst )
{
	WCHAR		buf[128], *itm=NULL;
	double		fvl;
	int			chk, ivl;
		
	WIN_TXT_DBS(dst->nam, IDC_EDIT_CATEGORYNAME, this);
	if ( Nul_Fld(dst->nam, FLD_WST) ){
		itm = _T("CurrencyName");
	}
	WIN_TXT_DBS(dst->crc, IDC_EDIT_CURRENCYCODE, this);
	if ( Nul_Fld(dst->crc, FLD_WST) ){
		itm = _T("CurrencyCode");
	}
	WIN_CBX_DBS(dst->isb, IDC_CHECK_BASE, this );
	WIN_CBX_DBS(dst->iss, IDC_CHECK_SECONDALY, this );
	GetDlgItem(IDC_EDIT_EXCHANGERATE)->GetWindowText( buf, memcnt(buf) );
	if ( wcslen( buf ) ){
		fvl = _wtof( buf );
		DBS_DBL_SET( dst->rte, &fvl );
	}
	else{
		itm = _T("ExchangeRate");
	}
	GetDlgItem(IDC_EDIT_ADPD)->GetWindowText( buf, memcnt(buf) );
	if ( wcslen( buf ) ){
		ivl = _wtoi( buf );
		DBS_INT_SET( dst->adp, &ivl );
	}
	else{
		itm = _T("AfterDecimalPointDigit");
	}
	if ( !( chk = !itm) ){
		swprintf_s(  buf, memcnt(buf), _T("%s is required item."), itm );
		MessageBox( buf, NULL, MB_ICONEXCLAMATION|MB_OK );
	}

	return( chk );
}


DWORD CCurrencyCvsDlg::ConfirmSaveChaned( int msg, int fce )
{
	Csr			csr=NULL;
	CRC_CVS		*ccv;
	int			ans=IDYES;
	WCHAR		*erm=NULL;

	while ( csr=Nxt_Lnk( m_Crc_rot, csr ) ){
		ccv = Rec_Of(CRC_CVS, csr);
		if ( !Nul_Fld(ccv->isb, FLD_INT) && !!DBS_INT_VAL(ccv->isb) ){
			break;
		}
	}
	if ( fce || ( !Cnt_Lnk( m_Crc_rot) || !!csr ) ){
		for( csr=NULL; csr=Nxt_Lnk( m_Crc_rot, csr ); ){
			ccv = Rec_Of(CRC_CVS, csr);
			if ( !Nul_Fld(ccv->mdt, FLD_DTM) || !Nul_Fld(ccv->ddt, FLD_DTM) || Nul_Fld(ccv->id, FLD_INT) ){
				break;
			}
		}
		if ( !!csr ){
			ans = Viw_Cfm_Svc( m_Crc_rot, Mfm_Sav_Crc, msg, fce, this, m_pDbs_obj, m_pPms_mng );
		}
	}
	else{
		MessageBox( _T("BasicalCurrency should be ON on one of either currency."), NULL, MB_ICONEXCLAMATION|MB_OK );
		ans = IDCANCEL;
	}
	return( ans );
}



BEGIN_MESSAGE_MAP(CCurrencyCvsDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_CBN_SELCHANGE(IDC_COMBO_BRANCH, &CCurrencyCvsDlg::OnCbnSelchangeComboBranch)
	ON_CBN_SELCHANGE(IDC_COMBO_FACILITY, &CCurrencyCvsDlg::OnCbnSelchangeComboFacility)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CCurrencyCvsDlg::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CCurrencyCvsDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CCurrencyCvsDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDCANCEL, &CCurrencyCvsDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CCurrencyCvsDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// CCurrencyCvsDlg メッセージ ハンドラ
BOOL CCurrencyCvsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pPms_mng = ((CMainFrame *)GetParent())->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)GetParent())->m_Dbs_obj;

	SetupComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// グリッドイベント
LRESULT CCurrencyCvsDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;
	int				row;
	CRC_CVS			*ccv;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:	row = ntf->row;
								if ( row > 0 ){
									ccv = (CRC_CVS *)(LONG_PTR)m_Crc_grd->GetData( CLM_DAT, row );
									if ( ccv ){
										SetEditData( ccv );
									}
								}
								break;
		}
		free( ntf );
	}

	return( rsl );
}


// Branch combobox change
void CCurrencyCvsDlg::OnCbnSelchangeComboBranch()
{
	int			idx;
	Csr			csr=NULL;
	WCHAR		*erm=NULL;
	BRC_INF		*brc=NULL;
	FCL_INF		*fcl;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);

	if ( ( ( idx = bcx->GetCurSel() ) != -1 ) && idx!=m_Brc_idx ){
		if ( ConfirmSaveChaned( TRUE, FALSE )!=IDCANCEL ){
			Del_All( m_Fcl_rot );
			Del_All( m_Crc_rot );
			fcx->ResetContent();
			m_Crc_grd->ResetContent();
			fcx->InsertString(-1, _T("") );
			if ( idx > 0 ){
				brc = (BRC_INF *)(LONG_PTR)bcx->GetItemData( idx );
				if ( Get_Mst( m_Fcl_rot, GTM_FCL_INF, DBS_WST_PTR(brc->brc), NULL, &erm, m_pDbs_obj, m_pPms_mng ) ){
					while( csr = Nxt_Lnk( m_Fcl_rot, csr ) ){
						fcl = Rec_Of(FCL_INF, csr);
						fcx->SetItemData( fcx->InsertString(-1, EDT_DBF_WST(fcl->nam, _T("")) ), (DWORD)(LONG_PTR)fcl );
					}
				}
			}
			fcx->SetCurSel(0);
			m_Fcl_idx = -1;
			OnCbnSelchangeComboFacility();
		}
		else{
			bcx->SetCurSel( idx = m_Brc_idx );
		}
	}
	m_Brc_idx = idx;
}

// Facility combobox change
void CCurrencyCvsDlg::OnCbnSelchangeComboFacility()
{
	int			idx, bix, row=0;
	Csr			csr=NULL;
	BRC_INF		*brc=NULL;
	FCL_INF		*fcl=NULL;
	CRC_CVS		*ccv, key[1];
	WCHAR		*erm=NULL;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);

	memset( key, 0, sizeof(key) );

	if ( ( ( idx = fcx->GetCurSel() ) != -1 ) && idx!=m_Fcl_idx ){
		if ( ConfirmSaveChaned( TRUE, FALSE )!=IDCANCEL ){
			Del_All( m_Crc_rot );
			m_Crc_grd->ResetContent();
			wcscpy_s( DBS_WST_PTR(key->opc), FLD_WST_SIZ(key->opc), m_pPms_mng->opc );
			if ( idx>0 ){
				fcl = (FCL_INF *)(LONG_PTR)fcx->GetItemData( idx );
				DBS_WST_CPY(key->brc, fcl->brc);
				DBS_WST_CPY(key->fcc, fcl->fcc);
			}
			else{
				if ( ( bix = bcx->GetCurSel() ) > 0 ){
					brc = (BRC_INF *)(LONG_PTR)bcx->GetItemData( bix );
					DBS_WST_CPY(key->brc, brc->brc);
				}
			}
			if ( Mfm_Get_Crc( m_Crc_rot, key, FALSE, &erm, m_pDbs_obj, m_pPms_mng ) ){
				m_Crc_grd->AddRow( Cnt_Lnk( m_Crc_rot ) );
				while( csr = Nxt_Lnk( m_Crc_rot, csr ) ){
					ccv = Rec_Of(CRC_CVS, csr);
					Set_Nul(ccv->mdt, FLD_DTM, TRUE);
					DispData( ccv, ++row );
				}
			}
		}
		else{
			fcx->SetCurSel( idx = m_Fcl_idx );
		}
	}
	ButtonEnable();
	m_Fcl_idx = idx;
}

// Updae button clich
void CCurrencyCvsDlg::OnBnClickedButtonUpdate()
{
	int			row;
	CRC_CVS		tmp[1], *ccv;
	Day			dte[1];

	if ( ( row = m_Crc_grd->GetSelRow() ) > 0 ){
		ccv = (CRC_CVS *)(LONG_PTR)m_Crc_grd->GetData( CLM_DAT, row );
		*tmp = *ccv;
		if ( GetData( tmp ) ){
			*ccv = *tmp;
			Get_Dte( dte );
			DBS_DTM_SET(ccv->mdt, dte);
			DispData( ccv, row );
			SetEditData( NULL );
			m_Crc_grd->SetSelectCell(-1, -1);
		}
	}
}

// Add button click
void CCurrencyCvsDlg::OnBnClickedButtonAdd()
{
	int			row, idx, bix;
	Csr			csr;
	CRC_CVS		tmp[1], *ccv;
	BRC_INF		*brc=NULL;
	FCL_INF		*fcl=NULL;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);


	memset( tmp, 0, sizeof(tmp) );

	if ( ( idx = fcx->GetCurSel() ) != -1 && ( bix = bcx->GetCurSel() ) != -1 ){
		if ( bix>0 )	brc = (BRC_INF *)(LONG_PTR)fcx->GetItemData( bix );
		if ( idx>0 )	fcl = (FCL_INF *)(LONG_PTR)fcx->GetItemData( idx );
		if ( GetData( tmp ) ){
			if ( csr=Add_Lnk( m_Crc_rot, tmp, sizeof(CRC_CVS) ) ){
				ccv = Rec_Of(CRC_CVS, csr);
				DBS_WST_SET(ccv->opc, m_pPms_mng->opc);
				if ( !!brc ){
					DBS_WST_SET(ccv->brc, DBS_WST_PTR(brc->brc));
				}
				if ( !!fcl ){
					DBS_WST_SET(ccv->fcc, DBS_WST_PTR(fcl->fcc));
				}
				row = m_Crc_grd->GetLineCount( FALSE );
				m_Crc_grd->AddRow( 1 );
				DispData( ccv, row+1 );
				SetEditData( NULL );
				m_Crc_grd->SetSelectCell(-1, -1);
			}
		}
		else{
			MessageBox( _T("CurrencyName is requierd item"), NULL, MB_ICONEXCLAMATION|MB_OK );
		}
	}
}

// Delete button click
void CCurrencyCvsDlg::OnBnClickedButtonDelete()
{
	int			row;
	CRC_CVS		*ccv;
	Day			dte[1];

	if ( ( row = m_Crc_grd->GetSelRow() ) > 0 ){
		ccv = (CRC_CVS *)(LONG_PTR)m_Crc_grd->GetData( CLM_DAT, row );
		if ( MessageBox( _T("Are you sure to delete?"), NULL, MB_ICONQUESTION|MB_YESNO ) == IDYES ){
			if ( Nul_Fld(ccv->id, FLD_INT) ){
				Del_Lnk( m_Crc_rot, Csr_Of(ccv) );
			}
			else{
				Get_Dte( dte );
				DBS_DAY_SET(ccv->ddt, dte);
			}
			m_Crc_grd->DelRow( row );
			m_Crc_grd->SetSelectCell(-1, -1);
			m_Crc_grd->Redraw();
		}
	}
}

// Close button click
void CCurrencyCvsDlg::OnBnClickedCancel()
{
	if ( ConfirmSaveChaned( TRUE, TRUE )==IDYES ){
		OnCancel();
	}
}

// Close winth save button click
void CCurrencyCvsDlg::OnBnClickedOk()
{
	if ( ConfirmSaveChaned( FALSE, FALSE )==IDYES ){
		OnOK();
	}
}

BOOL CCurrencyCvsDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}
