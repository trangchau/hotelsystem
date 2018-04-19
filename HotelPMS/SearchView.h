// HotelPMSView.h : CSearchView �N���X�̃C���^�[�t�F�C�X
//


#pragma once

#include "..\Common\bsc_def.h"
#include "..\Common\lnk_mng.h"
#include "..\Custom\SimpleGrid.h"
#include "..\PMSCustom\RsvSchPanel.h"
#include "..\PMSCustom\BookingPathCombo.h"
#include "..\Custom\DateEdit.h"
#include "..\pms_def.h"
#include ".\fnc\sch_fnc.h"
#include ".\fnc\sch_sql.h"

class CSearchView : public CFormView
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CSearchView();
	DECLARE_DYNCREATE(CSearchView)

private:
	CDateEdit		m_Rsv_avf;
	CDateEdit		m_Rsv_avt;
	CDateEdit		m_Rsv_dpf;
	CDateEdit		m_Rsv_dpt;

	CRsvSchPanel	m_Rsv_pnl;		// Reservation panel
	CRsvSchPanel	m_Cin_pnl;		// Checkin today panel
	CRsvSchPanel	m_Inh_pnl;		// Inhouse panel
	CRsvSchPanel	m_Cot_pnl;		// Checkout today panel

	CBookingPathCombo	m_Rsv_pth;
	CBookingPathCombo	m_Cin_pth;
	CBookingPathCombo	m_Inh_pth;
	CBookingPathCombo	m_Cot_pth;

// �t�H�[�� �f�[�^
public:
	//{{AFX_DATA(CSearchView)
	enum { IDD = IDD_DLG_SEARCH };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^�����o��ǉ����܂��B
	//}}AFX_DATA

	// ����
public:
	CSearchDoc* GetDocument() const;

	Lnk			m_Rsv_rot[1];			// �\��f�[�^��
	Lnk			m_Cin_rot[1];			// �`�F�b�N�C���f�[�^��
	Lnk			m_Inh_rot[1];			// �C���n�E�X�f�[�^��
	Lnk			m_Cot_rot[1];			// �`�F�b�N�A�E�g�f�[�^��

	int			m_Rdn_rii;				// reservation_info_id when clecked the right button of mouse
	int			m_Rdn_rdi;				// reservation_detail_id when clecked the right button of mouse
	SCH_RSV_LST	*m_Rdn_rsv;				// reservation information when clecked the right button of mouse

	PMS_MNG		*m_pPms_mng;			// PMS Management segment
	CDatabase	*m_pDbs_obj;			// Database object

	CSimpleGrid	*m_Rsv_grd;				// �\�񌟍����ʃO���b�h
	CSimpleGrid	*m_Cin_grd;				// �`�F�b�N�C���������ʃO���b�h
	CSimpleGrid	*m_Inh_grd;				// �C���n�E�X�������ʃO���b�h
	CSimpleGrid	*m_Cot_grd;				// �`�F�b�N�A�E�g�������ʃO���b�h

	CTabCtrl	m_Knd_tab;


// ����
public:
	//void ResizeParentToFit(BOOL bShrinkOnly = TRUE);

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
	virtual ~CSearchView();
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
	afx_msg void OnTcnSelchangeTabKind(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
	afx_msg LRESULT OnViewNotify( WPARAM, LPARAM );
protected:
	virtual void PostNcDestroy();
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonResearch();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonDefault2();
};

#ifndef _DEBUG  // HotelPMSView.cpp �̃f�o�b�O �o�[�W����
inline CHotelPMSDoc* CSearchView::GetDocument() const
   { return reinterpret_cast<CHotelPMSDoc*>(m_pDocument); }
#endif

