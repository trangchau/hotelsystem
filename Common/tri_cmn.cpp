/*
	部品管理システム共通関数
*/

#include <stdlib.h>
#include <stdio.h>
#include "bsc_def.h"
#include "dte_mng.h"
#include "tri_def.h"

#define	is_prsfact(x)		( (x)=='7' || (x)=='8' || (x)=='G' || (x)=='H' )
#define	is_digit(x)			( (x)>='0' && (x)<='9' )
#define	is_alpha(x)			( (x)>='A' && (x)<='Z' )

static	int		dgt_str(		// 数値文字列検定
char		*str )				// 検査対象文字列
{
	while( *str ){
		if ( !is_digit(*str) )	break;
		str++;
	}
	return( !(*str) );
}

static	int		dte_str(		// 数値文字列検定
char		*str )				// 検査対象文字列
{
	int		pos=0, chk;

	while( *str ){
		if ( pos==4 || pos==7 )	chk = *str=='/' || *str=='.';
		else					chk = is_digit(*str);
		if ( !chk )	break;
		pos++;
		str++;
	}
	return( !(*str) );
}

static int		str_ivl(		// 文字列→数値変換
char		*str,				// 文字列
int			siz )				// サイズ
{
	char		buf[20];

	siz = siz<sizeof(buf) ? siz: sizeof(buf-1);
	memcpy( buf, str, siz );
	buf[siz] = '\0';

	return( atoi( buf ) );
}


int		Axn_Chk(				// 車軸管理番号正当性チェック
char		*axn )				// 車軸管理番号	
{
	int			chk;
	
	chk  = (int)strlen(axn) == AXL_LEN;
	chk &= is_prsfact( *(axn+1) );
	chk &= is_digit( *axn );
	chk &= dgt_str( axn+2 );
	
	return( chk );
}

int		Whl_Chk(				// 車輪管理番号正当性チェック
char		*axn )				// 車軸管理番号	
{
	return( TRUE );
}

int		Bkd_Chk(				// ブレーキディスク管理番号正当性チェック
char		*bdn )				// ブレーキディスク管理番号	
{
	int			chk, len;

	len = (int)strlen(bdn);
	chk  = len>=5 && len<=11;
	chk &= *(bdn+3)=='-';
	chk &= is_alpha( *(bdn+0) ) && is_alpha( *(bdn+2) );
	chk &= is_digit( *(bdn+1) ) && dgt_str( bdn+4 );

	return( chk );
}

int		Brg_Chk(				// 内輪管理番号正当性チェック
char		*brn )				// 内輪管理番号	
{
	return( TRUE );
}

int		Tri_S2D(				// 入力した日付文字列を正規変換
char		*inp,				// 入力バッファ
int			siz )				// 入力バッファサイズ
{
	int			chk, len;
	int			yer, mth, day, mxd;
	int			max[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	len = (int)strlen(inp);
	chk = len==8 || len==10;
	chk &= len==8 ? dgt_str(inp): dte_str( inp );
	if ( chk ){
		yer = str_ivl( inp, 4 );
		mth = str_ivl( inp+( len==8 ? 4: 5 ), 2 );
		day = str_ivl( inp+( len==8 ? 6: 8 ), 2 );
		chk  = yer >= 1990;
		chk &= mth>0 && mth<13;
		if ( chk ){
			if ( mth==2 )	mxd=is_leap(yer) ? 29: 28;
			else			mxd = max[mth-1];
			chk &= day>0 && day<=mxd;
		}
	}
	if ( chk ){
		mxd = atoi(inp);
		sprintf_s( inp, siz, "%04d/%02d/%02d", yer, mth, day );
	}
	return( chk ? mxd: -1 );
}

void	edt_dte(			// 日付データを可視データへ変換
char		*dst,			// 格納領域
int			siz,			// 格納領域サイズ
int			dte )			// 日付データ
{
	if ( dte ){
		sprintf_s( dst, siz, "%04d/%02d/%02d",
			dte/10000, (dte/100)%100, dte%100 );
	}
	else{
		*dst = '\0';
	}
}

