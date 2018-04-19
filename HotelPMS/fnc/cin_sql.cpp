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
#include "cin_fnc.h"
#include "cin_sql.h"
#include "fld_def.h"

int		Cin_Get_Inf(				// Getting reservation informations 
CIN_INF_DAT		*dst,				// Pointer of storing destination
int				rid,				// reservation_infos_id
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	WCHAR		sql[512], opb[96];
	int			chk, pos, fct;
	CRecordset	*rec;
	Dbs_fld		*fld;
	Dbs_fld		ext[] = {	{ dst->cii,			_T("client_id"),					FLD_INT, FLD_INT_SIZ(dst->cii)		},
							{ dst->ckd,			_T("client_kind"),					FLD_INT, FLD_INT_SIZ(dst->ckd)		},
							{ dst->crk,			_T("client_rank"),					FLD_INT, FLD_INT_SIZ(dst->crk)		},
							{ dst->fnm,			_T("first_name"),					FLD_WST, FLD_WST_SIZ(dst->fnm)		},
							{ dst->lnm,			_T("last_name"),					FLD_WST, FLD_WST_SIZ(dst->lnm)		},
							{ dst->adr1,		_T("address1"),						FLD_WST, FLD_WST_SIZ(dst->adr1)		},
							{ dst->adr2,		_T("address2"),						FLD_WST, FLD_WST_SIZ(dst->adr2)		},
							{ dst->zip,			_T("zip_code"),						FLD_WST, FLD_WST_SIZ(dst->zip)		},
							{ dst->tel1,		_T("telephone_number1"),			FLD_WST, FLD_WST_SIZ(dst->tel1)		},
							{ dst->tel2,		_T("telephone_number2"),			FLD_WST, FLD_WST_SIZ(dst->tel2)		},
							{ dst->fax,			_T("fax_number"),					FLD_WST, FLD_WST_SIZ(dst->fax)		},
							{ dst->eml1,		_T("email_address1"),				FLD_WST, FLD_WST_SIZ(dst->eml1)		},
							{ dst->eml2,		_T("email_address2"),				FLD_WST, FLD_WST_SIZ(dst->eml2)		} };


	if ( chk = !!(fct = Fdf_Rsv_Inf( &fld, dst->rsv, NULL, ext, memcnt(ext) )) ){
		pos = swprintf_s( sql,	memcnt(sql),
							_T("SELECT R.*, C.id AS client_id, C.client_kind, C.first_name, C.last_name,")
							_T("C.address1,C.address2,C.zip_code,C.telephone_number1,C.telephone_number2,")
							_T("C.client_rank, C.fax_number,C.email_address1,C.email_address2 ")
							_T("FROM %s R ")									// reservation_infos
							_T("LEFT JOIN %s C ON C.id = R.client_info_id ")	// client_infos
							_T("WHERE R.operator_code='%s' ")
							_T("AND R.deleted_date IS NULL AND C.deleted_date IS NULL ")
							_T("AND R.id=%d "), TBN_RSV_INF, TBN_CLI_INF, SQL_Edt_Wst(opb, memcnt(opb), mng->opc), rid );

		memset( dst, 0, sizeof(CIN_INF_DAT) );
		rec = Dbs_Qry( dbc, sql, erm );

		if ( chk = !!rec ){
			if ( *erm )	free( *erm );
			*erm = NULL;
			chk = Dbs_Get( fld, erm, fct, rec )==QRY_NML;
			Dbs_Cls(rec);
		}
		free( fld );
	}
	return( chk );
}


int		Cin_Get_Dtl(				// Getting reservation details
Csr				rot,				// Pointer of storing destination
int				rid,				// reservation_infos_id
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	CIN_DTL_DAT	dtl[1];
	WCHAR		*sql, opb[96], dbf[20];
	int			chk, pos, flc, ssz=2048;

	Dbs_fld		*fld;
	Dbs_fld		ext[] = {	{ dtl->ckd,			_T("client_kind"),				FLD_INT, FLD_INT_SIZ(dtl->ckd)		},
							{ dtl->fnm,			_T("first_name"),				FLD_WST, FLD_WST_SIZ(dtl->fnm)		},
							{ dtl->lnm,			_T("last_name"),				FLD_WST, FLD_WST_SIZ(dtl->lnm)		},
							{ dtl->rei,			_T("reservation_equipment_id"),	FLD_INT, FLD_INT_SIZ(dtl->rei)		},
							{ dtl->eii,			_T("equipment_info_id"),		FLD_INT, FLD_INT_SIZ(dtl->eii)		},
							{ dtl->eti,			_T("equipment_type_id"),		FLD_INT, FLD_INT_SIZ(dtl->eti)		},
							{ dtl->rno,			_T("room_number"),				FLD_WST, FLD_WST_SIZ(dtl->rno)		},
							{ dtl->tnm,			_T("room_type_name"),			FLD_WST, FLD_WST_SIZ(dtl->tnm)		},
							{ dtl->rtn,			_T("rate_plan_name"),			FLD_WST, FLD_WST_SIZ(dtl->rtn)		},
							{ dtl->esi,			_T("equipment_states_id"),		FLD_INT, FLD_INT_SIZ(dtl->esi)		},
							{ dtl->usd,			_T("use_start_date"),			FLD_DAY, FLD_DAY_SIZ(dtl->usd)		},
							{ dtl->ued,			_T("use_end_date"),				FLD_DAY, FLD_DAY_SIZ(dtl->ued)		} };

	chk  = !!( sql = (WCHAR *)malloc( wsizeof(ssz) ) );
	chk &= !!( flc = Fdf_Rsv_Dtl( &fld, dtl->dtl, NULL, ext, memcnt(ext) ) );

	if ( chk ){
		pos = swprintf_s( sql,	ssz,
				_T("SELECT D.*, C.first_name, C.last_name, C.client_kind, DC.name AS rate_plan_name,")
				_T("EI.equipment_code as room_number,  ET.name as room_type_name, ")
				_T("RE.equipment_info_id, RE.equipment_type_id,ES.id AS equipment_states_id,")
				_T("RE.id as reservation_equipment_id, RE.use_start_date,  RE.use_end_date ")
				_T("FROM %s D ")												// reservation_details
				_T("LEFT JOIN %s C ON C.id = D.client_info_id ")				// client_infos
				_T("LEFT JOIN %s RE ON RE.reservation_detail_id = D.id ")		// reservation_equipments
				_T("AND cast(RE.use_start_date AS date) = cast(D.arrival_date AS date) ") 
				_T("LEFT JOIN %s EI ON EI.id = RE.equipment_info_id ")			// equipment_infos
				_T("LEFT JOIN %s ET ON ET.id = RE.equipment_type_id ")			// equipment_types
				_T("LEFT JOIN %s ES ON ES.equipment_info_id = EI.id ")			// equipment_states
				_T("LEFT JOIN %s DC ON DC.id = RE.description_id ")				// descriptions
				_T("WHERE D.operator_code='%s' ")
				_T("AND D.reservation_info_id=%d ")
				_T("AND D.arrival_date='%s' AND D.inhouse=0 ")
				_T("AND D.deleted_date IS NULL AND C.deleted_date IS NULL ")
				_T("AND RE.deleted_date IS NULL AND EI.deleted_date IS NULL AND ET.deleted_date IS NULL ")
				_T("ORDER BY D.id, RE.use_start_date "),
				TBN_RSV_DTL, TBN_CLI_INF, TBN_RSV_EQP, TBN_EQP_INF, TBN_EQP_TYP, TBN_EQP_STS, TBN_DSC_RPT,
				SQL_Edt_Wst(opb, memcnt(opb), mng->opc), rid, Edt_Dte(dbf, memcnt(dbf), mng->bdt) );
		memset( dtl, 0, sizeof(CIN_DTL_DAT) );
		chk = SQL_Get_Lst( rot, sql, fld, flc, dtl, sizeof(dtl), erm, dbc );
	}

	if ( sql )		free( sql );
	if ( fld )		free( fld );

	return( chk );
}

int		Cin_Sav_Dtl(				// save reservation_details and reservation_equipment
Csr				rot,				// root of checkin target
Day				*dte,				// update date
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk, flc, ssz=4096;
	Csr			csr=NULL;
	WCHAR		*sql, dbf[30], eib[30], etb[30];
	Dbs_fld		*fld;
	CIN_DTL_DAT	*dtl;

	if ( chk = !!( sql = (WCHAR *)malloc( wsizeof(ssz) ) ) ){
		if ( chk = Dbs_Trn( erm, dbs, TRN_STT ) ){
			while ( chk && !!( csr = Nxt_Lnk( rot, csr ) ) ){
				dtl = Rec_Of(CIN_DTL_DAT, csr);
				if ( !Nul_Fld(dtl->dtl->mdt, FLD_DTM) ){
					if ( chk = !!( flc = Fdf_Rsv_Dtl( &fld, dtl, NULL, NULL, 0 ) ) ){
						DBS_INT_CPY(dtl->dtl->mei, mng->lgi->id);
						DBS_DTM_SET(dtl->dtl->mdt, dte);
						chk = SQL_Exc_Upd( sql, ssz, TBN_RSV_DTL, erm, fld, flc, dbs ) != 0;
						free( fld );
					}
					if ( chk ){
						swprintf_s( eib, memcnt(eib),
							Nul_Fld(dtl->eii, FLD_INT) ? _T(" = NULL"): _T("=%d"), DBS_INT_VAL(dtl->eii) );
						swprintf_s( etb, memcnt(etb),
							Nul_Fld(dtl->eti, FLD_INT) ? _T(" = NULL"): _T("=%d"), DBS_INT_VAL(dtl->eti) );
						swprintf_s( sql, ssz, 
							_T("UPDATE %s ")					// reservation_equipment
							_T("SET equipment_info_id%s,")
							_T("equipment_type_id%s,")
							_T("modified_employee_id=%d, modified_date='%s' ")
							_T("WHERE id=%d "), 
							TBN_RSV_EQP, eib, etb, DBS_INT_VAL(mng->lgi->id), Edt_Dte(dbf, memcnt(dbf), dte), DBS_INT_VAL(dtl->rei) );
						chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
					}
				}
			}
			Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
		}
		free( sql );
	}

	return( chk );
}

int		Cin_Rom_Lck(				// lock the room
WCHAR			*rei,				// list of reservation_equipment_id
WCHAR			*esi,				// list of equipment_state_id
Day				*upd,				// update date
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	WCHAR		*sql, buf[512], dbf[40], bdb[20];
	int			chk;

	swprintf_s( buf, memcnt(buf),
		_T("UPDATE %s SET inhouse=1,last_used_date='%s',")		// reservation_equipments
		_T("modified_employee_id=%d, modified_date='%s' ")
		_T("WHERE id in ( "), 
		TBN_RSV_EQP, Edt_Dte(bdb, memcnt(bdb), mng->bdt), DBS_INT_VAL(mng->lgi->id), Edt_Dtm( dbf, memcnt(dbf), upd ) );
	chk = !!( sql = Add_Str( NULL, buf ) );
	if ( chk )	chk = !!( sql = Add_Str( sql, rei ) );
	if ( chk )	chk = !!( sql = Add_Str( sql, _T(")") ) );
	if ( chk )	chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
	if ( sql )	free( sql );
	sql = NULL;

	if ( chk ){
		swprintf_s( buf, memcnt(buf),
			_T("UPDATE %s SET occupied_state=%d,clean_state=%d,")	// equipment_states
			_T("modified_employee_id=%d, modified_date='%s' ")
			_T("WHERE id in ( "), 
			TBN_EQP_STS, USS_OCCUPIED, CLS_DIRTY, DBS_INT_VAL(mng->lgi->id), dbf );
		chk = !!( sql = Add_Str( NULL, buf ) );
		if ( chk )	chk = !!( sql = Add_Str( sql, esi ) );
		if ( chk )	chk = !!( sql = Add_Str( sql, _T(")") ) );
		if ( chk )	chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
		if ( sql )	free( sql );
	}

	return( chk );
}

int		Cin_Mlt_Exc(				// actual execute checkin
WCHAR			*rdi,				// list of reservation_detail_id 
WCHAR			*rei,				// list of reservation_equipment_id
WCHAR			*esi,				// list of equipment_state_id
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment

{
	WCHAR		*sql, buf[512], dbf[40];
	int			chk;
	Day			upd[1];

	Get_Dte( upd );

	swprintf_s( buf, memcnt(buf),
			_T("UPDATE %s SET check_in_status=%d,")					// reservation_infos
			_T("modified_employee_id=%d, modified_date='%s' ")
			_T("WHERE id in ( ")
			_T("SELECT distinct reservation_info_id FROM reservation_details ")
			_T("WHERE id in (" ), 
			TBN_RSV_INF, CIS_INHOUSE, DBS_INT_VAL(mng->lgi->id), Edt_Dtm( dbf, memcnt(dbf), upd ) );
	chk = !!( sql = Add_Str( NULL, buf ) );
	if ( chk )	chk = !!( sql = Add_Str( sql, rdi ) );
	if ( chk )	chk = !!( sql = Add_Str( sql, _T(") )") ) );
	if ( chk )	chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
	if ( sql )	free( sql );
	sql = NULL;

	if ( chk ){
		swprintf_s( buf, memcnt(buf),
			_T("UPDATE %s SET inhouse=1, check_in_date='%s',")		// reservation_details
			_T("modified_employee_id=%d, modified_date='%s' ")
			_T("WHERE id in ( "), 
			TBN_RSV_DTL, dbf, DBS_INT_VAL(mng->lgi->id), dbf );
		chk = !!( sql = Add_Str( NULL, buf ) );
		if ( chk )	chk = !!( sql = Add_Str( sql, rdi ) );
		if ( chk )	chk = !!( sql = Add_Str( sql, _T(")") ) );
		if ( chk )	chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
		if ( sql )	free( sql );
		sql = NULL;
	}
	if ( chk )		chk = Cin_Rom_Lck( rei, esi, upd, erm, dbs, mng );

/*
	if ( chk ){
		swprintf_s( buf, memcnt(buf),
			_T("UPDATE %s SET inhouse=1,last_used_date='%s',")		// reservation_equipments
			_T("modified_employee_id=%d, modified_date='%s' ")
			_T("WHERE id in ( "), 
			TBN_RSV_EQP, Edt_Dte(bdb, memcnt(bdb), mng->bdt), mng->lgi->epi, dbf );
		chk = !!( sql = Add_Str( NULL, buf ) );
		if ( chk )	chk = !!( sql = Add_Str( sql, rei ) );
		if ( chk )	chk = !!( sql = Add_Str( sql, _T(")") ) );
		if ( chk )	chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
		if ( sql )	free( sql );
		sql = NULL;
	}

	if ( chk ){
		swprintf_s( buf, memcnt(buf),
			_T("UPDATE %s SET occupied_state=%d,clean_state=%d,")	// equipment_states
			_T("modified_employee_id=%d, modified_date='%s' ")
			_T("WHERE id in ( "), 
			TBN_EQP_STS, USS_OCCUPIED, CLS_DIRTY, mng->lgi->epi, dbf );
		chk = !!( sql = Add_Str( NULL, buf ) );
		if ( chk )	chk = !!( sql = Add_Str( sql, esi ) );
		if ( chk )	chk = !!( sql = Add_Str( sql, _T(")") ) );
		if ( chk )	chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
		if ( sql )	free( sql );
	}
*/

	return( chk );
}

int		Cin_Mlt_Cin(				// execute checkin
Csr				rot,				// root of checkin target
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	WCHAR		*cma=_T(""), buf[512], *rdi=NULL, *rei=NULL, *esi=NULL;
	int			chk;
	CIN_DTL_DAT	*dtl;
	Csr			csr=NULL;

	while( csr=Nxt_Lnk( rot, csr ) ){
		dtl = Rec_Of(CIN_DTL_DAT, csr);
		if ( dtl->cin ){
			swprintf_s( buf, memcnt(buf), _T("%s%d"), cma, DBS_INT_VAL(dtl->dtl->id) );
			chk = !!( rdi = Add_Str( rdi, buf ) );
			if ( chk ){
				swprintf_s( buf, memcnt(buf), _T("%s%d"), cma, DBS_INT_VAL(dtl->rei) );
				chk = !!( rei = Add_Str( rei, buf ) );
			}
			if ( chk ){
				swprintf_s( buf, memcnt(buf), _T("%s%d"), cma, DBS_INT_VAL(dtl->esi) );
				chk = !!( esi = Add_Str( esi, buf ) );
			}
			cma = _T(",");
		}
	}

	if ( chk ){
		chk = Dbs_Trn( erm, dbs, TRN_STT );
		if ( chk ){
			chk = Cin_Mlt_Exc( rdi, rei, esi, erm, dbs, mng );
/*
		swprintf_s( buf, memcnt(buf),
				_T("UPDATE %s SET check_in_status=%d,")					// reservation_infos
				_T("modified_employee_id=%d, modified_date='%s' ")
				_T("WHERE id in ( ")
				_T("SELECT distinct reservation_info_id FROM reservation_details ")
				_T("WHERE id in (" ), 
				TBN_RSV_INF, CIS_INHOUSE, mng->lgi->epi, Edt_Dtm( dbf, memcnt(dbf), upd ) );
		chk = !!( sql = Add_Str( NULL, buf ) );
		if ( chk )	chk = !!( sql = Add_Str( sql, rdi ) );
		if ( chk )	chk = !!( sql = Add_Str( sql, _T(") )") ) );
		if ( chk )	chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
		if ( sql )	free( sql );
		sql = NULL;
		if ( chk ){
			swprintf_s( buf, memcnt(buf),
				_T("UPDATE %s SET inhouse=1, check_in_date='%s',")		// reservation_details
				_T("modified_employee_id=%d, modified_date='%s' ")
				_T("WHERE id in ( "), 
				TBN_RSV_DTL, dbf, mng->lgi->epi, dbf );
			chk = !!( sql = Add_Str( NULL, buf ) );
			if ( chk )	chk = !!( sql = Add_Str( sql, rdi ) );
			if ( chk )	chk = !!( sql = Add_Str( sql, _T(")") ) );
			if ( chk )	chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
			if ( sql )	free( sql );
			sql = NULL;
		}
		if ( chk ){
			swprintf_s( buf, memcnt(buf),
				_T("UPDATE %s SET inhouse=1,last_used_date='%s',")		// reservation_equipments
				_T("modified_employee_id=%d, modified_date='%s' ")
				_T("WHERE id in ( "), 
				TBN_RSV_EQP, Edt_Dte(bdb, memcnt(bdb), mng->bdt), mng->lgi->epi, dbf );
			chk = !!( sql = Add_Str( NULL, buf ) );
			if ( chk )	chk = !!( sql = Add_Str( sql, rei ) );
			if ( chk )	chk = !!( sql = Add_Str( sql, _T(")") ) );
			if ( chk )	chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
			if ( sql )	free( sql );
			sql = NULL;
		}
		if ( chk ){
			swprintf_s( buf, memcnt(buf),
				_T("UPDATE %s SET occupied_state=%d,clean_state=%d,")	// equipment_states
				_T("modified_employee_id=%d, modified_date='%s' ")
				_T("WHERE id in ( "), 
				TBN_EQP_STS, USS_OCCUPIED, CLS_DIRTY, mng->lgi->epi, dbf );
			chk = !!( sql = Add_Str( NULL, buf ) );
			if ( chk )	chk = !!( sql = Add_Str( sql, esi ) );
			if ( chk )	chk = !!( sql = Add_Str( sql, _T(")") ) );
			if ( chk )	chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
			if ( sql )	free( sql );
		}
*/
			Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );

		}
	}

	if ( rdi )	free( rdi );
	if ( rei )	free( rei );
	if ( esi )	free( esi );

	return( chk );
}

int			Cin_Exc_Cin(			// execute checkin
int				rei,				// reservation_equipment_id
int				rdi,				// reservation_detail_id
int				esi,				// equipment_state_id
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
//	WCHAR		sql[512], dbf[40], bdb[20];
	WCHAR		reb[512], rdb[40], esb[20];
//	Day			upd[1];
	int			chk;

//	Get_Dte( upd );

	if ( chk = Dbs_Trn( erm, dbs, TRN_STT ) ){

		swprintf_s( reb, memcnt(reb), _T("%d"), rei );
		swprintf_s( rdb, memcnt(rdb), _T("%d"), rdi );
		swprintf_s( esb, memcnt(esb), _T("%d"), esi );
	
		chk = Cin_Mlt_Exc( rdb, reb, esb, erm, dbs, mng );

/*
	if ( chk ){
		swprintf_s( sql, memcnt(sql),
				_T("UPDATE %s SET check_in_status=%d,")					// reservation_infos
				_T("modified_employee_id=%d, modified_date='%s' ")
				_T("WHERE id in ( ")
				_T("SELECT reservation_info_id FROM reservation_equipments ")
				_T("WHERE id=%d )"), 
				TBN_RSV_INF, CIS_INHOUSE, mng->lgi->epi, Edt_Dtm( dbf, memcnt(dbf), upd ), rei );
		chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
	}
	if ( chk ){
		swprintf_s( sql, memcnt(sql),
				_T("UPDATE %s SET inhouse=1, check_in_date='%s',")		// reservation_details
				_T("modified_employee_id=%d, modified_date='%s' ")
				_T("WHERE id in ( ")
				_T("SELECT reservation_detail_id FROM reservation_equipments ")
				_T("WHERE id=%d )"), 
				TBN_RSV_DTL, dbf, mng->lgi->epi, dbf, rei );
		chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
	}
	if ( chk ){
		swprintf_s( sql, memcnt(sql),
				_T("UPDATE %s SET inhouse=1,last_used_date='%s',")		// reservation_equipments
				_T("modified_employee_id=%d, modified_date='%s' ")
				_T("WHERE id=%d "),
				TBN_RSV_EQP, Edt_Dte(bdb, memcnt(bdb), mng->bdt), mng->lgi->epi, dbf, rei );
		chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
	}
	if ( chk ){
		swprintf_s( sql, memcnt(sql),
				_T("UPDATE %s SET occupied_state=%d,clean_state=%d,")	// equipment_states
				_T("modified_employee_id=%d, modified_date='%s' ")
				_T("WHERE equipment_info_id in ( ")
				_T("SELECT equipment_info_id FROM reservation_equipments ")
				_T("WHERE id=%d )"),
				TBN_EQP_STS, USS_OCCUPIED, CLS_DIRTY, mng->lgi->epi,
				Edt_Dtm( dbf, memcnt(dbf), upd ), rei );
		chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
	}
	*/
		Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
	}

	return( chk );
}
