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
#include "sch_fnc.h"
#include "sch_sql.h"
#include "fld_def.h"

static	int		mak_opt(			// making a search option query
WCHAR			*dst,				// sotring pointer
int				siz,				// sizeof storing area
int				pos,				// editting position
SCH_OPT			*opt )				// 検索オプション
{
	WCHAR		buf[260];

	if ( !Nul_Fld(opt->bid, FLD_INT) ){
		pos += swprintf_s( dst+pos, siz-pos, _T("AND R.reservation_number LIKE '%d%%' "), DBS_INT_VAL(opt->bid) );
	}
	if ( !Nul_Fld(opt->rmn, FLD_WST) ){
		SQL_Edt_Wst( buf, memcnt(buf), DBS_WST_PTR(opt->rmn) );
		pos += swprintf_s( dst+pos, siz-pos, _T("AND EI.equipment_code LIKE '%s%%' "), buf );
	}
	if ( !Nul_Fld(opt->eid, FLD_WST) ){
		SQL_Edt_Wst( buf, memcnt(buf), DBS_WST_PTR(opt->eid) );
		pos += swprintf_s( dst+pos, siz-pos, _T("AND R.external_reservation_number LIKE '%s%%' "), buf );
	}
	if ( !Nul_Fld(opt->nam, FLD_WST) ){
		SQL_Edt_Wst( buf, memcnt(buf), DBS_WST_PTR(opt->nam) );
		pos += swprintf_s( dst+pos, siz-pos, 
			_T("AND ( C.first_name LIKE '%s%%' OR C.last_name LIKE '%s%%' ")
			_T("OR DC.first_name LIKE '%s%%' OR DC.last_name LIKE '%s%%' )"), buf, buf, buf, buf );
	}
	if ( !Nul_Fld(opt->arvf, FLD_DAY) ){
		pos += swprintf_s( dst+pos, siz-pos, _T("AND D.arrival_date>='%s' "), Edt_Dte(buf, memcnt(buf), DBS_DAY_PTR(opt->arvf)) );
	}
	if ( !Nul_Fld(opt->arvt, FLD_DAY) ){
		pos += swprintf_s( dst+pos, siz-pos, _T("AND D.arrival_date<='%s' "), Edt_Dte(buf, memcnt(buf), DBS_DAY_PTR(opt->arvt)) );
	}
	if ( !Nul_Fld(opt->dptf, FLD_DAY) ){
		pos += swprintf_s( dst+pos, siz-pos, _T("AND D.arrival_date>='%s' "), Edt_Dte(buf, memcnt(buf), DBS_DAY_PTR(opt->dptf)) );
	}
	if ( !Nul_Fld(opt->dptt, FLD_DAY) ){
		pos += swprintf_s( dst+pos, siz-pos, _T("AND D.arrival_date<='%s' "), Edt_Dte(buf, memcnt(buf), DBS_DAY_PTR(opt->dptt)) );
	}
	if ( !Nul_Fld(opt->bfr, FLD_INT) ){
		pos += swprintf_s( dst+pos, siz-pos, _T("AND ( R.path_of_reservation=%d AND R.agent_info_id "), DBS_INT_VAL(opt->bfr) );
		pos += swprintf_s( dst+pos, siz-pos,
			DBS_INT_VAL(opt->bfr)==VIA_AGENT ?_T("=%d ) "): _T(" IS NULL ) "), DBS_INT_VAL((int *)opt->bfr+DBF_SIZ_INT) );
	}

	return( pos );
}

int		Sch_Rsv_Lst(				// 予約一覧取得
Csr				rot,				// 格納領域
int				rii,				// reservation_info_id ( One target if this item is nit zero )
int				shi,				// 検索対象
SCH_OPT			*opt,				// 検索オプション
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	WCHAR		sql[2048], rib[40], opb[64], brb[64], fcb[64];
	SCH_RSV_LST	rsv[1];
	int			pos, fct, chk;
	Dbs_fld		*fld;
	Dbs_fld		ext[] = {	{ rsv->ckd, _T("client_kind"),					FLD_INT, FLD_INT_SIZ(rsv->ckd) },
							{ rsv->fnm, _T("first_name"),					FLD_WST, FLD_WST_SIZ(rsv->fnm) },
							{ rsv->lnm, _T("last_name"),					FLD_WST, FLD_WST_SIZ(rsv->lnm) },
							{ rsv->bdy,	_T("business_date"),				FLD_DAY, FLD_DAY_SIZ(rsv->bdy) },
							{ rsv->rdi, _T("reservation_detail_id"),		FLD_INT, FLD_INT_SIZ(rsv->rdi) },
							{ rsv->dci, _T("detail_client_id"),				FLD_INT, FLD_INT_SIZ(rsv->dci) },
							{ rsv->dfn, _T("detail_fname"),					FLD_WST, FLD_WST_SIZ(rsv->dfn) },
							{ rsv->dln, _T("detail_lname"),					FLD_WST, FLD_WST_SIZ(rsv->dln) },
							{ rsv->arv, _T("arrival_date"),					FLD_DAY, FLD_DAY_SIZ(rsv->arv) },
							{ rsv->dpt, _T("departure_date"),				FLD_DAY, FLD_DAY_SIZ(rsv->dpt) },
							{ rsv->cid, _T("check_in_date"),				FLD_DAY, FLD_DAY_SIZ(rsv->cid) },
							{ rsv->cod, _T("check_out_date"),				FLD_DAY, FLD_DAY_SIZ(rsv->cod) },
							{ rsv->cxl, _T("D_canceled"),					FLD_INT, FLD_INT_SIZ(rsv->cxl) },
							{ rsv->eii, _T("EI_id"),						FLD_INT, FLD_INT_SIZ(rsv->eii) },
							{ rsv->rno, _T("equipment_code"),				FLD_WST, FLD_WST_SIZ(rsv->rno) } };

	if ( !!rii )	swprintf_s( rib, memcnt(rib), _T("AND R.id=%d "), rii );
	else			*rib = 0;

	if ( chk = !!(fct = Fdf_Rsv_Inf( &fld, rsv->rsv, NULL, ext, memcnt(ext) )) ){
		switch( shi ){
		case SHI_RSV:	pos = swprintf_s( sql,	memcnt(sql),
							_T("SELECT DISTINCT R.*, D.id as reservation_detail_id, ")
							_T("C.first_name, C.last_name, C.client_kind, B.business_date,")
							_T("DC.id as detail_client_id, DC.first_name as detail_fname, DC.last_name as detail_lname, ")
							_T("EI.id AS EI_id, EI.equipment_code, D.arrival_date, D.departure_date,")
							_T("D.check_in_date, D.check_out_date, D.canceled AS D_canceled ")
							_T("FROM %s R ")											// reservation_infos
							_T("LEFT JOIN %s D ON D.reservation_info_id = R.id ")		// reservation_details
							_T("LEFT JOIN %s C ON C.id = R.client_info_id ")			// client_infos
							_T("LEFT JOIN %s DC ON DC.id = D.client_info_id ")			// client_infos
							_T("LEFT JOIN %s B ON B.operator_code = R.operator_code AND B.branch_code = R.branch_code ")	// business_dates
							_T("LEFT JOIN %s RE ON RE.reservation_detail_id = D.id ")	// reservation_equipments
							_T("LEFT JOIN %s EI ON EI.id = RE.equipment_info_id ")		// equipment_infos
							_T("WHERE R.operator_code='%s' AND R.branch_code='%s' AND R.facility_code='%s' ")
							_T("AND R.deleted_date IS NULL AND D.deleted_date IS NULL AND RE.deleted_date IS NULL %s "),
							TBN_RSV_INF, TBN_RSV_DTL, TBN_CLI_INF, TBN_CLI_INF, TBN_BIZ_DTE, TBN_RSV_EQP, TBN_EQP_INF,
							SQL_Edt_Wst(opb, memcnt(opb), mng->opc), SQL_Edt_Wst(brb, memcnt(brb), mng->brc),
							SQL_Edt_Wst(fcb, memcnt(fcb), mng->fcc), rib );
						pos = mak_opt( sql, memcnt(sql), pos, opt ); 
						pos += swprintf_s( sql+pos, memcnt(sql)-pos,
							_T("GROUP BY D.id ORDER BY R.reservation_number, D.id" ) );
						break;
		case SHI_CIN:	pos = swprintf_s( sql,	memcnt(sql),
							_T("SELECT R.*, D.id as reservation_detail_id, RC.first_name, RC.last_name,") 
							_T("C.id as detail_client_id, C.first_name as detail_fname, C.last_name as detail_lname,") 
							_T("RC.client_kind, B.business_date, EI.id AS EI_id, EI.equipment_code,") 
							_T("D.arrival_date, D.departure_date,")
							_T("D.check_in_date, D.check_out_date, D.canceled AS D_canceled ")
							_T("FROM %s D ")											// reservation_details
							_T("LEFT JOIN %s R on R.id = D.reservation_info_id ")		// reservation_infos
							_T("LEFT JOIN %s C ON C.id = D.client_info_id ")			// client_infos
							_T("LEFT JOIN %s RC ON RC.id = R.client_info_id ")			// client_infos
							_T("LEFT JOIN %s B ON B.operator_code = R.operator_code ")	// business_dates
							_T("LEFT JOIN %s RE ON RE.reservation_detail_id = D.id ")	// reservation_equipments
							_T("LEFT JOIN %s EI ON EI.id = RE.equipment_info_id ")		// equipment_infos
							_T("WHERE R.operator_code='%s' AND R.branch_code='%s' ")
							_T("AND R.facility_code='%s' ") 
							_T("AND D.operator_code='%s' AND D.canceled=0 AND D.inhouse=0 ")
							_T("AND B.branch_code = R.branch_code AND D.arrival_date = B.business_date ")
							_T("AND B.deleted_date IS NULL AND R.deleted_date IS NULL AND B.deleted_date IS NULL %s ")
							_T("AND RE.use_start_date=D.arrival_date AND R.no_show=0 "),
							TBN_RSV_DTL, TBN_RSV_INF, TBN_CLI_INF, TBN_CLI_INF, TBN_BIZ_DTE, TBN_RSV_EQP, TBN_EQP_INF, 
							SQL_Edt_Wst(opb, memcnt(opb), mng->opc), SQL_Edt_Wst(brb, memcnt(brb), mng->brc),
							SQL_Edt_Wst(fcb, memcnt(fcb), mng->fcc), opb, rib );
						pos = mak_opt( sql, memcnt(sql), pos, opt ); 
						pos += swprintf_s( sql+pos, memcnt(sql)-pos,
							_T("ORDER BY R.reservation_number, D.id " ) );
							break;
		case SHI_INH:	pos = swprintf_s( sql,	memcnt(sql),
							_T("SELECT R.*, D.id as reservation_detail_id, RC.first_name, RC.last_name, NULL as business_date,") 
							_T("C.id as detail_client_id, C.first_name as detail_fname, C.last_name as detail_lname,")
							_T("C.client_kind, EI.equipment_code, D.arrival_date, D.departure_date,")
							_T("D.check_in_date, D.check_out_date, D.canceled AS D_canceled, EI.id AS EI_id ")
							_T("FROM %s D ")											// reservation_details
							_T("LEFT JOIN %s R on R.id = D.reservation_info_id ")		// reservation_infos
							_T("LEFT JOIN %s C ON C.id = D.client_info_id ")			// client_infos
							_T("LEFT JOIN %s RC ON RC.id = R.client_info_id ")			// client_infos
							_T("LEFT JOIN %s RE ON RE.reservation_detail_id = D.id ")	// reservation_equipments
							_T("LEFT JOIN %s EI ON EI.id = RE.equipment_info_id ")		// equipment_infos
							_T("WHERE R.operator_code='%s' AND R.branch_code='%s' AND R.facility_code='%s' ")
							_T("AND D.operator_code='%s' AND D.canceled=0 AND RE.inhouse<>0 ")
							_T("AND D.check_out_date IS NULL AND R.deleted_date IS NULL AND D.deleted_date IS NULL ")
							_T("AND RE.deleted_date IS NULL %s "),
							TBN_RSV_DTL, TBN_RSV_INF, TBN_CLI_INF, TBN_CLI_INF, TBN_RSV_EQP, TBN_EQP_INF, 
							SQL_Edt_Wst(opb, memcnt(opb), mng->opc), SQL_Edt_Wst(brb, memcnt(brb), mng->brc),
							SQL_Edt_Wst(fcb, memcnt(fcb), mng->fcc), opb, rib );
						pos = mak_opt( sql, memcnt(sql), pos, opt ); 
						pos += swprintf_s( sql+pos, memcnt(sql)-pos,
							_T("ORDER BY R.reservation_number, D.id ") );

						break;
		case SHI_COT:	pos = swprintf_s( sql,	memcnt(sql),
							_T("SELECT R.*, D.id as reservation_detail_id, RC.first_name, RC.last_name, ")
							_T("B.business_date,C.first_name as detail_fname,") 
							_T("C.id as detail_client_id, C.last_name as detail_lname,C.client_kind, EI.equipment_code, ")
							_T("D.arrival_date, D.departure_date,")
							_T("D.check_in_date, D.check_out_date, D.canceled AS D_canceled, EI.id AS EI_id ")
							_T("FROM %s D ")											// reservation_details
							_T("LEFT JOIN %s R on R.id = D.reservation_info_id ")		// reservation_infos 
							_T("LEFT JOIN %s C ON C.id = D.client_info_id ")			// client_infos
							_T("LEFT JOIN %s RC ON RC.id = R.client_info_id ")			// client_infos
							_T("LEFT JOIN %s RE ON RE.reservation_detail_id = D.id ")	// reservation_equipments
							_T("LEFT JOIN %s EI ON EI.id = RE.equipment_info_id ")		// equipment_infos
							_T("LEFT JOIN %s B ON B.operator_code=R.operator_code AND B.branch_code=R.branch_code ")	// business_dates
							_T("WHERE R.operator_code='%s' AND R.branch_code='%s' AND R.facility_code='%s' ")
							_T("AND D.operator_code='%s' ")
							_T("AND D.canceled=0 AND D.inhouse<>0 AND D.check_out_date IS NULL ")
							_T("AND D.departure_date <= B.business_date ")
							_T("AND R.deleted_date IS NULL AND D.deleted_date IS NULL AND RE.deleted_date IS NULL %s "),
							TBN_RSV_DTL, TBN_RSV_INF, TBN_CLI_INF, TBN_CLI_INF, TBN_RSV_EQP, TBN_EQP_INF, TBN_BIZ_DTE,
							SQL_Edt_Wst(opb, memcnt(opb), mng->opc), SQL_Edt_Wst(brb, memcnt(brb), mng->brc),
							SQL_Edt_Wst(fcb, memcnt(fcb), mng->fcc), opb, rib );
						pos = mak_opt( sql, memcnt(sql), pos, opt ); 
						pos += swprintf_s( sql+pos, memcnt(sql)-pos,
							_T("ORDER BY R.reservation_number, D.id ") );
						break;
		}
		chk = SQL_Get_Lst( rot, sql, fld, fct, rsv, sizeof(rsv), erm, dbc );
		free( fld );
	}

	return( chk );
}

