#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolBarCombo.h : ヘッダー ファイル
//

#include "..\Custom\CustomCombo.h"

/////////////////////////////////////////////////////////////////////////////
// CToolBarCombo ウィンドウ

//class CToolBarCombo : public CComboBox
class CToolBarCombo : public CCustomCombo
{
// コンストラクション
public:
	CToolBarCombo();
	CWnd		*m_NotifyWnd;

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CToolBarCombo)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CToolBarCombo();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CToolBarCombo)
	afx_msg void OnSelchange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。
