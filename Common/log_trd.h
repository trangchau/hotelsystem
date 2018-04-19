/*
	ログスレッド処理定義
*/

#ifndef	Log_TrdH
#define	Log_TrdH

#include	"bsc_def.h"
#include	"rng_buf.h"

#define	LGQ_CNT		64				// ログキュー数

#define	LDS_DEL		1				// 毎回削除
#define	LDS_KEP		2				// ログファイル保持

typedef	struct	{					// メインスレッドパラメータ
	TRD_CMN				*tcm;		// スレッド共通定義
	SYS_MNG				*mng;		// システム管理情報
	RBF_HDL				que;		// ログ要求キュー
	int					tdn;		// スレッド番号
}LOG_PRM;

typedef	struct	{					// ログキュー構造
	int					mdn;		// モジュール番号
	WCHAR				nam[11];	// スレッド名称
	int					knd;		// ログ種別
	int					usr;		// ユーザー定義種別
	WCHAR				*msg;		// ログメッセージ
	Day					dte[1];		// 発生日時
}LOG_QUE;

#include	"sys_log.h"

int		Bgn_Log_Trd( int, int, SYS_MNG * );				// ログスレッド開始
void	End_Log_Trd( int, int, SYS_MNG * );				// ログスレッド終了

#endif
