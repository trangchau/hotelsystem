/*
	Functions for checkout view definition
*/

#ifndef	cot_fncH
#define	cot_fncH

#include "..\stdafx.h"
#include "..\resource.h"
#include "..\ReservationDoc.h"
#include "..\ReservationView.h"
#include "..\..\Custom\SimpleGrid.h"
#include "..\..\pms_def.h"
#include "cot_sql.h"

CSimpleGrid	*Cot_Mak_Grd( int, RECT *, CWnd * );									// Creating a grid control
void		Cot_Dtl_Dsp( CWnd *, RSV_DTL_DAT *, INH_INF_DAT *, PMS_MNG * );			// Displaying reservation detail data
void		Cot_Dsp_Dat( CWnd *, CSimpleGrid *, INH_INF_DAT *, Csr, PMS_MNG * );	// Displaying reservation info data
int			Cot_Exc_Cot( int, CWnd *, CDatabase *, PMS_MNG * );						// execute to checkout

#endif
