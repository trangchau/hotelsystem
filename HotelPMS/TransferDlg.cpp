// TransferDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "TransferDlg.h"
//#include "CashierDlg.h"
#include ".\fnc\csh_fnc.h"
#include ".\fnc\viw_cmn.h"


// CTransferDlg ダイアログ
static	WCHAR	*Grd_ttl[] =	{	{ _T("Sel")	},
									{ _T("Room")		},
									{ _T("Name")		}};

static	GRD_CLM	Grd_clm[] =		{	{  32, GAL_CTR, GCT_CHK,  12 },		// Select
									{  64, GAL_LFT, GCT_NML,  12 },		// Room number
									{ 256, GAL_LFT, GCT_NML,  12 } };	// Name

#define	IDC_GRD_DST			8192

#define	CLM_DAT				1

#define	CLM_SEL				0
#define	CLM_ROM				1
#define	CLM_NAM				2

IMPLEMENT_DYNAMIC(CTransferDlg, CDialog)

CTransferDlg::CTransferDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransferDlg::IDD, pParent)
{
	m_Dst_grd = NULL;
	m_Dst_rot = NULL;
	m_Inh_dat = NULL;
}

CTransferDlg::~CTransferDlg()
{
	if ( m_Dst_grd )	delete	m_Dst_grd;
}

void CTransferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTransferDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_BN_CLICKED(IDCANCEL, &CTransferDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CTransferDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CTransferDlg::SetupComponent()
{
	int				idx;
	GRD_DEF			gdf[1];
	WINDOWPLACEMENT	wpc[1];

	//struct	{
	//	CWnd	*wnd;
	//	int		lmt;
	//	DWORD	cid;
	//}Cpn_[] = {
	//	{ m_Edt_csh,  14, IDC_EDIT_CASH			},
	//	{ m_Edt_cdt,  14, IDC_EDIT_CREDITCARD	},
	//	{ m_Edt_cpn,  14, IDC_EDIT_COUPON		},
	//	{ m_Edt_otr,  14, IDC_EDIT_OTHER		}
	//};

	//for ( idx=0; idx<memcnt(Cpn_); idx++ ){
	//	if ( !!Cpn_[idx].wnd ){
	//		Cpn_[idx].wnd->SubclassDlgItem(Cpn_[idx].cid, this);
	//	}
	//	if ( Cpn_[idx].lmt!=-1 )	((CEdit *)GetDlgItem(Cpn_[idx].cid))->SetLimitText( Cpn_[idx].lmt );
	//}

	memset( gdf, 0, sizeof(gdf) );

	gdf->lnh = 20;
	gdf->vlc = 8;
	gdf->vcc = memcnt(Grd_clm);
	gdf->hbc = RGB(128,255,255);
	gdf->hfc = RGB(0,0,0);
	gdf->dbc = RGB(255,255,255);
	gdf->dfc = RGB(0,0,0);
	gdf->sbc = RGB(0,0,0);
	gdf->sfc = RGB(255,255,255);
	gdf->cct = gdf->vcc;
	gdf->clm = Grd_clm;
	gdf->sel = GSL_NON;//id==IDC_GRD_CSH ? GSL_ROW: GSL_NON;
	gdf->hdr = TRUE;
	gdf->fsa = TRUE;

	GetDlgItem(IDC_STATIC_MESSAGE)->GetWindowPlacement( wpc );
	wpc->rcNormalPosition.top += 12;
	m_Dst_grd = new CSimpleGrid();
	m_Dst_grd->Create( gdf, wpc->rcNormalPosition, IDC_GRD_DST, this );
	for ( idx=0; idx<memcnt(Grd_ttl); idx++ ){
		m_Dst_grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}
}

// CTransferDlg メッセージ ハンドラ

BOOL CTransferDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetupComponent();
	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// Notify from grid
LRESULT CTransferDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;
	int				chk, row;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:	if ( ntf->row > 0 ){
									for ( row=1; row<=m_Dst_grd->GetLineCount(FALSE); row++ ){
										chk = row==ntf->row && !m_Dst_grd->GetCheck(CLM_SEL, row);
										m_Dst_grd->SetCheck( CLM_SEL, row, chk );
									}
								}
								break;
		case GN_QUERYINPUTTYPE:	if ( ntf->clm==CLM_SEL )	rsl = GCT_CHK;
								else						rsl = GCT_NML;
								break;
		}
		free( ntf );
	}

	return( rsl );
}

// close button click
void CTransferDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OnCancel();
}

// close with reflrct button click
void CTransferDlg::OnBnClickedOk()
{
	int			row;

	for ( row=1; row<=m_Dst_grd->GetLineCount(FALSE); row++ ){
		if ( !!m_Dst_grd->GetCheck(CLM_SEL, row) ){
			m_Inh_dat = (CSH_INH_DAT *)(LONG_PTR)m_Dst_grd->GetData( CLM_DAT, row );
			break;
		}
	}
	if ( row<=m_Dst_grd->GetLineCount(FALSE) ){
		OnOK();
	}
	else{
		MessageBox( _T("Destination room is not selected."), NULL, MB_ICONEXCLAMATION|MB_OK );
	}
}

// operation
// setting initial data
void CTransferDlg::InitData()
{
	int				row=0;
	WCHAR			buf[530];
	Csr				csr=NULL;
	CSH_INH_DAT		*inh;

	m_Dst_grd->ResetContent();
	m_Dst_grd->AddRow( Cnt_Lnk( m_Dst_rot ) );
	while ( csr=Nxt_Lnk( m_Dst_rot, csr ) ){
		inh = *Rec_Of(CSH_INH_DAT *, csr);
		m_Dst_grd->SetCellText( CLM_ROM, ++row, EDT_DBF_WST(inh->rmn, _T("")) );
		swprintf_s( buf, memcnt(buf), _T("%s %s"), EDT_DBF_WST(inh->fsn, _T("")), EDT_DBF_WST(inh->lsn, _T("")) );
		m_Dst_grd->SetCellText( CLM_NAM, row, buf );
		m_Dst_grd->SetData( CLM_DAT, row, (int)(LONG_PTR)inh );
	}
}

BOOL CTransferDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}
