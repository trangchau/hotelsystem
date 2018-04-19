/*
	初期化共通設定ファンクション
*/

#include <afx.h>
#include "sys_def.h"
#include "ini_inf.h"
#include "set_ini.h"
#include "scl_ble.h"
#include "cmn_fnc.h"
#include "dte_mng.h"
#include "sys_log.h"

int			Set_Cmm(			// 通信回線初期化設定
CMM_INF			*cmm,			// 格納領域
CMM_INI			*ini )			// 初期化設定情報
{
	int			chk;

	cmm->pot = ini->pot;			// ポート番号
	cmm->brt = ini->brt;			// ボーレート
	cmm->dtl = ini->dtl;			// データ長
	cmm->stl = ini->stl;			// ストップビット長
	cmm->pty = (*(ini->pty))&0xdf;	// パリティ検査
	
	chk  = cmm->brt==19200 || cmm->brt==9600 || cmm->brt==4800;
	chk |= cmm->brt== 2400 || cmm->brt==1200;
	chk &= cmm->dtl==8 || cmm->dtl==7;
	chk &= cmm->stl==1 || cmm->stl==2 || cmm->stl==15;
	chk &= cmm->pty=='E' || cmm->pty=='O' || cmm->pty=='N';

	return( chk ? IST_SUC: IST_INI );
}

int			Set_Sys(			// システム構成初期化
SYS_INF			*sys,			// 格納領域
SYS_INI			*ini )			// 初期化設定情報
{
	sys->tst = ini->tso;			// スレッド起動タイムアウト値
	sys->ted = ini->tto;			// スレッド終了タイムアウト値

	return( IST_SUC );
}

static	int		dcd_pwd(		// パスワード復号
WCHAR			*dst,			// 格納領域
WCHAR			*pwd,			// 暗号化パスワード
int				siz,			// 復号化後のサイズ
char			*key )			// 解除キー
{
	int			i, cnt, sts=IST_MEM;
	WCHAR		*tmp=NULL, *src = pwd;

	if ( !!pwd && !!wcslen( pwd ) ){
		cnt = (int)wcslen( pwd ) / 2;
		if ( tmp = (WCHAR *)malloc( wsizeof(cnt) ) ){
			for ( i=0; i<cnt; i++ ){
				*(tmp+i) = ( to_dgt(*src) << 4 ) | to_dgt(*(src+1));
				src += 2;
			}
			Dcd_Dat( dst, (unsigned long *)tmp, siz, key );
			dst[siz] = '\0'; 
			free( tmp );
			sts = IST_SUC;
		}
	}
	else{
		if ( !!pwd && !wcslen( pwd ) ){
			*dst = '\0';
			sts = IST_SUC;
		}
	}
	return( sts );
}

int		Get_Pwd(				// パスワード取得
WCHAR			*dst,			// 格納領域
WCHAR			*ini,			// 初期化情報(暗号化パスワード)
int				siz,			// 復号化後のサイズ
char			*key )			// 解除キー
{
	int			chk= ( !ini || !siz ) ? IST_SUC: IST_INI;

	if ( chk != IST_SUC ){
		chk = dcd_pwd( dst, ini, siz, key );
	}
	return( chk );
}

int			Set_Dbs(			// データベース接続初期化
DBS_INF			*dbs,			// 格納領域
DBS_INI			*ini,			// 初期化設定情報
char			*key )			// パスワード暗号解除キー
{
	wcscpy_s( dbs->dsn, sizeof(dbs->dsn), ini->dsn );	// ＤＢデータセット名
	wcscpy_s( dbs->sid, sizeof(dbs->sid), ini->sid );	// ＤＢサービス名
	wcscpy_s( dbs->uid, sizeof(dbs->uid), ini->uid );	// ＤＢユーザー名

	return( Get_Pwd( dbs->pwd, ini->pwd, ini->pfl, key ) );
}

int			Set_Log(			// ログ管理初期化
LOG_MNG			*log,			// 格納領域
LOG_INI			*ini,			// 初期化設定情報
WCHAR			*pgn )			// プログラム名(ログノード)
{
	Day			dte[1];
	int			idx;

	Get_Dte( dte );

	if ( ini->edt )	wcscpy_s( log->edt, sizeof(log->edt), ini->edt );
	if ( !( log->mxl = ini->mxl ) )	log->mxl = 1000;
	
	wcscpy_s( log->pth, sizeof(log->pth), ini->lgp ? ini->lgp: _T(".\\") );
	if ( log->pth && (int)wcslen(log->pth) ){
		idx = (int)wcslen(log->pth);
		while( idx-- && *(log->pth+idx)==_T('\\') )	*(log->pth+idx)='\0';
		if ( !wcslen(log->pth) )	wcscpy_s( log->pth, sizeof(log->pth)/wsizeof(1), _T("\\") );
	}
	Get_Lfn( log->exn, memcnt(log->exn), dte, LOG_RUN, pgn, log );
	Get_Lfn( log->rcn, memcnt(log->rcn), dte, LOG_RCV, pgn, log );
	Get_Lfn( log->sdn, memcnt(log->sdn), dte, LOG_SND, pgn, log );

	log->dys = ini->lkd ? ini->lkd: 30;
	_wcsupr_s( ini->lds, wcslen(ini->lds)+1 );
	log->dsp = !wcscmp(ini->lds, _T("DELETE" ) ) ? LDS_DEL: LDS_KEP;

	if ( log->dsp==LDS_DEL )	DeleteFile( log->exn );

	return( IST_SUC );
}
