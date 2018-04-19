// ClientSearchDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MainFrm.h"
#include "ClientSearchDlg.h"

#include ".\fnc\cli_sql.h"
//#include ".\fnc\mfm_sql.h"
#include ".\fnc\sql_cmn.h"
#include ".\fnc\viw_cmn.h"


// CClientSearchDlg ダイアログ

static	WCHAR	*Grd_ttl[] =	{	{ _T("Name")		},
									{ _T("Gender")		},
									{ _T("Tel1")	},
									{ _T("Tel2")	},
									{ _T("Email1")},
									{ _T("Email2")},
									{ _T("LastVisited")	},
									{ _T("Nationality")	},
									/*{ _T("Rate")		}*/};

static	GRD_CLM	Grd_clm[] =		{	{ 192, GAL_LFT, GCT_NML,  12 },		// Name
									{  48, GAL_LFT, GCT_NML,  12 },		// Gender
									{ 128, GAL_LFT, GCT_NML,  12 },		// Telephone1
									{ 128, GAL_LFT, GCT_NML,  12 },		// Telephone2
									{ 128, GAL_LFT, GCT_NML,  12 },		// EmailAddress1
									{ 128, GAL_LFT, GCT_NML,  12 },		// EmailAddress2
									{ 180, GAL_LFT, GCT_NML,  12 },		// LastVisit
									{  96, GAL_LFT, GCT_NML,  12 },		// Nationality
									/*{  96, GAL_CTR, GCT_NML,  12 }*/};// Rate

#define	IDC_GRD_CLI		(8192)	// Copy destination selecting grid

#define	CLM_DAT			0		// Data keeping column

#define	CLM_NAM			0		// Name
#define	CLM_GDR			1		// Gender
#define	CLM_TL1			3		// Telephone1
#define	CLM_TL2			4		// Telephone2
#define	CLM_EM1			5		// EmailAddress1
#define	CLM_EM2			6		// EmailAddress2
#define	CLM_LVT			7		// LastVisit
#define	CLM_NTO			8		// Nationality


IMPLEMENT_DYNAMIC(CClientSearchDlg, CDialog)

CClientSearchDlg::CClientSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientSearchDlg::IDD, pParent)
{
	m_pPms_mng = NULL;
	m_pDbs_obj = NULL;
	m_Ope_cod = NULL;
	m_Cli_grd = NULL;
	m_Sel_row = -1;
	memset( m_Cli_rot, 0, sizeof(m_Cli_rot) );
}

CClientSearchDlg::~CClientSearchDlg()
{
	if ( m_Cli_grd )	delete m_Cli_grd;
	Del_All( m_Cli_rot );
}

void CClientSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClientSearchDlg, CDialog)
	ON_WM_TIMER()
	ON_WM_GRIDNOTIFY()
	ON_BN_CLICKED(IDCANCEL, &CClientSearchDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CClientSearchDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CClientSearchDlg::OnBnClickedButtonSearch)
END_MESSAGE_MAP()


// CClientSearchDlg メッセージ ハンドラ

void CClientSearchDlg::SetupComponent()
{
	int				idx;
	WINDOWPLACEMENT	wpc[1];
	GRD_DEF			gdf[1];

	struct	{
		CWnd	*wnd;
		int		lmt;
		DWORD	cid;
	}Cpn_[] = {
		{ NULL,      255, IDC_EDIT_NAME			},
		{ NULL,       40, IDC_EDIT_TELEPHONE	},
		{ NULL,       40, IDC_EDIT_EMAILADDRESS	}
	};

	for ( idx=0; idx<memcnt(Cpn_); idx++ ){
		if ( !!Cpn_[idx].wnd ){
			Cpn_[idx].wnd->SubclassDlgItem(Cpn_[idx].cid, this);
		}
		if ( Cpn_[idx].lmt!=-1 )	((CEdit *)GetDlgItem(Cpn_[idx].cid))->SetLimitText( Cpn_[idx].lmt );
	}

	memset( gdf, 0, sizeof(gdf) );

	GetDlgItem(IDC_EDIT_NAME)->GetWindowPlacement( wpc );
	wpc->rcNormalPosition.top += 30;

	gdf->lnh = 20;
	gdf->vlc = 20;
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

	m_Cli_grd = new CSimpleGrid();
	m_Cli_grd->Create( gdf, wpc->rcNormalPosition, IDC_GRD_CLI, this );
	for ( idx=0; idx<memcnt(Grd_ttl); idx++ ){
		m_Cli_grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}
}

void	CClientSearchDlg::DispData( CLI_INF *cli, int row )
{
	WCHAR		*knd=_T("Unknown"), buf[520], *nam;

	swprintf_s( buf, memcnt(buf), _T("%s %s"), EDT_DBF_WST(cli->fnm, _T("")), EDT_DBF_WST(cli->lnm, _T("")) );
	m_Cli_grd->SetCellText( CLM_NAM, row, buf );
	nam = !Nul_Fld(cli->gdr, FLD_INT) ? Viw_Get_Gdr( DBS_INT_VAL(cli->gdr) ): NULL;
	m_Cli_grd->SetCellText( CLM_GDR, row, nam ? nam: _T("Unknown") );
	m_Cli_grd->SetCellText( CLM_TL1, row, EDT_DBF_WST(cli->tel1, _T("")) );
	m_Cli_grd->SetCellText( CLM_TL2, row, EDT_DBF_WST(cli->tel2, _T("")) );
	m_Cli_grd->SetCellText( CLM_EM1, row, EDT_DBF_WST(cli->eml1, _T("")) );
	m_Cli_grd->SetCellText( CLM_EM2, row, EDT_DBF_WST(cli->eml2, _T("")) );
//#define	CLM_LVT			4		// LastVisit
//#define	CLM_NTO			5		// Nationality

	m_Cli_grd->SetData( CLM_DAT, row, (int)(LONG_PTR)cli );
}

void CClientSearchDlg::SearchExecute()
{
	int			row=1;
	WCHAR		buf[512], fnm[256], lnm[256], tel[128], eml[128], *erm=NULL, *ptr;
	Csr			csr=NULL;
	CLI_INF		*cli;

	Del_All( m_Cli_rot );

	memset( fnm, 0, sizeof(fnm) );
	memset( lnm, 0, sizeof(lnm) );

	GetDlgItem(IDC_EDIT_NAME)->GetWindowText( buf, memcnt(buf) );
	if ( ptr = wcschr( buf, _T(' ') ) ){
		*ptr++ = 0;
	}
	wcscpy_s( fnm, memcnt(fnm), buf );
	if ( ptr )	wcscpy_s( lnm, memcnt(lnm), ptr );

	GetDlgItem(IDC_EDIT_TELEPHONE)->GetWindowText( tel, memcnt(tel) );
	GetDlgItem(IDC_EDIT_EMAILADDRESS)->GetWindowText( eml, memcnt(eml) );
		
	m_Cli_grd->ResetContent();

	if ( Cli_Get_Inf( m_Cli_rot, fnm, lnm, tel, eml, &erm, m_pDbs_obj, m_pPms_mng ) ){
		m_Cli_grd->AddRow( Cnt_Lnk(m_Cli_rot) );
		while( csr=Nxt_Lnk( m_Cli_rot, csr ) ){
			cli = Rec_Of(CLI_INF, csr);
			DispData( cli, row++ );
		}
		m_Cli_grd->Redraw();
	}
	else{
		if ( erm ){
			MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
	}
}

BOOL CClientSearchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetupComponent();

	SetTimer( 1, 100, NULL );
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CClientSearchDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer( nIDEvent );
	WCHAR			buf[520];
	int				exc;
	*buf = 0;

	if ( exc = !!wcslen(trim(m_Fst_nam)) ){
		wcscat_s( buf, memcnt(buf), m_Fst_nam );
		if ( !!wcslen(trim(m_Lst_nam)) )	wcscat_s( buf, memcnt(buf), _T(" ") );
	}
	if ( exc |= !!wcslen(trim(m_Lst_nam)) ){
		wcscat_s( buf, memcnt(buf), m_Lst_nam );
	}

	if ( exc ){
		GetDlgItem(IDC_EDIT_NAME)->SetWindowTextW( buf );
		SearchExecute();
	}

	CDialog::OnTimer(nIDEvent);
}

// search button click
void CClientSearchDlg::OnBnClickedButtonSearch()
{
		SearchExecute();
}

// グリッドイベント
LRESULT CClientSearchDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;
	CLI_INF			*cli;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:	m_Sel_row = ntf->row > 0 ? ntf->row: -1;
								break;
		case GN_LBUTTONDBLCLICK:if ( ntf->row > 0 ){
									*m_Cli_inf = *((CLI_INF *)(LONG_PTR)m_Cli_grd->GetData( CLM_DAT, ntf->row ));
									OnOK();									
								}
								break;
		}
		free( ntf );
	}

	return( rsl );
}

void CClientSearchDlg::OnBnClickedOk()
{
	if ( m_Sel_row != -1 ){
		*m_Cli_inf = *((CLI_INF *)(LONG_PTR)m_Cli_grd->GetData( CLM_DAT, m_Sel_row ));
		OnOK();
	}
	else{
		MessageBox( _T("Target is not selected."), NULL, MB_ICONEXCLAMATION|MB_OK );
	}
}

void CClientSearchDlg::OnBnClickedCancel()
{
	OnCancel();
}

BOOL CClientSearchDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}
