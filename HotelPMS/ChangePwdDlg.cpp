// ChangePwdDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "ChangePwdDlg.h"
#include "MainFrm.h"
#include ".\fnc\mfm_sql.h"
#include "..\Common\scl_ble.h"

#include ".\md5\md5_cpt.h"

// CChangePwdDlg ダイアログ

IMPLEMENT_DYNAMIC(CChangePwdDlg, CDialog)

CChangePwdDlg::CChangePwdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChangePwdDlg::IDD, pParent)
{

}

CChangePwdDlg::~CChangePwdDlg()
{
}

void CChangePwdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChangePwdDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CChangePwdDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CChangePwdDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CChangePwdDlg メッセージ ハンドラ
BOOL CChangePwdDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pPms_mng = ((CMainFrame *)GetParent())->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)GetParent())->m_Dbs_obj;

	//SetupComponent();
	((CEdit *)GetDlgItem(IDC_EDIT_CURRENT))->SetLimitText(20);
	((CEdit *)GetDlgItem(IDC_EDIT_NEWPASSWORD))->SetLimitText(20);
	((CEdit *)GetDlgItem(IDC_EDIT_NEWPASSWORD2))->SetLimitText(20);

	GetDlgItem(IDC_EDIT_CURRENT)->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
// Close button click
void CChangePwdDlg::OnBnClickedCancel()
{
	OnCancel();
}



// Close winth save button click
void CChangePwdDlg::OnBnClickedOk()
{
	int				cnt;
	WCHAR			crt[96], np1[96], np2[96], dbp[96], *erm=NULL, hsh[33];
	EMP_LYE			epl[1];
//	HASH_UINT128	hsh[1];

	GetDlgItem(IDC_EDIT_CURRENT)->GetWindowText( crt, memcnt(crt) );
	GetDlgItem(IDC_EDIT_NEWPASSWORD)->GetWindowText( np1, memcnt(np1) );
	GetDlgItem(IDC_EDIT_NEWPASSWORD2)->GetWindowText( np2, memcnt(np2) );
	
	if ( !wcscmp( np1, np2 ) ){
		if ( ( cnt = Mfm_Get_Pwd( epl, DBS_WST_PTR(m_pPms_mng->lgi->epc), &erm, m_pDbs_obj, m_pPms_mng ) ) != -1 ){
			//Pop_Str( dbp, DBS_WST_PTR(epl->pwd) );
			if ( !!wcslen(trim(crt)) ){
				Ecd_MD5( hsh, crt );
				//md5_cpt( hsh, crt, (DWORD)wcslen(crt) );
				//hsh->hex( crt );
			}
			else{
				*hsh=0;
				//*crt = 0;
			}
			if ( cnt>0 && !wcscmp( crt, DBS_WST_PTR(epl->pwd) ) ){
				//Psh_Str( dbp, memcnt(dbp), np1 );
				if ( !!wcslen(trim(np1)) ){
					Ecd_MD5( hsh, np1 );
//					md5_cpt( hsh, np1, (DWORD)wcslen(np1) );
//					hsh->hex( np2 );
				}
				else{
					*np2 = 0;
				}
				if ( Mfm_Upd_Pwd( DBS_WST_PTR(m_pPms_mng->lgi->epc), /*dbp*/hsh, &erm, m_pDbs_obj, m_pPms_mng ) ){
					OnOK();
				}
			}
			else{
				MessageBox( _T("Incorrect current password."), NULL, MB_ICONEXCLAMATION|MB_OK );
				GetDlgItem(IDC_EDIT_CURRENT)->SetWindowText( _T("") );
				GetDlgItem(IDC_EDIT_NEWPASSWORD)->SetWindowText( _T("") );
				GetDlgItem(IDC_EDIT_NEWPASSWORD2)->SetWindowText( _T("") );
				GetDlgItem(IDC_EDIT_CURRENT)->SetFocus();
			}
		}
	}
	else{
		MessageBox( _T("No mach between new password ")
					_T("and new password confirmation."), NULL, MB_ICONEXCLAMATION|MB_OK );
	}

	if ( erm ){
		MessageBox( erm, NULL, MB_ICONSTOP|MB_OK );
		free( erm );
		OnCancel();
	}
}
