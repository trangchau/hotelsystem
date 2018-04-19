#pragma once

#include "..\Custom\Calendar.h"
#include "..\Common\bsc_def.h"
#include "..\Common\dte_mng.h"


#define	CDR_DLG_SINGLE		1
#define	CDR_DLG_DUAL		2


#define	WM_DATEPICKERNOTIFY	((WM_USER_Calendar_NOTIFY)+1)

#define ON_WM_DATEPICKERNOTIFY() \
	{ WM_DATEPICKERNOTIFY, 0, 0, 0, AfxSig_vwl, \
	(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, LPARAM))&OnDatePickerNotify },

#define	DPC_SELECTDATE		1			// date selected notify


// CDatePickerDlg ダイアログ

class CDatePickerDlg : public CDialog
{
	DECLARE_DYNAMIC(CDatePickerDlg)
private:
	CCalendar		*m_Lft_cdr;
	CCalendar		*m_Rgt_cdr;

	int				m_Dul_mod;

	CWnd			*m_Ntf_wnd;
	Day				m_Def_dte[1];


public:
	CDatePickerDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDatePickerDlg();

	void	Initialize( int, int, int );
	void	SetLimit( int, int, int, int );
	void	SetPosition( CWnd *, CWnd * );
	void	SetDefaultDate( Day * );
	void	SetNotifyWindow( CWnd * );

// ダイアログ データ
	enum { IDD = IDD_DIALOG_DATEPICKER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT	OnCalendarNotify( WPARAM, LPARAM );
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
protected:
	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};
