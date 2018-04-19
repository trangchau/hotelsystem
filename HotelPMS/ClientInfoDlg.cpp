// ClientInfoDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "ClientInfoDlg.h"
#include "ClientSearchDlg.h"
#include ".\fnc\viw_cmn.h"
#include ".\fnc\mfm_sql.h"


// CClientInfoDlg ダイアログ
//IDC_EDIT_FIRSTNAME,
//IDC_EDIT_FAMILYNAME
//IDC_CHECK_MALE
//IDC_CHECK_FEMALE
//IDC_EDIT_ADDRESS1
//IDC_EDIT_ADDRESS2
//IDC_EDIT_ZIPCODE
//IDC_EDIT_TELEPHONE1
//IDC_EDIT_TELEPHONE2
//IDC_EDIT_FAXNUMBER
//IDC_COMBO_CLIENTRANK
//IDC_EDIT_DATEOFBIRTH
//IDC_CHECK_SMOKER2
//IDC_CHECK_SMOKER
//IDC_EDIT_EMAIL1
//IDC_EDIT_EMAIL2
//IDC_COMBO_NATIONALITY
//IDC_COMBO_LANGUAGE
//IDC_COMBO_SYSTEMLANGUAGE
//IDC_EDIT_IDNUMBER
//IDC_EDIT_DATEOFISSUE
//IDC_EDIT_DATEOFEXPIRE
//IDC_EDIT_IMMIGRATIONVISANO
//IDC_EDIT_VISAEXPIREDATE
//IDC_EDIT_CREDITCARDNO
//IDC_EDIT_CREDITCARDTITLE
//IDC_EDIT_VALIDTHRU,

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

void CClientInfoDlg::SetupComponent()
{
	int				idx;
	WCHAR			*fnm = _T("MS UI Gothic");
	Csr				csr=NULL;

	//WINDOWPLACEMENT	wpc[1];

	struct	{
		CWnd	*wnd;
		int		lmt;
		DWORD	cid;
	}Cpn_[] = {
		{ m_Fst_nam,255, IDC_EDIT_FIRSTNAME				},
		{ m_Fam_nam,255, IDC_EDIT_FAMILYNAME			},
		{ NULL,		255, IDC_EDIT_ADDRESS1				},
		{ NULL,		255, IDC_EDIT_ADDRESS2				},
		{ NULL,		 30, IDC_EDIT_ZIPCODE				},
		{ NULL,		255, IDC_EDIT_TELEPHONE1			},
		{ NULL,		255, IDC_EDIT_TELEPHONE2			},
		{ NULL,		255, IDC_EDIT_FAXNUMBER				},
		{ m_Bir_dte, -1, IDC_EDIT_DATEOFBIRTH			},
		{ NULL,		255, IDC_EDIT_EMAIL1				},
		{ NULL,		255, IDC_EDIT_EMAIL2				},
		{ m_Cli_rnk,-1,  IDC_COMBO_CLIENTRANK			},
		{ NULL,		255, IDC_EDIT_IDNUMBER				},
		{ m_Isu_dte, -1, IDC_EDIT_DATEOFISSUE			},
		{ m_Exp_dte, -1, IDC_EDIT_DATEOFEXPIRE			},
		{ NULL,		 40, IDC_EDIT_IMMIGRATIONVISANO		},
		{ m_Exp_Vsa, -1, IDC_EDIT_VISAEXPIREDATE		},
		{ NULL,		 20, IDC_EDIT_CREDITCARDNO			},
		{ NULL,		 64, IDC_EDIT_CREDITCARDTITLE		},
		{ NULL,		  5, IDC_EDIT_VALIDTHRU				}
	};

	for ( idx=0; idx<memcnt(Cpn_); idx++ ){
		if ( !!Cpn_[idx].wnd )		Cpn_[idx].wnd->SubclassDlgItem(Cpn_[idx].cid, this);
		if ( Cpn_[idx].lmt!=-1 )	((CEdit *)GetDlgItem(Cpn_[idx].cid))->SetLimitText( Cpn_[idx].lmt );
	}

	// Font setting
	m_Fst_nam->SetFont(fnm, 20, RGB(0,0,0) );
	m_Fam_nam->SetFont(fnm, 20, RGB(0,0,0) );

	// Hide the MarketSegment button if edit the client_infos of reservation_details
	GetDlgItem(IDC_STATIC_MARKETSEGMENT)->ShowWindow( m_Mks_edt ? SW_SHOWNORMAL: SW_HIDE );
	GetDlgItem(IDC_COMBO_MARKETSEGMENT)->ShowWindow( m_Mks_edt ? SW_SHOWNORMAL: SW_HIDE );

}

int		CClientInfoDlg::GetData()
{
	int			chk, gdr, bck1, bck2, err, idx, rnk;
	WCHAR		*itm=NULL, *erm=NULL, buf[192];
	CComboBox	*ccb=(CComboBox *)GetDlgItem(IDC_COMBO_NATIONALITY);
	CComboBox	*lcb=(CComboBox *)GetDlgItem(IDC_COMBO_LANGUAGE);
	CComboBox	*scb=(CComboBox *)GetDlgItem(IDC_COMBO_SYSTEMLANGUAGE);
	CComboBox	*mcb=(CComboBox *)GetDlgItem(IDC_COMBO_MARKETSEGMENT);

	WIN_TXT_DBS( m_Edt_buf->fnm, IDC_EDIT_FIRSTNAME, this );
	WIN_TXT_DBS( m_Edt_buf->lnm, IDC_EDIT_FAMILYNAME, this );
	bck1 = ((CButton *)GetDlgItem(IDC_CHECK_MALE))->GetCheck();
	bck2 = ((CButton *)GetDlgItem(IDC_CHECK_FEMALE))->GetCheck();
	if ( bck1 )			gdr = GDR_MALE;
	else if ( bck2 )	gdr = GDR_FEMALE;
	else				gdr = GDR_VOID;
	DBS_INT_SET(m_Edt_buf->gdr, &gdr); 
	WIN_TXT_DBS( m_Edt_buf->adr1, IDC_EDIT_ADDRESS1, this );
	WIN_TXT_DBS( m_Edt_buf->adr2, IDC_EDIT_ADDRESS2, this );
	WIN_TXT_DBS( m_Edt_buf->zip, IDC_EDIT_ZIPCODE, this );
	WIN_TXT_DBS( m_Edt_buf->tel1, IDC_EDIT_TELEPHONE1, this );
	WIN_TXT_DBS( m_Edt_buf->tel2, IDC_EDIT_TELEPHONE2, this );
	WIN_TXT_DBS( m_Edt_buf->fax, IDC_EDIT_FAXNUMBER, this );
	WIN_DAY_DBS( m_Edt_buf->bth, &err, IDC_EDIT_DATEOFBIRTH, this );
	if ( !( chk = !err ) ){
		itm = _T("DateOfBirth");
	}
	WIN_CBX_DBS( m_Edt_buf->mrd, IDC_CHECK_MARRIED, this );
	WIN_CBX_DBS( m_Edt_buf->ism, IDC_CHECK_SMOKER, this );
	WIN_TXT_DBS( m_Edt_buf->eml1, IDC_EDIT_EMAIL1, this );
	WIN_TXT_DBS( m_Edt_buf->eml2, IDC_EDIT_EMAIL2, this );
	WIN_TXT_DBS( m_Edt_buf->idn, IDC_EDIT_IDNUMBER, this );
	WIN_DAY_DBS( m_Edt_buf->isd, &err, IDC_EDIT_DATEOFISSUE, this );
	if ( chk && !( chk = !err ) ){
		itm = _T("DateOfIssue");
	}
	WIN_DAY_DBS( m_Edt_buf->exd, &err, IDC_EDIT_DATEOFEXPIRE, this );
	if ( chk && !( chk = !err ) ){
		itm = _T("DateOfExpire");
	}
	if ( chk && !Nul_Fld(m_Edt_buf->isd, FLD_DTM) && !Nul_Fld(m_Edt_buf->exd, FLD_DTM) ){
		if ( !( chk = Cmp_Dte( DBS_DAY_PTR(m_Edt_buf->isd), DBS_DAY_PTR(m_Edt_buf->exd) ) < 0 ) ){
			erm = _T("DateOfExpire should be grater than DateOfIssue");
		}
	}
	// syay period check is necessary!!!!!!
	WIN_TXT_DBS( m_Edt_buf->vsn, IDC_EDIT_IMMIGRATIONVISANO, this );
	WIN_DAY_DBS( m_Edt_buf->vex, &err, IDC_EDIT_VISAEXPIREDATE, this );
	if ( chk && !( chk = !err ) ){
		itm = _T("VISAExpireDate");
	}
	WIN_TXT_DBS( m_Edt_buf->ccn, IDC_EDIT_CREDITCARDNO, this );
	WIN_TXT_DBS( m_Edt_buf->cct, IDC_EDIT_CREDITCARDTITLE, this );

	if ( ( idx = ccb->GetCurSel() ) != -1 ){
		DBS_WST_SET(m_Edt_buf->nto, (WCHAR *)(LONG_PTR)ccb->GetItemData(idx));
		//wcscpy_s( DBS_WST_PTR(m_Edt_buf->nto), FLD_WST_SIZ(m_Edt_buf->nto), (WCHAR *)(LONG_PTR)ccb->GetItemData(idx) );
	}
	if ( ( idx = lcb->GetCurSel() ) != -1 ){
		DBS_WST_SET(m_Edt_buf->lng, (WCHAR *)(LONG_PTR)lcb->GetItemData(idx));
		//wcscpy_s( DBS_WST_PTR(m_Edt_buf->lng), FLD_WST_SIZ(m_Edt_buf->lng), (WCHAR *)(LONG_PTR)lcb->GetItemData(idx) );
	}
	if ( ( idx = scb->GetCurSel() ) != -1 ){
		DBS_WST_SET(m_Edt_buf->slg, (WCHAR *)(LONG_PTR)scb->GetItemData(idx));
		//wcscpy_s( DBS_WST_PTR(m_Edt_buf->slg), FLD_WST_SIZ(m_Edt_buf->slg), (WCHAR *)(LONG_PTR)lcb->GetItemData(idx) );
	}
	if ( chk && ( chk = ( rnk = m_Cli_rnk->GetRank() ) != -1 ) )	DBS_INT_SET(m_Edt_buf->crk,&rnk);
	else															DBS_INT_SET(m_Edt_buf->crk,(int *)0);

	if ( !erm && !!itm ){
		swprintf_s( buf, memcnt(buf), _T("%s is wrong"), itm );
		erm = buf;
	}
	if ( erm )	MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );

	m_Mkt_seg = 0;
	if ( m_Mks_edt && mcb->GetCurSel() > 0 ){
		m_Mkt_seg = (int)(LONG_PTR)mcb->GetItemData( mcb->GetCurSel() );
	}

	return( chk );
//IDC_COMBO_CLIENTRANK
//IDC_EDIT_VALIDTHRU,
}

IMPLEMENT_DYNAMIC(CClientInfoDlg, CDialog)

CClientInfoDlg::CClientInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientInfoDlg::IDD, pParent)
{
	m_pPms_mng = NULL;
	m_pDbs_obj = NULL;
	m_Dte_pcr = NULL;
	m_Fcs_edt = NULL;
	m_Mkt_seg = 0;
	m_Mks_edt = FALSE;
	memset( m_Edt_buf, 0, sizeof(m_Edt_buf) );
}

CClientInfoDlg::~CClientInfoDlg()
{
//	if ( m_Dte_pcr )	m_Dte_pcr->SendMessage( WM_CLOSE, 0, 0 );
}

void CClientInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClientInfoDlg, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CClientInfoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CClientInfoDlg::OnBnClickedCancel)
	ON_EN_KILLFOCUS(IDC_EDIT_DATEOFBIRTH, &CClientInfoDlg::OnEnKillfocusEditDateofbirth)
	ON_WM_EDITNOTIFY()
	ON_WM_DATEPICKERNOTIFY()
	ON_EN_KILLFOCUS(IDC_EDIT_DATEOFEXPIRE, &CClientInfoDlg::OnEnKillfocusEditDateofexpire)
	ON_EN_KILLFOCUS(IDC_EDIT_DATEOFISSUE, &CClientInfoDlg::OnEnKillfocusEditDateofissue)
	ON_EN_KILLFOCUS(IDC_EDIT_VISAEXPIREDATE, &CClientInfoDlg::OnEnKillfocusEditVisaexpiredate)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CHECK_MALE, &CClientInfoDlg::OnBnClickedCheckMale)
	ON_BN_CLICKED(IDC_CHECK_FEMALE, &CClientInfoDlg::OnBnClickedCheckFemale)
	ON_BN_CLICKED(IDC_BUTTON1, &CClientInfoDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

void	CClientInfoDlg::ShowDatePickerDlg( DWORD cid )
{
	Day			dte[1];
	WCHAR		buf[40];

	if ( m_Dte_pcr && !(m_Dte_pcr->IsWindowVisible() ) ){
		m_Fcs_edt = GetDlgItem(cid);
		m_Fcs_edt->GetWindowText( buf, memcnt(buf) );
		if ( !Str_Dte( dte, buf ) ){
			Get_Dte( dte );
		}

		m_Dte_pcr->SetNotifyWindow( this );
		m_Dte_pcr->SetDefaultDate( dte );
		m_Dte_pcr->SetPosition( this, GetDlgItem(cid) );
		m_Dte_pcr->SetLimit( 0, 0, dte->yer, dte->mth );
		//m_Dte_pcr->SetParent( this );
		m_Dte_pcr->ShowWindow( SW_NORMAL );
	}
}


// Timer event
void CClientInfoDlg::DispData()
{
	int			chk, rnk, idx;
	WCHAR		dbf[20];
	CComboBox	*ccb=(CComboBox *)GetDlgItem(IDC_COMBO_NATIONALITY);
	CComboBox	*lcb=(CComboBox *)GetDlgItem(IDC_COMBO_LANGUAGE);
	CComboBox	*scb=(CComboBox *)GetDlgItem(IDC_COMBO_SYSTEMLANGUAGE);
	CComboBox	*mcb=(CComboBox *)GetDlgItem(IDC_COMBO_MARKETSEGMENT);

//m_Cli_inf
	// name
	GetDlgItem(IDC_EDIT_FIRSTNAME)->SetWindowText( EDT_DBF_WST(m_Edt_buf->fnm,_T("")) );
	GetDlgItem(IDC_EDIT_FAMILYNAME)->SetWindowText( EDT_DBF_WST(m_Edt_buf->lnm,_T("")) );
	//Gender
	chk = !Nul_Fld(m_Edt_buf->gdr, FLD_INT) && DBS_INT_VAL(m_Edt_buf->gdr)==GDR_MALE;
	((CButton *)GetDlgItem(IDC_CHECK_MALE))->SetCheck( chk );
	chk = !Nul_Fld(m_Edt_buf->gdr, FLD_INT) && DBS_INT_VAL(m_Edt_buf->gdr)==GDR_FEMALE;
	((CButton *)GetDlgItem(IDC_CHECK_FEMALE))->SetCheck( chk );
	//Address
	GetDlgItem(IDC_EDIT_ADDRESS1)->SetWindowText( EDT_DBF_WST(m_Edt_buf->adr1,_T("")) );
	GetDlgItem(IDC_EDIT_ADDRESS2)->SetWindowText( EDT_DBF_WST(m_Edt_buf->adr2,_T("")) );
	// Zip
	GetDlgItem(IDC_EDIT_ZIPCODE)->SetWindowText( EDT_DBF_WST(m_Edt_buf->zip,_T("")) );
	// Telephone
	GetDlgItem(IDC_EDIT_TELEPHONE1)->SetWindowText( EDT_DBF_WST(m_Edt_buf->tel1,_T("")) );
	GetDlgItem(IDC_EDIT_TELEPHONE2)->SetWindowText( EDT_DBF_WST(m_Edt_buf->tel2,_T("")) );
	// Fax
	GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText( EDT_DBF_WST(m_Edt_buf->fax,_T("")) );
//IDC_COMBO_CLIENTRANK
	// Date of birth
	GetDlgItem(IDC_EDIT_DATEOFBIRTH)->SetWindowText( EDT_DBF_DAY(dbf, memcnt(dbf), m_Edt_buf->bth ) );
	// Married
	chk = !Nul_Fld(m_Edt_buf->mrd, FLD_INT) && !!DBS_INT_VAL(m_Edt_buf->mrd);
	((CButton *)GetDlgItem(IDC_CHECK_MARRIED))->SetCheck( chk );
	// Smoker
	chk = !Nul_Fld(m_Edt_buf->ism, FLD_INT) && !!DBS_INT_VAL(m_Edt_buf->ism);
	((CButton *)GetDlgItem(IDC_CHECK_SMOKER))->SetCheck( chk );
	// Email
	GetDlgItem(IDC_EDIT_EMAIL1)->SetWindowText( EDT_DBF_WST(m_Edt_buf->eml1,_T("")) );
	GetDlgItem(IDC_EDIT_EMAIL2)->SetWindowText( EDT_DBF_WST(m_Edt_buf->eml2,_T("")) );

	ccb->SetCurSel( get_idx( ccb, DBS_WST_PTR(m_Edt_buf->nto) ) );
	lcb->SetCurSel( get_idx( lcb, DBS_WST_PTR(m_Edt_buf->lng) ) );
	scb->SetCurSel( get_idx( scb, DBS_WST_PTR(m_Edt_buf->slg) ) );
	// ClientRank
	rnk = Nul_Fld(m_Edt_buf->crk, FLD_INT) ? CLL_DEFAULT: DBS_INT_VAL(m_Edt_buf->crk);
	m_Cli_rnk->SetRank( rnk );
	// ID Number
	GetDlgItem(IDC_EDIT_IDNUMBER)->SetWindowText( EDT_DBF_WST(m_Edt_buf->idn,_T("")) );
	// Date of issue
	GetDlgItem(IDC_EDIT_DATEOFISSUE)->SetWindowText( EDT_DBF_DAY(dbf, memcnt(dbf), m_Edt_buf->isd ) );
	// Date of Expire
	GetDlgItem(IDC_EDIT_DATEOFEXPIRE)->SetWindowText( EDT_DBF_DAY(dbf, memcnt(dbf), m_Edt_buf->exd ) );
	// VISA Number
	GetDlgItem(IDC_EDIT_IMMIGRATIONVISANO)->SetWindowText( EDT_DBF_DAY(dbf, memcnt(dbf), m_Edt_buf->vsn ) );
	// VISA Expire date
	GetDlgItem(IDC_EDIT_VISAEXPIREDATE)->SetWindowText( EDT_DBF_DAY(dbf, memcnt(dbf), m_Edt_buf->vex ) );
	// Creditcard Number
	GetDlgItem(IDC_EDIT_CREDITCARDNO)->SetWindowText( EDT_DBF_DAY(dbf, memcnt(dbf), m_Edt_buf->ccn ) );
	// Creditcard Title
	GetDlgItem(IDC_EDIT_CREDITCARDTITLE)->SetWindowText( EDT_DBF_DAY(dbf, memcnt(dbf), m_Edt_buf->cct ) );
//IDC_EDIT_VALIDTHRU,

	for ( idx=0; idx<mcb->GetCount(); idx++ ){
		if ( (int)(LONG_PTR)mcb->GetItemData( idx ) == m_Mkt_seg ){
			mcb->SetCurSel( idx );
			break;
		}
	}
}

// CClientInfoDlg メッセージ ハンドラ

BOOL CClientInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	Day		dte[1];

	SetupComponent();

	Get_Dte( dte );
	m_Dte_pcr = new CDatePickerDlg( this );
	m_Dte_pcr->Initialize( CDR_DLG_SINGLE, dte->yer, dte->mth );

	SetTimer( 1, 10, NULL );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// Timer event
void CClientInfoDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer( nIDEvent );
	int			/*chk, rnk, */idx;
	Csr			csr=NULL;
	//WCHAR		dbf[20];
	CON_TRY		*ctc;
	LAN_GAG		*lgc;
	MKT_SEG		*mks;
	CComboBox	*ccb=(CComboBox *)GetDlgItem(IDC_COMBO_NATIONALITY);
	CComboBox	*lcb=(CComboBox *)GetDlgItem(IDC_COMBO_LANGUAGE);
	CComboBox	*scb=(CComboBox *)GetDlgItem(IDC_COMBO_SYSTEMLANGUAGE);
	CComboBox	*mcb=(CComboBox *)GetDlgItem(IDC_COMBO_MARKETSEGMENT);

	while( csr=Nxt_Lnk( m_pPms_mng->ccl, csr ) ){
		ctc = Rec_Of(CON_TRY, csr);
		ccb->SetItemData( ccb->InsertString( -1, DBS_WST_PTR(ctc->ctn) ), (DWORD)(LONG_PTR)DBS_WST_PTR(ctc->ctc) );
	}
	while( csr=Nxt_Lnk( m_pPms_mng->lgl, csr ) ){
		lgc = Rec_Of(LAN_GAG, csr);
		lcb->SetItemData( lcb->InsertString( -1, DBS_WST_PTR(lgc->lgn) ), (DWORD)(LONG_PTR)DBS_WST_PTR(lgc->lgc) );
		scb->SetItemData( scb->InsertString( -1, DBS_WST_PTR(lgc->lgn) ), (DWORD)(LONG_PTR)DBS_WST_PTR(lgc->lgc) );
	}

	*m_Edt_buf = *m_Cli_inf;
	mcb->ResetContent();
	if ( m_Mks_edt ){
		mcb->SetItemData( mcb->InsertString( -1, _T("") ), 0 );
		while ( csr=Nxt_Lnk( m_pPms_mng->msl, csr ) ){
			mks = Rec_Of(MKT_SEG, csr);
			mcb->SetItemData( idx = mcb->InsertString( -1, EDT_DBF_WST(mks->nam,_T("")) ), (LONG_PTR)DBS_INT_VAL(mks->id) );
//			if ( m_Mkt_seg == DBS_INT_VAL(mks->id) ){
//				mcb->SetCurSel( idx );
//			}
		}
	}
	DispData();
	CDialog::OnTimer(nIDEvent);
}

// search  button click
void CClientInfoDlg::OnBnClickedButton1()
{
	CClientSearchDlg		*dlg;
	WCHAR					fst[256], fam[256];

	if ( dlg = new CClientSearchDlg(this) ){
		GetDlgItem(IDC_EDIT_FIRSTNAME)->GetWindowText( fst, memcnt(fst) );
		GetDlgItem(IDC_EDIT_FAMILYNAME)->GetWindowText( fam, memcnt(fam) );
		dlg->m_pDbs_obj = m_pDbs_obj;
		dlg->m_pPms_mng = m_pPms_mng;
		dlg->m_Fst_nam = fst;
		dlg->m_Lst_nam = fam;
		if ( dlg->DoModal()==IDOK ){
			*m_Edt_buf = *dlg->m_Cli_inf;
			DispData();
		}
		delete	dlg;
	}
}

LRESULT CClientInfoDlg::OnEditNotify( WPARAM wParam, LPARAM lParam )
{
	DWORD		cid;

	if ( LOWORD(wParam)==EN_RBUTTONCLICK ){
		cid = HIWORD(wParam);
		if ( cid==IDC_EDIT_DATEOFBIRTH || cid==IDC_EDIT_DATEOFISSUE || IDC_EDIT_DATEOFEXPIRE || IDC_EDIT_VISAEXPIREDATE){
			ShowDatePickerDlg(cid);
		}
	}
	return( 0 );
}

void CClientInfoDlg::OnDatePickerNotify( WPARAM wParam, LPARAM lParam )
{
	WCHAR			buf[20];

	switch( wParam ){
	case DPC_SELECTDATE:	swprintf_s( buf, memcnt(buf), _T("%4d/%02d/%02d"), lParam/10000, (lParam/100)%100, lParam % 100 );
							if ( m_Fcs_edt )	m_Fcs_edt->SetWindowText( buf );
							break;
	}
}

void CClientInfoDlg::OnBnClickedCheckMale()
{
	if ( ((CButton *)GetDlgItem(IDC_CHECK_MALE))->GetCheck() ){
		((CButton *)GetDlgItem(IDC_CHECK_FEMALE))->SetCheck(FALSE);
	}
}

void CClientInfoDlg::OnBnClickedCheckFemale()
{
	if ( ((CButton *)GetDlgItem(IDC_CHECK_FEMALE))->GetCheck() ){
		((CButton *)GetDlgItem(IDC_CHECK_MALE))->SetCheck(FALSE);
	}
}

void CClientInfoDlg::OnEnKillfocusEditDateofbirth()
{
	m_Dte_pcr->ShowWindow( SW_HIDE );
}

void CClientInfoDlg::OnEnKillfocusEditDateofexpire()
{
	m_Dte_pcr->ShowWindow( SW_HIDE );
}

void CClientInfoDlg::OnEnKillfocusEditDateofissue()
{
	m_Dte_pcr->ShowWindow( SW_HIDE );
}

void CClientInfoDlg::OnEnKillfocusEditVisaexpiredate()
{
	m_Dte_pcr->ShowWindow( SW_HIDE );
}

void CClientInfoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_Dte_pcr->ShowWindow( SW_HIDE );

	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CClientInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}

// OK button click
void CClientInfoDlg::OnBnClickedOk()
{
	if ( GetData() ){
		*m_Cli_inf = *m_Edt_buf;
		OnOK();
	}
}

// CANCEL button click
void CClientInfoDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OnCancel();
}
