// Checkout.cpp : CCheckoutView クラスの実装
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#include "CheckoutDoc.h"
#include "CheckoutView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCheckoutView

IMPLEMENT_DYNCREATE(CCheckoutView, CFormView)

BEGIN_MESSAGE_MAP(CCheckoutView, CFormView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_BN_CLICKED(IDCANCEL, &CCheckoutView::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CCheckoutView::OnBnClickedOk)
	ON_WM_GRIDNOTIFY()
	ON_WM_CASHIERNOTIFY()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CCheckoutView コンストラクション/デストラクション

CCheckoutView::CCheckoutView(): CFormView(CCheckoutView::IDD)
{
	m_Cot_grd = NULL;
	m_Sel_row = -1;
	m_Csh_ier = NULL;
	memset( m_Rsv_inf, 0, sizeof(m_Rsv_inf) );
	memset( m_Rsv_dtl, 0, sizeof(m_Rsv_dtl) );
}

CCheckoutView::~CCheckoutView()
{
	if ( m_Csh_ier )	delete 	m_Csh_ier;
	if ( !!m_Cot_grd )	delete	m_Cot_grd;
	Del_All( m_Rsv_dtl );
}

BOOL CCheckoutView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CCheckoutView 描画

void CCheckoutView::OnDraw(CDC* /*pDC*/)
{
	CCheckoutDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}


// CCheckoutView 印刷

BOOL CCheckoutView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CCheckoutView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CCheckoutView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}


// CCheckoutView 診断

#ifdef _DEBUG
void CCheckoutView::AssertValid() const
{
	CView::AssertValid();
}

void CCheckoutView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCheckoutDoc* CCheckoutView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCheckoutDoc)));
	return (CCheckoutDoc*)m_pDocument;
}
#endif //_DEBUG


// CCheckoutView メッセージ ハンドラ

void CCheckoutView::OnInitialUpdate()
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

	m_Rsv_nam.SubclassDlgItem(IDC_STATIC_BOOKINGNAME, this );
	m_Rsv_nam.SetFont(_T("MS UI Gothic"), 24, RGB(0,0,0) );

	m_Wrn_ing.SubclassDlgItem(IDC_STATIC_WARNING, this );
	m_Wrn_ing.SetFont(_T("MS UI Gothic"), 18, RGB(255,0,0) );

	WINDOWPLACEMENT			wpc[1];
	GetDlgItem(IDC_STATIC_CINLIST)->GetWindowPlacement( wpc );
	m_Cot_grd = Cot_Mak_Grd( IDC_GRD_CIN, &wpc->rcNormalPosition, this );

	((CChildFrame *)GetParent())->ModifyStyle( WS_MAXIMIZEBOX, 0, TRUE );

	m_Csh_ier = new CCashierDlg(this);
	m_Csh_ier->m_pPms_mng = m_pPms_mng;
	m_Csh_ier->m_pDbs_obj = m_pDbs_obj;
	m_Csh_ier->m_Chk_out = TRUE;
	m_Csh_ier->Create(IDD_DIALOG_CASHIER, this);
}

// グリッドイベント
LRESULT CCheckoutView::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	char			*txt;
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
 		case GN_QUERYINPUTTYPE:	break;
		}
		free( ntf );
	}

	return( rsl );
}

// グリッドイベント
LRESULT CCheckoutView::OnCashierNotify( WPARAM wParam, LPARAM lParam )
{
	CSH_NTF			*ntf = (CSH_NTF *)wParam;
	LRESULT			rsl=TRUE;
	Csr				csr;
	INH_DTL_DAT		*dtl;
	WCHAR			*wrn, buf[192];

	if ( ntf ){
		switch( ntf->ntf ){
		case CN_CHECKOUTING:
		case CN_ROOMCHANGED:	for ( csr=NULL; csr=Nxt_Lnk( m_Rsv_dtl, csr ); ){
									dtl = Rec_Of(INH_DTL_DAT, csr);
									if ( DBS_INT_VAL(dtl->dtl->id) == (int)lParam ){
										if ( Cmp_Dte( DBS_DAY_PTR(dtl->dtl->dpt), m_pPms_mng->bdt ) > 0 ){
											wrn = _T("This room is not subject to checkout today.");
										}
										else if ( Cmp_Dte( DBS_DAY_PTR(dtl->dtl->dpt), m_pPms_mng->bdt ) < 0 ){
											wrn = _T("This room has already passed the checkout date.");
										}
										else{
											wrn = ntf->ntf==CN_CHECKOUTING ? _T("Execute a checkout."): _T("");
										}
										if ( ntf->ntf==CN_CHECKOUTING ){
											swprintf_s( buf, memcnt(buf), _T("%s\nDo you continue?"), wrn );
											rsl = MessageBox( buf, NULL, MB_ICONQUESTION|MB_YESNO )==IDYES;
										}
										else{
											GetDlgItem(IDC_STATIC_WARNING)->SetWindowText( wrn );
										}
										break;
									}
								}
								break;
		case CN_CHECKOUT:		rsl = Cot_Exc_Cot( (int)lParam, this, m_pDbs_obj, m_pPms_mng );
								break;
		case CN_ALLCHECKOUT:	OpentCashier( FALSE );
								GetDocument()->OnCloseDocument();
								break;
		}
		free( ntf );
	}
	return( rsl );
}

// OK Button Click
void CCheckoutView::OnBnClickedOk()
{
	if ( !!m_Csh_ier->IsWindowVisible() ){
		if ( m_Csh_ier->Save() ){
			OpentCashier( FALSE );
			GetDocument()->OnCloseDocument();
		}
	}
}

// Cancel button click
void CCheckoutView::OnBnClickedCancel()
{
	if ( !!m_Csh_ier->IsWindowVisible() ){
		if ( !m_Csh_ier->IsUpdateExist() || Viw_Cfm_Dcd( this )==IDYES ){
			OpentCashier( FALSE );
			GetDocument()->OnCloseDocument();
		}
	}
}

// On Close the window
void CCheckoutView::OnClose()
{

	CFormView::OnClose();
}

// No Clienr area dispose
void CCheckoutView::PostNcDestroy()
{

	CFormView::PostNcDestroy();
}


//	オペレーション
int		CCheckoutView::GetData( int rii, int rdi )
{
	WCHAR			*erm=NULL;
	int				chk;

	m_Sel_row = 1;
	m_Rsv_did = rdi;

	if ( chk = Cot_Get_Inf( m_Rsv_inf, rii, &erm, m_pDbs_obj, m_pPms_mng ) ){
		if ( erm )		free( erm );
		if ( chk = Cot_Get_Dtl( m_Rsv_dtl,
				DBS_INT_VAL(m_Rsv_inf->rsv->id), &erm, m_pDbs_obj, m_pPms_mng ) ){
			((CChildFrame *)GetParent())->m_Rsv_id = rii;
			Cot_Dsp_Dat( this, m_Cot_grd, m_Rsv_inf, m_Rsv_dtl, m_pPms_mng );
		}
		OpentCashier( TRUE );
	}
	if ( erm ){
		MessageBox(erm/*_T("Faild to get a reservation data!")*/, 0, MB_ICONSTOP|MB_OK );
		free( erm );
	}

	return( chk );
}

int		CCheckoutView::CreateData( void )
{
	int			chk=TRUE;

	//m_Sel_row = 1;

	//Rsv_Dsp_Dat( this, m_Dtl_grd, NULL, NULL, m_pPms_mng );

	return( chk );
}

BOOL CCheckoutView::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CFormView::PreTranslateMessage(pMsg): FALSE );
}

void CCheckoutView::OpentCashier( int opn )
{
	int			idx, div = opn ? 166: -166;
	DWORD		idc[] = { IDOK, IDCANCEL, IDC_STATIC_WARNING };
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

	m_Cot_grd->ShowWindow( opn ? SW_HIDE: SW_SHOWNORMAL );
	GetDlgItem(IDC_CHECK_ALL)->ShowWindow( opn ? SW_HIDE: SW_SHOWNORMAL );
	GetDlgItem(IDC_BUTTON_EXECCHECKIN)->ShowWindow( opn ? SW_HIDE: SW_SHOWNORMAL  );

	GetDlgItem(IDOK)->SetWindowText( opn ? _T("Close with save"): _T("Close") );
	GetDlgItem(IDCANCEL)->ShowWindow( opn ? SW_SHOWNORMAL: SW_HIDE  );

	if ( opn ){
		m_Csh_ier->SetWindowPos( NULL, 0, 100, 0, 0, SWP_NOSIZE|SWP_NOZORDER );
		m_Csh_ier->SetupCashier( m_pPms_mng->opc, m_pPms_mng->brc, m_pPms_mng->fcc, DBS_INT_VAL(m_Rsv_inf->rsv->id), m_Rsv_did );
	}
	m_Csh_ier->ShowWindow( opn ? SW_SHOWNORMAL: SW_HIDE );
	//if ( !opn && !!m_Dct_csh )	GetDocument()->OnCloseDocument();
}
