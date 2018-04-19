#if !defined(AFX_KOSUSTATIC_H__1238DD05_39B7_438E_8E83_8ABC6DA72EA2__INCLUDED_)
#define AFX_KOSUSTATIC_H__1238DD05_39B7_438E_8E83_8ABC6DA72EA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomStatic.h : ヘッダー ファイル
//

#define	WM_STATICNOTIFY		(WM_USER+32777)

#define	SN_LBUTTONCLICK		0x1000				// 右ボタンクリック
#define	SN_RBUTTONCLICK		0x1001				// 右ボタンクリック
#define	SN_EDITCOMPLETE		0x1002				// 編集完了

#define ON_WM_STATICNOTIFY() \
	{ WM_STATICNOTIFY, 0, 0, 0, AfxSig_lwl, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< LRESULT (AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM) > \
		( &ThisClass :: OnStaticNotify)) },

#define	wsizeof(x)			((x)*sizeof(WCHAR))

/////////////////////////////////////////////////////////////////////////////
// CCustomStatic ウィンドウ

class CCustomStatic : public CStatic
{
private:
	CFont		m_Font;
	LOGFONT		m_LogFont;
	COLORREF	m_fgColor;
	HBRUSH		m_bgBrush;
	COLORREF	m_ftColor;
// コンストラクション
public:
	CCustomStatic();
	void	ResetFont( HFONT hFont );
	int			AutoSize;

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
	void SetBGColor(COLORREF bgColor);
	void SetFTColor(COLORREF fcColor);
	void SetFont(  WCHAR *pName, int nSize, COLORREF nFontColor );
	void SetText(  WCHAR *pText );
	//void SetFontByHandle( HFONT hFont, bool rdw );
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCustomStatic)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CCustomStatic();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CCustomStatic)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_KOSUSTATIC_H__1238DD05_39B7_438E_8E83_8ABC6DA72EA2__INCLUDED_)
