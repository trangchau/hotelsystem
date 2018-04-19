#pragma once


// CSelectPlanDlg ダイアログ

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
	CSelectPlanDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CSelectPlanDlg();

	Csr				m_Pln_lst;
	ECG_PLN_DAT		*m_Sel_pln;

// ダイアログ データ
	enum { IDD = IDD_DIALOG_SELECTRATEPLAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
