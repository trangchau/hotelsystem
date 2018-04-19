#pragma once

#include "..\Custom\SimpleGrid.h"
#include "..\pms_def.h"
#include ".\fnc\rsv_sql.h"

// CChargeDetailDlg �_�C�A���O

class CChargeDetailDlg : public CDialog
{
	DECLARE_DYNAMIC(CChargeDetailDlg)
private:
	RSV_RCG_DAT		*m_Rcg_dat;

public:
	CChargeDetailDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CChargeDetailDlg();

	CSimpleGrid		*m_Dtl_grd;
	PMS_MNG			*m_pPms_mng;
	WCHAR			m_Rom_typ[256];
	WCHAR			m_Pln_nam[256];
	Csr				m_Dtl_lst;

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_CHARGEDETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	LRESULT OnGridNotify( WPARAM, LPARAM );
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonCopyall();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
