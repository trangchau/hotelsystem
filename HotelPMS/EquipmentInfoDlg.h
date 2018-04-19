#pragma once


// CEquipmentInfoDlg ダイアログ

#include "..\Custom\SimpleGrid.h"
#include "..\Common\lnk_mng.h"
#include "..\pms_def.h"

class CEquipmentInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CEquipmentInfoDlg)

private:
	WCHAR			*m_Ope_cod;
	Lnk				m_Brc_rot[1];
	Lnk				m_Fcl_rot[1];
	Lnk				m_Typ_rot[1];
	Lnk				m_Inf_rot[1];
	int				m_Eqp_knd;

	int				m_Brc_idx;
	int				m_Fcl_idx;
	int				m_Eqt_idx;

	void			SetupComponent( void );
	void			DispData( EQP_INF *, int );
	void			SetEditData( EQP_INF * );
	void			SetAdditionalEnable( int );
	void			SetupAdditionalCbx( DWORD, int );
	void			SeekAdditionalCbx( DWORD, int );
	int				GetAdditionalCbx( DWORD );
	int				GetData( EQP_INF * );
	DWORD			ConfirmSaveChaned( int, int );
	void			ButtonEnable( void );	

public:
	CSimpleGrid		*m_Inf_grd;
	PMS_MNG			*m_pPms_mng;
	CDatabase		*m_pDbs_obj;

	int				InitData( WCHAR * );

public:
	CEquipmentInfoDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CEquipmentInfoDlg();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_EQUIPMENTINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
	afx_msg void OnCbnSelchangeComboBranch();
	afx_msg void OnCbnSelchangeComboFacility();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboEquipmenttype();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
