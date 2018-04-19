//#if !defined(AFX_Calendar_H__9FAADAA2_BAF8_4CE6_8641_FDB97F32EBB5__INCLUDED_)
//#define AFX_Calendar_H__9FAADAA2_BAF8_4CE6_8641_FDB97F32EBB5__INCLUDED_

//#if _MSC_VER > 1000
#pragma once
//#endif // _MSC_VER > 1000
// Calendar.h : �w�b�_�[ �t�@�C��
//

#define	CLG_JAPANESE				1			// ���{��
#define	CLG_ENGLISH					2			// �p��


#define	WM_USER_Calendar_NOTIFY				(WM_USER+16640)

#define	UCN_NEEDHOLIDAY				5			/* �x�ɗv�� */
#define	UCN_NEEDRESERVE				6			/* �\��v�� */
#define	UCN_NEEDSELDAYS				7			/* �I��v�� */
//#define	UCN_SELECTEND				2			/* �I���I�� */
#define	UCN_INCREMENT				10			/* ���O�i */
#define	UCN_DECREMENT				11			/* ����� */

#define	UCN_SEL_MONTH				1			/* �������I�� */
#define	UCN_SEL_WEEK				2			/* �T�����I�� */
#define	UCN_SEL_DAY					4			/* �������I�� */

#define	UCN_MOD_NONE				0			/* �I��s�� */
#define	UCN_MOD_SINGLE				1			/* �P��I�����[�h */
#define	UCN_MOD_MULTI				2			/* �����I�����[�h */

#define	UCN_CSP_LEFT				1			/* ���p�J�����_�[ */
#define	UCN_CSP_RIGHT				2			/* �E�p�J�����_�[ */
#define	UCN_CSP_WEKNUM				4			/* �T�ԍ��\�� */

#define	UCN_RSV_FIX					'1'			/* �m��\�� */
#define	UCN_RSV_TMP					'2'			/* ���\�� */

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

typedef	struct	{		// �{�^���Ǘ��\��
	WNDPROC	prv;		// �K��̃E�B���h�E�v���V�[�W��
//	LONG	prv;		// �K��̃E�B���h�E�v���V�[�W��
	int		cap;		// �}�E�X�L���v�`���[���t���O
	int		lck;		// �{�^���n�m���b�N��
}WND_DATA;

/////////////////////////////////////////////////////////////////////////////
// CCalendar �E�B���h�E

class CCalendar : public CWnd
{
private:
	CWnd		*m_pParent;
	WCHAR		*m_lpClassName;
	int			m_iHeight;
	int			m_iWidth;
	int			m_iLanguage;			// ����
	WCHAR		m_pFontName[LF_FACESIZE];
	HFONT		m_hFont;
	BOOL		m_bFirst;
	BOOL		m_bDrag;				// �h���b�O���t���O
	int			m_nDragCont;			// �A���h���b�O�ݒ�l
	HWND		m_hLeftButton;
	HWND		m_hRightButton;
	HFONT		m_hButtonFont;			// �{�^���p�t�H���g
	char		m_pHoliday[40];			// �x���e�[�u��
	char		m_pReserve[40];			// �\��e�[�u��
	TFCNSELDATE	m_cDays[7][6];			// ���e�[�u��
	char		m_cSelDays[7][6];		// �I���e�[�u��
	int			m_iSelWeek;				// �I���T
	int			m_WeekCount;			// �T��
	int			m_iPrvCX;				// �O�񑀍�Z���w
	int			m_iPrvCY;				// �O�񑀍�Z���x
	int			m_iSttCX;				// �h���b�O�J�n�Z���w
	int			m_iSttCY;				// �h���b�O�J�n�Z���x
	int			m_iSpecial;				// ����d�l
	DWORD		m_dwUser;				// ���[�U�[�d�l
	int			m_iSelMode;				// �I�����[�h
	int			m_iDragMode;			// �h���b�O���[�h
	int			m_iResvYear;			// ���ݕ\�����\��N
	int			m_iResvMonth;			// ���ݕ\�����\��
	int			m_iResvDay;				// ���ݕ\�����\���
	int			m_iLimitMaxYear;		// �ő�N��
	int			m_iLimitMaxMonth;		// �ő�N��
	int			m_iLimitMinYear;		// �ŏ��N��
	int			m_iLimitMinMonth;		// �ŏ��N��
	int			m_bShowButtons;			// �{�^���`��
	WND_DATA	m_Wnd_data[2];			// �{�^���E�B���h�E�Ǘ��\��

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

// �R���X�g���N�V����
public:
	CCalendar( CWnd *pParent );
	int		m_iYear;
	int		m_iMonth;

	void SetFont( WCHAR *pFontName, BOOL iRedraw=TRUE );
// �A�g���r���[�g
public:

// �I�y���[�V����
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

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CCalendar)
	public:
	virtual BOOL Create( int iYear, int iMonth, RECT *rct, UINT nID, DWORD dwUser=NULL, int iSpecial=0 );
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CCalendar();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

//#endif // !defined(AFX_Calendar_H__9FAADAA2_BAF8_4CE6_8641_FDB97F32EBB5__INCLUDED_)
