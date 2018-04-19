/*
	Functions for checkin view definition
*/

#ifndef	inh_fncH
#define	inh_fncH

#include "..\stdafx.h"
#include "..\resource.h"
#include "..\ReservationDoc.h"
#include "..\ReservationView.h"
#include "..\..\Custom\SimpleGrid.h"
#include "..\..\pms_def.h"
#include "inh_sql.h"

CSimpleGrid	*Cin_Mak_Grd( int, RECT *, CWnd * );									// Creating a grid control
void		Inh_Dtl_Dsp( CWnd *, INH_DTL_DAT *, INH_INF_DAT *, PMS_MNG * );			// Displaying reservation detail data
void		Inh_Dsp_Dat( CWnd *, CSimpleGrid *, INH_INF_DAT *, Csr, PMS_MNG * );	// Displaying reservation info data

#endif
