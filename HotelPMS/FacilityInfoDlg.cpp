// FacilityInfoDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MainFrm.h"
#include "FacilityInfoDlg.h"

#include ".\fnc\fci_sql.h"
#include ".\fnc\mfm_sql.h"
#include ".\fnc\sql_cmn.h"
#include ".\fnc\viw_cmn.h"

static	WCHAR	*Grd_ttl[] =	{	{ _T("Code")		},
									{ _T("Name")		},
									{ _T("Abbreviation")},
									{ _T("HaveE")		},
									{ _T("Direct")		},
									{ _T("DCC")			} };

static	GRD_CLM	Grd_clm[] =		{	{  96, GAL_LFT, GCT_NML,  12 },		// Code
									{ 256, GAL_LFT, GCT_NML,  12 },		// Name
									{ 128, GAL_LFT, GCT_NML,  12 },		// Abbreviation
									{  64, GAL_CTR, GCT_CHK,  12 },		// HaveEquipment
									{  64, GAL_CTR, GCT_CHK,  12 },		// DirectManagementBranch
									{  64, GAL_CTR, GCT_CHK,  12 } };	// DayCloseCharge

#define	IDC_GRD_FCL		(8192)	// Copy destination selecting grid

#define	CLM_DAT			0		// Data keeping column

#define	CLM_COD			0		// Code
#define	CLM_NAM			1		// Name
#define	CLM_ABR			2		// Abbreviation
#define	CLM_HVE			3		// HaveEquipment
#define	CLM_DMB			4		// DirectManagementBranch
#define	CLM_DCC			5		// DayCloseCharge

//IDC_EDIT_EXEMPTIONECI
//IDC_EDIT_NORMALTAIMECHECKIN
//IDC_EDIT_NORMALTAIMECHECKOUT
//IDC_EDIT_EXEMPTIONLCO
//IDC_STATIC_NORMALTIMECHECKIN
//IDC_STATIC_NORMALTIMECHECKOUT
//IDC_STATIC_EXEMPTIONECI
//IDC_STATIC_EXEMPTIONLCO
//IDC_CHECK_DAYCLOSECHARGE
//IDC_CHECK_HAVEEQUIPMENT
//IDC_CHECK_SENDCOMFIRMATIONMAIL
//IDC_CHECK_APPLYEXTENDCHARGE
//IDC_CHECK_FORCEDEPOSIOCOLLECT
//IDC_CHECK_NEEDCONVENTANT

// CFacilityInfoDlg ダイアログ

IMPLEMENT_DYNAMIC(CFacilityInfoDlg, CDialog)

CFacilityInfoDlg::CFacilityInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFacilityInfoDlg::IDD, pParent)
{
	m_pPms_mng = NULL;
	m_pDbs_obj = NULL;
	m_Ope_cod = NULL;
	m_Fcl_grd = NULL;
	m_Brc_idx = -1;
	memset( m_Brc_rot, 0, sizeof(m_Brc_rot) );
	memset( m_Fcl_rot, 0, sizeof(m_Fcl_rot) );
}

CFacilityInfoDlg::~CFacilityInfoDlg()
{
	Del_All( m_Brc_rot );
	Del_All( m_Fcl_rot );
	if ( m_Fcl_grd )	delete	m_Fcl_grd;
}

void CFacilityInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFacilityInfoDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CFacilityInfoDlg::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CFacilityInfoDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CFacilityInfoDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDCANCEL, &CFacilityInfoDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CFacilityInfoDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_BRANCH, &CFacilityInfoDlg::OnCbnSelchangeComboBranch)
END_MESSAGE_MAP()

int	 CFacilityInfoDlg::InitData( WCHAR *opc )
{
	WCHAR		*erm=NULL;
	int			chk, idx=0;
	Csr			csr=NULL;
	BRC_INF		*brc;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);

	m_Ope_cod = opc;

	m_Brc_idx = -1;
	Del_All( m_Brc_rot );
	Del_All( m_Fcl_rot );
	bcx->ResetContent();
	m_Fcl_grd->ResetContent();

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

void CFacilityInfoDlg::SetupComponent()
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
		{ NULL,      255, IDC_EDIT_OPERATORNAME				},
		{ NULL,      128, IDC_EDIT_ABBREVIATION				},
		{ NULL,      128, IDC_EDIT_FACILITYCODE				},
		{ NULL,      255, IDC_EDIT_ADDRESS1					},
		{ NULL,      255, IDC_EDIT_ADDRESS2					},
		{ NULL,       30, IDC_EDIT_TELEPHONE				},
		{ NULL,       30, IDC_EDIT_FAXNUMBER				},
		{ NULL,      255, IDC_EDIT_WEBURL					},
		{ NULL,      128, IDC_EDIT_EMAIL					},
		{ m_Nml_cin,  -1, IDC_EDIT_NORMALTAIMECHECKIN		},
		{ m_Nml_cot,  -1, IDC_EDIT_NORMALTAIMECHECKOUT		},
		{ NULL,        3, IDC_EDIT_EXEMPTIONECI				},
		{ NULL,        3, IDC_EDIT_EXEMPTIONLCO				},
//		{ NULL,       -1, IDC_STATIC_PANEL					},
		{ NULL,       -1, IDC_STATIC_FACILITYNAME			},
		{ NULL,       -1, IDC_STATIC_ABBREVIATION			},
		{ NULL,       -1, IDC_STATIC_FACILITYCODE			},
		{ NULL,       -1, IDC_STATIC_ADDRESS1				},
		{ NULL,       -1, IDC_STATIC_ADDRESS2				},
		{ NULL,       -1, IDC_STATIC_TELEPHONE				},
		{ NULL,       -1, IDC_STATIC_FAX					},
		{ NULL,       -1, IDC_STATIC_WEBURL					},
		{ NULL,       -1, IDC_STATIC_EMAIL					},
		{ NULL,       -1, IDC_STATIC_NORMALTIMECHECKIN		},
		{ NULL,       -1, IDC_STATIC_NORMALTIMECHECKOUT		},
		{ NULL,       -1, IDC_STATIC_EXEMPTIONECI			},
		{ NULL,       -1, IDC_STATIC_EXEMPTIONLCO			},
		{ NULL,       -1, IDC_STATIC_FACILITYKIND			},
		{ NULL,       -1, IDC_CHECK_DIRECTMANAGE			},
		{ NULL,       -1, IDC_CHECK_DAYCLOSECHARGE			},
		{ NULL,       -1, IDC_CHECK_HAVEEQUIPMENT			},
		{ NULL,       -1, IDC_CHECK_SENDCOMFIRMATIONMAIL	},
		{ NULL,       -1, IDC_CHECK_APPLYEXTENDCHARGE		},
		{ NULL,       -1, IDC_CHECK_FORCEDEPOSIOCOLLECT		},
		{ NULL,       -1, IDC_CHECK_NEEDCONVENTANT			},
//		{ NULL,       -1, IDC_BUTTON_ADD					},
//		{ NULL,       -1, IDC_BUTTON_DELETE					},
		{ m_Fcl_knd,  -1, IDC_COMBO_FACILITYKIND			}
	};

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

	m_Fcl_grd = new CSimpleGrid();
	m_Fcl_grd->Create( gdf, *rct, IDC_GRD_FCL, this );
	for ( idx=0; idx<memcnt(Grd_ttl); idx++ ){
		m_Fcl_grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}
}

int		CFacilityInfoDlg::GetData( FCL_INF *fcl )
{
	int			chk=TRUE, val, idx, opt, err=0;
	BRC_INF		*brc;
	Csr			csr=NULL;
	WCHAR		buf[96], *itm=NULL, *wrg=NULL;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);

	if ( chk = ( idx = bcx->GetCurSel() ) != -1 ){
		brc = (BRC_INF *)(LONG_PTR)bcx->GetItemData( idx );
		WIN_TXT_DBS(fcl->nam, IDC_EDIT_OPERATORNAME, this);
		WIN_TXT_DBS(fcl->abr, IDC_EDIT_ABBREVIATION, this);
		WIN_TXT_DBS(fcl->fcc, IDC_EDIT_FACILITYCODE, this);
		WIN_TXT_DBS(fcl->adr1, IDC_EDIT_ADDRESS1, this);
		WIN_TXT_DBS(fcl->adr2, IDC_EDIT_ADDRESS2, this);
		WIN_TXT_DBS(fcl->tel, IDC_EDIT_TELEPHONE, this);
		WIN_TXT_DBS(fcl->fax, IDC_EDIT_FAXNUMBER, this);
		WIN_TXT_DBS(fcl->url, IDC_EDIT_WEBURL, this);
		WIN_TXT_DBS(fcl->eml, IDC_EDIT_EMAIL, this);
		WIN_CBX_DBS(fcl->cms, IDC_CHECK_DIRECTMANAGE, this);
		WIN_TIM_DBS(fcl->cit, &err, IDC_EDIT_NORMALTAIMECHECKIN, this);
		if ( chk && !( chk = !err ) )	wrg=_T("NoramTimeCheckin"); 
		WIN_TIM_DBS(fcl->cot, &err, IDC_EDIT_NORMALTAIMECHECKOUT, this);
		if ( chk && !( chk = !err ) )	wrg=_T("NoramTimeCheckin"); 
		WIN_TXT_DBS(fcl->eci, IDC_EDIT_EXEMPTIONECI, this);
		WIN_TXT_DBS(fcl->lco, IDC_EDIT_EXEMPTIONLCO, this);
		WIN_CBX_DBS(fcl->dcc, IDC_CHECK_DAYCLOSECHARGE, this);
		WIN_CBX_DBS(fcl->hve, IDC_CHECK_HAVEEQUIPMENT, this);

		opt = 0;
		if ( CBX_GET_CHK(IDC_CHECK_APPLYEXTENDCHARGE,this) )	opt |= APPLY_EXTEND_CHARGE;
		if ( CBX_GET_CHK(IDC_CHECK_FORCEDEPOSIOCOLLECT,this) )	opt |= FORCE_DEPOSIT_COLLECT;
		if ( CBX_GET_CHK(IDC_CHECK_NEEDCONVENTANT,this) )		opt |= NEED_CONVENTANT;
		if ( CBX_GET_CHK(IDC_CHECK_SENDCOMFIRMATIONMAIL,this) )	opt |= SEND_CONFIRMATION_EMAIL;
		DBS_INT_SET(fcl->opt, &opt);


		if ( chk ){
			if ( chk = ( idx = m_Fcl_knd->GetKind() ) != -1 ){
				DBS_INT_SET(fcl->fck, &idx);
			}
			else{
				itm=_T("FacilityKind");
			}
		}

		if ( Nul_Fld(fcl->id, FLD_INT) ){
			DBS_WST_SET(fcl->opc, m_Ope_cod);
			DBS_WST_SET(fcl->brc, DBS_WST_PTR(brc->brc));
			DBS_INT_SET(fcl->adi, &(val=0));
		}

		if ( chk && !( chk = !Nul_Fld(fcl->nam, FLD_WST) ) )	itm=_T("BranchName");	
		if ( chk && !( chk = !Nul_Fld(fcl->brc, FLD_WST) ) )	itm=_T("BranchCode");
		if ( !chk ){
			if ( itm ){
				swprintf_s( buf, memcnt(buf), _T("%s is required item"), itm );
			}
			else{
				swprintf_s( buf, memcnt(buf), _T("%s is wrong"), wrg );
			}
			MessageBox( buf, NULL, MB_ICONEXCLAMATION|MB_OK );
		}
	}

	return( chk );
}

void	CFacilityInfoDlg::DispData( FCL_INF *fcl, int row )
{
	m_Fcl_grd->SetCellText( CLM_COD, row, EDT_DBF_WST(fcl->fcc, _T("")) );
	m_Fcl_grd->SetCellText( CLM_NAM, row, EDT_DBF_WST(fcl->nam, _T("")) );
	m_Fcl_grd->SetCellText( CLM_ABR, row, EDT_DBF_WST(fcl->abr, _T("")) );
	m_Fcl_grd->SetCheck( CLM_HVE, row, !!DBS_INT_VAL(fcl->hve ) );
	m_Fcl_grd->SetCheck( CLM_DMB, row, !!DBS_INT_VAL(fcl->cms ) );
	m_Fcl_grd->SetCheck( CLM_DCC, row, !!DBS_INT_VAL(fcl->dcc ) );

	m_Fcl_grd->SetData( CLM_DAT, row, (int)(LONG_PTR)fcl );
}

void	CFacilityInfoDlg::SetEditData( FCL_INF *fcl )
{
	WCHAR		buf[96];
	int			opt, enb, fnd=FALSE;
	Csr			csr=NULL;
	FCL_INF		*otr;

	GetDlgItem(IDC_EDIT_OPERATORNAME)->SetWindowText( !!fcl ? EDT_DBF_WST(fcl->nam,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_ABBREVIATION)->SetWindowText( !!fcl ? EDT_DBF_WST(fcl->abr,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_FACILITYCODE)->SetWindowText( !!fcl ? EDT_DBF_WST(fcl->fcc,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_ADDRESS1)->SetWindowText( !!fcl ? EDT_DBF_WST(fcl->adr1,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_ADDRESS2)->SetWindowText( !!fcl ? EDT_DBF_WST(fcl->adr2,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_TELEPHONE)->SetWindowText( !!fcl ? EDT_DBF_WST(fcl->tel,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText( !!fcl ? EDT_DBF_WST(fcl->fax,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_WEBURL)->SetWindowText( !!fcl ? EDT_DBF_WST(fcl->url,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_EMAIL)->SetWindowText( !!fcl ? EDT_DBF_WST(fcl->eml,_T("")):_T("") );
	((CButton *)(GetDlgItem(IDC_CHECK_DIRECTMANAGE)))->SetCheck( !!fcl && ( Nul_Fld(fcl->id,FLD_INT) || !!DBS_INT_VAL(fcl->cms) ) );
	GetDlgItem(IDC_EDIT_NORMALTAIMECHECKIN)->SetWindowText( !!fcl ? EDT_DBF_TIM(buf, memcnt(buf),fcl->cit):_T("") );
	GetDlgItem(IDC_EDIT_NORMALTAIMECHECKOUT)->SetWindowText( !!fcl ? EDT_DBF_TIM(buf, memcnt(buf),fcl->cot):_T("") );
	GetDlgItem(IDC_EDIT_EXEMPTIONECI)->SetWindowText( !!fcl ? EDT_DBF_INT(buf, memcnt(buf),fcl->eci,Edt_Num_Nml):_T("") );
	GetDlgItem(IDC_EDIT_EXEMPTIONLCO)->SetWindowText( !!fcl ? EDT_DBF_INT(buf, memcnt(buf),fcl->lco,Edt_Num_Nml):_T("") );
	((CButton *)(GetDlgItem(IDC_CHECK_HAVEEQUIPMENT)))->SetCheck( !!fcl && ( !Nul_Fld(fcl->hve, FLD_INT) && !!DBS_INT_VAL(fcl->hve) ) );
	((CButton *)(GetDlgItem(IDC_CHECK_DIRECTMANAGE)))->SetCheck( !!fcl && ( !Nul_Fld(fcl->cms, FLD_INT) && !!DBS_INT_VAL(fcl->cms) ) );
	((CButton *)(GetDlgItem(IDC_CHECK_DAYCLOSECHARGE)))->SetCheck( !!fcl && ( !Nul_Fld(fcl->dcc, FLD_INT) && !!DBS_INT_VAL(fcl->dcc) ) );

	opt = !!fcl ? DBS_INT_VAL(fcl->opt): 0;
	((CButton *)(GetDlgItem(IDC_CHECK_APPLYEXTENDCHARGE)))->SetCheck( !!( opt & APPLY_EXTEND_CHARGE ) );
	((CButton *)(GetDlgItem(IDC_CHECK_FORCEDEPOSIOCOLLECT)))->SetCheck( !!( opt & FORCE_DEPOSIT_COLLECT ) );
	((CButton *)(GetDlgItem(IDC_CHECK_NEEDCONVENTANT)))->SetCheck( !!( opt & NEED_CONVENTANT ) );
	((CButton *)(GetDlgItem(IDC_CHECK_SENDCOMFIRMATIONMAIL)))->SetCheck( !!( opt & SEND_CONFIRMATION_EMAIL ) );

	if ( !!fcl && !Nul_Fld(fcl->fck,FLD_INT) )	m_Fcl_knd->SetKind( DBS_INT_VAL(fcl->fck) );
	else										m_Fcl_knd->SetCurSel(-1);
		
	GetDlgItem(IDC_EDIT_FACILITYCODE)->EnableWindow( !!fcl && Nul_Fld(fcl->id, FLD_INT) );
	GetDlgItem(IDC_CHECK_DAYCLOSECHARGE)->EnableWindow( FALSE );
	if ( !(enb = !fcl) ){
		while ( csr = Nxt_Lnk( m_Fcl_rot, csr ) ){
			otr = Rec_Of(FCL_INF, csr);
			if ( !!DBS_INT_VAL(otr->dcc) ){
				if ( DBS_INT_VAL(otr->id) == DBS_INT_VAL(fcl->id) ){
					enb = TRUE;
				}
				else{
					fnd = TRUE;
				}
				break;
			}
		}
	}
	GetDlgItem(IDC_CHECK_DAYCLOSECHARGE)->EnableWindow( !!fcl && ( !fnd || enb ) );

	ButtonEnable();
}

void	CFacilityInfoDlg::ButtonEnable()	
{
	int			enb, sel;
	enb  = ((CComboBox *)GetDlgItem(IDC_COMBO_BRANCH))->GetCurSel() != -1;

	sel = m_Fcl_grd->GetSelRow() > 0;

	GetDlgItem(IDC_EDIT_OPERATORNAME)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ABBREVIATION)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_FACILITYCODE)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ADDRESS1)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ADDRESS2)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_TELEPHONE)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_FAXNUMBER)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_WEBURL)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_EMAIL)->EnableWindow( enb );
	GetDlgItem(IDC_CHECK_DIRECTMANAGE)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_NORMALTAIMECHECKIN)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_NORMALTAIMECHECKOUT)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_EXEMPTIONECI)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_EXEMPTIONLCO)->EnableWindow( enb );
	GetDlgItem(IDC_CHECK_HAVEEQUIPMENT)->EnableWindow( enb );
	//GetDlgItem(IDC_CHECK_DAYCLOSECHARGE)->EnableWindow( enb );
	GetDlgItem(IDC_CHECK_APPLYEXTENDCHARGE)->EnableWindow( enb );
	GetDlgItem(IDC_CHECK_FORCEDEPOSIOCOLLECT)->EnableWindow( enb );
	GetDlgItem(IDC_CHECK_NEEDCONVENTANT)->EnableWindow( enb );
	GetDlgItem(IDC_CHECK_SENDCOMFIRMATIONMAIL)->EnableWindow( enb );
	m_Fcl_knd->EnableWindow( enb );
	if ( !enb )	m_Fcl_knd->SetCurSel(-1);
	GetDlgItem(IDC_EDIT_FACILITYCODE)->EnableWindow( enb );

	GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow( sel && enb );
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow( enb);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow( sel && enb );
}

DWORD CFacilityInfoDlg::ConfirmSaveChaned( int msg, int fce )
{
	Csr			csr=NULL;
	FCL_INF		*fcl;
	int			chk, ans=IDYES;
	WCHAR		*erm=NULL;

	while ( csr=Nxt_Lnk( m_Fcl_rot, csr ) ){
		fcl = Rec_Of(FCL_INF, csr);
		if ( !Nul_Fld(fcl->dcc, FLD_INT) && !!DBS_INT_VAL(fcl->dcc) ){
			break;
		}
	}
	if ( chk = ( !Cnt_Lnk( m_Fcl_rot) || !!csr ) ){
		for( csr=NULL; csr=Nxt_Lnk( m_Fcl_rot, csr ); ){
			fcl = Rec_Of(FCL_INF, csr);
			if ( !Nul_Fld(fcl->mdt, FLD_DTM) || !Nul_Fld(fcl->ddt, FLD_DTM) || Nul_Fld(fcl->id, FLD_INT) ){
				break;
			}
		}
		if ( !!csr ){
			ans = Viw_Cfm_Svc( m_Fcl_rot, Fci_Sav_Dat, msg, fce, this, m_pDbs_obj, m_pPms_mng );
		}
	}
	else{
		MessageBox( _T("DayCloseCharge should be ON on one of either facility."), NULL, MB_ICONEXCLAMATION|MB_OK );
		ans = IDCANCEL;
	}

	return( ans );
}

// CFacilityInfoDlg メッセージ ハンドラ

BOOL CFacilityInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pPms_mng = ((CMainFrame *)GetParent())->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)GetParent())->m_Dbs_obj;

	SetupComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// グリッドイベント
LRESULT CFacilityInfoDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;
	int				row;
	FCL_INF			*fcl;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:	row = ntf->row;
								if ( row > 0 ){
									fcl = (FCL_INF *)(LONG_PTR)m_Fcl_grd->GetData( CLM_DAT, row );
									if ( fcl ){
										SetEditData( fcl );
									}
								}
								break;
		}
		free( ntf );
	}

	return( rsl );
}

// branch combobox changed
void CFacilityInfoDlg::OnCbnSelchangeComboBranch()
{
	int			idx, row=0;
	Csr			csr=NULL;
	WCHAR		*erm=NULL;
	BRC_INF		*brc;
	FCL_INF		*fcl;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);

	if ( ( ( idx = bcx->GetCurSel() ) != -1 ) && idx!=m_Brc_idx ){
		if ( ConfirmSaveChaned( TRUE, FALSE )!=IDCANCEL ){
			Del_All( m_Fcl_rot );
			m_Fcl_grd->ResetContent();
			brc = (BRC_INF *)(LONG_PTR)bcx->GetItemData( idx );
			if ( Fci_Get_Dat( m_Fcl_rot, m_pPms_mng->opc, DBS_WST_PTR(brc->brc), &erm, m_pDbs_obj, m_pPms_mng ) ){
				m_Fcl_grd->AddRow( Cnt_Lnk( m_Fcl_rot ) );
				while( csr = Nxt_Lnk( m_Fcl_rot, csr ) ){
					fcl = Rec_Of(FCL_INF, csr);
					Set_Nul(fcl->mdt, FLD_DTM, TRUE);
					DispData( fcl, ++row );
				}
			}
		}
		else{
			bcx->SetCurSel( idx = m_Brc_idx );
		}
	}
	m_Brc_idx = idx;
	ButtonEnable();
}

// Update button clicked
void CFacilityInfoDlg::OnBnClickedButtonUpdate()
{
	int			row;
	FCL_INF		tmp[1], *fcl;
	Day			dte[1];

	if ( ( row = m_Fcl_grd->GetSelRow() ) > 0 ){
		fcl = (FCL_INF *)(LONG_PTR)m_Fcl_grd->GetData( CLM_DAT, row );
		*tmp = *fcl;
		if ( GetData( tmp ) ){
			*fcl = *tmp;
			Get_Dte( dte );
			DBS_DTM_SET(fcl->mdt, dte);
			DispData( fcl, row );
			SetEditData( NULL );
			m_Fcl_grd->SetSelectCell( -1, -1 );
		}
	}
}

// Add button clicked
void CFacilityInfoDlg::OnBnClickedButtonAdd()
{
	int			row;
	Csr			csr;
	FCL_INF		tmp[1], *fcl;

	memset( tmp, 0, sizeof(tmp) );

	if ( GetData( tmp ) ){
		DBS_WST_SET(tmp->opc, m_pPms_mng->opc );
		if ( csr = Add_Lnk( m_Fcl_rot, tmp, sizeof(tmp) ) ){
			fcl = Rec_Of(FCL_INF, csr);
			row = m_Fcl_grd->GetLineCount( FALSE );
			m_Fcl_grd->AddRow( 1 );
			DispData( fcl, ++row );
			SetEditData( NULL );
			m_Fcl_grd->SetSelectCell( -1, -1 );
		}
	}
}

// Deletebuton clicked
void CFacilityInfoDlg::OnBnClickedButtonDelete()
{
	int			row;
	FCL_INF		*fcl;
	Day			dte[1];

	if ( ( row = m_Fcl_grd->GetSelRow() ) > 0 ){
		fcl = (FCL_INF *)(LONG_PTR)m_Fcl_grd->GetData( CLM_DAT, row );
		if ( MessageBox( _T("Are you sure to delete?"), NULL, MB_ICONQUESTION|MB_YESNO ) == IDYES ){
			if ( Nul_Fld(fcl->id, FLD_INT) ){
				Del_Lnk( m_Fcl_rot, Csr_Of(fcl) );
			}
			else{
				Get_Dte( dte );
				DBS_DAY_SET(fcl->ddt, dte);
			}
			m_Fcl_grd->DelRow( row );
			m_Fcl_grd->SetSelectCell(-1, -1);
			m_Fcl_grd->Redraw();
		}
	}
}

void CFacilityInfoDlg::OnBnClickedOk()
{
	if ( ConfirmSaveChaned( FALSE, FALSE )==IDYES ){
		OnOK();
	}
}

void CFacilityInfoDlg::OnBnClickedCancel()
{
	if ( ConfirmSaveChaned( TRUE, TRUE )==IDYES ){
		OnCancel();
	}
}

BOOL CFacilityInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}
