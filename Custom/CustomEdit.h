#pragma once

#define	WM_EDITNOTIFY		(WM_USER+32767)

#define	EN_LBUTTONCLICK		0x1000				// 右ボタンクリック
#define	EN_RBUTTONCLICK		0x1001				// 右ボタンクリック
#define	EN_EDITCOMPLETE		0x1002				// 編集完了

#define ON_WM_EDITNOTIFY() \
	{ WM_EDITNOTIFY, 0, 0, 0, AfxSig_lwl, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< LRESULT (AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM) > \
		( &ThisClass :: OnEditNotify)) },


class CCustomEdit : public CEdit
{
	DECLARE_DYNAMIC(CCustomEdit)
private:
	CFont		m_Font;
	LOGFONT		m_LogFont;
	COLORREF	m_ftColor;
	HBRUSH		m_bgBrush;

public:
	CCustomEdit();
	virtual ~CCustomEdit();
	virtual void SetFont( WCHAR *pName, int nSize, COLORREF );
	virtual void SetColor( COLORREF );
	virtual	void SetUserData( DWORD nData );

	DWORD		m_UserData;
	WCHAR		m_Txt_dat[1024];	// only for CSimpleGrid
	int			m_Txt_siz;	// only for CSimpleGrid

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	void InitSubclass(unsigned int nID, CWnd * pParent);
	void SetBGColor(COLORREF bgColor);
public:
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};


