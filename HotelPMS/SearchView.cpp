// HotelPMSView.cpp : CSearchView クラスの実装
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "ReservationDoc.h"

#include "SearchDoc.h"
#include "SearchView.h"
#include ".\fnc\sch_fnc.h"
//#include ".\fnc\sch_sql.h"
#include ".\fnc\viw_cmn.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	TEV_INI			1			// 初期化タイマーイベント
#define	TIM_INI			10			// 初期化タイマーインターバル

// CSearchView

IMPLEMENT_DYNCREATE(CSearchView, CFormView)

BEGIN_MESSAGE_MAP(CSearchView, CFormView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_KIND, &CSearchView::OnTcnSelchangeTabKind)
	ON_BN_CLICKED(IDOK, &CSearchView::OnBnClickedOk)
	ON_WM_TIMER()
	ON_WM_GRIDNOTIFY()
	ON_WM_VIEWNOTIFY()
	ON_BN_CLICKED(IDC_BUTTON_RESEARCH, &CSearchView::OnBnClickedButtonResearch)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT2, &CSearchView::OnBnClickedButtonDefault2)
END_MESSAGE_MAP()

// CSearchView コンストラクション/デストラクション

CSearchView::CSearchView(): CFormView(CSearchView::IDD)
{
//	m_Rdn_rii = 0;
//	m_Rdn_rdi = 0;
	m_Rdn_rsv = NULL;
	memset( m_Rsv_rot, 0, sizeof(m_Rsv_rot) );
	memset( m_Cin_rot, 0, sizeof(m_Cin_rot) );
	memset( m_Inh_rot, 0, sizeof(m_Inh_rot) );
	memset( m_Cot_rot, 0, sizeof(m_Cot_rot) );
}

CSearchView::~CSearchView()
{
	Del_All( m_Rsv_rot );
	Del_All( m_Cin_rot );
	Del_All( m_Inh_rot );
	Del_All( m_Cot_rot );

	if ( !!m_Rsv_grd )		delete m_Rsv_grd;
	if ( !!m_Cin_grd )		delete m_Cin_grd;
	if ( !!m_Inh_grd )		delete m_Inh_grd;
	if ( !!m_Cot_grd )		delete m_Cot_grd;
}

BOOL CSearchView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// CSearchView 描画

void CSearchView::OnDraw(CDC* /*pDC*/)
{
	CSearchDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}


// CSearchView 印刷

BOOL CSearchView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CSearchView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CSearchView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}


// CSearchView 診断

#ifdef _DEBUG
void CSearchView::AssertValid() const
{
	CView::AssertValid();
}

void CSearchView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSearchDoc* CSearchView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSearchDoc)));
	return (CSearchDoc*)m_pDocument;
}
#endif //_DEBUG


// CSearchView メッセージ ハンドラ

// ビュー初期化イベント
void CSearchView::OnInitialUpdate()
{
	((CChildFrame *)GetParent())->m_Nsz_viw = TRUE;	// Non sizable
	((CChildFrame *)GetParent())->m_Viw_knd = VWK_SCH;

	CView::OnInitialUpdate();

	// ダイアログリソースのサイズにフィットさせる
	ResizeParentToFit(false);

	GetMainFrm()->m_Sch_viw = this;

	m_pPms_mng = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->m_Dbs_obj;

	m_Rsv_pnl.SubclassDlgItem( IDC_STATIC_RSVPANEL, this );
	m_Cin_pnl.SubclassDlgItem( IDC_STATIC_CINPANEL, this );
	m_Inh_pnl.SubclassDlgItem( IDC_STATIC_INHPANEL, this );
	m_Cot_pnl.SubclassDlgItem( IDC_STATIC_COTPANEL, this );
	m_Rsv_avf.SubclassDlgItem( IDC_EDIT_RSV_ARRIVALFR, this );
	m_Rsv_avt.SubclassDlgItem( IDC_EDIT_RSV_ARRIVALTO, this );
	m_Rsv_dpf.SubclassDlgItem( IDC_EDIT_RSV_DEPARTUREFR, this );
	m_Rsv_dpt.SubclassDlgItem( IDC_EDIT_RSV_DEPARTURETO, this );
	m_Knd_tab.SubclassDlgItem( IDC_TAB_KIND, this );
	m_Rsv_pth.SubclassDlgItem( IDC_COMBO_RSV_BOOKIGFROM, this );
	m_Cin_pth.SubclassDlgItem( IDC_COMBO_INH_BOOKIGFROM, this );
	m_Inh_pth.SubclassDlgItem( IDC_COMBO_CIN_BOOKIGFROM, this );
	m_Cot_pth.SubclassDlgItem( IDC_COMBO_COT_BOOKIGFROM, this );
	m_Rsv_pth.SetAgentData( m_pPms_mng->agl );
	m_Cin_pth.SetAgentData( m_pPms_mng->agl );
	m_Inh_pth.SetAgentData( m_pPms_mng->agl );
	m_Cot_pth.SetAgentData( m_pPms_mng->agl );

	Mak_Sch_Pnl( this, m_pPms_mng );// 画面作成
	Sch_Tab_Chg( 0, this );			// タブを１ページ目にする

	Viw_Clc_Dfs(((CChildFrame *)GetParent())->m_Win_siz, this);
	
	((CChildFrame *)GetParent())->m_Ini_upd = TRUE;


	((CChildFrame *)GetParent())->ModifyStyle( WS_MAXIMIZEBOX, 0, TRUE );
    CMenu*  pSysMenu = ((CChildFrame *)GetParent())->GetSystemMenu(FALSE);
    pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
/*
	WCHAR			buf[30];
	m_Rsv_avf.SetWindowTextW( Edt_Dte(buf, memcnt(buf), m_pPms_mng->bdt) );
	GetDlgItem(IDC_CHECK_RSV_RSV)->SetCheck( TRUE );
*/
	SetTimer( TEV_INI, TIM_INI, NULL );
}

// タイマーイベント
void CSearchView::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer( nIDEvent );

	Sch_Sch_Exc( m_Rsv_rot, SHI_RSV, this, m_pPms_mng );
	Sch_Sch_Exc( m_Cin_rot, SHI_CIN, this, m_pPms_mng );
	Sch_Sch_Exc( m_Inh_rot, SHI_INH, this, m_pPms_mng );
	Sch_Sch_Exc( m_Cot_rot, SHI_COT, this, m_pPms_mng );

	CFormView::OnTimer(nIDEvent);
}

// タブの選択変更イベント
void CSearchView::OnTcnSelchangeTabKind(NMHDR *pNMHDR, LRESULT *pResult)
{
    int        sel;
    
    sel = m_Knd_tab.GetCurSel();
	Sch_Tab_Chg( sel, this );

	*pResult = 0;
}

// グリッドイベント
LRESULT CSearchView::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	CComboBox		*cbx;
	char			*txt;
	int				typ, rii, rdi, vkd=-1;
	LRESULT			rsl=TRUE;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONDBLCLICK:	rii = ntf->grd->GetData( 0, ntf->row );
									rdi = ntf->grd->GetData( 1, ntf->row );
									Sch_Rnc_Viw( ntf->cid, rii, rdi, FALSE );
									break;
		case GN_RBUTTONCLICK:		Sch_Ctx_Mnu( this, ntf->msx, ntf->msy, ntf->row, ntf->cid );
									break;
		case GN_LBUTTONCLICK:		rsl = TRUE;
									break;
 		case GN_EDITCOMPLETE:		if ( !!( txt = (char *)lParam ) ){
										if ( ntf->cid == IDC_GRD_RSV ){
										}
										free( txt );
									}
									break;
		case GN_QUERYCOMBODATA:		cbx = (CComboBox *)(LONG_PTR)lParam;
									break;
		case GN_COMBOCHANGED:		typ = m_Rsv_grd->GetData( ntf->clm, ntf->row );
									break;
 		case GN_QUERYINPUTTYPE:		typ = m_Rsv_grd->GetData( ntf->clm, ntf->row );
									break;
		case GN_QUERYCOLORDATA:		break;
		}
		free( ntf );
	}

	return( rsl );
}

// Notify from each view
LRESULT CSearchView::OnViewNotify( WPARAM wParam, LPARAM lParam )
{
	VIW_NTF			*ntf = (VIW_NTF *)wParam;

	if ( ntf ){
		switch( ntf->ntf ){
		//case VWN_RSV_CREATED:	Sch_Add_Rsv( (int)lParam, ntf->vwk, this, m_pDbs_obj, m_pPms_mng );
		//						break;
		//case VWN_RSV_CHANGED:	Sch_Upd_Rsv( (int)lParam, ntf->vwk, this, m_pDbs_obj, m_pPms_mng );
		//						break;
		case VWN_RSV_CREATED:	
		case VWN_RSV_CHANGED:	Sch_Sch_Exc( m_Rsv_rot, SHI_RSV, this, m_pPms_mng );
								Sch_Sch_Exc( m_Cin_rot, SHI_CIN, this, m_pPms_mng );
								Sch_Sch_Exc( m_Inh_rot, SHI_INH, this, m_pPms_mng );
								Sch_Sch_Exc( m_Cot_rot, SHI_COT, this, m_pPms_mng );
								break;
		case VWN_RSV_DELETED:	break;
		//case VWN_STS_CHANGED:	Bst_Upd_Sts( (int)(LONG_PTR)(ntf->usr), (int)lParam, this, m_Bst_mng );
		//						break;
		}
		free( ntf );
	}
	return( 0 );
}

// Windows command operation
BOOL CSearchView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (pHandlerInfo == NULL){
		if (nCode == CN_COMMAND){
			switch ( nID ){
			case ID_SCH_CIN_CHECKIN:
			case ID_SCH_COT_CHECKOUT:		break;
			case ID_SCH_INH_CASHIER:		if ( m_Rdn_rsv ){
												Sch_Rnc_Viw( IDC_GRD_INH, m_Rdn_rii, m_Rdn_rdi, TRUE );
											}
											break;
			case ID_SCH_RESERVATIONVIEW:	if ( m_Rdn_rsv ){
												Sch_Rnc_Viw( IDC_GRD_RSV, m_Rdn_rii, m_Rdn_rdi, FALSE );
											}
											break;
			case ID_SCH_DEPOSITMANAGE:		if ( m_Rdn_rsv ){
												Sch_Mng_Dps( this, m_pDbs_obj, m_pPms_mng );
											}
											break;
			}
		}
	}

	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// Search Button Click Eveng
void CSearchView::OnBnClickedButtonResearch()
{
	Csr		rot = Rsv_Get_Rot( m_Knd_tab.GetCurSel(), this );

	if ( rot )	Sch_Sch_Exc( rot, m_Knd_tab.GetCurSel(), this, m_pPms_mng );
}

// 非クライアント領域破棄
void CSearchView::PostNcDestroy()
{

	CFormView::PostNcDestroy();
}

void CSearchView::OnBnClickedOk()
{
	((CHotelPMSApp *)AfxGetApp())->pDocTemplate[VWK_RSV]->OpenDocumentFile(NULL);
}

BOOL CSearchView::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CFormView::PreTranslateMessage(pMsg): FALSE );
}

// DefaultButton click
void CSearchView::OnBnClickedButtonDefault2()
{
	Sch_Set_Def( this, m_pDbs_obj, m_pPms_mng );
}
