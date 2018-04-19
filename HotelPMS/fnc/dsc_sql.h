/*
	ホテルプロパティ管理システム
	予約ウィンドウ用データベースアクセス関数郡 定義
*/

#ifndef	dsc_sqlH
#define	dsc_sqlH

#include "..\..\Common\dbs_mng.h"
#include "..\..\pms_def.h"

int		Dsc_Get_Cat( Csr, DSC_CAT *, int, WCHAR **, CDatabase *, PMS_MNG * );	// Gtting description_categories
int		Dsc_Sav_Cat( Csr, WCHAR **, CDatabase *, PMS_MNG * );					// save description_categories
int		Dsc_Get_Tax( Csr, DSC_RPT *, int, WCHAR **, CDatabase *, PMS_MNG * );	// Gtting tax and other of descriptions
int		Dsc_Get_Atr( Csr, DSC_RPT *, Csr, WCHAR **, CDatabase *, PMS_MNG * );	// Gtting existing attribute
int		Dsc_Get_Dsc( Csr, DSC_RPT *, WCHAR **, CDatabase *, PMS_MNG * );		// Gtting descriptions
int		Dsc_Sav_Dsc( Csr, WCHAR **, CDatabase *, PMS_MNG * );					// save descriptions
int		Dsc_Get_Dps( Csr, DSC_RPT *, Csr, WCHAR **, CDatabase *, PMS_MNG * );	// getting other deposit/discount data

#endif
