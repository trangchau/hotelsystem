// HouseKeepingView.h : CHouseKeepingView クラスのインターフェイス
//


#pragma once

#include "..\Custom\CustomEdit.h"
#include "..\Custom\DateEdit.h"
#include "..\Custom\CustomStatic.h"
#include "..\Custom\SimpleGrid.h"
#include ".\fnc\viw_cmn.h"
#include ".\fnc\inh_fnc.h"
#include ".\fnc\inh_sql.h"
#include "HouseKeepingDoc.h"
#include "CashierDlg.h"


class CHouseKeepingView : public CFormView
{
protected: // シリアル化からのみ作成します。
	CHouseKeepingView();
	DECLARE_DYNCREATE(CHouseKeepingView)

private:
// 内部変数

	void		SetupComponent( void );

// フォーム データ
public:
	//{{AFX_DATA(CHouseKeepingView)
	enum { IDD = IDD_DLG_HOUSEKEEPING };
		// メモ: ClassWizard はこの位置にデータメンバを追加します。
	//}}AFX_DATA

	PMS_MNG		*m_pPms_mng;			// PMS Management segment
	CDatabase	*m_pDbs_obj;			// Database object

	//HKP_RCH_LST	*m_Rdn_wrn;				// warning list pointer when rbutton clicked

	WCHAR		*m_Ope_cod;
	WCHAR		*m_Brc_cod;				// target branch code
	WCHAR		*m_Fcl_cod;				// target facility code

	CSimpleGrid	*m_Hkp_grd;
	CSimpleGrid	*m_Rch_grd;

	Lnk			m_Hkp_rot[1];
	Lnk			m_Rch_rot[1];

	STS_CDN		*m_Cln_sts;
	STS_CDN		*m_Occ_sts;
	STS_CDN		*m_Sal_sts;
	int			m_Cln_cnt;
	int			m_Occ_cnt;
	int			m_Sal_cnt;

	int			InitData( WCHAR *opc, WCHAR *brc, WCHAR *fcc );


	// 属性
public:
	CHouseKeepingDoc* GetDocument() const;

// 操作
public:
	//void ResizeParentToFit(BOOL bShrinkOnly = TRUE);
	int			GetData( int, int, int );	// Getting data for exit on exist data
	int			CreateData( void );			// Creating a new reservation data

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
	virtual ~CHouseKeepingView();
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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
	afx_msg LRESULT OnViewNotify( WPARAM, LPARAM );
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCheckAllselect();
};

#ifndef _DEBUG  // HotelPMSView.cpp のデバッグ バージョン
inline CHotelPMSDoc* CHouseKeepingView::GetDocument() const
   { return reinterpret_cast<CHotelPMSDoc*>(m_pDocument); }
#endif

