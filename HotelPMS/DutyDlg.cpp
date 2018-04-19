// DutyDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "DutyDlg.h"


// CDutyDlg ダイアログ
#include "MainFrm.h"
#include ".\fnc\mfm_sql.h"
#include ".\fnc\epl_sql.h"
#include ".\fnc\viw_cmn.h"

static	WCHAR	*Grd_ttl[] =	{	{ _T("CategoryName")		},
									{ _T("Abbreviation")	} };

static	GRD_CLM	Grd_clm[] =		{	{ 298, GAL_LFT, GCT_NML,  12 },		// TypeName
									{ 186, GAL_LFT, GCT_NML,  12 } };	// Abbrebiation

#define	IDC_GRD_DTY		(8192)	// Copy destination selecting grid

#define	CLM_DAT			0		// Data keeping column

#define	CLM_NAM			0		// TypeName
#define	CLM_ABR			1		// Abbrebiation


IMPLEMENT_DYNAMIC(CDutyDlg, CDialog)

CDutyDlg::CDutyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDutyDlg::IDD, pParent)
{
	m_Dty_grd = NULL;
	m_pPms_mng = NULL;
	m_pDbs_obj = NULL;
	m_Ope_cod = NULL;
	m_Brc_idx = -1;
	m_Fcl_idx = -1;
	memset( m_Brc_rot, 0, sizeof(m_Brc_rot) );
	memset( m_Fcl_rot, 0, sizeof(m_Fcl_rot) );
	memset( m_Dty_rot, 0, sizeof(m_Dty_rot) );
}

CDutyDlg::~CDutyDlg()
{
	if ( m_Dty_grd )	delete m_Dty_grd;
	Del_All( m_Brc_rot );
	Del_All( m_Fcl_rot );
	Del_All( m_Dty_rot );
}

void CDutyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

int	 CDutyDlg::InitData( WCHAR *opc, WCHAR *bcd, WCHAR *fcd )
{
	int			chk;
	Csr			csr=NULL;
	WCHAR		*erm=NULL;
	BRC_INF		*brc;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);

	m_Ope_cod = opc;
	m_Brc_cod = bcd;
	m_Fcl_cod = fcd;

	m_Brc_idx = -1;
	m_Fcl_idx = -1;
	Del_All( m_Brc_rot );
	Del_All( m_Fcl_rot );
	Del_All( m_Dty_rot );
	bcx->ResetContent();
	fcx->ResetContent();
	m_Dty_grd->ResetContent();

	SetEditData( NULL );

	if ( chk = Get_Mst( m_Brc_rot, GTM_BRC_INF, NULL, NULL, &erm, m_pDbs_obj, m_pPms_mng ) ){
		bcx->InsertString(-1, _T("") );
		while( csr = Nxt_Lnk( m_Brc_rot, csr ) ){
			brc = Rec_Of(BRC_INF, csr);
			bcx->SetItemData( bcx->InsertString(-1, EDT_DBF_WST(brc->nam, _T("")) ), (DWORD)(LONG_PTR)brc );
		}
	}
	bcx->SetCurSel(0);
	OnCbnSelchangeComboBranch();

	return( chk );
}

void	CDutyDlg::DispData( DTY_INF *dty, int row )
{
	m_Dty_grd->SetCellText( CLM_NAM, row, EDT_DBF_WST(dty->nam, _T("")) );
	m_Dty_grd->SetCellText( CLM_ABR, row, EDT_DBF_WST(dty->abr, _T("")) );
	m_Dty_grd->SetData( CLM_DAT, row, (int)(LONG_PTR)dty );
}


void	CDutyDlg::SetEditData( DTY_INF *dty )
{
	GetDlgItem(IDC_EDIT_DUTYNAME)->SetWindowTextW( !!dty ? EDT_DBF_WST(dty->nam, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_ABBREVIATION)->SetWindowTextW( !!dty ? EDT_DBF_WST(dty->abr, _T("")):_T(""));
	ButtonEnable();
}

void	CDutyDlg::ButtonEnable()	
{
	int			enb, sel;
	enb  = ((CComboBox *)GetDlgItem(IDC_COMBO_BRANCH))->GetCurSel() != -1;
	enb &= ((CComboBox *)GetDlgItem(IDC_COMBO_FACILITY))->GetCurSel() != -1;

	sel = m_Dty_grd->GetSelRow() > 0;

	GetDlgItem(IDC_EDIT_DUTYNAME)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ABBREVIATION)->EnableWindow( enb );
	GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow( sel && enb );
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow( enb);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow( sel && enb );
}

DWORD CDutyDlg::ConfirmSaveChaned( int msg, int fce )
{
	Csr			csr=NULL;
	DTY_INF		*dty;
	int			row, cnt, ans=IDYES;
	WCHAR		*erm=NULL;

	while( csr=Nxt_Lnk( m_Dty_rot, csr ) ){
		dty = Rec_Of(DTY_INF, csr);
		if ( !Nul_Fld(dty->mdt, FLD_DTM) || !Nul_Fld(dty->ddt, FLD_DTM) || Nul_Fld(dty->id, FLD_INT) ){
			break;
		}
	}
	if ( !!csr ){
		cnt = m_Dty_grd->GetLineCount( FALSE );
		for ( row=1; row<=cnt; row++ ){
			dty = (DTY_INF *)(LONG_PTR)m_Dty_grd->GetData( CLM_DAT, row );
			DBS_INT_SET(dty->odr, &row );
		}
		ans = Viw_Cfm_Svc( m_Dty_rot, Dty_Sav_Dty, msg, fce, this, m_pDbs_obj, m_pPms_mng );
	}
	return( ans );
}


BEGIN_MESSAGE_MAP(CDutyDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_CBN_SELCHANGE(IDC_COMBO_BRANCH, &CDutyDlg::OnCbnSelchangeComboBranch)
	ON_CBN_SELCHANGE(IDC_COMBO_FACILITY, &CDutyDlg::OnCbnSelchangeComboFacility)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CDutyDlg::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDutyDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDutyDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDCANCEL, &CDutyDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDutyDlg::OnBnClickedOk)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SORTORDER, &CDutyDlg::OnDeltaposSpinSortorder)
END_MESSAGE_MAP()


// CDutyDlg メッセージ ハンドラ
BOOL CDutyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GRD_DEF			gdf[1];
	int				idx;
	RECT			rct[1];
	WINDOWPLACEMENT	wpc[1];

	m_pPms_mng = ((CMainFrame *)GetParent())->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)GetParent())->m_Dbs_obj;

	memset( gdf, 0, sizeof(gdf) );

	GetDlgItem(IDC_COMBO_BRANCH)->GetWindowPlacement( wpc );
	*rct = wpc->rcNormalPosition;
	//rct->left = 10;
	rct->top += 30;
	rct->right = 0;
	rct->bottom = 0;

	gdf->lnh = 20;
	gdf->vlc = 6;
	gdf->vcc = memcnt(Grd_clm);
	gdf->hbc = RGB(128,255,255);
	gdf->hfc = RGB(0,0,0);
	gdf->dbc = RGB(255,255,255);
	gdf->dfc = RGB(0,0,0);
	gdf->sbc = RGB(0,0,0);
	gdf->sfc = RGB(255,255,255);
	gdf->cct = gdf->vcc;
	gdf->clm = Grd_clm;
	gdf->sel = GSL_ROW;
	gdf->hdr = TRUE;
	gdf->fsa = TRUE;

	m_Dty_grd = new CSimpleGrid();
	m_Dty_grd->Create( gdf, *rct, IDC_GRD_DTY, this );
	for ( idx=0; idx<memcnt(Grd_ttl); idx++ ){
		m_Dty_grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// グリッドイベント
LRESULT CDutyDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;
	int				row;
	DTY_INF			*dty;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:	row = ntf->row;
								if ( row > 0 ){
									dty = (DTY_INF *)(LONG_PTR)m_Dty_grd->GetData( CLM_DAT, row );
									if ( dty ){
										SetEditData( dty );
									}
								}
								break;
		}
		free( ntf );
	}

	return( rsl );
}

// UPDOWN spinbox change evennt
void CDutyDlg::OnDeltaposSpinSortorder(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int			idx, six=0, dix=0;
	DTY_INF		*src, *dst;
	Day			dte[1];

	if ( ( idx = m_Dty_grd->GetSelRow() ) > 0 ){
		if ( pNMUpDown->iDelta > 0 ){
			if ( idx < m_Dty_grd->GetLineCount( FALSE ) ){
				six = idx;
				dix = idx+1;
			}
		}
		else{
			if ( idx > 1 ){
				six = idx;
				dix = idx-1;
			}
		}
		if ( six != dix ){
			Get_Dte( dte );
			src = (DTY_INF *)(LONG_PTR)m_Dty_grd->GetData( CLM_DAT, six );
			dst = (DTY_INF *)(LONG_PTR)m_Dty_grd->GetData( CLM_DAT, dix );
			DBS_DTM_SET(src->mdt, dte);
			m_Dty_grd->SetData( CLM_DAT, six, (int)(LONG_PTR)dst );
			m_Dty_grd->SetData( CLM_DAT, dix, (int)(LONG_PTR)src );
			DispData( src, dix );
			DispData( dst, six );
			m_Dty_grd->SetSelectCell( CLM_DAT, dix );
		}
	}



	*pResult = 0;
}

// Branch combobox change
void CDutyDlg::OnCbnSelchangeComboBranch()
{
	int			idx;
	Csr			csr=NULL;
	WCHAR		*erm=NULL;
	BRC_INF		*brc=NULL;
	FCL_INF		*fcl;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);

	if ( ( ( idx = bcx->GetCurSel() ) != -1 ) && idx!=m_Brc_idx ){
		if ( ConfirmSaveChaned( TRUE, FALSE )!=IDCANCEL ){
			Del_All( m_Fcl_rot );
			Del_All( m_Dty_rot );
			fcx->ResetContent();
			m_Dty_grd->ResetContent();
			fcx->InsertString(-1, _T("") );
			if ( idx > 0 ){
				brc = (BRC_INF *)(LONG_PTR)bcx->GetItemData( idx );
				if ( Get_Mst( m_Fcl_rot, GTM_FCL_INF, DBS_WST_PTR(brc->brc), NULL, &erm, m_pDbs_obj, m_pPms_mng ) ){
					while( csr = Nxt_Lnk( m_Fcl_rot, csr ) ){
						fcl = Rec_Of(FCL_INF, csr);
						fcx->SetItemData( fcx->InsertString(-1, EDT_DBF_WST(fcl->nam, _T("")) ), (DWORD)(LONG_PTR)fcl );
					}
				}
			}
			fcx->SetCurSel(0);
			m_Fcl_idx = -1;
			OnCbnSelchangeComboFacility();
		}
		else{
			bcx->SetCurSel( idx = m_Brc_idx );
		}
	}
	m_Brc_idx = idx;
}

// Facility combobox change
void CDutyDlg::OnCbnSelchangeComboFacility()
{
	int			idx, bix, row=0;
	Csr			csr=NULL;
	BRC_INF		*brc=NULL;
	FCL_INF		*fcl=NULL;
	DTY_INF		*dty, key[1];
	WCHAR		*erm=NULL;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);

	memset( key, 0, sizeof(key) );

	if ( ( ( idx = fcx->GetCurSel() ) != -1 ) && idx!=m_Fcl_idx ){
		if ( ConfirmSaveChaned( TRUE, FALSE )!=IDCANCEL ){
			Del_All( m_Dty_rot );
			m_Dty_grd->ResetContent();
			wcscpy_s( DBS_WST_PTR(key->opc), FLD_WST_SIZ(key->opc), m_pPms_mng->opc );
			if ( idx>0 ){
				fcl = (FCL_INF *)(LONG_PTR)fcx->GetItemData( idx );
				DBS_WST_CPY(key->brc, fcl->brc);
				DBS_WST_CPY(key->fcc, fcl->fcc);
			}
			else{
				if ( ( bix = bcx->GetCurSel() ) > 0 ){
					brc = (BRC_INF *)(LONG_PTR)bcx->GetItemData( bix );
					DBS_WST_CPY(key->brc, brc->brc);
				}
			}
			if ( Dty_Get_Dty( m_Dty_rot, key, FALSE, &erm, m_pDbs_obj, m_pPms_mng ) ){
				m_Dty_grd->AddRow( Cnt_Lnk( m_Dty_rot ) );
				while( csr = Nxt_Lnk( m_Dty_rot, csr ) ){
					dty = Rec_Of(DTY_INF, csr);
					Set_Nul(dty->mdt, FLD_DTM, TRUE);
					DispData( dty, ++row );
				}
			}
		}
		else{
			fcx->SetCurSel( idx = m_Fcl_idx );
		}
	}
	ButtonEnable();
	m_Fcl_idx = idx;
}

// Updae button clich
void CDutyDlg::OnBnClickedButtonUpdate()
{
	int			row;
	DTY_INF		tmp[1], *dty;
	Day			dte[1];

	if ( ( row = m_Dty_grd->GetSelRow() ) > 0 ){
		dty = (DTY_INF *)(LONG_PTR)m_Dty_grd->GetData( CLM_DAT, row );
		WIN_TXT_DBS(tmp->nam, IDC_EDIT_DUTYNAME, this);
		WIN_TXT_DBS(tmp->abr, IDC_EDIT_ABBREVIATION, this);
		if ( !Nul_Fld(tmp->nam,FLD_WST) ){
			DBS_WST_CPY(dty->nam, tmp->nam);
			DBS_WST_CPY(dty->abr, tmp->abr);
			Get_Dte( dte );
			DBS_DTM_SET(dty->mdt, dte);
			DispData( dty, row );
			SetEditData( NULL );
			m_Dty_grd->SetSelectCell(-1, -1);
		}
		else{
			MessageBox( _T("DuytName is requierd item"), NULL, MB_ICONEXCLAMATION|MB_OK );
		}
	}
}

// Add button click
void CDutyDlg::OnBnClickedButtonAdd()
{
	int			row, idx, bix;
	Csr			csr;
	DTY_INF		tmp[1], *dty;
	BRC_INF		*brc=NULL;
	FCL_INF		*fcl=NULL;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);


	if ( ( idx = fcx->GetCurSel() ) != -1 && ( bix = bcx->GetCurSel() ) != -1 ){
		if ( bix>0 )	brc = (BRC_INF *)(LONG_PTR)fcx->GetItemData( bix );
		if ( idx>0 )	fcl = (FCL_INF *)(LONG_PTR)fcx->GetItemData( idx );
		WIN_TXT_DBS(tmp->nam, IDC_EDIT_DUTYNAME, this);
		WIN_TXT_DBS(tmp->abr, IDC_EDIT_ABBREVIATION, this);
		if ( !Nul_Fld(tmp->nam,FLD_WST) ){
			if ( csr=Add_Lnk( m_Dty_rot, NULL, sizeof(DTY_INF) ) ){
				dty = Rec_Of(DTY_INF, csr);
				DBS_WST_SET(dty->opc, m_pPms_mng->opc);
				if ( !!brc ){
					DBS_WST_SET(dty->brc, DBS_WST_PTR(brc->brc));
				}
				if ( !!fcl ){
					DBS_WST_SET(dty->fcc, DBS_WST_PTR(fcl->fcc));
				}
				DBS_WST_CPY(dty->nam, tmp->nam);
				DBS_WST_CPY(dty->abr, tmp->abr);
				row = m_Dty_grd->GetLineCount( FALSE );
				m_Dty_grd->AddRow( 1 );
				DispData( dty, row+1 );
				SetEditData( NULL );
				m_Dty_grd->SetSelectCell(-1, -1);
			}
		}
		else{
			MessageBox( _T("DutyName is requierd item"), NULL, MB_ICONEXCLAMATION|MB_OK );
		}
	}
}

// Delete button click
void CDutyDlg::OnBnClickedButtonDelete()
{
	int			row;
	DTY_INF		*dty;
	Day			dte[1];

	if ( ( row = m_Dty_grd->GetSelRow() ) > 0 ){
		dty = (DTY_INF *)(LONG_PTR)m_Dty_grd->GetData( CLM_DAT, row );
		if ( MessageBox( _T("Are you sure to delete?"), NULL, MB_ICONQUESTION|MB_YESNO ) == IDYES ){
			if ( Nul_Fld(dty->id, FLD_INT) ){
				Del_Lnk( m_Dty_rot, Csr_Of(dty) );
			}
			else{
				Get_Dte( dte );
				DBS_DAY_SET(dty->ddt, dte);
			}
			m_Dty_grd->DelRow( row );
			m_Dty_grd->SetSelectCell(-1, -1);
			m_Dty_grd->Redraw();
		}
	}
}



// Close button click
void CDutyDlg::OnBnClickedCancel()
{
	if ( ConfirmSaveChaned( TRUE, TRUE )==IDYES ){
		OnCancel();
	}
}

// Close winth save button click
void CDutyDlg::OnBnClickedOk()
{
	if ( ConfirmSaveChaned( FALSE, FALSE )==IDYES ){
		OnOK();
	}
}

BOOL CDutyDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}
