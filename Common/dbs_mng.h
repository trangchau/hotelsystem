/*
		�f�[�^�x�[�X�A�N�Z�X�Ǘ� ��`
*/
#ifndef	dbs_mngH
#define	dbs_mngH

#include <afx.h>
#include <afxdb.h>
#include "bsc_def.h"
#include "dte_mng.h"

#define	TRN_STT		0						// �g�����U�N�V�����J�n
#define	TRN_CMT		1						// �g�����U�N�V��������
#define	TRN_RBK		2						// �g�����U�N�V�����j��

#define	FLD_INT		1						// �����t�B�[���h
#define	FLD_SGL		2						// �P���x�����t�B�[���h
#define	FLD_DBL		3						// �{���x�����t�B�[���h
#define	FLD_STR		4						// ������t�B�[���h
#define	FLD_WST		5						// WideChar������t�B�[���h
#define	FLD_AST		6						// ANSI������t�B�[���h
#define	FLD_ADS		7						// ������t�B�[���h(strcat)
#define	FLD_DAY		8						// ���t�t�B�[���h
#define	FLD_DTM		9						// ���t�E�����^�t�B�[���h
#define	FLD_TIM		10						// �����^�t�B�[���h
#define	FLD_STI		11						// string -> integer( result of function on query )
#define	FLD_WSI		12						// string -> integer( result of function on query )

#define	QRY_END		1						// ����ȏ�f�[�^�Ȃ�
#define	QRY_MEM		2						// �������[�s��
#define	QRY_DBS		3						// �f�[�^�x�[�X�G���[
#define	QRY_NML		0						// �܂��f�[�^����
#define	QRY_USR		4						// ���[�U�[��`�G���[

#define	FLD_INT_SIZ(x)	sizeof((x)[0])
#define	FLD_DBL_SIZ(x)	sizeof((x)[0])
#define	FLD_WST_SIZ(x)	(memcnt(x)-1)
#define	FLD_DAY_SIZ(x)	sizeof((x)[0])
#define	FLD_DTM_SIZ(x)	sizeof((x)[0])
#define	FLD_TIM_SIZ(x)	sizeof((x)[0])

#define	DBF_SIZ_INT			2				// DB Field size of int
#define	DBF_SIZ_DBL			2				// DB Field size of double precision floating decimal
#define	DBF_SIZ_WST(x)		((x)+2)			// DB Field size of WCHAR
#define	DBF_SIZ_DAY			2				// DB Field size of date
#define	DBF_SIZ_DTM			2				// DB Field size of date and time
#define	DBF_SIZ_TIM			2				// DB Field size of only time

typedef	struct	{							// �c�a���[��`
	void	*ptr;							// �f�[�^�i�[��
	WCHAR	*fln;							// �t�B�[���h(�J����)��
	int		typ;							// �t�B�[���h�^�C�v
	int		siz;							// �t�B�[���h�T�C�Y(char=sizeof, WCHAR=memcnt)
}Dbs_fld;

typedef	struct	{							// Multiple fielss definition
	Dbs_fld		*fld;						// Field definition pointer
	int			flc;						// Field definition count;
}Mlt_dbf;

void		*Fld_Ptr( void *, int );		// �t�B�[���h�|�C���^�擾
int			Nul_Fld( void *, int );			// NULL����
void		Set_Nul( void *, int, int );	// NULL�ݒ�
int			Dbs_Cnc( CDatabase *, WCHAR *, WCHAR *, WCHAR *, WCHAR *, WCHAR ** );
void		Dbs_Cut( CDatabase * );
CRecordset	*Dbs_Qry( CDatabase *, WCHAR *, WCHAR ** );
int			Dbs_Get( Dbs_fld *, WCHAR **, int, CRecordset * );
int			Dbs_MGt( Mlt_dbf *, WCHAR **, int, CRecordset * );
void		Dbs_Cls( CRecordset * );
int			Dbs_Upd( CDatabase *, WCHAR *, WCHAR ** );
int			Dbs_Trn( WCHAR **, CDatabase *, int );

typedef	WCHAR	(*INT_EDT_FNC)( WCHAR *, int, int );

#define	EDT_DBF_WST(s,d)		( Nul_Fld((s), FLD_WST) ? (d): (WCHAR *)Fld_Ptr((s), FLD_WST) )
#define	EDT_DBF_DAY(d,l,s)		( Nul_Fld((s), FLD_DAY) ? _T(""): Edt_Dte(d,l,(Day *)Fld_Ptr((s), FLD_DAY) ) )
#define	EDT_DBF_TIM(d,l,s)		( Nul_Fld((s), FLD_DAY) ? _T(""): Edt_Tim(d,l,(Day *)Fld_Ptr((s), FLD_TIM) ) )
#define	EDT_DBF_INT(d,l,s,f)	( Nul_Fld((s), FLD_INT) ? _T(""): f( d, l, *((int *)Fld_Ptr(s, FLD_INT) ) ) )
#define	EDT_DBF_DBL(d,l,s,m,f)	( Nul_Fld((s), FLD_DBL) ? _T(""): f( d, l, m, *((double *)Fld_Ptr(s, FLD_DBL) ) ) )

#define	DBS_INT_VAL(x)			*((int *)Fld_Ptr((x),FLD_INT))
#define	DBS_DBL_VAL(x)			*((double *)Fld_Ptr((x),FLD_DBL))

#define	DBS_DAY_PTR(x)			((Day *)Fld_Ptr((x),FLD_DAY))
#define	DBS_DTM_PTR(x)			((Day *)Fld_Ptr((x),FLD_DAY))
#define	DBS_TIM_PTR(x)			((Day *)Fld_Ptr((x),FLD_DAY))
#define	DBS_INT_PTR(x)			((int *)Fld_Ptr((x),FLD_INT))
#define	DBS_DBL_PTR(x)			((double *)Fld_Ptr((x),FLD_DBL))
#define	DBS_WST_PTR(x)			((WCHAR *)Fld_Ptr((x),FLD_WST))
#define	DBS_STR_PTR(x)			((char *)Fld_Ptr((x),FLD_STR))

#define	DBS_INT_CPY(d,s)		*DBS_INT_PTR(d)=*DBS_INT_PTR(s),Set_Nul(d,FLD_INT,Nul_Fld(s,FLD_INT))
#define	DBS_DBL_CPY(d,s)		*DBS_DBL_PTR(d)=*DBS_DBL_PTR(s),Set_Nul(d,FLD_DBL,Nul_Fld(s,FLD_DBL))
#define	DBS_WST_CPY(d,s)		wcscpy_s(DBS_WST_PTR(d),FLD_WST_SIZ(d),!Nul_Fld(s,FLD_WST) ? DBS_WST_PTR(s): _T("")),Set_Nul(d,FLD_WST,Nul_Fld(s,FLD_WST))
#define	DBS_DAY_CPY(d,s)		*DBS_DAY_PTR(d)=*DBS_DAY_PTR(s),Set_Nul(d,FLD_DAY,Nul_Fld(s,FLD_DAY))
#define	DBS_DTM_CPY(d,s)		*DBS_DTM_PTR(d)=*DBS_DTM_PTR(s),Set_Nul(d,FLD_DTM,Nul_Fld(s,FLD_DTM))
#define	DBS_TIM_CPY(d,s)		*DBS_TIM_PTR(d)=*DBS_TIM_PTR(s),Set_Nul(d,FLD_TIM,Nul_Fld(s,FLD_TIM))

#define	DBS_INT_SET(d,s)		Set_Nul(d,FLD_INT,!(s) );if(!!(s))*DBS_INT_PTR(d)=*(s)
#define	DBS_DBL_SET(d,s)		Set_Nul(d,FLD_DBL,!(s) );if(!!(s))*DBS_DBL_PTR(d)=*(s)
#define	DBS_WST_SET(d,s)		Set_Nul(d,FLD_WST,!(s) );if(!!(s))wcscpy_s(DBS_WST_PTR(d),FLD_WST_SIZ(d),(s))
#define	DBS_DAY_SET(d,s)		Set_Nul(d,FLD_DAY,!(s) );if(!!(s))*DBS_DAY_PTR(d)=*(s)
#define	DBS_DTM_SET(d,s)		Set_Nul(d,FLD_DTM,!(s) );if(!!(s))*DBS_DTM_PTR(d)=*(s)

//int		(*fnc)( void *, int, CWnd *, DWORD );	// �����֐�

#endif
