/*
	ホテルプロパティ管理システム
	予約ウィンドウ用データベースアクセス関数郡 定義
*/

#ifndef	rsv_sqlH
#define	rsv_sqlH

#include "..\..\Common\dbs_mng.h"
#include "..\..\pms_def.h"

typedef	struct	{								// reservation information data segment
	RSV_INF		rsv[1];							// reservation_infos
	CLI_INF		cli[1];							// client_infos
	Lnk			dtl[1];							// root of reservation_details
	int			est;							// editting status
}RSV_INF_DAT;

typedef	struct	{								// reservation detail data segment
	RSV_DTL		dtl[1];							// resevation details(Note that this structure must be in top every time for common access of viw_com.cpp)
	CLI_INF		cli[1];							// client_infos
	Lnk			req[1];							// root of reservation_equipments
	Lnk			rcg[1];							// root of reservation_charges
	int			eci[DBF_SIZ_INT];				// equipment_charge_info_id
	int			est;							// editting status
	int			smr;							// Same as made a reservation person
	int			cxl;							// target of cancellation
}RSV_DTL_DAT;

typedef	struct	{								// reservation detail data segment
	RSV_EQP		req[1];							// reservation_equipments
	int			esi[DBF_SIZ_INT];				// equipment_States_id
	WCHAR		rno[DBF_SIZ_WST(255)];			// equipment_code
	WCHAR		tnm[DBF_SIZ_WST(255)];			// room type name
}RSV_EQP_DAT;

typedef	struct	{								// reservation detail data segment
	EQP_INF		eqi[1];							// reservation_equipments
	int			eti[DBF_SIZ_INT];				// equipment_type_id
	int			esi[DBF_SIZ_INT];				// equipment_state_id
	WCHAR		tnm[DBF_SIZ_WST(255)];			// room type name
}RSV_EQP_INF;

typedef	struct	{
	RSV_CRG		rcg[1];							// reservation_equipments
	double		prc[DBF_SIZ_DBL];				// standard price
	int			pup[DBF_SIZ_INT];				// paid_up
	WCHAR		etn[DBF_SIZ_WST(255)];			// ET.name,
	WCHAR		eqc[DBF_SIZ_WST(255)];			// EI.equipment_code,
	WCHAR		agn[DBF_SIZ_WST(255)];			// AG.name
	WCHAR		pln[DBF_SIZ_WST(255)];			// DC.name( plan name from descriptions )
}RSV_RCG_DAT;

typedef	struct	{								// equipment_info + equipment_state_id
	EQP_INF		eqi[1];							// equipment_infos segment
	int			esi[DBF_SIZ_INT];				// equipment_state_id
}RSV_EQP_EQS;

int			Rsv_Get_Inf( RSV_INF_DAT *, int, WCHAR **, CDatabase *, PMS_MNG * );		// 予約情報取得
int			Rsv_Get_Eis( Csr, WCHAR **, CDatabase *, PMS_MNG * );						// Getting equipment_infos + equipment_state_id
int			Rsv_Get_Dtl( RSV_INF_DAT *, WCHAR **, CDatabase *, PMS_MNG * );				// 予約詳細取得
int			Rsv_Rsv_Sav( WCHAR **, RSV_INF_DAT *, Day *, CDatabase *, PMS_MNG * );		// save reservation data
int			Rsv_Rom_Avl( int, int, Day *, Day *, WCHAR **, CDatabase *, PMS_MNG * );	// checking room avalability 
int			Rsv_Idv_Cxl( int, int, WCHAR **, CDatabase *, PMS_MNG * );					// Cancellation direct of indivusual
int			Rsv_Cxl_Rsv( Csr, int, WCHAR **, CDatabase *, PMS_MNG * );					// Execute to cancel erservation
int			Rsv_Avl_Rms( Csr,int,int,Day *,Day *,WCHAR **,CDatabase *,PMS_MNG * );		// getting available room list
int			Rsv_Get_Rdt( RSV_DTL *, int, WCHAR **, CDatabase *, PMS_MNG * );			// Getting reservation_details
int			Rsv_Get_Eci( int, WCHAR **, CDatabase *, PMS_MNG * );						// Getting equipent_charge_info_id from reservation_equipments
int			Rsv_Chg_Req( RSV_EQP *, Csr, int, Day *, WCHAR **, CDatabase *, PMS_MNG * );// change the stay period and rooms
int			Rsv_Cpy_Req( RSV_EQP *, Csr, int, Day *, WCHAR **, CDatabase *, PMS_MNG * );// change the stay period and rooms

#endif
