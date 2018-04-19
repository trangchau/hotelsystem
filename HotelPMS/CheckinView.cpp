// HotelPMSView.cpp : CCheckinView クラスの実装
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#include "CheckinDoc.h"
#include "CheckinView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCheckinView

IMPLEMENT_DYNCREATE(CCheckinView, CFormView)

BEGIN_MESSAGE_MAP(CCheckinView, CFormView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_BN_CLICKED(IDCANCEL, &CCheckinView::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CCheckinView::OnBnClickedOk)
	ON_WM_GRIDNOTIFY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_CHECKALL, &CCheckinView::OnBnClickedCheckCheckall)
	ON_BN_CLICKED(IDC_BUTTON_EXECCHECKIN, &CCheckinView::OnBnClickedButtonExeccheckin)
END_MESSAGE_MAP()

// CCheckinView コンストラクション/デストラクション

void CCheckinView::SetupComponent()
{
	int				idx;
	WCHAR			*fnm = _T("MS UI Gothic");
	WINDOWPLACEMENT	wpc[1];

	struct	{
		CWnd	*wnd;
		int		lmt;
		DWORD	cid;
	}Cpn_[] = {
		{ m_Cli_typ,   -1, IDC_STATIC_CLIENTTYPE		},
		{ m_Rsv_nam,   -1, IDC_STATIC_BOOKINGNAME		}
	};

	for ( idx=0; idx<memcnt(Cpn_); idx++ ){
		Cpn_[idx].wnd->SubclassDlgItem(Cpn_[idx].cid, this);
		if ( Cpn_[idx].lmt!=-1 )	((CEdit *)Cpn_[idx].wnd)->SetLimitText( Cpn_[idx].lmt );
	}

	// Font setting
	m_Cli_typ->SetFont( fnm, 18, RGB(0,0,0) );
	m_Rsv_nam->SetFont( fnm, 24, RGB(0,0,0) );

	// BKColor setting(Required item)
//	m_Fst_nam->SetBGColor( REQ_COL );
	m_Cli_typ->AutoSize = TRUE;

	// checkin_details Grid
	GetDlgItem(IDC_STATIC_CINLIST)->GetWindowPlacement( wpc );
	m_Cin_grd = Cin_Mak_Grd( IDC_GRD_CIN, &wpc->rcNormalPosition, this );
	m_Cin_grd->InputEnable( TRUE );
}

CCheckinView::CCheckinView(): CFormView(CCheckinView::IDD)
{
	m_Cin_grd = NULL;
	m_Sel_row = -1;
	m_Rdn_row = 0;
	memset( m_Cin_inf, 0, sizeof(m_Cin_inf) );
}

CCheckinView::~CCheckinView()
{
	Cin_Dps_Cin( m_Cin_inf );
	if ( !!m_Cin_grd )	delete m_Cin_grd;
}

BOOL CCheckinView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CCheckinView 描画

void CCheckinView::OnDraw(CDC* /*pDC*/)
{
	CCheckinDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}


// CCheckinView 印刷

BOOL CCheckinView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CCheckinView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CCheckinView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}


// CCheckinView 診断

#ifdef _DEBUG
void CCheckinView::AssertValid() const
{
	CView::AssertValid();
}

void CCheckinView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCheckinDoc* CCheckinView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCheckinDoc)));
	return (CCheckinDoc*)m_pDocument;
}
#endif //_DEBUG


// CCheckinView メッセージ ハンドラ

void CCheckinView::OnInitialUpdate()
{
	((CChildFrame *)GetParent())->m_Nsz_viw = TRUE;	// Non sizable
	((CChildFrame *)GetParent())->m_Viw_knd = VWK_CIN;

	CView::OnInitialUpdate();

	// ダイアログリソースのサイズにフィットさせる
	ResizeParentToFit(false);

	Viw_Clc_Dfs(((CChildFrame *)GetParent())->m_Win_siz, this);
	
	((CChildFrame *)GetParent())->m_Ini_upd = TRUE;

	m_pPms_mng = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->m_Dbs_obj;

	SetupComponent();

	((CChildFrame *)GetParent())->ModifyStyle( WS_MAXIMIZEBOX, 0, TRUE );

}

// グリッドイベント
LRESULT CCheckinView::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	WCHAR			*txt;
	int				typ;
	LRESULT			rsl=TRUE;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONDBLCLICK:	
								break;
		case GN_LBUTTONCLICK:	if ( ntf->row>0 && ntf->clm==0){
									Cin_Cbx_Clk( ntf->row, this, m_pPms_mng );
								}
								break;						// 左ボタンクリック
 		case GN_EDITCOMPLETE:	if ( !!( txt = (WCHAR *)lParam ) ){
									if ( ntf->cid == IDC_GRD_CIN ){
										if ( ntf->clm==CIN_CLM_RNB ){
											// Edited the room No. columun
											Cin_Rom_Nbr( txt, ntf->row, this, m_pDbs_obj, m_pPms_mng );
										}
									}
									free( txt );
								}
								break;
		case GN_RBUTTONCLICK:	Cin_Ctx_Mnu( this, ntf->msx, ntf->msy, ntf->row );
								break;
		case GN_COMBOCHANGED:	typ = m_Cin_grd->GetData( ntf->clm, ntf->row );
								break;
		case GN_QUERYINPUTTYPE:	switch( ntf->clm ){
								case CIN_CLM_SEL:	rsl = GCT_CHK;	break;
								case CIN_CLM_RNB:	rsl = GCT_EDT;	break;
								default:rsl = GCT_NML;
								}
								break;
		case GN_QUERYCOLORDATA:	break;
		}
		free( ntf );
	}

	return( rsl );
}

// CheckAll checkbox click
void CCheckinView::OnBnClickedCheckCheckall()
{
	Cin_Chk_All( this, m_pPms_mng );
}

// ExecuteCheckin button click
void CCheckinView::OnBnClickedButtonExeccheckin()
{
	Cin_Exc_Clk( this, m_pDbs_obj, m_pPms_mng );
}

// Windows command operation
BOOL CCheckinView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	CIN_DTL_DAT		*dtl=NULL;

	if ( m_Sel_row != -1 ){
		dtl =  (CIN_DTL_DAT *)(LONG_PTR)m_Cin_grd->GetData( 0, m_Sel_row );
	}

//	IDD_DIALOG_CHARGEDETAIL
	if (pHandlerInfo == NULL){
		if (nCode == CN_COMMAND){
			switch ( nID ){
			case ID_CHECKIN_SELECTROOM:	Cin_Rom_Sel( this, m_pPms_mng );
										break;
			}
		}
	}

	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// OK Button Click
void CCheckinView::OnBnClickedOk()
{
	Cin_Sav_Dat( this, m_pDbs_obj, m_pPms_mng );
	GetDocument()->OnCloseDocument();
}

// Cancel button click
void CCheckinView::OnBnClickedCancel()
{
	GetDocument()->OnCloseDocument();
}

// On Close the window
void CCheckinView::OnClose()
{

	CFormView::OnClose();
}

// No Clienr area dispose
void CCheckinView::PostNcDestroy()
{
	CFormView::PostNcDestroy();
}


//	オペレーション
int		CCheckinView::GetData( int rid )
{
	int				chk;

	m_Sel_row = 1;

	chk = Cin_Get_Dat( rid, this, m_pDbs_obj, m_pPms_mng );

	return( chk );
}

int		CCheckinView::CreateData( void )
{
	int			chk=TRUE;

	//m_Sel_row = 1;

	//Rsv_Dsp_Dat( this, m_Dtl_grd, NULL, NULL, m_pPms_mng );

	return( chk );
}

BOOL CCheckinView::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CFormView::PreTranslateMessage(pMsg): FALSE );
}
