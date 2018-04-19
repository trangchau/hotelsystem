#pragma once

#include "..\Custom\CustomEdit.h"
#include "..\Custom\DateEdit.h"
#include "..\Custom\CommaEdit.h"
#include "..\Custom\SimpleGrid.h"
#include "..\PMSCustom\ChargeTypeCombo.h"
#include "..\PMSCustom\ChargeKindCombo.h"
#include "HotelPMSView.h"
#include "..\pms_def.h"
#include ".\fnc\rcg_fnc.h"
#include ".\fnc\ecg_sql.h"


// CRoomChargeDlg ダイアログ

class CRoomChargeDlg : public CDialog
{
	DECLARE_DYNAMIC(CRoomChargeDlg)

private:
	CDateEdit			m_Sst_dte[1];			// Sale start date
	CDateEdit			m_Sed_dte[1];			// Sale end date

	CCommaEdit			m_Eqp_amt[9];			// Amount

	CSimpleGrid			*m_Brt_grd;				// Booking root grid
	CSimpleGrid			*m_Apl_grd;				// Apply grid
	CTabCtrl			m_Brt_tab[1];			// Booking root tab

	CChargeKindCombo	m_Crg_knd[1];			// Charge Kind comboboz
	CChargeTypeCombo	m_Crg_typ[1];			// Charge Type comboboz

	void			SetupComponent();


public:
	CRoomChargeDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CRoomChargeDlg();

	PMS_MNG		*m_pPms_mng;			// PMS Management segment
	CDatabase	*m_pDbs_obj;			// Database object

	int			InitData( WCHAR *opc, WCHAR *brc, WCHAR *fcc );		// Data initialize

	Lnk			m_Brc_inf[1];			// branch_infos
	Lnk			m_Fcl_inf[1];			// facility_infos
	Lnk			m_Eqp_inf[1];			// equipment_types
	Lnk			m_Eqp_typ[1];			// equipment_types
	Lnk			m_Agt_inf[1];			// agent_infos
	Lnk			m_Apl_rot[1];			// apply

	PLN_		*m_Pln_lst;				// Plan data list
	int			m_Bkp_cnt;				// number of bookingpath
	int			m_Tab_idx;				// selected tab index
	int			m_Grd_row;				// selected grid row
	int			m_Add_new;				// Adding new

	WCHAR		*m_Ope_cod;				// target operator code
	WCHAR		*m_Brc_cod;				// target branch code
	WCHAR		*m_Fcl_cod;				// target facility code

	int			m_Fcl_iid;				// facility_infos id
	int			m_Dsc_cat;				// description_category id


	ECG_PLN_DTL	m_Pln_dtl[1];			// Plan detail

// ダイアログ データ
	enum { IDD = IDD_DIALOG_ROOMCHARGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	LRESULT OnGridNotify( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboRoomtype();
	afx_msg void OnTcnSelchangeTabBookingpath(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEditSalestartdate();
	afx_msg void OnEnKillfocusEditSaleenddate();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonCopyall();
	afx_msg void OnBnClickedButtonCopyto();
	afx_msg void OnBnClickedButtonCopytodetail();
	afx_msg void OnBnClickedButtonNew();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
