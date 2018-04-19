/*
	���`���X�g�o�b�t�@�Ǘ��\���̊e���`
*/

#include	<stdlib.h>
#include	<memory.h>
#include	"lnk_mng.h"

Csr		Jin_Lnk(				// ���`���X�g�̘A��(�A���Ώۂ̓R�s�[�łȂ��A�J�[�\�����������j
Csr		dst,					// �A���捪
Csr		src,					// �A���Ώۍ�
Csr		pos )					// NULL=�擪�Adst=�����A����ȊO��pos�̑O�ɘA��
{
	Csr			nxt;

	if ( !dst->nxt ){
		*dst = *src;
	}
	else if ( src->nxt ){
		if ( !pos ){
			nxt = dst->nxt;
			dst->nxt = src->nxt;
			src->prv->nxt = nxt;
			nxt->prv = src->prv;
		}
		else if ( pos==dst ){
			dst->prv->nxt = src->nxt;
			src->nxt->prv = dst->prv;
			dst->prv = src->prv;
		}
		else{
			nxt = pos->nxt;
			pos->nxt = src->nxt;
			src->prv->nxt = nxt;
			pos->nxt->prv = src->prv;
		}
		dst->cnt += src->cnt;
	}
	return( src->nxt );
}


Csr		Add_Lnk(				// �����o�[�̒ǉ�
Csr		rot,					// �ǉ��Ώۍ�
void	*dat,					// �ǉ����f�[�^
int		siz )					// �ǉ��f�[�^�T�C�Y
{
	Csr			add, prv;

	if ( add = (Csr)calloc( 1, LINKSIZE + siz ) ){
		if ( dat )	memcpy( Rec_Of(char, add), (char *)dat, siz );
		prv = rot->prv;
		add->nxt = NULL;
		add->prv = prv;

		rot->prv = add;
		if ( prv )		prv->nxt = add;
		else			rot->nxt = add;
		rot->cnt++;
	}
	return( add );
}

Csr		Ins_Lnk(				// �����o�[�̑}��
Csr		rot,					// �}���Ώۍ�
Csr		pos,					// �}���ꏊ�i���̒��O�ɑ}���ANULL���͖����ǉ��j
void	*dat,					// �}�����f�[�^
int		siz )					// �}���f�[�^�T�C�Y
{
	Csr			add, prv;

	if ( !pos || pos==rot ){
		add = Add_Lnk( rot, dat, siz );
	}
	else{
		if ( add = (Csr)calloc( 1, LINKSIZE + siz ) ){
			if ( dat )	memcpy( Rec_Of(char, add), (char *)dat, siz );
			prv = pos->prv;
			if ( prv )	prv->nxt = add;
			else		rot->nxt = add;
			add->prv = prv;
			add->nxt = pos;
			pos->prv = add;
			rot->cnt++;
		}
	}
	return( add );
}

Csr		Del_Lnk(				// �����o�[�̍폜
Csr		rot,					// �폜�Ώۍ�
Csr		pos )					// �폜�J�[�\��
{
	Csr			nxt=NULL, prv;

	if ( pos != rot ){
		if ( !pos )	pos = rot->nxt;
		if ( pos ){
			nxt = pos->nxt, prv = pos->prv;
			if ( nxt )	nxt->prv = prv;
			else		rot->prv = prv;
			if ( prv )	prv->nxt = nxt;
			else		rot->nxt = nxt;
			free( pos );
			rot->cnt--;
		}
	}
	return( nxt );
}

static	Csr	prg_lnk_(			// �����o�[����
Csr		rot,					// �Ώۍ�
Csr		pos )					// �����J�[�\��
{
	Csr			nxt, prv;

	if ( pos != rot ){
		if ( !pos )	pos = rot->nxt;
		if ( pos ){
			nxt = pos->nxt, prv = pos->prv;
			if ( nxt )	nxt->prv = prv;
			else		rot->prv = prv;

			if ( prv )	prv->nxt = nxt;
			else		rot->nxt = nxt;
			rot->cnt--;
		}
	}
	else{
		pos = NULL;
	}
	return( pos );
}

Csr		Mov_Lnk(				// �����o�[�ʒu�ړ�
Csr		drt,					// �ړ���f�[�^��
Csr		dst,					// �ړ���f�[�^�ʒu�i���̒��O�Ɉړ��j
Csr		srt,					// �ړ����f�[�^��
Csr		src )					// �ړ����f�[�^�J�[�\��
{
	Csr			prv;

	if ( src = prg_lnk_( srt, src ) ){
		if ( !dst ){
			prv = drt->prv;
			src->nxt = NULL;
			src->prv = prv;

			drt->prv = src;
			if ( prv )	prv->nxt = src;
			else		drt->nxt = src;
		}
		else{
			prv = dst->prv;
			if ( prv )	prv->nxt = src;
			else		drt->nxt = src;
			src->prv = prv;
			src->nxt = dst;
			dst->prv = src;
		}
		drt->cnt++;
		srt->cnt--;
	}
	return( src );
}

Csr		Nxt_Lnk(			// ���̃f�[�^�J�[�\���擾
Csr			rot,			// �擾�Ώۍ�
Csr			csr )			// ���݂̃J�[�\��
{
	if ( !csr )	csr = rot;
	return( csr->nxt );
}

Csr		Prv_Lnk(			// ���O�̃f�[�^�J�[�\���擾
Csr			rot,			// �擾�Ώۍ�
Csr			csr )			// ���݂̃J�[�\��
{
	if ( !csr )	csr = rot;
	return( csr->prv );
}

int		Del_All(			// �S�����o�[�̍폜
Csr			rot )			// �폜�Ώۍ�				
{
	Csr				csr, nxt;
	int				cnt = 0;

	if ( csr = rot->nxt ){
		while ( csr ){
			nxt = csr->nxt;
			free( csr );
			csr = nxt;
			cnt++;
		}
		rot->nxt = rot->prv = NULL;
		rot->cnt = 0;
	}
	return( cnt );
}

Csr		Idx_Lnk(			// �C���f�b�N�X�ɂ��擾
Csr			rot,			// �Ώۍ�
int			idx )			// �C���f�b�N�X�ԍ�				
{
	Csr				csr=NULL;

	if ( csr = rot->nxt ){
		while ( idx-- && csr )	csr = csr->nxt;
	}
	return( csr );
}

int		Cnt_Lnk(			// �����o�[�v��
Csr			rot )			// �v���Ώۍ�				
{
	return( rot->cnt );
}

int		Sot_Lnk(			// �����o�[����
Csr			rot,			// ����Ώۍ�
SORTCOMP	cmp )			// �����������֐�
{
	Csr			*tbl, *ptr, csr=NULL;
	int			idx, cnt;
	Lnk			tmp[1];

	if ( cnt = Cnt_Lnk( rot ) ){
		memset( tmp, 0, sizeof(tmp) );
		if ( ( ptr = tbl = (Csr *)calloc( cnt, sizeof(Csr) ) ) != NULL ){
			while( csr = Nxt_Lnk( rot, csr ) )	*ptr++ = csr;	// ���ق�ð��ٍ쐬
			qsort( tbl, cnt, sizeof(Csr), cmp );				// �������
			for ( idx=0, ptr=tbl; idx<cnt; idx++, ptr++ ){		// ��ď��Ɉꎞ����
				Mov_Lnk( tmp, NULL, rot, *ptr );				// �ް����ړ�
			}
			*rot = *tmp;										// �����e���R�s�[
			free( tbl );
		}
	}
	return( tbl || !cnt );
}
