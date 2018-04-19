#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RsvSchPanel.h : ヘッダー ファイル
//

#include "..\Custom\SimpleGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CRsvSchPanel ウィンドウ

class CRsvSchPanel : public CStatic
{
private:

	// コンストラクション
public:
	CRsvSchPanel();

// アトリビュート
public:

// オペレーション
public:

	// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRsvSchPanel)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CRsvSchPanel();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CRsvSchPanel)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。
