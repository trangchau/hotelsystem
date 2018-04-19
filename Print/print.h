/*------------------------------------------
    工数入力プログラム for Windows Ver 5.0
		Copyright 2001 S.Nagaoka
			印刷関係定義ファイル
--------------------------------------------*/
#ifndef	printH
#define	printH

#include "afn_cnv.h"

#define	PAI			3.14159265358979323846
#define	HPI			1.57079632679489661923
#define	QPI			0.785398163397448309616
#define	To_Deg(x)	( (x) * 180.0 / PAI )
#define To_Rad(x)	( (x) * PAI / 180.0 )

#define	MAX_PNM			128		// プリンター名最大長さ
#define	MAX_PPR			64		// 用紙名最大長さ

#define	COL_BLK		(0)				// 黒
#define	COL_MRN		(128)			// マルーン
#define	COL_GRN		(32768)			// 緑
#define	COL_OLV		(32896)			// オリーブ
#define	COL_NVY		(8388608)		// ネイビー
#define	COL_PRP		(8388736)		// 紫
#define	COL_TEL		(8421376)		// ティアル
#define	COL_GRY		(8421504)		// グレイ
#define	COL_SVR		(12632256)		// 銀
#define	COL_RED		(255)			// 赤
#define	COL_LIM		(65280)			// ライム
#define	COL_YLW		(65535)			// 黄
#define	COL_BLU		(16711680)		// 青
#define	COL_FCS		(16711935)		// ？
#define	COL_AQA		(16776960)		// 水色
#define	COL_LGL		(12632256)		// ライトグレー
#define	COL_DGL		(8421504)		// ダークグレー
#define	COL_WHT		(16777215)		// 白
#define	COL_NON		(536870911)		// なし

#define DEF_MTX 	0		// 異種座標(0:無  1:Ｙ反転  2:Ｘ反転)
#define	MM_UNT		0.1		// サイズ単位 (x/mm)
#define	STC_TXT		1		// 文字列タイプ
#define	STC_RCT		2		// 図形タイプ
#define	STC_LIN		3		// 線タイプ
#define	STC_ARC		4		// 円弧タイプ
#define	STC_BMP		5		// ビットマップタイプ
#define	STC_BLK		6		// ブロックタイプ

/*	All_Dat構造体	*/
#define	MOD_PRN     0       // プリンター出力
#define	MOD_PRV     1		// プレビュー出力

/*	テキストタイプ構造体	*/
#define	FST_NML		400		// ボールド（標準）
#define	FST_BLD		700		// ボールド（太字）

#define	SGL_LIN		0x0020	// 単一行
#define	HRZ_CNT		0x0001	// 水平方向センターリング
#define	HRZ_LFT		0x0000	// 左詰
#define	HRZ_RGT		0x0002	// 右詰
#define	VRT_CNT		0x0004	// 垂直方向センターリング（単一行時のみ）
#define	VRT_TOP		0x0000	// 上付き
#define	VRT_BTM		0x0008	// 下付き
#define	WRD_BRK		0x0010	// ワードブレイク

/*	円弧タイプ構造体	*/
#define	TYP_ARC		0		// 円弧タイプ
#define	TYP_CHD		1		// 弦タイプ
#define	TYP_PIE		2		// 扇タイプ
#define	TYP_CCL		3		// 円タイプ

/*	ペンスタイル	*/
#define	PNS_SLD		0		// 実線のペン
#define	PNS_DSH		1		// 破線のペン
#define	PNS_DOT		2		// 点線のペン
#define	PNS_DSD		3		// 一点鎖線のペン
#define	PNS_DDD		4		// 二点鎖線のペン

/*	ブラシスタイル	*/
#define	BRS_CLR		0		// ブラシクリア
#define	BRS_SLD		1		// 純色
#define	BRS_BDI		2		// 45度下向きのハッチ (左から右へ)
#define	BRS_CRS		3		// 水平と垂直のクロスハッチ
#define	BRS_DCS		4		// 45度のクロスハッチ
#define	BRS_FDI		5		// 45度上向きのハッチ (左から右へ)
#define	BRS_HRZ		6		// 水平ハッチ
#define	BRS_VRT		7		// 垂直ハッチ

/*	プリンター	*/
#define	MAX_PNM		128		// プリンター名最大長さ
#define	MAX_PPR		64		// 用紙名最大長さ

/*	印刷種別	*/
#define	PDV_TXT		1		// 文字列
#define	PDV_RCT		2		// 矩形
#define	PDV_LIN		3		// 線分
#define	PDV_ARC		4		// 円弧
#define	PDV_BMP		5		// ビットマップ
#define	PDV_BLK		6		// ブロック

typedef struct{					// 文字列データタイプ構造体
	char		*nam;			// 名前
	int			xps;			// ｘ座標（1/10mm）
	int			yps;			// ｙ座標（1/10mm）
	int			wid;			// 幅（1/10mm）
	int			hgt;			// 高さ（1/10mm）
	WCHAR		*txt;			// テキスト文字列
	int			mod;			// 描画モード（センタリング等）
	unsigned	fcl;			// 前景色
	unsigned	bcl;			// 背景色
	WCHAR		*fnt;			// フォント名
	int			siz;			// フォントサイズ
	int			uln;			// 下線有無
	int			cln;			// 消線有無
	int			itl;			// 斜体有無
	int			bld;			// 太字サイズ( 0 ～ 1000 )
	int			vsb;			// 可視
}Txt_def;

typedef struct{					// 矩形データタイプ構造体
	char		*nam;			// 名前
	int			xps;			// ｘ座標（1/10mm）
	int			yps;			// ｙ座標（1/10mm）
	int			wid;			// 幅（1/10mm）
	int			hgt;			// 高さ（1/10mm）
	int 		pns;			// ペンスタイル
	int			lnw;			// 線幅（1/10mm）
	unsigned	fcl;			// 前景色
	int			brs;			// ブラシスタイル
	unsigned	bcl;			// 背景色
	int			rnd;			// 角丸有無
	int			vsb;			// 可視
}Rct_def;

typedef struct{					// 円弧データタイプ構造体
	char		*nam;			// 名前
	int			xps;			// 中心ｘ座標（1/10mm）
	int			yps;			// 中心ｙ座標（1/10mm）
	int			xrs;			// x半径（1/10mm）
	int			yrs;			// y半径（1/10mm）
	double		brd;			// 始点角（ラジアン）
	double		erd;			// 終点角（ラジアン）
	int			typ;			// 描画タイプ
	int 		pns;			// ペンスタイル
	int			lnw;			// 線幅（1/10mm）
	unsigned	fcl;			// 前景色
	int			brs;			// ブラシスタイル
	unsigned	bcl;			// 背景色
	int			vsb;			// 可視
}Arc_def;

typedef struct{					// 線データタイプ構造体
	char		*nam;			// 名前
	int			bgx;			// 始点ｘ座標（1/10mm）
	int			bgy;			// 始点ｙ座標（1/10mm）
	int			edx;			// 終点ｘ座標（1/10mm）
	int			edy;			// 終点ｙ座標（1/10mm）
	int 		pns;			// ペンスタイル
	int			lnw;			// 線幅（1/10mm）
	unsigned	fcl;			// 前景色
	int			vsb;			// 可視
}Lin_def;

typedef struct{			 		// ビットマップデータ構造体
	WCHAR			*nam;		// 名前
	WCHAR			*bmp;		// ビットマップ名
	int				xps;		// ｘ座標（1/10mm）
	int				yps;		// ｙ座標（1/10mm）
	int				wid;		// 幅（1/10mm）
	int				hgt;		// 高さ（1/10mm）
	int				vsb;		// 可視
}Bmp_def;

typedef struct{		 			// ブロックデータタイプ構造体
	char		*nam;			// 名前
	int			xps;			// ｘ座標（1/10mm）
	int			yps;			// ｙ座標（1/10mm）
	int			wid;			// 幅（1/10mm）
	int			hgt;			// 高さ（1/10mm）
	int 		pns;			// ペンスタイル
	int			lnw;			// 線幅（1/10mm）
	unsigned	fcl;			// 前景色
	int			brs;			// ブラシスタイル
	unsigned	bcl;			// 背景色
	Txt_def		*txt;			// 文字列データ
	int			txc;			// 文字列データ数
	Rct_def		*rct;			// 矩形データ
	int			rcc;			// 矩形データ数
	Lin_def		*lin;			// 線データ
	int			lic;			// 線データ数
	Arc_def		*arc;			// 円弧データ
	int			acc;			// 円弧データ数
	Bmp_def		*bmp;			// ビットマップデータ
	int			bmc;			// ビットマップデータ数
	int			vsb;			// 可視
}Blk_def;

typedef	struct{					// 印刷データ構造体
	Txt_def		*txt;			// 文字列データ
	int			txc;			// 文字列データ数
	Rct_def		*rct;			// 矩形データ
	int			rcc;			// 矩形データ数
	Lin_def		*lin;			// 線データ
	int			lic;			// 線データ数
	Arc_def		*arc;			// 円弧データ
	int			acc;			// 円弧データ数
	Bmp_def		*bmp;			// ビットマップデータ
	int			bmc;			// ビットマップデータ数
	Blk_def		*blk;			// ブロックデータ
	int			blc;			// ブロックデータ数
}Prn_def;

typedef struct{					// 用紙データ構造体
	short		wid; 			// ユーザー定義用紙幅
	short		hgt; 			// ユーザー定義用紙高さ
	WCHAR		*pnm;			// 用紙名
	short		siz; 			// 用紙サイズ
	short		orn; 			// 用紙方向
	WCHAR		*ttl;			// レポートタイトル
}Ppr_def;

typedef struct{					// 印刷デバイス定義
	int			xpc;			// 印刷可能水平領域(ﾄﾞｯﾄ) (ディスプレイの幅(ピクセル単位))
	int			ypc;			// 印刷可能垂直領域(ﾄﾞｯﾄ) (ディスプレイの高さ(ラスタ行単位))
	int			xmm;			// 印刷可能水平領域(mm)   (物理画面の幅 (mm))
	int			ymm;			// 印刷可能垂直領域(mm)   (物理画面の高さ (mm))
	int			hmg;			// 水平マージン(ﾄﾞｯﾄ)
	int			vmg;			// 垂直マージン(ﾄﾞｯﾄ)
	float		dpp;			// ﾎﾟｲﾝﾄ→ﾄﾞｯﾄ変換係数
}Dev_inf;

typedef	struct{					// デフォルトプレビュースタイル構造体
	int			lft;			// 表示位置（左上隅Ｘ)(ピクセル)
	int			top;			// 表示位置（左上隅Ｙ)(ピクセル)
	int			wid;			// 表示幅(ピクセル)
	int			hit;			// 表示高さ(ピクセル)
	int			shw;			// 表示モード(ピクセル)
	int			scl;			// デフォルトスケール
	int			mbr;			// メニューバー有無
}Prv_def;

typedef struct{					// 印刷データ構造体
	Ppr_def		*ppr;			// 用紙データ構造体
	Prn_def		*prn;			// 印刷データ構造体
	Prv_def		*prv;			// デフォルトプレビュースタイル
	Dev_inf		*dev;			// デバイス定義構造体
	HINSTANCE	ins;			// ｱﾌﾟﾘｹｰｼｮﾝｲﾝｽﾀﾝｽﾊﾝﾄﾞﾙ
	HWND		par; 			// 親ウィンドウハンドル
	HWND		wnd; 			// プレビューウィンドウハンドル
	int			mod;			// 出力先
	Mtx_def		mtx[1];			// 座標変換マトリクス
	HDC			hdc;			// プリンタデバイスハンドル
}Prn_mng;

void	Get_Pdv_Inf( Prn_mng *, int );				// プリンター情報取得
void	Prn_Out_Dat( HDC, Prn_mng *, int, HRGN );	// 印刷データ出力
int		Prn_Opn( char *, Prn_mng * );				// プリンターオープン
int		Prn_Exc( int, Prn_mng * );					// 印刷実行
int		Prn_Cls( Prn_mng * );						// プリンタクローズ

#endif
