/*
	���`���X�g�o�b�t�@�Ǘ��\���̊e���`
*/
#ifndef		lnk_mngH
#define		lnk_mngH

typedef	struct	_tag_root	{
	struct	_tag_root	*nxt;		// ���̐��`���X�g�|�C���^	
	struct	_tag_root	*prv;		// �O�̐��`���X�g�|�C���^
	int					cnt;		// �����o����
}Lnk, *Csr;

#define	LINKSIZE	(sizeof(Lnk))

#define	Rec_Of(t,p)		((t *)((p)+1))				// �f�[�^�ʒu�擾

typedef	int (__cdecl *SORTCOMP)(const void *elem1, const void *elem2); 

Csr		Jin_Lnk( Csr, Csr, Csr );
Csr		Add_Lnk( Csr, void *, int );
Csr		Ins_Lnk( Csr, Csr, void *, int );
Csr		Del_Lnk( Csr, Csr );
Csr		Mov_Lnk( Csr, Csr, Csr, Csr );
Csr		Nxt_Lnk( Csr, Csr );
Csr		Prv_Lnk( Csr, Csr );
int		Del_All( Csr );
Csr		Idx_Lnk( Csr, int );
int		Cnt_Lnk( Csr );
int		Sot_Lnk( Csr, SORTCOMP );

#endif
