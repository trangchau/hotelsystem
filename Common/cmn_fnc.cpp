/*
	各種雑多関数群
*/

#include <afxwin.h>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "bsc_def.h"
#include "cmn_fnc.h"
//#include "dte_mng.h"
//#include "sys_log.h"
//#include "trd_mng.h"
#include "bin_fil.h"

CRITICAL_SECTION	Trc_crs[1];	// トレースの出力排他制御
int					Ini_trc;	// トレース初期化済フラグ

/*
void		Get_Vsn(			// プログラムバージョンの取得
HWND			wnd,			// バージョン表示ウィンドウハンドル
GAZS_MNG		*mng )			// システム管理構造
{
	// version.libをプロジェクトに追加する事
	char				pth[MAX_PATH]; 
	DWORD				dmy, siz; 
	LPVOID				dat, buf; 
	UINT				bsz; 
	VS_FIXEDFILEINFO	*inf;

	pth[GetModuleFileName( mng->ins, pth, sizeof(pth) )] = 0; 

	if ( siz = GetFileVersionInfoSize( pth, &dmy ) ){
		if ( dat = calloc( 1, siz ) ){
			if ( GetFileVersionInfo( pth, 0, siz, dat ) ){
				if ( VerQueryValue( dat, _T("\\"), &buf, &bsz)){

					inf = (VS_FIXEDFILEINFO *)buf; 
					mng->ver[0] = (inf->dwFileVersionMS) >> 16;
					mng->ver[1] = (inf->dwFileVersionMS) & 0xffff;
					mng->ver[2] = (inf->dwFileVersionLS) >> 16;
					mng->ver[3] = (inf->dwFileVersionLS) & 0xffff;

					//if ( wnd ){
					//	sprintf_s( pth, MAX_PATH, "Release ver.%d.%d.%d.%d",
					//		mng->ver[0], mng->ver[1], mng->ver[2], mng->ver[3]);
					//	SetWindowText( wnd, pth );
					//}
				}
			}
			free( dat );
		}
	}
}
*/

void	DBG_OUT(			// デバッグ出力
char		*src,			// ソースファイル名
int			lin,			// 行
char		*fmt,			// 書式
... )						// 可変引数リスト
{
	char		div[4][MAX_PATH]; 
	int			pos=0;
	va_list		argp;
	char		_out_buf[2048];

	_splitpath_s( src, div[0], sizeof(div[0]), div[1], sizeof(div[1]), div[2], sizeof(div[2]), div[3], sizeof(div[3]) );
	va_start( argp, fmt );
	pos = vsprintf_s( _out_buf, sizeof(_out_buf), fmt, argp );
	va_end( argp );
	sprintf_s( _out_buf+pos, sizeof(_out_buf)-pos-1, " : (%s%s (%d))\n", div[2], div[3], lin );
	TRACE( _out_buf );
#ifdef	TRS_PUT				// トレースをファイルへ出力
	FILE		*fp;

	if ( !!Ini_trc ){
		EnterCriticalSection( Trc_crs );
		if ( fp = fopen( TRC_FNM, "a+" ) ){
			fprintf( fp, _out_buf );
			fclose( fp );
		}
		LeaveCriticalSection( Trc_crs );
	}
#endif
}

int		Fil_Exs(			// ファイル存在検定
LPCWSTR		fnm )			// 検定対象ファイル名
{
	int			chk;
	DWORD		atr;

	atr = GetFileAttributes( fnm );
	if ( chk = atr != 0xFFFFFFFF ){
		chk = !( atr & FILE_ATTRIBUTE_DIRECTORY );
	}
	return( chk );
}

void	WStr_NCpy(					// 最大サイズ指定の文字列複写
WCHAR		*dpt,					// 格納領域
WCHAR		*spt,					// 転送元
int			siz )					// 最大文字数
{
	int				now=0, cwd;
	WCHAR	*src, *dst;

	src = (WCHAR *)spt;
	dst = (WCHAR *)dpt;

	while ( *src ){
		//cwd = is_kanji(*src) ? 2: 1;
		cwd = 1;
		if ( now+cwd > siz )	break;
		*dst++ = *src++;
		if ( cwd==2 )	*dst++ = *src++;
		now += cwd;
	}
	*dst = '\0';
}

WCHAR	*Add_Str(					// 文字列の追加
WCHAR		*src,					// 元文字列
WCHAR		*str )					// 追加文字列
{
	int			len=0;
	WCHAR		*dst;

	if ( src )		len = (int)wcslen(src);

	len += (int)wcslen(str)+1;
	if ( dst = (WCHAR *)calloc( 1, wsizeof(len) ) ){
		if ( src )	wcscpy_s( dst, len, src );
		wcscat_s( dst, len, str );
	}
	if( src )	free( src );

	return( dst );
}

int		xtoi(							// hex文字列→整数変換
WCHAR		*str )						// 対象文字列
{
	int			ans = 0;

	while( *str ){
		ans *= 16;
		ans += to_dgt(*str);
		str++;
	}
	return( ans );
}

WORD		Bin_BCD(		// バイナリ→BCD変換
int			bin )			// バイナリデータ
{
	WORD		bcd=0, wbn, dgt=1, rmn;

	wbn = (WORD)bin;
	while ( wbn ){
		rmn = wbn % 10;
		bcd |= rmn << 4 * (dgt-1);
		wbn /= 10;
		dgt++;
	}
	return( bcd );
}

int			BCD_Bin(		// BCD→バイナリ変換
WORD		bcd )			// BCDデータ
{
	int			bin=0, dgt=1;

	while( bcd ){
		bin += ( bcd & 0xf ) * dgt;
		bcd >>= 4;
		dgt *= 10;
	}
	return( bin );
}

WCHAR	*trim(				// 文字列トリミング
WCHAR		*str )			// 対象文字列
{
	int			pos = (int)wcslen(str);

	while( --pos>=0 && ( *(str+pos)==_T(' ') || *(str+pos)==_T('\t') ) ){
		*(str+pos) = '\0';
	}
	return( str );
}

void	Get_Dir(				// ディレクトリ部分のみ取り出し
char	*path )
{
	BOOL			flg = FALSE;
	char			*p = NULL;

	while(*path != '\0') {
		if(flg) {
			flg = FALSE;
		} else {
			if(is_kanji(*path)) {
				flg = TRUE;
			} else if(*path == '\\') {
				p = path;
			}
		}
		path++;
	}
	if(p != NULL)	*p = '\0';
}

int		Str_UCmp(						// 文字列の大文字小文字無視比較
char			*dst,					// 被比較対象文字列
char			*src )					// 比較対象文字列
{
	unsigned char	*uds, *usr;
	int				chk=0;

	uds = (unsigned char *)dst;
	usr = (unsigned char *)src;

	do{
		if ( *uds>='a' && *uds<='z' ){
			chk = ((*uds)&0xdf) - ((*usr)&0xdf);
		}
		else{
			chk = (*uds) - (*usr);
		}
		if ( !!chk )		break;
		//uds++;
		usr++;
	}while( *uds++ );
	return( chk );
}

double		Rnd_Dec(					// rounding to ordering digit
double			val,					// 元データ
int				dps,					// 小数点以下桁数
int				rmt )					// 丸め方式
{
	if ( rmt==CEDT_ROUND_TRUNCATION ){
		val = floor( val * powf(10.0,(float)dps) ) / powf(10.0,(float)dps);
	}
	else{
		val = floor( val * powf(10.0,(float)dps ) + 0.5 ) / powf(10.0,(float)dps);
	}

	return( val );
}

WCHAR		*Cma_Edt(					// comma edit
double			val,					// editting value(sould be after rounding)
int				dps )					// digit of below the decimal point
{
	int			siz, cct, cma, idx, pfx, sgn;
	WCHAR		*rsl, *spt, *dpt, buf[40];

	swprintf_s( buf, memcnt(buf), _T("%.*f"), dps, val );

	pfx = dps>0 ? dps+1: 0;
	siz = (int)wcslen( buf );
	sgn = *buf == _T('-') ? 1: 0;

	cct = ( (siz-pfx-sgn) / 3 - 1 ) + ( ( (siz-pfx-sgn) % 3 ) ? 1:0 );
	if ( !!( rsl = (WCHAR *)malloc( wsizeof(siz+cct+sgn+1) ) ) ){
		dpt = rsl + siz + cct;
		*dpt-- = 0;
		spt = buf + siz - 1;
		while( pfx-- )	*dpt-- = *spt--, siz--;
		for ( idx=0, cma=0; idx<siz; idx++ ){
			*dpt-- = *spt--;
			if ( cct && ++cma==3 && *spt!=_T('-') ){
				*dpt-- = _T(',');
				cma = 0;
				cct--;
			}
		}
	}

	return( rsl );
}

WCHAR	*Prg_Cma(						// コンマを削除
WCHAR	*src )							// 元文字列
{
	WCHAR		*spt=src, *dpt=src;

	if ( *spt ){
		while( *spt ){
			if ( *spt!=_T(' ') && *spt!=_T(',') ){
				*dpt++ = *spt;
			}
			spt++;
		}
		*dpt = 0;
	}
	return( src );
}
