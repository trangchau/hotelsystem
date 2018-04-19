#pragma once


// CSelectPlanDlg �_�C�A���O

#include "..\Common\bsc_def.h"
#include "..\Common\lnk_mng.h"
#include "..\pms_def.h"
#include "..\Common\dbs_mng.h"
//#include "..\Common\dbs_mng.h"
#include ".\fnc\ecg_sql.h"
#include ".\fnc\sql_cmn.h"

class CSelectPlanDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectPlanDlg)

public:
	CSelectPlanDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CSelectPlanDlg();

	Csr				m_Pln_lst;
	ECG_PLN_DAT		*m_Sel_pln;

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_SELECTRATEPLAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
