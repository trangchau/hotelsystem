#pragma once


// CCurrencyCvsDlg ダイアログ
#include "..\Custom\SimpleGrid.h"
#include "..\Common\lnk_mng.h"
#include "..\pms_def.h"

class CCurrencyCvsDlg : public CDialog
{
	DECLARE_DYNAMIC(CCurrencyCvsDlg)
private:
	WCHAR			*m_Ope_cod;
	WCHAR			*m_Brc_cod;				// target branch code
	WCHAR			*m_Fcl_cod;				// target facility code

	Lnk				m_Brc_rot[1];
	Lnk				m_Fcl_rot[1];
	Lnk				m_Crc_rot[1];

	int				m_Brc_idx;
	int				m_Fcl_idx;

	CSimpleGrid		*m_Crc_grd;

	void			SetupComponent( void );
	void			DispData( CRC_CVS *, int );
	void			SetEditData( CRC_CVS * );
	int				GetData( CRC_CVS * );
	DWORD			ConfirmSaveChaned( int, int );
	void			ButtonEnable( void );	

public:
	CCurrencyCvsDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CCurrencyCvsDlg();
	PMS_MNG			*m_pPms_mng;
	CDatabase		*m_pDbs_obj;

	int				InitData( WCHAR *opc, WCHAR *brc, WCHAR *fcc );

// ダイアログ データ
	enum { IDD = IDD_DIALOG_CURRENCYCVS };

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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
