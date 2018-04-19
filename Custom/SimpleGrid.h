#pragma once


// CSimpleGrid

#include "CustomEdit.h"
#include "DateEdit.h"
#include "TimeEdit.h"
#include "CommaEdit.h"
#include "CustomCombo.h"

#define	GAL_LFT		(DT_LEFT|DT_SINGLELINE|DT_VCENTER)
#define	GAL_CTR		(DT_CENTER|DT_SINGLELINE|DT_VCENTER)
#define	GAL_RGT		(DT_RIGHT|DT_SINGLELINE|DT_VCENTER)

#define	GCT_NML		0			// 通常セル
#define	GCT_EDT		1			// 入力可能セル
#define	GCT_DED		2			// 日付入力可能セル
#define	GCT_TED		4			// 時刻入力可能セル
#define	GCT_CED		8			// Comma edit cell
#define	GCT_CHK		16			// チェックボックスセル
#define	GCT_CMB		32			// コンボボックスセル
#define	GCT_CLR		64			// 色選択セル
#define	GCT_PNS		128			// 線種選択セル

#define	GCT_CDF		-1			// カラム定義に従う

#define	GCT_NMO		512			// 数値入力のみ

#define	GSL_NON		0			// 選択なし
#define	GSL_ROW		1			// 行選択方式
#define	GSL_CLM		2			// 列選択方式


typedef	struct	{
	int			wid;			// 列幅
	int			alg;			// オリジン
	int			typ;			// セルタイプ
	int			dgt;			// 桁数（バイト数）
}GRD_CLM;

class CSimpleGrid;

typedef	struct	{				// グリッド定義
	int			vlc;			// 可視行数（ヘッダー行含む）
	int			vcc;			// 可視桁数
	int			lnh;			// １行高さ
	int			sel;			// 選択方式
	COLORREF	hbc;			// ヘッダー背景色
	COLORREF	hfc;			// ヘッダー文字色
	COLORREF	dbc;			// 明細背景色
	COLORREF	dfc;			// 明細文字色
	COLORREF	sbc;			// 選択状態背景色
	COLORREF	sfc;			// 選択状態文字色
	int			fsa;			// フォントサイズ自動調整
	int			hdr;			// ヘッダー有無
	int			cct;			// カラム数
	GRD_CLM		*clm;			// カラム定義
}GRD_DEF;

typedef	struct	{				// グリッド通知構造体
	int			ntf;			// 通知コード
	int			cid;			// コントロールＩＤ
	int			msx;			// マウス座標（親ウィンドウ上相対位置）
	int			msy;			// マウス座標（親ウィンドウ上相対位置）
	int			clm;			// カラム位置（絶対位置）
	int			row;			// ロー位置（絶対位置）
	CSimpleGrid	*grd;			// Instance of own grid
}GRD_NTF;

#define	WM_GRIDNOTIFY		(WM_USER+16384)		// グリッド通知メッセージ

#define	GN_LBUTTONCLICK		1					// 左ボタンクリック
#define	GN_RBUTTONCLICK		2					// 右ボタンクリック
#define	GN_EDITCOMPLETE		3					// 編集完了
#define	GN_QUERYCOMBODATA	4					// コンボボックスデータ要求
#define	GN_COMBOCHANGED		5					// コンボボックス変更完了
#define	GN_QUERYINPUTTYPE	6					// 入力方式要求
#define	GN_QUERYCMEPARAM	7					// 入力方式要求
#define	GN_QUERYCOLORDATA	8					// 色データ要求
#define	GN_SELECTCHANGE		9					// Select change

#define	GN_LBUTTONDBLCLICK	10					// 左ボタンダブルクリック

#define ON_WM_GRIDNOTIFY() \
	{ WM_GRIDNOTIFY, 0, 0, 0, AfxSig_lwl, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< LRESULT (AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM) > \
		( &ThisClass :: OnGridNotify)) },

class CSimpleGrid : public CWnd
{
	DECLARE_DYNAMIC(CSimpleGrid)
private:
	CWnd		*m_pParent;
	RECT		m_Size[1];
	GRD_DEF		m_Grd_def[1];
	HFONT		m_Font;

	int				m_TopRow;			// 表示先頭行（１からスタート）
	int				m_LeftColumn;		// 表示先頭カラム（０からスタート）
	int				m_LineCount;		// 現在の全行数（ヘッダー含む）
	WCHAR			*m_Str_tbl;			// 表示文字列テーブル
	int				*m_Val_tbl;			// 項目データテーブル
	char			*m_Enb_tbl;			// カラム操作有効データ
	COLORREF		*m_Fcl_tbl;			// フォント色テーブル
	COLORREF		*m_Bcl_tbl;			// 背景色テーブル
	int				m_VScroll;			// 垂直スクロールあり
	int				m_HScroll;			// 水平スクロールあり
	int				m_SelRow;			// 選択行
	int				m_SelColumn;		// 選択列
	int				m_InputEnable;		// 入力有効フラグ
	int				m_Fst_flg;			// 最初の呼出しフラグ
	CCustomEdit		*m_EditBox;			// エディットボックス
	CDateEdit		*m_DateEditBox;		// エディットボックス
	CTimeEdit		*m_TimeEditBox;		// エディットボックス
	CCommaEdit		*m_CommaEditBox;	// エディットボックス
	CCustomCombo	*m_ComboBox;		// コンボボックス
	int				m_EditFlg;			// 編集中フラグ
	WCHAR			m_InputChar;		// 選択状態で押した文字キー

	void	CalcCellRect( RECT *, int, int );
	int		MPointToCell( int *, int *, int, int );
	void	SetVScrollBar( void );
	void	SetHScrollBar( void );
	LRESULT	SendNotify( int, int, int, int, int, LPARAM );
	LRESULT	PostNotify( int, int, int, int, int, LPARAM );

	void	ChangeSelect( int, int );		// 選択カラム（ロー）変更
	void	OnArrowKey( UINT );
	void	OnSpaceKey( void );
	void	OnEnterKey( char );
	void	SelectCell( int, int );

public:
	CSimpleGrid( /*RECT *rct, CWnd *pParent*/ );
	virtual ~CSimpleGrid();
	int		AddRow( int );
	int		DelRow( int );
	void	ResetContent();
	void	GetCellText( WCHAR *, int , int, int );
	void	SetCellText( int, int, WCHAR * );
	void	SetCellColor( int, int, COLORREF, COLORREF );
	int		SetCheck( int, int, int );
	int		GetCheck( int, int );
	int		GetSelRow( void );
	int		GetSelCol( void );
	int		SetEnable( int, int, int );
	int		SetData( int, int, int );
	int		GetData( int, int );
	int		InputEnable( int );
	int		IsHeader();
	void	SetSelectCell( int, int );
	int		GetLineCount( int IncludingHeader );
	void	Redraw(void);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual BOOL Create(GRD_DEF *, const RECT& rect, UINT nID, CWnd* pParentWnd);
protected:
	virtual void PostNcDestroy();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnEditNotify( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnComboNotify( WPARAM wParam, LPARAM lParam );
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


