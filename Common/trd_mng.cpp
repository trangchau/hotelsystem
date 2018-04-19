/*
	スレッド管理処理
*/

#include <afx.h>
#include <windows.h>
#include "sys_def.h"
#include "trd_mng.h"
#include "cmn_fnc.h"
#include "sys_log.h"

static	int	bgn_trd(		// スレッド開始処理
TRD_ENT		ent,			// スレッドエントリーアドレス
LPVOID		prm,			// スレッドパラメータ
TRD_CMN		*tcm,			// スレッド共通構造
int			wat )			// 起動を待ち時間（ms 0=待たない）
{
	int			chk;

	tcm->cst = TRS_SUC;

	chk  = !!( tcm->stt = ::CreateEvent( NULL, FALSE, FALSE, NULL ) );
	chk &= !!( tcm->cmp = ::CreateEvent( NULL, TRUE, FALSE, NULL ) );	// 完了は手動リセット(自滅用)
	chk &= !!( tcm->stp = ::CreateEvent( NULL, FALSE, FALSE, NULL ) );
	chk &= !!( tcm->stc = ::CreateEvent( NULL, FALSE, FALSE, NULL ) );
	chk &= !!( tcm->ctn = ::CreateEvent( NULL, FALSE, FALSE, NULL ) );
	chk &= !!( tcm->ctc = ::CreateEvent( NULL, FALSE, FALSE, NULL ) );
	chk &= !!( tcm->off = ::CreateEvent( NULL, FALSE, FALSE, NULL ) );
	chk &= !!( tcm->ofc = ::CreateEvent( NULL, FALSE, FALSE, NULL ) );
	chk &= !!( tcm->end = ::CreateEvent( NULL, FALSE, FALSE, NULL ) );
	chk &= !!( tcm->abt = ::CreateEvent( NULL, FALSE, FALSE, NULL ) );
	if ( chk ){
/*
	tcm->stt = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	tcm->cmp = ::CreateEvent( NULL, TRUE, FALSE, NULL );	// 完了は手動リセット(自滅用)
	tcm->stp = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	tcm->stc = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	tcm->ctn = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	tcm->ctc = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	tcm->off = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	tcm->ofc = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	tcm->end = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	tcm->abt = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	if ( tcm->cmp && tcm->stp && tcm->end ){
*/
		tcm->trd = CreateThread( NULL, 0, ent, prm, 0, &tcm->tid );
		if ( !tcm->trd ){
			tcm->cst = TRS_OSE;
			tcm->oss = GetLastError();
			DBG_OUT( __FILE__, __LINE__, "CreateThreadエラー OSERROR=%d", tcm->oss );
		}
		else{
			if ( wat ){
				switch( WaitForSingleObject( tcm->stt, wat ) ){
				case WAIT_OBJECT_0:	break;
				case WAIT_TIMEOUT:	tcm->cst = TRS_TMO;
									DBG_OUT( __FILE__, __LINE__,
										"スレッド起動タイムアウトエラー" );
									break;
				default:			tcm->cst = TRS_OSE;
									tcm->oss = GetLastError();
									DBG_OUT( __FILE__, __LINE__,
										"WaitForSingleObjectエラー OSERROR=%d", tcm->oss );
									break;
				}
			}
		}
		if ( tcm->cst != TRS_SUC ){
			if ( tcm->cmp )	CloseHandle( tcm->cmp );
			if ( tcm->stp )	CloseHandle( tcm->stp );
			if ( tcm->stc )	CloseHandle( tcm->stc );
			if ( tcm->ctn )	CloseHandle( tcm->ctn );
			if ( tcm->ctc )	CloseHandle( tcm->ctc );
			if ( tcm->off )	CloseHandle( tcm->off );
			if ( tcm->ofc )	CloseHandle( tcm->ofc );
			if ( tcm->end )	CloseHandle( tcm->end );
			if ( tcm->stt )	CloseHandle( tcm->stt );
			if ( tcm->abt )	CloseHandle( tcm->abt );
		}
	}
	else{
		tcm->cst = TRS_MEM;
		tcm->oss = GetLastError();
		DBG_OUT( __FILE__, __LINE__, "CreateEventエラー OSERROR=%d", tcm->oss );
	}
	return( tcm->cst );
}

static	void	end_trd(	// スレッド終了
int			tot,			// スレッド終了待ちタイムアウト値(ms)
TRD_CMN		*tcm )			// スレッド共通構造
{
	if ( tcm && tcm->trd && tcm->tid ){
		SetEvent( tcm->end );
		if ( WaitForSingleObject( tcm->cmp, tot ) == WAIT_TIMEOUT ){
			DBG_OUT( __FILE__, __LINE__, "スレッド終了待ちタイムアウト index=%d", tcm->tix );
			TerminateThread( tcm->trd, 0 );
		}
		if ( tcm->cmp )	CloseHandle( tcm->cmp );
		if ( tcm->stp )	CloseHandle( tcm->stp );
		if ( tcm->stc )	CloseHandle( tcm->stc );
		if ( tcm->ctn )	CloseHandle( tcm->ctn );
		if ( tcm->ctc )	CloseHandle( tcm->ctc );
		if ( tcm->off )	CloseHandle( tcm->off );
		if ( tcm->ofc )	CloseHandle( tcm->ofc );
		if ( tcm->end )	CloseHandle( tcm->end );
		if ( tcm->stt )	CloseHandle( tcm->stt );
		if ( tcm->abt )	CloseHandle( tcm->abt );
		CloseHandle( tcm->trd );
	}
}

int		Trd_Stt(				// スレッド開始処理
TST_PRM		*tst,				// 起動スレッド情報
int			src,				// 起動元スレッド番号
void		*mnp,				// システム管理情報
void		*usr )				// ユーザー定義構造
{
	int			cst;
	SYS_MNG		*mng=(SYS_MNG *)mnp;
	TRD_INF		*trd=mng->trd+tst->tdn;

	trd->prm = calloc( 1, tst->siz );
	trd->tcm->ctf = tst->rcv;

	if ( trd->prm ){
		if ( ( cst = tst->stp( trd->prm, mng, trd->tcm, usr ) ) == TRS_SUC ){
			WStr_NCpy( trd->nam, tst->nam, sizeof(trd->nam)/wsizeof(1) );
			if ( bgn_trd( tst->ent, trd->prm, trd->tcm, mng->sys->tst ) == TRS_SUC ){
				cst = trd->tcm->cst;
				trd->atr = 0;
				trd->tcm->tix = tst->tdn;
				trd->psz = tst->siz;
				DBG_OUT( __FILE__, __LINE__, "TDN=%-2d Name=%s TID=0x%x",
						trd->tcm->tix, tst->nam, trd->tcm->tid );
			}
			else{
				*trd->nam = '\0';
				Sys_Log( src, LOG_ERR, mng, 0,
					_T("%sスレッド起動失敗 cst=%d ose=%d "),
							tst->nam, trd->tcm->cst, trd->tcm->oss ); 
				cst = trd->tcm->cst;
				free( trd->prm );
				trd->prm = NULL;
			}
		}
	}
	return( cst );
}

void	Trd_End(				// スレッド終了処理
int			tdn,				// 停止スレッド番号
int			siz,				// パラメータサイズ
CST_TPM		cst,				// パラメータ破棄関数
int			src,				// 停止元スレッド番号
void		*mnp )				// システム管理情報
{
	SYS_MNG		*mng=(SYS_MNG *)mnp;
	TRD_INF		*trd=mng->trd+tdn;

	end_trd( mng->sys->ted, trd->tcm );
	if ( cst && trd->prm ){
		cst( trd->prm );
		free( trd->prm );
	}
	memset( trd, 0, sizeof(TRD_INF) );
}
