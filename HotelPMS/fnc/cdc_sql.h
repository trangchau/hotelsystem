/*
	�z�e���v���p�e�B�Ǘ��V�X�e��
	�\��E�B���h�E�p�f�[�^�x�[�X�A�N�Z�X�֐��S ��`
*/

#ifndef	cdc_sqlH
#define	cdc_sqlH

#include "..\..\Common\dbs_mng.h"
#include "..\..\pms_def.h"

int		Cdc_Get_Dat( Csr, WCHAR *, WCHAR **, CDatabase *, PMS_MNG * );	// Gtting branch_info
int		Cdc_Sav_Dat( Csr, WCHAR **, CDatabase *, PMS_MNG * );			// Saving branch_info

#endif
