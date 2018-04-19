// LoginDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "LoginDlg.h"
#include "MainFrm.h"
#include "..\Common\scl_ble.h"
#include ".\fnc\mfm_sql.h"

#include ".\md5\md5_cpt.h"

// CLoginDlg ダイアログ

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	m_Pms_cnt = 0;
	memset( m_Old_uid, 0, sizeof(m_Old_uid ) );
}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CLoginDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoginDlg メッセージ ハンドラ
BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pPms_mng = ((CMainFrame *)GetParent())->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)GetParent())->m_Dbs_obj;

	//SetupComponent();

	GetDlgItem(IDC_EDIT_USERID)->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
// Close button click
void CLoginDlg::OnBnClickedCancel()
{
	OnCancel();
}

// Close winth save button click
void CLoginDlg::OnBnClickedOk()
{
	int				cnt;
	WCHAR			uid[64], pwd[64], dbp[64], *erm=NULL, hsh[33];
	//HASH_UINT128	hsh[1];

	GetDlgItem(IDC_EDIT_USERID)->GetWindowText( uid, memcnt(uid) );
	GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowText( pwd, memcnt(pwd) );
	
	if ( ( cnt = Mfm_Get_Pwd( m_pPms_mng->lgi, uid, &erm, m_pDbs_obj, m_pPms_mng ) ) != -1 ){
		//Pop_Str( dbp, DBS_WST_PTR(m_pPms_mng->lgi->pwd) );
		if ( !!wcslen(trim(pwd)) ){
			//md5_cpt( hsh, pwd, (DWORD)wcslen(pwd) );
			//hsh->hex( pwd );
			Ecd_MD5( hsh, pwd );
		}
		else{
			*pwd=0;
		}
		if ( cnt>0 && !wcscmp( hsh, DBS_WST_PTR(m_pPms_mng->lgi->pwd) ) ){
			if ( !DBS_INT_VAL(m_pPms_mng->lgi->lck) ){
				OnOK();
			}
			else{
				MessageBox( _T("Your account has been locked.\n")
							_T("Please contact your adoministrator."), NULL, MB_ICONSTOP|MB_OK );
			}
		}
		else{
			MessageBox( _T("Incorrect ID or password."), NULL, MB_ICONEXCLAMATION|MB_OK );
			//GetDlgItem(IDC_EDIT_USERID)->SetWindowText( _T("") );
			GetDlgItem(IDC_EDIT_PASSWORD)->SetWindowText( _T("") );
			//GetDlgItem(IDC_EDIT_USERID)->SetFocus();
			GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
			if ( cnt>0 ){
				if ( !wcscmp(m_Old_uid, uid) || m_Pms_cnt==0 ){
					if ( ++m_Pms_cnt == 3 ){
						if ( Mfm_Lck_Pwd( uid, &erm, m_pDbs_obj, m_pPms_mng ) ){
							MessageBox( 
								_T("Because you mistaken password on 3 times,")
								_T("your account was locked."), NULL, MB_ICONSTOP|MB_OK );
							OnCancel();
						}
					}
				}
				else{
					m_Pms_cnt = 0;
				}
			}
			wcscpy_s( m_Old_uid, memcnt(m_Old_uid), uid );
		}
	}
	else{
		if ( erm ){
			MessageBox( erm, NULL, MB_ICONSTOP|MB_OK );
			free( erm );
		}
		OnCancel();
	}
}
