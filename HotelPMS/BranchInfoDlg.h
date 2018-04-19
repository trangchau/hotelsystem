#pragma once

#include "..\pms_def.h"
#include "..\Custom\SimpleGrid.h"

// CBranchInfoDlg ダイアログ

class CBranchInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CBranchInfoDlg)
private:
	WCHAR		*m_Ope_cod;
	Lnk			m_Brc_rot[1];

	PMS_MNG		*m_pPms_mng;			// PMS Management segment
	CDatabase	*m_pDbs_obj;			// Database object

	CSimpleGrid	*m_Brc_grd;

	DWORD		ConfirmSaveChaned( int, int );
	void		SetupComponent();
	void		DispData( BRC_INF *, int );
	int			GetData( BRC_INF * );
	void		SetEditData( BRC_INF * );
	void		ButtonEnable( void );	

public:
	CBranchInfoDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CBranchInfoDlg();

	int			InitData( WCHAR * );

// ダイアログ データ
	enum { IDD = IDD_DIALOG_BRANCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	//afx_msg void OnBnClickedOk();
	//afx_msg void OnBnClickedCancel();
public:
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
