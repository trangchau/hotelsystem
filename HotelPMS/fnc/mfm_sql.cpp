/*
	ホテルプロパティ管理システム
	検索ウィンドウ用データベースアクセス関数
*/

#include "..\stdafx.h"
#include "..\resource.h"
#include <afxdb.h>
#include "..\..\Common\bsc_def.h"
#include "..\..\Common\sys_def.h"
#include "..\..\Common\dbs_mng.h"
#include "..\..\Common\cmn_fnc.h"
#include "..\..\Common\sys_log.h"

#include "..\..\pms_def.h"
#include "sql_cmn.h"
#include "fld_def.h"
#include "mfm_sql.h"
#include "viw_cmn.h"

typedef	int	(*SQL_FDF_FNC)( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );

static	int		get_opr(			// 運営者情報読込み
Csr				rot,				// 格納領域
WCHAR			**erm,				// エラーメッセージ格納領域
CDatabase		*dbc,				// データベース接続オブジェクト
PMS_MNG			*mng )				// HotelPMS system management segment
{
	OPR_INF		opr[1];
	WCHAR		sql[256], opb[96];
	Dbs_fld		*fld;
	int			flc, chk;

	if ( chk = !!( flc = Fdf_Opr_Inf( &fld, opr, NULL, NULL, 0 ) ) ){
		swprintf_s( sql, memcnt(sql),
					_T("SELECT * FROM %s ")				// operator_infos
					_T("WHERE operator_code='%s' ")
					_T("AND deleted_date IS NULL"),
					TBN_OPR_INF, SQL_Edt_Wst( opb, memcnt(opb), mng->opc ) );

		chk = SQL_Get_Lst( rot, sql, fld, flc, opr, sizeof(opr), erm, dbc );
		free( fld );
	}
	return( chk );
}

static	int	get_brc(				// Getting each branch datas
Csr				rot,				// root of storing buffer
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	BRC_INF		brc[1];
	WCHAR		sql[256], opb[96];
	Dbs_fld		*fld;
	int			flc, chk;

	if ( chk = !!( flc = Fdf_Brc_Inf( &fld, brc, NULL, NULL, 0 ) ) ){
		swprintf_s( sql, memcnt(sql),
					_T("SELECT * FROM %s ")			// branch_infos
					_T("WHERE operator_code='%s' ")
					_T("AND deleted_date IS NULL ")
					_T("ORDER BY id "),
					TBN_BRC_INF, SQL_Edt_Wst( opb, memcnt(opb), mng->opc ) );

		chk = SQL_Get_Lst( rot, sql, fld, flc, brc, sizeof(brc), erm, dbc );
		free( fld );
	}
	return( chk );
}

static	int	get_fcc(				// Getting each facility datas
Csr				rot,				// root of storing buffer
WCHAR			*brc,				// branch code
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	FCL_INF		fcc[1];
	WCHAR		sql[256], opb[96], brb[96];
	Dbs_fld		*fld;
	int			flc, chk;

	if ( chk = !!( flc = Fdf_Fcl_Inf( &fld, fcc, NULL, NULL, 0 ) ) ){
		swprintf_s( sql, memcnt(sql),
						_T("SELECT * FROM %s ")			// facility_infos
						_T("WHERE operator_code='%s' ")
						_T("AND branch_code='%s' ")
						_T("AND deleted_date IS NULL"),
						TBN_FCL_INF,
						SQL_Edt_Wst( opb, memcnt(opb), mng->opc ),
						SQL_Edt_Wst( brb, memcnt(brb), brc ) );
		chk = SQL_Get_Lst( rot, sql, fld, flc, fcc, sizeof(fcc), erm, dbc );
		free( fld );
	}

	return( chk );
}

static	int	get_bld(				// Getting each facility_building_info datas
Csr				rot,				// root of storing buffer
WCHAR			*brc,				// branch code
WCHAR			*fcc,				// facility code
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	BLD_INF		bld[1];
	WCHAR		sql[256], opb[96];
	Dbs_fld		*fld;
	int			flc, chk;

	if ( chk = !!( flc = Fdf_Bld_Inf( &fld, bld, NULL, NULL, 0 ) ) ){
		swprintf_s( sql, memcnt(sql),
						_T("SELECT * FROM %s ")				// facility_building_infos
						_T("WHERE operator_code='%s' ")
						_T("AND branch_info_id=%d ")
						_T("AND facility_info_id=%d ")
						_T("AND deleted_date IS NULL"),
						TBN_FCL_BLD, SQL_Edt_Wst( opb, memcnt(opb), mng->opc ),
						(int)(LONG_PTR)brc, (int)(LONG_PTR)fcc );
		chk = SQL_Get_Lst( rot, sql, fld, flc, bld, sizeof(bld), erm, dbc );
		free( fld );
	}

	return( chk );
}

static	int	get_eqi(				// Getting equipment_infos
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	EQP_INF		eqp[1];
	WCHAR		sql[512], opb[96], brb[96], fcb[96];
	Dbs_fld		*fld;
	int			flc, chk;

	if ( chk = !!( flc = Fdf_Eqp_Inf( &fld, eqp, NULL, NULL, 0 ) ) ){
		swprintf_s( sql, memcnt(sql),
						_T("SELECT * FROM %s ")				// equipment_infos
						_T("WHERE operator_code='%s' ")
						_T("AND branch_code='%s' ")
						_T("AND facility_code='%s' ")
						_T("AND equipment_kind=%d ")
						_T("AND deleted_date IS NULL"),
						TBN_EQP_INF,
						SQL_Edt_Wst(opb,memcnt(opb),mng->opc),
						SQL_Edt_Wst(brb,memcnt(brb),mng->brc),
						SQL_Edt_Wst(fcb,memcnt(fcb),mng->fcc), EQK_ROOM );
		chk = SQL_Get_Lst( mng->eil, sql, fld, flc, eqp, sizeof(eqp), erm, dbc );
		free( fld );
	}

	return( chk );
}

static	int	get_eqt(				// Getting equipment_types
Csr				rot,				// root of storing buffer
WCHAR			*brc,				// branch code
WCHAR			*fcc,				// facility code
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
					_T("SELECT * FROM %s ")				// equipment_types
					_T("WHERE operator_code='%s' ")
					_T("AND branch_code='%s' ")
					_T("AND facility_code='%s' ")
					_T("AND equipment_kind=%d ")
					_T("AND deleted_date IS NULL"), 
					TBN_EQP_TYP,
					SQL_Edt_Wst(opb,memcnt(opb),mng->opc),
					SQL_Edt_Wst(brb,memcnt(brb),brc),
					SQL_Edt_Wst(fcb,memcnt(fcb),fcc), EQK_ROOM );

		chk = SQL_Get_Lst( rot, sql, fld, flc, eqt, sizeof(eqt), erm, dbc );
		free( fld );
	}
	return( chk );
}

static	int	get_mks(				// Getting market_segmernts
Csr				rot,				// root of storing buffer
WCHAR			*brc,				// branch code
WCHAR			*fcc,				// facility code
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	MKT_SEG		mks[1];
	WCHAR		sql[512], opb[96], brb[96], fcb[96];
	Dbs_fld		*fld;
	int			flc, chk;

	if ( chk = !!( flc = Fdf_Mkt_Seg( &fld, mks, NULL, NULL, 0 ) ) ){
		swprintf_s( sql, memcnt(sql),
					_T("SELECT * FROM %s ")				// market_segments
					_T("WHERE operator_code='%s' ")
					//_T("AND branch_code='%s' ")
					//_T("AND facility_code='%s' ")
					_T("AND deleted_date IS NULL"), 
					TBN_MKT_SEG,
					SQL_Edt_Wst(opb,memcnt(opb),mng->opc),
					SQL_Edt_Wst(brb,memcnt(brb),brc),
					SQL_Edt_Wst(fcb,memcnt(fcb),fcc) );

		chk = SQL_Get_Lst( rot, sql, fld, flc, mks, sizeof(mks), erm, dbc );
		free( fld );
	}
	return( chk );
}

static	int	get_agt(				// Getting agent_infos
Csr				rot,				// root of storing buffer
WCHAR			*brc,				// branch code
WCHAR			*fcc,				// facility code
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	AGT_INF		agt[1];
	WCHAR		sql[512], opb[96];
	Dbs_fld		*fld;
	int			flc, chk;

	if ( chk = !!( flc = Fdf_Agt_Inf( &fld, agt, NULL, NULL, 0 ) ) ){
		swprintf_s( sql, memcnt(sql),
						_T("SELECT * FROM %s ")				// agent_infos
						_T("WHERE operator_code='%s' ")
						_T("AND deleted_date IS NULL"),
						TBN_AGT_INF, SQL_Edt_Wst(opb,memcnt(opb),mng->opc) );
		chk = SQL_Get_Lst( rot, sql, fld, flc, agt, sizeof(agt), erm, dbc );
		free( fld );
	}

	return( chk );
}

static	int	get_ctc(				// Getting country code and names
Csr				dst,				// root of storing buffer
WCHAR			*ucc,				// use_country_code
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	WCHAR		sql[256], ucb[20];
	int			chk;
	CON_TRY		ctc[1];	
	Dbs_fld		fld[] = {	{ ctc->ici,	_T("IC_id"),	FLD_INT, FLD_INT_SIZ(ctc->ici) },
							{ ctc->ctc,	_T("alpha2"),	FLD_WST, FLD_WST_SIZ(ctc->ctc) },
							{ ctc->cni,	_T("CN_id"),	FLD_INT, FLD_INT_SIZ(ctc->cni) },
							{ ctc->ctn,	_T("name"),		FLD_WST, FLD_WST_SIZ(ctc->ctn) }};

	swprintf_s( sql, memcnt(sql),
			_T("SELECT IC.id AS IC_id, CN.id AS CN_id, alpha2, name ")
			_T("FROM %s IC ")				// international_country_codes
			_T("LEFT JOIN country_code_names AS CN ON CN.international_country_code_id = IC.id ")
			_T("WHERE use_country_code='%s' "), TBN_ITN_CTR, SQL_Edt_Wst(ucb, memcnt(ucb), ucc ) );
	chk = SQL_Get_Lst( dst, sql, fld, memcnt(fld), ctc, sizeof(ctc), erm, dbc );

	return( chk );
}

static	int	get_lgc(				// Getting language code and names
Csr				dst,				// root of storing buffer
WCHAR			*ucc,				// use_country_code
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	WCHAR		sql[256], ucb[20];
	int			chk;
	LAN_GAG		lgc[1];	
	Dbs_fld		fld[] = {	{ lgc->ili,	_T("IL_id"),	FLD_INT, FLD_INT_SIZ(lgc->ili) },
							{ lgc->lgc,	_T("alpha2"),	FLD_WST, FLD_WST_SIZ(lgc->lgc) },
							{ lgc->lni,	_T("LN_id"),	FLD_INT, FLD_INT_SIZ(lgc->lni) },
							{ lgc->lgn,	_T("name"),		FLD_WST, FLD_WST_SIZ(lgc->lgn) }};

	swprintf_s( sql, memcnt(sql),
			_T("SELECT IL.id AS IL_id, LN.id AS LN_id, alpha2, name ")
			_T("FROM %s IL ")			// international_language_codes
			_T("LEFT JOIN language_code_names AS LN ON LN.international_language_code_id = IL.id ")
			_T("WHERE use_country_code='%s' "), TBN_ITN_LNG, SQL_Edt_Wst(ucb, memcnt(ucb), ucc ) );
	chk = SQL_Get_Lst( dst, sql, fld, memcnt(fld), lgc, sizeof(lgc), erm, dbc );

	return( chk );
}

/*
static	int	get_bzc(				// Getting business_calendar
Csr				dst,				// root of storing buffer
WCHAR			*brc,				// branch code
WCHAR			*fcc,				// facility code
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	BIZ_CDR		bzc[1], *bzp;
	BIZ_CDR_DAT	*dat;
	WCHAR		sql[512], opb[96], brb[96], fcb[96], buf[60];
	Dbs_fld		*fld;
	int			flc, chk, idx;
	Lnk			rot[1], *csr=NULL, *cse;
	
	swprintf_s( brb, memcnt(brb), _T("AND branch_code='%s' "), SQL_Edt_Wst(buf,memcnt(buf),mng->brc) );
	swprintf_s( fcb, memcnt(fcb), _T("AND facility_code='%s' "), SQL_Edt_Wst(buf,memcnt(buf),mng->fcc) );

	memset( rot, 0, sizeof(rot) );
	if ( chk = !!( flc = Fdf_Biz_Cdr( &fld, bzc, NULL, NULL, 0 ) ) ){
		for ( idx=0; !!chk && idx<3; idx++ ){
			swprintf_s( sql, memcnt(sql),
							_T("SELECT * FROM business_calendars ")
							_T("WHERE operator_code='%s' %s %s ")
							_T("AND deleted_date IS NULL"), SQL_Edt_Wst(opb,memcnt(opb),mng->opc),
							idx < 2 ? brb: _T(""), idx < 1 ? fcb: _T("") );
			if ( chk = SQL_Get_Lst( rot, sql, fld, flc, bzc, sizeof(bzc), erm, dbc ) ){
				if ( !!Cnt_Lnk( rot ) )	break;
			}
		}
		if ( chk ){
			while( csr=Nxt_Lnk( rot, csr ) ){
				bzp = Rec_Of(BIZ_CDR, csr);
				if ( chk = !!( cse = Add_Lnk( dst, NULL, sizeof(BIZ_CDR_DAT) ) ) ){
					dat = Rec_Of(BIZ_CDR_DAT, cse);
					*dat->dte = *DBS_DAY_PTR(bzp->dte);
					dat->knd = DBS_INT_VAL(bzp->knd);
				}
			}
		}
		free( fld );
	}
	Del_All( rot );

	return( chk );
}
*/

static	int	get_mst(				// Getting master data
Csr				dst,				// root of storing buffer
SQL_FDF_FNC		fnc,				// making field definition function
void			*tmp,				// temporaly data buffer(each table structure
int				tsz,				// sizeof temporaly data buffer
WCHAR			*tbn,				// table name
WCHAR			*brc,				// branch code
WCHAR			*fcc,				// facility code
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	WCHAR		sql[512], opb[96], brb[96], fcb[96], buf[60];
	Dbs_fld		*fld;
	int			flc, chk, idx;
	
	swprintf_s( brb, memcnt(brb), _T("AND branch_code='%s' "), SQL_Edt_Wst(buf,memcnt(buf),mng->brc) );
	swprintf_s( fcb, memcnt(fcb), _T("AND facility_code='%s' "), SQL_Edt_Wst(buf,memcnt(buf),mng->fcc) );

	if ( chk = !!( flc = fnc( &fld, tmp, NULL, NULL, 0 ) ) ){
		for ( idx=0; !!chk && idx<3; idx++ ){
			swprintf_s( sql, memcnt(sql),
							_T("SELECT * FROM %s ")
							_T("WHERE operator_code='%s' %s %s ")
							_T("AND deleted_date IS NULL"), tbn, SQL_Edt_Wst(opb,memcnt(opb),mng->opc),
							idx < 2 ? brb: _T(""), idx < 1 ? fcb: _T("") );
			if ( chk = SQL_Get_Lst( dst, sql, fld, flc, tmp, tsz, erm, dbc ) ){
				if ( !!Cnt_Lnk( dst ) )	break;
			}
		}
		free( fld );
	}

	return( chk );
}

int		Get_Ech_Mst(				// getting each masters
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
/*
OPR_INF		brc[1];
	BRC_INF		brc[1];
	FCL_INF		fcl[1];
	EQP_INF		eqp[1];
	EQP_TYP		eqt[1];
	AGT_INF		agt[1];

	static	struct	{
		int		tbk;
		Csr		dst;
		void	*tmp;
		WCHAR	*sql;
	}Mst_[] = {
//		{ FDF_OPR_INF, mng->opl, opr,
		{ FDF_BRC_INF, mng->brl. brc,
		{ FDF_FCL_INF, mng->fcl, fcl,
		{ FDF_EQP_INF, mng->eil, eqp,
		{ FDF_EQP_TYP, mng->etl, eqt,
		{ FDF_AGT_INF, mng->agl, agt,
	};
*/
	int			chk=TRUE;
	Lnk			rot[1], *csr, *cse;
	BIZ_CDR		biz[1], *bzp;
	BIZ_CDR_DAT	*dat;
	CRC_CVS		cvs[1], *cvp;

	memset( rot, 0, sizeof(rot) );

	if ( chk = get_opr( rot, erm, dbc, mng ) ){
		if ( chk = !!( csr = Nxt_Lnk( rot, NULL ) ) ){
			*(mng->opr) = *Rec_Of(OPR_INF, csr);
		}
		Del_All( rot );
	}
	if( chk ){
		// branch_infos and facility_infos
		if ( chk = get_brc( mng->brl, erm, dbc, mng ) ){
			chk = get_fcc( mng->fcl, mng->brc, erm, dbc, mng );
		}
	}
	if ( chk ){
		// equipment_infos and equipment_types
		if ( chk = get_eqi( erm, dbc, mng ) ){
			chk = get_eqt( mng->etl, mng->brc, mng->fcc, erm, dbc, mng );
		}
	}
	if ( chk ){
		// Getting market_segmrnts
		chk = get_mks( mng->msl, mng->brc, mng->fcc, erm, dbc, mng );
	}
	if ( chk ){
		// agent_infos
		chk = get_agt( mng->agl, mng->brc, mng->fcc, erm, dbc, mng );
	}
	if ( chk ){
		// business_calendar
		//chk = get_bzc( mng->bcl, mng->brc, mng->fcc, erm, dbc, mng );
		chk = get_mst( rot, Fdf_Biz_Cdr, biz, sizeof(biz), TBN_BIZ_CDR, mng->brc, mng->fcc, erm, dbc, mng );
		if ( chk ){
			for( csr=NULL; csr=Nxt_Lnk( rot, csr ); ){
				bzp = Rec_Of(BIZ_CDR, csr);
				if ( chk = !!( cse = Add_Lnk( mng->bcl, NULL, sizeof(BIZ_CDR_DAT) ) ) ){
					dat = Rec_Of(BIZ_CDR_DAT, cse);
					*dat->dte = *DBS_DAY_PTR(bzp->dte);
					dat->knd = DBS_INT_VAL(bzp->knd);
				}
			}
		}
		Del_All( rot );
	}
	if ( chk ){
		// business_calendar
		chk = get_mst( rot, Fdf_Crc_Cvs, cvs, sizeof(cvs), TBN_CRC_CVS, mng->brc, mng->fcc, erm, dbc, mng );
		if ( chk ){
			for ( csr=NULL; csr = Nxt_Lnk( rot, csr ); ){
				cvp = Rec_Of(CRC_CVS, csr);
				if ( DBS_INT_VAL(cvp->isb)!=0 ){
					mng->bsc->adp = DBS_INT_VAL(cvp->adp);
					mng->bsc->rte = DBS_DBL_VAL(cvp->rte);
					wcscpy_s(mng->bsc->cod, memcnt(mng->bsc->cod), DBS_WST_PTR(cvp->crc) ); 
					wcscpy_s(mng->bsc->nam, memcnt(mng->bsc->nam), DBS_WST_PTR(cvp->nam) ); 
					break;
				}
			}
		}
		Del_All( rot );
	}
	if ( chk ){
		// Getting country code and names
		if ( chk = get_ctc( mng->ccl, DBS_WST_PTR(mng->lgi->nto), erm, dbc, mng ) ){
			chk = get_lgc( mng->lgl, DBS_WST_PTR(mng->lgi->lng), erm, dbc, mng );
		}
	}


	return( chk );
}

int		Get_Mst(					// getting each masters
Csr				rot,				// storing root
int				knd,				// Kind of master
WCHAR			*brc,				// branch code ( only in case of facility_infos )
WCHAR			*fcc,				// facility code ( only in case of equipment_types or equipment_infos )
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk;

	switch( knd ){
	case GTM_OPR_INF:	chk = get_opr( rot, erm, dbc, mng );			break;
	case GTM_BRC_INF:	chk = get_brc( rot, erm, dbc, mng );			break;
	case GTM_FCL_INF:	chk = get_fcc( rot, brc, erm, dbc, mng );		break;
	case GTM_EQP_TYP:	chk = get_eqt( rot, brc, fcc, erm, dbc, mng );	break;
	case GTM_AGT_INF:	chk = get_agt( rot, brc, fcc, erm, dbc, mng );	break;
	case GTM_BLD_INF:	chk = get_bld( rot, brc, fcc, erm, dbc, mng );	break;
	case GTM_MKT_SEG:	chk = get_mks( rot, brc, fcc, erm, dbc, mng );	break;
//	case GTM_BIZ_CDR:	chk = get_bzc( rot, brc, fcc, erm, dbc, mng );	break;
	}

	return( chk );
}


int		Get_Bdt(					// Getting the business date
Day				*dst,				// Desination storeing poinrt
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk;
	WCHAR		sql[512];
	Day			bdt[DBF_SIZ_DAY];
	CRecordset	*rec;
	Dbs_fld		fld[1] = { bdt, _T("business_date"), FLD_DAY, FLD_DAY_SIZ(bdt) };

	swprintf_s( sql, memcnt(sql),
				_T("SELECT business_date FROM %s ")			// business_dates
				_T("WHERE operator_code='%s' AND branch_code='%s' ")
				_T("AND deleted_date IS NULL "), TBN_BIZ_DTE, mng->opc, mng->brc );


	rec = Dbs_Qry( dbc, sql, erm );

	if ( chk = !!rec ){
		if ( *erm )	free( *erm );
		*erm = NULL;
		chk = Dbs_Get( fld, erm, memcnt(fld), rec )==QRY_NML;
		Dbs_Cls(rec);
		if ( chk ){
			*dst = *DBS_DAY_PTR(bdt);
		}
	}
	return( chk );
}


int		Mfm_Get_Crc(				// Gtting currency_conversions
Csr				rot,				// root of storing buffer
CRC_CVS			*key,				// key data
int				sec,				// except service kind
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	CRC_CVS		ccv[1];
	WCHAR		sql[512], opb[96], brb[96], fcb[96];
	Dbs_fld		*fld;
	int			flc, chk;

	memset( ccv, 0, sizeof(ccv) );

	if ( chk = !!( flc = Fdf_Crc_Cvs( &fld, ccv, NULL, NULL, 0 ) ) ){
		swprintf_s( brb, memcnt(brb), !Nul_Fld(key->brc,FLD_WST) ? _T("='%s' "): _T(" IS NULL "), DBS_WST_PTR(key->brc) );
		swprintf_s( fcb, memcnt(fcb), !Nul_Fld(key->fcc,FLD_WST) ? _T("='%s' "): _T(" IS NULL "), DBS_WST_PTR(key->fcc) );
		swprintf_s( sql, memcnt(sql),
					_T("SELECT * ")
					_T("FROM %s ")				// currency_conversions
					_T("WHERE operator_code='%s' ")
					_T("AND branch_code%s ")
					_T("AND facility_code%s ")
					_T("AND deleted_date IS NULL ")
					_T("ORDER BY id "),
					TBN_CRC_CVS, SQL_Edt_Wst(opb,memcnt(opb),DBS_WST_PTR(key->opc)), brb, fcb );

		chk = SQL_Get_Lst( rot, sql, fld, flc, ccv, sizeof(ccv), erm, dbc );
		free( fld );
	}
	return( chk );
}


int		Mfm_Sav_Crc(				// save currency_conversions
Csr				rot,				// root of destination
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk, flc, cri;
	Dbs_fld		*fld;
	Csr			csr=NULL;
	CRC_CVS		*ccv;
	Day			dte[1];
	WCHAR		sql[1920];

	Get_Dte( dte );
	chk = Dbs_Trn( erm, dbs, TRN_STT );
	while( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
		ccv = Rec_Of(CRC_CVS, csr);
		if ( chk  = !!( flc = Fdf_Crc_Cvs( &fld, ccv, NULL, NULL, 0 ) ) ){
			DBS_INT_CPY(ccv->mei, mng->lgi->id);
			if ( Nul_Fld(ccv->id, FLD_INT) ){
				DBS_DTM_SET(ccv->cdt, dte);
				chk = ( cri = SQL_Exc_Ins( sql, memcnt(sql), TBN_CRC_CVS, erm, fld, flc, dbs ) ) > 0;
			}
			else if ( !Nul_Fld(ccv->mdt, FLD_DTM) ){
				DBS_DTM_SET(ccv->mdt, dte);
				chk = ( cri = SQL_Exc_Upd( sql, memcnt(sql), TBN_CRC_CVS, erm, fld, flc, dbs ) ) > 0;
			}
			else if ( !Nul_Fld(ccv->ddt, FLD_DTM) ){
				DBS_DTM_SET(ccv->ddt, dte);
				chk = ( cri = SQL_Exc_Upd( sql, memcnt(sql), TBN_CRC_CVS, erm, fld, flc, dbs ) ) > 0;
			}
			free( fld );
		}
	}
	Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );

	return( chk );
}

int			Mfm_Get_Pwd(			// getting password
EMP_LYE			*dst,				// storing pointer
WCHAR			*uid,				// user ID
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int				chk, cnt, flc;
	EMP_LYE			epl[1];
	WCHAR			sql[256], uib[128], opb[96];
	Dbs_fld			*fld;
	CRecordset		*rec;

	memset( epl, 0, sizeof(epl) );

	if ( chk = !!( flc = Fdf_Emp_Lye( &fld, epl, NULL, NULL, 0 ) ) ){
		swprintf_s( sql, memcnt(sql),
			_T("SELECT * FROM %s ")				// employee
			_T("WHERE employee_code='%s' ")
			_T("AND operator_code='%s' "),
			TBN_EMP_LYE, SQL_Edt_Wst(uib,memcnt(uib),uid),
			SQL_Edt_Wst(opb,memcnt(opb),mng->opc) );

		if ( chk = !!( rec = Dbs_Qry( dbs, sql, erm ) ) ){
			switch( Dbs_Get( fld, erm, flc, rec ) ){
			case QRY_NML:	cnt=1;		*dst = *epl;	break;
			case QRY_END:	cnt=0;						break;
			default:		chk=FALSE;					break;
			}
			delete	rec;
		}
		free( fld );
	}
	return( chk ? cnt: -1 );
}

int			Mfm_Upd_Pwd(			// updating password
WCHAR			*uid,				// user ID
WCHAR			*pwd,				// password
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	WCHAR		sql[256], opb[96];

	swprintf_s( sql, memcnt(sql),
		_T("UPDATE %s SET password='%s' ")		// employee
		_T("WHERE employee_code='%s' ")
		_T("AND operator_code='%s' "), TBN_EMP_LYE, pwd, uid, SQL_Edt_Wst(opb,memcnt(opb),mng->opc) );

	return( Dbs_Upd( dbs, sql, erm )==QRY_NML );
}

int			Mfm_Lck_Pwd(			// locking password
WCHAR			*uid,				// user ID
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	WCHAR		sql[256], opb[96];

	swprintf_s( sql, memcnt(sql),
		_T("UPDATE %s SET locked=1 ")		// employee
		_T("WHERE employee_code='%s' ")
		_T("AND operator_code='%s' "), TBN_EMP_LYE, uid, SQL_Edt_Wst(opb,memcnt(opb),mng->opc) );

	return( Dbs_Upd( dbs, sql, erm )==QRY_NML );
}

int			Mfm_Get_Rsv(			// getting reservation_info_id
DRL_PRM			*dst,				// storing pointer
WCHAR			*str,				// inputted string
int				knd,				// launch kind
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk, pos=0, cnt=0;
	WCHAR		sql[1024], kyb[64], opb[96], brb[96], fcb[96];
	Dbs_fld		fld[] =		{	{ dst->rii, _T("RI_id"), FLD_INT, FLD_INT_SIZ(dst->rii) },
								{ dst->rdi, _T("RD_id"), FLD_INT, FLD_INT_SIZ(dst->rdi) }};
	CRecordset	*rec;

	pos = swprintf_s( sql, memcnt(sql),
		_T("SELECT RI.id AS RI_id, RD.id AS RD_id ")
		_T("FROM %s RI ")																		// reservation_infos
		_T("LEFT JOIN %s RD ON RD.reservation_info_id = RI.id AND RD.deleted_date IS NULL ")		// reservation_detail_id
		_T("LEFT JOIN %s RE ON RE.reservation_detail_id = RD.id AND RE.deleted_date IS NULL  ")	// reservation_equipment_id
		_T("LEFT JOIN %s EI ON EI.id = RE.equipment_info_id AND EI.deleted_date IS NULL  ")		// equipment_info_id
		_T("WHERE RI.deleted_date IS NULL AND EI.equipment_code='%s' ")
		_T("AND RD.canceled=0 AND RI.no_show=0 AND RI.operator_code='%s' ")
		_T("AND RI.branch_code='%s' AND RI.facility_code='%s' "),
		TBN_RSV_INF, TBN_RSV_DTL, TBN_RSV_EQP, TBN_EQP_INF, SQL_Edt_Wst(kyb,memcnt(kyb),str),
		SQL_Edt_Wst(opb,memcnt(opb),mng->opc), SQL_Edt_Wst(brb,memcnt(brb),mng->brc), SQL_Edt_Wst(fcb,memcnt(fcb),mng->fcc));

	switch( knd ){
	case VWK_CIN:	pos += swprintf_s( sql+pos, memcnt(sql)-pos,
						_T("AND CAST(RD.arrival_date AS date)=CAST(RE.use_start_date AS date) ")
						_T("AND RD.inhouse=0 ") );
					break;
	case VWK_INH:	pos += swprintf_s( sql+pos, memcnt(sql)-pos,
						_T("AND RE.inhouse!=0 ") );
					break;
	case VWK_COT:	pos += swprintf_s( sql+pos, memcnt(sql)-pos,
						_T("AND CAST(RD.departure_date AS date)=CAST(RE.use_end_date AS date) ")
						_T("AND RD.inhouse!=0 ") );
					break;
	}

	if ( chk = !!(rec = Dbs_Qry( dbs, sql, erm ) ) ){
		switch( Dbs_Get( fld, erm, memcnt(fld), rec ) ){
		case QRY_NML:	cnt = 1;		break;
		case QRY_END:	cnt = 0;		break;
		default:		chk = FALSE;	break;
		}
		delete	rec;
	}

	return( chk ? cnt: -1 );
}
