// EquipmentTypeDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "EquipmentTypeDlg.h"
#include "MainFrm.h"
#include ".\fnc\mfm_sql.h"
#include ".\fnc\eqp_sql.h"
#include ".\fnc\viw_cmn.h"

static	WCHAR	*Grd_ttl[] =	{	{ _T("TypeName")		},
									{ _T("Abbreviation")	} };

static	GRD_CLM	Grd_clm[] =		{	{ 298, GAL_LFT, GCT_NML,  12 },		// TypeName
									{ 186, GAL_LFT, GCT_NML,  12 } };	// Abbrebiation

#define	IDC_GRD_TYP		(8192)	// Copy destination selecting grid

#define	CLM_DAT			0		// Data keeping column

#define	CLM_NAM			0		// TypeName
#define	CLM_ABR			1		// Abbrebiation


// CEquipmentTypeDlg ダイアログ

IMPLEMENT_DYNAMIC(CEquipmentTypeDlg, CDialog)

CEquipmentTypeDlg::CEquipmentTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEquipmentTypeDlg::IDD, pParent)
{
	m_Typ_grd = NULL;
	m_pPms_mng = NULL;
	m_pDbs_obj = NULL;
	m_Ope_cod = NULL;
	m_Brc_idx = -1;
	m_Fcl_idx = -1;
	memset( m_Brc_rot, 0, sizeof(m_Brc_rot) );
	memset( m_Fcl_rot, 0, sizeof(m_Fcl_rot) );
	memset( m_Typ_rot, 0, sizeof(m_Typ_rot) );
}

CEquipmentTypeDlg::~CEquipmentTypeDlg()
{
	if ( m_Typ_grd )	delete m_Typ_grd;
	Del_All( m_Brc_rot );
	Del_All( m_Fcl_rot );
	Del_All( m_Typ_rot );
}

void CEquipmentTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

int	 CEquipmentTypeDlg::InitData( WCHAR *opc )
{
	int			chk;
	Csr			csr=NULL;
	WCHAR		*erm=NULL;
	BRC_INF		*brc;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);

	m_Ope_cod = opc;

	m_Brc_idx = -1;
	m_Fcl_idx = -1;
	Del_All( m_Brc_rot );
	Del_All( m_Fcl_rot );
	Del_All( m_Typ_rot );
	bcx->ResetContent();
	fcx->ResetContent();
	m_Typ_grd->ResetContent();

	SetEditData( NULL );

	if ( chk = Get_Mst( m_Brc_rot, GTM_BRC_INF, NULL, NULL, &erm, m_pDbs_obj, m_pPms_mng ) ){
		while( csr = Nxt_Lnk( m_Brc_rot, csr ) ){
			brc = Rec_Of(BRC_INF, csr);
			bcx->SetItemData( bcx->InsertString(-1, EDT_DBF_WST(brc->nam, _T("")) ), (DWORD)(LONG_PTR)brc );
		}
	}
	if ( Cnt_Lnk( m_Brc_rot ) == 1 ){
		bcx->SetCurSel(0);
		OnCbnSelchangeComboBranch();
	}

	return( chk );
}

void	CEquipmentTypeDlg::DispData( EQP_TYP *eqt, int row )
{
	m_Typ_grd->SetCellText( CLM_NAM, row, EDT_DBF_WST(eqt->nam, _T("")) );
	m_Typ_grd->SetCellText( CLM_ABR, row, EDT_DBF_WST(eqt->abr, _T("")) );
	m_Typ_grd->SetData( CLM_DAT, row, (int)(LONG_PTR)eqt );
}


void	CEquipmentTypeDlg::SetEditData( EQP_TYP *eqt )
{
	GetDlgItem(IDC_EDIT_TYPENAME)->SetWindowTextW( !!eqt ? EDT_DBF_WST(eqt->nam, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_ABBREVIATION)->SetWindowTextW( !!eqt ? EDT_DBF_WST(eqt->abr, _T("")):_T(""));
	ButtonEnable();
}

void	CEquipmentTypeDlg::ButtonEnable()	
{
	int			enb, sel;
	enb  = ((CComboBox *)GetDlgItem(IDC_COMBO_BRANCH))->GetCurSel() != -1;
	enb &= ((CComboBox *)GetDlgItem(IDC_COMBO_FACILITY))->GetCurSel() != -1;

	sel = m_Typ_grd->GetSelRow() > 0;

	GetDlgItem(IDC_EDIT_TYPENAME)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ABBREVIATION)->EnableWindow( enb );
	GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow( sel && enb );
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow( enb);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow( sel && enb );
}

DWORD CEquipmentTypeDlg::ConfirmSaveChaned( int msg, int fce )
{
	Csr			csr=NULL;
	EQP_TYP		*eqt;
	int			ans=IDYES;
	WCHAR		*erm=NULL;

	while( csr=Nxt_Lnk( m_Typ_rot, csr ) ){
		eqt = Rec_Of(EQP_TYP, csr);
		if ( !Nul_Fld(eqt->mdt, FLD_DTM) || !Nul_Fld(eqt->ddt, FLD_DTM) || Nul_Fld(eqt->id, FLD_INT) ){
			break;
		}
	}
	if ( !!csr ){
		ans = Viw_Cfm_Svc( m_Typ_rot, Eqp_Sav_Typ, msg, fce, this, m_pDbs_obj, m_pPms_mng );
		//if ( fce ){
		//	ans = MessageBox( 
		//		_T("Are you sure you want to discard your changes?"), NULL, MB_ICONQUESTION|MB_YESNO );
		//}
		//else{
		//	if ( !msg || ( ans = MessageBox( 
		//		_T("Do you save the changes?"), NULL, MB_ICONQUESTION|MB_YESNOCANCEL ) )==IDYES ){
		//		if ( !Eqp_Sav_Typ( m_Typ_rot, &erm, m_pDbs_obj, m_pPms_mng ) ){
		//			if ( erm ){
		//				MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
		//				free( erm );
		//				ans = IDCANCEL;
		//			}
		//		}
		//		else{
		//		}
		//	}
		//}
	}
	return( ans );
}

BEGIN_MESSAGE_MAP(CEquipmentTypeDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_CBN_SELCHANGE(IDC_COMBO_BRANCH, &CEquipmentTypeDlg::OnCbnSelchangeComboBranch)
	ON_CBN_SELCHANGE(IDC_COMBO_FACILITY, &CEquipmentTypeDlg::OnCbnSelchangeComboFacility)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CEquipmentTypeDlg::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CEquipmentTypeDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CEquipmentTypeDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDCANCEL, &CEquipmentTypeDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CEquipmentTypeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEquipmentTypeDlg メッセージ ハンドラ

BOOL CEquipmentTypeDlg::OnInitDialog()
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
	gdf->vlc = 10;
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

	m_Typ_grd = new CSimpleGrid();
	m_Typ_grd->Create( gdf, *rct, IDC_GRD_TYP, this );
	for ( idx=0; idx<memcnt(Grd_ttl); idx++ ){
		m_Typ_grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// グリッドイベント
LRESULT CEquipmentTypeDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;
	int				row;
	EQP_TYP			*eqt;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:	row = ntf->row;
								if ( row > 0 ){
									eqt = (EQP_TYP *)(LONG_PTR)m_Typ_grd->GetData( CLM_DAT, row );
									if ( eqt ){
										SetEditData( eqt );
									}
								}
								break;
		}
		free( ntf );
	}

	return( rsl );
}

// Branch combobox change
void CEquipmentTypeDlg::OnCbnSelchangeComboBranch()
{
	int			idx, cnt=0;
	Csr			csr=NULL;
	WCHAR		*erm=NULL;
	BRC_INF		*brc;
	FCL_INF		*fcl;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);

	if ( ( ( idx = bcx->GetCurSel() ) != -1 ) && idx!=m_Brc_idx ){
		if ( ConfirmSaveChaned( TRUE, FALSE )!=IDCANCEL ){
			Del_All( m_Fcl_rot );
			Del_All( m_Typ_rot );
			fcx->ResetContent();
			m_Typ_grd->ResetContent();
			brc = (BRC_INF *)(LONG_PTR)bcx->GetItemData( idx );
			if ( Get_Mst( m_Fcl_rot, GTM_FCL_INF, DBS_WST_PTR(brc->brc), NULL, &erm, m_pDbs_obj, m_pPms_mng ) ){
				while( csr = Nxt_Lnk( m_Fcl_rot, csr ) ){
					fcl = Rec_Of(FCL_INF, csr);
					if ( !!DBS_INT_VAL(fcl->hve) ){
						fcx->SetItemData( fcx->InsertString(-1, EDT_DBF_WST(fcl->nam, _T("")) ), (DWORD)(LONG_PTR)fcl );
						++cnt;
					}
				}
				if ( cnt==1 ){
					fcx->SetCurSel(0);
				}
				OnCbnSelchangeComboFacility();
			}
		}
		else{
			bcx->SetCurSel( idx = m_Brc_idx );
		}
	}
	m_Brc_idx = idx;
}

// Facility combobox change
void CEquipmentTypeDlg::OnCbnSelchangeComboFacility()
{
	int			idx, row=0, knd;
	Csr			csr=NULL;
	FCL_INF		*fcl;
	EQP_TYP		*eqt, key[1];
	WCHAR		*erm=NULL;
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);

	memset( key, 0, sizeof(key) );

	if ( ( ( idx = fcx->GetCurSel() ) != -1 ) && idx!=m_Fcl_idx ){
		if ( ConfirmSaveChaned( TRUE, FALSE )!=IDCANCEL ){
			Del_All( m_Typ_rot );
			m_Typ_grd->ResetContent();
			fcl = (FCL_INF *)(LONG_PTR)fcx->GetItemData( idx );
			knd = Viw_Get_Eqk( NULL, DBS_INT_VAL(fcl->fck) );
			wcscpy_s( DBS_WST_PTR(key->opc), FLD_WST_SIZ(key->opc), m_pPms_mng->opc );
			DBS_WST_CPY(key->brc, fcl->brc);
			DBS_WST_CPY(key->fcc, fcl->fcc);
			DBS_WST_CPY(key->fcc, fcl->fcc);
			DBS_INT_SET(key->ekd, &knd ); 
			if ( Eqp_Get_Typ( m_Typ_rot, key, &erm, m_pDbs_obj, m_pPms_mng ) ){
				m_Typ_grd->AddRow( Cnt_Lnk( m_Typ_rot ) );
				while( csr = Nxt_Lnk( m_Typ_rot, csr ) ){
					eqt = Rec_Of(EQP_TYP, csr);
					Set_Nul(eqt->mdt, FLD_DTM, TRUE);
					DispData( eqt, ++row );
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
void CEquipmentTypeDlg::OnBnClickedButtonUpdate()
{
	int			row;
	EQP_TYP		tmp[1], *eqt;
	Day			dte[1];

	if ( ( row = m_Typ_grd->GetSelRow() ) > 0 ){
		eqt = (EQP_TYP *)(LONG_PTR)m_Typ_grd->GetData( CLM_DAT, row );
		WIN_TXT_DBS(tmp->nam, IDC_EDIT_TYPENAME, this);
		WIN_TXT_DBS(tmp->abr, IDC_EDIT_ABBREVIATION, this);
		if ( !Nul_Fld(tmp->nam,FLD_WST) ){
			DBS_WST_CPY(eqt->nam, tmp->nam);
			DBS_WST_CPY(eqt->abr, tmp->abr);
			Get_Dte( dte );
			DBS_DTM_SET(eqt->mdt, dte);
			DispData( eqt, row );
			SetEditData( NULL );
			m_Typ_grd->SetSelectCell(-1, -1);
		}
		else{
			MessageBox( _T("TypeName is requierd item"), NULL, MB_ICONEXCLAMATION|MB_OK );
		}
	}
}

// Add button click
void CEquipmentTypeDlg::OnBnClickedButtonAdd()
{
	int			row, knd, idx;
	Csr			csr;
	EQP_TYP		tmp[1], *eqt;
	FCL_INF		*fcl;
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);


	if ( ( idx = fcx->GetCurSel() ) != -1 ){
		fcl = (FCL_INF *)(LONG_PTR)fcx->GetItemData( idx );
		WIN_TXT_DBS(tmp->nam, IDC_EDIT_TYPENAME, this);
		WIN_TXT_DBS(tmp->abr, IDC_EDIT_ABBREVIATION, this);
		if ( !Nul_Fld(tmp->nam,FLD_WST) ){
			if ( csr=Add_Lnk( m_Typ_rot, NULL, sizeof(EQP_TYP) ) ){
				eqt = Rec_Of(EQP_TYP, csr);
				DBS_WST_SET(eqt->opc, m_pPms_mng->opc);
				DBS_WST_SET(eqt->brc, DBS_WST_PTR(fcl->brc));
				DBS_WST_SET(eqt->fcc, DBS_WST_PTR(fcl->fcc));
				DBS_WST_CPY(eqt->nam, tmp->nam);
				DBS_WST_CPY(eqt->abr, tmp->abr);
				knd = Viw_Get_Eqk( NULL, DBS_INT_VAL(fcl->fck) );
				DBS_INT_SET(eqt->ekd, &knd);
				row = m_Typ_grd->GetLineCount( FALSE );
				m_Typ_grd->AddRow( 1 );
				DispData( eqt, row+1 );
				SetEditData( NULL );
				m_Typ_grd->SetSelectCell(-1, -1);
			}
		}
		else{
			MessageBox( _T("TypeName is requierd item"), NULL, MB_ICONEXCLAMATION|MB_OK );
		}
	}
}

// Delete button click
void CEquipmentTypeDlg::OnBnClickedButtonDelete()
{
	int			row;
	EQP_TYP		*eqt;
	Day			dte[1];

	if ( ( row = m_Typ_grd->GetSelRow() ) > 0 ){
		eqt = (EQP_TYP *)(LONG_PTR)m_Typ_grd->GetData( CLM_DAT, row );
		if ( MessageBox( _T("Are you sure to delete?"), NULL, MB_ICONQUESTION|MB_YESNO ) == IDYES ){
			if ( Nul_Fld(eqt->id, FLD_INT) ){
				Del_Lnk( m_Typ_rot, Csr_Of(eqt) );
			}
			else{
				Get_Dte( dte );
				DBS_DAY_SET(eqt->ddt, dte);
			}
			m_Typ_grd->DelRow( row );
			m_Typ_grd->SetSelectCell(-1, -1);
			m_Typ_grd->Redraw();
		}
	}
}



// Close button click
void CEquipmentTypeDlg::OnBnClickedCancel()
{
	if ( ConfirmSaveChaned( TRUE, TRUE )==IDYES ){
		OnCancel();
	}
}

// Close winth save button click
void CEquipmentTypeDlg::OnBnClickedOk()
{
	if ( ConfirmSaveChaned( FALSE, FALSE )==IDYES ){
		OnOK();
	}
}

BOOL CEquipmentTypeDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}
