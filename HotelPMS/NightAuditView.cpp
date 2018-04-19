// NightAuditView.cpp : CNightAuditView クラスの実装
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#include "NightAuditDoc.h"
#include "NightAuditView.h"
#include ".\fnc\nad_fnc.h"
#include ".\fnc\viw_cmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNightAuditView

static	WCHAR	*Grd_ttl[] =	{	{ _T("Status")		},
									{ _T("Rsv.No")		},
									{ _T("Ext.Rsv.No")	},
									{ _T("Room")		},
									{ _T("GuestName")	},
									{ _T("ReservName")	},
									/*{ _T("ReservName")	}*/};

static	GRD_CLM	Grd_clm[] =		{	{  96, GAL_LFT, GCT_NML,  12 },		// Currency
									{  96, GAL_LFT, GCT_NML,  12 },		// Reservation number
									{ 112, GAL_LFT, GCT_NML,  12 },		// External reservation number
									{  96, GAL_LFT, GCT_NML,  12 },		// room number
									{ 192, GAL_LFT, GCT_NML,  12 },		// Guest name
									{ 192, GAL_LFT, GCT_NML,  12 },		// Reservation name
									/*{ 128, GAL_LFT, GCT_NML,  14 },*/};	// Other


IMPLEMENT_DYNCREATE(CNightAuditView, CFormView)

BEGIN_MESSAGE_MAP(CNightAuditView, CFormView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_BN_CLICKED(IDCANCEL, &CNightAuditView::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CNightAuditView::OnBnClickedOk)
	ON_WM_GRIDNOTIFY()
	ON_WM_VIEWNOTIFY()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


void CNightAuditView::SetupComponent()
{
	int				idx;
	WINDOWPLACEMENT	wpc[1];
	//RECT			rct[1];
	GRD_DEF			gdf[1];

	struct	{
		CWnd	*wnd;
		int		lmt;
		DWORD	cid;
	}Cpn_[] = {
		{ m_Biz_dte,  -1, IDC_STATIC_BUSINESSDATE }};

	for ( idx=0; idx<memcnt(Cpn_); idx++ ){
		if ( !!Cpn_[idx].wnd ){
			Cpn_[idx].wnd->SubclassDlgItem(Cpn_[idx].cid, this);
		}
		if ( Cpn_[idx].lmt!=-1 )	((CEdit *)GetDlgItem(Cpn_[idx].cid))->SetLimitText( Cpn_[idx].lmt );
	}

	m_Biz_dte->SetFont( _T("MS UI Gothic"), 18, RGB(0,0,0) );

	memset( gdf, 0, sizeof(gdf) );

	GetDlgItem(IDC_STATIC_BUSINESSDATE)->GetWindowPlacement( wpc );
	wpc->rcNormalPosition.top += 30;

	gdf->lnh = 20;
	gdf->vlc = 9;
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

	m_Wrn_grd = new CSimpleGrid();
	m_Wrn_grd->Create( gdf, wpc->rcNormalPosition, IDC_GRD_WRN, this );
	for ( idx=0; idx<memcnt(Grd_ttl); idx++ ){
		m_Wrn_grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}
}

int	 CNightAuditView::InitData( WCHAR *opc, WCHAR *bcd, WCHAR *fcd )
{
	WCHAR			buf[30];

	m_Ope_cod = opc;
	m_Brc_cod = bcd;
	m_Fcl_cod = fcd;

	m_Biz_dte->SetWindowText( Edt_Dte(buf, memcnt(buf), m_pPms_mng->bdt ) );

	return( Nad_Get_Dat( this, m_pDbs_obj, m_pPms_mng ) );
}

// CNightAuditView コンストラクション/デストラクション

CNightAuditView::CNightAuditView(): CFormView(CNightAuditView::IDD)
{
	m_Wrn_grd = NULL;
	m_pPms_mng = NULL;
	m_pDbs_obj = NULL;
	m_Ope_cod = NULL;
	m_Rdn_rii = 0;
	m_Rdn_rdi = 0;
	m_Rdn_wrn = NULL;
	memset( m_Wrn_rot, 0, sizeof(m_Wrn_rot) );
}

CNightAuditView::~CNightAuditView()
{
	if ( m_Wrn_grd )	delete m_Wrn_grd;
	Del_All( m_Wrn_rot );
}

BOOL CNightAuditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CNightAuditView 描画

void CNightAuditView::OnDraw(CDC* /*pDC*/)
{
	CNightAuditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}


// CNightAuditView 印刷

BOOL CNightAuditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CNightAuditView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CNightAuditView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}


// CNightAuditView 診断

#ifdef _DEBUG
void CNightAuditView::AssertValid() const
{
	CView::AssertValid();
}

void CNightAuditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNightAuditDoc* CNightAuditView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNightAuditDoc)));
	return (CNightAuditDoc*)m_pDocument;
}
#endif //_DEBUG


// CNightAuditView メッセージ ハンドラ

void CNightAuditView::OnInitialUpdate()
{
	((CChildFrame *)GetParent())->m_Nsz_viw = TRUE;	// Non sizable
	((CChildFrame *)GetParent())->m_Viw_knd = VWK_NAD;

	CView::OnInitialUpdate();

	// ダイアログリソースのサイズにフィットさせる
	ResizeParentToFit(false);
	GetMainFrm()->m_Nad_viw = this;

	Viw_Clc_Dfs(((CChildFrame *)GetParent())->m_Win_siz, this);
	
	((CChildFrame *)GetParent())->m_Ini_upd = TRUE;

	m_pPms_mng = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->m_Dbs_obj;

	//m_Rsv_nam.SubclassDlgItem(IDC_STATIC_BOOKINGNAME, this );
	//m_Rsv_nam.SetFont(_T("MS UI Gothic"), 24, RGB(0,0,0) );

	SetupComponent();
//	SetSize( FALSE );
}

// グリッドイベント
LRESULT CNightAuditView::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;
	//int				row;
	//double			fvl;
	//WCHAR			*txt;
	//CRC_CVS			*ccv;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_RBUTTONCLICK:	if ( ntf->row > 0 ){
									Nad_Ctx_Mnu( this, ntf->msx, ntf->msy, ntf->row, ntf->cid );
								}
								break;
		//case GN_EDITCOMPLETE:	txt = (WCHAR *)lParam;
		//						if ( ccv = (CRC_CVS *)(LONG_PTR)m_Ccv_grd->GetData( CLM_DAT, ntf->row ) ){
		//							fvl = _wtof( txt );
		//							*DBS_DBL_PTR(ccv->rte) = fvl;
		//							Set_Nul( ccv->mdt, FLD_DTM, FALSE );
		//							m_Ccv_grd->SetCellText( ntf->clm, ntf->row, txt );
		//						}
		//						free( txt );
		//						break;
		//case GN_QUERYINPUTTYPE:	rsl = ntf->clm==CLM_RTE ? GCT_EDT: GCT_NML;
		//						break;
		}
		free( ntf );
	}

	return( rsl );
}

// Notify from each view
LRESULT CNightAuditView::OnViewNotify( WPARAM wParam, LPARAM lParam )
{
	VIW_NTF			*ntf = (VIW_NTF *)wParam;

	if ( ntf ){
		switch( ntf->ntf ){
		case VWN_RSV_CREATED:	
		case VWN_RSV_CHANGED:	Nad_Get_Dat( this, m_pDbs_obj, m_pPms_mng );
								break;
		case VWN_RSV_DELETED:	break;
		}
		free( ntf );
	}
	return( 0 );
}

// Windows command operation
BOOL CNightAuditView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (pHandlerInfo == NULL){
		if (nCode == CN_COMMAND){
			switch ( nID ){
			case ID_NIGHTAUDIT_CHECKIN:				Nad_Rnc_Viw( VWK_CIN, m_Rdn_rii, m_Rdn_rdi );
													break;
			case ID_NIGHTAUDIT_CHECKOUT:			Nad_Rnc_Viw( VWK_COT, m_Rdn_rii, m_Rdn_rdi );
													break;
			case ID_NIGHTAUDIT_NOSHOW:				Nad_Mak_Nsw( m_Rdn_rii, m_Rdn_rdi, this, m_pDbs_obj, m_pPms_mng );
													break;
			case ID_NIGHTAUDIT_RESERVATION:			Nad_Rnc_Viw( VWK_RSV, m_Rdn_rii, m_Rdn_rdi );
													break;
			case ID_NIGHTAUDIT_CANCELRESERVATION:	Nad_Cxl_Rsv( m_Rdn_rii, m_Rdn_rdi, this, m_pDbs_obj, m_pPms_mng );
													break;
			}
		}
	}

	return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// OK Button Click
void CNightAuditView::OnBnClickedOk()
{
	if ( MessageBox( _T("Are you sure to start the day closing process?"), NULL, MB_ICONQUESTION|MB_YESNO )==IDYES ){
		Nad_Day_Cls( this, m_pDbs_obj, m_pPms_mng );
	}
	GetDocument()->OnCloseDocument();
}

// Cancel button click
void CNightAuditView::OnBnClickedCancel()
{
	GetDocument()->OnCloseDocument();
}

// On Close the window
void CNightAuditView::OnClose()
{

	CFormView::OnClose();
}

// No Clienr area dispose
void CNightAuditView::PostNcDestroy()
{
	CFormView::PostNcDestroy();
}

BOOL CNightAuditView::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CFormView::PreTranslateMessage(pMsg): FALSE );
}
