#pragma once


// CEquipmentTypeDlg ダイアログ

#include "..\Custom\SimpleGrid.h"
#include "..\Common\lnk_mng.h"
#include "..\pms_def.h"

class CEquipmentTypeDlg : public CDialog
{
	DECLARE_DYNAMIC(CEquipmentTypeDlg)

private:
	WCHAR			*m_Ope_cod;
	Lnk				m_Brc_rot[1];
	Lnk				m_Fcl_rot[1];
	Lnk				m_Typ_rot[1];

	int				m_Brc_idx;
	int				m_Fcl_idx;

	void			DispData( EQP_TYP *, int );
	void			SetEditData( EQP_TYP * );
	DWORD			ConfirmSaveChaned( int, int );
	void			ButtonEnable( void );	

public:
	CSimpleGrid		*m_Typ_grd;
	PMS_MNG			*m_pPms_mng;
	CDatabase		*m_pDbs_obj;

	int				InitData( WCHAR * );

public:
	CEquipmentTypeDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CEquipmentTypeDlg();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_EQUIPMENTTYPE };

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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
