/*
	初期化データアクセス関数
*/

#include <stdio.h>
#include ".\Common\lnk_mng.h"
#include "pms_ini.h"

static	Ini_fil		Ini_dat;

static	Ini_inf		Ini_def[] = {				// 初期化ファイル定義
	{ SCT_SYS, IKY_STP, FALSE, FALSE, ITP_INT, &Ini_dat.stp      },
	{ SCT_COV, IKY_TSO, FALSE, FALSE, ITP_INT, &Ini_dat.sys->tso },
	{ NULL,    IKY_TTO, FALSE, FALSE, ITP_INT, &Ini_dat.sys->tto },
	{ SCT_DBS, IKY_DBN, FALSE, FALSE, ITP_STR, &Ini_dat.dbs->dsn },
	{ NULL,    IKY_DBU, FALSE, FALSE, ITP_STR, &Ini_dat.dbs->uid },
	{ NULL,    IKY_DBP, FALSE, FALSE, ITP_STR, &Ini_dat.dbs->pwd },
	{ NULL,    IKY_DCT, FALSE, FALSE, ITP_INT, &Ini_dat.dbs->cto },
	{ NULL,    IKY_CSC, FALSE, FALSE, ITP_STR, &Ini_dat.dbs->scc },
//	{ NULL,    IKY_CSL, FALSE, FALSE, ITP_INT, &Ini_dat.dbs->pfl },
	{ SCT_HIS, IKY_WPC, TRUE,  FALSE, ITP_INT, &Ini_dat.wpc      },
	{ NULL,    IKY_FLS, FALSE, FALSE, ITP_INT, &Ini_dat.fls      },
	{ NULL,    IKY_OBC, FALSE, FALSE, ITP_STR, &Ini_dat.obc      },
	{ NULL,    IKY_OFC, FALSE, FALSE, ITP_STR, &Ini_dat.ofc      },
	{ SCT_PVT, IKY_OPC, FALSE, FALSE, ITP_STR, &Ini_dat.opc      }
};

static	void	mak_fph(	// 初期化ファイルパスを作成する
WCHAR	*dst,				// 格納領域
int		siz,				// Size of output buffer
WCHAR	*fnm )				// 初期化ファイル名
{
	WCHAR			pth[MAX_PATH], drv[_MAX_DRIVE], dir[_MAX_DIR], nod[_MAX_FNAME], ext[_MAX_EXT];
	
	if ( !!GetModuleFileName( NULL, pth, MAX_PATH ) ){
		_wsplitpath_s( pth, drv, _MAX_DRIVE, dir, _MAX_DIR, nod, _MAX_FNAME, ext, _MAX_EXT );
		swprintf_s( dst, siz, _T("%s%s%s"), drv, dir, fnm );
		//SetCurrentDirectory( pth );
	}
}

static	int		fil_exs(	// ファイル存在検定
WCHAR		*fnm )			// 検定対象ファイル名
{
	int			chk;
	DWORD		atr;

	atr = GetFileAttributes( fnm );
	if ( chk = atr != 0xFFFFFFFF ){
		chk = !( atr & FILE_ATTRIBUTE_DIRECTORY );
	}
	return( chk );
}


Ini_fil *Get_PIni(			// 初期化情報取得
WCHAR	*fnm )				// 初期化ファイル名
{
	int		chk;

	memset( &Ini_dat, 0, sizeof(Ini_dat) );

	mak_fph( Ini_dat.fnm, memcnt(Ini_dat.fnm), fnm );
	if ( chk = fil_exs( Ini_dat.fnm ) ){
		chk = Get_Ini( Ini_def, memcnt(Ini_def), Ini_dat.fnm );
	}
	return( chk ? &Ini_dat: NULL );
}

Ini_fil *Get_PIfl(			// 初期化情報ファイル取得
void )						// 引数なし 
{
	return( &Ini_dat );
}

void	Cst_PIni( void )	// 初期化情報破棄
{
	Cst_Ini( Ini_def, memcnt(Ini_def) );
}

int		Set_PIni(			// 初期化情報書込
WCHAR	*fnm )
{
	int		chk;

	chk = Set_Ini( Ini_def, memcnt(Ini_def), Ini_dat.fnm );
	return( chk );
}

int		Set_PIni(			// 初期化情報文字列設定
WCHAR		*sct,			// セクション情報
WCHAR		*key,			// キー情報
WCHAR		*str )			// 設定文字列
{
	int			i;
	WCHAR		*nsc;
	WCHAR		*val, **dst;

	if ( val = (WCHAR *)malloc( wsizeof(wcslen(str)+1) ) ){
		wcscpy_s( val, wcslen(str)+1, str );
		for ( i=0; i<memcnt(Ini_def); i++ ){
			if ( (Ini_def+i)->sct != NULL )	nsc = (Ini_def+i)->sct;
			if ( !wcscmp(nsc, sct) && !wcscmp( (Ini_def+i)->key, key ) ){
				if ( (Ini_def+i)->typ==ITP_STR ){
					if ( !(Ini_def+i)->mcl && !(Ini_def+i)->mlt ){
						dst = (WCHAR **)((Ini_def+i)->val);
						if ( *dst )	free( *dst );
						*dst = val;
						str = NULL;
					}
				}
				break;
			}
		}
		if ( !!str )	free( val );
	}
	return( !!val && !str );
}
