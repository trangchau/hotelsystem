/*------------------------------------------
    工数入力プログラム for Windows Ver 5.0
		Copyright 2001 S.Nagaoka
			各種定義ファイル
--------------------------------------------*/
#ifndef	ksu_defH
#define	ksu_defH

#include "..\cmn\bsc_def.h"
#include "..\cmn\lnk_mng.h"
#include "..\cmn\dbs_def.h"
#include "ksu_ini.h"

#define	INI_FNM		".\\kosu.ini"
#define	PSL_FNM		".\\Pslset.ini"
#define	MTH_FNM		".\\%02d%02d%04d.kdt"

#define	NAM_PST		"Personal-Setting Data Arsnova Corp. Allrights Reserved"

#define	GetMainFrm()	((CMainFrame *)((CKosuApp *)AfxGetApp())->m_pMainWnd)

typedef	struct	{					// 描画属性定義
	HFONT		hFont;				// フォントハンドル
	int			fhg;				// フォント高さ
	HBRUSH		mbs;				// 主ウィンドウ色ブラシ
}DrawAttr;

#define	MAN_WIN	0					// メインウィンドウ情報
#define	KSU_WIN	1					// 工数ウィンドウ情報
#define	KIN_WIN	2					// 勤怠ウィンドウ情報
#define	GPH_WIN	3					// グラフウィンドウ情報

typedef	struct	{					// ウィンドウ情報
	int			psx;				// Ｘ座標
	int			psy;				// Ｙ座標
	int			wid;				// 幅
	int			hgt;				// 高
	int			mxd;				// 最大化
}Win_inf;

typedef	struct	{					// 個人設定情報
	int			red;				// 読み込み済
	Win_inf		wif[4];				// 各ウィンドウ情報
	int			wop[4];				// ウィンドウオープン状態
	int			yer;				// 最後に編集した年
	int			mth;				// 最後に編集した月
	int			fsz;				// フォントサイズ
	TCHAR		fnm[LF_FACESIZE];	// フォント名
	int			zom;				// 勤怠表表示縮尺率
	int			ksz;				// 工数表表示縮尺率
	Win_inf		kpw[1];				// 工数表ウィンドウ情報
	int			rsv[94];			// 予備領域
}Psl_set;

typedef	struct	{					// 工数時間グラフデータ
	int			day;				// 日付インデックス
	int			ser;				// 管理シリアル番号
	int			bgn;				// 開始時刻(絶対時刻)
	int			tim;				// 時間(実働時間)
}Ksu_gph;

typedef	struct	{					// 工数明細
	JobDtl		*dtl;				// ＪＯＢ明細ポインタ
	int			jno;				// ＪＯＢ番号
	int			sub;				// サブコード
	int			jdv;				// ＪＯＢ区分
	JobDiv		*jdp;				// ＪＯＢ区分ポインタ
	int			ser;				// 管理シリアル番号
	char		div[2];				// 所属区分
	int			tim[33];			// 工数（実働時間）縦合計含む
}Ksu_dtl;

typedef	struct	{					// 勤怠データ
	int			stt[31];			// 開始時刻
	int			edt[31];			// 終了時刻
	int			inr[31];			// 定時間内
	int			otr[31];			// 定時間外
	int			mnt[31];			// 深夜作業
	int			hwk[31];			// 日・祝日作業
	int			hol[31];			// 休日区分
	int			pnl[31];			// ペナルティ情報
	int			rat[31];			// 遅刻時間
	int			ery[31];			// 早退時間
	int			flp[31];			// 振替休日元日付
	int			ser;				// 管理最大シリアル番号
}Kin_dat;

typedef	struct	{					// 工数データ
	int			eno;				// 社員番号
	int			yer;				// 年
	int			mth;				// 月
	int			day;				// 日(index)
	int			cls;				// 締め日
	int			dct;				// 日数
	int			dys[31];			// 日
	int			knd[31];			// 曜日
	Kin_dat		kin[1];				// 勤怠データ
	int			ttl[32];			// 横合計（総合計含む）
	Lnk			dtl[1];				// 工数明細
	Lnk			gph[1];				// グラフデータ
}Mth_dat;

typedef	struct	{					// 全体管理データ
	int				svr;			// サーバー接続中
	int				cdy;			// 締め日(-1は末日）
	Day				now[1];			// 現在日付
	Lnk				cdr[1];			// カレンダマスタデータ
	Lnk				els[1];			// 従業員マスタデータ
	Lnk				prj[1];			// プロジェクトマスタ
	Lnk				job[1];			// ＪＯＢマスタ
	Lnk				jdv[1];			// ＪＯＢ区分マスタ
	Lnk				div[1];			// 部署マスタ
	Lnk				grp[1];			// グループマスタ
	Empl			epl[1];			// ログイン従業員
	Sysinf			sys[1];			// システム情報
	Lnk				brk[1];			// 休憩時間テーブル
	Mth_dat			mdt[1];			// １ヶ月分データ
	Psl_set			psl[1];			// 個人設定情報
	Ini_fil			*ini;			// 初期化データ
	int				cwi;			// 生成ウィンドウ番号
	DrawAttr		atr[1];			// 描画属性
}Mng_dat;

#define	FNT_SIZ	12					// フォントサイズ
#define	FNT_NAM	"ＭＳ ゴシック"		// フォント名
#define	KIN_ZOM	50					// 勤怠表表示縮尺規定値
#define	KZM_MIN	10					// 勤怠表表示縮尺最小値
#define	KZM_MAX	150					// 勤怠表表示縮尺最大値

#define	CLR_MAN	RGB(113,153,193)	// メインウィンドウ色

#define	CLR_NML	RGB(255,255,255)	// 平日描画色
#define	CLR_HOL	RGB(189,204,253)	// 休日描画色
#define	CLR_SUN	RGB(252,154,156)	// 日祝描画色
#define	CLR_SEL	RGB(214,248,243)	// 選択セル描画色

#define	JNM_LEN	60					// ＪＯＢ名称長さ

#define	MSG_LOGIN	(WM_USER+5000)

#endif
