/*
	予約検索処理関数
*/

//#include "windows.h"
#include "..\..\Common\bsc_def.h"
#include "..\..\Common\dbs_mng.h"
#include "..\..\Common\dte_mng.h"
#include "..\..\Common\cmn_fnc.h"
#include "..\..\Custom\SimpleGrid.h"
#include "..\..\Custom\CustomStatic.h"
#include "..\..\PMSCustom\BookingPathCombo.h"
#include "..\..\PMSCustom\ClientRankCombo.h"
#include "..\ChildFrm.h"
#include "..\ReservationView.h"
#include "..\ChargeDetailDlg.h"
#include "..\ClientInfoDlg.h"
#include "..\RoomSelectDlg.h"
#include "rsv_sql.h"
#include "bst_sql.h"
#include "rsv_fnc.h"
#include "ecg_sql.h"
#include "viw_cmn.h"


static	WCHAR	*Grd_ttl[] =	{	{ _T("New")			},
									{ _T("Sel")			},
									{ _T("Name")		},
									{ _T("Status")		},
									{ _T("R.Typ")		},
									{ _T("R.No.")		},
									{ _T("Arrv")		},
									{ _T("E.C/I")		},
									{ _T("Dept")		},
									{ _T("L.C/O")		},
									{ _T("Ad/Ch")		},
									{ _T("Amount")		} };

static	GRD_CLM	Grd_clm[] =		{	{  26, GAL_CTR, GCT_NML,  20 },		// New
									{  26, GAL_CTR, GCT_CHK,  20 },		// Sel
									{ 192, GAL_LFT, GCT_NML,  20 },		// Name
									{  90, GAL_CTR, GCT_NML,  20 },		// Status
									{ 144, GAL_LFT, GCT_NML, 256 },		// R.Typ
									{  84, GAL_LFT, GCT_NML,  12 },		// R No.
									{  90, GAL_CTR, GCT_NML,  12 },		// Arrv.
									{  44, GAL_CTR, GCT_CHK,  12 },		// E.C/I.
									{  90, GAL_CTR, GCT_NML,  12 },		// Dept.
									{  44, GAL_CTR, GCT_CHK,  12 },		// L C/O
									{  48, GAL_CTR, GCT_NML,  12 },		// Ad/Ch
									{ 118, GAL_CTR, GCT_NML,  12 } };	// Amount


static	WCHAR	*Rom_ttl[] =	{	{ _T("UseStart")		},
									{ _T("UseEnd")		},
									{ _T("RoomType")		},
									{ _T("RoomNo.")	},
									{ _T("Amount")		} };

static	GRD_CLM	Rom_clm[] =		{	{  90, GAL_CTR, GCT_DED,  10 },		// UseStart
									{  90, GAL_CTR, GCT_DED,  10 },		// UseEnd
									{ 144, GAL_LFT, GCT_NML,  64 },		// RoomType
									{  84, GAL_LFT, GCT_EDT,  20 },		// RoomNo
									{  88, GAL_RGT, GCT_NML,  12 } };	// Amount



#define	SEL_ROW_(x)				((CReservationView *)(x))->m_Sel_row
#define	RSV_INF_(x)				((CReservationView *)(x))->m_Rsv_inf
#define	RTE_PLN_(x)				((CReservationView *)(x))->m_Rte_pln
#define	OPE_COD_(x)				((CReservationView *)(x))->m_Opr_cod
#define	BRC_COD_(x)				((CReservationView *)(x))->m_Brc_cod
#define	FCL_COD_(x)				((CReservationView *)(x))->m_Fcl_cod
#define	CTM_ROW_(x)				((CReservationView *)(x))->m_Ctm_row
#define	EQP_ISI_(x)				((CReservationView *)(x))->m_Eqp_isi

#define	is_new(x)				(!!Nul_Fld((x)->id,FLD_INT))
#define	BKG_PTH(b,a,r)			(b)=Nul_Fld((r)->bfr,FLD_INT)?0:DBS_INT_VAL((r)->bfr),(a)=Nul_Fld((r)->bfr,FLD_INT)?0:((b)==VIA_AGENT?DBS_INT_VAL((r)->aii):0)

static	void	del_req(			// reservation_equipment delete
Csr			rot,					// target root
Csr			csr )					// target cursor
{
	RSV_EQP_DAT	*req;
	
	req = Rec_Of(RSV_EQP_DAT, csr);
	//Del_All( req->rcg );
	Del_Lnk( rot, csr );
}

static	int		inf_est(			// Making a editing status of reservation_infos
RSV_INF		*inf,					// reservation_infos data
Csr			drt,					// root of reservation_details
PMS_MNG		*mng )					// Hotel Property Management System Structure
{
	RSV_DTL_DAT	*dtl;
	int			est=0;
	Csr			csr=NULL;

	est |= ( Cmp_Dte( DBS_DAY_PTR(inf->dpt), mng->bdt ) < 0 ) ? RSV_EST_OLD: 0;
	est |= is_new(inf) ? RSV_EST_NEW: 0;
	est |= RSV_EST_RSV;
	est |= DBS_INT_VAL(inf->cxl) ? RSV_EST_CXL: 0;		// canceled 
	est |= DBS_INT_VAL(inf->nsw) ? RSV_EST_NSW: 0;		// No Show
	est |= Nul_Fld(inf->bfr,FLD_INT) ? RSV_EST_NBP: 0;			// no booking path select  

	while( csr=Nxt_Lnk( drt, csr ) ){
		dtl = Rec_Of(RSV_DTL_DAT, csr);
		if ( !Nul_Fld(dtl->dtl->cid, FLD_DTM) ){
			est |= Nul_Fld(dtl->dtl->cod, FLD_DTM) ? RSV_EST_INH: RSV_EST_COT;
		}
	}

	return( est );
}

static	int		dtl_est(			// Making a editing status of reservation_details
RSV_INF		*rif,					// reservation_infos data
RSV_DTL		*rdt,					// reservation_details data
int			smr,					// same as reserved person of client_infos
PMS_MNG		*mng )					// Hotel Property Management System Structure
{
	int			est=0;
	Csr			csr=NULL;

	est |= ( Cmp_Dte( DBS_DAY_PTR(rdt->dpt), mng->bdt ) < 0 ) ? RSV_EST_OLD: 0;
	est |= RSV_EST_RSV;
	est |= (!Nul_Fld(rif->nsw,FLD_INT) && !!DBS_INT_VAL(rif->nsw)) ? RSV_EST_NSW: 0;
	est |= Nul_Fld(rif->bfr,FLD_INT) ? RSV_EST_NBP: 0;  
	est |= is_new(rif) ? RSV_EST_NEW: 0;
	if ( !Nul_Fld(rdt->cid, FLD_DTM) ){
		est |= Nul_Fld(rdt->cod, FLD_DTM) ? RSV_EST_INH: RSV_EST_COT;
	}
	est |= (!Nul_Fld(rdt->cxl,FLD_INT) && !!DBS_INT_VAL(rdt->cxl) ) ? RSV_EST_CXL: 0;
//	est |= (!Nul_Fld(rif->cii,FLD_INT) && !Nul_Fld(rdt->cii,FLD_INT) && DBS_INT_VAL(rif->cii)==DBS_INT_VAL(rdt->cii)) ? RSV_EST_SAM:0;
	est |= !!smr ? RSV_EST_SAM:0;

	return( est );
}

CSimpleGrid		*Rsv_Mak_Grd(		// 予約明細グリッド作成
int			id,						// id of grid control
RECT		*rct,					// Rect of grid
CWnd		*wnd )					// 表示先ウィンドウ
{
	CSimpleGrid		*grd;
	GRD_DEF			gdf[1];
	int				idx;

	memset( gdf, 0, sizeof(gdf) );

	gdf->lnh = 20;
	gdf->vlc = id == IDC_GRD_DTL ? 8: 6;
	gdf->vcc = id == IDC_GRD_DTL ? memcnt(Grd_clm): memcnt(Rom_clm);
	gdf->hbc = RGB(128,255,255);
	gdf->hfc = RGB(0,0,0);
	gdf->dbc = RGB(255,255,255);
	gdf->dfc = RGB(0,0,0);
	gdf->sbc = RGB(0,0,0);
	gdf->sfc = RGB(255,255,255);
	gdf->cct = gdf->vcc;
	gdf->clm = id == IDC_GRD_DTL ? Grd_clm: Rom_clm;
	gdf->sel = id==IDC_GRD_ROM ? GSL_NON: GSL_ROW;
	gdf->hdr = TRUE;
	gdf->fsa = TRUE;

	grd = new CSimpleGrid();
	grd->Create( gdf, *rct, id, wnd );
	for ( idx=0; idx<(int)( id == IDC_GRD_DTL ? memcnt(Grd_ttl): memcnt(Rom_ttl)); idx++ ){
		grd->SetCellText( idx, 0, id == IDC_GRD_DTL ? Grd_ttl[idx]: Rom_ttl[idx] );
	}

	return( grd );
}

static	void	rte_pln(				// getting and setting rate plan
RSV_DTL_DAT	*dtl,						// reservation_detail
CWnd		*wnd,						// Parent window
CDatabase	*dbs,						// ODBC Database object
PMS_MNG		*mng )						// Hotel Property management System Structure
{
	int					chk, idx, olp=0, sam, eti, avl;
	WCHAR				*erm=NULL;
	Csr					csr=NULL, rot=RTE_PLN_(wnd);
	Day					arv[1], dpt[1];
	ECG_SQL_PRM			prm[1];
	ECG_PLN_DAT			*pln;
	CBookingPathCombo	*bpc = (CBookingPathCombo *)(wnd->GetDlgItem(IDC_COMBO_BOOKINGPATH));
	CComboBox			*plc = (CComboBox *)(wnd->GetDlgItem(IDC_COMBO_RATEPLANNAME));
	CEquipmentTypeCombo	*cbx = (CEquipmentTypeCombo *)(wnd->GetDlgItem(IDC_COMBO_ROOMTYPE));

	eti = cbx->GetType( &avl );

	Rsv_Dps_Rte( rot );			// Dispose current data
	plc->ResetContent();

	if ( !!eti && !!avl ){
		bpc->GetItemIndex( &prm->bfr, &prm->agi );
		*arv = *DBS_DAY_PTR(dtl->dtl->arv);
		*dpt = *DBS_DAY_PTR(dtl->dtl->dpt);
		prm->std = arv;
		prm->edd = dpt;
		Add_Dte( prm->edd, -1 );	// Because it possible to sale one day before of departure date 
		prm->opc = mng->opc;
		prm->brc = mng->brc;
		prm->fcc = mng->fcc;

		if ( chk = Ecg_Get_Pln( rot, eti, prm, &erm, dbs, mng ) ){
			while( csr = Nxt_Lnk( rot, csr ) ){
				pln = Rec_Of(ECG_PLN_DAT, csr);
				plc->SetItemData( (idx = plc->InsertString(-1, DBS_WST_PTR(pln->pln)) ), (DWORD)(LONG_PTR)pln );
				if ( sam = DBS_INT_VAL(pln->eci)==DBS_INT_VAL(dtl->eci) )	plc->SetCurSel(idx);
			}
			if( !sam && Cnt_Lnk( rot )== 1){
				plc->SetCurSel(0);
			}
		}
		else{
			if ( erm ){
				wnd->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
				free( erm );
			}
		}
		if( eti>0 ){
			 cbx->SetType( eti );
		}
	}
	Rsv_Sel_Pln( dtl, wnd, dbs, mng );
}

static	void	inf_enb(						// reservation_infos input enable
RSV_INF_DAT	*rsv,								// reservation_infos data
CWnd		*wnd )								// Parent window
{
	int			idx;

	struct	_Enb	{
		DWORD		cid;		// component id
		int			dss;		// disable status
	}Enb_[] = {
		//{ IDC_EDIT_FIRSTARRIVAL,	RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW },
		//{ IDC_EDIT_LASTDEPARTURE,	RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW },
		{ IDC_EDIT_EXTBOOKINGID,	RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW },
		{ IDC_EDIT_FIRSTNAME,		RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW },
		{ IDC_EDIT_FAMILYNAME,		RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW },
		{ IDC_EDIT_ADDRESS1,		RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW },
		{ IDC_EDIT_ADDRESS2,		RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW },
		{ IDC_EDIT_TELEPHONE1,		RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW },
		{ IDC_EDIT_TELEPHONE2,		RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW },
		{ IDC_EDIT_FAXNUMBER,		RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW },
		{ IDC_EDIT_EMAIL1,			RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW },
		{ IDC_EDIT_EMAIL2,			RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW },
		{ IDC_RADIO_PERSONAL,		RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_INH|RSV_EST_COT },
		{ IDC_RADIO_GROUP,			RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_INH|RSV_EST_COT },
		{ IDC_COMBO_BOOKINGPATH,	RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_INH|RSV_EST_COT },
		{ IDC_COMBO_CLIENTRANK,		RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW },
		{ IDC_SPIN_ROOMNUMBER,		RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW },
		{ IDC_EDIT_ROOMNUMBER,		RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW } 
	};
	for ( idx=0; idx<memcnt(Enb_); idx++ ){
		wnd->GetDlgItem((Enb_+idx)->cid)->EnableWindow( ( rsv->est & (Enb_+idx)->dss ) == 0 );
	}
}

#define	NO_RTP_CHG		(RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_INH|RSV_EST_COT|RSV_EST_NBP)	// No room type change
#define	NO_RNO_CHG		(RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW)										// No room number change
#define	NO_NED_SAV		(RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_COT)							// No need to save
#define	NO_RSV_CXL		(RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_INH|RSV_EST_COT)				// No cencel

static	void	dtl_enb(						// reservation_details input enable
RSV_DTL_DAT	*dtl,								// reservation_details data
CWnd		*wnd )								// Parent window
{
	int			idx;

	struct	_Enb	{
		DWORD		cid;		// component id
		int			dss;		// disable status
	}Enb_[] = {
		{ IDC_EDIT_FIRSTNAME_DETAIL,	RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_COT|RSV_EST_SAM },
		{ IDC_EDIT_FAMILYNAME_DETAIL,	RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_COT|RSV_EST_SAM },
		{ IDC_EDIT_ARRIVALDATE,			RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_INH|RSV_EST_COT },
		{ IDC_EDIT_ARRIVALTIME,			RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_INH|RSV_EST_COT },
		{ IDC_CHECK_EARLYCI,			RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_INH|RSV_EST_COT },
		{ IDC_EDIT_DEPARTUREDATE,		RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_COT },
		{ IDC_EDIT_DEPARTURETIME,		RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_COT },
		{ IDC_CHECK_LATECO,				RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_COT },
		{ IDC_EDIT_NIGHTS,				RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_COT },
		{ IDC_SPIN_NIGHTS,				RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_COT },
		{ IDC_EDIT_ADULTNUMBER,			RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_COT },
		{ IDC_SPIN_ADULTNUMBER,			RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_COT },
		{ IDC_EDIT_CHILDNUMBER,			RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_COT },
		{ IDC_SPIN_CHILDNUMBER,			RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_COT },
		//{ IDC_BUTTON_ROOMTYPE,			RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_INH|RSV_EST_COT },
		{ IDC_EDIT_ROOMCHANGE,			RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_COT },
		{ IDC_SPIN_ROOMCHANGE,			RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_COT },
		{ IDC_GRD_ROM,					RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_COT },
		{ IDC_EDIT_NOTE,				RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_COT },
		{ IDC_COMBO_ROOMTYPE,			NO_RTP_CHG },
		{ IDC_COMBO_RATEPLANNAME,		NO_RTP_CHG },
		{ IDC_CHECK_SAMERESERVATION,	RSV_EST_OLD|RSV_EST_CXL|RSV_EST_NSW|RSV_EST_COT }
	};
	for ( idx=0; idx<memcnt(Enb_); idx++ ){
		wnd->GetDlgItem((Enb_+idx)->cid)->EnableWindow( ( dtl->est & (Enb_+idx)->dss ) == 0 );
	}
}

static	void	dsp_rmd(						// Displaying room data on 1 row
CSimpleGrid	*grd,								// displaying target
RSV_EQP_DAT	*req,								// reservation_equipments
RSV_DTL_DAT	*dtl,								// reservation_details
int			row )								// displaying row
{
	WCHAR		buf[40], *ast;
	Csr			csr=NULL;
	RSV_RCG_DAT	*rcg;
	double		amt=0;
	int			add;

	grd->SetCellText( ROM_USD_CLM, row, !Nul_Fld(req->req->usd, FLD_DAY) ? EDT_DBF_DAY(buf, memcnt(buf), req->req->usd): _T("") );
	grd->SetCellText( ROM_UED_CLM, row, !Nul_Fld(req->req->ued, FLD_DAY) ? EDT_DBF_DAY(buf, memcnt(buf), req->req->ued): _T("") );
	grd->SetCellText( ROM_TYP_CLM, row, !Nul_Fld(req->tnm, FLD_WST) ? (WCHAR *)Fld_Ptr(req->tnm, FLD_WST): _T("") );
	grd->SetCellText( ROM_NBR_CLM, row, !Nul_Fld(req->rno, FLD_WST) ? (WCHAR *)Fld_Ptr(req->rno, FLD_WST): _T("") );
	while ( csr=Nxt_Lnk( dtl->rcg, csr ) ){
		rcg = Rec_Of(RSV_RCG_DAT, csr);
		add  = Cmp_Dte(DBS_DAY_PTR(req->req->usd), DBS_DAY_PTR(rcg->rcg->udt)) <= 0;
		add &= Cmp_Dte(DBS_DAY_PTR(req->req->ued), DBS_DAY_PTR(rcg->rcg->udt)) >  0;
		if ( add ){
			if ( Nul_Fld(rcg->rcg->amt, FLD_DBL) ){
				if( !Nul_Fld(rcg->prc, FLD_DBL) ){
					amt += DBS_DBL_VAL(rcg->prc);
				}
			}
			else{
				amt += DBS_DBL_VAL(rcg->rcg->amt);
			}
		}
	}
	if ( ast = Cma_Edt( amt, 0 ) ){
		grd->SetCellText( ROM_AMT_CLM, row, ast );
		free( ast );
	}
}

static	int		rcg_cnt(						// Calculate to possible room change number
Day			*arv,								// arrival_date
Day			*dpt,								// departure_date
PMS_MNG		*mng )								// Property management System management structure
{
	int			cnt=-1;
	Day			atp[1];

	*atp = *arv;
	if ( Cmp_Dte( atp, mng->bdt ) < 0 ){
		*atp = *mng->bdt;
		Add_Dte( atp, -1 );
	}
	//cnt = Cmp_Dte(dpt, atp);
	cnt = Clc_Dys(atp, dpt);

	return( cnt = cnt > 0 ? cnt-1: 0 );
}

static	void	rcg_lmt(						// Setting room change limit times
int			lmt,								// limit times
CWnd		*wnd )								// Parent window
{
	((CSpinButtonCtrl *)wnd->GetDlgItem(IDC_SPIN_ROOMCHANGE))->SetRange(0, lmt);
}

static	void	dsp_rom(						// displaying room detail
CSimpleGrid	*grd,								// target grid
RSV_DTL_DAT	*dtl,								// 予約詳細データ
PMS_MNG		*mng )								// Property management System management structure
{
	int					enb, row=0;
	RSV_EQP_DAT			*req;
	Csr					csr=NULL;

	grd->ResetContent();
	grd->InputEnable(TRUE);

	while( csr=Nxt_Lnk( dtl->req, csr ) ){
		req = Rec_Of(RSV_EQP_DAT, csr);
		enb = !( NO_RNO_CHG & dtl->est ) && (Cmp_Dte( DBS_DAY_PTR(req->req->ued), mng->bdt ) > 0);
		if ( Nul_Fld(req->req->ddt,FLD_DTM) ){
			row++;
			grd->AddRow(1);
			grd->SetData( 0, row, (int)(LONG_PTR)req );
			grd->SetEnable( -1, row, enb );
			dsp_rmd( grd, req, dtl, row );
		}
	}
}

static	void	dsp_dtl(						// 予約詳細表示
RSV_INF_DAT	*inf,								// NULL when new creation
RSV_DTL_DAT	*dtl,								// 予約詳細データ
CWnd		*wnd,								// 表示先ウィンドウ
CDatabase	*dbs,								// ODBC database object
PMS_MNG		*mng )								// Property management System management structure
{
	int					/*enb, */cnt, bfr, aii, rcc, row=0;
	WCHAR				buf[32];
//	RSV_EQP_DAT			*req;
	Csr					csr=NULL;
	Day					*arv, *dpt;
	CEquipmentTypeCombo	*ecb=(CEquipmentTypeCombo *)(wnd->GetDlgItem(IDC_COMBO_ROOMTYPE));
	CComboBox			*cbx=(CComboBox *)(wnd->GetDlgItem(IDC_COMBO_RATEPLANNAME));
	CSimpleGrid			*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_ROM));

	arv = DBS_DAY_PTR(dtl->dtl->arv);
	dpt = DBS_DAY_PTR(dtl->dtl->dpt);

	dtl->est = dtl_est( inf->rsv, dtl->dtl, dtl->smr, mng );
	Viw_Sts_Ope( (CCustomStatic	*)wnd->GetDlgItem(IDC_STATIC_DETAILSTS), dtl->est );

	// name
	wnd->GetDlgItem(IDC_EDIT_FIRSTNAME_DETAIL)->SetWindowText( EDT_DBF_WST(dtl->cli->fnm,_T("")) );
	wnd->GetDlgItem(IDC_EDIT_FAMILYNAME_DETAIL)->SetWindowText( EDT_DBF_WST(dtl->cli->lnm,_T("")) );
	// arrival date & tile
	wnd->GetDlgItem(IDC_EDIT_ARRIVALDATE)->SetWindowText( Edt_Dte( buf, memcnt(buf), DBS_DAY_PTR(dtl->dtl->arv)) );
	wnd->GetDlgItem(IDC_EDIT_ARRIVALTIME)->SetWindowText( EDT_DBF_TIM(buf, memcnt(buf), dtl->dtl->atm) );
	((CButton *)wnd->GetDlgItem(IDC_CHECK_EARLYCI))->SetCheck( !!DBS_INT_VAL(dtl->dtl->eci) );
	// departure date & time
	wnd->GetDlgItem(IDC_EDIT_DEPARTUREDATE)->SetWindowText( Edt_Dte( buf, memcnt(buf), DBS_DAY_PTR(dtl->dtl->dpt)) );
	swprintf_s( buf, memcnt(buf), _T("%d"), Clc_Dys((Day *)Fld_Ptr(dtl->dtl->arv, FLD_DAY), (Day *)Fld_Ptr(dtl->dtl->dpt, FLD_DAY)) );
	wnd->GetDlgItem(IDC_EDIT_DEPARTURETIME)->SetWindowText( EDT_DBF_TIM(buf, memcnt(buf), dtl->dtl->dtm) );
	((CButton *)wnd->GetDlgItem(IDC_CHECK_LATECO))->SetCheck( !!DBS_INT_VAL(dtl->dtl->lco) );
	// nights
	swprintf_s( buf, memcnt(buf), _T("%d"), Clc_Dys(arv, dpt) );
	wnd->GetDlgItem(IDC_EDIT_NIGHTS)->SetWindowText( buf );
	rcc = ( rcc = Cmp_Dte( mng->bdt, arv ) ) > 0 ? rcc: 0;
	((CSpinButtonCtrl *)wnd->GetDlgItem(IDC_SPIN_NIGHTS))->SetRange(rcc,UD_MAXVAL );
	// adult number
	wnd->GetDlgItem(IDC_EDIT_ADULTNUMBER)->SetWindowTextW( EDT_DBF_INT(buf, memcnt(buf), dtl->dtl->psn, Edt_Num_Nml) );
	((CSpinButtonCtrl *)wnd->GetDlgItem(IDC_SPIN_ADULTNUMBER))->SetRange(1,UD_MAXVAL );
	// child number
	wnd->GetDlgItem(IDC_EDIT_CHILDNUMBER)->SetWindowTextW( EDT_DBF_INT(buf, memcnt(buf), dtl->dtl->chn, Edt_Num_Nml) );
	((CSpinButtonCtrl *)wnd->GetDlgItem(IDC_SPIN_CHILDNUMBER))->SetRange(0,UD_MAXVAL );
	// room type
	ecb = (CEquipmentTypeCombo *)(wnd->GetDlgItem(IDC_COMBO_ROOMTYPE));
	BKG_PTH(bfr, aii, inf->rsv );
	Viw_Scb_Eqt( ecb, bfr, aii,	arv, dpt, !!(dtl->est & NO_RTP_CHG), wnd, dbs, mng );
	ecb->SetType( Nul_Fld(dtl->dtl->eti, FLD_INT) ? -1: DBS_INT_VAL(dtl->dtl->eti) );
	// Disable the room type selector if booking path not selected.
	//wnd->GetDlgItem(IDC_COMBO_ROOMTYPE)->EnableWindow( enb );

	// rate plan name
	rte_pln( dtl, wnd, dbs, mng );

	// room change
	cnt = Cnt_Lnk( dtl->req );
	swprintf_s( buf, memcnt(buf), _T("%d"), ( cnt > 0 ? cnt-1: 0 ) );
	wnd->GetDlgItem(IDC_EDIT_ROOMCHANGE)->SetWindowText( buf );
	rcg_lmt( rcg_cnt(arv, dpt, mng), wnd );

	// note
	wnd->GetDlgItem(IDC_EDIT_NOTE)->SetWindowText( EDT_DBF_WST(dtl->dtl->nte,_T("")) );
	// Same as made a reservation person
	((CButton *)wnd->GetDlgItem(IDC_CHECK_SAMERESERVATION))->SetCheck( !!dtl->smr );
	wnd->GetDlgItem(IDC_BUTTON_SEARCHDETAIL)->EnableWindow( !dtl->smr );

	dsp_rom( grd, dtl, mng );

	dtl_enb( dtl, wnd );
}

static	void	dsp_dtg(						// Displaying reservation_details grid
int			row,								// Row of grid
RSV_DTL_DAT	*dtl,								// Displaying data
CWnd		*wnd,								// Parent window
PMS_MNG		*mng )								// Hotel Property Management System Structure
{
	WCHAR				buf[520];
	int					rdi=0;
	EQP_TYP				*eqt=NULL;
	CSimpleGrid			*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL));
	Csr					csr=NULL;

	while( csr=Nxt_Lnk(mng->etl, csr) ){
		if ( DBS_INT_VAL(Rec_Of(EQP_TYP, csr)->id)==DBS_INT_VAL(dtl->dtl->eti) ){
			eqt = Rec_Of(EQP_TYP, csr);
			break;
		}
	}
		
	grd->SetCellText( RSV_NEW_CLM, row, Nul_Fld(dtl->dtl->id,FLD_INT) ? _T("*"): _T("") );	// New
	swprintf_s( buf, memcnt(buf), _T("%s %s"), EDT_DBF_WST(dtl->cli->fnm,_T("")), EDT_DBF_WST(dtl->cli->lnm,_T("")) );
	grd->SetCellText( RSV_NAM_CLM, row, buf );												// Name
	grd->SetCellText( RSV_STS_CLM, row, Viw_Sts_Ope( NULL, dtl->est ) );					// Status
	if ( eqt )	grd->SetCellText( RSV_RTP_CLM, row, EDT_DBF_WST(eqt->nam,_T("")) );			// R.Typ
//	grd->SetCellText( RSV_RNB_CLM, row, EDT_DBF_WST(dtl->rno,_T("")) );						// R.No.
	grd->SetCellText( RSV_ARV_CLM, row, EDT_DBF_DAY(buf, memcnt(buf), dtl->dtl->arv) );		// Arrv
	grd->SetCheck( RSV_ECI_CLM, row, !!DBS_INT_VAL(dtl->dtl->eci) );						// E.C/I
	grd->SetCellText( RSV_DPT_CLM, row, EDT_DBF_DAY(buf, memcnt(buf), dtl->dtl->dpt) );		// Dept
	grd->SetCheck( RSV_LCO_CLM, row, !!DBS_INT_VAL(dtl->dtl->lco) );						// L C/O
	swprintf_s( buf, memcnt(buf), _T("%d/%d"), DBS_INT_VAL(dtl->dtl->psn), DBS_INT_VAL(dtl->dtl->chn) );
	grd->SetCellText(RSV_ADC_CLM, row, buf );												//Ad/Ch

	grd->SetData( RSV_DAT_CLM, row, (int)(LONG_PTR)dtl );
}

static	void	fst_lst(				// update firstarrival lastdeparture
RSV_INF_DAT	*inf,						// NULL when new creation
CWnd		*wnd )						// Parent window
{
	Csr			csr=NULL;
	RSV_DTL_DAT	*dtl;
	Day			arv[1], dpt[1];
	WCHAR		buf[40];

	memset( arv, 0, sizeof(arv) );
	memset( dpt, 0, sizeof(dpt) );
	arv->yer = 9999;

	if ( !!Cnt_Lnk( inf->dtl ) ){
		while( csr = Nxt_Lnk( inf->dtl, csr ) ){
			dtl = Rec_Of(RSV_DTL_DAT, csr);
			if ( Cmp_Dte( arv, DBS_DAY_PTR(dtl->dtl->arv) ) >= 0  )	*arv = *DBS_DAY_PTR(dtl->dtl->arv);
			if ( Cmp_Dte( dpt, DBS_DAY_PTR(dtl->dtl->dpt) ) <= 0  )	*dpt = *DBS_DAY_PTR(dtl->dtl->dpt);
		}
		*DBS_DAY_PTR(inf->rsv->arv) = *arv;
		*DBS_DAY_PTR(inf->rsv->dpt) = *dpt;
		wnd->GetDlgItem(IDC_EDIT_FIRSTARRIVAL)->SetWindowText( EDT_DBF_DAY(buf, memcnt(buf), inf->rsv->arv) );
		wnd->GetDlgItem(IDC_EDIT_LASTDEPARTURE)->SetWindowText( EDT_DBF_DAY(buf, memcnt(buf), inf->rsv->dpt) );
	}
}

static	void	dsp_inf(						// displaying reservation_infos
RSV_INF_DAT	*inf,								// NULL when new creation
CWnd		*wnd )								// Parent window
{
	WCHAR				buf[520];
	int					rdi=0, row=0, ckd, bfr, aii, rnk;

	// Text datas on reservation_infos
	swprintf_s( buf, memcnt(buf), _T("%s"), is_new(inf->rsv) ? _T("Creating"): EDT_DBF_INT(buf, memcnt(buf), inf->rsv->rvn, Edt_Num_Nml) );
	fst_lst( inf, wnd );
	wnd->GetDlgItem(IDC_STATIC_BOOKINGID)->SetWindowText( buf );
	wnd->GetDlgItem(IDC_EDIT_EXTBOOKINGID)->SetWindowText( EDT_DBF_WST(inf->rsv->ern,_T("")) );
	wnd->GetDlgItem(IDC_EDIT_FIRSTNAME)->SetWindowText( EDT_DBF_WST(inf->cli->fnm,_T("")) );
	wnd->GetDlgItem(IDC_EDIT_FAMILYNAME)->SetWindowText( EDT_DBF_WST(inf->cli->lnm,_T(""))  );
	wnd->GetDlgItem(IDC_EDIT_ADDRESS1)->SetWindowText( EDT_DBF_WST(inf->cli->adr1,_T("")) );
	wnd->GetDlgItem(IDC_EDIT_ADDRESS2)->SetWindowText( EDT_DBF_WST(inf->cli->adr2,_T("")) );
	wnd->GetDlgItem(IDC_EDIT_TELEPHONE1)->SetWindowText( EDT_DBF_WST(inf->cli->tel1,_T("")) );
	wnd->GetDlgItem(IDC_EDIT_TELEPHONE2)->SetWindowText( EDT_DBF_WST(inf->cli->tel2,_T("")) );
	wnd->GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText( EDT_DBF_WST(inf->cli->fax,_T("")) );
	wnd->GetDlgItem(IDC_EDIT_EMAIL1)->SetWindowText( EDT_DBF_WST(inf->cli->eml1,_T("")) );
	wnd->GetDlgItem(IDC_EDIT_EMAIL2)->SetWindowText( EDT_DBF_WST(inf->cli->eml2,_T("")) );

	// ReservationType
	ckd =  is_new(inf->rsv) ? CLK_PERSONAL: DBS_INT_VAL(inf->cli->ckd);
	((CButton *)wnd->GetDlgItem(IDC_RADIO_PERSONAL))->SetCheck( ckd == CLK_PERSONAL );
	((CButton *)wnd->GetDlgItem(IDC_RADIO_GROUP))->SetCheck( ckd == CLK_GROUP );
	// BookingPath
	BKG_PTH( bfr, aii, inf->rsv );
	((CBookingPathCombo *)(wnd->GetDlgItem(IDC_COMBO_BOOKINGPATH)))->SetItemIndex( bfr, aii );
	// ClientRank
	rnk = Nul_Fld(inf->cli->crk,FLD_INT) ? CLL_DEFAULT: DBS_INT_VAL(inf->cli->crk);
	((CClientRankCombo *)wnd->GetDlgItem(IDC_COMBO_CLIENTRANK))->SetRank( rnk );
	// minimum room numbers
	((CSpinButtonCtrl *)wnd->GetDlgItem(IDC_SPIN_ROOMNUMBER))->SetRange( Cnt_Lnk(inf->dtl), UD_MAXVAL );
}

static	void	dsp_dat(						// Displaying reservation_infos data
RSV_INF_DAT	*inf,								// NULL when new creation
CDatabase	*dbs,								// ODBC database object
CWnd		*wnd,								// Parent window
PMS_MNG		*mng )								// Property management System management structure
{
	WCHAR				buf[20];
	RSV_DTL_DAT			*dtl;
	Csr					csr=NULL;
	int					idx=0, rdi=0, row=0;
	CSimpleGrid			*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL));
	CTabCtrl			*tab = (CTabCtrl *)wnd->GetDlgItem(IDC_TAB_RSV_DETAIL);

	inf->est = inf_est( inf->rsv, inf->dtl, mng );
	Viw_Sts_Ope( (CCustomStatic	*)wnd->GetDlgItem(IDC_STATIC_INFOSTS), inf->est );

	dsp_inf( inf, wnd );
	inf_enb( inf, wnd );

	// Setting reservation_details grid
	grd->ResetContent();
	grd->AddRow( Cnt_Lnk( inf->dtl ) );

	while( csr = Nxt_Lnk( inf->dtl, csr ) ){
		dtl = Rec_Of(RSV_DTL_DAT, csr);
		dtl->est = dtl_est( inf->rsv, dtl->dtl, dtl->smr, mng );
		if ( !row ){
			// Displaying first reservation_detail to reservation_detail area
			dsp_dtl( inf, dtl, wnd, dbs, mng );
		}
		dsp_dtg( ++row, dtl, wnd, mng );
		grd->SetEnable( RSV_CHK_CLM, row, TRUE );
	}
	grd->SetSelectCell( 0, 1 );
	// room numbers editbox It shoud be this location because editchange event is happen just aftre this setting
	swprintf_s( buf, memcnt(buf), _T("%d"), Cnt_Lnk(inf->dtl) );
	wnd->GetDlgItem(IDC_EDIT_ROOMNUMBER)->SetWindowText( buf );

	// setting carette
	wnd->GetDlgItem(IDC_EDIT_FIRSTNAME)->SetFocus();

}

static	int		get_spd(				// getting stay period from screen
Day			*arv,						// storing arrival date pointer
Day			*dpt,						// storing departure date pointer
Day			*bdt,						// business date
int			est,						// Editting status
int			glc,						// greater or lesser check
CWnd		*wnd )						// Parent window
{
	WCHAR		buf[192], *itm, *atn=_T("ArrivalDate"), *dtn=_T("DepartureDate"); 
	WCHAR		*pfx=NULL, *pfx1=_T("%s should be same or greater than today"), *pfx2=_T("%s is wrong");
	int			chk, dys;

	wnd->GetDlgItem(IDC_EDIT_ARRIVALDATE)->GetWindowText( buf, memcnt(buf) );
	if ( chk = Str_Dte( arv, buf ) ){
		if ( chk = !bdt || !!(est&RSV_EST_INH) || ( Cmp_Dte( arv, bdt ) >= 0 ) ){
			wnd->GetDlgItem(IDC_EDIT_DEPARTUREDATE)->GetWindowText( buf, memcnt(buf) );
			if ( chk = Str_Dte( dpt, buf ) ){
				if ( chk = !bdt || ( Cmp_Dte( dpt, bdt ) >= 0 ) ){
					if ( chk = !glc || ( Cmp_Dte( arv, dpt ) <= 0 ) ){
						dys = Clc_Dys( arv, dpt );
					}
					else{
						pfx = _T("ArrivalDate is grater than DepartureDate");
					}
				}
				else{
					itm = dtn, pfx = pfx1;
				}
			}
			else{
				itm=dtn, pfx=pfx2;
			}
		}
		else{
			itm = atn, pfx = pfx1;
		}
	}
	else{
		itm=atn, pfx=pfx2;
	}
	if ( pfx ){
		swprintf_s( buf, memcnt(buf), pfx, itm );
		wnd->MessageBox( buf, NULL, MB_ICONEXCLAMATION|MB_OK );
	}
	return( chk ? dys: -1 );
}

static	int		rom_avl(				// confirmation room avalable on inner same reservation
RSV_EQP_DAT	*own,						// room user data
int			eii,						// equipment_info_id
Day			*usd,						// use start date
Day			*ued,						// use end date
CWnd		*wnd,						// Parent window
CDatabase	*dbs,						// ODBC Database object
PMS_MNG		*mng )						// Hotel Property management System Structure
{
	int			chk=TRUE, cnt, avl=FALSE;
	WCHAR		*erm=NULL;
	Csr			csr=NULL, cse=NULL;
	RSV_INF_DAT	*inf = RSV_INF_(wnd);
	RSV_DTL_DAT	*dtl;
	RSV_EQP_DAT	*req;
	Day			*dsd, *ded;

	// At first confirm avalable on inner(own reservation) data 
	while ( csr=Nxt_Lnk( inf->dtl, csr ) ){
		dtl = Rec_Of(RSV_DTL_DAT, csr);
		while ( cse=Nxt_Lnk( dtl->req, cse ) ){
			req = Rec_Of(RSV_EQP_DAT, cse);
			if ( eii==DBS_INT_VAL(req->req->eii) ){
				if ( Csr_Of(req) != Csr_Of(own) ){
					dsd = DBS_DAY_PTR(req->req->usd);
					ded = DBS_DAY_PTR(req->req->ued);
					//if ( Viw_Ovl_Pod( usd, ued, dsd, ded ) )	break;
					if ( ( Cmp_Dte( usd, dsd ) >= 0 && Cmp_Dte( usd, ded ) < 0 ) 
						|| ( Cmp_Dte( ued, dsd ) > 0 && Cmp_Dte( ued, ded ) <= 0 ) ){
							break;
					}
				}
			}
		}
		if ( !!cse )	break;
	}
	if ( !csr ){
		if ( chk = ( cnt = Rsv_Rom_Avl( eii, DBS_INT_VAL(own->req->rdi), usd, ued, &erm, dbs, mng ) ) != -1 ){
			avl = cnt==0;
		}
	}
	if ( erm ){
		wnd->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
		free( erm );
	}
	return( chk ? avl: -1 );
}

int		cfm_avl(						// conform room abailable when stay period changed
RSV_EQP_DAT		*req,					// confirmation target
CWnd			*wnd,					// Parent window
CDatabase		*dbs,					// ODBC Database object
PMS_MNG			*mng )					// Hotel Property management System Structure
{
	int			chk=!req;

	if ( !chk && !Nul_Fld(req->req->eii, FLD_INT) ){
		if ( ( chk = rom_avl( req, DBS_INT_VAL(req->req->eii), DBS_DAY_PTR(req->req->usd), DBS_DAY_PTR(req->req->ued), wnd, dbs, mng ) ) != -1 ){
			if ( !chk ){
				wnd->MessageBox( _T("Room is not available.Cancel this room once."), NULL, MB_ICONEXCLAMATION|MB_OK );
				DBS_INT_SET(req->req->eii, (int *)NULL);
				DBS_INT_SET(req->req->eti, (int *)NULL);
				*req->rno = 0;
				*req->tnm = 0;
			}
		}
	}
	return( chk!=-1 && chk ); 
}

static	int		set_req(				// setting the reservation_equipment
CSimpleGrid		*grd,					// Displaying target grid
RSV_DTL_DAT		*dtl,					// setting target
Day				*arv,					// arrival date
Day				*dpt,					// departure date
CWnd			*wnd,					// Parent window
CDatabase		*dbs,					// ODBC Database object
PMS_MNG			*mng )					// Hotel Property management System Structure
{
	Csr			csr;
	RSV_EQP_DAT	*req;

	if ( csr = Nxt_Lnk( dtl->req, NULL ) ){
		// set arrival date into first using room
		req = Rec_Of(RSV_EQP_DAT, csr);
		*DBS_DAY_PTR(req->req->usd) = *arv;
		cfm_avl( req, wnd, dbs, mng );
		dsp_rmd( grd, req, dtl, 1 ); 
	}
	if ( csr = Prv_Lnk( dtl->req, NULL ) ){
		// set departure date into last using room
		req = Rec_Of(RSV_EQP_DAT, csr);
		*DBS_DAY_PTR(req->req->ued) = *dpt;
		cfm_avl( req, wnd, dbs, mng );
		dsp_rmd( grd, req, dtl, grd->GetLineCount( FALSE ) ); 
	}
	return( TRUE );
}

static	ECG_PLN_DAT	*get_pln(		// Getting selecting plan
CWnd			*wnd )				// Target window
{
	int			idx;
	ECG_PLN_DAT	*pln = NULL;
	CComboBox	*cbx = (CComboBox *)(wnd->GetDlgItem(IDC_COMBO_RATEPLANNAME));

	if ( ( idx = cbx->GetCurSel() ) != -1 ){
		pln = (ECG_PLN_DAT *)(LONG_PTR)cbx->GetItemData( idx );
	}

	return( pln );
}

double		Rsv_Apl_Amt(			// amount apply by apply day kind
RSV_RCG_DAT		*rcg,				// destination pointer
RSV_CRG			*crg,				// destination pointer
Day				*dte,				// applying date
ECG_PLN_DAT		*pln,				// plan data
PMS_MNG			*mng )				// Hotel Property Management Structure
{
	int			edi, dkd, hli, bhi;
	int			dkt[] = { DKD_SUNDAY, DKD_MONDAY, DKD_TUESDAY, DKD_WEDNESDAY, DKD_THURSDAY, DKD_FRIDAY, DKD_SATURDAY };
	BIZ_CDR_DAT	*cdr;
	ECG_DTL_DAT	*egd;
	Csr		csr=NULL;
	double	amt=0, *hol=NULL, *bhl=NULL;

	while( csr=Nxt_Lnk( pln->dtl, csr ) ){
		egd = Rec_Of(ECG_DTL_DAT, csr);
		if ( !!( DBS_INT_VAL(egd->adk) & DKD_PRE_HOLIDAY ) )	bhl = DBS_DBL_PTR(egd->prc), bhi = DBS_INT_VAL(egd->edi);
		if ( !!( DBS_INT_VAL(egd->adk) & DKD_HOLIDAY) )			hol = DBS_DBL_PTR(egd->prc), hli = DBS_INT_VAL(egd->edi);
	}
	if ( hol || bhl ){
		while( csr=Nxt_Lnk( mng->bcl, csr ) ){
			cdr = Rec_Of(BIZ_CDR_DAT, csr);
			if ( cdr->dte->yer==dte->yer && cdr->dte->mth==dte->mth && cdr->dte->day==dte->day ){
				if ( bhl && cdr->knd==CDK_PRE_HOLIDAY ){
					amt = *bhl;
					edi = bhi;
					break;
				}
				if ( hol && cdr->knd==CDK_HOLIDAY ){
					amt = *hol;
					edi = hli;
					break;
				}
			}
		}
	}
	if ( !csr ){
		dkd = dkt[Get_Dow(dte)];
		while( csr=Nxt_Lnk( pln->dtl, csr ) ){
			egd = Rec_Of(ECG_DTL_DAT, csr);
			if ( !!( DBS_INT_VAL(egd->adk) & dkd ) )	amt = *DBS_DBL_PTR(egd->prc), edi = DBS_INT_VAL(egd->edi);
		}
	}
	if ( !!rcg ){
		DBS_DBL_SET(rcg->prc, &amt); 
		DBS_INT_SET(rcg->rcg->edi, &edi); 
	}
	if ( !!crg ){
		DBS_INT_SET(crg->edi, &edi);
	}
	return( amt );
}

static	void	cli_def(			// setting client_infos table default value
CLI_INF			*dst,				// destination pointer
int				knd,				// client_kind
int				rnk,				// client_rank
WCHAR			*opc )				// operator_code
{
	int			val;

	DBS_WST_SET(dst->opc, opc );			// operator_code of client_infos
	DBS_INT_SET(dst->ckd, &knd);			// client_kind
	DBS_INT_SET(dst->crk, &rnk);			// client_rank
	DBS_INT_SET(dst->usc, &(val=0));		// Usage count
	DBS_INT_SET(dst->nsc, &(val=0));		// No Show count
}

static	void	set_rcg(				// charge data setting
RSV_RCG_DAT		*rcg,				// destinationpointer
RSV_DTL_DAT		*dtl,				// target reservation_detail data
CWnd			*wnd,				// Parent window
PMS_MNG			*mng )				// Hotel Property Management Structure
{
	void	*nul=NULL;
	double	amt;

	ECG_PLN_DAT	*pln = get_pln( wnd );

	DBS_INT_CPY(rcg->rcg->rii, RSV_INF_(wnd)->rsv->id);
	DBS_INT_CPY(rcg->rcg->rdi, dtl->dtl->id);
	if ( pln ){
		DBS_INT_CPY(rcg->rcg->cgk, pln->cgk);
		DBS_INT_CPY(rcg->rcg->dsi, pln->dsi);
		//DBS_INT_CPY(rcg->rcg->eci, pln->eci);
		Set_Nul(rcg->prc, FLD_DBL, FALSE);
		amt = Rsv_Apl_Amt(rcg, NULL, DBS_DAY_PTR(rcg->rcg->udt), pln, mng);
		//if ( Nul_Fld(rcg->rcg->amt, FLD_DBL) ){
		//	DBS_DBL_CPY(rcg->rcg->amt, rcg->prc);
		//}
		DBS_WST_CPY(rcg->etn, pln->etn);
		DBS_WST_CPY(rcg->eqc, pln->eqc);
		DBS_WST_CPY(rcg->agn, pln->agn);
	}
	else{
		DBS_INT_SET(rcg->rcg->cgk, (int *)nul);
		DBS_INT_SET(rcg->rcg->dsi, (int *)nul);
		DBS_INT_SET(rcg->rcg->edi, (int *)nul);
		DBS_DBL_SET(rcg->prc, (double *)nul);
		DBS_WST_SET(rcg->etn, (WCHAR *)nul);
		DBS_WST_SET(rcg->eqc, (WCHAR *)nul);
		DBS_WST_SET(rcg->agn, (WCHAR *)nul);
	}
}

static	void	req_def(				// setting reservation_equipment default data
RSV_EQP_DAT	*dst,						// destination data pointer
Day			*arv,						// arrival date
Day			*dpt,						// departure date
WCHAR		*opc )						// operator_code
{
	int			val;

	DBS_WST_SET(dst->req->opc, opc);
	DBS_DAY_SET(dst->req->usd, arv);
	DBS_DAY_SET(dst->req->ued, dpt);
	DBS_INT_SET(dst->req->inh, &(val=0));
}

static	RSV_EQP_DAT	*dtl_def(			// setting reservation_details default data
RSV_DTL_DAT	*dst,						// destination data pointer
Day			*arv,						// arrival date
Day			*dpt,						// departure date
WCHAR		*opc )						// operator_code
{
	Csr			csr;
	int			val, dys, idx;
	Day			dte[1];
	RSV_EQP_DAT	*req;
	RSV_RCG_DAT	*rcg;

	DBS_WST_SET(dst->dtl->opc, opc);
	DBS_DAY_SET(dst->dtl->arv, arv);
	DBS_DAY_SET(dst->dtl->dpt, dpt);
	DBS_INT_SET(dst->dtl->psn, &(val = 1) );			
	DBS_INT_SET(dst->dtl->chn, &(val = 0) );
	DBS_INT_SET(dst->dtl->inh, &(val = 0) );
	DBS_INT_SET(dst->dtl->cxl, &(val = 0) );
	DBS_INT_SET(dst->dtl->eci, &(val = 0) );
	DBS_INT_SET(dst->dtl->lco, &(val = 0) );
	cli_def( dst->cli, CLK_PERSONAL, CLL_GENERAL, opc );

	if ( csr = Add_Lnk( dst->req, NULL, sizeof(RSV_EQP_DAT) ) ){
		req = Rec_Of(RSV_EQP_DAT, csr);
		req_def( req, arv, dpt, opc );
	}
	if ( csr ){
		dys = Clc_Dys( arv, dpt );
		for ( idx=0, *dte=*arv; idx<dys; idx++ ){
			if ( !!( csr = Add_Lnk( dst->rcg, NULL, sizeof(RSV_RCG_DAT) ) ) ){
				rcg = Rec_Of(RSV_RCG_DAT, csr);
				DBS_WST_SET(rcg->rcg->opc, opc);
				DBS_INT_SET(rcg->rcg->pup, &(val = 0) );
				DBS_DAY_SET(rcg->rcg->udt, dte );
				Add_Dte( dte, 1 );
			}
		}
		if ( idx<dys )	Del_All( dst->rcg );
	}

	return( !!csr ? req: NULL );
}

static	void	get_cli(					// Getting client_inf data
RSV_INF_DAT		*inf,						// Getting reservation_infos
CWnd			*wnd )						// Reservation view window
{
	int			rnk;

	WIN_TXT_DBS(inf->cli->fnm, IDC_EDIT_FIRSTNAME, wnd);
	WIN_TXT_DBS(inf->cli->lnm, IDC_EDIT_FAMILYNAME, wnd);
	WIN_TXT_DBS(inf->cli->adr1, IDC_EDIT_ADDRESS1, wnd);
	WIN_TXT_DBS(inf->cli->adr2, IDC_EDIT_ADDRESS2, wnd);
	WIN_TXT_DBS(inf->cli->tel1, IDC_EDIT_TELEPHONE1, wnd);
	WIN_TXT_DBS(inf->cli->tel2, IDC_EDIT_TELEPHONE2, wnd);
	WIN_TXT_DBS(inf->cli->fax, IDC_EDIT_FAXNUMBER, wnd);
	WIN_TXT_DBS(inf->cli->eml1, IDC_EDIT_EMAIL1, wnd);
	WIN_TXT_DBS(inf->cli->eml2, IDC_EDIT_EMAIL2, wnd);
	if ( ( rnk = ((CClientRankCombo *)(wnd->GetDlgItem(IDC_COMBO_CLIENTRANK)))->GetRank() ) != -1 ){
		DBS_INT_SET(inf->cli->crk,&rnk);
	}
	else{
		DBS_INT_SET(inf->cli->crk,(int *)0);
	}
}

static	int		get_inf(					// Getting reservation_infos data from screen
RSV_INF_DAT		*inf,						// Getting reservation_infos
CWnd			*wnd,						// Reservation view window
PMS_MNG			*mng )						// Property management System management structure
{
	int					chk, psn, grp, pth, agt, rnk;
	WCHAR				*req=NULL;
	CBookingPathCombo	*cbx;
	
	WIN_TXT_DBS(inf->rsv->ern, IDC_EDIT_EXTBOOKINGID, wnd);

	psn = CBX_GET_CHK(IDC_RADIO_PERSONAL,wnd);
	grp = CBX_GET_CHK(IDC_RADIO_GROUP,wnd);
	if ( chk = ( !!psn || !!grp ) ){
		*DBS_INT_PTR(inf->cli->ckd) = psn ? CLK_PERSONAL: CLK_GROUP;
		Set_Nul(inf->cli->ckd,FLD_INT,FALSE);
	}
	else{
		req = _T("ReservationType");
	}
	if ( chk && !( chk=!WIN_TXT_DBS(inf->cli->fnm, IDC_EDIT_FIRSTNAME, wnd) ) ){
		req = _T("FirstName");
	}
	if ( chk )	get_cli( inf, wnd );

	//if ( chk )	WIN_TXT_DBS(inf->cli->lnm, IDC_EDIT_FAMILYNAME, wnd);
	//if ( chk )	WIN_TXT_DBS(inf->cli->adr1, IDC_EDIT_ADDRESS1, wnd);
	//if ( chk )	WIN_TXT_DBS(inf->cli->adr2, IDC_EDIT_ADDRESS2, wnd);
	//if ( chk )	WIN_TXT_DBS(inf->cli->tel1, IDC_EDIT_TELEPHONE1, wnd);
	//if ( chk )	WIN_TXT_DBS(inf->cli->tel2, IDC_EDIT_TELEPHONE2, wnd);
	//if ( chk )	WIN_TXT_DBS(inf->cli->fax, IDC_EDIT_FAXNUMBER, wnd);
	//if ( chk )	WIN_TXT_DBS(inf->cli->eml1, IDC_EDIT_EMAIL1, wnd);
	//if ( chk )	WIN_TXT_DBS(inf->cli->eml2, IDC_EDIT_EMAIL2, wnd);

	if ( chk ){
		cbx = (CBookingPathCombo *)wnd->GetDlgItem(IDC_COMBO_BOOKINGPATH);
		cbx->GetItemIndex( &pth, &agt );
		if ( chk = pth!=-1 ){
			Set_Nul(inf->rsv->bfr,FLD_INT,FALSE);
			*DBS_INT_PTR(inf->rsv->bfr) = pth;
			Set_Nul(inf->rsv->aii,FLD_INT,pth!=VIA_AGENT);
			*DBS_INT_PTR(inf->rsv->aii) = agt;
		}
		else{
			req = _T("BookingPath");
		}
	}
	if ( chk ){
		if ( chk = ( rnk = ((CClientRankCombo *)(wnd->GetDlgItem(IDC_COMBO_CLIENTRANK)))->GetRank() ) != -1 ){
			Set_Nul(inf->cli->crk,FLD_INT,FALSE);
			*DBS_INT_PTR(inf->cli->crk) = rnk;
		}
		else{
			req = _T("ClientRank");
		}
	}

	if ( req ){
		WCHAR		buf[128];
		swprintf_s( buf, memcnt(buf), _T("%s is required item"), req );
		wnd->MessageBox( buf, 0, MB_ICONEXCLAMATION|MB_OK );
	}

	return( chk );
}

static	int		get_dtl(					// Getting reservation_details data from screen
RSV_DTL_DAT		*dtl,						// Getting reservation_infos
CWnd			*wnd,						// Reservation view window
PMS_MNG			*mng )						// Property management System management structure
{
	int					chk, err, eti;
	WCHAR				*rqi=NULL, *erm=NULL;
	Csr					csr=NULL;
	CEquipmentTypeCombo	*etc = (CEquipmentTypeCombo *)(wnd->GetDlgItem(IDC_COMBO_ROOMTYPE));

	if ( !( chk = !!dtl->smr ) ){
		WIN_TXT_DBS(dtl->cli->fnm, IDC_EDIT_FIRSTNAME_DETAIL, wnd);
		WIN_TXT_DBS(dtl->cli->lnm, IDC_EDIT_FAMILYNAME_DETAIL, wnd);

		if ( !( chk = !Nul_Fld(dtl->cli->fnm, FLD_WST) || Nul_Fld(dtl->cli->lnm, FLD_WST) ) ){
			rqi = _T("FirstName");
		}
		if ( chk && Nul_Fld(dtl->cli->id, FLD_WST) && !Nul_Fld(dtl->cli->fnm, FLD_WST) ){
			cli_def( dtl->cli, CLK_PERSONAL, CLL_GENERAL, OPE_COD_(wnd) );
		}
	}

	// arrival date and departure date
	if ( chk ){
		chk = get_spd( DBS_DAY_PTR(dtl->dtl->arv), DBS_DAY_PTR(dtl->dtl->dpt), mng->bdt, dtl->est, TRUE, wnd ) != -1;
	}

	// arrival time and departure time
	if ( chk ){
		WIN_TIM_DBS(dtl->dtl->atm, &err, IDC_EDIT_ARRIVALTIME, wnd);
		if ( !( chk = !err ) )	erm = _T("arrival time is invalid");
	}
	if ( chk ){
		WIN_DAY_DBS(dtl->dtl->dtm, &err, IDC_EDIT_DEPARTURETIME, wnd);
		if ( !( chk = !err ) )	erm = _T("departure time is invalid");
	}

	if ( chk && !Nul_Fld(dtl->dtl->atm, FLD_TIM) && !Nul_Fld(dtl->dtl->dtm, FLD_TIM) ){
		chk = Cmp_Tim(DBS_TIM_PTR(dtl->dtl->atm),DBS_DAY_PTR(dtl->dtl->dtm))<=0;
		if ( !chk )	erm = _T("arrival time is grater than departure time");
	}

	WIN_CBX_DBS(dtl->dtl->eci,IDC_CHECK_EARLYCI,wnd);
	WIN_CBX_DBS(dtl->dtl->lco,IDC_CHECK_LATECO,wnd);

	if ( chk && !( chk = !WIN_INT_DBS(dtl->dtl->psn, IDC_EDIT_ADULTNUMBER, wnd) ) ){
		rqi = _T("adult person number");
	}
	if ( chk && !( chk = !WIN_INT_DBS(dtl->dtl->chn, IDC_EDIT_CHILDNUMBER, wnd) ) ){
		rqi = _T("children number");
	}
	if ( chk ){
		if ( !!( eti = etc->GetType( NULL ) ) ){
		//if ( chk = !!( eti = etc->GetType( NULL ) ) ){
			Set_Nul(dtl->dtl->eti, FLD_INT, FALSE);		// To not NULL inhouse
			*DBS_INT_PTR(dtl->dtl->eti) = eti;
		}
		//else{
		//	rqi = _T("RequestedRoomType");
		//}
	}

	WIN_TXT_DBS(dtl->dtl->nte, IDC_EDIT_NOTE, wnd);

	if ( erm ){
		wnd->MessageBox( erm, 0, MB_ICONEXCLAMATION|MB_OK );
	}
	else if ( rqi ){
		WCHAR		buf[128];
		swprintf_s( buf, memcnt(buf), _T("%s is required item"), rqi );
		wnd->MessageBox( buf, 0, MB_ICONEXCLAMATION|MB_OK );
	}

	return( chk );
}

void	Rsv_Sch_Cli(				// client search button click
RSV_DTL_DAT		*dtl,				// NULL=client_infs ofreservation_infos, !NULL=reservation_details
CWnd			*wnd,				// ReservationView
CDatabase		*dbs,				// Database object
PMS_MNG			*mng )				// Hotel Property Management System Structure
{
	CClientInfoDlg	*dlg;

	if ( dlg = new CClientInfoDlg() ){
		dlg->m_pPms_mng = mng;
		dlg->m_pDbs_obj = dbs;
		dlg->m_Mkt_seg = !dtl && !Nul_Fld(RSV_INF_(wnd)->rsv->msi, FLD_INT) ? DBS_INT_VAL(RSV_INF_(wnd)->rsv->msi): 0;
		dlg->m_Mks_edt = !dtl;
		get_cli( RSV_INF_(wnd), wnd );
		dlg->m_Cli_inf = dtl ? dtl->cli: RSV_INF_(wnd)->cli;
		if ( dlg->DoModal()==IDOK ){
			if ( !!dlg->m_Mkt_seg ){
				DBS_INT_SET(RSV_INF_(wnd)->rsv->msi, &dlg->m_Mkt_seg);
			}
			else{
				Set_Nul(RSV_INF_(wnd)->rsv->msi, FLD_INT, TRUE);
			}
			if ( dtl )	dsp_dtl( RSV_INF_(wnd), dtl, wnd, dbs, mng );
			else		dsp_inf( RSV_INF_(wnd), wnd );
		}
		delete dlg;
	}

}

void	Rsv_Bph_Chg(				// Booking path change event
RSV_DTL_DAT		*dtl,				// selecting reservation_details 
CWnd			*wnd,				// ReservationView
CDatabase		*dbs,				// Database object
PMS_MNG			*mng )				// Hotel Property Management System Structure
{
	int					pth, agi, sam, eti, avl;
	RSV_INF_DAT			*inf = RSV_INF_(wnd);
	CBookingPathCombo	*bpc = (CBookingPathCombo *)(wnd->GetDlgItem(IDC_COMBO_BOOKINGPATH));
	CComboBox			*rtc = (CComboBox *)(wnd->GetDlgItem(IDC_COMBO_ROOMTYPE));

	bpc->GetItemIndex( &pth, &agi );
	sam  = pth == ( Nul_Fld(inf->rsv->bfr,FLD_INT) ? -1: DBS_INT_VAL(inf->rsv->bfr) ); 
	if ( sam && pth==VIA_AGENT )	sam &= !Nul_Fld(inf->rsv->aii,FLD_INT) &&  DBS_INT_VAL(inf->rsv->aii) == agi; 
	if ( !sam && pth != -1 ) {
		eti = ((CEquipmentTypeCombo *)rtc)->GetType( &avl );
		Viw_Scb_Eqt( rtc, pth, agi, DBS_DAY_PTR(dtl->dtl->arv), DBS_DAY_PTR(dtl->dtl->dpt), FALSE, wnd, dbs, mng );
		if ( eti )	((CEquipmentTypeCombo *)rtc)->SetType( eti );
		rte_pln( dtl, wnd, dbs, mng );
	}
	Set_Nul(inf->rsv->bfr, FLD_INT, pth==-1);
	*DBS_INT_PTR(inf->rsv->bfr) = pth;
	Set_Nul(inf->rsv->aii, FLD_INT, pth!=VIA_AGENT);
	*DBS_INT_PTR(inf->rsv->aii) = agi;

	dtl->est = dtl_est( inf->rsv, dtl->dtl, dtl->smr, mng );
	dtl_enb( dtl, wnd );
}

void	Rsv_Rct_Chg(					// Room number editbox change 
int			rct,						// Room number from editbox
CWnd		*wnd,						// Parent window
PMS_MNG		*mng )						// Property management System management structure
{
	int			crc, exc=0;
	Csr			csr=NULL, rot=RSV_INF_(wnd)->dtl;
	WCHAR		buf[81];
	RSV_DTL_DAT	*dtl, *src;
	CSimpleGrid	*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL));

	if ( !!grd && rct>0){
		if ( ( crc=grd->GetLineCount( FALSE ) ) > rct ){
			while( csr=Nxt_Lnk(rot, csr) ){
				dtl = Rec_Of(RSV_DTL_DAT, csr);
				if ( !Nul_Fld(dtl->dtl->id,FLD_INT) )	exc++;
			}
			if ( exc <= rct ){
				swprintf_s( buf, memcnt(buf), _T("Are you sure you want to delete the lower %d rows?"), crc-rct );
				if ( wnd->MessageBox( buf, NULL, MB_ICONQUESTION|MB_YESNO )==IDYES ){
					while( crc > rct )	grd->DelRow( crc-- );
					grd->Redraw();
				}
			}
			else{
				wnd->MessageBox( _T("Because include some exisitng datas it impossible to delete"), NULL, MB_ICONSTOP|MB_OK );
				swprintf_s( buf, memcnt(buf), _T("%d"), crc );
				wnd->GetDlgItem(IDC_EDIT_ROOMNUMBER)->SetWindowText( buf );
			}
		}
		else if ( crc < rct ){
			while( crc++ < rct ){
				if ( !!( csr = Add_Lnk( rot, NULL, sizeof(RSV_DTL_DAT) ) ) ){
					dtl = Rec_Of(RSV_DTL_DAT, csr);
					csr = Prv_Lnk(rot, csr);			// Set default data from 1 upper line
					src = Rec_Of(RSV_DTL_DAT, csr);
					dtl_def( dtl, DBS_DAY_PTR(src->dtl->arv), DBS_DAY_PTR(src->dtl->dpt), OPE_COD_(wnd) );
					if ( grd->AddRow( 1 ) ){
						grd->Redraw();
						dsp_dtg( crc, dtl, wnd, mng );
					}
					else{
						Del_Lnk( rot, csr );
					}
				}
			}
		}
		fst_lst( RSV_INF_(wnd), wnd );
	}
}

void		Rsv_Chk_All(					// CheckAll checkbox click
CWnd			*wnd,						// Parent window
PMS_MNG			*mng )						// Hotel Property Management Structure
{
	RSV_DTL_DAT	*dtl;
	int			row;
	int			chk = ((CButton *)(wnd->GetDlgItem(IDC_CHECK_CHECKALL)))->GetCheck();
	CSimpleGrid	*grd = (CSimpleGrid *)((CReservationView *)wnd)->GetDlgItem(IDC_GRD_DTL);

	for ( row=1; row<=grd->GetLineCount( FALSE ); row++ ){
		if ( !!( dtl = (RSV_DTL_DAT *)(LONG_PTR)grd->GetData( RSV_DAT_CLM, row ) ) ){
			grd->SetCheck( RSV_CHK_CLM, row, dtl->cxl = (chk & !(dtl->est&NO_RSV_CXL)) && !Nul_Fld(dtl->dtl->id, FLD_INT) );
		}
	}
}

int		Rsv_Chg_Dtl(				// Change the selecting detail of detail grid
int				row,				// selected row
CWnd			*wnd,				// Parent window
CDatabase		*dbs,				// ODBC Database object
PMS_MNG			*mng )				// Hotel Property Management Structure
{
	RSV_DTL_DAT	*dtl;
	CSimpleGrid	*grd=((CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL)));
	int			ret=TRUE;

	if ( row == 0 )		SEL_ROW_(wnd)=-1;

	if ( SEL_ROW_(wnd) != row && SEL_ROW_(wnd) != -1 ){
		// Save current detail data
		dtl =  (RSV_DTL_DAT *)(LONG_PTR)grd->GetData( RSV_DAT_CLM, SEL_ROW_(wnd) );
		if ( !( NO_NED_SAV & dtl->est ) ){
			if( ret = get_dtl( dtl, wnd, mng ) ){
				dsp_dtg( SEL_ROW_(wnd), dtl, wnd, mng );
			}
		}
		if ( ret ){
			SEL_ROW_(wnd) = row;
			// Change to new selected data
			dtl =  (RSV_DTL_DAT *)(LONG_PTR)grd->GetData( RSV_DAT_CLM, row );
			if ( dtl ){
				dsp_dtl( RSV_INF_(wnd), dtl, wnd, dbs, mng );
			}
		}
		fst_lst( RSV_INF_(wnd), wnd );
	}
	return( ret );
}

int		Rsv_Chk_Clk(					// cancellation checkbox check
int				row,					// selected row
CWnd			*wnd,					// Parent window
CDatabase		*dbs,					// ODBC Database object
PMS_MNG			*mng )					// Hotel Property Management Structure
{
	RSV_DTL_DAT	*dtl;
	CSimpleGrid	*grd=((CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL)));
	int			chk;

	chk = !grd->GetCheck( RSV_CHK_CLM, row );
	if ( !!( dtl = (RSV_DTL_DAT *)(LONG_PTR)grd->GetData( RSV_DAT_CLM, row ) ) ){
		if ( chk )	chk &= !(dtl->est&NO_RSV_CXL) && !Nul_Fld(dtl->dtl->id, FLD_INT);
		grd->SetCheck( RSV_CHK_CLM, row, chk );
		dtl->cxl = chk;
	}

	return( TRUE );
}

void	Rsv_Sam_Rsv(					// Same as reserved person checkbox click event
CWnd		*wnd,						// Parent window
CDatabase	*dbs,						// Database object
PMS_MNG		*mng )						// Property management System management structure
{

	RSV_DTL_DAT	*dtl;
	int			chk=((CButton *)(wnd->GetDlgItem(IDC_CHECK_SAMERESERVATION)))->GetCheck();
	CSimpleGrid	*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL));

	if ( SEL_ROW_(wnd) != -1 ){
		if ( !!( dtl =  (RSV_DTL_DAT *)(LONG_PTR)grd->GetData( RSV_DAT_CLM, SEL_ROW_(wnd) ) ) ){
			// if diferrent with current status
			if ( dtl->smr != chk ){
				dtl->smr = chk;
				if ( chk ){
					WIN_TXT_DBS(dtl->cli->fnm, IDC_EDIT_FIRSTNAME, wnd);
					WIN_TXT_DBS(dtl->cli->lnm, IDC_EDIT_FAMILYNAME, wnd);
					DBS_WST_CPY(dtl->cli->lnm, RSV_INF_(wnd)->cli->lnm );
					Set_Nul(dtl->dtl->cii,FLD_INT,TRUE);
				}
				else{
					Set_Nul(dtl->dtl->cii,FLD_INT,FALSE);
					Set_Nul(dtl->cli->fnm,FLD_WST,FALSE);
					Set_Nul(dtl->cli->lnm,FLD_WST,FALSE);
				}
				dsp_dtl( RSV_INF_(wnd), dtl, wnd, dbs, mng );
			}
		}
	}
}
/*
void	Rsv_Sty_Chg(					// Stay period change event
RSV_DTL_DAT	*dtl,						// reservation detail
CWnd		*wnd,						// Target window
CDatabase	*dbs,						// ODBC Database object
PMS_MNG		*mng )						// Property management System management structure
{
	WCHAR		*msg=NULL, buf[128];
	Day			arv[1], dpt[1];
	int			dys;
	CSimpleGrid	*grd = (CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_ROM));

	if ( !(NO_NED_SAV & dtl->est ) ){
		if ( ( dys = get_spd( arv, dpt, mng->bdt, dtl->est, FALSE, wnd ) ) != -1 ){
			if ( Cmp_Dte( arv, dpt ) <= 0 ){
				dys = Clc_Dys( arv, dpt );
				swprintf_s( buf, memcnt(buf), _T("%d"), dys );
				wnd->GetDlgItem(IDC_EDIT_NIGHTS)->SetWindowText( buf );
				Rsv_Ngt_Chg( dys, dtl, wnd, dbs, mng );
				fst_lst( RSV_INF_(wnd), wnd );
			}
		}
		if ( msg )	wnd->MessageBox( msg, NULL, MB_ICONEXCLAMATION|MB_OK );
	}
}
*/

void	Rsv_Sel_Clk(				// Client kind select event
CWnd			*wnd )				// ReservationView
{
	CReservationView	*viw = (CReservationView *)wnd;
	int					knd;
	
	knd = ((CButton *)(viw->GetDlgItem(IDC_RADIO_PERSONAL)))->GetCheck() ? CLK_PERSONAL: CLK_GROUP;
	viw->GetDlgItem(IDC_CHECK_SAMERESERVATION)->EnableWindow( knd==CLK_PERSONAL );
	if ( knd==CLK_GROUP ){
		((CButton *)(viw->GetDlgItem(IDC_CHECK_SAMERESERVATION)))->SetCheck( FALSE );
		viw->OnBnClickedCheckSamereservation();
	}
}

/*
void	Rsv_Ngt_Chg(					// Change the night editbox event
int				ngt,					// inputted night number
RSV_DTL_DAT		*dtl,					// selecting reservation_details 
CWnd			*wnd,					// Target window
CDatabase		*dbs,					// ODBC Database object
PMS_MNG			*mng )					// HOtel Property Management System Structure
{
	static	int	prc=FALSE;
	WCHAR		buf[20];
	RSV_RCG_DAT	*rcg;
	Day			nsd[1];
	int			dys, idx, val, ont;
	Csr			csr;
	CSimpleGrid	*grd = (CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_ROM));

	if ( !prc && !(NO_NED_SAV & dtl->est ) ){
		prc = TRUE;
		ont = Clc_Dys( DBS_DAY_PTR(dtl->dtl->arv), DBS_DAY_PTR(dtl->dtl->dpt) );
		if ( ont != ngt ){
			if ( get_spd( DBS_DAY_PTR(dtl->dtl->arv), DBS_DAY_PTR(dtl->dtl->dpt), mng->bdt, dtl->est, TRUE, wnd ) != -1 ){
				*nsd = *DBS_DAY_PTR(dtl->dtl->arv);
				Add_Dte( nsd, dys = ngt );
				*DBS_DAY_PTR(dtl->dtl->dpt) = *nsd;
				rte_pln( dtl, wnd, dbs, mng );
				if ( Cmp_Dte( nsd, mng->bdt ) >= 0 ){
					wnd->GetDlgItem(IDC_EDIT_DEPARTUREDATE)->SetWindowText( Edt_Dte( buf, memcnt(buf), nsd ) );
					((CSpinButtonCtrl *)wnd->GetDlgItem(IDC_SPIN_ROOMCHANGE))->SetRange(0,dys-1);
					// Set room chhange number if that is greater than nights
					wnd->GetDlgItem(IDC_EDIT_ROOMCHANGE)->GetWindowText( buf, memcnt(buf) );
					if ( _wtoi(buf) > dys ){
						swprintf_s( buf, memcnt(buf), _T("%d"), dys-1 );
						wnd->GetDlgItem(IDC_EDIT_ROOMCHANGE)->SetWindowText( buf );
					}
					set_req( grd, dtl, DBS_DAY_PTR(dtl->dtl->arv), nsd, wnd, dbs, mng );
					if ( ( dys = Clc_Dys( DBS_DAY_PTR(dtl->dtl->arv), nsd ) - Cnt_Lnk( dtl->rcg ) ) > 0 ){
						Add_Dte( nsd, -dys );
						for ( idx=0; idx<dys; idx++ ){
							if ( !!( csr = Add_Lnk( dtl->rcg, NULL, sizeof(RSV_RCG_DAT) ) ) ){
								rcg = Rec_Of(RSV_RCG_DAT, csr);
								DBS_WST_SET(rcg->rcg->opc, OPE_COD_(wnd));
								DBS_INT_SET(rcg->rcg->pup, &(val = 0) );
								DBS_DAY_SET(rcg->rcg->udt, nsd );
								set_rcg( rcg, dtl, wnd, mng );
								Add_Dte( nsd, 1 );
							}
						}
					}
					else{
						// remaind the old data because for acutual amount keep
					}
				}
				else{
					dys = Clc_Dys( DBS_DAY_PTR(dtl->dtl->arv), DBS_DAY_PTR(dtl->dtl->dpt) );
					swprintf_s( buf, memcnt(buf), _T("%d"), dys );
					wnd->GetDlgItem(IDC_EDIT_NIGHTS)->SetWindowText( buf );
				}
				fst_lst( RSV_INF_(wnd), wnd );
			}
		}
		prc = FALSE;
	}
}
*/

void		Rsv_Rtp_Chg(				// Requested room type combobox change
RSV_DTL_DAT	*dtl,						// reservation_detail
CWnd		*wnd,						// Parent window
CDatabase	*dbs,						// ODBC Database object
PMS_MNG		*mng )						// Hotel Property management System Structure
{
	int					eti, avl;
	CEquipmentTypeCombo	*cbx = (CEquipmentTypeCombo *)(wnd->GetDlgItem(IDC_COMBO_ROOMTYPE));

	eti = cbx->GetType( &avl );
	if ( !!eti && !avl ){
		wnd->MessageBox( _T("This room type has not price setting"), NULL, MB_ICONEXCLAMATION|MB_OK );
	}
	if ( get_spd( DBS_DAY_PTR(dtl->dtl->arv), DBS_DAY_PTR(dtl->dtl->dpt), mng->bdt, dtl->est, TRUE, wnd ) != -1 ){
		rte_pln( dtl, wnd, dbs, mng );
	}
}

void	Rsv_Sel_Pln(					// rate plan combobox change event
RSV_DTL_DAT		*dtl,					// target reservation_detail data
CWnd			*wnd,					// Parent window
CDatabase		*dbs,					// ODBC Database object
PMS_MNG			*mng )					// Hotel Property Management Structure
{
	int			row=1;
	RSV_RCG_DAT	*rcg;
	RSV_EQP_DAT	*req;
	Csr			csr=NULL;
	CSimpleGrid	*grd = (CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_ROM));

	while( csr=Nxt_Lnk( dtl->rcg, csr ) ){
		rcg = Rec_Of(RSV_RCG_DAT, csr);
		set_rcg( rcg, dtl, wnd, mng );
	}
	while( csr = Nxt_Lnk( dtl->req, csr ) ){
		req = Rec_Of(RSV_EQP_DAT, csr);
		dsp_rmd( grd, req, dtl, row++ );
	}
}

void	Rsv_Rom_Chg(					// Change the room change times editbox event
RSV_DTL_DAT	*dtl,						// Reservation detail data
CWnd		*wnd,						// Target window
CDatabase	*dbs,						// ODBC Database object
PMS_MNG		*mng )						// HOtel Property Management System Structure
{
	int			idx, nct, cct, row, dys;
	WCHAR		buf[20];
	RSV_EQP_DAT	*req, *prv;
	Day			arv[1], dpt[1];
	Csr			csr;
	CSimpleGrid	*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_ROM));

	if ( !(NO_NED_SAV & dtl->est ) ){
		if ( get_spd( arv, dpt, NULL, dtl->est, TRUE, wnd ) != -1 ){		// Getting stay period fro screen
			dys = rcg_cnt( arv, dpt, mng );
			rcg_lmt( dys, wnd );
			wnd->GetDlgItem(IDC_EDIT_ROOMCHANGE)->GetWindowText( buf, memcnt(buf) );
			nct = _wtoi( buf );
			if ( nct > ( cct = Cnt_Lnk( dtl->req ) - 1 ) ){
				if ( nct <= dys ){
					for ( idx=0; idx<(nct-cct); idx++ ){
						if ( csr = Add_Lnk( dtl->req, NULL, sizeof(RSV_EQP_DAT) ) ){
							req = Rec_Of(RSV_EQP_DAT, csr);
							prv = (RSV_EQP_DAT *)(LONG_PTR)grd->GetData( 0, grd->GetLineCount( FALSE ) );
							*req = *prv;
							*req->rno = 0;
							*req->tnm = 0;
							Set_Nul(req->req->eii,FLD_INT,TRUE);		// equipment_info_id
							Set_Nul(req->req->eti,FLD_INT,TRUE);		// equipment_type_id 
							Set_Nul(req->req->lud,FLD_INT,TRUE); 		// last_used_date
							*DBS_INT_PTR(req->req->inh) = FALSE;; 
							DBS_DAY_CPY(req->req->usd, req->req->ued);
							Add_Dte( DBS_DAY_PTR(req->req->usd), -1 );
							DBS_DAY_CPY(prv->req->ued, req->req->usd);
							grd->AddRow( 1 );
							grd->SetData( 0, grd->GetLineCount( FALSE ), (int)(LONG_PTR)req );
							dsp_rmd( grd, req, dtl, grd->GetLineCount( FALSE ) );
							dsp_rmd( grd, prv, dtl, grd->GetLineCount( FALSE )-1 ); 
							for ( row=grd->GetLineCount( FALSE )-1; row>1 ; row-- ){
								req = (RSV_EQP_DAT *)(LONG_PTR)grd->GetData( 0, row );
								prv = (RSV_EQP_DAT *)(LONG_PTR)grd->GetData( 0, row-1 );
								if ( !Cmp_Dte( DBS_DAY_PTR(req->req->usd), DBS_DAY_PTR(req->req->ued) ) ){
									Add_Dte( DBS_DAY_PTR(req->req->usd), -1);
									DBS_DAY_CPY(prv->req->ued, req->req->usd);
									dsp_rmd( grd, req, dtl, row );
									dsp_rmd( grd, prv, dtl, row-1 );
								}
							}

						}
					}
					grd->Redraw();
				}
				else{
					swprintf_s( buf, memcnt(buf), _T("%d"), cct );
					wnd->GetDlgItem(IDC_EDIT_ROOMCHANGE)->SetWindowText( buf );
				}
			}
			else if ( cct > nct ){
				while( cct > nct ){
					req = (RSV_EQP_DAT *)(LONG_PTR)grd->GetData( 0, cct+1 );
					prv = (RSV_EQP_DAT *)(LONG_PTR)grd->GetData( 0, cct );
					DBS_DAY_CPY(prv->req->ued, req->req->ued);
					cfm_avl( prv, wnd, dbs, mng );
					dsp_rmd( grd, prv, dtl, cct);
					grd->DelRow( cct + 1 );
					//Del_Lnk( dtl->req, csr );
					del_req( dtl->req, ((Csr)req)-1 );
					--cct;
				}
				grd->Redraw();
			}
		}
	}
}

void	Rsv_Rom_Dte(					// Room use date changed
WCHAR		*txt,						// inputted text
int			clm,						// column on the grid
int			row,						// row on the grid
RSV_DTL_DAT	*dtl,						// pointer of reservation_details
CWnd		*wnd,						// Parent window
CDatabase	*dbs,						// ODBC Database object
PMS_MNG		*mng )						// Hotel Property management System Structure
{
	int			chk, cnt, dys, orw;
	WCHAR		*msg=NULL;
	RSV_EQP_DAT	*req, *otr=NULL;
	Day			idt[1], odt[1], arv[1], dpt[1];
	CSimpleGrid	*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_ROM));

	if ( !Str_Dte( idt, txt ) ){
		msg = _T("Inputted date is wrong");
	}
	else{
		if ( !(NO_NED_SAV & dtl->est ) ){
			if ( chk = ( dys = get_spd( arv, dpt, NULL, dtl->est, TRUE, wnd ) ) != -1 ){		// Getting stay period fro screen
				if ( !!( cnt = Cnt_Lnk( dtl->req ) ) ){
					req = (RSV_EQP_DAT *)(LONG_PTR)grd->GetData( 0, row );
					*odt = *DBS_DAY_PTR(clm==ROM_USD_CLM ? req->req->usd: req->req->ued);
					if ( !!Cmp_Dte( odt, idt ) ){
						if( row==1 && clm==ROM_USD_CLM ){
							msg = _T("Can not change the date which start of period");
						}
						else if ( row==cnt && clm==ROM_UED_CLM ){
							msg = _T("Can not change date which end of period");
						}
						else if ( Cmp_Dte( idt, mng->bdt ) < 0 ){
							msg = _T("Can not change date to less than today");
						}
						else{
							if ( Cmp_Dte( idt, odt ) < 0 ){			// Inputed date is less than existied date
								if ( clm==ROM_USD_CLM ){						// In use case of start date 
									otr = (RSV_EQP_DAT *)(LONG_PTR)grd->GetData( 0, orw = row-1 );
									if ( chk = Cmp_Dte( DBS_DAY_PTR(otr->req->usd), idt) < 0 ){
										*DBS_DAY_PTR(otr->req->ued) = *idt;
										*DBS_DAY_PTR(req->req->usd) = *idt;
										grd->SetCellText( ROM_UED_CLM, orw, txt );
									}
								}
								else{								// In case of end date
									if ( row < grd->GetLineCount(FALSE) ){
										otr = (RSV_EQP_DAT *)(LONG_PTR)grd->GetData( 0, orw = row+1 );
									}
									if ( chk = Cmp_Dte( DBS_DAY_PTR(req->req->usd), idt) < 0 ){
										*DBS_DAY_PTR(req->req->ued) = *idt;
										if ( otr ){
											*DBS_DAY_PTR(otr->req->usd) = *idt;	
											grd->SetCellText( ROM_USD_CLM, orw, txt );
										}
									}
								}
							}
							else{									// Inputed date is greater than existied date
								if ( clm==ROM_USD_CLM ){
									if ( row > 1 ){
										otr = (RSV_EQP_DAT *)(LONG_PTR)grd->GetData( 0, orw = row-1 );
									}
									if ( chk = Cmp_Dte( DBS_DAY_PTR(req->req->ued), idt) > 0 ){
										*DBS_DAY_PTR(req->req->usd) = *idt;
										if ( otr ){
											*DBS_DAY_PTR(otr->req->ued) = *idt;	
											grd->SetCellText( ROM_UED_CLM, orw, txt );
										}
									}
								}
								else{
									otr = (RSV_EQP_DAT *)(LONG_PTR)grd->GetData( 0, orw = row+1 );
									if ( chk = Cmp_Dte( DBS_DAY_PTR(otr->req->ued), idt) > 0 ){
										*DBS_DAY_PTR(otr->req->usd) = *idt;
										*DBS_DAY_PTR(req->req->ued) = *idt;
										grd->SetCellText( ROM_USD_CLM, orw, txt );
									}
								}
							}
							if ( chk ){
								if ( !cfm_avl( otr, wnd, dbs, mng ) )	dsp_rmd( grd, otr, dtl, orw );
								if ( !cfm_avl( req, wnd, dbs, mng ) )	dsp_rmd( grd, req, dtl, row );
							}
							if ( chk )	grd->SetCellText( clm, row, txt );
							else		msg = _T("Date change impossible because date conflict occurred");
						}
					}
				}
			}
		}
	}
	if ( msg )	wnd->MessageBox( msg, NULL, MB_ICONEXCLAMATION|MB_OK );
}

void		Rsv_Rom_Nbr(				// Room number edit complete event
WCHAR		*txt,						// inputted text
int			row,						// row on the grid
CWnd		*wnd,						// Parent window
CDatabase	*dbs,						// ODBC Database object
PMS_MNG		*mng )						// Hotel Property management System Structure
{
	int			avl;
	Csr			csr=NULL;
	RSV_EQP_EQS	*eqi;
	EQP_TYP		*eqt;
	RSV_EQP_DAT	*req;
	WCHAR		*erm=NULL, *err=NULL;
	CSimpleGrid	*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_ROM));
	Day			*usd, *ued;

	req = (RSV_EQP_DAT *)(LONG_PTR)grd->GetData( 0, row );

	if ( !!wcslen( txt ) ){
		while( csr=Nxt_Lnk( EQP_ISI_(wnd), csr ) ){
			eqi = Rec_Of(RSV_EQP_EQS, csr);
			if ( !wcscmp( DBS_WST_PTR(eqi->eqi->eco), txt ) ){
				break;
			}
		}
		if ( !!csr ){			// found!
			for( csr=NULL; csr=Nxt_Lnk( mng->etl, csr ); ){
				eqt = Rec_Of(EQP_TYP, csr);
				if ( DBS_INT_VAL(eqt->id)==DBS_INT_VAL(eqi->eqi->eti) ){
					break;
				}
			}
			eqt = !!csr ? eqt: NULL;
			usd=DBS_DAY_PTR(req->req->usd);
			ued=DBS_DAY_PTR(req->req->ued);
			if ( ( avl = rom_avl( req, DBS_INT_VAL(eqi->eqi->id), usd, ued, wnd, dbs, mng ) ) != -1 ){
				if ( avl ){
					DBS_INT_CPY(req->req->eii, eqi->eqi->id);
					//Set_Nul(req->req->eii,FLD_INT,FALSE);
					//*DBS_INT_PTR(req->req->eii) = DBS_INT_VAL(eqi->id);
					DBS_WST_CPY(req->rno, eqi->eqi->eco);
					DBS_INT_CPY(req->esi, eqi->esi);
					if ( eqt ){
						DBS_INT_CPY(req->req->eti, eqt->id);
						//Set_Nul(req->req->eti,FLD_INT, FALSE);
						//*DBS_INT_PTR(req->req->eti)=*DBS_INT_PTR(eqt->id);
						DBS_WST_CPY(req->tnm, eqt->nam);
					}
					grd->SetCellText( ROM_NBR_CLM, row, txt );
					grd->SetCellText( ROM_TYP_CLM, row, DBS_WST_PTR(eqt->nam) );
				}
				else{
					err = _T("Room is not avalable on this period");
				}
			}
		}
		else{
			err = _T("Room number not found");
		}
		if ( err ){
			wnd->MessageBox( err, NULL, MB_ICONEXCLAMATION|MB_OK );
		}
	}
	else{				// room is not avalable
		DBS_INT_SET(req->req->eii, (int *)NULL);
		DBS_WST_SET(req->rno, (WCHAR *)NULL);
		DBS_INT_SET(req->req->eti, (int *)NULL);
		DBS_WST_SET(req->tnm, (WCHAR *)NULL);
		grd->SetCellText( ROM_NBR_CLM, row, _T("") );
		grd->SetCellText( ROM_TYP_CLM, row, _T("") );
	}
}

void	Rsv_Ctx_Mnu(			// Open context menu on search view
CWnd		*wnd,				// Search view
int			msx,				// Mouse point x
int			msy )				// Mouse point y
{
	CMenu		mnu, *pmn=NULL;
	POINT		pnt[1] = { msx, msy };

	mnu.LoadMenu(IDR_MENU_RSV_ROM);
	pmn = mnu.GetSubMenu(0);
	// mouse on the panel
	wnd->ClientToScreen( pnt );
	pmn->TrackPopupMenu( TPM_TOPALIGN/*TPM_LEFTALIGN*/|TPM_RIGHTBUTTON, pnt->x, pnt->y, wnd, NULL );
}

void	Rsv_Crg_Dtl(				// Context menu of charge detail
RSV_DTL_DAT		*dtl,				// target reservation_detail data
int				row,				// Selected row
CWnd			*wnd,				// Parent window
//CDatabase		*dbs,				// ODBC Database object
PMS_MNG			*mng )				// Hotel Property Management Structure
{
	CChargeDetailDlg	*dlg;
	CSimpleGrid			*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_ROM));

	if ( row != -1 ){
		if ( dlg = new CChargeDetailDlg( wnd ) ){
			dlg->m_pPms_mng = mng;
			dlg->m_Dtl_lst = dtl->rcg;
			wnd->GetDlgItem(IDC_COMBO_ROOMTYPE)->GetWindowText( dlg->m_Rom_typ, memcnt(dlg->m_Rom_typ) );
			wnd->GetDlgItem(IDC_COMBO_RATEPLANNAME)->GetWindowText( dlg->m_Pln_nam, memcnt(dlg->m_Pln_nam) );
			if ( dlg->DoModal()==IDOK ){
				dsp_rom( grd, dtl, mng );
			}
			delete dlg;
		}
	}
}

void	Rsv_Rom_Sel(				// Context menu of charge detail
RSV_DTL_DAT		*dtl,				// target reservation_detail data
int				row,				// Selected row
CWnd			*wnd,				// Parent window
//CDatabase		*dbs,				// ODBC Database object
PMS_MNG			*mng )				// Hotel Property Management Structure
{
	int				idx=CTM_ROW_(wnd)-1;
	RSV_INF_DAT		*inf = RSV_INF_(wnd);
	RSV_EQP_DAT		*req;
	CRoomSelectDlg	*dlg;
	CComboBox		*plc = (CComboBox *)(wnd->GetDlgItem(IDC_COMBO_RATEPLANNAME));
	CSimpleGrid		*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_ROM));

	req = Rec_Of(RSV_EQP_DAT, Idx_Lnk( dtl->req, idx ) );
	if ( dlg = new CRoomSelectDlg( wnd ) ){
		dlg->m_Bok_pth = Nul_Fld(inf->rsv->bfr, FLD_INT) ? 0: DBS_INT_VAL(inf->rsv->bfr);
		dlg->m_Agt_iid = Nul_Fld(inf->rsv->aii, FLD_INT) ? 0: DBS_INT_VAL(inf->rsv->aii);
		dlg->m_Req_eti = Nul_Fld(dtl->dtl->eti, FLD_INT) ? 0: DBS_INT_VAL(dtl->dtl->eti);
		dlg->m_Rsv_did = DBS_INT_VAL(dtl->dtl->id);
		dlg->m_Eqp_tid = !!req && Nul_Fld(req->req->eti, FLD_INT) ? 0: DBS_INT_VAL(req->req->eti);
		dlg->m_Equ_iid = !!req && Nul_Fld(req->req->eii, FLD_INT) ? 0: DBS_INT_VAL(req->req->eii);
		dlg->m_Use_std = DBS_DAY_PTR(req->req->usd);
		dlg->m_Use_edd = DBS_DAY_PTR(req->req->ued);
		if ( ( idx = plc->GetCurSel() ) != -1 ){
			plc->GetLBText( idx, dlg->m_Pln_nam );
		}
		if ( dlg->DoModal()==IDOK ){
			DBS_INT_CPY(req->req->eii, dlg->m_Eqp_inf->eqi->id);
			DBS_WST_CPY(req->rno, dlg->m_Eqp_inf->eqi->eco);
			DBS_INT_CPY(req->req->eti, dlg->m_Eqp_inf->eti );
			DBS_INT_CPY(req->esi, dlg->m_Eqp_inf->esi );
			DBS_WST_CPY(req->tnm, dlg->m_Eqp_inf->tnm );
			grd->SetCellText( ROM_NBR_CLM, row, DBS_WST_PTR(dlg->m_Eqp_inf->eqi->eco) );
			grd->SetCellText( ROM_TYP_CLM, row, DBS_WST_PTR(dlg->m_Eqp_inf->tnm) );
		}
		delete	dlg;
	}
}

int		Rsv_Get_Dat(						// Getting the existing data
int				rid,						// resservation_infos id
CWnd			*wnd,						// Parent window
CDatabase		*dbs,						// ODBC Database object
PMS_MNG			*mng )						// Hotel Property Management Structure
{
	WCHAR			*erm=NULL;
	int				chk;
	RSV_INF_DAT		*inf=RSV_INF_(wnd);
	RSV_DTL_DAT		*dtl;
	Csr				csr=NULL;

	SEL_ROW_(wnd) = 1;

	if ( chk = Rsv_Get_Eis( EQP_ISI_(wnd), &erm, dbs, mng ) ){
		if ( chk = Rsv_Get_Inf( inf, rid, &erm, dbs, mng ) ){
			if ( erm )		free( erm );
			if ( chk = Rsv_Get_Dtl( inf, &erm, dbs, mng ) ){
				while ( csr=Nxt_Lnk( inf->dtl,csr ) ){
					dtl = Rec_Of(RSV_DTL_DAT,csr);
					// Judgment on same as reserved person
					dtl->smr  = !Nul_Fld(inf->rsv->cii, FLD_INT) && !Nul_Fld(dtl->dtl->cii,FLD_INT);
					dtl->smr &= DBS_INT_VAL(inf->rsv->cii) == DBS_INT_VAL(dtl->dtl->cii);
				}
				((CChildFrame *)(wnd->GetParent()))->m_Rsv_id = rid;
				dsp_dat( inf, dbs, wnd, mng );
			}
		}
	}
	if ( erm ){
		wnd->MessageBox(erm/*_T("Faild to get a reservation data!")*/, 0, MB_ICONSTOP|MB_OK );
		free( erm );
	}

	return( chk );
}

int		Rsv_Cre_Rsv(					// Create a new reservation
CCI_PRM		*cci,						// Create a reservation parameter
CWnd		*wnd,						// Target window
CDatabase	*dbs,						// Database object
PMS_MNG		*mng )						// Property management System management structure
{
	int			chk, val;
	RSV_INF_DAT	*inf=RSV_INF_(wnd);
	RSV_DTL_DAT	*dtl;
	RSV_EQP_DAT	*req;
	Csr			csr;
	WCHAR		*ptr, *erm=NULL;
	Day			arv[1], dpt[1];

	SEL_ROW_(wnd) = 1;

	Rsv_Dps_Dtl( inf->dtl );	// dispose previous data ( Actually,always nothing previous data because this function is called when just after create view )

	memset( inf, 0, sizeof(RSV_INF_DAT) );

	*arv = !!cci ? *cci->arv: *mng->bdt;
	*dpt = !!cci ? *cci->dpt: *mng->bdt;
	if ( !cci )	Add_Dte(dpt, 1);

	DBS_WST_SET(inf->rsv->opc, OPE_COD_(wnd));
	DBS_WST_SET(inf->rsv->brc, BRC_COD_(wnd));
	DBS_WST_SET(inf->rsv->fcc, FCL_COD_(wnd));
	DBS_DAY_SET(inf->rsv->arv, arv);						// arrival_date
	DBS_DAY_SET(inf->rsv->dpt, dpt);						// departure_date
	DBS_INT_SET(inf->rsv->olr, &(val=0));					// online_reservation
	DBS_INT_SET(inf->rsv->cxl, &(val=0));					// canceled
	DBS_INT_SET(inf->rsv->nsw, &(val=0));					// No Show
	DBS_INT_SET(inf->rsv->cis, &(val=CIS_BEFORE_CHECKIN));	// checkin status
	DBS_WST_SET(inf->cli->opc, OPE_COD_(wnd));				// operator_code of client_infos
	DBS_INT_SET(inf->cli->usc, &(val=0));					// Usage count
	DBS_INT_SET(inf->cli->nsc, &(val=0));					// No Show count
	DBS_INT_SET(inf->cli->crk, &(val=CLL_DEFAULT));			// default of client_rankt

	inf->est = inf_est( inf->rsv, inf->dtl, mng );

	if ( chk = !!( csr = Add_Lnk( inf->dtl, NULL, sizeof(RSV_DTL_DAT) ) ) ){
		dtl = Rec_Of(RSV_DTL_DAT, csr);
		if ( chk = !!( req = dtl_def( dtl, arv, dpt, OPE_COD_(wnd) ) ) ){
			dtl->smr = TRUE;
			if ( cci ){
				DBS_INT_SET(dtl->dtl->eti,&(cci->eti));
				DBS_INT_SET(req->req->eti,&(cci->eti));
				DBS_INT_SET(req->req->eii,&(cci->eii));
				DBS_INT_SET(req->esi,&(cci->esi));
				if ( ptr = Viw_Get_Rtn(mng->etl, cci->eti) ){
					DBS_WST_SET(req->tnm, ptr);
				}
				if ( ptr = Viw_Get_Rmn(mng->eil, cci->eii) ){
					DBS_WST_SET(req->rno, ptr);
				}
			}
		}
		//dtl->est = dtl_est( inf->rsv, dtl->dtl, dtl->smr, mng );
	}

	if ( chk ){
		if ( chk = Rsv_Get_Eis( EQP_ISI_(wnd), &erm, dbs, mng ) ){
			dsp_dat( inf, dbs, wnd, mng );
		}
		else if ( erm ){
			wnd->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
	}

	return( chk );
}

static	void	snd_bst(				// send to BookingStatusView
RSV_INF_DAT			*inf,				// target of send data
int					cre,				// created a new reservation
CWnd				*wnd,				// Parent window
PMS_MNG				*mng )				// Property Management Sysytem structure
{
	BST_ROM_STS		*sts;
	RSV_DTL_DAT		*dtl;
	RSV_EQP_DAT		*req;
	int				cnt=0;
	Lnk				rot[1], *csr=NULL, *cse=NULL, *csn;

	memset( rot, 0, sizeof(rot) );
	while ( csr = Nxt_Lnk( inf->dtl, csr ) ){
		dtl = Rec_Of(RSV_DTL_DAT, csr);
		while( cse = Nxt_Lnk( dtl->req, cse ) ){
			// set data when equipment_infos data is exist
			req = Rec_Of(RSV_EQP_DAT, cse);
			if ( !Nul_Fld(req->req->eii, FLD_INT) ){
				if ( csn = Add_Lnk( rot, NULL, sizeof(BST_ROM_STS) ) ){
					sts = Rec_Of(BST_ROM_STS, csn);
					*(sts->req) = *req->req;
					DBS_INT_CPY(sts->rvn, inf->rsv->rvn);
					DBS_WST_CPY(sts->ern, inf->rsv->ern);
					DBS_INT_CPY(sts->bfr, inf->rsv->bfr);
					DBS_INT_CPY(sts->aii, inf->rsv->aii);
					DBS_INT_CPY(sts->eci, dtl->dtl->eci);
					DBS_INT_CPY(sts->lco, dtl->dtl->lco);
					DBS_INT_CPY(sts->cxl, dtl->dtl->cxl);
					DBS_INT_CPY(sts->esi, req->esi);
					DBS_WST_CPY(sts->fnm, dtl->smr ? inf->cli->fnm: dtl->cli->fnm);
					DBS_WST_CPY(sts->lnm, dtl->smr ? inf->cli->lnm: dtl->cli->lnm);
					DBS_DAY_CPY(sts->arv, dtl->dtl->arv);
					DBS_DAY_CPY(sts->dpt, dtl->dtl->dpt);
					Set_Nul(sts->cin, FLD_INT, FALSE);
					*DBS_INT_PTR(sts->cin) = !Nul_Fld(dtl->dtl->cid, FLD_INT);
					Set_Nul(sts->cot, FLD_INT, FALSE);
					*DBS_INT_PTR(sts->cot) = !Nul_Fld(dtl->dtl->cod, FLD_INT);
					DBS_WST_CPY(sts->rno, req->rno);
					DBS_WST_CPY(sts->rtn, req->tnm);
					cnt++;
				}
				else{
					break;
				}
			}
		}
		if ( !!cse )	break;
	}
	if ( !csr && !!cnt ){			// non error occured when csr is null
		Viw_Snd_Ntf( VIW_SND_BST, cre ? VWN_RSV_CREATED: VWN_RSV_CHANGED, rot, cnt, wnd );	// send message because this with memory free here
	}
	else{
		Del_All( rot );
	}
}


int		 Rsv_Sav_Dat(					// Save reservation data
RSV_INF_DAT			*inf,				// reservation_infos
CWnd				*wnd,				// Parent window
CDatabase			*dbs,				// ODBC Database object
PMS_MNG				*mng )				// Property Management Sysytem structure
{
	int			chk, cre;
	WCHAR		*erm=NULL;
	Day			dte[1];
	RSV_DTL_DAT	*dtl;
	CSimpleGrid	*grd=((CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL)));

	Get_Dte( dte);
	
	if ( !!( dtl =  (RSV_DTL_DAT *)(LONG_PTR)grd->GetData( RSV_DAT_CLM, SEL_ROW_(wnd) ) ) ){
		if ( !( NO_NED_SAV & dtl->est ) ){
			if ( chk = get_inf( inf, wnd, mng ) ){
				cre = !!Nul_Fld(inf->rsv->id, FLD_INT);
				if( chk = get_dtl( dtl, wnd, mng ) ){
					if ( chk = Rsv_Rsv_Sav( &erm, inf, dte, dbs, mng ) ){
					}
					else{
						if ( erm ){
							wnd->MessageBox( erm, 0, MB_ICONEXCLAMATION|MB_OK );
							free( erm );
						}
					}
				}
			}
			if ( chk ){
				snd_bst( RSV_INF_(wnd), cre, wnd, mng );
				Viw_Pst_Ntf( VIW_SND_SCH, cre ? VWN_RSV_CREATED: VWN_RSV_CHANGED, NULL, DBS_INT_VAL(inf->rsv->id), wnd );
				Viw_Pst_Ntf( VIW_SND_NAD, cre ? VWN_RSV_CREATED: VWN_RSV_CHANGED, NULL, DBS_INT_VAL(inf->rsv->id), wnd );
			}
		}
	}
	return( chk );
}

void	Rsv_Dps_Dtl(				// resrvation_details dispose
Csr			rot )					// root of reservation_details
{
	RSV_DTL_DAT	*dtl;

	Csr			csr=NULL, cse=NULL;

	while( csr=Nxt_Lnk(rot, csr) ){
		dtl = Rec_Of(RSV_DTL_DAT, csr);
		Del_All( dtl->rcg );
		Del_All( dtl->req );
	}
	Del_All( rot );
}

void		Rsv_Dps_Rte(				// dispose the rate plan
Csr			rot )						// root of rate plan
{
	Csr		csr;

	csr = Nxt_Lnk( rot, NULL );
	while ( csr ){
		Del_All( Rec_Of(ECG_PLN_DAT, csr)->dtl );
		csr = Del_Lnk( rot, csr );
	}
}

void	Cpy_Edt(						// Copy edit data to detail on same reservation
CCustomEdit	*dst,						// destination edit pointer
CCustomEdit *src )						// source edit pointer
{
	WCHAR		buf[256];

	if ( !!src )	src->GetWindowText( buf, memcnt(buf) );
	else			*buf = 0;
	dst->SetWindowText( buf );
}

//=================================================================================================================

static	WCHAR	*get_pdd(				// Getting and checking a period date
Day				*dst,					// destination pointer of getted date
WCHAR			*idt,					// inputted data
RSV_DTL_DAT		*dtl,					// selecting reservation_details 
PMS_MNG			*mng )					// HOtel Property Management System Structure
{
	WCHAR		*fmt=NULL;

	if ( Str_Dte( dst, idt ) ){
		if ( Cmp_Dte( dst, mng->bdt ) < 0 ){
			fmt = _T("%s should be same or greater than today");
		}
	}
	else{
		fmt =_T("%s is wrong");
	}

	return( fmt );
}

static	int	get_pod(					// getting stay period data
Day				*arv,					// arrival date
Day				*dpt,					// departure date
int				knd,					// change kind
RSV_DTL_DAT		*dtl,					// selecting reservation_details 
CWnd			*wnd,					// Target window
PMS_MNG			*mng )					// HOtel Property Management System Structure
{
	int			chk, ngt;
	WCHAR		buf[192], *itm=NULL, *msg=NULL, *fmt=NULL;

	switch( knd ){
	case RSV_CGK_ARV:	wnd->GetDlgItem(IDC_EDIT_ARRIVALDATE)->GetWindowText( buf, memcnt(buf) );
						if( chk = !( fmt=get_pdd( arv, buf, dtl, mng ) ) ){
							//*DBS_DAY_PTR(dtl->dtl->arv) = *arv;
							*dpt = *DBS_DAY_PTR(dtl->dtl->dpt);
							ngt = Clc_Dys( arv, dpt );
						}
						else{
							itm = _T("ArrivalDate");
							wnd->GetDlgItem(IDC_EDIT_ARRIVALDATE)->SetWindowText(
								Edt_Dte(buf, memcnt(buf), DBS_DAY_PTR(dtl->dtl->arv) ) );
						}
						break;
	case RSV_CGK_DPT:	wnd->GetDlgItem(IDC_EDIT_DEPARTUREDATE)->GetWindowText( buf, memcnt(buf) );
						if( chk = !( fmt=get_pdd( dpt, buf, dtl, mng ) ) ){
							//*DBS_DAY_PTR(dtl->dtl->dpt) = *dpt;
							*arv = *DBS_DAY_PTR(dtl->dtl->arv);
							ngt = Clc_Dys( arv, dpt );
						}
						else{
							itm = _T("DepartureDate");
							wnd->GetDlgItem(IDC_EDIT_DEPARTUREDATE)->SetWindowText(
								Edt_Dte(buf, memcnt(buf), DBS_DAY_PTR(dtl->dtl->dpt) ) );
						}
						break;
	case RSV_CGK_NGT:	wnd->GetDlgItem(IDC_EDIT_NIGHTS)->GetWindowText( buf, memcnt(buf) );
						ngt = _wtoi( buf );
						*arv = *dpt = *DBS_DAY_PTR(dtl->dtl->arv);
						Add_Dte( dpt, ngt );
						break;
	default:			chk=FALSE;
						msg = _T("System error");
						break;
	}
	if ( chk ){
//		swprintf_s( buf, memcnt(buf), _T("%d"), ngt );
//		wnd->GetDlgItem(IDC_EDIT_NIGHTS)->SetWindowText( buf );
	}
	else{
		if ( fmt ){
			swprintf_s( buf,memcnt(buf), fmt, itm ? itm: _T("?????") );
			msg = buf;
		}
		if ( msg ){
			wnd->MessageBox( msg, NULL, MB_ICONEXCLAMATION|MB_OK );
		}
	}

	return( chk ? ngt: -1 );
}

static	int	rmk_rcg(					// remake reservation_charges
RSV_DTL_DAT		*dtl,					// selecting reservation_details 
int				ngt,					// night count
Day				*arv,					// arrival_date
CWnd			*wnd,					// Target window
PMS_MNG			*mng )					// HOtel Property Management System Structure
{
	int			chk, idx, val;
	Lnk			rot[1], *csr=NULL, *cse;
	Day			dte[1];
	RSV_RCG_DAT	*rcg, *old;

	memset( rot, 0, sizeof(rot) );
	for ( idx=0, *dte=*arv; idx<ngt; idx++ ){
		if ( chk = !!( csr = Add_Lnk( rot, NULL, sizeof(RSV_RCG_DAT) ) ) ){
			rcg = Rec_Of(RSV_RCG_DAT, csr);
			DBS_WST_SET(rcg->rcg->opc, OPE_COD_(wnd));
			DBS_INT_SET(rcg->rcg->pup, &(val = 0) );
			DBS_DAY_SET(rcg->rcg->udt, dte );
			set_rcg( rcg, dtl, wnd, mng );
			Add_Dte( dte, 1 );
		}
	}

	for ( csr=NULL; csr = Nxt_Lnk( dtl->rcg, csr ); ){
		old = Rec_Of(RSV_RCG_DAT, csr);
		for ( cse=NULL; cse=Nxt_Lnk( rot, cse ); ){
			rcg = Rec_Of(RSV_RCG_DAT, cse);
			if ( !Cmp_Dte( DBS_DAY_PTR(old->rcg->udt), DBS_DAY_PTR(rcg->rcg->udt) ) ){
				DBS_DBL_CPY(rcg->rcg->amt, old->rcg->amt);
				break;
			}
		}
	}

	Del_All( dtl->rcg );
	*dtl->rcg = *rot;

	return( chk );
}

int		Rsv_Pod_Chg(					// change the stay period
int				knd,					// change kind
RSV_DTL_DAT		*dtl,					// selecting reservation_details 
CWnd			*wnd,					// Target window
CDatabase		*dbs,					// ODBC Database object
PMS_MNG			*mng )					// HOtel Property Management System Structure
{
	static	int		prc=FALSE;
	int				chk=TRUE, ngt/*, idx, val, cnt*/;
//	Csr				csr;
	WCHAR			buf[40], *itm=NULL;
	Day				arv[1], dpt[1];
//	RSV_RCG_DAT		*rcg;
	CSimpleGrid		*grd = (CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_ROM));

	if ( !prc && !(NO_NED_SAV & dtl->est ) ){
		prc = TRUE;		// To prevent dual calling
		if ( chk = ( ngt = get_pod( arv, dpt, knd, dtl, wnd, mng ) ) != -1 ){
			if ( !!Cmp_Dte(arv, DBS_DAY_PTR(dtl->dtl->arv)) || !!Cmp_Dte(dpt, DBS_DAY_PTR(dtl->dtl->dpt)) ){
				if ( Cmp_Dte( arv, dpt ) <= 0 ){
					*DBS_DAY_PTR(dtl->dtl->arv) = *arv;
					*DBS_DAY_PTR(dtl->dtl->dpt) = *dpt;
					wnd->GetDlgItem(IDC_EDIT_ARRIVALDATE)->SetWindowTextW( Edt_Dte(buf, memcnt(buf), arv) );
					wnd->GetDlgItem(IDC_EDIT_DEPARTUREDATE)->SetWindowTextW( Edt_Dte(buf, memcnt(buf), dpt) );
					swprintf_s( buf, memcnt(buf), _T("%d"), ngt );
					wnd->GetDlgItem(IDC_EDIT_NIGHTS)->SetWindowText( buf );
					rte_pln( dtl, wnd, dbs, mng );			// becase it may confrict to valid_sale_period
					((CSpinButtonCtrl *)wnd->GetDlgItem(IDC_SPIN_ROOMCHANGE))->SetRange(0,ngt-1);	// possible count of room change
					// Set room chhange number if that is greater than nights
					wnd->GetDlgItem(IDC_EDIT_ROOMCHANGE)->GetWindowText( buf, memcnt(buf) );
					if ( _wtoi(buf) > ngt ){
						swprintf_s( buf, memcnt(buf), _T("%d"), ngt-1 );
						wnd->GetDlgItem(IDC_EDIT_ROOMCHANGE)->SetWindowText( buf );	// room change edit change event will happen 
					}
					set_req( grd, dtl, DBS_DAY_PTR(dtl->dtl->arv), dpt, wnd, dbs, mng );
					if ( !( chk = rmk_rcg( dtl, ngt, arv, wnd, mng ) ) ){
						wnd->MessageBox( _T("System error. Insuffcient memory."), NULL, MB_ICONSTOP|MB_OK );
					}
					fst_lst( RSV_INF_(wnd), wnd );
				}
				else{
				}
			}
		}
		prc=FALSE;
	}

	return( chk );
}

int			Rsv_Cxl_Exc(
CWnd			*wnd,					// Target window
CDatabase		*dbs,					// ODBC Database object
PMS_MNG			*mng )					// HOtel Property Management System Structure
{
	int				chk=TRUE, cnt=0, cre, val;
	WCHAR			*erm=NULL;
	Lnk				rot[1], *csr=NULL, *cse;
	RSV_INF_DAT		*inf = RSV_INF_(wnd);
	RSV_DTL_DAT		*dtl;
	CSimpleGrid		*grd = (CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL));

	memset( rot, 0, sizeof(rot) );

	while ( csr = Nxt_Lnk( inf->dtl, csr ) ){
		dtl = Rec_Of(RSV_DTL_DAT, csr);
		if ( dtl->cxl ){
			if ( chk = !!( cse = Add_Lnk( rot, NULL, sizeof(int) ) ) ){
				*Rec_Of(int, cse) = DBS_INT_VAL(dtl->dtl->id);
				cnt++;
			}
		}
	}

	if ( chk && cnt ){
		while ( csr = Nxt_Lnk( inf->dtl, csr ) ){
			dtl = Rec_Of(RSV_DTL_DAT, csr);
			if ( dtl->cxl ){
				DBS_INT_SET(dtl->dtl->cxl, &(val=1) );
			}
		}
		if ( chk = Rsv_Cxl_Rsv( rot, DBS_INT_VAL(inf->rsv->id), &erm, dbs, mng ) ){
			Rsv_Dps_Dtl( RSV_INF_(wnd)->dtl );
			Rsv_Get_Dat( DBS_INT_VAL(RSV_INF_(wnd)->rsv->id), wnd, dbs, mng );
			cre = !!Nul_Fld(inf->rsv->id, FLD_INT);
			snd_bst( RSV_INF_(wnd), cre, wnd, mng );
			Viw_Pst_Ntf( VIW_SND_SCH, cre ? VWN_RSV_CREATED: VWN_RSV_CHANGED, NULL, DBS_INT_VAL(inf->rsv->id), wnd );
			Viw_Pst_Ntf( VIW_SND_NAD, cre ? VWN_RSV_CREATED: VWN_RSV_CHANGED, NULL, DBS_INT_VAL(inf->rsv->id), wnd );
		}
		else{
			if ( erm ){
				wnd->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
				free( erm );
			}
		}
	}
	if ( !cnt ){
		wnd->MessageBox( _T("Cancellation target not selected."), NULL, MB_ICONEXCLAMATION|MB_OK );
	}

	Del_All( rot );

	return( chk );
}
