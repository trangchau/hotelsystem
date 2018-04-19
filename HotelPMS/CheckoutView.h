// CheckoutView.h : CCheckoutView �N���X�̃C���^�[�t�F�C�X
//


#pragma once

#include "..\Custom\CustomEdit.h"
#include "..\Custom\DateEdit.h"
#include "..\Custom\CustomStatic.h"
#include "..\Custom\SimpleGrid.h"
#include ".\fnc\viw_cmn.h"
#include ".\fnc\cot_fnc.h"
#include ".\fnc\cot_sql.h"
#include "CheckoutDoc.h"
#include "CashierDlg.h"


class CCheckoutView : public CFormView
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CCheckoutView();
	DECLARE_DYNCREATE(CCheckoutView)

private:
// �����ϐ�

	CSimpleGrid			*m_Cot_grd;

	INH_INF_DAT			m_Rsv_inf[1];
	int					m_Sel_row;		// Row number of grd on with selecting 

	CCashierDlg			*m_Csh_ier;

	CCustomStatic		m_Rsv_nam;
	CCustomStatic		m_Wrn_ing;

	int					m_Rsv_did;

	void				OpentCashier( int );

// �t�H�[�� �f�[�^
public:
	//{{AFX_DATA(CCheckoutView)
	enum { IDD = IDD_DLG_CHECKOUT };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^�����o��ǉ����܂��B
	//}}AFX_DATA

	// ����
public:
	CCheckoutDoc* GetDocument() const;

	PMS_MNG		*m_pPms_mng;			// PMS Management segment
	CDatabase	*m_pDbs_obj;			// Database object

	Lnk			m_Rsv_dtl[1];

// ����
public:
	//void ResizeParentToFit(BOOL bShrinkOnly = TRUE);
	int			GetData( int, int );	// Getting data for exit on exist data
	int			CreateData( void );		// Creating a new reservation data

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
	virtual ~CCheckoutView();
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
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
	afx_msg LRESULT OnCashierNotify( WPARAM, LPARAM );
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#ifndef _DEBUG  // HotelPMSView.cpp �̃f�o�b�O �o�[�W����
inline CHotelPMSDoc* CCheckoutView::GetDocument() const
   { return reinterpret_cast<CHotelPMSDoc*>(m_pDocument); }
#endif

