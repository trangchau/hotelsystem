#pragma once


// CAddDepositDlg ダイアログ
#include "..\pms_def.h"
#include "..\Custom\SimpleGrid.h"
#include "..\Custom\CommaEdit.h"
#include ".\fnc\csh_sql.h"

class CAddDepositDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddDepositDlg)
private:

	CCommaEdit		m_Edt_csh[1];

	CSimpleGrid		*m_Dps_grd;
	CSimpleGrid		*m_Ttl_grd;
	void			SetupComponent();
	void			SetData();
	void			InitData();
	int				GetData();


public:
	CAddDepositDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CAddDepositDlg();

	CDatabase		*m_pDbs_obj;
	PMS_MNG			*m_pPms_mng;

	WCHAR			*m_Ope_cod;
	WCHAR			*m_Brc_cod;
	WCHAR			*m_Fcl_cod;

	int				m_Dir_ect;

	double			m_Csh_amt;
	CSH_INH_DAT		*m_Inh_dat;
	Csr				m_Ccv_rot;
	CRC_CVS			*m_Crc_cvs;
	int				m_Pay_mtd;
	CSH_TAX_DAT		*m_Dps_mst;
//	Csr				m_Dps_rot;

// ダイアログ データ
	enum { IDD = IDD_DIALOG_ADDDEPOSIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
	afx_msg void OnCbnSelchangeComboCurrency();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnKillfocusEditCash();
public:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonRefund();
};
