/*
	Hotel Property Management System
	Query functions for clients definition
*/

#ifndef	cli_sqlH
#define	cli_sqlH

#include "..\..\Common\dbs_mng.h"
#include "..\..\pms_def.h"

int			Cli_Get_Inf( Csr, WCHAR *, WCHAR *, WCHAR *, WCHAR *, WCHAR **, CDatabase *, PMS_MNG * );	// Getting client informations for reservation
int			Msg_Get_Msg( Csr, MKT_SEG *, int, WCHAR **, CDatabase *, PMS_MNG * );	// Gtting description_categories
int			Msg_Sav_Msg( Csr, WCHAR **, CDatabase *, PMS_MNG * );					// save description_categories

#endif
