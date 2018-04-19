/*
	ＷＩＮ３２ 印刷 ルーチン
	印刷用紙情報取得関数ファイル
	Copyright(C) Arsnova Corp. 2003
*/

#include <afx.h>
#include <Windows.h>
#include <Winnt.h>
#include <Winspool.h>
#include <stdlib.h>
#include "..\Common\bsc_def.h"
#include "print.h"
#include "ppr_inf.h"

int		Get_Ppr_Nbr(			// 用紙番号取得
short		*nbr,				// 用紙番号
WCHAR		*pnm,				// プリンター名
WCHAR		*ppr )				// 用紙名
{
	int					fnd = false;			// サポート有無
	WCHAR 				*out;					// 番号、用紙名出力先
	WCHAR 				*wrk;					// ワークポインタ
	unsigned long		plc = 0;				// ターゲット位置
	unsigned long		cnt;					// 番号、用紙名個数
	unsigned long 		i;

	/*	サポート用紙名取得、探索	*/
	cnt = DeviceCapabilities( pnm, _T("LPT1"), DC_PAPERNAMES, NULL, NULL );
	out = (WCHAR *)calloc( cnt, wsizeof(MAX_PPR) );
	if ( out ){
		DeviceCapabilities( pnm, _T("LPT1"), DC_PAPERNAMES, out, NULL );
		wrk = out;
		for( i = 0; i < cnt; i++ ){
			if ( wcscmp( wrk, ppr ) == 0 ){
				fnd = true;
				break;
			}
			plc++;
			wrk += MAX_PPR;
		}
		free( out );
	}
	/*	用紙番号取得、探索	*/
	if ( fnd ){
		cnt = DeviceCapabilities( pnm, _T("LPT1"), DC_PAPERS, NULL, NULL );
		if ( cnt >= plc + 1 ){
			out = (WCHAR *)calloc( cnt, wsizeof( sizeof(WORD) ) );
			if ( out ){
				DeviceCapabilities( pnm, _T("LPT1"), DC_PAPERS, out, NULL );
				wrk = out;
				wrk = (WCHAR *)((WORD *)wrk + plc);
				*nbr = *((WORD *)wrk );
				free( out );
			}
		}
		else	fnd = false;
	}
	return( fnd );
}

int		Get_Ppr_Nam(			// 用紙番号から用紙名取得
WCHAR		*ppr,				// 用紙名
int			siz,				// Sizeof output buffer
short		nbr,				// 用紙番号
WCHAR		*pnm )				// プリンター名
{
	int					fnd = false;			// サポート有無
	WCHAR 				*out;					// 番号、用紙名出力先
	WORD				*wrk;					// ワークポインタ
	unsigned long		plc = 0;				// ターゲット位置
	unsigned long		cnt;					// 番号、用紙名個数
	unsigned long 		i;

	/*	用紙番号取得、探索	*/

	cnt = DeviceCapabilities( pnm, _T("LPT1"), DC_PAPERS, NULL, NULL );
	out = (WCHAR *)calloc( cnt, wsizeof( sizeof(WORD) ) );
	if ( out ){
		DeviceCapabilities( pnm, _T("LPT1"), DC_PAPERS, out, NULL );
		wrk = (WORD *)out;
		for( i = 0; i < cnt; i++ ){
			if ( *wrk == nbr ){
				fnd = true;
				break;
			}
			plc++;
			wrk++;
		}
		free( out );
	}
	/*	サポート用紙名取得、探索	*/
	if ( fnd ){
		cnt = DeviceCapabilities( pnm, _T("LPT1"), DC_PAPERNAMES, NULL, NULL );
		if ( cnt >= plc + 1 ){
			out = (WCHAR *)calloc( cnt, wsizeof(MAX_PPR) );
			if ( out ){
				DeviceCapabilities( pnm, _T("LPT1"), DC_PAPERNAMES, out, NULL );
				wrk = (WORD *)out;
				wrk = (WORD *)((char *)wrk + ( MAX_PPR * plc ));
				wcscpy_s( ppr, siz, (WCHAR *)wrk );
			}
			free( out );
		}
	}
	return( fnd );
}

int		Get_Ppr_Siz(			// 用紙サイズ取得
SIZEL		*siz,				// 用紙幅、高さ
WCHAR		*pnm,				// プリンター名
WCHAR		*ppr )				// 用紙名
{
	int	 				suc = false;
	int	 				err = ERROR_INSUFFICIENT_BUFFER;// エラーコード
	unsigned long		byt = 0;
	HANDLE				prh;					// プリンターハンドル
	FORM_INFO_1			*dat = NULL;
	PRINTER_DEFAULTS	def;					// プリンター初期情報

	def.pDatatype = NULL;
	def.pDevMode = NULL;
	def.DesiredAccess = PRINTER_ALL_ACCESS;
	if ( OpenPrinter( pnm, &prh, &def ) ){
		GetForm( prh, ppr, 1, (LPBYTE)dat, 0, &byt );
		if ( err == ERROR_INSUFFICIENT_BUFFER ){
			dat = (FORM_INFO_1 *)calloc( 1, byt );
			if ( dat ){
				if ( GetForm( prh, ppr, 1, (LPBYTE)dat, byt, &byt ) ){;
					siz->cx = ( dat->Size.cx );
					siz->cy = ( dat->Size.cy );
					suc = true;
				}
				free( dat );
			}
		}
		ClosePrinter( prh );
	}
	return ( suc );
}

int		Enm_Prn(				// プリンター名列挙
char		*ptr,				// 書込み先
int			siz )				// Size of output buffer
{
	int				suc;
	DWORD			prm, byt, cnt, i;
	LPBYTE			use, buf;

	prm = PRINTER_ENUM_CONNECTIONS|PRINTER_ENUM_LOCAL;
	byt = cnt = 0;
	EnumPrinters( prm, NULL, 4, NULL, 0, &byt, &cnt );

	if ( GetLastError() == ERROR_INSUFFICIENT_BUFFER ){
		if ( ( buf = ( LPBYTE )calloc( 1, byt ) ) != NULL ){
			suc = EnumPrinters( prm , NULL, 4, buf, byt, &byt, &cnt );
			if ( suc && ptr ){
				use = (LPBYTE)ptr;
				for ( i=0; i<cnt; i++ ){
					wcscpy_s( (WCHAR *)use, siz,
								(((PRINTER_INFO_4 *)buf) + i )->pPrinterName );
					use += MAX_PNM;
				}
			}
			free( buf );
		}
	}
	return( cnt );
}

int			Enm_Ppr(			// 用紙名列挙
WCHAR		*ptr,				// 書込み先
WCHAR		*pnm )				// プリンター名
{
	int 		cnt;

	cnt = DeviceCapabilities( pnm, _T("LPT1"), DC_PAPERNAMES, NULL, NULL );
	if ( ptr && cnt != -1 )
		DeviceCapabilities( pnm, _T("LPT1"), DC_PAPERNAMES, ptr, NULL );

	return( cnt );
}

