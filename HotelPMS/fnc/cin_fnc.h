/*
	Functions for checkin view definition
*/

#ifndef	cin_fncH
#define	cin_fncH

#include "..\stdafx.h"
#include "..\resource.h"
#include "..\ReservationDoc.h"
#include "..\ReservationView.h"
#include "..\..\Custom\SimpleGrid.h"
#include "..\..\pms_def.h"
#include "cin_sql.h"

#define	CIN_CLM_DAT			1

#define	CIN_CLM_SEL			0
#define	CIN_CLM_NAM			1
#define	CIN_CLM_STS			2
#define	CIN_CLM_RTP			3
#define	CIN_CLM_RNB			4
#define	CIN_CLM_ARV			5
#define	CIN_CLM_DPT			6
#define	CIN_CLM_PSN			7

CSimpleGrid	*Cin_Mak_Grd( int, RECT *, CWnd * );						// Creating a grid control
int			Cin_Get_Dat( int, CWnd *, CDatabase *, PMS_MNG * );			// Getting the existing data
void		Cin_Cbx_Clk( int, CWnd *, PMS_MNG * );						// checkin checkbox click
void		Cin_Chk_All( CWnd *, PMS_MNG * );							// CheckAll checkbox click
void		Cin_Exc_Clk( CWnd *, CDatabase *, PMS_MNG * );				// Execute checkin button click
void		Cin_Ctx_Mnu( CWnd *, int, int, int );						// Open context menu on checkin grid
void		Cin_Rom_Nbr( WCHAR *, int, CWnd *, CDatabase *, PMS_MNG * );// Room number edit complete event
void		Cin_Rom_Sel( CWnd *, PMS_MNG * );							// Context menu of charge detail
int			Cin_Sav_Dat( CWnd *, CDatabase *, PMS_MNG * );				// saving data
void		Cin_Dps_Cin( CIN_INF_DAT * );								// Dispose the checkin information data 

#endif
