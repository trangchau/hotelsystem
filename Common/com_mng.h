/*
	ＲＳ－２３２Ｃ 通信ルーチン
*/

#ifndef	COM_MNGH
#define	COM_MNGH

// 通信制御コード
#define	ENQ			0x5
#define	ACK			0x6
#define	NAK			0x15
#define	STX			0x2
#define	ETX			0x3
#define	EOT			0x4
#define	DLE			0x10
#define	DC1			0x11
#define	DC4			0x14
#define	CR			0xd

// 通信エラーステータス
#define	CER_NON		0					// エラー無し
#define	CER_OVR		1					// オーバーランエラー
#define	CER_PTY		2					// パリティエラー
#define	CER_FRM		3					// フレーミングエラー
#define	CER_BRK		4					// ブレークキャラクター検出
#define	CER_IOE		5					// Ｉ／Ｏエラー
#define	CER_RBO		6					// 受信バッファオーバーフロー
#define	CER_SBO		7					// 送信バッファオーバーフロー
#define	CER_MOD		8					// モードエラー

#define	CER_OPN		20					// オープンされていないポートのアクセス
#define	CER_TOT		50					// タイムアウトエラー
#define	CER_MEM		60					// メモリー不足エラー
#define	CER_NDT		61					// データ無しエラー

#define	CER_UNW		99					// 不明なエラー

// 通信イベントテータス
#define	CEV_RCV		1					// 受信完了イベント
#define	CEV_SND		2					// 送信完了イベント
#define	CEV_ERR		3					// エラー発生イベント

typedef	struct	{						// 通信内容定義
	int		pot;						// 通信ポート番号(1,2,3,...)
	int		brd;						// ボーレート(19200,9600,4800,...)
	int		stp;						// ストップビット長(1=1, 15=1.5, 2=2)
	int		dtl;						// データビット長(5,6,7,8)
	char	pty;						// パリティ(E=Even,O=Odd,M=Mark,N=None)
	int		rto;						// 受信タイムアウト値(ms)
	int		sto;						// 送信タイムアウト値(ms)
	int		(*evt)(int,int,int,void *);	// イベントハンドラ(コールバック)
}Com_def;

// 内部定義
#define	RRQ_MAX			128				// 受信要求キュー最大数
#define	RQU_MAX			128				// 受信キュー最大数
#define	SQU_MAX			128				// 送信キュー最大数

#define	SRQ_EVT			0				// 受信要求イベントインデックス
#define	SED_EVT			1				// 受信スレッド終了要求イベントインデックス

#define	RRQ_EVT			0				// 送信要求イベントインデックス
#define	RED_EVT			1				// 送信スレッド終了要求イベントインデックス

#define	EED_EVT			0				// エラー監視スレッド終了要求イベントインデックス

#define	TRE_RTY			10				// スレッド終了監視最大リトライ回数

typedef	struct	{						// 通信内容データ
	int		siz;						// データサイズ
	int		tim;						// タイムアウト値
	char	ecr;						// 受信終端キャラクタ
	char	dat[1];						// 受信データ
}Com_dat;

typedef	struct	{				// リングバッファ管理機構
	int		red;						// 読出ポインター
	int		wrt;						// 書込ポインター
	int		max;						// 最大バッファ数
	int		siz;						// １オブジェクトのサイズ
	void	*buf;						// データポインタ
}Rng_buf;

#define	Cbf_Siz(x)	(sizeof(Com_dat) + (x) - 1)

typedef	struct	{				// 通信制御データ内部定義
	Com_def				def;			// ユーザー定義
	CRITICAL_SECTION	rra;			// 受信要求ｷｭｰｱｸｾｽ用ｸﾙﾃｨｶﾙｾｸｼｮﾝ
	CRITICAL_SECTION	rqa;			// 受信ｷｭｰｱｸｾｽ用ｸﾙﾃｨｶﾙｾｸｼｮﾝ
	CRITICAL_SECTION	sqa;			// 送信ｷｭｰｱｸｾｽ用ｸﾙﾃｨｶﾙｾｸｼｮﾝ

	HANDLE				hnd;			// 通信ポートのハンドル
	OVERLAPPED			sov;			// 多重Ｉ／Ｏ制御(送信)
	OVERLAPPED			rov;			// 多重Ｉ／Ｏ制御(受信)

	HANDLE				rtd;			// 受信スレッドハンドル
	HANDLE				std;			// 送信スレッドハンドル
	int					rid;			// 受信スレッドＩＤ
	int					sid;			// 送信スレッドＩＤ
	int					rtm;			// 受信スレッド終了
	int					stm;			// 送信スレッド終了

	HANDLE				rev[2];			// 受信要求・スレッド終了要求イベント
	HANDLE				sev[2];			// 送信要求・スレッド終了要求イベント

	Rng_buf				rrb;			// 受信要求リングバッファ
	Rng_buf				rbf;			// 受信リングバッファ
	Rng_buf				sbf;			// 送信リングバッファ

	void				*usr;			// ユーザー定義データ

	int					end;			// 終了フラグ
}Com_ctl;

HANDLE	Com_Opn( Com_def *, void *usr );		// 通信回線のオープン
void	Com_Cls( HANDLE );						// 通信回線のクローズ
int		Com_Snd( HANDLE, char *, int, int );	// データの送信
int		Com_Rcv( HANDLE, char, int, int );		// データの受信要求
int		Com_Get( void *, HANDLE );				// 受信データの取得
void	Com_Rts( HANDLE, int );					// ＲＴＳの操作
void	Com_Dtr( HANDLE, int );					// ＤＴＲの操作
char	*Edt_Cmf( char *, char * );				// 通信フレームをログ用に編集
char	*Get_Cen( int );						// 通信エラーメッセージ取得

#endif
