/*
	ホテルプロパティ管理システム
	検索ウィンドウ用データベースアクセス関数郡 定義
*/

#ifndef	sch_sqlH
#define	sch_sqlH

#include "sch_fnc.h"

typedef	struct	{								// reservation list segment
	RSV_INF			rsv[1];
	// items of reservation_details below:
	Day				bdy[DBF_SIZ_DAY];			// business date
	WCHAR			fnm[DBF_SIZ_WST(255)];		// first_name
	WCHAR			lnm[DBF_SIZ_WST(255)];		// last_name
	int				ckd[DBF_SIZ_INT];			// client_kind
	int				rdi[DBF_SIZ_INT];			// reservation_detail id
	int				dci[DBF_SIZ_INT];			// client_info_id of 
	WCHAR			dfn[DBF_SIZ_WST(255)];		// first_name
	WCHAR			dln[DBF_SIZ_WST(255)];		// last_name
	Day				arv[DBF_SIZ_DAY];			// arrival_date
	Day				dpt[DBF_SIZ_DAY];			// departure_date
	Day				cid[DBF_SIZ_DAY];			// check_in_date
	Day				cod[DBF_SIZ_DAY];			// check_out_date
	int				cxl[DBF_SIZ_INT];			// canceledf 
	// items as equipment below:
	int				eii[DBF_SIZ_INT];			// equipment_info_id of 
	WCHAR			rno[DBF_SIZ_WST(255)];		// room number ( equipment_code on equipment_infos )
}SCH_RSV_LST;


int		Sch_Rsv_Lst( Csr, int, int, SCH_OPT *, WCHAR **, CDatabase *, PMS_MNG * );		// Get reservation list as each kind

#endif
