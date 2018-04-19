/*
	Hotel Property Management System
	Query functions for inhouse view definition
*/

#ifndef	inh_sqlH
#define	inh_sqlH

#include "..\..\Common\dbs_mng.h"
#include "..\..\pms_def.h"

typedef	struct	{								// reservation information data segment
	RSV_INF		rsv[1];							// reservation_infos
	int			cii[DBF_SIZ_INT];				// client_info_id
	int			ckd[DBF_SIZ_INT];				// client_kind
	int			crk[DBF_SIZ_INT];				// client_rank
	WCHAR		fnm[DBF_SIZ_WST(255)];			// first_name of client_infos
	WCHAR		lnm[DBF_SIZ_WST(255)];			// first_name of client_infos
	WCHAR		adr1[DBF_SIZ_WST(255)];			// address1
	WCHAR		adr2[DBF_SIZ_WST(255)];			// address2
	WCHAR		zip[DBF_SIZ_WST(255)];			// zip_code
	WCHAR		tel1[DBF_SIZ_WST(255)];			// telephone_number1
	WCHAR		tel2[DBF_SIZ_WST(255)];			// telephone_number2
	WCHAR		fax[DBF_SIZ_WST(255)];			// fax_number
	WCHAR		eml1[DBF_SIZ_WST(255)];			// email_address1
	WCHAR		eml2[DBF_SIZ_WST(255)];			// email_address2
	int			est;							// editting status
}INH_INF_DAT;

typedef	struct	{								// reservation detail data segment
	RSV_DTL		dtl[1];							// resevation details
	int			ckd[DBF_SIZ_INT];				// client_kind
	WCHAR		fnm[DBF_SIZ_WST(255)];			// first_name of client_infos
	WCHAR		lnm[DBF_SIZ_WST(255)];			// first_name of client_infos
	int			rei[DBF_SIZ_INT];				// id of reservation_equipment
	WCHAR		rno[DBF_SIZ_WST(255)];			// equipment_code
	int			eti[DBF_SIZ_INT];				// equipment_types id(room type name)
	WCHAR		tnm[DBF_SIZ_WST(255)];			// room type name
	Day			usd[DBF_SIZ_DAY];				// use_start_date
	Day			ued[DBF_SIZ_DAY];				// use_end_date
	int			smr;							// Same as made a reservation person
}INH_DTL_DAT;


int			Inh_Get_Inf( INH_INF_DAT *, int, WCHAR **, CDatabase *, PMS_MNG * );	// Getting reservation informations for checkin
int			Inh_Get_Dtl( Csr, int, WCHAR **, CDatabase *, PMS_MNG * );				// Getting reservation details for checkin

#endif
