#pragma once


// CPaymentDlg ダイアログ
#include "..\pms_def.h"
#include "..\Custom\SimpleGrid.h"
#include "..\Custom\CommaEdit.h"
#include ".\fnc\csh_sql.h"

class CPaymentDlg : public CDialog
{
	DECLARE_DYNAMIC(CPaymentDlg)
private:
	CSimpleGrid		*m_Tax_grd;
	CSimpleGrid		*m_Ttl_grd;

	CCommaEdit		m_Edt_csh[1];
	CCommaEdit		m_Edt_cdt[1];
	CCommaEdit		m_Edt_cpn[1];
	CCommaEdit		m_Edt_otr[1];

	double			m_Ttl_amt;

	void			SetupComponent();
	void			SetData();
	void			SetBalance();


public:
	CPaymentDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CPaymentDlg();

	CDatabase		*m_pDbs_obj;
	PMS_MNG			*m_pPms_mng;

	CRC_CVS			*m_Crc_cvs;
	CRC_CVS			*m_Crc_bse;
	Csr				m_Pay_rot;
	Csr				m_Tax_rot;
	Csr				m_Ccv_rot;
	Csr				m_Dps_rot;
	Csr				m_Dpd_rot;

	double			m_Csh_amt;
	double			m_Cdt_amt;
	double			m_Cpn_amt;
	double			m_Otr_amt;
	double			m_Dps_amt;
	Lnk				m_Dps_pay[1];

	void			InitData( void );

	int				m_Chk_out;

// ダイアログ データ
	enum { IDD = IDD_DIALOG_PAYMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
	afx_msg void OnCbnSelchangeComboCurrency();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnKillfocusEditCash();
};
