#pragma once

#include "..\pms_def.h"
#include "..\Custom\SimpleGrid.h"
#include "..\Custom\TimeEdit.h"
#include "..\PMSCustom\FacilityKindCombo.h"

// CFacilityInfoDlg ダイアログ

class CFacilityInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CFacilityInfoDlg)
private:
	WCHAR				*m_Ope_cod;
	Lnk					m_Brc_rot[1];
	Lnk					m_Fcl_rot[1];
	int					m_Brc_idx;

	CSimpleGrid			*m_Fcl_grd;

	PMS_MNG				*m_pPms_mng;			// PMS Management segment
	CDatabase			*m_pDbs_obj;			// Database object

	CTimeEdit			m_Nml_cin[1];
	CTimeEdit			m_Nml_cot[1];
	CFacilityKindCombo	m_Fcl_knd[1];


	DWORD				ConfirmSaveChaned( int, int );
	void				SetupComponent();
	void				DispData( FCL_INF *, int row );
	int					GetData( FCL_INF * );
	void				SetEditData( FCL_INF * );
	void				ButtonEnable( void );	

public:
	CFacilityInfoDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CFacilityInfoDlg();

	int			InitData( WCHAR * );

// ダイアログ データ
	enum { IDD = IDD_DIALOG_FACILITY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
public:
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboBranch();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
