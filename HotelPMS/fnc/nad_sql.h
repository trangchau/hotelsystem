/*
	Hotel property management sysytem
	Functions for booking status window definition
*/

#ifndef	nad_sqlH
#define	nad_sqlH

#include "..\..\Common\dbs_mng.h"
#include "..\..\pms_def.h"

#define	NAS_NCI				1				// No checkin
#define	NAS_NCO				2				// No checkout

typedef	struct	{							// reservation list segment
	int			rii[DBF_SIZ_INT];			// reservation_info_id
	int			rdi[DBF_SIZ_INT];			// reservation_detail_id
	int			rvn[DBF_SIZ_INT];			// reservation_number
	int			eii[DBF_SIZ_INT];			// equipment_info_id
	int			rei[DBF_SIZ_INT];			// reservation_equipment_id
	WCHAR		ern[DBF_SIZ_WST(255)];		// external reservation_number
	WCHAR		rno[DBF_SIZ_WST(255)];		// room number(equipment_code)
	WCHAR		rfn[DBF_SIZ_WST(255)];		// first_name of reservation_infos
	WCHAR		rln[DBF_SIZ_WST(255)];		// last_name of reservation_infos
	WCHAR		dfn[DBF_SIZ_WST(255)];		// first_name of reservation_details
	WCHAR		dln[DBF_SIZ_WST(255)];		// last_name of reservation_details
	Day			arv[DBF_SIZ_DAY];			// arrival_date
	Day			dpt[DBF_SIZ_DAY];			// departure_date
	int			sts;						// warning status
}NAD_WRN_LST;

typedef	struct	{							// reservation_charge information
	RSV_CRG		rcg[1];						// reservation_charges
	int			psn[DBF_SIZ_INT];			// persons of equipment_charge
	int			ups[DBF_SIZ_INT];			// persons of reservation_details
	double		prc[DBF_SIZ_DBL];			// normal price
	int			eii[DBF_SIZ_INT];			// equipment_info_id
}NAD_RCG_INF;

typedef	struct	{							// sale information
	SAL_DTL		dtl[1];						// sales_details
	PMT_DTL		pay[1];						// payment_details
	int			pup;						// payment exist
}NAD_SAL_DTL;

typedef	struct	{							// sale information
	SAL_INF		sal[1];						// sales_infos
	Lnk			rcg[1];						// erservation_charges
	Lnk			dtl[1];						// sales_details
	int			eii[DBF_SIZ_INT];			// equipment_info_id
	int			rii[DBF_SIZ_INT];			// reservation_info_id
	int			rdi[DBF_SIZ_INT];			// reservation_detail_id
	WCHAR		flg[DBF_SIZ_WST(30)];		// folio_language
}NAD_SAL_INF;

int		Nad_Wrn_Lst( Csr, WCHAR *, WCHAR *, WCHAR *, WCHAR **, CDatabase *, PMS_MNG * );	// gelling list of warning( Not checkin yet, Not chheckout yet )
int		Nad_Nsw_Exc( int, Day *, WCHAR **, CDatabase *, PMS_MNG * );						// Make reservation NoShow
int		Nad_Day_Cls( WCHAR **, CDatabase *, PMS_MNG * );									// execute day close process

#endif
