/*
	Hotel Property Management System
	Query functions for checkin view
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
#include "hkp_fnc.h"
#include "hkp_sql.h"
#include "cin_sql.h"
#include "cot_sql.h"
#include "fld_def.h"

int		Hkp_Get_Dat(				// Getting house keeping data
Csr				dst,				// root of storing destination
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk, fct;
	WCHAR		sql[8192], opb[96], brb[96], fcb[96];
	HKP_DTL_DAT	tmp[1];
	Dbs_fld		*fld;
	Dbs_fld		ext[] = {	{ tmp->fnm1,_T("RI_first_name"),	FLD_WST, FLD_WST_SIZ(tmp->fnm1) },
							{ tmp->lnm1,_T("RI_last_name"),		FLD_WST, FLD_WST_SIZ(tmp->lnm1) },
							{ tmp->fnm2,_T("RD_first_name"),	FLD_WST, FLD_WST_SIZ(tmp->fnm2) },
							{ tmp->lnm2,_T("RD_last_name"),		FLD_WST, FLD_WST_SIZ(tmp->lnm2) },
							{ tmp->rei, _T("RE_id"),			FLD_INT, FLD_INT_SIZ(tmp->rei) },
							{ tmp->eii, _T("EI_id"),			FLD_INT, FLD_INT_SIZ(tmp->rei) },
							{ tmp->eti, _T("ET_id"),			FLD_INT, FLD_INT_SIZ(tmp->rei) },
							{ tmp->esi, _T("ES_id"),			FLD_INT, FLD_INT_SIZ(tmp->rei) },
							{ tmp->rmn, _T("room_number"),		FLD_WST, FLD_WST_SIZ(tmp->rmn) },
							{ tmp->tnm, _T("type_name"),		FLD_WST, FLD_WST_SIZ(tmp->tnm) },
							{ tmp->usd, _T("use_start_date"),	FLD_DAY, FLD_DAY_SIZ(tmp->usd) },
							{ tmp->ued, _T("use_end_date"),		FLD_DAY, FLD_DAY_SIZ(tmp->ued) },
							{ tmp->ise, _T("EI_sale_enable"),	FLD_INT, FLD_INT_SIZ(tmp->ise) },
							{ tmp->sse, _T("ES_sale_enable"),	FLD_INT, FLD_INT_SIZ(tmp->sse) }};

	memset( tmp, 0, sizeof(tmp) );

	if ( chk = !!(fct = Fdf_Eqp_Sts( &fld, tmp->sts, NULL, ext, memcnt(ext) )) ){
		swprintf_s( sql, memcnt(sql),
			_T("SELECT ES.*, CI1.first_name AS RI_first_name, CI1.last_name AS RI_last_name, ")
			_T("CI2.first_name AS RD_first_name, CI2.last_name AS RD_last_name, ")
			_T("RE.id AS RE_id, EI.id AS EI_id, ET.id AS ET_id, ES.id AS ES_id, ")
			_T("EI.equipment_code AS room_number, ET.name AS type_name," )
			_T("EI.sale_enable AS EI_sale_enable, ES.sale_enable AS ES_sale_enable," )
			_T("RE.use_start_date, RE.use_end_date ")
			_T("FROM %s ES ")			// equipment_states
			_T("LEFT JOIN %s EI ON EI.id = ES.equipment_info_id AND EI.deleted_date IS NULL ")					// equipment_infos
			_T("LEFT JOIN %s ET ON ET.id = EI.equipment_type_id AND ET.deleted_date IS NULL ")					// equipment_types
			_T("LEFT JOIN %s RE ON RE.equipment_info_id=EI.id AND RE.deleted_date IS NULL AND RE.inhouse!=0 ")	// reservation_equipments
			_T("LEFT JOIN %s RD ON RD.id = RE.reservation_detail_id AND RD.deleted_date IS NULL ")				// reservation_details
			_T("LEFT JOIN %s RI ON RI.id = RE.reservation_info_id AND RI.deleted_date IS NULL ")				// reservation_details
			_T("LEFT JOIN %s CI1 ON CI1.id = RI.client_info_id AND CI1.deleted_date IS NULL ")					// client_infos (reservation_infos )
			_T("LEFT JOIN %s CI2 ON CI2.id = RD.client_info_id AND CI2.deleted_date IS NULL ")					// client_infos (reservation_details )
			_T("WHERE EI.operator_code='%s' AND EI.branch_code='%s' AND EI.facility_code='%s' ")
			_T("AND ES.deleted_date IS NULL "),
			TBN_EQP_STS, TBN_EQP_INF, TBN_EQP_TYP, TBN_RSV_EQP, TBN_RSV_DTL, TBN_RSV_INF, TBN_CLI_INF, TBN_CLI_INF,  
			SQL_Edt_Wst(opb, memcnt(opb), mng->opc), SQL_Edt_Wst(brb, memcnt(brb), mng->brc),
			SQL_Edt_Wst(fcb, memcnt(fcb), mng->fcc)	);

		chk = SQL_Get_Lst( dst, sql, fld, fct, tmp, sizeof(tmp), erm, dbc );
		free( fld );
	}

	return( chk );
}

int		Hkp_Get_Rch(				// Getting room change informations 
Csr				dst,				// root of storing destination
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk;

	WCHAR		sql[8192], opb[96], brb[96], fcb[96], bdb[20];
	HKP_RCH_DAT	tmp[1];
	Dbs_fld		fld[] = {	{ tmp->rii,	_T("RI_id"),				FLD_INT, FLD_INT_SIZ(tmp->rii)		},
							{ tmp->rdi,	_T("RD_id"),				FLD_INT, FLD_INT_SIZ(tmp->rdi)		},
							{ tmp->rei1,_T("RE1_id"),				FLD_INT, FLD_INT_SIZ(tmp->rei1)		},
							{ tmp->rei2,_T("RE2_id"),				FLD_INT, FLD_INT_SIZ(tmp->rei2)		},
							{ tmp->eii1,_T("EI1_id"),				FLD_INT, FLD_INT_SIZ(tmp->eii1)		},
							{ tmp->eii2,_T("EI2_id"),				FLD_INT, FLD_INT_SIZ(tmp->eii2)		},
							{ tmp->esi1,_T("ES1_id"),				FLD_INT, FLD_INT_SIZ(tmp->esi1)		},
							{ tmp->esi2,_T("ES2_id"),				FLD_INT, FLD_INT_SIZ(tmp->esi2)		},
							{ tmp->rmn1,_T("EI1_equipment_code"),	FLD_WST, FLD_WST_SIZ(tmp->rmn1)		},
							{ tmp->rmn2,_T("EI2_equipment_code"),	FLD_WST, FLD_WST_SIZ(tmp->rmn2)		},
							{ tmp->fnm1,_T("CI1_first_name"),		FLD_WST, FLD_WST_SIZ(tmp->fnm1)		},
							{ tmp->lnm1,_T("CI1_last_name"),		FLD_WST, FLD_WST_SIZ(tmp->lnm1)		},
							{ tmp->fnm2,_T("CI2_first_name"),		FLD_WST, FLD_WST_SIZ(tmp->fnm2)		},
							{ tmp->lnm2,_T("CI2_last_name"),		FLD_WST, FLD_WST_SIZ(tmp->lnm2)		}};

	memset( tmp, 0, sizeof(tmp) );
	Edt_Dte( bdb, memcnt(bdb), mng->bdt );

	swprintf_s( sql, memcnt(sql), 
		_T("SELECT RI.id AS RI_id, RD.id AS RD_id, RE1.id AS RE1_id, RE2.id AS RE2_id, ")
		_T("EI1.id AS EI1_id, EI2.id AS EI2_id, ES1.id AS ES1_id, ES2.id AS ES2_id,")
		_T("CI1.first_name AS CI1_first_name, CI1.last_name AS CI1_last_name, ")
		_T("CI2.first_name AS CI2_first_name, CI2.last_name AS CI2_last_name, ")
		_T("EI1.equipment_code AS EI1_equipment_code, EI2.equipment_code AS EI2_equipment_code ") 
		_T("FROM %s RD ")																		// reservation_details
		_T("LEFT JOIN %s RI ON RI.id = RD.reservation_info_id ")								// reservation_infos
		_T("LEFT JOIN %s RE1 ON RE1.reservation_detail_id = RD.id ")							// reservation_equipments
		_T("AND CAST(RE1.use_end_date AS DATE) = '%s' AND RE1.deleted_date IS NULL ")
		_T("LEFT JOIN %s RE2 ON RE2.reservation_detail_id = RD.id ")							// reservation_equipments
		_T("AND CAST(RE2.use_start_date AS DATE) = '%s' AND RE2.deleted_date IS NULL ")
		_T("LEFT JOIN %s EI1 ON EI1.id = RE1.equipment_info_id AND EI1.deleted_date IS NULL ")	// equipment_infos
		_T("LEFT JOIN %s EI2 ON EI2.id = RE2.equipment_info_id AND EI2.deleted_date IS NULL ")	// equipment_infos
		_T("LEFT JOIN %s ES1 ON ES1.equipment_info_id = EI1.id AND ES1.deleted_date IS NULL ")	// equipment_states
		_T("LEFT JOIN %s ES2 ON ES2.equipment_info_id = EI2.id AND ES2.deleted_date IS NULL ")	// equipment_states
		_T("LEFT JOIN %s CI1 ON CI1.id = RI.client_info_id AND CI1.deleted_date IS NULL ")		// client_infos(reservation_infos)
		_T("LEFT JOIN %s CI2 ON CI2.id = RD.client_info_id AND CI2.deleted_date IS NULL ")		// client_infos(reservation_details)
		_T("WHERE RD.inhouse!=0 ")
		_T("AND RI.operator_code='%s' AND RI.branch_code='%s' AND RI.facility_code='%s' ")
		_T("AND RE1.id IS NOT NULL AND RE2.id iS NOT NULL AND RE2.last_used_date IS NULL "),
		TBN_RSV_DTL, TBN_RSV_INF, TBN_RSV_EQP, bdb, TBN_RSV_EQP, bdb,
		TBN_EQP_INF,TBN_EQP_INF, TBN_EQP_STS,TBN_EQP_STS, TBN_CLI_INF, TBN_CLI_INF,
		SQL_Edt_Wst(opb, memcnt(opb), mng->opc), SQL_Edt_Wst(brb, memcnt(brb), mng->brc),
		SQL_Edt_Wst(fcb, memcnt(fcb), mng->fcc)	);

	chk = SQL_Get_Lst( dst, sql, fld, memcnt(fld), tmp, sizeof(tmp), erm, dbc );

	return( chk );
}

int		Hkp_Exc_Rch(					// execute to room change
Csr				rot,					// root of storing destination
WCHAR			**erm,					// error message store pointer
CDatabase		*dbs,					// Database ODBC object
PMS_MNG			*mng )					// HotelPMS system management segment
{
	WCHAR		*cma=_T(""), buf[512], *rei1=NULL, *rei2=NULL, *esi1=NULL, *esi2=NULL;
	int			chk;
	HKP_RCH_DAT	*rch;
	Csr			csr=NULL;
	Day			upd[1];

	while( csr=Nxt_Lnk( rot, csr ) ){
		rch = Rec_Of(HKP_RCH_DAT, csr);
		if ( rch->rch ){
			swprintf_s( buf, memcnt(buf), _T("%s%d"), cma, DBS_INT_VAL(rch->rei1) );
			if ( chk = !!( rei1 = Add_Str( rei1, buf ) ) ){
				swprintf_s( buf, memcnt(buf), _T("%s%d"), cma, DBS_INT_VAL(rch->rei2) );
				chk = !!( rei2 = Add_Str( rei2, buf ) );
			}
			if ( chk ){
				swprintf_s( buf, memcnt(buf), _T("%s%d"), cma, DBS_INT_VAL(rch->esi1) );
				if ( chk = !!( esi1 = Add_Str( esi1, buf ) ) ){
					swprintf_s( buf, memcnt(buf), _T("%s%d"), cma, DBS_INT_VAL(rch->esi2) );
					chk = !!( esi2 = Add_Str( esi2, buf ) );
				}
			}
			cma = _T(",");
		}
	}

	if ( chk ){
		if ( chk = Dbs_Trn( erm, dbs, TRN_STT ) ){
			Get_Dte( upd );
			if ( chk = Cot_Rom_Rls( rei1, upd, erm, dbs, mng ) ){
				chk = Cin_Rom_Lck( rei2, esi2, upd, erm, dbs, mng );
			}
			Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
		}
	}

	if ( rei1 )	free( rei1 );
	if ( rei2 )	free( rei2 );
	if ( esi1 )	free( esi1 );
	if ( esi2 )	free( esi2 );

	return( chk );
}

int		Hkp_Chg_Sts(					// changing room status
HKP_DTL_DAT		*dtl,					// updaing data
int				knd,					// kind of status
WCHAR			**erm,					// error message store pointer
CDatabase		*dbs,					// Database ODBC object
PMS_MNG			*mng )					// HotelPMS system management segment
{
	int			idx, chk;
	WCHAR		sql[380], buf[20];
	Day			dte[1];

	Get_Dte( dte );

	if ( knd==STS_SAL ){
		if ( chk = Dbs_Trn( erm, dbs, TRN_STT ) ){
			for ( idx=0; chk && idx<2; idx++ ){
				swprintf_s( sql, memcnt(sql),
					_T("UPDATE %s SET sale_enable=%d, modified_employee_id=%d, modified_date='%s' ")
					_T("WHERE id=%d "),
					!idx ? TBN_EQP_STS: TBN_EQP_INF, !idx ? DBS_INT_VAL(dtl->sse): DBS_INT_VAL(dtl->ise),
					DBS_INT_VAL(mng->lgi->id), Edt_Dte(buf, memcnt(buf), dte ),
					!idx ? DBS_INT_VAL(dtl->sts->id): DBS_INT_VAL(dtl->eii) );
				chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
			}
			Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
		}
	}
	else{
		swprintf_s( sql, memcnt(sql),
			_T("UPDATE %s SET %s=%d, modified_employee_id=%d, modified_date='%s' ")
			_T("WHERE id=%d "),
			TBN_EQP_STS, knd==STS_CLN ? _T("clean_state"): _T("occupied_state"), 
			knd==STS_CLN ? DBS_INT_VAL(dtl->sts->cln): DBS_INT_VAL(dtl->sts->ocp), 
			DBS_INT_VAL(mng->lgi->id), Edt_Dte(buf, memcnt(buf), dte ), DBS_INT_VAL(dtl->sts->id) );

		if ( chk = Dbs_Trn( erm, dbs, TRN_STT ) ){
			chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
			Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
		}
	}

	return( chk );
}
