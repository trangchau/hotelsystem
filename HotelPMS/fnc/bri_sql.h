/*
	ホテルプロパティ管理システム
	予約ウィンドウ用データベースアクセス関数郡 定義
*/

#ifndef	bri_sqlH
#define	bri_sqlH

#include "..\..\Common\dbs_mng.h"
#include "..\..\pms_def.h"

int		Bri_Get_Dat( Csr, WCHAR *, WCHAR **, CDatabase *, PMS_MNG * );	// Gtting branch_info
int		Bri_Sav_Dat( Csr, WCHAR **, CDatabase *, PMS_MNG * );			// Saving branch_info

#endif
