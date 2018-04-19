/*
	Functions for checkin view definition
*/

#ifndef	csh_fncH
#define	csh_fncH

#include "..\stdafx.h"
#include "..\resource.h"
#include "..\..\Custom\SimpleGrid.h"
#include "..\..\pms_def.h"
//#include "inh_sql.h"

#define	IDC_GRD_CSH		8192
#define	IDC_GRD_CTX		8193
#define	IDC_GRD_DPS		8194

#define	CLM_CRC					0		// Currency code
#define	CLM_MTD					1		// payment method
#define	CLM_BLS					2		// Balance
#define	CLM_BLE					3		// Balance of payment currency
#define	CLM_PMT					4		// Payment of payment currency

#define	CLM_TDC			0		// tax description
#define	CLM_RTE			1		// tax rate
#define	CLM_TXP			2		// tax price
#define	CLM_TXA			3		// tax price abroad

CSimpleGrid	*Csh_Mak_Grd( int, RECT *, CWnd * );							// Creating a grid control
int			Csh_Get_Tax( WCHAR **, CWnd *, CDatabase *, PMS_MNG * );		// Tax and additional data
int			Csh_All_Sal( Csr, CWnd *,  CDatabase *, PMS_MNG * );			// add new selecting products
int			Csh_Add_Dps( CWnd *, CDatabase *, PMS_MNG * );					// adding a deposit
void		Csh_Dsp_Tax( CSimpleGrid *, Csr, Csr, int, PMS_MNG * );			// displaying tax definision
void		Csh_Dsp_Dps( CSimpleGrid *, Csr, int, CRC_CVS *, CRC_CVS * );	// displaying deposit
double		Csh_Dsp_Ttl( CSimpleGrid *,Csr,Csr,Csr,CRC_CVS *,PMS_MNG * );	// total calculation and display
void		Csh_Dsp_Sal( CWnd *, PMS_MNG * );								// displaying sale list 
//void		Csh_Edt_Cmp( DWORD, int, int, WCHAR *, CWnd *, PMS_MNG * );		// edit to complete of unit price or quantity
void		Csh_Ecp_Pdt( DWORD, int, int, WCHAR *, CWnd *, PMS_MNG * );		// edit to complete of unit price or quantity
void		Csh_Ecp_Ttl( int,Csr,WCHAR *,CSimpleGrid *,CWnd *,PMS_MNG *);	// edit to complete of unit price or quantity
void		Csh_Dsp_Crt( CRC_CVS *, CWnd * );								// display the currency rate
LRESULT		Csh_Clm_Typ( DWORD, int, int, CWnd * );							// required colmun type
int			Csh_All_Pay( CWnd *, CDatabase *, PMS_MNG * );					// all payment
int			Csh_Cls_Sav( CWnd *, CDatabase *, PMS_MNG * );					// save on close
void		Csh_Prt_Pay( CWnd *, CDatabase *, PMS_MNG * );					// partical payment
int			Csh_Use_Dps( CSimpleGrid *,WCHAR *,int,int,CWnd *,PMS_MNG *);	// using a deposit
void		Csh_Chk_All( int, CWnd *, PMS_MNG * );							// set all checkbox on Csh_grd
int			Csh_Trs_Far( CWnd *,CDatabase *, PMS_MNG * );					// Transfer to other room
void		Csh_Dps_Ihl( CWnd * );											// dispose the room list

#endif
