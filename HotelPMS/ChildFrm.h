// ChildFrm.h : CChildFrame �N���X�̃C���^�[�t�F�C�X
//

#pragma once


class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// ����
public:
	int				m_Viw_knd;						// �r���[���
	int				m_Ini_upd;						// �����\�������t���O
	POINT			m_Win_siz[1];					// �K���̃E�B���h�E�T�C�Y

	int				m_Rsv_id;						// reservation_info_id of be editting
	int				m_Nsz_viw;						// Non sizable view

// ����
public:

// �I�[�o�[���C�h
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
};
