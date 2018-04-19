#pragma once


// CTransferDlg ダイアログ
#include "..\pms_def.h"
#include "..\Custom\SimpleGrid.h"
#include "..\Custom\CommaEdit.h"
#include ".\fnc\csh_sql.h"

class CTransferDlg : public CDialog
{
	DECLARE_DYNAMIC(CTransferDlg)
private:
	CSimpleGrid		*m_Dst_grd;

	void			SetupComponent();
	void			InitData();

public:
	CTransferDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CTransferDlg();

	Csr				m_Dst_rot;
	CSH_INH_DAT		*m_Inh_dat;

// ダイアログ データ
	enum { IDD = IDD_DIALOG_TRANSFERDST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
