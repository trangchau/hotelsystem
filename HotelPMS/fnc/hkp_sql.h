/*
	Hotel Property Management System
	Query functions for housekeeping view definition
*/

#ifndef	hkp_sqlH
#define	hkp_sqlH

#include "..\..\Common\dbs_mng.h"
#include "..\..\pms_def.h"

typedef	struct	{								// room change information data segment
	int			rii[DBF_SIZ_INT];				// reservation_info_id
	int			rdi[DBF_SIZ_INT];				// reservation_detail_id
	int			rei1[DBF_SIZ_INT];				// reservation_equipment_id( Current )
	int			rei2[DBF_SIZ_INT];				// reservation_equipment_id( Next )
	int			eii1[DBF_SIZ_INT];				// equipment_info_id( Current )
	int			eii2[DBF_SIZ_INT];				// equipment_info_id( Next )
	int			esi1[DBF_SIZ_INT];				// equipment_state_id( Current )
	int			esi2[DBF_SIZ_INT];				// equipment_state_id( Next )
	WCHAR		rmn1[DBF_SIZ_WST(255)];			// equipment_code(room number of current )
	WCHAR		rmn2[DBF_SIZ_WST(255)];			// equipment_code(room number of next )
	WCHAR		fnm1[DBF_SIZ_WST(255)];			// first_name of client_infos ( reservation_infos )
	WCHAR		lnm1[DBF_SIZ_WST(255)];			// first_name of client_infos ( reservation_infos )
	WCHAR		fnm2[DBF_SIZ_WST(255)];			// first_name of client_infos ( reservation_details )
	WCHAR		lnm2[DBF_SIZ_WST(255)];			// first_name of client_infos ( reservation_details )
	int			rch;							// execute room change
}HKP_RCH_DAT;

typedef	struct	{								// reservation detail data segment
	EQP_STS		sts[1];							// equipment states
	WCHAR		fnm1[DBF_SIZ_WST(255)];			// first_name of client_infos
	WCHAR		lnm1[DBF_SIZ_WST(255)];			// first_name of client_infos
	WCHAR		fnm2[DBF_SIZ_WST(255)];			// first_name of client_infos
	WCHAR		lnm2[DBF_SIZ_WST(255)];			// first_name of client_infos
	int			rei[DBF_SIZ_INT];				// id of reservation_equipment
	int			eii[DBF_SIZ_INT];				// equipment_types id(room type name)
	int			eti[DBF_SIZ_INT];				// equipment_types id(room type name)
	int			esi[DBF_SIZ_INT];				// equipment_states id(room type name)
	WCHAR		rmn[DBF_SIZ_WST(255)];			// equipment_code
	WCHAR		tnm[DBF_SIZ_WST(255)];			// room type name
	Day			usd[DBF_SIZ_DAY];				// use_start_date
	Day			ued[DBF_SIZ_DAY];				// use_end_date
	int			ise[DBF_SIZ_INT];				// sale_enable on equipment_infos
	int			sse[DBF_SIZ_INT];				// sale_enable on equipment_states
}HKP_DTL_DAT;


int			Hkp_Get_Dat( Csr, WCHAR **, CDatabase *, PMS_MNG * );				// Getting room change informations for checkin
int			Hkp_Get_Rch( Csr, WCHAR **, CDatabase *, PMS_MNG * );				// Getting room change informations for checkin
int			Hkp_Exc_Rch( Csr, WCHAR **, CDatabase *, PMS_MNG * );				// execute to room change
int			Hkp_Chg_Sts( HKP_DTL_DAT *, int, WCHAR **, CDatabase *, PMS_MNG * );// Changing room satus

#endif
