#if !defined(AFX_GRPHVIEW_H__60946D96_F2BA_11D5_B320_00508B04D67D__INCLUDED_)
#define AFX_GRPHVIEW_H__60946D96_F2BA_11D5_B320_00508B04D67D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GrphView.h : ヘッダー ファイル
//
#include "..\Graph\fig_mnp.h"
#include "..\Graph\gph_dsp.h"
#include ".\fnc\gph_sub.h"

/////////////////////////////////////////////////////////////////////////////
// CGrphView ビュー

class CGrphView : public CView
{
private:
	BST_MNG		*m_Bst_mng;
	int			m_OpeMode;					// 現在操作モード
	void		CalcScrollRenge( int );		// スクロール状態設定

protected:
	CGrphView();           // 動的生成に使用されるプロテクト コンストラクタ
	DECLARE_DYNCREATE(CGrphView)

// アトリビュート
public:
//	Mng_dat		*m_Mng_dat;	// 勤怠管理データ

// オペレーション
public:
	void		Refresh( int bReset );	// 表示内容一新

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CGrphView)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void OnDraw(CDC* pDC);      // このビューを描画するためにオーバーライドしました。
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	virtual ~CGrphView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CGrphView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_GRPHVIEW_H__60946D96_F2BA_11D5_B320_00508B04D67D__INCLUDED_)
