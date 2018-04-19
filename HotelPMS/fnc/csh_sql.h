/*
	ホテルプロパティ管理システム
	予約ウィンドウ用データベースアクセス関数郡 定義
*/

#ifndef	csh_sqlH
#define	csh_sqlH

#include "..\..\Common\dbs_mng.h"
#include "..\..\pms_def.h"

typedef	struct	{
	int			dci[DBF_SIZ_INT];		// description_category_id
	WCHAR		dcn[DBF_SIZ_WST(255)];	// description_category_name
	DSC_RPT		pdt[1];					// description segment
	double		qty;					// selecting count
}CSH_PDT_LST;

typedef	struct	{
	SAL_INF		sal[1];					// sales_infos segment
	Lnk			dtl[1];					// sales_details segment
}CSH_SAL_DAT;

typedef	struct	{
	int			apl[DBF_SIZ_INT];		// tax application
	int			dkd[DBF_SIZ_INT];		// description_kind
	int			prk[DBF_SIZ_INT];		// price_kind
	WCHAR		rmn[DBF_SIZ_WST(255)];	// room number
	WCHAR		dcn[DBF_SIZ_WST(255)];	// description_category_name
	SAL_DTL		dtl[1];					// sales_infos segment
}CSH_SAL_DTL;

typedef	struct	{
	int			rii[DBF_SIZ_INT];		// reservation_info_id
	int			rdi[DBF_SIZ_INT];		// reservation_detail_id
	int			rei[DBF_SIZ_INT];		// reservation_equipment_id
	int			eii[DBF_SIZ_INT];		// equipment_info_id
	WCHAR		fsn[DBF_SIZ_WST(255)];	// first_name of client_infos
	WCHAR		lsn[DBF_SIZ_WST(255)];	// last_name of client_infos
	WCHAR		slg[DBF_SIZ_WST(255)];	// system_language of client_infos
	WCHAR		rmn[DBF_SIZ_WST(255)];	// room number
	//SAL_INF		sal[1];					// sales_infos segment
	CSH_SAL_DAT	sal[1];					// sales_infos segment
	Lnk			dps[1];					// root of deposit data
}CSH_INH_DAT;

typedef	struct	{
	double		ttl;					// total amount of this tax application
	double		amt;					// amount of this tax after calculation
	double		abr;					// amount of this tax after calculation( abroad )
	int			rii[DBF_SIZ_INT];		// reservation_info_id
	int			rdi[DBF_SIZ_INT];		// reservation_detail_id
	DSC_RPT		tax[1];					// tax definition
}CSH_TAX_DAT;

typedef	struct	{
	double		ttl;					// total of thos deposit
	double		bls;					// balance of payment currency
	double		pay;					// payment of payment currency
	double		pye;					// payment of original currency
	int			rii[DBF_SIZ_INT];		// reservation_info_id
	int			rdi[DBF_SIZ_INT];		// reservation_detail_id
	int			adp[DBF_SIZ_INT];		// after_decimal_point of currency_conversion
	int			cci[DBF_SIZ_INT];		// currency_conversion_id
	int			isb[DBF_SIZ_INT];		// is_base of currency_conversion
	double		rte[DBF_SIZ_INT];		// exchange_rate of currency_conversion
	WCHAR		crc[DBF_SIZ_WST(255)];	// currency_code of currency_conversion
	SAL_DTL		dtl[1];					// deposit data(sales_details)
	PMT_DTL		pmt[1];					// deposit data(payment_infos)
}CSH_DPS_DAT;


int		Csh_Get_Dat( Csr, WCHAR **, CDatabase *, PMS_MNG * );						// Gtting operator_info
int		Csh_Get_Dps( Csr, int, WCHAR **, CDatabase *, PMS_MNG * );					// Gtting deposit data
int		Csh_Sav_Dat( Csr, int, Day *, WCHAR **, CDatabase *, PMS_MNG * );			// Saving operator_info
int		Csh_Get_Pdt( Csr,WCHAR *,WCHAR *,WCHAR *,WCHAR **,CDatabase *,PMS_MNG * );	// Getting product list for product selecting
int		Csh_Inh_Sal( Csr, int, WCHAR **, CDatabase *, PMS_MNG * );					// Getting inbhouse sales data
int		Csh_Get_Ccv( Csr, CRC_CVS *, WCHAR **, CDatabase *, PMS_MNG * );			// Gtting currency_conversions
int		Csh_Pay_Mnt( CSH_SAL_DAT *,Csr,Csr,Day *,WCHAR **,CDatabase *,PMS_MNG * );	// payment
int		Dsc_Get_Dsc( Csr, DSC_RPT *, int, WCHAR **, CDatabase *, PMS_MNG * );		// Gtting tdeposit and discount of descriptions
int		Csh_Sav_Dps( CSH_DPS_DAT *, Day *, WCHAR **, CDatabase *, PMS_MNG * );		// Saving deposit data
int		Csh_Sav_Dps( Csr, Day *, WCHAR **, CDatabase *, PMS_MNG * );				// Saving deposit data
int		Csh_Get_Sii( int, SAL_INF *, WCHAR **, CDatabase *, PMS_MNG * );			// getting sales_info_id by reservation_detail_id
int		Csh_Sav_Ccv( Csr, Day *, WCHAR **, CDatabase *, PMS_MNG * );				// saving currency_conversion

#endif
