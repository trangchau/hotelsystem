// ProductDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "ProductDlg.h"

#include ".\fnc\dsc_fnc.h"
#include ".\fnc\mfm_sql.h"
#include ".\fnc\dsc_sql.h"
#include ".\fnc\viw_cmn.h"

// CProductDlg ダイアログ

//IDC_COMBO_BRANCH
//IDC_COMBO_FACILITY
//IDC_COMBO_CATEGORY
//IDC_EDIT_CATEGORYNAME
//IDC_EDIT_ABBREVIATION
//IDC_EDIT_CODE
//IDC_EDIT_READABLECODE
//IDC_EDIT_ARRIVALDATE
//IDC_EDIT_DEPARTUREDATE
//IDC_COMBO_PRODUCTKIND
//IDC_COMBO_TRANSFERORIGIN
//IDC_COMBO_PRICEKIND
//IDC_EDIT_PRICE
//IDC_CHECK_SALEENABLE
//IDC_CHECK_ATTRIBUTE

IMPLEMENT_DYNAMIC(CProductDlg, CDialog)

CProductDlg::CProductDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProductDlg::IDD, pParent)
{
	m_Pdt_grd = NULL;
	m_Apl_grd = NULL;
	m_pPms_mng = NULL;
	m_pDbs_obj = NULL;
	m_Ope_cod = NULL;
	m_Brc_cod = NULL;
	m_Fcl_cod = NULL;
	m_Brc_idx = -1;
	m_Fcl_idx = -1;
	m_Cat_idx = -1;
	m_Pdk_idx = -1;
	memset( m_Brc_rot, 0, sizeof(m_Brc_rot) );
	memset( m_Fcl_rot, 0, sizeof(m_Fcl_rot) );
	memset( m_Cat_rot, 0, sizeof(m_Cat_rot) );
	memset( m_Pdt_rot, 0, sizeof(m_Pdt_rot) );
	memset( m_Tax_rot, 0, sizeof(m_Tax_rot) );
}

CProductDlg::~CProductDlg()
{
	if ( m_Pdt_grd )	delete m_Pdt_grd;
	if ( m_Apl_grd )	delete m_Apl_grd;
	Del_All( m_Brc_rot );
	Del_All( m_Fcl_rot );
	Del_All( m_Cat_rot );
	Del_All( m_Pdt_rot );
	Del_All( m_Tax_rot );
}

void CProductDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CProductDlg::SetupComponent()
{
	int				idx, cnt;
	WINDOWPLACEMENT	wpc[1];
	RECT			*rct;
	DSC_CDN			*cdn;
	CComboBox		*cbx;

	struct	{
		CWnd	*wnd;
		int		lmt;
		DWORD	cid;
	}Cpn_[] = {
		{ NULL,      255, IDC_EDIT_CATEGORYNAME	},
		{ NULL,      255, IDC_EDIT_ABBREVIATION	},
		{ NULL,      255, IDC_EDIT_CODE			},
		{ NULL,      255, IDC_EDIT_READABLECODE	},
		{ m_Edt_vss,  -1, IDC_EDIT_ARRIVALDATE	},
		{ m_Edt_vse,  -1, IDC_EDIT_DEPARTUREDATE},
		{ m_Edt_prc,  13, IDC_EDIT_PRICE		}
	};

	for ( idx=0; idx<memcnt(Cpn_); idx++ ){
		if ( !!Cpn_[idx].wnd ){
			Cpn_[idx].wnd->SubclassDlgItem(Cpn_[idx].cid, this);
		}
		if ( Cpn_[idx].lmt!=-1 )	((CEdit *)GetDlgItem(Cpn_[idx].cid))->SetLimitText( Cpn_[idx].lmt );
	}

	m_Edt_prc->DecimalPlacess = m_pPms_mng->bsc->adp;
	m_Edt_prc->RoundMethod = CEDT_ROUND_UP;

	GetDlgItem(IDC_COMBO_BRANCH)->GetWindowPlacement( wpc );
	rct = &wpc->rcNormalPosition;
	//rct->left = 10;
	rct->top += 30;
	rct->right = 0;
	rct->bottom = 0;
	m_Pdt_grd = Dsc_Mak_Grd( IDC_GRD_PDT, rct, this );
	GetDlgItem(IDC_STATIC_APL)->GetWindowPlacement( wpc );
	m_Apl_grd = Dsc_Mak_Grd( IDC_GRD_APL, rct, this );

	cbx = (CComboBox *)GetDlgItem(IDC_COMBO_PRODUCTKIND);
	cnt = Viw_Get_Dsc( &cdn, DSC_DRK );
	for ( idx=0; idx<cnt; idx++, cdn++ ){
		cbx->SetItemData( cbx->InsertString( -1, cdn->nam ), cdn->cod );
	}

}

int	 CProductDlg::InitData( WCHAR *opc, WCHAR *bcd, WCHAR *fcd )
{
	int			chk;
	Csr			csr=NULL;
	WCHAR		*erm=NULL;
	BRC_INF		*brc;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);
	CComboBox	*ccx = (CComboBox *)GetDlgItem(IDC_COMBO_CATEGORY);

	m_Ope_cod = opc;
	m_Brc_cod = bcd;
	m_Fcl_cod = fcd;

	m_Brc_idx = -1;
	m_Fcl_idx = -1;
	m_Cat_idx = -1;
	Del_All( m_Brc_rot );
	Del_All( m_Fcl_rot );
	Del_All( m_Cat_rot );
	Del_All( m_Pdt_rot );
	bcx->ResetContent();
	fcx->ResetContent();
	ccx->ResetContent();
	m_Pdt_grd->ResetContent();

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

	((CComboBox *)GetDlgItem(IDC_COMBO_PRICEKIND))->ResetContent();

	return( chk );
}

void	CProductDlg::DispData( DSC_RPT *pdt, int row )
{
	WCHAR		buf[96], *ptr;
	int			idx, cnt, pos;
	DSC_CDN		*cdn;

	cnt = Viw_Get_Dsc( &cdn, DSC_DRK );

	m_Pdt_grd->SetCellText( CLM_NAM, row, EDT_DBF_WST(pdt->nam, _T("")) );
	m_Pdt_grd->SetCellText( CLM_ABR, row, EDT_DBF_WST(pdt->abr, _T("")) );
	m_Pdt_grd->SetCellText( CLM_ABR, row, EDT_DBF_WST(pdt->abr, _T("")) );
	m_Pdt_grd->SetCheck( CLM_SEN, row, !!DBS_INT_VAL(pdt->sen) );
	if ( !Nul_Fld(pdt->vss, FLD_DTM) )	Edt_Dte(buf, memcnt(buf), DBS_DAY_PTR(pdt->vss));
	else								wcscpy_s(buf, memcnt(buf), _T("                ") );
	wcscat_s( buf, memcnt(buf), _T("-") );
	pos = (int)wcslen( buf );
	if ( !Nul_Fld(pdt->vse, FLD_DTM) )	Edt_Dte(buf+pos, memcnt(buf)-pos, DBS_DAY_PTR(pdt->vse));
	m_Pdt_grd->SetCellText( CLM_SPR, row, buf );
	for ( idx=0; idx<cnt; idx++, cdn++ ){
		if ( (cdn+idx)->cod == DBS_INT_VAL(pdt->dkd) ){
			m_Pdt_grd->SetCellText( CLM_KND, row, cdn->nam );
			break;
		}
	}
	if ( ptr = Cma_Edt( DBS_DBL_VAL(pdt->prc), m_pPms_mng->bsc->adp ) ){
		m_Pdt_grd->SetCellText( CLM_PRC, row, ptr );
		free( ptr );
	}
	m_Pdt_grd->SetData( CLM_DAT, row, (int)(LONG_PTR)pdt );
}

void	CProductDlg::SetComboIndex( DWORD cid, int dat )
{
	int			cnt, idx;
	CComboBox	*cbx;

	cnt = ( cbx = (CComboBox *)GetDlgItem(cid) )->GetCount();
	cbx->SetCurSel( -1 );
	for ( idx=0; idx<cnt; idx++ ){
		if ( cbx->GetItemData( idx ) == dat ){
			cbx->SetCurSel( idx );
			break;
		}
	}
}

#define	CEX_ATR			1
#define	CEX_DPS			2

int CProductDlg::CheckExisting( DSC_RPT *pdt, int knd )
{
	DSC_RPT		*tax;
	WCHAR		buf[192], *erm=NULL;
	int			chk=TRUE, exs, sam;
	Lnk			rot[1], *csr=NULL;
	Day			*txs, *txe, *pds, *pde;

	if ( !!DBS_INT_VAL(pdt->sen) ){
		pds = !Nul_Fld(pdt->vss, FLD_DAY) ? DBS_DAY_PTR(pdt->vss): NULL;
		pde = !Nul_Fld(pdt->vse, FLD_DAY) ? DBS_DAY_PTR(pdt->vse): NULL;

		while( csr=Nxt_Lnk( m_Pdt_rot, csr ) ){
			tax = Rec_Of(DSC_RPT, csr);
			sam  =  Nul_Fld(pdt->id, FLD_INT) &&  Nul_Fld(tax->id, FLD_INT);
			sam |= !Nul_Fld(pdt->id, FLD_INT) && !Nul_Fld(tax->id, FLD_INT) &&  DBS_INT_VAL(pdt->id)==DBS_INT_VAL(tax->id);
			if ( knd==CEX_ATR ){
				sam = !sam && DBS_INT_VAL(tax->atr) & DBS_INT_VAL(pdt->atr);
			}
			else{
				sam = !sam && DBS_INT_VAL(tax->dkd)==DBS_INT_VAL(pdt->dkd) && DBS_INT_VAL(tax->prk)==DBS_INT_VAL(pdt->prk);
			}
			if ( sam ){
				if ( !!DBS_INT_VAL(tax->sen) ){
					txs = !Nul_Fld(tax->vss, FLD_DAY) ? DBS_DAY_PTR(tax->vss): NULL;
					txe = !Nul_Fld(tax->vse, FLD_DAY) ? DBS_DAY_PTR(tax->vse): NULL;
					if ( Viw_Ovl_Pod( pds, pde, txs, txe ) )	break;
				}
			}
		}
		chk = !( exs = !!csr );
		
		memset( rot, 0, sizeof(rot) );

		if ( !exs ){
			if ( knd==CEX_ATR )	chk = Dsc_Get_Atr( rot, pdt, m_Pdt_rot, &erm, m_pDbs_obj, m_pPms_mng );
			else				chk = Dsc_Get_Dps( rot, pdt, m_Pdt_rot, &erm, m_pDbs_obj, m_pPms_mng );
			if ( chk ){
				while ( csr=Nxt_Lnk( rot, csr ) ){
					tax = Rec_Of(DSC_RPT, csr);
					if ( knd==CEX_ATR ){
						sam = DBS_INT_VAL(tax->atr) & DBS_INT_VAL(pdt->atr);
					}
					else{
						sam = DBS_INT_VAL(tax->dkd)==DBS_INT_VAL(pdt->dkd) && DBS_INT_VAL(tax->prk)==DBS_INT_VAL(pdt->prk);
					}
					if ( sam ){
						if ( !!DBS_INT_VAL(tax->sen) ){
							txs = !Nul_Fld(tax->vss, FLD_DAY) ? DBS_DAY_PTR(tax->vss): NULL;
							txe = !Nul_Fld(tax->vse, FLD_DAY) ? DBS_DAY_PTR(tax->vse): NULL;
							if ( Viw_Ovl_Pod( pds, pde, txs, txe ) )	break;
						}
					}
				}
				chk = !( exs = !!csr );
			}
			else{
				if ( !!erm ){
					MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
					free( erm );
				}
			}
		}
		if ( exs ){
			swprintf_s( buf, memcnt(buf),
				_T("This %s%s already exist."),
				DBS_INT_VAL(pdt->dkd)==DRK_TAX ? _T("Tax"): ( knd==CEX_ATR ? _T("Addditional"): _T("") ),
				knd==CEX_ATR ? _T(" Attribute"): _T(" ProductKind") );
			MessageBox( buf, NULL, MB_ICONEXCLAMATION|MB_OK );
		}
		Del_All( rot );
	}

	return( chk );
}

void	CProductDlg::ShowAttribute( DSC_RPT *pdt, int dkd )
{
	int			idx, atr=0, shw = ( dkd==DRK_TAX || dkd==DRK_ADDITIONAL );
	WCHAR		buf[40];
	DWORD		cid[] = {	IDC_RADIO_ATTRIBUTE1, IDC_RADIO_ATTRIBUTE2, IDC_RADIO_ATTRIBUTE3,
							IDC_RADIO_ATTRIBUTE4, IDC_RADIO_ATTRIBUTE5 };

	GetDlgItem(IDC_STATIC_ATTRIBUTE)->ShowWindow( shw ? SW_SHOWNORMAL: SW_HIDE );
	atr = ( shw && !!pdt ) ? (DBS_INT_VAL(pdt->atr) >> ( dkd==DRK_TAX ? 0: 10 )): 0;
	for ( idx=0; idx<memcnt(cid); idx++ ){
		if ( shw ){
			swprintf_s( buf, memcnt(buf), _T("%sAttribute%d"), dkd==DRK_TAX ? _T("Tax"): _T("Addditional"), idx+1 );
			GetDlgItem(cid[idx])->SetWindowText( buf );
			((CButton *)GetDlgItem(cid[idx]))->SetCheck( !!( atr & 1 ) );
			atr>>=1;
		}
		GetDlgItem(cid[idx])->ShowWindow( shw ? SW_SHOWNORMAL: SW_HIDE );
	}
}

void	CProductDlg::SetEditData( DSC_RPT *pdt )
{
	int			row, cnt;
	DSC_RPT		*tax;
	WCHAR		buf[96];

	GetDlgItem(IDC_EDIT_CATEGORYNAME)->SetWindowTextW( !!pdt ? EDT_DBF_WST(pdt->nam, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_ABBREVIATION)->SetWindowTextW( !!pdt ? EDT_DBF_WST(pdt->abr, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_CODE)->SetWindowTextW( !!pdt ? EDT_DBF_WST(pdt->dsc, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_READABLECODE)->SetWindowTextW( !!pdt ? EDT_DBF_WST(pdt->rac, _T("")):_T(""));
	GetDlgItem(IDC_EDIT_ARRIVALDATE)->SetWindowText( !!pdt ? EDT_DBF_DAY( buf, memcnt(buf), pdt->vss ): _T("") );
	GetDlgItem(IDC_EDIT_DEPARTUREDATE)->SetWindowText( !!pdt ? EDT_DBF_DAY( buf, memcnt(buf), pdt->vse ): _T("") );

	SetPriceKind( !!pdt ? DBS_INT_VAL(pdt->dkd): 0 );

	SetComboIndex(IDC_COMBO_PRODUCTKIND, !!pdt ? DBS_INT_VAL(pdt->dkd): -1);
	SetComboIndex(IDC_COMBO_PRICEKIND, !!pdt ? DBS_INT_VAL(pdt->prk):-1);

	m_Edt_prc->SetValue( !!pdt ? DBS_DBL_VAL(pdt->prc): 0 , !pdt || Nul_Fld(pdt->prc, FLD_DBL) );
	((CButton *)GetDlgItem(IDC_CHECK_SALEENABLE))->SetCheck( !!pdt && !!DBS_INT_VAL(pdt->sen) );

	ShowAttribute( pdt, !!pdt ? DBS_INT_VAL(pdt->dkd): 0 );


//	((CButton *)GetDlgItem(IDC_CHECK_ATTRIBUTE))->SetCheck( !!pdt && !!DBS_INT_VAL(pdt->atr) );

	cnt = m_Apl_grd->GetLineCount( FALSE );
	for ( row=1; row<=cnt; row++ ){
		tax = (DSC_RPT *)(LONG_PTR)m_Apl_grd->GetData( CLM_TXD, row );
		m_Apl_grd->SetCheck( CLM_APL, row, !!pdt ? !!(DBS_INT_VAL(tax->atr) & DBS_INT_VAL(pdt->apl) ): FALSE );
	}
	
	ButtonEnable();
}

void	CProductDlg::ButtonEnable()	
{
	int			enb, sel;
	enb  = ((CComboBox *)GetDlgItem(IDC_COMBO_BRANCH))->GetCurSel() != -1;
	enb &= ((CComboBox *)GetDlgItem(IDC_COMBO_FACILITY))->GetCurSel() != -1;
	enb &= ((CComboBox *)GetDlgItem(IDC_COMBO_CATEGORY))->GetCurSel() != -1;

	sel = m_Pdt_grd->GetSelRow() > 0;

	GetDlgItem(IDC_EDIT_CATEGORYNAME)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ABBREVIATION)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_CODE)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_READABLECODE)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_ARRIVALDATE)->EnableWindow( enb );
	GetDlgItem(IDC_EDIT_DEPARTUREDATE)->EnableWindow( enb );
	GetDlgItem(IDC_COMBO_PRODUCTKIND)->EnableWindow( enb );
	//GetDlgItem(IDC_COMBO_TRANSFERORIGIN)->EnableWindow( enb );
	GetDlgItem(IDC_COMBO_PRICEKIND)->EnableWindow( enb );
	if ( !enb ){
		((CComboBox *)GetDlgItem(IDC_COMBO_PRODUCTKIND))->SetCurSel(-1);
		//((CComboBox *)GetDlgItem(IDC_COMBO_TRANSFERORIGIN))->SetCurSel(-1);
		((CComboBox *)GetDlgItem(IDC_COMBO_PRICEKIND))->SetCurSel(-1);
	}
	m_Edt_prc->EnableWindow( enb );
	GetDlgItem(IDC_CHECK_SALEENABLE)->EnableWindow( enb );
//	GetDlgItem(IDC_CHECK_ATTRIBUTE)->EnableWindow( enb );;

	GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow( sel && enb );
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow( enb);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow( sel && enb );
}

int	CProductDlg::GetData( DSC_RPT *pdt )
{
	DWORD		cid[] = {	IDC_RADIO_ATTRIBUTE1, IDC_RADIO_ATTRIBUTE2, IDC_RADIO_ATTRIBUTE3,
							IDC_RADIO_ATTRIBUTE4, IDC_RADIO_ATTRIBUTE5 };
	WCHAR		buf[96], *itm=NULL, *wrg=NULL;
	int			val, idx, ape, pke, dke, ser, eer, chk, nul, cnt, row, apl;
	double		fvl;
	FCL_INF		*fcl;
	DSC_RPT		*tax;
	CComboBox	*cbx;

	WIN_TXT_DBS(pdt->nam, IDC_EDIT_CATEGORYNAME, this);
	WIN_TXT_DBS(pdt->abr, IDC_EDIT_ABBREVIATION, this);
	WIN_TXT_DBS(pdt->dsc, IDC_EDIT_CODE, this);
	WIN_TXT_DBS(pdt->rac, IDC_EDIT_READABLECODE, this);
	WIN_TXT_DBS(pdt->rac, IDC_EDIT_READABLECODE, this);
	WIN_DAY_DBS(pdt->vss, &ser, IDC_EDIT_ARRIVALDATE, this);
	WIN_DAY_DBS(pdt->vse, &eer, IDC_EDIT_DEPARTUREDATE, this);
	nul = m_Edt_prc->GetValue( &fvl );
	Set_Nul(pdt->prc, FLD_DBL, nul);
	*DBS_DBL_PTR(pdt->prc) = fvl;
	cbx = (CComboBox *)GetDlgItem(IDC_COMBO_PRODUCTKIND);
	if ( !( dke = ( idx = cbx->GetCurSel() ) == -1 ) ){
		DBS_INT_SET(pdt->dkd, &(val = (int)(LONG_PTR)cbx->GetItemData( idx ) ) );
		DBS_INT_SET(pdt->tfi, (int *)0 );
		if ( val == DRK_TRANSFER ){
			cbx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);
			if ( ( idx = cbx->GetCurSel() ) > 0 ){
				fcl = (FCL_INF *)(LONG_PTR)cbx->GetItemData(idx);
				DBS_INT_SET(pdt->tfi, fcl->id);
			}
		}
	}
	cbx = (CComboBox *)GetDlgItem(IDC_COMBO_PRICEKIND);
	if ( !( pke = ( idx = cbx->GetCurSel() ) == -1 ) ){
		DBS_INT_SET(pdt->prk, &(val = (int)(LONG_PTR)cbx->GetItemData( idx ) ) );
	}
	cbx = (CComboBox *)GetDlgItem(IDC_COMBO_CATEGORY);
	if ( ( idx = cbx->GetCurSel() ) != -1 ){
		DBS_INT_SET(pdt->dci, DBS_INT_PTR( ((DSC_CAT *)(LONG_PTR)cbx->GetItemData( idx ))->id) );
	}

	WIN_CBX_DBS(pdt->sen, IDC_CHECK_SALEENABLE, this );
	
	cnt = m_Apl_grd->GetLineCount( FALSE );
	for ( apl=0, row=1; row<=cnt; row++ ){
		tax = (DSC_RPT *)(LONG_PTR)m_Apl_grd->GetData( CLM_TXD, row );
		if ( m_Apl_grd->GetCheck( CLM_APL, row ) ){
			apl |=  DBS_INT_VAL(tax->atr);
		}
	}

	DBS_INT_SET(pdt->apl, &apl);

	ape = 0;
	if ( !dke ){
		apl = 0;
		if ( DBS_INT_VAL(pdt->dkd)==DRK_TAX || DBS_INT_VAL(pdt->dkd)==DRK_ADDITIONAL ){
			for ( apl=0, idx=0; idx<memcnt(cid); idx++ ){
				if ( ((CButton *)GetDlgItem(cid[idx]))->GetCheck() ){
					apl = 1 << idx;
					break;
				}
			}
			apl <<= DBS_INT_VAL(pdt->dkd)==DRK_TAX ? 0: 10;
			ape =  !apl;
		}
		DBS_INT_SET(pdt->atr, &apl );
	}

	if ( !( chk = !Nul_Fld(pdt->nam, FLD_WST) ) ){
		itm = _T("ProductName" );
	}
	else if ( !( chk = !ser ) ){
		wrg = _T("ValidSalePeriod(Start date)");
	}
	else if ( !( chk = !eer ) ){
		wrg = _T("ValidSalePeriod(End date)");
	}
	else if ( !( chk = !dke ) ){
		itm = _T("ProductKind");
	}
	else if ( !( chk = !pke ) ){
		itm = _T("PriceKind");
	}
	else if ( !( chk = !ape ) ){
		itm = _T("Attribute");
	}
	else if ( !!DBS_INT_VAL(pdt->atr) ){
		chk = CheckExisting( pdt, CEX_ATR );
	}
	else if ( DBS_INT_VAL(pdt->dkd)==DRK_DEPOSIT || DBS_INT_VAL(pdt->dkd)==DRK_DISCOUNT ){
		chk = CheckExisting( pdt, CEX_DPS );
	}

	if ( !chk ){
		if ( itm || wrg ){
			swprintf_s( buf, memcnt(buf), _T("%s is %s."), itm ? itm: wrg, itm ? _T("requierd item"): _T("wrong") );
			MessageBox( buf, NULL, MB_ICONEXCLAMATION|MB_OK );
		}
	}
	else{
		ButtonEnable();
	}

	return( chk );
//	name, pricekind, apl, atr, sen, org
}


DWORD CProductDlg::ConfirmSaveChaned( int msg, int fce )
{
	Csr			csr=NULL;
	DSC_RPT		*pdt;
	int			ans=IDYES;
	WCHAR		*erm=NULL;

	while( csr=Nxt_Lnk( m_Pdt_rot, csr ) ){
		pdt = Rec_Of(DSC_RPT, csr);
		if ( !Nul_Fld(pdt->mdt, FLD_DTM) || !Nul_Fld(pdt->ddt, FLD_DTM) || Nul_Fld(pdt->id, FLD_INT) ){
			break;
		}
	}
	if ( !!csr ){
		ans = Viw_Cfm_Svc( m_Pdt_rot, Dsc_Sav_Dsc, msg, fce, this, m_pDbs_obj, m_pPms_mng );
	}
	return( ans );
}

// Getting and setting tax descriptions
int CProductDlg::SetTax( DSC_RPT *key )	
{
	WCHAR		*erm=NULL;
	Csr			csr=NULL;
	DSC_RPT		*tax;
	int			chk, row=1;
	WCHAR		buf[80];

	if ( chk = Dsc_Get_Tax( m_Tax_rot, key, FALSE, &erm, m_pDbs_obj, m_pPms_mng ) ){
		m_Apl_grd->ResetContent();
		m_Apl_grd->AddRow( Cnt_Lnk( m_Tax_rot ) );
		while ( csr=Nxt_Lnk(m_Tax_rot, csr) ){
			tax = Rec_Of(DSC_RPT, csr);
			m_Apl_grd->SetCellText(CLM_DSC, row, DBS_WST_PTR(tax->nam) );
			Viw_Prc_Edt( buf, memcnt(buf), DBS_DBL_VAL(tax->prc), DBS_INT_VAL(tax->prk),  m_pPms_mng );
			m_Apl_grd->SetCellText(CLM_TPC, row, buf );
			m_Apl_grd->SetData( CLM_TXD, row, (int)(LONG_PTR)tax );
			m_Apl_grd->SetEnable( -1, row++, TRUE );
		}
	}
	else{
		if ( erm ){
			MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
	}
	return( chk );
}

void CProductDlg::SetPriceKind( int dkd )
{
	int			idx, cnt;
	DSC_CDN		*cdn;
	CComboBox	*pcx = (CComboBox *)GetDlgItem(IDC_COMBO_PRICEKIND);

	pcx->ResetContent();

	if ( ( cnt = Viw_Ext_Dsc( &cdn, DSC_PRK, dkd ) ) != -1 ){
		for ( idx=0; idx<cnt; idx++, cdn ){
			pcx->SetItemData( pcx->InsertString( -1, (cdn+idx)->nam ), (cdn+idx)->cod );
		}
		free( cdn );
	}
}


BEGIN_MESSAGE_MAP(CProductDlg, CDialog)
	ON_WM_GRIDNOTIFY()
	ON_CBN_SELCHANGE(IDC_COMBO_BRANCH, &CProductDlg::OnCbnSelchangeComboBranch)
	ON_CBN_SELCHANGE(IDC_COMBO_FACILITY, &CProductDlg::OnCbnSelchangeComboFacility)
	ON_CBN_SELCHANGE(IDC_COMBO_CATEGORY, &CProductDlg::OnCbnSelchangeComboCategory)
	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCTKIND, &CProductDlg::OnCbnSelchangeComboProductKind)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CProductDlg::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CProductDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CProductDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDCANCEL, &CProductDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CProductDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CProductDlg メッセージ ハンドラ
BOOL CProductDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pPms_mng = ((CMainFrame *)GetParent())->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)GetParent())->m_Dbs_obj;

	SetupComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

// グリッドイベント
LRESULT CProductDlg::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	LRESULT			rsl=TRUE;
	int				row, chk;
	DSC_RPT			*pdt;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_LBUTTONCLICK:	row = ntf->row;
								if ( row > 0 ){
									if ( ntf->cid == IDC_GRD_PDT ){
										pdt = (DSC_RPT *)(LONG_PTR)m_Pdt_grd->GetData( CLM_DAT, row );
										if ( pdt ){
											SetEditData( pdt );
										}
									}
									else{
										chk = m_Apl_grd->GetCheck( CLM_APL, ntf->row );
										m_Apl_grd->SetCheck( CLM_APL, ntf->row, !chk );
									}
								}
								break;
		case GN_QUERYINPUTTYPE:	if ( ntf->cid==IDC_GRD_TAP && ntf->clm==CLM_APL ){
									rsl = GCT_CHK;
								}
								else{
									rsl = GCT_NML;
								}
								break;
		}
		free( ntf );
	}

	return( rsl );
}

// Branch combobox change
void CProductDlg::OnCbnSelchangeComboBranch()
{
	int			idx;
	Csr			csr=NULL;
	WCHAR		*erm=NULL;
	BRC_INF		*brc=NULL;
	FCL_INF		*fcl;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);
	CComboBox	*ccx = (CComboBox *)GetDlgItem(IDC_COMBO_CATEGORY);

	if ( ( ( idx = bcx->GetCurSel() ) != -1 ) && idx!=m_Brc_idx ){
		if ( ConfirmSaveChaned( TRUE, FALSE )!=IDCANCEL ){
			Del_All( m_Fcl_rot );
			Del_All( m_Cat_rot );
			Del_All( m_Pdt_rot );
			Del_All( m_Tax_rot );
			fcx->ResetContent();
			ccx->ResetContent();
			m_Pdt_grd->ResetContent();
			m_Apl_grd->ResetContent();
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
			m_Cat_idx = -1;
			OnCbnSelchangeComboFacility();
			SetEditData( NULL );
		}
		else{
			bcx->SetCurSel( idx = m_Brc_idx );
		}
	}
	m_Brc_idx = idx;
}

// Facility combobox change
void CProductDlg::OnCbnSelchangeComboFacility()
{
	int			idx, bix;
	Csr			csr=NULL;
	WCHAR		*erm=NULL;
	BRC_INF		*brc=NULL;
	FCL_INF		*fcl=NULL;
	DSC_CAT		key[1], *cat;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);
	CComboBox	*ccx = (CComboBox *)GetDlgItem(IDC_COMBO_CATEGORY);

	memset( key, 0, sizeof(key) );

	if ( ( ( idx = fcx->GetCurSel() ) != -1 ) && ( ( bix = bcx->GetCurSel() ) != -1 ) && idx!=m_Fcl_idx ){
		if ( ConfirmSaveChaned( TRUE, FALSE )!=IDCANCEL ){
			Del_All( m_Cat_rot );
			Del_All( m_Pdt_rot );
			Del_All( m_Tax_rot );
			ccx->ResetContent();
			m_Pdt_grd->ResetContent();
			m_Apl_grd->ResetContent();
			wcscpy_s( DBS_WST_PTR(key->opc), FLD_WST_SIZ(key->brc), m_pPms_mng->opc );
			if ( bix>0 ){
				brc = (BRC_INF *)(LONG_PTR)bcx->GetItemData( bix );
				DBS_WST_CPY(key->brc, brc->brc);
			}
			if ( idx>0 ){
				fcl = (FCL_INF *)(LONG_PTR)fcx->GetItemData( idx );
				DBS_WST_CPY(key->fcc, fcl->fcc);
			}

			if ( Dsc_Get_Cat( m_Cat_rot, key, TRUE, &erm, m_pDbs_obj, m_pPms_mng ) ){
				while( csr = Nxt_Lnk( m_Cat_rot, csr ) ){
					cat = Rec_Of(DSC_CAT, csr);
					ccx->SetItemData( ccx->InsertString(-1, EDT_DBF_WST(cat->nam, _T("")) ), (DWORD)(LONG_PTR)cat );
				}
			}
			SetEditData( NULL );

			//ccx->SetCurSel(0);
			m_Cat_idx = 0;
			OnCbnSelchangeComboCategory();
		}
		else{
			fcx->SetCurSel( idx = m_Fcl_idx );
		}
	}
	m_Fcl_idx = idx;
}

// Category combobox change
void CProductDlg::OnCbnSelchangeComboCategory()
{
	int			idx, fix, bix, row=0;
	Csr			csr=NULL;
	BRC_INF		*brc=NULL;
	FCL_INF		*fcl=NULL;
	DSC_CAT		*cat;
	DSC_RPT		*pdt, key[1];
	WCHAR		*erm=NULL;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);
	CComboBox	*ccx = (CComboBox *)GetDlgItem(IDC_COMBO_CATEGORY);

	memset( key, 0, sizeof(key) );
	idx = ccx->GetCurSel();
	bix = bcx->GetCurSel();
	fix = fcx->GetCurSel();

	if ( idx!=-1 && bix!=-1 && fix!=-1 && idx!=m_Cat_idx ){
		if ( ConfirmSaveChaned( TRUE, FALSE )!=IDCANCEL ){
			Del_All( m_Pdt_rot );
			m_Pdt_grd->ResetContent();
			wcscpy_s( DBS_WST_PTR(key->opc), FLD_WST_SIZ(key->opc), m_pPms_mng->opc );

			if ( bix>0 ){
				brc = (BRC_INF *)(LONG_PTR)bcx->GetItemData( bix );
				DBS_WST_CPY(key->brc, brc->brc);
			}
			if ( fix>0 ){
				fcl = (FCL_INF *)(LONG_PTR)fcx->GetItemData( fix );
				DBS_WST_CPY(key->fcc, fcl->fcc);
			}
			cat = (DSC_CAT *)(LONG_PTR)ccx->GetItemData( idx );
			DBS_INT_CPY(key->dci, cat->id);
			if ( Dsc_Get_Dsc( m_Pdt_rot, key, &erm, m_pDbs_obj, m_pPms_mng ) ){
				m_Pdt_grd->AddRow( Cnt_Lnk( m_Pdt_rot ) );
				while( csr = Nxt_Lnk( m_Pdt_rot, csr ) ){
					pdt = Rec_Of(DSC_RPT, csr);
					Set_Nul(pdt->mdt, FLD_DTM, TRUE);
					DispData( pdt, ++row );
				}
			}
			SetEditData( NULL );
			if ( !SetTax( key ) )	OnCancel();
		}
		else{
			ccx->SetCurSel( idx = m_Cat_idx );
		}
	}
	ButtonEnable();
	m_Cat_idx = idx;
}

// ProductKind combobox change
void CProductDlg::OnCbnSelchangeComboProductKind()
{
	int			idx, row, dkd;
	DSC_RPT		*pdt;
	CComboBox	*cbx = (CComboBox *)GetDlgItem(IDC_COMBO_PRODUCTKIND);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);

	if ( ( idx = cbx->GetCurSel() ) != -1 && m_Pdk_idx != idx ){
		if ( ( dkd = (int)(LONG_PTR)cbx->GetItemData(idx) ) == DRK_TRANSFER ){
			if ( fcx->GetCurSel() <= 0 ){
				MessageBox( _T("Facility should select  at first, if setting the transfer."), NULL, MB_ICONEXCLAMATION|MB_OK );
				idx = m_Pdk_idx;
				cbx->SetCurSel(idx);
			}
		}
		pdt = ( ( row = m_Pdt_grd->GetSelRow() ) > 0 ) ? (DSC_RPT *)(LONG_PTR)m_Pdt_grd->GetData( CLM_DAT, row ): NULL;
		ShowAttribute( pdt, dkd );
		SetPriceKind( dkd );
	}
	m_Pdk_idx = idx;
}

// Updae button clich
void CProductDlg::OnBnClickedButtonUpdate()
{
	int			row;
	DSC_RPT		tmp[1], *pdt;
	Day			dte[1];

	if ( ( row = m_Pdt_grd->GetSelRow() ) > 0 ){
		pdt = (DSC_RPT *)(LONG_PTR)m_Pdt_grd->GetData( CLM_DAT, row );
		*tmp = *pdt;
		if ( GetData( tmp ) ){
			*pdt = *tmp;
			Get_Dte( dte );
			DBS_DTM_SET(pdt->mdt, dte);
			DispData( pdt, row );
			m_Pdt_grd->SetSelectCell(-1, -1);
			SetEditData( NULL );
		}
	}
}

// Add button click
void CProductDlg::OnBnClickedButtonAdd()
{
	int			row, idx, bix;
	Csr			csr;
	BRC_INF		*brc=NULL;
	FCL_INF		*fcl=NULL;
	DSC_RPT		tmp[1], *pdt;
	CComboBox	*bcx = (CComboBox *)GetDlgItem(IDC_COMBO_BRANCH);
	CComboBox	*fcx = (CComboBox *)GetDlgItem(IDC_COMBO_FACILITY);

	memset( tmp, 0, sizeof(tmp) );

	if ( ( idx = fcx->GetCurSel() ) != -1 && ( bix = bcx->GetCurSel() ) != -1 ){
		DBS_WST_SET(tmp->opc, m_pPms_mng->opc);
		if ( bix>0 ){
			brc = (BRC_INF *)(LONG_PTR)fcx->GetItemData( bix );
			DBS_WST_CPY(tmp->brc, brc->brc);
		}
		if ( idx>0 ){
			fcl = (FCL_INF *)(LONG_PTR)fcx->GetItemData( idx );
			DBS_WST_CPY(tmp->fcc, fcl->fcc);
		}
		if ( GetData( tmp ) ){
			if ( csr=Add_Lnk( m_Pdt_rot, tmp, sizeof(DSC_RPT) ) ){
				pdt = Rec_Of(DSC_RPT, csr);
				row = m_Pdt_grd->GetLineCount( FALSE );
				m_Pdt_grd->AddRow( 1 );
				DispData( pdt, row+1 );
				m_Pdt_grd->SetSelectCell(-1, -1);
				SetEditData( NULL );
			}
		}
	}
}

// Delete button click
void CProductDlg::OnBnClickedButtonDelete()
{
	int			row;
	DSC_RPT		*pdt;
	Day			dte[1];

	if ( ( row = m_Pdt_grd->GetSelRow() ) > 0 ){
		pdt = (DSC_RPT *)(LONG_PTR)m_Pdt_grd->GetData( CLM_DAT, row );
		if ( MessageBox( _T("Are you sure to delete?"), NULL, MB_ICONQUESTION|MB_YESNO ) == IDYES ){
			if ( Nul_Fld(pdt->id, FLD_INT) ){
				Del_Lnk( m_Pdt_rot, Csr_Of(pdt) );
			}
			else{
				Get_Dte( dte );
				DBS_DAY_SET(pdt->ddt, dte);
			}
			m_Pdt_grd->DelRow( row );
			m_Pdt_grd->SetSelectCell(-1, -1);
			m_Pdt_grd->Redraw();
			SetEditData( NULL );
		}
	}
}



// Close button click
void CProductDlg::OnBnClickedCancel()
{
	if ( ConfirmSaveChaned( TRUE, TRUE )==IDYES ){
		OnCancel();
	}
}

// Close winth save button click
void CProductDlg::OnBnClickedOk()
{
	if ( ConfirmSaveChaned( FALSE, FALSE )==IDYES ){
		OnOK();
	}
}

BOOL CProductDlg::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CDialog::PreTranslateMessage(pMsg): FALSE );
}
