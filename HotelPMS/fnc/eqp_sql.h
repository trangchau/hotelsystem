/*
	�z�e���v���p�e�B�Ǘ��V�X�e��
	�\��E�B���h�E�p�f�[�^�x�[�X�A�N�Z�X�֐��S ��`
*/

#ifndef	eqp_sqlH
#define	eqp_sqlH

#include "..\..\Common\dbs_mng.h"
#include "..\..\pms_def.h"

int		Eqp_Get_Typ( Csr, EQP_TYP *, WCHAR **, CDatabase *, PMS_MNG * );// Gtting equipment_type
int		Eqp_Sav_Typ( Csr, WCHAR **, CDatabase *, PMS_MNG * );			// save equipment_typss
int		Eqp_Get_Inf( Csr, EQP_INF *, WCHAR **, CDatabase *, PMS_MNG * );// Gtting equipment_info
int		Eqp_Sav_Inf( Csr, WCHAR **, CDatabase *, PMS_MNG * );			// save equipment_infos

#endif
