#pragma once


// CProductDlg ダイアログ
#include "..\Custom\SimpleGrid.h"
#include "..\Custom\CommaEdit.h"
#include "..\Custom\DateEdit.h"
#include "..\Common\lnk_mng.h"
#include "..\pms_def.h"

class CProductDlg : public CDialog
{
	DECLARE_DYNAMIC(CProductDlg)
private:
	WCHAR			*m_Ope_cod;
	WCHAR			*m_Brc_cod;				// target branch code
	WCHAR			*m_Fcl_cod;				// target facility code

	Lnk				m_Brc_rot[1];
	Lnk				m_Fcl_rot[1];
	Lnk				m_Cat_rot[1];
	Lnk				m_Pdt_rot[1];

	int				m_Brc_idx;
	int				m_Fcl_idx;
	int				m_Cat_idx;
	int				m_Pdk_idx;

	CCommaEdit		m_Edt_prc[1];
	CDateEdit		m_Edt_vss[1];
	CDateEdit		m_Edt_vse[1];

	CSimpleGrid		*m_Pdt_grd;
	CSimpleGrid		*m_Apl_grd;

	void			SetupComponent();
	void			DispData( DSC_RPT *, int );
	int				CheckExisting( DSC_RPT *, int );
	void			ShowAttribute( DSC_RPT *, int );
	void			SetEditData( DSC_RPT * );
	DWORD			ConfirmSaveChaned( int, int );
	void			ButtonEnable( void );	
	int				GetData( DSC_RPT * );
	void			SetComboIndex( DWORD, int );
	int				SetTax( DSC_RPT * );
	void			SetPriceKind( int );

public:
	CProductDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CProductDlg();

	PMS_MNG			*m_pPms_mng;
	CDatabase		*m_pDbs_obj;

	Lnk				m_Tax_rot[1];


	int				InitData( WCHAR *opc, WCHAR *brc, WCHAR *fcc );


// ダイアログ データ
	enum { IDD = IDD_DIALOG_PRODUCT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
	afx_msg void OnCbnSelchangeComboBranch();
	afx_msg void OnCbnSelchangeComboFacility();
	afx_msg void OnCbnSelchangeComboCategory();
	afx_msg void OnCbnSelchangeComboProductKind();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
