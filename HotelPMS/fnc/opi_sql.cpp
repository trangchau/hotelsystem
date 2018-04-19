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

int		Opi_Get_Dat(				// Gtting operator_info
OPR_INF			*dst,				// pointer of destination
WCHAR			*opc,				// operator_code
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	WCHAR		sql[512], opb[96];
	int			chk, pos, flc;
	Dbs_fld		*fld;
	CRecordset	*rec;

	//int			Dbs_MGt( Mlt_dbf *, int *, WCHAR **, int, CRecordset * );

	if ( chk = !!(flc = Fdf_Opr_Inf( &fld, dst, NULL, NULL, 0 ) ) ){
		pos = swprintf_s( sql,	memcnt(sql),
						_T("SELECT * ")
						_T("FROM %s ")
						_T("WHERE operator_code='%s' ")
						_T("AND deleted_date IS NULL "), TBN_OPR_INF, SQL_Edt_Wst(opb, memcnt(opb), opc) );

		memset( dst, 0, sizeof(OPR_INF) );
		rec = Dbs_Qry( dbc, sql, erm );

		if ( chk = !!rec ){
			if ( *erm )	free( *erm );
			*erm = NULL;
			chk = Dbs_Get( fld, erm, flc, rec )==QRY_NML;
			Dbs_Cls(rec);
		}
		free( fld );
	}

	return( chk );
}

int		Opi_Sav_Dat(			// Saving operator_info
OPR_INF			*opr,			// Saving data
Day				*dte,			// Saving datetime
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk, fct, opi;
	Dbs_fld		*fld;
	WCHAR		sql[1024];

	if ( chk = Dbs_Trn( erm, dbs, TRN_STT ) ){
		if ( chk = !!( fct = Fdf_Opr_Inf( &fld, opr, NULL, NULL, 0 ) ) ){
			Set_Nul(opr->mei, FLD_INT,FALSE);				// modified_employee_id
			DBS_INT_CPY(opr->mei, mng->lgi->id);
			if ( Nul_Fld(opr->id,FLD_INT) ){
				Set_Nul(opr->cdt,FLD_DTM,FALSE);
				*DBS_DTM_PTR(opr->cdt) = *dte;
				chk = !!( opi = SQL_Exc_Ins( sql, memcnt(sql), TBN_OPR_INF, erm, fld, fct, dbs ) );
			}
			else{
				Set_Nul(opr->mdt,FLD_DTM,FALSE);		// set modified date when update
				*DBS_DTM_PTR(opr->mdt) = *dte;
				Set_Nul(opr->ddt,FLD_DTM,TRUE);			// set null into deleted date
				chk = SQL_Exc_Upd( sql, memcnt(sql), TBN_OPR_INF, erm, fld, fct, dbs );
				opi = DBS_INT_VAL(opr->id);
			}
			free( fld );
		}
		Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
	}
	return( chk ? opi: 0  );
}
