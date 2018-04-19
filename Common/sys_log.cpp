/*
	�V�X�e�����O����
*/

#include <afx.h>
#include <stdio.h>
#include "sys_log.h"
#include "dte_mng.h"
#include "log_trd.h"

static	WCHAR	Ftl_msg[2048];

WCHAR	*Edt_Lfn(				// ���O�t�@�C�����ҏW
WCHAR		*fnm,				// ���t�@�C����
Day			*dte )				// ���t
{
	int			siz;
	WCHAR		drv[_MAX_DRIVE], dir[_MAX_DIR], nam[_MAX_FNAME], ext[_MAX_EXT];
	WCHAR		*dst;

	_wsplitpath_s( fnm, drv, sizeof(drv)/wsizeof(1), dir, sizeof(dir)/wsizeof(1), nam, sizeof(nam)/wsizeof(1), ext, sizeof(ext)/wsizeof(1) );
	siz = (int)wcslen( drv ) + (int)wcslen( dir ) + (int)wcslen( nam ) + (int)wcslen( ext ) + 80;
	if ( dst = (WCHAR *)malloc( wsizeof(siz) ) ){
		if ( dte ){
			swprintf_s( dst, siz, _T("%s%s%s%04d%02d%02d%s"),
				drv, dir, nam, dte->yer, dte->mth, dte->day, ext );
		}
		else{
			swprintf_s( dst, siz, _T("%s%s%s????????%s"), drv, dir, nam, ext );
		}
	}
	return( dst );
}

WCHAR	*Get_Lfn(			// ���O�t�@�C�����擾
WCHAR		*dst,				// �i�[�̈�
int			siz,				// Size of output buffer
Day			*dte,				// ���t
int			knd,				// ���O���
WCHAR		*pgn,				// �v���O��������
LOG_MNG		*log )				// ���O�X���b�h�p�����[�^
{
	WCHAR		*fnm;

	switch( knd ){
	case LOG_RUN:	swprintf_s( dst, siz, _T("%s\\%s.log"), log->pth, pgn );	break;
	case LOG_RCV:	swprintf_s( dst, siz, _T("%s\\COM.log"), log->pth );		break;
	case LOG_SND:	swprintf_s( dst, siz, _T("%s\\COM.log"), log->pth );		break;
	}
	if ( dte && !!( fnm = Edt_Lfn( dst, dte ) ) ){
		wcscpy_s( dst, MAX_PATH, fnm );
		free( fnm );
	}
	return( dst );
}

WCHAR	*Log_Knd(			// ���O��ʖ��擾
int			knd )			// ��ʃR�[�h
{
	WCHAR		*kst;

	switch( knd ){
	case LOG_RUN:	kst = _T("�ʏ�");	break;		// ���s���O
	case LOG_OPE:	kst = _T("����");	break;		// ���샍�O
	case LOG_ERR:	kst = _T("�ُ�");	break;		// �G���[���O
	default:		kst = _T("�s��");	break;
	}
	return( kst );	
}

void	Log_Err(			// ���O�t�@�C���G���[�\��
LOG_MNG		*mng,			// ���O�Ǘ��\��
int			ose )			// �n�^�r�G���[
{
	swprintf_s( Ftl_msg, memcnt(Ftl_msg), _T("���O�t�@�C���G���[! \n")
			_T("file name=[%s]\n error code=[%d]"), mng->exn, ose );
	MessageBox( NULL, Ftl_msg, _T("���O�Ǘ��\��"), MB_ICONEXCLAMATION|MB_OK );
}

static	int		put_log(	// ���O���ڏo��(�������[�o�b�t�@��K�v�Ƃ��܂���)
int			knd,			// ���O���
LOG_MNG		*log,			// ���O�Ǘ��\��
Day			*dte,			// ��������
WCHAR		*fmt,			// ���O�t�H�[�}�b�g
... )						// �ψ������X�g
{
	FILE		*fp=NULL;
	Day			*dpt, tmp[1];
	va_list		argp;
	char		*mdl="������";

	if ( log->opn ){
		if ( !( dpt = dte ) )	Get_Dte( tmp ), dpt = tmp;
		LOG_EXC
		if ( !fp )	Log_Err( log, GetLastError() );
	}
	return( !!fp );
}

int		Opn_Log(			// ���O�I�[�v��
LOG_MNG		*mng )			// ���O�Ǘ��\��
{
	InitializeCriticalSection( mng->crs );
	mng->opn = TRUE;
	mng->opn = put_log( LOG_RUN, mng, NULL, _T("�V�X�e�����O���J�݂��܂����B") );
	return( mng->opn );
}

void	Cls_Log(			// ���O�N���[�Y
LOG_MNG		*mng )			// ���O�Ǘ��\��
{
	if ( mng->opn ){
		put_log( LOG_RUN, mng, NULL, _T("�V�X�e�����O������܂����B") );
		DeleteCriticalSection( mng->crs );
		mng->opn = FALSE;
	}
}

void	Sys_Log(			// �V�X�e�����O���s
int			mdn,			// ���W���[���ԍ�
int			knd,			// ���O���
SYS_MNG		*mng,			// �V�X�e���Ǘ��\��
int			usr,			// ���[�U�[��`�f�[�^
WCHAR		*fmt,			// ���O�t�H�[�}�b�g
... )						// �ψ������X�g
{
#ifndef	NO_LOGGING
	WCHAR		*msg, *mdl=_T("�ŏ��");
	LOG_QUE		que[1];
	va_list		argp;
	Day			*dpt=que->dte;
	TRD_INF		*trd=mng->trd+mng->log->tdn;
	LOG_PRM		*prm;
	LOG_MNG		*log = mng->log;
	FILE		*fp=NULL;
//	Csr			csr;
	int			tdl=FALSE;

	Get_Dte( que->dte );

	if ( mng->log->opn ){
		if ( msg = (WCHAR *)malloc( wsizeof(LOG_SIZ) ) ){
			if ( *(mng->trd+mdn)->nam )	mdl = (mng->trd+mdn)->nam;
			que->mdn = mdn;
			wcscpy_s( que->nam, sizeof(que->nam)/wsizeof(1), mdl );
			que->msg = msg;
			que->knd = knd;
			que->usr = usr;
			va_start( argp, fmt );
			wvsprintf( msg, fmt, argp );
			va_end( argp );
			if ( msg ){
//				EnterCriticalSection( mng->lbx );
//				if ( csr = Add_Lnk( mng->lgm, NULL, (int)strlen(msg)+1+sizeof(que) ) ){
//					*Rec_Of(LOG_QUE, csr) = *que;
//					strcpy_s( (char *)(Rec_Of(LOG_QUE, csr)+1), 1024, msg );
//				}
//				LeaveCriticalSection( mng->lbx );
			}
			if ( tdl = trd->tcm->sts!=TRD_NON ){	// ���O�X���b�h����
				prm = (LOG_PRM *)trd->prm;
				if ( Wrt_Rbf( que, prm->que ) ){
					ReleaseSemaphore( Rbf_Smp( prm->que ), 1, NULL );
				}
				else{
					free( msg );
					msg = NULL;
				}
			}
			else{
				free( msg );
				msg = NULL;
			}
		}
		if ( !tdl || !msg ){		// ���O�X���b�h�������A���b�Z�[�W�o�b�t�@�m�ێ��s
			LOG_EXC		// ���ڃ��M���O���܂��I
			if ( !fp )	Log_Err( mng->log, GetLastError() );
		}
	}
/*
	char		txt[2048];
	int			idx;

	idx  = sprintf( txt, "%04d/%02d/%02d %02d:%02d:%02d ",
				dpt->yer, dpt->mth, dpt->day, dpt->hor, dpt->min, dpt->sec );
	if ( knd!=LOG_SND && knd!=LOG_RCV ){
		idx += sprintf( txt+idx, "[%-4s]:", Log_Knd( knd ) );
		idx += sprintf( txt+idx, "[%-10s]:", mdl );
	}
	va_start( argp, fmt );
	vsprintf( txt+idx, fmt, argp );
	va_end( argp );
	EnterCriticalSection( mng->lbx );
	if ( csr = Add_Lnk( mng->lgm, NULL, strlen(txt)+1+sizeof(knd) ) ){
		*Rec_Of(int, csr) = knd;
		strcpy( (char *)(Rec_Of(int, csr)+1), txt );
	}
	LeaveCriticalSection( mng->lbx );
*/
#endif
}

int		Edt_Lgh(				// ���O�w�b�_�[�ҏW
char			*dst,			// �i�[�̈�
LOG_QUE			*log )			// ���O�f�[�^
{
	int			idx;
	Day			*dpt=log->dte;

	idx  = sprintf_s( dst, 4096, "%04d/%02d/%02d %02d:%02d:%02d ",
				dpt->yer, dpt->mth, dpt->day, dpt->hor, dpt->min, dpt->sec );
	idx += sprintf_s( dst+idx, 4096, "[%-4s]:", Log_Knd( log->knd ) );
	idx += sprintf_s( dst+idx, 4096, "[%-10s]:", log->nam );
	return( idx );
}


static	int		is_nums(		// ���l�����񌟒�
WCHAR		*str,				// ���蕶����
int			siz )				// ��r������
{
	WCHAR	*ptr = (WCHAR *)str;

	if ( !!siz ){
		while( siz-- && *ptr ){
			if ( *ptr<_T('0') || *ptr>_T('9') )	break;
			ptr++;
		}
	}
	return( siz==-1 );
}

static	int		to_num(			// �����ɕϊ�
WCHAR		*ptr,				// ������|�C���^
int			siz )				// ������T�C�Y
{
	WCHAR		buf[10];

	memcpy( buf, ptr, wsizeof(siz) );
	*(buf+siz) = '\0';
	return( _wtoi( buf ) );
}

static	int		get_fdt(		// �t�@�C�����t�擾
Day			*dst,				// ���t�i�[�̈�
WCHAR		*fnm,				// �t�@�C����
WCHAR		*srn )				// ���t�@�C����
{
	int			chk;
	WCHAR		*ptr, drv[_MAX_DRIVE], dir[_MAX_DIR], nam[_MAX_FNAME], ext[_MAX_EXT];

	_wsplitpath_s( fnm, drv, sizeof(drv)/wsizeof(1), dir, sizeof(dir)/wsizeof(1), nam, sizeof(nam)/wsizeof(1), ext, sizeof(ext)/wsizeof(1) );

	if ( chk = wcslen(fnm) == (wcslen(nam)+wcslen(ext)+wsizeof(8)) ){
		if ( chk = !memcmp( fnm, nam, wsizeof(wcslen(nam)) ) ){
			ptr = fnm + wcslen(nam);
			if ( chk = is_nums( ptr, 8 ) ){
				dst->yer = to_num( ptr+0, 4 );
				dst->mth = to_num( ptr+4, 2 );
				dst->day = to_num( ptr+6, 2 );
				chk = Chk_Dte( dst );
			}
		}
	}
	return( chk );
}

void		Del_Log(			// �s�v���O�폜
int			dys,				// �ۗL����(��)
WCHAR		*bsn )				// ���t�@�C����
{
	WIN32_FIND_DATA		fnd[1];
	HANDLE				hdl;
	WCHAR				*fnm;
	int					lop;
	Day					fdt[1], dte[1], *dpt;
	Lnk					rot[1], *csr;

	memset( rot, 0, sizeof(rot) );

	Get_Dte( dte );
	Add_Dte( dte, -dys );

	if ( fnm = Edt_Lfn( bsn, NULL ) ){
		if ( lop = ( hdl = FindFirstFile( fnm, fnd ) ) != INVALID_HANDLE_VALUE ){
			while( lop ){
				if ( get_fdt( fdt, fnd->cFileName, bsn ) ){
					if ( Cmp_Dte( dte, fdt ) > 0 )	Add_Lnk( rot, fdt, sizeof(fdt) );
				}
				lop = FindNextFile( hdl, fnd );
			}
			FindClose( hdl );
		}
		free( fnm );
	}
	csr = Nxt_Lnk( rot, NULL );
	while( csr ){
		dpt = Rec_Of(Day, csr);
		if ( fnm = Edt_Lfn( bsn, dpt ) ){
			DeleteFile( fnm );
			free( fnm );
		}
		csr = Del_Lnk( rot, csr );
	}
}

