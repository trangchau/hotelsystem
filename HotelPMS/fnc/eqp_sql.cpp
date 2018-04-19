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
#include "fld_def.h"
#include "eqp_sql.h"


int		Eqp_Get_Typ(				// Gtting equipment_type
Csr				rot,				// root of storing buffer
EQP_TYP			*key,				// key data
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	EQP_TYP		eqt[1];
	WCHAR		sql[512], opb[96], brb[96], fcb[96];
	Dbs_fld		*fld;
	int			flc, chk;

	if ( chk = !!( flc = Fdf_Eqp_Typ( &fld, eqt, NULL, NULL, 0 ) ) ){
		swprintf_s( sql, memcnt(sql),
					_T("SELECT * FROM %s ")			// equipment_types
					_T("WHERE operator_code='%s' ")
					_T("AND branch_code='%s' ")
					_T("AND facility_code='%s' ")
					_T("AND equipment_kind=%d ")
					_T("AND deleted_date IS NULL"),
					TBN_EQP_TYP,
					SQL_Edt_Wst(opb,memcnt(opb),DBS_WST_PTR(key->opc)),
					SQL_Edt_Wst(brb,memcnt(brb),DBS_WST_PTR(key->brc)),
					SQL_Edt_Wst(fcb,memcnt(fcb),DBS_WST_PTR(key->fcc)),
					EQK_ROOM );

		chk = SQL_Get_Lst( rot, sql, fld, flc, eqt, sizeof(eqt), erm, dbc );
		free( fld );
	}
	return( chk );
}

int		Eqp_Sav_Typ(				// save equipment_typss
Csr				rot,				// root of destination
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk, flc, eti;
	Dbs_fld		*fld;
	Csr			csr=NULL;
	EQP_TYP		*eqt;
	Day			dte[1];
	WCHAR		sql[1920];

	Get_Dte( dte );
	chk = Dbs_Trn( erm, dbs, TRN_STT );
	while( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
		eqt = Rec_Of(EQP_TYP, csr);
		if ( chk  = !!( flc = Fdf_Eqp_Typ( &fld, eqt, NULL, NULL, 0 ) ) ){
			DBS_INT_CPY(eqt->mei, mng->lgi->id);
			if ( Nul_Fld(eqt->id, FLD_INT) ){
				DBS_DTM_SET(eqt->cdt, dte);
				chk = ( eti = SQL_Exc_Ins( sql, memcnt(sql), TBN_EQP_TYP, erm, fld, flc, dbs ) ) > 0;
			}
			else if ( !Nul_Fld(eqt->mdt, FLD_DTM) ){
				DBS_DTM_SET(eqt->mdt, dte);
				chk = ( eti = SQL_Exc_Upd( sql, memcnt(sql), TBN_EQP_TYP, erm, fld, flc, dbs ) ) > 0;
			}
			else if ( !Nul_Fld(eqt->ddt, FLD_DTM) ){
				DBS_DTM_SET(eqt->ddt, dte);
				chk = ( eti = SQL_Exc_Upd( sql, memcnt(sql), TBN_EQP_TYP, erm, fld, flc, dbs ) ) > 0;
			}
			free( fld );
		}
	}
	Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );

	return( chk );
}


int		Eqp_Get_Inf(				// Gtting equipment_info
Csr				rot,				// root of storing buffer
EQP_INF			*key,				// key data
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	EQP_INF		eqi[1];
	WCHAR		sql[512], opb[96], brb[96], fcb[96];
	Dbs_fld		*fld;
	int			flc, chk;

	memset( eqi, 0, sizeof(eqi) );

	if ( chk = !!( flc = Fdf_Eqp_Inf( &fld, eqi, NULL, NULL, 0 ) ) ){
		swprintf_s( sql, memcnt(sql),
					_T("SELECT * FROM %s ")
					_T("WHERE operator_code='%s' ")
					_T("AND branch_code='%s' ")
					_T("AND facility_code='%s' ")
					_T("AND equipment_kind=%d ")
					_T("AND equipment_type_id=%d ")
					_T("AND deleted_date IS NULL"),
					TBN_EQP_INF,
					SQL_Edt_Wst(opb,memcnt(opb),DBS_WST_PTR(key->opc)),
					SQL_Edt_Wst(brb,memcnt(brb),DBS_WST_PTR(key->brc)),
					SQL_Edt_Wst(fcb,memcnt(fcb),DBS_WST_PTR(key->fcc)),
					DBS_INT_VAL(key->ekd), DBS_INT_VAL(key->eti) );

		chk = SQL_Get_Lst( rot, sql, fld, flc, eqi, sizeof(eqi), erm, dbc );
		free( fld );
	}
	return( chk );
}

int		Eqp_Sav_Inf(				// save equipment_infos
Csr				rot,				// root of destination
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk, flc, eti;
	Dbs_fld		*fld;
	Csr			csr=NULL;
	EQP_INF		*eqi;
	Day			dte[1];
	WCHAR		sql[1920];

	Get_Dte( dte );
	chk = Dbs_Trn( erm, dbs, TRN_STT );
	while( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
		eqi = Rec_Of(EQP_INF, csr);
		if ( chk  = !!( flc = Fdf_Eqp_Inf( &fld, eqi, NULL, NULL, 0 ) ) ){
			DBS_INT_CPY(eqi->mei, mng->lgi->id);
			if ( Nul_Fld(eqi->id, FLD_INT) ){
				DBS_DTM_SET(eqi->cdt, dte);
				chk = ( eti = SQL_Exc_Ins( sql, memcnt(sql), TBN_EQP_INF, erm, fld, flc, dbs ) ) > 0;
			}
			else if ( !Nul_Fld(eqi->mdt, FLD_DTM) ){
				DBS_DTM_SET(eqi->mdt, dte);
				chk = ( eti = SQL_Exc_Upd( sql, memcnt(sql), TBN_EQP_INF, erm, fld, flc, dbs ) ) > 0;
			}
			else if ( !Nul_Fld(eqi->ddt, FLD_DTM) ){
				DBS_DTM_SET(eqi->ddt, dte);
				chk = ( eti = SQL_Exc_Upd( sql, memcnt(sql), TBN_EQP_INF, erm, fld, flc, dbs ) ) > 0;
			}
			free( fld );
		}
	}
	Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );

	return( chk );
}
