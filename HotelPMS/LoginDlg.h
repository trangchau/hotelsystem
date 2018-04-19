#pragma once


// CLoginDlg ダイアログ
#include "..\Common\lnk_mng.h"
#include "..\pms_def.h"

class CLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginDlg)
private:
	int			m_Pms_cnt;
	WCHAR		m_Old_uid[64];

public:
	CLoginDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CLoginDlg();

	PMS_MNG			*m_pPms_mng;
	CDatabase		*m_pDbs_obj;

// ダイアログ データ
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
