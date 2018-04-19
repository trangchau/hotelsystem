/*
	Booking status graphic operation manipulator
*/

#ifndef	gph_dspH
#define	gph_dspH

#include "..\..\Common\bsc_def.h"
#include "..\..\Common\dte_mng.h"
#include "bst_sql.h"

// Color codes
#define	COL_STY			RGB(128,128,255)			// Stay arrow 
#define	COL_DUS			RGB(  0,208, 0)				// Day use arrow
#define	COL_ELO			RGB(255,128,128)			// Early checkin/Late checkout terminal symbole 

#define	DGG_BCL			RGB(192, 255, 192)			// Day gage background color 

// Figure layers
#define	LYR_HGG			1							// Horizon gage object
#define	LYR_VGG			2							// Vertical dage object
#define	LYR_DAT			3							// Data object
#define	LYR_ERR			4							// Error object

// Graphic unit
#define	GPH_UNT			40							// Pixcel width unit per 1 data 
#define	GPH_HIT			30							// Pixcel height unit per 1 line
#define	ARW_HIT			24							// Pixcel height unit per 1 arrow
#define	GAG_HGT			32
#define	GPH_WID(x)		(GPH_UNT*2.0*(x))			// Pixcel unit of 1 day

#define	LIN_SEP			0x8000						// 行分割線属性
#define	is_sep_(x)		(!!((x)&LIN_SEP))			// 行分割線判定
#define	lin_of_(x)		((x)&0x7fff)				// 分割線の行取り出し

typedef	struct	{									// 描画データユーザー領域
	short				mth;						// 月
	short				day;						// 日
}GAG_DAT;

typedef	struct	{									// グラフ入力管理データ
	HWND				ghd;						// 日付目盛ウィンドウハンドル
	Lnk					grt[1];						// 日付目盛根
	Area				gsy[1];						// 日付目盛システム座標
	Area				gdv[1];						// 日付目盛デバイス座標

	HWND				hnd;						// 予約グラフウィンドウハンドル
	Lnk					rot[1];						// 予約グラフ根
	Area				sys[1];						// 予約グラフシステム座標
	Area				dvc[1];						// 予約グラフデバイス座標

	int					aln;						// 左矢印登録番号
	int					arn;						// 右矢印登録番号
	int					rln;						// 左台形登録番号
	int					rrn;						// 右台形登録番号

	int					idx;						// 日付テーブルインデックス
	double				coe;						// 拡大率
	HFONT				fnt;						// フォントハンドル
	int					fhg;						// フォント高さ
	int					rnw;						// 部屋番号幅
	int					rtw;						// 部屋タイプ幅
	Day					std[1];						// 期間開始日付
	Day					edd[1];						// 期間終了日付
	int					dys;						// 期間日数

	HWND				par;						// 親ウィンドウハンドル
	Lnk					rom[1];						// 部屋リスト

	Fig_mng				*fig;						// Fig_mng pointer
	PMS_MNG				*mng;						// Property Management System Management structure
}BST_MNG;

typedef	struct	{									// グリッド通知構造体
	int					ntf;						// 通知コード
	int					cid;						// コントロールＩＤ
	int					msx;						// Mouse x device coodinate
	int					msy;						// Mouse y device coodinate
	int					rii;						// Reservation id of hitting data
	int					rdi;						// Reservation detail id of hitting data
	LPVOID				usr;						// Data pointer of user definition
}GPH_NTF;

typedef	struct	{									// room and day position
	BST_ROM_LST			*rom;						// room information
	Day					osd[1];						// Use Start date( old )
	Day					oed[1];						// Use End date( old ) 
	Day					nsd[1];						// USe Start date ( new )
	Day					ned[1];						// Use End date ( new )
	Fig_elm				*elm;						// pointer of element of figure
}GPH_RDY;

#define	WM_GRAPHNOTIFY		(WM_USER+16385)			// Notify message from bst_hph to parent window

// Notify codes
#define	GPN_DRAG			1						// Drag event
#define	GPN_CREATE			2						// Create a reservation event
#define	GPN_CHANGE			3						// Change a reservation event
#define	GPN_RBUTTONDOWN		20						// Rbuttondown event

#define ON_WM_GRAPHNOTIFY() \
	{ WM_GRAPHNOTIFY, 0, 0, 0, AfxSig_lwl, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< LRESULT (AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM) > \
		( &ThisClass :: OnGraphNotify)) },

typedef	void (*APL_CBK)( int, int, int, LPVOID );	// ユーザーコールバック関数
typedef	void (*RDN_CBK)( int, int, Dpn *, LPVOID );	// ユーザーコールバック関数
typedef	void (*DRG_CBK)( Dpn *, Dpn *, LPVOID );	// ユーザーコールバック関数

void	Chg_Mod( int, BST_MNG * );				// 操作モード切替え
void	Chg_Dvc( HANDLE, Area *, Area * );			// デバイスサイズ変更
void	Chg_Pos( HANDLE, Area * );					// 位置移動
void	Trm_Dsp( HANDLE );							// 図形表示終了処理
void	Siz_Gph( BST_MNG *, int, int, int, int );	// グラフウィンドウサイズ変更処理
void	Ers_Bok( BST_MNG * );						// Cancellation of booking
int		Mak_Gph( BST_MNG *, HINSTANCE, HWND, int );	// 子ウィンドウ生成処理
int		Rmk_Gph( RECT *, BST_MNG * );				// remake graphic data
int		Del_Gph( int, int, BST_MNG * );				// Delete graph from list ( by reservation_equipment_id or reservation_detail_id )
int		Upd_Gph( BST_ROM_STS *, BST_MNG * );		// Change the graphic element
void	Drw_Rif( CDC *, CWnd *, BST_MNG * );		// ROOM情報描画
void	Dst_Gph( Csr );								// delete all status data

#endif
