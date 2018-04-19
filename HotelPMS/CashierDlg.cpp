// CashierDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "CashierDlg.h"
#include "ProductSelDlg.h"
#include "PaymentDlg.h"

#include ".\fnc\csh_fnc.h"

// CCashierDlg ダイアログ

IMPLEMENT_DYNAMIC(CCashierDlg, CDialog)

CCashierDlg::CCashierDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCashierDlg::IDD, pParent)
{
	m_Csh_grd = NULL;
	m_Tax_grd = NULL;
	m_Dps_grd = NULL;
	m_pDbs_obj = NULL;
	m_pPms_mng = NULL;
	m_Rsv_id = 0;
	m_Tab_idx = -1;
	m_Crc_cvs = NULL;
	m_Rom_cnt = 0;
	m_Bls_amt = 0;
	m_Chk_out = FALSE;
	memset( m_Inh_rot, 0, sizeof(m_Inh_rot) );
	memset( m_Tax_rot, 0, sizeof(m_Tax_rot) );
	memset( m_Ccv_rot, 0, sizeof(m_Ccv_rot) );
	memset( m_Dps_rot, 0, sizeof(m_Dps_rot) );
}

CCashierDlg::~CCashierDlg()
{
	if ( m_Csh_grd )	delete m_Csh_grd;
	if ( m_Tax_grd )	delete m_Tax_grd;
	if ( m_Dps_grd )	delete m_Dps_grd;
	Del_All( m_Tax_rot );
	Del_All( m_Ccv_rot );
	Del_All( m_Dps_rot );
	Csh_Dps_Ihl( this );
}

void CCashierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCashierDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ROOM, &CCashierDlg::OnTcnSelchangeTabRoom)
	ON_BN_CLICKED(IDC_BUTTON_ADDPRODUCT, &CCashierDlg::OnBnClickedButtonAddproduct)
	ON_CBN_SELCHANGE(IDC_COMBO_CURRENCY, &CCashierDlg::OnCbnSelchangeComboCurrency)
	ON_BN_CLICKED(IDC_BUTTON_PARTICALPAYMENT, &CCashierDlg::OnBnClickedButtonParticalpayment)
	ON_BN_CLICKED(IDC_BUTTON_ALLPAYMENT, &CCashierDlg::OnBnClickedButtonAllpayment)
	ON_BN_CLICKED(IDC_BUTTON_ADDDEPOSIT, &CCashierDlg::OnBnClickedButtonAdddeposit)
	ON_BN_CLICKED(IDC_CHECK_ALL, &CCashierDlg::OnBnClickedCheckAll)
	ON_BN_CLICKED(IDC_BUTTON_TRANSFER, &CCashierDlg::OnBnClickedButtonTransfer)
END_MESSAGE_MAP()

// CCashierDlg メッセージ ハンドラ

BOOL CCashierDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CHILD;

	return CDialog::PreCreateWindow(cs);
}

BOOL CCashierDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	WINDOWPLACEMENT	wpc[1];

	//ModifyStyleEx( 0, WS_EX_TOPMOST, TRUE );

	GetDlgItem(IDC_TAB_ROOM)->GetWindowPlacement( wpc );
	wpc->rcNormalPosition.top += 24;
	wpc->rcNormalPosition.left += 1;
	m_Csh_grd = Csh_Mak_Grd( IDC_GRD_CSH, &wpc->rcNormalPosition, this );
	GetDlgItem(IDC_STATIC_TAX)->GetWindowPlacement( wpc );
	m_Tax_grd = Csh_Mak_Grd( IDC_GRD_CTX, &wpc->rcNormalPosition, this );
	GetDlgItem(IDC_STATIC_DEPOSIT)->GetWindowPlacement( wpc );
	wpc->rcNormalPosition.top += 12;
	m_Dps_grd = Csh_Mak_Grd( IDC_GRD_DPS, &wpc->rcNormalPosition, this );

	if ( m_Chk_out ){
		GetDlgItem(IDC_BUTTON_PARTICALPAYMENT)->ShowWindow( SW_HIDE );
		GetDlgItem(IDC_BUTTON_ALLPAYMENT)->SetWindowText( _T("CheckOut") );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// グリッドイベント
LRESULT CCashierDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;
	int				idx, row;
	WCHAR			*txt;
	CSH_INH_DAT		*inh;

	if ( ntf ){
		//dtl = (CSH_SAL_DTL *)m_Csh_grd->GetData(CLM_DAT, row = ntf->row );
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:	row = ntf->row;
								//if ( row > 0 ){
								//	eqt = (EQP_TYP *)(LONG_PTR)m_Typ_grd->GetData( CLM_DAT, row );
								//	if ( eqt ){
								//		SetEditData( eqt );
								//	}
								//}
								break;
		case GN_EDITCOMPLETE:	if ( txt = (WCHAR *)lParam ){
									//Csh_Edt_Cmp( ntf->cid, ntf->clm, ntf->row, txt, this, m_pPms_mng ); 
									if ( ntf->cid==IDC_GRD_CSH ){
										Csh_Ecp_Pdt( ntf->cid, ntf->clm, ntf->row, txt, this, m_pPms_mng ); 
									}
									else{
										idx = ((CTabCtrl *)GetDlgItem(IDC_TAB_ROOM))->GetCurSel();
										if ( idx!= -1 ){
											inh = Rec_Of(CSH_INH_DAT, Idx_Lnk(m_Inh_rot, idx));
											Csh_Ecp_Ttl( ntf->row, inh->sal->dtl, txt, m_Tax_grd, this, m_pPms_mng );
											Csh_Dsp_Ttl( m_Tax_grd, inh->sal->dtl, m_Tax_rot, m_Dps_rot, m_Crc_cvs, m_pPms_mng );
										}
									}
									free( txt );
								}
								break;
		case GN_QUERYINPUTTYPE:	rsl = Csh_Clm_Typ( ntf->cid, ntf->clm, ntf->row, this );
								break;
		}
		free( ntf );
	}

	return( rsl );
}

// Tab select change
void CCashierDlg::OnTcnSelchangeTabRoom(NMHDR *pNMHDR, LRESULT *pResult)
{
	int			idx;

	if ( ( idx = ((CTabCtrl *)GetDlgItem(IDC_TAB_ROOM))->GetCurSel() ) != m_Tab_idx ){
		m_Tab_idx = idx;
		Csh_Dsp_Sal( this, m_pPms_mng );
	}
	*pResult = 0;
}

// Add product button click
void CCashierDlg::OnBnClickedButtonAddproduct()
{
	CProductSelDlg		*dlg;
	Lnk					rot[1];

	memset( rot, 0, sizeof(rot) );

	if ( dlg = new CProductSelDlg(this) ){
		if ( dlg->InitData( m_Ope_cod, m_Brc_cod, m_Fcl_cod ) ){
			dlg->m_Sel_rot = rot;
			if ( dlg->DoModal() == IDOK ){
				Csh_All_Sal( rot, this, m_pDbs_obj, m_pPms_mng );
			}
		}
		delete dlg;
	}

	Del_All( rot );
}

// Add deposit button click
void CCashierDlg::OnBnClickedButtonAdddeposit()
{
	if ( ((CTabCtrl *)GetDlgItem(IDC_TAB_ROOM))->GetCurSel() != -1 ){
		Csh_Add_Dps( this, m_pDbs_obj, m_pPms_mng );
	}
}

// partical payment buttom click
void CCashierDlg::OnBnClickedButtonParticalpayment()
{
	if ( ((CTabCtrl *)GetDlgItem(IDC_TAB_ROOM))->GetCurSel() != -1 ){
		Csh_Prt_Pay( this, m_pDbs_obj, m_pPms_mng );
	}
}

// All payment buttom click
void CCashierDlg::OnBnClickedButtonAllpayment()
{
	if ( ((CTabCtrl *)GetDlgItem(IDC_TAB_ROOM))->GetCurSel() != -1 ){
		if ( Csh_All_Pay( this, m_pDbs_obj, m_pPms_mng ) ){
//			OnCancel();
		}
	}
}

// transfer buttom click
void CCashierDlg::OnBnClickedButtonTransfer()
{
	if ( !Csh_Trs_Far( this, m_pDbs_obj, m_pPms_mng ) ){
		MessageBox( _T("System error! Insuffcient Memory!!"), NULL, MB_ICONSTOP|MB_OK );
		OnCancel();
	}
}

// currency combobox change event
void CCashierDlg::OnCbnSelchangeComboCurrency()
{
	int			idx;
	CRC_CVS		*ccv;
	CComboBox	*cbx = (CComboBox *)GetDlgItem(IDC_COMBO_CURRENCY);

	if ( ( idx = cbx->GetCurSel() ) != -1 ){
		ccv = (CRC_CVS *)(LONG_PTR)cbx->GetItemData( idx );
		if ( m_Crc_cvs != ccv ){
			m_Crc_cvs = ccv;
			Csh_Dsp_Crt( ccv, GetDlgItem(IDC_STATIC_RATE) );
			Csh_Dsp_Sal( this, m_pPms_mng );
		}
	}
}

// CheckAll checkbox click
void CCashierDlg::OnBnClickedCheckAll()
{
	int				chk;

	chk = ((CButton *)GetDlgItem(IDC_CHECK_ALL))->GetCheck();
	Csh_Chk_All( chk, this, m_pPms_mng );
}

// operations
BOOL CCashierDlg::SetupCashier( WCHAR *opc, WCHAR *brc, WCHAR *fcc, int rii, int rdi )
{
	int			chk, idx=0, cur=0;
	Csr			csr=NULL, cse=NULL;
	WCHAR		*erm=NULL, buf[1024];
	CSH_INH_DAT	*inh;
	CSH_SAL_DTL	*dtl;
	CSH_DPS_DAT	*dps;
	CRC_CVS		cky[1], *ccv;
	CTabCtrl	*tab = (CTabCtrl *)GetDlgItem(IDC_TAB_ROOM);
	CComboBox	*cbx = (CComboBox *)GetDlgItem(IDC_COMBO_CURRENCY);

	m_Ope_cod = opc;
	m_Brc_cod = brc;
	m_Fcl_cod = fcc;
	m_Rsv_id = rii;

	memset( cky, 0, sizeof(cky) );
	DBS_WST_SET(cky->opc, opc);
	DBS_WST_SET(cky->brc, brc);
	DBS_WST_SET(cky->fcc, fcc);

	Csh_Dps_Ihl( this );
	Del_All( m_Tax_rot );
	Del_All( m_Dps_rot );
	tab->DeleteAllItems();

	m_Tab_idx = -1;
	m_Csh_grd->ResetContent();
	m_Dps_grd->ResetContent();
	cbx->ResetContent();

	chk  = Csh_Get_Tax( &erm, this, m_pDbs_obj, m_pPms_mng );
	if ( chk )	chk = Csh_Get_Ccv( m_Ccv_rot, cky, &erm, m_pDbs_obj, m_pPms_mng );
	if ( chk )	chk = Csh_Inh_Sal( m_Inh_rot, m_Rsv_id, &erm, m_pDbs_obj, m_pPms_mng );

	if ( chk ){
		m_Rom_cnt = Cnt_Lnk( m_Inh_rot );
		if ( chk = Csh_Get_Dat( m_Inh_rot, &erm, m_pDbs_obj, m_pPms_mng ) ){
			if ( chk = Csh_Get_Dps( m_Inh_rot, 0, &erm, m_pDbs_obj, m_pPms_mng ) ){
				while( csr=Nxt_Lnk( m_Inh_rot, csr ) ){
					inh = Rec_Of(CSH_INH_DAT, csr);
					swprintf_s( buf, memcnt(buf), _T("%s(%s %s)"), 
						EDT_DBF_WST(inh->rmn,_T("")), EDT_DBF_WST(inh->fsn,_T("")), EDT_DBF_WST(inh->lsn,_T("")) );
					tab->InsertItem( idx, buf );
					Set_Nul(inh->sal->sal->mdt, FLD_DTM, TRUE);
					while( cse=Nxt_Lnk( inh->sal->dtl, cse ) ){
						dtl = Rec_Of(CSH_SAL_DTL, cse);
						Set_Nul(dtl->dtl->mdt, FLD_DTM, TRUE);
					}
					while( cse=Nxt_Lnk( inh->dps, cse ) ){
						dps = Rec_Of(CSH_DPS_DAT, cse);
						Set_Nul(dps->dtl->mdt, FLD_DTM, TRUE);
						Set_Nul(dps->pmt->mdt, FLD_DTM, TRUE);
					}
					if ( DBS_INT_VAL(inh->rdi) == rdi ){
						cur = idx;
					}
					idx++;
				}

				cbx->InsertString( -1, _T("") );
				while( csr=Nxt_Lnk( m_Ccv_rot, csr ) ){
					ccv = Rec_Of(CRC_CVS, csr);
					if ( !DBS_INT_VAL(ccv->isb) ){
						cbx->SetItemData( idx=cbx->InsertString( -1, EDT_DBF_WST(ccv->crc, _T("")) ), (LONG_PTR)ccv );
						if ( !!DBS_INT_VAL(ccv->iss) ){
							cbx->SetCurSel( idx );
							Csh_Dsp_Crt( ccv, GetDlgItem(IDC_STATIC_RATE) );
							m_Crc_cvs = ccv;
						}
					}
				}
				if ( !!Cnt_Lnk( m_Inh_rot ) ){
					m_Tab_idx = 0;
					Csh_Dsp_Sal( this, m_pPms_mng );
				}
				tab->SetCurSel( cur );
				m_Tab_idx = cur;
				Csh_Dsp_Sal( this, m_pPms_mng );
			}
		}
	}
	if ( !chk ){
		Csh_Dps_Ihl( this );
		if ( erm ){
			MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
	}

	return( chk );
}

// save
BOOL CCashierDlg::Save( void )
{
	int			chk;

	chk = Csh_Cls_Sav( this, m_pDbs_obj, m_pPms_mng );
		
	return( chk );
}

// updating exist check
BOOL CCashierDlg::IsUpdateExist( void )
{
	Csr			csr=NULL, cse=NULL;
	CSH_INH_DAT	*inh;
	CSH_SAL_DTL	*dtl;
	CSH_DPS_DAT	*dps;

	while( !cse && !!( csr=Nxt_Lnk( m_Inh_rot, csr ) ) ){
		inh = Rec_Of(CSH_INH_DAT, csr);
		if ( Cnt_Lnk( inh->sal->dtl ) ){
			if ( Nul_Fld(inh->sal->sal->id, FLD_INT) || !Nul_Fld(inh->sal->sal->mdt, FLD_DTM) || !Nul_Fld(inh->sal->sal->ddt, FLD_DTM) ){
				break;
			}
			while( cse=Nxt_Lnk( inh->sal->dtl, cse ) ){
				dtl = Rec_Of(CSH_SAL_DTL, cse);
				if ( Nul_Fld(dtl->dtl->id, FLD_INT) || !Nul_Fld(dtl->dtl->mdt, FLD_DTM) || !Nul_Fld(dtl->dtl->ddt, FLD_DTM) ){
					break;
				}
			}
			if ( !!cse )	break;
		}
		while( cse=Nxt_Lnk( inh->dps, cse ) ){
			dps = Rec_Of(CSH_DPS_DAT, cse);
			if ( Nul_Fld(dps->dtl->id, FLD_INT) || !Nul_Fld(dps->dtl->mdt, FLD_DTM) || !Nul_Fld(dps->dtl->ddt, FLD_DTM) ){
				break;
			}
		}
		if ( !!cse )	break;
	}

	return( !!csr );
}
