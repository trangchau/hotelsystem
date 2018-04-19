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
#include "inh_fnc.h"
#include "inh_sql.h"
#include "fld_def.h"

int		Inh_Get_Inf(				// Getting reservation informations 
INH_INF_DAT		*dst,				// Pointer of storing destination
int				rid,				// reservation_infos_id
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	WCHAR		sql[512], opb[96];;
	int			chk, pos, fct;
	CRecordset	*rec;
	Dbs_fld		*fld;
	Dbs_fld		ext[] = {	{ dst->cii,			_T("client_id"),					FLD_INT, FLD_INT_SIZ(dst->cii)		},
							{ dst->ckd,			_T("client_kind"),					FLD_INT, FLD_INT_SIZ(dst->ckd)		},
							{ dst->crk,			_T("client_rank"),					FLD_INT, FLD_INT_SIZ(dst->crk)		},
							{ dst->fnm,			_T("first_name"),					FLD_WST, FLD_WST_SIZ(dst->fnm)		},
							{ dst->lnm,			_T("last_name"),					FLD_WST, FLD_WST_SIZ(dst->lnm)		},
							{ dst->adr1,		_T("address1"),						FLD_WST, FLD_WST_SIZ(dst->adr1)		},
							{ dst->adr2,		_T("address2"),						FLD_WST, FLD_WST_SIZ(dst->adr2)		},
							{ dst->zip,			_T("zip_code"),						FLD_WST, FLD_WST_SIZ(dst->zip)		},
							{ dst->tel1,		_T("telephone_number1"),			FLD_WST, FLD_WST_SIZ(dst->tel1)		},
							{ dst->tel2,		_T("telephone_number2"),			FLD_WST, FLD_WST_SIZ(dst->tel2)		},
							{ dst->fax,			_T("fax_number"),					FLD_WST, FLD_WST_SIZ(dst->fax)		},
							{ dst->eml1,		_T("email_address1"),				FLD_WST, FLD_WST_SIZ(dst->eml1)		},
							{ dst->eml2,		_T("email_address2"),				FLD_WST, FLD_WST_SIZ(dst->eml2)		} };


	if ( chk = !!(fct = Fdf_Rsv_Inf( &fld, dst->rsv, NULL, ext, memcnt(ext) )) ){
		pos = swprintf_s( sql,	memcnt(sql),
							_T("SELECT R.*, C.id AS client_id, C.client_kind, C.first_name, C.last_name,")
							_T("C.address1,C.address2,C.zip_code,C.telephone_number1,C.telephone_number2,")
							_T("C.client_rank, C.fax_number,C.email_address1,C.email_address2 ")
							_T("FROM %s R ")									// reservation_infos
							_T("LEFT JOIN %s C ON C.id = R.client_info_id ")	// client_infos
							_T("WHERE R.operator_code='%s' ")
							_T("AND R.deleted_date IS NULL AND C.deleted_date IS NULL ")
							_T("AND R.id=%d "),
							TBN_RSV_INF, TBN_CLI_INF, SQL_Edt_Wst(opb, memcnt(opb), mng->opc), rid );

		memset( dst, 0, sizeof(INH_INF_DAT) );
		rec = Dbs_Qry( dbc, sql, erm );

		if ( chk = !!rec ){
			if ( *erm )	free( *erm );
			*erm = NULL;
			chk = Dbs_Get( fld, erm, fct, rec )==QRY_NML;
			Dbs_Cls(rec);
		}
		free( fld );
	}
	return( chk );
}


int		Inh_Get_Dtl(				// Getting reservation details
Csr				rot,				// Pointer of storing destination
int				rid,				// reservation_infos_id
WCHAR			**erm,				// error message store pointer
CDatabase		*dbc,				// Database ODBC object
PMS_MNG			*mng )				// HotelPMS system management segment
{
	INH_DTL_DAT	dtl[1];
	WCHAR		sql[1024], opb[96];
	int			pos;

	Dbs_fld		fld[] = {	{ dtl->dtl->id,		_T("id"),						FLD_INT, FLD_INT_SIZ(dtl->dtl->id)},
							{ dtl->dtl->rii,	_T("reservation_info_id"),		FLD_INT, FLD_INT_SIZ(dtl->dtl->rii)},
							{ dtl->dtl->cii,	_T("client_info_id"),			FLD_INT, FLD_INT_SIZ(dtl->dtl->cii)},
							{ dtl->dtl->arv,	_T("arrival_date"),				FLD_DAY, FLD_DAY_SIZ(dtl->dtl->arv)},
							{ dtl->dtl->atm,	_T("arrival_time"),				FLD_TIM, FLD_TIM_SIZ(dtl->dtl->atm)},
							{ dtl->dtl->dpt,	_T("departure_date"),			FLD_DAY, FLD_DAY_SIZ(dtl->dtl->dpt)},
							{ dtl->dtl->dtm,	_T("departure_time"),			FLD_TIM, FLD_TIM_SIZ(dtl->dtl->dtm)},
							{ dtl->dtl->cid,	_T("check_in_date"),			FLD_DTM, FLD_DTM_SIZ(dtl->dtl->cid)},
							{ dtl->dtl->cod,	_T("check_out_date"),			FLD_DTM, FLD_DTM_SIZ(dtl->dtl->cod)},
							{ dtl->dtl->eci,	_T("early_check_in"),			FLD_INT, FLD_INT_SIZ(dtl->dtl->eci)},
							{ dtl->dtl->lco,	_T("late_check_out"),			FLD_INT, FLD_INT_SIZ(dtl->dtl->lco)},
							{ dtl->dtl->psn,	_T("person_number"),			FLD_INT, FLD_INT_SIZ(dtl->dtl->psn)},
							{ dtl->dtl->chn,	_T("children_number"),			FLD_INT, FLD_INT_SIZ(dtl->dtl->chn)},
							{ dtl->dtl->eti,	_T("equipment_type_id"),		FLD_INT, FLD_INT_SIZ(dtl->dtl->eti)},
							{ dtl->dtl->inh,	_T("inhouse"),					FLD_INT, FLD_INT_SIZ(dtl->dtl->inh)},
							{ dtl->dtl->cxl,	_T("canceled"),					FLD_INT, FLD_INT_SIZ(dtl->dtl->cxl)},
							{ dtl->dtl->nte,	_T("note"),						FLD_WST, FLD_WST_SIZ(dtl->dtl->nte)},
							{ dtl->dtl->cdt,	_T("created_date"),				FLD_DTM, FLD_DTM_SIZ(dtl->dtl->cdt)},
							{ dtl->dtl->mdt,	_T("modified_date"),			FLD_DTM, FLD_DTM_SIZ(dtl->dtl->mdt)},
							{ dtl->dtl->ddt,	_T("deleted_date"),				FLD_DTM, FLD_DTM_SIZ(dtl->dtl->ddt)},
							{ dtl->ckd,			_T("client_kind"),				FLD_INT, FLD_INT_SIZ(dtl->ckd)		},
							{ dtl->fnm,			_T("first_name"),				FLD_WST, FLD_WST_SIZ(dtl->fnm)		},
							{ dtl->lnm,			_T("last_name"),				FLD_WST, FLD_WST_SIZ(dtl->lnm)		},
							{ dtl->rei,			_T("reservation_equipment_id"),	FLD_INT, FLD_INT_SIZ(dtl->eti)		},
							{ dtl->rno,			_T("room_number"),				FLD_WST, FLD_WST_SIZ(dtl->rno)		},
							{ dtl->eti,			_T("equipment_type_id"),		FLD_INT, FLD_INT_SIZ(dtl->eti)		},
							{ dtl->tnm,			_T("room_type_name"),			FLD_WST, FLD_WST_SIZ(dtl->tnm)		},
							{ dtl->usd,			_T("use_start_date"),			FLD_DAY, FLD_DAY_SIZ(dtl->usd)		},
							{ dtl->ued,			_T("use_end_date"),				FLD_DAY, FLD_DAY_SIZ(dtl->ued)		} };

	pos = swprintf_s( sql,	memcnt(sql),
							_T("SELECT D.*, C.first_name, C.last_name, C.client_kind, ")
							_T("EI.equipment_code as room_number,  ET.name as room_type_name, ")
							_T("RE.id as reservation_equipment_id, RE.use_start_date,  RE.use_end_date ")
							_T("FROM %s D ")											// reservation_details
							_T("LEFT JOIN %s C ON C.id = D.client_info_id ")			// client_infos
							_T("LEFT JOIN %s RE ON RE.reservation_detail_id = D.id ")	// reservation_equipments
							_T("LEFT JOIN %s EI ON EI.id = RE.equipment_info_id ")		// equipment_infos
							_T("LEFT JOIN %s ET ON ET.id = RE.equipment_type_id ")		// equipment_types
							_T("WHERE D.operator_code='%s' ")
							_T("AND D.reservation_info_id=%d ")
							_T("AND D.deleted_date IS NULL AND C.deleted_date IS NULL ")
							_T("AND RE.deleted_date IS NULL AND EI.deleted_date IS NULL AND ET.deleted_date IS NULL ")
							_T("ORDER BY D.id, RE.use_start_date "),
							TBN_RSV_DTL, TBN_CLI_INF, TBN_RSV_EQP, TBN_EQP_INF, TBN_EQP_TYP,
							SQL_Edt_Wst(opb, memcnt(opb), mng->opc), rid );

	return( SQL_Get_Lst( rot, sql, fld, memcnt(fld), dtl, sizeof(dtl), erm, dbc ) );
}
