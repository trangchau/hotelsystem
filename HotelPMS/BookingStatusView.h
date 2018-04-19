// BookingStatusView.h : CBookingStatusView クラスのインターフェイス
//


#pragma once

#include ".\fig\fig_mnp.h"
#include ".\fnc\bst_gph.h"
#include "..\pms_def.h"

class CBookingStatusView : public CView
{
protected: // シリアル化からのみ作成します。
	CBookingStatusView();
	DECLARE_DYNCREATE(CBookingStatusView)

// 属性
public:
	CBookingStatusDoc* GetDocument() const;

	PMS_MNG		*m_pPms_mng;			// PMS Management segment
	CDatabase	*m_pDbs_obj;			// Database object
	int			m_Ope_mod;				// Current operation code of graphic

//private:
	BST_MNG		*m_Bst_mng;				// Booking status graphic management structure
	int			m_Rdn_rii;				// reservation_infos id on hitted when right button pushed
	int			m_Rdn_rdi;				// reservation_details id on hitted when right button pushed
	BST_ROM_STS	*m_Rdn_sts;				// Pointer of BST_ROM_STS on hitted when right button pushed

	void		SetScrollRenge( int, int );		// スクロール状態設定

// 操作
public:

	void		ReDraw( void );
	void		OnPeriodChange( WCHAR *, WCHAR * );

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
	virtual ~CBookingStatusView();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
//	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void PostNcDestroy();

// 生成された、メッセージ割り当て関数
protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnGraphNotify( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnViewNotify( WPARAM wParam, LPARAM lParam );
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
//	afx_msg BOOL OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // BookingStatusView.cpp のデバッグ バージョン
inline CBookingStatusDoc* CBookingStatusView::GetDocument() const
   { return reinterpret_cast<CBookingStatusDoc*>(m_pDocument); }
#endif

