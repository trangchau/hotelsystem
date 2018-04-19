/*
	システムログ処理
*/

#ifndef	sys_logH
#define	sys_logH

#include <windows.h>
#include "sys_def.h"
#include "log_trd.h"

#define	LOG_RUN			1			// 動作ログ
#define	LOG_OPE			2			// 操作ログ
#define	LOG_ERR			3			// エラーログ
#define	LOG_SND			101			// 送信ログ
#define	LOG_RCV			102			// 受信ログ

#define	LOG_SIZ		2048			// ログメッセージサイズ

#define	LOG_EXC	\
		EnterCriticalSection( log->crs );\
		if ( !_wfopen_s( &fp, log->exn, _T("a+") ) ){\
			fwprintf( fp, _T("%04d/%02d/%02d %02d:%02d:%02d "),\
				dpt->yer, dpt->mth, dpt->day, dpt->hor, dpt->min, dpt->sec );\
			fwprintf( fp, _T("[%-4s]:"), Log_Knd( knd ) );\
			fwprintf( fp, _T("[%-10s]:"), mdl );\
			va_start( argp, fmt );\
			vfwprintf( fp, fmt, argp );\
			va_end( argp );\
			fwprintf( fp, _T("\n") );\
			fclose( fp );\
		}\
		LeaveCriticalSection( log->crs );

WCHAR	*Edt_Lfn( WCHAR *, Day * );									// ログファイル名編集
WCHAR	*Get_Lfn( WCHAR *, int, Day *, int, WCHAR *, LOG_MNG * );	// ログファイル名取得
WCHAR	*Log_Knd( int );											// ログ種別名取得
int		Opn_Log( LOG_MNG * );										// ログオープン
void	Cls_Log( LOG_MNG * );										// ログクローズ
void	Log_Err( LOG_MNG *, int );									// ログファイルエラー表示
void	Sys_Log( int, int, SYS_MNG *, int, WCHAR *, ... );			// システムログ実行
int		Edt_Lgh( WCHAR *, LOG_QUE * );								// ログヘッダー編集
void	Del_Log( int, WCHAR * );									// 不要ログ削除

#endif
