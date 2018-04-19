/*
	ホテルプロパティ管理システム
	予約ウィンドウ用データベースアクセス関数郡 定義
*/

#ifndef	agt_sqlH
#define	agt_sqlH

#include "..\..\Common\dbs_mng.h"
#include "..\..\pms_def.h"

int		Agt_Get_Dat( Csr, WCHAR *, WCHAR **, CDatabase *, PMS_MNG * );	// Gtting branch_info
int		Agt_Sav_Dat( Csr, WCHAR **, CDatabase *, PMS_MNG * );			// Saving branch_info

#endif
