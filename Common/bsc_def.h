/*
	基本データ定義ファイル
*/
#ifndef	bsc_defH
#define	bsc_defH

typedef	struct	{					// 日付フィールド
	short		yer;				// 年
	short		mth;				// 月
	short		day;				// 日
	short		hor;				// 時
	short		min;				// 分
	short		sec;				// 秒
}Day;

#define		memcnt(x)		((sizeof(x))/(sizeof(x[0])))	// 配列個数取得
#define		Csr_Of(x)		(((Csr)(x))-1)					// カーソル算出
#define		is_kanji(x)	(((x)>=0x80&&(x)<=0xa0)||(x)>=0xe0)
#define		to_dgt(x)	((x)>'9' ? (((x)-0x41)+10):((x)-'0'))
#define		is_num(x)	((x)>='0' && (x)<='9')
#define		is_alp(x)	((x)>='A' && (x)<='Z')

#define		wsizeof(x)	((x)*sizeof(WCHAR))

#define		WM_USER_CLOSE			(WM_USER+8192+99)

char	*Han_Zen( unsigned char *, unsigned char * );	// 半角→全角変換
int		Zen_Han( unsigned char *, unsigned char * );	// 全角→半角変換

// スレッド関係
#define	TRS_SUC		1			// 正常終了
#define	TRS_MEM		2			// メモリー不足
#define	TRS_PRM		3			// パラメータ異常
#define	TRS_USR		4			// ユーザーキャンセル
#define	TRS_OSE		5			// オぺレーティングシステム(API)エラー
#define	TRS_TMO		6			// 起動待ちタイムアウト

#define	LDS_DEL		1			// 毎回削除
#define	LDS_KEP		2			// ログファイル保持

#endif
