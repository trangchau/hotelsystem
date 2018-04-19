// HotelPMSView.h : CCheckinView �N���X�̃C���^�[�t�F�C�X
//


#pragma once

#include "..\Custom\CustomEdit.h"
#include "..\Custom\DateEdit.h"
#include "..\Custom\CustomStatic.h"
#include "..\Custom\SimpleGrid.h"
#include ".\fnc\viw_cmn.h"
#include ".\fnc\cin_fnc.h"
#include ".\fnc\cin_sql.h"
#include "CheckinDoc.h"


class CCheckinView : public CFormView
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CCheckinView();
	DECLARE_DYNCREATE(CCheckinView)

private:
// �����ϐ�

	CCustomStatic		m_Cli_typ[1];
	CCustomStatic		m_Rsv_nam[1];

	void		SetupComponent();


// �t�H�[�� �f�[�^
public:
	//{{AFX_DATA(CCheckinView)
	enum { IDD = IDD_DLG_CHECKIN };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^�����o��ǉ����܂��B
	//}}AFX_DATA

	CSimpleGrid			*m_Cin_grd;

	CIN_INF_DAT			m_Cin_inf[1];
	Lnk					m_Rsv_dtl[1];
	int					m_Sel_row;		// Row number of grd on with selecting 


	// ����
public:
	CCheckinDoc* GetDocument() const;

	PMS_MNG		*m_pPms_mng;			// PMS Management segment
	CDatabase	*m_pDbs_obj;			// Database object

	CIN_DTL_DAT	*m_Rdn_dtl;				// CIN_DTL_DAT when mouse right button clicked
	int			m_Rdn_row;				// row of checkin grid when mouse right button clicked

// ����
public:
	//void ResizeParentToFit(BOOL bShrinkOnly = TRUE);
	int			GetData( int );			// Getting data for exit on exist data
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
	virtual ~CCheckinView();
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
	afx_msg void OnClose();
	afx_msg void OnBnClickedCheckCheckall();
	afx_msg void OnBnClickedButtonExeccheckin();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#ifndef _DEBUG  // HotelPMSView.cpp �̃f�o�b�O �o�[�W����
inline CHotelPMSDoc* CCheckinView::GetDocument() const
   { return reinterpret_cast<CHotelPMSDoc*>(m_pDocument); }
#endif

