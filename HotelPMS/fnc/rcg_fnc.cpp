/*
	Room charge dialog functions
*/

//#include "windows.h"
#include "..\resource.h"
#include "..\..\Common\bsc_def.h"
#include "..\..\Common\dbs_mng.h"
#include "..\..\Common\dte_mng.h"
#include "..\..\Custom\SimpleGrid.h"
#include "..\..\PMSCustom\BookingPathCombo.h"
#include "..\RoomChargeDlg.h"
#include "..\CopySelectDlg.h"
#include "..\CategorySelectDlg.h"
#include "rcg_fnc.h"
#include "ecg_sql.h"
#include "mfm_sql.h"
#include "viw_cmn.h"

static	WCHAR	*Grd_ttl[] =	{	{ _T("Plan Name")	},
									{ _T("Type")	}, 
									{ _T("Kind")	}, 
									{ _T("Person")		}, 
									{ _T("Sale start")	},
									{ _T("Sale end")	},
									{ _T("Sale Enable")	} };

static	GRD_CLM	Grd_clm[] =		{	{ 386, GAL_LFT, GCT_NML, 256 },		// New
									{  84, GAL_CTR, GCT_NML,  12 },		// ChargeKind
									{  84, GAL_CTR, GCT_NML,  12 },		// ChargeType
									{  54, GAL_RGT, GCT_NML,  12 },		// person
									{  90, GAL_CTR, GCT_NML,  12 },		// Sale start
									{  90, GAL_CTR, GCT_NML,  12 },		// Sale end
									{ 126, GAL_CTR, GCT_NML,  12 } };	// Sale enable

static	WCHAR	*Apl_ttl[] =	{	{ _T("Apl")			},
									{ _T("Description")	},
									{ _T("Price")		} };

static	GRD_CLM	Apl_clm[] =		{	{  24, GAL_CTR, GCT_CHK,  12 },		// Apl
									{ 154, GAL_LFT, GCT_NML,  12 },		// Description
									{  94, GAL_RGT, GCT_NML,  12 } };	// Price

static	struct	{
	int		cid;		// control id
	int		dkd;		// day kind
	int		enn;		// enabling null
}Amt_[] = {
	{ IDC_EDIT_AMOUNT1,DKD_SUNDAY,FALSE }, { IDC_EDIT_AMOUNT2,DKD_MONDAY,FALSE }, { IDC_EDIT_AMOUNT3,DKD_TUESDAY,FALSE },
	{ IDC_EDIT_AMOUNT4,DKD_WEDNESDAY,FALSE }, { IDC_EDIT_AMOUNT5,DKD_THURSDAY,FALSE }, { IDC_EDIT_AMOUNT6,DKD_FRIDAY,FALSE },
	{ IDC_EDIT_AMOUNT7,DKD_SATURDAY,FALSE }, { IDC_EDIT_AMOUNT8,DKD_HOLIDAY,TRUE }, { IDC_EDIT_AMOUNT9,DKD_PRE_HOLIDAY,TRUE }
};

#define	TAB_IDX_(x)					((CTabCtrl *)((x)->GetDlgItem(IDC_TAB_BOOKINGPATH)))->GetCurSel()
#define	PLN_CNT_(x)					((CRoomChargeDlg *)(x))->m_Bkp_cnt
#define	PLN_LST_(x)					((CRoomChargeDlg *)(x))->m_Pln_lst
#define	PLN_DTL_(x)					((CRoomChargeDlg *)(x))->m_Pln_dtl
#define	SEL_IDX_(x)					((CRoomChargeDlg *)(x))->m_Tab_idx
#define	SEL_ROW_(x)					((CRoomChargeDlg *)(x))->m_Grd_row
#define	OPE_COD_(x)					((CRoomChargeDlg *)(x))->m_Ope_cod
#define	BRC_COD_(x)					((CRoomChargeDlg *)(x))->m_Brc_cod
#define	FCL_COD_(x)					((CRoomChargeDlg *)(x))->m_Fcl_cod
#define	APL_ROT_(x)					((CRoomChargeDlg *)(x))->m_Apl_rot
#define	FCL_IID_(x)					((CRoomChargeDlg *)(x))->m_Fcl_iid
#define	DSC_CAT_(x)					((CRoomChargeDlg *)(x))->m_Dsc_cat
#define	ADD_NEW_(x)					((CRoomChargeDlg *)(x))->m_Add_new


static	void	enb_cpn(			// Enable/Disable Components
int			bkp,					// Enable of Bokkingpath
int			dtl,					// Enable of detail items
int			upd,					// Enable of update button
int			add,					// Enable of add button
int			del,					// Enable of delete button
CWnd		*wnd )					// parent window
{
	DWORD		bid[] = {	IDC_TAB_BOOKINGPATH, IDC_GRD_DTL, IDC_BUTTON_COPYTO, IDC_BUTTON_NEW };
	DWORD		did[] = {	IDC_EDIT_PLANNAME, IDC_COMBO_CHARGEKIND,IDC_COMBO_CHARGETYPE, IDC_EDIT_SALESTARTDATE,IDC_EDIT_SALEENDDATE,
							IDC_COMBO_CHARGEKIND, IDC_COMBO_CHARGETYPE, IDC_BUTTON_COPYTODETAIL,
							IDC_EDIT_AMOUNT1, IDC_EDIT_AMOUNT2, IDC_EDIT_AMOUNT3, IDC_EDIT_AMOUNT4, IDC_EDIT_AMOUNT5, IDC_EDIT_AMOUNT6,
							IDC_EDIT_AMOUNT7, IDC_EDIT_AMOUNT8, IDC_EDIT_AMOUNT9, IDC_BUTTON_COPYALL, IDC_EDIT_PERSON };
	int			idx;
	DWORD		cid;

	for ( idx=0; idx<memcnt(bid); idx++ ){
		(wnd->GetDlgItem(bid[idx]))->EnableWindow( bkp );
		//if ( !bkp && bid[idx] != IDC_BUTTON_XXXXXXXX )	(wnd->GetDlgItem(bid[idx]))->SetWindowText( _T("") );
	}
	if ( !bkp ){
		SEL_IDX_(wnd) = -1;
		SEL_ROW_(wnd) = -1;
	}
	for ( idx=0; idx<memcnt(did); idx++ ){
		(wnd->GetDlgItem(cid=did[idx]))->EnableWindow( dtl && ( did[idx]!=IDC_BUTTON_COPYTODETAIL || !ADD_NEW_(wnd) ));
		if ( !dtl && ( cid != IDC_BUTTON_COPYALL ) && ( cid != IDC_BUTTON_COPYTODETAIL ) ){
			(wnd->GetDlgItem(did[idx]))->SetWindowText( _T("") );
		}
	}
	if ( !dtl ){
		((CComboBox *)(wnd->GetDlgItem(IDC_COMBO_CHARGEKIND)))->SetCurSel(-1);
		((CComboBox *)(wnd->GetDlgItem(IDC_COMBO_CHARGETYPE)))->SetCurSel(-1);
	}
	(wnd->GetDlgItem(IDC_BUTTON_UPDATE))->EnableWindow( upd );
	(wnd->GetDlgItem(IDC_BUTTON_ADD))->EnableWindow( add );
	(wnd->GetDlgItem(IDC_BUTTON_DELETE))->EnableWindow( del );
}

CSimpleGrid		*Rcg_Mak_Grd(		// Create each grid
int			id,						// id of grid control
RECT		*rct,					// Rect of grid
CWnd		*wnd )					// 表示先ウィンドウ
{
	CSimpleGrid		*grd;
	GRD_DEF			gdf[1];
	int				idx;

	memset( gdf, 0, sizeof(gdf) );

	gdf->lnh = 20;
	gdf->vlc = id==IDC_GRD_DTL ? 10: 4;
	gdf->vcc = id==IDC_GRD_DTL ? memcnt(Grd_clm): memcnt(Apl_clm);
	gdf->hbc = RGB(128,255,255);
	gdf->hfc = RGB(0,0,0);
	gdf->dbc = RGB(255,255,255);
	gdf->dfc = RGB(0,0,0);
	gdf->sbc = RGB(0,0,0);
	gdf->sfc = RGB(255,255,255);
	gdf->cct = gdf->vcc;
	gdf->clm = id==IDC_GRD_DTL ? Grd_clm: Apl_clm;
	gdf->sel = GSL_ROW;
	gdf->hdr = TRUE;
	gdf->fsa = TRUE;

	grd = new CSimpleGrid();
	grd->Create( gdf, *rct, id, wnd );
	for ( idx=0; idx<(int)(id==IDC_GRD_DTL ? memcnt(Grd_ttl): memcnt(Apl_ttl)); idx++ ){
		grd->SetCellText( idx, 0, id==IDC_GRD_DTL ? Grd_ttl[idx]: Apl_ttl[idx] );
	}

	return( grd );
}

void	Rcg_Dps_Pln(					// Dispose the plan list
PLN_		*pln,						// delete target
int			plc )						// number of plan
{
	int			idx;

	if ( pln ){
		for ( idx=0; idx<plc; idx++ ){
			Del_All( (pln+idx)->rot );
		}
		free( pln );
	}
}

int		Rcg_Ini_Dat(					// Initialize each data
CWnd			*par,					// Parent window
CDatabase		*dbs,					// ODBC Database object
PMS_MNG			*mng )					// Hotel Property Managenemt System Structure
{
	CComboBox			*cbx;
	CTabCtrl			*tab;
	Csr					csr=NULL;
	WCHAR				*erm=NULL, *opc, *brc, *fcc, ctn[256];
	int					chk, idx, cti;
	CRoomChargeDlg		*wnd = (CRoomChargeDlg *)par;
	CBookingPathCombo	bpc[1];
	Lnk					brt[1];

	
	opc = OPE_COD_(wnd);
	brc = BRC_COD_(wnd);
	fcc = FCL_COD_(wnd);

	// Branch select combobox
	if ( chk = Get_Mst( wnd->m_Brc_inf, GTM_BRC_INF, NULL, NULL, &erm, dbs, mng ) ){
		cbx = (CComboBox *)(wnd->GetDlgItem(IDC_COMBO_BRANCH));
		cbx->ResetContent();
		while( csr=Nxt_Lnk( wnd->m_Brc_inf, csr ) ){
			cbx->SetItemData( idx = cbx->InsertString( -1, DBS_WST_PTR(Rec_Of(BRC_INF,csr)->nam) ), (DWORD)(LONG_PTR)Rec_Of(BRC_INF,csr) );
			if ( !wcscmp( brc, DBS_WST_PTR(Rec_Of(BRC_INF,csr)->brc) ) )	cbx->SetCurSel( idx );
		}
	}

	// Facility select combobox
	if ( chk && ( chk = Get_Mst( wnd->m_Fcl_inf, GTM_FCL_INF, brc, NULL, &erm, dbs, mng ) ) ){
		cbx = (CComboBox *)(wnd->GetDlgItem(IDC_COMBO_FACILITY));
		cbx->ResetContent();
		while( csr=Nxt_Lnk( wnd->m_Fcl_inf, csr ) ){
			if ( DBS_INT_VAL(Rec_Of(FCL_INF,csr)->fck) == FCK_HOTEL ){
				cbx->SetItemData( idx = cbx->InsertString( -1, DBS_WST_PTR(Rec_Of(FCL_INF,csr)->nam) ), (DWORD)(LONG_PTR)Rec_Of(FCL_INF,csr) );
				if ( !wcscmp( fcc, DBS_WST_PTR(Rec_Of(FCL_INF,csr)->fcc) ) )	cbx->SetCurSel( idx );
			}
		}
	}

	// Equipmrnt type select combobox
	if ( chk && ( chk = Get_Mst( wnd->m_Eqp_typ, GTM_EQP_TYP, brc, fcc, &erm, dbs, mng ) ) ){
		cbx = (CComboBox *)(wnd->GetDlgItem(IDC_COMBO_ROOMTYPE));
		cbx->ResetContent();
		while( csr=Nxt_Lnk( wnd->m_Eqp_typ, csr ) ){
			if ( DBS_INT_VAL(Rec_Of(EQP_TYP,csr)->ekd) == EQK_ROOM ){
				cbx->SetItemData( idx = cbx->InsertString(-1,DBS_WST_PTR(Rec_Of(EQP_TYP,csr)->nam)), (DWORD)(LONG_PTR)Rec_Of(EQP_TYP,csr) );
			}
		}
	}

	// Initialize booking root tab
	tab = (CTabCtrl *)(wnd->GetDlgItem(IDC_TAB_BOOKINGPATH));
	tab->DeleteAllItems();
	if ( chk && ( chk = Get_Mst( wnd->m_Agt_inf, GTM_AGT_INF, brc, fcc, &erm, dbs, mng ) ) ){
		Rcg_Dps_Pln( wnd->m_Pln_lst, wnd->m_Bkp_cnt );
		wnd->m_Bkp_cnt = 0;
		memset( brt ,0, sizeof(brt) );
		if ( chk = bpc->GetFixedRoot( brt, TRUE ) ){
			if ( chk = !!( wnd->m_Pln_lst = (PLN_ *)calloc( Cnt_Lnk(wnd->m_Agt_inf)+(Cnt_Lnk(brt)), sizeof(PLN_) ) ) ){
				idx = 0;
				while( csr = Nxt_Lnk( brt, csr ) ){
					(wnd->m_Pln_lst+idx)->bkp = *Rec_Of(DWORD, csr);
					tab->InsertItem(idx++, (WCHAR*)(LONG_PTR)(*(Rec_Of(DWORD, csr)+1)) );
					wnd->m_Bkp_cnt++;
				}
				while( csr=Nxt_Lnk( wnd->m_Agt_inf, csr ) ){
					(wnd->m_Pln_lst+idx)->bkp = VIA_AGENT;
					(wnd->m_Pln_lst+idx)->aii = DBS_INT_VAL(Rec_Of(AGT_INF,csr)->id);
					tab->InsertItem(idx++,  DBS_WST_PTR(Rec_Of(AGT_INF,csr)->nam));
					wnd->m_Bkp_cnt++;
				}
				tab->SetCurSel( /*SEL_IDX_(wnd) =*/ 0 );
			}
			Del_All( brt );
		}
	}

	while( csr = Nxt_Lnk( mng->fcl, csr ) ){
		if ( !wcscmp( DBS_WST_PTR(Rec_Of(FCL_INF, csr)->fcc), FCL_COD_(wnd) ) ){
			FCL_IID_(wnd) = DBS_INT_VAL(Rec_Of(FCL_INF, csr)->id);
			break;
		}
	}

	if ( chk ){
		chk = ( cti = Ecg_Get_Cat( ctn, memcnt(ctn), FCL_IID_(wnd), DRK_SERVICE, OPE_COD_(wnd), BRC_COD_(wnd), FCL_COD_(wnd), &erm, dbs, mng ) ) != -1;
		if ( chk ){
			if ( cti != 0 ){
				DSC_CAT_(wnd) = cti;
				wnd->GetDlgItem(IDC_STATIC_CATEGORY)->SetWindowText( ctn );
			}
			else{
				CCategorySelectDlg	*dlg;
				if ( dlg = new CCategorySelectDlg() ){
					dlg->m_pPms_mng = mng;
					dlg->m_pDbs_obj = dbs;
					dlg->m_Ope_cod = OPE_COD_(wnd);
					dlg->m_Brc_cod = BRC_COD_(wnd);
					dlg->m_Fcl_cod = FCL_COD_(wnd);
					dlg->DoModal();
					if ( !!( chk = dlg->m_Result ) ){
						DSC_CAT_(wnd) = dlg->m_Sel_dci;
						wnd->GetDlgItem(IDC_STATIC_CATEGORY)->SetWindowText( dlg->m_Sel_nam );
					}
					delete dlg;
				}
			}
		}
	}

	((CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL)))->ResetContent();
	((CChargeTypeCombo *)(wnd->GetDlgItem(IDC_COMBO_CHARGETYPE)))->SetEquipmentKind( EQK_ROOM );
	((CChargeKindCombo *)(wnd->GetDlgItem(IDC_COMBO_CHARGEKIND)))->SetData();
	enb_cpn( FALSE, FALSE, FALSE, FALSE, FALSE, wnd );

	return( chk );
}

static	int		get_vsd(				// getting stay period from screen
Day			*std,						// storing start date pointer
Day			*edd,						// storing end date pointer
int			glc,						// greater or lesser check
int			msd,						// displaying message is on 
CWnd		*wnd )						// Parent window
{
	WCHAR		buf[192], *itm, *atn=_T("SaleStartDate"), *dtn=_T("SaleEndDate"); 
	WCHAR		*pfx=NULL, *pfx1=_T("SaleEndDate should be same or greater than SaleStartDate"), *pfx2=_T("%s is wrong");
	int			chk, dys, snl, enl;

	wnd->GetDlgItem(IDC_EDIT_SALESTARTDATE)->GetWindowText( buf, memcnt(buf) );
	if ( !( chk = (snl = !wcslen(buf) ) || Str_Dte( std, buf ) ) ){
		itm = atn, pfx = pfx2;
	}
	std->yer = snl ? -1: std->yer;
	if ( chk ){
		wnd->GetDlgItem(IDC_EDIT_SALEENDDATE)->GetWindowText( buf, memcnt(buf) );
		if ( !( chk = ( enl = !wcslen(buf) ) || Str_Dte( edd, buf ) ) ){
			itm = dtn, pfx = pfx2;
		}
		edd->yer = enl ? -1: edd->yer;
	}
	if ( chk && !snl && !enl ){
		if ( chk = !glc || ( Cmp_Dte( std, edd ) <= 0 ) ){
			dys = Clc_Dys( std, edd );
		}
		else{
			pfx = pfx1;
		}
	}
	if ( pfx && msd ){
		swprintf_s( buf, memcnt(buf), pfx, itm );
		wnd->MessageBox( buf, NULL, MB_ICONEXCLAMATION|MB_OK );
	}
	return( chk ? dys: -1 );
}

static	void	dsp_apl(				// displaying apply
Csr				rot,					// Root of apply data
CWnd			*wnd,					// Parent window
PMS_MNG			*mng )					// Hotel Property Managenemt System Structure
{
	int			row=1;
	WCHAR		buf[40];
	Csr			csr=NULL;
	ECG_APL_DAT	*apl;
	CSimpleGrid	*grd = (CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_APL));

	grd->ResetContent();
	while( csr=Nxt_Lnk( rot, csr ) ){
		apl = Rec_Of(ECG_APL_DAT, csr);
		grd->AddRow(1);
		grd->SetCellText(1,row,DBS_WST_PTR(apl->nam) );
		Viw_Prc_Edt( buf, memcnt(buf), DBS_DBL_VAL(apl->prc), DBS_INT_VAL(apl->pkd), mng );
		grd->SetCellText(2,row, buf );
		grd->SetData( 1, row, (int)(LONG_PTR)apl );
		grd->SetEnable( -1, row, TRUE );
		row++;
	}
}

static	int		get_apl(				// getting application
CWnd		*wnd,						// Parent window
CDatabase	*dbs,						// ODBC Database object
PMS_MNG		*mng )						// Hotel Property Managenemt System Structure
{
	int			chk;
	ECG_SQL_PRM	prm[1];
	WCHAR		*erm=NULL;
	Day			std[1], edd[1];

	memset( prm, 0, sizeof(prm) );

	prm->opc = OPE_COD_(wnd);
	prm->brc = BRC_COD_(wnd);
	prm->fcc = FCL_COD_(wnd);


	Del_All( APL_ROT_(wnd) );
	if ( chk = get_vsd( std, edd, TRUE, FALSE, wnd ) != -1 ){
		if ( std->yer != -1 )	prm->std = std;
		if ( edd->yer != -1 )	prm->edd = edd;
		chk = Ecg_Apl_Dat( APL_ROT_(wnd), prm, &erm, dbs, mng );
	}
	dsp_apl( APL_ROT_(wnd), wnd, mng );

	if ( chk ){
	}
	else{
		if ( erm ){
			wnd->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
	}

	return( chk );
}

void			Rcg_Sld_Chg(			// Sale date change event
CWnd		*wnd,						// Parent window
CDatabase	*dbs,						// ODBC Database object
PMS_MNG		*mng )						// Hotel Property Managenemt System Structure
{
	get_apl( wnd, dbs, mng );
}

static	PLN_	*get_tab(				// getting tab index by bookingpath and agent_info_id
int			bkp,						// Bookingpath
int			aii,						// agent_info_id
CWnd		*wnd )						// Parent window
{
	PLN_		*pln=((CRoomChargeDlg *)wnd)->m_Pln_lst;
	//CTabCtrl	*tab=(CTabCtrl *)(wnd->GetDlgItem(IDC_TAB_BOOKINGPATH));
	int			idx, cnt=((CRoomChargeDlg *)wnd)->m_Bkp_cnt;//tab->GetItemCount();

	if ( pln ){
		for( idx=0; idx<cnt; idx++ ){
			if ( (pln+idx)->bkp==bkp && (pln+idx)->aii==aii )	break;
		}
	}

	return( idx<cnt ? pln+idx: NULL);
}

void		Rcg_Dsp_Pln(				// Displaying plan list on grid on selected tab
CWnd		*wnd )						// Parent window
{
	Csr					csr=NULL;
	CTabCtrl			*tab=(CTabCtrl *)(wnd->GetDlgItem(IDC_TAB_BOOKINGPATH));
	int					row=0, idx = tab->GetCurSel();
	PLN_				*pln=((CRoomChargeDlg *)wnd)->m_Pln_lst + idx;
	CSimpleGrid			*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL));
	CChargeKindCombo	*ckc = (CChargeKindCombo *)(wnd->GetDlgItem(IDC_COMBO_CHARGEKIND));
	CChargeTypeCombo	*ctc = (CChargeTypeCombo *)(wnd->GetDlgItem(IDC_COMBO_CHARGETYPE));
	ECG_PLN_DAT			*dat;
	WCHAR				buf[40];

	if ( SEL_IDX_(wnd) != tab->GetCurSel() ){
		SEL_IDX_(wnd) = tab->GetCurSel();
		grd->ResetContent();
		grd->AddRow( Cnt_Lnk( pln->rot ) );
		while( csr=Nxt_Lnk( pln->rot, csr ) ){
			dat = Rec_Of(ECG_PLN_DAT, csr);
			grd->SetCellText( 0, ++row, DBS_WST_PTR(dat->pln) );								// Plan name
			grd->SetCellText( 1, row, ctc->GetItemName(DBS_INT_VAL(dat->cgp)) );				// Type(Particular)
			grd->SetCellText( 2, row, ckc->GetItemName(DBS_INT_VAL(dat->cgk)) );				// Kind
			grd->SetCellText( 3, row, EDT_DBF_INT(buf, memcnt(buf), dat->psn, Edt_Num_Nml ) );	// Person
			grd->SetCellText( 4, row, EDT_DBF_DAY(buf, memcnt(buf), dat->vss ) );				// valid_sale_period_start
			grd->SetCellText( 5, row, EDT_DBF_DAY(buf, memcnt(buf), dat->vse ) );				// valid_sale_period_end
			grd->SetCellText( 6, row, !!DBS_INT_VAL(dat->sen) ? _T("○"):  _T("×") );			// valid_sale_period_end
		}
		enb_cpn( TRUE, FALSE, FALSE, FALSE, FALSE, wnd );
		SEL_ROW_(wnd) = -1;
	}
}

int		Rcg_Get_Lst(					// Getting the plan list on rooptype
CWnd		*wnd,						// Parent window
CDatabase	*dbs,						// ODBC Database object
PMS_MNG		*mng )						// Hotel Property Managenemt System Structure
{
	Lnk			rot[1], *csr=NULL;
	int			chk, idx, aii=0, atp, bkp=0;
	WCHAR		*erm=NULL;
	ECG_SQL_PRM	prm[1];
	EQP_TYP		*eqp;
	ECG_PLN_DAT	*dat;
	PLN_		*pln;
	CComboBox	*cbx = (CComboBox *)(wnd->GetDlgItem(IDC_COMBO_ROOMTYPE));

	memset( rot, 0, sizeof(rot) );

	if ( !(chk = (idx = cbx->GetCurSel())==-1 ) ){
		memset( prm, 0, sizeof(prm) );
		if ( chk = !!( eqp = (EQP_TYP *)(LONG_PTR)cbx->GetItemData(idx) ) ){
			prm->eti = DBS_INT_VAL(eqp->id);
			prm->opc = OPE_COD_(wnd);
			prm->brc = BRC_COD_(wnd);
			prm->fcc = FCL_COD_(wnd);
			for ( idx=0; idx<((CRoomChargeDlg *)(wnd))->m_Bkp_cnt; idx++ ){
				Del_All( (((CRoomChargeDlg *)(wnd))->m_Pln_lst+idx)->rot );
			}
			if ( chk = Ecg_Erp_Lst( rot, prm, &erm, dbs, mng ) ){
				while ( csr=Nxt_Lnk( rot, csr ) ){
					dat = Rec_Of(ECG_PLN_DAT, csr);
					atp = Nul_Fld(dat->aii,FLD_INT) ? 0: DBS_INT_VAL(dat->aii);
					if ( bkp!=DBS_INT_VAL(dat->bfr) || aii!=atp  ){
						bkp = DBS_INT_VAL(dat->bfr);
						aii = atp;
						if ( pln = get_tab( bkp, aii, wnd ) ){
							Del_All( pln->rot );
						}
					}
					if ( pln ){
						if ( !(chk = !!Add_Lnk( pln->rot, dat, sizeof(ECG_PLN_DAT) )) ){
							break;
						}
					}
				}
			}
			else{
				if ( erm )	wnd->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
				free( erm );
			}
			if ( chk ){
				SEL_IDX_(wnd) = -1;
				Rcg_Dsp_Pln( wnd );
				enb_cpn( TRUE, FALSE, FALSE, FALSE, FALSE, wnd );
			}
		}
	}
	else{
		enb_cpn( FALSE, FALSE, FALSE, FALSE, FALSE, wnd );
	}

	Del_All( rot );
	SEL_ROW_(wnd) = -1;

	return( chk );
}


//#define	WKD_PRC_(p, a, w, b)	(!Nul_Fld((p),FLD_DBL) && (DBS_INT_VAL(a) & (w) ) ? Edt_Dbl_Nml(b,memcnt(b),_T("%.f"),DBS_DBL_VAL(p)): _T(""))
#define	WKD_PRC_(p, a, w, b)	((DBS_INT_VAL(a) & (w) ) ? : _T(""))

static	void	dsp_dtl(				// displaying detail data
CWnd		*wnd,						// Parent window
CDatabase	*dbs,						// ODBC Database object
PMS_MNG		*mng )						// Hotel Property Managenemt System Structure
{
	ECG_PLN_DTL	*dtl = PLN_DTL_(wnd);
	ECG_APL_DAT	*apl;
	EQP_CGD		*cgd;
	Csr			csr=NULL;
	WCHAR		buf[40];
	int			idx;
	CSimpleGrid	*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_APL));

	wnd->GetDlgItem(IDC_EDIT_PLANNAME)->SetWindowTextW( EDT_DBF_WST( dtl->dsc->nam, _T("") ) );
	((CChargeKindCombo *)(wnd->GetDlgItem(IDC_COMBO_CHARGEKIND)))->SetItemIndex( DBS_INT_VAL(dtl->crg->cgk) );
	((CChargeKindCombo *)(wnd->GetDlgItem(IDC_COMBO_CHARGETYPE)))->SetItemIndex( DBS_INT_VAL(dtl->crg->cgp) );
	wnd->GetDlgItem(IDC_EDIT_SALESTARTDATE)->SetWindowTextW( EDT_DBF_DAY(buf, memcnt(buf), dtl->dsc->vss) );
	wnd->GetDlgItem(IDC_EDIT_SALEENDDATE)->SetWindowTextW( EDT_DBF_DAY(buf, memcnt(buf), dtl->dsc->vse) );
	for ( idx=0; idx<memcnt(Amt_); idx++ ){
		// At first, clear all amounts
		((CCommaEdit *)(wnd->GetDlgItem(Amt_[idx].cid)))->SetValue( 0, TRUE );
	}
	while ( csr=Nxt_Lnk( dtl->dtl, csr ) ){
		cgd = Rec_Of(EQP_CGD, csr);
		for ( idx=0; idx<memcnt(Amt_); idx++ ){
			if ( !!((DBS_INT_VAL(cgd->adk) & Amt_[idx].dkd)) ){ 
				((CCommaEdit *)(wnd->GetDlgItem(Amt_[idx].cid)))->SetValue( DBS_DBL_VAL(cgd->prc), FALSE );
			}
		}
	}
	get_apl( wnd, dbs, mng );
	for ( idx=0; idx<grd->GetLineCount(FALSE); idx++ ){
		apl = (ECG_APL_DAT *)(LONG_PTR)grd->GetData( 1, idx+1 );
		grd->SetCheck( 0, idx+1, !!(DBS_INT_VAL(dtl->dsc->apl) & DBS_INT_VAL(apl->atr)) );
	}
	((CButton *)(wnd->GetDlgItem(IDC_CHECK_SALEENABLE)))->SetCheck( DBS_INT_VAL(dtl->dsc->sen)!=0 );
	wnd->GetDlgItem(IDC_EDIT_PERSON)->SetWindowTextW( EDT_DBF_INT(buf, memcnt(buf), dtl->crg->psn, Edt_Num_Nml) );
}

void	Rcg_Sel_Pln(					// On select 1 plan on the grid
int			row,						// row of gris
CWnd		*wnd,						// Parent window
CDatabase	*dbs,						// ODBC Database object
PMS_MNG		*mng )						// Hotel Property Managenemt System Structure
{
	int			idx=TAB_IDX_(wnd);
	Csr			csr;
	ECG_PLN_DAT	*dat;
	ECG_SQL_PRM	prm[1];
	WCHAR		*erm=NULL;

	memset( prm, 0, sizeof(prm) );

	if ( SEL_ROW_(wnd) != row ){
		SEL_ROW_(wnd) = row;
		if ( idx != -1 && idx <PLN_CNT_(wnd) ){
			if ( csr = Idx_Lnk( (PLN_LST_(wnd)+idx)->rot, row-1 ) ){
				dat = Rec_Of(ECG_PLN_DAT, csr);
				prm->eci = DBS_INT_VAL(dat->eci);
				Del_All( PLN_DTL_(wnd)->dtl );
				if ( Ecg_Erp_Dtl( PLN_DTL_(wnd), prm, &erm, dbs, mng ) ){
					enb_cpn( TRUE, TRUE, TRUE, TRUE, TRUE, wnd );
					dsp_dtl( wnd, dbs, mng );
				}
			}
		}
	}
}

static	int		amt_chg(				// chekking amount(equipment_charge_details) changed 
ECG_PLN_DTL	*dst,						// deatination of storing area
ECG_PLN_DTL	*org )						// original data of rate plan data for chekking data changed
{
	Csr			csd=NULL, cso;
	EQP_CGD		*dcg, *ocg;
	int			chg=FALSE;

	if ( !( chg = Cnt_Lnk( dst->dtl ) != Cnt_Lnk( org->dtl ) ) ){
		while ( csd = Nxt_Lnk( dst->dtl, csd ) ){
			dcg = Rec_Of(EQP_CGD, csd);
			for ( cso=NULL; cso = Nxt_Lnk( org->dtl, cso ); ){
				ocg = Rec_Of(EQP_CGD, cso);
				if ( Nul_Fld(dcg->adk,FLD_INT) == Nul_Fld(ocg->adk,FLD_INT) ){
					if ( !Nul_Fld(dcg->adk,FLD_INT) ){
						if ( DBS_INT_VAL(dcg->adk) == DBS_INT_VAL(ocg->adk) ){
							if ( Nul_Fld(dcg->prc,FLD_DBL) == Nul_Fld(ocg->prc,FLD_DBL) ){
								if ( !Nul_Fld(dcg->prc,FLD_DBL) ){
									if ( DBS_DBL_VAL(dcg->prc) == DBS_DBL_VAL(ocg->prc) ){
										break;
									}
								}
							}
						}
					}
				}
			}
			if ( !!( chg = !cso ) )	break;

		}
	}

	return( chg );
}

static	int		get_dtl(				// get detail data from screen
ECG_PLN_DTL	*dst,						// deatination of storing area
int			*chg,						// changed status 
ECG_PLN_DTL	*org,						// original data of rate plan data for chekking data changed
CWnd		*wnd )						// Parent window
{
	EQP_CGD		*cgd;
	ECG_APL_DAT	*apl;
	EQP_TYP		*eqt;
	Day			dte[2];
	Csr			csr;
	double		val, amt[9];
	int			chk, nul, idx, idy, cnt, adk[9], tap, ctp;
	WCHAR		*itm=NULL, erm[128];
	CSimpleGrid	*grd=((CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_APL)));

	memset( amt, 0, sizeof(amt) );
	memset( adk, 0, sizeof(adk) );

	if ( !!chg )	*chg = FALSE;

	if ( chk = !Get_Str_Dat(dst->dsc->nam, FLD_WST_SIZ(dst->dsc->nam), wnd, IDC_EDIT_PLANNAME) ){
		if ( !!chg )	*chg |= !!wcscmp( DBS_WST_PTR(dst->dsc->nam), DBS_WST_PTR(org->dsc->nam) );
	}
	else{
		itm = _T("PlanNAme ");
	}
	if ( chk && !!( chk = get_vsd( dte, dte+1, TRUE, TRUE, wnd ) != -1 ) ){
		Get_Day_Dat(dst->dsc->vss,0,wnd,IDC_EDIT_SALESTARTDATE);
		Get_Day_Dat(dst->dsc->vse,0,wnd,IDC_EDIT_SALEENDDATE);
		if ( !!chg ){
			*chg |= !!Cmp_Dte( DBS_DAY_PTR(dst->dsc->vss), DBS_DAY_PTR(org->dsc->vss) );
			*chg |= !!Cmp_Dte( DBS_DAY_PTR(dst->dsc->vse), DBS_DAY_PTR(org->dsc->vse) );
		}
	}
	if ( chk ){
		if ( chk = ( idx = ((CChargeTypeCombo *)(wnd->GetDlgItem(IDC_COMBO_CHARGETYPE)))->GetItemIndex() ) != -1 ){
			Set_Nul(dst->crg->cgp, FLD_INT, FALSE);
			*DBS_INT_PTR(dst->crg->cgp) = idx;
			if ( !!chg )	*chg |= DBS_INT_VAL(dst->crg->cgp) != DBS_INT_VAL(org->crg->cgp);
		}
		else{
			itm = _T("ChargeType ");
		}
	}
	if ( chk ){
		if ( chk = ( idx = ((CChargeTypeCombo *)(wnd->GetDlgItem(IDC_COMBO_CHARGEKIND)))->GetItemIndex() ) != -1 ){
			Set_Nul(dst->crg->cgk, FLD_INT, FALSE);
			*DBS_INT_PTR(dst->crg->cgk) = idx;
			if ( !!chg )	*chg |= DBS_INT_VAL(dst->crg->cgk) != DBS_INT_VAL(org->crg->cgk);
		}
		else{
			itm = _T("ChargeKind ");
		}
	}
	if ( chk ){
		Get_Int_Dat(dst->crg->psn,0,wnd,IDC_EDIT_PERSON);
		if ( !!chg ){
			ctp = Nul_Fld(dst->crg->psn,FLD_INT) != Nul_Fld(org->crg->psn,FLD_INT);
			if ( !ctp && !Nul_Fld(dst->crg->psn,FLD_INT) ){
				ctp = DBS_INT_VAL(dst->crg->psn) != DBS_INT_VAL(org->crg->psn);
			}
			*chg |= ctp;
		}
	}
	if ( chk ){
		for ( cnt=idx=0; idx<memcnt(Amt_); idx++ ){
			nul = ((CCommaEdit *)(wnd->GetDlgItem(Amt_[idx].cid)))->GetValue( &val );
			if ( chk = !( !!nul && !Amt_[idx].enn ) ){
				if ( !nul ){
					for ( idy=0; idy<memcnt(amt); idy++ ){
						if ( amt[idy] > 0.0 ){
							if ( val == amt[idy] ){
								adk[idy] |= Amt_[idx].dkd;
								break;
							}
						}
						else{
							amt[idy] = val;
							adk[idy] = Amt_[idx].dkd;
							++cnt;
							break;
						}
					}
				}
			}
			else{
				break;
			}
		}
		if ( chk ){
			for ( idx=0; idx<cnt; idx++ ){
				if ( chk = !!( csr = Add_Lnk( dst->dtl, NULL, sizeof(EQP_CGD) ) ) ){
					cgd = Rec_Of(EQP_CGD, csr);
					Set_Nul(cgd->adk, FLD_INT, FALSE);
					*DBS_INT_PTR(cgd->adk) = adk[idx];
					Set_Nul(cgd->prc, FLD_DBL, FALSE);
					*DBS_DBL_PTR(cgd->prc) = amt[idx];
				}
			}
			if ( !chk )	wnd->MessageBox( _T("Memory allocation error"), NULL, MB_ICONEXCLAMATION|MB_OK );
		}
		else{
			itm = _T("Sun ~ Sat");
		}
		if ( chk && !!chg ){
			*chg |= amt_chg( dst, org );
		}
	}
	if ( chk ){
		for ( idx=tap=0; idx<grd->GetLineCount(FALSE); idx++ ){
			if ( grd->GetCheck( 0, idx+1 ) ){
				apl = (ECG_APL_DAT *)(LONG_PTR)grd->GetData( 1, idx+1 );
				tap |= DBS_INT_VAL(apl->atr);
			}
		}
		Set_Nul(dst->dsc->apl, FLD_INT, FALSE);
		*DBS_INT_PTR(dst->dsc->apl) = tap;
		if ( !!chg ){
			ctp = Nul_Fld(dst->dsc->apl,FLD_INT) != Nul_Fld(org->dsc->apl,FLD_INT);
			if ( !ctp && !Nul_Fld(dst->dsc->apl,FLD_INT) ){
				ctp = DBS_INT_VAL(dst->dsc->apl) != DBS_INT_VAL(org->dsc->apl);
			}
			*chg |= ctp;
		}
	}
	if ( chk ){
		WIN_CBX_DBS( dst->dsc->sen, IDC_CHECK_SALEENABLE, wnd );
		if ( !!chg )	*chg |= DBS_INT_VAL(dst->dsc->sen) != DBS_INT_VAL(org->dsc->sen);
	}
	if ( chk ){
		idx = ((CComboBox *)(wnd->GetDlgItem(IDC_COMBO_ROOMTYPE)))->GetCurSel();
		if ( chk = idx != -1 ){
			Set_Nul(dst->crg->eti, FLD_INT, FALSE);			// equipment_type_is
			eqt = (EQP_TYP *)(LONG_PTR)((CComboBox *)(wnd->GetDlgItem(IDC_COMBO_ROOMTYPE)))->GetItemData( idx );
			*DBS_INT_PTR(dst->crg->eti) = DBS_INT_VAL(eqt->id);
		}
	}
	if ( itm ){
		swprintf_s( erm, memcnt(erm), _T("%s id required item"), itm );
		wnd->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
	}
	return( chk );
}

void	Rcg_Del_Dtl(
CWnd		*wnd,						// Parent window
CDatabase	*dbs,						// ODBC Database object
PMS_MNG		*mng )						// Hotel Property Managenemt System Structure
{
	Day				dte[1];
	Csr				csr=NULL;
	WCHAR			*erm=NULL;
	int				chk;

	Get_Dte( dte );

	if ( SEL_IDX_(wnd) != -1 && SEL_ROW_(wnd) != -1 ){
		if ( !Nul_Fld(PLN_DTL_(wnd)->crg->id, FLD_INT) ){
			if ( chk = Ecg_Erp_Del( PLN_DTL_(wnd), dte, &erm, dbs, mng ) ){
				if ( csr = Idx_Lnk( (PLN_LST_(wnd)+SEL_IDX_(wnd))->rot, SEL_ROW_(wnd)-1 ) ){
					((CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL)))->DelRow( SEL_ROW_(wnd) );
					((CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL)))->Redraw();
					Del_Lnk( (PLN_LST_(wnd)+SEL_IDX_(wnd))->rot, csr );
				}
				enb_cpn( TRUE, FALSE, FALSE, FALSE, FALSE, wnd );
				((CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL)))->SetSelectCell( -1, -1 );
			}
			else{
				if ( erm ){
					wnd->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
					free( erm );
				}
			}
		}
		else{
			if ( csr = Idx_Lnk( (PLN_LST_(wnd)+SEL_IDX_(wnd))->rot, SEL_ROW_(wnd)-1 ) ){
				((CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL)))->DelRow( SEL_ROW_(wnd) );
				((CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL)))->Redraw();
				Del_Lnk( (PLN_LST_(wnd)+SEL_IDX_(wnd))->rot, csr );
			}
			enb_cpn( TRUE, FALSE, FALSE, FALSE, FALSE, wnd );
			((CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL)))->SetSelectCell( -1, -1 );
		}
		SEL_ROW_(wnd) = -1;
	}
}

static	void	set_wst(				// new setting of WCHAR
WCHAR		*dst,						// destination pointer
int			siz,						// destination size
WCHAR		*src )						// source pointer
{
	Set_Nul(dst, FLD_WST, FALSE);
	wcscpy_s( DBS_WST_PTR(dst), siz, src );
}

static	void	set_int(				// new setting of integer
int			*dst,						// destination pointer
int			src )						// source data
{
	Set_Nul(dst, FLD_WST, FALSE);
	*DBS_INT_PTR(dst) = src;
}

static	void	set_dte(				// Create/Modified information
int			*mei,						// modified_employee destination
Day			*cdt,						// created_date destination
int			lie,						// login enployee id
Day			*dte )						// Current date time
{
	Set_Nul(mei, FLD_INT, FALSE);
	*DBS_INT_PTR(mei) = lie;
	Set_Nul(cdt, FLD_DTM, FALSE);
	*DBS_DTM_PTR(cdt) = *dte;
}

static	void	set_def(				// setting default datas on adding
ECG_PLN_DTL	*dst,						// destination of update data
CWnd		*wnd )						// Parent window
{
	// keys of equipment_charge_infos
	set_wst( dst->crg->opc, FLD_WST_SIZ(dst->crg->opc), OPE_COD_(wnd) ); 
	set_wst( dst->crg->brc, FLD_WST_SIZ(dst->crg->brc), BRC_COD_(wnd) ); 
	set_wst( dst->crg->fcc, FLD_WST_SIZ(dst->crg->fcc), FCL_COD_(wnd) );

	// keys of descriptions
	set_wst( dst->dsc->opc, FLD_WST_SIZ(dst->dsc->opc), OPE_COD_(wnd) ); 
	set_wst( dst->dsc->brc, FLD_WST_SIZ(dst->dsc->brc), BRC_COD_(wnd) ); 
	set_wst( dst->dsc->fcc, FLD_WST_SIZ(dst->dsc->fcc), FCL_COD_(wnd) );
	
	// descripttions defaults
	set_int(dst->dsc->dkd, DRK_SERVICE );									// description_kind
	set_int(dst->crg->bfr, (PLN_LST_(wnd)+SEL_IDX_(wnd))->bkp);				// reservation_route
	set_int(dst->crg->aii, (PLN_LST_(wnd)+SEL_IDX_(wnd))->aii);				// agent_id
	Set_Nul(dst->crg->aii,FLD_INT,DBS_INT_VAL(dst->crg->bfr)!=VIA_AGENT);
	set_wst(dst->dsc->dsc, FLD_WST_SIZ(dst->dsc->dsc), FCL_COD_(wnd) );		// description_code
	set_int(dst->dsc->dci, DSC_CAT_(wnd) );									//description_category_id
	set_int(dst->dsc->prk, PRK_AMOUNT);										// price_kind
	set_int(dst->dsc->atr, 0);												// tax_attribution
	set_int(dst->dsc->tfi, FCL_IID_(wnd));									// transfer_origin_facility_id
}

static	void	upd_grd(				// displaying data update on detail grid 
ECG_PLN_DTL	*dtl,						// source of update data
int			ins,						// insert data
CWnd		*wnd )						// Parent window
{
	PLN_			*pln;
	ECG_PLN_DAT		*dat;
	Csr				csr;

	*DBS_INT_PTR(PLN_DTL_(wnd)->crg->id) = 0;
	pln = PLN_LST_(wnd) + SEL_IDX_(wnd);
	if ( ins ){
		csr = Add_Lnk( pln->rot, NULL, sizeof(ECG_PLN_DAT) );
	}
	else{
		csr = Idx_Lnk( pln->rot, SEL_ROW_(wnd)-1 );
	}
	if ( csr  ){
		dat = Rec_Of(ECG_PLN_DAT, csr);
		DBS_INT_CPY(dat->eci, dtl->crg->id);
		DBS_INT_CPY(dat->eii, dtl->crg->eii);
		DBS_INT_CPY(dat->eti, dtl->crg->eti);
		DBS_INT_CPY(dat->aii, dtl->crg->aii);
		DBS_INT_CPY(dat->dsi, dtl->crg->dsi);
		DBS_INT_CPY(dat->cgk, dtl->crg->cgk);
		DBS_INT_CPY(dat->cgp, dtl->crg->cgp);
		DBS_INT_CPY(dat->psn, dtl->crg->psn);
		DBS_INT_CPY(dat->sen, dtl->dsc->sen);
		DBS_DAY_CPY(dat->vss, dtl->dsc->vss);
		DBS_DAY_CPY(dat->vse, dtl->dsc->vse);
		DBS_WST_CPY(dat->pln, dtl->dsc->nam);
	}
}

static	void	set_new(				// setting a ew mode
int			mod,						// setting mode( TRUE=to new, FALSE=to normal
CWnd		*wnd,						// Parent window
CDatabase	*dbs,						// ODBC Database object
PMS_MNG		*mng )						// Hotel Property Managenemt System Structure
{
	ADD_NEW_(wnd) = mod;
	if ( mod ){
		wnd->GetDlgItem(IDC_BUTTON_NEW)->SetWindowTextW( _T("StopNew") );
		enb_cpn( TRUE, TRUE, FALSE, TRUE, FALSE, wnd );
		get_apl( wnd, dbs, mng );
	}
	else{
		wnd->GetDlgItem(IDC_BUTTON_NEW)->SetWindowTextW( _T("New") );
		enb_cpn( TRUE, FALSE, FALSE, FALSE, FALSE, wnd );
	}
}

void	Rcg_Sav_Dtl(					// saving detail data
int			add,						// add data
CWnd		*wnd,						// Parent window
CDatabase	*dbs,						// ODBC Database object
PMS_MNG		*mng )						// Hotel Property Managenemt System Structure
{
	Day				dte[1];
	int				chg, oct=0;
	Csr				csr=NULL, cse=NULL;
	ECG_PLN_DTL		dtl[1];
	EQP_CGD			*cgd;
	WCHAR			*erm=NULL;
	CSimpleGrid		*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL));

	if ( SEL_IDX_(wnd) != -1 && ( ADD_NEW_(wnd) || SEL_ROW_(wnd) != -1 ) ){
		memset( dtl, 0, sizeof(dtl) );
		if ( !add ){
			*dtl->crg = *PLN_DTL_(wnd)->crg;
			*dtl->dsc = *PLN_DTL_(wnd)->dsc;
		}
		if ( get_dtl( dtl, add ? NULL: &chg, add ? NULL: PLN_DTL_(wnd), wnd ) ){
			if ( add || chg ){
				Get_Dte( dte );
				if ( add )	set_def( dtl, wnd );
				set_dte(dtl->crg->mei, add ? dtl->crg->cdt: dtl->crg->mdt, DBS_INT_VAL(mng->lgi->id), dte );
				set_dte(dtl->dsc->mei, add ? dtl->dsc->cdt: dtl->dsc->mdt, DBS_INT_VAL(mng->lgi->id), dte );
				while( csr=Nxt_Lnk( dtl->dtl, csr ) ){
					cgd = Rec_Of(EQP_CGD, csr);
					set_wst( cgd->opc, FLD_WST_SIZ(cgd->opc), OPE_COD_(wnd) ); 
					set_wst( cgd->brc, FLD_WST_SIZ(cgd->brc), BRC_COD_(wnd) ); 
					set_wst( cgd->fcc, FLD_WST_SIZ(cgd->fcc), FCL_COD_(wnd) );
				}
				if ( Ecg_Erp_Sav( dtl, add, dte, &erm, dbs, mng ) ){
					upd_grd( dtl, add, wnd );
					SEL_IDX_(wnd) = -1;
					Rcg_Dsp_Pln( wnd );
					((CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL)))->Redraw();
					enb_cpn( TRUE, FALSE, FALSE, FALSE, FALSE, wnd );
					SEL_ROW_(wnd) = -1;
				}
				else{
					if ( erm ){
						wnd->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
						free( erm );
					}
				}
				Del_All( dtl->dtl );
			}
			if ( !add && !chg ){
				wnd->MessageBox( _T("Data was not changed"), NULL, MB_ICONINFORMATION|MB_OK );
			}
			set_new( FALSE, wnd, dbs, mng );
		}
	}
}

void	Rcg_Cpy_Pln(					// copy rate plan
int			dtl,						// detail copy
CWnd		*wnd,						// Parent window
CDatabase	*dbs,						// ODBC Database object
PMS_MNG		*mng )						// Hotel Property Managenemt System Structure
{
	Lnk				rot[1], drt[1], *csr;
	WCHAR			*erm=NULL;
	ECG_PLN_DAT		*src;
	CCopySelectDlg	*dlg;

	if ( SEL_IDX_(wnd) != -1 && ( !dtl || SEL_ROW_(wnd) != -1 ) ){
		memset( drt, 0, sizeof(drt) );
		memset( rot, 0, sizeof(rot) );
		if ( dlg = new CCopySelectDlg() ){
			dlg->m_pPms_mng = mng;
			dlg->m_Omt_bkp = (PLN_LST_(wnd)+SEL_IDX_(wnd))->bkp;
			dlg->m_Omt_aii = (PLN_LST_(wnd)+SEL_IDX_(wnd))->aii;
			dlg->m_Sel_lst = rot;
			dlg->DoModal();
			if ( dlg->m_Result ){
				if ( dtl ){
					if ( csr = Idx_Lnk( (PLN_LST_(wnd)+SEL_IDX_(wnd))->rot, SEL_ROW_(wnd)-1 ) ){
						src = Rec_Of(ECG_PLN_DAT, csr);
						csr = Add_Lnk( drt, src, sizeof(ECG_PLN_DAT ) );
					}
				}
				if ( !dtl || !!csr ){
					if ( Ecg_Cpy_Pln( rot, !dtl ? (PLN_LST_(wnd)+SEL_IDX_(wnd))->rot: drt, &erm, dbs, mng ) ){
						Rcg_Get_Lst( wnd, dbs, mng );
					}
					else{
						if ( erm ){
							wnd->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
							free( erm );
						}
					}
				}

				Del_All( rot );
			}
			delete dlg;
		}
		Del_All( drt );
	}
}

void	Rcg_New_Dtl(					// copy rate plan
CWnd		*wnd,						// Parent window
CDatabase	*dbs,						// ODBC Database object
PMS_MNG		*mng )						// Hotel Property Managenemt System Structure
{
	set_new( !ADD_NEW_(wnd), wnd, dbs, mng );
	((CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL)))->SetSelectCell( -1, -1 );
}

