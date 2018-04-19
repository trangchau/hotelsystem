// HotelPMSView.h : CHotelPMSView クラスのインターフェイス
//


#pragma once

#include "stdafx.h"
#include "HotelPMSDoc.h"

class CHotelPMSView : public CFormView
{
protected: // シリアル化からのみ作成します。
	CHotelPMSView();
	DECLARE_DYNCREATE(CHotelPMSView)

	// フォーム データ
public:
	//{{AFX_DATA(CHotelPMSView)
	enum { IDD = IDD_ABOUTBOX };
		// メモ: ClassWizard はこの位置にデータメンバを追加します。
	//}}AFX_DATA

// 属性
public:
	CHotelPMSDoc* GetDocument() const;

// 操作
public:
	int			GetData( int );			// Getting data for exit on exist data
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
	virtual ~CHotelPMSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // HotelPMSView.cpp のデバッグ バージョン
inline CHotelPMSDoc* CHotelPMSView::GetDocument() const
   { return reinterpret_cast<CHotelPMSDoc*>(m_pDocument); }
#endif

