/*
	線形リストバッファ管理構造の各種定義
*/

#include	<stdlib.h>
#include	<memory.h>
#include	"lnk_mng.h"

Csr		Jin_Lnk(				// 線形リストの連結(連結対象はコピーでなく、カーソル結合される）
Csr		dst,					// 連結先根
Csr		src,					// 連結対象根
Csr		pos )					// NULL=先頭、dst=末尾、それ以外はposの前に連結
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


Csr		Add_Lnk(				// メンバーの追加
Csr		rot,					// 追加対象根
void	*dat,					// 追加元データ
int		siz )					// 追加データサイズ
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

Csr		Ins_Lnk(				// メンバーの挿入
Csr		rot,					// 挿入対象根
Csr		pos,					// 挿入場所（この直前に挿入、NULL時は末尾追加）
void	*dat,					// 挿入元データ
int		siz )					// 挿入データサイズ
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

Csr		Del_Lnk(				// メンバーの削除
Csr		rot,					// 削除対象根
Csr		pos )					// 削除カーソル
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

static	Csr	prg_lnk_(			// メンバー抹消
Csr		rot,					// 対象根
Csr		pos )					// 抹消カーソル
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

Csr		Mov_Lnk(				// メンバー位置移動
Csr		drt,					// 移動先データ根
Csr		dst,					// 移動先データ位置（この直前に移動）
Csr		srt,					// 移動元データ根
Csr		src )					// 移動元データカーソル
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

Csr		Nxt_Lnk(			// 次のデータカーソル取得
Csr			rot,			// 取得対象根
Csr			csr )			// 現在のカーソル
{
	if ( !csr )	csr = rot;
	return( csr->nxt );
}

Csr		Prv_Lnk(			// 直前のデータカーソル取得
Csr			rot,			// 取得対象根
Csr			csr )			// 現在のカーソル
{
	if ( !csr )	csr = rot;
	return( csr->prv );
}

int		Del_All(			// 全メンバーの削除
Csr			rot )			// 削除対象根				
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

Csr		Idx_Lnk(			// インデックスによる取得
Csr			rot,			// 対象根
int			idx )			// インデックス番号				
{
	Csr				csr=NULL;

	if ( csr = rot->nxt ){
		while ( idx-- && csr )	csr = csr->nxt;
	}
	return( csr );
}

int		Cnt_Lnk(			// メンバー計数
Csr			rot )			// 計数対象根				
{
	return( rot->cnt );
}

int		Sot_Lnk(			// メンバー整列
Csr			rot,			// 整列対象根
SORTCOMP	cmp )			// 整列条件判定関数
{
	Csr			*tbl, *ptr, csr=NULL;
	int			idx, cnt;
	Lnk			tmp[1];

	if ( cnt = Cnt_Lnk( rot ) ){
		memset( tmp, 0, sizeof(tmp) );
		if ( ( ptr = tbl = (Csr *)calloc( cnt, sizeof(Csr) ) ) != NULL ){
			while( csr = Nxt_Lnk( rot, csr ) )	*ptr++ = csr;	// ｶｰｿﾙのﾃｰﾌﾞﾙ作成
			qsort( tbl, cnt, sizeof(Csr), cmp );				// ｸｲｯｸｿｰﾄ
			for ( idx=0, ptr=tbl; idx<cnt; idx++, ptr++ ){		// ｿｰﾄ順に一時根へ
				Mov_Lnk( tmp, NULL, rot, *ptr );				// ﾃﾞｰﾀを移動
			}
			*rot = *tmp;										// 根内容をコピー
			free( tbl );
		}
	}
	return( tbl || !cnt );
}
