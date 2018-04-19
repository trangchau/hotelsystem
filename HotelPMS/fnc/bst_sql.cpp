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
#include "bst_fnc.h"
#include "bst_sql.h"
#include "fld_def.h"

int		Bst_Rom_Lst(				// 部屋一覧取得
Csr				rot,				// 格納領域
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	WCHAR		sql[512], opb[96];
	BST_ROM_LST	rom[1];
	int			pos;

	Dbs_fld		fld[] = {	{ rom->id,   _T("id"),							FLD_INT, FLD_INT_SIZ(rom->id)   },
							{ rom->opc,  _T("operator_code"),				FLD_WST, FLD_WST_SIZ(rom->opc)  },
							{ rom->brc,  _T("branch_code"),					FLD_WST, FLD_WST_SIZ(rom->brc)  },
							{ rom->fcc,  _T("facility_code"),				FLD_WST, FLD_WST_SIZ(rom->fcc)  },
							{ rom->knd,  _T("equipment_kind"),				FLD_INT, FLD_INT_SIZ(rom->knd)  },
							{ rom->eti,  _T("equipment_type_id"),			FLD_INT, FLD_INT_SIZ(rom->eti)  },
							{ rom->esi,  _T("S_id"),						FLD_INT, FLD_INT_SIZ(rom->esi)  },
							{ rom->fbi,  _T("facility_building_info_id"),	FLD_INT, FLD_INT_SIZ(rom->fbi)  },
							{ rom->cod,  _T("equipment_code"),				FLD_WST, FLD_WST_SIZ(rom->cod)  },
							{ rom->adi1, _T("additional_info1"),			FLD_INT, FLD_INT_SIZ(rom->adi1) },
							{ rom->adi2, _T("additional_info2"),			FLD_INT, FLD_INT_SIZ(rom->adi2) },
							{ rom->adi3, _T("additional_info3"),			FLD_INT, FLD_INT_SIZ(rom->adi3) },
							{ rom->nam,  _T("name"),						FLD_WST, FLD_WST_SIZ(rom->nam)  },
							{ rom->abr,  _T("abbreviation"),				FLD_WST, FLD_WST_SIZ(rom->abr)  },
							{ rom->sen,  _T("sale_enable"),					FLD_INT, FLD_INT_SIZ(rom->sen)  },
							{ rom->tpn,  _T("room_type_name"),				FLD_WST, FLD_WST_SIZ(rom->tpn)  } };


	pos = swprintf_s( sql,	memcnt(sql),
							_T("SELECT E.*, T.name as room_type_name, S.id AS S_id ")	// equipment_infos
							_T("FROM %s E ")
							_T("LEFT JOIN %s T ON T.id = E.equipment_type_id ")			// equipment_types
							_T("LEFT JOIN %s S ON S.equipment_info_id = E.id ")			// equipment_states
							_T("WHERE E.operator_code='%s' ")
							_T("AND E.deleted_date IS NULL ")
							_T("AND E.equipment_kind=%d ")
							_T("ORDER BY equipment_code "), 
							TBN_EQP_INF, TBN_EQP_TYP, TBN_EQP_STS, 
							SQL_Edt_Wst(opb, memcnt(opb), mng->opc), EQK_ROOM );

	memset( rom->bst, 0, sizeof(rom->bst) );	// Clear the booking statis list root

	return( SQL_Get_Lst( rot, sql, fld, memcnt(fld), rom, sizeof(rom), erm, dbc ) );
}

int		Bst_Use_Lst(				// 部屋使用状況一覧取得
Csr				rot,				// 格納領域
Day				*pfr,				// display period from date
Day				*pto,				// display period to date
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	WCHAR		*sql, opb[94], brb[94], fcb[94], frb[40], tob[40];
	BST_ROM_STS	use[1];
	int			chk, pos, flc, ssz=2048;
	Dbs_fld		*fld;
	Dbs_fld		ext[] = {	{ use->rvn,			_T("reservation_number"),			FLD_INT, FLD_INT_SIZ(use->rvn)	 },
							{ use->ern,			_T("external_reservation_number"),	FLD_WST, FLD_WST_SIZ(use->ern)	 },
							{ use->bfr,			_T("path_of_reservation"),			FLD_INT, FLD_INT_SIZ(use->bfr)	 },
							{ use->aii,			_T("agent_info_id"),				FLD_INT, FLD_INT_SIZ(use->aii)	 },
							{ use->eci,			_T("early_check_in"),				FLD_INT, FLD_INT_SIZ(use->eci)	 },
							{ use->lco,			_T("late_check_out"),				FLD_INT, FLD_INT_SIZ(use->lco)	 },
							{ use->esi,			_T("equipment_state_id"),			FLD_INT, FLD_INT_SIZ(use->esi)	 },
							{ use->fnm,			_T("first_name"),					FLD_WST, FLD_WST_SIZ(use->fnm)	 },
							{ use->lnm,			_T("last_name"),					FLD_WST, FLD_WST_SIZ(use->lnm)	 },
							{ use->rno,			_T("equipment_code"),				FLD_WST, FLD_WST_SIZ(use->rno)	 },
							{ use->rtn,			_T("type_name"),					FLD_WST, FLD_WST_SIZ(use->rtn)	 },
							{ use->arv,			_T("arrival_date"),					FLD_DAY, FLD_DAY_SIZ(use->arv)	 },
							{ use->dpt,			_T("departure_date"),				FLD_DAY, FLD_DAY_SIZ(use->dpt)	 },
							{ use->cin,			_T("checked_in"),					FLD_WSI, FLD_INT_SIZ(use->cin)	 },
							{ use->cot,			_T("checked_out"),					FLD_WSI, FLD_INT_SIZ(use->cot)	 },
							{ use->cxl,			_T("canceled"),						FLD_WSI, FLD_INT_SIZ(use->cxl)	 } };

	chk  = !!( sql = (WCHAR *)malloc( wsizeof(ssz) ) );
	chk &= !!( flc = Fdf_Rsv_Eqp( &fld, use->req, NULL, ext, memcnt(ext) ) );

	memset( use, 0, sizeof(use) );

	if ( chk ){
		pos = swprintf_s( sql,	ssz,
						_T("SELECT E.*, EI.equipment_code, D.early_check_in, D.late_check_out,")
						_T("D.arrival_date, D.departure_date, R.path_of_reservation, R.agent_info_id,")
						_T("R.reservation_number, R.external_reservation_number,D.canceled,")
						_T("CONVERT(D.check_in_date IS NOT NULL, signed) AS checked_in, ")
						_T("CONVERT(D.check_out_date IS NOT NULL, signed) AS checked_out, ")
						_T("C.first_name, C.last_name, ET.name AS type_name, ES.id AS equipment_state_id ")
						_T("FROM %s E ")											// reservation_equipments
						_T("LEFT JOIN %s R ON R.id = E.reservation_info_id ")		// reservation_infos
						_T("LEFT JOIN %s D ON D.id = E.reservation_detail_id ")		// reservation_details
						_T("LEFT JOIN %s EI on EI.id = E.equipment_info_id ")		// equipment_infos
						_T("LEFT JOIN %s ET on ET.id = E.equipment_type_id ")		// equipment_types
						_T("LEFT JOIN %s ES on ES.equipment_info_id = EI.id ")		// equipment_states
						_T("LEFT JOIN %s C on C.id = D.client_info_id ")			// client_infos
						_T("WHERE E.equipment_info_id IS NOT NULL ")
						_T("AND E.deleted_date IS NULL AND R.deleted_date IS NULL AND D.deleted_date IS NULL ")
						_T("AND E.use_end_date >= '%s' AND use_start_date <= '%s' " )
						_T("AND R.operator_code='%s' AND R.branch_code='%s' AND R.facility_code='%s' ")
						_T("AND R.canceled=0 AND R.no_show=0 AND D.canceled=0 ")
						_T("ORDER BY E.equipment_info_id, E.use_start_date"),
						TBN_RSV_EQP, TBN_RSV_INF, TBN_RSV_DTL, TBN_EQP_INF, TBN_EQP_TYP, TBN_EQP_STS, TBN_CLI_INF,
						Edt_Dte(frb, memcnt(frb), pfr ), Edt_Dte(tob, memcnt(tob), pto ),
						SQL_Edt_Wst(opb, memcnt(opb), mng->opc), SQL_Edt_Wst(brb,
						memcnt(brb), mng->brc), SQL_Edt_Wst(fcb, memcnt(fcb), mng->fcc) );

		chk = SQL_Get_Lst( rot, sql, fld, flc, use, sizeof(use), erm, dbc );
	}

	if ( sql )		free( sql );
	if ( fld )		free( fld );
	
	return( chk );
}

int		Bst_Get_Trg(				// Getting tearing room
BST_TRG_ROM		*dst,				// pointer of destination
int				rdi,				// reservation_info_id
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk, siz=4096, flc, idx;
	WCHAR		*sql;
	Dbs_fld		*fld;
	RSV_EQP		rec[1];
	Lnk			rot[1], *csr;

	memset( rec, 0, sizeof(rec) );
	memset( rot, 0, sizeof(rot) );

	chk  = !!(sql = (WCHAR *)malloc( wsizeof(siz) ) );
	chk &= !!(flc = Fdf_Rsv_Eqp( &fld, rec, NULL, NULL, 0 ) );
	if ( chk ){
		swprintf_s( sql, siz,
			_T("SELECT RE.* ")
			_T("FROM %s RE ")																			// reservation_equipments
			_T("LEFT JOIN %s RD ON RD.id = RE.reservation_detail_id AND RD.deleted_date IS NULL ")		// reservation_details
			_T("LEFT JOIN %s RE2 ON RE2.reservation_detail_id = RD.id AND RE2.deleted_date IS NULL ")	// reservation_equipments
			_T("WHERE RD.id=%d ")
			_T("AND RD.canceled=0 AND RE.deleted_date IS NULL ")
			_T("AND RE.equipment_info_id=RE2.equipment_info_id ")
			_T("AND ( CAST(RE.use_end_date AS date) = CAST(RE2.use_start_date as date) ")
			_T("OR CAST(RE.use_start_date AS date) = CAST(RE2.use_end_date as date) ) ")
			_T("ORDER BY RE.use_start_date "),
			TBN_RSV_EQP, TBN_RSV_DTL, TBN_RSV_EQP, rdi );
		if ( chk = SQL_Get_Lst( rot, sql, fld, flc, rec, sizeof(rec), erm, dbc ) ){
			if ( dst->exs = !!Cnt_Lnk( rot ) ){
				for ( idx=0, csr=NULL; idx<2; idx++ ){
					if ( chk = !!( csr = Nxt_Lnk( rot, csr ) ) ){
						dst->req[idx] = *Rec_Of(RSV_EQP, csr);
					}
					else{
						break;
					}
				}
			}
		}
	}
	if ( sql )	free( sql );
	if ( fld )	free( fld );

	Del_All( rot );

	return( chk );
}

int		Bst_Doc_Trg(				// Dccking tearing room
BST_TRG_ROM		*dst,				// pointer of destination
WCHAR			**erm,				// error message store pointer
CDatabase		*dbs,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	int			chk;
	WCHAR		sql[256], db1[40], db2[40];
	Day			dte[1];

	Get_Dte( dte );

	chk = Dbs_Trn( erm, dbs, TRN_STT );
	if( chk ){
		swprintf_s( sql, memcnt(sql),
			_T("UPDATE %s ")										// reservation_equipment
			_T("SET use_end_date='%s', modified_employee_id=%d,")
			_T("modified_date='%s' ")
			_T("WHERE id=%d "),
			TBN_RSV_EQP, Edt_Dte(db1, memcnt(db1), DBS_DAY_PTR(dst->req[1].ued)),
			DBS_INT_VAL(mng->lgi->id), Edt_Dte(db2, memcnt(db2), dte), DBS_INT_VAL(dst->req[0].id) );
		chk = Dbs_Upd( dbs, sql, erm ) == QRY_NML;

		if ( chk ){
			swprintf_s( sql, memcnt(sql),
				_T("UPDATE %s ")										// reservation_charges
				_T("SET reservation_equipment_id=%d, modified_employee_id=%d,")
				_T("modified_date='%s' ")
				_T("WHERE reservation_info_id=%d ")
				_T("AND reservation_detail_id=%d AND reservation_equipment_id=%d "),
				TBN_RSV_CRG, DBS_INT_VAL(dst->req[0].id),
				DBS_INT_VAL(mng->lgi->id), db2, DBS_INT_VAL(dst->req[0].rii), 
				DBS_INT_VAL(dst->req[0].rdi), DBS_INT_VAL(dst->req[1].id ) );
			chk = Dbs_Upd( dbs, sql, erm ) == QRY_NML;
		}
		if ( chk ){
			swprintf_s( sql, memcnt(sql),
				_T("UPDATE %s ")										// reservation_equipment
				_T("SET deleted_date='%s', modified_employee_id=%d,")
				_T("modified_date='%s' ")
				_T("WHERE id=%d "),
				TBN_RSV_EQP, db2, DBS_INT_VAL(mng->lgi->id), db2, DBS_INT_VAL(dst->req[1].id) );
			chk = Dbs_Upd( dbs, sql, erm ) == QRY_NML;
		}
		Dbs_Trn( erm, dbs, chk ? TRN_CMT: TRN_RBK );
	}

	return( chk );
}
