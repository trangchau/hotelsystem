#pragma once

#include "..\Custom\SimpleGrid.h"
#include "..\pms_def.h"

// CCopySelectDlg ダイアログ
typedef	struct	{
	WCHAR	*nam;
	int		bkp;		// booking path
	int		aii;		// agent_infos id
	int		sel;		// selected
}Dst_;


class CCopySelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CCopySelectDlg)

private:
	CSimpleGrid		*m_Sel_grd;
	Dst_			*m_Dst_lst;

public:
	PMS_MNG			*m_pPms_mng;
	int				m_Result;
	int				m_Omt_bkp;
	int				m_Omt_aii;
	Csr				m_Sel_lst;

	CCopySelectDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CCopySelectDlg();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_COPYSELECT };

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
