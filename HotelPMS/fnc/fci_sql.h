/*
	�z�e���v���p�e�B�Ǘ��V�X�e��
	�\��E�B���h�E�p�f�[�^�x�[�X�A�N�Z�X�֐��S ��`
*/

#ifndef	fci_sqlH
#define	fci_sqlH

#include "..\..\Common\dbs_mng.h"
#include "..\..\pms_def.h"

int		Fci_Get_Dat( Csr, WCHAR *, WCHAR *, WCHAR **, CDatabase *, PMS_MNG * );	// Gtting branch_info
//int		Fci_Sav_Dat( Csr, int, Day *, WCHAR **, CDatabase *, PMS_MNG * );		// Saving branch_info
int		Fci_Sav_Dat( Csr, WCHAR **, CDatabase *, PMS_MNG * );						// Saving branch_info

#endif
