/*
	リングバッファ管理
*/

#include <afx.h>
#include <stdio.h>
#include <windows.h>
#include "bsc_def.h"
#include "bin_fil.h"
#include "lnk_mng.h"
#include "rng_buf.h"

#define	nxt_buf(x,b)	( ((x)+1)==b->cnt ? 0:((x)+1) )
#define	MAX_BUF			256
#define	Cnt_Of(x)		( (x)->red > (x)->wrt ? \
						( (x)->cnt - ( (x)->red - (x)->wrt ) ): \
						( (x)->wrt - (x)->red ) )

typedef	struct	{					// リングバッファ管理構造
	WCHAR				nam[10];	// 名称
	DWORD				siz;		// 要素サイズ
	DWORD				cnt;		// 要素数
	DWORD				red;		// 読み込みポインタ
	DWORD				wrt;		// 書き込みポインタ
	char				*buf;		// バッファポインタ
	HANDLE				smp;		// 通知セマフォ
	RBF_HDL				hdl;		// ハンドル
	RBF_HDL				old;		// 旧ハンドル
	RBF_HDL				ref;		// 参照用旧ハンドル
	int					sav;		// 保存要否			
	CRITICAL_SECTION	csc[1];		// 排他アクセス制御
}RNG_BUF;

static	CRITICAL_SECTION	Crt_[1];
static	RNG_BUF				*Tbl_[MAX_BUF];
static	int					Init_;
static	int					Cnt_;

static	int		etr_tbl(		// 管理テーブルに追加
RNG_BUF			*rbf )			// 対象リングバッファ
{
	int			idx;

	for ( idx=0; idx<MAX_BUF; idx++ ){
		if ( !(*(Tbl_+idx)) ){
			*(Tbl_+idx) = rbf;
			break;
		}
	}
	return( idx<MAX_BUF ? idx+1: 0 );
}

static	RNG_BUF	*sch_rbf(		// リングバッファ検索
RBF_HDL		hdl,				// リングバッファハンドル
int			cst )				// 消しこみ有無
{
	int			idx;
	RNG_BUF		*rbf=NULL;

	for ( idx=0; idx<MAX_BUF; idx++ ){
		if ( !!(*(Tbl_+idx)) && (*(Tbl_+idx))->hdl == hdl ){
			rbf = *(Tbl_+idx);
			break;
		}
	}
	if ( idx<MAX_BUF && cst )	*(Tbl_+idx) = NULL;
	return( rbf );
}

HANDLE		Rbf_Smp(			// セマフォハンドル取得
RBF_HDL		hdl )				// リングバッファハンドル
{
	RNG_BUF		*rbf;
	HANDLE		ret=NULL;

	EnterCriticalSection( Crt_ );
	rbf = sch_rbf( hdl, FALSE );
	LeaveCriticalSection( Crt_ );

	if ( !!rbf )	ret=rbf->smp;
	return( ret );
}

RBF_HDL		Cre_Rbf(			// リングバッファ生成
DWORD		siz,				// リングバッファサイズ
int			cnt,				// 個数
int			sav )				// 緊急保存要否
{
	RNG_BUF			*rbf;
	int				hdl=-1, chk;

	if ( !Init_ ){
		Cnt_ = 0;
		InitializeCriticalSection( Crt_ );
		Init_ = TRUE;
	}

	if ( chk = !!( rbf = (RNG_BUF *)calloc( 1, sizeof(RNG_BUF) ) ) ){
		InitializeCriticalSection( rbf->csc );
		rbf->siz = siz;
		rbf->cnt = cnt;
		rbf->sav = sav;
		rbf->smp = CreateSemaphore( NULL, 0, cnt, NULL );
		rbf->buf = (char *)calloc( cnt, siz );
		chk = !!rbf->buf && !!rbf->smp;
		if ( chk ){
			EnterCriticalSection( Crt_ );
			if ( chk = !!( rbf->hdl = etr_tbl( rbf ) ) )	Cnt_++;
			LeaveCriticalSection( Crt_ );
		}
		if( !chk )	{
			if ( rbf->buf )	free( rbf->buf );
			if ( rbf->smp )	CloseHandle( rbf->smp );
			free( rbf );
			rbf = NULL;
		}
	}
	return( chk ? rbf->hdl: NULL );
}

void		Cst_Rbf(			// リングバッファ破棄
RBF_HDL		hdl )				// 破棄対象リングバッファハンドル
{
	RNG_BUF		*rbf;

	EnterCriticalSection( Crt_ );
	if ( rbf = sch_rbf( hdl, TRUE ) ){
		EnterCriticalSection( rbf->csc );
		if ( rbf->smp )	CloseHandle( rbf->smp );
		if ( rbf->buf )	free( rbf->buf );
		LeaveCriticalSection( rbf->csc );
		DeleteCriticalSection( rbf->csc );
		free( rbf );
		--Cnt_;
	}
	LeaveCriticalSection( Crt_ );
	if ( !Cnt_ ){
		DeleteCriticalSection( Crt_ );
		Init_ = FALSE;
	}
}

bool	Wrt_Rbf(				// リングバッファ書き込み
void		*dat,				// 書き込みデータポインタ
RBF_HDL		hdl )				// 対象リングバッファハンドル
{
	DWORD		nxt;
	bool		ret;
	char		*wpt;
	RNG_BUF		*rbf;

	EnterCriticalSection( Crt_ );
	rbf = sch_rbf( hdl, FALSE );
	LeaveCriticalSection( Crt_ );

	if ( ret = !!rbf ){
		EnterCriticalSection( rbf->csc );
		nxt = nxt_buf( rbf->wrt, rbf );
		if ( ret = ( nxt != rbf->red ) ){		// false = バッファフル
			wpt = rbf->buf + rbf->siz * rbf->wrt;
			memcpy( wpt, (char *)dat, rbf->siz );
			rbf->wrt = nxt;
		}
		LeaveCriticalSection( rbf->csc );
	}
	return( ret );
}

bool	Red_Rbf(				// リングバッファ読み込み
void		*dat,				// 読み込みデータポインタ
RBF_HDL		hdl )				// 対象リングバッファハンドル
{
	bool		ret;
	char		*rpt;
	RNG_BUF		*rbf;

	EnterCriticalSection( Crt_ );
	rbf = sch_rbf( hdl, FALSE );
	LeaveCriticalSection( Crt_ );

	if ( ret = !!rbf ){
		EnterCriticalSection( rbf->csc );
		if ( ret = ( rbf->wrt != rbf->red ) ){		// false = バッファエンプティ
			rpt = rbf->buf + rbf->siz * rbf->red;
			memcpy( (char *)dat, rpt, rbf->siz );
			rbf->red = nxt_buf( rbf->red, rbf );
		}
		LeaveCriticalSection( rbf->csc );
	}
	return( ret );
}

bool	Ref_Rbf(				// リングバッファ参照(デキュー無しの取得)
void		*dat,				// データ格納ポインタ
int			idx,				// 参照インデックス
RBF_HDL		hdl )				// 対象リングバッファハンドル
{
	bool		ret;
	char		*rpt;
	DWORD		pos;
	RNG_BUF		*rbf;

	EnterCriticalSection( Crt_ );
	rbf = sch_rbf( hdl, FALSE );
	LeaveCriticalSection( Crt_ );

	if ( ret = !!rbf ){
		pos = rbf->red;
		if ( ret = idx < (int)Cnt_Of( rbf ) ){
			EnterCriticalSection( rbf->csc );
			while ( idx-- )		pos = nxt_buf( pos, rbf );
			if ( ret = ( rbf->wrt != rbf->red ) ){		// false = バッファエンプティ
				rpt = rbf->buf + rbf->siz * pos;
				memcpy( (char *)dat, rpt, rbf->siz );
			}
			LeaveCriticalSection( rbf->csc );
		}
	}
	return( ret );
}

int		Cnt_Rbf(				// バッファリング数取得
RBF_HDL		hdl )				// 対象リングバッファハンドル
{
	RNG_BUF		*rbf;
	int			ret=0;

	EnterCriticalSection( Crt_ );
	rbf = sch_rbf( hdl, FALSE );
	LeaveCriticalSection( Crt_ );

//	if ( !!rbf ){
//		ret = rbf->red > rbf->wrt ? 
//			( rbf->cnt - ( rbf->red - rbf->wrt ) ): ( rbf->wrt - rbf->red ) );
//	}
//	return( ret );
	return( !!rbf ? Cnt_Of(rbf): 0 );
}

int		Sav_Rbf(				// リングバッファ保存
WCHAR		*fnm )				// ファイル名
{
	int			idx, chk=!Init_;
	RNG_BUF		*rbf;
	HANDLE		fil;

	if ( !chk ){				// 初期化されている場合
		if ( chk = ( fil = Bin_Opn( fnm, BIN_WRT ) ) != BOP_ERR ){
			EnterCriticalSection( Crt_ );
			for ( idx=0; chk && idx<MAX_BUF; idx++ ){
				if ( !!( rbf = *(Tbl_+idx) ) && !!rbf->sav ){
					swprintf_s( rbf->nam, memcnt(rbf->nam), _T("RBF%04d"), idx+1 );
					if ( chk = Bin_Wrt( rbf, sizeof(RNG_BUF), fil ) != BIN_ERR ){
						chk = Bin_Wrt( rbf->buf, rbf->siz*rbf->cnt, fil ) != BIN_ERR;
					}
				}
			}
			LeaveCriticalSection( Crt_ );
			Bin_Cls( fil );
		}
	}
	return( chk );
}

int		Rst_Rbf(				// リングバッファ復元
WCHAR		*fnm )				// ファイル名
{
	int			chk;
	RNG_BUF		tmp[1], *rbf;
	HANDLE		fil;

	if ( chk = !Init_ ){			// 初期化されていない場合
		Cnt_ = 0;
		InitializeCriticalSection( Crt_ );
		Init_ = TRUE;
	}

	if ( chk = ( fil = Bin_Opn( fnm, BIN_RED ) ) != BOP_ERR ){
		while( chk && Bin_Red( tmp, sizeof(tmp), fil ) != BIN_ERR ){
			if ( chk = !!( rbf = (RNG_BUF *)calloc( 1, sizeof(RNG_BUF) ) ) ){
				InitializeCriticalSection( rbf->csc );
				rbf->siz = tmp->siz;
				rbf->cnt = tmp->cnt;
				rbf->sav = tmp->sav;
				rbf->red = tmp->red;
				rbf->wrt = tmp->wrt;
				rbf->smp = CreateSemaphore( NULL, 0, tmp->cnt, NULL );
				rbf->buf = (char *)calloc( tmp->cnt, tmp->siz );
				chk = !!rbf->buf && !!rbf->smp;
				if ( chk ){
					chk = Bin_Red( rbf->buf, rbf->siz*rbf->cnt, fil ) != BIN_ERR;
				}
				if ( chk ){
					rbf->old = tmp->hdl;
					EnterCriticalSection( Crt_ );
					if ( chk = !!( rbf->hdl = etr_tbl( rbf ) ) )	Cnt_++;
					LeaveCriticalSection( Crt_ );
				}
				if ( !chk ){
					if ( rbf->buf )	free( rbf->buf );
					if ( rbf->smp )	CloseHandle( rbf->smp );
					free( rbf );
					rbf = NULL;
				}
			}
		}
		Bin_Cls( fil );
	}
	return( chk );
}

RBF_HDL		Get_Rbf(			// 復旧リングバッファ新ハンドル取得
RBF_HDL			old )			// 旧リングバッファハンドル
{
	int			idx;
	RNG_BUF		*rbf;
	RBF_HDL		hdl=NULL;

	if ( Init_ && old ){
		EnterCriticalSection( Crt_ );
		for ( idx=0; !hdl && idx<MAX_BUF; idx++ ){
			if ( !!( rbf = *(Tbl_+idx) ) && rbf->ref==old ){
				hdl = rbf->hdl;
			}
		}
		LeaveCriticalSection( Crt_ );
	}
	return( hdl );
}

RBF_HDL		Xch_Rbf(			// リングバッファ入れ替え取得
RBF_HDL			old,			// 旧リングバッファハンドル
RBF_HDL			crn )			// 現リングバッファハンドル
{
	int			idx;
	RNG_BUF		*orb=NULL, *nrb=NULL;

	if ( Init_ && old ){
		EnterCriticalSection( Crt_ );
		for ( idx=0; !nrb && idx<MAX_BUF; idx++ ){
			if ( !!( orb = *(Tbl_+idx) ) && orb->old==old ){
				if ( !!( nrb = *(Tbl_+crn-1) ) ){
					if ( nrb->buf )	free( nrb->buf );
					nrb->buf = orb->buf;
					nrb->siz = orb->siz;
					nrb->cnt = orb->cnt;
					nrb->sav = orb->sav;
					nrb->red = orb->red;
					nrb->wrt = orb->wrt;
					nrb->ref = orb->old;
					DeleteCriticalSection( orb->csc );
					if ( orb->smp )	CloseHandle( orb->smp );
					free( orb );
					*(Tbl_+idx) = NULL;
					--Cnt_;
					for ( idx=0; idx<(int)Cnt_Of(nrb); idx++ ){
						SetEvent( nrb->smp );
					}
				}
				break;
			}
		}
		LeaveCriticalSection( Crt_ );
	}
	if ( !!orb && !nrb )	Cst_Rbf( old );
	return( nrb ? nrb->hdl: NULL );
}

void	Rbf_Prg(			// 復旧リングバッファ破棄
void )
{
	int			idx;
	RNG_BUF		*rbf;

	if ( Init_ ){
		for ( idx=0; idx<MAX_BUF; idx++ ){
			if ( !!( rbf = *(Tbl_+idx) ) && !!rbf->old ){
				Cst_Rbf( rbf->hdl );
			}
		}
	}
}
