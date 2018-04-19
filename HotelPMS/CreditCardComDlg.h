#pragma once

#include "..\pms_def.h"
#include "..\Custom\SimpleGrid.h"


// CCreditCardComDlg �_�C�A���O

class CCreditCardComDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreditCardComDlg)
private:
	WCHAR		*m_Ope_cod;

	Lnk			m_Cdc_rot[1];

	PMS_MNG		*m_pPms_mng;			// PMS Management segment
	CDatabase	*m_pDbs_obj;			// Database object

	CSimpleGrid	*m_Cdc_grd;

	DWORD		ConfirmSaveChaned( int, int );
	void		SetupComponent();
	void		DispData( CDC_COM *, int );
	int			GetData( CDC_COM * );
	void		SetEditData( CDC_COM * );
	void		ButtonEnable( void );	

public:
	CCreditCardComDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CCreditCardComDlg();

	int			InitData( WCHAR * );

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_CREDITCARDCOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
public:
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
