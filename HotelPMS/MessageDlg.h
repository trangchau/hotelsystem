#pragma once


// CMessageDlg �_�C�A���O

class CMessageDlg : public CDialog
{
	DECLARE_DYNAMIC(CMessageDlg)

public:
	CMessageDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CMessageDlg();

	void	SetMessage( WCHAR * );

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
};
