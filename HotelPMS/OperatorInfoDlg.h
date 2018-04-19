#pragma once


#include "..\pms_def.h"

// COperatorInfoDlg ダイアログ

class COperatorInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(COperatorInfoDlg)

private:
	WCHAR		*m_Ope_cod;
	OPR_INF		m_Ope_inf[1];

	PMS_MNG		*m_pPms_mng;			// PMS Management segment
	CDatabase	*m_pDbs_obj;			// Database object

public:
	COperatorInfoDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~COperatorInfoDlg();

	void		SetupComponent();
	int			InitData( WCHAR * );

// ダイアログ データ
	enum { IDD = IDD_DIALOG_OPERATOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
