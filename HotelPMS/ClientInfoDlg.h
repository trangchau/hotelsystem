#pragma once

#include "..\pms_def.h"
#include "..\Custom\CustomEdit.h"
#include "..\Custom\DateEdit.h"
#include "..\PMSCustom\ClientRankCombo.h"
#include "DatePickerDlg.h"

// CClientInfoDlg ダイアログ

class CClientInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CClientInfoDlg)
private:
	CCustomEdit			m_Fst_nam[1];
	CCustomEdit			m_Fam_nam[1];

	CDateEdit			m_Bir_dte[1];
	CDateEdit			m_Isu_dte[1];
	CDateEdit			m_Exp_dte[1];
	CDateEdit			m_Exp_Vsa[1];

	CClientRankCombo	m_Cli_rnk[1];

	CDatePickerDlg		*m_Dte_pcr;

	CWnd				*m_Fcs_edt;

	void	SetupComponent();
	int		GetData();
	void	DispData();
	void	ShowDatePickerDlg( DWORD );

public:
	CClientInfoDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CClientInfoDlg();

	PMS_MNG		*m_pPms_mng;			// PMS Management segment
	CDatabase	*m_pDbs_obj;			// Database object

	CLI_INF		*m_Cli_inf;
	CLI_INF		m_Edt_buf[1];

	int			m_Mkt_seg;				// market_segment_id
	int			m_Mks_edt;				// Edit enable to market_segment_id


// ダイアログ データ
	enum { IDD = IDD_DIALOG_CLIENTINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnKillfocusEditDateofbirth();
	afx_msg LRESULT OnEditNotify( WPARAM, LPARAM );
	afx_msg void OnDatePickerNotify( WPARAM, LPARAM );
	afx_msg void OnEnKillfocusEditDateofexpire();
	afx_msg void OnEnKillfocusEditDateofissue();
	afx_msg void OnEnKillfocusEditVisaexpiredate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCheckMale();
	afx_msg void OnBnClickedCheckFemale();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton1();
};
