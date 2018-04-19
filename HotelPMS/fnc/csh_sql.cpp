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
#include "csh_sql.h"
#include "fld_def.h"

int		Csh_Get_Dat(				// Gtting operator_info
Csr				dst,				// information of getting data 
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	WCHAR		*sql, buf[192], *sll;
	int			chk, idx, rdi=0, siz=1920;
	Lnk			rot[1], *csr=NULL, *cse=NULL;
	CSH_INH_DAT	*inh;
	CSH_SAL_DTL	*dtl;
	Mlt_dbf		mdf[2];
	struct	_Tmp {
		SAL_INF		inf[1];
		CSH_SAL_DTL	dtl[1];
	}rec[1], *tpp;
	Dbs_fld		fld[] = {	{ rec->dtl->apl, _T("tax_application"),	FLD_INT, FLD_INT_SIZ(rec->dtl->apl) },
							{ rec->dtl->dkd, _T("description_kind"),FLD_INT, FLD_INT_SIZ(rec->dtl->dkd) },
							{ rec->dtl->prk, _T("price_kind"),		FLD_INT, FLD_INT_SIZ(rec->dtl->prk) },
							{ rec->dtl->rmn, _T("equipment_code"),	FLD_WST, FLD_WST_SIZ(rec->dtl->rmn) },
							{ rec->dtl->dcn, _T("name"),			FLD_WST, FLD_WST_SIZ(rec->dtl->dcn) }};

	memset( rec, 0, sizeof(rec) );
	memset( rot, 0, sizeof(rot) );

	chk  = !!( sql = (WCHAR *)malloc( wsizeof(siz) ) );
	chk &= !!(mdf[0].flc = Fdf_Sal_Inf( &mdf[0].fld, rec->inf, NULL, NULL, 0 ) );
	chk &= !!(mdf[1].flc = Fdf_Sal_Dtl( &mdf[1].fld, rec->dtl->dtl, _T("SD_"), fld, memcnt(fld) ) );
	chk &= !!Fdf_Mak_Sll( &sll, FDF_SAL_DTL, _T("SD"), _T("SD_") );

	if ( chk ){
		swprintf_s( sql, siz, 
			_T("SELECT SI.*, %s, DS.tax_application, DS.name, DS.description_kind, ")
			_T("DS.price_kind, EI.equipment_code ")
			_T("FROM %s SD ")																	// sales_details
			_T("LEFT JOIN %s SI ON SI.id = SD.sales_info_id AND SI.deleted_date IS NULL ")		// sales_infos
			_T("LEFT JOIN %s DS ON DS.id = SD.description_id AND DS.deleted_date IS NULL ")		// descriptions
			_T("LEFT JOIN %s EI ON EI.id = SD.equipment_info_id AND EI.deleted_date IS NULL ")	// equipment_infos
			_T("WHERE SI.reservation_detail_id IN (" ),
			sll, TBN_SAL_DTL, TBN_SAL_INF, TBN_DSC_RPT, TBN_EQP_INF );
		for ( idx=0; chk && !!(csr=Nxt_Lnk(dst, csr)); idx++ ){
			inh = Rec_Of(CSH_INH_DAT, csr);
			swprintf_s( buf, memcnt(buf), _T("%s%d"), !idx ? _T(""): _T(","), DBS_INT_VAL(inh->rdi) );
			chk = !!( sql = Add_Str( sql, buf ) );
		}
		if ( chk ){
			swprintf_s( buf, memcnt(buf),
				_T(") ")
				_T("AND SD.deleted_date IS NULL ")
				_T("AND SI.reservation_info_id=%d ")
				_T("AND ( SI.paid=0 OR SI.paid IS NULL ) AND DS.description_kind!=%d ")
				_T("ORDER BY SI.reservation_detail_id, SD.sales_datetime, SD.id "),
				DBS_INT_VAL(inh->rii), DRK_DEPOSIT );
			if ( chk = !!( sql = Add_Str( sql, buf ) ) ){
				chk = SQL_Get_MLs( rot, sql, mdf, memcnt(mdf), rec, sizeof(rec), erm, dbc );
			}
		}
	}
	while ( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
		tpp = Rec_Of(struct _Tmp, csr );
		if ( rdi != DBS_INT_VAL(tpp->inf->rdi) ){
			rdi = DBS_INT_VAL(tpp->inf->rdi);
			inh = NULL;
			for ( cse=NULL; cse=Nxt_Lnk( dst, cse ); ){
				inh = Rec_Of(CSH_INH_DAT, cse);
				if ( DBS_INT_VAL(inh->rdi) == rdi ){
					*inh->sal->sal = *tpp->inf;
					break;
				}
			}
			inh = !!cse ? inh: NULL;
		}
		if ( !!inh ){
			if ( cse = Add_Lnk( inh->sal->dtl, NULL, sizeof(CSH_SAL_DTL) ) ){
				dtl = Rec_Of(CSH_SAL_DTL, cse);
				*dtl->dtl = *tpp->dtl->dtl;
				DBS_INT_CPY(dtl->apl, tpp->dtl->apl);
				DBS_INT_CPY(dtl->dkd, tpp->dtl->dkd);
				DBS_INT_CPY(dtl->prk, tpp->dtl->prk);
				DBS_WST_CPY(dtl->dcn, tpp->dtl->dcn);
				DBS_WST_CPY(dtl->rmn, tpp->dtl->rmn);
			}
		}
	}

	if ( mdf[0].fld )	free( mdf[0].fld );
	if ( mdf[1].fld )	free( mdf[1].fld );
	if ( sll )			free( sll );
	if ( sql )			free( sql );

	Del_All( rot );
	return( chk );
}

int		Csh_Get_Dps(				// Gtting deposit data
Csr				dst,				// information of getting data
int				rdi,				// unit getting if this item is not 0
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk, idx, siz=980;
	Lnk			rot[1], *csr=NULL, *cse;
	WCHAR		*sql, buf[256];
	CSH_INH_DAT	*inh;
	CSH_DPS_DAT	dps[1], *dpp;
	Mlt_dbf		mdf[2];
	Dbs_fld		ext[] = {	{ dps->rii, _T("reservation_info_id"),	FLD_INT, FLD_INT_SIZ(dps->rii) },
							{ dps->rdi, _T("reservation_detail_id"),FLD_INT, FLD_INT_SIZ(dps->rdi) },
							{ dps->crc, _T("currency_code"),		FLD_WST, FLD_WST_SIZ(dps->crc) },
							{ dps->isb, _T("is_base"),				FLD_INT, FLD_INT_SIZ(dps->isb) },
							{ dps->cci, _T("CC_id"),				FLD_INT, FLD_INT_SIZ(dps->cci) },
							{ dps->rte, _T("exchange_rate"),		FLD_DBL, FLD_DBL_SIZ(dps->rte) },
							{ dps->adp, _T("after_dec_point"),		FLD_INT, FLD_INT_SIZ(dps->adp) }};

	memset( rot, 0, sizeof(rot) );
	memset( dps, 0, sizeof(dps) );

	chk  = !!( sql = (WCHAR *)malloc( wsizeof(siz) ) );
	chk &= !!( mdf[0].flc = Fdf_Pmt_Dtl( &mdf[0].fld, dps->pmt, NULL, NULL, 0 ) );
	chk &= !!( mdf[1].flc = Fdf_Sal_Dtl( &mdf[1].fld, dps->dtl, NULL, ext, memcnt(ext) ) );

	if ( chk ){
		swprintf_s( sql, siz,
			_T("SELECT PD.*, SD.*, SI.reservation_info_id, SI.reservation_detail_id, ")
			_T("CC.currency_code, CC.after_dec_point, CC.is_base, CC.exchange_rate, CC.id AS CC_id ")
			_T("FROM %s SD ")																		// sales_details
			_T("LEFT JOIN %s SI ON SI.id = SD.sales_info_id AND SI.deleted_date IS NULL ")			// sales_infos
			_T("LEFT JOIN %s PD ON PD.sales_detail_id = SD.id AND PD.deleted_date IS NULL ")		// payment_details
			_T("LEFT JOIN %s DS ON DS.id = SD.description_id AND SD.deleted_date IS NULL ")			// descriptions
			_T("LEFT JOIN %s CC ON CC.id = PD.currency_conversion_id AND CC.deleted_date IS NULL ")	// currency_conversions
			_T("WHERE SI.reservation_detail_id IN (  "),
			TBN_SAL_DTL, TBN_SAL_INF, TBN_PMT_DTL, TBN_DSC_RPT, TBN_CRC_CVS );

		if ( !!rdi ){
			swprintf_s( buf, memcnt(buf), _T("%d"), rdi );
			chk = !!( sql = Add_Str( sql, buf ) );
		}
		else{
			for ( idx=0; chk && !!(csr=Nxt_Lnk(dst, csr)); idx++ ){
				inh = Rec_Of(CSH_INH_DAT, csr);
				swprintf_s( buf, memcnt(buf), _T("%s%d"), !idx ? _T(""): _T(","), DBS_INT_VAL(inh->rdi) );
				chk = !!( sql = Add_Str( sql, buf ) );
			}
		}
		swprintf_s( buf, memcnt(buf),
			_T(" ) AND DS.description_kind=%d ")
			_T("AND SD.deleted_date IS NULL ")
			_T("ORDER BY SI.reservation_detail_id, SD.sales_datetime "), DRK_DEPOSIT );
		if ( chk = !!( sql = Add_Str( sql, buf ) ) ){
			chk = SQL_Get_MLs( rot, sql, mdf, memcnt(mdf), dps, sizeof(dps), erm, dbc );
		}
	}
	if ( !rdi ){
		while ( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
			dpp = Rec_Of(CSH_DPS_DAT, csr );
			if ( rdi != DBS_INT_VAL(dpp->rdi) ){
				rdi = DBS_INT_VAL(dpp->rdi);
				inh = NULL;
				for ( cse=NULL; cse=Nxt_Lnk( dst, cse ); ){
					inh = Rec_Of(CSH_INH_DAT, cse);
					if ( DBS_INT_VAL(inh->rdi) == rdi ){
						break;
					}
				}
				inh = !!cse ? inh: NULL;
			}
			if ( !!inh ){
				if ( cse = Add_Lnk( inh->dps, NULL, sizeof(CSH_DPS_DAT) ) ){
					*Rec_Of(CSH_DPS_DAT, cse) = *dpp;
				}
			}
		}
	}
	else{
		*dst = *rot;
		memset( rot, 0, sizeof(rot) );
	}
	if ( sql )			free( sql );
	if ( mdf[0].fld )	free( mdf[0].fld );
	if ( mdf[1].fld )	free( mdf[1].fld );

	Del_All( rot );

	return( chk );
}

static	int	sav_sli(			// saving a sales_infos
CSH_SAL_DAT		*sal,			// Saving data
int				sup,			// is sale up( numbering invoice number 
Day				*dte,			// Saving datetime
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk, fct, sii, val=0, ivn[DBF_SIZ_INT];
	Dbs_fld		*fld, ivf[] = { ivn, _T("invoice_number"), FLD_WSI, FLD_INT_SIZ(ivn) };
	WCHAR		sql[1024], opb[96], brb[96], fcb[96];
	CRecordset	*rec;
	
	if ( chk = !!( fct = Fdf_Sal_Inf( &fld, sal->sal, NULL, NULL, 0 ) ) ){
		DBS_INT_CPY(sal->sal->mei, mng->lgi->id );
		if ( Nul_Fld(sal->sal->id,FLD_INT) ){
			DBS_DTM_SET(sal->sal->cdt, dte );
			DBS_INT_SET(sal->sal->ivn, &val );
			chk = !!( sii = SQL_Exc_Ins( sql, memcnt(sql), TBN_SAL_INF, erm, fld, fct, dbs ) );
		}
		else{
			sii = DBS_INT_VAL(sal->sal->id);
			if ( !Nul_Fld(sal->sal->mdt, FLD_DTM) ){
				DBS_DTM_SET(sal->sal->mdt, dte );
				chk = SQL_Exc_Upd( sql, memcnt(sql), TBN_SAL_INF, erm, fld, fct, dbs );
			}
		}
		if ( chk && sup ){
			swprintf_s( sql, memcnt(sql), 
				_T("SELECT MAX(invoice_number)+1 AS invoice_number ")
				_T("FROM %s ")			// sales_infos
				_T("WHERE operator_code='%s' AND branch_code='%s' AND facility_code='%s' "),
				TBN_SAL_INF,
				SQL_Edt_Wst(opb, memcnt(opb), DBS_WST_PTR(sal->sal->opc)),
				SQL_Edt_Wst(brb, memcnt(brb), DBS_WST_PTR(sal->sal->brc)),
				SQL_Edt_Wst(fcb, memcnt(fcb), DBS_WST_PTR(sal->sal->fcc)) );
			if ( chk = !!( rec = Dbs_Qry( dbs, sql, erm ) ) ){
				chk = Dbs_Get( ivf, erm, memcnt(ivf), rec ) == QRY_NML;
				delete rec;
			}
			if ( chk ){	
				swprintf_s( sql, memcnt(sql), 
					_T("UPDATE %s SET invoice_number=%d ")		// sales_infos
					_T("WHERE id=%d "), TBN_SAL_INF, DBS_INT_VAL(ivn), sii );
				if ( chk = Dbs_Upd( dbs, sql, erm )==QRY_NML ){
					DBS_INT_SET(sal->sal->id, &sii);
				}
			}			
		}
		free( fld );
	}

	return( chk ? sii: 0 );
}

static	int	sav_sld(			// saving a sales_details
SAL_DTL			*dtl,			// Saving data
int				sii,			// sales_info_id
Day				*dte,			// Saving datetime
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk, fct, sdi;
	Dbs_fld		*fld;
	WCHAR		sql[1024];

	if ( chk = !!( fct = Fdf_Sal_Dtl( &fld, dtl, NULL, NULL, 0 ) ) ){
		DBS_INT_CPY(dtl->mei, mng->lgi->id );
		DBS_INT_SET(dtl->sii, &sii );
		if ( Nul_Fld(dtl->id,FLD_INT) ){
			DBS_DTM_SET(dtl->cdt, dte );
			DBS_DTM_SET(dtl->sdt, dte );
			DBS_DAY_SET(dtl->sld, mng->bdt );
			chk = !!( sdi = SQL_Exc_Ins( sql, memcnt(sql), TBN_SAL_DTL, erm, fld, fct, dbs ) );
		}
		else if ( !Nul_Fld(dtl->mdt, FLD_DTM) ){
			DBS_DTM_SET(dtl->mdt, dte );
			chk = SQL_Exc_Upd( sql, memcnt(sql), TBN_SAL_DTL, erm, fld, fct, dbs );
			sdi = DBS_INT_VAL(dtl->id);
		}
		free( fld );
	}

	return( chk ? sdi: 0 );
}

static	int	sav_pmt(			// saving a payment_details
PMT_DTL			*dtl,			// Saving data
int				sii,			// sales_info_id
int				sdi,			// sales_details_id
Day				*dte,			// Saving datetime
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk, fct, pdi;
	Dbs_fld		*fld;
	WCHAR		sql[1024];

	if ( chk = !!( fct = Fdf_Pmt_Dtl( &fld, dtl, NULL, NULL, 0 ) ) ){
		DBS_INT_CPY(dtl->mei, mng->lgi->id );
		DBS_INT_SET(dtl->sii, &sii );
		if ( !!sdi )	DBS_INT_SET(dtl->sdi, &sdi );
		if ( Nul_Fld(dtl->id,FLD_INT) ){
			DBS_DTM_SET(dtl->cdt, dte );
			DBS_DTM_SET(dtl->pdt, dte );
			DBS_DAY_SET(dtl->pmd, mng->bdt );
			chk = !!( pdi = SQL_Exc_Ins( sql, memcnt(sql), TBN_PMT_DTL, erm, fld, fct, dbs ) );
		}
		else if ( !Nul_Fld(dtl->mdt, FLD_DTM) ){
			DBS_DTM_SET(dtl->mdt, dte );
			chk = SQL_Exc_Upd( sql, memcnt(sql), TBN_PMT_DTL, erm, fld, fct, dbs );
			pdi = DBS_INT_VAL(dtl->id);
		}
		free( fld );
	}

	return( chk ? pdi: 0 );
}

static	int		sav_dat(		// Saving operator_info
CSH_SAL_DAT		*sal,			// sales_infos data
Csr				rot,			// payment data
Csr				dpr,			// root of history of payment by deposit
int				sup,			// is sale up( numbering invoice number )
Day				*dte,			// Saving datetime
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk, sii, sdi, pdi;
	CSH_SAL_DTL	*dtl;
	CSH_DPS_DAT	*dps;
	PMT_DTL		*pmt;
	Csr			csr=NULL;

	if ( chk = !!( sii = sav_sli( sal, sup, dte, erm, dbs, mng ) ) ){
		while( chk && !!( csr=Nxt_Lnk( sal->dtl, csr ) ) ){
			dtl = Rec_Of(CSH_SAL_DTL, csr);
			chk = !!sav_sld( dtl->dtl, sii, dte, erm, dbs, mng );
		}
	}
	if ( rot ){
		while( chk && !!( csr=Nxt_Lnk(rot, csr) ) ){
			sdi = 0;
			pmt = Rec_Of(PMT_DTL, csr);
			chk = !!sav_pmt( pmt, sii, sdi, dte, erm, dbs, mng );
		}
	}
	if ( dpr ){
		while( chk && !!( csr=Nxt_Lnk(dpr, csr) ) ){
			sdi = 0;
			dps = Rec_Of(CSH_DPS_DAT, csr);
			if ( chk = !!( sdi = sav_sld( dps->dtl, sii, dte, erm, dbs, mng ) ) ){
				DBS_INT_SET( dps->dtl->id, &sdi);
				chk = !!( pdi = sav_pmt( dps->pmt, sii, sdi, dte, erm, dbs, mng ) );
				if ( chk ){
					DBS_INT_SET( dps->pmt->id, &pdi);
				}
			}
		}
	}

	return( chk );
}

static	int		sav_dps(		// Saving deposit data
CSH_DPS_DAT		*dps,			// deposit data
Day				*dte,			// Saving datetime
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk, sdi;

	if ( chk = !!( sdi = sav_sld( dps->dtl, DBS_INT_VAL(dps->dtl->sii), dte, erm, dbs, mng ) ) ){
		chk = !!sav_pmt( dps->pmt, DBS_INT_VAL(dps->dtl->sii), sdi, dte, erm, dbs, mng );
	}

	return( chk );
}

int		Csh_Sav_Dat(			// Saving operator_info
Csr				rot,			// Saving data
int				sup,			// is sale up( numbering invoice number )
Day				*dte,			// Saving datetime
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk;
	Csr			csr=NULL, cse=NULL;
	CSH_INH_DAT	*inh;

	if ( chk = Dbs_Trn( erm, dbs, TRN_STT ) ){
		while ( chk && !!( csr = Nxt_Lnk( rot, csr ) ) ){
			inh = Rec_Of(CSH_INH_DAT, csr);
			chk = sav_dat( inh->sal, NULL, NULL, sup, dte,erm, dbs, mng );
			while( chk && !!( cse=Nxt_Lnk( inh->dps, cse ) ) ){
				chk = sav_dps( Rec_Of(CSH_DPS_DAT, cse), dte, erm, dbs, mng );
			}
		}
		Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
	}

	return( chk );
}

int		Csh_Get_Pdt(			// Getting product list for product selecting
Csr				dst,			// root of storing data
WCHAR			*opc,			// operator_code
WCHAR			*brc,			// branch_code
WCHAR			*fcc,			// facility_code
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk, flc;
	CSH_PDT_LST	pdt[1];
	WCHAR		sql[1024], opb[96], brb[96], fcb[96], dbf[20], buf[96];
	Dbs_fld		*fld;
	Dbs_fld		ext[] = {	{ pdt->dci, _T("DC_id"),	FLD_INT, FLD_INT_SIZ(pdt->dci) },
							{ pdt->dcn, _T("DC_name"),	FLD_WST, FLD_WST_SIZ(pdt->dcn) }};

	memset( pdt, 0, sizeof(pdt) );

	*brb = *fcb = 0;
	Edt_Dte( dbf, memcnt(dbf), mng->bdt );
	if ( brc )	swprintf_s( brb, memcnt(brb), _T(" OR DS.branch_code='%s' ") , SQL_Edt_Wst(buf, memcnt(buf), brc) );
	if ( fcc )	swprintf_s( fcb, memcnt(fcb), _T(" OR DS.facility_code='%s' ") , SQL_Edt_Wst(buf, memcnt(buf), fcc) );

	if ( chk = !!( flc = Fdf_Dsc_Rpt( &fld, pdt->pdt, NULL, ext, memcnt(ext) ) ) ){
		swprintf_s( sql, memcnt(sql), 
			_T("SELECT DS.*, DC.id AS DC_id, DC.name AS DC_name ")
			_T("FROM %s DS ")																			// descriptions
			_T("LEFT JOIN %s DC ON DC.id = DS.description_category_id AND DC.deleted_date IS NULL ")	// description_categories
			_T("WHERE DS.operator_code='%s' ")
			_T("AND ( DS.branch_code IS NULL %s ) ")
			_T("AND ( DS.facility_code IS NULL %s ) ")
			_T("AND ( ( DS.valid_sale_period_start IS NULL OR DS.valid_sale_period_start <= '%s' ) ")
			_T("AND (   DS.valid_sale_period_end IS NULL OR DS.valid_sale_period_end >= '%s' ) ) ")
			_T("AND DS.sale_enable != 0 ")
			_T("AND DS.deleted_date IS NULL ")
			_T("AND DS.description_kind IN ( %d, %d ) ")
			_T("ORDER BY DC.display_order, DC.id "),
			TBN_DSC_RPT, TBN_DSC_CAT,
			SQL_Edt_Wst(opb, memcnt(opb), opc), brb, fcb, dbf, dbf, DRK_MERCHANDISE, DRK_EXTRA_SERVICE );
		chk = SQL_Get_Lst( dst, sql, fld, flc, pdt, sizeof(pdt), erm, dbs );
		free( fld );
	}

	return( chk );
}

int		Csh_Inh_Sal(			// Getting inbhouse sales data
Csr				dst,			// root of storing area	
int				rii,			// reservation_info_id
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{

	int			chk, flc;
	WCHAR		sql[1024];
	CSH_INH_DAT	inh[1];
	Dbs_fld		*fld;
	Dbs_fld		ext[] = {	{ inh->rii, _T("RI_id"),			FLD_INT, FLD_INT_SIZ(inh->rii) },
							{ inh->rdi, _T("RD_id"),			FLD_INT, FLD_INT_SIZ(inh->rdi) },
							{ inh->rei, _T("RE_id"),			FLD_INT, FLD_INT_SIZ(inh->rei) },
							{ inh->eii, _T("EI_id"),			FLD_INT, FLD_INT_SIZ(inh->eii) },
							{ inh->fsn, _T("first_name"),		FLD_WST, FLD_WST_SIZ(inh->fsn) },
							{ inh->lsn, _T("last_name"),		FLD_WST, FLD_WST_SIZ(inh->lsn) },
							{ inh->slg, _T("system_language"),	FLD_WST, FLD_WST_SIZ(inh->slg) },
							{ inh->rmn, _T("room_number"),		FLD_WST, FLD_WST_SIZ(inh->rmn) }};

	memset( inh, 0, sizeof(inh) );

	if ( chk = !!( flc = Fdf_Sal_Inf( &fld, inh->sal, NULL, ext, memcnt(ext) ) ) ){
		swprintf_s( sql, memcnt(sql), 
			_T("SELECT RD.id AS RD_id, SI.*, RE.id AS RE_id, RD.reservation_info_id AS RI_id,")
			_T("EI.id AS EI_id, EI.equipment_code AS room_number, CI.first_name, CI.last_name, CI.system_language ")
			_T("FROM %s RD ")																					// reservation_details
			_T("LEFT JOIN %s SI ON SI.reservation_detail_id = RD.id AND SI.deleted_date IS NULL ")				// sales_infos
			_T("AND ( SI.paid=0 OR SI.paid IS NULL ) ")
			_T("LEFT JOIN %s RE ON RE.reservation_detail_id = RD.id AND RE.deleted_date IS NULL ")				// reservation_equipments
//			_T("AND RE.use_start_date <= '%s' AND RE.use_end_date > '%s' ")
			_T("AND RE.inhouse!=0 ")
			_T("LEFT JOIN %s EI ON EI.id = RE.equipment_info_id AND EI.deleted_date IS NULL ")					// equipment_infos
			_T("LEFT JOIN %s CI ON CI.id = RD.client_info_id AND CI.deleted_date IS NULL ")						// client_infos
			_T("WHERE RD.reservation_info_id = %d ")
			_T("AND RD.inhouse != 0 AND RD.deleted_date IS NULL ")
			_T("ORDER BY EI.equipment_code "),
			TBN_RSV_DTL, TBN_SAL_INF, TBN_RSV_EQP,
			/*Edt_Dte(dbf, memcnt(dbf), mng->bdt), dbf,*/
			TBN_EQP_INF, TBN_CLI_INF, rii );
		chk = SQL_Get_Lst( dst, sql, fld, flc, inh, sizeof(inh), erm, dbs );
		free( fld );
	}

	return( chk );
}

int		Csh_Get_Ccv(				// Gtting currency_conversions
Csr				rot,				// root of storing buffer
CRC_CVS			*key,				// key data
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	CRC_CVS		ccv[1], *ptr;
	WCHAR		sql[512], opb[96], kyb[256], buf[96], crc[256];
	Dbs_fld		*fld;
	Csr			csr=NULL;
	int			flc, chk, pos, atr=0;

	*crc = 0;
	memset( ccv, 0, sizeof(ccv) );

	pos = swprintf_s( kyb, memcnt(kyb), _T("AND (branch_code IS NULL ") );
	if ( !Nul_Fld(key->brc,FLD_WST) ){
		pos += swprintf_s( kyb+pos, memcnt(kyb)-pos, _T("OR branch_code='%s' "), SQL_Edt_Wst(buf,memcnt(buf),DBS_WST_PTR(key->brc)) );
	}
	pos += swprintf_s( kyb+pos, memcnt(kyb)-pos, _T(") ") );
	pos += swprintf_s( kyb+pos, memcnt(kyb)-pos, _T("AND (facility_code IS NULL ") );
	if ( !Nul_Fld(key->fcc,FLD_WST) ){
		pos += swprintf_s( kyb+pos, memcnt(kyb)-pos, _T("OR facility_code='%s' "), SQL_Edt_Wst(buf,memcnt(buf),DBS_WST_PTR(key->fcc)) );
	}
	pos += swprintf_s( kyb+pos, memcnt(kyb)-pos, _T(") ") );

	if ( chk = !!( flc = Fdf_Crc_Cvs( &fld, ccv, NULL, NULL, 0 ) ) ){
		swprintf_s( sql, memcnt(sql),
					_T("SELECT * FROM %s ")				// currency_conversions
					_T("WHERE operator_code='%s' ")
					_T("%s ")
					_T("AND deleted_date IS NULL ")
					_T("ORDER BY currency_code, branch_code DESC, facility_code DESC"),
					TBN_CRC_CVS, SQL_Edt_Wst(opb,memcnt(opb),DBS_WST_PTR(key->opc)), kyb );

		if ( ( chk = SQL_Get_Lst( rot, sql, fld, flc, ccv, sizeof(ccv), erm, dbc ) ) ){
			// Leave only one attribute that most nearly to own facility if exist multiple same attribute
			// For example, ( opc=1, brc=NULL, fcc=NULL ), (opc=1, brc=1, fcc=NULL)<-leave
			// ORDER BY is for this reason
			csr = Nxt_Lnk( rot, NULL );
			while( csr ){
				ptr = Rec_Of(CRC_CVS, csr);
				if ( !!wcscmp(DBS_WST_PTR(ptr->crc), crc ) ){
					wcscpy_s(crc, memcnt(crc), DBS_WST_PTR(ptr->crc) );
					csr = Nxt_Lnk( rot, csr );
				}
				else{
					csr = Del_Lnk( rot, csr );
				}
			}
		}

		free( fld );
	}
	return( chk );
}

int		Csh_Pay_Mnt(			// payment
CSH_SAL_DAT		*sal,			// sales data
Csr				prt,			// payment data
Csr				dpr,			// root of history of payment by deposit 
Day				*dte,			// Saving datetime
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk;
	Csr			csr=NULL, cse=NULL;

	if ( chk = Dbs_Trn( erm, dbs, TRN_STT ) ){
		chk = sav_dat( sal, prt, dpr, TRUE, dte, erm, dbs, mng );
		Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
	}

	return( chk );
}

int		Dsc_Get_Dsc(				// Gtting tdeposit and discount of descriptions
Csr				rot,				// root of storing buffer
DSC_RPT			*key,				// key data
int				ntv,				// Only native ( business_date is in alid_sale_period ) 
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	CSH_TAX_DAT	dsc[1], *ptr;
	WCHAR		sql[1024], opb[96], kyb[512];
	Dbs_fld		*fld;
	Csr			csr=NULL;
	int			flc, chk, dkd=0, pkd=0;

	memset( dsc, 0, sizeof(dsc) );

	SQL_Dsc_Cnd( kyb, memcnt(kyb), key, ntv, mng );

	if ( chk = !!( flc = Fdf_Dsc_Rpt( &fld, dsc->tax, NULL, NULL, 0 ) ) ){
		swprintf_s( sql, memcnt(sql),
					_T("SELECT * FROM %s ")				// descriptions
					_T("WHERE operator_code='%s' ")
					_T("%s ")
					_T("AND description_kind IN ( %d, %d ) ")
// bucause these conditions within kyb( SQL_Dsc_Cnd function )
//					_T("AND sale_enable!=0 ")
//					_T("AND deleted_date IS NULL ")
					_T("ORDER BY description_kind ASC, price_kind ASC, branch_code DESC, facility_code DESC"),
					TBN_DSC_RPT, SQL_Edt_Wst(opb,memcnt(opb),DBS_WST_PTR(key->opc)), kyb, DRK_DEPOSIT, DRK_DISCOUNT );

		if ( ( chk = SQL_Get_Lst( rot, sql, fld, flc, dsc, sizeof(dsc), erm, dbc ) ) ){
			// Leave only one attribute that most nearly to own facility if exist multiple same attribute
			// For example, ( opc=1, brc=NULL, fcc=NULL ), (opc=1, brc=1, fcc=NULL)<-leave
			// ORDER BY is for this reason
			csr = Nxt_Lnk( rot, NULL );
			while( csr ){
				ptr = Rec_Of(CSH_TAX_DAT, csr);
				if ( DBS_INT_VAL(ptr->tax->dkd) != dkd || DBS_INT_VAL(ptr->tax->prk) != pkd ){
					dkd = DBS_INT_VAL(ptr->tax->dkd);
					pkd = DBS_INT_VAL(ptr->tax->prk);
					csr = Nxt_Lnk( rot, csr );
				}
				else{
					csr = Del_Lnk( rot, csr );
				}
			}
		}

		free( fld );
	}
	return( chk );
}

int		Csh_Sav_Dps(			// Saving deposit data
Csr				rot,			// deposit data
Day				*dte,			// Saving datetime
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int			chk;
	Csr			csr=NULL;
	CSH_DPS_DAT	*dps;

	if ( chk = Dbs_Trn( erm, dbs, TRN_STT ) ){
		while ( csr=Nxt_Lnk( rot, csr ) ){
			dps = Rec_Of(CSH_DPS_DAT, csr);
			chk = sav_dps( dps, dte, erm, dbs, mng );
		}
		Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
	}

	return( chk );
}

/*
int		Csh_Sav_Dps(			// Saving deposit data
Csr				rot,			// root of deposit data
Day				*dte,			// Saving datetime
WCHAR			**erm,			// Pointer of storing error message
CDatabase		*dbs,			// ODBC Database object
PMS_MNG			*mng )			// Hotel Property Management Sysytem structure
{
	int		chk;
	Csr		csr=NULL;

	if ( chk = Dbs_Trn( erm, dbs, TRN_STT ) ){
		while( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
			chk = sav_dps( Rec_Of(CSH_DPS_DAT, csr), dte, erm, dbs, mng );
		}
		Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
	}
	return( chk );
}
*/

int		Csh_Get_Sii(				// getting sales_info_id by reservation_detail_id
int				rdi,				// reservation_detail_id
SAL_INF			*sal,				// sales_infos data
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk, sts, sit, sii[DBF_SIZ_INT];
	WCHAR		sql[256];
	Dbs_fld		fld[1] = { sii, _T("id"), FLD_INT, FLD_INT_SIZ(sii) };
	CRecordset	*rec;
	Day			dte[1];
	CSH_SAL_DAT	sdt[1];

	swprintf_s( sql, memcnt(sql),
		_T("SELECT id FROM %s WHERE reservation_detail_id=%d ")
		_T("AND deleted_date IS NULL AND ( paid IS NULL OR paid=0 )"),
		TBN_SAL_INF, rdi );

	if ( chk = !!( rec = Dbs_Qry( dbs, sql, erm ) ) ){
		sts = Dbs_Get( fld, erm, memcnt(fld), rec );
		delete	rec;
	}
	if ( !( chk = ( sts == QRY_NML ) ) ){
		if ( sts == QRY_END ){
			Get_Dte( dte );
			memset( sdt, 0, sizeof(sdt) );
			*sdt->sal = *sal;
			if ( chk = Dbs_Trn( erm, dbs, TRN_STT ) ){
				chk = !!( sit = sav_sli( sdt, FALSE, dte, erm, dbs, mng ) );
				DBS_INT_SET(sii, &sit);
				Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
			}
		}
	}

	return( chk ? DBS_INT_VAL(sii): 0 );
}

int		Csh_Sav_Ccv(				// saving currency_conversion
Csr				rot,				// root of saveing data
Day				*dte,				// update date
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk, flc, siz=4096;
	Csr			csr=NULL;
	CRC_CVS		*ccv;
	Dbs_fld		*fld;
	WCHAR		*sql;

	if ( chk = !!( sql = (WCHAR *)malloc( wsizeof( siz ) ) ) ){
		if ( chk = Dbs_Trn( erm, dbs, TRN_STT ) ){
			while ( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
				ccv = Rec_Of(CRC_CVS, csr);
				if ( !Nul_Fld(ccv->mdt, FLD_DTM) ){
					DBS_INT_CPY(ccv->mei, mng->lgi->id );
					DBS_DTM_SET(ccv->mdt, dte );
					if ( chk = !!( flc = Fdf_Crc_Cvs( &fld, ccv, NULL, NULL, 0 ) ) ){
						chk = !!SQL_Exc_Upd( sql, siz, TBN_CRC_CVS, erm, fld, flc, dbs );
						free( fld );
					}
				}
			}
			Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
		}
		free( sql );
	}

	return( chk );
}
