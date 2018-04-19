/*
	線形リストバッファ管理構造の各種定義
*/
#ifndef		lnk_mngH
#define		lnk_mngH

typedef	struct	_tag_root	{
	struct	_tag_root	*nxt;		// 次の線形リストポインタ	
	struct	_tag_root	*prv;		// 前の線形リストポインタ
	int					cnt;		// メンバ総数
}Lnk, *Csr;

#define	LINKSIZE	(sizeof(Lnk))

#define	Rec_Of(t,p)		((t *)((p)+1))				// データ位置取得

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
