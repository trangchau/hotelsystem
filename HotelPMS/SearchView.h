// HotelPMSView.h : CSearchView クラスのインターフェイス
//


#pragma once

#include "..\Common\bsc_def.h"
#include "..\Common\lnk_mng.h"
#include "..\Custom\SimpleGrid.h"
#include "..\PMSCustom\RsvSchPanel.h"
#include "..\PMSCustom\BookingPathCombo.h"
#include "..\Custom\DateEdit.h"
#include "..\pms_def.h"
#include ".\fnc\sch_fnc.h"
#include ".\fnc\sch_sql.h"

class CSearchView : public CFormView
{
protected: // シリアル化からのみ作成します。
	CSearchView();
	DECLARE_DYNCREATE(CSearchView)

private:
	CDateEdit		m_Rsv_avf;
	CDateEdit		m_Rsv_avt;
	CDateEdit		m_Rsv_dpf;
	CDateEdit		m_Rsv_dpt;

	CRsvSchPanel	m_Rsv_pnl;		// Reservation panel
	CRsvSchPanel	m_Cin_pnl;		// Checkin today panel
	CRsvSchPanel	m_Inh_pnl;		// Inhouse panel
	CRsvSchPanel	m_Cot_pnl;		// Checkout today panel

	CBookingPathCombo	m_Rsv_pth;
	CBookingPathCombo	m_Cin_pth;
	CBookingPathCombo	m_Inh_pth;
	CBookingPathCombo	m_Cot_pth;

// フォーム データ
public:
	//{{AFX_DATA(CSearchView)
	enum { IDD = IDD_DLG_SEARCH };
		// メモ: ClassWizard はこの位置にデータメンバを追加します。
	//}}AFX_DATA

	// 属性
public:
	CSearchDoc* GetDocument() const;

	Lnk			m_Rsv_rot[1];			// 予約データ根
	Lnk			m_Cin_rot[1];			// チェックインデータ根
	Lnk			m_Inh_rot[1];			// インハウスデータ根
	Lnk			m_Cot_rot[1];			// チェックアウトデータ根

	int			m_Rdn_rii;				// reservation_info_id when clecked the right button of mouse
	int			m_Rdn_rdi;				// reservation_detail_id when clecked the right button of mouse
	SCH_RSV_LST	*m_Rdn_rsv;				// reservation information when clecked the right button of mouse

	PMS_MNG		*m_pPms_mng;			// PMS Management segment
	CDatabase	*m_pDbs_obj;			// Database object

	CSimpleGrid	*m_Rsv_grd;				// 予約検索結果グリッド
	CSimpleGrid	*m_Cin_grd;				// チェックイン検索結果グリッド
	CSimpleGrid	*m_Inh_grd;				// インハウス検索結果グリッド
	CSimpleGrid	*m_Cot_grd;				// チェックアウト検索結果グリッド

	CTabCtrl	m_Knd_tab;


// 操作
public:
	//void ResizeParentToFit(BOOL bShrinkOnly = TRUE);

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 実装
public:
	virtual ~CSearchView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTabKind(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
	afx_msg LRESULT OnViewNotify( WPARAM, LPARAM );
protected:
	virtual void PostNcDestroy();
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonResearch();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonDefault2();
};

#ifndef _DEBUG  // HotelPMSView.cpp のデバッグ バージョン
inline CHotelPMSDoc* CSearchView::GetDocument() const
   { return reinterpret_cast<CHotelPMSDoc*>(m_pDocument); }
#endif

