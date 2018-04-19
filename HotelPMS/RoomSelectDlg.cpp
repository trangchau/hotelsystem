// RoomSelectDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MainFrm.h"
#include "RoomSelectDlg.h"


// CRoomSelectDlg ダイアログ

static	WCHAR	*Grd_ttl[] =	{	{ _T("Sel")			},
									{ _T("RoomNumber")	},
									{ _T("RoomType")	},
									{ _T("Smk")			},
									{ _T("Wing")		},
									{ _T("Face")		},
									{ _T("View")		}};

static	GRD_CLM	Grd_clm[] =		{	{  32, GAL_CTR, GCT_CHK,  12 },		// Sel
									{  96, GAL_LFT, GCT_NML,  12 },		// RoomNumber
									{ 144, GAL_LFT, GCT_NML,  12 },		// RoomType
									{  32, GAL_CTR, GCT_CHK,  12 },		// Smoke
									{ 144, GAL_LFT, GCT_NML,  12 },		// Wing
									{ 144, GAL_LFT, GCT_NML,  12 },		// Face
									{ 144, GAL_LFT, GCT_NML,  12 } };	// View

#define	IDC_GRD_ROM			8192

#define	RSL_CLM_DAT			1

#define	RSL_CLM_SEL			0
#define	RSL_CLM_NBR			1
#define	RSL_CLM_TYP			2
#define	RSL_CLM_SMK			3
#define	RSL_CLM_WNG			4
#define	RSL_CLM_FCE			5
#define	RSL_CLM_VIW			6

IMPLEMENT_DYNAMIC(CRoomSelectDlg, CDialog)

static	WCHAR	*adk_nam(			// Additional kind name
EQP_ADT	*adt,						// additional kind list
int		cnt,						// additional kind list count
int		apl )						// application bits
{
	int			idx;
	WCHAR		*nam=_T("");

	for ( idx=0; idx<cnt; idx++ ){
		if ( !!( (adt+idx)->cod & apl ) ){
			nam = (adt+idx)->nam;
			break;
		}
	}
	return( nam );
}

CRoomSelectDlg::CRoomSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRoomSelectDlg::IDD, pParent)
{
	m_Rom_grd = NULL;
	m_Bok_pth = 0;
	m_Agt_iid = 0;
	m_Req_eti = 0;
	m_Eqp_tid = 0;
	m_Rsv_did = 0;
	m_Adk_wng = NULL;
	m_Adk_fce = NULL;
	m_Adk_viw = NULL;
	m_Adc_wng = 0;
	m_Adc_fce = 0;
	m_Adc_viw = 0;
	m_Equ_iid = 0;
	m_Eqp_sid = 0;
	m_Use_std = NULL;
	m_Use_edd = NULL;
	memset( m_Rom_rot, 0, sizeof(m_Rom_rot) );
	memset( m_Pln_nam, 0, sizeof(m_Pln_nam) );
	memset( m_Eqp_inf, 0, sizeof(m_Eqp_inf) );
}

CRoomSelectDlg::~CRoomSelectDlg()
{
	if ( m_Rom_grd )	delete	m_Rom_grd;
	Del_All( m_Rom_rot );
}

void CRoomSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


void CRoomSelectDlg::SetupComponent()
{
	int				idx;
	WINDOWPLACEMENT	wpc[1];
	GRD_DEF			gdf[1];

	struct	{
		CWnd	*wnd;
		int		lmt;
		DWORD	cid;
	}Cpn_[] = {
		{ m_Cmb_bph,   -1, IDC_COMBO_BOOKINGPATH	}
	};

	for ( idx=0; idx<memcnt(Cpn_); idx++ ){
		Cpn_[idx].wnd->SubclassDlgItem(Cpn_[idx].cid, this);
		if ( Cpn_[idx].lmt!=-1 )	((CEdit *)Cpn_[idx].wnd)->SetLimitText( Cpn_[idx].lmt );
	}

	memset( gdf, 0, sizeof(gdf) );

	GetDlgItem(IDC_COMBO_ROOMTYPE)->GetWindowPlacement( wpc );
	wpc->rcNormalPosition.top += 30;

	gdf->lnh = 20;
	gdf->vlc = 12;
	gdf->vcc = memcnt(Grd_clm);
	gdf->hbc = RGB(128,255,255);
	gdf->hfc = RGB(0,0,0);
	gdf->dbc = RGB(255,255,255);
	gdf->dfc = RGB(0,0,0);
	gdf->sbc = RGB(0,0,0);
	gdf->sfc = RGB(255,255,255);
	gdf->cct = gdf->vcc;
	gdf->clm = Grd_clm;
	gdf->sel = GSL_NON;
	gdf->hdr = TRUE;
	gdf->fsa = TRUE;

	m_Rom_grd = new CSimpleGrid();
	m_Rom_grd->Create( gdf, wpc->rcNormalPosition, IDC_GRD_ROM, this );
	for ( idx=0; idx<memcnt(Grd_ttl); idx++ ){
		m_Rom_grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}
	GetDlgItem(IDC_COMBO_BOOKINGPATH)->ShowWindow(SW_HIDE);
}

void CRoomSelectDlg::DispRoom(		// displaying 1 room data
CSimpleGrid	*grd,					// target grid
RSV_EQP_INF	*eqi,					// available room data
int			row,					// row
int			eii )					// selected room information
{
	grd->SetCheck( RSL_CLM_SEL, row, eii==DBS_INT_VAL(eqi->eqi->id) );
	grd->SetCellText( RSL_CLM_NBR, row, EDT_DBF_WST(eqi->eqi->eco, _T("")) ); 
	grd->SetCellText( RSL_CLM_TYP, row, EDT_DBF_WST(eqi->tnm, _T("")) );
	grd->SetCheck( RSL_CLM_SMK, row, !!(DBS_INT_VAL(eqi->eqi->adi2) & EQA_SMOKING) );
	grd->SetCellText( RSL_CLM_WNG, row, adk_nam(m_Adk_wng, m_Adc_wng, DBS_INT_VAL(eqi->eqi->adi1) ) );
	grd->SetCellText( RSL_CLM_FCE, row, adk_nam(m_Adk_fce, m_Adc_fce, DBS_INT_VAL(eqi->eqi->adi1) ) );
	grd->SetCellText( RSL_CLM_VIW, row, adk_nam(m_Adk_viw, m_Adc_viw, DBS_INT_VAL(eqi->eqi->adi1) ) );
}

void CRoomSelectDlg::DispRoomList(	// displaying available room list
CSimpleGrid	*grd,					// target grid
Csr			rot,					// root of available room list
int			eii )					// selected room information
{
	int			row=0;
	Csr			csr=NULL;
	RSV_EQP_INF	*eqi;

	grd->ResetContent();
	grd->AddRow( Cnt_Lnk( rot ) );
	while ( csr=Nxt_Lnk( rot, csr ) ){
		eqi = Rec_Of(RSV_EQP_INF, csr);
		DispRoom( grd, eqi, ++row, eii );
		grd->SetData( RSL_CLM_DAT, row, (int)(LONG_PTR)eqi );
	}
	grd->InputEnable( TRUE );
}


BEGIN_MESSAGE_MAP(CRoomSelectDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_CBN_SELCHANGE(IDC_COMBO_ROOMTYPE, &CRoomSelectDlg::OnCbnSelchangeComboRoomtype)
	ON_BN_CLICKED(IDCANCEL, &CRoomSelectDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CRoomSelectDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRoomSelectDlg メッセージ ハンドラ

BOOL CRoomSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pPms_mng = ((CMainFrame *)GetParent())->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)GetParent())->m_Dbs_obj;

	SetupComponent();
	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// Notify from grid
LRESULT CRoomSelectDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	int				chk, row;
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:	if ( ntf->row > 0 ){
									chk = m_Rom_grd->GetCheck( RSL_CLM_SEL, ntf->row );
									for ( row=1; row<=m_Rom_grd->GetLineCount(FALSE); row++ ){
										m_Rom_grd->SetCheck( RSL_CLM_SEL, row, row==ntf->row && chk );
									}
								}
								break;
		case GN_QUERYINPUTTYPE:	if ( ntf->clm==RSL_CLM_SEL )	rsl=GCT_CHK;
								else							rsl=GCT_NML;
								break;
		}
		free( ntf );
	}

	return( rsl );
}

// room type combobox change 
void CRoomSelectDlg::OnCbnSelchangeComboRoomtype()
{
	int			idx, eti=0;
	WCHAR		*erm=NULL;
	CComboBox	*cbx = (CComboBox *)GetDlgItem(IDC_COMBO_ROOMTYPE);

	idx = cbx->GetCurSel();
	if ( idx > 0 )	eti = DBS_INT_VAL(((EQP_TYP *)(LONG_PTR)cbx->GetItemData(idx))->id);
	if ( Rsv_Avl_Rms( m_Rom_rot, eti, m_Rsv_did, m_Use_std, m_Use_edd, &erm, m_pDbs_obj, m_pPms_mng ) ){
		DispRoomList( m_Rom_grd, m_Rom_rot, m_Equ_iid );
	}
	else{
		if ( erm ){
			MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
	}
}

// Close button click
void CRoomSelectDlg::OnBnClickedCancel()
{
	OnCancel();
}

// Close with reflect button click
void CRoomSelectDlg::OnBnClickedOk()
{
	int			row, fnd=FALSE;

	for ( row=1; row<=m_Rom_grd->GetLineCount(FALSE); row++ ){
		if ( fnd = m_Rom_grd->GetCheck( RSL_CLM_SEL, row ) ){
			*m_Eqp_inf = *((RSV_EQP_INF *)(LONG_PTR)m_Rom_grd->GetData( RSL_CLM_DAT, row ));
			break;
		}
	}
	if ( fnd ){
		OnOK();
	}
	else{
		MessageBox( _T("Room is note select."), NULL, MB_ICONEXCLAMATION|MB_OK );
	}
}

// operations
// InitData
void CRoomSelectDlg::InitData()
{
	int			idx;
	WCHAR		*ptr;
	Csr			csr;
	EQP_TYP		*eqt;
	CComboBox	*cbx = (CComboBox *)GetDlgItem(IDC_COMBO_ROOMTYPE);

	m_Cmb_bph->SetAgentData( m_pPms_mng->agl );

	GetDlgItem(IDC_STATIC_RATEPLAN)->SetWindowText( m_Pln_nam );

	// booking path name
	ptr = m_Cmb_bph->GetPathName( m_Bok_pth, m_Agt_iid );
	GetDlgItem(IDC_STATIC_BOOKINGPATH)->SetWindowText( ptr ? ptr: _T("") );

	// requested room type name and toom type cdombobox
	cbx->InsertString( -1, _T("All") );
	for( csr=NULL, ptr=_T(""); csr = Nxt_Lnk( m_pPms_mng->etl, csr ); ){
		eqt = Rec_Of(EQP_TYP, csr);
		cbx->SetItemData( idx = cbx->InsertString( -1, EDT_DBF_WST(eqt->nam, _T("")) ), (LONG_PTR)eqt );
		if ( DBS_INT_VAL(eqt->id) == m_Req_eti ){
			ptr = EDT_DBF_WST(eqt->nam, _T(""));
			// set roomtype if room has not dselected yet
			if ( !m_Eqp_tid )	cbx->SetCurSel(idx);
		}
		// set roomtype if room has already dselected
		if ( !!m_Eqp_tid && m_Eqp_tid==DBS_INT_VAL(eqt->id) )	cbx->SetCurSel(idx);
	}
	GetDlgItem(IDC_STATIC_REQUESTEDROOMTYPE)->SetWindowText( ptr ? ptr: _T("") );

	m_Adc_wng = Viw_Get_Eqa( &m_Adk_wng, ADK_WNG );
	m_Adc_fce = Viw_Get_Eqa( &m_Adk_fce, ADK_FCE );
	m_Adc_viw = Viw_Get_Eqa( &m_Adk_viw, ADK_VIW );
	OnCbnSelchangeComboRoomtype();
}
