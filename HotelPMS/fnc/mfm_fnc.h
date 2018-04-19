/*
	Functions for Main Frame Definition
*/

#ifndef	mfm_fncH
#define	mfm_fncH

#include "..\stdafx.h"
#include "..\MainFrm.h"
#include "..\ChildFrm.h"

#define	TOL_HGT			36					// ツールバーの高さ
#define	CBX_HGT			14					// コンボボックスの高さ

#define	TEV_INI			1					// 初期化処理実行タイマーイベント
#define	TIM_INI			10					// 初期化処理実行タイマーインターバル値

#define	TEV_ITV			2					// Supervising interval timer event ID
#define	TIM_ITV			1000				// Supervising interval timer tick value

#define	GBD_CNT			5					// Counter of getting business date ( TIM_ITV * this value ) 

// 初期化ステージ番号定義
#define	IST_INI			1			// INIファイル読込
#define	IST_DBC			2			// データベース接続開始
#define	IST_BDT			3			// Getting business_date
#define	IST_MST			4			// マスターデータ取得
#define	IST_SWD			5			// 検索ウィンドウ開始
#define	IST_LGI			6			// ログイン
#define	IST_END			99			// 初期化終了

int		Mfm_Cre_BsB( CBookingStsToolBar *, CWnd * );	// Create a booking status tool bar
int		Mfm_Cre_ITB( CInfoToolBar *, CWnd * );			// Create a info tool bar
int		Mfm_Cre_RDr( CRoomDirectToolBar *, CWnd * );	// Create a inputting directnumber tool bar
int		Mfm_Ini_Prc(  CMainFrame * );					// Initialize process of MainFrame
int		Mfm_Itv_Prc(  CMainFrame * );					// Interval process of MainFrame
CView	*Mfm_Get_Bsv( CMainFrame * );					// Getting the BookingStatusView pointer
void	Mfm_Cre_Rsv( CMainFrame * );					// Create a new reservation
int		Mfm_Log_Out( CMainFrame *, int );				// Exec logout
int		Mfm_Chg_Pwd( CMainFrame * );					// Change password
void	Mfm_Dir_Rch( CMainFrame *, DWORD );				// Launch direct

#endif
