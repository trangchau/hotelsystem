// SelectPlanDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "SelectPlanDlg.h"
#include ".\fnc\viw_cmn.h"


// CSelectPlanDlg ダイアログ

IMPLEMENT_DYNAMIC(CSelectPlanDlg, CDialog)

CSelectPlanDlg::CSelectPlanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectPlanDlg::IDD, pParent)
{
	m_Pln_lst = NULL;
	m_Sel_pln = NULL;
}

CSelectPlanDlg::~CSelectPlanDlg()
{
}

void CSelectPlanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSelectPlanDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSelectPlanDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSelectPlanDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSelectPlanDlg メッセージ ハンドラ

BOOL CSelectPlanDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ECG_PLN_DAT	*pln;
	CComboBox	*cbx = (CComboBox *)GetDlgItem(IDC_COMBO_RATEPLAN);

	Csr			csr=NULL;
	WCHAR		*msg;

	while( csr=Nxt_Lnk( m_Pln_lst, csr ) ){
		pln = Rec_Of(ECG_PLN_DAT, csr);
		cbx->SetItemData( cbx->InsertString( -1, DBS_WST_PTR(pln->pln) ), (DWORD)(LONG_PTR)pln );
	}
	if( Cnt_Lnk( m_Pln_lst ) == 1 ){
		cbx->SetCurSel(0);
		msg = _T("Room rate plan need changed. Is it okay with the plan below?");
	}
	else{
		msg = _T("There are multiple rate plans. Please select an optimal rate plan");
	}
	GetDlgItem(IDC_STATIC_MESSAGE)->SetWindowText( msg );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CSelectPlanDlg::OnBnClickedOk()
{
	CComboBox	*cbx = (CComboBox *)GetDlgItem(IDC_COMBO_RATEPLAN);
	int			idx = cbx->GetCurSel();

	if ( idx!=-1 ){
		m_Sel_pln = (ECG_PLN_DAT *)(LONG_PTR)(cbx->GetItemData( idx ));
		OnOK();
	}
	else{
		MessageBox( _T("Unselected rate plan"), NULL, MB_ICONEXCLAMATION|MB_OK );
	}
}

void CSelectPlanDlg::OnBnClickedCancel()
{
	m_Sel_pln = NULL;
	OnCancel();
}

BOOL CSelectPlanDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}
