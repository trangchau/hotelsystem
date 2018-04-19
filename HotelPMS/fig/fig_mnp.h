/*
	図形作成・操作ルーチン定義
*/

#ifndef	fig_mnpH
#define	fig_mnpH

//#include <windows.h>
#include <afx.h>
#include <math.h>
#include "..\..\Print\afn_cnv.h"
#include "..\..\Common\bsc_def.h"
#include "..\..\Common\lnk_mng.h"

#define	SEL_ALL		-1				// Select all of layers

#define	COL_BLK		(0)				// Black
#define	COL_MRN		(128)			// maroon
#define	COL_GRN		(32768)			// green
#define	COL_OLV		(32896)			// olive
#define	COL_NVY		(8388608)		// naivy
#define	COL_PRP		(8388736)		// purple
#define	COL_TEL		(8421376)		// ティアル
#define	COL_GRY		(8421504)		// glay
#define	COL_SVR		(12632256)		// silver
#define	COL_RED		(255)			// red
#define	COL_LIM		(65280)			// lime
#define	COL_YLW		(65535)			// yellow
#define	COL_BLU		(16711680)		// blue
#define	COL_FCS		(16711935)		// ？
#define	COL_AQA		(16776960)		// light blue
#define	COL_LGL		(12632256)		// light glay
#define	COL_DGL		(8421504)		// dark gray
#define	COL_WHT		(16777215)		// whote
#define	COL_NON		(536870911)		// none

#define	MOD_NML		0				// normal
#define	MOD_SEL		1				// selecting（ラバーバンド）モード
#define	MOD_CRT		2				// creating（ラバーバンド）モード
#define	MOD_MOV		3				// moving element（シャドー）モード
#define	MOD_CHG		4				// chainging shape of element（シャドー）モード
#define	MOD_CUT		5				// cutting element（ラバーバンド）モード


#define	C_PI		3.141592654
#define	to_rad(x)	((x)*(C_PI/180.0))
#define	RDW_OFS		10				// 再描画領域のオフセットサイズ（直線矢印分）

#define	min_(x,y)	((x)<(y) ? (x):(y))
#define	max_(x,y)	((x)>(y) ? (x):(y))

#define	ELM_LIN		1				// 線分要素
#define	ELM_RCT		2				// 矩形要素

typedef	void (*Drw_txt)(HDC,WCHAR *,Dpn *,Dpn *,double,int,HWND);		// 文字列描画内部関数
typedef	void (*Mus_Cbk)( int, int, int, LPVOID );						// マウス操作コールバック関数定義
typedef	int	(*Ope_Cbk)( int, int, int, Dpn *, Dpn *, void *, LPVOID );	// 図形操作コールバック関数定義
typedef	void (*Drw_Cbk)( HDC, void *, Drw_txt, HWND, LPVOID );			// 図形描画コールバック関数定義

typedef struct {					//  座標
	int				x, y;			//  座標値
}Ipn;

typedef struct {					//  直線分
	Dpn				sp[1];			//  始点
	Dpn				ep[1];			//  終点
}Lin;

typedef	struct	{					// 領域定義情報
	double	top;					// 縦方向先頭位置
	double	lft;					// 横方向先頭位置
	double	wid;					// 領域幅
	double	hit;					// 領域高
}Area;

typedef	struct	{					// 矩形
	Dpn				sp[1];			// 始点
	Dpn				ep[1];			// 終点
	int				lmk;			// 左側端点形状番号
	COLORREF		lmc;			// Color of left terminal mark
	int				rmk;			// 右側端点形状番号
	COLORREF		rmc;			// Color of right terminal mark
//	WCHAR			*txt;			// Text of displaying data
}Rct;

typedef	struct	{					// 端点マーク管理構造
	int				bcl;			// 塗りつぶし色
	int				pct;			// 頂点数
	Dpn				pnt[1];			// 頂点列
}Trm_mrk;

typedef	struct	FIG_ELM {			// 要素構造
	int				typ;			// 要素種別
	int				mrk;			// 短点形状付加属性
	int				lyr;			// 図形階層(ユーザー領域)
	Lnk				sbl[1];			// 下層要素(Fig_elm型で下層レイヤを定義)
	struct	{						// 包含矩形
		Dpn			sp;				// 左下隅座標
		Dpn			ep;				// 右上隅座標
	}srn;
	int				vis;			// 可視属性
	int				sel;			// 選択フラグ
	int				pcl;			// ペンの色
	int				bcl;			// ブラシの色
	union	{						// 要素定義
		Lin			lin;			// 線分要素
		Rct			rct;			// 矩形要素
	}elm;
	LPVOID			usr;			// Pointer of user data
}Fig_elm;

typedef	struct	{
	HWND			par;			// 親ウィンドウハンドル
	HWND			wnd;			// 子ウィンドウハンドル
	Area			dvc;			// デバイス領域（親ウィンドウ上の相対位置）
	Area			vpt;			// デバイス描画領域（ビューポート）
	Area			wpt;			// ワールド座標領域（ウィンドウポート）
	Mtx_def			s2d[1];			// ワールド－デバイス変換行列
	Mtx_def			d2s[1];			// デバイス－ワールド変換行列
	Csr				top;			// 上層幾何構造列
	double			frt;			// フォントサイズ変換係数
	int				ope;			// 図形操作処理実行中フラグ
	int				drg;			// ドラッグ中フラグ
	int				rbr;			// ラバーバンド表示中フラグ
	int				rtp;			// ラバーバンド種別（図形作成用）
	int				dtl;			// 操作詳細
	int				dir;			// 変形／切断方向
	int				sts;			// マウス状態
	int				dst;			// マウス状態(詳細)
	int				drm;			// Dragging method on dagging
	Fig_elm			*fnd;			// 詳細操作時にヒットした要素
	struct	{
		Dpn			sp;				// 図形始点とマウスポイントの差
		Dpn			ep;				// 図形終点とマウスポイントの差
	}dif;
	Fig_elm			*hit;			// 直前にヒットした要素
	Ipn				mss;			// 選択開始マウス座標
	Ipn				mse;			// 選択終了マウス座標
	Fig_elm			*rht;			// Hit figure on right button down

	Mus_Cbk			mcb;			// マウス操作コールバック
	Ope_Cbk			ocb;			// 図形操作コールバック
	Drw_Cbk			dcb;			// 描画操作コールバック
	Lnk				mrk[1];			// 端点形状格納域
	HINSTANCE		ins;			// インスタンスハンドル
	LPVOID			usr;			// ユーザーデータ
}Fig_mng;

HWND	Fig_Ini( HINSTANCE, HWND, Area *, Area *, Area *, Csr, LPVOID );
Fig_mng	*Fig_Set_Cbk( HANDLE, Mus_Cbk, Ope_Cbk, Drw_Cbk, LPVOID );
void	Fig_Set_Swn( HANDLE, Area *, int );
void	Fig_Set_Dwn( HANDLE, Area *, Area *, Area *, int, int );
void	Fig_Rdw( HANDLE );
void	Fig_Shw( HANDLE, int );
void	Fig_Rct_Drw( HANDLE, Dpn *, Dpn * );
int		Fig_Set_Ope( HANDLE, int, int );
void	Fig_Drw_Elm( HANDLE, Fig_elm * );
int		Fig_Fnt_Siz( HANDLE, double );
int		Fig_Ent_Tmk( HANDLE, Trm_mrk * );
void	Fig_Trm( HANDLE );

#endif
