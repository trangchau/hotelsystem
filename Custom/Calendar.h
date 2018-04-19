//#if !defined(AFX_Calendar_H__9FAADAA2_BAF8_4CE6_8641_FDB97F32EBB5__INCLUDED_)
//#define AFX_Calendar_H__9FAADAA2_BAF8_4CE6_8641_FDB97F32EBB5__INCLUDED_

//#if _MSC_VER > 1000
#pragma once
//#endif // _MSC_VER > 1000
// Calendar.h : ヘッダー ファイル
//

#define	CLG_JAPANESE				1			// 日本語
#define	CLG_ENGLISH					2			// 英語


#define	WM_USER_Calendar_NOTIFY				(WM_USER+16640)

#define	UCN_NEEDHOLIDAY				5			/* 休暇要求 */
#define	UCN_NEEDRESERVE				6			/* 予約要求 */
#define	UCN_NEEDSELDAYS				7			/* 選択要求 */
//#define	UCN_SELECTEND				2			/* 選択終了 */
#define	UCN_INCREMENT				10			/* 月前進 */
#define	UCN_DECREMENT				11			/* 月後退 */

#define	UCN_SEL_MONTH				1			/* 月部分選択 */
#define	UCN_SEL_WEEK				2			/* 週部分選択 */
#define	UCN_SEL_DAY					4			/* 日部分選択 */

#define	UCN_MOD_NONE				0			/* 選択不可 */
#define	UCN_MOD_SINGLE				1			/* 単一選択モード */
#define	UCN_MOD_MULTI				2			/* 複数選択モード */

#define	UCN_CSP_LEFT				1			/* 左用カレンダー */
#define	UCN_CSP_RIGHT				2			/* 右用カレンダー */
#define	UCN_CSP_WEKNUM				4			/* 週番号表示 */

#define	UCN_RSV_FIX					'1'			/* 確定予約 */
#define	UCN_RSV_TMP					'2'			/* 仮予約 */

#define ON_USER_CalendarNOTIFY() \
	{ WM_USER_Calendar_NOTIFY, 0, 0, 0, AfxSig_vwl, \
	(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, LPARAM))&OnCalendarNotify },

typedef	struct	{
	int		iYear;
	int		iMonth;
	int		iDay;
	char	*pHoliday;
	char	*pReserve;
	char	*pSelect;
	int		iWeek;
	DWORD	dwUser;
	int		iSpecial;
}TFCLNDRNTFY;

typedef	struct	{
	int		iYear;
	int		iMonth;
	int		iDay;
	int		iWeek;
}TFCNSELDATE;

typedef	struct	{		// ボタン管理構造
	WNDPROC	prv;		// 規定のウィンドウプロシージャ
//	LONG	prv;		// 規定のウィンドウプロシージャ
	int		cap;		// マウスキャプチャー中フラグ
	int		lck;		// ボタンＯＮロック中
}WND_DATA;

/////////////////////////////////////////////////////////////////////////////
// CCalendar ウィンドウ

class CCalendar : public CWnd
{
private:
	CWnd		*m_pParent;
	WCHAR		*m_lpClassName;
	int			m_iHeight;
	int			m_iWidth;
	int			m_iLanguage;			// 言語
	WCHAR		m_pFontName[LF_FACESIZE];
	HFONT		m_hFont;
	BOOL		m_bFirst;
	BOOL		m_bDrag;				// ドラッグ中フラグ
	int			m_nDragCont;			// 連続ドラッグ設定値
	HWND		m_hLeftButton;
	HWND		m_hRightButton;
	HFONT		m_hButtonFont;			// ボタン用フォント
	char		m_pHoliday[40];			// 休日テーブル
	char		m_pReserve[40];			// 予約テーブル
	TFCNSELDATE	m_cDays[7][6];			// 日テーブル
	char		m_cSelDays[7][6];		// 選択テーブル
	int			m_iSelWeek;				// 選択週
	int			m_WeekCount;			// 週数
	int			m_iPrvCX;				// 前回操作セルＸ
	int			m_iPrvCY;				// 前回操作セルＹ
	int			m_iSttCX;				// ドラッグ開始セルＸ
	int			m_iSttCY;				// ドラッグ開始セルＹ
	int			m_iSpecial;				// 特殊仕様
	DWORD		m_dwUser;				// ユーザー仕様
	int			m_iSelMode;				// 選択モード
	int			m_iDragMode;			// ドラッグモード
	int			m_iResvYear;			// 現在表示中予約年
	int			m_iResvMonth;			// 現在表示中予約月
	int			m_iResvDay;				// 現在表示中予約日
	int			m_iLimitMaxYear;		// 最大年月
	int			m_iLimitMaxMonth;		// 最大年月
	int			m_iLimitMinYear;		// 最小年月
	int			m_iLimitMinMonth;		// 最小年月
	int			m_bShowButtons;			// ボタン描画
	WND_DATA	m_Wnd_data[2];			// ボタンウィンドウ管理構造

	void DrawTitle( HDC );
	void DrawDetail( HDC );
	void NeedHoliday();
	void NeedReserve();
	void NeedSelect();
	int  MouseToCell( int *, int *, int, int );
	void GetCellRect( RECT *, int, int, int );
	void DrawWeekTriangle( HDC, int );
	void DrawCircle( HDC, RECT * );
	void increment();
	void decrement();

	void SetNtfyData( TFCLNDRNTFY *ntf );

// コンストラクション
public:
	CCalendar( CWnd *pParent );
	int		m_iYear;
	int		m_iMonth;

	void SetFont( WCHAR *pFontName, BOOL iRedraw=TRUE );
// アトリビュート
public:

// オペレーション
public:	
	void		SetLanguage( int iLanguage );
	void		SetReserveDate( int iYear, int iMonth, int iDay );
	void		SetLimit( int iMinYear, int iMinMonth, int iMaxYear, int iMaxMonth );
	int			Increment();
	int			Decrement();
	void		ResetSelect();
	void		ResetSelWeek();
	int			GetSelMonth( TFCNSELDATE *dst );
	int			GetSelWeek( TFCNSELDATE *dst );
	int			GetWeekDate( TFCNSELDATE *dst, int idx );
	TFCNSELDATE	*GetSelDays( int *cnt );
	void		SetSelMode( int iSelMode );
	void		SetSelDays();
	int			ShowButtons( int );
	void		ChangeYears( int iYear, int iMonth );
	void		Redraw();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CCalendar)
	public:
	virtual BOOL Create( int iYear, int iMonth, RECT *rct, UINT nID, DWORD dwUser=NULL, int iSpecial=0 );
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CCalendar();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CCalendar)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

//#endif // !defined(AFX_Calendar_H__9FAADAA2_BAF8_4CE6_8641_FDB97F32EBB5__INCLUDED_)
