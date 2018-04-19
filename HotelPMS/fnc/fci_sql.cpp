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

int		Fci_Get_Dat(				// Gtting branch_info
Csr				dst,				// root of destination
WCHAR			*opc,				// operator_code
WCHAR			*brc,				// branch_code
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	WCHAR		sql[512], opb[96], brb[96];
	int			chk, pos, flc;
	FCL_INF		fcl[1];
	Dbs_fld		*fld;

	if ( chk = !!(flc = Fdf_Fcl_Inf( &fld, fcl, NULL, NULL, 0 ) ) ){
		pos = swprintf_s( sql,	memcnt(sql),
						_T("SELECT * ")
						_T("FROM %s ")
						_T("WHERE operator_code='%s' ")
						_T("AND branch_code='%s' ")
						_T("AND deleted_date IS NULL ")
						_T("ORDER BY id "),
						TBN_FCL_INF, SQL_Edt_Wst(opb, memcnt(opb), opc), SQL_Edt_Wst(brb, memcnt(brb), brc) );

		memset( fcl, 0, sizeof(fcl) );
		chk = SQL_Get_Lst( dst, sql, fld, flc, fcl, sizeof(fcl), erm, dbc );
		free( fld );
	}

	return( chk );
}

/*
int		Fci_Sav_Dat(			// Saving operator_info
Csr				rot,			// Saving data
int				cnt,			// Saving count
Day				*dte,			// Saving datetime
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk=TRUE, fct, fci, idx;
	Dbs_fld		*fld;
	WCHAR		sql[1024];
	Csr			csr=NULL;
	FCL_INF		*fcl;

	if ( chk = Dbs_Trn( erm, dbs, TRN_STT ) ){
		for ( idx=0; !!cnt && idx<cnt; idx++ ){
			while( !!chk && !!( csr=Nxt_Lnk( rot+idx, csr ) ) ){
				fcl = Rec_Of(FCL_INF, csr);
				if ( !Nul_Fld(fcl->nam,FLD_WST) ){
					if ( chk = !!( fct = Fdf_Fcl_Inf( &fld, fcl, NULL, NULL, 0 ) ) ){
						Set_Nul(fcl->mei, FLD_INT,FALSE);				// modified_employee_id
						*DBS_INT_PTR(fcl->mei) = mng->lgi->epi;
						if ( Nul_Fld(fcl->id,FLD_INT) ){
							Set_Nul(fcl->cdt,FLD_DTM,FALSE);
							*DBS_DTM_PTR(fcl->cdt) = *dte;
							chk = !!( fci = SQL_Exc_Ins( sql, memcnt(sql), TBN_FCL_INF, erm, fld, fct, dbs ) );
						}
						else{
							Set_Nul(fcl->mdt,FLD_DTM,FALSE);		// set modified date when update
							*DBS_DTM_PTR(fcl->mdt) = *dte;
							Set_Nul(fcl->ddt,FLD_DTM,TRUE);			// set null into deleted date
							chk = SQL_Exc_Upd( sql, memcnt(sql), TBN_FCL_INF, erm, fld, fct, dbs );
							fci = DBS_INT_VAL(fcl->id);
						}
						free( fld );
					}
				}
			}
		}
		Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
	}
	return( chk  );
}
*/

int		Fci_Sav_Dat(				// save facility_infos
Csr				rot,				// root of destination
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk, flc, fci;
	Dbs_fld		*fld;
	Csr			csr=NULL;
	FCL_INF		*fcl;
	Day			dte[1];
	WCHAR		sql[1920];

	Get_Dte( dte );
	chk = Dbs_Trn( erm, dbs, TRN_STT );
	while( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
		fcl = Rec_Of(FCL_INF, csr);
		if ( chk  = !!( flc = Fdf_Fcl_Inf( &fld, fcl, NULL, NULL, 0 ) ) ){
			DBS_INT_CPY(fcl->mei, mng->lgi->id);
			if ( Nul_Fld(fcl->id, FLD_INT) ){
				DBS_DTM_SET(fcl->cdt, dte);
				chk = ( fci = SQL_Exc_Ins( sql, memcnt(sql), TBN_FCL_INF, erm, fld, flc, dbs ) ) > 0;
			}
			else if ( !Nul_Fld(fcl->mdt, FLD_DTM) ){
				DBS_DTM_SET(fcl->mdt, dte);
				chk = ( fci = SQL_Exc_Upd( sql, memcnt(sql), TBN_FCL_INF, erm, fld, flc, dbs ) ) > 0;
			}
			else if ( !Nul_Fld(fcl->ddt, FLD_DTM) ){
				DBS_DTM_SET(fcl->ddt, dte);
				chk = ( fci = SQL_Exc_Upd( sql, memcnt(sql), TBN_FCL_INF, erm, fld, flc, dbs ) ) > 0;
			}
			free( fld );
		}
	}
	Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );

	return( chk );
}