#pragma once

#define	WM_COMBONOTIFY		(WM_USER+32781)

#define	CN_RBUTTONCLICK		1					// 右ボタンクリック
#define	CN_CHANGED			2					// 編集完了

#define ON_WM_COMBONOTIFY() \
	{ WM_COMBONOTIFY, 0, 0, 0, AfxSig_lwl, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< LRESULT (AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM) > \
		( &ThisClass :: OnComboNotify)) },


// CCustomCombo

class CCustomCombo : public CComboBox
{
	DECLARE_DYNAMIC(CCustomCombo)
private:
	CFont		m_Font;
	LOGFONT		m_LogFont;
	COLORREF	m_ftColor;
	HBRUSH		m_bgBrush;

public:
	CCustomCombo();
	virtual ~CCustomCombo();
	virtual void SetFont( WCHAR *pName, int nSize, COLORREF );
	void InitSubclass(unsigned int nID, CWnd * pParent);
	void SetBGColor(COLORREF bgColor);
	virtual void SetColor( COLORREF );

protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnCbnSelchange();
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
};


