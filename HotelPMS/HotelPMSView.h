// HotelPMSView.h : CHotelPMSView �N���X�̃C���^�[�t�F�C�X
//


#pragma once

#include "stdafx.h"
#include "HotelPMSDoc.h"

class CHotelPMSView : public CFormView
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CHotelPMSView();
	DECLARE_DYNCREATE(CHotelPMSView)

	// �t�H�[�� �f�[�^
public:
	//{{AFX_DATA(CHotelPMSView)
	enum { IDD = IDD_ABOUTBOX };
		// ����: ClassWizard �͂��̈ʒu�Ƀf�[�^�����o��ǉ����܂��B
	//}}AFX_DATA

// ����
public:
	CHotelPMSDoc* GetDocument() const;

// ����
public:
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
	virtual ~CHotelPMSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // HotelPMSView.cpp �̃f�o�b�O �o�[�W����
inline CHotelPMSDoc* CHotelPMSView::GetDocument() const
   { return reinterpret_cast<CHotelPMSDoc*>(m_pDocument); }
#endif

