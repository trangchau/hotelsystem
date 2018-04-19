// InhouseView.h : CInhouseView �N���X�̃C���^�[�t�F�C�X
//


#pragma once

#include "..\Custom\CustomEdit.h"
#include "..\Custom\DateEdit.h"
#include "..\Custom\CustomStatic.h"
#include "..\Custom\SimpleGrid.h"
#include ".\fnc\viw_cmn.h"
#include ".\fnc\inh_fnc.h"
#include ".\fnc\inh_sql.h"
#include "InhouseDoc.h"
#include "CashierDlg.h"


class CInhouseView : public CFormView
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CInhouseView();
	DECLARE_DYNCREATE(CInhouseView)

private:
// �����ϐ�

	CSimpleGrid			*m_Inh_grd;

	INH_INF_DAT			m_Rsv_inf[1];
	Lnk					m_Rsv_dtl[1];
	int					m_Sel_row;		// Row number of grd on with selecting 

	CCustomStatic		m_Cli_typ[1];
	CCustomStatic		m_Rsv_nam[1];

	CCashierDlg			*m_Csh_ier;

	int					m_Dct_csh;		// direct launch a cashier
	int					m_Rsv_did;		// reservation_detail_id

	void				SetupComponent( void );
	void				OpentCashier( int );

// �t�H�[�� �f�[�^
public:
	//{{AFX_DATA(CInhouseView)
	enum { IDD = IDD_DLG_INHOUSE };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^�����o��ǉ����܂��B
	//}}AFX_DATA

	// ����
public:
	CInhouseDoc* GetDocument() const;

	PMS_MNG		*m_pPms_mng;			// PMS Management segment
	CDatabase	*m_pDbs_obj;			// Database object

// ����
public:
	//void ResizeParentToFit(BOOL bShrinkOnly = TRUE);
	int			GetData( int, int, int );	// Getting data for exit on exist data
	int			CreateData( void );			// Creating a new reservation data

// �I�[�o�[���C�h
public:
	virtual void OnDraw(CDC* pDC);  // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h����܂��B
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ����
public:
	virtual ~CInhouseView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedCancel();
protected:
	virtual void PostNcDestroy();
public:
//	afx_msg void OnTcnSelchangeTabRsvDetail(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
	afx_msg LRESULT OnCashierNotify( WPARAM, LPARAM );
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonExeccheckin();
};

#ifndef _DEBUG  // HotelPMSView.cpp �̃f�o�b�O �o�[�W����
inline CHotelPMSDoc* CInhouseView::GetDocument() const
   { return reinterpret_cast<CHotelPMSDoc*>(m_pDocument); }
#endif

