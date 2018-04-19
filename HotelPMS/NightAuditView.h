// NightAuditView.h : CNightAuditView �N���X�̃C���^�[�t�F�C�X
//


#pragma once

#include "..\Custom\CustomEdit.h"
#include "..\Custom\DateEdit.h"
#include "..\Custom\CustomStatic.h"
#include "..\Custom\SimpleGrid.h"
#include ".\fnc\viw_cmn.h"
#include ".\fnc\nad_fnc.h"
#include ".\fnc\nad_sql.h"
#include "NightAuditDoc.h"


class CNightAuditView : public CFormView
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CNightAuditView();
	DECLARE_DYNCREATE(CNightAuditView)

private:
	CSimpleGrid		*m_Wrn_grd;
	CCustomStatic	m_Biz_dte[1];

	void			SetupComponent();

// �t�H�[�� �f�[�^
public:
	//{{AFX_DATA(CNightAuditView)
	enum { IDD = IDD_DLG_NIGHTAUDIT };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^�����o��ǉ����܂��B
	//}}AFX_DATA

	// ����
public:
	CNightAuditDoc* GetDocument() const;

	PMS_MNG		*m_pPms_mng;			// PMS Management segment
	CDatabase	*m_pDbs_obj;			// Database object

	int			m_Rdn_rii;				// reservation_info_id when rbutton clicked
	int			m_Rdn_rdi;				// reservation_detail_id when rbutton clicked
	NAD_WRN_LST	*m_Rdn_wrn;				// warning list pointer when rbutton clicked

	WCHAR		*m_Ope_cod;
	WCHAR		*m_Brc_cod;				// target branch code
	WCHAR		*m_Fcl_cod;				// target facility code

	Lnk			m_Wrn_rot[1];			// root of editting data

	int			InitData( WCHAR *opc, WCHAR *brc, WCHAR *fcc );

// ����
public:

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
	virtual ~CNightAuditView();
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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg LRESULT OnGridNotify( WPARAM, LPARAM );
	afx_msg LRESULT OnViewNotify( WPARAM, LPARAM );
	afx_msg void OnClose();
};

#ifndef _DEBUG  // HotelPMSView.cpp �̃f�o�b�O �o�[�W����
inline CHotelPMSDoc* CNightAuditView::GetDocument() const
   { return reinterpret_cast<CHotelPMSDoc*>(m_pDocument); }
#endif

