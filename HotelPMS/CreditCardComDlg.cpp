// CreditCardComDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MainFrm.h"
#include "CreditCardComDlg.h"

#include ".\fnc\cdc_sql.h"
#include ".\fnc\mfm_sql.h"
#include ".\fnc\sql_cmn.h"
#include ".\fnc\viw_cmn.h"

// CCreditCardComDlg ダイアログ

static	WCHAR	*Grd_ttl[] =	{	{ _T("Code")		},
									{ _T("Name")		},
									{ _T("Cls.Day")		},
									{ _T("PayDay")		},
									{ _T("Rate")		}};

static	GRD_CLM	Grd_clm[] =		{	{  96, GAL_LFT, GCT_NML,  12 },		// Code
									{ 256, GAL_LFT, GCT_NML,  12 },		// Name
									{  96, GAL_CTR, GCT_NML,  12 },		// CloseDay
									{  96, GAL_CTR, GCT_NML,  12 },		// PayDay
									{  96, GAL_CTR, GCT_NML,  12 }};	// RateFee

#define	IDC_GRD_BRC		(8192)	// Copy destination selecting grid

#define	CLM_DAT			0		// Data keeping column

#define	CLM_COD			0		// Code
#define	CLM_NAM			1		// Name
#define	CLM_CDY			2		// CloseDay
#define	CLM_PDY			3		// PayDay
#define	CLM_FEE			4		// RateFee


static	int		get_idx(		// Getting combobox index
CComboBox		*cbx,			// ComboBox
WCHAR			*ccd )			// Code
{
	int			idx, cnt;

	cnt = cbx->GetCount();

	for ( idx=0; idx<cnt; idx++ ){
		if ( !wcscmp( ccd, (WCHAR *)(LONG_PTR)(cbx->GetItemData(idx)) ) ){
			break;
		}
	}

	return( idx<cnt ? idx: -1 );
}

static	int		get_day(
Day			*dst,
CWnd		*edt )
{
	int			chk=TRUE;
	WCHAR		buf[10];
	Day			dte[1] = { 2000, 12, 0, 0, 0, 0 };

	edt->GetWindowText( buf, memcnt(buf) );
	if ( wcslen( trim(buf) ) > 0 ){
		dte->day = _wtoi( buf );
		if ( chk = ( dte->day >=1 && dte->day <=31 ) ){
			DBS_DAY_SET(dst, dte);
		}
	}
	else{
		DBS_DAY_SET(dst, (Day *)NULL);
	}

	return( chk );
}

IMPLEMENT_DYNAMIC(CCreditCardComDlg, CDialog)

CCreditCardComDlg::CCreditCardComDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreditCardComDlg::IDD, pParent)
{
	m_pPms_mng = NULL;
	m_pDbs_obj = NULL;
	m_Ope_cod = NULL;
	m_Cdc_grd = NULL;
	memset( m_Cdc_rot, 0, sizeof(m_Cdc_rot) );
}

CCreditCardComDlg::~CCreditCardComDlg()
{
	if ( m_Cdc_grd )	delete m_Cdc_grd;
	Del_All( m_Cdc_rot );
}

void CCreditCardComDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCreditCardComDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CCreditCardComDlg::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CCreditCardComDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CCreditCardComDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDCANCEL, &CCreditCardComDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CCreditCardComDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCreditCardComDlg メッセージ ハンドラ

int	 CCreditCardComDlg::InitData( WCHAR *opc )
{
	WCHAR		*erm=NULL;
	CDC_COM		*cdc;
	CON_TRY		*ctc;
	int			chk, row=0;
	Csr			csr=NULL;
	CComboBox	*ccb=(CComboBox *)GetDlgItem(IDC_COMBO_COUNTRY);

	m_Ope_cod = opc;

	Del_All( m_Cdc_rot );
	m_Cdc_grd->ResetContent();

	SetEditData( NULL );

	if ( chk = Cdc_Get_Dat( m_Cdc_rot, opc, &erm, m_pDbs_obj, m_pPms_mng ) ){
		m_Cdc_grd->AddRow( Cnt_Lnk( m_Cdc_rot ) );
		while( csr = Nxt_Lnk( m_Cdc_rot, csr ) ){
			cdc = Rec_Of(CDC_COM, csr);
			Set_Nul(cdc->mdt, FLD_DTM, TRUE);
			DispData( cdc, ++row );
		}
	}
	else{
		if( !!erm ){
			MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
	}
	
	if ( chk ){
		while( csr=Nxt_Lnk( m_pPms_mng->ccl, csr ) ){
			ctc = Rec_Of(CON_TRY, csr);
			ccb->SetItemData( ccb->InsertString( -1, DBS_WST_PTR(ctc->ctn) ), (DWORD)(LONG_PTR)DBS_WST_PTR(ctc->ctc) );
		}
	}

	return( chk );
}

void CCreditCardComDlg::SetupComponent()
{
	int				idx;
	RECT			rct[1];
	GRD_DEF			gdf[1];

	struct	{
		CWnd	*wnd;
		int		lmt;
		DWORD	cid;
	}Cpn_[] = {
		{ NULL,      255, IDC_EDIT_COMPANYNAME		},
		{ NULL,      128, IDC_EDIT_COMPANYCODE		},
		{ NULL,       16, IDC_EDIT_ZIPCODE			},
		{ NULL,      255, IDC_EDIT_ADDRESS1			},
		{ NULL,      255, IDC_EDIT_ADDRESS2			},
		{ NULL,       30, IDC_EDIT_TELEPHONE1		},
		{ NULL,       30, IDC_EDIT_TELEPHONE2		},
		{ NULL,       30, IDC_EDIT_FAXNUMBER		},
		{ NULL,      255, IDC_EDIT_WEBURL			},
		{ NULL,      128, IDC_EDIT_EMAIL1			},
		{ NULL,      128, IDC_EDIT_EMAIL2			},
		{ NULL,        2, IDC_EDIT_CLOSEDAY			},
		{ NULL,        2, IDC_EDIT_PAYDAY			},
		{ NULL,        3, IDC_EDIT_RATE				},
//		{ NULL,       -1, IDC_STATIC_COMPANYNAME	},
		{ NULL,       -1, IDC_STATIC_ABBREVIATION	},
		{ NULL,       -1, IDC_STATIC_COMPANYCODE	},
		{ NULL,       -1, IDC_STATIC_ZIPCODE		},
		{ NULL,       -1, IDC_STATIC_ADDRESS1		},
		{ NULL,       -1, IDC_STATIC_ADDRESS2		},
		{ NULL,       -1, IDC_STATIC_TELEPHONE1		},
		{ NULL,       -1, IDC_STATIC_TELEPHONE2		},
		{ NULL,       -1, IDC_STATIC_FAX			},
		{ NULL,       -1, IDC_STATIC_WEBURL			},
		{ NULL,       -1, IDC_STATIC_EMAIL1			},
		{ NULL,       -1, IDC_STATIC_EMAIL2			}
	};

	for ( idx=0; idx<memcnt(Cpn_); idx++ ){
		if ( !!Cpn_[idx].wnd ){
			Cpn_[idx].wnd->SubclassDlgItem(Cpn_[idx].cid, this);
		}
		if ( Cpn_[idx].lmt!=-1 )	((CEdit *)GetDlgItem(Cpn_[idx].cid))->SetLimitText( Cpn_[idx].lmt );
	}

	memset( gdf, 0, sizeof(gdf) );

	GetClientRect( rct );
	rct->left = 10;
	rct->top += 10;
	rct->right = 0;
	rct->bottom = 0;

	gdf->lnh = 20;
	gdf->vlc = 7;
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

	m_Cdc_grd = new CSimpleGrid();
	m_Cdc_grd->Create( gdf, *rct, IDC_GRD_BRC, this );
	for ( idx=0; idx<memcnt(Grd_ttl); idx++ ){
		m_Cdc_grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}
}

int		CCreditCardComDlg::GetData( CDC_COM *cdc )
{
	int			chk, idx;
	double		fvl;
	WCHAR		buf[96], *itm=NULL, *wrg=NULL;
	CComboBox	*ccb=(CComboBox *)GetDlgItem(IDC_COMBO_COUNTRY);

//	if ( m_Cdc_grd->GetSelRow() > 0 ){
		WIN_TXT_DBS(cdc->nam, IDC_EDIT_COMPANYNAME, this);
//		WIN_TXT_DBS(cdc->abr, IDC_EDIT_ABBREVIATION, this);
		WIN_TXT_DBS(cdc->ccc, IDC_EDIT_COMPANYCODE, this);
		WIN_TXT_DBS(cdc->zip, IDC_EDIT_ZIPCODE, this);
		WIN_TXT_DBS(cdc->adr1, IDC_EDIT_ADDRESS1, this);
		WIN_TXT_DBS(cdc->adr2, IDC_EDIT_ADDRESS2, this);
		WIN_TXT_DBS(cdc->tel1, IDC_EDIT_TELEPHONE1, this);
		WIN_TXT_DBS(cdc->tel2, IDC_EDIT_TELEPHONE2, this);
		WIN_TXT_DBS(cdc->fax, IDC_EDIT_FAXNUMBER, this);
		WIN_TXT_DBS(cdc->url, IDC_EDIT_WEBURL, this);
		WIN_TXT_DBS(cdc->eml1, IDC_EDIT_EMAIL1, this);
		WIN_TXT_DBS(cdc->eml2, IDC_EDIT_EMAIL2, this);
		GetDlgItem(IDC_EDIT_RATE)->GetWindowText( buf, memcnt(buf) );
		if ( chk = wcslen( trim(buf) ) > 0 ){
			if ( chk = (fvl = _wtof( buf )) <= 100 ){
				DBS_DBL_SET(cdc->rte, &fvl);
			}
			else{
				wrg = _T("RateFee");
			}
		}
		else{
			itm = _T("RateFee");
		}
		if ( chk && !( chk = get_day( cdc->cld, GetDlgItem(IDC_EDIT_CLOSEDAY) ) ) ){
			wrg = _T("CloseDay");
		}
		if ( chk && !( chk = get_day( cdc->pdy, GetDlgItem(IDC_EDIT_PAYDAY) ) ) ){
			wrg = _T("PayDay");
		}

		if ( Nul_Fld(cdc->id, FLD_INT) ){
			DBS_WST_SET(cdc->opc, m_Ope_cod);
		}
		if ( ( idx = ccb->GetCurSel() ) != -1 ){
			Set_Nul( cdc->ctr, FLD_WST, NULL );
			wcscpy_s( DBS_WST_PTR(cdc->ctr), FLD_WST_SIZ(cdc->ctr), (WCHAR *)(LONG_PTR)ccb->GetItemData(idx) );
		}
		else{
			Set_Nul( cdc->ctr, FLD_WST, TRUE );
		}

		if ( chk && !( chk = !Nul_Fld(cdc->nam, FLD_WST) ) )	itm=_T("CredircardCompanyName");	
		if ( chk && !( chk = !Nul_Fld(cdc->ccc, FLD_WST) ) )	itm=_T("CredircardCompanyCode");
		if ( !chk ){
			swprintf_s( buf, memcnt(buf),  _T("%s is %s"), itm ? itm: wrg, itm ? _T("required item"): _T("wrong") );
			MessageBox( buf, NULL, MB_ICONEXCLAMATION|MB_OK );
		}
//	}

	return( chk );
}

void	CCreditCardComDlg::DispData( CDC_COM *cdc, int row )
{
	WCHAR		*knd=_T("Unknown"), buf[40];

	m_Cdc_grd->SetCellText( CLM_COD, row, EDT_DBF_WST(cdc->ccc, _T("")) );
	m_Cdc_grd->SetCellText( CLM_NAM, row, EDT_DBF_WST(cdc->nam, _T("")) );
	swprintf_s( buf, memcnt(buf), Nul_Fld(cdc->cld, FLD_DAY) ? _T(""): _T("%d"), DBS_DAY_PTR(cdc->cld)->day );
	m_Cdc_grd->SetCellText( CLM_CDY, row, buf );
	swprintf_s( buf, memcnt(buf), Nul_Fld(cdc->pdy, FLD_DAY) ? _T(""): _T("%d"), DBS_DAY_PTR(cdc->pdy)->day );
	m_Cdc_grd->SetCellText( CLM_PDY, row, buf );
	m_Cdc_grd->SetCellText( CLM_FEE, row, EDT_DBF_DBL(buf, memcnt(buf), cdc->rte,_T("%.0f"), Edt_Dbl_Nml) );

	m_Cdc_grd->SetData( CLM_DAT, row, (int)(LONG_PTR)cdc );
}

void	CCreditCardComDlg::SetEditData( CDC_COM *cdc )
{
	int			fnd=FALSE;
	Csr			csr=NULL;
	WCHAR		buf[10];
	CComboBox	*ccb=(CComboBox *)GetDlgItem(IDC_COMBO_COUNTRY);

	GetDlgItem(IDC_EDIT_COMPANYNAME)->SetWindowText( !!cdc ? EDT_DBF_WST(cdc->nam,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_COMPANYCODE)->SetWindowText( !!cdc ? EDT_DBF_WST(cdc->ccc,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_ZIPCODE)->SetWindowText( !!cdc ? EDT_DBF_WST(cdc->zip,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_ADDRESS1)->SetWindowText( !!cdc ? EDT_DBF_WST(cdc->adr1,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_ADDRESS2)->SetWindowText( !!cdc ? EDT_DBF_WST(cdc->adr2,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_TELEPHONE1)->SetWindowText( !!cdc ? EDT_DBF_WST(cdc->tel1,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_TELEPHONE2)->SetWindowText( !!cdc ? EDT_DBF_WST(cdc->tel2,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText( !!cdc ? EDT_DBF_WST(cdc->fax,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_WEBURL)->SetWindowText( !!cdc ? EDT_DBF_WST(cdc->url,_T("")):_T("") );
	if ( !!cdc && !Nul_Fld(cdc->rte, FLD_DBL) ){
		swprintf_s( buf, memcnt(buf), _T("%.0f"), DBS_DBL_VAL(cdc->rte) );
	}
	else{
		*buf = 0;
	}
	GetDlgItem(IDC_EDIT_RATE)->SetWindowText( buf );
	GetDlgItem(IDC_EDIT_EMAIL1)->SetWindowText( !!cdc ? EDT_DBF_WST(cdc->eml1,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_EMAIL2)->SetWindowText( !!cdc ? EDT_DBF_WST(cdc->eml2,_T("")):_T("") );
	if ( !!cdc && !Nul_Fld(cdc->cld, FLD_DAY) ){
		swprintf_s(buf, memcnt(buf), _T("%d"), DBS_DAY_PTR(cdc->cld)->day );
	}
	else{
		*buf = 0;
	}
	GetDlgItem(IDC_EDIT_CLOSEDAY)->SetWindowText( buf );
	if ( !!cdc && !Nul_Fld(cdc->pdy, FLD_DAY) ){
		swprintf_s(buf, memcnt(buf), _T("%d"), DBS_DAY_PTR(cdc->pdy)->day );
	}
	else{
		*buf = 0;
	}
	GetDlgItem(IDC_EDIT_PAYDAY)->SetWindowText( buf );
	ccb->SetCurSel( !!cdc && !Nul_Fld(cdc->ctr, FLD_WST) ? get_idx( ccb, DBS_WST_PTR(cdc->ctr) ): -1 );

	ButtonEnable();
}

void	CCreditCardComDlg::ButtonEnable()	
{
	int			enb=TRUE, sel;

	sel = m_Cdc_grd->GetSelRow() > 0;

	GetDlgItem(IDC_EDIT_COMPANYNAME)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_COMPANYCODE)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ZIPCODE)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ADDRESS1)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ADDRESS2)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_TELEPHONE1)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_TELEPHONE2)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_FAXNUMBER)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_WEBURL)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_EMAIL1)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_EMAIL2)->EnableWindow( enb );

	GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow( sel && enb );
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow( enb);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow( sel && enb );
}

DWORD CCreditCardComDlg::ConfirmSaveChaned( int msg, int fce )
{
	Csr			csr=NULL;
	CDC_COM		*cdc;
	int			ans=IDYES;
	WCHAR		*erm=NULL;

	for( csr=NULL; csr=Nxt_Lnk( m_Cdc_rot, csr ); ){
		cdc = Rec_Of(CDC_COM, csr);
		if ( !Nul_Fld(cdc->mdt, FLD_DTM) || !Nul_Fld(cdc->ddt, FLD_DTM) || Nul_Fld(cdc->id, FLD_INT) ){
			break;
		}
	}
	if ( !!csr ){
		ans = Viw_Cfm_Svc( m_Cdc_rot, Cdc_Sav_Dat, msg, fce, this, m_pDbs_obj, m_pPms_mng );
	}

	return( ans );
}

BOOL CCreditCardComDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pPms_mng = ((CMainFrame *)GetParent())->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)GetParent())->m_Dbs_obj;

	SetupComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// グリッドイベント
LRESULT CCreditCardComDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;
	int				row;
	CDC_COM			*cdc;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:	row = ntf->row;
								if ( row > 0 ){
									cdc = (CDC_COM *)(LONG_PTR)m_Cdc_grd->GetData( CLM_DAT, row );
									if ( cdc ){
										SetEditData( cdc );
									}
								}
								break;
		}
		free( ntf );
	}

	return( rsl );
}

// Update button clicked
void CCreditCardComDlg::OnBnClickedButtonUpdate()
{
	int			row;
	CDC_COM		tmp[1], *cdc;
	Day			dte[1];

	if ( ( row = m_Cdc_grd->GetSelRow() ) > 0 ){
		cdc = (CDC_COM *)(LONG_PTR)m_Cdc_grd->GetData( CLM_DAT, row );
		*tmp = *cdc;
		if ( GetData( tmp ) ){
			*cdc = *tmp;
			Get_Dte( dte );
			DBS_DTM_SET(cdc->mdt, dte);
			DispData( cdc, row );
			SetEditData( NULL );
			m_Cdc_grd->SetSelectCell( -1, -1 );
		}
	}
}

// Add button clicked
void CCreditCardComDlg::OnBnClickedButtonAdd()
{
	int			row;
	Csr			csr;
	CDC_COM		tmp[1], *cdc;

	memset( tmp, 0, sizeof(tmp) );

	if ( GetData( tmp ) ){
		DBS_WST_SET(tmp->opc, m_pPms_mng->opc );
		if ( csr = Add_Lnk( m_Cdc_rot, tmp, sizeof(tmp) ) ){
			cdc = Rec_Of(CDC_COM, csr);
			row = m_Cdc_grd->GetLineCount( FALSE );
			m_Cdc_grd->AddRow( 1 );
			DispData( cdc, ++row );
			m_Cdc_grd->Redraw();
			SetEditData( NULL );
			m_Cdc_grd->SetSelectCell( -1, -1 );
		}
	}
}

// Deletebuton clicked
void CCreditCardComDlg::OnBnClickedButtonDelete()
{
	int			row;
	CDC_COM		*cdc;
	Day			dte[1];

	if ( ( row = m_Cdc_grd->GetSelRow() ) > 0 ){
		cdc = (CDC_COM *)(LONG_PTR)m_Cdc_grd->GetData( CLM_DAT, row );
		if ( MessageBox( _T("Are you sure to delete?"), NULL, MB_ICONQUESTION|MB_YESNO ) == IDYES ){
			if ( Nul_Fld(cdc->id, FLD_INT) ){
				Del_Lnk( m_Cdc_rot, Csr_Of(cdc) );
			}
			else{
				Get_Dte( dte );
				DBS_DAY_SET(cdc->ddt, dte);
			}
			m_Cdc_grd->DelRow( row );
			m_Cdc_grd->SetSelectCell(-1, -1);
			m_Cdc_grd->Redraw();
		}
	}
}

void CCreditCardComDlg::OnBnClickedOk()
{
	if ( ConfirmSaveChaned( FALSE, FALSE )==IDYES ){
		OnOK();
	}
}

void CCreditCardComDlg::OnBnClickedCancel()
{
	if ( ConfirmSaveChaned( TRUE, TRUE )==IDYES ){
		OnCancel();
	}
}

BOOL CCreditCardComDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}
