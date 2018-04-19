#pragma once


// CRoomSelectDlg ダイアログ
#include "..\Custom\CustomEdit.h"
#include "..\Custom\SimpleGrid.h"
#include "..\PMSCustom\BookingPathCombo.h"
#include "..\pms_def.h"
#include ".\fnc\viw_cmn.h"

class CRoomSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CRoomSelectDlg)

private:
	CSimpleGrid			*m_Rom_grd;		// Room list grid
	CBookingPathCombo	m_Cmb_bph[1];	// Booking path controller
	Lnk					m_Rom_rot[1];	// available room list
	EQP_ADT				*m_Adk_wng;		// wing name list
	EQP_ADT				*m_Adk_fce;		// face name list
	EQP_ADT				*m_Adk_viw;		// view name list
	int					m_Adc_wng;		// wing name list count
	int					m_Adc_fce;		// face name list count
	int					m_Adc_viw;		// view name list count

	void				SetupComponent();
	void				DispRoom( CSimpleGrid *, RSV_EQP_INF *, int, int );
	void				DispRoomList( CSimpleGrid *, Csr, int );
	void				InitData();

public:
	CRoomSelectDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CRoomSelectDlg();

	PMS_MNG		*m_pPms_mng;			// PMS Management segment
	CDatabase	*m_pDbs_obj;			// Database object

	int			m_Bok_pth;				// booking path
	int			m_Agt_iid;				// booking agent id
	int			m_Req_eti;				// requested equipment_type data
	int			m_Rsv_did;				// reservation_detail_id
	RSV_EQP_INF	m_Eqp_inf[1];			// equipment_info data
	int			m_Equ_iid;				// equipment_info_id which selected
	int			m_Eqp_tid;				// equipment_type data
	int			m_Eqp_sid;				// equipment_states data
	Day			*m_Use_std;				// use start date
	Day			*m_Use_edd;				// use end date
	WCHAR		m_Pln_nam[256];			// rate plan name

// ダイアログ データ
	enum { IDD = IDD_DIALOG_ROOMSELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	LRESULT OnGridNotify( WPARAM wParam, LPARAM lParam );
public:
	afx_msg void OnCbnSelchangeComboRoomtype();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
