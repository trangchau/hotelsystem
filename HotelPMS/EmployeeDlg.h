#pragma once


// CEmployeeDlg ダイアログ

#include "..\Custom\SimpleGrid.h"
#include "..\Custom\DateEdit.h"
#include "..\Common\lnk_mng.h"
#include "..\pms_def.h"

class CEmployeeDlg : public CDialog
{
	DECLARE_DYNAMIC(CEmployeeDlg)
private:
	WCHAR			*m_Ope_cod;
	WCHAR			*m_Brc_cod;
	WCHAR			*m_Fcl_cod;

	Lnk				m_Brc_rot[1];
	Lnk				m_Fcl_rot[1];
	Lnk				m_Epl_rot[1];

	Lnk				m_Dty_rot[1];

	int				m_Brc_idx;
	int				m_Fcl_idx;

	void			SetupComponent();
	void			DispData( EMP_LYE *, int );
	void			SetEditData( EMP_LYE * );
	int				GetData( EMP_LYE * );
	DWORD			ConfirmSaveChaned( int, int );
	void			ButtonEnable( void );

	CDateEdit		m_Bth_day[1];
	CDateEdit		m_Iss_day[1];
	CDateEdit		m_Exp_day[1];
	CDateEdit		m_Epl_day[1];

public:
	CSimpleGrid		*m_Epl_grd;
	PMS_MNG			*m_pPms_mng;
	CDatabase		*m_pDbs_obj;

	int				InitData( WCHAR *, WCHAR *, WCHAR * );

public:
	CEmployeeDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CEmployeeDlg();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_EMPLOYEE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
	afx_msg void OnCbnSelchangeComboBranch();
	afx_msg void OnCbnSelchangeComboFacility();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckMale();
	afx_msg void OnBnClickedCheckFemale();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnBnClickedButtonResetpassword();
};
