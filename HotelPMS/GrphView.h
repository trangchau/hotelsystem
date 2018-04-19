#if !defined(AFX_GRPHVIEW_H__60946D96_F2BA_11D5_B320_00508B04D67D__INCLUDED_)
#define AFX_GRPHVIEW_H__60946D96_F2BA_11D5_B320_00508B04D67D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GrphView.h : �w�b�_�[ �t�@�C��
//
#include "..\Graph\fig_mnp.h"
#include "..\Graph\gph_dsp.h"
#include ".\fnc\gph_sub.h"

/////////////////////////////////////////////////////////////////////////////
// CGrphView �r���[

class CGrphView : public CView
{
private:
	BST_MNG		*m_Bst_mng;
	int			m_OpeMode;					// ���ݑ��샂�[�h
	void		CalcScrollRenge( int );		// �X�N���[����Ԑݒ�

protected:
	CGrphView();           // ���I�����Ɏg�p�����v���e�N�g �R���X�g���N�^
	DECLARE_DYNCREATE(CGrphView)

// �A�g���r���[�g
public:
//	Mng_dat		*m_Mng_dat;	// �ΑӊǗ��f�[�^

// �I�y���[�V����
public:
	void		Refresh( int bReset );	// �\�����e��V

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CGrphView)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h���܂����B
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	virtual ~CGrphView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CGrphView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_GRPHVIEW_H__60946D96_F2BA_11D5_B320_00508B04D67D__INCLUDED_)
