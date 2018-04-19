// MainFrm.cpp : CMainFrame クラスの実装
//

#include "stdafx.h"
#include "HotelPMS.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "BookingStatusDoc.h"
#include "BookingStatusView.h"
#include "NightAuditDoc.h"
#include "NightAuditView.h"
#include "HouseKeepingDoc.h"
#include "HouseKeepingView.h"
#include "MessageDlg.h"

#include ".\fig\fig_ope.h"
#include ".\fig\rbr_bnd.h"

#include ".\fnc\bst_fnc.h"
#include ".\fnc\mfm_fnc.h"
#include ".\fnc\viw_cmn.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//　CBookingStsToolBar
BEGIN_MESSAGE_MAP(CBookingStsToolBar, CToolBar)
	ON_WM_EDITNOTIFY()
END_MESSAGE_MAP()

LRESULT CBookingStsToolBar::OnEditNotify( WPARAM wParam, LPARAM lParam )
{
	return( AfxGetApp()->GetMainWnd()->SendMessage( WM_EDITNOTIFY, wParam, lParam ) ); 
}

//　CInfoToolBar
BEGIN_MESSAGE_MAP(CInfoToolBar, CToolBar)
	ON_WM_COMBONOTIFY()
	ON_WM_STATICNOTIFY()
END_MESSAGE_MAP()

LRESULT CInfoToolBar::OnComboNotify( WPARAM wParam, LPARAM lParam )
{
	return( AfxGetApp()->GetMainWnd()->SendMessage( WM_COMBONOTIFY, wParam, lParam ) ); 
}

LRESULT CInfoToolBar::OnStaticNotify( WPARAM wParam, LPARAM lParam )
{
	return( AfxGetApp()->GetMainWnd()->SendMessage( WM_STATICNOTIFY, wParam, lParam ) ); 
}

//　CRoomDirectToolBar
BEGIN_MESSAGE_MAP(CRoomDirectToolBar, CToolBar)
	ON_WM_EDITNOTIFY()
END_MESSAGE_MAP()

LRESULT CRoomDirectToolBar::OnEditNotify( WPARAM wParam, LPARAM lParam )
{
	return( AfxGetApp()->GetMainWnd()->SendMessage( WM_EDITNOTIFY, wParam, lParam ) );
}

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_BOOKING_ADDBOOKING, &CMainFrame::OnUpdateBookingAddbooking)
	ON_UPDATE_COMMAND_UI(ID_BOOKING_CANCELBOOKING, &CMainFrame::OnUpdateBookingCancelbooking)
	ON_UPDATE_COMMAND_UI(ID_BOOKING_CUTBOOKING, &CMainFrame::OnUpdateBookingCutbooking)
	ON_UPDATE_COMMAND_UI(ID_BOOKING_MOVEBOOKING, &CMainFrame::OnUpdateBookingMovebooking)
	ON_UPDATE_COMMAND_UI(ID_PRICESETTING_ROOMCHARGE, &CMainFrame::OnUpdatePricesettingRoomcharge)
	ON_UPDATE_COMMAND_UI(ID_PRICESETTING_PRODUCTSCATEGORY, &CMainFrame::OnUpdatePricesettingProductscategory)
	ON_UPDATE_COMMAND_UI(ID_PRICESETTING_PRODUCTSPRICE, &CMainFrame::OnUpdatePricesettingProductsprice)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_CURRENCYCONVERSION, &CMainFrame::OnUpdateSettingsCurrencyconversion)
	ON_UPDATE_COMMAND_UI(ID_VIEW_NIGHTAUDIT, &CMainFrame::OnUpdateViewNightAudit)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HOUSEKEEPING, &CMainFrame::OnUpdateViewHouseKeeping)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BOOKINGSTATUSVIEW, &CMainFrame::OnUpdateViewBookingstatusview)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SEARCHVIEW, &CMainFrame::OnUpdateViewSearchview)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEWRESERVATION, &CMainFrame::OnUpdateFileNewreservation)
	ON_UPDATE_COMMAND_UI(ID_FILE_LOGOUT, &CMainFrame::OnUpdateFileLogout)        
	ON_UPDATE_COMMAND_UI(ID_FILE_CHANGEPASSWORD, &CMainFrame::OnUpdateFileChangePassword)
	ON_UPDATE_COMMAND_UI(TBP_PERIODFROM, &CMainFrame::OnUpdatePeriodfrom)
	ON_UPDATE_COMMAND_UI(TBP_PERIODTO, &CMainFrame::OnUpdatePeriodto)
	ON_WM_EDITNOTIFY()
	ON_WM_COMBONOTIFY()
	ON_WM_STATICNOTIFY()
	ON_WM_LBUTTONDOWN()
	ON_UPDATE_COMMAND_UI(ID_MASTER_OPERATORINFO, &CMainFrame::OnUpdateMasterOperatorinfo)
	ON_UPDATE_COMMAND_UI(ID_MASTER_BRANCHINFO, &CMainFrame::OnUpdateMasterBranchinfo)
	ON_UPDATE_COMMAND_UI(ID_MASTER_FACILITYINFO, &CMainFrame::OnUpdateMasterFacilityinfo)
	ON_UPDATE_COMMAND_UI(ID_MASTER_EQUIPMENTTYPE, &CMainFrame::OnUpdateMasterEquipmenttype)
	ON_UPDATE_COMMAND_UI(ID_MASTER_EQUIPMENTINFO, &CMainFrame::OnUpdateMasterEquipmentinfo)
	ON_UPDATE_COMMAND_UI(ID_MASTER_CURRENCYCONVERSION, &CMainFrame::OnUpdateMasterCurrencyConversion)
	ON_UPDATE_COMMAND_UI(ID_MASTER_TRAVELAGENT, &CMainFrame::OnUpdateMasterTravelAgent)
	ON_UPDATE_COMMAND_UI(ID_MASTER_CREDITCARDCOMPANY, &CMainFrame::OnUpdateMasterCreditCardCompany)
	ON_UPDATE_COMMAND_UI(ID_MASTER_EMPLOYEEINFO, &CMainFrame::OnUpdateMasterEmployeeInfo)
	ON_UPDATE_COMMAND_UI(ID_MASTER_DUTYINFO, &CMainFrame::OnUpdateMasterDutyInfo)
	ON_UPDATE_COMMAND_UI(ID_MASTER_MARKETSEGMENT, &CMainFrame::OnUpdateMasterMarketSegment)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ステータス ライン インジケータ
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame コンストラクション/デストラクション

CMainFrame::CMainFrame()
{
	m_Ini_inf = NULL;
	m_GBd_cnt = 0;
	m_Sch_viw = NULL;
	m_Bst_viw = NULL;
	m_Nad_viw = NULL;
	m_pRoomChargeDlg = NULL;
	m_pCategoryDlg = NULL;
	m_pOperatorInfoDlg = NULL;
	m_pBranchInfoDlg = NULL;
	m_pFacilityInfoDlg = NULL;
	m_pEquipmentTypeDlg = NULL;
	m_pEquipmentInfoDlg = NULL;
	m_pCurrencyCvsDlg = NULL;
	m_pCurrencyRateDlg = NULL;
	m_pTravelAgentDlg = NULL;
	m_pCreditCardComDlg = NULL;
	m_pEmployeeDlg = NULL;
	m_pDutyDlg = NULL;
	m_pMarketSegmentDlg = NULL;
	m_Dte_pcr = NULL;


	memset( m_Pms_mng, 0, sizeof(m_Pms_mng) );
}

CMainFrame::~CMainFrame()
{
	if ( m_pRoomChargeDlg )		delete	m_pRoomChargeDlg;
	if ( m_pCategoryDlg )		delete	m_pCategoryDlg;
	if ( m_pProductDlg )		delete	m_pProductDlg;
	if ( m_pOperatorInfoDlg )	delete	m_pOperatorInfoDlg;
	if ( m_pBranchInfoDlg )		delete	m_pBranchInfoDlg;
	if ( m_pFacilityInfoDlg )	delete	m_pFacilityInfoDlg;
	if ( m_pEquipmentTypeDlg )	delete	m_pEquipmentTypeDlg;
	if ( m_pEquipmentInfoDlg )	delete	m_pEquipmentInfoDlg;
	if ( m_pCurrencyCvsDlg )	delete	m_pCurrencyCvsDlg;
	if ( m_pCurrencyRateDlg )	delete	m_pCurrencyRateDlg;
	if ( m_pTravelAgentDlg )	delete	m_pTravelAgentDlg;
	if ( m_pCreditCardComDlg )	delete	m_pCreditCardComDlg;
	if ( m_pEmployeeDlg )		delete	m_pEmployeeDlg;
	if ( m_pDutyDlg )			delete	m_pDutyDlg;
	if ( m_pMarketSegmentDlg )	delete	m_pMarketSegmentDlg;
	//if ( m_Dte_pcr )	m_Dte_pcr->SendMessage( WM_CLOSE, 0, 0 );


//	if ( !!m_Sch_dlg )	delete m_Sch_dlg;
}

void CMainFrame::DockControlBarLeftOf(		// バーを右側に配置
CToolBar	*bar,							// 配置するバー
CToolBar	*lbr )							// 左側に位置する別のバー
{
	CRect	rect;
	DWORD	lst;
	UINT	nst = 0;

	RecalcLayout();
	lbr->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	lst = lbr->GetBarStyle();

	nst = (lst&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : nst;
	nst = (lst&CBRS_ALIGN_BOTTOM && nst==0) ? AFX_IDW_DOCKBAR_BOTTOM : nst;
	nst = (lst&CBRS_ALIGN_LEFT && nst==0) ? AFX_IDW_DOCKBAR_LEFT : nst;
	nst = (lst&CBRS_ALIGN_RIGHT && nst==0) ? AFX_IDW_DOCKBAR_RIGHT : nst;

	DockControlBar( bar, nst, &rect );
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
/*	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("ツール バーの作成に失敗しました。\n");
		return -1;      // 作成できませんでした。
	}
	m_wndToolBar.SetHeight( TOL_HGT );
*/
	if ( !Mfm_Cre_ITB( &m_Inf_tbr, this ) ){
		TRACE0("ツール バーの作成に失敗しました。\n");
		return -1;      // 作成できませんでした。
	}

	if ( !Mfm_Cre_BsB( &m_wndBookingStsBar, this ) ){
		TRACE0("ツール バーの作成に失敗しました。\n");
		return -1;      // 作成できませんでした。
	}

	if ( !Mfm_Cre_RDr( &m_wndRoomDirectBar, this ) ){
		TRACE0("ツール バーの作成に失敗しました。\n");
		return -1;      // 作成できませんでした。
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("ステータス バーの作成に失敗しました。\n");
		return -1;      // 作成できませんでした。
	}

	// TODO: ツール バーをドッキング可能にしない場合は、これらの 3 行を削除してください。
	EnableDocking(CBRS_ALIGN_ANY);

	m_Inf_tbr.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_Inf_tbr);
	m_wndBookingStsBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBarLeftOf( &m_wndBookingStsBar, &m_Inf_tbr );
	m_wndRoomDirectBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBarLeftOf( &m_wndRoomDirectBar, &m_wndBookingStsBar );
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBarLeftOf( &m_wndToolBar, &m_wndRoomDirectBar );

	// Cerate a RoomCharge dialog
	m_pRoomChargeDlg = new CRoomChargeDlg;
	m_pRoomChargeDlg->Create( IDD_DIALOG_ROOMCHARGE, this );

	// Cerate a Category dialog
	m_pCategoryDlg = new CCategoryDlg;
	m_pCategoryDlg->Create( IDD_DIALOG_CATEGORY, this );

	// Cerate a Product dialog
	m_pProductDlg = new CProductDlg;
	m_pProductDlg->Create( IDD_DIALOG_PRODUCT, this );
	
	// Cerate a OperatorInfo dialog
	m_pOperatorInfoDlg = new COperatorInfoDlg;
	m_pOperatorInfoDlg->Create( IDD_DIALOG_OPERATOR, this );

	// Cerate a BranchInfo dialog
	m_pBranchInfoDlg = new CBranchInfoDlg;
	m_pBranchInfoDlg->Create( IDD_DIALOG_BRANCH, this );

	// Cerate a FailityInfo dialog
	m_pFacilityInfoDlg = new CFacilityInfoDlg;
	m_pFacilityInfoDlg->Create( IDD_DIALOG_FACILITY, this );

	// Cerate a EquipmwntType dialog
	m_pEquipmentTypeDlg = new CEquipmentTypeDlg;
	m_pEquipmentTypeDlg->Create( IDD_DIALOG_EQUIPMENTTYPE, this );

	// Cerate a EquipmwntInfo dialog
	m_pEquipmentInfoDlg = new CEquipmentInfoDlg;
	m_pEquipmentInfoDlg->Create( IDD_DIALOG_EQUIPMENTINFO, this );

	// Cerate a CurrencyConversion dialog
	m_pCurrencyCvsDlg = new CCurrencyCvsDlg;
	m_pCurrencyCvsDlg->Create( IDD_DIALOG_CURRENCYCVS, this );

	// Cerate a CurrencyRate dialog
	m_pCurrencyRateDlg = new CCurrencyRateDlg;
	m_pCurrencyRateDlg->Create( IDD_DIALOG_CURRENCYRATE, this );

	// Cerate a TravelAgent dialog
	m_pTravelAgentDlg = new CTravelAgentDlg;
	m_pTravelAgentDlg->Create( IDD_DIALOG_TRAVELAGENT, this );

	// Cerate a CreditCardCompany dialog
	m_pCreditCardComDlg = new CCreditCardComDlg;
	m_pCreditCardComDlg->Create( IDD_DIALOG_CREDITCARDCOM, this );

	// Cerate a employee dialog
	m_pEmployeeDlg = new CEmployeeDlg;
	m_pEmployeeDlg->Create( IDD_DIALOG_EMPLOYEE, this );

	// Cerate a duty dialog
	m_pDutyDlg = new CDutyDlg;
	m_pDutyDlg->Create( IDD_DIALOG_DUTY, this );

	// Cerate a market_segment dialog
	m_pMarketSegmentDlg = new CMarketSegmentDlg;
	m_pMarketSegmentDlg->Create( IDD_DIALOG_MAKETSEGMENT, this );

	Day		dte[1];
	Get_Dte( dte );
	m_Dte_pcr = new CDatePickerDlg( this );
	m_Dte_pcr->Initialize( CDR_DLG_SINGLE, dte->yer, dte->mth );

	m_Ini_stg = IST_INI;
	SetTimer( TEV_INI, TIM_INI, NULL );
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return TRUE;
}


// CMainFrame 診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame メッセージ ハンドラ

// タイマーイベント
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	int				chk=FALSE;

	KillTimer(nIDEvent);

	switch( nIDEvent ){
	case TEV_INI:	chk = Mfm_Ini_Prc( this );	break;
	case TEV_ITV:	chk = Mfm_Itv_Prc( this );	break;
	}

	if ( chk )	CMDIFrameWnd::OnTimer(nIDEvent);
	else		PostMessage( WM_CLOSE, 0, 0 );	//OnClose();
}

// メインフォームクローズ
void CMainFrame::OnClose()
{
	WINDOWPLACEMENT		wpc[1];

	if ( Mfm_Log_Out( this, FALSE ) ){
		if ( m_Dbs_obj->IsOpen() ){
			Dbs_Cut( m_Dbs_obj );
		}

		// Windowの位置・サイズを保存
		GetWindowPlacement( wpc );
		if ( Cnt_Lnk(m_Ini_inf->wpc)==4 ){
			*Rec_Of(int, Idx_Lnk( m_Ini_inf->wpc, 0 )) = wpc->rcNormalPosition.left;
			*Rec_Of(int, Idx_Lnk( m_Ini_inf->wpc, 1 )) = wpc->rcNormalPosition.top;
			*Rec_Of(int, Idx_Lnk( m_Ini_inf->wpc, 2 )) = wpc->rcNormalPosition.right;
			*Rec_Of(int, Idx_Lnk( m_Ini_inf->wpc, 3 )) = wpc->rcNormalPosition.bottom;
		}

		// 全画面状態かどうかを保存
		m_Ini_inf->fls = wpc->showCmd == SW_SHOWMAXIMIZED;

		// each master data dispose
		Del_All( m_Pms_mng->brl );
		Del_All( m_Pms_mng->fcl );
		Del_All( m_Pms_mng->eil );
		Del_All( m_Pms_mng->etl );
		Del_All( m_Pms_mng->agl );
		Del_All( m_Pms_mng->bcl );
		Del_All( m_Pms_mng->ccl );
		Del_All( m_Pms_mng->lgl );
		Del_All( m_Pms_mng->msl );

		Set_PIni( INI_FNM );
		Cst_PIni();

		CMDIFrameWnd::OnClose();
	}
}

// Display period start on Booking status view
void CMainFrame::OnUpdatePeriodfrom(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CChildFrame *)GetActiveFrame())->m_Viw_knd == VWK_BST);
}

// Display period end on Booking status view
void CMainFrame::OnUpdatePeriodto(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CChildFrame *)GetActiveFrame())->m_Viw_knd == VWK_BST);
}

// Add booking on Booking status view
void CMainFrame::OnUpdateBookingAddbooking(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CChildFrame *)GetActiveFrame())->m_Viw_knd == VWK_BST);
}

// Cancel booking on Booking status view
void CMainFrame::OnUpdateBookingCancelbooking(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CChildFrame *)GetActiveFrame())->m_Viw_knd == VWK_BST);
}

// Cut booking on Booking status view
void CMainFrame::OnUpdateBookingCutbooking(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CChildFrame *)GetActiveFrame())->m_Viw_knd == VWK_BST);
}

// Move booking on Booking status view
void CMainFrame::OnUpdateBookingMovebooking(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CChildFrame *)GetActiveFrame())->m_Viw_knd == VWK_BST);
}

void CMainFrame::OnUpdateViewBookingstatusview(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnUpdatePricesettingRoomcharge(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnUpdatePricesettingProductscategory(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnUpdatePricesettingProductsprice(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnUpdateViewSearchview(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnUpdateFileNewreservation(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnUpdateFileLogout(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

void CMainFrame::OnUpdateFileChangePassword(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

// OperatorIifo Setup
void CMainFrame::OnUpdateMasterOperatorinfo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

// BranchIifo Setup
void CMainFrame::OnUpdateMasterBranchinfo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

// FacilityIifo Setup
void CMainFrame::OnUpdateMasterFacilityinfo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

// Equipment type Setup
void CMainFrame::OnUpdateMasterEquipmenttype(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

// Equipment info Setup
void CMainFrame::OnUpdateMasterEquipmentinfo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

// Currency conversion Setup
void CMainFrame::OnUpdateMasterCurrencyConversion(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

// Travel Agent Setup
void CMainFrame::OnUpdateMasterTravelAgent(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

// Credit Card Company Setup
void CMainFrame::OnUpdateMasterCreditCardCompany(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

// Employee Setup
void CMainFrame::OnUpdateMasterEmployeeInfo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

// Duty Setup
void CMainFrame::OnUpdateMasterDutyInfo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

// MarketSegment Setup
void CMainFrame::OnUpdateMasterMarketSegment(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

// Currency rate Setup
void CMainFrame::OnUpdateSettingsCurrencyconversion(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

// Night Audit
void CMainFrame::OnUpdateViewNightAudit(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( !!m_Pms_mng->fci && !!DBS_INT_VAL(m_Pms_mng->fci->dcc) );
}

// House Keeping
void CMainFrame::OnUpdateViewHouseKeeping(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

static	int		exv_cbk( int vkd, CView *viw, LPVOID prm )
{
	return( TRUE );
}

static	int		rvw_cbk( int vkd, CView *viw, LPVOID prm )
{
	int			rsl = FALSE;

	PMS_MNG	*mng = (PMS_MNG *)prm;

	switch( vkd ){
	case VWK_NAD:	rsl = ((CNightAuditView *)viw)->InitData( mng->opc, mng->brc, mng->fcc );	break;
	case VWK_HKP:	rsl = ((CHouseKeepingView *)viw)->InitData( mng->opc, mng->brc, mng->fcc );	break;
	}
	return( rsl );
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	int		exc = FALSE, ope = OPE_CHG;
	static	int	fst=TRUE;
	CBookingStatusView	*bsv;

	bsv = (CBookingStatusView *)Mfm_Get_Bsv( this );

	if (pHandlerInfo == NULL){
		if (nCode == CN_COMMAND){
			switch ( nID ){
			case ID_VIEW_SEARCHVIEW:		if ( m_Sch_viw )	SetTopOfChildren(m_Sch_viw);	break;
			case ID_VIEW_BOOKINGSTATUSVIEW:	if ( m_Bst_viw )	SetTopOfChildren(m_Bst_viw);	break;
			case ID_FILE_NEWRESERVATION:	Mfm_Cre_Rsv( this );								break;
			case ID_BOOKING_ADDBOOKING:		ope |= OPE_CRT;		exc = TRUE;						break;
			case ID_BOOKING_CUTBOOKING:		ope |= OPE_CUT;		exc = TRUE;						break;
			case ID_BOOKING_MOVEBOOKING:	ope |= OPE_MOV;		exc = TRUE;						break;
			case ID_BOOKING_CANCELBOOKING:	if ( bsv ){
												Bst_Cxl_Rsv( bsv, m_Dbs_obj, bsv->m_Bst_mng );
												//Ers_Bok(bsv->m_Bst_mng);
												exc = TRUE;
											}
											break;
			case ID_PRICESETTING_ROOMCHARGE:if ( m_pRoomChargeDlg->InitData( m_Pms_mng->opc, m_Pms_mng->brc, m_Pms_mng->fcc ) ){
												m_pRoomChargeDlg->ShowWindow(SW_NORMAL);
											}
											break;
			case ID_PRICESETTING_PRODUCTSCATEGORY:
											if ( m_pCategoryDlg->InitData( m_Pms_mng->opc, m_Pms_mng->brc, m_Pms_mng->fcc ) ){
												m_pCategoryDlg->ShowWindow(SW_NORMAL);
											}
											break;
			case ID_PRICESETTING_PRODUCTSPRICE:
											if ( m_pProductDlg->InitData( m_Pms_mng->opc, m_Pms_mng->brc, m_Pms_mng->fcc ) ){
												m_pProductDlg->ShowWindow(SW_NORMAL);
											}
											break;
			case ID_MASTER_OPERATORINFO:	if ( m_pOperatorInfoDlg->InitData( m_Pms_mng->opc ) ){
												m_pOperatorInfoDlg->ShowWindow(SW_NORMAL);
											}
											break;
			case ID_MASTER_BRANCHINFO:		if ( m_pBranchInfoDlg->InitData( m_Pms_mng->opc ) ){
												m_pBranchInfoDlg->ShowWindow(SW_NORMAL);
											}
											break;
			case ID_MASTER_FACILITYINFO:	if ( m_pFacilityInfoDlg->InitData( m_Pms_mng->opc ) ){
												m_pFacilityInfoDlg->ShowWindow(SW_NORMAL);
											}
											break;
			case ID_MASTER_EQUIPMENTTYPE:	if ( m_pEquipmentTypeDlg->InitData( m_Pms_mng->opc ) ){
												m_pEquipmentTypeDlg->ShowWindow(SW_NORMAL);
											}
											break;
			case ID_MASTER_EQUIPMENTINFO:	if ( m_pEquipmentInfoDlg->InitData( m_Pms_mng->opc ) ){
												m_pEquipmentInfoDlg->ShowWindow(SW_NORMAL);
											}
											break;
			case ID_MASTER_CURRENCYCONVERSION:
											if ( m_pCurrencyCvsDlg->InitData( m_Pms_mng->opc, m_Pms_mng->brc, m_Pms_mng->fcc ) ){
												m_pCurrencyCvsDlg->ShowWindow(SW_NORMAL);
											}
											break;
			case ID_MASTER_TRAVELAGENT:	if ( m_pTravelAgentDlg->InitData( m_Pms_mng->opc ) ){
												m_pTravelAgentDlg->ShowWindow(SW_NORMAL);
											}
											break;
			case ID_MASTER_CREDITCARDCOMPANY:
											if ( m_pCreditCardComDlg->InitData( m_Pms_mng->opc ) ){
												m_pCreditCardComDlg->ShowWindow(SW_NORMAL);
											}
											break;
			case ID_MASTER_EMPLOYEEINFO:	if ( m_pEmployeeDlg->InitData( m_Pms_mng->opc, m_Pms_mng->brc, m_Pms_mng->fcc ) ){
												m_pEmployeeDlg->ShowWindow(SW_NORMAL);
											}
											break;
			case ID_MASTER_DUTYINFO:		if ( m_pDutyDlg->InitData( m_Pms_mng->opc, m_Pms_mng->brc, m_Pms_mng->fcc ) ){
												m_pDutyDlg->ShowWindow(SW_NORMAL);
											}
											break;
			case ID_MASTER_MARKETSEGMENT:	if ( m_pMarketSegmentDlg->InitData( m_Pms_mng->opc, m_Pms_mng->brc, m_Pms_mng->fcc ) ){
												m_pMarketSegmentDlg->ShowWindow(SW_NORMAL);
											}
											break;
			case ID_SETTINGS_CURRENCYCONVERSION:
											if ( m_pCurrencyRateDlg->InitData( m_Pms_mng->opc, m_Pms_mng->brc, m_Pms_mng->fcc ) ){
												m_pCurrencyRateDlg->ShowWindow(SW_NORMAL);
											}
											break;
			case ID_VIEW_NIGHTAUDIT:		Rnc_Ech_Viw( VWK_NAD, exv_cbk, rvw_cbk, m_Pms_mng );
											break;
			case ID_VIEW_HOUSEKEEPING:		Rnc_Ech_Viw( VWK_HKP, exv_cbk, rvw_cbk, m_Pms_mng );
											break;
			case ID_FILE_LOGOUT:			Mfm_Log_Out( this, FALSE );
											break;
			case ID_FILE_CHANGEPASSWORD:	Mfm_Chg_Pwd( this );
											break;
			}
			if ( exc ){
				if ( bsv ){
					if ( ope != OPE_CHG )	Chg_Mod( ope, bsv->m_Bst_mng );
					bsv->m_Ope_mod = ope;
				}
			}
		}
		else if (nCode == CN_UPDATE_COMMAND_UI){
			if ( nID==ID_BOOKING_ADDBOOKING && !!bsv )	((CCmdUI*)pExtra)->SetCheck( !!(bsv->m_Ope_mod&OPE_CRT) );
			if ( nID==ID_BOOKING_CUTBOOKING && !!bsv )	((CCmdUI*)pExtra)->SetCheck( !!(bsv->m_Ope_mod&OPE_CUT) );
			if ( nID==ID_BOOKING_MOVEBOOKING && !!bsv )	((CCmdUI*)pExtra)->SetCheck( !!(bsv->m_Ope_mod&OPE_MOV) );
		}
	}

	//return ( exc ? exc: CMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) );
	return ( CMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo) );
}

LRESULT CMainFrame::OnEditNotify( WPARAM wParam, LPARAM lParam )
{
	DWORD	cid;
	WCHAR	fbf[20], tbf[20];

	if ( ( cid = HIWORD(wParam) )==TBP_PERIODFROM || cid==TBP_PERIODTO ){
		if ( LOWORD(wParam)==WM_KEYUP ){
			m_wndBookingStsBar.m_Edt_pfr.GetWindowText( fbf, memcnt(fbf) );
			m_wndBookingStsBar.m_Edt_pto.GetWindowText( tbf, memcnt(tbf) );
			((CBookingStatusView *)m_Bst_viw)->OnPeriodChange( fbf, tbf );
		}
		else if ( LOWORD(wParam)!=EN_LBUTTONCLICK ){
			((CBookingStatusView *)m_Bst_viw)->OnPeriodChange( NULL, NULL );
		}
	}
	else{
		if ( LOWORD(wParam)==WM_KEYUP || LOWORD(wParam)==WM_KILLFOCUS ){
			Mfm_Dir_Rch( this, cid );
		}
	}
	return( 0 );
}

LRESULT CMainFrame::OnComboNotify( WPARAM wParam, LPARAM lParam )
{
	CComboBox		*cbx=NULL;
	Csr				csr=NULL;

	if ( LOWORD(wParam)==CN_CHANGED ){
		switch( HIWORD(wParam) ){
		case TBP_ID_BRANCH_COMBO:	cbx = &m_Inf_tbr.m_Cmb_brc;
									m_Pms_mng->bri = (BRC_INF *)(LONG_PTR)cbx->GetItemData(cbx->GetCurSel());
									break;
		case TBP_ID_FACILITY_COMBO:	cbx = &m_Inf_tbr.m_Cmb_fcc;
									m_Pms_mng->fci = (FCL_INF *)(LONG_PTR)cbx->GetItemData(cbx->GetCurSel());
									break;
		}
	}
	else if ( LOWORD(wParam)!=EN_LBUTTONCLICK ){
	}
	return( 0 );
}

LRESULT CMainFrame::OnStaticNotify( WPARAM wParam, LPARAM lParam )
{
	return( TRUE );
}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CMDIFrameWnd::OnLButtonDown(nFlags, point);
}

#define	WM_FLOATSTATUS	0x36D

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	int			idx=0, ret=TRUE;

	if ( pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_RBUTTONDOWN ){
		if ( m_Dte_pcr )	m_Dte_pcr->ShowWindow( SW_HIDE );
	}
	if ( pMsg->message==WM_WINDOWPOSCHANGED ){
		idx = 2;
	}

	if ( pMsg->message!=275 && pMsg->message!=49214 ){
		idx = 3;
	}
	if( WM_KEYDOWN == pMsg->message ){
		switch( pMsg->wParam ){
		case VK_RETURN:	
		case VK_ESCAPE:	ret=FALSE;
						pMsg->wParam=0;
						break;
		default:		break;
		}
	}
	//return ret ? CMDIFrameWnd::PreTranslateMessage(pMsg): 1/*ret*/;
	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

//--------------------------------------------------------------------------------------------------
// Operaton
// set room number and dates into bookingstatus tool bar
void	CMainFrame::SetBookingStsPanel( WCHAR *ptr )
{
	m_wndBookingStsBar.m_Stc_bdt.SetWindowText( ptr );
}

// set display period into bookingstatus tool bar
void	CMainFrame::SetBookingStsPeriod( Day * pfr, Day *pto )
{
	WCHAR		buf[40];

	m_wndBookingStsBar.m_Edt_pfr.SetWindowText( Edt_Dte(buf, memcnt(buf), pfr ) );
	m_wndBookingStsBar.m_Edt_pto.SetWindowText( Edt_Dte(buf, memcnt(buf), pto ) );
}

// set button status into bookingstatus tool bar
void	CMainFrame::SetBookingStsButton( int bck )
{
	CMenu				mnu, *pmn=NULL;
	int					ope = OPE_CHG;
	CBookingStatusView	*bsv = (CBookingStatusView *)Mfm_Get_Bsv( this );

	mnu.LoadMenu(IDR_BOOKINGSTS);
	pmn = mnu.GetSubMenu(2);

	if ( bsv ){
		if ( !!(bck&BSB_CRT) )	ope |= OPE_CRT;
		if ( !!(bck&BSB_CUT) )	ope |= OPE_CUT;
		if ( !!(bck&BSB_MOV) )	ope |= OPE_MOV;

		if ( ope != OPE_CHG )	Chg_Mod( ope, bsv->m_Bst_mng );
		bsv->m_Ope_mod = ope;
	}

	pmn->CheckMenuItem( ID_BOOKING_ADDBOOKING,		MF_BYCOMMAND | ( !!(bck&BSB_CRT) ? MF_CHECKED:MF_UNCHECKED ) );
	pmn->CheckMenuItem( ID_BOOKING_CUTBOOKING,		MF_BYCOMMAND | ( !!(bck&BSB_CUT) ? MF_CHECKED:MF_UNCHECKED ) );
	pmn->CheckMenuItem( ID_BOOKING_MOVEBOOKING,		MF_BYCOMMAND | ( !!(bck&BSB_MOV) ? MF_CHECKED:MF_UNCHECKED ) );
	pmn->CheckMenuItem( ID_BOOKING_CANCELBOOKING,	MF_BYCOMMAND | ( !!(bck&BSB_ERS) ? MF_CHECKED:MF_UNCHECKED ) );

	m_wndBookingStsBar.SendMessage(TB_CHECKBUTTON, ID_BOOKING_ADDBOOKING, !!(bck&BSB_CRT));
	m_wndBookingStsBar.SendMessage(TB_CHECKBUTTON, ID_BOOKING_CUTBOOKING, !!(bck&BSB_CUT));
	m_wndBookingStsBar.SendMessage(TB_CHECKBUTTON, ID_BOOKING_MOVEBOOKING, !!(bck&BSB_MOV));
	m_wndBookingStsBar.SendMessage(TB_CHECKBUTTON, ID_BOOKING_CANCELBOOKING, !!(bck&BSB_ERS));
}

// set business_date into info tool bar
void	CMainFrame::SetBusinessDate( Day *dte )
{
	WCHAR			buf[40];

	m_Inf_tbr.m_Stc_bdt.SetWindowText( Edt_Dte( buf, memcnt(buf), dte ) );
}

// set business_date into info tool bar
void	CMainFrame::SetLoginUser( EMP_LYE *epl )
{
	WCHAR			nam[520];

	if ( epl ){
		swprintf_s( nam, memcnt(nam), _T("%s %s"), EDT_DBF_WST(epl->fnm,_T("")), EDT_DBF_WST(epl->lnm,_T("")) );
	}
	else{
		*nam = 0;
	}
	m_Inf_tbr.m_Lgi_usr.SetWindowText( nam );
}

