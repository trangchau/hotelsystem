/*
	予約情報処理関数
*/

#ifndef	rcg_fncH
#define	rcg_fncH

#include "..\stdafx.h"
#include "..\resource.h"
#include "..\..\Custom\SimpleGrid.h"
#include "..\..\pms_def.h"
#include "..\RoomChargeDlg.h"
#include "rsv_sql.h"

#define	IDC_GRD_DTL		(8192)			// 予約詳細グリッドID
#define	IDC_GRD_APL		(IDC_GRD_DTL+1)	// 部屋割付グリッドID
/*
#define	RCG_CHK_CLM		0				// Room use start date column on grid
#define	ROM_UED_CLM		1				// Room use end date column on grid
#define	ROM_TYP_CLM		2				// Room use room type column on grid
#define	ROM_NBR_CLM		3				// Room use room number column on grid
*/

typedef	struct	{						// Tab data management structure
	int			bkp;					// Booking path
	int			aii;					// id of agent_infos
	Lnk			rot[1];					// root of Plan list 
}PLN_;


CSimpleGrid	*Rcg_Mak_Grd( int, RECT *, CWnd * );					// Make a grid component
int			Rcg_Ini_Dat( CWnd *, CDatabase *, PMS_MNG * );			// Loading initialize each data and setting
int			Rcg_Get_Lst( CWnd *, CDatabase *, PMS_MNG * );			// Getting the plan list on rooptype
void		Rcg_Dps_Pln( PLN_ *, int );								// Dispose the plan list
void		Rcg_Dsp_Pln( CWnd * );									// Displaying detail grid
void		Rcg_Sel_Pln( int, CWnd *, CDatabase *, PMS_MNG * );		// On select 1 plan on the grid
void		Rcg_Sld_Chg( CWnd *, CDatabase *, PMS_MNG * );			// Sale date change event
void		Rcg_Del_Dtl( CWnd *, CDatabase *, PMS_MNG * );			// Delete rate plan
void		Rcg_Sav_Dtl( int, CWnd *, CDatabase *, PMS_MNG * );		// update a rate plan
void		Rcg_Cpy_Pln( int, CWnd *, CDatabase *, PMS_MNG * );		// Copy rate plan
void		Rcg_New_Dtl( CWnd *, CDatabase *, PMS_MNG * );			// copy rate plan

#endif
