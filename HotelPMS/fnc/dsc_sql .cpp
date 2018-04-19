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


int		Dsc_Get_Cat(				// Gtting description_categories
Csr				rot,				// root of storing buffer
DSC_CAT			*key,				// key data
int				sec,				// except service kind
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	DSC_CAT		cat[1];
	WCHAR		sql[512], opb[96], brb[96], fcb[96], kyb[128];
	Dbs_fld		*fld;
	int			flc, chk;

	memset( cat, 0, sizeof(cat) );

	*kyb = 0;
	if ( sec ){
		swprintf_s( kyb, memcnt(kyb), _T("AND (DS.description_kind!=%d OR DS.description_kind IS NULL) "), DRK_SERVICE );
	}

	if ( chk = !!( flc = Fdf_Dsc_Cat( &fld, cat, NULL, NULL, 0 ) ) ){
		swprintf_s( brb, memcnt(brb), !Nul_Fld(key->brc,FLD_WST) ? _T("='%s' "): _T(" IS NULL "), DBS_WST_PTR(key->brc) );
		swprintf_s( fcb, memcnt(fcb), !Nul_Fld(key->fcc,FLD_WST) ? _T("='%s' "): _T(" IS NULL "), DBS_WST_PTR(key->fcc) );
		swprintf_s( sql, memcnt(sql),
					_T("SELECT DISTINCT * ")
					_T("FROM %s DC ")																			// description_categories
					_T("LEFT JOIN %s DS ON DS.description_category_id = DC.id AND DS.deleted_date IS NULL ")	// descriptions
					_T("WHERE DC.operator_code='%s' ")
					_T("AND DC.branch_code%s ")
					_T("AND DC.facility_code%s %s ")
					_T("AND DC.deleted_date IS NULL ")
					_T("GROUP BY DC.id ")
					_T("ORDER BY DC.display_order "),
					TBN_DSC_CAT, TBN_DSC_RPT, SQL_Edt_Wst(opb,memcnt(opb),DBS_WST_PTR(key->opc)), brb, fcb, kyb );

		chk = SQL_Get_Lst( rot, sql, fld, flc, cat, sizeof(cat), erm, dbc );
		free( fld );
	}
	return( chk );
}

int		Dsc_Sav_Cat(				// save description_categories
Csr				rot,				// root of destination
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk, flc, cti;
	Dbs_fld		*fld;
	Csr			csr=NULL;
	DSC_CAT		*cat;
	Day			dte[1];
	WCHAR		sql[1920];

	Get_Dte( dte );
	chk = Dbs_Trn( erm, dbs, TRN_STT );
	while( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
		cat = Rec_Of(DSC_CAT, csr);
		if ( chk  = !!( flc = Fdf_Dsc_Cat( &fld, cat, NULL, NULL, 0 ) ) ){
			DBS_INT_CPY(cat->mei, mng->lgi->id);
			if ( Nul_Fld(cat->id, FLD_INT) ){
				DBS_DTM_SET(cat->cdt, dte);
				chk = ( cti = SQL_Exc_Ins( sql, memcnt(sql), TBN_DSC_CAT, erm, fld, flc, dbs ) ) > 0;
			}
			else {		// All update because display_order may be changed
				DBS_DTM_SET(cat->mdt, dte);
				chk = ( cti = SQL_Exc_Upd( sql, memcnt(sql), TBN_DSC_CAT, erm, fld, flc, dbs ) ) > 0;
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

static int 	apl_cmp(		// compeare the tax_applcation for decending sort 
const void	*dpt,			// key data 1
const void	*spt )			// key data 2
{
	DSC_RPT		*dst = Rec_Of(DSC_RPT, *((Csr *)dpt ) );
	DSC_RPT		*src = Rec_Of(DSC_RPT, *((Csr *)spt ) );
	return( DBS_INT_VAL(dst->apl) < DBS_INT_VAL(src->apl) );
}

int		Dsc_Get_Tax(				// Gtting tax and other of descriptions
Csr				rot,				// root of storing buffer
DSC_RPT			*key,				// key data
int				ntv,				// Only native ( business_date is in alid_sale_period ) 
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	DSC_RPT		dsc[1], *ptr;
	WCHAR		sql[1024], opb[96], kyb[512];
	Dbs_fld		*fld;
	Csr			csr=NULL;
	int			flc, chk, atr=0;

	memset( dsc, 0, sizeof(dsc) );

	SQL_Dsc_Cnd( kyb, memcnt(kyb), key, ntv, mng );

	if ( chk = !!( flc = Fdf_Dsc_Rpt( &fld, dsc, NULL, NULL, 0 ) ) ){
		swprintf_s( sql, memcnt(sql),
					_T("SELECT * FROM %s ")				// descriptions
					_T("WHERE operator_code='%s' ")
					_T("%s ")
					_T("AND tax_attribution!=0 ")
// bucause these conditions within kyb( SQL_Dsc_Cnd function )
//					_T("AND sale_enable!=0 ")
//					_T("AND deleted_date IS NULL ")
					_T("ORDER BY tax_attribution ASC, branch_code DESC, facility_code DESC"),
					TBN_DSC_RPT, SQL_Edt_Wst(opb,memcnt(opb),DBS_WST_PTR(key->opc)), kyb );

		if ( ( chk = SQL_Get_Lst( rot, sql, fld, flc, dsc, sizeof(dsc), erm, dbc ) ) ){
			// Leave only one attribute that most nearly to own facility if exist multiple same attribute
			// For example, ( opc=1, brc=NULL, fcc=NULL ), (opc=1, brc=1, fcc=NULL)<-leave
			// ORDER BY is for this reason
			csr = Nxt_Lnk( rot, NULL );
			while( csr ){
				ptr = Rec_Of(DSC_RPT, csr);
				if ( DBS_INT_VAL(ptr->atr) != atr ){
					atr = DBS_INT_VAL(ptr->atr);
					csr = Nxt_Lnk( rot, csr );
				}
				else{
					csr = Del_Lnk( rot, csr );
				}
			}
			Sot_Lnk( rot, apl_cmp ); // decending sort because has some other applications to apper
		}

		free( fld );
	}
	return( chk );
}

static	WCHAR	*add_str( WCHAR	*dst, WCHAR *src )
{
	int		dsz=0, ssz;
	WCHAR	*npt;

	ssz = (int)wcslen( src );
	dsz = ssz + ( dst ? (int)wcslen(dst)+1: 1 );
	if ( npt = (WCHAR *)malloc( wsizeof(dsz) ) ){
		if ( !!dst )	wcscpy_s( npt, dsz, dst );
		else			*dst = 0;
		wcscat_s( npt, dsz, src );
		if ( dst )		free( dst );
	}

	return( npt );
}

int		Dsc_Get_Atr(				// Gtting existing attribute
Csr				dst,				// root pointer of destination
DSC_RPT			*key,				// key data
Csr				rot,				// excepting key
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			siz=1024, chk, pos=0, cnt=0, flc;
	DSC_RPT		dsc[1], *ptr;
	WCHAR		*sql, opb[96], brb[96], fcb[96], buf[40];
	Dbs_fld		*fld;
	Csr			csr=NULL;

	memset( dsc, 0, sizeof(dsc) );

	if ( chk = !!( sql = (WCHAR *)malloc( wsizeof(siz) ) ) ){ 
		swprintf_s( brb, memcnt(brb), !Nul_Fld(key->brc,FLD_WST) ? _T("='%s' "): _T(" IS NULL "), DBS_WST_PTR(key->brc) );
		swprintf_s( fcb, memcnt(fcb), !Nul_Fld(key->fcc,FLD_WST) ? _T("='%s' "): _T(" IS NULL "), DBS_WST_PTR(key->fcc) );
		if ( chk = !!( flc = Fdf_Dsc_Rpt( &fld, dsc, NULL, NULL, 0 ) ) ){
			pos = swprintf_s( sql, siz,
						_T("SELECT * ")
						_T("FROM %s ")		// descriptions
						_T("WHERE operator_code='%s' ")
						_T("AND branch_code%s ")
						_T("AND facility_code%s ")
						_T("AND deleted_date IS NULL "),
						TBN_DSC_RPT, SQL_Edt_Wst(opb,memcnt(opb),DBS_WST_PTR(key->opc)), brb, fcb );
			// excepting editting data
			while( !!chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
				ptr = Rec_Of(DSC_RPT, csr);
				if ( !Nul_Fld(ptr->id, FLD_INT) ){
					swprintf_s( buf, memcnt(buf), _T("%s%s%d"),
						!cnt ? _T("AND id NOT IN ("): _T(""), 
						!cnt ? _T(""): _T(","), DBS_INT_VAL(ptr->id) );
					chk = !!( sql = add_str( sql, buf ) );
					cnt++;
				}
			}
			if ( !!cnt )	sql = add_str( sql, _T(")") );
			if ( chk ){
				chk = SQL_Get_Lst( dst, sql, fld, flc, dsc, sizeof(dsc), erm, dbc );
			}
			free( fld );
		}
		free( sql );
	}

	return( chk );
}

int		Dsc_Get_Dsc(				// Gtting descriptions
Csr				rot,				// root of storing buffer
DSC_RPT			*key,				// key data
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	DSC_RPT		dsc[1];
	WCHAR		sql[512], opb[96], brb[96], fcb[96];
	Dbs_fld		*fld;
	int			flc, chk;

	memset( dsc, 0, sizeof(dsc) );

	if ( chk = !!( flc = Fdf_Dsc_Rpt( &fld, dsc, NULL, NULL, 0 ) ) ){
		swprintf_s( brb, memcnt(brb), !Nul_Fld(key->brc,FLD_WST) ? _T("='%s' "): _T(" IS NULL "), DBS_WST_PTR(key->brc) );
		swprintf_s( fcb, memcnt(fcb), !Nul_Fld(key->fcc,FLD_WST) ? _T("='%s' "): _T(" IS NULL "), DBS_WST_PTR(key->fcc) );
		swprintf_s( sql, memcnt(sql),
					_T("SELECT * FROM %s ")				// descriptions
					_T("WHERE operator_code='%s' ")
					_T("AND branch_code%s ")
					_T("AND facility_code%s ")
					_T("AND description_category_id=%d ")
					_T("AND deleted_date IS NULL"),
					TBN_DSC_RPT, SQL_Edt_Wst(opb,memcnt(opb),DBS_WST_PTR(key->opc)),
					brb, fcb, DBS_INT_VAL(key->dci) );

		chk = SQL_Get_Lst( rot, sql, fld, flc, dsc, sizeof(dsc), erm, dbc );
		free( fld );
	}
	return( chk );
}

int		Dsc_Sav_Dsc(				// save descriptions
Csr				rot,				// root of destination
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk, flc, dci;
	Dbs_fld		*fld;
	Csr			csr=NULL;
	DSC_RPT		*dsc;
	Day			dte[1];
	WCHAR		sql[1920];

	Get_Dte( dte );
	chk = Dbs_Trn( erm, dbs, TRN_STT );
	while( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
		dsc = Rec_Of(DSC_RPT, csr);
		if ( chk  = !!( flc = Fdf_Dsc_Rpt( &fld, dsc, NULL, NULL, 0 ) ) ){
			DBS_INT_CPY(dsc->mei, mng->lgi->id);
			if ( Nul_Fld(dsc->id, FLD_INT) ){
				DBS_DTM_SET(dsc->cdt, dte);
				chk = ( dci = SQL_Exc_Ins( sql, memcnt(sql), TBN_DSC_RPT, erm, fld, flc, dbs ) ) > 0;
			}
			else if ( !Nul_Fld(dsc->mdt, FLD_DTM) ){
				DBS_DTM_SET(dsc->mdt, dte);
				chk = ( dci = SQL_Exc_Upd( sql, memcnt(sql), TBN_DSC_RPT, erm, fld, flc, dbs ) ) > 0;
			}
			else if ( !Nul_Fld(dsc->ddt, FLD_DTM) ){
				DBS_DTM_SET(dsc->ddt, dte);
				chk = ( dci = SQL_Exc_Upd( sql, memcnt(sql), TBN_DSC_RPT, erm, fld, flc, dbs ) ) > 0;
			}
			free( fld );
		}
	}
	Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );

	return( chk );
}

int		Dsc_Get_Dps(				// getting other deposit/discount data
Csr				dst,				// root pointer of destination
DSC_RPT			*key,				// key data
Csr				rot,				// excepting key
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			siz=1024, chk, pos=0, cnt=0, flc;
	DSC_RPT		dsc[1], *ptr;
	WCHAR		*sql, opb[96], brb[96], fcb[96], buf[40];
	Dbs_fld		*fld;
	Csr			csr=NULL;

	memset( dsc, 0, sizeof(dsc) );

	if ( chk = !!( sql = (WCHAR *)malloc( wsizeof(siz) ) ) ){ 
		swprintf_s( brb, memcnt(brb), !Nul_Fld(key->brc,FLD_WST) ? _T("='%s' "): _T(" IS NULL "), DBS_WST_PTR(key->brc) );
		swprintf_s( fcb, memcnt(fcb), !Nul_Fld(key->fcc,FLD_WST) ? _T("='%s' "): _T(" IS NULL "), DBS_WST_PTR(key->fcc) );
		if ( chk = !!( flc = Fdf_Dsc_Rpt( &fld, dsc, NULL, NULL, 0 ) ) ){
			pos = swprintf_s( sql, siz,
						_T("SELECT * ")
						_T("FROM %s ")		// descriptions
						_T("WHERE operator_code='%s' ")
						_T("AND branch_code%s ")
						_T("AND facility_code%s ")
						_T("AND ( descriptoin_kind=%d OR descriptoin_kind=%d ) ")
						_T("AND deleted_date IS NULL "),
						TBN_DSC_RPT, 
						SQL_Edt_Wst(opb,memcnt(opb),DBS_WST_PTR(key->opc)),
						brb, fcb, DRK_DEPOSIT, DRK_DISCOUNT );
			// excepting editting data
			while( !!chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
				ptr = Rec_Of(DSC_RPT, csr);
				if ( !Nul_Fld(ptr->id, FLD_INT) ){
					swprintf_s( buf, memcnt(buf), _T("%s%s%d"),
						!cnt ? _T("AND id NOT IN ("): _T(""), 
						!cnt ? _T(""): _T(","), DBS_INT_VAL(ptr->id) );
					chk = !!( sql = add_str( sql, buf ) );
					cnt++;
				}
			}
			if ( !!cnt )	sql = add_str( sql, _T(")") );
			if ( chk ){
				chk = SQL_Get_Lst( dst, sql, fld, flc, dsc, sizeof(dsc), erm, dbc );
			}
			free( fld );
		}
		free( sql );
	}

	return( chk );
}
