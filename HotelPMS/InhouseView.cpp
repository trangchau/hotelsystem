// InhouseView.cpp : CInhouseView クラスの実装
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#include "InhouseDoc.h"
#include "InhouseView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInhouseView

IMPLEMENT_DYNCREATE(CInhouseView, CFormView)

BEGIN_MESSAGE_MAP(CInhouseView, CFormView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_BN_CLICKED(IDCANCEL, &CInhouseView::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CInhouseView::OnBnClickedOk)
	ON_WM_GRIDNOTIFY()
	ON_WM_CASHIERNOTIFY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_EXECCHECKIN, &CInhouseView::OnBnClickedButtonExeccheckin)
END_MESSAGE_MAP()


void CInhouseView::SetupComponent()
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
	m_Inh_grd = Cin_Mak_Grd( IDC_GRD_CIN, &wpc->rcNormalPosition, this );

	((CChildFrame *)GetParent())->ModifyStyle( WS_MAXIMIZEBOX, 0, TRUE );

	m_Csh_ier = new CCashierDlg(this);
	m_Csh_ier->Create(IDD_DIALOG_CASHIER, this);
	m_Csh_ier->m_pPms_mng = m_pPms_mng;
	m_Csh_ier->m_pDbs_obj = m_pDbs_obj;
}

// CInhouseView コンストラクション/デストラクション

CInhouseView::CInhouseView(): CFormView(CInhouseView::IDD)
{
	m_Inh_grd = NULL;
	m_Sel_row = -1;
	m_Csh_ier = NULL;
	m_Dct_csh = 0;
	m_Rsv_did = 0;

	memset( m_Rsv_inf, 0, sizeof(m_Rsv_inf) );
	memset( m_Rsv_dtl, 0, sizeof(m_Rsv_dtl) );
}

CInhouseView::~CInhouseView()
{
	if ( m_Csh_ier )	delete 	m_Csh_ier;
	if ( m_Inh_grd )	delete	m_Inh_grd;
	Del_All( m_Rsv_dtl );
}

BOOL CInhouseView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CInhouseView 描画

void CInhouseView::OnDraw(CDC* /*pDC*/)
{
	CInhouseDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}


// CInhouseView 印刷

BOOL CInhouseView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CInhouseView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CInhouseView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}


// CInhouseView 診断

#ifdef _DEBUG
void CInhouseView::AssertValid() const
{
	CView::AssertValid();
}

void CInhouseView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CInhouseDoc* CInhouseView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CInhouseDoc)));
	return (CInhouseDoc*)m_pDocument;
}
#endif //_DEBUG


// CInhouseView メッセージ ハンドラ

void CInhouseView::OnInitialUpdate()
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

	//m_Rsv_nam.SubclassDlgItem(IDC_STATIC_BOOKINGNAME, this );
	//m_Rsv_nam.SetFont(_T("MS UI Gothic"), 24, RGB(0,0,0) );

	SetupComponent();
//	SetSize( FALSE );
}

// グリッドイベント
LRESULT CInhouseView::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	CComboBox		*cbx;
	char			*txt;
	int				typ;
	LRESULT			rsl=TRUE;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:	
								break;
		case GN_LBUTTONDBLCLICK:if ( ntf->cid == IDC_GRD_CIN ){
									//rid = m_Rsv_grd->GetData( 0, ntf->row );
									//Rnc_Rsv_Viw( rid );
								}
								break;						// 左ボタンクリック
 		case GN_EDITCOMPLETE:	if ( !!( txt = (char *)lParam ) ){
									if ( ntf->cid == IDC_GRD_CIN ){
									}
									free( txt );
								}
								break;
		case GN_QUERYCOMBODATA:	cbx = (CComboBox *)(LONG_PTR)lParam;
								break;
		case GN_COMBOCHANGED:	typ = m_Inh_grd->GetData( ntf->clm, ntf->row );
								break;
 		case GN_QUERYINPUTTYPE:	typ = m_Inh_grd->GetData( ntf->clm, ntf->row );
								break;
		case GN_QUERYCOLORDATA:	break;
		}
		free( ntf );
	}

	return( rsl );
}

// Notify from cashier
LRESULT CInhouseView::OnCashierNotify( WPARAM wParam, LPARAM lParam )
{
	CSH_NTF			*ntf = (CSH_NTF *)wParam;
	LRESULT			rsl=TRUE;

	if ( ntf ){
		switch( ntf->ntf ){
		case CN_ROOMCHANGED:	break;
		}
		free( ntf );
	}
	return( 0 );
}

// Cashier button click
void CInhouseView::OnBnClickedButtonExeccheckin()
{
	if ( !m_Csh_ier->IsWindowVisible() ){
		OpentCashier( TRUE );
	}
	else{
		OpentCashier( FALSE );
	}
}

// OK Button Click
void CInhouseView::OnBnClickedOk()
{
	if ( !!m_Csh_ier->IsWindowVisible() ){
		if ( m_Csh_ier->Save() ){
			OpentCashier( FALSE );
		}
	}
	else{
		GetDocument()->OnCloseDocument();
	}
}

// Cancel button click
void CInhouseView::OnBnClickedCancel()
{
	if ( !!m_Csh_ier->IsWindowVisible() ){
		if ( !m_Csh_ier->IsUpdateExist() || Viw_Cfm_Dcd( this )==IDYES ){
			OpentCashier( FALSE );
		}
	}
//	GetDocument()->OnCloseDocument();
}

// On Close the window
void CInhouseView::OnClose()
{

	CFormView::OnClose();
}

// No Clienr area dispose
void CInhouseView::PostNcDestroy()
{
	CFormView::PostNcDestroy();
}

BOOL CInhouseView::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CFormView::PreTranslateMessage(pMsg): FALSE );
}


//	オペレーション
int		CInhouseView::GetData( int rii, int rdi, int csh )
{
	WCHAR			*erm=NULL;
	int				chk;

	m_Sel_row = 1;
	m_Dct_csh = csh;
	m_Rsv_did = rdi;

	if ( chk = Inh_Get_Inf( m_Rsv_inf, rii, &erm, m_pDbs_obj, m_pPms_mng ) ){
		if ( erm )		free( erm );
		if ( chk = Inh_Get_Dtl( m_Rsv_dtl,
				DBS_INT_VAL(m_Rsv_inf->rsv->id), &erm, m_pDbs_obj, m_pPms_mng ) ){
			((CChildFrame *)GetParent())->m_Rsv_id = rii;
			Inh_Dsp_Dat( this, m_Inh_grd, m_Rsv_inf, m_Rsv_dtl, m_pPms_mng );
		}
		if ( csh )	OnBnClickedButtonExeccheckin();
	}
	if ( erm ){
		MessageBox(erm/*_T("Faild to get a reservation data!")*/, 0, MB_ICONSTOP|MB_OK );
		free( erm );
	}

	return( chk );
}

int		CInhouseView::CreateData( void )
{
	int			chk=TRUE;

	//m_Sel_row = 1;

	//Rsv_Dsp_Dat( this, m_Dtl_grd, NULL, NULL, m_pPms_mng );

	return( chk );
}

void CInhouseView::OpentCashier( int opn )
{
	int			idx, div = opn ? 166: -166;
	DWORD		idc[] = { IDOK, IDCANCEL };
	WINDOWPLACEMENT		wpc[1];
	RECT				*rct=&wpc->rcNormalPosition;

	((CChildFrame *)GetParent())->m_Nsz_viw = FALSE;
	for ( idx=0; idx<memcnt(idc); idx++ ){
		GetDlgItem(idc[idx])->GetWindowPlacement( wpc );
		wpc->rcNormalPosition.top += div;
		wpc->rcNormalPosition.bottom += div;
		GetDlgItem(idc[idx])->SetWindowPlacement( wpc );
	}
	GetWindowPlacement( wpc );
	wpc->rcNormalPosition.bottom += div;
	SetWindowPlacement( wpc );
	GetParent()->GetWindowPlacement( wpc );
	wpc->rcNormalPosition.bottom += div;
	((CChildFrame *)GetParent())->m_Win_siz->y = wpc->rcNormalPosition.bottom;
	GetParent()->SetWindowPlacement( wpc );
	((CChildFrame *)GetParent())->m_Nsz_viw = TRUE;

	m_Inh_grd->ShowWindow( opn ? SW_HIDE: SW_SHOWNORMAL );
	GetDlgItem(IDC_CHECK_ALL)->ShowWindow( opn ? SW_HIDE: SW_SHOWNORMAL );
	GetDlgItem(IDC_BUTTON_EXECCHECKIN)->ShowWindow( opn ? SW_HIDE: SW_SHOWNORMAL  );

	GetDlgItem(IDOK)->SetWindowText( opn ? _T("Close with save"): _T("Close") );
	GetDlgItem(IDCANCEL)->ShowWindow( opn ? SW_SHOWNORMAL: SW_HIDE  );

	if ( opn ){
		m_Csh_ier->SetWindowPos( NULL, 0, 100, 0, 0, SWP_NOSIZE|SWP_NOZORDER );
		m_Csh_ier->SetupCashier( m_pPms_mng->opc, m_pPms_mng->brc, m_pPms_mng->fcc, DBS_INT_VAL(m_Rsv_inf->rsv->id), m_Rsv_did );
	}
	m_Csh_ier->ShowWindow( opn ? SW_SHOWNORMAL: SW_HIDE );
	if ( !opn && !!m_Dct_csh )	GetDocument()->OnCloseDocument();
}
