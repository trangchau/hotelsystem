/*
	ホテルプロパティ管理システム
	予約ウィンドウ用データベースアクセス関数郡 定義
*/

#ifndef	epl_sqlH
#define	epl_sqlH

#include "..\..\Common\dbs_mng.h"
#include "..\..\pms_def.h"

int		Epl_Get_Dat( Csr, EMP_LYE *, WCHAR **, CDatabase *, PMS_MNG * );	// Gtting employees
int		Epl_Sav_Dat( Csr, WCHAR **, CDatabase *, PMS_MNG * );				// save employees
int		Epl_Get_Dty( Csr, EMP_LYE *, WCHAR **, CDatabase *, PMS_MNG * );	// getting duties
int		Epl_Cfm_Dup( int, WCHAR *,  WCHAR **, CDatabase *, PMS_MNG * );		// Confirmation dupulicae employee code
int		Dty_Get_Dty( Csr, DTY_INF *, int, WCHAR **, CDatabase *, PMS_MNG * );	// Gtting description_categories
int		Dty_Sav_Dty( Csr, WCHAR **, CDatabase *, PMS_MNG * );					// save description_categories

#endif
