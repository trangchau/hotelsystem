/*
	�V�X�e�����O����
*/

#ifndef	sys_logH
#define	sys_logH

#include <windows.h>
#include "sys_def.h"
#include "log_trd.h"

#define	LOG_RUN			1			// ���샍�O
#define	LOG_OPE			2			// ���샍�O
#define	LOG_ERR			3			// �G���[���O
#define	LOG_SND			101			// ���M���O
#define	LOG_RCV			102			// ��M���O

#define	LOG_SIZ		2048			// ���O���b�Z�[�W�T�C�Y

#define	LOG_EXC	\
		EnterCriticalSection( log->crs );\
		if ( !_wfopen_s( &fp, log->exn, _T("a+") ) ){\
			fwprintf( fp, _T("%04d/%02d/%02d %02d:%02d:%02d "),\
				dpt->yer, dpt->mth, dpt->day, dpt->hor, dpt->min, dpt->sec );\
			fwprintf( fp, _T("[%-4s]:"), Log_Knd( knd ) );\
			fwprintf( fp, _T("[%-10s]:"), mdl );\
			va_start( argp, fmt );\
			vfwprintf( fp, fmt, argp );\
			va_end( argp );\
			fwprintf( fp, _T("\n") );\
			fclose( fp );\
		}\
		LeaveCriticalSection( log->crs );

WCHAR	*Edt_Lfn( WCHAR *, Day * );									// ���O�t�@�C�����ҏW
WCHAR	*Get_Lfn( WCHAR *, int, Day *, int, WCHAR *, LOG_MNG * );	// ���O�t�@�C�����擾
WCHAR	*Log_Knd( int );											// ���O��ʖ��擾
int		Opn_Log( LOG_MNG * );										// ���O�I�[�v��
void	Cls_Log( LOG_MNG * );										// ���O�N���[�Y
void	Log_Err( LOG_MNG *, int );									// ���O�t�@�C���G���[�\��
void	Sys_Log( int, int, SYS_MNG *, int, WCHAR *, ... );			// �V�X�e�����O���s
int		Edt_Lgh( WCHAR *, LOG_QUE * );								// ���O�w�b�_�[�ҏW
void	Del_Log( int, WCHAR * );									// �s�v���O�폜

#endif
