/*
	�z�e���v���p�e�B�Ǘ��V�X�e��
	�\��E�B���h�E�p�f�[�^�x�[�X�A�N�Z�X�֐��S ��`
*/

#ifndef	opi_sqlH
#define	opi_sqlH

#include "..\..\Common\dbs_mng.h"
#include "..\..\pms_def.h"

int		Opi_Get_Dat( OPR_INF *, WCHAR *, WCHAR **, CDatabase *, PMS_MNG * );				// Gtting operator_info
int		Opi_Sav_Dat( OPR_INF *, Day *, WCHAR **, CDatabase *, PMS_MNG * );			// Saving operator_info

#endif
