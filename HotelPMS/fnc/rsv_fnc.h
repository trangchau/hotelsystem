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
#include "ecg_sql.h"

#define	IDC_GRD_DTL		(8192)			// Reservation detail grid
#define	IDC_GRD_ROM		(IDC_GRD_DTL+1)	// Room assign grid

#define	RSV_DAT_CLM		0				// New creation status column on grid

#define	RSV_NEW_CLM		0				// New creation status column on grid
#define	RSV_CHK_CLM		1				// Checkbox column on grid
#define	RSV_NAM_CLM		2				// reservation name column on grid
#define	RSV_STS_CLM		3				// reservation status column on grid
#define	RSV_RTP_CLM		4				// Room type column on grid
#define	RSV_RNB_CLM		5				// Room number column on grid
#define	RSV_ARV_CLM		6				// arrival date column on grid
#define	RSV_ECI_CLM		7				// early checkin column on grid
#define	RSV_DPT_CLM		8				// departure date column on grid
#define	RSV_LCO_CLM		9				// late checkout column on grid
#define	RSV_ADC_CLM		10				// adult/child number column on grid
#define	RSV_AMT_CLM		11				// Amount column on grid


#define	ROM_USD_CLM		0				// Room use start date column on grid
#define	ROM_UED_CLM		1				// Room use end date column on grid
#define	ROM_TYP_CLM		2				// Room use room type column on grid
#define	ROM_NBR_CLM		3				// Room use room number column on grid
#define	ROM_AMT_CLM		4				// Room use amount column on grid

// Create a checkin parameter
typedef	struct	{						// Checkin parameter
	int					eii;			// equipment_info_id
	int					eti;			// equipment_type_id
	int					esi;			// equipment_state_id
	Day					arv[1];			// Arrival date
	Day					dpt[1];			// Departure date
}CCI_PRM;

CSimpleGrid	*Rsv_Mak_Grd( int, RECT *, CWnd * );											// Create each grids
double		Rsv_Apl_Amt( RSV_RCG_DAT *, RSV_CRG *, Day *, ECG_PLN_DAT *, PMS_MNG * );		// amount apply by apply day kind
void		Rsv_Sel_Clk( CWnd * );															// Client kind select event
void		Rsv_Sch_Cli( RSV_DTL_DAT *, CWnd *, CDatabase *, PMS_MNG * );					// client search button click
void		Rsv_Bph_Chg( RSV_DTL_DAT *, CWnd *, CDatabase *, PMS_MNG * );					// Booking path combobox change event
void		Rsv_Rct_Chg( int, CWnd *, PMS_MNG * );											// Number of rooms editbox change
void		Rsv_Chk_All( CWnd *, PMS_MNG * );												// CheckAll checkbox click
int			Rsv_Chg_Dtl( int, CWnd *, CDatabase *, PMS_MNG * );								// detail list select change event
int			Rsv_Chk_Clk( int, CWnd *, CDatabase *, PMS_MNG * );								// cancellation checkbox check
void		Rsv_Sam_Rsv( CWnd *, CDatabase *, PMS_MNG * );									// Same as reserved person checkbox click event
//void		Rsv_Sty_Chg( RSV_DTL_DAT *, CWnd *, CDatabase *, PMS_MNG * );					// Stay period date edit change event
//void		Rsv_Ngt_Chg( int, RSV_DTL_DAT *, CWnd *, CDatabase *, PMS_MNG * );				// Change the night editbox event
void		Rsv_Rtp_Chg( RSV_DTL_DAT *, CWnd *, CDatabase *, PMS_MNG * );					// getting and setting rate plan
void		Rsv_Sel_Pln( RSV_DTL_DAT *, CWnd *, CDatabase *, PMS_MNG * );					// rate plan combobox change event
void		Rsv_Rom_Chg( RSV_DTL_DAT *, CWnd *, CDatabase *, PMS_MNG * );					// Change the room change edit event
void		Rsv_Rom_Dte( WCHAR *, int, int, RSV_DTL_DAT *, CWnd *, CDatabase *, PMS_MNG * );// Room use date changed
void		Rsv_Rom_Nbr( WCHAR *, int, CWnd *, CDatabase *, PMS_MNG * );					// Room number edit complete event
void		Rsv_Ctx_Mnu( CWnd *, int, int );												// Open context menu on search view
void		Rsv_Crg_Dtl( RSV_DTL_DAT *, int, CWnd *, PMS_MNG * );							// Context menu of charge detail
void		Rsv_Rom_Sel( RSV_DTL_DAT *, int, CWnd *, PMS_MNG * );							// Context menu of charge detail
int			Rsv_Get_Dat( int, CWnd *, CDatabase *, PMS_MNG * );			 					// Getting the existing data
int			Rsv_Cre_Rsv( CCI_PRM *, CWnd *, CDatabase *, PMS_MNG * );						// Create a new reservation
int			Rsv_Sav_Dat( RSV_INF_DAT *, CWnd *, CDatabase *, PMS_MNG * );					// Saveing data
void		Rsv_Dps_Dtl( Csr );																// resrvation_details dispose
void		Rsv_Dps_Rte( Csr );																// dispose the rate plan
void		Cpy_Edt( CCustomEdit *, CCustomEdit * );										// Copy edit data to detail on same reservation
int			Rsv_Cxl_Exc( CWnd *, CDatabase *, PMS_MNG * );									// Execute to cancel reservation

#define	RSV_CGK_ARV				1		// changed arrival date
#define	RSV_CGK_DPT				2		// changed departure date
#define	RSV_CGK_NGT				3		// changed night count


int			Rsv_Pod_Chg( int, RSV_DTL_DAT *, CWnd *, CDatabase *, PMS_MNG * );				// change the stay period

#endif
