/*
	ログ管理タスク
*/

#include <afx.h>
#include <stdio.h>
#include "sys_log.h"
#include "dte_mng.h"
#include "bin_fil.h"
#include "trd_mng.h"
#include "log_trd.h"

static	void	log_exc(		// ロギング実行
LOG_QUE		*req,				// 要求データ
LOG_PRM		*prm )				// ログスレッドパラメータ
{
	FILE		*fp;
	LOG_MNG		*log = prm->mng->log;
	Day			*dpt = req->dte;
	WCHAR		*fnm;

	switch( req->knd ){
	case LOG_RCV:	fnm = prm->mng->log->rcn;	break;
	case LOG_SND:	fnm = prm->mng->log->sdn;	break;
	default:		fnm = prm->mng->log->exn;	break;
	}

	EnterCriticalSection( log->crs );
	if ( !_wfopen_s( &fp, fnm, _T("a+") ) ){
		fwprintf( fp, _T("%04d/%02d/%02d %02d:%02d:%02d "),
				dpt->yer, dpt->mth, dpt->day, dpt->hor, dpt->min, dpt->sec );
		if ( req->knd!=LOG_SND && req->knd!=LOG_RCV ){
			fwprintf( fp, _T("[%-4s]:"), Log_Knd( req->knd ) );
			fwprintf( fp, _T("[%-10s]:"), req->nam );
		}
		fwprintf( fp, _T("%s\n"), req->msg );
		fclose( fp );
	}
	free( req->msg );
	LeaveCriticalSection( log->crs );
}

static	DWORD WINAPI log_trd(	// メインスレッド
LPVOID	usr )					// スレッドパラメータ
{
	LOG_PRM		*prm = (LOG_PRM *)usr;
	SYS_MNG		*mng = prm->mng;
	int			sts, end=FALSE, idx=0, knd[] = { LOG_RUN, LOG_RCV, LOG_SND };
	WCHAR		fnm[MAX_PATH];
	LOG_QUE		req[1];
	HANDLE		evt[2];

	prm->tcm->cst = TRS_SUC;
	prm->tcm->sts = TRD_RUN;
	evt[0] = prm->tcm->end;			// スレッド終了要求イベント
	evt[1] = Rbf_Smp( prm->que );	// ログ実行要求イベント

	Sys_Log( prm->tdn, LOG_RUN, mng, 0, _T("ログスレッド起動完了") );

	SetEvent( prm->tcm->stt );

	while( !end ){
		sts = WaitForMultipleObjects( 2, evt, FALSE, 1000 );
		switch( sts ){
		case WAIT_OBJECT_0:		end = TRUE;
								break;
		case WAIT_OBJECT_0+1:	if ( Red_Rbf( req, prm->que ) ){
									log_exc( req, prm );
								}
								break;
		case WAIT_TIMEOUT:		Get_Lfn( fnm, memcnt(fnm), NULL, knd[idx++], prm->mng->pgn, prm->mng->log );
								Del_Log( prm->mng->log->dys, fnm );
								if ( idx==memcnt(knd) )	idx=0;
								break;
		}
	}
	while( Red_Rbf( req, prm->que ) )	log_exc( req, prm );
	Sys_Log( prm->tdn, LOG_RUN, mng, 0, _T("ログスレッド閉鎖完了") );
	SetEvent( prm->tcm->cmp );
	return 0;
}

static	int		set_prm(		// スレッドパラメータ設定
void		*ptr,				// パラメータポインタ
void		*mnp,				// システム管理情報
TRD_CMN		*tcm,				// スレッド管理共通情報
void		*usr )				// ユーザー定義構造
{
	LOG_PRM		*prm = (LOG_PRM *)ptr;
	SYS_MNG		*mng = (SYS_MNG *)mnp;

	prm->tcm = tcm;
	prm->mng = mng;
	prm->tdn = (int)((DWORD)(LONG_PTR)usr);
	prm->que = Cre_Rbf( sizeof(LOG_QUE), LGQ_CNT, TRUE );
	return( prm->que ? TRS_SUC: TRS_MEM );
}

int		Bgn_Log_Trd(			// ログスレッド開始処理
int			src,				// 起動元スレッド番号
int			tdn,				// ログスレッド番号
SYS_MNG		*mng )				// システム管理情報
{
	TST_PRM			tst[1];

	tst->tdn = tdn;
	tst->ent = log_trd;
	tst->nam = _T("ログ");
	tst->stp = set_prm;
	tst->siz = sizeof(LOG_PRM);
	mng->log->tdn = tdn;
	return( Trd_Stt( tst, src, mng, (void *)(LONG_PTR)tdn ) );
}

static	void	cst_prm(		// スレッドパラメータ破棄
void		*ptr )				// パラメータポインタ
{
	LOG_PRM		*prm = (LOG_PRM *)ptr;

	if ( prm->que )		Cst_Rbf( prm->que );
}

void	End_Log_Trd(			// ログスレッド終了
int			src,				// 停止元スレッド番号
int			tdn,				// ログスレッド番号
SYS_MNG		*mng )				// システム管理情報
{
	(mng->trd+tdn)->tcm->sts = TRD_NON;
	Trd_End( tdn, sizeof(LOG_PRM), cst_prm, src, mng );
}
