#pragma once

#include "..\pms_def.h"
#include "..\Custom\SimpleGrid.h"


// CClientSearchDlg �_�C�A���O

class CClientSearchDlg : public CDialog
{
	DECLARE_DYNAMIC(CClientSearchDlg)
private:
	WCHAR		*m_Ope_cod;

	Lnk			m_Cli_rot[1];

	CSimpleGrid	*m_Cli_grd;

	int			m_Sel_row;

	void		SetupComponent();
	void		SearchExecute();
	void		DispData( CLI_INF *, int );

public:
	CClientSearchDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CClientSearchDlg();

	WCHAR		*m_Fst_nam;
	WCHAR		*m_Lst_nam;

	PMS_MNG		*m_pPms_mng;			// PMS Management segment
	CDatabase	*m_pDbs_obj;			// Database object

	CLI_INF		m_Cli_inf[1];


// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_SEARCHCLIENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
public:
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonSearch();
};
