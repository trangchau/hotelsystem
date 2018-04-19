/*
	Functions of night audit
*/

#ifndef	nad_fncH
#define	nad_fncH

#include "..\stdafx.h"
#include "..\resource.h"
#include "..\..\Custom\SimpleGrid.h"

#define	IDC_GRD_WRN		(8192)	// Copy destination selecting grid

#define	NAD_CLM_DAT		0		// Data keeping column

#define	NAD_CLM_STS		0		// Currency
#define	NAD_CLM_RVN		1		// Reservation number
#define	NAD_CLM_ERN		2		// External reservation number
#define	NAD_CLM_RMN		3		// room number
#define	NAD_CLM_GNM		4		// Guest name
#define	NAD_CLM_RNM		5		// Reservation name
#define	NAD_CLM_OTR		6		// Other

int		Nad_Get_Dat( CWnd *, CDatabase *, PMS_MNG * );			// getting worning data
void	Nad_Ctx_Mnu( CWnd *, int, int, int, int );				// Open context menu on search view
void	Nad_Rnc_Viw( int, int, int );							// Raunch each view
int		Nad_Mak_Nsw( int, int, CWnd *, CDatabase *, PMS_MNG * );// Make a NowShow
void	Nad_Cxl_Rsv( int, int, CWnd *, CDatabase *, PMS_MNG * );// execute a reservation cancellation
int		Nad_Day_Cls( CWnd *, CDatabase *, PMS_MNG * );			// execute day close

#endif
