/*
	ホテルプロパティ管理システム
	検索ウィンドウ用データベースアクセス関数
*/

#include <afx.h>
#include <afxdb.h>
#include "..\..\Common\bsc_def.h"
#include "..\..\Common\sys_def.h"
#include "..\..\Common\dbs_mng.h"
#include "..\..\Common\cmn_fnc.h"
#include "..\..\Common\sys_log.h"

#include "..\..\pms_def.h"
#include "sql_cmn.h"
#include "nad_sql.h"
#include "fld_def.h"

int		Nad_Wrn_Lst(				// gelling list of warning( Not checkin yet, Not chheckout yet )
Csr				rot,				// root of storing area
WCHAR			*opc,				// operator_code
WCHAR			*brc,				// branch_code
WCHAR			*fcc,				// facility_code
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	WCHAR		sql[2048], cnd[192], opb[96], brb[96], fcb[96], dbf[20];
	NAD_WRN_LST	wrn[1];
	int			chk=TRUE, pos, idx;
	Lnk			brt[1], *csr=NULL;

	Dbs_fld		fld[] = {	{ wrn->rii, _T("reservation_info_id"),			FLD_INT, FLD_INT_SIZ(wrn->rii) },
							{ wrn->rdi, _T("id"),							FLD_INT, FLD_INT_SIZ(wrn->rdi) },
							{ wrn->rvn, _T("reservation_number"),			FLD_INT, FLD_INT_SIZ(wrn->rvn) },
							{ wrn->eii, _T("equipment_info_id"),			FLD_INT, FLD_INT_SIZ(wrn->eii) },
							{ wrn->rei, _T("reservation_equipment_id"),		FLD_INT, FLD_INT_SIZ(wrn->rei) },
							{ wrn->ern, _T("external_reservation_number"),	FLD_WST, FLD_WST_SIZ(wrn->ern) },
							{ wrn->rno, _T("equipment_code"),				FLD_WST, FLD_WST_SIZ(wrn->rno) },
							{ wrn->rfn, _T("ICI_first_name"),				FLD_WST, FLD_WST_SIZ(wrn->rfn) },
							{ wrn->rln, _T("ICI_last_name"),				FLD_WST, FLD_WST_SIZ(wrn->rln) },
							{ wrn->dfn, _T("DCI_first_name"),				FLD_WST, FLD_WST_SIZ(wrn->dfn) },
							{ wrn->dln, _T("DCI_last_name"),				FLD_WST, FLD_WST_SIZ(wrn->dln) },
							{ wrn->arv, _T("arrival_date"),					FLD_DAY, FLD_DAY_SIZ(wrn->arv) },
							{ wrn->dpt, _T("departure_date"),				FLD_DAY, FLD_DAY_SIZ(wrn->dpt) }};

	memset( wrn, 0, sizeof(wrn) );
	memset( brt, 0, sizeof(brt) );

	for ( idx=0; chk && idx<2; idx++ ){
		swprintf_s( cnd, memcnt(cnd),
			!idx ?	_T("( CAST(RD.arrival_date AS DATE)='%s' AND CAST(RD.departure_date AS DATE) > '%s' AND RD.inhouse = 0 )"):
					_T("( CAST(RD.departure_date AS DATE)<='%s' AND RD.inhouse = 1 )"),
			Edt_Dte( dbf, memcnt(dbf), mng->bdt ), dbf );

		pos = swprintf_s( sql,	memcnt(sql),
			_T("SELECT ICI.first_name AS ICI_first_name, ICI.last_name AS ICI_last_name, ")
			_T("DCI.first_name AS DCI_first_name, DCI.last_name AS DCI_last_name, ")
			_T("EI.equipment_code, RD.*, RI.reservation_number, RI.external_reservation_number,")
			_T("EI.id AS equipment_info_id, RE.id AS reservation_equipment_id ")
			_T("FROM %s RD ")												// reservation_details
			_T("LEFT JOIN %s RI ON RI.id = RD.reservation_info_id ")		// reservation_infos
			_T("LEFT JOIN %s RE ON RE.reservation_detail_id = RD.id  ")		// reservation_equipments
			_T("AND RE.use_start_date = RD.arrival_date ")
			_T("LEFT JOIN %s EI ON EI.id = RE.equipment_info_id ")			// equipment_infos
			_T("LEFT JOIN %s ICI ON ICI.id = RI.client_info_id ")			// client_infos
			_T("LEFT JOIN %s DCI ON DCI.id = RD.client_info_id ")			// client_infos
			_T("WHERE %s ")
			_T("AND RD.canceled=0 AND RD.deleted_date IS NULL AND RI.no_show=0 ")
			_T("AND RI.operator_code='%s' AND RI.branch_code='%s' AND RI.facility_code='%s' "),
			TBN_RSV_DTL, TBN_RSV_INF, TBN_RSV_EQP, TBN_EQP_INF, TBN_CLI_INF, TBN_CLI_INF, cnd,
			SQL_Edt_Wst(opb, memcnt(opb), mng->opc), SQL_Edt_Wst(brb, memcnt(brb), mng->brc),
			SQL_Edt_Wst(fcb, memcnt(fcb), mng->fcc)	);

		if ( chk = SQL_Get_Lst( !idx ? rot: brt, sql, fld, memcnt(fld), wrn, sizeof(wrn), erm, dbc ) ){
			while ( csr=Nxt_Lnk( !idx ? rot: brt, csr ) ){
				Rec_Of(NAD_WRN_LST, csr)->sts = !idx ? NAS_NCI: NAS_NCO;
			}
		}
	}

	if ( chk )	Jin_Lnk( rot, brt, NULL );

	return( chk );
}


int		Nad_Nsw_Exc(				// Make reservation NoShow
int				rii,				// reservation_info_id
Day				*upd,				// Update datetime
WCHAR			**erm,				// stioring pointer of error message
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk, cnt[DBF_SIZ_INT];
	WCHAR		sql[512], dbf[20]; 
	WCHAR		*msg =	_T("Because there is another room that has already been checked in,\n")
						_T("can not make that NoShow this reservation");
	Dbs_fld		fld[1] = { cnt, _T("checkin_count"), FLD_WSI, FLD_INT_SIZ(cnt) };
	CRecordset	*rec;

	swprintf_s( sql, memcnt(sql),
		_T("SELECT COUNT(*) AS checkin_count ")
		_T("FROM %s ")							// reservation_details 
		_T("WHERE reservation_info_id=%d ")
		_T("AND inhouse!=0 AND canceled=0 AND deleted_date IS NULL" ),
		TBN_RSV_DTL, rii );
	if ( chk = !!( rec = Dbs_Qry( dbs, sql, erm ) ) ){
		chk = Dbs_Get( fld, erm, memcnt(fld), rec ) == QRY_NML;
		delete	rec;
	}
	if ( chk ){
		if ( DBS_INT_VAL(cnt)==0 ){
			if ( chk = Dbs_Trn( erm, dbs, TRN_STT ) ){
				swprintf_s( sql, memcnt(sql),
					_T("UPDATE %s SET no_show=1,")							// reservation_infos
					_T("modified_employee_id=%d, modified_date='%s' ")
					_T("WHERE id=%d "),
					TBN_RSV_INF, DBS_INT_VAL(mng->lgi->id), Edt_Dte(dbf, memcnt(dbf), upd), rii );
				chk = Dbs_Upd( dbs, sql, erm ) == QRY_NML;
				if ( chk ){
					swprintf_s( sql, memcnt(sql),
						_T("UPDATE %s SET noshow_count=noshow_count+1,")	// client_infos
						_T("modified_employee_id=%d, modified_date='%s' ")
						_T("WHERE id= ( SELECT client_info_id FROM %s ")	// reservation_infos
						_T("WHERE id=%d )"),
						TBN_CLI_INF, DBS_INT_VAL(mng->lgi->id), dbf, TBN_RSV_INF, rii );
					chk = Dbs_Upd( dbs, sql, erm ) == QRY_NML;
				}
				Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
			}
		}
		else{
			*erm = _wcsdup( msg );
			chk = -1;
		}
	}

	return( chk );
}

static	int	get_rcg(					// getting room charge data
Csr				rot,					// root of storing area
WCHAR			**erm,					// stioring pointer of error message
CDatabase		*dbs,					// Database ODBC object
PMS_MNG			*mng )					// HotelPMS system management segment
{
	int			chk, rdi=0;
	Lnk			tmp[1], *csr=NULL, *csn;
	NAD_SAL_INF	*sal=NULL;
	WCHAR		sql[2190], sld[20], opb[96], brb[96], fcb[96], *sls, *rcs;
	Mlt_dbf		mdf[2];
	struct	rec_ {
		SAL_INF		sal[1];					// sales_infos
		NAD_RCG_INF	rcg[1];					// reservation_charges information
		int			eii[DBF_SIZ_INT];		// equipment_info_id
		int			rii[DBF_SIZ_INT];		// reservation_info_id
		int			rdi[DBF_SIZ_INT];		// reservation_detail_id
		WCHAR		flg[DBF_SIZ_WST(30)];	// folio_language
	}rec[1], *ptr;
	Dbs_fld		fld[] = {	{ rec->eii,		_T("equipment_info_id"),		FLD_INT, FLD_INT_SIZ(rec->eii)		},
							{ rec->rii,		_T("RC_reservation_info_id"),	FLD_INT, FLD_INT_SIZ(rec->rii)		},
							{ rec->rdi,		_T("RC_reservation_detail_id"),	FLD_INT, FLD_INT_SIZ(rec->rdi)		},
							{ rec->rcg->psn,_T("persons"),					FLD_INT, FLD_INT_SIZ(rec->rcg->psn)	},
							{ rec->rcg->ups,_T("use_persons"),				FLD_INT, FLD_INT_SIZ(rec->rcg->ups)	},
							{ rec->rcg->prc,_T("price"),					FLD_DBL, FLD_DBL_SIZ(rec->rcg->prc)	},
							{ rec->rcg->eii,_T("equipment_info_id"),		FLD_INT, FLD_INT_SIZ(rec->rcg->eii)	},
							{ rec->flg,		_T("system_language"),			FLD_WST, FLD_WST_SIZ(rec->flg)		}};

	memset( tmp, 0, sizeof(tmp) );
	Edt_Dte( sld, memcnt(sld), mng->bdt );

	chk  = !!(mdf[0].flc = Fdf_Sal_Inf( &mdf[0].fld, rec->sal, _T("SI_"), NULL, 0 ) );				// sales_infos
	chk &= !!(mdf[1].flc = Fdf_Rsv_Crg( &mdf[1].fld, rec->rcg->rcg, _T("RC_"), fld, memcnt(fld) ) );// reservation_charges
	chk &= !!(Fdf_Mak_Sll( &sls, FDF_SAL_INF, _T("SI"), _T("SI_") ));								// Make sales_infos select list
	chk &= !!(Fdf_Mak_Sll( &rcs, FDF_RSV_CRG, _T("RC"), _T("RC_") ));								// Make reservation_charges select list

	if ( chk ){
		swprintf_s( sql, memcnt(sql),
			_T("SELECT %s, %s, RE.equipment_info_id, EC.persons, CI.system_language, ")
			_T("RD.person_number AS use_persons, ED.price, RE.equipment_info_id ")
			_T("FROM %s RC ")																// reservation_charges
			_T("LEFT JOIN %s RI ON RI.id = RC.reservation_info_id ")						// reservation_infos
			_T("LEFT JOIN %s RD ON RD.id = RC.reservation_detail_id ")						// reservation_details
			_T("LEFT JOIN %s SI ON SI.reservation_detail_id = RC.reservation_detail_id ")	// sales_infos
			_T("LEFT JOIN %s RE ON RE.id = RC.reservation_equipment_id ")					// reservation_equimpents
			_T("LEFT JOIN %s CI ON CI.id = RD.client_info_id ")								// client_infos
			_T("LEFT JOIN %s ED ON ED.id = RC.equipment_charge_detail_id ")					// equipment_charge_details
			_T("LEFT JOIN %s EC ON EC.id = ED.equipment_charge_info_id ")					// equipment_charge_infos
			_T("AND SI.paid=0 ")
			_T("WHERE CAST(RC.use_date AS DATE) = '%s' ")
			_T("AND RI.no_show=0 AND RI.canceled=0 AND RD.canceled=0 AND RD.inhouse!=0 ")
			_T("AND RC.deleted_date IS NULL ")
			_T("AND RI.operator_code='happyday' AND RI.branch_code='happyday' AND RI.facility_code='hotel' ")
			_T("ORDER BY RI.id, RC.id "),
			sls, rcs, 
			TBN_RSV_CRG, TBN_RSV_INF, TBN_RSV_DTL, TBN_SAL_INF, TBN_RSV_EQP,
			TBN_CLI_INF, TBN_EQP_CGD, TBN_EQP_CRG, sld,
			SQL_Edt_Wst(opb, memcnt(opb), mng->opc), SQL_Edt_Wst(brb, memcnt(brb), mng->brc),
			SQL_Edt_Wst(fcb, memcnt(fcb), mng->fcc)	);
		chk = SQL_Get_MLs( tmp, sql, mdf, memcnt(mdf), rec, sizeof(rec), erm, dbs );
	}

	if ( rcs )			free( rcs );
	if ( sls )			free( sls );
	if ( mdf[1].fld )	free( mdf[1].fld );
	if ( mdf[0].fld )	free( mdf[0].fld );

	while ( chk && !!( csr = Nxt_Lnk( tmp, csr ) ) ){
		ptr = Rec_Of(struct rec_, csr);
		if ( rdi != DBS_INT_VAL(ptr->rcg->rcg->rdi) ){
			rdi = DBS_INT_VAL(ptr->rcg->rcg->rdi);
			if ( chk = !!( csn = Add_Lnk( rot, NULL, sizeof(NAD_SAL_INF) ) ) ){
				sal = Rec_Of(NAD_SAL_INF, csn);
				*sal->sal = *ptr->sal;
				DBS_INT_CPY(sal->eii, ptr->eii);
				DBS_INT_CPY(sal->rii, ptr->rii);
				DBS_INT_CPY(sal->rdi, ptr->rdi);
				DBS_WST_CPY(sal->flg, ptr->flg);
			}
		}
		if ( sal ){
			if ( chk = !!( csn = Add_Lnk( sal->rcg, NULL, sizeof(NAD_RCG_INF) ) ) ){
				*Rec_Of(NAD_RCG_INF, csn) = *ptr->rcg;
			}
		}
	}

	Del_All( tmp );

	return( chk );
}

static	int	mak_sal(					// make sales data
Csr				rot,					// tareet data
WCHAR			**erm,					// stioring pointer of error message
PMS_MNG			*mng )					// HotelPMS system management segment
{
	int			chk=TRUE, rdi=0, cnt, val;
	double		fvl;
	Csr			csr=NULL, cse=NULL, csn=NULL;
	NAD_SAL_INF	*sal;
	NAD_RCG_INF	*rcg;
	NAD_SAL_DTL	*dtl;
	CRC_CVS		*ccv;
	Day			dte[1];

	Get_Dte( dte );

	while ( csn = Nxt_Lnk( mng->ccl, csn ) ){
		ccv = Rec_Of(CRC_CVS, csn);
		if ( DBS_INT_VAL(ccv->isb) )	break;
	}

	while ( chk && !!( csr = Nxt_Lnk( rot, csr ) ) ){
		sal = Rec_Of(NAD_SAL_INF, csr);
		if ( Nul_Fld(sal->sal->id, FLD_INT) ){
			DBS_WST_SET(sal->sal->opc, mng->opc);
			DBS_INT_CPY(sal->sal->rii, sal->rii);
			DBS_INT_CPY(sal->sal->rdi, sal->rdi);
			DBS_INT_CPY(sal->sal->eii, sal->eii);
			DBS_WST_SET(sal->sal->brc, mng->brc);
			DBS_WST_SET(sal->sal->fcc, mng->fcc);
			DBS_INT_SET(sal->sal->ivn, (int *)NULL);
			if ( Nul_Fld(sal->flg, FLD_WST) ){
				DBS_WST_SET(sal->sal->flg, mng->dfl);
			}
			else{
				DBS_WST_CPY(sal->sal->flg, sal->flg);
			}
		}
		while( chk && !!( cse = Nxt_Lnk( sal->rcg, cse ) ) ){
			rcg = Rec_Of(NAD_RCG_INF, cse);
			if ( chk = !!( csn = Add_Lnk( sal->dtl, NULL, sizeof(NAD_SAL_DTL) ) ) ){
				dtl = Rec_Of(NAD_SAL_DTL, csn);
				DBS_WST_SET(dtl->dtl->opc, mng->opc);
				DBS_INT_CPY(dtl->dtl->sii, sal->sal->id);
				DBS_DAY_CPY(dtl->dtl->sld, rcg->rcg->udt);
				DBS_DAY_SET(dtl->dtl->sdt, dte);
				DBS_INT_CPY(dtl->dtl->dsi, rcg->rcg->dsi);
				if ( Nul_Fld(rcg->psn, FLD_INT ) )	cnt = 1;
				else								cnt = DBS_INT_VAL(rcg->psn);
				fvl = (double)(DBS_INT_VAL(rcg->ups) / cnt + ( !!(DBS_INT_VAL(rcg->ups)%cnt) ? 1: 0 )); 
				DBS_DBL_SET(dtl->dtl->qty, &fvl);
				DBS_DBL_CPY(dtl->dtl->nmp, rcg->prc);
				DBS_DBL_CPY(dtl->dtl->slp, Nul_Fld(rcg->rcg->amt, FLD_DBL) ? rcg->prc: rcg->rcg->amt); 
				DBS_INT_SET(dtl->dtl->asi, &(val=0));
				DBS_INT_CPY(dtl->dtl->eqi, rcg->eii);
				if ( dtl->pup = !!DBS_INT_VAL(rcg->rcg->pup) ){
					DBS_WST_SET(dtl->pay->opc, mng->opc);
					DBS_INT_CPY(dtl->pay->sii, sal->sal->id);
					DBS_DAY_SET(dtl->pay->pmd, mng->bdt);
					DBS_INT_CPY(dtl->pay->cci, ccv->id);
					DBS_DBL_CPY(dtl->pay->rte, ccv->rte);
					DBS_DBL_CPY(dtl->pay->amt, dtl->dtl->slp);
					DBS_INT_SET(dtl->pay->mtd, &(val=PAYMENT_RECEIVABLE));
				}
			}
		}
	}

	if ( !chk ){
		*erm = _wcsdup( _T("System error. Insufficent memory.") );
	}

	return( chk );
}

static	int	sav_sli(			// saving a sales_infos
SAL_INF			*sal,			// Saving data
Day				*dte,			// Saving datetime
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk=TRUE, fct, sii, val=0, ivn[DBF_SIZ_INT];
	Dbs_fld		*fld, ivf[] = { ivn, _T("invoice_number"), FLD_WSI, FLD_INT_SIZ(ivn) };
	WCHAR		sql[1024];
	
	if ( Nul_Fld(sal->id,FLD_INT) ){
		if ( chk = !!( fct = Fdf_Sal_Inf( &fld, sal, NULL, NULL, 0 ) ) ){
			DBS_INT_CPY(sal->mei, mng->lgi->id );
			DBS_DTM_SET(sal->cdt, dte );
			DBS_INT_SET(sal->ivn, &val );
			chk = !!( sii = SQL_Exc_Ins( sql, memcnt(sql), TBN_SAL_INF, erm, fld, fct, dbs ) );
			free( fld );
		}
	}
	else{
		sii = DBS_INT_VAL(sal->id);
	}

	return( chk ? sii: 0 );
}

static	int	sav_sld(			// saving a sales_details
SAL_DTL			*dtl,			// Saving data
int				sii,			// sales_info_id
Day				*dte,			// Saving datetime
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk=TRUE, fct, sdi;
	Dbs_fld		*fld;
	WCHAR		sql[1024];

	if ( Nul_Fld(dtl->id,FLD_INT) ){
		if ( chk = !!( fct = Fdf_Sal_Dtl( &fld, dtl, NULL, NULL, 0 ) ) ){
			DBS_INT_CPY(dtl->mei, mng->lgi->id );
			DBS_INT_SET(dtl->sii, &sii );
			DBS_DTM_SET(dtl->cdt, dte );
			DBS_DTM_SET(dtl->sdt, dte );
			DBS_DAY_SET(dtl->sld, mng->bdt );
			chk = !!( sdi = SQL_Exc_Ins( sql, memcnt(sql), TBN_SAL_DTL, erm, fld, fct, dbs ) );
			free( fld );
		}
	}

	return( chk ? sdi: 0 );
}

static	int	sav_pmt(			// saving a payment_details
PMT_DTL			*pmt,			// Saving data
int				sii,			// sales_info_id
int				sdi,			// sales_detail_id
Day				*dte,			// Saving datetime
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk=TRUE, fct;
	Dbs_fld		*fld;
	WCHAR		sql[1024];

	if ( Nul_Fld(pmt->id,FLD_INT) ){
		if ( chk = !!( fct = Fdf_Pmt_Dtl( &fld, pmt, NULL, NULL, 0 ) ) ){
			DBS_INT_CPY(pmt->mei, mng->lgi->id );
			DBS_INT_SET(pmt->sii, &sii );
			DBS_INT_SET(pmt->sdi, &sdi );
			DBS_DTM_SET(pmt->cdt, dte );
			chk = !!( sdi = SQL_Exc_Ins( sql, memcnt(sql), TBN_PMT_DTL, erm, fld, fct, dbs ) );
			free( fld );
		}
	}

	return( chk ? sdi: 0 );
}

static	int	exc_sup(					// execute to sale up
Csr				rot,					// tareet data
WCHAR			**erm,					// stioring pointer of error message
CDatabase		*dbs,					// Database ODBC object
PMS_MNG			*mng )					// HotelPMS system management segment
{
	int			chk=TRUE, sii, sdi;
	Csr			csr=NULL, cse=NULL;
	NAD_SAL_INF	*sal;
	NAD_SAL_DTL	*dtl;
	Day			dte[1];

	Get_Dte( dte );
	
	while ( chk && !!( csr = Nxt_Lnk( rot, csr ) ) ){
		sal = Rec_Of(NAD_SAL_INF, csr);
		if ( chk = ( sii = sav_sli( sal->sal, dte, erm, dbs, mng ) ) != -1 ){
			while ( chk && !!( cse = Nxt_Lnk( sal->dtl, cse ) ) ){
				dtl = Rec_Of(NAD_SAL_DTL, cse);
				if ( chk = ( sdi = sav_sld( dtl->dtl, sii, dte, erm, dbs, mng ) ) != -1 ){
					if ( !!dtl->pup ){
						chk = sav_pmt( dtl->pay, sii, sdi, dte, erm, dbs, mng ) != -1;
					}
				}
			}
		}
	}

	return( chk );
}

static	int	upd_bdt(					// update business date
WCHAR			**erm,					// stioring pointer of error message
CDatabase		*dbs,					// Database ODBC object
PMS_MNG			*mng )					// HotelPMS system management segment
{
	int			chk;
	WCHAR		sql[256], opb[96], brb[96];

	swprintf_s( sql, memcnt(sql),
		_T("UPDATE business_dates SET business_date = business_date + 1 ")
		_T("WHERE operator_code='%s' AND branch_code='%s' ")
		_T("AND deleted_date IS NULL "),
		SQL_Edt_Wst(opb, memcnt(opb), mng->opc), SQL_Edt_Wst(brb, memcnt(brb), mng->brc) );

	chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;

	return( chk );
}

int			Nad_Day_Cls(				// execute day close process
WCHAR			**erm,					// stioring pointer of error message
CDatabase		*dbs,					// Database ODBC object
PMS_MNG			*mng )					// HotelPMS system management segment
{
	int			chk;
	Lnk			rot[1], *csr=NULL;

	memset( rot, 0, sizeof(rot) );
	
	if ( chk = Dbs_Trn( erm, dbs, TRN_STT ) ){
		// sell up the room charge
		if ( chk = get_rcg( rot, erm, dbs, mng ) ){
			if ( chk = mak_sal( rot, erm, mng ) ){
				chk = exc_sup( rot, erm, dbs, mng );
			}
		}
		while ( csr=Nxt_Lnk( rot, csr ) ){
			Del_All( Rec_Of(NAD_SAL_INF, csr)->rcg );
			Del_All( Rec_Of(NAD_SAL_INF, csr)->dtl );
		}
		Del_All( rot );

		// update business date
		if ( chk ){
			chk = upd_bdt( erm, dbs, mng );
		}


		Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
	}

	return( chk );
}
