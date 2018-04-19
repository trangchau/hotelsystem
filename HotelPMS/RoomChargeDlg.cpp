// RoomChargeDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MainFrm.h"
#include "RoomChargeDlg.h"
#include ".\fnc\rcg_fnc.h"
#include ".\fnc\mfm_sql.h"
#include ".\fnc\ecg_sql.h"
#include "..\Common\dbs_mng.h"
#include ".\fnc\viw_cmn.h"

// CRoomChargeDlg ダイアログ

IMPLEMENT_DYNAMIC(CRoomChargeDlg, CDialog)

CRoomChargeDlg::CRoomChargeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRoomChargeDlg::IDD, pParent)
{
	m_Brt_grd = NULL;
	m_Apl_grd = NULL;
	m_Pln_lst = NULL;
	m_Ope_cod = NULL;
	m_Brc_cod = NULL;
	m_Fcl_cod = NULL;

	m_Bkp_cnt = 0;
	m_Tab_idx = -1;
	m_Grd_row = -1;
	m_Fcl_iid = 0;
	m_Dsc_cat = 0;
	m_Add_new = FALSE;

	memset( m_Pln_dtl, 0, sizeof(m_Pln_dtl) );

	memset( m_Brc_inf, 0, sizeof(m_Brc_inf) );
	memset( m_Fcl_inf, 0, sizeof(m_Fcl_inf) );
	memset( m_Eqp_inf, 0, sizeof(m_Eqp_inf) );
	memset( m_Eqp_typ, 0, sizeof(m_Eqp_typ) );
	memset( m_Agt_inf, 0, sizeof(m_Agt_inf) );
	memset( m_Apl_rot, 0, sizeof(m_Apl_rot) );
}

CRoomChargeDlg::~CRoomChargeDlg()
{
	if ( !!m_Brt_grd )	delete m_Brt_grd;
	if ( !!m_Apl_grd )	delete m_Apl_grd;

	Rcg_Dps_Pln( m_Pln_lst, m_Bkp_cnt );

	Del_All( m_Brc_inf );
	Del_All( m_Fcl_inf );
	Del_All( m_Eqp_inf );
	Del_All( m_Eqp_typ );
	Del_All( m_Agt_inf );
	Del_All( m_Apl_rot );

	Del_All( m_Pln_dtl->dtl );
}

void CRoomChargeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRoomChargeDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_CBN_SELCHANGE(IDC_COMBO_ROOMTYPE, &CRoomChargeDlg::OnCbnSelchangeComboRoomtype)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_BOOKINGPATH, &CRoomChargeDlg::OnTcnSelchangeTabBookingpath)
	ON_EN_KILLFOCUS(IDC_EDIT_SALESTARTDATE, &CRoomChargeDlg::OnEnKillfocusEditSalestartdate)
	ON_EN_KILLFOCUS(IDC_EDIT_SALEENDDATE, &CRoomChargeDlg::OnEnKillfocusEditSaleenddate)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CRoomChargeDlg::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CRoomChargeDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CRoomChargeDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_COPYALL, &CRoomChargeDlg::OnBnClickedButtonCopyall)
	ON_BN_CLICKED(IDC_BUTTON_COPYTO, &CRoomChargeDlg::OnBnClickedButtonCopyto)
	ON_BN_CLICKED(IDC_BUTTON_COPYTODETAIL, &CRoomChargeDlg::OnBnClickedButtonCopytodetail)
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CRoomChargeDlg::OnBnClickedButtonNew)
END_MESSAGE_MAP()

void CRoomChargeDlg::SetupComponent()
{
	int				idx;
	WINDOWPLACEMENT	wpc[1];

	struct	{
		CWnd	*wnd;
		int		lmt;
		DWORD	cid;
	}Cpn_[] = {
		{ m_Sst_dte,   -1, IDC_EDIT_SALESTARTDATE	},
		{ m_Sed_dte,   -1, IDC_EDIT_SALEENDDATE		},
		{ m_Brt_tab,   -1, IDC_TAB_BOOKINGPATH		},
		{ m_Crg_typ,   -1, IDC_COMBO_CHARGETYPE		},
		{ m_Crg_knd,   -1, IDC_COMBO_CHARGEKIND		},
		{ m_Eqp_amt,   -1, IDC_EDIT_AMOUNT1			},
		{ m_Eqp_amt+1, 11, IDC_EDIT_AMOUNT2			},
		{ m_Eqp_amt+2, 11, IDC_EDIT_AMOUNT3			},
		{ m_Eqp_amt+3, 11, IDC_EDIT_AMOUNT4			},
		{ m_Eqp_amt+4, 11, IDC_EDIT_AMOUNT5			},
		{ m_Eqp_amt+5, 11, IDC_EDIT_AMOUNT6			},
		{ m_Eqp_amt+6, 11, IDC_EDIT_AMOUNT7			},
		{ m_Eqp_amt+7, 11, IDC_EDIT_AMOUNT8			},
		{ m_Eqp_amt+8, 11, IDC_EDIT_AMOUNT9			}
	};

	for ( idx=0; idx<memcnt(Cpn_); idx++ ){
		Cpn_[idx].wnd->SubclassDlgItem(Cpn_[idx].cid, this);
		if ( Cpn_[idx].lmt!=-1 )	((CEdit *)Cpn_[idx].wnd)->SetLimitText( Cpn_[idx].lmt );
	}

	// rate plan list Grid
	GetDlgItem(IDC_TAB_BOOKINGPATH)->GetWindowPlacement( wpc );
	wpc->rcNormalPosition.top += 20;
	m_Brt_grd = Rcg_Mak_Grd( IDC_GRD_DTL, &wpc->rcNormalPosition, this );

	// Apply Grid
	GetDlgItem(IDC_STATIC_APPLY)->GetWindowPlacement( wpc );
	m_Apl_grd = Rcg_Mak_Grd( IDC_GRD_APL, &wpc->rcNormalPosition, this );
	m_Apl_grd->InputEnable( TRUE );

	((CEdit *)GetDlgItem(IDC_EDIT_PERSON))->SetLimitText( 2 );
}

// CRoomChargeDlg メッセージ ハンドラ

BOOL CRoomChargeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pPms_mng = ((CMainFrame *)GetParent())->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)GetParent())->m_Dbs_obj;

	SetupComponent();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// グリッドイベント
LRESULT CRoomChargeDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	CComboBox		*cbx;
	WCHAR			*txt;
	int				/*typ,*/ chk;
	LRESULT			rsl=TRUE;
//	RSV_DTL_DAT		*dtl;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_EDITCOMPLETE:	if ( !!( txt = (WCHAR *)lParam) ){
									free( txt );
								}
								break;
		case GN_SELECTCHANGE:
		case GN_LBUTTONCLICK:	if ( ntf->cid==IDC_GRD_DTL ){
									if ( ntf->row > 0 ){
										Rcg_Sel_Pln( ntf->row, this, m_pDbs_obj, m_pPms_mng );
									}
								}
								else{
									chk = m_Apl_grd->GetCheck(0, ntf->row);
									m_Apl_grd->SetCheck(0, ntf->row,!chk);
								}
								break;
		case GN_LBUTTONDBLCLICK:break;						// 左ボタンクリック
		case GN_QUERYCOMBODATA:	cbx = (CComboBox *)(LONG_PTR)lParam;
								break;
		case GN_COMBOCHANGED:	break;
		case GN_QUERYINPUTTYPE:	if ( ntf->cid==IDC_GRD_APL ){
									if ( ntf->clm == 0 )	rsl=GCT_CHK;
								}
								break;
		case GN_QUERYCOLORDATA:	break;
		}
		free( ntf );
	}

	return( rsl );
}

// room type select change
void CRoomChargeDlg::OnCbnSelchangeComboRoomtype()
{
	Rcg_Get_Lst( this, m_pDbs_obj, m_pPms_mng );
}

// Bookingpath select change
void CRoomChargeDlg::OnTcnSelchangeTabBookingpath(NMHDR *pNMHDR, LRESULT *pResult)
{
	Rcg_Dsp_Pln( this );
	*pResult = 0;
}

// date of sale start kill focus
void CRoomChargeDlg::OnEnKillfocusEditSalestartdate()
{
	Rcg_Sld_Chg( this, m_pDbs_obj, m_pPms_mng );
}

// date of sale end kill focus
void CRoomChargeDlg::OnEnKillfocusEditSaleenddate()
{
	Rcg_Sld_Chg( this, m_pDbs_obj, m_pPms_mng );
}

// Copy All button click
void CRoomChargeDlg::OnBnClickedButtonCopyall()
{
	int			idx, nul;
	double		val;

	if ( !( nul = m_Eqp_amt[0].GetValue( &val ) ) ){
		for ( idx=1; idx<memcnt(m_Eqp_amt); idx++ ){
			m_Eqp_amt[idx].SetValue( val, FALSE );
		}
	}
}

// CopyTo button click
void CRoomChargeDlg::OnBnClickedButtonCopyto()
{
	Rcg_Cpy_Pln( FALSE, this, m_pDbs_obj, m_pPms_mng );
}

// New button click
void CRoomChargeDlg::OnBnClickedButtonNew()
{
	Rcg_New_Dtl( this, m_pDbs_obj, m_pPms_mng );
}

// CopyTo(Detail) button click
void CRoomChargeDlg::OnBnClickedButtonCopytodetail()
{
	Rcg_Cpy_Pln( TRUE, this, m_pDbs_obj, m_pPms_mng );
}

// Update button click
void CRoomChargeDlg::OnBnClickedButtonUpdate()
{
	Rcg_Sav_Dtl( FALSE, this, m_pDbs_obj, m_pPms_mng  );
}

// Add button click
void CRoomChargeDlg::OnBnClickedButtonAdd()
{
	Rcg_Sav_Dtl( TRUE, this, m_pDbs_obj, m_pPms_mng  );
}

// Delete button click
void CRoomChargeDlg::OnBnClickedButtonDelete()
{
	if ( MessageBox( _T("Are you sure to delete this plan?"), NULL, MB_ICONQUESTION|MB_YESNO ) == IDYES ){
		Rcg_Del_Dtl(  this, m_pDbs_obj, m_pPms_mng  );
	}
}

BOOL CRoomChargeDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}

// CRoomChargeDlg オペレーション
int	 CRoomChargeDlg::InitData( WCHAR *opc, WCHAR *brc, WCHAR *fcc )
{
	m_Ope_cod = opc;
	m_Brc_cod = brc;
	m_Fcl_cod = fcc;

	return( Rcg_Ini_Dat( this, m_pDbs_obj, m_pPms_mng ) );
}
