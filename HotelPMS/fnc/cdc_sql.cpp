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

int		Cdc_Get_Dat(				// Gtting creditcard_companies
Csr				dst,				// root of destination
WCHAR			*opc,				// operator_code
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	WCHAR		sql[512], opb[96];
	int			chk, pos, flc;
	CDC_COM		cdc[1];
	Dbs_fld		*fld;

	if ( chk = !!(flc = Fdf_Cdc_Inf( &fld, cdc, NULL, NULL, 0 ) ) ){
		pos = swprintf_s( sql,	memcnt(sql),
						_T("SELECT * ")
						_T("FROM %s ")
						_T("WHERE operator_code='%s' ")
						_T("AND deleted_date IS NULL ")
						_T("ORDER BY id "), TBN_CDC_INF, SQL_Edt_Wst(opb, memcnt(opb), opc) );

		memset( cdc, 0, sizeof(cdc) );
		chk = SQL_Get_Lst( dst, sql, fld, flc, cdc, sizeof(cdc), erm, dbc );
		free( fld );
	}

	return( chk );
}

int		Cdc_Sav_Dat(			// Saving ceditcard_companies
Csr				rot,			// Saving data
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk, flc, cci;
	Dbs_fld		*fld;
	Csr			csr=NULL;
	CDC_COM		*cdc;
	Day			dte[1];
	WCHAR		sql[1920];

	Get_Dte( dte );
	chk = Dbs_Trn( erm, dbs, TRN_STT );
	while( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
		cdc = Rec_Of(CDC_COM, csr);
		if ( chk  = !!( flc = Fdf_Cdc_Inf( &fld, cdc, NULL, NULL, 0 ) ) ){
			DBS_INT_CPY(cdc->mei, mng->lgi->id);
			if ( Nul_Fld(cdc->id, FLD_INT) ){
				DBS_DTM_SET(cdc->cdt, dte);
				chk = ( cci = SQL_Exc_Ins( sql, memcnt(sql), TBN_CDC_INF, erm, fld, flc, dbs ) ) > 0;
			}
			else if ( !Nul_Fld(cdc->mdt, FLD_DTM) ){
				DBS_DTM_SET(cdc->mdt, dte);
				chk = ( cci = SQL_Exc_Upd( sql, memcnt(sql), TBN_CDC_INF, erm, fld, flc, dbs ) ) > 0;
			}
			else if ( !Nul_Fld(cdc->ddt, FLD_DTM) ){
				DBS_DTM_SET(cdc->ddt, dte);
				chk = ( cci = SQL_Exc_Upd( sql, memcnt(sql), TBN_CDC_INF, erm, fld, flc, dbs ) ) > 0;
			}
			free( fld );
		}
	}
	Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );

	return( chk );
}
