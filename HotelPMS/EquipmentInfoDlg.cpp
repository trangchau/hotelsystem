// EquipmentInfoDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "EquipmentInfoDlg.h"
#include "MainFrm.h"
#include ".\fnc\mfm_sql.h"
#include ".\fnc\eqp_sql.h"
#include ".\fnc\viw_cmn.h"

//IDC_COMBO_BRANCH
//IDC_COMBO_FACILITY
//IDC_COMBO_EQUIPMENTTYP
//IDC_EDIT_EQUIPMENTKIND
//IDC_EDIT_NAME
//IDC_EDIT_ABBREVIATION
//IDC_EDIT_EQUIPMENTCODE
//IDC_EDIT_REGULARCAPA
//IDC_SPIN_REGULARCAPA
//IDC_EDIT_EXTRACAPA
//IDC_SPIN_EXTRACAPA
//IDC_CHECK_SALEENABLE
//IDC_CHECK_SALEENABLE2
//IDC_COMBO_BUILDING
//IDC_COMBO_VIEW,284
//IDC_COMBO_DIRECTION
//IDC_COMBO_SIZE
//IDC_BUTTON_UPDATE
//IDC_BUTTON_ADD
//IDC_BUTTON_DELETE


static	WCHAR	*Grd_ttl[] =	{	{ _T("Code")		},
									{ _T("Name")		},
									{ _T("Abbreviation")},
									{ _T("Regular")		},
									{ _T("Extra")		} };

static	GRD_CLM	Grd_clm[] =		{	{ 128, GAL_LFT, GCT_NML,  12 },		// Code
									{ 298, GAL_LFT, GCT_NML,  12 },		// Name
									{ 186, GAL_LFT, GCT_NML,  12 },		// Abbreviation
									{  80, GAL_RGT, GCT_NML,  12 },		// Regular
									{  80, GAL_RGT, GCT_NML,  12 } };	// Extra

#define	IDC_GRD_INF		(8192)	// Copy destination selecting grid

#define	CLM_DAT			0		// Data keeping column

#define	CLM_COD			0		// Code
#define	CLM_NAM			1		// Name
#define	CLM_ABR			2		// Abbreviation
#define	CLM_RGL			3		// Regular
#define	CLM_EXT			4		// Extra

// CEquipmentInfoDlg ダイアログ

IMPLEMENT_DYNAMIC(CEquipmentInfoDlg, CDialog)

CEquipmentInfoDlg::CEquipmentInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEquipmentInfoDlg::IDD, pParent)
{
	m_Inf_grd = NULL;
	m_pPms_mng = NULL;
	m_pDbs_obj = NULL;
	m_Ope_cod = NULL;
	m_Eqp_knd = 0;
	m_Brc_idx = -1;
	m_Fcl_idx = -1;
	m_Eqt_idx = -1;
	memset( m_Brc_rot, 0, sizeof(m_Brc_rot) );
	memset( m_Fcl_rot, 0, sizeof(m_Fcl_rot) );
	memset( m_Typ_rot, 0, sizeof(m_Typ_rot) );
	memset( m_Inf_rot, 0, sizeof(m_Inf_rot) );
}

CEquipmentInfoDlg::~CEquipmentInfoDlg()
{
	if ( m_Inf_grd )	delete m_Inf_grd;
	Del_All( m_Brc_rot );
	Del_All( m_Fcl_rot );
	Del_All( m_Typ_rot );
	Del_All( m_Inf_rot );
}

void CEquipmentInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CEquipmentInfoDlg::SetupAdditionalCbx( DWORD cid, int knd )
{
	int			cnt, idx;
	EQP_ADT		*adt;
	CComboBox	*cbx = (CComboBox *)GetDlgItem(cid);

	if ( !!cbx && ( cnt = Viw_Get_Eqa( &adt, knd ) ) != -1 ){
		for ( idx=0; idx<cnt; idx++, adt++ ){
			cbx->SetItemData( cbx->InsertString( -1, adt->nam ), (DWORD)adt->cod );
		}
	}
}

void CEquipmentInfoDlg::SeekAdditionalCbx( DWORD cid, int add )
{
	int			cnt, idx;
	CComboBox	*cbx = (CComboBox *)GetDlgItem(cid);

	cbx->SetCurSel(-1);
	if ( !!cbx && ( cnt = cbx->GetCount() ) != -1 ){
		for ( idx=0; idx<cnt; idx++ ){
			if ( cbx->GetItemData( idx ) & add ){
				cbx->SetCurSel( idx );
				break;
			}
		}
	}
}

int CEquipmentInfoDlg::GetAdditionalCbx( DWORD cid )
{
	int			val=0, idx;
	CComboBox	*cbx = (CComboBox *)GetDlgItem(cid);

	idx = cbx->GetCurSel();
	if ( idx != -1 ){
		val = (int)(LONG_PTR)cbx->GetItemData( idx );
	}
	return( val );
}

void CEquipmentInfoDlg::SetupComponent()
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
		{ NULL,      255, IDC_EDIT_EQUIPMENTKIND	},
		{ NULL,      255, IDC_EDIT_NAME				},
		{ NULL,      255, IDC_EDIT_ABBREVIATION		},
		{ NULL,      255, IDC_EDIT_EQUIPMENTCODE	},
		{ NULL,        3, IDC_EDIT_REGULARCAPA		},
		{ NULL,        3, IDC_EDIT_EXTRACAPA		}
	};

	for ( idx=0; idx<memcnt(Cpn_); idx++ ){
		if ( !!Cpn_[idx].wnd ){
			Cpn_[idx].wnd->SubclassDlgItem(Cpn_[idx].cid, this);
		}
		if ( Cpn_[idx].lmt!=-1 )	((CEdit *)GetDlgItem(Cpn_[idx].cid))->SetLimitText( Cpn_[idx].lmt );
	}
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_REGULARCAPA))->SetRange(1, 999);
	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_EXTRACAPA))->SetRange(0, 999);

	GetDlgItem(IDC_EDIT_EQUIPMENTKIND)->EnableWindow( FALSE );

	memset( gdf, 0, sizeof(gdf) );

	GetDlgItem(IDC_COMBO_BRANCH)->GetWindowPlacement( wpc );
	*rct = wpc->rcNormalPosition;
	//rct->left = 10;
	rct->top += 30;
	rct->right = 0;
	rct->bottom = 0;

	gdf->lnh = 20;
	gdf->vlc = 10;
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

	m_Inf_grd = new CSimpleGrid();
	m_Inf_grd->Create( gdf, *rct, IDC_GRD_INF, this );
	for ( idx=0; idx<memcnt(Grd_ttl); idx++ ){
		m_Inf_grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}
	SetupAdditionalCbx(IDC_COMBO_WING,			ADK_WNG );
	SetupAdditionalCbx(IDC_COMBO_VIEW,			ADK_VIW );
	SetupAdditionalCbx(IDC_COMBO_DIRECTION,		ADK_FCE );
	SetupAdditionalCbx(IDC_COMBO_SIZE,			ADK_SIZ );
}

int	 CEquipmentInfoDlg::InitData( WCHAR *opc )
{
	int			chk;
	Csr			csr=NULL;
	WCHAR		*erm=NULL;
	BRC_INF		*brc;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);

	m_Ope_cod = opc;

	m_Brc_idx = -1;
	m_Fcl_idx = -1;
	m_Eqt_idx = -1;

	Del_All( m_Brc_rot );
	Del_All( m_Fcl_rot );
	Del_All( m_Typ_rot );
	Del_All( m_Inf_rot );
	bcx->ResetContent();
	((CComboBox *)GetDlgItem(IDC_COMBO_FACILITY))->ResetContent();
	((CComboBox *)GetDlgItem(IDC_COMBO_EQUIPMENTTYPE))->ResetContent();
	((CComboBox *)GetDlgItem(IDC_COMBO_BUILDING))->ResetContent();
	m_Inf_grd->ResetContent();

	SetEditData( NULL );

	if ( chk = Get_Mst( m_Brc_rot, GTM_BRC_INF, NULL, NULL, &erm, m_pDbs_obj, m_pPms_mng ) ){
		while( csr = Nxt_Lnk( m_Brc_rot, csr ) ){
			brc = Rec_Of(BRC_INF, csr);
			bcx->SetItemData( bcx->InsertString(-1, EDT_DBF_WST(brc->nam, _T("")) ), (DWORD)(LONG_PTR)brc );
		}
	}
	if ( Cnt_Lnk( m_Brc_rot ) == 1 ){
		bcx->SetCurSel(0);
		OnCbnSelchangeComboBranch();
	}

	return( chk );
}

void	CEquipmentInfoDlg::DispData( EQP_INF *eqi, int row )
{
	WCHAR		buf[20];

	m_Inf_grd->SetCellText( CLM_COD, row, EDT_DBF_WST(eqi->eco, _T("")) );
	m_Inf_grd->SetCellText( CLM_NAM, row, EDT_DBF_WST(eqi->nam, _T("")) );
	m_Inf_grd->SetCellText( CLM_ABR, row, EDT_DBF_WST(eqi->abr, _T("")) );
	m_Inf_grd->SetCellText( CLM_RGL, row, EDT_DBF_INT(buf, memcnt(buf), eqi->rcp, Edt_Num_Nml) );
	m_Inf_grd->SetCellText( CLM_EXT, row, EDT_DBF_INT(buf, memcnt(buf), eqi->ecp, Edt_Num_Nml) );
	m_Inf_grd->SetData( CLM_DAT, row, (int)(LONG_PTR)eqi );
}


void	CEquipmentInfoDlg::SetEditData( EQP_INF *eqi )
{
	WCHAR		buf[20];

	GetDlgItem(IDC_EDIT_NAME)->SetWindowTextW( !!eqi ? EDT_DBF_WST(eqi->nam, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_ABBREVIATION)->SetWindowTextW( !!eqi ? EDT_DBF_WST(eqi->abr, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_EQUIPMENTCODE)->SetWindowTextW( !!eqi ? EDT_DBF_WST(eqi->eco, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_REGULARCAPA)->SetWindowTextW( !!eqi ? EDT_DBF_INT(buf, memcnt(buf), eqi->rcp, Edt_Num_Nml):_T(""));
	GetDlgItem(IDC_EDIT_EXTRACAPA)->SetWindowTextW( !!eqi ? EDT_DBF_INT(buf, memcnt(buf), eqi->ecp, Edt_Num_Nml):_T(""));
	((CButton *)GetDlgItem(IDC_CHECK_SALEENABLE))->SetCheck( !!eqi && !!DBS_INT_VAL(eqi->sle) );
	((CButton *)GetDlgItem(IDC_CHECK_CANSMOKE))->SetCheck( !!eqi && !!(DBS_INT_VAL(eqi->adi2) & EQA_SMOKING) );

	SeekAdditionalCbx( IDC_COMBO_WING, !!eqi ? DBS_INT_VAL(eqi->adi1): 0 );
	SeekAdditionalCbx( IDC_COMBO_VIEW, !!eqi ? DBS_INT_VAL(eqi->adi1): 0 );
	SeekAdditionalCbx( IDC_COMBO_DIRECTION, !!eqi ? DBS_INT_VAL(eqi->adi1): 0 );
	SeekAdditionalCbx( IDC_COMBO_SIZE, !!eqi ? DBS_INT_VAL(eqi->adi1): 0 );
	ButtonEnable();
}

DWORD CEquipmentInfoDlg::ConfirmSaveChaned( int msg, int fce )
{
	Csr			csr=NULL;
	EQP_INF		*eqi;
	int			ans=IDYES;
	WCHAR		*erm=NULL;

	while( csr=Nxt_Lnk( m_Inf_rot, csr ) ){
		eqi = Rec_Of(EQP_INF, csr);
		if ( !Nul_Fld(eqi->mdt, FLD_DTM) || !Nul_Fld(eqi->ddt, FLD_DTM) || Nul_Fld(eqi->id, FLD_INT) ){
			break;
		}
	}
	if ( !!csr ){
		ans = Viw_Cfm_Svc( m_Inf_rot, Eqp_Sav_Inf, msg, fce, this, m_pDbs_obj, m_pPms_mng );
		//if ( fce ){
		//	ans = MessageBox( 
		//		_T("Are you sure you want to discard your changes?"), NULL, MB_ICONQUESTION|MB_YESNO );
		//}
		//else{
		//	if ( !msg || ( ans = MessageBox( 
		//		_T("Do you save the changes?"), NULL, MB_ICONQUESTION|MB_YESNOCANCEL ) )==IDYES ){
		//		if ( !Eqp_Sav_Inf( m_Inf_rot, &erm, m_pDbs_obj, m_pPms_mng ) ){
		//			if ( erm ){
		//				MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
		//				free( erm );
		//				ans = IDCANCEL;
		//			}
		//		}
		//		else{
		//		}
		//	}
		//}
	}
	return( ans );
}

int CEquipmentInfoDlg::GetData( EQP_INF *dst )
{
	int			idx, val, chk;
	WCHAR		buf[256], *itm=NULL;
	EQP_TYP		*eqt;
	CComboBox	*cbx;

	WIN_TXT_DBS(dst->nam, IDC_EDIT_NAME, this);
	WIN_TXT_DBS(dst->abr, IDC_EDIT_ABBREVIATION, this);
	WIN_TXT_DBS(dst->eco, IDC_EDIT_EQUIPMENTCODE, this);
	WIN_INT_DBS(dst->rcp, IDC_EDIT_REGULARCAPA, this );
	WIN_INT_DBS(dst->ecp, IDC_EDIT_EXTRACAPA, this );
	WIN_CBX_DBS(dst->sle, IDC_CHECK_SALEENABLE, this );
	val = 0;
	val = ((CButton *)GetDlgItem(IDC_CHECK_SALEENABLE))->GetCheck() ? EQA_SMOKING: EQA_NOSMOKING;
	DBS_INT_SET(dst->adi2, &val);
	val  = GetAdditionalCbx( IDC_COMBO_WING );
	val |= GetAdditionalCbx( IDC_COMBO_VIEW );
	val |= GetAdditionalCbx( IDC_COMBO_DIRECTION );
	val |= GetAdditionalCbx( IDC_COMBO_SIZE );
	DBS_INT_SET(dst->adi1, &val);

	cbx = (CComboBox *)GetDlgItem(IDC_COMBO_EQUIPMENTTYPE);
	if ( ( idx = cbx->GetCurSel() ) != -1 ){
		eqt = (EQP_TYP *)(LONG_PTR)cbx->GetItemData( idx );
		DBS_INT_SET(dst->eti, DBS_INT_PTR(eqt->id));
	}
	DBS_INT_SET(dst->ekd, &m_Eqp_knd);
	cbx = (CComboBox *)GetDlgItem(IDC_COMBO_BUILDING);
	if ( ( idx = cbx->GetCurSel() ) != -1 ){
		val = (int)(LONG_PTR)cbx->GetItemData( idx );
		DBS_INT_SET(dst->fbi, &val);
	}
	else{
		Set_Nul(dst->fbi, FLD_INT, TRUE);
	}
	if ( !( chk = !Nul_Fld(dst->eco, FLD_WST) ) ){
		itm = _T("EquipmentCode");
	}
	else if ( !( chk = !Nul_Fld(dst->ecp, FLD_INT) ) ){
		itm = _T("RegularCapacity");
	}
	else if ( !( chk = !Nul_Fld(dst->ecp, FLD_INT) ) ){
		itm = _T("ExtraCapacity");
	}
	if ( !chk ){
		swprintf_s( buf, memcnt(buf), _T("%s is requierd item."), itm );
		MessageBox( buf, NULL, MB_ICONEXCLAMATION|MB_OK );
	}
	return( chk );
}

void	CEquipmentInfoDlg::SetAdditionalEnable( int knd )
{
	int			idx;

	struct	{
		DWORD	cid;
		int		knd;
	}Tbl_[] = {
		{ IDC_COMBO_BUILDING,	EQK_ROOM	},
		{ IDC_COMBO_VIEW,		EQK_ROOM	},
		{ IDC_COMBO_DIRECTION,	EQK_ROOM	},
		{ IDC_COMBO_SIZE,		EQK_LOCKER	},
	};

	for ( idx=0; idx<memcnt(Tbl_); idx++ ){
		GetDlgItem((Tbl_+idx)->cid)->EnableWindow( (Tbl_+idx)->knd == knd );
		if ( (Tbl_+idx)->knd == knd ){
			((CComboBox *)GetDlgItem((Tbl_+idx)->cid))->SetCurSel(-1);
		}
	}
}

void	CEquipmentInfoDlg::ButtonEnable()	
{
	int			enb, sel;
	enb  = ((CComboBox *)GetDlgItem(IDC_COMBO_BRANCH))->GetCurSel() != -1;
	enb &= ((CComboBox *)GetDlgItem(IDC_COMBO_FACILITY))->GetCurSel() != -1;
	enb &= ((CComboBox *)GetDlgItem(IDC_COMBO_EQUIPMENTTYPE))->GetCurSel() != -1;

	sel = m_Inf_grd->GetSelRow() > 0;

	GetDlgItem(IDC_EDIT_NAME)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ABBREVIATION)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_EQUIPMENTCODE)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_REGULARCAPA)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_EXTRACAPA)->EnableWindow( enb );
	GetDlgItem(IDC_CHECK_SALEENABLE)->EnableWindow( enb );
	GetDlgItem(IDC_CHECK_CANSMOKE)->EnableWindow( enb );
	GetDlgItem(IDC_COMBO_WING)->EnableWindow( enb );
	GetDlgItem(IDC_COMBO_VIEW)->EnableWindow( enb );
	GetDlgItem(IDC_COMBO_DIRECTION)->EnableWindow( enb );
	GetDlgItem(IDC_COMBO_SIZE)->EnableWindow( enb );
	GetDlgItem(IDC_COMBO_BUILDING)->EnableWindow( enb );

	GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow( sel && enb );
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow( enb);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow( sel && enb );
}

BEGIN_MESSAGE_MAP(CEquipmentInfoDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_CBN_SELCHANGE(IDC_COMBO_BRANCH, &CEquipmentInfoDlg::OnCbnSelchangeComboBranch)
	ON_CBN_SELCHANGE(IDC_COMBO_FACILITY, &CEquipmentInfoDlg::OnCbnSelchangeComboFacility)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CEquipmentInfoDlg::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CEquipmentInfoDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CEquipmentInfoDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDCANCEL, &CEquipmentInfoDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CEquipmentInfoDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_EQUIPMENTTYPE, &CEquipmentInfoDlg::OnCbnSelchangeComboEquipmenttype)
END_MESSAGE_MAP()


// CEquipmentInfoDlg メッセージ ハンドラ

BOOL CEquipmentInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	m_pPms_mng = ((CMainFrame *)GetParent())->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)GetParent())->m_Dbs_obj;


	SetupComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// Grid notify
LRESULT CEquipmentInfoDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;
	EQP_INF			*eqi;
	int				row;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:	row = ntf->row;
								if ( row > 0 ){
									eqi = (EQP_INF *)(LONG_PTR)m_Inf_grd->GetData( CLM_DAT, row );
									if ( eqi ){
										SetEditData( eqi );
									}
								}
								break;
		}
		free( ntf );
	}

	return( rsl );
}

// Branch combobox change
void CEquipmentInfoDlg::OnCbnSelchangeComboBranch()
{
	int			idx, cnt=0;
	Csr			csr=NULL;
	WCHAR		*erm=NULL;
	BRC_INF		*brc;
	FCL_INF		*fcl;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);
	CComboBox	*tcx = (CComboBox *)GetDlgItem(IDC_COMBO_EQUIPMENTTYPE);

	if ( ( idx = bcx->GetCurSel() ) != -1 && m_Brc_idx!=idx ){
		if ( ConfirmSaveChaned( TRUE, FALSE )!=IDCANCEL ){
			Del_All( m_Fcl_rot );
			Del_All( m_Typ_rot );
			Del_All( m_Inf_rot );
			fcx->ResetContent();
			tcx->ResetContent();
			m_Fcl_idx = -1;
			m_Eqt_idx = -1;
			m_Inf_grd->ResetContent();
			brc = (BRC_INF *)(LONG_PTR)bcx->GetItemData( idx );
			if ( Get_Mst( m_Fcl_rot, GTM_FCL_INF, DBS_WST_PTR(brc->brc), NULL, &erm, m_pDbs_obj, m_pPms_mng ) ){
				while( csr = Nxt_Lnk( m_Fcl_rot, csr ) ){
					fcl = Rec_Of(FCL_INF, csr);
					if ( !!DBS_INT_VAL(fcl->hve) ){
						fcx->SetItemData( fcx->InsertString(-1, EDT_DBF_WST(fcl->nam, _T("")) ), (DWORD)(LONG_PTR)fcl );
						++cnt;
					}
				}
				if ( cnt==1 ){
					fcx->SetCurSel(0);
				}
			}
			OnCbnSelchangeComboFacility();
		}
		else{
			bcx->SetCurSel( idx = m_Brc_idx );
		}
	}
	m_Brc_idx = idx;
	ButtonEnable();
}

// Facility combobox change
void CEquipmentInfoDlg::OnCbnSelchangeComboFacility()
{
	int			idx, bix, cnt=0, bri, fci;
	Lnk			rot[1], *csr=NULL;
	BRC_INF		*brc;
	BLD_INF		*bld;
	FCL_INF		*fcl;
	EQP_TYP		*eqt;
	WCHAR		*erm=NULL, *nam;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);
	CComboBox	*tcx = (CComboBox *)GetDlgItem(IDC_COMBO_EQUIPMENTTYPE);
	CComboBox	*blc = (CComboBox *)GetDlgItem(IDC_COMBO_BUILDING);

	if ( ( bix = bcx->GetCurSel() ) != -1 && ( idx = fcx->GetCurSel() ) != -1 && idx!=m_Fcl_idx ){
		if ( ConfirmSaveChaned( TRUE, FALSE )!=IDCANCEL ){
			Del_All( m_Typ_rot );
			Del_All( m_Inf_rot );
			m_Eqt_idx = -1;
			tcx->ResetContent();
			blc->ResetContent();
			m_Inf_grd->ResetContent();
			brc = (BRC_INF *)(LONG_PTR)bcx->GetItemData( bix );
			fcl = (FCL_INF *)(LONG_PTR)fcx->GetItemData( idx );

			m_Eqp_knd = Viw_Get_Eqk( &nam, DBS_INT_VAL(fcl->fck) );
			GetDlgItem(IDC_EDIT_EQUIPMENTKIND)->SetWindowText( nam );
			SetAdditionalEnable( m_Eqp_knd );

			bri = DBS_INT_VAL(brc->id);
			fci = DBS_INT_VAL(fcl->id);

			memset( rot, 0, sizeof(rot) );

			if ( Get_Mst( rot, GTM_BLD_INF, (WCHAR *)(LONG_PTR)bri, (WCHAR *)(LONG_PTR)fci, &erm, m_pDbs_obj, m_pPms_mng ) ){
				while ( csr = Nxt_Lnk( rot, csr ) ){
					bld = Rec_Of(BLD_INF, csr);
					blc->SetItemData( blc->InsertString(-1, EDT_DBF_WST(bld->nam, _T("")) ), (DWORD)(LONG_PTR)DBS_INT_VAL(bld->id) );
				}
				if ( Get_Mst( m_Typ_rot, GTM_EQP_TYP, DBS_WST_PTR(fcl->brc), DBS_WST_PTR(fcl->fcc), &erm, m_pDbs_obj, m_pPms_mng ) ){
					while( csr = Nxt_Lnk( m_Typ_rot, csr ) ){
						eqt = Rec_Of(EQP_TYP, csr);
						tcx->SetItemData( tcx->InsertString(-1, EDT_DBF_WST(eqt->nam, _T("")) ), (DWORD)(LONG_PTR)eqt );
						++cnt;
					}
					if ( cnt==1 ){
						tcx->SetCurSel(0);
					}
				}
			}
			OnCbnSelchangeComboEquipmenttype();

			Del_All( rot );
		}
		else{
			fcx->SetCurSel( idx = m_Fcl_idx );
		}
	}
	m_Fcl_idx = idx;
	ButtonEnable();
}

// Equipment type combobox change
void CEquipmentInfoDlg::OnCbnSelchangeComboEquipmenttype()
{
	int			fix, tix, row=0;
	Csr			csr=NULL;
	FCL_INF		*fcl;
	EQP_TYP		*eqt;
	EQP_INF		*eqi, key[1];
	WCHAR		*erm=NULL;
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);
	CComboBox	*tcx = (CComboBox *)GetDlgItem(IDC_COMBO_EQUIPMENTTYPE);

	memset( key, 0, sizeof(key) );

	if ( ( fix = fcx->GetCurSel() ) != -1 && ( tix = tcx->GetCurSel() ) != -1 && tix!=m_Eqt_idx ){
		if ( ConfirmSaveChaned( TRUE, FALSE )!=IDCANCEL ){
			Del_All( m_Inf_rot );
			m_Inf_grd->ResetContent();
			fcl = (FCL_INF *)(LONG_PTR)fcx->GetItemData( fix );
			eqt = (EQP_TYP *)(LONG_PTR)tcx->GetItemData( tix );
			wcscpy_s( DBS_WST_PTR(key->opc), FLD_WST_SIZ(key->opc), m_pPms_mng->opc );
			DBS_WST_CPY(key->brc, fcl->brc);
			DBS_WST_CPY(key->fcc, fcl->fcc);
			DBS_WST_CPY(key->fcc, fcl->fcc);
			DBS_INT_SET(key->ekd, eqt->ekd); 
			DBS_INT_CPY(key->eti, eqt->id); 
			if ( Eqp_Get_Inf( m_Inf_rot, key, &erm, m_pDbs_obj, m_pPms_mng ) ){
				m_Inf_grd->AddRow( Cnt_Lnk(m_Inf_rot) );
				while( csr = Nxt_Lnk( m_Inf_rot, csr ) ){
					eqi = Rec_Of(EQP_INF, csr);
					DispData( eqi, ++row );
					Set_Nul(eqi->mdt, FLD_DTM, TRUE);
				}
			}
		}
		else{
			tcx->SetCurSel( tix = m_Eqt_idx );
		}
	}
	m_Eqt_idx = tix;
	ButtonEnable();
}

// Updae button click
void CEquipmentInfoDlg::OnBnClickedButtonUpdate()
{
	int			row;
	EQP_INF		*eqi, tmp[1];
	Day			dte[1];

	memset( tmp, 0, sizeof(tmp) );

	if ( ( row = m_Inf_grd->GetSelRow() ) > 0 ){
		eqi = (EQP_INF *)(LONG_PTR)m_Inf_grd->GetData( CLM_DAT, row );
		*tmp = *eqi;
		if ( GetData( tmp ) ){
			*eqi = *tmp;
			Get_Dte( dte );
			DBS_DTM_SET(eqi->mdt, dte);
			DispData( eqi, row );
			SetEditData( NULL );
			m_Inf_grd->SetSelectCell( -1, -1 );
		}
	}
}

// Add button click
void CEquipmentInfoDlg::OnBnClickedButtonAdd()
{
	int			row, idx;
	Csr			csr;
	EQP_INF		*eqi, tmp[1];
	FCL_INF		*fcl;
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);

	memset( tmp, 0, sizeof(tmp) );

	if ( ( idx = fcx->GetCurSel() ) != -1 ){
		fcl = (FCL_INF *)(LONG_PTR)fcx->GetItemData( idx );
		if ( GetData( tmp ) ){
			DBS_WST_SET(tmp->opc, m_pPms_mng->opc );
			DBS_WST_CPY(tmp->brc, fcl->brc);
			DBS_WST_CPY(tmp->fcc, fcl->fcc);
			if ( csr = Add_Lnk( m_Inf_rot, tmp, sizeof(tmp) ) ){
				eqi = Rec_Of(EQP_INF, csr);
				row = m_Inf_grd->GetLineCount( FALSE );
				m_Inf_grd->AddRow( 1 );
				DispData( eqi, ++row );
				SetEditData( NULL );
				m_Inf_grd->SetSelectCell( -1, -1 );
			}
		}
	}
}

// Delete button click
void CEquipmentInfoDlg::OnBnClickedButtonDelete()
{
	int			row;
	EQP_INF		*eqi;
	Day			dte[1];

	if ( ( row = m_Inf_grd->GetSelRow() ) > 0 ){
		eqi = (EQP_INF *)(LONG_PTR)m_Inf_grd->GetData( CLM_DAT, row );
		if ( MessageBox( _T("Are you sure to delete?"), NULL, MB_ICONQUESTION|MB_YESNO ) == IDYES ){
			if ( Nul_Fld(eqi->id, FLD_INT) ){
				Del_Lnk( m_Inf_rot, Csr_Of(eqi) );
			}
			else{
				Get_Dte( dte );
				DBS_DAY_SET(eqi->ddt, dte);
			}
			m_Inf_grd->DelRow( row );
			m_Inf_grd->SetSelectCell(-1, -1);
			m_Inf_grd->Redraw();
		}
	}
}

// Close button click
void CEquipmentInfoDlg::OnBnClickedCancel()
{
	if ( ConfirmSaveChaned( TRUE, TRUE )==IDYES ){
		OnCancel();
	}
}

// Close winth save button click
void CEquipmentInfoDlg::OnBnClickedOk()
{
	if ( ConfirmSaveChaned( FALSE, FALSE )==IDYES ){
		OnOK();
	}
}

BOOL CEquipmentInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}
