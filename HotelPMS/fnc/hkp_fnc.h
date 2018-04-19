/*
	Functions for checkin view definition
*/

#ifndef	hkp_fncH
#define	hkp_fncH

#include "..\stdafx.h"
#include "..\resource.h"
#include "..\ReservationDoc.h"
#include "..\ReservationView.h"
#include "..\..\Custom\SimpleGrid.h"
#include "..\..\pms_def.h"
#include "hkp_sql.h"

#define	IDC_GRD_HKP			(8192)
#define	IDC_GRD_RCH			(IDC_GRD_HKP+1)

#define	HKP_CLM_DAT			1

#define	HKP_CLM_RMN			0
#define	HKP_CLM_SST			1
#define	HKP_CLM_CST			2
#define	HKP_CLM_OST			3
#define	HKP_CLM_GNM			4
#define	HKP_CLM_POD			5


#define	HKP_CCL_SEL			0		// Select
#define	HKP_CCL_CRM			1		// current room number
#define	HKP_CCL_NRM			2		// next room number
#define	HKP_CCL_GNM			3		// guest name


CSimpleGrid	*Hkp_Mak_Grd( int, RECT *, CWnd * );							// Creating a grid control
int			Hkp_Get_Rch( CWnd *, CDatabase *, PMS_MNG * );					// getting and displaying room chane data
int			Hkp_Get_Dat( CWnd *, CDatabase *, PMS_MNG * );					// getting and displaying house keeping data
int			Hkp_Set_Cmb( CComboBox *, int, int, CWnd * );					// setting combobox data
int			Hkp_Get_Cmb( int, int, int, CWnd *, CDatabase *, PMS_MNG * );	// getting combobox data

#endif
