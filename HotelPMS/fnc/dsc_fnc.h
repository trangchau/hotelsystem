/*
	Functions for checkin view definition
*/

#ifndef	dsc_fncH
#define	dsc_fncH

#include "..\stdafx.h"
#include "..\resource.h"
#include "..\..\Custom\SimpleGrid.h"
#include "..\..\pms_def.h"

#define	IDC_GRD_PDT		(8192)			// Copy destination selecting grid
#define	IDC_GRD_TAP		(IDC_GRD_PDT+1)	// Copy destination selecting grid

#define	CLM_DAT			0		// Data keeping column
#define	CLM_TXD			1		// Tax data keeping column

#define	CLM_NAM			0		// Name
#define	CLM_ABR			1		// Abbreviation
#define	CLM_SEN			2		// S.E
#define	CLM_SPR			3		// SalePeriod
#define	CLM_KND			4		// Kind
#define	CLM_PRC			5		// Price

#define CLM_APL			0		// Application checkbox column
#define	CLM_DSC			1		// Tax desctiption
#define	CLM_TPC			2		// Tax price 

CSimpleGrid	*Dsc_Mak_Grd( int, RECT *, CWnd * );									// Creating a grid control

#endif
