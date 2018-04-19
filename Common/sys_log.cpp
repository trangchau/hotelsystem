/*
	システムログ処理
*/

#include <afx.h>
#include <stdio.h>
#include "sys_log.h"
#include "dte_mng.h"
#include "log_trd.h"

static	WCHAR	Ftl_msg[2048];

WCHAR	*Edt_Lfn(				// ログファイル名編集
WCHAR		*fnm,				// 元ファイル名
Day			*dte )				// 日付
{
	int			siz;
	WCHAR		drv[_MAX_DRIVE], dir[_MAX_DIR], nam[_MAX_FNAME], ext[_MAX_EXT];
	WCHAR		*dst;

	_wsplitpath_s( fnm, drv, sizeof(drv)/wsizeof(1), dir, sizeof(dir)/wsizeof(1), nam, sizeof(nam)/wsizeof(1), ext, sizeof(ext)/wsizeof(1) );
	siz = (int)wcslen( drv ) + (int)wcslen( dir ) + (int)wcslen( nam ) + (int)wcslen( ext ) + 80;
	if ( dst = (WCHAR *)malloc( wsizeof(siz) ) ){
		if ( dte ){
			swprintf_s( dst, siz, _T("%s%s%s%04d%02d%02d%s"),
				drv, dir, nam, dte->yer, dte->mth, dte->day, ext );
		}
		else{
			swprintf_s( dst, siz, _T("%s%s%s????????%s"), drv, dir, nam, ext );
		}
	}
	return( dst );
}

WCHAR	*Get_Lfn(			// ログファイル名取得
WCHAR		*dst,				// 格納領域
int			siz,				// Size of output buffer
Day			*dte,				// 日付
int			knd,				// ログ種別
WCHAR		*pgn,				// プログラム名称
LOG_MNG		*log )				// ログスレッドパラメータ
{
	WCHAR		*fnm;

	switch( knd ){
	case LOG_RUN:	swprintf_s( dst, siz, _T("%s\\%s.log"), log->pth, pgn );	break;
	case LOG_RCV:	swprintf_s( dst, siz, _T("%s\\COM.log"), log->pth );		break;
	case LOG_SND:	swprintf_s( dst, siz, _T("%s\\COM.log"), log->pth );		break;
	}
	if ( dte && !!( fnm = Edt_Lfn( dst, dte ) ) ){
		wcscpy_s( dst, MAX_PATH, fnm );
		free( fnm );
	}
	return( dst );
}

WCHAR	*Log_Knd(			// ログ種別名取得
int			knd )			// 種別コード
{
	WCHAR		*kst;

	switch( knd ){
	case LOG_RUN:	kst = _T("通常");	break;		// 実行ログ
	case LOG_OPE:	kst = _T("操作");	break;		// 操作ログ
	case LOG_ERR:	kst = _T("異常");	break;		// エラーログ
	default:		kst = _T("不明");	break;
	}
	return( kst );	
}

void	Log_Err(			// ログファイルエラー表示
LOG_MNG		*mng,			// ログ管理構造
int			ose )			// Ｏ／Ｓエラー
{
	swprintf_s( Ftl_msg, memcnt(Ftl_msg), _T("ログファイルエラー! \n")
			_T("file name=[%s]\n error code=[%d]"), mng->exn, ose );
	MessageBox( NULL, Ftl_msg, _T("ログ管理構造"), MB_ICONEXCLAMATION|MB_OK );
}

static	int		put_log(	// ログ直接出力(メモリーバッファを必要としません)
int			knd,			// ログ種別
LOG_MNG		*log,			// ログ管理構造
Day			*dte,			// 発生日時
WCHAR		*fmt,			// ログフォーマット
... )						// 可変引数リスト
{
	FILE		*fp=NULL;
	Day			*dpt, tmp[1];
	va_list		argp;
	char		*mdl="初期化";

	if ( log->opn ){
		if ( !( dpt = dte ) )	Get_Dte( tmp ), dpt = tmp;
		LOG_EXC
		if ( !fp )	Log_Err( log, GetLastError() );
	}
	return( !!fp );
}

int		Opn_Log(			// ログオープン
LOG_MNG		*mng )			// ログ管理構造
{
	InitializeCriticalSection( mng->crs );
	mng->opn = TRUE;
	mng->opn = put_log( LOG_RUN, mng, NULL, _T("システムログを開設しました。") );
	return( mng->opn );
}

void	Cls_Log(			// ログクローズ
LOG_MNG		*mng )			// ログ管理構造
{
	if ( mng->opn ){
		put_log( LOG_RUN, mng, NULL, _T("システムログを閉鎖しました。") );
		DeleteCriticalSection( mng->crs );
		mng->opn = FALSE;
	}
}

void	Sys_Log(			// システムログ実行
int			mdn,			// モジュール番号
int			knd,			// ログ種別
SYS_MNG		*mng,			// システム管理構造
int			usr,			// ユーザー定義データ
WCHAR		*fmt,			// ログフォーマット
... )						// 可変引数リスト
{
#ifndef	NO_LOGGING
	WCHAR		*msg, *mdl=_T("最上位");
	LOG_QUE		que[1];
	va_list		argp;
	Day			*dpt=que->dte;
	TRD_INF		*trd=mng->trd+mng->log->tdn;
	LOG_PRM		*prm;
	LOG_MNG		*log = mng->log;
	FILE		*fp=NULL;
//	Csr			csr;
	int			tdl=FALSE;

	Get_Dte( que->dte );

	if ( mng->log->opn ){
		if ( msg = (WCHAR *)malloc( wsizeof(LOG_SIZ) ) ){
			if ( *(mng->trd+mdn)->nam )	mdl = (mng->trd+mdn)->nam;
			que->mdn = mdn;
			wcscpy_s( que->nam, sizeof(que->nam)/wsizeof(1), mdl );
			que->msg = msg;
			que->knd = knd;
			que->usr = usr;
			va_start( argp, fmt );
			wvsprintf( msg, fmt, argp );
			va_end( argp );
			if ( msg ){
//				EnterCriticalSection( mng->lbx );
//				if ( csr = Add_Lnk( mng->lgm, NULL, (int)strlen(msg)+1+sizeof(que) ) ){
//					*Rec_Of(LOG_QUE, csr) = *que;
//					strcpy_s( (char *)(Rec_Of(LOG_QUE, csr)+1), 1024, msg );
//				}
//				LeaveCriticalSection( mng->lbx );
			}
			if ( tdl = trd->tcm->sts!=TRD_NON ){	// ログスレッド存在
				prm = (LOG_PRM *)trd->prm;
				if ( Wrt_Rbf( que, prm->que ) ){
					ReleaseSemaphore( Rbf_Smp( prm->que ), 1, NULL );
				}
				else{
					free( msg );
					msg = NULL;
				}
			}
			else{
				free( msg );
				msg = NULL;
			}
		}
		if ( !tdl || !msg ){		// ログスレッド無しか、メッセージバッファ確保失敗
			LOG_EXC		// 直接ロギングします！
			if ( !fp )	Log_Err( mng->log, GetLastError() );
		}
	}
/*
	char		txt[2048];
	int			idx;

	idx  = sprintf( txt, "%04d/%02d/%02d %02d:%02d:%02d ",
				dpt->yer, dpt->mth, dpt->day, dpt->hor, dpt->min, dpt->sec );
	if ( knd!=LOG_SND && knd!=LOG_RCV ){
		idx += sprintf( txt+idx, "[%-4s]:", Log_Knd( knd ) );
		idx += sprintf( txt+idx, "[%-10s]:", mdl );
	}
	va_start( argp, fmt );
	vsprintf( txt+idx, fmt, argp );
	va_end( argp );
	EnterCriticalSection( mng->lbx );
	if ( csr = Add_Lnk( mng->lgm, NULL, strlen(txt)+1+sizeof(knd) ) ){
		*Rec_Of(int, csr) = knd;
		strcpy( (char *)(Rec_Of(int, csr)+1), txt );
	}
	LeaveCriticalSection( mng->lbx );
*/
#endif
}

int		Edt_Lgh(				// ログヘッダー編集
char			*dst,			// 格納領域
LOG_QUE			*log )			// ログデータ
{
	int			idx;
	Day			*dpt=log->dte;

	idx  = sprintf_s( dst, 4096, "%04d/%02d/%02d %02d:%02d:%02d ",
				dpt->yer, dpt->mth, dpt->day, dpt->hor, dpt->min, dpt->sec );
	idx += sprintf_s( dst+idx, 4096, "[%-4s]:", Log_Knd( log->knd ) );
	idx += sprintf_s( dst+idx, 4096, "[%-10s]:", log->nam );
	return( idx );
}


static	int		is_nums(		// 数値文字列検定
WCHAR		*str,				// 検定文字列
int			siz )				// 比較文字数
{
	WCHAR	*ptr = (WCHAR *)str;

	if ( !!siz ){
		while( siz-- && *ptr ){
			if ( *ptr<_T('0') || *ptr>_T('9') )	break;
			ptr++;
		}
	}
	return( siz==-1 );
}

static	int		to_num(			// 数字に変換
WCHAR		*ptr,				// 文字列ポインタ
int			siz )				// 文字列サイズ
{
	WCHAR		buf[10];

	memcpy( buf, ptr, wsizeof(siz) );
	*(buf+siz) = '\0';
	return( _wtoi( buf ) );
}

static	int		get_fdt(		// ファイル日付取得
Day			*dst,				// 日付格納領域
WCHAR		*fnm,				// ファイル名
WCHAR		*srn )				// 元ファイル名
{
	int			chk;
	WCHAR		*ptr, drv[_MAX_DRIVE], dir[_MAX_DIR], nam[_MAX_FNAME], ext[_MAX_EXT];

	_wsplitpath_s( fnm, drv, sizeof(drv)/wsizeof(1), dir, sizeof(dir)/wsizeof(1), nam, sizeof(nam)/wsizeof(1), ext, sizeof(ext)/wsizeof(1) );

	if ( chk = wcslen(fnm) == (wcslen(nam)+wcslen(ext)+wsizeof(8)) ){
		if ( chk = !memcmp( fnm, nam, wsizeof(wcslen(nam)) ) ){
			ptr = fnm + wcslen(nam);
			if ( chk = is_nums( ptr, 8 ) ){
				dst->yer = to_num( ptr+0, 4 );
				dst->mth = to_num( ptr+4, 2 );
				dst->day = to_num( ptr+6, 2 );
				chk = Chk_Dte( dst );
			}
		}
	}
	return( chk );
}

void		Del_Log(			// 不要ログ削除
int			dys,				// 保有期間(日)
WCHAR		*bsn )				// 元ファイル名
{
	WIN32_FIND_DATA		fnd[1];
	HANDLE				hdl;
	WCHAR				*fnm;
	int					lop;
	Day					fdt[1], dte[1], *dpt;
	Lnk					rot[1], *csr;

	memset( rot, 0, sizeof(rot) );

	Get_Dte( dte );
	Add_Dte( dte, -dys );

	if ( fnm = Edt_Lfn( bsn, NULL ) ){
		if ( lop = ( hdl = FindFirstFile( fnm, fnd ) ) != INVALID_HANDLE_VALUE ){
			while( lop ){
				if ( get_fdt( fdt, fnd->cFileName, bsn ) ){
					if ( Cmp_Dte( dte, fdt ) > 0 )	Add_Lnk( rot, fdt, sizeof(fdt) );
				}
				lop = FindNextFile( hdl, fnd );
			}
			FindClose( hdl );
		}
		free( fnm );
	}
	csr = Nxt_Lnk( rot, NULL );
	while( csr ){
		dpt = Rec_Of(Day, csr);
		if ( fnm = Edt_Lfn( bsn, dpt ) ){
			DeleteFile( fnm );
			free( fnm );
		}
		csr = Del_Lnk( rot, csr );
	}
}

