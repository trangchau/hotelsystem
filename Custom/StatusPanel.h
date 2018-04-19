#if !defined(AFX_STATUSPANEL_H__59335514_1878_4C69_AD17_519CFC9CD4B9__INCLUDED_)
#define AFX_STATUSPANEL_H__59335514_1878_4C69_AD17_519CFC9CD4B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatusPanel.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CStatusPanel ウィンドウ

class CStatusPanel : public CWnd
{
private:
	int			m_Init;
	RECT		m_Rect;
//	CBitmap		m_Bitmap;
	CBitmap		m_BitMask;
	int			m_Width;
	int			m_Height;

	UINT		m_FrameID[1];
	UINT		m_StatusID[3][2];
	int			m_Status[3];
	POINT		m_BmpSize[3];
	CBitmap		m_FrmBitmap[1];
	CBitmap		m_StsBitmap[3][2];

	// コンストラクション
public:
	DWORD		m_UserData;
	void		SetFrameID( UINT );
	void		SetStatusID( int, UINT, UINT, int );
	void		UpdteStatus( int, int );
	CStatusPanel();

// アトリビュート
public:

// オペレーション
public:
	void	SetBitmap( UINT );

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CStatusPanel)
	public:
	virtual BOOL Create( const RECT& rect, CWnd* pParentWnd, UINT nID);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CStatusPanel();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CStatusPanel)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STATUSPANEL_H__59335514_1878_4C69_AD17_519CFC9CD4B9__INCLUDED_)
