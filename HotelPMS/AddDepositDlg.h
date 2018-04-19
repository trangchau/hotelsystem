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

	Lnk				m_Ccv_lnk[1];
	Lnk				m_Dps_lnk[1];
	CSH_TAX_DAT		m_Dpo_sit[1];

	CSH_DPS_DAT		*AddDeposit( int );
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

	int				m_Rsv_iid;
	int				m_Rsv_did;
	int				m_Sal_iid;
	int				m_Eqp_iid;

	int				m_Dir_ect;

	double			m_Csh_amt;
	int				m_Pay_mtd;

	Csr				m_Ccv_rot;
	Csr				m_Dps_rot;
	WCHAR			*m_Fst_nam;
	WCHAR			*m_Lst_nam;
	WCHAR			*m_Rom_nbr;

	CRC_CVS			*m_Crc_cvs;
	CSH_TAX_DAT		*m_Dps_mst;

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
