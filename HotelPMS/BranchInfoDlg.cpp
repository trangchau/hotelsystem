// BranchInfoDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MainFrm.h"
#include "BranchInfoDlg.h"

#include ".\fnc\bri_sql.h"
#include ".\fnc\sql_cmn.h"
#include ".\fnc\viw_cmn.h"

// CBranchInfoDlg ダイアログ


static	WCHAR	*Grd_ttl[] =	{	{ _T("Code")		},
									{ _T("Name")		},
									{ _T("Abbreviation")},
									{ _T("Direct")		}};

static	GRD_CLM	Grd_clm[] =		{	{  96, GAL_LFT, GCT_NML,  12 },		// Code
									{ 256, GAL_LFT, GCT_NML,  12 },		// Name
									{ 128, GAL_LFT, GCT_NML,  12 },		// Abbreviation
									{  64, GAL_CTR, GCT_CHK,  12 }};	// DirectManagementBranch

#define	IDC_GRD_BRC		(8192)	// Copy destination selecting grid

#define	CLM_DAT			0		// Data keeping column

#define	CLM_COD			0		// Code
#define	CLM_NAM			1		// Name
#define	CLM_ABR			2		// Abbreviation
#define	CLM_DMB			3		// DirectManagementBranch

IMPLEMENT_DYNAMIC(CBranchInfoDlg, CDialog)

CBranchInfoDlg::CBranchInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBranchInfoDlg::IDD, pParent)
{
	m_pPms_mng = NULL;
	m_pDbs_obj = NULL;
	m_Ope_cod = NULL;
	m_Brc_grd = NULL;
	memset( m_Brc_rot, 0, sizeof(m_Brc_rot) );
}

CBranchInfoDlg::~CBranchInfoDlg()
{
	if ( m_Brc_grd )	delete m_Brc_grd;
	Del_All( m_Brc_rot );
}

void CBranchInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBranchInfoDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CBranchInfoDlg::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CBranchInfoDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CBranchInfoDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDCANCEL, &CBranchInfoDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CBranchInfoDlg::OnBnClickedOk)
END_MESSAGE_MAP()

int	 CBranchInfoDlg::InitData( WCHAR *opc )
{
	WCHAR		*erm=NULL;
	int			chk, row=0;
	Csr			csr=NULL;
	BRC_INF		*brc;

	m_Ope_cod = opc;

	Del_All( m_Brc_rot );
	m_Brc_grd->ResetContent();

	SetEditData( NULL );

	if ( chk = Bri_Get_Dat( m_Brc_rot, opc, &erm, m_pDbs_obj, m_pPms_mng ) ){
		m_Brc_grd->AddRow( Cnt_Lnk( m_Brc_rot ) );
		while( csr = Nxt_Lnk( m_Brc_rot, csr ) ){
			brc = Rec_Of(BRC_INF, csr);
			Set_Nul(brc->mdt, FLD_DTM, TRUE);
			DispData( brc, ++row );
		}
	}
	else{
		if( !!erm ){
			MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
	}

	return( chk );
}

void CBranchInfoDlg::SetupComponent()
{
	int				idx;
	RECT			rct[1];
	GRD_DEF			gdf[1];

	struct	{
		CWnd	*wnd;
		int		lmt;
		DWORD	cid;
	}Cpn_[] = {
		{ NULL,      255, IDC_EDIT_OPERATORNAME		},
		{ NULL,      128, IDC_EDIT_ABBREVIATION		},
		{ NULL,      128, IDC_EDIT_BRANCHCODE		},
		{ NULL,      255, IDC_EDIT_ADDRESS1			},
		{ NULL,      255, IDC_EDIT_ADDRESS2			},
		{ NULL,       30, IDC_EDIT_TELEPHONE		},
		{ NULL,       30, IDC_EDIT_FAXNUMBER		},
		{ NULL,      255, IDC_EDIT_WEBURL			},
		{ NULL,      128, IDC_EDIT_EMAIL			},
//		{ NULL,       -1, IDC_STATIC_PANEL			},
		{ NULL,       -1, IDC_STATIC_BRANCHNAME		},
		{ NULL,       -1, IDC_STATIC_ABBREVIATION	},
		{ NULL,       -1, IDC_STATIC_BRANCHCODE		},
		{ NULL,       -1, IDC_STATIC_ADDRESS1		},
		{ NULL,       -1, IDC_STATIC_ADDRESS2		},
		{ NULL,       -1, IDC_STATIC_TELEPHONE		},
		{ NULL,       -1, IDC_STATIC_FAX			},
		{ NULL,       -1, IDC_STATIC_WEBURL			},
		{ NULL,       -1, IDC_STATIC_EMAIL			},
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

	m_Brc_grd = new CSimpleGrid();
	m_Brc_grd->Create( gdf, *rct, IDC_GRD_BRC, this );
	for ( idx=0; idx<memcnt(Grd_ttl); idx++ ){
		m_Brc_grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}
}


int		CBranchInfoDlg::GetData( BRC_INF *brc )
{
	int			chk, val;
	WCHAR		buf[96], *itm=NULL;

//	if ( m_Brc_grd->GetSelRow() > 0 ){
		WIN_TXT_DBS(brc->nam, IDC_EDIT_OPERATORNAME, this);
		WIN_TXT_DBS(brc->abr, IDC_EDIT_ABBREVIATION, this);
		WIN_TXT_DBS(brc->brc, IDC_EDIT_BRANCHCODE, this);
		WIN_TXT_DBS(brc->adr1, IDC_EDIT_ADDRESS1, this);
		WIN_TXT_DBS(brc->adr2, IDC_EDIT_ADDRESS2, this);
		WIN_TXT_DBS(brc->tel, IDC_EDIT_TELEPHONE, this);
		WIN_TXT_DBS(brc->fax, IDC_EDIT_FAXNUMBER, this);
		WIN_TXT_DBS(brc->url, IDC_EDIT_WEBURL, this);
		WIN_TXT_DBS(brc->eml, IDC_EDIT_EMAIL, this);
		WIN_CBX_DBS(brc->cms, IDC_CHECK_DIRECTMANAGE, this);

		if ( Nul_Fld(brc->id, FLD_INT) ){
			DBS_WST_SET(brc->opc, m_Ope_cod);
			DBS_INT_SET(brc->adi, &val);
			DBS_INT_SET(brc->acm, &val);
//			DBS_INT_SET(brc->asb, &val);
		}

		if ( !( chk = !Nul_Fld(brc->nam, FLD_WST) ) )			itm=_T("BranchName");	
		if ( chk && !( chk = !Nul_Fld(brc->brc, FLD_WST) ) )	itm=_T("BranchCode");
		if ( !chk ){
			swprintf_s( buf, memcnt(buf), _T("%s is required item"), itm ? itm: _T("???????") );
			MessageBox( buf, NULL, MB_ICONEXCLAMATION|MB_OK );
		}
//	}

	return( chk );
}

void	CBranchInfoDlg::DispData( BRC_INF *brc, int row )
{
	m_Brc_grd->SetCellText( CLM_COD, row, EDT_DBF_WST(brc->brc, _T("")) );
	m_Brc_grd->SetCellText( CLM_NAM, row, EDT_DBF_WST(brc->nam, _T("")) );
	m_Brc_grd->SetCellText( CLM_ABR, row, EDT_DBF_WST(brc->abr, _T("")) );
	m_Brc_grd->SetCheck( CLM_DMB, row, !!DBS_INT_VAL(brc->cms ) );

	m_Brc_grd->SetData( CLM_DAT, row, (int)(LONG_PTR)brc );
}

void	CBranchInfoDlg::SetEditData( BRC_INF *brc )
{
	int			fnd=FALSE;
	Csr			csr=NULL;

	GetDlgItem(IDC_EDIT_OPERATORNAME)->SetWindowText( !!brc ? EDT_DBF_WST(brc->nam,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_ABBREVIATION)->SetWindowText( !!brc ? EDT_DBF_WST(brc->abr,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_BRANCHCODE)->SetWindowText( !!brc ? EDT_DBF_WST(brc->brc,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_ADDRESS1)->SetWindowText( !!brc ? EDT_DBF_WST(brc->adr1,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_ADDRESS2)->SetWindowText( !!brc ? EDT_DBF_WST(brc->adr2,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_TELEPHONE)->SetWindowText( !!brc ? EDT_DBF_WST(brc->tel,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText( !!brc ? EDT_DBF_WST(brc->fax,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_WEBURL)->SetWindowText( !!brc ? EDT_DBF_WST(brc->url,_T("")):_T("") );
	GetDlgItem(IDC_EDIT_EMAIL)->SetWindowText( !!brc ? EDT_DBF_WST(brc->eml,_T("")):_T("") );
	((CButton *)(GetDlgItem(IDC_CHECK_DIRECTMANAGE)))->SetCheck( !!brc && ( Nul_Fld(brc->id,FLD_INT) || !!DBS_INT_VAL(brc->cms) ) );
	GetDlgItem(IDC_EDIT_BRANCHCODE)->EnableWindow( !!brc && Nul_Fld(brc->id, FLD_INT) );

	ButtonEnable();
}

void	CBranchInfoDlg::ButtonEnable()	
{
	int			enb=TRUE, sel;

	sel = m_Brc_grd->GetSelRow() > 0;

	GetDlgItem(IDC_EDIT_OPERATORNAME)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ABBREVIATION)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_BRANCHCODE)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ADDRESS1)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ADDRESS2)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_TELEPHONE)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_FAXNUMBER)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_WEBURL)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_EMAIL)->EnableWindow( enb );
	GetDlgItem(IDC_CHECK_DIRECTMANAGE)->EnableWindow( enb );

	GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow( sel && enb );
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow( enb);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow( sel && enb );
}

DWORD CBranchInfoDlg::ConfirmSaveChaned( int msg, int fce )
{
	Csr			csr=NULL;
	BRC_INF		*brc;
	int			ans=IDYES;
	WCHAR		*erm=NULL;

	for( csr=NULL; csr=Nxt_Lnk( m_Brc_rot, csr ); ){
		brc = Rec_Of(BRC_INF, csr);
		if ( !Nul_Fld(brc->mdt, FLD_DTM) || !Nul_Fld(brc->ddt, FLD_DTM) || Nul_Fld(brc->id, FLD_INT) ){
			break;
		}
	}
	if ( !!csr ){
		ans = Viw_Cfm_Svc( m_Brc_rot, Bri_Sav_Dat, msg, fce, this, m_pDbs_obj, m_pPms_mng );
	}

	return( ans );
}

BOOL CBranchInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pPms_mng = ((CMainFrame *)GetParent())->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)GetParent())->m_Dbs_obj;

	SetupComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// グリッドイベント
LRESULT CBranchInfoDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;
	int				row;
	BRC_INF			*brc;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:	row = ntf->row;
								if ( row > 0 ){
									brc = (BRC_INF *)(LONG_PTR)m_Brc_grd->GetData( CLM_DAT, row );
									if ( brc ){
										SetEditData( brc );
									}
								}
								break;
		}
		free( ntf );
	}

	return( rsl );
}

// Update button clicked
void CBranchInfoDlg::OnBnClickedButtonUpdate()
{
	int			row;
	BRC_INF		tmp[1], *brc;
	Day			dte[1];

	if ( ( row = m_Brc_grd->GetSelRow() ) > 0 ){
		brc = (BRC_INF *)(LONG_PTR)m_Brc_grd->GetData( CLM_DAT, row );
		*tmp = *brc;
		if ( GetData( tmp ) ){
			*brc = *tmp;
			Get_Dte( dte );
			DBS_DTM_SET(brc->mdt, dte);
			DispData( brc, row );
			SetEditData( NULL );
			m_Brc_grd->SetSelectCell( -1, -1 );
		}
	}
}

// Add button clicked
void CBranchInfoDlg::OnBnClickedButtonAdd()
{
	int			row;
	Csr			csr;
	BRC_INF		tmp[1], *brc;

	memset( tmp, 0, sizeof(tmp) );

	if ( GetData( tmp ) ){
		DBS_WST_SET(tmp->opc, m_pPms_mng->opc );
		if ( csr = Add_Lnk( m_Brc_rot, tmp, sizeof(tmp) ) ){
			brc = Rec_Of(BRC_INF, csr);
			row = m_Brc_grd->GetLineCount( FALSE );
			m_Brc_grd->AddRow( 1 );
			DispData( brc, ++row );
			SetEditData( NULL );
			m_Brc_grd->SetSelectCell( -1, -1 );
		}
	}
}

// Deletebuton clicked
void CBranchInfoDlg::OnBnClickedButtonDelete()
{
	int			row;
	BRC_INF		*brc;
	Day			dte[1];

	if ( ( row = m_Brc_grd->GetSelRow() ) > 0 ){
		brc = (BRC_INF *)(LONG_PTR)m_Brc_grd->GetData( CLM_DAT, row );
		if ( MessageBox( _T("Are you sure to delete?"), NULL, MB_ICONQUESTION|MB_YESNO ) == IDYES ){
			if ( Nul_Fld(brc->id, FLD_INT) ){
				Del_Lnk( m_Brc_rot, Csr_Of(brc) );
			}
			else{
				Get_Dte( dte );
				DBS_DAY_SET(brc->ddt, dte);
			}
			m_Brc_grd->DelRow( row );
			m_Brc_grd->SetSelectCell(-1, -1);
			m_Brc_grd->Redraw();
		}
	}
}

void CBranchInfoDlg::OnBnClickedOk()
{
	if ( ConfirmSaveChaned( FALSE, FALSE )==IDYES ){
		OnOK();
	}
}

void CBranchInfoDlg::OnBnClickedCancel()
{
	if ( ConfirmSaveChaned( TRUE, TRUE )==IDYES ){
		OnCancel();
	}
}

BOOL CBranchInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}
