#pragma once


// CProductSelDlg ダイアログ
#include "..\Custom\SimpleGrid.h"
#include "..\Common\lnk_mng.h"
#include "..\pms_def.h"
#include ".\fnc\csh_sql.h"


class CProductSelDlg : public CDialog
{
	DECLARE_DYNAMIC(CProductSelDlg)

private:
	WCHAR			*m_Ope_cod;
	WCHAR			*m_Brc_cod;
	WCHAR			*m_Fcl_cod;

	Csr				m_Pdt_rot;
	int				m_Cat_cnt;
	int				m_Tab_idx;

//	void			DispData( DSC_RPT *, int );
//	DWORD			ConfirmSaveChaned( int, int );
//	void			ButtonEnable( void );	

	void			DispProduct( CSH_PDT_LST *, int );
	void			DispProductList( void );
	void			DisposeProductList( void );
	int				MakeProductList();

public:
	CProductSelDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CProductSelDlg();

	CSimpleGrid		*m_Pdt_grd;

	PMS_MNG			*m_pPms_mng;
	CDatabase		*m_pDbs_obj;

	Csr				m_Sel_rot;

	int				InitData( WCHAR *, WCHAR *, WCHAR * );

	afx_msg void OnTcnSelchangeTabCategory(NMHDR *pNMHDR, LRESULT *pResult);

// ダイアログ データ
	enum { IDD = IDD_DIALOG_PRODUCTSEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
