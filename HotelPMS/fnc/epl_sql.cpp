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
#include "epl_sql.h"


int		Epl_Get_Dat(				// Gtting employees
Csr				rot,				// root of storing buffer
EMP_LYE			*key,				// key data
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	EMP_LYE		epl[1];
	WCHAR		sql[512], opb[96], brb[96], fcb[96], ky1[128], ky2[128];
	Dbs_fld		*fld;
	int			flc, chk;

	swprintf_s( ky1, memcnt(ky1), 
		Nul_Fld(key->brc, FLD_WST) ? _T(" IS NULL"): _T("='%s'"), SQL_Edt_Wst(brb,memcnt(brb),DBS_WST_PTR(key->brc)) );
	swprintf_s( ky2, memcnt(ky2), 
		Nul_Fld(key->fcc, FLD_WST) ? _T(" IS NULL"): _T("='%s'"), SQL_Edt_Wst(fcb,memcnt(fcb),DBS_WST_PTR(key->fcc)) );

	if ( chk = !!( flc = Fdf_Emp_Lye( &fld, epl, NULL, NULL, 0 ) ) ){
		swprintf_s( sql, memcnt(sql),
					_T("SELECT * FROM %s ")			// equipment_types
					_T("WHERE operator_code='%s' ")
					_T("AND branch_code%s ")
					_T("AND facility_code%s ")
					_T("AND deleted_date IS NULL"),
					TBN_EMP_LYE, 
					SQL_Edt_Wst(opb,memcnt(opb),DBS_WST_PTR(key->opc)), ky1, ky2 );

		chk = SQL_Get_Lst( rot, sql, fld, flc, epl, sizeof(epl), erm, dbc );
		free( fld );
	}
	return( chk );
}

int		Epl_Sav_Dat(				// save employees
Csr				rot,				// root of destination
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk, flc, epi, siz=16384;
	Dbs_fld		*fld;
	Csr			csr=NULL;
	EMP_LYE		*epl;
	Day			dte[1];
	WCHAR		*sql;

	Get_Dte( dte );
	if ( chk = !!( sql = (WCHAR *)malloc( wsizeof(siz) ) ) ){
		if ( chk = Dbs_Trn( erm, dbs, TRN_STT ) ){
			while( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
				epl = Rec_Of(EMP_LYE, csr);
				if ( chk  = !!( flc = Fdf_Emp_Lye( &fld, epl, NULL, NULL, 0 ) ) ){
					DBS_INT_CPY(epl->mei, mng->lgi->id);
					if ( Nul_Fld(epl->id, FLD_INT) ){
						DBS_DTM_SET(epl->cdt, dte);
						chk = ( epi = SQL_Exc_Ins( sql, siz, TBN_EMP_LYE, erm, fld, flc, dbs ) ) > 0;
					}
					else if ( !Nul_Fld(epl->mdt, FLD_DTM) ){
						DBS_DTM_SET(epl->mdt, dte);
						chk = ( epi = SQL_Exc_Upd( sql, siz, TBN_EMP_LYE, erm, fld, flc, dbs ) ) > 0;
					}
					else if ( !Nul_Fld(epl->ddt, FLD_DTM) ){
						DBS_DTM_SET(epl->ddt, dte);
						chk = ( epi = SQL_Exc_Upd( sql, siz, TBN_EMP_LYE, erm, fld, flc, dbs ) ) > 0;
					}
					free( fld );
				}
			}
			Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
		}
		free( sql );
	}

	return( chk );
}

int		Epl_Get_Dty(				// getting duties
Csr				rot,				// root of destination
EMP_LYE			*key,				// key data
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	DTY_INF		dty[1];
	WCHAR		sql[256], opb[96], brb[96], fcb[96], ky1[128], ky2[128];
	Dbs_fld		*fld;
	int			flc, chk;

	swprintf_s( ky1, memcnt(ky1), 
		Nul_Fld(key->brc, FLD_WST) ? _T(""): _T(" OR branch_code='%s'"), SQL_Edt_Wst(brb,memcnt(brb),DBS_WST_PTR(key->brc)) );
	swprintf_s( ky2, memcnt(ky2), 
		Nul_Fld(key->fcc, FLD_WST) ? _T(""): _T(" OR facility_code='%s'"), SQL_Edt_Wst(fcb,memcnt(fcb),DBS_WST_PTR(key->fcc)) );

	if ( chk = !!( flc = Fdf_Dty_Inf( &fld, dty, NULL, NULL, 0 ) ) ){
		swprintf_s( sql, memcnt(sql),
						_T("SELECT * FROM %s ")			// facility_infos
						_T("WHERE operator_code='%s' ")
						_T("AND ( branch_code IS NULL %s ) ")
						_T("AND ( facility_code IS NULL %s ) ")
						_T("AND deleted_date IS NULL ")
						_T("ORDER BY branch_code DESC, facility_code DESC, display_order DESC "),
						TBN_DTY_INF,
						SQL_Edt_Wst( opb, memcnt(opb), DBS_WST_PTR(key->opc) ), ky1, ky2 );
		chk = SQL_Get_Lst( rot, sql, fld, flc, dty, sizeof(dty), erm, dbs );
		free( fld );
	}

	return( chk );
}

int		Epl_Cfm_Dup(				// Confirmation dupulicae employee code
int				epi,				// employee_id
WCHAR			*epc,				// employee_code
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk, cnt[DBF_SIZ_INT];
	WCHAR		sql[380], opb[96], cdb[96];
	Dbs_fld		fld[1] = {	cnt, _T("count"), FLD_WSI, FLD_INT_SIZ(cnt) };
	CRecordset	*rec;

	swprintf_s( sql, memcnt(sql),
		_T("SELECT COUNT(*) AS count FROM %s ")					// employee
		_T("WHERE id!=%d AND employee_code='%s' ")
		_T("AND deleted_date IS NULL ")
		_T("AND operator_code='%s' "),
		TBN_EMP_LYE, epi, SQL_Edt_Wst( cdb, memcnt(cdb), epc ),
		SQL_Edt_Wst( opb, memcnt(opb), mng->opc ) );

	if ( chk = !!( rec = Dbs_Qry( dbs, sql, erm ) ) ){
		switch( Dbs_Get( fld, erm, memcnt(fld), rec ) ){
		case QRY_NML:							break;
		case QRY_END:	*DBS_INT_PTR(cnt)=0;	break;
		default:		chk = FALSE;			break;
		}
		delete	rec;
	}

	return( chk ? DBS_INT_VAL(cnt): -1 );
}

int		Dty_Get_Dty(				// Gtting duties
Csr				rot,				// root of storing buffer
DTY_INF			*key,				// key data
int				sec,				// except service kind
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	DTY_INF		dty[1];
	WCHAR		sql[512], opb[96], brb[96], fcb[96], kyb[128];
	Dbs_fld		*fld;
	int			flc, chk;

	memset( dty, 0, sizeof(dty) );

	*kyb = 0;

	if ( chk = !!( flc = Fdf_Dty_Inf( &fld, dty, NULL, NULL, 0 ) ) ){
		swprintf_s( brb, memcnt(brb), !Nul_Fld(key->brc,FLD_WST) ? _T("='%s' "): _T(" IS NULL "), DBS_WST_PTR(key->brc) );
		swprintf_s( fcb, memcnt(fcb), !Nul_Fld(key->fcc,FLD_WST) ? _T("='%s' "): _T(" IS NULL "), DBS_WST_PTR(key->fcc) );
		swprintf_s( sql, memcnt(sql),
					_T("SELECT * ")
					_T("FROM %s ")																			// duties
					_T("WHERE operator_code='%s' ")
					_T("AND branch_code%s ")
					_T("AND facility_code%s %s ")
					_T("AND deleted_date IS NULL ")
					_T("ORDER BY display_order "),
					TBN_DTY_INF, SQL_Edt_Wst(opb,memcnt(opb),DBS_WST_PTR(key->opc)), brb, fcb, kyb );

		chk = SQL_Get_Lst( rot, sql, fld, flc, dty, sizeof(dty), erm, dbc );
		free( fld );
	}
	return( chk );
}

int		Dty_Sav_Dty(				// save duties
Csr				rot,				// root of destination
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk, flc, dti;
	Dbs_fld		*fld;
	Csr			csr=NULL;
	DTY_INF		*dty;
	Day			dte[1];
	WCHAR		sql[1920];

	Get_Dte( dte );
	chk = Dbs_Trn( erm, dbs, TRN_STT );
	while( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
		dty = Rec_Of(DTY_INF, csr);
		if ( chk  = !!( flc = Fdf_Dty_Inf( &fld, dty, NULL, NULL, 0 ) ) ){
			DBS_INT_CPY(dty->mei, mng->lgi->id);
			if ( Nul_Fld(dty->id, FLD_INT) ){
				DBS_DTM_SET(dty->cdt, dte);
				chk = ( dti = SQL_Exc_Ins( sql, memcnt(sql), TBN_DTY_INF, erm, fld, flc, dbs ) ) > 0;
			}
			else {		// All update because display_order may be changed
				DBS_DTM_SET(dty->mdt, dte);
				chk = ( dti = SQL_Exc_Upd( sql, memcnt(sql), TBN_DTY_INF, erm, fld, flc, dbs ) ) > 0;
			}
/*
			else if ( !Nul_Fld(cat->mdt, FLD_DTM) ){
				DBS_DTM_SET(cat->mdt, dte);
				chk = ( cti = SQL_Exc_Upd( sql, memcnt(sql), TBN_DSC_CAT, erm, fld, flc, dbs ) ) > 0;
			}
			else if ( !Nul_Fld(cat->ddt, FLD_DTM) ){
				DBS_DTM_SET(cat->ddt, dte);
				chk = ( cti = SQL_Exc_Upd( sql, memcnt(sql), TBN_DSC_CAT, erm, fld, flc, dbs ) ) > 0;
			}
*/
			free( fld );
		}
	}
	Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );

	return( chk );
}

