/*
	���i�Ǘ��V�X�e�����ʊ֐�
*/

#include <stdlib.h>
#include <stdio.h>
#include "bsc_def.h"
#include "dte_mng.h"
#include "tri_def.h"

#define	is_prsfact(x)		( (x)=='7' || (x)=='8' || (x)=='G' || (x)=='H' )
#define	is_digit(x)			( (x)>='0' && (x)<='9' )
#define	is_alpha(x)			( (x)>='A' && (x)<='Z' )

static	int		dgt_str(		// ���l�����񌟒�
char		*str )				// �����Ώە�����
{
	while( *str ){
		if ( !is_digit(*str) )	break;
		str++;
	}
	return( !(*str) );
}

static	int		dte_str(		// ���l�����񌟒�
char		*str )				// �����Ώە�����
{
	int		pos=0, chk;

	while( *str ){
		if ( pos==4 || pos==7 )	chk = *str=='/' || *str=='.';
		else					chk = is_digit(*str);
		if ( !chk )	break;
		pos++;
		str++;
	}
	return( !(*str) );
}

static int		str_ivl(		// �����񁨐��l�ϊ�
char		*str,				// ������
int			siz )				// �T�C�Y
{
	char		buf[20];

	siz = siz<sizeof(buf) ? siz: sizeof(buf-1);
	memcpy( buf, str, siz );
	buf[siz] = '\0';

	return( atoi( buf ) );
}


int		Axn_Chk(				// �Ԏ��Ǘ��ԍ��������`�F�b�N
char		*axn )				// �Ԏ��Ǘ��ԍ�	
{
	int			chk;
	
	chk  = (int)strlen(axn) == AXL_LEN;
	chk &= is_prsfact( *(axn+1) );
	chk &= is_digit( *axn );
	chk &= dgt_str( axn+2 );
	
	return( chk );
}

int		Whl_Chk(				// �ԗ֊Ǘ��ԍ��������`�F�b�N
char		*axn )				// �Ԏ��Ǘ��ԍ�	
{
	return( TRUE );
}

int		Bkd_Chk(				// �u���[�L�f�B�X�N�Ǘ��ԍ��������`�F�b�N
char		*bdn )				// �u���[�L�f�B�X�N�Ǘ��ԍ�	
{
	int			chk, len;

	len = (int)strlen(bdn);
	chk  = len>=5 && len<=11;
	chk &= *(bdn+3)=='-';
	chk &= is_alpha( *(bdn+0) ) && is_alpha( *(bdn+2) );
	chk &= is_digit( *(bdn+1) ) && dgt_str( bdn+4 );

	return( chk );
}

int		Brg_Chk(				// ���֊Ǘ��ԍ��������`�F�b�N
char		*brn )				// ���֊Ǘ��ԍ�	
{
	return( TRUE );
}

int		Tri_S2D(				// ���͂������t������𐳋K�ϊ�
char		*inp,				// ���̓o�b�t�@
int			siz )				// ���̓o�b�t�@�T�C�Y
{
	int			chk, len;
	int			yer, mth, day, mxd;
	int			max[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	len = (int)strlen(inp);
	chk = len==8 || len==10;
	chk &= len==8 ? dgt_str(inp): dte_str( inp );
	if ( chk ){
		yer = str_ivl( inp, 4 );
		mth = str_ivl( inp+( len==8 ? 4: 5 ), 2 );
		day = str_ivl( inp+( len==8 ? 6: 8 ), 2 );
		chk  = yer >= 1990;
		chk &= mth>0 && mth<13;
		if ( chk ){
			if ( mth==2 )	mxd=is_leap(yer) ? 29: 28;
			else			mxd = max[mth-1];
			chk &= day>0 && day<=mxd;
		}
	}
	if ( chk ){
		mxd = atoi(inp);
		sprintf_s( inp, siz, "%04d/%02d/%02d", yer, mth, day );
	}
	return( chk ? mxd: -1 );
}

void	edt_dte(			// ���t�f�[�^�����f�[�^�֕ϊ�
char		*dst,			// �i�[�̈�
int			siz,			// �i�[�̈�T�C�Y
int			dte )			// ���t�f�[�^
{
	if ( dte ){
		sprintf_s( dst, siz, "%04d/%02d/%02d",
			dte/10000, (dte/100)%100, dte%100 );
	}
	else{
		*dst = '\0';
	}
}

