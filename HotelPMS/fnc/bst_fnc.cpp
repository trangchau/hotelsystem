/*
	予約状況処理関数
*/

#include "..\stdafx.h"
#include "..\HotelPMS.h"
#include "..\MainFrm.h"
#include "..\ChildFrm.h"
#include "..\ReservationView.h"
#include "..\CheckinView.h"
#include "..\InhouseView.h"
#include "..\CheckoutView.h"
#include "..\SelectPlanDlg.h"
#include "..\resource.h"
#include "bst_fnc.h"
#include "viw_cmn.h"
#include "bst_sql.h"
#include "cin_sql.h"
#include "ecg_sql.h"
#include "rsv_fnc.h"


static	int		cnv_pxl_(			// ポイント数→ピクセル数変換
HDC			hdc,					// 対象デバイスコンテクスト
int			pnt )					// 指定ポイント数
{
	double		dpi = (double)GetDeviceCaps(hdc, LOGPIXELSX);
	return( (int)(dpi / 72.0 * (double)pnt) );
}

HFONT		Bst_Cre_Fnt(			// 文字列用フォント作成
WCHAR		*fnm,					// フォント名
int			siz,					// フォントサイズ（ポイント）
int			bld,					// ボールド指定
int			itr,					// イタリック
HDC			hdc )					// 作成対象デバイスコンテクスト
{
	HFONT				fnh;			// フォントハンドル
	LOGFONT				fnt[1];		   	// フォント情報構造体

	memset( fnt, 0, sizeof(fnt) );
	fnt->lfCharSet = DEFAULT_CHARSET;		// SHIFTJIS_CHARSET;
	wcscpy_s( fnt->lfFaceName, _countof(fnt->lfFaceName), fnm );			// フォント名
	fnt->lfHeight = cnv_pxl_(hdc,siz) * -1;	// フォントサイズ
	fnt->lfWidth = cnv_pxl_(hdc,siz) / 2;	// フォントサイズ
	fnt->lfWeight = bld ? 700: 400;			// スタイル～ボールド（400,700)
	fnt->lfQuality = PROOF_QUALITY;			// 文字品質
	fnh = CreateFontIndirect( fnt );		// フォント作成

	return( fnh );
}

int			Bst_Get_Fsz(			// 文字サイズ（水平ピクセル数取得）
int			pnt,					// ポイント数
HDC			hdc )					// 計算対象デバイスコンテクスト
{
	return( cnv_pxl_(hdc,pnt) / 2 );
}

int		Bst_Get_Rom(				// 部屋一覧取得実行
Csr					rot,			// 格納領域
CBookingStatusView	*viw )
{
	WCHAR		*msg=NULL;
	int			chk;

	if ( !( chk = Bst_Rom_Lst( rot, &msg, viw->m_pDbs_obj, viw->m_pPms_mng ) ) ){
		if ( msg ){
			viw->MessageBox( msg, 0, MB_ICONSTOP|MB_OK );
		}
	}
	if ( msg )		free( msg );

	return( chk );
}

static	BST_ROM_LST	*sch_rom(		// Searching Room definition
Csr			rot,					// Room definition root
int			id )					// equipment_infos id
{
	Csr			csr=NULL;
	BST_ROM_LST	*rom;

	while ( csr = Nxt_Lnk( rot, csr ) ){
		rom = Rec_Of(BST_ROM_LST, csr);
		if ( DBS_INT_VAL(rom->id)==id )	break; 
	}

	return( !!csr ? rom: NULL );
}

int		Bst_Mak_Sts(
CBookingStatusView	*viw,
BST_MNG				*bst )
{
	Lnk			rot[1], *csr=NULL;
	WCHAR		*msg=NULL;
	int			chk, eid=0;
	BST_ROM_STS	*sts;
	BST_ROM_LST	*rom;

	while ( csr = Nxt_Lnk( bst->rom, csr ) ){
		Del_All( Rec_Of(BST_ROM_LST, csr)->bst );
	}

	memset( rot, 0, sizeof(rot) );
	if ( chk = Bst_Use_Lst( rot, viw->m_Bst_mng->std, viw->m_Bst_mng->edd, &msg, viw->m_pDbs_obj, viw->m_pPms_mng ) ){
		while ( csr = Nxt_Lnk( rot, csr ) ){
			sts = Rec_Of(BST_ROM_STS, csr);
			if ( DBS_INT_VAL(sts->req->eii) != eid ){
				eid = DBS_INT_VAL(sts->req->eii);
				rom = sch_rom( bst->rom, eid );
			}
			if ( !!rom ){
				sts->dtl = rom;
				chk = !!Add_Lnk( rom->bst, sts, sizeof(BST_ROM_STS) );
			}
		}
		Del_All(rot);
	}
	GetMainFrm()->SetBookingStsPeriod( viw->m_Bst_mng->std, viw->m_Bst_mng->edd );

	return( chk );
}

void	Bst_Pod_Chg(
CBookingStatusView	*viw,
BST_MNG				*bst )
{
	RECT		rct[1];
	CDC			*pDC = viw->GetDC();

	viw->GetClientRect( rct );

	Bst_Mak_Sts( viw, bst );
	bst->dys = Clc_Dys( bst->std, bst->edd );
	Rmk_Gph( rct, bst );
	Siz_Gph( bst, rct->right, rct->bottom, Cnt_Lnk(bst->rom), TRUE );
	bst->sys->top = -( viw->GetScrollPos( SB_VERT ) * ( GPH_HIT + 1 ) );
	Fig_Set_Swn( bst->hnd, bst->sys, TRUE );
	Drw_Rif( pDC, viw, bst );
	viw->ReleaseDC( pDC );
}

static	void	upd_gph(	// graphics update
BST_ROM_STS		*sts,		// target data
CWnd			*viw,		// parent window
BST_MNG			*bst )		// BookingStatusView management structure 
{
	Csr			csr=NULL, cse;
	BST_ROM_STS	*old;
	BST_ROM_LST	*rom;

	while( csr=Nxt_Lnk( bst->rom, csr ) ){
		rom = Rec_Of(BST_ROM_LST, csr);
		if ( !Nul_Fld(sts->req->eii, FLD_INT) && !DBS_INT_VAL(sts->cxl) ){
			if ( DBS_INT_VAL(rom->id) == DBS_INT_VAL(sts->req->eii) ){
				sts->dtl = rom;
				if ( !!( cse = Add_Lnk( rom->bst, sts, sizeof(BST_ROM_STS) ) ) ){
					Upd_Gph( Rec_Of(BST_ROM_STS, cse), bst );
				}
				for ( csr=NULL; csr=Nxt_Lnk( rom->bst, csr ); ){
					if ( csr != cse ){
						old = Rec_Of(BST_ROM_STS, csr);
						if ( DBS_INT_VAL(old->req->id) == DBS_INT_VAL(sts->req->id) ){
							Del_Lnk( rom->bst, csr );
							break;
						}
					}
				}
				break;
			}
		}
		else{
			for ( cse=NULL; cse=Nxt_Lnk( rom->bst, cse ); ){
				old = Rec_Of(BST_ROM_STS, cse);
				if ( DBS_INT_VAL(old->req->id) == DBS_INT_VAL(sts->req->id) ){
					Del_Gph( 0, DBS_INT_VAL(old->req->id), bst );
					Del_Lnk( rom->bst, cse );
					break;
				}
			}
			if ( !!cse )	break;
		}
	}
}

void	Bst_Upd_Gph(			// graphics update from each view
Csr					rot,		// user data					
CBookingStatusView	*viw,		// parent window
BST_MNG				*bst )		// BookingStatusView management structure 
{
	Csr			csr=NULL;

	while ( csr=Nxt_Lnk( rot, csr ) ){
		upd_gph( Rec_Of(BST_ROM_STS, csr), viw, bst );
	}
	Del_All( rot );
}

void	Bst_Ctx_Mnu(			// Open context menu on search view
CWnd		*wnd,				// Search view
int			msx,				// Mouse point x
int			msy,				// Mouse point y
LPVOID		usr,				// Pointer of BST_ROM_STS
PMS_MNG		*mng )				// HotenPMS management structure 
{
	CMenu		mnu, *pmn=NULL;
	POINT		pnt[1] = { msx, msy };
	BST_ROM_STS	*sts=(BST_ROM_STS *)usr;
	int			enb, flg;

	mnu.LoadMenu(IDR_MENU_BST);
	pmn = mnu.GetSubMenu(0);
//	wnd->ClientToScreen( pnt );

	// Condition of checkin menu enabling
	enb = Cmp_Dte( DBS_DAY_PTR(sts->arv), mng->bdt )>=0 && !DBS_INT_VAL(sts->cin) && !DBS_INT_VAL(sts->cot);
	flg = MF_BYCOMMAND|(enb ? MF_ENABLED: MF_DISABLED|MF_GRAYED);
	pmn->EnableMenuItem( ID_BOOKINGSTATUS_CANCELRESERVATIONDIRECT, flg );

	enb = Cmp_Dte( DBS_DAY_PTR(sts->arv), mng->bdt )==0 && !DBS_INT_VAL(sts->cin) && !DBS_INT_VAL(sts->cot);
	flg = MF_BYCOMMAND|(enb ? MF_ENABLED: MF_DISABLED|MF_GRAYED);
	pmn->EnableMenuItem( ID_BOOKINGSTATUS_CHECKIN, flg );
	pmn->EnableMenuItem( ID_BOOKINGSTATUS_CHECKINDIRECT, flg );

	// Condition of inhouse menu enabling 
	enb = !!DBS_INT_VAL(sts->cin) && !DBS_INT_VAL(sts->cot);
	flg = MF_BYCOMMAND|(enb ? MF_ENABLED: MF_DISABLED|MF_GRAYED);
	pmn->EnableMenuItem( ID_BOOKINGSTATUS_INHOUSESTATUS, flg );

	// Condition of checkout menu enabling
	enb = Cmp_Dte( DBS_DAY_PTR(sts->dpt), mng->bdt )<=0 && !!DBS_INT_VAL(sts->cin) && !DBS_INT_VAL(sts->cot);
	flg = MF_BYCOMMAND|(enb ? MF_ENABLED: MF_DISABLED|MF_GRAYED);
	pmn->EnableMenuItem( ID_BOOKINGSTATUS_CHECKOUT, flg );
	pmn->EnableMenuItem( ID_BOOKINGSTATUS_CHECKOUTDIRECT, flg );

	pmn->TrackPopupMenu( TPM_TOPALIGN/*TPM_LEFTALIGN*/|TPM_RIGHTBUTTON, pnt->x, pnt->y, wnd, NULL );
}

typedef	struct	{
	int			rii;		// reservation_info_id
	int			rdi;		// reservation_detail_id
	int			csh;		// raunch a casiher
}RPR_;

static	int		exv_cbk(
int		vkd,
CView	*viw,
LPVOID	prm )
{
	return( ((CChildFrame *)(viw->GetParent()))->m_Rsv_id == (int)(LONG_PTR)prm );
}

static	int		rvw_cbk(
int		vkd,
CView	*viw,
LPVOID	prm )
{
	int			chk=FALSE;
	RPR_		*rpr=(RPR_ *)(LONG_PTR)prm;

	switch( vkd ){
	case VWK_RSV:	chk = ((CReservationView *)viw)->GetData( rpr->rii );				break;
	case VWK_CIN:	chk = ((CCheckinView *)viw)->GetData( rpr->rii );					break;
	case VWK_INH:	chk = ((CInhouseView *)viw)->GetData( rpr->rii, rpr->rdi, FALSE );	break;
	case VWK_COT:	chk = ((CCheckoutView *)viw)->GetData( rpr->rii, rpr->rdi );		break;
	}

	return( chk );
}

void	Bst_Rch_Viw(			// Launch each view
DWORD		mid,				// Menu ID
int			rii,				// ID of reservation_infos
int			rdi )				// ID of reservation_details
{
	int			vwk=-1;
	RPR_		rpr[1];

	rpr->rii = rii;
	rpr->rdi = rdi;
	rpr->csh = FALSE;

	if ( rii ){
		switch( mid ){
		case ID_BOOKINGSTATUS_RESERVATIONVIEW:	vwk = VWK_RSV;	break;
		case ID_BOOKINGSTATUS_CHECKIN:			vwk = VWK_CIN;	break;
		case ID_BOOKINGSTATUS_INHOUSESTATUS:	vwk = VWK_INH;	break;
		case ID_BOOKINGSTATUS_CHECKOUT:			vwk = VWK_COT;	break;
		}
		if ( vwk != -1 )	Rnc_Ech_Viw(vwk, exv_cbk, rvw_cbk, (LPVOID)(LONG_PTR)rpr);
	}
}

static	int		crv_cbk(
int		vkd,
CView	*viw,
LPVOID	prm )
{
	GPH_RDY		*rdy=(GPH_RDY *)(LONG_PTR)prm;
	CCI_PRM		cci[1];

	memset( cci, 0, sizeof(cci) );
	cci->eii = DBS_INT_VAL(rdy->rom->id);
	cci->eti = DBS_INT_VAL(rdy->rom->eti);
	cci->esi = DBS_INT_VAL(rdy->rom->esi);
	*cci->arv = *rdy->nsd;
	*cci->dpt = *rdy->ned;

	return( ((CReservationView *)viw)->CreateData( cci ) );
}

void	Bst_Rch_Crv(				// Launch create a new reservation view
GPH_RDY *rdy )
{
	Rnc_Ech_Viw(VWK_RSV, NULL, crv_cbk, (LPVOID)(LONG_PTR)rdy);
}

static	int		get_rcg(		// confirm room charge is available
Csr			dst,				// storing root
int			eci,				// current equipmnent_chage_info_id
BST_ROM_STS	*sts,				// bookingstatus data
GPH_RDY		*rdy,				// Chainging specification
CWnd		*viw,				// parent window
CDatabase	*dbs,				// ODBC Database object
BST_MNG		*bst )				// BookingStatusView management structure 
{
	ECG_SQL_PRM	prm[1];
	ECG_PLN_DAT	*pln=NULL;
//	ECG_DTL_DAT	*dtl;
	RSV_CRG		*crg;
	int			chk, cnt, val=0, idx;
	double		amt;
	Day			usd[1];
	Lnk			rot[1], *csr=NULL, *cse;
	WCHAR		*erm=NULL;

	memset( prm, 0, sizeof(prm) );
	prm->bfr = DBS_INT_VAL(sts->bfr);
	prm->agi = Nul_Fld(sts->aii, FLD_INT) ? 0: DBS_INT_VAL(sts->aii);
	prm->eti = DBS_INT_VAL(rdy->rom->eti);
	prm->std = rdy->nsd;
	prm->edd = rdy->ned;
	prm->opc = bst->mng->opc;
	prm->brc = bst->mng->brc;
	prm->fcc = bst->mng->fcc;

	memset( rot, 0, sizeof(rot) );

	chk = Ecg_Chg_Crg( rot, prm, &erm, dbs, bst->mng ) > 0;

	if ( chk && ( chk = !!Cnt_Lnk(rot) ) ){
		while( csr=Nxt_Lnk( rot, csr ) ){
			pln = Rec_Of(ECG_PLN_DAT, csr);
			if ( DBS_INT_VAL(pln->eci) == eci )	break;
		}
		if ( !csr ){
			CSelectPlanDlg	*dlg = new CSelectPlanDlg( viw );
			if ( dlg ){
				dlg->m_Pln_lst = rot;
				dlg->DoModal();
				pln = dlg->m_Sel_pln;
				delete	dlg;
			}
		}
		if ( chk = !!pln ){		// making reservation_charge_details
			cnt = Clc_Dys(rdy->nsd, rdy->ned);
			*usd = *(rdy->nsd);
			for ( idx=0; chk && idx<cnt; idx++ ){
				if ( chk = !!( cse = Add_Lnk( dst, NULL, sizeof(RSV_CRG) ) ) ){
					crg = Rec_Of(RSV_CRG, cse);
					DBS_WST_SET(crg->opc, prm->opc);
					DBS_INT_CPY(crg->rii, sts->req->rii);
					DBS_INT_CPY(crg->rdi, sts->req->rdi);
					DBS_INT_CPY(crg->rei, sts->req->id);
					DBS_DAY_SET(crg->udt, usd);
					DBS_INT_CPY(crg->cgk, pln->cgk);
					DBS_INT_CPY(crg->dsi, pln->dsi);
					DBS_INT_SET(crg->pup, &val);
					amt = Rsv_Apl_Amt(NULL, crg, usd, pln, bst->mng);
					DBS_DBL_SET(crg->amt, &amt);
					Add_Dte( usd, 1 );
				}
			}
			if ( !chk )	Del_All( dst );
		}
	}
	else{
		viw->MessageBox(_T("Because there is no price setting on this reservation")
						_T("condition,\ncan not to change."), NULL, MB_ICONEXCLAMATION|MB_OK );
	}
	if ( erm ){
		viw->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
		free( erm );
	}

	csr=Nxt_Lnk( rot, NULL );
	while( csr ){
		Del_All( Rec_Of(ECG_PLN_DAT, csr)->dtl );
		csr = Del_Lnk( rot, csr );
	}
	return( chk );
}

static	int		has_rcg(		// confirm for have some room changes
int			*rmc,				// having some room chamges
BST_ROM_STS	*sts,				// bookingstatus data
GPH_RDY		*rdy,				// Chainging specification
CWnd		*viw,				// parent window
CDatabase	*dbs,				// ODBC Database object
BST_MNG		*bst )				// BookingStatusView management structure 
{
	int			chk=TRUE, dif;
//	RSV_DTL		dtl[1];
	WCHAR		*erm=NULL;

//	if ( chk = Rsv_Get_Rdt( dtl, DBS_INT_VAL(sts->req->rdi), &erm, dbs, bst->mng ) ){
//		*rmc  = !!Cmp_Dte(DBS_DAY_PTR(sts->req->usd), DBS_DAY_PTR(dtl->arv) ); 
//		*rmc |= !!Cmp_Dte(DBS_DAY_PTR(sts->req->ued), DBS_DAY_PTR(dtl->dpt) ); 
//	}
//	else{
//		if ( erm ){
//			viw->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
//			free( erm );
//		}
//	}
	*rmc  = !!Cmp_Dte(rdy->osd, DBS_DAY_PTR(sts->arv) ); 
	*rmc |= !!Cmp_Dte(rdy->oed, DBS_DAY_PTR(sts->dpt) ); 
	if ( *rmc ){
		dif  = !!Cmp_Dte(rdy->osd, rdy->nsd ); 
		dif |= !!Cmp_Dte(rdy->oed, rdy->ned ); 
		if ( dif ){
			viw->MessageBox( _T("Because this reservation has some room changes,\n")
							_T("can not change date of room using"), NULL, MB_ICONEXCLAMATION|MB_OK );
		}
		chk = !dif;
	}

	return( chk );
}

static	int		psb_chg(		// Confirm to change date is possible
BST_ROM_STS	*sts,				// bookingstatus data
GPH_RDY		*rdy,				// Chainging specification
CWnd		*viw,				// parent window
BST_MNG		*bst )				// BookingStatusView management structure 
{
	int			chk=TRUE;
	WCHAR		*msg=NULL;
	Csr			csr=NULL, cse=NULL;
	BST_ROM_STS	*otr;

	if ( chk = Cmp_Dte(rdy->osd, bst->mng->bdt) >= 0 ){
		if ( !!DBS_INT_VAL(sts->cin) ){
			if ( !Cmp_Dte( rdy->osd, DBS_DAY_PTR(sts->arv) ) ){
				if ( !( chk = !Cmp_Dte(rdy->osd, rdy->nsd ) ) ){
					msg = _T("Because this room already checked in,\n")
						_T("can not change the start date of room using.");
				}
			}
		}
	}
	else{
		msg = _T("Because this reservation is past,\ncan not change any dondition.");
	}
	if ( chk ){
		// check overlap to other reservation
		while( csr=Nxt_Lnk( rdy->rom->bst, csr ) ){
			otr = Rec_Of(BST_ROM_STS, csr);
			if ( DBS_INT_VAL(otr->req->id) != DBS_INT_VAL(sts->req->id) ){
				if ( ( Cmp_Dte( rdy->nsd, DBS_DAY_PTR(otr->req->usd) ) >= 0  
					&& Cmp_Dte( rdy->nsd, DBS_DAY_PTR(otr->req->ued) ) < 0 ) 
					||( Cmp_Dte( rdy->ned, DBS_DAY_PTR(otr->req->usd) ) >= 0  
					&& Cmp_Dte( rdy->ned, DBS_DAY_PTR(otr->req->ued) ) < 0 ) ){
					break;
				}
			}
		}
		if ( !( chk = !csr ) ){
			msg = _T("Competing to other reservation." );
		}
	}

	if ( !chk ){
		viw->MessageBox( msg, NULL, MB_ICONEXCLAMATION|MB_OK );
	}

	return( chk );
}

static	BST_ROM_STS	*cpy_sts(	// booking status copy
BST_ROM_STS	*sts,				// bookingstatus data
GPH_RDY		*rdy,				// Chainging specification
BST_MNG		*bst )				// BookingStatusView management structure 
{
	Csr			csr=NULL;
	
	if ( csr = Add_Lnk( rdy->rom->bst, sts, sizeof(BST_ROM_STS) ) ){
		sts = Rec_Of(BST_ROM_STS, csr);
		*sts->req->usd = *rdy->nsd;
		*sts->req->ued = *rdy->ned;
		sts->cut = 0;
		rdy->elm->usr = sts;
	}

	return( csr ? sts: NULL );
}

static	int	doc_rom(			// docking the tearing room
int			rdi,				// reservation_detail_id
CWnd		*viw,				// parent window
CDatabase	*dbs,				// ODBC Database object
BST_MNG		*bst )				// BookingStatusView management structure 
{
	int			chk;
	WCHAR		*erm=NULL;
	Csr			csr=NULL, cse;
	BST_TRG_ROM	trg[1];
	BST_ROM_LST	*rom;
	BST_ROM_STS	*sts;

	if ( chk = Bst_Get_Trg( trg, rdi, &erm, dbs, bst->mng ) ){
		if ( trg->exs ){
			DBS_DAY_CPY(trg->req[0].ued, trg->req[1].ued);
			if ( chk = Bst_Doc_Trg( trg, &erm, dbs, bst->mng ) ){
				while( csr=Nxt_Lnk( bst->rom, csr ) ){
					rom = Rec_Of(BST_ROM_LST, csr);
					for ( cse=NULL; cse=Nxt_Lnk( rom->bst, cse ); ){
						sts = Rec_Of(BST_ROM_STS, cse);
						if ( DBS_INT_VAL(sts->req->id)==DBS_INT_VAL(trg->req[0].id) ){
							*sts->req = *trg->req;
							Upd_Gph( sts, bst );
							break;
						}
						else if ( DBS_INT_VAL(sts->req->id)==DBS_INT_VAL(trg->req[1].id) ){
							Del_Gph( 0, DBS_INT_VAL(sts->req->id), bst );
							Del_Lnk( rom->bst, cse );
							break;
						}
					}
				}
			}
		}
	}
	else{
		if ( erm ){
			viw->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
	}

	return( chk );
}

int		Bst_Rsv_Chg(			// Chainging a reservation from graph
BST_ROM_STS	*sts,				// bookingstatus data
GPH_RDY		*rdy,				// Chainging specification
CWnd		*viw,				// parent window
CDatabase	*dbs,				// ODBC Database object
BST_MNG		*bst )				// BookingStatusView management structure 
{
	int			chk, eci, rmc, ori;
	WCHAR		msg[1024], bfb[20], btb[20], afb[20], atb[20], *erm=NULL;
	RSV_EQP		req[1];
	Lnk			rot[1], nrt[1];
	Day			upd[1];
	BST_ROM_STS	*ost=NULL, stp[1];

	Get_Dte( upd );

	memset( rot, 0, sizeof(rot) );
	memset( nrt, 0, sizeof(nrt) );

	swprintf_s( msg, memcnt(msg),
		_T("Do you want to make the following changes?\n")
		_T("Rsv.ID	: %d(E.ID=%s)\n")
		_T("Name	: %s %s\n")
		_T("Before	: %s-%s %s(%s)\n")
		_T("After	: %s-%s %s(%s)\n"),
		DBS_INT_VAL(sts->rvn), EDT_DBF_WST(sts->ern,_T("None")),
		EDT_DBF_WST(sts->fnm,_T("")),EDT_DBF_WST(sts->lnm,_T("")),
		Edt_Dte(bfb, memcnt(bfb), rdy->osd ),
		Edt_Dte(btb, memcnt(btb), rdy->oed ),
		DBS_WST_PTR(sts->rno),DBS_WST_PTR(sts->dtl->tpn),
		Edt_Dte(afb, memcnt(afb), rdy->nsd ),
		Edt_Dte(atb, memcnt(atb), rdy->ned ),
		DBS_WST_PTR(rdy->rom->cod), DBS_WST_PTR(rdy->rom->tpn) );

	if ( chk = psb_chg( sts, rdy, viw, bst ) ){		// Confirm to change date is possible
		if ( chk = has_rcg( &rmc, sts, rdy, viw, dbs, bst ) ){		// confirm for have some room changes
			if ( viw->MessageBox( msg, NULL, MB_ICONQUESTION|MB_YESNO ) == IDYES ){
				eci = Rsv_Get_Eci( DBS_INT_VAL(sts->req->id), &erm, dbs, bst->mng );
				if ( chk = get_rcg( rot, eci, sts, rdy, viw, dbs, bst ) ){
					*req = *sts->req;
					if( sts->cut > 0 ){
						ori = DBS_INT_VAL(sts->req->id);
						Set_Nul(req->id, FLD_INT, TRUE );
					}
					if ( chk ){
						DBS_DAY_SET(req->usd, rdy->nsd);
						DBS_DAY_SET(req->ued, rdy->ned);
						DBS_INT_SET(req->eii, DBS_INT_PTR(rdy->rom->id));
						if ( sts->cut > 0 ){
							sts->cut--;
							chk = Rsv_Cpy_Req( req, rot, ori, upd, &erm, dbs, bst->mng );
							if ( chk ){
								if ( Cmp_Dte(DBS_DAY_PTR(sts->req->usd),rdy->nsd)<0){
									*DBS_DAY_PTR(sts->req->ued)=*rdy->nsd;
								}
								else if ( Cmp_Dte(DBS_DAY_PTR(sts->req->ued),rdy->ned)>0){
									*DBS_DAY_PTR(sts->req->usd)=*rdy->ned;
								}
								chk = !!( sts = cpy_sts( ost=sts, rdy, bst ) );
								DBS_INT_CPY(sts->req->id, req->id);
							}
						}
						else{
							chk = Rsv_Chg_Req( req, rot, rmc, upd, &erm, dbs, bst->mng );
							if ( chk ){
								DBS_DAY_SET(sts->arv, rdy->nsd); 
								DBS_DAY_SET(sts->dpt, rdy->ned); 
							}
						}
						if ( chk ){
							DBS_DAY_SET(sts->req->usd, rdy->nsd); 
							DBS_DAY_SET(sts->req->ued, rdy->ned); 
							DBS_INT_CPY(sts->req->eti, rdy->rom->eti); 
							DBS_INT_CPY(sts->req->eii, rdy->rom->id); 
							sts->dtl = rdy->rom;
							*stp = *sts;
							upd_gph( stp, viw, bst );
							if ( ost ){
								*stp = *ost;
								upd_gph( stp, viw, bst );
							}
							//	Viw_Snd_Ntf( VIW_SND_BST, VWN_RSV_CHANGED, nrt, Cnt_Lnk(nrt), viw );	// send message because this with memory free here
							Viw_Pst_Ntf( VIW_SND_SCH, VWN_RSV_CHANGED, NULL, DBS_INT_VAL(sts->req->rii), viw );
							Viw_Pst_Ntf( VIW_SND_NAD, VWN_RSV_CHANGED, NULL, DBS_INT_VAL(sts->req->rii), viw );
						}
						else{
							if ( erm ){
								viw->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
								free( erm );
							}
						}
					}
					else{
						viw->MessageBox( _T("System error.Not enough memory"), NULL, MB_ICONEXCLAMATION|MB_OK );
					}
				}
				Del_All( rot );
			}
		}
	}
	Del_All( rot );
	Del_All( nrt );

	if ( chk ){
		chk = doc_rom( DBS_INT_VAL(sts->req->rdi), viw, dbs, bst );
	}
	return( chk );
}

void	Bst_Cin_Dir(			// Execute checkin direct
BST_ROM_STS	*sts,				// Target rom status
CWnd		*viw,				// parent window
CDatabase	*dbs,				// ODBC Database object
BST_MNG		*bst )				// BookingStatusView management structure 
{
	WCHAR			*msg, *erm=NULL;
	int				chk, val, rei, rdi, esi;

	rei = DBS_INT_VAL(sts->req->id);
	rdi = DBS_INT_VAL(sts->req->rdi);
	esi = DBS_INT_VAL(sts->esi);

	if ( chk = Cin_Exc_Cin( rei, rdi, esi, &erm, dbs, bst->mng ) ){
		DBS_INT_SET(sts->cin, &(val = TRUE ) );
		DBS_INT_SET(sts->req->inh, &(val = TRUE) );
		msg = _T("Checkin successed");
		Viw_Pst_Ntf( VIW_SND_SCH, VWN_RSV_CHANGED, NULL, DBS_INT_VAL(sts->req->rii), viw );
		Viw_Pst_Ntf( VIW_SND_NAD, VWN_RSV_CHANGED, NULL, DBS_INT_VAL(sts->req->rii), viw );
	}
	else{
		if ( erm )	msg = erm;
	}
	if ( msg )	viw->MessageBox( msg, NULL, MB_ICONINFORMATION|MB_OK );
	if ( erm )	free( erm );
}
/*
void	Bst_Cxl_Dir(			// Execute cancellation direct
BST_ROM_STS	*sts,				// Target rom status
CWnd		*viw,				// parent window
CDatabase	*dbs,				// ODBC Database object
BST_MNG		*bst )				// BookingStatusView management structure 
{
	WCHAR			*msg, *erm=NULL;
	int				chk;

	if ( viw->MessageBox( _T("Are you sure to cancel this reservation?"), NULL, MB_ICONQUESTION|MB_YESNO )==IDYES ){
		if ( chk = Rsv_Idv_Cxl( DBS_INT_VAL(sts->req->rdi), DBS_INT_VAL(sts->req->rii), &erm, dbs, bst->mng ) ){
			//DBS_INT_SET(sts->cin, &(val = TRUE ) );
			//DBS_INT_SET(sts->req->inh, &(val = TRUE) );
			Del_Gph( DBS_INT_VAL(sts->req->rdi), 0, bst );
			msg = _T("Cancellation successed");
			Viw_Pst_Ntf( VIW_SND_SCH, VWN_RSV_CHANGED, NULL, DBS_INT_VAL(sts->req->rii), viw );
			Viw_Pst_Ntf( VIW_SND_NAD, VWN_RSV_CHANGED, NULL, DBS_INT_VAL(sts->req->rii), viw );
		}
		else{
			if ( erm )	msg = erm;
		}
		if ( msg )	viw->MessageBox( msg, NULL, MB_ICONINFORMATION|MB_OK );
		if ( erm )	free( erm );
	}
}
*/
void	Bst_Cxl_Dir(			// Execute cancellation direct
BST_ROM_STS	*sts,				// Target rom status
CWnd		*viw,				// parent window
CDatabase	*dbs,				// ODBC Database object
BST_MNG		*bst )				// BookingStatusView management structure 
{
	int			rii;
	WCHAR		buf[2014], bfb[20], btb[20], *erm=NULL, *msg=NULL;
	BST_ROM_LST	*rom;
	BST_ROM_STS	*ost;
	Csr			csr=NULL, cse;
	
	if ( !DBS_INT_VAL(sts->cin) && !DBS_INT_VAL(sts->cot) && !DBS_INT_VAL(sts->cxl) ){
		swprintf_s( buf, memcnt(buf),
			_T("Do you want to cancel following reservation?\n")
			_T("Rsv.ID	: %d(E.ID=%s)\n")
			_T("Name	: %s %s\n")
			_T("Stay	: %s-%s"),
			DBS_INT_VAL(sts->rvn), EDT_DBF_WST(sts->ern,_T("None")),
			EDT_DBF_WST(sts->fnm,_T("")),EDT_DBF_WST(sts->lnm,_T("")),
			Edt_Dte(bfb, memcnt(bfb), DBS_DAY_PTR(sts->arv) ),
			Edt_Dte(btb, memcnt(btb), DBS_DAY_PTR(sts->dpt) ) );
		if ( viw->MessageBox( buf, NULL, MB_ICONQUESTION|MB_YESNO )==IDYES ){
			if ( Rsv_Idv_Cxl( DBS_INT_VAL(sts->req->rdi), rii = DBS_INT_VAL(sts->req->rii), &erm, dbs, bst->mng ) ){
				//DBS_INT_SET(sts->cin, &(val = TRUE ) );
				//DBS_INT_SET(sts->req->inh, &(val = TRUE) );
				Del_Gph( DBS_INT_VAL(sts->req->rdi), 0, bst );
				while( csr = Nxt_Lnk( bst->rom, csr ) ){
					rom = Rec_Of(BST_ROM_LST, csr);
					cse = Nxt_Lnk( rom->bst, NULL );
					while( cse ){
						ost = Rec_Of(BST_ROM_STS, cse);
						if (  DBS_INT_VAL(sts->req->rdi)== DBS_INT_VAL(ost->req->rdi) ){
							cse = Del_Lnk( rom->bst, cse );
						}
						else{
							cse = Nxt_Lnk( rom->bst, cse );
						}
					}
				}
				msg = _T("Cancellation successed");
				Viw_Pst_Ntf( VIW_SND_SCH, VWN_RSV_CHANGED, NULL, rii, viw );
				Viw_Pst_Ntf( VIW_SND_NAD, VWN_RSV_CHANGED, NULL, rii, viw );
			}
			else{
				if ( erm )	msg = erm;
			}
			if ( msg )	viw->MessageBox( msg, NULL, MB_ICONINFORMATION|MB_OK );
			if ( erm )	free( erm );
		}
	}
	else{
		swprintf_s( buf, memcnt(buf),
			_T("Because reservation already %s, can not cancel."),
			!!DBS_INT_VAL(sts->cxl) ? _T("canceled"): ( !!DBS_INT_VAL(sts->cot) ? _T("checked out"): _T("checked in") ) );
		viw->MessageBox( buf, NULL, MB_ICONEXCLAMATION|MB_OK );
	}
}

void	Bst_Cxl_Rsv(			// Cancel reservation
CWnd		*viw,				// parent window
CDatabase	*dbs,				// ODBC Database object
BST_MNG		*bst )				// BookingStatusView management structure 
{
	Csr			csr=NULL;
	Fig_elm		*elm;
	BST_ROM_STS	*sts;

	while ( csr = Nxt_Lnk( bst->rot, csr ) ){
		elm = Rec_Of(Fig_elm, csr);
		if ( elm->sel ){
			sts = (BST_ROM_STS *)elm->usr;
			Bst_Cxl_Dir( sts, viw, dbs, bst );
			break;
		}
	}
}

void	Bst_Upd_Sts(			// Booking status update
int			rei,				// reservation_equipment_id
int			knd,				// update detail
CWnd		*viw,				// parent window
BST_MNG		*bst )				// BookingStatusView management structure 
{
	Csr			csr=NULL, cse=NULL;
	BST_ROM_LST	*rom;
	BST_ROM_STS	*sts;
	int			val;

	while( csr=Nxt_Lnk( bst->rom, csr ) ){
		rom = Rec_Of(BST_ROM_LST, csr);
		while( cse=Nxt_Lnk( rom->bst, cse ) ){
			sts = Rec_Of(BST_ROM_STS, cse);
			if ( DBS_INT_VAL(sts->req->id) == rei ){
				switch( knd ){
				case STS_CHG_CHECKIN:	DBS_INT_SET(sts->cin, &(val=TRUE) );	break;	
				case STS_CHG_CHECKOUT:	DBS_INT_SET(sts->cot, &(val=TRUE) );	break;
				}
				break;
			}
		}
	}
}

void	Bst_Dps_Dat(				// Dispose room statis data casting
Csr			rot )					// Root of BST_ROM_LST
{
	Csr			csr=Nxt_Lnk( rot, NULL);
	BST_ROM_LST	*rom;

	while( csr ){
		rom = Rec_Of(BST_ROM_LST, csr);
		Del_All(rom->bst);
		csr = Del_Lnk( rot, csr );
	}
}

