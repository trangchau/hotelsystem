#pragma once

#include "..\Custom\SimpleGrid.h"
#include "..\pms_def.h"

// CCategorySelectDlg ダイアログ

class CCategorySelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CCategorySelectDlg)

private:
	CSimpleGrid		*m_Sel_grd;
	Lnk				m_Cat_lst[1];

public:
	PMS_MNG			*m_pPms_mng;
	CDatabase		*m_pDbs_obj;
	WCHAR			*m_Ope_cod;
	WCHAR			*m_Brc_cod;
	WCHAR			*m_Fcl_cod;
	int				m_Result;
	int				m_Sel_dci;
	WCHAR			m_Sel_nam[256];

	CCategorySelectDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CCategorySelectDlg();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_CATEGORYSELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	LRESULT OnGridNotify( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
