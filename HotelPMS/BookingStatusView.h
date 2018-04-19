// BookingStatusView.h : CBookingStatusView �N���X�̃C���^�[�t�F�C�X
//


#pragma once

#include ".\fig\fig_mnp.h"
#include ".\fnc\bst_gph.h"
#include "..\pms_def.h"

class CBookingStatusView : public CView
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CBookingStatusView();
	DECLARE_DYNCREATE(CBookingStatusView)

// ����
public:
	CBookingStatusDoc* GetDocument() const;

	PMS_MNG		*m_pPms_mng;			// PMS Management segment
	CDatabase	*m_pDbs_obj;			// Database object
	int			m_Ope_mod;				// Current operation code of graphic

//private:
	BST_MNG		*m_Bst_mng;				// Booking status graphic management structure
	int			m_Rdn_rii;				// reservation_infos id on hitted when right button pushed
	int			m_Rdn_rdi;				// reservation_details id on hitted when right button pushed
	BST_ROM_STS	*m_Rdn_sts;				// Pointer of BST_ROM_STS on hitted when right button pushed

	void		SetScrollRenge( int, int );		// �X�N���[����Ԑݒ�

// ����
public:

	void		ReDraw( void );
	void		OnPeriodChange( WCHAR *, WCHAR * );

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
	virtual ~CBookingStatusView();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
//	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void PostNcDestroy();

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnGraphNotify( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnViewNotify( WPARAM wParam, LPARAM lParam );
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
//	afx_msg BOOL OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // BookingStatusView.cpp �̃f�o�b�O �o�[�W����
inline CBookingStatusDoc* CBookingStatusView::GetDocument() const
   { return reinterpret_cast<CBookingStatusDoc*>(m_pDocument); }
#endif

