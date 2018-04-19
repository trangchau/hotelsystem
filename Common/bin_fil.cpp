#include <stdio.h>
#include <windows.h>

#include "Bin_Fil.h"

int			Bin_Red(					// �u���b�N�ǂݍ���
void		*buf,			   			// �ǂݍ��݃o�b�t�@
int			siz,			   			// �o�b�t�@�T�C�Y
HANDLE		fil )						// �t�@�C���n���h��
{
	ULONG			len;
	int				chk, err;

	chk = ReadFile( fil, buf, siz, &len, NULL );
	if ( !chk )	err = GetLastError();
	return( ( chk && ( !len || (ULONG)siz==len ) ) ? len: BIN_ERR );
}

int			Bin_Wrt(					// �u���b�N��������
void		*buf,			   			// �������݃o�b�t�@
int			siz,			   			// �o�b�t�@�T�C�Y
HANDLE		fil )						// �t�@�C���n���h��
{
	ULONG			len;
	int				chk;

	chk = WriteFile( fil, buf, siz, &len, NULL );
	return( ( chk && (ULONG)siz==len ) ? len: BIN_ERR );
}

HANDLE		Bin_Opn(					// �t�@�C���I�[�v��
WCHAR	*fnm,				   			// �t�@�C����
int		mod )				   			// �I�[�v�����[�h
{
	HANDLE				fil;
	int					opn, cre;

	opn = mod == BIN_RED ? GENERIC_READ: GENERIC_WRITE;
	cre = mod == BIN_RED ? OPEN_EXISTING: CREATE_ALWAYS;

	fil = CreateFile( fnm, opn, 0, NULL, cre, FILE_ATTRIBUTE_ARCHIVE, NULL );
	return( fil );
}

void		Bin_Cls(					// �t�@�C���N���[�Y
HANDLE		fil )						// �t�@�C���n���h��
{
	CloseHandle( fil );
}
