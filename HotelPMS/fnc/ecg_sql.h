/*
	ホテルプロパティ管理システム
	検索ウィンドウ用データベースアクセス関数郡 定義
*/

#ifndef	ecg_sqlH
#define	ecg_sqlH

//#include "afx.h"
//#include "rcg_fnc.h"

typedef	struct	{								// reservation list segment
	int				eci[DBF_SIZ_INT];			// EC.ID,
	int				eti[DBF_SIZ_INT];			// EC.equipment_type_id,
	int				agi[DBF_SIZ_INT];			// EC.agent_info_id,
	WCHAR			etn[DBF_SIZ_WST(255)];		// ET.name,
	int				bfr[DBF_SIZ_INT];			// EC.reservation_route,
	WCHAR			agn[DBF_SIZ_WST(255)];		// AG.name
}ECG_AVL_EQT;

typedef	struct	{								// equipment_charge_details data segment
	int				adk[DBF_SIZ_INT];			// apply_day_kind
	int				edi[DBF_SIZ_INT];			// equipment_charge_details_id
	double			prc[DBF_SIZ_DBL];			// standard price
}ECG_DTL_DAT;


typedef	struct	{								// reservation list segment
	int				eci[DBF_SIZ_INT];			// EC.ID,
	int				eii[DBF_SIZ_INT];			// EC.equipment_info_id,
	int				eti[DBF_SIZ_INT];			// EC.equipment_type_id,
	int				aii[DBF_SIZ_INT];			// EC.agent_info_id,
	int				dsi[DBF_SIZ_INT];			// EC.description_id,
	int				cgk[DBF_SIZ_INT];			// EC.charge_kind,
	int				cgp[DBF_SIZ_INT];			// EC.charge_particulars,
	int				adk[DBF_SIZ_INT];			// apply_day_kind
	double			prc[DBF_SIZ_DBL];			// standard price
	int				edi[DBF_SIZ_INT];			// equipment_charge_details_id
	int				psn[DBF_SIZ_INT];			// EC.persons,
	WCHAR			etn[DBF_SIZ_WST(255)];		// ET.name,
	WCHAR			eqc[DBF_SIZ_WST(255)];		// EI.equipment_code,
	int				bfr[DBF_SIZ_INT];			// EC.reservation_route,
	WCHAR			agn[DBF_SIZ_WST(255)];		// AG.name
	WCHAR			pln[DBF_SIZ_WST(255)];		// DC.name( plan name from descriptions )
	int				sen[DBF_SIZ_INT];			// DC.sale_enable
	Day				vss[DBF_SIZ_DAY];			// valid_sale_period_start
	Day				vse[DBF_SIZ_DAY];			// valid_sale_period_start
	Lnk				dtl[1];						// ECG_DTL_DAT
}ECG_PLN_DAT;

typedef	struct	{								// equipment_charge details segment
	EQP_CRG			crg[1];						// equuipment_charge_infos
	EQP_CGD			cgd[1];						// equuipment_charge_details
	DSC_RPT			dsc[1];						// descriptions
	Lnk				dtl[1];						// root of equipment_charge_details
}ECG_PLN_DTL;


typedef	struct	{
	int				bfr;						// Booking path
	int				agi;						// id of agent_infos
	int				eii;						// id of equipment_types
	int				eti;						// id of equipment_types
	int				eci;						// id of equipment_charge_infos
	Day				*std;						// Date of stay starting
	Day				*edd;						// Date of stay ending
	WCHAR			*opc;						// operator code
	WCHAR			*brc;						// branch code
	WCHAR			*fcc;						// facility code
}ECG_SQL_PRM;


typedef	struct	{								// Apply segment
	int				dsi[DBF_SIZ_INT];			// id of descriptions
	WCHAR			nam[DBF_SIZ_WST(255)];		// name of description
	Day				vss[DBF_SIZ_DAY];			// valid sale period start
	Day				vse[DBF_SIZ_DAY];			// valid sale period end
	int				atr[DBF_SIZ_INT];			// tax attribution
	int				pkd[DBF_SIZ_INT];			// price kind
	double			prc[DBF_SIZ_DBL];			// price
}ECG_APL_DAT;


typedef	struct	{								// rate plan structure
	ECG_PLN_DAT		pln[1];						// id of equipment_charge_infos
	WCHAR			nam[DBF_SIZ_WST(255)];		// Rate Plan Name
}RSV_ECG_DAT;

int		Ecg_Get_Pln( Csr, int, ECG_SQL_PRM *, WCHAR **, CDatabase *, PMS_MNG * );				// Getting room charge list
int		Ecg_Avl_Eqt( Csr, ECG_SQL_PRM *, int, WCHAR **, CDatabase *, PMS_MNG * );				// Getting avalable room type on booking path
int		Ecg_Apl_Dat( Csr, ECG_SQL_PRM *, WCHAR **, CDatabase *, PMS_MNG * );					// Getting apply from description
int		Ecg_Erp_Lst( Csr, ECG_SQL_PRM *, WCHAR **, CDatabase *, PMS_MNG * ); 					// Getting equipment rate plan list
int		Ecg_Erp_Dtl( ECG_PLN_DTL *, ECG_SQL_PRM *, WCHAR **, CDatabase *, PMS_MNG * );			// Getting equipment rate plan detail
int		Ecg_Erp_Del( ECG_PLN_DTL *, Day *, WCHAR **, CDatabase *, PMS_MNG * );					// Delete equipment rate plan detail
int		Ecg_Erp_Sav( ECG_PLN_DTL *, int, Day *, WCHAR **, CDatabase *, PMS_MNG * );				// Update equipment rate plan detail
int		Ecg_Get_Cat( WCHAR *, int, int, int, WCHAR *, WCHAR *, WCHAR *, WCHAR **, CDatabase *, PMS_MNG * );	// getting category code
int		Ecg_Cpy_Pln( Csr, Csr, WCHAR **, CDatabase *, PMS_MNG * );								// copying rate plan
int		Ecg_Cst_Lst( Csr, ECG_SQL_PRM *, WCHAR **, CDatabase *, PMS_MNG * );					// Getting description_category list
int		Ecg_Chg_Crg( Csr, ECG_SQL_PRM *, WCHAR **, CDatabase *, PMS_MNG * );					// confirm existing equipment_charge

#endif
