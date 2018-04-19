/*
	内部データ⇔外部Ｉ／Ｆデータ
	変換
*/

#include <stdio.h>
#include "tri_def.h"
#include "inr_otr.h"


static	void	set_ivl(	// 整数値を文字列フィールドに設定
char		*dst,			// 文字列領域
int			siz,			// 格納領域サイズ
int			val )			// 元データ
{
	char		buf[16];

	sprintf_s( buf, sizeof(buf), "%0*d", siz, val );
	memcpy( dst, buf, siz );
}

static	void	set_yym(	// 年月値を文字列フィールドに設定
char		*dst,			// 文字列領域
int			siz,			// 格納領域サイズ
int			val )			// 元データ
{
	char		buf[16];

	sprintf_s( buf, sizeof(buf), "%04d%s%02d",
			val/100, siz==7 ? "=": "", val%100 );
	memcpy( dst, buf, siz );
}

static	void	set_svl(	// 文字列値設定	
char		*dst,			// 格納領域	
char		*src,			// 元データ
int			siz )			// 元データ領域サイズ	
{
	memcpy( dst, src, siz );
	*(dst+siz) = '\0';
}

void	Inr_Otr(			// 内部データ→外部データ変換	
ORG_EQP		*dst,			// 格納領域
INR_EQP		*src )			// 元データ
{
	int			idx;

	memset( dst, ' ', sizeof(ORG_EQP) );

	memcpy( dst->org, src->org, sizeof(dst->org) );
	set_ivl( dst->cno, sizeof(dst->cno), src->cno );
	set_ivl( dst->prt, sizeof(dst->prt), src->prt );
	memcpy( dst->prs, src->prs, sizeof(dst->prs) );
	memcpy( dst->axn, src->axn, sizeof(dst->axn) );
	memcpy( dst->mlt, src->mlt, sizeof(dst->mlt) );
	set_yym( dst->mld, sizeof(dst->mld), src->mld );
	memcpy( dst->iso, src->iso, sizeof(dst->iso) );
	set_ivl( dst->gcm, sizeof(dst->gcm), src->gcm );
	memcpy( dst->gcn, src->gcn, sizeof(dst->gcn) );
	set_yym( dst->gcd, sizeof(dst->gcd), src->gcd );
	set_ivl( dst->rad, sizeof(dst->rad), src->rad );
	for ( idx=0; idx<memcnt(dst->jco); idx++ ){
		set_ivl( (dst->jco+idx)->mkr, sizeof((dst->jco+idx)->mkr), (src->jco+idx)->mkr );
		set_yym( (dst->jco+idx)->mkd, sizeof((dst->jco+idx)->mkd), (src->jco+idx)->mkd );
		memcpy( (dst->jco+idx)->mkn, (src->jco+idx)->mkn, sizeof((dst->jco+idx)->mkn) );
	}
	for ( idx=0; idx<memcnt(dst->jb); idx++ ){
		set_ivl( (dst->jb+idx)->mkr, sizeof((dst->jb+idx)->mkr), (src->jb+idx)->mkr );
		set_yym( (dst->jb+idx)->mkd, sizeof((dst->jb+idx)->mkd), (src->jb+idx)->mkd );
		memcpy( (dst->jb+idx)->mkn, (src->jb+idx)->mkn, sizeof((dst->jb+idx)->mkn) );
	}
	for ( idx=0; idx<memcnt(dst->whl); idx++ ){
		set_ivl( (dst->whl+idx)->mkr, sizeof((dst->whl+idx)->mkr), (src->whl+idx)->mkr );
		set_yym( (dst->whl+idx)->mkd, sizeof((dst->whl+idx)->mkd), (src->whl+idx)->mkd );
		memcpy( (dst->whl+idx)->mln, (src->whl+idx)->mln, sizeof((dst->whl+idx)->mln) );
		memcpy( (dst->whl+idx)->sqn, (src->whl+idx)->sqn, sizeof((dst->whl+idx)->sqn) );
	}
	for ( idx=0; idx<memcnt(dst->bkd); idx++ ){
		set_ivl( dst->bkd[idx][0].mkr, sizeof(dst->bkd[idx][0].mkr), src->bkd[idx][0].mkr );
		set_yym( dst->bkd[idx][0].mkd, sizeof(dst->bkd[idx][0].mkd), src->bkd[idx][0].mkd );
		memcpy( dst->bkd[idx][0].mkn, src->bkd[idx][0].mkn, sizeof(dst->bkd[idx][0].mkn) );
		set_ivl( dst->bkd[idx][1].mkr, sizeof(dst->bkd[idx][1].mkr), src->bkd[idx][1].mkr );
		set_yym( dst->bkd[idx][1].mkd, sizeof(dst->bkd[idx][1].mkd), src->bkd[idx][1].mkd );
		memcpy( dst->bkd[idx][1].mkn, src->bkd[idx][1].mkn, sizeof(dst->bkd[idx][1].mkn) );
	}
	set_ivl( dst->cnm, sizeof(dst->cnm), src->cnm );
	set_yym( dst->cnd, sizeof(dst->cnd), src->cnd );
	memcpy( dst->cnn, src->cnn, sizeof(dst->cnn) );
	memcpy( dst->cnk, src->cnk, sizeof(dst->cnk) );
	set_ivl( dst->etd, sizeof(dst->etd), src->etd );
	set_ivl( dst->tek, sizeof(dst->tek), src->tek );
	set_ivl( dst->ted, sizeof(dst->ted), src->ted );
	memcpy( dst->tep, src->tep, sizeof(dst->tep) );
	set_ivl( dst->chd, sizeof(dst->chd), src->chd );
	set_ivl( dst->aek, sizeof(dst->aek), src->aek );
	set_ivl( dst->ack, sizeof(dst->ack), src->ack );
	memcpy( dst->aax, src->aax, sizeof(dst->aax) );
}

int		chg_dt7(			// '-'年月読替え	
char		*src )			// 元データ
{
	int			val;
	char		buf[10];

	set_svl( buf, src, 4 );
	val = atoi( buf ) * 100;
	set_svl( buf, src+5, 2 );
	val += atoi( buf );

	return( val );
}

static	int		str_val(	// 文字列→数値変換
char		*str,			// 文字列データ
int			siz )			// 文字列サイズ
{
	char		buf[20];

	memcpy( buf, str, siz );
	*(buf+siz) = '\0';
	return( atoi( buf ) );
}

void	Otr_Inr(			// 外部データ→内部データ変換	
INR_EQP		*dst,			// 格納領域
ORG_EQP		*src )			// 元データ
{
	int			idx;

	set_svl( dst->org, src->org, sizeof(src->org) );
	dst->cno = str_val( src->cno, sizeof(src->cno) );
	dst->prt = str_val( src->prt, sizeof(src->prt)  );
	set_svl( dst->prs, src->prs, sizeof(src->prs) );
	set_svl( dst->axn, src->axn, sizeof(src->axn) );
	set_svl( dst->mlt, src->mlt, sizeof(src->mlt) );
	dst->mld = chg_dt7( src->mld );
	set_svl( dst->iso, src->iso, sizeof(src->iso) );
	dst->gcm = str_val( src->gcm, sizeof(src->gcm) );
	set_svl( dst->gcn, src->gcn, sizeof(src->gcn) );
	dst->gcd = chg_dt7( src->gcd );
	dst->rad = str_val( src->rad, sizeof(src->rad) );
	for ( idx=0; idx<memcnt(dst->jco); idx++ ){
		(dst->jco+idx)->mkr = str_val( (src->jco+idx)->mkr, sizeof((src->jco+idx)->mkr) );
		(dst->jco+idx)->mkd = str_val( (src->jco+idx)->mkd, sizeof((src->jco+idx)->mkd) );
		set_svl( (dst->jco+idx)->mkn, (src->jco+idx)->mkn, sizeof((src->jco+idx)->mkn) );
	}
	for ( idx=0; idx<memcnt(dst->jb); idx++ ){
		(dst->jb+idx)->mkr = atoi( (src->jb+idx)->mkr );
		(dst->jb+idx)->mkd = atoi( (src->jb+idx)->mkd );
		set_svl( (dst->jb+idx)->mkn, (src->jb+idx)->mkn, sizeof((src->jb+idx)->mkn) );
	}
	for ( idx=0; idx<memcnt(dst->whl); idx++ ){
		dst->whl[idx].mkr = str_val( src->whl[idx].mkr, sizeof(src->whl[idx].mkr) );
		dst->whl[idx].mkd = str_val( src->whl[idx].mkd, sizeof(src->whl[idx].mkd) );
		set_svl( dst->whl[idx].mln, src->whl[idx].mln, sizeof(src->whl[idx].mln) );
		set_svl( dst->whl[idx].sqn, src->whl[idx].sqn, sizeof(src->whl[idx].sqn) );
	}
	for ( idx=0; idx<memcnt(dst->bkd); idx++ ){
		dst->bkd[idx][0].mkr = str_val( src->bkd[idx][0].mkr, sizeof(src->bkd[idx][0].mkr) );
		dst->bkd[idx][0].mkd = str_val( src->bkd[idx][0].mkd, sizeof(src->bkd[idx][0].mkd) );
		set_svl( dst->bkd[idx][0].mkn, src->bkd[idx][0].mkn, sizeof(src->bkd[idx][0].mkn) );
		dst->bkd[idx][1].mkr = str_val( src->bkd[idx][1].mkr, sizeof(src->bkd[idx][0].mkr) );
		dst->bkd[idx][1].mkd = str_val( src->bkd[idx][1].mkd, sizeof(src->bkd[idx][1].mkd) );
		set_svl( dst->bkd[idx][1].mkn, src->bkd[idx][1].mkn, sizeof(src->bkd[idx][1].mkn) );
	}
	dst->cnm = str_val( src->cnm, sizeof(src->cnm) );
	dst->cnd = str_val( src->cnd, sizeof(src->cnd) );
	set_svl( dst->cnn, src->cnn, sizeof(src->cnn) );
	set_svl( dst->cnk, src->cnk, sizeof(src->cnk) );
	dst->etd = str_val( src->etd, sizeof(src->etd) );
	dst->tek = str_val( src->tek, sizeof(src->tek) );
	dst->ted = str_val( src->ted, sizeof(src->ted) );
	set_svl( dst->tep, src->tep, sizeof(src->tep) );
	dst->chd = str_val( src->chd, sizeof(src->chd) );
	dst->aek = str_val( src->aek, sizeof(src->aek) );
	dst->ack = str_val( src->ack, sizeof(src->ack) );
	set_svl( dst->aax, src->aax, sizeof(src->aax) );
}

INR_BRG		*Get_Brg(		// 内輪情報取得
BRG_INF			*brg )		// 元情報
{
	INR_BRG		*inr=NULL;

	if ( *(brg->mkr)!=' ' && *(brg->mkn)!=' ' ){
		inr = (INR_BRG *)malloc( sizeof(INR_BRG) );
		if ( inr ){
			inr->mkr = str_val( brg->mkr, sizeof(brg->mkr) ); 
			inr->mkd = str_val( brg->mkd, sizeof(brg->mkd) ); 
			set_svl( inr->mkn, brg->mkn, sizeof(brg->mkn) ); 
		}
	}
	return( inr );
}


