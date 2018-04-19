// CheckoutView.h : CCheckoutView クラスのインターフェイス
//


#pragma once

#include "..\Custom\CustomEdit.h"
#include "..\Custom\DateEdit.h"
#include "..\Custom\CustomStatic.h"
#include "..\Custom\SimpleGrid.h"
#include ".\fnc\viw_cmn.h"
#include ".\fnc\cot_fnc.h"
#include ".\fnc\cot_sql.h"
#include "CheckoutDoc.h"
#include "CashierDlg.h"


class CCheckoutView : public CFormView
{
protected: // シリアル化からのみ作成します。
	CCheckoutView();
	DECLARE_DYNCREATE(CCheckoutView)

private:
// 内部変数

	CSimpleGrid			*m_Cot_grd;

	INH_INF_DAT			m_Rsv_inf[1];
	int					m_Sel_row;		// Row number of grd on with selecting 

	CCashierDlg			*m_Csh_ier;

	CCustomStatic		m_Rsv_nam;
	CCustomStatic		m_Wrn_ing;

	int					m_Rsv_did;

	void				OpentCashier( int );

// フォーム データ
public:
	//{{AFX_DATA(CCheckoutView)
	enum { IDD = IDD_DLG_CHECKOUT };
		// メモ: ClassWizard はこの位置にデータメンバを追加します。
	//}}AFX_DATA

	// 属性
public:
	CCheckoutDoc* GetDocument() const;

	PMS_MNG		*m_pPms_mng;			// PMS Management segment
	CDatabase	*m_pDbs_obj;			// Database object

	Lnk			m_Rsv_dtl[1];

// 操作
public:
	//void ResizeParentToFit(BOOL bShrinkOnly = TRUE);
	int			GetData( int, int );	// Getting data for exit on exist data
	int			CreateData( void );		// Creating a new reservation data

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
	virtual ~CCheckoutView();
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
	afx_msg void OnBnClickedCancel();
protected:
	virtual void PostNcDestroy();
public:
//	afx_msg void OnTcnSelchangeTabRsvDetail(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
	afx_msg LRESULT OnCashierNotify( WPARAM, LPARAM );
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#ifndef _DEBUG  // HotelPMSView.cpp のデバッグ バージョン
inline CHotelPMSDoc* CCheckoutView::GetDocument() const
   { return reinterpret_cast<CHotelPMSDoc*>(m_pDocument); }
#endif

