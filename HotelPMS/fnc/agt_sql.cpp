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
//#include "rsv_fnc.h"
//#include "rsv_sql.h"
#include "fld_def.h"

int		Agt_Get_Dat(				// Gtting agent_infos
Csr				dst,				// root of destination
WCHAR			*opc,				// operator_code
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	WCHAR		sql[512], opb[96];
	int			chk, pos, flc;
	AGT_INF		agt[1];
	Dbs_fld		*fld;

	if ( chk = !!(flc = Fdf_Agt_Inf( &fld, agt, NULL, NULL, 0 ) ) ){
		pos = swprintf_s( sql,	memcnt(sql),
						_T("SELECT * ")
						_T("FROM %s ")
						_T("WHERE operator_code='%s' ")
						_T("AND deleted_date IS NULL ")
						_T("ORDER BY id "), TBN_AGT_INF, SQL_Edt_Wst(opb, memcnt(opb), opc) );

		memset( agt, 0, sizeof(agt) );
		chk = SQL_Get_Lst( dst, sql, fld, flc, agt, sizeof(agt), erm, dbc );
		free( fld );
	}

	return( chk );
}

int		Agt_Sav_Dat(			// Saving agent_infos
Csr				rot,			// Saving data
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk, flc, aii;
	Dbs_fld		*fld;
	Csr			csr=NULL;
	AGT_INF		*agt;
	Day			dte[1];
	WCHAR		sql[1920];

	Get_Dte( dte );
	chk = Dbs_Trn( erm, dbs, TRN_STT );
	while( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
		agt = Rec_Of(AGT_INF, csr);
		if ( chk  = !!( flc = Fdf_Agt_Inf( &fld, agt, NULL, NULL, 0 ) ) ){
			DBS_INT_CPY(agt->mei, mng->lgi->id);
			if ( Nul_Fld(agt->id, FLD_INT) ){
				DBS_DTM_SET(agt->cdt, dte);
				chk = ( aii = SQL_Exc_Ins( sql, memcnt(sql), TBN_AGT_INF, erm, fld, flc, dbs ) ) > 0;
			}
			else if ( !Nul_Fld(agt->mdt, FLD_DTM) ){
				DBS_DTM_SET(agt->mdt, dte);
				chk = ( aii = SQL_Exc_Upd( sql, memcnt(sql), TBN_AGT_INF, erm, fld, flc, dbs ) ) > 0;
			}
			else if ( !Nul_Fld(agt->ddt, FLD_DTM) ){
				DBS_DTM_SET(agt->ddt, dte);
				chk = ( aii = SQL_Exc_Upd( sql, memcnt(sql), TBN_AGT_INF, erm, fld, flc, dbs ) ) > 0;
			}
			free( fld );
		}
	}
	Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );

	return( chk );
}
