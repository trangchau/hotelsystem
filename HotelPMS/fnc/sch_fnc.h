/*
	ó\ñÒåüçıèàóùä÷êî
*/

#ifndef	sch_fncH
#define	sch_fncH

#include "..\stdafx.h"
#include "..\resource.h"
//#include "..\SearchDoc.h"
//#include "..\SearchView.h"
#include "..\..\Custom\SimpleGrid.h"

#define	SHI_RSV			0			// Reservation searching
#define	SHI_CIN			1			// Checkin today searching
#define	SHI_INH			2			// Inhouse today searching
#define	SHI_COT			3			// Checkout today searching

typedef	struct	{						// Search option segment
	int			bid[DBF_SIZ_INT];		// Booking id
	WCHAR		rmn[DBF_SIZ_WST(255)];	// Room number
	WCHAR		eid[DBF_SIZ_WST(255)];	// Extra booking id
	WCHAR		nam[DBF_SIZ_WST(255)];	// Booking name
	Day			arvf[DBF_SIZ_DAY];		// Arrival date from
	Day			arvt[DBF_SIZ_DAY];		// Arrival date to
	Day			dptf[DBF_SIZ_DAY];		// Departure date from
	Day			dptt[DBF_SIZ_DAY];		// Departure date to
	int			bfr[DBF_SIZ_INT*2];		// Booking from
}SCH_OPT;

void	Mak_Sch_Pnl( CWnd *, PMS_MNG * );
void	Sch_Tab_Chg( int, CWnd * );
int		Sch_Sch_Exc( Csr, int, CWnd *, PMS_MNG * );				// Getting and displaying each data
Csr		Rsv_Get_Rot( int,  CWnd * );							// Getting data root
void	Sch_Ctx_Mnu( CWnd *, int, int, int, int );				// Open context menu on search view
void	Sch_Rnc_Viw( int, int, int, int );						// Raunch each view
void	Sch_Add_Rsv( int, int, CWnd *, CDatabase *, PMS_MNG * );// added new reservation notify from ReservationView
void	Sch_Upd_Rsv( int, int, CWnd *, CDatabase *, PMS_MNG * );// updated a reservation notify from ReservationView
void	Sch_Rch_Viw( DWORD, int );								// Launch each view
int		Sch_Mng_Dps( CWnd *, CDatabase *, PMS_MNG * );			// managemnt a deposit
void	Sch_Set_Def( CWnd *, CDatabase *, PMS_MNG * );			// setting default search option 

#endif
