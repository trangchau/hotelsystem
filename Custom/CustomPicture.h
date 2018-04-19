#if !defined(AFX_PICTURE_H__E4DDD256_DB1C_4CEB_9B47_65D0622757E2__INCLUDED_)
#define AFX_PICTURE_H__E4DDD256_DB1C_4CEB_9B47_65D0622757E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomPicture.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CCustomPicture �E�B���h�E

class CCustomPicture : public CWnd
{
private:
	int			m_Init;
	RECT		m_Rect;
	CBitmap		m_Bitmap;
	CBitmap		m_BitMask;
	int			m_Width;
	int			m_Height;

// �R���X�g���N�V����
public:
	DWORD		m_UserData;
	CCustomPicture();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:
	void	SetBitmap( UINT nResource );		// ���\�[�X

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CPicture)
	public:
	virtual BOOL Create( const RECT& rect, CWnd* pParentWnd, UINT nID);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CCustomPicture();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CPicture)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_PICTURE_H__E4DDD256_DB1C_4CEB_9B47_65D0622757E2__INCLUDED_)
