// HouseKeepingView.cpp : CHouseKeepingView クラスの実装
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#include "HouseKeepingDoc.h"
#include "HouseKeepingView.h"
#include ".\fnc\hkp_fnc.h"
#include ".\fnc\viw_cmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHouseKeepingView

void CHouseKeepingView::SetupComponent()
{
//	int				idx;
	WINDOWPLACEMENT	wpc[1];
	//RECT			rct[1];
	GRD_DEF			gdf[1];
/*
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
*/
	memset( gdf, 0, sizeof(gdf) );

	GetDlgItem(IDC_STATIC_ROOMSTATUS)->GetWindowPlacement( wpc );
	wpc->rcNormalPosition.top += 12;
	m_Hkp_grd = Hkp_Mak_Grd( IDC_GRD_HKP, &wpc->rcNormalPosition, this );
	GetDlgItem(IDC_STATIC_ROOMCHANGES)->GetWindowPlacement( wpc );
	wpc->rcNormalPosition.top += 12;
	m_Rch_grd = Hkp_Mak_Grd( IDC_GRD_RCH, &wpc->rcNormalPosition, this );
}

int	 CHouseKeepingView::InitData( WCHAR *opc, WCHAR *bcd, WCHAR *fcd )
{
	int			chk;
//	WCHAR		buf[30];

	m_Ope_cod = opc;
	m_Brc_cod = bcd;
	m_Fcl_cod = fcd;

	if ( chk = Hkp_Get_Dat( this, m_pDbs_obj, m_pPms_mng ) ){
		chk = Hkp_Get_Rch( this, m_pDbs_obj, m_pPms_mng );
	}
	return( chk );
}

IMPLEMENT_DYNCREATE(CHouseKeepingView, CFormView)

BEGIN_MESSAGE_MAP(CHouseKeepingView, CFormView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_BN_CLICKED(IDCANCEL, &CHouseKeepingView::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CHouseKeepingView::OnBnClickedOk)
	ON_WM_GRIDNOTIFY()
	ON_BN_CLICKED(IDC_BUTTON1, &CHouseKeepingView::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK_ALLSELECT, &CHouseKeepingView::OnBnClickedCheckAllselect)
END_MESSAGE_MAP()


// CHouseKeepingView コンストラクション/デストラクション

CHouseKeepingView::CHouseKeepingView(): CFormView(CHouseKeepingView::IDD)
{
	m_Hkp_grd = NULL;
	m_Rch_grd = NULL;
	m_pPms_mng = NULL;
	m_pDbs_obj = NULL;
	m_Ope_cod = NULL;
	m_Cln_sts = NULL;
	m_Occ_sts = NULL;
	m_Sal_sts = NULL;
	m_Cln_cnt = Viw_Get_Sts( &m_Cln_sts, STS_CLN );
	m_Occ_cnt = Viw_Get_Sts( &m_Occ_sts, STS_OCC );
	m_Sal_cnt = Viw_Get_Sts( &m_Sal_sts, STS_SAL );
	memset( m_Hkp_rot, 0, sizeof(m_Hkp_rot) );
	memset( m_Rch_rot, 0, sizeof(m_Rch_rot) );
}

CHouseKeepingView::~CHouseKeepingView()
{
	if ( m_Hkp_grd )	delete m_Hkp_grd;
	if ( m_Rch_grd )	delete m_Rch_grd;
	Del_All( m_Hkp_rot );
	Del_All( m_Rch_rot );
}

BOOL CHouseKeepingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CHouseKeepingView 描画

void CHouseKeepingView::OnDraw(CDC* /*pDC*/)
{
	CHouseKeepingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}


// CHouseKeepingView 印刷

BOOL CHouseKeepingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CHouseKeepingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CHouseKeepingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}


// CHouseKeepingView 診断

#ifdef _DEBUG
void CHouseKeepingView::AssertValid() const
{
	CView::AssertValid();
}

void CHouseKeepingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHouseKeepingDoc* CHouseKeepingView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHouseKeepingDoc)));
	return (CHouseKeepingDoc*)m_pDocument;
}
#endif //_DEBUG


// CHouseKeepingView メッセージ ハンドラ

void CHouseKeepingView::OnInitialUpdate()
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
LRESULT CHouseKeepingView::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;
	HKP_RCH_DAT		*rch;
	//int				row;
	//double			fvl;
	//WCHAR			*txt;
	//CRC_CVS			*ccv;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:	if ( ntf->row > 0 ){
									if ( ntf->cid == IDC_GRD_RCH ){
										rch = (HKP_RCH_DAT *)(LONG_PTR)m_Rch_grd->GetData( HKP_CLM_DAT, ntf->row );
										rch->rch = m_Rch_grd->GetCheck( HKP_CCL_SEL, ntf->row );
									}
								}
								break;
		case GN_RBUTTONCLICK:	if ( ntf->row > 0 ){
									//Nad_Ctx_Mnu( this, ntf->msx, ntf->msy, ntf->row, ntf->cid );
								}
								break;
		case GN_QUERYINPUTTYPE:	if ( ntf->cid == IDC_GRD_RCH ){
									rsl = ntf->clm==HKP_CCL_SEL ? GCT_CHK: GCT_NML;
								}
								else{
									switch( ntf->clm ){
									case HKP_CLM_SST:
									case HKP_CLM_CST:
									case HKP_CLM_OST:	rsl = GCT_CMB;	break;
									default:			rsl = GCT_NML;	break;
									}
								}
								break;
		case GN_QUERYCOMBODATA:	if ( ntf->cid == IDC_GRD_HKP ){
									Hkp_Set_Cmb( (CComboBox *)(LONG_PTR)lParam, ntf->clm, ntf->row, this );
								}
								break;
		case GN_COMBOCHANGED:	if ( ntf->cid == IDC_GRD_HKP ){
									Hkp_Get_Cmb( (int)(LONG_PTR)lParam, ntf->clm, ntf->row, this, m_pDbs_obj, m_pPms_mng );
								}
								break;
		}
		free( ntf );
	}

	return( rsl );
}

// Notify from each view
LRESULT CHouseKeepingView::OnViewNotify( WPARAM wParam, LPARAM lParam )
{
	VIW_NTF			*ntf = (VIW_NTF *)wParam;

	if ( ntf ){
		/*
		switch( ntf->ntf ){
		case VWN_RSV_CREATED:	
		case VWN_RSV_CHANGED:	Nad_Get_Dat( this, m_pDbs_obj, m_pPms_mng );
								break;
		case VWN_RSV_DELETED:	break;
		}
		*/
		free( ntf );
	}
	return( 0 );
}

// Windows command operation
BOOL CHouseKeepingView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (pHandlerInfo == NULL){
		if (nCode == CN_COMMAND){
			/*
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
			*/
		}
	}

	return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// All check button click
void CHouseKeepingView::OnBnClickedCheckAllselect()
{
	int			row, chk = ((CButton *)GetDlgItem(IDC_CHECK_ALLSELECT))->GetCheck();
	HKP_RCH_DAT	*rch;

	for ( row=1; row<=m_Rch_grd->GetLineCount(FALSE); row++ ){
		rch = (HKP_RCH_DAT *)(LONG_PTR)m_Rch_grd->GetData( HKP_CLM_DAT, row );
		rch->rch = chk;
		m_Rch_grd->SetCheck( HKP_CCL_SEL, row, chk );
	}
}

// Execute room change button click
void CHouseKeepingView::OnBnClickedButton1()
{
	int			chk;
	WCHAR		*erm=NULL;

	if ( MessageBox( _T("Are you sure to complete room change?"), NULL, MB_ICONQUESTION|MB_YESNO ) == IDYES ){
		if ( chk = Hkp_Exc_Rch( m_Rch_rot, &erm, m_pDbs_obj, m_pPms_mng ) ){
			if ( chk = Hkp_Get_Dat( this, m_pDbs_obj, m_pPms_mng ) ){
				chk = Hkp_Get_Rch( this, m_pDbs_obj, m_pPms_mng );
			}
		}
		else{
			if ( erm ){
				MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
				free( erm );
			}
		}
	}
}

// OK Button Click
void CHouseKeepingView::OnBnClickedOk()
{
	GetDocument()->OnCloseDocument();
}

// Cancel button click
void CHouseKeepingView::OnBnClickedCancel()
{
	GetDocument()->OnCloseDocument();
}

BOOL CHouseKeepingView::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CFormView::PreTranslateMessage(pMsg): FALSE );
}
