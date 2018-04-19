/*
	Functions for ReservationView 
*/

#ifndef	rsv_fncH
#define	rsv_fncH

#include "..\stdafx.h"
#include "..\resource.h"
//#include "..\ReservationDoc.h"
//#include "..\ReservationView.h"
#include "..\..\Custom\SimpleGrid.h"
#include "..\..\pms_def.h"
#include "rsv_sql.h"

#define	IDC_GRD_DTL		(8192)			// Reservation detail grid
#define	IDC_GRD_ROM		(IDC_GRD_DTL+1)	// Room assign grid

// Data status of reservation detail
#define	DTL_STS_NEW		0				// Creating
#define	DTL_STS_RSV		1				// Reservaed
#define	DTL_STS_INH		2				// Inhouse(Checked in)
#define	DTL_STS_COT		3				// Checked out
#define	DTL_STS_CXL		4				// Canceled

#define	ROM_USD_CLM		0				// Room use start date column on grid
#define	ROM_UED_CLM		1				// Room use end date column on grid
#define	ROM_TYP_CLM		2				// Room use room type column on grid
#define	ROM_NBR_CLM		3				// Room use room number column on grid

#define	RSV_EST_OLD		1				// past reservation ( All editing impossible -> departure date is less than today ) 
#define	RSV_EST_RSV		2				// reservaed and before checkin
#define	RSV_EST_INH		4				// Checked in and before checkout
#define	RSV_EST_COT		8				// Checked out  

// Create a checkin parameter
typedef	struct	{						// Checkin parameter
	int					eii;			// equipment_info_id
	int					eti;			// equipment_type_id
	Day					arv[1];			// Arrival date
	Day					dpt[1];			// Departure date
}CCI_PRM;

CSimpleGrid	*Rsv_Mak_Grd( int, RECT *, CWnd * );											// Create each grids
void		Rsv_Rom_Dsp( CWnd *, Csr, PMS_MNG * );											// Setup the room list
void		Rsv_Dsp_DGr( int, RSV_DTL_DAT *, CWnd *, PMS_MNG * );							// Displaying reservation_details grid
void		Rsv_Dtl_Dsp( CWnd *, RSV_DTL_DAT *, RSV_INF_DAT *, PMS_MNG * );					// Displaying reservation detail data
void		Rsv_Dsp_Dat( RSV_INF_DAT *, Csr, CDatabase *, CWnd *, PMS_MNG * );				// Displaying reservation_details
//void		Rsv_Tab_Chg( int, RSV_INF_DAT *, Csr, CWnd *, PMS_MNG * );						// ó\ñÒè⁄ç◊É^ÉuïœçX
void		Rsv_Sel_Clk( CWnd * );															// Client kind select event
void		Rsv_Sty_Chg( RSV_DTL_DAT *, CWnd *, PMS_MNG * );								// Stay period change event
void		Rsv_Bph_Chg( RSV_INF_DAT *, RSV_DTL_DAT *, CWnd *, CDatabase *, PMS_MNG * );	// Booking path change event
void		Rsv_Ngt_Chg( RSV_DTL_DAT *, CWnd *, PMS_MNG * );								// Change the night editbox event
void		Rsv_Rcg_Chg( RSV_DTL_DAT *, CWnd *, PMS_MNG * );								// Change the room change edit event
void		Rsv_Rom_Dte( WCHAR *, int, int, RSV_DTL_DAT *, CWnd *, PMS_MNG * );				// Room use date changed
void		Rsv_Rom_Nbr( WCHAR *, int, CDatabase *, CWnd *, PMS_MNG * );					// Room number edit complete event
int			Rsv_Cre_Rsv( CWnd *, RSV_INF_DAT *, Csr, CCI_PRM *, CDatabase *, PMS_MNG * );	// Create a new reservation
int			Rsv_Sav_Dat( RSV_INF_DAT *, Csr, CWnd *, CDatabase *, PMS_MNG * );				// Saveing data
void		Rsv_Rct_Chg( Csr, CWnd *, int, PMS_MNG * );										// Room number editbox change
void		Rsv_Rte_Pln( Csr, int, RSV_DTL_DAT *, CDatabase *, CWnd *, PMS_MNG * );			// getting and setting rate plan
void		Rsv_Dsp_Rte( Csr );																// dispose the rate plan
int			Rsv_Inf_Gfs( RSV_DTL_DAT *, Csr, CWnd *, PMS_MNG * );							// Getting reservation_details data from screen
int			Rsv_Dtl_Gfs( RSV_DTL_DAT *, CWnd *, PMS_MNG * );								// Getting reservation_details data from screen
void		Rsv_Dsp_Dtl( Csr );																// resrvation_details dispose
int			Rsv_Get_Dat( int, CWnd *, CDatabase *, PMS_MNG * );			 					// Getting the existing data

#endif
