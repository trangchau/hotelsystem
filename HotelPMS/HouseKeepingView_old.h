#pragma once

#include "..\Custom\SimpleGrid.h"
#include ".\fnc\viw_cmn.h"
#include "HouseKeepingDoc.h"


// CHouseKeepingView フォーム ビュー

class CHouseKeepingView : public CFormView
{
	DECLARE_DYNCREATE(CHouseKeepingView)
private:
	CSimpleGrid		*m_Wrn_grd;

	void			SetupComponent();

protected:
	CHouseKeepingView();           // 動的生成で使用される protected コンストラクタ
	virtual ~CHouseKeepingView();

public:
	enum { IDD = IDD_DLG_HOUSEKEPING };

	PMS_MNG		*m_pPms_mng;			// PMS Management segment
	CDatabase	*m_pDbs_obj;			// Database object

	//HKP_RCH_LST	*m_Rdn_wrn;				// warning list pointer when rbutton clicked

	WCHAR		*m_Ope_cod;
	WCHAR		*m_Brc_cod;				// target branch code
	WCHAR		*m_Fcl_cod;				// target facility code

	Lnk			m_Rch_rot[1];			// root of room change data

	int			InitData( WCHAR *opc, WCHAR *brc, WCHAR *fcc );


#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedCancel();
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
	afx_msg LRESULT OnViewNotify( WPARAM, LPARAM );
};


