#pragma once


// CChangePwdDlg �_�C�A���O
#include "..\Common\lnk_mng.h"
#include "..\pms_def.h"

class CChangePwdDlg : public CDialog
{
	DECLARE_DYNAMIC(CChangePwdDlg)

public:
	CChangePwdDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CChangePwdDlg();

	PMS_MNG			*m_pPms_mng;
	CDatabase		*m_pDbs_obj;

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_CHANGEPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
