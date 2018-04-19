/*
	Hotel Property Management System
	SQL functions for reservationView
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
#include "rsv_fnc.h"
#include "rsv_sql.h"
#include "fld_def.h"


int		Rsv_Get_Inf(				// Getting reservation_infos and client_infos
RSV_INF_DAT		*dst,				// root of storing area
int				rid,				// reservation_infos_id
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	WCHAR		sql[2048], opb[96], *sll;
	int			chk, pos;
	CRecordset	*rec;
	Mlt_dbf		mdf[2];

	//int			Dbs_MGt( Mlt_dbf *, int *, WCHAR **, int, CRecordset * );

	if ( chk = !!(mdf[1].flc = Fdf_Cli_Inf( &mdf[1].fld, dst->cli, _T("C_"), NULL, 0 ) ) ){
		if ( chk = !!(mdf[0].flc = Fdf_Rsv_Inf( &mdf[0].fld, dst->rsv, NULL, NULL, 0 )) ){
			if ( chk = !!Fdf_Mak_Sll( &sll, FDF_CLI_INF, _T("C"), _T("C_") ) ){		// Make select list
				pos = swprintf_s( sql,	memcnt(sql),
								_T("SELECT R.*, %s  ")
								_T("FROM %s R ")								// reservation_infos
								_T("LEFT JOIN %s C ON C.id = R.client_info_id ")// client_infos
								_T("WHERE R.operator_code='%s' ")
								_T("AND R.deleted_date IS NULL AND C.deleted_date IS NULL ")
								_T("AND R.id=%d "),
								sll, TBN_RSV_INF, TBN_CLI_INF, SQL_Edt_Wst(opb, memcnt(opb), mng->opc), rid );

				memset( dst, 0, sizeof(RSV_INF_DAT) );
				rec = Dbs_Qry( dbc, sql, erm );

				if ( chk = !!rec ){
					if ( *erm )	free( *erm );
					*erm = NULL;
					chk = Dbs_MGt( mdf, erm, memcnt(mdf), rec )==QRY_NML;
					Dbs_Cls(rec);
				}
				free( sll );
			}
			free( mdf[0].fld );
		}
		free( mdf[1].fld );
	}
	return( chk );
}

int		Rsv_Get_Eis(				// Getting equipment_infos + equipment_state_id
Csr				dtl,				// root of reservation details
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk, flc;
	WCHAR		sql[256], opb[96], brb[96], fcb[96];
	RSV_EQP_EQS	eqs[1];
	Dbs_fld		*fld;
	Dbs_fld		ext[1] = { eqs->esi, _T("ES_id"), FLD_INT, FLD_INT_SIZ(eqs->esi) };

	memset( eqs, 0, sizeof(eqs) );

	if ( chk = !!( flc = Fdf_Eqp_Inf( &fld, eqs->eqi, NULL, ext, memcnt(ext) ) ) ){
		swprintf_s( sql, memcnt(sql),
			_T("SELECT EI.*, ES.id AS ES_id ")
			_T("FROM %s EI, %s ES ")
			_T("WHERE ES.equipment_info_id = EI.id ")
			_T("AND EI.operator_code='%s' AND EI.branch_code='%s' AND EI.facility_code='%s' ")
			_T("AND EI.deleted_date IS NULL "),
			TBN_EQP_INF, TBN_EQP_STS,
			SQL_Edt_Wst(opb, memcnt(opb), mng->opc), SQL_Edt_Wst(brb, memcnt(brb), mng->brc),
			SQL_Edt_Wst(fcb, memcnt(fcb), mng->fcc) );

		chk = SQL_Get_Lst( dtl, sql, fld, flc, eqs, sizeof(eqs), erm, dbc );
		free( fld );
	}

	return( chk );
}
/*
static	int		get_req(			// Getting list of reservation_equipments
Csr				dtl,				// root of reservation details
int				rii,				// id of reservation_infos
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	RSV_EQP_DAT	req[1], *rep;
	WCHAR		*sql, opb[96], *cma=_T("");
	int			chk, pos, ssz=8192, flc, rdi=0;
	Lnk			rot[1], *csr=NULL, *drt, *cse;
	Dbs_fld		*fld;
	Dbs_fld		ext[] = {	{ req->rno,			_T("room_number"),				FLD_WST, FLD_WST_SIZ(req->rno)		},
							{ req->tnm,			_T("room_type_name"),			FLD_WST, FLD_WST_SIZ(req->tnm)		} };

	chk  = !!( sql = (WCHAR *)malloc( wsizeof(ssz) ) );
	chk &= !!( flc = Fdf_Rsv_Eqp( &fld, req->req, NULL, ext, memcnt(ext) ) );				// reservation_details

	memset( req, 0, sizeof(req) );
	if ( chk ){
		pos = swprintf_s( sql,	ssz,
							_T("SELECT RE.*, ")
							_T("EI.equipment_code as room_number,  ET.name as room_type_name ")
							_T("FROM reservation_equipments RE ")
							_T("LEFT JOIN equipment_infos EI ON EI.id = RE.equipment_info_id ")
							_T("LEFT JOIN equipment_types ET ON ET.id = RE.equipment_type_id ")
							_T("WHERE RE.operator_code='%s' ")
							_T("AND RE.reservation_info_id=%d ")
							_T("AND RE.deleted_date IS NULL AND RE.reservation_detail_id IN ("), 
							SQL_Edt_Wst( opb, memcnt(opb), mng->opc ), rii );
		while( csr = Nxt_Lnk( dtl, csr ) ){
			pos += swprintf_s( sql+pos, ssz-pos, _T("%s%d"), cma, DBS_INT_VAL(Rec_Of(RSV_DTL_DAT, csr)->dtl->id) );
			cma = _T(",");
		}
		pos += swprintf_s( sql+pos, ssz-pos, _T(") ORDER BY RE.reservation_detail_id, RE.use_start_date ") );
		memset( rot, 0, sizeof(rot) );
		if ( chk = SQL_Get_Lst( rot, sql, fld, flc, req, sizeof(req), erm, dbc ) ){
			while( csr = Nxt_Lnk( rot, csr ) ){
				rep = Rec_Of(RSV_EQP_DAT, csr);
				if ( rdi != DBS_INT_VAL(rep->req->rdi) ){
					rdi = DBS_INT_VAL(rep->req->rdi);
					for ( cse=NULL; cse=Nxt_Lnk( dtl, cse); ){
						if ( DBS_INT_VAL(Rec_Of(RSV_DTL_DAT, cse)->dtl->id) == rdi ){
							drt = Rec_Of(RSV_DTL_DAT, cse)->req;
							break;
						}
					}
					if ( drt ){
						if ( !( chk = !!Add_Lnk( drt, rep, sizeof(RSV_EQP_DAT) ) ) )	break;
					}
				}
			}
		}
		Del_All( rot );
	}

	if ( sql )	free( sql );
	if ( fld )	free( fld );

	return( chk );
}
*/

int		Rsv_Get_Dtl(				// Getting each room information
RSV_INF_DAT		*inf,				// reservation_infos data
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	RSV_DTL_DAT	*dtl=NULL;
	RSV_EQP_DAT	*req=NULL;
	WCHAR		*sql, *cls, *res, *rcs, opb[96];
	Mlt_dbf		mdf[4];
	Lnk			rot[1], *csr=NULL, *cse;
	int			chk, ssz=16384, rdi=0, rei=0, rid=DBS_INT_VAL(inf->rsv->id);

	struct	_Tmp {
		RSV_DTL_DAT	dtl[1];
		RSV_EQP		req[1];
		RSV_CRG		rcg[1];
		RSV_EQP_DAT	eqd[1];
	}rec[1], *tpp;
	Dbs_fld		ext[] = {	{ rec->eqd->rno,	_T("room_number"),		FLD_WST, FLD_WST_SIZ(rec->eqd->rno)	},
							{ rec->eqd->tnm,	_T("room_type_name"),	FLD_WST, FLD_WST_SIZ(rec->eqd->tnm)	},
							{ rec->eqd->esi,	_T("ES_id"),			FLD_INT, FLD_INT_SIZ(rec->eqd->esi)	},
							{ rec->dtl->eci,	_T("ptan_id"),			FLD_INT, FLD_INT_SIZ(rec->dtl->eci)	} };

	chk  = !!(sql = (WCHAR *)malloc( wsizeof(ssz) ));
	chk &= !!(mdf[0].flc = Fdf_Rsv_Dtl( &mdf[0].fld, rec->dtl->dtl, NULL, NULL, 0 ) );			// reservation_details
	chk &= !!(mdf[1].flc = Fdf_Cli_Inf( &mdf[1].fld, rec->dtl->cli, _T("C_"), NULL, 0 ) );		// client_infos
	chk &= !!(mdf[2].flc = Fdf_Rsv_Eqp( &mdf[2].fld, rec->req, _T("RE_"), NULL, 0 ) );			// reservation_equipments
	chk &= !!(mdf[3].flc = Fdf_Rsv_Crg( &mdf[3].fld, rec->rcg, _T("RC_"), ext, memcnt(ext) ) );	// reservation_charges
	chk &= !!(Fdf_Mak_Sll( &cls, FDF_CLI_INF, _T("C"), _T("C_") ));								// Make client_infos select list
	chk &= !!(Fdf_Mak_Sll( &res, FDF_RSV_EQP, _T("RE"), _T("RE_") ));							// Make reservation_equipments select list
	chk &= !!(Fdf_Mak_Sll( &rcs, FDF_RSV_CRG, _T("RC"), _T("RC_") ));							// Make reservation_charges select list

	memset( rec, 0, sizeof(rec) );
	memset( rot, 0, sizeof(rot) );

	if ( chk ){
/*
		swprintf_s( sql,	ssz,
			_T("SELECT D.*, %s, %s, %s, ET.name AS room_type_name, EI.equipment_code AS room_number ")
			_T("FROM reservation_details D ")
			_T("LEFT JOIN client_infos C ON C.id = D.client_info_id ")
			_T("LEFT JOIN reservation_equipments RE ON RE.reservation_detail_id = D.id AND RE.reservation_info_id=%d ")
			_T("LEFT JOIN reservation_charges RC ON RC.reservation_detail_id = D.id AND RE.reservation_info_id=%d ")
			//_T(" AND RC.reservation_equipment_id=RE.id ")
			_T("LEFT JOIN equipment_infos EI ON EI.id = RE.equipment_info_id ")
			_T("LEFT JOIN equipment_types ET ON ET.id = RE.equipment_type_id ")
			_T("WHERE D.operator_code='%s' AND D.reservation_info_id=%d ")
			_T("AND D.deleted_date IS NULL AND C.deleted_date IS NULL AND RE.deleted_date IS NULL AND RC.deleted_date IS NULL ")
			_T("ORDER BY D.arrival_date, RE.use_start_date, RE.use_start_date, RC.use_date"),
			cls, res, rcs, rid, rid, SQL_Edt_Wst(opb, memcnt(opb), mng->opc), rid );
*/
		swprintf_s( sql,	ssz,
//			_T("SELECT DISTINCT D.*, %s, %s, %s, ET.name AS room_type_name, EI.equipment_code AS room_number,")
			_T("SELECT D.*, %s, %s, %s, ET.name AS room_type_name, EI.equipment_code AS room_number,")
			_T("ED.equipment_charge_info_id AS ptan_id, ES.id AS ES_id ")
			_T("FROM %s D ")																			// reservation_details
			_T("LEFT JOIN %s C ON C.id = D.client_info_id ")											// client_infos
			_T("LEFT JOIN %s RE ON RE.reservation_detail_id = D.id AND RE.reservation_info_id=%d ")		// reservation_equipments
			_T("LEFT JOIN %s RC ON RC.reservation_detail_id = D.id AND RE.reservation_info_id=%d ")		// reservation_charges
			_T("LEFT JOIN %s EI ON EI.id = RE.equipment_info_id ")										// equipment_infos
			_T("LEFT JOIN %s ET ON ET.id = RE.equipment_type_id ")										// equipment_types
			_T("LEFT JOIN %s ES ON ES.equipment_info_id = EI.id ")										// equipment_states
			_T("LEFT JOIN %s ED ON ED.id = RC.equipment_charge_detail_id ")								// equipment_charge_details
			/*_T("LEFT JOIN %s EC ON EC.id = ED.equipment_charge_info_id ")*/								// equipment_charge_infos
			_T("WHERE D.operator_code='%s' AND D.reservation_info_id=%d ")
			_T("AND D.deleted_date IS NULL AND C.deleted_date IS NULL AND RE.deleted_date IS NULL AND RC.deleted_date IS NULL ")
//			_T("GROUP BY ED.equipment_charge_info_id "),
			_T("ORDER BY D.id, D.arrival_date, RE.use_start_date, RE.use_start_date, RC.use_date"),
			cls, res, rcs, 
			TBN_RSV_DTL, TBN_CLI_INF,
			TBN_RSV_EQP, rid,
			TBN_RSV_CRG, rid,
			TBN_EQP_INF, TBN_EQP_TYP, TBN_EQP_STS, TBN_EQP_CGD, /*TBN_EQP_CRG,*/
			SQL_Edt_Wst(opb, memcnt(opb), mng->opc), rid );
		chk = SQL_Get_MLs( rot, sql, mdf, memcnt(mdf), rec, sizeof(rec), erm, dbc );
	}

	while ( !!chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
		tpp = Rec_Of(struct _Tmp, csr );
		if ( rdi != DBS_INT_VAL(tpp->dtl->dtl->id) ){
			rdi = DBS_INT_VAL(tpp->dtl->dtl->id);
			if ( chk = !!( cse = Add_Lnk( inf->dtl, NULL, sizeof(RSV_DTL_DAT) ) ) ){
				dtl = Rec_Of(RSV_DTL_DAT, cse);
				*dtl = *(tpp->dtl);
				//*(dtl->dtl)= *(tpp->dtl);
				//*(dtl->cli)= *(tpp->cli);
			}
		}
		if ( chk ){
			if ( rei != DBS_INT_VAL(tpp->req->id) ){
				rei = DBS_INT_VAL(tpp->req->id);
				if ( chk = !!( cse = Add_Lnk( dtl->req, NULL, sizeof(RSV_EQP_DAT) ) ) ){
					req = Rec_Of(RSV_EQP_DAT, cse);
					*(req->req) = *(tpp->req);
					DBS_INT_CPY(req->esi, tpp->eqd->esi);
					DBS_WST_CPY(req->rno, tpp->eqd->rno);
					DBS_WST_CPY(req->tnm, tpp->eqd->tnm);
				}
			}
		}
		if ( chk && !Nul_Fld(tpp->rcg->id,FLD_INT) ){
			if ( chk = !!( cse = Add_Lnk( dtl->rcg , NULL, sizeof(RSV_RCG_DAT) ) ) ){
				*Rec_Of(RSV_RCG_DAT, cse)->rcg = *tpp->rcg;
			}
		}
	}

	if ( sql )			free( sql );
	if ( mdf[0].fld )	free( mdf[0].fld );
	if ( mdf[1].fld )	free( mdf[1].fld );
	if ( mdf[2].fld )	free( mdf[2].fld );
	if ( mdf[3].fld )	free( mdf[3].fld );
	if ( cls )			free( cls );
	if ( res )			free( res );
	if ( rcs )			free( rcs );
	
	Del_All( rot );
	//if ( chk )	chk = get_req( inf->dtl, rid, erm, dbc, mng );			// Getting list of reservation_equipments

	return( chk );
}

static	int		new_rvn(		// Getting new reservation number
WCHAR			*sql,			// SQL statement storing buffer
int				ssz,			// sizeof SQL statement storing buffer(element)
WCHAR			**erm,			// Pointer of storing error message
RSV_INF_DAT		*inf,			// reservation_infos data for insert
Day				*dte,			// datetime of insert/update
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	CRecordset	*rec;
	WCHAR		*opc=NULL, *brc=NULL, *fcc=NULL, dbf[40];
	int			chk, rvn[DBF_SIZ_INT];
	Dbs_fld		fld[] = {	{ inf->rsv->opc,	_T("operator_code"),	FLD_WST, FLD_DTM_SIZ(inf->rsv->opc) },
							{ inf->rsv->brc,	_T("branch_code"),		FLD_WST, FLD_DTM_SIZ(inf->rsv->brc) },
							{ inf->rsv->fcc,	_T("facility_code"),	FLD_WST, FLD_DTM_SIZ(inf->rsv->fcc) },
							{ rvn,				_T("new_number"),		FLD_INT, FLD_INT_SIZ(rvn)			} };

	if ( chk = ( !!(opc=SQL_Edt_Flv(fld+0)) && !!(brc = SQL_Edt_Flv(fld+1)) && !!(fcc = SQL_Edt_Flv(fld+2)) ) ){
		swprintf_s( sql, ssz,
			_T("UPDATE %s SET reservation_number = reservation_number + 1, ")		// reservation_numbers
			_T("modified_employee_id=%d, modified_date='%s' ")
			_T("WHERE operator_code=%s AND branch_code=%s AND facility_code=%s ")
			_T("AND deleted_date IS NULL "), TBN_RSV_NBR, DBS_INT_VAL(mng->lgi->id), Edt_Dtm(dbf, memcnt(dbf), dte), opc, brc, fcc );
		if ( chk = Dbs_Upd( dbs, sql, erm )==QRY_NML ){
			swprintf_s( sql, ssz,
				_T("SELECT MAX(reservation_number) new_number FROM %s ")			// reservation_numbers
				_T("WHERE operator_code=%s AND branch_code=%s AND facility_code=%s ")
				_T("AND deleted_date IS NULL "), TBN_RSV_NBR, opc, brc, fcc );
			if ( chk = !!( rec = Dbs_Qry( dbs, sql, erm ) ) ){
				chk = Dbs_Get( fld+3, erm, 1, rec )==QRY_NML;
				Dbs_Cls(rec);
			}
		}
	}
	if ( opc )	free( opc );
	if ( brc )	free( brc );
	if ( fcc )	free( fcc );

	return( chk ? DBS_INT_VAL(rvn): 0 );
}

static	int		del_all(		// delete all records on beneath tables
WCHAR			*sql,			// SQL statement storing buffer
int				ssz,			// sizeof SQL statement storing buffer(element)
WCHAR			**erm,			// Pointer of storing error message
RSV_INF			*rsv,			// reservation_infos data for insert/update
Day				*dte,			// datetime of insert/update
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk=TRUE, idx;
	WCHAR		dbf[40];
	WCHAR		*tbn[] = { TBN_RSV_DTL, TBN_RSV_EQP, TBN_RSV_CRG };

	/*
	// delete all client_infos
	swprintf_s( sql, ssz, 
			_T("UPDATE %s SET modified_employee_id=%d, deleted_date='%s' ")
			_T("WHERE id=%s OR id IN ( ")
			_T("SELECT client_info_id FROM reservation_details WHERE reservation_info_id=%d ) "),
			TBN_CLI_INF, mng->lgi->epi, Edt_Dtm(dbf, memcnt(dbf), dte), DBS_INT_VAL(rsv->id), DBS_INT_VAL(rsv->id) );
	chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
	*/

	// delete all other relation tables
	for ( idx=0; !!chk && idx<memcnt(tbn); idx++ ){
		swprintf_s( sql, ssz, 
			_T("UPDATE %s SET modified_employee_id=%d, deleted_date='%s' ")
			_T("WHERE reservation_info_id=%d "),
			tbn[idx], DBS_INT_VAL(mng->lgi->id), Edt_Dtm(dbf, memcnt(dbf), dte), DBS_INT_VAL(rsv->id) );
		chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
	}

	return( chk );
}

static	int		sav_cli(		// Saving client_infos on reservation_infos
WCHAR			*sql,			// SQL statement storing buffer
int				ssz,			// sizeof SQL statement storing buffer(element)
WCHAR			**erm,			// Pointer of storing error message
CLI_INF			*cli,			// reservation_infos data for insert
Day				*dte,			// date of insert/update
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk, fct, cii;
	Dbs_fld		*fld;

	if ( chk = !!( fct = Fdf_Cli_Inf( &fld, cli, NULL, NULL, 0 ) ) ){
		Set_Nul(cli->mei, FLD_INT,FALSE);				// modified_employee_id
		DBS_INT_CPY(cli->mei, mng->lgi->id);
		if ( Nul_Fld(cli->id,FLD_INT) ){
			Set_Nul(cli->cdt,FLD_DTM,FALSE);
			*DBS_DTM_PTR(cli->cdt) = *dte;
			chk = !!( cii = SQL_Exc_Ins( sql, ssz, TBN_CLI_INF, erm, fld, fct, dbs ) );
		}
		else{
			Set_Nul(cli->mdt,FLD_DTM,FALSE);		// set modified date when update
			*DBS_DTM_PTR(cli->mdt) = *dte;
			Set_Nul(cli->ddt,FLD_DTM,TRUE);			// set null into deleted date
			chk = SQL_Exc_Upd( sql, ssz, TBN_CLI_INF, erm, fld, fct, dbs );
			cii = DBS_INT_VAL(cli->id);
		}
		free( fld );
	}
	return( chk ? cii: 0  );
}

static	int		sav_rif(		// Insert or update a reservation_infos
WCHAR			*sql,			// SQL statement storing buffer
int				ssz,			// sizeof SQL statement storing buffer(element)
WCHAR			**erm,			// Pointer of storing error message
RSV_INF			*inf,			// reservation_infos data
int				rvn,			// reservation_number
Day				*dte,			// date of insert/update
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk, rii, flc;
	Dbs_fld		*fld;

	if ( chk = !!( flc = Fdf_Rsv_Inf( &fld, inf, NULL, NULL, 0 ) ) ){
		Set_Nul(inf->mei, FLD_INT,FALSE);
		DBS_INT_CPY(inf->mei, mng->lgi->id);			// client_kind
		Set_Nul(inf->rvn, FLD_INT,FALSE);
		*DBS_INT_PTR(inf->rvn) = rvn;				// reservation_number

		if ( Nul_Fld(inf->id, FLD_INT) ){
			Set_Nul(inf->cdt,FLD_DTM,FALSE);
			*DBS_DTM_PTR(inf->cdt) = *dte;
			Set_Nul(inf->rcd,FLD_DTM,FALSE);
			*DBS_DTM_PTR(inf->rcd) = *dte;
			chk = !!( rii = SQL_Exc_Ins( sql, ssz, TBN_RSV_INF, erm, fld, flc, dbs ) );
		}
		else{
			Set_Nul(inf->mdt,FLD_DTM,FALSE);		// set modified date when update
			Get_Dte( DBS_DTM_PTR(inf->mdt) );
			rii = DBS_INT_VAL(inf->id);
			chk = SQL_Exc_Upd( sql, ssz, TBN_RSV_INF, erm, fld, flc, dbs );
		}
		free( fld );
	}
	return( chk ? rii: 0 );
}

static	int		sav_rcg(		// saving reservation_charges
WCHAR			*sql,			// SQL statement storing buffer
int				ssz,			// sizeof SQL statement storing buffer(element)
WCHAR			**erm,			// Pointer of storing error message
int				rii,			// id of reservation_infos
int				rdi,			// id of reservation_details
Csr				crt,			// root of reservation_charges data for insert/update
Day				*dte,			// date of insert/update
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk, rci, flc, pvc=0;
	Dbs_fld		*fld;
	WCHAR		*sel;
	RSV_RCG_DAT	*dat;
	RSV_CRG		tmp[1], *rcg, *prv;
	Lnk			rot[1], *csr=NULL, *cse=NULL;
	QRY_UPD_FNC	fnc;

	memset( tmp, 0, sizeof(tmp) );
	memset( rot, 0, sizeof(rot) );
	chk  = !!( flc = Fdf_Rsv_Crg( &fld, tmp, NULL, NULL, 0 ) );
	chk &= !!(Fdf_Mak_Sll( &sel, FDF_RSV_CRG, NULL, NULL ));

	if ( chk ){
		swprintf_s( sql, ssz, 
			_T("SELECT %s FROM %s WHERE reservation_info_id=%d ")
			_T("AND reservation_detail_id=%d"), sel, TBN_RSV_CRG, rii, rdi );
		chk = SQL_Get_Lst( rot, sql, fld, flc, tmp, sizeof(tmp), erm, dbs );
	}
	if ( fld )	free( fld );

	while ( !!chk && !!( csr=Nxt_Lnk( crt, csr ) ) ){
		dat = Rec_Of(RSV_RCG_DAT, csr);
		rcg = dat->rcg;
		if ( chk  = !!( flc = Fdf_Rsv_Crg( &fld, rcg, NULL, NULL, 0 ) ) ){
			cse = pvc<Cnt_Lnk( rot ) ? Nxt_Lnk( rot, cse ): NULL;
			Set_Nul(rcg->rii,FLD_INT,FALSE);
			*DBS_INT_PTR(rcg->rii) = rii;
			Set_Nul(rcg->rdi,FLD_INT,FALSE);
			*DBS_INT_PTR(rcg->rdi) = rdi;
			Set_Nul(rcg->mei,FLD_INT,FALSE);
			DBS_INT_CPY(rcg->mei, mng->lgi->id);
			if ( cse ){
				prv = Rec_Of(RSV_CRG, cse);
				DBS_INT_CPY(rcg->id, prv->id);
				DBS_DTM_CPY(rcg->cdt, prv->cdt);
				Set_Nul(rcg->ddt, FLD_DTM, TRUE);
				Set_Nul(rcg->mdt, FLD_DTM, FALSE);
				*DBS_DTM_PTR(rcg->mdt) = *dte;
				pvc++;
			}
			else{
				Set_Nul(rcg->id, FLD_INT, TRUE);
				Set_Nul(rcg->cdt, FLD_DTM, FALSE);
				*DBS_DTM_PTR(rcg->cdt) = *dte;
			}
			fnc = !!cse ? SQL_Exc_Upd: SQL_Exc_Ins;
			if ( chk = ( rci = fnc( sql, ssz, TBN_RSV_CRG, erm, fld, flc, dbs ) != 0 ) ){
				Set_Nul(rcg->id, FLD_INT, FALSE);
				*DBS_INT_PTR(rcg->id) = rci;
			}
			free( fld );
		}
	}

	Del_All( rot );
	if ( sel )	free( sel );

	return( chk );
}

static	int		sav_req(		// saving reservation_equipments
WCHAR			*sql,			// SQL statement storing buffer
int				ssz,			// sizeof SQL statement storing buffer(element)
WCHAR			**erm,			// Pointer of storing error message
int				rii,			// id of reservation_infos
int				rdi,			// id of reservation_details
Csr				ert,			// root of reservation_equipments data for insert/update
Day				*dte,			// date of insert/update
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk, rei, flc, pvc=0;
	Dbs_fld		*fld;
	WCHAR		*sel, dbf[128], sdb[40], edb[40];
	RSV_EQP_DAT	*req;
	RSV_EQP		tmp[1], *prv;
	Lnk			rot[1], *csr=NULL, *cse=NULL;
	QRY_UPD_FNC	fnc;

	memset( tmp, 0, sizeof(tmp) );
	memset( rot, 0, sizeof(rot) );
	chk  = !!( flc = Fdf_Rsv_Eqp( &fld, tmp, NULL, NULL, 0 ) );
	chk &= !!(Fdf_Mak_Sll( &sel, FDF_RSV_EQP, NULL, NULL ));

	if ( chk ){
		swprintf_s( sql, ssz, 
			_T("SELECT %s FROM %s WHERE reservation_info_id=%d ")
			_T("AND reservation_detail_id=%d"), sel, TBN_RSV_EQP, rii, rdi );
		chk = SQL_Get_Lst( rot, sql, fld, flc, tmp, sizeof(tmp), erm, dbs );
	}
	if ( fld )	free( fld );

	while ( !!chk && !!( csr=Nxt_Lnk( ert, csr ) ) ){
		req = Rec_Of(RSV_EQP_DAT, csr);
		if ( chk  = !!( flc = Fdf_Rsv_Eqp( &fld, req->req, NULL, NULL, 0 ) ) ){
			cse = pvc<Cnt_Lnk( rot ) ? Nxt_Lnk( rot, cse ): NULL;
			Set_Nul(req->req->rii,FLD_INT,FALSE);
			*DBS_INT_PTR(req->req->rii) = rii;
			Set_Nul(req->req->rdi,FLD_INT,FALSE);
			*DBS_INT_PTR(req->req->rdi) = rdi;
			Set_Nul(req->req->mei,FLD_INT,FALSE);
			DBS_INT_CPY(req->req->mei, mng->lgi->id);
			if ( cse ){
				prv = Rec_Of(RSV_EQP, cse);
				DBS_INT_CPY(req->req->id, prv->id);
				DBS_DTM_CPY(req->req->cdt, prv->cdt);
				Set_Nul(req->req->ddt, FLD_DTM, TRUE);
				Set_Nul(req->req->mdt, FLD_DTM, FALSE);
				*DBS_DTM_PTR(req->req->mdt) = *dte;
				pvc++;
			}
			else{
				Set_Nul(req->req->id, FLD_INT, TRUE);
				Set_Nul(req->req->cdt, FLD_DTM, FALSE);
				*DBS_DTM_PTR(req->req->cdt) = *dte;
			}
			fnc = !!cse ? SQL_Exc_Upd: SQL_Exc_Ins;
			if ( chk = ( rei = fnc( sql, ssz, TBN_RSV_EQP, erm, fld, flc, dbs ) ) != 0 ){
				Set_Nul(req->req->id, FLD_INT, FALSE);
				*DBS_INT_PTR(req->req->id) = rei;
			}
			if ( chk ){
				swprintf_s( dbf, memcnt(dbf),
					Cmp_Dte(DBS_DAY_PTR(req->req->usd), DBS_DAY_PTR(req->req->ued))==0 ? 
					_T("AND use_date='%s'"):_T("AND use_date>='%s' AND use_date<'%s' " ), 
					Edt_Dte( sdb, memcnt(sdb), DBS_DAY_PTR(req->req->usd) ), 
					Edt_Dte( edb, memcnt(edb), DBS_DAY_PTR(req->req->ued) ) );
				swprintf_s( sql, ssz,
					_T("UPDATE reservation_charges SET reservation_equipment_id=%d ")
					_T("WHERE reservation_info_id=%d AND reservation_detail_id=%d ")
					_T("%s AND deleted_date IS NULL" ),
					rei, rii, rdi, dbf );
				chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
			}
			free( fld );
		}
	}

	Del_All( rot );
	if ( sel )	free( sel );

	return( chk );
}


static	int		sav_dtl(		// save reservation_details
WCHAR			*sql,			// SQL statement storing buffer
int				ssz,			// sizeof SQL statement storing buffer(element)
WCHAR			**erm,			// Pointer of storing error message
int				rii,			// id of reservation_infos
int				cii,			// id of client_infos on reservation_infos
Csr				drt,			// root of reservation_details data for insert/update
Day				*dte,			// date of insert/update
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk, rdi, flc, pvc=0;
	Dbs_fld		*fld;
	WCHAR		*sel;
	RSV_DTL_DAT	*dtl;
	RSV_DTL		tmp[1], *prv;
	Lnk			rot[1], *csr=NULL, *cse=NULL;
	QRY_UPD_FNC	fnc;

	memset( tmp, 0, sizeof(tmp) );
	memset( rot, 0, sizeof(rot) );
	chk  = !!( flc = Fdf_Rsv_Dtl( &fld, tmp, NULL, NULL, 0 ) );
	chk &= !!(Fdf_Mak_Sll( &sel, FDF_RSV_DTL, NULL, NULL ));

	if ( chk ){
		swprintf_s( sql, ssz, 
			_T("SELECT %s FROM %s WHERE reservation_info_id=%d "), sel, TBN_RSV_DTL, rii );
		chk = SQL_Get_Lst( rot, sql, fld, flc, tmp, sizeof(tmp), erm, dbs );
	}
	if ( fld )	free( fld );

	while ( !!chk && !!( csr=Nxt_Lnk( drt, csr ) ) ){
		dtl = Rec_Of(RSV_DTL_DAT, csr);
		if ( chk  = !!( flc = Fdf_Rsv_Dtl( &fld, dtl->dtl, NULL, NULL, 0 ) ) ){
			cse = pvc<Cnt_Lnk( rot ) ? Nxt_Lnk( rot, cse ): NULL;
			if ( !dtl->smr ){
				if ( !Nul_Fld(dtl->cli->fnm, FLD_WST) ){	
					// Saving client_infos related reservation_infos when it is not same the reservation_infos
					chk = !!( cii = sav_cli( sql, ssz, erm, dtl->cli, dte, dbs, mng ) );
				}
			}
			else{
				DBS_INT_SET(dtl->dtl->cii, &cii);
			}
			if ( chk ){
				Set_Nul(dtl->dtl->rii,FLD_INT,FALSE);
				*DBS_INT_PTR(dtl->dtl->rii) = rii;
				if ( !Nul_Fld(dtl->cli->fnm, FLD_WST) ){
					Set_Nul(dtl->dtl->cii,FLD_INT,FALSE);
					*DBS_INT_PTR(dtl->dtl->cii) = cii;
				}
				Set_Nul(dtl->dtl->mei,FLD_INT,FALSE);
				DBS_INT_CPY(dtl->dtl->mei, mng->lgi->id);
				if ( cse ){
					prv = Rec_Of(RSV_DTL, cse);
					DBS_INT_CPY(dtl->dtl->id, prv->id);
					Set_Nul(dtl->dtl->ddt, FLD_DTM, TRUE);
					Set_Nul(dtl->dtl->mdt, FLD_DTM, FALSE);
					*DBS_DTM_PTR(dtl->dtl->mdt) = *dte;
					pvc++;
				}
				else{
					Set_Nul(dtl->dtl->id, FLD_INT, TRUE);
					Set_Nul(dtl->dtl->cdt, FLD_DTM, FALSE);
					*DBS_DTM_PTR(dtl->dtl->cdt) = *dte;
				}
				fnc = !!cse ? SQL_Exc_Upd: SQL_Exc_Ins;
				if ( chk = ( rdi = fnc( sql, ssz, TBN_RSV_DTL, erm, fld, flc, dbs ) ) != 0 ){
					Set_Nul(dtl->dtl->id, FLD_INT, FALSE);
					*DBS_INT_PTR(dtl->dtl->id) = rdi;
				}
				if ( chk ){
					// save the reservation_charges
					chk = sav_rcg( sql, ssz, erm, rii, rdi, dtl->rcg, dte, dbs, mng );
				}
				if ( chk ){
					// save the reservation_equipments
					chk = sav_req( sql, ssz, erm, rii, rdi, dtl->req, dte, dbs, mng );
				}
			}
			free( fld );
		}
	}

	Del_All( rot );
	if ( sel )	free( sel );

	return( chk );
}

int		Rsv_Rsv_Sav(			// Insert a new reservation data
WCHAR			**erm,			// Pointer of storing error message
RSV_INF_DAT		*inf,			// reservation_infos data for insert
Day				*dte,			// datetime of insert/update
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk, rvn, rii, cii, siz=8192;
	WCHAR		*sql;
	Csr			csr=NULL;

	if ( chk = !!( sql = (WCHAR *)malloc( wsizeof(siz) ) ) ){
		chk = Dbs_Trn( erm, dbs, TRN_STT );		// Start transaction
		chk = !!chk && !!del_all( sql, siz, erm, inf->rsv, dte, dbs, mng );

		if ( chk ){
			rvn = DBS_INT_VAL(inf->rsv->rvn);
			// At first, saving the client_infos table related reservation_infos
			chk = !!( cii = sav_cli( sql, siz, erm, inf->cli, dte, dbs, mng ) );
		}
		if ( chk && ( Nul_Fld(inf->rsv->id, FLD_INT) ) ){
			// Getting new reservation_number when creating new reservation
			Set_Nul( inf->rsv->cii, FLD_INT, FALSE );
			*DBS_INT_PTR( inf->rsv->cii) = cii;
			chk = !!( rvn = new_rvn( sql, siz, erm, inf, dte, dbs, mng ) );
		}
		if ( chk ){
			// insert/update the reservation_infos table
			chk = !!( rii = sav_rif( sql, siz, erm, inf->rsv, rvn, dte, dbs, mng ) );
		}
		if ( chk ){
			// insert/update the reservation_details, reservation_equipments, reservation_charges
			chk = sav_dtl( sql, siz, erm, rii, cii, inf->dtl, dte, dbs, mng );
		}

#ifdef	_DEBUG
//chk = FALSE;
#endif
		
		Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );

		free( sql );
	}

	return( chk );
}

int		Rsv_Rom_Avl(			// checking room avalability 
int				eii,			// equipment_info_id
int				rdi,			// Owm reservation_detail id
Day				*usd,			// use start date
Day				*ued,			// use end date
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk, rct=0;
	WCHAR		usc[DBF_SIZ_WST(50)];
	WCHAR		sql[1024], std[20], edd[20], opb[128], brb[128], fcb[128];
	Dbs_fld		fld[1] = { usc,	_T("use_count"),	FLD_WST, FLD_WST_SIZ(usc) };
	CRecordset	*rec;

	Edt_Dte( std, memcnt(std), usd );
	Edt_Dte( edd, memcnt(edd), ued );

	swprintf_s( sql, memcnt(sql), 
			_T("SELECT COUNT(*) AS use_count ")
			_T("FROM %s RE ")												// reservation_equipments
			_T("LEFT JOIN %s RI ON RI.id = RE.reservation_info_id ")		// reservation_infos 
			_T("LEFT JOIN %s RD ON RD.id = RE.reservation_detail_id ")		// reservation_details
			_T("WHERE ( ( RE.use_start_date>='%s' AND RE.use_start_date<'%s' ) ")
			_T("OR ( RE.use_end_date>'%s' AND RE.use_end_date<='%s' ) ")
			_T("OR ( RE.use_start_date<='%s' AND RE.use_end_date>'%s' ) ")
			_T(") AND RE.reservation_detail_id != %d AND RE.equipment_info_id IS NOT NULL ")
			_T("AND RD.canceled = 0 AND RE.deleted_date IS NULL AND RE.equipment_info_id=%d ")
			_T("AND RI.operator_code='%s' AND RI.branch_code='%s' AND RI.facility_code='%s' "),  
			TBN_RSV_EQP, TBN_RSV_INF, TBN_RSV_DTL, std, edd, std, edd, std, edd, rdi, eii,
			SQL_Edt_Wst( opb, memcnt(opb), mng->opc ),
			SQL_Edt_Wst( brb, memcnt(brb), mng->brc ), 
			SQL_Edt_Wst( fcb, memcnt(fcb), mng->fcc ) );

	rec = Dbs_Qry( dbs, sql, erm );

	if ( chk = !!rec ){
		if ( *erm )	free( *erm );
		*erm = NULL;
		chk = Dbs_Get( fld, erm, memcnt(fld), rec )==QRY_NML;
		Dbs_Cls(rec);
	}
	return( chk ? _wtoi(DBS_WST_PTR(usc)): -1 );
}

int		Rsv_Avl_Rms(			// getting available room list
Csr				rot,			// root of storiing data
int				eti,			// equipmenttype_id ( all if 0 )
int				rdi,			// Owm reservation_detail id
Day				*usd,			// use start date
Day				*ued,			// use end date
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Property Management System Structure
{
	int			chk, flc;
	RSV_EQP_INF	eqi[1];
	WCHAR		sql[1280], tyb[40], std[20], edd[20], opb[128], brb[128], fcb[128];;
	Dbs_fld		*fld;
	Dbs_fld		ext[] = {	{ eqi->eti, _T("ET_id"),	FLD_INT, FLD_INT_SIZ(eqi->eti) },
							{ eqi->esi, _T("ES_id"),	FLD_INT, FLD_INT_SIZ(eqi->esi) },
							{ eqi->tnm, _T("type_name"),FLD_WST, FLD_WST_SIZ(eqi->tnm) } };

	memset( eqi, 0, sizeof(eqi) );

	swprintf_s( tyb, memcnt(tyb), eti ? _T("AND EI.equipment_type_id=%d "): _T(""), eti );
	Edt_Dte( std, memcnt(std), usd );
	Edt_Dte( edd, memcnt(edd), ued );

	if ( chk  = !!( flc = Fdf_Eqp_Inf( &fld, eqi->eqi, NULL, ext, memcnt(ext) ) ) ){
		swprintf_s( sql, memcnt(sql),
			_T("SELECT EI.*, ET.id AS ET_id, ET.name AS type_name,")
			_T("ES.id AS ES_id ")
			_T("FROM %s EI, %s ET, %s ES ")										// equipment_infos, equipment_types, equipment_states
			_T("WHERE ET.id = EI.equipment_type_id ")
			_T("AND ES.equipment_info_id= EI.id ")
			_T("AND EI.operator_code='%s' AND EI.branch_code='%s' AND EI.facility_code='%s' ")
			_T("AND EI.deleted_date IS NULL AND EI.sale_enable!=0 ")
			_T("%s AND EI.id NOT IN ( ")
			_T("SELECT equipment_info_id FROM %s RE ")							// reservation_equipments
			_T("LEFT JOIN %s RD ON RD.id = RE.reservation_detail_id ")			// reservation_details
			_T("WHERE (	( RE.use_start_date>='%s' AND RE.use_start_date<'%s' ) ")
			_T("OR ( RE.use_end_date>'%s' AND RE.use_end_date<='%s' ) ")
			_T("OR ( RE.use_start_date<='%s' AND RE.use_end_date>'%s' ) ")
			_T(") AND RE.reservation_detail_id != %d AND RE.equipment_info_id IS NOT NULL ")
			_T("AND RD.canceled = 0 AND RE.deleted_date IS NULL ) ")
			_T("ORDER BY EI.equipment_code "),
			TBN_EQP_INF, TBN_EQP_TYP, TBN_EQP_STS, 
			SQL_Edt_Wst( opb, memcnt(opb), mng->opc ),
			SQL_Edt_Wst( brb, memcnt(brb), mng->brc ), 
			SQL_Edt_Wst( fcb, memcnt(fcb), mng->fcc ), tyb,
			TBN_RSV_EQP, TBN_RSV_DTL, std, edd, std, edd, std, edd, rdi );
			chk = SQL_Get_Lst( rot, sql, fld, flc, eqi, sizeof(eqi), erm, dbs );
		free( fld );
	}
	
	return( chk );
}

int		Rsv_Idv_Cxl(			// Cancellation direct
int			rdi,				// target reservation_detail_id
int			rii,				// target reservation_info_id
WCHAR		**erm,				// Pointer of storing error message
CDatabase	*dbs,				// ODBC Database object
PMS_MNG		*mng )				// Property Management System Structure
{
	int			chk;
	Lnk			rot[1], *csr;

	memset( rot, 0, sizeof(rot) );

	if ( chk = !!( csr = Add_Lnk( rot, NULL, sizeof(int) ) ) ){
		*Rec_Of(int, csr) = rdi;
		chk = Rsv_Cxl_Rsv( rot, rii, erm, dbs, mng );
	}

	Del_All( rot );
	/*
	WCHAR		sql[1024], dbf[40];
	Day			upd[1];
	int			chk, rct=0, sts, ncx[DBF_SIZ_INT], rii[DBF_SIZ_INT];
	CRecordset	*rec;
	Dbs_fld		fld[1] =	{ rii, _T("reservation_info_id"), FLD_INT, FLD_INT_SIZ(ncx) };

	Get_Dte( upd );

	if ( chk = Dbs_Trn( erm, dbs, TRN_STT ) ){
		swprintf_s( sql, memcnt(sql),
			_T("UPDATE %s SET canceled=1, ")		// reservation_details
			_T("modified_employee_id=%d, modified_date='%s' ")
			_T("WHERE id=%d " ), TBN_RSV_DTL, mng->lgi->epi, Edt_Dtm(dbf, memcnt(dbf), upd ), rdi );
		chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
		if ( chk ){
			swprintf_s( sql, memcnt(sql),
				_T("SELECT reservation_info_id ")
				_T("FROM %s ")						// reservation_details
				_T("WHERE canceled=0 AND reservation_info_id IN ( " )
				_T("SELECT reservation_info_id FROM reservation_details ")
				_T("WHERE id = %d ) ")
				_T("AND deleted_date IS NULL "), TBN_RSV_DTL, rdi );
			rec = Dbs_Qry( dbs, sql, erm );

			if ( chk = !!rec ){
				sts = Dbs_Get( fld, erm, memcnt(fld), rec );
				switch( sts ){
				case QRY_END:				break;
				case QRY_NML:	rct++;		break;
				default:		chk=FALSE;	break;
				}
				Dbs_Cls(rec);
			}
			if ( !!chk && !rct ){
				swprintf_s( sql, memcnt(sql),
					_T("UPDATE %s SET canceled=1, ")		// reservation_infos
					_T("modified_employee_id=%d, modified_date='%s' ")
					_T("WHERE id IN (" )
					_T("SELECT reservation_info_id FROM reservation_details ")
					_T("WHERE id = %d ) "),
					TBN_RSV_INF, mng->lgi->epi, Edt_Dtm(dbf, memcnt(dbf), upd ), rdi );
				chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
			}
		}
		Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
	}
	*/

	return( chk );
}


int		Rsv_Cxl_Rsv(			// Execute to cancel erservation
Csr			rot,				// root of target reservation_detail_id
int			rii,				// reservation_info_id
WCHAR		**erm,				// Pointer of storing error message
CDatabase	*dbs,				// ODBC Database object
PMS_MNG		*mng )				// Property Management System Structure
{
	WCHAR		*sql, *kys=NULL, dbf[40];
	Day			upd[1];
	Csr			csr=NULL;
	int			chk=TRUE, rct=0, sts, ncx[DBF_SIZ_INT], /*rii[DBF_SIZ_INT],*/ siz;
	CRecordset	*rec;
	Dbs_fld		fld[1] =	{ ncx, _T("reservation_info_id"), FLD_INT, FLD_INT_SIZ(ncx) };

	Get_Dte( upd );

	while( chk && !!( csr = Nxt_Lnk( rot, csr ) ) ){
		swprintf_s( dbf, memcnt(dbf), _T("%s%d"), kys ? _T(","): _T(""), *Rec_Of(int, csr) );
		chk = !!( kys = Add_Str( kys, dbf ) );
	}

	siz = (int)wcslen( kys ) + 1024;

	if ( chk && ( chk = !!( sql = (WCHAR *)malloc( wsizeof( siz ) ) ) ) ){
		;
		if ( chk = Dbs_Trn( erm, dbs, TRN_STT ) ){
			swprintf_s( sql,siz,
				_T("UPDATE %s SET canceled=1, ")		// reservation_details
				_T("modified_employee_id=%d, modified_date='%s' ")
				_T("WHERE id IN ( %s ) " ), TBN_RSV_DTL, DBS_INT_VAL(mng->lgi->id), Edt_Dtm(dbf, memcnt(dbf), upd ), kys );
			chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
			if ( chk ){
				swprintf_s( sql, siz,
					_T("SELECT reservation_info_id ")
					_T("FROM %s ")						// reservation_details
//					_T("WHERE canceled=0 AND reservation_info_id IN ( " )
//					_T("SELECT reservation_info_id FROM reservation_details ")
//					_T("WHERE id = %d ) ")
					_T("WHERE canceled=0 AND reservation_info_id=%d ")
					_T("AND deleted_date IS NULL "), TBN_RSV_DTL, rii/*rdi*/ );
				rec = Dbs_Qry( dbs, sql, erm );

				if ( chk = !!rec ){
					sts = Dbs_Get( fld, erm, memcnt(fld), rec );
					switch( sts ){
					case QRY_END:				break;
					case QRY_NML:	rct++;		break;
					default:		chk=FALSE;	break;
					}
					Dbs_Cls(rec);
				}
				if ( !!chk && !rct ){
					swprintf_s( sql, siz,
						_T("UPDATE %s SET canceled=1, ")		// reservation_infos
						_T("modified_employee_id=%d, modified_date='%s' ")
						_T("WHERE id=%d" ),
						//_T("WHERE id IN (" )
						//_T("SELECT reservation_info_id FROM reservation_details ")
						//_T("WHERE id = %d ) "),
						TBN_RSV_INF, DBS_INT_VAL(mng->lgi->id), Edt_Dtm(dbf, memcnt(dbf), upd ), rii/*rdi*/ );
					chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
				}
			}
			Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
		}
		free( sql );
	}
	if ( kys )	free( kys );

	return( chk );
}

//---------------------------------------------------------------------------------------------
// Only use from operation on BookingStatus

int		Rsv_Get_Eci(			// Getting equipent_charge_info_id from reservation_equipments
int			rei,				// reservation_equipment_id
WCHAR		**erm,				// Pointer of storing error message
CDatabase	*dbs,				// ODBC Database object
PMS_MNG		*mng )				// Property Management System Structure
{
	int			chk, eci[DBF_SIZ_INT];
	WCHAR		sql[512];
	Dbs_fld		fld[1] = { eci, _T("equipment_charge_info_id"), FLD_INT, FLD_INT_SIZ(eci) };
	CRecordset	*rec;

	memset( eci, 0, sizeof(eci) );

	swprintf_s( sql, memcnt(sql),
		_T("SELECT ED.equipment_charge_info_id ") 
		_T("FROM %s RC ")												// reservation_charges
		_T("LEFT JOIN %s ED ON ED.id = RC.equipment_charge_detail_id ")	// equipment_charge_details
		_T("WHERE RC.reservation_equipment_id=%d "), TBN_RSV_CRG, TBN_EQP_CGD, rei );

	rec = Dbs_Qry( dbs, sql, erm );

	if ( chk = !!rec ){		// Only first one getting
		if ( *erm )	free( *erm );
		*erm = NULL;
		chk = Dbs_Get( fld, erm, memcnt(fld), rec )==QRY_NML;
		Dbs_Cls(rec);
	}
	return( chk ? DBS_INT_VAL(eci): -1 );
}

int		Rsv_Get_Rdt(			// Getting reservation_details
RSV_DTL		*dst,				// pointer of destination 
int			rdi,				// reservation_equipment_id
WCHAR		**erm,				// Pointer of storing error message
CDatabase	*dbs,				// ODBC Database object
PMS_MNG		*mng )				// Property Management System Structure
{
	int			chk, flc;
	WCHAR		sql[256];
	Dbs_fld		*fld;
	CRecordset	*rec;

	if ( chk = !!( flc = Fdf_Rsv_Dtl( &fld, dst, NULL, NULL, 0 ) ) ){
		swprintf_s( sql, memcnt(sql),
			_T("SELECT * FROM %s WHERE id=%d "), TBN_RSV_DTL, rdi );	// reservation_details
		rec = Dbs_Qry( dbs, sql, erm );

		if ( chk = !!rec ){		// Only first one getting
			chk = Dbs_Get( fld, erm, flc, rec )==QRY_NML;
			Dbs_Cls(rec);
		}
	}
	return( chk );
}

int		Rsv_Chg_Req(			// change the stay period and rooms
RSV_EQP		*req,				// reservation_equipment(chainging specification
Csr			rot,				// root of reservation_charges data
int			rmc,				// has some room changes
Day			*upd,				// updated date
WCHAR		**erm,				// Pointer of storing error message
CDatabase	*dbs,				// ODBC Database object
PMS_MNG		*mng )				// Property Management System Structure
{
	int			chk, flc, cnt=0;
	WCHAR		sql[2560], *sel, dbf[20], sdb[29], edb[20];
	Lnk			ort[1], *csr=NULL, *cse=NULL;
	RSV_CRG		tmp[1], *crg, *old;
	Dbs_fld		*fld;
	QRY_UPD_FNC	fnc;

	memset( ort, 0, sizeof(ort) );

	chk = !!( Fdf_Mak_Sll( &sel, FDF_RSV_CRG, NULL, NULL ) );

	if ( chk = Dbs_Trn( erm, dbs, TRN_STT ) ){
		// delete all reservation_charges
		swprintf_s( sql, memcnt(sql), 
				_T("UPDATE %s SET deleted_date='%s', modified_employee_id=%d, modified_date='%s' ")
				_T("WHERE reservation_equipment_id=%d"),
				TBN_RSV_CRG, Edt_Dte(dbf, memcnt(dbf), upd), DBS_INT_VAL(mng->lgi->id), dbf, DBS_INT_VAL(req->id) );
		if( chk = Dbs_Upd( dbs, sql, erm )==QRY_NML ){
			swprintf_s( sql, memcnt(sql),
				_T("UPDATE %s SET ")
				_T("use_start_date='%s', use_end_date='%s', equipment_type_id=%d, equipment_info_id=%d,")
				_T("modified_employee_id=%d, modified_date='%s' ")
				_T("WHERE id=%d"), TBN_RSV_EQP,
				Edt_Dte(sdb, memcnt(sdb), DBS_DAY_PTR(req->usd)),
				Edt_Dte(edb, memcnt(edb), DBS_DAY_PTR(req->ued)),
				DBS_INT_VAL(req->eti), DBS_INT_VAL(req->eii), DBS_INT_VAL(mng->lgi->id), dbf, DBS_INT_VAL(req->id) );
			chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
		}
		if ( chk && !rmc ){
			swprintf_s( sql, memcnt(sql),
				_T("UPDATE %s SET ")
				_T("arrival_date='%s', departure_date='%s', modified_employee_id=%d, modified_date='%s' ")
				_T("WHERE id=%d "),
				TBN_RSV_DTL, sdb, edb, DBS_INT_VAL(mng->lgi->id), dbf, DBS_INT_VAL(req->rdi) );
			if ( chk = Dbs_Upd( dbs, sql, erm )==QRY_NML ){
				swprintf_s( sql, memcnt(sql),
					_T("UPDATE %s SET ")
					_T("first_arrival_date=")
					_T("(SELECT min(arrival_date) FROM reservation_details ")
					_T("WHERE reservation_info_id=%d AND canceled=0 AND deleted_date IS NULL ),")
					_T("last_departure_date=") 
					_T("(SELECT max(departure_date) FROM reservation_details ")
					_T("WHERE reservation_info_id=%d AND canceled=0 AND deleted_date IS NULL ),")
					_T("modified_employee_id=%d, modified_date='%s' ")
					_T("WHERE id=%d "),
					TBN_RSV_INF, DBS_INT_VAL(req->rii), DBS_INT_VAL(req->rii),
					DBS_INT_VAL(mng->lgi->id), dbf, DBS_INT_VAL(req->rii) );
				chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
			}

		}
		if ( chk && ( chk  = !!( flc = Fdf_Rsv_Crg( &fld, tmp, NULL, NULL, 0 ) ) ) ){
			swprintf_s( sql, memcnt(sql),
				_T("SELECT %s ")
				_T("FROM %s ")
				_T("WHERE reservation_equipment_id=%d"), sel, TBN_RSV_CRG, DBS_INT_VAL(req->id) );
			chk = SQL_Get_Lst( ort, sql, fld, flc, tmp, sizeof(tmp), erm, dbs );
			free( fld );
		}
		while( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
			crg = Rec_Of(RSV_CRG, csr);
			cse = ( cnt < Cnt_Lnk( ort ) ) ? Nxt_Lnk( ort, cse ): NULL;
			DBS_INT_CPY(crg->mei, mng->lgi->id);
			if ( cse ){
				old = Rec_Of(RSV_CRG, cse);
				DBS_INT_CPY(crg->id, old->id);
				DBS_DTM_CPY(crg->cdt, old->cdt);
				DBS_DTM_SET(crg->mdt, upd);
				Set_Nul(crg->ddt, FLD_DTM, TRUE);
				cnt++;
			}
			else{
				DBS_DTM_SET(crg->cdt, upd);
			}
			if ( chk = !!( flc = Fdf_Rsv_Crg( &fld, crg, NULL, NULL, 0 ) ) ){
				fnc = !!cse ? SQL_Exc_Upd: SQL_Exc_Ins;
				chk = fnc( sql, memcnt(sql), TBN_RSV_CRG, erm, fld, flc, dbs ) != 0;
				free( fld );
			}
		}
		Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
	}

	Del_All( ort );
	if ( sel )		free( sel );

	return( chk );
}

int		Rsv_Cpy_Req(			// change the stay period and rooms
RSV_EQP		*req,				// reservation_equipment(chainging specification
Csr			rot,				// root of reservation_charges data
int			ori,				// old reservation_equipment_id
Day			*upd,				// updated date
WCHAR		**erm,				// Pointer of storing error message
CDatabase	*dbs,				// ODBC Database object
PMS_MNG		*mng )				// Property Management System Structure
{
	int			chk, flc, rei;
	WCHAR		sql[1024], *sel, dbf[20], sdb[20], edb[20];
	Dbs_fld		*fld;
	Lnk			ort[1], *csr=NULL, *cse=NULL;
	RSV_CRG		tmp[1], *crg, *old;

	memset( ort, 0, sizeof(ort) );
	memset( tmp, 0, sizeof(tmp) );

	chk = Dbs_Trn( erm, dbs, TRN_STT );
	if ( chk ){
		chk = !!( Fdf_Mak_Sll( &sel, FDF_RSV_CRG, NULL, NULL ) );

		if ( chk && ( chk  = !!( flc = Fdf_Rsv_Eqp( &fld, req, NULL, NULL, 0 ) ) ) ){
			DBS_DAY_SET(req->cdt, upd);
			DBS_INT_CPY(req->mei, mng->lgi->id);
			chk = ( rei = SQL_Exc_Ins( sql, memcnt(sql), TBN_RSV_EQP, erm, fld, flc, dbs ) ) > 0;
			free( fld );
		}
		if ( chk ){
			if( chk = !!( flc = Fdf_Rsv_Crg( &fld, tmp, NULL, NULL, 0 ) ) ){
				swprintf_s( sql, memcnt(sql),
					_T("SELECT %s ")
					_T("FROM %s ")
					_T("WHERE reservation_equipment_id=%d ")
					_T("AND use_date>='%s' AND use_date<='%s' ")
					_T("AND deleted_date IS NULL ")
					_T("ORDER BY use_date "), sel, TBN_RSV_CRG, ori,
					Edt_Dte(sdb, memcnt(sdb), DBS_DAY_PTR(req->usd)),
					Edt_Dte(edb, memcnt(edb), DBS_DAY_PTR(req->ued)) );
				chk = SQL_Get_Lst( ort, sql, fld, flc, tmp, sizeof(tmp), erm, dbs );
				free( fld );
			}
		}
		if ( chk ){
			while( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
				crg = Rec_Of(RSV_CRG, csr);
				if ( chk = !!( cse = Nxt_Lnk( ort, cse ) ) ){
					old = Rec_Of(RSV_CRG, cse);
					DBS_DBL_CPY(old->amt, crg->amt);
					*DBS_INT_PTR(old->rei) = rei;
					//	Day				udt[DBF_SIZ_DAY];		// use_date
					DBS_INT_CPY(old->cgk, crg->cgk);
					DBS_INT_CPY(old->dsi, crg->dsi);
					DBS_INT_CPY(old->edi, crg->edi);
					DBS_DAY_SET(old->mdt, upd);
					DBS_INT_CPY(old->mei, mng->lgi->id);
					if( chk = !!( flc = Fdf_Rsv_Crg( &fld, old, NULL, NULL, 0 ) ) ){
						chk = SQL_Exc_Upd( sql, memcnt(sql), TBN_RSV_CRG, erm, fld, flc, dbs ) > 0;
						free( fld );
					}
				}
			}
			Del_All( ort );
		}
		if ( chk ){
			swprintf_s( sql, memcnt(sql), 
				_T("update %s ") 
				_T("SET use_end_date=IF(use_start_date < '%s', '%s', use_end_date),")
				_T("use_start_date=IF(use_end_date > '%s', '%s', use_start_date)," )
				_T("modified_employee_id=%d, modified_date='%s' " )
				_T("WHERE id= %d "), TBN_RSV_EQP, sdb, sdb, edb, edb,
				DBS_INT_VAL(mng->lgi->id), Edt_Dtm(dbf, memcnt(dbf), upd), ori );
			chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
		}
		if ( chk ){
			DBS_INT_SET(req->id, &rei);
		}

		Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
		if ( sel )	free( sel );
	}


	return( chk );
}
