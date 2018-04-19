// HotelPMSView.h : CReservationView �N���X�̃C���^�[�t�F�C�X
//


#pragma once

#include "..\Custom\CustomEdit.h"
#include "..\Custom\DateEdit.h"
#include "..\Custom\TimeEdit.h"
#include "..\PMSCustom\BookingPathCombo.h"
#include "..\PMSCustom\ClientRankCombo.h"
#include "..\PMSCustom\EquipmentTypeCombo.h"
#include "..\Custom\CustomStatic.h"
#include "..\Custom\SimpleGrid.h"
#include ".\fnc\viw_cmn.h"
#include ".\fnc\rsv_fnc.h"
#include ".\fnc\rsv_sql.h"
#include "HotelPMSView.h"
#include "ReservationDoc.h"
#include "DatePickerDlg.h"


class CReservationView : public CFormView
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CReservationView();
	DECLARE_DYNCREATE(CReservationView)

private:
// �����ϐ�
	CCustomEdit			m_Fst_nam[1];
	CCustomEdit			m_Fam_nam[1];
	CCustomEdit			m_Fst_dtl[1];
	CCustomEdit			m_Fam_dtl[1];
	CDateEdit			m_Arv_dte[1];
	CDateEdit			m_Dpt_dte[1];
	CDateEdit			m_Arv_dtl[1];
	CDateEdit			m_Dpt_dtl[1];
	CTimeEdit			m_Arv_tim[1];
	CTimeEdit			m_Dpt_tim[1];
	CEdit				m_Rsv_nte[1];

	CBookingPathCombo	m_Bok_pth[1];
	CClientRankCombo	m_Cli_rnk[1];
	CEquipmentTypeCombo	m_Eqp_typ[1];

	CCustomStatic		m_Rsv_typ[1];
	CCustomStatic		m_Bpt_ttl[1];

	CSimpleGrid			*m_Dtl_grd;
	CSimpleGrid			*m_Rom_grd;


	CCustomStatic		m_Dtl_sts[1];
	CCustomStatic		m_Inf_sts[1];

	CWnd				*m_Fcs_edt;

	CDatePickerDlg		*m_Dte_pcr;

	void	SetupComponent( void );
	void	ShowCalenderDlg( DWORD );


// �t�H�[�� �f�[�^
public:
	//{{AFX_DATA(CReservationView)
	enum { IDD = IDD_DLG_RESERVATION };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^�����o��ǉ����܂��B
	//}}AFX_DATA

	// ����
public:
	CReservationDoc* GetDocument() const;

	PMS_MNG		*m_pPms_mng;			// PMS Management segment
	CDatabase	*m_pDbs_obj;			// Database object

	int					m_Edt_sts;		// Editing status
	int					m_Sel_row;		// Row number of grd on with selecting 
	WCHAR				*m_Opr_cod;		// Target operator code
	WCHAR				*m_Brc_cod;		// Target branch code
	WCHAR				*m_Fcl_cod;		// Target facility code

	RSV_INF_DAT			m_Rsv_inf[1];
	Lnk					m_Rte_pln[1];
	Lnk					m_Eqp_isi[1];

	int					m_Ctm_row;		// Context menu selecting row

// ����
public:
	int		GetData( int );									// Getting data for exit on exist data
	int		CreateData(  CCI_PRM *  );						// Creating a new reservation data

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
	virtual ~CReservationView();
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
	afx_msg LRESULT OnEditNotify( WPARAM, LPARAM );
	afx_msg void OnDatePickerNotify( WPARAM, LPARAM );
	afx_msg void OnClose();
	afx_msg void OnEnChangeEditFirstname();
	afx_msg void OnBnClickedCheckSamereservation();
	afx_msg void OnEnChangeEditRoomnumber();
	afx_msg void OnEnChangeEditAdultnumber();
	afx_msg void OnEnChangeEditChildnumber();
	afx_msg void OnCbnSelchangeComboBookingpath();
	afx_msg void OnBnClickedRadioGroup();
	afx_msg void OnBnClickedRadioPersonal();
	afx_msg void OnCbnSelchangeComboRoomtype();
	afx_msg void OnEnChangeEditRoomchange();
	afx_msg void OnEnKillfocusEditArrivaldate();
	afx_msg void OnEnKillfocusEditDeparturedate();
	afx_msg void OnEnChangeEditFamilyname();
	afx_msg void OnCbnSelchangeComboRateplanname();
	afx_msg void OnDeltaposSpinNights(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEditNights();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnBnClickedCheckCheckall();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnEnSetfocusEditArrivaldate();
	afx_msg void OnEnSetfocusEditDeparturedate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonSearchdetail();
	afx_msg void OnBnClickedButtonCancel();
};

#ifndef _DEBUG  // HotelPMSView.cpp �̃f�o�b�O �o�[�W����
inline CHotelPMSDoc* CReservationView::GetDocument() const
   { return reinterpret_cast<CHotelPMSDoc*>(m_pDocument); }
#endif

