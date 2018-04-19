/*
	Hotel property management sysytem
	Functions for booking status window definition
*/

#ifndef	bst_sqlH
#define	bst_sqlH

#include "..\..\Common\dbs_mng.h"
#include "..\..\pms_def.h"

//#include "bst_fnc.h"


typedef	struct	{							// reservation list segment
	int			id[DBF_SIZ_INT];			// id of equipment_infos
	WCHAR		opc[DBF_SIZ_WST(30)];		// operator_code
	WCHAR		brc[DBF_SIZ_WST(30)];		// branch_code
	WCHAR		fcc[DBF_SIZ_WST(30)];		// facility_code
	int			knd[DBF_SIZ_INT];			// equipment_kind
	int			eti[DBF_SIZ_INT];			// equipment_type_id
	int			esi[DBF_SIZ_INT];			// equipment_state_id
	int			fbi[DBF_SIZ_INT];			// facility_building_info_id
	WCHAR		cod[DBF_SIZ_WST(255)];		// equipment_code
	int			adi1[DBF_SIZ_INT];			// additional_info1
	int			adi2[DBF_SIZ_INT];			// additional_info2
	int			adi3[DBF_SIZ_INT];			// additional_info3
	WCHAR		nam[DBF_SIZ_WST(255)];		// name
	WCHAR		abr[DBF_SIZ_WST(255)];		// abbreviation
	int			sen[DBF_SIZ_INT];			// sale_enable
	WCHAR		tpn[DBF_SIZ_WST(255)];		// room type name
	Lnk			bst[1];						// booking list of each rooms
}BST_ROM_LST;

typedef	struct	{							// Reservation status detail segment
	RSV_EQP		req[1];						// reservation equipment segment
	int			rvn[DBF_SIZ_INT];			// reservation_number
	WCHAR		ern[DBF_SIZ_WST(255)];		// extra reservation number
	int			bfr[DBF_SIZ_INT];			// booking root
	int			aii[DBF_SIZ_INT];			// agent_inf_id
	int			eci[DBF_SIZ_INT];			// Early checkin
	int			lco[DBF_SIZ_INT];			// Late checkout
	int			esi[DBF_SIZ_INT];			// equipment_states_id
	WCHAR		fnm[DBF_SIZ_WST(255)];		// first name of room user
	WCHAR		lnm[DBF_SIZ_WST(255)];		// first name of room user
	WCHAR		rno[DBF_SIZ_WST(255)];		// room number
	WCHAR		rtn[DBF_SIZ_WST(255)];		// room type name
	Day			arv[DBF_SIZ_DAY];			// arrival date of individual reservation
	Day			dpt[DBF_SIZ_DAY];			// arrival date of individual reservation
	int			cin[DBF_SIZ_INT];			// checked in( Checked in date is not Null )
	int			cot[DBF_SIZ_INT];			// Checked out ( Checked out date is not Null )
	int			cxl[DBF_SIZ_INT];			// Canceled
	BST_ROM_LST	*dtl;						// Pointer of BST_ROM_LST(reservation_detail
	Csr			fig;						// cursor of Fig_elm
	int			cut;						// copy data of cutted
}BST_ROM_STS;

typedef	struct	{							// tearing room data segment
	RSV_EQP		req[2];						// reservation_equipment1
	int			exs;						// existing tearing room
}BST_TRG_ROM;

int		Bst_Rom_Lst( Csr, WCHAR **, CDatabase *, PMS_MNG * );				// Gettimg list of room definition
int		Bst_Use_Lst( Csr, Day * ,Day *, WCHAR **, CDatabase *, PMS_MNG * );	// Getting list of room using status
int		Bst_Get_Trg( BST_TRG_ROM *, int, WCHAR **, CDatabase *, PMS_MNG * );// Getting tearing room
int		Bst_Doc_Trg( BST_TRG_ROM *, WCHAR **, CDatabase *, PMS_MNG * );		// Dccking tearing room

#endif
