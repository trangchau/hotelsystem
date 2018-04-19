// MessageDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MessageDlg.h"


// CMessageDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CMessageDlg, CDialog)

CMessageDlg::CMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMessageDlg::IDD, pParent)
{
     Create(CMessageDlg::IDD,pParent);
}

CMessageDlg::~CMessageDlg()
{
}

void CMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMessageDlg, CDialog)
END_MESSAGE_MAP()


// CMessageDlg ���b�Z�[�W �n���h��

void CMessageDlg::PostNcDestroy()
{
	delete this;

	CDialog::PostNcDestroy();
}

void CMessageDlg::SetMessage( WCHAR *msg )
{
	GetDlgItem( IDC_STATIC_MSG )->SetWindowText( msg );
}
