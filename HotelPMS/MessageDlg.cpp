// MessageDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MessageDlg.h"


// CMessageDlg ダイアログ

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


// CMessageDlg メッセージ ハンドラ

void CMessageDlg::PostNcDestroy()
{
	delete this;

	CDialog::PostNcDestroy();
}

void CMessageDlg::SetMessage( WCHAR *msg )
{
	GetDlgItem( IDC_STATIC_MSG )->SetWindowText( msg );
}
