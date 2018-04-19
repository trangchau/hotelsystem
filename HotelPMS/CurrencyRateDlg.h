#pragma once


// CCurrencyRateDlg �_�C�A���O
#include "..\Custom\SimpleGrid.h"
#include "..\Common\lnk_mng.h"
#include "..\pms_def.h"

class CCurrencyRateDlg : public CDialog
{
	DECLARE_DYNAMIC(CCurrencyRateDlg)
private:
	WCHAR			*m_Ope_cod;
	WCHAR			*m_Brc_cod;				// target branch code
	WCHAR			*m_Fcl_cod;				// target facility code

	Lnk				m_Ccv_rot[1];			// root of editting data

	CSimpleGrid		*m_Ccv_grd;

	void			SetupComponent();

public:
	CCurrencyRateDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CCurrencyRateDlg();

	PMS_MNG			*m_pPms_mng;
	CDatabase		*m_pDbs_obj;

	int				InitData( WCHAR *opc, WCHAR *brc, WCHAR *fcc );

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_CURRENCYRATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
