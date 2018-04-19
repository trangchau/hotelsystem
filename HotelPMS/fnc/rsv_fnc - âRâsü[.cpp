/*
	予約検索処理関数
*/

//#include "windows.h"
#include "..\..\Common\bsc_def.h"
#include "..\..\Common\dbs_mng.h"
#include "..\..\Common\dte_mng.h"
#include "..\..\Custom\SimpleGrid.h"
#include "..\..\Custom\CustomStatic.h"
#include "..\..\PMSCustom\BookingPathCombo.h"
#include "..\..\PMSCustom\ClientRankCombo.h"
#include "..\ChildFrm.h"
#include "..\ReservationView.h"
#include "rsv_sql.h"
#include "rsv_fnc.h"
#include "ecg_sql.h"
#include "viw_cmn.h"


static	WCHAR	*Grd_ttl[] =	{	{ _T("New")	},
									{ _T("Name")		},
									{ _T("Status")		},
									{ _T("R.Typ")		},
									{ _T("R.No.")		},
									{ _T("Arrv")		},
									{ _T("Dept")		},
									{ _T("Ad/Ch")		} };

static	GRD_CLM	Grd_clm[] =		{	{  26, GAL_CTR, GCT_NML,  20 },		// New
									{ 192, GAL_LFT, GCT_NML,  20 },		// Name
									{  96, GAL_CTR, GCT_NML,  20 },		// Status
									{ 144, GAL_LFT, GCT_NML, 256 },		// R.Typ
									{  64, GAL_LFT, GCT_NML,  12 },		// R No.
									{  84, GAL_LFT, GCT_NML,  12 },		// Arrv.
									{  84, GAL_LFT, GCT_NML,  12 },		// Dept.
									{  48, GAL_CTR, GCT_NML,  12 } };	// Ad/Ch


static	WCHAR	*Rom_ttl[] =	{	{ _T("UseStart")		},
									{ _T("UseEnd")		},
									{ _T("RoomType")		},
									{ _T("RoomNo.")	},
									{ _T("Amount")		} };

static	GRD_CLM	Rom_clm[] =		{	{  84, GAL_CTR, GCT_DED,  10 },		// UseStart
									{  84, GAL_LFT, GCT_DED,  10 },		// UseEnd
									{ 144, GAL_LFT, GCT_NML,  64 },		// RoomType
									{  84, GAL_CTR, GCT_EDT,  20 },		// RoomNo
									{  84, GAL_LFT, GCT_NML,  12 } };	// Amount



#define	SEL_ROW_(x)				((CReservationView *)(x))->m_Sel_row
#define	RSV_INF_(x)				((CReservationView *)(x))->m_Rsv_inf
#define	RSV_DTL_(x)				((CReservationView *)(x))->m_Rsv_dtl
#define	RTE_PLN_(x)				((CReservationView *)(x))->m_Rte_pln


static	void	del_rcg(				// reservation_charges delete
Csr			rot,						// target root
Csr			csr )						// target cursor
{
	RSV_RCG_DAT	*rcg;
	
	rcg = Rec_Of(RSV_RCG_DAT, csr);

	if ( Nul_Fld(rcg->rcg->id, FLD_INT) ){
		Del_Lnk( rot, csr );
	}
	else{
		Set_Nul( rcg->rcg->ddt, FLD_DTM, FALSE );
	}
}

static	void	del_req(				// reservation_equipment delete
Csr			rot,						// target root
Csr			csr )						// target cursor
{
	RSV_EQP_DAT	*req;
	
	req = Rec_Of(RSV_EQP_DAT, csr);

	if ( Nul_Fld(req->req->id, FLD_INT) ){
		Del_Lnk( rot, csr );
	}
	else{
		Set_Nul( req->req->ddt, FLD_DTM, FALSE );
		for( csr=NULL; csr=Nxt_Lnk(req->rcg, csr); ){
			del_rcg( req->rcg, csr );
		}
	}
}


static	int		req_cnt(				// Avalable reservation_equipment number
Csr			rot )						// root of reservation_equipment
{
	int			cnt=0;
	Csr			csr=NULL;
	while( csr=Nxt_Lnk( rot, csr ) )	if ( Nul_Fld(Rec_Of(RSV_EQP_DAT,csr)->req->ddt, FLD_DTM) )	cnt++;
	return( cnt );
}

static	Csr		add_req(				// Addition one row to reservation_equipments
Csr				rot,					// root of reservation_equipment
void			*ptr,					// data pointer
int				siz )					// data size
{
	int		idx = req_cnt( rot );
	Csr		csr;

	// set to new record if deleted data exist
	if ( csr = Idx_Lnk( rot, idx ) ){
		Set_Nul(Rec_Of(RSV_EQP_DAT, csr)->req->ddt, FLD_DTM, TRUE);
	}
	else{
		csr = Add_Lnk( rot, ptr, siz );
	}

	return( csr );
}

static	Csr		idx_req(				// getting reservation_equipment by index
Csr				rot,					// root of reservation_equipmrnt
int				idx )					// Getting index
{
	Csr			csr=NULL;
	int			idy=0;

	while( csr=Nxt_Lnk( rot, csr ) ){
		if ( Nul_Fld(Rec_Of(RSV_EQP_DAT, csr)->req->ddt,FLD_DAY) ){
			if ( idx==idy )	break;
			idy++;
		}
	}
	return( csr );
}

static	int		mak_est(			// Making a editing status
CWnd		*wnd,					// parent window
PMS_MNG		*mng )					// Hotel Property Management System Structure
{
	RSV_INF_DAT	*inf=RSV_INF_(wnd);
	RSV_DTL_DAT	*dtl;
	int			est=0;
	Csr			csr=NULL;

	est |= ( Cmp_Dte( inf->rsv->dpt, mng->bdt ) < 0 ) ? RSV_EST_OLD: 0;
	est |= RSV_EST_RSV;
	while( csr=Nxt_Lnk( RSV_DTL_(wnd), csr ) ){
		dtl = Rec_Of(RSV_DTL_DAT, csr);
		if ( Nul_Fld(dtl->dtl->ddt, FLD_DTM) ){
			if ( !Nul_Fld(dtl->dtl->cid, FLD_DTM) ){
				if ( Nul_Fld(dtl->dtl->cod, FLD_DTM) ){
					est |= RSV_EST_INH;
					break;
				}
				else{
					est |= RSV_EST_COT;
				}
			}
		}
	}

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
	gdf->vlc = id == IDC_GRD_DTL ? 8: 5;
	gdf->vcc = id == IDC_GRD_DTL ? memcnt(Grd_clm): memcnt(Rom_clm);
	gdf->hbc = RGB(128,255,255);
	gdf->hfc = RGB(0,0,0);
	gdf->dbc = RGB(255,255,255);
	gdf->dfc = RGB(0,0,0);
	gdf->sbc = RGB(0,0,0);
	gdf->sfc = RGB(255,255,255);
	gdf->cct = gdf->vcc;
	gdf->clm = id == IDC_GRD_DTL ? Grd_clm: Rom_clm;
	gdf->sel = GSL_ROW;
	gdf->hdr = TRUE;
	gdf->fsa = TRUE;

	grd = new CSimpleGrid();
	grd->Create( gdf, *rct, id, wnd );
	for ( idx=0; idx<(int)( id == IDC_GRD_DTL ? memcnt(Grd_ttl): memcnt(Rom_ttl)); idx++ ){
		grd->SetCellText( idx, 0, id == IDC_GRD_DTL ? Grd_ttl[idx]: Rom_ttl[idx] );
	}

	return( grd );
}


static	int	dec_dst(							// Decision for reservaion detail status
RSV_DTL_DAT	*dtl )								// 予約詳細データ
{
	int				sts;

	if ( !DBS_INT_VAL(dtl->dtl->rii) ){
		sts = DTL_STS_NEW;
	}
	else if ( DBS_INT_VAL(dtl->dtl->cxl)!=0 ){
		sts = DTL_STS_CXL;
	}
	else if ( Nul_Fld(dtl->dtl->cid,FLD_DTM) && Nul_Fld(dtl->dtl->cod,FLD_DTM) ){
		sts = DTL_STS_RSV;
	}
	else if ( !Nul_Fld(dtl->dtl->cid,FLD_DTM) && Nul_Fld(dtl->dtl->cod,FLD_DTM) ){
		sts = DTL_STS_INH;
	}
	else if ( !Nul_Fld(dtl->dtl->cid,FLD_DTM) && !Nul_Fld(dtl->dtl->cod,FLD_DTM) ){
		sts = DTL_STS_COT;
	}

	return( sts );
}

WCHAR		*set_stp(							// Setting the status panel
CCustomStatic	*pnl,							// Panel pointer
int				sts )							// Status of reservation detail
{
	COLORREF	fcl, bcl;
	WCHAR		*str=_T("Unknown");

	switch( sts ){
	case DTL_STS_NEW:	fcl=STC_NWF;	bcl=STC_NWB;	str = _T(" Creating ");		break;
	case DTL_STS_RSV:	fcl=STC_RVF;	bcl=STC_RVB;	str = _T(" Reserved ");		break;
	case DTL_STS_INH:	fcl=STC_IHF;	bcl=STC_IHB;	str = _T(" Inhouse ");		break;
	case DTL_STS_COT:	fcl=STC_COF;	bcl=STC_COB;	str = _T(" Checked out ");	break;
	case DTL_STS_CXL:	fcl=STC_CXF;	bcl=STC_CXB;	str = _T(" Canceled ");		break;
	}
	if ( pnl ){
		pnl->SetBGColor( bcl );
		pnl->SetFTColor( fcl );
		pnl->SetText( str );
	}

	return( str );
}

static	EQP_TYP		*sch_rtp(						// Searching room type data
int				eti,								// ID of equipment_types
PMS_MNG			*mng )
{
	Csr			csr=NULL;

	while( csr=Nxt_Lnk(mng->etl, csr) ){
		if ( DBS_INT_VAL(Rec_Of(EQP_TYP, csr)->id)==eti ){
			break;
		}
	}

	return( !!csr ? Rec_Of(EQP_TYP, csr): NULL );
}

static	void	dsp_rom(						// Displaying room data on 1 row
CSimpleGrid	*grd,								// displaying target
RSV_EQP_DAT	*req,								// reservation_equipment
int			row )								// displaying row
{
	WCHAR		buf[40];

	grd->SetEnable( -1, row, TRUE );
	grd->SetCellText( ROM_USD_CLM, row, !Nul_Fld(req->req->usd, FLD_DAY) ? EDT_DBF_DAY(buf, memcnt(buf), req->req->usd): _T("") );
	grd->SetCellText( ROM_UED_CLM, row, !Nul_Fld(req->req->ued, FLD_DAY) ? EDT_DBF_DAY(buf, memcnt(buf), req->req->ued): _T("") );
	grd->SetCellText( ROM_TYP_CLM, row, !Nul_Fld(req->tnm, FLD_WST) ? (WCHAR *)Fld_Ptr(req->tnm, FLD_WST): _T("") );
	grd->SetCellText( ROM_NBR_CLM, row, !Nul_Fld(req->rno, FLD_WST) ? (WCHAR *)Fld_Ptr(req->rno, FLD_WST): _T("") );
}

void		Rsv_Rom_Dsp(						// Setup the room list
CWnd		*wnd,								// 表示先ウィンドウ
Csr			rot,								// Root of reservation detail data
PMS_MNG		*mng )								// Property management System management structure
{
	Csr			csr=NULL;
	RSV_EQP_DAT	*req;
	int			row=0;
	CSimpleGrid	*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_ROM));

	grd->ResetContent();
	grd->InputEnable(TRUE);

	while( csr=Nxt_Lnk(rot, csr ) ){
		req = Rec_Of(RSV_EQP_DAT, csr);
		if ( Nul_Fld(req->req->ddt,FLD_DTM) ){
			row++;
			grd->AddRow(1);
			grd->SetData( 0, row, (int)(LONG_PTR)req );
			dsp_rom( grd, req, row );
		}
	}
}

void		Rsv_Dtl_Dsp(						// 予約詳細表示
CWnd		*wnd,								// 表示先ウィンドウ
RSV_DTL_DAT	*dtl,								// 予約詳細データ
RSV_INF_DAT	*rsv,								// NULL when new creation
PMS_MNG		*mng )								// Property management System management structure
{
	WCHAR				buf[32];
	Csr					csr=NULL;
	int					enb, sts, sam, cnt;
	CEquipmentTypeCombo	*cbx;

	// status panel
	sts = dec_dst( dtl );
	set_stp( (CCustomStatic	*)wnd->GetDlgItem(IDC_STATIC_DETAILSTS), sts );

	// name
	wnd->GetDlgItem(IDC_EDIT_FIRSTNAME_DETAIL)->SetWindowText( EDT_DBF_WST(dtl->cli->fnm,_T("")) );
	wnd->GetDlgItem(IDC_EDIT_FAMILYNAME_DETAIL)->SetWindowText( EDT_DBF_WST(dtl->cli->lnm,_T("")) );

	// arrival date & tile
	wnd->GetDlgItem(IDC_EDIT_ARRIVALDATE)->SetWindowText( Edt_Dte( buf, memcnt(buf), DBS_DAY_PTR(dtl->dtl->arv)) );
	wnd->GetDlgItem(IDC_EDIT_ARRIVALTIME)->SetWindowText( EDT_DBF_TIM(buf, memcnt(buf), dtl->dtl->atm) );
	((CButton *)wnd->GetDlgItem(IDC_CHECK_EARLYCI))->SetCheck( !!DBS_INT_VAL(dtl->dtl->eci) );
	enb = !dtl || Cmp_Dte(mng->bdt, (Day *)Fld_Ptr(dtl->dtl->arv, FLD_DAY)) <= 0;
	wnd->GetDlgItem(IDC_EDIT_ARRIVALDATE)->EnableWindow( enb );
	wnd->GetDlgItem(IDC_EDIT_ARRIVALTIME)->EnableWindow( enb );
	wnd->GetDlgItem(IDC_CHECK_EARLYCI)->EnableWindow( enb );

	// departure date & time
	wnd->GetDlgItem(IDC_EDIT_DEPARTUREDATE)->SetWindowText( Edt_Dte( buf, memcnt(buf), DBS_DAY_PTR(dtl->dtl->dpt)) );
	swprintf_s( buf, memcnt(buf), _T("%d"), Clc_Dys((Day *)Fld_Ptr(dtl->dtl->arv, FLD_DAY), (Day *)Fld_Ptr(dtl->dtl->dpt, FLD_DAY)) );
	wnd->GetDlgItem(IDC_EDIT_DEPARTURETIME)->SetWindowText( EDT_DBF_TIM(buf, memcnt(buf), dtl->dtl->dtm) );
	((CButton *)wnd->GetDlgItem(IDC_CHECK_LATECO))->SetCheck( !!DBS_INT_VAL(dtl->dtl->lco) );
	enb = !dtl || Cmp_Dte(mng->bdt, (Day *)Fld_Ptr(dtl->dtl->dpt, FLD_DAY)) <= 0;
	wnd->GetDlgItem(IDC_EDIT_DEPARTUREDATE)->EnableWindow( enb );
	wnd->GetDlgItem(IDC_EDIT_DEPARTURETIME)->EnableWindow( enb );
	wnd->GetDlgItem(IDC_CHECK_LATECO)->EnableWindow( enb );

	// nights
	wnd->GetDlgItem(IDC_EDIT_NIGHTS)->SetWindowText( buf );
	((CSpinButtonCtrl *)wnd->GetDlgItem(IDC_SPIN_NIGHTS))->SetRange(0,UD_MAXVAL );
	wnd->GetDlgItem(IDC_EDIT_NIGHTS)->EnableWindow( enb );
	wnd->GetDlgItem(IDC_SPIN_NIGHTS)->EnableWindow( enb );

	// adult number
	wnd->GetDlgItem(IDC_EDIT_ADULTNUMBER)->SetWindowTextW( EDT_DBF_INT(buf, memcnt(buf), dtl->dtl->psn, Edt_Num_Nml) );
	((CSpinButtonCtrl *)wnd->GetDlgItem(IDC_SPIN_ADULTNUMBER))->SetRange(1,UD_MAXVAL );
	wnd->GetDlgItem(IDC_EDIT_ADULTNUMBER)->EnableWindow( enb );
	wnd->GetDlgItem(IDC_SPIN_ADULTNUMBER)->EnableWindow( enb );

	// child number
	wnd->GetDlgItem(IDC_EDIT_CHILDNUMBER)->SetWindowTextW( EDT_DBF_INT(buf, memcnt(buf), dtl->dtl->chn, Edt_Num_Nml) );
	((CSpinButtonCtrl *)wnd->GetDlgItem(IDC_SPIN_CHILDNUMBER))->SetRange(0,UD_MAXVAL );
	wnd->GetDlgItem(IDC_EDIT_CHILDNUMBER)->EnableWindow( enb );
	wnd->GetDlgItem(IDC_SPIN_CHILDNUMBER)->EnableWindow( enb );

	// room type
	cbx = (CEquipmentTypeCombo *)(wnd->GetDlgItem(IDC_COMBO_ROOMTYPE));
	cbx->SetType( Nul_Fld(dtl->dtl->eti, FLD_INT) ? -1: DBS_INT_VAL(dtl->dtl->eti) );

	// Disable the room type selector if booking path not selected.
	cbx->EnableWindow( enb && ((CComboBox *)(wnd->GetDlgItem(IDC_COMBO_BOOKINGPATH)))->GetCurSel()!=-1 );
	//wnd->GetDlgItem(IDC_COMBO_ROOMTYPE)->EnableWindow( enb );
	wnd->GetDlgItem(IDC_BUTTON_ROOMTYPE)->EnableWindow( enb );

	// room change
	swprintf_s( buf, memcnt(buf), _T("%d"), ( cnt = req_cnt( dtl->req ) ) > 0 ? cnt-1: 0 );
	wnd->GetDlgItem(IDC_EDIT_ROOMCHANGE)->SetWindowText( buf );
	((CSpinButtonCtrl *)wnd->GetDlgItem(IDC_SPIN_ROOMCHANGE))->SetRange(0,cnt-1);
	wnd->GetDlgItem(IDC_EDIT_ROOMCHANGE)->EnableWindow( enb );
	wnd->GetDlgItem(IDC_SPIN_ROOMCHANGE)->EnableWindow( enb );

	// room number
//	wnd->GetDlgItem(IDC_EDIT_ROOM)->SetWindowTextW( EDT_DBF_WST(dtl->rno,_T("")) );
//	wnd->GetDlgItem(IDC_EDIT_ROOM)->EnableWindow( enb );
//	wnd->GetDlgItem(IDC_BUTTON_ROOM)->EnableWindow( enb );

	// RatePlanName
	wnd->GetDlgItem(IDC_COMBO_RATEPLANNAME)->EnableWindow( enb && ((CComboBox *)(wnd->GetDlgItem(IDC_COMBO_BOOKINGPATH)))->GetCurSel()!=-1 );
	
	// Room Assign Grid
	wnd->GetDlgItem(IDC_GRD_ROM)->EnableWindow( enb );

	// note
	wnd->GetDlgItem(IDC_EDIT_NOTE)->SetWindowText( EDT_DBF_WST(dtl->dtl->nte,_T("")) );
	wnd->GetDlgItem(IDC_EDIT_NOTE)->EnableWindow( enb );

	// Same as made a reservation person
	((CButton *)wnd->GetDlgItem(IDC_CHECK_SAMERESERVATION))->SetCheck( sam = Nul_Fld(rsv->rsv->id,FLD_INT) || !!dtl->smr );
	wnd->GetDlgItem(IDC_CHECK_SAMERESERVATION)->EnableWindow( enb );
	// decision then names inputting is enable or disable with same reserved person
	wnd->GetDlgItem(IDC_EDIT_FIRSTNAME_DETAIL)->EnableWindow( enb && !sam );
	wnd->GetDlgItem(IDC_EDIT_FAMILYNAME_DETAIL)->EnableWindow( enb && !sam );

	Rsv_Rom_Dsp( wnd, dtl->req, mng );
}

void		Rsv_Dsp_DGr(						// Displaying reservation_details grid
int			row,								// Row of grid
RSV_DTL_DAT	*dtl,								// Displaying data
CWnd		*wnd,								// Parent window
PMS_MNG		*mng )								// Hotel Property Management System Structure
{
	WCHAR				buf[520];
	int					rdi=0;
	EQP_TYP				*eqt;
	CSimpleGrid			*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL));

	grd->SetCellText( 0, row, Nul_Fld(dtl->dtl->id,FLD_INT) ? _T("*"): _T("") );
	swprintf_s( buf, memcnt(buf), _T("%s %s"), EDT_DBF_WST(dtl->cli->fnm,_T("")), EDT_DBF_WST(dtl->cli->lnm,_T("")) );
	grd->SetCellText( 1, row, buf );												// Name
	grd->SetCellText( 2, row, set_stp( NULL, dec_dst( dtl ) ) );					// Status
	if ( eqt = sch_rtp( DBS_INT_VAL(dtl->dtl->eti), mng ) ){
		grd->SetCellText( 3, row, EDT_DBF_WST(eqt->nam,_T("")) );						// R.Typ
	}
//	grd->SetCellText( 4, row, EDT_DBF_WST(dtl->rno,_T("")) );						// R.No.
	grd->SetCellText( 5, row, EDT_DBF_DAY(buf, memcnt(buf), dtl->dtl->arv) );		// Arrv
	grd->SetCellText( 6, row, EDT_DBF_DAY(buf, memcnt(buf), dtl->dtl->dpt) );		// Dept
	swprintf_s( buf, memcnt(buf), _T("%d/%d"), DBS_INT_VAL(dtl->dtl->psn), DBS_INT_VAL(dtl->dtl->chn) );
	grd->SetCellText( 7, row, buf );												//Ad/Ch

	grd->SetData( 0, row, (int)(LONG_PTR)dtl );
}


void		Rsv_Dsp_Dat(						// Displaying reservation_infos data
RSV_INF_DAT	*rsv,								// NULL when new creation
Csr			rot,								// Root of reservation_details data
CDatabase	*dbs,								// ODBC database object
CWnd		*wnd,								// Parent window
PMS_MNG		*mng )								// Property management System management structure
{
	WCHAR				buf[520];
	RSV_DTL_DAT			*dtl;	
	Csr					csr=NULL;
	int					idx=0, rdi=0, row=0, ckd, bfr;
	CSimpleGrid			*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL));
	CTabCtrl			*tab = (CTabCtrl *)wnd->GetDlgItem(IDC_TAB_RSV_DETAIL);
	CBookingPathCombo	*cbx;

	// Text datas on reservation_infos
	swprintf_s( buf, memcnt(buf), _T("%s"), !!rsv ? EDT_DBF_INT(buf, memcnt(buf), rsv->rsv->rvn, Edt_Num_Nml):_T("*************") );
	wnd->GetDlgItem(IDC_STATIC_BOOKINGID)->SetWindowText( buf );
	wnd->GetDlgItem(IDC_EDIT_FIRSTARRIVAL)->SetWindowText( EDT_DBF_DAY(buf, memcnt(buf), rsv->rsv->arv) );
	wnd->GetDlgItem(IDC_EDIT_LASTDEPARTURE)->SetWindowText( EDT_DBF_DAY(buf, memcnt(buf), rsv->rsv->dpt) );
	wnd->GetDlgItem(IDC_EDIT_FIRSTNAME)->SetWindowText( EDT_DBF_WST(rsv->cli->fnm,_T("")) );
	wnd->GetDlgItem(IDC_EDIT_FAMILYNAME)->SetWindowText( EDT_DBF_WST(rsv->cli->lnm,_T(""))  );
	wnd->GetDlgItem(IDC_EDIT_ADDRESS1)->SetWindowText( EDT_DBF_WST(rsv->cli->adr1,_T("")) );
	wnd->GetDlgItem(IDC_EDIT_ADDRESS2)->SetWindowText( EDT_DBF_WST(rsv->cli->adr2,_T("")) );
	wnd->GetDlgItem(IDC_EDIT_TELEPHONE1)->SetWindowText( EDT_DBF_WST(rsv->cli->tel1,_T("")) );
	wnd->GetDlgItem(IDC_EDIT_TELEPHONE2)->SetWindowText( EDT_DBF_WST(rsv->cli->tel2,_T("")) );
	wnd->GetDlgItem(IDC_EDIT_FAXNUMBER)->SetWindowText( EDT_DBF_WST(rsv->cli->fax,_T("")) );
	wnd->GetDlgItem(IDC_EDIT_EMAIL1)->SetWindowText( EDT_DBF_WST(rsv->cli->eml1,_T("")) );
	wnd->GetDlgItem(IDC_EDIT_EMAIL2)->SetWindowText( EDT_DBF_WST(rsv->cli->eml2,_T("")) );

	wnd->GetDlgItem(IDC_EDIT_FIRSTARRIVAL)->EnableWindow( FALSE );
	wnd->GetDlgItem(IDC_EDIT_LASTDEPARTURE)->EnableWindow( FALSE );

	// ReservationType
	ckd =  Nul_Fld(rsv->rsv->id,FLD_INT) ? CLK_PERSONAL: DBS_INT_VAL(rsv->cli->ckd);
	((CButton *)wnd->GetDlgItem(IDC_RADIO_PERSONAL))->SetCheck( ckd == CLK_PERSONAL );
	((CButton *)wnd->GetDlgItem(IDC_RADIO_GROUP))->SetCheck( ckd == CLK_GROUP );

	// BookingPath
	cbx = (CBookingPathCombo *)wnd->GetDlgItem(IDC_COMBO_BOOKINGPATH);

	if ( rsv ){
		cbx->SetItemIndex( DBS_INT_VAL(rsv->rsv->bfr), DBS_INT_VAL(rsv->rsv->aii) );
	}
	else{
		cbx->SetCurSel(-1);
	}

	// ClientRank
	((CClientRankCombo *)wnd->GetDlgItem(IDC_COMBO_CLIENTRANK))->SetRank(
				Nul_Fld(rsv->cli->crk,FLD_INT) ? CLL_DEFAULT: DBS_INT_VAL(rsv->cli->crk) );

	// minimum room numbers
	((CSpinButtonCtrl *)wnd->GetDlgItem(IDC_SPIN_ROOMNUMBER))->SetRange( !!rot ? Cnt_Lnk(rot): 1, UD_MAXVAL );

	// Setting reservation_details grid
	grd->ResetContent();
	grd->AddRow( Cnt_Lnk( rot ) );

	while( csr = Nxt_Lnk( rot, csr ) ){
		dtl = Rec_Of(RSV_DTL_DAT, csr);
		// Judgment on same as reserved person
		dtl->smr = !Nul_Fld(rsv->rsv->cii, FLD_INT) && !Nul_Fld(dtl->dtl->cii,FLD_INT);
		if ( dtl->smr )	dtl->smr = DBS_INT_VAL(rsv->rsv->cii)==DBS_INT_VAL(dtl->dtl->cii);
		// Displaying first reservation_detail to reservation_detail area
		if ( !idx ){
			bfr = Nul_Fld(rsv->rsv->bfr,FLD_INT) ? 0: DBS_INT_VAL(rsv->rsv->bfr);
			Viw_Scb_Eqt( (CComboBox *)(wnd->GetDlgItem(IDC_COMBO_ROOMTYPE)),
				bfr, DBS_INT_VAL(rsv->rsv->aii), 
				DBS_DAY_PTR(dtl->dtl->arv), DBS_DAY_PTR(dtl->dtl->dpt), wnd, dbs, mng );
			Rsv_Dtl_Dsp( wnd, dtl, rsv, mng );
		}
		row++;
		Rsv_Dsp_DGr( row, dtl, wnd, mng );
//			swprintf_s( buf, memcnt(buf), _T("Guest%d%s"), idx+1, Nul_Fld(dtl->dtl->id, FLD_INT) ? _T("(*)"): _T("") );
//			tab->InsertItem( idx++, buf );
		idx++;
	}
	grd->SetSelectCell( 0, 1 );
	// room numbers editbox It shoud be this location because editchange event is happen just aftre this setting
	swprintf_s( buf, memcnt(buf), _T("%d"), !!rot ? Cnt_Lnk(rot): 1 );
	wnd->GetDlgItem(IDC_EDIT_ROOMNUMBER)->SetWindowText( buf );

	// setting carette
	wnd->GetDlgItem(IDC_EDIT_FIRSTNAME)->SetFocus();

}

static	int		get_spd(				// getting stay period from screen
Day			*arv,						// storing arrival date pointer
Day			*dpt,						// storing departure date pointer
Day			*bdt,						// business date
int			glc,						// greater or lesser check
CWnd		*wnd )						// Parent window
{
	WCHAR		buf[192], *itm, *atn=_T("ArrivalDate"), *dtn=_T("DepartureDate"); 
	WCHAR		*pfx=NULL, *pfx1=_T("%s should be same or greater than today"), *pfx2=_T("%s is wrong");
	int			chk, dys;

	wnd->GetDlgItem(IDC_EDIT_ARRIVALDATE)->GetWindowText( buf, memcnt(buf) );
	if ( chk = Str_Dte( arv, buf ) ){
		if ( chk = !bdt || ( Cmp_Dte( arv, bdt ) >= 0 ) ){
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

static	int		set_req(				// setting the reservation_equipment
CSimpleGrid		*grd,					// Displaying target grid
RSV_DTL_DAT		*dtl,					// setting target
Day				*arv,					// arrival date
Day				*dpt )					// departure date
{
	Csr			csr;
	RSV_EQP_DAT	*req;

	if ( csr = Nxt_Lnk( dtl->req, NULL ) ){
		req = Rec_Of(RSV_EQP_DAT, csr);
		*DBS_DAY_PTR(req->req->usd) = *arv;
		dsp_rom( grd, req, 1 ); 
	}
	if ( csr = Prv_Lnk( dtl->req, NULL ) ){
		req = Rec_Of(RSV_EQP_DAT, csr);
		*DBS_DAY_PTR(req->req->ued) = *dpt;
		dsp_rom( grd, req, grd->GetLineCount( FALSE ) ); 
	}
	return( TRUE );
}

void	Rsv_Sty_Chg(					// Stay period change event
RSV_DTL_DAT	*dtl,						// reservation detail
CWnd		*wnd,						// Target window
PMS_MNG		*mng )						// Property management System management structure
{
	WCHAR		*msg=NULL, buf[128];
	Day			arv[1], dpt[1];
	int			dys;
	CSimpleGrid	*grd = (CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_ROM));

	if ( ( dys = get_spd( arv, dpt, mng->bdt, FALSE, wnd ) ) != -1 ){
		if ( Cmp_Dte( arv, dpt ) <= 0 ){
			dys = Clc_Dys( arv, dpt );
			swprintf_s( buf, memcnt(buf), _T("%d"), dys );
			wnd->GetDlgItem(IDC_EDIT_NIGHTS)->SetWindowText( buf );
			set_req( grd, dtl, arv, dpt );
		}
	}
	if ( msg )	wnd->MessageBox( msg, NULL, MB_ICONEXCLAMATION|MB_OK );
}

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

void	Rsv_Bph_Chg(				// Booking path change event
RSV_INF_DAT		*inf,				// reservation_infos data 
RSV_DTL_DAT		*dtl,				// selecting reservation_details 
CWnd			*wnd,				// ReservationView
CDatabase		*dbs,				// Database object
PMS_MNG			*mng )				// Hotel Property Management System Structure
{
	int					pth, agi, sam;
	CBookingPathCombo	*bpc = (CBookingPathCombo *)(wnd->GetDlgItem(IDC_COMBO_BOOKINGPATH));
	CComboBox			*rtc = (CComboBox *)(wnd->GetDlgItem(IDC_COMBO_ROOMTYPE));

	bpc->GetItemIndex( &pth, &agi );
	sam  = pth == ( Nul_Fld(inf->rsv->bfr,FLD_INT) ? -1: DBS_INT_VAL(inf->rsv->bfr) ); 
	if ( sam && pth==VIA_AGENT )	sam &= !Nul_Fld(inf->rsv->aii,FLD_INT) &&  DBS_INT_VAL(inf->rsv->aii) == agi; 
	if ( !sam && pth != -1 ) {
		Viw_Scb_Eqt( rtc, pth, agi, DBS_DAY_PTR(dtl->dtl->arv), DBS_DAY_PTR(dtl->dtl->dpt), wnd, dbs, mng );
	}
	rtc->EnableWindow( pth != -1 );
	wnd->GetDlgItem(IDC_COMBO_RATEPLANNAME)->EnableWindow( pth != -1 );
}

void	Rsv_Ngt_Chg(					// Change the night editbox event
RSV_DTL_DAT		*dtl,					// selecting reservation_details 
CWnd			*wnd,					// Target window
PMS_MNG			*mng )					// HOtel Property Management System Structure
{
	WCHAR		buf[20];
	Day			arv[1], dpt[1];
	int			dys;
	CSimpleGrid	*grd = (CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_ROM));

	if ( get_spd( arv, dpt, mng->bdt, TRUE, wnd ) != -1 ){
		wnd->GetDlgItem(IDC_EDIT_NIGHTS)->GetWindowText( buf, memcnt(buf) );
		*dpt = *arv;
		Add_Dte( dpt, dys = _wtoi( buf ) );
		wnd->GetDlgItem(IDC_EDIT_DEPARTUREDATE)->SetWindowText( Edt_Dte( buf, memcnt(buf), dpt ) );
		((CSpinButtonCtrl *)wnd->GetDlgItem(IDC_SPIN_ROOMCHANGE))->SetRange(0,dys-1);
		// Set room chhange number if that is greater than nights
		wnd->GetDlgItem(IDC_EDIT_ROOMCHANGE)->GetWindowText( buf, memcnt(buf) );
		if ( _wtoi( buf ) > dys ){
			swprintf_s( buf, memcnt(buf), _T("%d"), dys-1 );
			wnd->GetDlgItem(IDC_EDIT_ROOMCHANGE)->SetWindowText( buf );
		}
		set_req( grd, dtl, arv, dpt );

	}
}


void	Rsv_Rcg_Chg(					// Change the room change times editbox event
RSV_DTL_DAT	*dtl,						// Reservation detail data
CWnd		*wnd,						// Target window
PMS_MNG		*mng )						// HOtel Property Management System Structure
{
	int			idx, nct, cct, row, dys;
	WCHAR		buf[20];
	RSV_EQP_DAT	*req, *prv;
	Day			arv[1], dpt[1];
	Csr			csr;
	CSimpleGrid	*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_ROM));

	if ( ( dys = get_spd( arv, dpt, mng->bdt, TRUE, wnd ) ) != -1 ){
		wnd->GetDlgItem(IDC_EDIT_ROOMCHANGE)->GetWindowText( buf, memcnt(buf) );
		nct = _wtoi( buf );
		cct = req_cnt( dtl->req ) - 1;
		if ( nct > cct ){
			if ( nct < dys ){
				for ( idx=0; idx<(nct-cct); idx++ ){
					if ( csr = add_req( dtl->req, NULL, sizeof(RSV_EQP_DAT) ) ){
						req = Rec_Of(RSV_EQP_DAT, csr);
						prv = (RSV_EQP_DAT *)(LONG_PTR)grd->GetData( 0, grd->GetLineCount( FALSE ) );
						*req = *prv;
						DBS_DAY_CPY(req->req->usd, req->req->ued);
						Add_Dte( DBS_DAY_PTR(req->req->usd), -1 );
						DBS_DAY_CPY(prv->req->ued, req->req->usd);
						grd->AddRow( 1 );
						grd->SetData( 0, grd->GetLineCount( FALSE ), (int)(LONG_PTR)req );
						dsp_rom( grd, req, grd->GetLineCount( FALSE ) );
						dsp_rom( grd, prv, grd->GetLineCount( FALSE )-1 ); 
						for ( row=grd->GetLineCount( FALSE )-1; row>1 ; row-- ){
							req = (RSV_EQP_DAT *)(LONG_PTR)grd->GetData( 0, row );
							prv = (RSV_EQP_DAT *)(LONG_PTR)grd->GetData( 0, row-1 );
							if ( !Cmp_Dte( DBS_DAY_PTR(req->req->usd), DBS_DAY_PTR(req->req->ued) ) ){
								Add_Dte( DBS_DAY_PTR(req->req->usd), -1);
								DBS_DAY_CPY(prv->req->ued, req->req->usd); 
								dsp_rom( grd, req, row );
								dsp_rom( grd, prv, row-1 );
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
				dsp_rom( grd, prv, cct);
				grd->DelRow( cct + 1 );
				//Del_Lnk( dtl->req, csr );
				del_req( dtl->req, ((Csr)req)-1 );
				--cct;
			}
			grd->Redraw();
		}
	}
}

void	Rsv_Rom_Dte(					// Room use date changed
WCHAR		*txt,						// inputted text
int			clm,						// column on the grid
int			row,						// row on the grid
RSV_DTL_DAT	*dtl,						// pointer of reservation_details
CWnd		*wnd,						// Parent window
PMS_MNG		*mng )						// Hotel Property management System Structure
{
	int			chk, cnt, dys;
	WCHAR		*msg=NULL;
	RSV_EQP_DAT	*req, *otr;
	Day			idt[1], odt[1], arv[1], dpt[1];
	CSimpleGrid	*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_ROM));

	if ( !Str_Dte( idt, txt ) ){
		msg = _T("Inputted date is wrong");
	}
	else{
		if ( chk = ( dys = get_spd( arv, dpt, NULL, TRUE, wnd ) ) != -1 ){		// Getting stay period fro screen
			if ( !!( cnt = req_cnt( dtl->req ) ) ){
				req = (RSV_EQP_DAT *)(LONG_PTR)grd->GetData( 0, row );
				*odt = *DBS_DAY_PTR(clm==ROM_USD_CLM ? req->req->usd: req->req->ued);
				if ( !!Cmp_Dte( odt, idt ) ){
					if( row==1 && clm==ROM_USD_CLM ){
						msg = _T("Can not change the date which start of period");
					}
					else if ( row==cnt && clm==ROM_UED_CLM ){
						msg = _T("Can not change date which end of period");
					}
					else{
						if ( Cmp_Dte( idt, odt ) < 0 ){			// Inputed date is less than existied date
							if ( clm==ROM_USD_CLM ){						// In use case of start date 
								otr = (RSV_EQP_DAT *)(LONG_PTR)grd->GetData( 0, row-1 );
								if ( chk = Cmp_Dte( DBS_DAY_PTR(otr->req->usd), idt) < 0 ){
									*DBS_DAY_PTR(otr->req->ued) = *idt;
									*DBS_DAY_PTR(req->req->usd) = *idt;
									grd->SetCellText( 1, row-1, txt );
								}
							}
							else{								// In case of end date
								if ( chk = Cmp_Dte( DBS_DAY_PTR(req->req->usd), idt) < 0 ){
									*DBS_DAY_PTR(req->req->ued) = *idt;
								}
							}
						}
						else{									// Inputed date is greater than existied date
							if ( clm==ROM_USD_CLM ){
								if ( chk = Cmp_Dte( DBS_DAY_PTR(req->req->ued), idt) > 0 ){
									*DBS_DAY_PTR(req->req->ued) = *idt;
								}
							}
							else{
								otr = (RSV_EQP_DAT *)(LONG_PTR)grd->GetData( 0, row+1 );
								if ( chk = Cmp_Dte( DBS_DAY_PTR(otr->req->ued), idt) > 0 ){
									*DBS_DAY_PTR(otr->req->usd) = *idt;
									*DBS_DAY_PTR(req->req->ued) = *idt;
									grd->SetCellText( 0, row+1, txt );
								}
							}
						}
						if ( chk )	grd->SetCellText( clm, row, txt );
						else		msg = _T("Date change impossible because date conflict occurred");
					}
				}
			}
		}
	}
	if ( msg )	wnd->MessageBox( msg, NULL, MB_ICONEXCLAMATION|MB_OK );
}

void		Rsv_Rte_Pln(				// getting and setting rate plan
Csr			rot,						// storing root
int			eti,						// id of equipment_types
RSV_DTL_DAT	*dtl,						// reservation_detail
CDatabase	*dbs,						// ODBC Database object
CWnd		*wnd,						// Parent window
PMS_MNG		*mng )						// Hotel Property management System Structure
{
	int					chk;
	WCHAR				*erm=NULL;
	ECG_SQL_PRM			prm[1];
	CBookingPathCombo	*bpc = (CBookingPathCombo *)(wnd->GetDlgItem(IDC_COMBO_BOOKINGPATH));

	bpc->GetItemIndex( &prm->pth, &prm->agi );
	prm->std = DBS_DAY_PTR(dtl->dtl->arv);
	prm->edd = DBS_DAY_PTR(dtl->dtl->dpt);
	prm->opc = mng->opc;
	prm->brc = mng->brc;
	prm->fcc = mng->fcc;

	chk = Ecg_Get_Pln( rot, eti, prm, &erm, dbs, mng );
}

void		Rsv_Dsp_Rte(				// dispose the rate plan
Csr			rot )						// root of rate plan
{
	Del_All( rot );
}


void		Rsv_Rom_Nbr(				// Room number edit complete event
WCHAR		*txt,						// inputted text
int			row,						// row on the grid
CDatabase	*dbs,						// ODBC Database object
CWnd		*wnd,						// Parent window
PMS_MNG		*mng )						// Hotel Property management System Structure
{
	int			cnt;
	Csr			csr=NULL;
	EQP_INF		*eqi;
	EQP_TYP		*eqt;
	RSV_EQP_DAT	*req;
	WCHAR		*erm=NULL, *err=NULL;
	CSimpleGrid	*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_ROM));
	Day			*usd, *ued;

	while( csr=Nxt_Lnk( mng->eil, csr ) ){
		eqi = Rec_Of(EQP_INF, csr);
		if ( !wcscmp( DBS_WST_PTR(eqi->eco), txt ) ){
			break;
		}
	}
	if ( !!csr ){			// found!
		for( csr=NULL; csr=Nxt_Lnk( mng->etl, csr ); ){
			eqt = Rec_Of(EQP_TYP, csr);
			if ( DBS_INT_VAL(eqt->id)==DBS_INT_VAL(eqi->eti) ){
				break;
			}
		}
		eqt = !!csr ? eqt: NULL;
		req = (RSV_EQP_DAT *)(LONG_PTR)grd->GetData( 0, row );
		usd=DBS_DAY_PTR(req->req->usd);
		ued=DBS_DAY_PTR(req->req->ued);
		if ( ( cnt = Rsv_Rom_Avl( DBS_INT_VAL(eqi->id), DBS_INT_VAL(req->req->rdi), usd, ued, &erm, dbs, mng ) ) != -1 ){
			if ( cnt==0 ){		// room is avalable
				Set_Nul(req->req->eii,FLD_INT,FALSE);
				*DBS_INT_PTR(req->req->eii) = DBS_INT_VAL(eqi->id);
				DBS_WST_CPY(req->rno, eqi->eco);
				if ( eqt ){
					Set_Nul(req->req->eti,FLD_INT, FALSE);
					*DBS_INT_PTR(req->req->eti)=*DBS_INT_PTR(eqt->id);
					DBS_WST_CPY(req->tnm, eqt->nam);
				}
				grd->SetCellText( ROM_NBR_CLM, row, txt );
				grd->SetCellText( ROM_TYP_CLM, row, DBS_WST_PTR(eqt->nam) );
			}
			else{				// room is not avalable
				err = _T("Room is not avalable on this period");
			}
		}
	}
	else{				// room is not avalable
		err = _T("Room number not found");
	}
	if ( erm || err ){
		wnd->MessageBox( erm ? erm: err, NULL, MB_ICONEXCLAMATION|MB_OK );
		if ( erm )	free( erm );
	}
}


//-----------------------------------------------------------------------------------------------------------------------
static	void	set_dtl(				// Setting a new reservation_detail
RSV_DTL_DAT		*dtl,					// destination pointer
Day				*arv,					// default arrival date
Day				*dpt )					// default departure date
{
	Csr			csr;
	RSV_EQP_DAT	*req;

	Set_Nul(dtl->dtl->arv,FLD_DAY,FALSE);
	Set_Nul(dtl->dtl->dpt,FLD_DAY,FALSE);
	*DBS_DTM_PTR(dtl->dtl->arv) = *arv;			// arrival_date 
	*DBS_DTM_PTR(dtl->dtl->dpt) = *dpt;			// departure date
	Set_Nul(dtl->dtl->psn,FLD_INT,FALSE);		// Set default 1 into person number(Adult)
	*DBS_INT_PTR(dtl->dtl->psn) = 1;			
	Set_Nul(dtl->dtl->chn,FLD_INT,FALSE);		// Set default 0 into children number
	*DBS_INT_PTR(dtl->dtl->chn) = 0;
	Set_Nul(dtl->cli->crk,FLD_INT,FALSE);		// Set degault General into client_rank
	*DBS_INT_PTR(dtl->cli->crk) = CLL_GENERAL;
	Set_Nul(dtl->cli->ckd,FLD_INT,FALSE);
	*DBS_INT_PTR(dtl->cli->ckd) = CLK_PERSONAL;	// Set default Personal into client_kind

	if ( csr = Add_Lnk( dtl->req, NULL, sizeof(RSV_EQP_DAT) ) ){
		req = Rec_Of(RSV_EQP_DAT, csr);
		Set_Nul(req->req->usd, FLD_DAY, FALSE);	
		Set_Nul(req->req->ued, FLD_DAY, FALSE);	
		*DBS_DAY_PTR(req->req->usd) = *arv; 
		*DBS_DAY_PTR(req->req->ued) = *dpt; 
	}
}

void	Rsv_Rct_Chg(					// Room number editbox change 
Csr			rot,						// Root of reservation_detail data
CWnd		*wnd,						// Parent window
int			rct,						// Room number from editbox
PMS_MNG		*mng )						// Property management System management structure
{
	int			crc, exc=0;
	Csr			csr=NULL;
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
					set_dtl( dtl, DBS_DAY_PTR(src->dtl->arv), DBS_DAY_PTR(src->dtl->dpt) );
					if ( grd->AddRow( 1 ) ){
						grd->Redraw();
						Rsv_Dsp_DGr( crc, dtl, wnd, mng );
					}
					else{
						Del_Lnk( rot, csr );
					}
				}
			}
		}
	}
}


int		Rsv_Cre_Rsv(					// Create a new reservation
CWnd		*wnd,						// Target window
RSV_INF_DAT	*inf,						// 
Csr			rot,						// Root of detail data
CCI_PRM		*cci,						// Create a reservation parameter
CDatabase	*dbs,						// Database object
PMS_MNG		*mng )						// Property management System management structure
{
	int			chk;
	RSV_DTL_DAT	*dtl;
	RSV_EQP_DAT	*req;
	Csr			csr;
	WCHAR		*ptr;
	CSimpleGrid	*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_DTL));

	memset( inf, 0, sizeof(RSV_INF_DAT) );

	if ( !( chk = !cci ) ){
		if ( chk = !!( csr = Add_Lnk( rot, NULL, sizeof(RSV_DTL_DAT) ) ) ){
			Set_Nul(inf->rsv->arv, FLD_DAY, FALSE);	
			Set_Nul(inf->rsv->dpt, FLD_DAY, FALSE);	
			*DBS_DAY_PTR(inf->rsv->arv) = *cci->arv; 
			*DBS_DAY_PTR(inf->rsv->dpt) = *cci->dpt; 

			dtl = Rec_Of(RSV_DTL_DAT, csr);
			Set_Nul(dtl->dtl->arv, FLD_DAY, FALSE);	
			Set_Nul(dtl->dtl->dpt, FLD_DAY, FALSE);	
			Set_Nul(dtl->dtl->eti, FLD_INT, FALSE);	
			*DBS_DAY_PTR(dtl->dtl->arv) = *cci->arv; 
			*DBS_DAY_PTR(dtl->dtl->dpt) = *cci->dpt; 
			*((int *)Fld_Ptr(dtl->dtl->eti, FLD_INT)) = cci->eti;
			
			if ( !!( ptr = Viw_Get_Rmn(mng->eil, cci->eii) ) ){
				if ( csr = Add_Lnk( dtl->req, NULL, sizeof(RSV_EQP_DAT) ) ) {
					req = Rec_Of(RSV_EQP_DAT, csr);
					Set_Nul(req->req->usd, FLD_DAY, FALSE);	
					Set_Nul(req->req->ued, FLD_DAY, FALSE);	
					Set_Nul(req->rno, FLD_WST, FALSE);	
					*DBS_DAY_PTR(req->req->usd) = *cci->arv; 
					*DBS_DAY_PTR(req->req->ued) = *cci->dpt; 
					wcscpy_s((WCHAR *)Fld_Ptr(req->rno, FLD_WST), FLD_WST_SIZ(req->rno), ptr );
				}
			}
		}
	}
	else{
		// New data addition to detail
		Set_Nul(inf->rsv->arv, FLD_DAY, FALSE);
		Set_Nul(inf->rsv->dpt, FLD_DAY, FALSE);
		*DBS_DAY_PTR(inf->rsv->arv) = *DBS_DAY_PTR(inf->rsv->dpt) = *mng->bdt; 
		Add_Dte( DBS_DAY_PTR(inf->rsv->dpt), 1 ); 

		if ( csr = Add_Lnk( rot, NULL, sizeof(RSV_DTL_DAT) ) ){
			dtl = Rec_Of(RSV_DTL_DAT, csr);
			set_dtl( dtl, DBS_DAY_PTR(inf->rsv->arv), DBS_DAY_PTR(inf->rsv->dpt) );
		}
	}


	if ( chk )	Rsv_Dsp_Dat( inf, rot, dbs, wnd, mng );

	return( TRUE );
}

int		Rsv_Inf_Gfs(						// Getting reservation_infos data from screen
RSV_INF_DAT		*inf,						// Getting reservation_infos
Csr				dtl,						// Getting reservation_details
CWnd			*wnd,						// Reservation view window
PMS_MNG			*mng )						// Property management System management structure
{
	int					chk, psn, grp, pth, agt, rnk;
	WCHAR				*req=NULL;
	CBookingPathCombo	*cbx;
	
	psn = CBX_GET_CHK(IDC_RADIO_PERSONAL,wnd);
	grp = CBX_GET_CHK(IDC_RADIO_GROUP,wnd);
	if ( chk = ( !!psn || !!grp ) ){
		*DBS_INT_PTR(inf->cli->ckd) = psn ? CLK_PERSONAL: CLK_GROUP;
		Set_Nul(inf->cli->ckd,FLD_INT,FALSE);
	}
	else{
		req = _T("ReservationType");
	}

	//IDC_EDIT_FIRSTARRIVAL
	//IDC_EDIT_LASTDEPARTUREL

	if ( chk && !( chk=!WIN_TXT_DBS(inf->cli->fnm, IDC_EDIT_FIRSTNAME, wnd) ) ){
		req = _T("FirstName");
	}
	if ( chk )	WIN_TXT_DBS(inf->cli->lnm, IDC_EDIT_FAMILYNAME, wnd);
	if ( chk )	WIN_TXT_DBS(inf->cli->adr1, IDC_EDIT_ADDRESS1, wnd);
	if ( chk )	WIN_TXT_DBS(inf->cli->adr2, IDC_EDIT_ADDRESS2, wnd);
	if ( chk )	WIN_TXT_DBS(inf->cli->tel1, IDC_EDIT_TELEPHONE1, wnd);
	if ( chk )	WIN_TXT_DBS(inf->cli->tel2, IDC_EDIT_TELEPHONE2, wnd);
	if ( chk )	WIN_TXT_DBS(inf->cli->fax, IDC_EDIT_FAXNUMBER, wnd);
	if ( chk )	WIN_TXT_DBS(inf->cli->eml1, IDC_EDIT_EMAIL1, wnd);
	if ( chk )	WIN_TXT_DBS(inf->cli->eml2, IDC_EDIT_EMAIL2, wnd);

	Set_Nul(inf->rsv->opc, FLD_WST, FALSE);		// To not NULL operator_code
	wcscpy_s(DBS_WST_PTR(inf->rsv->opc), FLD_WST_SIZ(inf->rsv->opc), mng->opc );
	Set_Nul(inf->rsv->brc, FLD_WST, FALSE);		// To not NULL branch_code
	wcscpy_s(DBS_WST_PTR(inf->rsv->brc), FLD_WST_SIZ(inf->rsv->brc), mng->brc );
	Set_Nul(inf->rsv->fcc, FLD_WST, FALSE);		// To not NULL facility_code
	wcscpy_s(DBS_WST_PTR(inf->rsv->fcc), FLD_WST_SIZ(inf->rsv->fcc), mng->fcc );
	Set_Nul(inf->rsv->nsw, FLD_INT, FALSE);		// To not NULL no show
	Set_Nul(inf->rsv->cxl, FLD_INT, FALSE);		// To not NULL canceled

	Set_Nul(inf->rsv->cis, FLD_INT, FALSE);		// To not NULL canceled
	*DBS_INT_PTR(inf->rsv->cis) = CIS_BEFORE_CHECKIN;

	if ( chk ){
		cbx = (CBookingPathCombo *)wnd->GetDlgItem(IDC_COMBO_BOOKINGPATH);
		cbx->GetItemIndex( &pth, &agt );
		if ( chk = pth!=-1 ){
			Set_Nul(inf->rsv->bfr,FLD_INT,FALSE);
			*DBS_INT_PTR(inf->rsv->bfr) = pth;
			if ( pth==VIA_AGENT ){
				Set_Nul(inf->rsv->aii,FLD_INT,FALSE);
				*DBS_INT_PTR(inf->rsv->aii) = agt;
			}
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

int		Rsv_Dtl_Gfs(						// Getting reservation_details data from screen
RSV_DTL_DAT		*dtl,						// Getting reservation_infos
CWnd			*wnd,						// Reservation view window
PMS_MNG			*mng )						// Property management System management structure
{
	int			chk, err;
	WCHAR		*rqi=NULL, *erm=NULL;
	Csr			csr=NULL;
	RSV_EQP_DAT	*req;

	WIN_TXT_DBS(dtl->cli->fnm, IDC_EDIT_FIRSTNAME_DETAIL, wnd);
	WIN_TXT_DBS(dtl->cli->lnm, IDC_EDIT_FAMILYNAME_DETAIL, wnd);
	if ( Nul_Fld(dtl->cli->id, FLD_INT) ){
		Set_Nul(dtl->cli->ckd, FLD_INT, FALSE );
		*DBS_INT_PTR(dtl->cli->ckd) = CLK_PERSONAL;
	}

	// arrival date and departure date
	chk = get_spd( DBS_DAY_PTR(dtl->dtl->arv), DBS_DAY_PTR(dtl->dtl->dpt), mng->bdt, TRUE, wnd ) != -1;

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
	int		eii;
	CEquipmentTypeCombo		*etc = (CEquipmentTypeCombo *)(wnd->GetDlgItem(IDC_COMBO_ROOMTYPE));
	if ( ( eii = etc->GetType( NULL ) ) != -1 ){
		Set_Nul(dtl->dtl->eti, FLD_INT, FALSE);		// To not NULL inhouse
		*DBS_INT_PTR(dtl->dtl->eti) = eii;
	}
	else{
		rqi = _T("RequestedRoomType");
	}

	WIN_TXT_DBS(dtl->dtl->nte, IDC_EDIT_NOTE, wnd);

	// Out of screen fields
	Set_Nul(dtl->dtl->inh, FLD_INT, FALSE);		// To not NULL inhouse
	Set_Nul(dtl->dtl->cxl, FLD_INT, FALSE);		// To not NULL canceled
	Set_Nul(dtl->dtl->opc, FLD_WST, FALSE);		// To not NULL operator_code
	wcscpy_s(DBS_WST_PTR(dtl->dtl->opc), FLD_WST_SIZ(dtl->dtl->opc), mng->opc );

	while( csr=Nxt_Lnk( dtl->req, csr ) ){
		req = Rec_Of(RSV_EQP_DAT, csr);
		Set_Nul(req->req->opc, FLD_WST, FALSE);		// To not NULL operator_code
		wcscpy_s(DBS_WST_PTR(req->req->opc), FLD_WST_SIZ(req->req->opc), mng->opc );
		Set_Nul(req->req->inh,FLD_INT,FALSE);
		*DBS_INT_PTR(req->req->inh) = 0;
	}

	if ( erm ){
		wnd->MessageBox( erm, 0, MB_ICONEXCLAMATION|MB_OK );
	}
	else if ( rqi ){
		WCHAR		buf[128];
		swprintf_s( buf, memcnt(buf), _T("%s is required item"), req );
		wnd->MessageBox( buf, 0, MB_ICONEXCLAMATION|MB_OK );
	}

	return( chk );
}

int		 Rsv_Sav_Dat(					// Save reservation data
RSV_INF_DAT			*inf,				// reservation_infos
Csr					rot,				// reservation_details
CWnd				*wnd,				// Parent window
CDatabase			*dbs,				// ODBC Database object
PMS_MNG				*mng )				// Property Management Sysytem structure
{
	int			chk;
	WCHAR		*erm=NULL;

	if ( chk = Rsv_Inf_Gfs( inf, rot, wnd, mng ) ){
		if ( chk = Rsv_Ins_Rsv( &erm, inf, rot, dbs, mng ) ){
		}
		else{
			if ( erm ){
				wnd->MessageBox( erm, 0, MB_ICONEXCLAMATION|MB_OK );
				free( erm );
			}
		}
	}

	return( chk );
}

void	Rsv_Dsp_Dtl(				// resrvation_details dispose
Csr			rot )					// root of reservation_details
{
	RSV_DTL_DAT	*dtl;
	RSV_EQP_DAT	*req;

	Csr			csr=NULL, cse=NULL;

	while( csr=Nxt_Lnk(rot, csr) ){
		dtl = Rec_Of(RSV_DTL_DAT, csr);
		while( cse=Nxt_Lnk( dtl->req, cse ) ){
			req = Rec_Of(RSV_EQP_DAT, cse);
			Del_All( req->rcg );
		}
		Del_All( dtl->req );
	}
	Del_All( rot );
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
	Csr				dtl=RSV_DTL_(wnd);

	SEL_ROW_(wnd) = 1;

	if ( chk = Rsv_Get_Inf( inf, rid, &erm, dbs, mng ) ){
		if ( erm )		free( erm );
		if ( chk = Rsv_Get_Dtl( dtl, DBS_INT_VAL(inf->rsv->id), &erm, dbs, mng ) ){
			((CChildFrame *)(wnd->GetParent()))->m_Rsv_id = rid;
			Rsv_Dsp_Dat( inf, dtl, dbs, wnd, mng );
		}
	}
	if ( erm ){
		wnd->MessageBox(erm/*_T("Faild to get a reservation data!")*/, 0, MB_ICONSTOP|MB_OK );
		free( erm );
	}

	return( chk );
}

