// MarketSegmentDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MarketSegmentDlg.h"
#include "MainFrm.h"
#include ".\fnc\mfm_sql.h"
#include ".\fnc\cli_sql.h"
#include ".\fnc\viw_cmn.h"

static	WCHAR	*Grd_ttl[] =	{	{ _T("Name")		},
									{ _T("Abbreviation")	} };

static	GRD_CLM	Grd_clm[] =		{	{ 298, GAL_LFT, GCT_NML,  12 },		// TypeName
									{ 186, GAL_LFT, GCT_NML,  12 } };	// Abbrebiation

#define	IDC_GRD_MSG		(8192)	// Copy destination selecting grid

#define	CLM_DAT			0		// Data keeping column

#define	CLM_NAM			0		// TypeName
#define	CLM_ABR			1		// Abbrebiation


// CMarketSegmentDlg ダイアログ

IMPLEMENT_DYNAMIC(CMarketSegmentDlg, CDialog)

CMarketSegmentDlg::CMarketSegmentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMarketSegmentDlg::IDD, pParent)
{
	m_Msg_grd = NULL;
	m_pPms_mng = NULL;
	m_pDbs_obj = NULL;
	m_Ope_cod = NULL;
	m_Brc_idx = -1;
	m_Fcl_idx = -1;
	memset( m_Brc_rot, 0, sizeof(m_Brc_rot) );
	memset( m_Fcl_rot, 0, sizeof(m_Fcl_rot) );
	memset( m_Msg_rot, 0, sizeof(m_Msg_rot) );
}

CMarketSegmentDlg::~CMarketSegmentDlg()
{
	if ( m_Msg_grd )	delete m_Msg_grd;
	Del_All( m_Brc_rot );
	Del_All( m_Fcl_rot );
	Del_All( m_Msg_rot );
}

void CMarketSegmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

int	 CMarketSegmentDlg::InitData( WCHAR *opc, WCHAR *bcd, WCHAR *fcd )
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
	Del_All( m_Msg_rot );
	bcx->ResetContent();
	fcx->ResetContent();
	m_Msg_grd->ResetContent();

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

void	CMarketSegmentDlg::DispData( MKT_SEG *msg, int row )
{
	m_Msg_grd->SetCellText( CLM_NAM, row, EDT_DBF_WST(msg->nam, _T("")) );
	m_Msg_grd->SetCellText( CLM_ABR, row, EDT_DBF_WST(msg->abr, _T("")) );
	m_Msg_grd->SetData( CLM_DAT, row, (int)(LONG_PTR)msg );
}


void	CMarketSegmentDlg::SetEditData( MKT_SEG *msg )
{
	GetDlgItem(IDC_EDIT_NAME)->SetWindowTextW( !!msg ? EDT_DBF_WST(msg->nam, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_ABBREVIATION)->SetWindowTextW( !!msg ? EDT_DBF_WST(msg->abr, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_COMMENT)->SetWindowTextW( !!msg ? EDT_DBF_WST(msg->cmt, _T("")):_T(""));
	ButtonEnable();
}

void	CMarketSegmentDlg::ButtonEnable()	
{
	int			enb, sel;
	enb  = ((CComboBox *)GetDlgItem(IDC_COMBO_BRANCH))->GetCurSel() != -1;
	enb &= ((CComboBox *)GetDlgItem(IDC_COMBO_FACILITY))->GetCurSel() != -1;

	sel = m_Msg_grd->GetSelRow() > 0;

	GetDlgItem(IDC_EDIT_NAME)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ABBREVIATION)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_COMMENT)->EnableWindow( enb );
	GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow( sel && enb );
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow( enb);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow( sel && enb );
}

DWORD CMarketSegmentDlg::ConfirmSaveChaned( int msg, int fce )
{
	Csr			csr=NULL;
	MKT_SEG		*seg;
	int			row, cnt, ans=IDYES;
	WCHAR		*erm=NULL;

	while( csr=Nxt_Lnk( m_Msg_rot, csr ) ){
		seg = Rec_Of(MKT_SEG, csr);
		if ( !Nul_Fld(seg->mdt, FLD_DTM) || !Nul_Fld(seg->ddt, FLD_DTM) || Nul_Fld(seg->id, FLD_INT) ){
			break;
		}
	}
	if ( !!csr ){
		cnt = m_Msg_grd->GetLineCount( FALSE );
		for ( row=1; row<=cnt; row++ ){
			seg = (MKT_SEG *)(LONG_PTR)m_Msg_grd->GetData( CLM_DAT, row );
			//DBS_INT_SET(seg->odr, &row );
		}
		ans = Viw_Cfm_Svc( m_Msg_rot, Msg_Sav_Msg, msg, fce, this, m_pDbs_obj, m_pPms_mng );
	}
	return( ans );
}



BEGIN_MESSAGE_MAP(CMarketSegmentDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_CBN_SELCHANGE(IDC_COMBO_BRANCH, &CMarketSegmentDlg::OnCbnSelchangeComboBranch)
	ON_CBN_SELCHANGE(IDC_COMBO_FACILITY, &CMarketSegmentDlg::OnCbnSelchangeComboFacility)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CMarketSegmentDlg::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CMarketSegmentDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CMarketSegmentDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDCANCEL, &CMarketSegmentDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CMarketSegmentDlg::OnBnClickedOk)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SORTORDER, &CMarketSegmentDlg::OnDeltaposSpinSortorder)
END_MESSAGE_MAP()


// CMarketSegmentDlg メッセージ ハンドラ
BOOL CMarketSegmentDlg::OnInitDialog()
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

	m_Msg_grd = new CSimpleGrid();
	m_Msg_grd->Create( gdf, *rct, IDC_GRD_MSG, this );
	for ( idx=0; idx<memcnt(Grd_ttl); idx++ ){
		m_Msg_grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// グリッドイベント
LRESULT CMarketSegmentDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;
	int				row;
	MKT_SEG			*msg;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:	row = ntf->row;
								if ( row > 0 ){
									msg = (MKT_SEG *)(LONG_PTR)m_Msg_grd->GetData( CLM_DAT, row );
									if ( msg ){
										SetEditData( msg );
									}
								}
								break;
		}
		free( ntf );
	}

	return( rsl );
}

// UPDOWN spinbox change evennt
void CMarketSegmentDlg::OnDeltaposSpinSortorder(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int			idx, six=0, dix=0;
	MKT_SEG		*src, *dst;
	Day			dte[1];

	if ( ( idx = m_Msg_grd->GetSelRow() ) > 0 ){
		if ( pNMUpDown->iDelta > 0 ){
			if ( idx < m_Msg_grd->GetLineCount( FALSE ) ){
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
			src = (MKT_SEG *)(LONG_PTR)m_Msg_grd->GetData( CLM_DAT, six );
			dst = (MKT_SEG *)(LONG_PTR)m_Msg_grd->GetData( CLM_DAT, dix );
			DBS_DTM_SET(src->mdt, dte);
			m_Msg_grd->SetData( CLM_DAT, six, (int)(LONG_PTR)dst );
			m_Msg_grd->SetData( CLM_DAT, dix, (int)(LONG_PTR)src );
			DispData( src, dix );
			DispData( dst, six );
			m_Msg_grd->SetSelectCell( CLM_DAT, dix );
		}
	}



	*pResult = 0;
}

// Branch combobox change
void CMarketSegmentDlg::OnCbnSelchangeComboBranch()
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
			Del_All( m_Msg_rot );
			fcx->ResetContent();
			m_Msg_grd->ResetContent();
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
void CMarketSegmentDlg::OnCbnSelchangeComboFacility()
{
	int			idx, bix, row=0;
	Csr			csr=NULL;
	BRC_INF		*brc=NULL;
	FCL_INF		*fcl=NULL;
	MKT_SEG		*msg, key[1];
	WCHAR		*erm=NULL;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);

	memset( key, 0, sizeof(key) );

	if ( ( ( idx = fcx->GetCurSel() ) != -1 ) && idx!=m_Fcl_idx ){
		if ( ConfirmSaveChaned( TRUE, FALSE )!=IDCANCEL ){
			Del_All( m_Msg_rot );
			m_Msg_grd->ResetContent();
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
			if ( Msg_Get_Msg( m_Msg_rot, key, FALSE, &erm, m_pDbs_obj, m_pPms_mng ) ){
				m_Msg_grd->AddRow( Cnt_Lnk( m_Msg_rot ) );
				while( csr = Nxt_Lnk( m_Msg_rot, csr ) ){
					msg = Rec_Of(MKT_SEG, csr);
					Set_Nul(msg->mdt, FLD_DTM, TRUE);
					DispData( msg, ++row );
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
void CMarketSegmentDlg::OnBnClickedButtonUpdate()
{
	int			row;
	MKT_SEG		tmp[1], *msg;
	Day			dte[1];

	if ( ( row = m_Msg_grd->GetSelRow() ) > 0 ){
		msg = (MKT_SEG *)(LONG_PTR)m_Msg_grd->GetData( CLM_DAT, row );
		WIN_TXT_DBS(tmp->nam, IDC_EDIT_NAME, this);
		WIN_TXT_DBS(tmp->abr, IDC_EDIT_ABBREVIATION, this);
		WIN_TXT_DBS(tmp->cmt, IDC_EDIT_COMMENT, this);
		if ( !Nul_Fld(tmp->nam,FLD_WST) ){
			DBS_WST_CPY(msg->nam, tmp->nam);
			DBS_WST_CPY(msg->abr, tmp->abr);
			DBS_WST_CPY(msg->cmt, tmp->cmt);
			Get_Dte( dte );
			DBS_DTM_SET(msg->mdt, dte);
			DispData( msg, row );
			SetEditData( NULL );
			m_Msg_grd->SetSelectCell(-1, -1);
		}
		else{
			MessageBox( _T("Name is requierd item"), NULL, MB_ICONEXCLAMATION|MB_OK );
		}
	}
}

// Add button click
void CMarketSegmentDlg::OnBnClickedButtonAdd()
{
	int			row, idx, bix;
	Csr			csr;
	MKT_SEG		tmp[1], *msg;
	BRC_INF		*brc=NULL;
	FCL_INF		*fcl=NULL;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);


	if ( ( idx = fcx->GetCurSel() ) != -1 && ( bix = bcx->GetCurSel() ) != -1 ){
		if ( bix>0 )	brc = (BRC_INF *)(LONG_PTR)fcx->GetItemData( bix );
		if ( idx>0 )	fcl = (FCL_INF *)(LONG_PTR)fcx->GetItemData( idx );
		WIN_TXT_DBS(tmp->nam, IDC_EDIT_NAME, this);
		WIN_TXT_DBS(tmp->abr, IDC_EDIT_ABBREVIATION, this);
		WIN_TXT_DBS(tmp->cmt, IDC_EDIT_COMMENT, this);
		if ( !Nul_Fld(tmp->nam,FLD_WST) ){
			if ( csr=Add_Lnk( m_Msg_rot, NULL, sizeof(MKT_SEG) ) ){
				msg = Rec_Of(MKT_SEG, csr);
				DBS_WST_SET(msg->opc, m_pPms_mng->opc);
				if ( !!brc ){
					DBS_WST_SET(msg->brc, DBS_WST_PTR(brc->brc));
				}
				if ( !!fcl ){
					DBS_WST_SET(msg->fcc, DBS_WST_PTR(fcl->fcc));
				}
				DBS_WST_CPY(msg->nam, tmp->nam);
				DBS_WST_CPY(msg->abr, tmp->abr);
				DBS_WST_CPY(msg->cmt, tmp->cmt);
				row = m_Msg_grd->GetLineCount( FALSE );
				m_Msg_grd->AddRow( 1 );
				DispData( msg, row+1 );
				SetEditData( NULL );
				m_Msg_grd->SetSelectCell(-1, -1);
			}
		}
		else{
			MessageBox( _T("Name is requierd item"), NULL, MB_ICONEXCLAMATION|MB_OK );
		}
	}
}

// Delete button click
void CMarketSegmentDlg::OnBnClickedButtonDelete()
{
	int			row;
	MKT_SEG		*msg;
	Day			dte[1];

	if ( ( row = m_Msg_grd->GetSelRow() ) > 0 ){
		msg = (MKT_SEG *)(LONG_PTR)m_Msg_grd->GetData( CLM_DAT, row );
		if ( MessageBox( _T("Are you sure to delete?"), NULL, MB_ICONQUESTION|MB_YESNO ) == IDYES ){
			if ( Nul_Fld(msg->id, FLD_INT) ){
				Del_Lnk( m_Msg_rot, Csr_Of(msg) );
			}
			else{
				Get_Dte( dte );
				DBS_DAY_SET(msg->ddt, dte);
			}
			m_Msg_grd->DelRow( row );
			m_Msg_grd->SetSelectCell(-1, -1);
			m_Msg_grd->Redraw();
		}
	}
}



// Close button click
void CMarketSegmentDlg::OnBnClickedCancel()
{
	if ( ConfirmSaveChaned( TRUE, TRUE )==IDYES ){
		OnCancel();
	}
}

// Close winth save button click
void CMarketSegmentDlg::OnBnClickedOk()
{
	if ( ConfirmSaveChaned( FALSE, FALSE )==IDYES ){
		OnOK();
	}
}

BOOL CMarketSegmentDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}
