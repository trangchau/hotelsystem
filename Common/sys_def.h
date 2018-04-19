/*
	システム共通定義ファイル
*/
#ifndef	sys_def
#define	sys_def

#include <windows.h>
#include "bsc_def.h"
#include "trd_mng.h"
#include "lnk_mng.h"

#define	TRC_FNM	".\\Trace.log"			// トレースファイル名

#define	MAX_TDC			20				// 最大スレッド数

typedef	struct {						// ログ管理初期化仕様
	WCHAR				*lgp;			// ログファイルパス
	int					lkd;			// ログ保持日数
	WCHAR				*lds;			// ログファイルの操作
	int					mxl;			// 表示最大行数
	WCHAR				*edt;			// 表示エディター
}LOG_INI;

typedef	struct {						// ログ管理
	int					opn;			// ログオープンフラグ
	WCHAR				pth[MAX_PATH];	// ログ格納パス
	int					dsp;			// ログファイル操作
	int					dys;			// 保持日数
	int					tdn;			// ログスレッド番号
	int					mxl;			// 表示最大行数
	WCHAR				exn[MAX_PATH];	// 実行ログファイル名
	WCHAR				sdn[MAX_PATH];	// 送信ログファイル名
	WCHAR				rcn[MAX_PATH];	// 受信ログファイル名
	WCHAR				edt[MAX_PATH];	// 表示エディター
	CRITICAL_SECTION	crs[1];			// 排他制御
}LOG_MNG;

typedef	struct {						// システム情報初期化仕様
	int					tso;			// スレッド起動タイムアウト
	int					tto;			// スレッド終了タイムアウト
}SYS_INI;

typedef	struct	{						// システム情報
	int					typ;			// 通信タイプ
	int					tst;			// スレッド開始タイムアウト値
	int					ted;			// スレッド終了タイムアウト値
}SYS_INF;

typedef	struct	{						// シリアル通信初期化仕様
	int					pot;			// ポート番号
	int					brt;			// ボーレート
	int					dtl;			// データ長
	int					stl;			// ストップビット長
	char				*pty;			// パリティ検査
}CMM_INI;

typedef	struct	{						// シリアル通信設定情報
	int					pot;			// ポート番号
	int					brt;			// ボーレート
	int					dtl;			// データ長
	int					stl;			// ストップビット長
	char				pty;			// パリティ検査
}CMM_INF;

typedef	struct	{						// データベース接続初期化仕様
	WCHAR				*dsn;			// データセット名
	WCHAR				*sid;			// サービスID
	WCHAR				*uid;			// ユーザーID
	WCHAR				*pwd;			// パスワード
	WCHAR				*scc;			// security code
	int					cto;			// 接続タイムアウト値
	int					pfl;			// プロフィール長
}DBS_INI;

typedef	struct	{						// データベース接続定義
	WCHAR				dsn[MAX_PATH];	// データセット名
	WCHAR				sid[MAX_PATH];	// サービスID
	WCHAR				uid[MAX_PATH];	// ユーザーID
	WCHAR				pwd[MAX_PATH];	// パスワード
	int					pfl;			// プロフィール長
}DBS_INF;

typedef	struct	SYS_DEF	{				// システム共通管理構造
	SYS_INF				sys[1];			// システム情報
	LOG_MNG				log[1];			// ログ管理情報
	TRD_INF				trd[MAX_TDC];	// スレッド管理データ
	HINSTANCE			ins;			// アプリケーションインスタンス
	HWND				wnd;			// メインウィンドウハンドル
	HANDLE				mtx;			// 二重起動防止ミューテックス
	void				*ini;			// 初期化情報ポインタ
	int					ver[4];			// 自身のバージョン
	WCHAR				pgn[128];		// 自身のプログラム名
	WCHAR				ttl[128];		// プログラムタイトル
	CRITICAL_SECTION	lbx[1];			// ログリストアクセス排他制御
	Lnk					lgm[1];			// ログメッセージリスト
	int					abt;			// アボートフラグ
}SYS_MNG;

#endif
	