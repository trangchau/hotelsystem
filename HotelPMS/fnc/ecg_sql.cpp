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
#include "ecg_sql.h"

int		Ecg_Get_Pln(				// Getting equipment charge list
Csr				dst,				// root of storing area
int				eti,				// id of equipment_types( 0 if not order ) 
ECG_SQL_PRM		*prm,				// parameter datas for this function
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk, pos=0, eci=0;
	ECG_PLN_DAT	chg[1], *ecs, *ecd;
	Lnk			rot[1], *csr=NULL, *cse;
	WCHAR		sql[2048], ext[1025], opb[128], brb[128], fcb[128];

	Dbs_fld		fld[] = {	{ chg->eci, _T("EC_id"),				FLD_INT, FLD_INT_SIZ(chg->eci) },
							{ chg->eii, _T("EC_equipment_info_id"),	FLD_INT, FLD_INT_SIZ(chg->eii) },
							{ chg->eti, _T("EC_equipment_type_id"),	FLD_INT, FLD_INT_SIZ(chg->eti) },
							{ chg->aii,	_T("EC_agent_info_id"),		FLD_INT, FLD_INT_SIZ(chg->aii) },
							{ chg->dsi, _T("EC_description_id"),	FLD_INT, FLD_INT_SIZ(chg->dsi) },
							{ chg->cgk, _T("EC_charge_kind"),		FLD_INT, FLD_INT_SIZ(chg->cgk) },
							{ chg->cgp, _T("EC_charge_particulars"),FLD_INT, FLD_INT_SIZ(chg->cgp) },
							{ chg->psn, _T("EC_persons"),			FLD_INT, FLD_INT_SIZ(chg->psn) },
							{ chg->prc, _T("ED_price"),				FLD_DBL, FLD_DBL_SIZ(chg->prc) },
							{ chg->adk, _T("ED_apply_day_kind"),	FLD_INT, FLD_INT_SIZ(chg->adk) },
							{ chg->edi, _T("ED_id"),				FLD_INT, FLD_INT_SIZ(chg->edi) },
							{ chg->etn, _T("ET_name"),				FLD_WST, FLD_WST_SIZ(chg->etn) },
							{ chg->eqc, _T("EI_equipment_code"),	FLD_WST, FLD_WST_SIZ(chg->eqc) },
							{ chg->bfr, _T("EC_reservation_route"),	FLD_WST, FLD_WST_SIZ(chg->bfr) },
							{ chg->agn, _T("AG_name"),				FLD_WST, FLD_WST_SIZ(chg->agn) },
							{ chg->sen, _T("DS_sale_enable"),		FLD_INT, FLD_INT_SIZ(chg->sen) },
							{ chg->pln, _T("DS_name"),				FLD_WST, FLD_WST_SIZ(chg->pln) } };

	pos  =	swprintf_s( ext, memcnt(ext), _T("AND EC.reservation_route & %d "), prm->bfr );
	pos +=	swprintf_s( ext+pos, memcnt(ext)-pos, _T("AND EC.agent_info_id ") );
	pos +=	swprintf_s( ext+pos, memcnt(ext)-pos, prm->bfr==VIA_AGENT ? _T("=%d "): _T("IS NULL "), prm->agi );
	if ( !!eti )	pos +=	swprintf_s( ext+pos, memcnt(ext)-pos, _T("AND ET.id=%d "), eti );
	pos += swprintf_s( ext+pos, memcnt(ext)-pos, _T("AND ( DS.valid_sale_period_start IS NULL ") );
	if ( !!prm->std ){
		pos += swprintf_s( ext+pos, memcnt(ext)-pos, _T("OR DS.valid_sale_period_start <= '%d/%02d/%02d' ) "),
							prm->std->yer, prm->std->mth, prm->std->day );
	}
	else{
		pos += swprintf_s( ext+pos, memcnt(ext)-pos, _T(") ") );
	}
	pos += swprintf_s( ext+pos, memcnt(ext)-pos, _T("AND ( DS.valid_sale_period_end IS NULL ") );
	if ( !!prm->edd ){
		pos += swprintf_s( ext+pos, memcnt(ext)-pos, _T("OR DS.valid_sale_period_end >= '%d/%02d/%02d' ) "),
							prm->edd->yer, prm->edd->mth, prm->edd->day );
	}
	else{
		pos += swprintf_s( ext+pos, memcnt(ext)-pos, _T(") ") );
	}

	swprintf_s( sql, memcnt(sql),
							_T("SELECT EC.id AS EC_id, ED.id AS ED_id, EC.equipment_info_id AS EC_equipment_info_id, ")
							_T("EC.equipment_type_id AS EC_equipment_type_id, EC.agent_info_id AS EC_agent_info_id,")
							_T("EC.description_id AS EC_description_id, EC.charge_kind AS EC_charge_kind, ")
							_T("EC.charge_particulars AS EC_charge_particulars, EC.persons AS EC_persons,")
							_T("ED.price AS ED_price, ED.apply_day_kind AS ED_apply_day_kind,")
							_T("ET.name AS ET_name, EI.equipment_code AS EI_equipment_code,DS.sale_enable AS DS_sale_enable,")
							_T("EC.reservation_route AS EC_reservation_route, AG.name AS AG_name, DS.name AS DS_name ")
							_T("FROM %s EC ")												// equipment_charge_infos
							_T("LEFT JOIN %s EI ON EI.id = EC.equipment_info_id ")			// equipment_infos
							_T("LEFT JOIN %s ET ON ET.id = EC.equipment_type_id ")			// equipment_types
							_T("LEFT JOIN %s AG ON AG.id = EC.agent_info_id ")				// agent_infos
							_T("LEFT JOIN %s DS ON DS.id = EC.description_id ")				// descriptions
							_T("LEFT JOIN %s ED ON ED.equipment_charge_info_id = EC.id ")	// equipment_charge_details
							_T("WHERE EC.operator_code='%s' ")
							_T("AND EC.branch_code='%s' ")
							_T("AND EC.facility_code='%s' ")
							_T("AND DS.sale_enable = 1 %s" )
							_T("AND EC.deleted_date IS NULL AND ED.deleted_date IS NULL ")
							_T("ORDER BY EC.id, EC.reservation_route, EC.agent_info_id, ")
							_T("EC.equipment_type_id,DS.valid_sale_period_start "), 
							TBN_EQP_CRG, TBN_EQP_INF, TBN_EQP_TYP, TBN_AGT_INF, TBN_DSC_RPT, TBN_EQP_CGD,
							SQL_Edt_Wst( opb, memcnt(opb), prm->opc ),
							SQL_Edt_Wst( brb, memcnt(brb), prm->brc ), SQL_Edt_Wst( fcb, memcnt(fcb), prm->fcc ), ext );

	memset( chg, 0, sizeof(chg) ); 
	memset( rot, 0, sizeof(rot) ); 
	if ( chk = SQL_Get_Lst( rot, sql, fld, memcnt(fld), chg, sizeof(chg), erm, dbc ) ){
		while( !!chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
			ecs = Rec_Of(ECG_PLN_DAT, csr);
			if ( eci != DBS_INT_VAL(ecs->eci) ){
				eci = DBS_INT_VAL(ecs->eci);
				if ( chk = !!( cse=Add_Lnk( dst, NULL, sizeof(ECG_PLN_DAT) ) ) ){
					ecd = Rec_Of(ECG_PLN_DAT, cse);
					*ecd = *ecs;
				}
			}
			if ( chk ){
				if ( chk = !!( cse = Add_Lnk( ecd->dtl, NULL, sizeof(ECG_DTL_DAT) ) ) ){
					DBS_INT_CPY(Rec_Of(ECG_DTL_DAT,cse)->adk, ecs->adk); 
					DBS_INT_CPY(Rec_Of(ECG_DTL_DAT,cse)->edi, ecs->edi); 
					DBS_DBL_CPY(Rec_Of(ECG_DTL_DAT,cse)->prc, ecs->prc); 
				}
			}
		}
	}
	Del_All( rot );
	return( chk );
}

int		Ecg_Avl_Eqt(			// Getting avalable room type on booking path
Csr				rot,			// root of storing data
ECG_SQL_PRM		*prm,			// parameter datas for this function
int				pst,			// past data
WCHAR			**erm,			// error message store pointer
CDatabase		*dbc,			// Database ODBC object
PMS_MNG			*mng )			// HotelPMS system management segment
{
	int			chk, pos;
	ECG_AVL_EQT	eqt[1];
	WCHAR		sql[1024], opb[128], brb[128], fcb[128], bfb[128], dbf[192];

	Dbs_fld		fld[] = {	{ eqt->eti, _T("EC_equipment_type_id"),	FLD_INT, FLD_INT_SIZ(eqt->eti) },
							{ eqt->agi,	_T("EC_agent_info_id"),		FLD_INT, FLD_INT_SIZ(eqt->agi) },
							{ eqt->etn, _T("ET_name"),				FLD_WST, FLD_WST_SIZ(eqt->etn) },
							{ eqt->bfr, _T("EC_reservation_route"),	FLD_WST, FLD_WST_SIZ(eqt->bfr) },
							{ eqt->agn, _T("AG_name"),				FLD_WST, FLD_WST_SIZ(eqt->agn) } };

	// Date period condition make
	pos = swprintf_s( dbf, memcnt(dbf), _T("AND ( DS.valid_sale_period_start IS NULL ") );
	if ( !!prm->std ){
		pos += swprintf_s( dbf+pos, memcnt(dbf)-pos, _T("OR DS.valid_sale_period_start <= '%d/%02d/%02d' ) "),
							prm->std->yer, prm->std->mth, prm->std->day );
	}
	else{
		pos += swprintf_s( dbf+pos, memcnt(dbf)-pos, _T(") ") );
	}
	pos += swprintf_s( dbf+pos, memcnt(dbf)-pos, _T("AND ( DS.valid_sale_period_end IS NULL ") );
	if ( !!prm->edd ){
		pos += swprintf_s( dbf+pos, memcnt(dbf)-pos, _T("OR DS.valid_sale_period_end >= '%d/%02d/%02d' ) "),
							prm->edd->yer, prm->edd->mth, prm->edd->day );
	}
	else{
		pos += swprintf_s( dbf+pos, memcnt(dbf)-pos, _T(") ") );
	}


	// agent_info_id is null when reservation_route is not VIA_AGENT
	//if ( !!prm->pth ){
		swprintf_s( bfb, memcnt(bfb), prm->bfr==VIA_AGENT ? _T("=%d"): _T(" IS NULL"), prm->agi );
	//}

	swprintf_s( sql, memcnt(sql),
							_T("SELECT distinct EC.equipment_type_id AS EC_equipment_type_id, ")
							_T("EC.agent_info_id AS EC_agent_info_id, EC.reservation_route AS EC_reservation_route,")
							_T("AG.name AS AG_name, ET.name AS ET_name ")
							_T("FROM %s EC ")										// equipment_charge_infos
							_T("LEFT JOIN %s ET ON ET.id = EC.equipment_type_id ")	// equipment_types
							_T("LEFT JOIN %s DS ON DS.id = EC.description_id ")		// descriptions
							_T("LEFT JOIN %s AG ON AG.id = EC.agent_info_id ")		// agent_infos
							_T("WHERE EC.operator_code='%s' ")
							_T("AND EC.branch_code='%s' ")
							_T("AND EC.facility_code='%s' ")
							_T("%s ")
							_T("AND EC.reservation_route & %d ")
							_T("AND EC.agent_info_id %s %s ")
							_T("GROUP BY EC.equipment_type_id, EC.agent_info_id, EC.reservation_route ")
							_T("ORDER BY EC.reservation_route, EC.agent_info_id, EC.equipment_type_id "),
							TBN_EQP_CRG, TBN_EQP_TYP, TBN_DSC_RPT, TBN_AGT_INF,
							SQL_Edt_Wst( opb, memcnt(opb), prm->opc ), SQL_Edt_Wst( brb, memcnt(brb), prm->brc ),
							SQL_Edt_Wst( fcb, memcnt(fcb), prm->fcc ), dbf, prm->bfr, bfb, 
							pst ? _T(""): _T("AND DS.sale_enable=1 AND EC.deleted_date IS NULL ") );
	memset( eqt, 0, sizeof(eqt) ); 
	chk = SQL_Get_Lst( rot, sql, fld, memcnt(fld), eqt, sizeof(eqt), erm, dbc );
	return( chk );
}

int		Ecg_Apl_Dat(			// Getting apply from description
Csr				rot,			// root of storing data
ECG_SQL_PRM		*prm,			// parameter datas for this function
WCHAR			**erm,			// error message store pointer
CDatabase		*dbc,			// Database ODBC object
PMS_MNG			*mng )			// HotelPMS system management segment
{
	int			chk, pos;
	ECG_APL_DAT	apl[1];
	WCHAR		sql[512], dbf[218], opb[96], brb[96], fcb[96], sdb[20], edb[20];

	Dbs_fld		fld[] = {	{ apl->dsi, _T("id"),						FLD_INT, FLD_INT_SIZ(apl->dsi) },
							{ apl->nam,	_T("name"),						FLD_WST, FLD_WST_SIZ(apl->nam) },
							{ apl->vss, _T("valid_sale_period_start"),	FLD_DAY, FLD_DAY_SIZ(apl->vss) },
							{ apl->vse, _T("valid_sale_period_end"),	FLD_DAY, FLD_DAY_SIZ(apl->vse) },
							{ apl->atr, _T("tax_attribution"),			FLD_INT, FLD_INT_SIZ(apl->atr) },
							{ apl->pkd, _T("price_kind"),				FLD_INT, FLD_INT_SIZ(apl->pkd) },
							{ apl->prc, _T("price"),					FLD_DBL, FLD_DBL_SIZ(apl->prc) } };

	pos  = swprintf_s( dbf, memcnt(dbf), _T("AND ( valid_sale_period_start IS NULL ") );
	if ( prm->std ){	
		pos += swprintf_s( dbf+pos, memcnt(dbf)-pos, _T("OR valid_sale_period_start<='%s' ) "), Edt_Dte( sdb,memcnt(sdb), prm->std ) );
	}
	else{
		pos += swprintf_s( dbf+pos, memcnt(dbf)-pos, _T(") ") );
	}
	pos += swprintf_s( dbf+pos, memcnt(dbf)-pos, _T("AND ( valid_sale_period_end IS NULL ") );
	if ( prm->edd ){	
		pos += swprintf_s( dbf+pos, memcnt(dbf)-pos, _T("OR valid_sale_period_end>='%s') "), Edt_Dte( edb,memcnt(edb), prm->edd ) );
	}
	else{
		pos += swprintf_s( dbf+pos, memcnt(dbf)-pos, _T(") ") );
	}

	swprintf_s( sql, memcnt(sql),
							_T("SELECT * FROM %s " )			// descriptions
							_T("WHERE operator_code='%s' ")
							_T("AND ( branch_code IS NULL OR branch_code='%s' ) ")
							_T("AND ( facility_code IS NULL OR facility_code='%s' ) ")
							_T("AND deleted_date IS NULL ")
							_T("AND tax_attribution <> 0 AND sale_enable=1 ")
							_T("%s"),
							TBN_DSC_RPT,
							SQL_Edt_Wst( opb, memcnt(opb), prm->opc ), SQL_Edt_Wst( brb, memcnt(brb), prm->brc ),
							SQL_Edt_Wst( fcb, memcnt(fcb), prm->fcc ), dbf );

	memset( apl, 0, sizeof(apl) ); 
	chk = SQL_Get_Lst( rot, sql, fld, memcnt(fld), apl, sizeof(apl), erm, dbc );

	return( chk );
}

int		Ecg_Erp_Lst(			// Getting equipment rate plan list
Csr				rot,			// root of storing data
ECG_SQL_PRM		*prm,			// parameter datas for this function
WCHAR			**erm,			// error message store pointer
CDatabase		*dbc,			// Database ODBC object
PMS_MNG			*mng )			// HotelPMS system management segment
{
	int			chk;
	WCHAR		sql[1920], opb[96], brb[96], fcb[96];
	ECG_PLN_DAT	epl[1];
	Dbs_fld		fld[] = {	{ epl->eci, _T("EC_id"),						FLD_INT, FLD_INT_SIZ(epl->eci) },
							{ epl->eii, _T("EC_equipment_info_id"),			FLD_INT, FLD_INT_SIZ(epl->eii) },
							{ epl->eti, _T("EC_equipment_type_id"),			FLD_INT, FLD_INT_SIZ(epl->eti) },
							{ epl->aii, _T("EC_agent_info_id"),				FLD_INT, FLD_INT_SIZ(epl->aii) },
							{ epl->dsi, _T("EC_description_id"),			FLD_INT, FLD_INT_SIZ(epl->dsi) },
							{ epl->cgk, _T("EC_charge_kind"),				FLD_INT, FLD_INT_SIZ(epl->cgk) },
							{ epl->cgp, _T("EC_charge_particulars"),		FLD_INT, FLD_INT_SIZ(epl->cgp) },
							{ epl->psn, _T("EC_persons"),					FLD_INT, FLD_INT_SIZ(epl->psn) },
							{ epl->etn, _T("ET_name"),						FLD_WST, FLD_WST_SIZ(epl->etn) },
							{ epl->eqc, _T("EI_equipment_code"),			FLD_WST, FLD_WST_SIZ(epl->eqc) },
							{ epl->bfr, _T("EC_reservation_route"),			FLD_INT, FLD_INT_SIZ(epl->bfr) },
							{ epl->agn, _T("AG_name"),						FLD_WST, FLD_WST_SIZ(epl->agn) },
							{ epl->sen, _T("DC_sale_enable"),				FLD_INT, FLD_INT_SIZ(epl->sen) },
							{ epl->pln, _T("DC_name"),						FLD_WST, FLD_WST_SIZ(epl->pln) },
							{ epl->vss, _T("DC_valid_sale_period_start"),	FLD_DAY, FLD_DAY_SIZ(epl->vss) },
							{ epl->vse, _T("DC_valid_sale_period_end"),		FLD_DAY, FLD_DAY_SIZ(epl->vse) } };


	swprintf_s( sql, memcnt(sql),
							_T("SELECT EC.id AS EC_id, EC.equipment_info_id AS EC_equipment_info_id, ")
							_T("EC.equipment_type_id AS EC_equipment_type_id, EC.agent_info_id AS EC_agent_info_id,")
							_T("EC.description_id AS EC_description_id, EC.charge_kind AS EC_charge_kind,") 
							_T("EC.charge_particulars AS EC_charge_particulars, EC.persons AS EC_persons,")
							_T("EC.reservation_route AS EC_reservation_route, AG.name AS AG_name, ")
							_T("ET.name AS ET_name, EI.equipment_code AS EI_equipment_code,")
							_T("DC.name AS DC_name, DC.valid_sale_period_start AS DC_valid_sale_period_start, ")
							_T("DC.sale_enable AS DC_sale_enable, DC.valid_sale_period_end AS DC_valid_sale_period_end ")  
							_T("FROM %s EC ")										// equipment_charge_infos
							_T("LEFT JOIN %s EI ON EI.id = EC.equipment_info_id ")	// equipment_infos
							_T("LEFT JOIN %s ET ON ET.id = EC.equipment_type_id ")	// equipment_types
							_T("LEFT JOIN %s AG ON AG.id = EC.agent_info_id ")		// agent_infos
							_T("LEFT JOIN %s DC ON DC.id = EC.description_id ")		// descriptions
							_T("WHERE EC.operator_code='%s' AND EC.branch_code='%s' AND EC.facility_code='%s' ")
							//_T("AND DC.sale_enable = 1 ")	//2018/2/4 because need a data that is no sale on configuration screen 
							_T("AND ET.id=%d AND EC.deleted_date IS NULL ")
							_T("ORDER BY EC.reservation_route, ")
							_T("EC.agent_info_id, EC.equipment_type_id, DC.valid_sale_period_start DESC"),
							TBN_EQP_CRG, TBN_EQP_INF, TBN_EQP_TYP, TBN_AGT_INF, TBN_DSC_RPT,
							SQL_Edt_Wst( opb, memcnt(opb), prm->opc ), SQL_Edt_Wst( brb, memcnt(brb), prm->brc ),
							SQL_Edt_Wst( fcb, memcnt(fcb), prm->fcc ), prm->eti );
	memset( epl, 0, sizeof(epl) ); 
	chk = SQL_Get_Lst( rot, sql, fld, memcnt(fld), epl, sizeof(epl), erm, dbc );

	return( chk );
}

int		Ecg_Erp_Dtl(			// Getting equipment rate plan detail
ECG_PLN_DTL		*dst,			// storing data pointer
ECG_SQL_PRM		*prm,			// parameter datas for this function
WCHAR			**erm,			// error message store pointer
CDatabase		*dbc,			// Database ODBC object
PMS_MNG			*mng )			// HotelPMS system management segment
{
	WCHAR		*sql, *cgs, *cds, *dcs;
	int			chk, pos, siz=4096;
	Mlt_dbf		mdf[3];
	Lnk			rot[1], *csr=NULL;
	ECG_PLN_DTL	*pld;

	chk  = !!(sql = (WCHAR *)malloc( wsizeof(siz) ) ); 
	chk &= !!(mdf[0].flc = Fdf_Eqp_Crg( &mdf[0].fld, dst->crg, _T("EC_"), NULL, 0 ));
	chk &= !!(mdf[1].flc = Fdf_Dsc_Rpt( &mdf[1].fld, dst->dsc, _T("DC_"), NULL, 0 ));
	chk &= !!(mdf[2].flc = Fdf_Eqp_Cgd( &mdf[2].fld, dst->cgd, _T("ED_"), NULL, 0 ));
	chk &= !!Fdf_Mak_Sll( &cgs, FDF_EQP_CRG, _T("EC"), _T("EC_") );
	chk &= !!Fdf_Mak_Sll( &cds, FDF_DSC_RPT, _T("DC"), _T("DC_") );
	chk &= !!Fdf_Mak_Sll( &dcs, FDF_EQP_CGD, _T("ED"), _T("ED_") );

	if ( chk ){
		pos = swprintf_s( sql,	siz,
						_T("SELECT %s, %s, %s  ")
						_T("FROM %s EC ")			// equipment_charge_infos
						_T("LEFT JOIN equipment_charge_details ED ON ED.equipment_charge_info_id = EC.id ")
						_T("LEFT JOIN descriptions DC ON DC.id = EC.description_id ")
						_T("WHERE EC.id = %d AND ED.deleted_date IS NULL "), cgs, cds, dcs, TBN_EQP_CRG, prm->eci );

		memset( dst, 0, sizeof(ECG_PLN_DTL) );
		memset( rot, 0, sizeof(rot) );
		if ( chk = SQL_Get_MLs( rot, sql, mdf, memcnt(mdf), dst, sizeof(ECG_PLN_DTL), erm, dbc ) ){
			while ( csr = Nxt_Lnk( rot, csr ) ){
				pld = Rec_Of(ECG_PLN_DTL, csr);
				if ( !( chk = !!Add_Lnk( dst->dtl, pld->cgd, sizeof(EQP_CGD) ) ) ){
					Del_All( dst->dtl );
					break;
				}
			}
		}
		Del_All( rot );
	}

	if ( sql )			free( sql );
	if ( mdf[0].fld )	free( mdf[0].fld );
	if ( mdf[1].fld )	free( mdf[1].fld );
	if ( mdf[2].fld )	free( mdf[2].fld );
	if ( cgs )			free( cgs );
	if ( cds )			free( cds );
	if ( dcs )			free( dcs );

	return( chk );
}

int		Ecg_Erp_Del(			// Delete equipment rate plan detail
ECG_PLN_DTL		*pln,			// target data pointer
Day				*dte,			// Insert/Update date
WCHAR			**erm,			// error message store pointer
CDatabase		*dbs,			// Database ODBC object
PMS_MNG			*mng )			// HotelPMS system management segment
{
	WCHAR		sql[256], dbf[40];
	int			chk, idx;
	struct	{
		WCHAR		*tbn;
		WCHAR		*itn;
		int			*val;
	}Tbl_[] = {
		{ TBN_EQP_CRG, _T("id"),						DBS_INT_PTR(pln->crg->id) },
		{ TBN_DSC_RPT, _T("id"),						DBS_INT_PTR(pln->dsc->id) },
		{ TBN_EQP_CGD, _T("equipment_charge_info_id"),	DBS_INT_PTR(pln->crg->id) }
	};

	if ( chk = Dbs_Trn( erm, dbs, TRN_STT ) ){		// Start transaction
		for ( idx=0; chk && idx<memcnt(Tbl_); idx++ ){
			swprintf_s( sql, memcnt(sql),
					_T("UPDATE %s SET deleted_date='%s', modified_employee_id=%d ")
					_T("WHERE %s=%d "), (Tbl_+idx)->tbn, Edt_Dtm(dbf, memcnt(dbf), dte),
					DBS_INT_VAL(mng->lgi->id), (Tbl_+idx)->itn, *((Tbl_+idx)->val) );
			chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
		}
		Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
	}
	return( chk );
}

int		Ecg_Erp_Sav(			// Update equipment rate plan detail
ECG_PLN_DTL		*pln,			// target data pointer
int				ins,			// insert operation
Day				*dte,			// Insert/Update date
WCHAR			**erm,			// error message store pointer
CDatabase		*dbs,			// Database ODBC object
PMS_MNG			*mng )			// HotelPMS system management segment
{
	int				chk, flc, pvc=0, ssz=2048, dsi, eci, edi;
	Dbs_fld			*fld;
	EQP_CGD			*cgd, *pvs, tmp[1];
	WCHAR			*sql, *sel, dbf[40];
	Csr				csr=NULL, cse=NULL;
	QRY_UPD_FNC		fnc = ins ? SQL_Exc_Ins: SQL_Exc_Upd;
	Lnk				rot[1];

	chk = Dbs_Trn( erm, dbs, TRN_STT );		// Start transaction

	memset( rot, 0, sizeof(rot) );

	if ( chk = !!( sql=(WCHAR *)malloc( wsizeof(ssz) ) ) ){
		if ( chk = !!( flc = Fdf_Dsc_Rpt( &fld, pln->dsc, NULL, NULL, 0 ) ) ){
			chk = ( dsi = fnc( sql, ssz, TBN_DSC_RPT, erm, fld, flc, dbs ) ) != 0;
			free( fld );
			if ( chk ){
				if ( ins ){
					Set_Nul(pln->dsc->id, FLD_INT, FALSE);
					*DBS_INT_PTR(pln->dsc->id) = dsi;
					Set_Nul(pln->crg->dsi, FLD_INT, FALSE);
					*DBS_INT_PTR(pln->crg->dsi) = dsi;
				}
				if ( chk = !!( flc = Fdf_Eqp_Crg( &fld, pln->crg, NULL, NULL, 0 ) ) ){
					chk = ( eci = fnc( sql, ssz, TBN_EQP_CRG, erm, fld, flc, dbs ) ) != 0;
					eci = ins ? eci: DBS_INT_VAL(pln->crg->id);
					free( fld );
				}
			}
			if ( chk ){
				if ( ins ){
					Set_Nul(pln->crg->id, FLD_INT, FALSE);
					*DBS_INT_PTR(pln->crg->id) = eci;
				}
				// At first, set date to deleted_date on equipment_charge_details( cast('yyyy/mm/dd hh:mm:ss' as datetime) )
				swprintf_s( sql, ssz,
					_T("UPDATE %s ")					// equipment_charge_details
					_T("SET deleted_date='%s', modified_employee_id=%d ")
					_T(" WHERE equipment_charge_info_id=%d"),
					TBN_EQP_CGD, Edt_Dtm( dbf, memcnt(dbf), dte ), DBS_INT_VAL(mng->lgi->id), eci );
				chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;

				// Collecting all equipment_charge_details
				if ( chk ){
					memset( tmp, 0, sizeof(tmp) );
					chk  = !!( flc = Fdf_Eqp_Cgd( &fld, tmp, NULL, NULL, 0 ) );
					chk &= !!( Fdf_Mak_Sll( &sel, FDF_EQP_CGD, NULL, NULL ) );
					if ( chk ){
						swprintf_s( sql, ssz,
							_T("SELECT %s  ")
							_T("FROM %s ")			// equipment_charge_details
							_T("WHERE equipment_charge_info_id = %d "), sel, TBN_EQP_CGD, eci );
						chk = SQL_Get_Lst( rot, sql, fld, flc, tmp, sizeof(tmp), erm, dbs );
					}
					if ( fld )		free( fld );
					if ( sel )		free( sel );
				}

				while( chk && !!( csr=Nxt_Lnk( pln->dtl, csr ) ) ){
					cgd = Rec_Of(EQP_CGD, csr);
					Set_Nul(cgd->mei, FLD_INT, FALSE );
					DBS_INT_CPY(cgd->mei, mng->lgi->id);
					cse = pvc < Cnt_Lnk( rot ) ? Nxt_Lnk( rot, cse ): NULL;
					if ( cse ){				// reuse previous record
						pvs = Rec_Of(EQP_CGD, cse);
						DBS_INT_CPY(cgd->id, pvs->id);
						DBS_INT_CPY(cgd->eci, pvs->eci);
						DBS_DTM_CPY(cgd->cdt, pvs->cdt);
						Set_Nul(cgd->mdt, FLD_DTM, FALSE );
						*DBS_DTM_PTR(cgd->mdt) = *dte;
						Set_Nul(cgd->ddt, FLD_DTM, TRUE );
						pvc++;
					}
					else{
						Set_Nul(cgd->eci, FLD_INT, FALSE);
						*DBS_INT_PTR(cgd->eci) = eci;
						Set_Nul(cgd->cdt, FLD_DTM, FALSE );
						*DBS_DTM_PTR(cgd->cdt) = *dte;
					}
					if ( chk = !!( flc = Fdf_Eqp_Cgd( &fld, cgd, NULL, NULL, 0 ) ) ){
						fnc = cse ? SQL_Exc_Upd: SQL_Exc_Ins;
						if ( chk = ( edi = fnc( sql, ssz, TBN_EQP_CGD, erm, fld, flc, dbs ) != 0 ) ){
							if ( !cse ){
								Set_Nul(cgd->id, FLD_INT, FALSE);
								*DBS_INT_PTR(cgd->id) = edi;
							}
						}
						free( fld );
					}
				}
			}

			Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );		// Start transaction
		}
		free( sql );
	}
	Del_All( rot );

	return( chk );
}


int		Ecg_Get_Cat(			// getting category code
WCHAR			*dst,			// category name storiing area
int				siz,			// category name storiing area size
int				fii,			// facility_infos_id
int				dsk,			// desctiption_kind
WCHAR			*opc,			// operatoe_code
WCHAR			*brc,			// branch_code
WCHAR			*fcc,			// facility_code
WCHAR			**erm,			// error message store pointer
CDatabase		*dbc,			// Database ODBC object
PMS_MNG			*mng )			// HotelPMS system management segment
{
	int			chk, cti[DBF_SIZ_INT];
	WCHAR		sql[512], opb[96], brb[96], fcb[96], nam[DBF_SIZ_WST(255)];
	CRecordset	*rec;
	Dbs_fld		fld[] ={	{ cti, _T("description_category_id"),	FLD_INT, FLD_INT_SIZ(cti) },
							{ nam, _T("name"),						FLD_WST, FLD_WST_SIZ(nam) }};

	swprintf_s( sql, memcnt(sql),
				_T("SELECT DISTINCT D.description_category_id, DC.name ")
				_T("FROM %s D ")		// descriptions
				_T("LEFT JOIN description_categories DC ON DC.id = description_category_id ")
				_T("WHERE D.operator_code='%s' and D.branch_code='%s' AND D.facility_code='%s' ")
				_T("AND D.transfer_origin_facility_id=%d AND D.description_kind=%d "), 
				TBN_DSC_RPT,
				SQL_Edt_Wst( opb, memcnt(opb), opc ), SQL_Edt_Wst( brb, memcnt(brb), brc ),
				SQL_Edt_Wst( fcb, memcnt(fcb), fcc ), fii, dsk );

	rec = Dbs_Qry( dbc, sql, erm );

	if ( chk = !!rec ){
		if ( *erm )	free( *erm );
		*erm = NULL;
		chk = Dbs_Get( fld, erm, memcnt(fld), rec )==QRY_NML;
		if ( chk ){
			wcscpy_s( dst, siz, DBS_WST_PTR(nam) );
		}
		Dbs_Cls(rec);
	}

	return( chk ? DBS_INT_VAL(cti): 0 );		// err=-1, nothing=0, normal=category_code
}

int		Ecg_Cpy_Pln(			// copying rate plan
Csr				dst,			// desitination list
Csr				src,			// source plan list
WCHAR			**erm,			// error message store pointer
CDatabase		*dbs,			// Database ODBC object
PMS_MNG			*mng )			// HotelPMS system management segment
{
	WCHAR		sql[1024], dbf[40];
	ECG_PLN_DAT	*dat;
	Csr			csr=NULL, cse=NULL;
	int			chk, bkp, aii, eci[DBF_SIZ_INT];
	Day			dte[1];
	Dbs_fld		fld[1] = {	eci, _T("new_id"), FLD_INT, FLD_INT_SIZ(eci) };
	CRecordset	*rec;

	Get_Dte( dte );
	
	chk = Dbs_Trn( erm, dbs, TRN_STT );

	while( chk && !!( csr = Nxt_Lnk( src, csr ) ) ){
		dat = Rec_Of(ECG_PLN_DAT, csr);
		while( chk && !!( cse = Nxt_Lnk( dst, cse ) ) ){
			bkp = *Rec_Of(int, cse);
			aii = *(Rec_Of(int, cse)+1);

			swprintf_s( sql, memcnt(sql),
					_T("INSERT INTO %s ( ")				// descriptions
					_T("operator_code,branch_code,facility_code,description_code,description_kind,")
					_T("description_category_id,name,abbreviation,valid_sale_period_start,valid_sale_period_end,")
					_T("price_kind,price,tax_application,tax_attribution,sale_enable,readable_code,")
					_T("transfer_origin_facility_id,")
					_T("modified_employee_id,created_date,modified_date,deleted_date )")
					_T("SELECT ")
					_T("operator_code,branch_code,facility_code,description_code,description_kind,")
					_T("description_category_id,name,abbreviation,valid_sale_period_start,valid_sale_period_end,")
					_T("price_kind,price,tax_application,tax_attribution,sale_enable,readable_code,")
					_T("transfer_origin_facility_id,")
					_T("%d, '%s', NULL, NULL ")
					_T("FROM %s WHERE id=%d AND deleted_date IS NULL"),		// descriptions
					TBN_DSC_RPT,
					DBS_INT_VAL(mng->lgi->id), Edt_Dte( dbf, memcnt(dbf), dte ),
					TBN_DSC_RPT, DBS_INT_VAL(dat->dsi), DBS_INT_VAL(dat->eci) );
			chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
			if ( chk ){
				swprintf_s( sql, memcnt(sql),
					_T("INSERT INTO %s ( ")								// equipment_charge_infos
					_T("operator_code, branch_code, facility_code, equipment_type_id, equipment_info_id, ")
					_T("agent_info_id, description_id, charge_kind, charge_particulars, reservation_route,")
					_T("persons, CM_rooms_unit, CM_minstay, CM_maxstay, ")
					_T("modified_employee_id, created_date, modified_date, deleted_date ) ")
					_T("SELECT ")
					_T("operator_code, branch_code, facility_code, equipment_type_id, equipment_info_id, ")
					_T("%d, description_id, charge_kind, charge_particulars, %d,")
					_T("persons, CM_rooms_unit, CM_minstay, CM_maxstay, ")
					_T("%d, '%s', NULL, NULL ")
					_T("FROM %s WHERE id=%d AND deleted_date IS NULL"),	// equipment_charge_infos 
					TBN_EQP_CRG, aii, bkp, DBS_INT_VAL(mng->lgi->id), Edt_Dte( dbf, memcnt(dbf), dte ),
					TBN_EQP_CRG, DBS_INT_VAL(dat->eci) );
				chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
			}
			if ( chk ){
				rec = Dbs_Qry( dbs, _T("SELECT MAX(id) as new_id from equipment_charge_infos"), erm );
				if ( chk = !!rec ){
					if ( *erm )		free( *erm );
					*erm = NULL;
					chk = Dbs_Get( fld, erm, 1, rec )==QRY_NML;
					Dbs_Cls(rec);
				}
			}
			if ( chk ){
				swprintf_s( sql, memcnt(sql),
					_T("INSERT INTO %s ( ")													// equipment_charge_details
					_T("operator_code, branch_code, facility_code, equipment_charge_info_id, price,")
					_T("apply_day_kind, ")
					_T("modified_employee_id, created_date, modified_date, deleted_date ) ")
					_T("SELECT ")
					_T("operator_code, branch_code, facility_code, %d, price,")
					_T("apply_day_kind, ")
					_T("%d, '%s', NULL, NULL ")
					_T("FROM %s WHERE equipment_charge_info_id=%d AND deleted_date IS NULL"),// equipment_charge_details
					TBN_EQP_CGD,
					DBS_INT_VAL(eci), DBS_INT_VAL(mng->lgi->id), Edt_Dte( dbf, memcnt(dbf), dte ), TBN_EQP_CGD, DBS_INT_VAL(dat->eci) );
				chk = Dbs_Upd( dbs, sql, erm )==QRY_NML;
			}
		}
	}

	Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );

	return( chk );
}

int		Ecg_Cst_Lst(			// Getting description_category list
Csr				rot,			// root of storing data
ECG_SQL_PRM		*prm,			// parameter datas for this function
WCHAR			**erm,			// error message store pointer
CDatabase		*dbc,			// Database ODBC object
PMS_MNG			*mng )			// HotelPMS system management segment
{
	WCHAR		*sql, *cts, opb[96], brb[96], fcb[96];
	DSC_CAT		cat[1];
	int			chk, flc, pos, siz=4096;
	Dbs_fld		*fld;

	chk  = !!(sql = (WCHAR *)malloc( wsizeof(siz) ) ); 
	chk &= !!(flc = Fdf_Dsc_Cat( &fld, cat, NULL, NULL, 0 ));
	chk &= !!Fdf_Mak_Sll( &cts, FDF_DSC_CAT, NULL, NULL );

	if ( chk ){
		pos = swprintf_s( sql,	siz,
				_T("SELECT %s ")
				_T("FROM %s ")				// description_categories
				_T("WHERE operator_code='%s' and ( branch_code IS NULL OR branch_code='%s' ) ")
				_T("AND ( facility_code IS NULL OR facility_code='%s' ) ")
				_T("AND deleted_date IS NULL "),
				cts, TBN_DSC_CAT, 
				SQL_Edt_Wst( opb, memcnt(opb), prm->opc ), 
				SQL_Edt_Wst( brb, memcnt(brb), prm->brc ),
				SQL_Edt_Wst( fcb, memcnt(fcb), prm->fcc ) );

		memset( cat, 0, sizeof(cat) );

		chk = SQL_Get_Lst( rot, sql, fld, flc, cat, sizeof(cat), erm, dbc );
	}

	if ( sql )	free( sql );
	if ( fld )	free( fld );
	if ( cts )	free( cts );

	return( chk );
}

int		Ecg_Chg_Crg(			// confirm chainging equipment_charge
Csr				dst,			// root of destination
ECG_SQL_PRM		*prm,			// parameter datas for this function
WCHAR			**erm,			// error message store pointer
CDatabase		*dbc,			// Database ODBC object
PMS_MNG			*mng )			// HotelPMS system management segment
{
	WCHAR		sql[1280], agb[20], eib[20], opb[64], brb[64], fcb[64], sdb[20], edb[20];
	int			chk, eci=0;
	ECG_PLN_DAT	pln[1], *ecs, *ecd;
	Lnk			rot[1], *csr=NULL, *cse;

	Dbs_fld		fld[] = {	{ pln->eci, _T("EC_id"),				FLD_INT, FLD_INT_SIZ(pln->eci) },
							{ pln->eii, _T("equipment_info_id"),	FLD_INT, FLD_INT_SIZ(pln->eii) },
							{ pln->aii, _T("agent_info_id"),		FLD_INT, FLD_INT_SIZ(pln->aii) },
							{ pln->dsi, _T("description_id"),		FLD_INT, FLD_INT_SIZ(pln->dsi) },
							{ pln->cgk, _T("charge_kind"),			FLD_INT, FLD_INT_SIZ(pln->cgk) },
							{ pln->cgp, _T("charge_particulars"),	FLD_INT, FLD_INT_SIZ(pln->cgp) },
							{ pln->edi, _T("ED_id"),				FLD_INT, FLD_INT_SIZ(pln->edi) },
							{ pln->psn, _T("persons"),				FLD_INT, FLD_INT_SIZ(pln->psn) },
							{ pln->prc, _T("price"),				FLD_DBL, FLD_DBL_SIZ(pln->prc) },
							{ pln->adk, _T("apply_day_kind"),		FLD_INT, FLD_INT_SIZ(pln->adk) },
							{ pln->bfr, _T("reservation_route"),	FLD_INT, FLD_INT_SIZ(pln->bfr) },
							{ pln->agn, _T("agent_name"),			FLD_WST, FLD_WST_SIZ(pln->agn) },
							{ pln->pln, _T("plan_name"),			FLD_WST, FLD_WST_SIZ(pln->pln) } };

	swprintf_s( agb, memcnt(agb), !prm->agi ? _T(" IS NULL "): _T("=%d"), prm->agi );
	swprintf_s( eib, memcnt(eib), !prm->eii ? _T(" IS NULL "): _T("=%d"), prm->eii );

	chk = swprintf_s( sql, memcnt(sql),
		_T("SELECT EC.id AS EC_id, EC.equipment_info_id, EC.agent_info_id, EC.description_id,")
		_T("EC.charge_kind, EC.charge_particulars, ED.id AS ED_id,")			
		_T("EC.persons, ED.price, ED.apply_day_kind, EC.reservation_route, ")
		_T("AG.name AS agent_name, DC.name AS plan_name ")		
		_T("FROM %s EC ")												// equipment_charge_infos
		_T("LEFT JOIN %s DC ON DC.id = EC.description_id ")				// descriptions
		_T("LEFT JOIN %s ED ON ED.equipment_charge_info_id = EC.id ")	// equipment_charge_details
		_T("LEFT JOIN %s AG ON AG.id = EC.agent_info_id ")				// agent_infos
		_T("WHERE EC.operator_code='%s' AND EC.branch_code='%s' AND EC.facility_code='%s' ")
		_T("AND EC.reservation_route & %d AND EC.agent_info_id %s ")
		_T("AND EC.equipment_type_id=%d AND EC.equipment_info_id %s ")
		_T("AND ( DC.valid_sale_period_start IS NULL OR DC.valid_sale_period_start <= '%s' ) ")
		_T("AND ( DC.valid_sale_period_end IS NULL OR DC.valid_sale_period_end >= '%s' ) ")
		_T("AND DC.sale_enable != 0 AND EC.deleted_date IS NULL AND DC.deleted_date IS NULL "),
		TBN_EQP_CRG, TBN_DSC_RPT,TBN_EQP_CGD, TBN_AGT_INF,
		SQL_Edt_Wst( opb, memcnt(opb), prm->opc ), SQL_Edt_Wst( brb, memcnt(brb), prm->brc ), SQL_Edt_Wst( fcb, memcnt(fcb), prm->fcc ),
		prm->bfr, agb, prm->eti, eib, Edt_Dte(sdb,memcnt(sdb),prm->std), Edt_Dte(edb,memcnt(edb),prm->edd) );


	memset( pln, 0, sizeof(pln) ); 
	memset( rot, 0, sizeof(rot) ); 
	if ( chk = SQL_Get_Lst( rot, sql, fld, memcnt(fld), pln, sizeof(pln), erm, dbc ) ){
		while( !!chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
			ecs = Rec_Of(ECG_PLN_DAT, csr);
			if ( eci != DBS_INT_VAL(ecs->eci) ){
				eci = DBS_INT_VAL(ecs->eci);
				if ( chk = !!( cse=Add_Lnk( dst, NULL, sizeof(ECG_PLN_DAT) ) ) ){
					ecd = Rec_Of(ECG_PLN_DAT, cse);
					*ecd = *ecs;
				}
			}
			if ( chk ){
				if ( chk = !!( cse = Add_Lnk( ecd->dtl, NULL, sizeof(ECG_DTL_DAT) ) ) ){
					DBS_INT_CPY(Rec_Of(ECG_DTL_DAT,cse)->edi, ecs->edi); 
					DBS_INT_CPY(Rec_Of(ECG_DTL_DAT,cse)->adk, ecs->adk); 
					DBS_INT_CPY(Rec_Of(ECG_DTL_DAT,cse)->edi, ecs->edi); 
					DBS_DBL_CPY(Rec_Of(ECG_DTL_DAT,cse)->prc, ecs->prc); 
				}
			}
		}
	}
	Del_All( rot );
	return( chk );		// err=-1, nothing=0, normal=category_code
}
