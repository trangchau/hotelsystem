// OperatorInfoDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MainFrm.h"
#include "OperatorInfoDlg.h"
//#include "..\Common\dbs_mng.h"
#include ".\fnc\opi_sql.h"
#include ".\fnc\sql_cmn.h"
#include ".\fnc\viw_cmn.h"

//IDC_EDIT_OPERATORNAME
//IDC_EDIT_ABBREVIATION
//IDC_EDIT_ADDRESS1
//IDC_EDIT_ADDRESS2
//IDC_EDIT_TELEPHONE
//IDC_EDIT_FAXNUMBER
//IDC_EDIT_WEBURL
//IDC_EDIT_EMAIL

// COperatorInfoDlg ダイアログ

void COperatorInfoDlg::SetupComponent()
{
	int				idx;

	struct	{
		CWnd	*wnd;
		int		lmt;
		DWORD	cid;
	}Cpn_[] = {
		{ NULL, 255, IDC_EDIT_OPERATORNAME	},
		{ NULL, 128, IDC_EDIT_ABBREVIATION	},
		{ NULL, 255, IDC_EDIT_ADDRESS1		},
		{ NULL, 255, IDC_EDIT_ADDRESS2		},
		{ NULL,  30, IDC_EDIT_TELEPHONE		},
		{ NULL,  30, IDC_EDIT_FAXNUMBER		},
		{ NULL, 255, IDC_EDIT_WEBURL		},
		{ NULL, 128, IDC_EDIT_EMAIL			}
	};

	for ( idx=0; idx<memcnt(Cpn_); idx++ ){
		if ( !!Cpn_[idx].wnd )		Cpn_[idx].wnd->SubclassDlgItem(Cpn_[idx].cid, this);
		if ( Cpn_[idx].lmt!=-1 )	((CEdit *)GetDlgItem(Cpn_[idx].cid))->SetLimitText( Cpn_[idx].lmt );
	}
}

IMPLEMENT_DYNAMIC(COperatorInfoDlg, CDialog)

COperatorInfoDlg::COperatorInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COperatorInfoDlg::IDD, pParent)
{
	m_pPms_mng = NULL;
	m_pDbs_obj = NULL;
	m_Ope_cod = NULL;
	memset( m_Ope_inf, 0, sizeof(m_Ope_inf) );
}

COperatorInfoDlg::~COperatorInfoDlg()
{
}

void COperatorInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COperatorInfoDlg, CDialog)
	ON_BN_CLICKED(IDOK, &COperatorInfoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &COperatorInfoDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// COperatorInfoDlg メッセージ ハンドラ

// COperatorInfoDlg オペレーション
int	 COperatorInfoDlg::InitData( WCHAR *opc )
{
	WCHAR		*erm=NULL;
	int			chk;

	m_Ope_cod = opc;
	chk = Opi_Get_Dat( m_Ope_inf, opc, &erm, m_pDbs_obj, m_pPms_mng );
	if ( chk ){
		GetDlgItem(IDC_EDIT_OPERATORNAME)->SetWindowText( EDT_DBF_WST(m_Ope_inf->nam,_T("")) );
		GetDlgItem(IDC_EDIT_ABBREVIATION)->SetWindowText( EDT_DBF_WST(m_Ope_inf->abr,_T("")) );
		GetDlgItem(IDC_EDIT_ADDRESS1)->SetWindowText( EDT_DBF_WST(m_Ope_inf->adr1,_T("")) );
		GetDlgItem(IDC_EDIT_ADDRESS2)->SetWindowText( EDT_DBF_WST(m_Ope_inf->adr2,_T("")) );
		GetDlgItem(IDC_EDIT_TELEPHONE)->SetWindowText( EDT_DBF_WST(m_Ope_inf->tel,_T("")) );
		GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText( EDT_DBF_WST(m_Ope_inf->fax,_T("")) );
		GetDlgItem(IDC_EDIT_WEBURL)->SetWindowText( EDT_DBF_WST(m_Ope_inf->url,_T("")) );
		GetDlgItem(IDC_EDIT_EMAIL)->SetWindowText( EDT_DBF_WST(m_Ope_inf->eml,_T("")) );
	}
	else{
		if( !!erm ){
			MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
	}

	return( chk );
}

BOOL COperatorInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pPms_mng = ((CMainFrame *)GetParent())->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)GetParent())->m_Dbs_obj;

	SetupComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void COperatorInfoDlg::OnBnClickedOk()
{
	int			chk, val=0;
	Day			dte[1];
	WCHAR		*erm=NULL;

	WIN_TXT_DBS(m_Ope_inf->nam, IDC_EDIT_OPERATORNAME, this);
	WIN_TXT_DBS(m_Ope_inf->abr, IDC_EDIT_ABBREVIATION, this);
	WIN_TXT_DBS(m_Ope_inf->adr1, IDC_EDIT_ADDRESS1, this);
	WIN_TXT_DBS(m_Ope_inf->adr2, IDC_EDIT_ADDRESS2, this);
	WIN_TXT_DBS(m_Ope_inf->tel, IDC_EDIT_TELEPHONE, this);
	WIN_TXT_DBS(m_Ope_inf->fax, IDC_EDIT_FAXNUMBER, this);
	WIN_TXT_DBS(m_Ope_inf->url, IDC_EDIT_WEBURL, this);
	WIN_TXT_DBS(m_Ope_inf->eml, IDC_EDIT_EMAIL, this);

	if ( Nul_Fld(m_Ope_inf->id, FLD_INT) ){
		DBS_INT_SET(m_Ope_inf->adi, &val);
		DBS_INT_SET(m_Ope_inf->acm, &val);
		DBS_INT_SET(m_Ope_inf->asb, &val);
	}

	Get_Dte( dte );
	if ( !( chk = !Nul_Fld(m_Ope_inf->nam, FLD_WST) ) ){
		MessageBox( _T("OperatorName is required item"), NULL, MB_ICONEXCLAMATION|MB_OK );
	}
	else{
		chk = Opi_Sav_Dat( m_Ope_inf, dte, &erm, m_pDbs_obj, m_pPms_mng );
		if ( !chk && !!erm ){
			MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
	}

	if ( chk )	OnOK();
}

void COperatorInfoDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OnCancel();
}

BOOL COperatorInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}
