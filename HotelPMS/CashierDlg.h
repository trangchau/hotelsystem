#pragma once


#include "..\pms_def.h"
#include "..\Custom\SimpleGrid.h"
#include ".\fnc\csh_sql.h"

// CCashierDlg ダイアログ
class CCashierDlg;

typedef	struct	{				// Cashier notification structure
	int					ntf;	// Notify code
	int					cid;	// control ID
	class CCashierDlg	*csh;	// Instance of own grid
}CSH_NTF;

#define	WM_CASHIERNOTIFY	(WM_USER+16390)		// Notification from cashier messeage ID

#define	CN_CHECKOUTING		1					// Before one room checkouted 
#define	CN_CHECKOUT			2					// One room checkouted 
#define	CN_ALLCHECKOUT		3					// All checkouted 
#define	CN_ROOMCHANGED		4					// Room tab select change

#define ON_WM_CASHIERNOTIFY() \
	{ WM_CASHIERNOTIFY, 0, 0, 0, AfxSig_lwl, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< LRESULT (AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM) > \
		( &ThisClass :: OnCashierNotify)) },

class CCashierDlg : public CDialog
{
	DECLARE_DYNAMIC(CCashierDlg)

private:
	int				m_Rsv_id;

public:
	CCashierDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CCashierDlg();

	WCHAR			*m_Ope_cod;
	WCHAR			*m_Brc_cod;
	WCHAR			*m_Fcl_cod;

	CDatabase		*m_pDbs_obj;
	PMS_MNG			*m_pPms_mng;

	CSimpleGrid		*m_Csh_grd;
	CSimpleGrid		*m_Tax_grd;
	CSimpleGrid		*m_Dps_grd;

	Lnk				m_Inh_rot[1];
	Lnk				m_Tax_rot[1];
	Lnk				m_Ccv_rot[1];
	Lnk				m_Dps_rot[1];

	double			m_Bls_amt;

	CRC_CVS			*m_Crc_cvs;

	//CSH_SAL_DAT		*m_Sal_dat;
	int				m_Tab_idx;
	int				m_Rom_cnt;
	BOOL			SetupCashier( WCHAR *, WCHAR *, WCHAR *, int, int );
	BOOL			Save( void );
	BOOL			IsUpdateExist( void );

	int				m_Chk_out;

// ダイアログ データ
	enum { IDD = IDD_DIALOG_CASHIER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
	afx_msg void OnTcnSelchangeTabRoom(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAddproduct();
	afx_msg void OnCbnSelchangeComboCurrency();
	afx_msg void OnBnClickedButtonParticalpayment();
	afx_msg void OnBnClickedButtonAllpayment();
	afx_msg void OnBnClickedButtonAdddeposit();
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnBnClickedButtonTransfer();
};
