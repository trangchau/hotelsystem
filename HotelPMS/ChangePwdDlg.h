#pragma once


// CChangePwdDlg ダイアログ
#include "..\Common\lnk_mng.h"
#include "..\pms_def.h"

class CChangePwdDlg : public CDialog
{
	DECLARE_DYNAMIC(CChangePwdDlg)

public:
	CChangePwdDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CChangePwdDlg();

	PMS_MNG			*m_pPms_mng;
	CDatabase		*m_pDbs_obj;

// ダイアログ データ
	enum { IDD = IDD_DIALOG_CHANGEPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
