#pragma once


// CCurrencyRateDlg ダイアログ
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
	CCurrencyRateDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CCurrencyRateDlg();

	PMS_MNG			*m_pPms_mng;
	CDatabase		*m_pDbs_obj;

	int				InitData( WCHAR *opc, WCHAR *brc, WCHAR *fcc );

// ダイアログ データ
	enum { IDD = IDD_DIALOG_CURRENCYRATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
