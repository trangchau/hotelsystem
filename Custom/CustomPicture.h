#if !defined(AFX_PICTURE_H__E4DDD256_DB1C_4CEB_9B47_65D0622757E2__INCLUDED_)
#define AFX_PICTURE_H__E4DDD256_DB1C_4CEB_9B47_65D0622757E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomPicture.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CCustomPicture ウィンドウ

class CCustomPicture : public CWnd
{
private:
	int			m_Init;
	RECT		m_Rect;
	CBitmap		m_Bitmap;
	CBitmap		m_BitMask;
	int			m_Width;
	int			m_Height;

// コンストラクション
public:
	DWORD		m_UserData;
	CCustomPicture();

// アトリビュート
public:

// オペレーション
public:
	void	SetBitmap( UINT nResource );		// リソース

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CPicture)
	public:
	virtual BOOL Create( const RECT& rect, CWnd* pParentWnd, UINT nID);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CCustomPicture();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CPicture)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_PICTURE_H__E4DDD256_DB1C_4CEB_9B47_65D0622757E2__INCLUDED_)
