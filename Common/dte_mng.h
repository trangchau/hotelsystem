/*
			���t����֐���`
*/
#ifndef		dte_mngH
#define		dte_mngH

#define	is_kanji(x)	(((x)>=0x80&&(x)<=0xa0)||(x)>=0xe0)
#define	is_leap(x)	(((x)%400)==0||(((x)%4)==0&&((x)%100)!=0))
#define	day_dte(x)	(x)->yer,(x)->mth,(x)->day
#define	day_tim(x)	(x)->hor,(x)->min,(x)->sec

int		Get_Dte( Day * );						// �V�X�e�����t�擾
short	Get_Mxd( Day * );						// �ő���t�擾
void	Add_Dte( Day *, int );					// �������Z
int		Cmp_Dte( Day *, Day * );				// ���t��r
int		Cmp_Dtm( Day *, Day * );				// ���t������r
int		Cmp_Tim( Day *, Day * );				// ������r
int		Chk_Dte( Day * );						// ���t����������
int		Str_Dte( Day *, WCHAR * );				// �����񁨓��t�ϊ�
int		Str_Tim( Day *, WCHAR * );				// �����񁨎����ϊ�
int		Clc_Dys( Day *, Day * );				// ����(����)�v�Z
double	Clc_Tim( Day *, Day * );				// �o�ߎ��Ԍv�Z
int		Clc_Age( Day * );						// �N��v�Z
int		Get_Dow( Day * );						// �j���v�Z
WCHAR 	*Edt_Dte( WCHAR *, int, Day * );		// ���t�𕶎���ɕҏW
WCHAR 	*Edt_Tim( WCHAR *, int, Day * );		// �����𕶎���ɕҏW
WCHAR 	*Edt_Dtm( WCHAR *, int, Day * );		// ���t�����𕶎���ɕҏW

#endif
