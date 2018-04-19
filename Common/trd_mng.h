/*
	スレッド管理処理
*/

#ifndef	trd_mngH
#define	trd_mngH

#include "sys_def.h"

#define	TRD_NON		0				// スレッド不在
#define	TRD_SPD		1				// サスペンド
#define	TRD_RUN		2				// 実行中

typedef	struct	{					// スレッド管理共通項目
	HANDLE				trd;		// スレッドハンドル
	DWORD				tid;		// スレッドＩＤ
	HANDLE				stt;		// スレッド起動完了イベント
	HANDLE				stp;		// 停止指令イベント
	HANDLE				stc;		// 停止完了イベント
	HANDLE				ctn;		// 再開指令イベント
	HANDLE				ctc;		// 再開完了イベント
	HANDLE				off;		// 電源オフイベント
	HANDLE				ofc;		// 電源オフ完了イベント
	HANDLE				end;		// 終了指令イベント
	HANDLE				cmp;		// 完了通知イベント
	HANDLE				abt;		// 緊急停止通知イベント
	int					cst;		// 完了ステータス
	int					oss;		// オぺレーティングシステムステータス
	int					sts;		// 現在状態
	int					tix;		// スレッドインデックス
	int					ctf;		// 継続動作フラグ
}TRD_CMN;

typedef	int		(*SET_TPM)( void *, void *, TRD_CMN *, void * );	// スレッドパラメータ設定関数
typedef	void	(*CST_TPM)( void * );								// スレッドパラメータ破棄関数

typedef	DWORD (WINAPI *TRD_ENT)( LPVOID );

typedef	struct	{				// スレッド開始パラメータ
	int			tdn;			// スレッド番号
	TRD_ENT		ent;			// エントリーアドレス
	WCHAR		*nam;			// スレッド名
	SET_TPM		stp;			// パラメータ設定関数
	int			siz;			// パラメータサイズ
	int			rcv;			// 継続運転フラグ
}TST_PRM;

typedef	struct	{					// スレッド管理
	TRD_CMN				tcm[1];		// スレッドパラメータ（共通部）ポインタ
	WCHAR				nam[11];	// スレッド名称
	void				*prm;		// スレッドパラメータ
	int					psz;		// パラメータサイズ
	DWORD				atr;		// スレッド属性
	int					sub;		// 同一スレッド枝番
	int					nxt;		// 同一スレッドチェイン
}TRD_INF;

int		Trd_Stt( TST_PRM *, int, void *, void * );				// スレッド開始処理
void	Trd_End( int, int, CST_TPM, int, void * );				// スレッド終了処理

#endif
