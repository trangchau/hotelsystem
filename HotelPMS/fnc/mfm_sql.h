/*
	ホテルプロパティ管理システム
	MainFrame用データベースアクセス関数郡 定義
*/

#ifndef	mfm_sqlH
#define	mfm_sqlH

#include "mfm_fnc.h"

// Get_Mst kind
#define	GTM_OPR_INF				1				// operator_infos
#define	GTM_BRC_INF				2				// branch_infos
#define	GTM_FCL_INF				3				// facility_infos
#define	GTM_EQP_INF				4				// equipment_infos
#define	GTM_EQP_TYP				5				// equipment_types
#define	GTM_MKT_SEG				6				// market_segment
#define	GTM_AGT_INF				7				// agent_types
#define	GTM_BIZ_CDR				8				// business_calendar
#define	GTM_BLD_INF				9				// facility_building_infos

typedef	struct	{								// Country code and Names
	int			cni[DBF_SIZ_INT];				// country_code_name_id	
	WCHAR		ctn[DBF_SIZ_WST(255)];			// Coubtry Name
	int			ici[DBF_SIZ_INT];				// internationat_country_code_id	
	WCHAR		ctc[DBF_SIZ_WST(255)];			// Coubtry Code(2 digit )
}CON_TRY;

typedef	struct	{								// Language code and Names
	int			lni[DBF_SIZ_INT];				// language_code_name_id	
	WCHAR		lgn[DBF_SIZ_WST(255)];			// Language Name
	int			ili[DBF_SIZ_INT];				// internationat_language_code_id	
	WCHAR		lgc[DBF_SIZ_WST(255)];			// Language Code(2 digit )
}LAN_GAG;

typedef	struct	{								// Direct launch parameter
	int			rii[DBF_SIZ_INT];				// reservation_info_id	
	int			rdi[DBF_SIZ_INT];				// reservation_detail_id	
}DRL_PRM;

int		Get_Ech_Mst( WCHAR **, CDatabase *, PMS_MNG * );							// Getting each masters
int		Get_Bdt( Day *, WCHAR **, CDatabase *, PMS_MNG * );							// Getting the business date
int		Get_Mst( Csr, int, WCHAR *, WCHAR *, WCHAR **, CDatabase *, PMS_MNG * );	// getting each masters
int		Mfm_Get_Crc( Csr, CRC_CVS *, int, WCHAR **, CDatabase *, PMS_MNG * );		// Gtting currency_conversions
int		Mfm_Sav_Crc( Csr, WCHAR **, CDatabase *, PMS_MNG * );						// save currency_conversions
int		Mfm_Get_Pwd( EMP_LYE *, WCHAR *, WCHAR **, CDatabase *, PMS_MNG * );		// getting password
int		Mfm_Upd_Pwd( WCHAR *, WCHAR *, WCHAR **, CDatabase *, PMS_MNG * );			// updating password
int		Mfm_Lck_Pwd( WCHAR *, WCHAR **, CDatabase *, PMS_MNG * );					// locking password
int		Mfm_Get_Rsv( DRL_PRM *, WCHAR *, int, WCHAR **, CDatabase *, PMS_MNG * );	// getting reservation_info_id

#endif
