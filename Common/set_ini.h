/*
	初期化共通設定ファンクション定義
*/

#ifndef	set_iniH
#define	set_iniH

#define	IST_SUC		0			// 初期化成功
#define	IST_MEM		1			// メモリー不足
#define	IST_OSE		2			// Ｏ／Ｓエラー
#define	IST_INI		3			// 初期設定ファイルの誤り
#define	IST_LGO		4			// ログオープンエラー

int		Set_Cmm( CMM_INF *, CMM_INI * );			// 通信回線初期化設定
int		Set_Sys( SYS_INF *, SYS_INI * );			// システム構成初期化
int		Set_Log( LOG_MNG *, LOG_INI *, char * );	// ログ管理初期化
int		Set_Dbs( DBS_INF *, DBS_INI *, char * );	// データベース接続初期化
int		Get_Pwd( char *, char *, int, char * );		// パスワード取得

#endif
