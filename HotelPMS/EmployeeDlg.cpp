// EmployeeDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "EmployeeDlg.h"
#include "MainFrm.h"
#include ".\fnc\mfm_sql.h"
#include ".\fnc\epl_sql.h"
#include ".\fnc\viw_cmn.h"

#include ".\md5\md5_cpt.h"

static	WCHAR	*Grd_ttl[] =	{	{ _T("Code")	},
									{ _T("Name")	},
									{ _T("Gender")	},
									{ _T("Post")	},
									{ _T("Emp.Date")},
									{ _T("Kind")	} };

static	GRD_CLM	Grd_clm[] =		{	{  96, GAL_LFT, GCT_NML,  12 },		// Code
									{ 192, GAL_LFT, GCT_NML,  12 },		// Name
									{  64, GAL_CTR, GCT_NML,  12 },		// Gender
									{ 160, GAL_LFT, GCT_NML,  12 },		// Post
									{  88, GAL_CTR, GCT_NML,  12 },		// Employment date
									{  96, GAL_LFT, GCT_NML,  12 } };	// Kind

#define	IDC_GRD_EPL		(8192)	// Copy destination selecting grid

#define	CLM_DAT			0		// Data keeping column

#define	CLM_COD			0		// Code
#define	CLM_NAM			1		// Name
#define	CLM_GDR			2		// Gender
#define	CLM_PST			3		// Post
#define	CLM_EPD			4		// Employment date
#define	CLM_KND			5		// Kind

//IDC_COMBO_BRANCH
//IDC_COMBO_FACILITY
//IDC_EDIT_FIRSTNAME
//IDC_EDIT_LASTNAME
//IDC_EDIT_ZIPCODE
//IDC_EDIT_EMPLOYEECODE
//IDC_COMBO_KIND
//IDC_EDIT_ADDRESS1
//IDC_EDIT_ADDRESS2
//IDC_EDIT_TELEPHONE1
//IDC_EDIT_TELEPHONE2
//IDC_EDIT_EMAIL1
//IDC_EDIT_EMAIL2
//IDC_CHECK_MALE
//IDC_CHECK_FEMALE
//IDC_EDIT_DATEOFBIRTH
//IDC_CHECK_MARRIED
//IDC_COMBO_POST
//IDC_COMBO_LANGUAGE
//IDC_COMBO_SYSTEMLANGUAGE
//IDC_COMBO_NATIONALITY
//IDC_EDIT_EMPLOYMENTDATE
//IDC_EDIT_IDNUMBER
//IDC_EDIT_DATEOFISSUE
//IDC_EDIT_DATEOFEXPIRE

// CEmployeeDlg ダイアログ

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

/*
static	WCHAR	*get_knd(		// Getting employee kind name
EPL_CDN			*epk,			// name table
int				cnt,			// table count
int				knd )			// kind code
{
	int				idx;
	WCHAR			*nam;

	for ( nam=NULL, idx=0; idx<cnt; idx++ ){
		if ( knd==(epk+idx)->cod ){
			nam = (epk+idx)->nam;
			break;
		}
	}

	return( nam );
}
*/


IMPLEMENT_DYNAMIC(CEmployeeDlg, CDialog)

CEmployeeDlg::CEmployeeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEmployeeDlg::IDD, pParent)
{
	m_Epl_grd = NULL;
	m_pPms_mng = NULL;
	m_pDbs_obj = NULL;
	m_Ope_cod = NULL;
	m_Brc_cod = NULL;
	m_Fcl_cod = NULL;
	m_Brc_idx = -1;
	m_Fcl_idx = -1;
	memset( m_Brc_rot, 0, sizeof(m_Brc_rot) );
	memset( m_Fcl_rot, 0, sizeof(m_Fcl_rot) );
	memset( m_Epl_rot, 0, sizeof(m_Epl_rot) );
	memset( m_Dty_rot, 0, sizeof(m_Dty_rot) );
}

CEmployeeDlg::~CEmployeeDlg()
{
	if ( m_Epl_grd )	delete m_Epl_grd;
	Del_All( m_Brc_rot );
	Del_All( m_Fcl_rot );
	Del_All( m_Epl_rot );
	Del_All( m_Dty_rot );
}

void CEmployeeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
void CEmployeeDlg::SetupComponent()
{
	int				idx, cnt;
	WCHAR			*fnm = _T("MS UI Gothic");
	WINDOWPLACEMENT	wpc[1];
	GRD_DEF			gdf[1];
	EPL_CDN			*epk;
	CComboBox		*cbx = (CComboBox *)GetDlgItem(IDC_COMBO_KIND);

	struct	{
		CWnd	*wnd;
		int		lmt;
		DWORD	cid;
	}Cpn_[] = {
		{ NULL,        -1, IDC_COMBO_BRANCH			},
		{ NULL,        -1, IDC_COMBO_FACILITY		},
		{ NULL,       255, IDC_EDIT_FIRSTNAME		},
		{ NULL,       255, IDC_EDIT_LASTNAME		},
		{ NULL,        40, IDC_EDIT_ZIPCODE			},
		{ NULL,        20, IDC_EDIT_EMPLOYEECODE	},
		{ NULL,        -1, IDC_COMBO_KIND			},
		{ NULL,       255, IDC_EDIT_ADDRESS1		},
		{ NULL,       255, IDC_EDIT_ADDRESS2		},
		{ NULL,        40, IDC_EDIT_TELEPHONE1		},
		{ NULL,        40, IDC_EDIT_TELEPHONE2		},
		{ NULL,       128, IDC_EDIT_EMAIL1			},
		{ NULL,       128, IDC_EDIT_EMAIL2			},
		{ NULL,        -1, IDC_CHECK_MALE			},
		{ NULL,        -1, IDC_CHECK_FEMALE			},
		{ m_Bth_day,   -1, IDC_EDIT_DATEOFBIRTH		},
		{ NULL,        -1, IDC_CHECK_MARRIED		},
		{ NULL,        -1, IDC_COMBO_POST			},
		{ NULL,        -1, IDC_COMBO_LANGUAGE		},
		{ NULL,        -1, IDC_COMBO_SYSTEMLANGUAGE	},
		{ NULL,        -1, IDC_COMBO_NATIONALITY	},
		{ m_Epl_day,   -1, IDC_EDIT_EMPLOYMENTDATE	},
		{ NULL,        40, IDC_EDIT_IDNUMBER		},
		{ m_Iss_day,   -1, IDC_EDIT_DATEOFISSUE		},
		{ m_Exp_day,   -1, IDC_EDIT_DATEOFEXPIRE	}
	};

	for ( idx=0; idx<memcnt(Cpn_); idx++ ){
		if ( !!Cpn_[idx].wnd )		Cpn_[idx].wnd->SubclassDlgItem(Cpn_[idx].cid, this);
		if ( Cpn_[idx].lmt!=-1 )	((CEdit *)GetDlgItem(Cpn_[idx].cid))->SetLimitText( Cpn_[idx].lmt );
	}

	GetDlgItem(IDC_COMBO_BRANCH)->GetWindowPlacement( wpc );
	wpc->rcNormalPosition.top += 30;

	memset( gdf, 0, sizeof(gdf) );

	gdf->lnh = 20;
	gdf->vlc = 11;
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

	m_Epl_grd = new CSimpleGrid();
	m_Epl_grd->Create( gdf, wpc->rcNormalPosition, IDC_GRD_EPL, this );
	for ( idx=0; idx<memcnt(Grd_ttl); idx++ ){
		m_Epl_grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}

	cnt = Viw_Get_Epl( &epk, EPL_KND );
	for ( idx=0; idx<cnt; idx++ ){
		cbx->SetItemData( cbx->InsertString( -1, (epk+idx)->nam ), (epk+idx)->cod ); 
	}
}

int	 CEmployeeDlg::InitData( WCHAR *opc, WCHAR *bcc, WCHAR *fcc )
{
	int			chk;
	Csr			csr=NULL;
	WCHAR		*erm=NULL;
	BRC_INF		*brc;
	CON_TRY		*ctc;
	LAN_GAG		*lgc;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);
	CComboBox	*ccb = (CComboBox *)GetDlgItem(IDC_COMBO_NATIONALITY);
	CComboBox	*lcb = (CComboBox *)GetDlgItem(IDC_COMBO_LANGUAGE);
	CComboBox	*scb = (CComboBox *)GetDlgItem(IDC_COMBO_SYSTEMLANGUAGE);

	m_Ope_cod = opc;
	m_Ope_cod = bcc;
	m_Ope_cod = fcc;

	m_Brc_idx = -1;
	m_Fcl_idx = -1;

	Del_All( m_Brc_rot );
	Del_All( m_Fcl_rot );
	Del_All( m_Epl_rot );
	Del_All( m_Dty_rot );
	bcx->ResetContent();
	fcx->ResetContent();
	ccb->ResetContent();
	lcb->ResetContent();
	scb->ResetContent();
	m_Epl_grd->ResetContent();

	while( csr=Nxt_Lnk( m_pPms_mng->ccl, csr ) ){
		ctc = Rec_Of(CON_TRY, csr);
		ccb->SetItemData( ccb->InsertString( -1, DBS_WST_PTR(ctc->ctn) ), (DWORD)(LONG_PTR)DBS_WST_PTR(ctc->ctc) );
	}
	while( csr=Nxt_Lnk( m_pPms_mng->lgl, csr ) ){
		lgc = Rec_Of(LAN_GAG, csr);
		lcb->SetItemData( lcb->InsertString( -1, DBS_WST_PTR(lgc->lgn) ), (DWORD)(LONG_PTR)DBS_WST_PTR(lgc->lgc) );
		scb->SetItemData( scb->InsertString( -1, DBS_WST_PTR(lgc->lgn) ), (DWORD)(LONG_PTR)DBS_WST_PTR(lgc->lgc) );
	}

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

void	CEmployeeDlg::DispData( EMP_LYE *epl, int row )
{
	int			cnt, idx;
	WCHAR		buf[520], *nam;
	EPL_CDN		*epk;
	DTY_INF		*dty;
	Csr			csr=NULL;

	cnt = Viw_Get_Epl( &epk, EPL_KND );

	m_Epl_grd->SetCellText( CLM_COD, row, EDT_DBF_WST(epl->epc, _T("")) );
	swprintf_s( buf, memcnt(buf), _T("%s %s"), EDT_DBF_WST(epl->fnm, _T("")), EDT_DBF_WST(epl->lnm, _T("")) ); 
	m_Epl_grd->SetCellText( CLM_NAM, row, buf );
	nam = NULL;
	if ( !Nul_Fld(epl->gdr, FLD_INT) ){
		switch( DBS_INT_VAL(epl->gdr) ){
		case GDR_MALE:		nam = _T("Male");	break;
		case GDR_FEMALE:	nam = _T("Female");	break;
		case GDR_VOID:		nam = NULL;			break;
		}
	}
	m_Epl_grd->SetCellText( CLM_GDR, row, nam ? nam: _T("") );
	for ( nam=NULL, idx=0; idx<cnt; idx++ ){
		if ( DBS_INT_VAL(epl->knd)==(epk+idx)->cod ){
			nam = (epk+idx)->nam;
			break;
		}
	}
	m_Epl_grd->SetCellText( CLM_KND, row, nam ? nam: _T("") );
	nam = NULL;
	if ( !Nul_Fld(epl->dti, FLD_INT) ){
		while ( csr = Nxt_Lnk( m_Dty_rot, csr ) ){
			dty = Rec_Of(DTY_INF, csr);
			if ( DBS_INT_VAL(dty->id)==DBS_INT_VAL(epl->dti) ){
				nam = DBS_WST_PTR(dty->nam);
				break;
			}
		}
	}
	m_Epl_grd->SetCellText( CLM_PST, row, nam ? nam: _T("") );
	m_Epl_grd->SetCellText( CLM_EPD, row, Edt_Dte(buf, memcnt(buf), DBS_DAY_PTR(epl->epd) ) );
	m_Epl_grd->SetData( CLM_DAT, row, (int)(LONG_PTR)epl );
}


void	CEmployeeDlg::SetEditData( EMP_LYE *epl )
{
	int			idx;
	WCHAR		buf[64];
	CComboBox	*kcb = (CComboBox *)GetDlgItem(IDC_COMBO_KIND);
	CComboBox	*pcb = (CComboBox *)GetDlgItem(IDC_COMBO_POST);
	CComboBox	*ccb = (CComboBox *)GetDlgItem(IDC_COMBO_NATIONALITY);
	CComboBox	*lcb = (CComboBox *)GetDlgItem(IDC_COMBO_LANGUAGE);
	CComboBox	*scb = (CComboBox *)GetDlgItem(IDC_COMBO_SYSTEMLANGUAGE);

	GetDlgItem(IDC_EDIT_FIRSTNAME)->SetWindowTextW( !!epl ? EDT_DBF_WST(epl->fnm, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_LASTNAME)->SetWindowTextW( !!epl ? EDT_DBF_WST(epl->lnm, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_ZIPCODE)->SetWindowTextW( !!epl ? EDT_DBF_WST(epl->zip, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_EMPLOYEECODE)->SetWindowTextW( !!epl ? EDT_DBF_WST(epl->epc, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_ADDRESS1)->SetWindowTextW( !!epl ? EDT_DBF_WST(epl->adr1, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_ADDRESS2)->SetWindowTextW( !!epl ? EDT_DBF_WST(epl->adr2, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_TELEPHONE1)->SetWindowTextW( !!epl ? EDT_DBF_WST(epl->tel1, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_TELEPHONE2)->SetWindowTextW( !!epl ? EDT_DBF_WST(epl->tel2, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_EMAIL1)->SetWindowTextW( !!epl ? EDT_DBF_WST(epl->eml1, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_EMAIL2)->SetWindowTextW( !!epl ? EDT_DBF_WST(epl->eml2, _T("")):_T(""));
	for ( idx=0; idx<kcb->GetCount(); idx++ ){
		if ( !!epl && kcb->GetItemData(idx)==DBS_INT_VAL(epl->knd) )	break;
	}
	kcb->SetCurSel( idx<kcb->GetCount() ? idx: -1 );
	((CButton *)GetDlgItem(IDC_CHECK_MALE))->SetCheck( !!epl && DBS_INT_VAL(epl->gdr)==GDR_MALE );
	((CButton *)GetDlgItem(IDC_CHECK_FEMALE))->SetCheck( !!epl && DBS_INT_VAL(epl->gdr)==GDR_FEMALE );
	GetDlgItem(IDC_EDIT_DATEOFBIRTH)->SetWindowTextW( !!epl ? EDT_DBF_DAY(buf, memcnt(buf), epl->bth): _T("") );
	((CButton *)GetDlgItem(IDC_CHECK_MARRIED))->SetCheck( !!epl && DBS_INT_VAL(epl->mrd)==GDR_FEMALE );
	if ( !!epl && !Nul_Fld(epl->dti, FLD_INT) ){
		for ( idx=0; idx<pcb->GetCount(); idx++ ){
			if ( pcb->GetItemData(idx)==DBS_INT_VAL(epl->dti) )	break;
		}
	}
	else{
		idx = -1;
	}
	pcb->SetCurSel( idx );
	ccb->SetCurSel( !!epl ? get_idx( ccb, DBS_WST_PTR(epl->nto) ): -1 );
	lcb->SetCurSel( !!epl ? get_idx( lcb, DBS_WST_PTR(epl->lng) ): -1 );
	scb->SetCurSel( !!epl ? get_idx( scb, DBS_WST_PTR(epl->slg) ): -1 );
	GetDlgItem(IDC_EDIT_EMPLOYMENTDATE)->SetWindowTextW( !!epl ? EDT_DBF_DAY(buf, memcnt(buf), epl->epd): _T("") );
	GetDlgItem(IDC_EDIT_IDNUMBER)->SetWindowTextW( !!epl ? EDT_DBF_WST(epl->idn, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_DATEOFISSUE)->SetWindowTextW( !!epl ? EDT_DBF_DAY(buf, memcnt(buf), epl->isd): _T("") );
	GetDlgItem(IDC_EDIT_DATEOFEXPIRE)->SetWindowTextW( !!epl ? EDT_DBF_DAY(buf, memcnt(buf), epl->exd): _T("") );
	ButtonEnable();
}

void	CEmployeeDlg::ButtonEnable()	
{
	int			enb, sel;
	enb  = ((CComboBox *)GetDlgItem(IDC_COMBO_BRANCH))->GetCurSel() != -1;
	enb &= ((CComboBox *)GetDlgItem(IDC_COMBO_FACILITY))->GetCurSel() != -1;

	sel = m_Epl_grd->GetSelRow() > 0;

	GetDlgItem(IDC_EDIT_FIRSTNAME)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_LASTNAME)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ZIPCODE)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_EMPLOYEECODE)->EnableWindow( enb );
	GetDlgItem(IDC_COMBO_KIND)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ADDRESS1)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ADDRESS2)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_TELEPHONE1)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_TELEPHONE2)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_EMAIL1)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_EMAIL2)->EnableWindow( enb );
	GetDlgItem(IDC_CHECK_MALE)->EnableWindow( enb );
	GetDlgItem(IDC_CHECK_FEMALE)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_DATEOFBIRTH)->EnableWindow( enb );
	GetDlgItem(IDC_CHECK_MARRIED)->EnableWindow( enb );
	GetDlgItem(IDC_COMBO_POST)->EnableWindow( enb );
	GetDlgItem(IDC_COMBO_LANGUAGE)->EnableWindow( enb );
	GetDlgItem(IDC_COMBO_SYSTEMLANGUAGE)->EnableWindow( enb );
	GetDlgItem(IDC_COMBO_NATIONALITY)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_EMPLOYMENTDATE)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_IDNUMBER)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_DATEOFISSUE)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_DATEOFEXPIRE)->EnableWindow( enb );

	GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow( sel && enb );
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow( enb);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow( sel && enb );
}

int CEmployeeDlg::GetData( EMP_LYE *dst )
{
	int				idx, val, chk, bde, ise, exe, epe, epi, rsl;
	WCHAR			buf[256], *itm=NULL, *wrg=NULL, *erm=NULL;
	HASH_UINT128	hsh[1];
	CComboBox		*ccb = (CComboBox *)GetDlgItem(IDC_COMBO_NATIONALITY);
	CComboBox		*lcb = (CComboBox *)GetDlgItem(IDC_COMBO_LANGUAGE);
	CComboBox		*scb = (CComboBox *)GetDlgItem(IDC_COMBO_SYSTEMLANGUAGE);

	WIN_TXT_DBS(dst->fnm, IDC_EDIT_FIRSTNAME, this);
	WIN_TXT_DBS(dst->lnm, IDC_EDIT_LASTNAME, this);
	WIN_TXT_DBS(dst->zip, IDC_EDIT_ZIPCODE, this);
	WIN_TXT_DBS(dst->epc, IDC_EDIT_EMPLOYEECODE, this );
	WIN_TXT_DBS(dst->adr1, IDC_EDIT_ADDRESS1, this );
	WIN_TXT_DBS(dst->adr2, IDC_EDIT_ADDRESS2, this );
	WIN_TXT_DBS(dst->tel1, IDC_EDIT_TELEPHONE1, this );
	WIN_TXT_DBS(dst->tel2, IDC_EDIT_TELEPHONE2, this );
	WIN_TXT_DBS(dst->eml1, IDC_EDIT_EMAIL1, this );
	WIN_TXT_DBS(dst->eml2, IDC_EDIT_EMAIL2, this );
	WIN_TXT_DBS(dst->idn, IDC_EDIT_IDNUMBER, this );
	if ( ((CButton *)GetDlgItem(IDC_CHECK_MALE))->GetCheck() )			val = GDR_MALE;
	else if ( ((CButton *)GetDlgItem(IDC_CHECK_FEMALE))->GetCheck() )	val = GDR_FEMALE;
	else																val = GDR_VOID;
	DBS_INT_SET(dst->gdr, &val);
	WIN_CBX_DBS(dst->mrd, IDC_CHECK_MARRIED, this );
	WIN_DAY_DBS(dst->bth, &bde, IDC_EDIT_DATEOFBIRTH, this );
	WIN_DAY_DBS(dst->isd, &ise, IDC_EDIT_DATEOFISSUE, this );
	WIN_DAY_DBS(dst->exd, &exe, IDC_EDIT_DATEOFEXPIRE, this );
	WIN_DAY_DBS(dst->epd, &epe, IDC_EDIT_EMPLOYMENTDATE, this );
	if ( ( idx = ((CComboBox *)GetDlgItem(IDC_COMBO_POST))->GetCurSel() ) != -1 ){
		val = (int)(LONG_PTR)((CComboBox *)GetDlgItem(IDC_COMBO_POST))->GetItemData(idx);
	}
	DBS_INT_SET(dst->dti, idx!=-1 ? &val: (int *)NULL);
	if ( ( idx = ccb->GetCurSel() ) != -1 ){
		DBS_WST_SET(dst->nto, (WCHAR *)(LONG_PTR)ccb->GetItemData(idx));
//		wcscpy_s( DBS_WST_PTR(dst->nto), FLD_WST_SIZ(dst->nto), (WCHAR *)(LONG_PTR)ccb->GetItemData(idx) );
	}
	if ( ( idx = lcb->GetCurSel() ) != -1 ){
		DBS_WST_SET(dst->lng, (WCHAR *)(LONG_PTR)lcb->GetItemData(idx));
//		wcscpy_s( DBS_WST_PTR(dst->lng), FLD_WST_SIZ(dst->lng), (WCHAR *)(LONG_PTR)lcb->GetItemData(idx) );
	}
	if ( ( idx = scb->GetCurSel() ) != -1 ){
		DBS_WST_SET(dst->slg, (WCHAR *)(LONG_PTR)scb->GetItemData(idx));
//		wcscpy_s( DBS_WST_PTR(dst->slg), FLD_WST_SIZ(dst->slg), (WCHAR *)(LONG_PTR)lcb->GetItemData(idx) );
	}

	GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowText( buf, memcnt(buf) );
	if ( !!wcslen(buf) ){
		md5_cpt( hsh, buf, (DWORD)wcslen(buf) );
		hsh->hex( buf );
		DBS_WST_SET(dst->pwd, buf);
	}

	if ( !( chk = !bde ) ){
		wrg = _T("DateOfBirth");
	}
	else if ( !( chk = !ise ) ){
		wrg = _T("DateOfIssue");
	}
	else if ( !( chk = !exe ) ){
		wrg = _T("DateOfExpire");
	}
	else if ( !( chk = !epe ) ){
		wrg = _T("EmploymentDate");
	}
	else if ( !( chk = !Nul_Fld(dst->epc, FLD_WST) ) ){
		itm = _T("EmployeeCode");
	}
	else if ( !( chk = !Nul_Fld(dst->fnm, FLD_WST) ) ){
		itm = _T("FirstName");
	}
	else if ( !( chk = !Nul_Fld(dst->lnm, FLD_WST) ) ){
		itm = _T("LastName");
	}
	else if ( !( chk = !Nul_Fld(dst->bth, FLD_DAY) ) ){
		itm = _T("DateOfBirth");
	}
	else if ( !( chk = !Nul_Fld(dst->adr1, FLD_DAY) ) ){
		itm = _T("Address1");
	}
	else if ( !( chk = !Nul_Fld(dst->epd, FLD_DAY) ) ){
		itm = _T("EmploymentDate");
	}

	if ( chk ){
		if ( chk = ( idx = ((CComboBox *)GetDlgItem(IDC_COMBO_KIND))->GetCurSel() ) != -1 ){
			val = (int)(LONG_PTR)((CComboBox *)GetDlgItem(IDC_COMBO_KIND))->GetItemData(idx);
			DBS_INT_SET(dst->knd, &val);
		}
		else{
			itm = _T("EmployeeKind");
		}
	}

	if ( !chk ){
		swprintf_s( buf, memcnt(buf), _T("%s is %s."), itm ? itm: wrg, itm ? _T("requierd item"): _T("wrong") );
		MessageBox( buf, NULL, MB_ICONEXCLAMATION|MB_OK );
	}

	if ( chk ){
		epi = !Nul_Fld(dst->id, FLD_INT) ? DBS_INT_VAL(dst->id): 0;
		if ( chk = ( rsl = Epl_Cfm_Dup( epi, DBS_WST_PTR(dst->epc), &erm, m_pDbs_obj, m_pPms_mng ) ) != -1 ){
			if ( !( chk = !rsl ) ){
				MessageBox( _T("This EmployeeCode has been used by other employee. \n")
								_T("Please change to other code."), NULL, MB_ICONEXCLAMATION|MB_OK );
			}
		}
		else{
			if ( erm ){
				MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
				free( erm );
			}
		}
	}

	return( chk );
}

DWORD CEmployeeDlg::ConfirmSaveChaned( int msg, int fce )
{
	Csr			csr=NULL;
	EMP_LYE		*epl;
	int			ans=IDYES;
	WCHAR		*erm=NULL;

	while( csr=Nxt_Lnk( m_Epl_rot, csr ) ){
		epl = Rec_Of(EMP_LYE, csr);
		if ( !Nul_Fld(epl->mdt, FLD_DTM) || !Nul_Fld(epl->ddt, FLD_DTM) || Nul_Fld(epl->id, FLD_INT) ){
			break;
		}
	}
	if ( !!csr ){
		ans = Viw_Cfm_Svc( m_Epl_rot, Epl_Sav_Dat, msg, fce, this, m_pDbs_obj, m_pPms_mng );
	}
	return( ans );
}


BEGIN_MESSAGE_MAP(CEmployeeDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_CBN_SELCHANGE(IDC_COMBO_BRANCH, &CEmployeeDlg::OnCbnSelchangeComboBranch)
	ON_CBN_SELCHANGE(IDC_COMBO_FACILITY, &CEmployeeDlg::OnCbnSelchangeComboFacility)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CEmployeeDlg::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CEmployeeDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CEmployeeDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDCANCEL, &CEmployeeDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CEmployeeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_MALE, &CEmployeeDlg::OnBnClickedCheckMale)
	ON_BN_CLICKED(IDC_CHECK_FEMALE, &CEmployeeDlg::OnBnClickedCheckFemale)
	ON_BN_CLICKED(IDC_BUTTON_RESETPASSWORD, &CEmployeeDlg::OnBnClickedButtonResetpassword)
END_MESSAGE_MAP()


// CEmployeeDlg メッセージ ハンドラ
BOOL CEmployeeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pPms_mng = ((CMainFrame *)GetParent())->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)GetParent())->m_Dbs_obj;

	SetupComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// グリッドイベント
LRESULT CEmployeeDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;
	int				row;
	EMP_LYE			*epl;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:	row = ntf->row;
								if ( row > 0 ){
									epl = (EMP_LYE *)(LONG_PTR)m_Epl_grd->GetData( CLM_DAT, row );
									if ( epl ){
										SetEditData( epl );
									}
								}
								break;
		}
		free( ntf );
	}

	return( rsl );
}

// Branch combobox change
void CEmployeeDlg::OnCbnSelchangeComboBranch()
{
	int			idx, cnt=0;
	Csr			csr=NULL;
	WCHAR		*erm=NULL;
	BRC_INF		*brc;
	FCL_INF		*fcl;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);
	CComboBox	*pcx = (CComboBox *)GetDlgItem(IDC_COMBO_POST);

	if ( ( ( idx = bcx->GetCurSel() ) != -1 ) && idx!=m_Brc_idx ){
		if ( ConfirmSaveChaned( TRUE, FALSE )!=IDCANCEL ){
			Del_All( m_Fcl_rot );
			Del_All( m_Epl_rot );
			Del_All( m_Dty_rot );
			fcx->ResetContent();
			pcx->ResetContent();
			m_Epl_grd->ResetContent();
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
			SetEditData( NULL );
		}
		else{
			bcx->SetCurSel( idx = m_Brc_idx );
		}
	}
	m_Brc_idx = idx;
}

// Facility combobox change
void CEmployeeDlg::OnCbnSelchangeComboFacility()
{
	int			idx, bix, row=0;
	Csr			csr=NULL;
	BRC_INF		*brc;
	FCL_INF		*fcl;
	DTY_INF		*dty;
	EMP_LYE		*epl, key[1];
	WCHAR		*erm=NULL;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);
	CComboBox	*pcx = (CComboBox *)GetDlgItem(IDC_COMBO_POST);

	memset( key, 0, sizeof(key) );

	if ( ( ( idx = fcx->GetCurSel() ) != -1 ) && ( ( bix = bcx->GetCurSel() ) != -1 ) && idx!=m_Fcl_idx ){
//	if ( ( ( idx = fcx->GetCurSel() ) != -1 ) && idx!=m_Fcl_idx ){
		if ( ConfirmSaveChaned( TRUE, FALSE )!=IDCANCEL ){
			Del_All( m_Epl_rot );
			Del_All( m_Dty_rot );
			pcx->ResetContent();
			m_Epl_grd->ResetContent();
			fcl = (FCL_INF *)(LONG_PTR)fcx->GetItemData( idx );
			wcscpy_s( DBS_WST_PTR(key->opc), FLD_WST_SIZ(key->opc), m_pPms_mng->opc );
//			DBS_WST_CPY(key->brc, fcl->brc);
//			DBS_WST_CPY(key->fcc, fcl->fcc);
			if ( bix>0 ){
				brc = (BRC_INF *)(LONG_PTR)bcx->GetItemData( bix );
				DBS_WST_CPY(key->brc, brc->brc);
			}
			if ( idx>0 ){
				fcl = (FCL_INF *)(LONG_PTR)fcx->GetItemData( idx );
				DBS_WST_CPY(key->fcc, fcl->fcc);
			}
			if ( Epl_Get_Dty( m_Dty_rot, key, &erm, m_pDbs_obj, m_pPms_mng ) ){
				if ( Epl_Get_Dat( m_Epl_rot, key, &erm, m_pDbs_obj, m_pPms_mng ) ){
					while( csr = Nxt_Lnk( m_Dty_rot, csr ) ){
						dty = Rec_Of(DTY_INF, csr);
						pcx->SetItemData( pcx->InsertString( -1, DBS_WST_PTR(dty->nam) ), DBS_INT_VAL(dty->id) );
					}
					m_Epl_grd->AddRow( Cnt_Lnk( m_Epl_rot ) );
					while( csr = Nxt_Lnk( m_Epl_rot, csr ) ){
						epl = Rec_Of(EMP_LYE, csr);
						Set_Nul(epl->mdt, FLD_DTM, TRUE);
						DispData( epl, ++row );
					}
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

void CEmployeeDlg::OnBnClickedCheckMale()
{
	if ( ((CButton *)GetDlgItem(IDC_CHECK_MALE))->GetCheck() ){
		((CButton *)GetDlgItem(IDC_CHECK_FEMALE))->SetCheck(FALSE);
	}
}

void CEmployeeDlg::OnBnClickedCheckFemale()
{
	if ( ((CButton *)GetDlgItem(IDC_CHECK_FEMALE))->GetCheck() ){
		((CButton *)GetDlgItem(IDC_CHECK_MALE))->SetCheck(FALSE);
	}
}

// Updae button clich
void CEmployeeDlg::OnBnClickedButtonUpdate()
{
	int			row;
	EMP_LYE		tmp[1], *epl;
	Day			dte[1];

	if ( ( row = m_Epl_grd->GetSelRow() ) > 0 ){
		epl = (EMP_LYE *)(LONG_PTR)m_Epl_grd->GetData( CLM_DAT, row );
		*tmp = *epl;
		if ( GetData( tmp ) ){
			*epl = *tmp;
			Get_Dte( dte );
			DBS_DTM_SET(epl->mdt, dte);
			DispData( epl, row );
			SetEditData( NULL );
			m_Epl_grd->SetSelectCell(-1, -1);
		}
	}
}

// Add button click
void CEmployeeDlg::OnBnClickedButtonAdd()
{
	int			row, idx, bix, val;
	Csr			csr;
	EMP_LYE		tmp[1], *epl;
	BRC_INF		*brc;
	FCL_INF		*fcl;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);

	memset( tmp, 0, sizeof(tmp) );

	if ( ( idx = fcx->GetCurSel() ) != -1 && ( bix = bcx->GetCurSel() ) != -1 ){
//	if ( ( idx = fcx->GetCurSel() ) != -1 ){
		DBS_WST_SET(tmp->opc, m_pPms_mng->opc);
		if ( bix>0 ){
			brc = (BRC_INF *)(LONG_PTR)fcx->GetItemData( bix );
			DBS_WST_CPY(tmp->brc, brc->brc);
		}
		if ( idx>0 ){
			fcl = (FCL_INF *)(LONG_PTR)fcx->GetItemData( idx );
			DBS_WST_CPY(tmp->fcc, fcl->fcc);
		}
		if ( GetData( tmp ) ){
			if ( csr=Add_Lnk( m_Epl_rot, NULL, sizeof(EMP_LYE) ) ){
				epl = Rec_Of(EMP_LYE, csr);
				*epl = *tmp;
				//DBS_WST_SET(epl->opc, m_pPms_mng->opc);
				//DBS_WST_SET(epl->brc, DBS_WST_PTR(fcl->brc));
				//DBS_WST_SET(epl->fcc, DBS_WST_PTR(fcl->fcc));
				DBS_INT_SET(epl->uii, &(val=0) );
				DBS_INT_SET(epl->rli, &(val=0) );
				DBS_INT_SET(epl->lck, &(val=0) );
				DBS_WST_SET(epl->pwd, _T(""));
				row = m_Epl_grd->GetLineCount( FALSE );
				m_Epl_grd->AddRow( 1 );
				DispData( epl, row+1 );
				SetEditData( NULL );
				m_Epl_grd->SetSelectCell(-1, -1);
			}
		}
	}
}

// Delete button click
void CEmployeeDlg::OnBnClickedButtonDelete()
{
	int			row;
	EMP_LYE		*epl;
	Day			dte[1];

	if ( ( row = m_Epl_grd->GetSelRow() ) > 0 ){
		epl = (EMP_LYE *)(LONG_PTR)m_Epl_grd->GetData( CLM_DAT, row );
		if ( MessageBox( _T("Are you sure to delete?"), NULL, MB_ICONQUESTION|MB_YESNO ) == IDYES ){
			if ( Nul_Fld(epl->id, FLD_INT) ){
				Del_Lnk( m_Epl_rot, Csr_Of(epl) );
			}
			else{
				Get_Dte( dte );
				DBS_DAY_SET(epl->ddt, dte);
			}
			m_Epl_grd->DelRow( row );
			m_Epl_grd->SetSelectCell(-1, -1);
			m_Epl_grd->Redraw();
		}
	}
}

// ResetPassword button click
void CEmployeeDlg::OnBnClickedButtonResetpassword()
{
	WCHAR		pwd[40];

	Viw_Gen_Pwd( pwd, 10 );
	GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowTextW( pwd );
}


// Close button click
void CEmployeeDlg::OnBnClickedCancel()
{
	if ( ConfirmSaveChaned( TRUE, TRUE )==IDYES ){
		OnCancel();
	}
}

// Close winth save button click
void CEmployeeDlg::OnBnClickedOk()
{
	if ( ConfirmSaveChaned( FALSE, FALSE )==IDYES ){
		OnOK();
	}
}

BOOL CEmployeeDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}
