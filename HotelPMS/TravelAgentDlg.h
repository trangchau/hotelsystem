#pragma once

#include "..\pms_def.h"
#include "..\Custom\SimpleGrid.h"

// CTravelAgentDlg ダイアログ

class CTravelAgentDlg : public CDialog
{
	DECLARE_DYNAMIC(CTravelAgentDlg)
private:
	WCHAR		*m_Ope_cod;
	WCHAR		*m_Brc_cod;
	WCHAR		*m_Fcl_cod;

	Lnk			m_Agt_rot[1];

	PMS_MNG		*m_pPms_mng;			// PMS Management segment
	CDatabase	*m_pDbs_obj;			// Database object

	CSimpleGrid	*m_Agt_grd;

	DWORD		ConfirmSaveChaned( int, int );
	void		SetupComponent();
	void		DispData( AGT_INF *, int );
	int			GetData( AGT_INF * );
	void		SetEditData( AGT_INF * );
	void		ButtonEnable( void );	

public:
	CTravelAgentDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CTravelAgentDlg();

	int			InitData( WCHAR * );

// ダイアログ データ
	enum { IDD = IDD_DIALOG_TRAVELAGENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

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
