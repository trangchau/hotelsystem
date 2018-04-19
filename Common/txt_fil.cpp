/*
	�e�L�X�g�t�@�C���A�N�Z�X�֐�
*/

#include <afx.h>
#include <stdio.h>
#include <windows.h>

#include "txt_fil.h"

int			Txt_Red(					// �P�s�ǂݍ���
WCHAR		*buf,			   			// �ǂݍ��݃o�b�t�@
int			siz,			   			// �o�b�t�@�T�C�Y
HANDLE		fil )						// �t�@�C���n���h��
{
	static	WCHAR	chr;
	int				cnt=0;
	ULONG			len;
	BOOL			chk;

	do{
		chk = ReadFile( fil, &chr, 1, &len, NULL );
		if ( chk==TRUE && len!=0 ){
			if ( cnt+1 > siz )	break;
			if ( chr != '\r' )	*buf++ = chr;
			++cnt;
		}
		else{
			break;
		}
	}while( chr != '\n' );
	*buf = '\0';
	return( cnt );
}

int			Txt_Wrt(					// �P�s��������
WCHAR		*buf,			   			// �������݃o�b�t�@
int			siz,			   			// �o�b�t�@�T�C�Y
HANDLE		fil )						// �t�@�C���n���h��
{
	ULONG			ttl=0, len;
	WCHAR			*ptr;
	BOOL			chk;

	do{
		if ( ( ptr = wcschr( buf, _T('\n') ) ) != NULL ){
			siz = (int)(ptr - buf);
		}
		else{
			siz = (int)wcslen(buf);
		}
		if ( siz ){
			chk = WriteFile( fil, buf, siz, &len, NULL );
			ttl += len;
		}
		else{
			chk = TRUE;
		}
		if ( ptr && chk ){
			chk = WriteFile( fil, _T("\r\n"), 2, &len, NULL );
			ttl += len;
			buf = ptr+1;
		}
		if ( !chk )		break;
	}while( ptr && *buf );
	return( chk ? ttl: -1 );
}

HANDLE		Txt_Opn(					// �t�@�C���I�[�v��
WCHAR	*fnm,				   			// �t�@�C����
int		mod )				   			// �I�[�v�����[�h
{
	HANDLE				fil;
	int					opn, cre;

	switch( mod ){
	case TXT_RED:	opn = GENERIC_READ;
					cre = OPEN_EXISTING;
					break;
	case TXT_WRT:	opn = GENERIC_WRITE;
					cre = CREATE_ALWAYS;
					break;
	case TXT_APD:	opn = GENERIC_WRITE;
					cre = OPEN_ALWAYS;
					break;
	}

	fil = CreateFile( fnm, opn, 0, NULL, cre, FILE_ATTRIBUTE_ARCHIVE, NULL );
	if ( fil != TOP_ERR && mod == TXT_APD ){
		SetFilePointer( fil, 0, NULL, FILE_END );
	}
	return( fil );
}

void		Txt_Cls(					// �t�@�C���N���[�Y
HANDLE		fil )						// �t�@�C���n���h��
{
	CloseHandle( fil );
}
