#pragma once


// CLoginDlg �_�C�A���O
#include "..\Common\lnk_mng.h"
#include "..\pms_def.h"

class CLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginDlg)
private:
	int			m_Pms_cnt;
	WCHAR		m_Old_uid[64];

public:
	CLoginDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CLoginDlg();

	PMS_MNG			*m_pPms_mng;
	CDatabase		*m_pDbs_obj;

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
