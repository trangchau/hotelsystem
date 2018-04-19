// TravelAgentDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MainFrm.h"
#include "TravelAgentDlg.h"


#include ".\fnc\agt_sql.h"
#include ".\fnc\mfm_sql.h"
#include ".\fnc\sql_cmn.h"
#include ".\fnc\viw_cmn.h"

// CTravelAgentDlg ダイアログ


static	WCHAR	*Grd_ttl[] =	{	{ _T("Code")		},
									{ _T("Name")		},
									{ _T("Abbreviation")},
									{ _T("Kind")		}};

static	GRD_CLM	Grd_clm[] =		{	{  96, GAL_LFT, GCT_NML,  12 },		// Code
									{ 256, GAL_LFT, GCT_NML,  12 },		// Name
									{ 128, GAL_LFT, GCT_NML,  12 },		// Abbreviation
									{  64, GAL_CTR, GCT_NML,  12 }};	// Agent Kind

#define	IDC_GRD_BRC		(8192)	// Copy destination selecting grid

#define	CLM_DAT			0		// Data keeping column

#define	CLM_COD			0		// Code
#define	CLM_NAM			1		// Name
#define	CLM_ABR			2		// Abbreviation
#define	CLM_KND			3		// Agent Kind

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

IMPLEMENT_DYNAMIC(CTravelAgentDlg, CDialog)

CTravelAgentDlg::CTravelAgentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTravelAgentDlg::IDD, pParent)
{
	m_pPms_mng = NULL;
	m_pDbs_obj = NULL;
	m_Ope_cod = NULL;
	m_Agt_grd = NULL;
	memset( m_Agt_rot, 0, sizeof(m_Agt_rot) );
}

CTravelAgentDlg::~CTravelAgentDlg()
{
	if ( m_Agt_grd )	delete m_Agt_grd;
	Del_All( m_Agt_rot );
}

void CTravelAgentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTravelAgentDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CTravelAgentDlg::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CTravelAgentDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CTravelAgentDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDCANCEL, &CTravelAgentDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CTravelAgentDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTravelAgentDlg メッセージ ハンドラ

int	 CTravelAgentDlg::InitData( WCHAR *opc )
{
	WCHAR		*erm=NULL;
	AGT_INF		*agt;
	CON_TRY		*ctc;
	int			chk, row=0;
	Csr			csr=NULL;
	CComboBox	*ccb=(CComboBox *)GetDlgItem(IDC_COMBO_COUNTRY);

	m_Ope_cod = opc;

	Del_All( m_Agt_rot );
	m_Agt_grd->ResetContent();

	SetEditData( NULL );

	if ( chk = Agt_Get_Dat( m_Agt_rot, opc, &erm, m_pDbs_obj, m_pPms_mng ) ){
		m_Agt_grd->AddRow( Cnt_Lnk( m_Agt_rot ) );
		while( csr = Nxt_Lnk( m_Agt_rot, csr ) ){
			agt = Rec_Of(AGT_INF, csr);
			Set_Nul(agt->mdt, FLD_DTM, TRUE);
			DispData( agt, ++row );
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

void CTravelAgentDlg::SetupComponent()
{
	int				idx;
	RECT			rct[1];
	GRD_DEF			gdf[1];

	struct	{
		CWnd	*wnd;
		int		lmt;
		DWORD	cid;
	}Cpn_[] = {
		{ NULL,      255, IDC_EDIT_AGENTNAME		},
		{ NULL,      128, IDC_EDIT_ABBREVIATION		},
		{ NULL,      128, IDC_EDIT_AGENTCODE		},
		{ NULL,       16, IDC_EDIT_ZIPCODE			},
		{ NULL,      255, IDC_EDIT_ADDRESS1			},
		{ NULL,      255, IDC_EDIT_ADDRESS2			},
		{ NULL,       30, IDC_EDIT_TELEPHONE1		},
		{ NULL,       30, IDC_EDIT_TELEPHONE2		},
		{ NULL,       30, IDC_EDIT_FAXNUMBER		},
		{ NULL,      255, IDC_EDIT_WEBURL			},
		{ NULL,      128, IDC_EDIT_EMAIL1			},
		{ NULL,      128, IDC_EDIT_EMAIL2			},
//		{ NULL,       -1, IDC_STATIC_PANEL			},
		{ NULL,       -1, IDC_STATIC_BRANCHNAME		},
		{ NULL,       -1, IDC_STATIC_ABBREVIATION	},
		{ NULL,       -1, IDC_STATIC_AGENTCODE		},
		{ NULL,       -1, IDC_STATIC_ZIPCODE		},
		{ NULL,       -1, IDC_STATIC_ADDRESS1		},
		{ NULL,       -1, IDC_STATIC_ADDRESS2		},
		{ NULL,       -1, IDC_STATIC_TELEPHONE1		},
		{ NULL,       -1, IDC_STATIC_TELEPHONE2		},
		{ NULL,       -1, IDC_STATIC_FAX			},
		{ NULL,       -1, IDC_STATIC_WEBURL			},
		{ NULL,       -1, IDC_STATIC_EMAIL1			},
		{ NULL,       -1, IDC_STATIC_EMAIL2			},
		{ NULL,       -1, IDC_CHECK_DIRECTMANAGE	}
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

	m_Agt_grd = new CSimpleGrid();
	m_Agt_grd->Create( gdf, *rct, IDC_GRD_BRC, this );
	for ( idx=0; idx<memcnt(Grd_ttl); idx++ ){
		m_Agt_grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}
}


int		CTravelAgentDlg::GetData( AGT_INF *agt )
{
	int			chk, val, idx;
	WCHAR		buf[96], *itm=NULL;
	CComboBox	*ccb=(CComboBox *)GetDlgItem(IDC_COMBO_COUNTRY);

//	if ( m_Agt_grd->GetSelRow() > 0 ){
		WIN_TXT_DBS(agt->nam, IDC_EDIT_AGENTNAME, this);
		WIN_TXT_DBS(agt->abr, IDC_EDIT_ABBREVIATION, this);
		WIN_TXT_DBS(agt->agc, IDC_EDIT_AGENTCODE, this);
		WIN_TXT_DBS(agt->zip, IDC_EDIT_ZIPCODE, this);
		WIN_TXT_DBS(agt->adr1, IDC_EDIT_ADDRESS1, this);
		WIN_TXT_DBS(agt->adr2, IDC_EDIT_ADDRESS2, this);
		WIN_TXT_DBS(agt->tel1, IDC_EDIT_TELEPHONE1, this);
		WIN_TXT_DBS(agt->tel2, IDC_EDIT_TELEPHONE2, this);
		WIN_TXT_DBS(agt->fax, IDC_EDIT_FAXNUMBER, this);
		WIN_TXT_DBS(agt->url, IDC_EDIT_WEBURL, this);
		WIN_TXT_DBS(agt->eml1, IDC_EDIT_EMAIL1, this);
		WIN_TXT_DBS(agt->eml2, IDC_EDIT_EMAIL2, this);
		val = ((CButton *)GetDlgItem(IDC_CHECK_OTA) )->GetCheck() ? AGK_ONLINE: AGK_REAL;
		DBS_INT_SET(agt->agk, &val );

		if ( Nul_Fld(agt->id, FLD_INT) ){
			DBS_WST_SET(agt->opc, m_Ope_cod);
//			DBS_INT_SET(agt->adi, &val);
//			DBS_INT_SET(agt->acm, &val);
//			DBS_INT_SET(brc->asb, &val);
		}
		if ( ( idx = ccb->GetCurSel() ) != -1 ){
			Set_Nul( agt->ctr, FLD_WST, NULL );
			wcscpy_s( DBS_WST_PTR(agt->ctr), FLD_WST_SIZ(agt->ctr), (WCHAR *)(LONG_PTR)ccb->GetItemData(idx) );
		}
		else{
			Set_Nul( agt->ctr, FLD_WST, TRUE );
		}

		if ( !( chk = !Nul_Fld(agt->nam, FLD_WST) ) )			itm=_T("AgentName");	
		if ( chk && !( chk = !Nul_Fld(agt->agc, FLD_WST) ) )	itm=_T("AgentCode");
		if ( !chk ){
			swprintf_s( buf, memcnt(buf), _T("%s is required item"), itm ? itm: _T("???????") );
			MessageBox( buf, NULL, MB_ICONEXCLAMATION|MB_OK );
		}
//	}

	return( chk );
}

void	CTravelAgentDlg::DispData( AGT_INF *agt, int row )
{
	WCHAR		*knd=_T("Unknown");

	m_Agt_grd->SetCellText( CLM_COD, row, EDT_DBF_WST(agt->agc, _T("")) );
	m_Agt_grd->SetCellText( CLM_NAM, row, EDT_DBF_WST(agt->nam, _T("")) );
	m_Agt_grd->SetCellText( CLM_ABR, row, EDT_DBF_WST(agt->abr, _T("")) );

	knd = DBS_INT_VAL(agt->agk)==AGK_ONLINE ? _T("OTA"): _T("Real");
	m_Agt_grd->SetCellText( CLM_KND, row, knd );

//	m_Agt_grd->SetCheck( CLM_DMB, row, !!DBS_INT_VAL(agt->cms ) );

	m_Agt_grd->SetData( CLM_DAT, row, (int)(LONG_PTR)agt );
}

void	CTravelAgentDlg::SetEditData( AGT_INF *agt )
{
	int			fnd=FALSE;
	Csr			csr=NULL;
	CComboBox	*ccb=(CComboBox *)GetDlgItem(IDC_COMBO_COUNTRY);

	GetDlgItem(IDC_EDIT_AGENTNAME)->SetWindowText( !!agt ? EDT_DBF_WST(agt->nam,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_ABBREVIATION)->SetWindowText( !!agt ? EDT_DBF_WST(agt->abr,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_AGENTCODE)->SetWindowText( !!agt ? EDT_DBF_WST(agt->agc,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_ZIPCODE)->SetWindowText( !!agt ? EDT_DBF_WST(agt->zip,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_ADDRESS1)->SetWindowText( !!agt ? EDT_DBF_WST(agt->adr1,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_ADDRESS2)->SetWindowText( !!agt ? EDT_DBF_WST(agt->adr2,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_TELEPHONE1)->SetWindowText( !!agt ? EDT_DBF_WST(agt->tel1,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_TELEPHONE2)->SetWindowText( !!agt ? EDT_DBF_WST(agt->tel2,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText( !!agt ? EDT_DBF_WST(agt->fax,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_WEBURL)->SetWindowText( !!agt ? EDT_DBF_WST(agt->url,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_EMAIL1)->SetWindowText( !!agt ? EDT_DBF_WST(agt->eml1,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_EMAIL2)->SetWindowText( !!agt ? EDT_DBF_WST(agt->eml2,_T("")):_T("") );
	((CButton *)GetDlgItem(IDC_CHECK_OTA))->SetCheck( !!agt && DBS_INT_VAL(agt->agk)==AGK_ONLINE );
	((CButton *)GetDlgItem(IDC_CHECK_REAL))->SetCheck( !!agt && DBS_INT_VAL(agt->agk)==AGK_REAL );
//	((CButton *)(GetDlgItem(IDC_CHECK_DIRECTMANAGE)))->SetCheck( !!brc && ( Nul_Fld(brc->id,FLD_INT) || !!DBS_INT_VAL(brc->cms) ) );
//	GetDlgItem(IDC_EDIT_BRANCHCODE)->EnableWindow( !!brc && Nul_Fld(brc->id, FLD_INT) );
	ccb->SetCurSel( !!agt && !Nul_Fld(agt->ctr, FLD_WST) ? get_idx( ccb, DBS_WST_PTR(agt->ctr) ): -1 );

	ButtonEnable();
}

void	CTravelAgentDlg::ButtonEnable()	
{
	int			enb=TRUE, sel;

	sel = m_Agt_grd->GetSelRow() > 0;

	GetDlgItem(IDC_EDIT_AGENTNAME)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ABBREVIATION)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_AGENTCODE)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ZIPCODE)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ADDRESS1)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ADDRESS2)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_TELEPHONE1)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_TELEPHONE2)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_FAXNUMBER)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_WEBURL)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_EMAIL1)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_EMAIL2)->EnableWindow( enb );
	GetDlgItem(IDC_CHECK_OTA)->EnableWindow( enb );
	GetDlgItem(IDC_CHECK_REAL)->EnableWindow( enb );

	GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow( sel && enb );
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow( enb);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow( sel && enb );
}

DWORD CTravelAgentDlg::ConfirmSaveChaned( int msg, int fce )
{
	Csr			csr=NULL;
	AGT_INF		*agt;
	int			ans=IDYES;
	WCHAR		*erm=NULL;

	for( csr=NULL; csr=Nxt_Lnk( m_Agt_rot, csr ); ){
		agt = Rec_Of(AGT_INF, csr);
		if ( !Nul_Fld(agt->mdt, FLD_DTM) || !Nul_Fld(agt->ddt, FLD_DTM) || Nul_Fld(agt->id, FLD_INT) ){
			break;
		}
	}
	if ( !!csr ){
		ans = Viw_Cfm_Svc( m_Agt_rot, Agt_Sav_Dat, msg, fce, this, m_pDbs_obj, m_pPms_mng );
	}

	return( ans );
}

BOOL CTravelAgentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pPms_mng = ((CMainFrame *)GetParent())->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)GetParent())->m_Dbs_obj;

	SetupComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// グリッドイベント
LRESULT CTravelAgentDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;
	int				row;
	AGT_INF			*agt;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:	row = ntf->row;
								if ( row > 0 ){
									agt = (AGT_INF *)(LONG_PTR)m_Agt_grd->GetData( CLM_DAT, row );
									if ( agt ){
										SetEditData( agt );
									}
								}
								break;
		}
		free( ntf );
	}

	return( rsl );
}

// Update button clicked
void CTravelAgentDlg::OnBnClickedButtonUpdate()
{
	int			row;
	AGT_INF		tmp[1], *agt;
	Day			dte[1];

	if ( ( row = m_Agt_grd->GetSelRow() ) > 0 ){
		agt = (AGT_INF *)(LONG_PTR)m_Agt_grd->GetData( CLM_DAT, row );
		*tmp = *agt;
		if ( GetData( tmp ) ){
			*agt = *tmp;
			Get_Dte( dte );
			DBS_DTM_SET(agt->mdt, dte);
			DispData( agt, row );
			SetEditData( NULL );
			m_Agt_grd->SetSelectCell( -1, -1 );
		}
	}
}

// Add button clicked
void CTravelAgentDlg::OnBnClickedButtonAdd()
{
	int			row;
	Csr			csr;
	AGT_INF		tmp[1], *agt;

	memset( tmp, 0, sizeof(tmp) );

	if ( GetData( tmp ) ){
		DBS_WST_SET(tmp->opc, m_pPms_mng->opc );
		if ( csr = Add_Lnk( m_Agt_rot, tmp, sizeof(tmp) ) ){
			agt = Rec_Of(AGT_INF, csr);
			row = m_Agt_grd->GetLineCount( FALSE );
			m_Agt_grd->AddRow( 1 );
			DispData( agt, ++row );
			SetEditData( NULL );
			m_Agt_grd->SetSelectCell( -1, -1 );
		}
	}
}

// Deletebuton clicked
void CTravelAgentDlg::OnBnClickedButtonDelete()
{
	int			row;
	AGT_INF		*agt;
	Day			dte[1];

	if ( ( row = m_Agt_grd->GetSelRow() ) > 0 ){
		agt = (AGT_INF *)(LONG_PTR)m_Agt_grd->GetData( CLM_DAT, row );
		if ( MessageBox( _T("Are you sure to delete?"), NULL, MB_ICONQUESTION|MB_YESNO ) == IDYES ){
			if ( Nul_Fld(agt->id, FLD_INT) ){
				Del_Lnk( m_Agt_rot, Csr_Of(agt) );
			}
			else{
				Get_Dte( dte );
				DBS_DAY_SET(agt->ddt, dte);
			}
			m_Agt_grd->DelRow( row );
			m_Agt_grd->SetSelectCell(-1, -1);
			m_Agt_grd->Redraw();
		}
	}
}

void CTravelAgentDlg::OnBnClickedOk()
{
	if ( ConfirmSaveChaned( FALSE, FALSE )==IDYES ){
		OnOK();
	}
}

void CTravelAgentDlg::OnBnClickedCancel()
{
	if ( ConfirmSaveChaned( TRUE, TRUE )==IDYES ){
		OnCancel();
	}
}

BOOL CTravelAgentDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}
