/*
	初期化データアクセス関数定義
*/

#ifndef	pms_iniH
#define	pms_iniH

#include <afx.h>
#include <stdlib.h>
#include ".\Common\sys_def.h"
#include ".\Common\lnk_mng.h"
#include ".\Common\ini_inf.h"

#define	INI_FNM		_T("HotelPms_UTF8.ini")

#define	SCT_SYS		_T("SystemComposition")
#define	SCT_COV		_T("ConstantValues")
#define	SCT_DBS		_T("Database")
#define	SCT_HIS		_T("History")
#define	SCT_PVT		_T("PrivateInformation")

#define	IKY_STP		_T("SystemType")
#define	IKY_TSO		_T("ThreadStartTimeout")
#define	IKY_TTO		_T("ThreadTerminateTimeout")

#define	IKY_DBN		_T("DBName")
#define	IKY_DBU		_T("UserID")
#define	IKY_DBP		_T("Password")
#define	IKY_CSC		_T("ChecksumCode")
//#define	IKY_CSL		_T("ChecksumLength")
#define	IKY_DCT		_T("ConnectionTimeout")

#define	IKY_WPC		_T("WindowPlacement")
#define	IKY_FLS		_T("FullScreen")
#define	IKY_OBC		_T("LastBranchCode")
#define	IKY_OFC		_T("LastFacilityCode")

#define	IKY_OPC		_T("OperatorCode")


typedef	struct	{					// プログラム初期化情報
	WCHAR		fnm[MAX_PATH];		// 初期化ファイル名
	int			stp;				// システムタイプ
	Lnk			wpc[1];				// ウィンドウ状態
	int			fls;				// フルスクリーンモード
	SYS_INI		sys[1];				// システム情報初期化仕様
	DBS_INI		dbs[1];				// データベース情報
	WCHAR		*opc;				// オペレーターコード
	WCHAR		*obc;				// Branch code when last use
	WCHAR		*ofc;				// Facility code when last use
}Ini_fil;

Ini_fil *Get_PIni( WCHAR * );					// 初期化情報取得
Ini_fil *Get_PIfl( void );						// 初期化情報ファイル取得
void	Cst_PIni( void );						// 初期化情報破棄
int		Set_PIni( WCHAR * );					// 初期化情報書込
int		Set_PIni( WCHAR *, WCHAR *, WCHAR * );	// 初期化情報文字列設定

#endif