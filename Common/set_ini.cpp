/*
	���������ʐݒ�t�@���N�V����
*/

#include <afx.h>
#include "sys_def.h"
#include "ini_inf.h"
#include "set_ini.h"
#include "scl_ble.h"
#include "cmn_fnc.h"
#include "dte_mng.h"
#include "sys_log.h"

int			Set_Cmm(			// �ʐM����������ݒ�
CMM_INF			*cmm,			// �i�[�̈�
CMM_INI			*ini )			// �������ݒ���
{
	int			chk;

	cmm->pot = ini->pot;			// �|�[�g�ԍ�
	cmm->brt = ini->brt;			// �{�[���[�g
	cmm->dtl = ini->dtl;			// �f�[�^��
	cmm->stl = ini->stl;			// �X�g�b�v�r�b�g��
	cmm->pty = (*(ini->pty))&0xdf;	// �p���e�B����
	
	chk  = cmm->brt==19200 || cmm->brt==9600 || cmm->brt==4800;
	chk |= cmm->brt== 2400 || cmm->brt==1200;
	chk &= cmm->dtl==8 || cmm->dtl==7;
	chk &= cmm->stl==1 || cmm->stl==2 || cmm->stl==15;
	chk &= cmm->pty=='E' || cmm->pty=='O' || cmm->pty=='N';

	return( chk ? IST_SUC: IST_INI );
}

int			Set_Sys(			// �V�X�e���\��������
SYS_INF			*sys,			// �i�[�̈�
SYS_INI			*ini )			// �������ݒ���
{
	sys->tst = ini->tso;			// �X���b�h�N���^�C���A�E�g�l
	sys->ted = ini->tto;			// �X���b�h�I���^�C���A�E�g�l

	return( IST_SUC );
}

static	int		dcd_pwd(		// �p�X���[�h����
WCHAR			*dst,			// �i�[�̈�
WCHAR			*pwd,			// �Í����p�X���[�h
int				siz,			// ��������̃T�C�Y
char			*key )			// �����L�[
{
	int			i, cnt, sts=IST_MEM;
	WCHAR		*tmp=NULL, *src = pwd;

	if ( !!pwd && !!wcslen( pwd ) ){
		cnt = (int)wcslen( pwd ) / 2;
		if ( tmp = (WCHAR *)malloc( wsizeof(cnt) ) ){
			for ( i=0; i<cnt; i++ ){
				*(tmp+i) = ( to_dgt(*src) << 4 ) | to_dgt(*(src+1));
				src += 2;
			}
			Dcd_Dat( dst, (unsigned long *)tmp, siz, key );
			dst[siz] = '\0'; 
			free( tmp );
			sts = IST_SUC;
		}
	}
	else{
		if ( !!pwd && !wcslen( pwd ) ){
			*dst = '\0';
			sts = IST_SUC;
		}
	}
	return( sts );
}

int		Get_Pwd(				// �p�X���[�h�擾
WCHAR			*dst,			// �i�[�̈�
WCHAR			*ini,			// ���������(�Í����p�X���[�h)
int				siz,			// ��������̃T�C�Y
char			*key )			// �����L�[
{
	int			chk= ( !ini || !siz ) ? IST_SUC: IST_INI;

	if ( chk != IST_SUC ){
		chk = dcd_pwd( dst, ini, siz, key );
	}
	return( chk );
}

int			Set_Dbs(			// �f�[�^�x�[�X�ڑ�������
DBS_INF			*dbs,			// �i�[�̈�
DBS_INI			*ini,			// �������ݒ���
char			*key )			// �p�X���[�h�Í������L�[
{
	wcscpy_s( dbs->dsn, sizeof(dbs->dsn), ini->dsn );	// �c�a�f�[�^�Z�b�g��
	wcscpy_s( dbs->sid, sizeof(dbs->sid), ini->sid );	// �c�a�T�[�r�X��
	wcscpy_s( dbs->uid, sizeof(dbs->uid), ini->uid );	// �c�a���[�U�[��

	return( Get_Pwd( dbs->pwd, ini->pwd, ini->pfl, key ) );
}

int			Set_Log(			// ���O�Ǘ�������
LOG_MNG			*log,			// �i�[�̈�
LOG_INI			*ini,			// �������ݒ���
WCHAR			*pgn )			// �v���O������(���O�m�[�h)
{
	Day			dte[1];
	int			idx;

	Get_Dte( dte );

	if ( ini->edt )	wcscpy_s( log->edt, sizeof(log->edt), ini->edt );
	if ( !( log->mxl = ini->mxl ) )	log->mxl = 1000;
	
	wcscpy_s( log->pth, sizeof(log->pth), ini->lgp ? ini->lgp: _T(".\\") );
	if ( log->pth && (int)wcslen(log->pth) ){
		idx = (int)wcslen(log->pth);
		while( idx-- && *(log->pth+idx)==_T('\\') )	*(log->pth+idx)='\0';
		if ( !wcslen(log->pth) )	wcscpy_s( log->pth, sizeof(log->pth)/wsizeof(1), _T("\\") );
	}
	Get_Lfn( log->exn, memcnt(log->exn), dte, LOG_RUN, pgn, log );
	Get_Lfn( log->rcn, memcnt(log->rcn), dte, LOG_RCV, pgn, log );
	Get_Lfn( log->sdn, memcnt(log->sdn), dte, LOG_SND, pgn, log );

	log->dys = ini->lkd ? ini->lkd: 30;
	_wcsupr_s( ini->lds, wcslen(ini->lds)+1 );
	log->dsp = !wcscmp(ini->lds, _T("DELETE" ) ) ? LDS_DEL: LDS_KEP;

	if ( log->dsp==LDS_DEL )	DeleteFile( log->exn );

	return( IST_SUC );
}
