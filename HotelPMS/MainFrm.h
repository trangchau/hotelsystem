// MainFrm.h : CMainFrame クラスのインターフェイス
//


#pragma once

#include "resource.h"
#include "..\pms_ini.h"				// 初期化情報処理
#include "..\pms_def.h"				// 初期化情報処理
#include "..\Common\dbs_mng.h"		// データベース処理
#include "..\Custom\CustomEdit.h"	// Original edit box
#include "..\Custom\CustomStatic.h"	// Original edit box

#include "..\PMSCustom\ToolBarCombo.h"		// データベース処理
#include "RoomChargeDlg.h"
#include "CategoryDlg.h"
#include "OperatorInfoDlg.h"
#include "BranchInfoDlg.h"
#include "FacilityInfoDlg.h"
#include "EquipmentTypeDlg.h"
#include "EquipmentInfoDlg.h"
#include "DatePickerDlg.h"
#include "ProductDlg.h"
#include "CurrencyCvsDlg.h"
#include "CurrencyRateDlg.h"
#include "TravelAgentDlg.h"
#include "CreditCardComDlg.h"
#include "EmployeeDlg.h"
#include "DutyDlg.h"
#include "MarketSegmentDlg.h"

// Tool bar parts definition
//#define	TBP_ID_BDATE_STATIC				32768
//#define	TBP_ID_BRANCH_COMBO				(TBP_ID_BDATE_STATIC+1)
//#define	TBP_ID_FACILITY_COMBO			(TBP_ID_BRANCH_COMBO+1)
//#define	TBP_ID_ROOMING_STATIC			(TBP_ID_FACILITY_COMBO+1)
//#define	TBP_ID_DCRCIN_EDIT				(TBP_ID_ROOMING_STATIC+1)
//#define	TBP_ID_DCRINH_EDIT				(TBP_ID_DCRCIN_EDIT+1)
//#define	TBP_ID_DCRCOT_EDIT				(TBP_ID_DCRINH_EDIT+1)

#define	BSB_CRT			1		// create button
#define	BSB_CUT			2		// cut button
#define	BSB_MOV			4		// move button
#define	BSB_ERS			8		// erase button

class CInfoToolBar : public CToolBar
{
public:
	CCustomStatic		m_Stc_bdt;
	CCustomStatic		m_Lgi_usr;
	//CToolBarCombo		m_Cmb_brc;
	//CToolBarCombo		m_Cmb_fcc;
	CCustomCombo		m_Cmb_brc;
	CCustomCombo		m_Cmb_fcc;
	CFont				m_Cmb_font;
	CFont				m_Stc_font;
	CFont				m_Lgi_font;
	afx_msg LRESULT OnComboNotify( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnStaticNotify( WPARAM wParam, LPARAM lParam );
protected:
	DECLARE_MESSAGE_MAP()
};

class CBookingStsToolBar : public CToolBar
{
public:
	CDateEdit			m_Edt_pfr;		// display period start
	CStatic				m_Stc_btw;
	CDateEdit			m_Edt_pto;		// display period end
	CStatic				m_Stc_bdt;
	CFont				m_font;
	afx_msg LRESULT OnEditNotify( WPARAM wParam, LPARAM lParam );
protected:
	DECLARE_MESSAGE_MAP()
};

class CRoomDirectToolBar : public CToolBar
{
public:
	CStatic				m_Stc_cin;
	CCustomEdit			m_Edt_cin;
	CStatic				m_Stc_inh;
	CCustomEdit			m_Edt_inh;
	CStatic				m_Stc_cot;
	CCustomEdit			m_Edt_cot;
	CFont				m_font;
	afx_msg LRESULT OnEditNotify( WPARAM wParam, LPARAM lParam );
protected:
	DECLARE_MESSAGE_MAP()
};


class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// 属性
public:
	CInfoToolBar		m_Inf_tbr;
	CRoomDirectToolBar  m_wndRoomDirectBar;
	Ini_fil				*m_Ini_inf;
	PMS_MNG				m_Pms_mng[1];
	CDatabase			m_Dbs_obj[1];

	WCHAR				m_Dbs_pth[128];

	int					m_Ini_stg;				// Stage of initial process
	int					m_GBd_cnt;				// Count of getting business date interval

	CView				*m_Sch_viw;				// Search view handle
	CView				*m_Bst_viw;				// Booking status view handle
	CView				*m_Nad_viw;				// Night ausit view handle

	CDatePickerDlg		*m_Dte_pcr;

// 操作
public:
	void		SetBookingStsPanel( WCHAR *ptr );
	void		SetBookingStsPeriod( Day *, Day * );
	void		SetBookingStsButton( int );
	void		SetBusinessDate( Day * );
	void		SetLoginUser( EMP_LYE * );

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ローカル変数・メソッド
private:

	//THREAD_START_ROUTINE	DBConnect( LPVOID );	// データベース接続スレッド


// 実装
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // コントロール バー用メンバ
	CStatusBar			m_wndStatusBar;
	CToolBar			m_wndToolBar;
	CBookingStsToolBar  m_wndBookingStsBar;

	CRoomChargeDlg		*m_pRoomChargeDlg;
	CCategoryDlg		*m_pCategoryDlg;
	CProductDlg			*m_pProductDlg;
	COperatorInfoDlg	*m_pOperatorInfoDlg;
	CBranchInfoDlg		*m_pBranchInfoDlg;
	CFacilityInfoDlg	*m_pFacilityInfoDlg;
	CEquipmentTypeDlg	*m_pEquipmentTypeDlg;
	CEquipmentInfoDlg	*m_pEquipmentInfoDlg;
	CCurrencyCvsDlg		*m_pCurrencyCvsDlg;
	CCurrencyRateDlg	*m_pCurrencyRateDlg;
	CTravelAgentDlg		*m_pTravelAgentDlg;
	CCreditCardComDlg	*m_pCreditCardComDlg;
	CEmployeeDlg		*m_pEmployeeDlg;
	CDutyDlg			*m_pDutyDlg;
	CMarketSegmentDlg	*m_pMarketSegmentDlg;

	void	DockControlBarLeftOf( CToolBar* Bar, CToolBar* LeftOf );


// 生成された、メッセージ割り当て関数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnUpdateBookingAddbooking(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBookingCancelbooking(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBookingCutbooking(CCmdUI *pCmdUI);
	afx_msg void OnUpdateBookingMovebooking(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePricesettingRoomcharge(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePricesettingProductscategory(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePricesettingProductsprice(CCmdUI *pCmdUI);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnUpdateViewBookingstatusview(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewSearchview(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileNewreservation(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileLogout(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileChangePassword(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePeriodfrom(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePeriodto(CCmdUI *pCmdUI);
	afx_msg LRESULT OnEditNotify( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnComboNotify( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnStaticNotify( WPARAM wParam, LPARAM lParam );
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnUpdateMasterOperatorinfo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMasterBranchinfo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMasterFacilityinfo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMasterEquipmenttype(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMasterEquipmentinfo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMasterCurrencyConversion(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMasterTravelAgent(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMasterCreditCardCompany(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMasterEmployeeInfo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMasterDutyInfo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMasterMarketSegment(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSettingsCurrencyconversion(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewNightAudit(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewHouseKeeping(CCmdUI *pCmdUI);
};


