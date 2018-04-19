/*
	Hotel Property Management System
	Definition default field of each table
*/

#ifndef	fld_defH
#define	fld_defH

#include "..\..\Common\dbs_mng.h"
#include "..\..\pms_def.h"

#define	FDF_OPR_INF				1				// operator_infos
#define	FDF_BRC_INF				2				// branch_infos
#define	FDF_FCL_INF				3				// facility_infos
#define	FDF_BLD_INF				4				// facility_building_infos
#define	FDF_DTY_INF				5				// duties
#define	FDF_EMP_LYE				6				// employees
#define	FDF_BIZ_CDR				7				// business_calendar
#define	FDF_EQP_INF				8				// equipment_infos
#define	FDF_EQP_TYP				9				// equipment_types
#define	FDF_EQP_STS				10				// equipment_states
#define	FDF_AGT_INF				11				// agent_infos
#define	FDF_CDC_INF				12				// creditcard_companies
#define	FDF_MKT_SEG				13				// maarket_segments
#define	FDF_CLI_INF				14				// client_infos
#define	FDF_CRC_CVS				20				// currency_conversions
#define	FDF_RSV_INF				51				// reservation_infos
#define	FDF_RSV_DTL				52				// reservation_details
#define	FDF_RSV_EQP				53				// reservation_equipments
#define	FDF_RSV_CRG				54				// reservation_charges
#define	FDF_DSC_RPT				55				// descriptions
#define	FDF_DSC_CAT				56				// descriptio_categoriens
#define	FDF_EQP_CRG				57				// equipment_charge_infos
#define	FDF_EQP_CGD				58				// equipment_charge_details
#define	FDF_SAL_INF				59				// sales_infos
#define	FDF_SAL_DTL				60				// sales_details
#define	FDF_PMT_DTL				61				// payment_details

int			Fdf_Mak_Sll( WCHAR **, int, WCHAR *, WCHAR * );					// Make select list
int			Fdf_Opr_Inf( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the operator_infos field definition
int			Fdf_Brc_Inf( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the branch_infos field definition
int			Fdf_Fcl_Inf( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the facility_infos field definition
int			Fdf_Dty_Inf( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the duties field definition
int			Fdf_Emp_Lye( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the employees field definition
int			Fdf_Bld_Inf( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the facility_building_infos field definition
int			Fdf_Biz_Cdr( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the business_calendar field definition
int			Fdf_Eqp_Inf( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the equipnet_infos field definition
int			Fdf_Eqp_Typ( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the equipnet_types field definition
int			Fdf_Eqp_Sts( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the equipnet_states field definition
int			Fdf_Mkt_Seg( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the maarket_segments field definition
int			Fdf_Agt_Inf( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the agent_infos field definition
int			Fdf_Cdc_Inf( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the creditcard_companies field definition
int			Fdf_Cli_Inf( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the client_infos field definition
int			Fdf_Crc_Cvs( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the currency_conversions field definition
int			Fdf_Rsv_Inf( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the reservation_infos field definition
int			Fdf_Rsv_Dtl( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the reservation_infos field definition
int			Fdf_Rsv_Eqp( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the reservation_equipments field definition
int			Fdf_Rsv_Crg( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the reservation_charges field definition
int			Fdf_Dsc_Rpt( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the descriptions field definition
int			Fdf_Dsc_Cat( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the description_categories field definition
int			Fdf_Eqp_Crg( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the equipment_charge_infos field definition
int			Fdf_Eqp_Cgd( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the equipment_charge_details field definition
int			Fdf_Sal_Inf( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the sales_infos field definition
int			Fdf_Sal_Dtl( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the sales_details field definition
int			Fdf_Pmt_Dtl( Dbs_fld **, void *, WCHAR *, Dbs_fld *, int );		// Making the payment_details field definition

#endif
