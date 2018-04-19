#pragma once


// CDutyDlg �_�C�A���O

#include "..\Custom\SimpleGrid.h"
#include "..\Common\lnk_mng.h"
#include "..\pms_def.h"

class CDutyDlg : public CDialog
{
	DECLARE_DYNAMIC(CDutyDlg)
private:
	WCHAR			*m_Ope_cod;
	WCHAR			*m_Brc_cod;				// target branch code
	WCHAR			*m_Fcl_cod;				// target facility code

	Lnk				m_Brc_rot[1];
	Lnk				m_Fcl_rot[1];
	Lnk				m_Dty_rot[1];

	int				m_Brc_idx;
	int				m_Fcl_idx;

	CSimpleGrid		*m_Dty_grd;

	void			DispData( DTY_INF *, int );
	void			SetEditData( DTY_INF * );
	DWORD			ConfirmSaveChaned( int, int );
	void			ButtonEnable( void );	

public:
	CDutyDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CDutyDlg();

	PMS_MNG			*m_pPms_mng;
	CDatabase		*m_pDbs_obj;

	int				InitData( WCHAR *opc, WCHAR *brc, WCHAR *fcc );

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_DUTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

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
public:
	afx_msg void OnDeltaposSpinSortorder(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
