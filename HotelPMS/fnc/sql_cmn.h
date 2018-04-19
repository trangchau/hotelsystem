/*
	ホテルプロパティ管理システム
	検索ウィンドウ用データベースアクセス関数郡 定義
*/

#ifndef	sql_cmnH
#define	sql_cmnH

//#include "sql_cmn.h"

#define	TBN_OPR_INF		_T("operator_infos")
#define	TBN_BRC_INF		_T("branch_infos")
#define	TBN_FCL_INF		_T("facility_infos")
#define	TBN_FCL_BLD		_T("facility_building_infos")
#define	TBN_DTY_INF		_T("duties")
#define	TBN_EMP_LYE		_T("employees")
#define	TBN_AGT_INF		_T("agent_infos")
#define	TBN_CDC_INF		_T("creditcard_companies")
#define	TBN_RSV_INF		_T("reservation_infos")
#define	TBN_RSV_DTL		_T("reservation_details")
#define	TBN_RSV_EQP		_T("reservation_equipments")
#define	TBN_RSV_CRG		_T("reservation_charges")
#define	TBN_CLI_INF		_T("client_infos")
#define	TBN_EQP_TYP		_T("equipment_types")
#define	TBN_EQP_INF		_T("equipment_infos")
#define	TBN_EQP_CRG		_T("equipment_charge_infos")
#define	TBN_EQP_CGD		_T("equipment_charge_details")
#define	TBN_DSC_CAT		_T("description_categories")
#define	TBN_DSC_RPT		_T("descriptions")
#define	TBN_MKT_SEG		_T("market_segments")
#define	TBN_ITN_CTR		_T("international_country_codes")
#define	TBN_ITN_LNG		_T("international_language_codes")
#define	TBN_CRC_CVS		_T("currency_conversions")
#define	TBN_BIZ_CDR		_T("business_calendars")
#define	TBN_BIZ_DTE		_T("business_dates")
#define	TBN_EQP_STS		_T("equipment_states")
#define	TBN_RSV_NBR		_T("reservation_numbers")
#define	TBN_SAL_INF		_T("sales_infos")
#define	TBN_SAL_DTL		_T("sales_details")
#define	TBN_PMT_DTL		_T("payment_details")

typedef	int	(*QRY_UPD_FNC)( WCHAR *, int, WCHAR *, WCHAR **, Dbs_fld *, int, CDatabase * );

int		SQL_Get_Lst( Csr, WCHAR *, Dbs_fld *, int, void *, int, WCHAR **, CDatabase * );	// Universal getting function of table list
int		SQL_Get_MLs( Csr, WCHAR *, Mlt_dbf *, int, void *, int, WCHAR **, CDatabase * );	// Universal multiple getting function of table list
void	SQL_Dsc_Cnd( WCHAR *, int, DSC_RPT *, int, PMS_MNG * );								// Making a condition of description

WCHAR	*SQL_Edt_Wst( WCHAR *, int, WCHAR * );												// wide char string edit ( escape sequence addition )
WCHAR	*SQL_Edt_Flv( Dbs_fld * );					
//WCHAR	*SQL_Edt_Dtm( WCHAR *, int, Day * );													// editing date time for query string
int		SQL_Exc_Ins( WCHAR *, int, WCHAR *, WCHAR **, Dbs_fld *, int, CDatabase * );		// Make inserting sql and execute it
int		SQL_Exc_Upd( WCHAR *, int, WCHAR *, WCHAR **, Dbs_fld *, int, CDatabase * );		// Make updating sql and execute it

#endif
