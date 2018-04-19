#if !defined(AFX_CUSTOMLISTBOX_H__7819539E_A89B_450E_AB33_1D1EE6630E13__INCLUDED_)
#define AFX_CUSTOMLISTBOX_H__7819539E_A89B_450E_AB33_1D1EE6630E13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomListBox.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CCustomListBox ウィンドウ

class CCustomListBox : public CListBox
{
private:
	LOGFONT		m_LogFont;
	COLORREF	m_fgColor;
	HBRUSH		m_bgBrush;
	CFont		m_Font; 
// コンストラクション
public:
	CCustomListBox();
	void	ResetFont( HFONT hFont );

// アトリビュート
public:

// オペレーション
public:
	// 文字にアンダーラインをつける
	BOOL SetTextUnderLine( BOOL bflg = FALSE);
	// 文字をイタリックにする
	BOOL SetTextItalic( BOOL bflg = FALSE);
	// 高さ設定
	BOOL SetTextHeight(int height);
	// 背景色の指定
	BOOL SetBackColor( COLORREF color);
	// 文字をボールドにする
	BOOL SetTextBold( BOOL bflg = FALSE);
	// 文字の色を指定する
    BOOL SetForeColor( COLORREF color);
	// 初期化・サブクラス化
	void InitSubclass( UINT, COLORREF, COLORREF, int, int, CWnd * );
	void SetFontByHandle( HFONT hFont, bool rdw );
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCustomListBox)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CCustomListBox();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CCustomListBox)
//	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CUSTOMLISTBOX_H__7819539E_A89B_450E_AB33_1D1EE6630E13__INCLUDED_)
