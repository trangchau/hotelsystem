// CategorySelectDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "CategorySelectDlg.h"
#include "..\Common\bsc_def.h"
#include ".\fnc\ecg_sql.h"
#include ".\fnc\viw_cmn.h"



// CCategorySelectDlg ダイアログ

static	WCHAR	*Grd_ttl[] =	{	{ _T("Sel")			},
									{ _T("Category")	} };

static	GRD_CLM	Grd_clm[] =		{	{  24, GAL_CTR, GCT_CHK,  12 },		// Cpy
									{ 192, GAL_LFT, GCT_NML,  12 } };	// Destination

#define	IDC_GRD_DTL		(8192)	// Category selecting grid

IMPLEMENT_DYNAMIC(CCategorySelectDlg, CDialog)

CCategorySelectDlg::CCategorySelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCategorySelectDlg::IDD, pParent)
{
	m_Sel_grd = NULL;
	m_pPms_mng = NULL;
	m_pDbs_obj = NULL;
	memset( m_Cat_lst, 0, sizeof(m_Cat_lst) );
	m_Ope_cod = NULL;
	m_Brc_cod = NULL;
	m_Fcl_cod = NULL;
	m_Sel_dci = 0;
	memset( m_Sel_nam, 0, sizeof(m_Sel_nam) );
	m_Result = FALSE;

}

CCategorySelectDlg::~CCategorySelectDlg()
{
	if ( m_Sel_grd )	delete	m_Sel_grd;
	Del_All( m_Cat_lst );
}

void CCategorySelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCategorySelectDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_BN_CLICKED(IDCANCEL, &CCategorySelectDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CCategorySelectDlg::OnBnClickedOk)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCategorySelectDlg メッセージ ハンドラ

BOOL CCategorySelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GRD_DEF			gdf[1];
	int				idx;
	RECT			rct[1];

	memset( gdf, 0, sizeof(gdf) );
	rct->left = 10;
	rct->top = 10;
	rct->right = 0;
	rct->bottom = 0;

	gdf->lnh = 20;
	gdf->vlc = 8;
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

	m_Sel_grd = new CSimpleGrid();
	m_Sel_grd->Create( gdf, *rct, IDC_GRD_DTL, this );
	for ( idx=0; idx<memcnt(Grd_ttl); idx++ ){
		m_Sel_grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}

	SetTimer( 1, 10, NULL );
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CCategorySelectDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer( nIDEvent );

	Csr				csr=NULL;
	DSC_CAT			*cat;
	WCHAR			*erm=NULL;
	ECG_SQL_PRM		prm[1];
	int				row=1;

	memset( prm, 0, sizeof(prm) );
	prm->opc = m_Ope_cod;
	prm->brc = m_Brc_cod;
	prm->fcc = m_Fcl_cod;
	if ( Ecg_Cst_Lst( m_Cat_lst, prm, &erm, m_pDbs_obj, m_pPms_mng ) ){
		while ( csr=Nxt_Lnk( m_Cat_lst, csr ) ){
			cat = Rec_Of(DSC_CAT, csr);
			m_Sel_grd->AddRow(1);
			m_Sel_grd->SetCellText( 1, row, DBS_WST_PTR(cat->nam) );
			m_Sel_grd->SetData( 1, row++, (int)(LONG_PTR)cat );
		}
		m_Sel_grd->Redraw();
	}
	else{
		if ( erm ){
			MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
		m_Result = FALSE;
		PostMessage( WM_CLOSE, 0, 0 );
	}

	CDialog::OnTimer(nIDEvent);
}

LRESULT CCategorySelectDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	WCHAR			*txt;
	int				idx, chk;
	LRESULT			rsl=TRUE;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_EDITCOMPLETE:	if ( !!( txt = (WCHAR *)lParam) ){
									free( txt );
								}
								break;
		case GN_SELECTCHANGE:	break;
		case GN_LBUTTONCLICK:	if ( ntf->row>0 ){
									chk = m_Sel_grd->GetCheck(0, ntf->row);
									m_Sel_grd->SetCheck(0, ntf->row,!chk);
									if ( !chk ){
										for ( idx=1; idx<=m_Sel_grd->GetLineCount(FALSE); idx++ ){
											if ( idx != ntf->row )	m_Sel_grd->SetCheck(0, idx, FALSE);
										}
									}
								}
								break;
		case GN_LBUTTONDBLCLICK:break;						// 左ボタンクリック
		case GN_QUERYCOMBODATA:	break;
		case GN_COMBOCHANGED:	break;
		case GN_QUERYINPUTTYPE:	if ( ntf->clm == 0 )	rsl=GCT_CHK;
								break;
		case GN_QUERYCOLORDATA:	break;
		}
		free( ntf );
	}

	return( rsl );
}

void CCategorySelectDlg::OnBnClickedOk()
{
	DSC_CAT		*cat;
	int			idx, cnt=0;

	for ( idx=1; idx<=m_Sel_grd->GetLineCount(FALSE); idx++ ){
		if ( m_Sel_grd->GetCheck( 0, idx ) ){
			cat = (DSC_CAT *)(LONG_PTR)m_Sel_grd->GetData( 1, idx );
			m_Sel_dci = DBS_INT_VAL(cat->id);
			wcscpy_s( m_Sel_nam, memcnt(m_Sel_nam), DBS_WST_PTR(cat->nam) );
			m_Result = TRUE;
		}
	}
	if ( !m_Result ){
		MessageBox( _T("No selected"), NULL, MB_ICONEXCLAMATION|MB_OK );
	}
	else{
		OnOK();
	}
}

void CCategorySelectDlg::OnBnClickedCancel()
{
	m_Result = FALSE;
	OnCancel();
}

BOOL CCategorySelectDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}
