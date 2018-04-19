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
#include "cli_sql.h"
#include "fld_def.h"

int		Cli_Get_Inf(				// Getting client informations 
Csr				rot,				// root of storing destination
WCHAR			*fnm,				// first name
WCHAR			*lnm,				// last name
WCHAR			*tel,				// telephone number
WCHAR			*eml,				// email address
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	CLI_INF		cli[1];
	WCHAR		*sql, opb[96], dbf[520];
	Dbs_fld		*fld;
	int			chk, pos, flc, ssz=2048;

	chk  = !!( sql = (WCHAR *)malloc( wsizeof(ssz) ) );
	chk &= !!( flc = Fdf_Cli_Inf( &fld, cli, NULL, NULL, 0 ) );

	if ( chk ){
		pos = swprintf_s( sql,	ssz,
				_T("SELECT * ")
				_T("FROM %s ")													// client_infos
				_T("WHERE operator_code='%s' AND deleted_date IS NULL "),
				TBN_CLI_INF, SQL_Edt_Wst(opb, memcnt(opb), mng->opc) );

		if ( !!wcslen(fnm) ){
			pos += swprintf_s( sql+pos, ssz-pos, 
				_T("AND ( first_name LIKE '%s%%' OR last_name LIKE '%s%%' %s "),
				SQL_Edt_Wst(dbf, memcnt(dbf), fnm), dbf, !!wcslen(lnm) ? _T(""): _T(")") );
		}
		if ( !!wcslen(lnm) ){
			pos += swprintf_s( sql+pos, ssz-pos, 
				_T("%s first_name LIKE '%s%%' OR last_name LIKE '%s%%' ) "),
				!!wcslen(fnm) ? _T("OR "): _T("AND ("),
				SQL_Edt_Wst(dbf, memcnt(dbf), lnm), dbf );
		}
		if ( !!wcslen(tel) ){
			pos += swprintf_s( sql+pos, ssz-pos, 
				_T("AND ( telephone_number1 LIKE '%s%%' OR telephone_number2 LIKE '%s%%' ) "),
				SQL_Edt_Wst(dbf, memcnt(dbf), tel) );
		}
		if ( !!wcslen(eml) ){
			pos += swprintf_s( sql+pos, ssz-pos, 
				_T("AND ( email_address1 LIKE '%s%%' OR email_address2 LIKE '%s%%' ) "),
				SQL_Edt_Wst(dbf, memcnt(dbf), eml) );
		}
		memset( cli, 0, sizeof(cli) );
		chk = SQL_Get_Lst( rot, sql, fld, flc, cli, sizeof(cli), erm, dbc );
	}
	if ( sql )		free( sql );
	if ( fld )		free( fld );

	return( chk );
}

int		Msg_Get_Msg(				// Gtting market_segments
Csr				rot,				// root of storing buffer
MKT_SEG			*key,				// key data
int				sec,				// except service kind
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	MKT_SEG		msg[1];
	WCHAR		sql[512], opb[96], brb[96], fcb[96], kyb[128];
	Dbs_fld		*fld;
	int			flc, chk;

	memset( msg, 0, sizeof(msg) );

	*kyb = 0;

	if ( chk = !!( flc = Fdf_Mkt_Seg( &fld, msg, NULL, NULL, 0 ) ) ){
		swprintf_s( brb, memcnt(brb), !Nul_Fld(key->brc,FLD_WST) ? _T("='%s' "): _T(" IS NULL "), DBS_WST_PTR(key->brc) );
		swprintf_s( fcb, memcnt(fcb), !Nul_Fld(key->fcc,FLD_WST) ? _T("='%s' "): _T(" IS NULL "), DBS_WST_PTR(key->fcc) );
		swprintf_s( sql, memcnt(sql),
					_T("SELECT * ")
					_T("FROM %s ")									// duties
					_T("WHERE operator_code='%s' ")
					_T("AND branch_code%s ")
					_T("AND facility_code%s %s ")
					_T("AND deleted_date IS NULL "),
					//_T("ORDER BY display_order "),
					TBN_MKT_SEG, SQL_Edt_Wst(opb,memcnt(opb),DBS_WST_PTR(key->opc)), brb, fcb, kyb );

		chk = SQL_Get_Lst( rot, sql, fld, flc, msg, sizeof(msg), erm, dbc );
		free( fld );
	}
	return( chk );
}

int		Msg_Sav_Msg(				// save marketsegments
Csr				rot,				// root of destination
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk, flc, msi;
	Dbs_fld		*fld;
	Csr			csr=NULL;
	MKT_SEG		*msg;
	Day			dte[1];
	WCHAR		sql[1920];

	Get_Dte( dte );
	chk = Dbs_Trn( erm, dbs, TRN_STT );
	while( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
		msg = Rec_Of(MKT_SEG, csr);
		if ( chk  = !!( flc = Fdf_Mkt_Seg( &fld, msg, NULL, NULL, 0 ) ) ){
			DBS_INT_CPY(msg->mei, mng->lgi->id);
			if ( Nul_Fld(msg->id, FLD_INT) ){
				DBS_DTM_SET(msg->cdt, dte);
				chk = ( msi = SQL_Exc_Ins( sql, memcnt(sql), TBN_MKT_SEG, erm, fld, flc, dbs ) ) > 0;
			}
			else {		// All update because display_order may be changed
				DBS_DTM_SET(msg->mdt, dte);
				chk = ( msi = SQL_Exc_Upd( sql, memcnt(sql), TBN_MKT_SEG, erm, fld, flc, dbs ) ) > 0;
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

