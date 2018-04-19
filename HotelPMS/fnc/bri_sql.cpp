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
#include "rsv_fnc.h"
#include "rsv_sql.h"
#include "fld_def.h"

int		Bri_Get_Dat(				// Gtting branch_info
Csr				dst,				// root of destination
WCHAR			*opc,				// operator_code
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	WCHAR		sql[512], opb[96];
	int			chk, pos, flc;
	BRC_INF		brc[1];
	Dbs_fld		*fld;

	if ( chk = !!(flc = Fdf_Brc_Inf( &fld, brc, NULL, NULL, 0 ) ) ){
		pos = swprintf_s( sql,	memcnt(sql),
						_T("SELECT * ")
						_T("FROM %s ")
						_T("WHERE operator_code='%s' ")
						_T("AND deleted_date IS NULL ")
						_T("ORDER BY id "), TBN_BRC_INF, SQL_Edt_Wst(opb, memcnt(opb), opc) );

		memset( brc, 0, sizeof(brc) );
		chk = SQL_Get_Lst( dst, sql, fld, flc, brc, sizeof(brc), erm, dbc );
		free( fld );
	}

	return( chk );
}

int		Bri_Sav_Dat(			// Saving operator_info
Csr				rot,			// Saving data
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk, flc, bri;
	Dbs_fld		*fld;
	Csr			csr=NULL;
	BRC_INF		*brc;
	Day			dte[1];
	WCHAR		sql[1920];

	Get_Dte( dte );
	chk = Dbs_Trn( erm, dbs, TRN_STT );
	while( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
		brc = Rec_Of(BRC_INF, csr);
		if ( chk  = !!( flc = Fdf_Brc_Inf( &fld, brc, NULL, NULL, 0 ) ) ){
			DBS_INT_CPY(brc->mei, mng->lgi->id);
			if ( Nul_Fld(brc->id, FLD_INT) ){
				DBS_DTM_SET(brc->cdt, dte);
				chk = ( bri = SQL_Exc_Ins( sql, memcnt(sql), TBN_BRC_INF, erm, fld, flc, dbs ) ) > 0;
			}
			else if ( !Nul_Fld(brc->mdt, FLD_DTM) ){
				DBS_DTM_SET(brc->mdt, dte);
				chk = ( bri = SQL_Exc_Upd( sql, memcnt(sql), TBN_BRC_INF, erm, fld, flc, dbs ) ) > 0;
			}
			else if ( !Nul_Fld(brc->ddt, FLD_DTM) ){
				DBS_DTM_SET(brc->ddt, dte);
				chk = ( bri = SQL_Exc_Upd( sql, memcnt(sql), TBN_BRC_INF, erm, fld, flc, dbs ) ) > 0;
			}
			free( fld );
		}
	}
	Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );

	return( chk );
}
