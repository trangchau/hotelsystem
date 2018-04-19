/*
	ＷＩＮ３２ 印刷 ルーチン
	用紙情報設定ファイル
	Copyright(C) Arsnova Corp. 2003
*/

#include <afx.h>
#include <windows.h>
#include <winnt.h>
#include "print.h"
#include "ppr_siz.h"
#include "ppr_inf.h"
#include "..\Common\bsc_def.h"

static	struct Paper{					// 用紙サイズ取得構造体
	short	nam;						// 用紙名
	short	wid;						// 用紙幅	(1/10mm)
	short	hgt;						// 用紙高さ	(1/10mm)
}Ppr[] ={
	{ DMPAPER_LETTER				, 2159, 2794 },// Letter 8 1/2 x 11 in
	{ DMPAPER_LETTERSMALL			, 2159, 2794 },// Letter Small 8 1/2 x 11 in
	{ DMPAPER_TABLOID				, 2794, 4318 },// Tabloid 11 x 17 in
	{ DMPAPER_LEDGER				, 4318, 2794 },// Ledger 17 x 11 in
	{ DMPAPER_LEGAL					, 2159, 3556 },// Legal 8 1/2 x 14 in
	{ DMPAPER_STATEMENT				, 1397, 2159 },// Statement 5 1/2 x 8 1/2 in
	{ DMPAPER_EXECUTIVE				, 1842, 2667 },// Executive 7 1/4 x 10 1/2 in
	{ DMPAPER_A3					, 2970, 4200 },// A3 297 x 420 mm
	{ DMPAPER_A4					, 2100, 2970 },// A4 210 x 297 mm
	{ DMPAPER_A4SMALL				, 2100, 2970 },// A4 Small 210 x 297 mm
	{ DMPAPER_A5					, 1480, 2100 },// A5 148 x 210 mm
	{ DMPAPER_B4					, 2570, 3640 },// B4 (JIS) 250 x 354
	{ DMPAPER_B5					, 1820, 2570 },// B5 (JIS) 182 x 257 mm
	{ DMPAPER_FOLIO					, 2159, 3302 },// Folio 8 1/2 x 13 in
	{ DMPAPER_QUARTO				, 2150, 2750 },// Quarto 215 x 275 mm
	{ DMPAPER_10X14					, 2540, 3556 },// 10x14 in
	{ DMPAPER_11X17					, 2794, 4318 },// 11x17 in
	{ DMPAPER_NOTE					, 2159, 2794 },// Note 8 1/2 x 11 in
	{ DMPAPER_ENV_9					, 984, 2254 },// Envelope #9 3 7/8 x 8 7/8
	{ DMPAPER_ENV_10				, 1048, 2413 },// Envelope #10 4 1/8 x 9 1/2
	{ DMPAPER_ENV_11				, 1143, 2635 },// Envelope #11 4 1/2 x 10 3/8
//	{ DMPAPER_ENV_12				,,},// Envelope #12 4 \276 x 11
//	{ DMPAPER_ENV_14				,,},// Envelope #14 5 x 11 1/2
//	{ DMPAPER_CSHEET				,,},// C size sheet
//	{ DMPAPER_DSHEET				,,},// D size sheet
//	{ DMPAPER_ESHEET				,,},// E size sheet
	{ DMPAPER_ENV_DL				, 1100, 2200 },// Envelope DL 110 x 220mm
	{ DMPAPER_ENV_C5				, 2620, 2290 },// Envelope C5 162 x 229 mm
	{ DMPAPER_ENV_C3				, 3240, 4580 },// Envelope C3  324 x 458 mm
	{ DMPAPER_ENV_C4				, 2290, 3240 },// Envelope C4  229 x 324 mm
	{ DMPAPER_ENV_C6				, 1140, 1620 },// Envelope C6  114 x 162 mm
	{ DMPAPER_ENV_C65				, 1140, 2290 },// Envelope C65 114 x 229 mm
	{ DMPAPER_ENV_B4				, 2500, 3530 },// Envelope B4  250 x 353 mm
	{ DMPAPER_ENV_B5				, 1760, 2500 },// Envelope B5  176 x 250 mm
	{ DMPAPER_ENV_B6				, 1760, 1250 },// Envelope B6  176 x 125 mm
	{ DMPAPER_ENV_ITALY				, 1100, 2300 },// Envelope 110 x 230 mm
	{ DMPAPER_ENV_MONARCH			, 984, 1905 },// Envelope Monarch 3.875 x 7.5 in
	{ DMPAPER_ENV_PERSONAL			, 921, 1651 },// 6 3/4 Envelope 3 5/8 x 6 1/2 in
	{ DMPAPER_FANFOLD_US			, 3778, 2794 },// US Std Fanfold 14 7/8 x 11 in
	{ DMPAPER_FANFOLD_STD_GERMAN	, 2159, 3048 },// German Std Fanfold 8 1/2 x 12 in
	{ DMPAPER_FANFOLD_LGL_GERMAN	, 2159, 3302 },// German Legal Fanfold 8 1/2 x 13 in
	{ DMPAPER_ISO_B4				, 2500, 3530 },// B4 (ISO) 250 x 353 mm
	{ DMPAPER_JAPANESE_POSTCARD		, 1000, 1480 },// Japanese Postcard 100 x 148 mm
	{ DMPAPER_9X11					, 2286, 2794 },// 9 x 11 in
	{ DMPAPER_10X11					, 2540, 2794 },// 10 x 11 in
	{ DMPAPER_15X11					, 3810, 2794 },// 15 x 11 in
	{ DMPAPER_ENV_INVITE			, 2200, 2200 },// Envelope Invite 220 x 220 mm
//	{ DMPAPER_RESERVED_48			,,},// RESERVED--DO NOT USE
//	{ DMPAPER_RESERVED_49			,,},// RESERVED--DO NOT USE
//	{ DMPAPER_LETTER_EXTRA			,,},// Letter Extra 9 \275 x 12 in
//	{ DMPAPER_LEGAL_EXTRA			,,},// Legal Extra 9 \275 x 15 in
	{ DMPAPER_TABLOID_EXTRA			, 2969, 4572 },// Tabloid Extra 11.69 x 18 in
	{ DMPAPER_A4_EXTRA				, 2355, 3223 },// A4 Extra 9.27 x 12.69 in
//	{ DMPAPER_LETTER_TRANSVERSE		,,},// Letter Transverse 8 \275 x 11 in
	{ DMPAPER_A4_TRANSVERSE			, 2100, 2970 },// A4 Transverse 210 x 297 mm
//	{ DMPAPER_LETTER_EXTRA_TRANSVERSE	,,},// Letter Extra Transverse 9\275 x 12 in
	{ DMPAPER_A_PLUS				, 2270, 3560 },// SuperA/SuperA/A4 227 x 356 mm
	{ DMPAPER_B_PLUS				, 3050, 4870 },// SuperB/SuperB/A3 305 x 487 mm
	{ DMPAPER_LETTER_PLUS			, 2159, 3223 },// Letter Plus 8.5 x 12.69 in
	{ DMPAPER_A4_PLUS				, 2100, 3300 },// A4 Plus 210 x 330 mm
	{ DMPAPER_A5_TRANSVERSE			, 1480, 2100 },// A5 Transverse 148 x 210 mm
	{ DMPAPER_B5_TRANSVERSE			, 1820, 2570 },// B5 (JIS) Transverse 182 x 257 mm
	{ DMPAPER_A3_EXTRA				, 3220, 4450 },// A3 Extra 322 x 445 mm
	{ DMPAPER_A5_EXTRA				, 1740, 2350 },// A5 Extra 174 x 235 mm
	{ DMPAPER_B5_EXTRA				, 2010, 2760 },// B5 (ISO) Extra 201 x 276 mm
	{ DMPAPER_A2					, 4200, 5940 },// A2 420 x 594 mm
	{ DMPAPER_A3_TRANSVERSE			, 2970, 4200 },// A3 Transverse 297 x 420 mm
	{ DMPAPER_A3_EXTRA_TRANSVERSE	, 3220, 4450 },// A3 Extra Transverse 322 x 445 mm
//	{ DMPAPER_DBL_JAPANESE_POSTCARD , 2000, 1480 },// Japanese Double Postcard 200 x 148 mm
//	{ DMPAPER_A6					, 1050, 1480 },// A6 105 x 148 mm
//	{ DMPAPER_JENV_KAKU2			,,},// Japanese Envelope Kaku #2
//	{ DMPAPER_JENV_KAKU3			,,},// Japanese Envelope Kaku #3
//	{ DMPAPER_JENV_CHOU3			,,},// Japanese Envelope Chou #3
//	{ DMPAPER_JENV_CHOU4			,,},// Japanese Envelope Chou #4
//	{ DMPAPER_LETTER_ROTATED		,,},// Letter Rotated 11 x 8 1/2 11 in
//	{ DMPAPER_A3_ROTATED			, 4200, 2970 },// A3 Rotated 420 x 297 mm
//	{ DMPAPER_A4_ROTATED			, 2970, 2100 },// A4 Rotated 297 x 210 mm
//	{ DMPAPER_A5_ROTATED			, 2100, 1480 },// A5 Rotated 210 x 148 mm
//	{ DMPAPER_B4_JIS_ROTATED		, 3640, 2570 },// B4 (JIS) Rotated 364 x 257 mm
//	{ DMPAPER_B5_JIS_ROTATED		, 2570, 1820 },// B5 (JIS) Rotated 257 x 182 mm
//	{ DMPAPER_JAPANESE_POSTCARD_ROTATED	, 1480, 1000 },// Japanese Postcard Rotated 148 x 100 mm
//	{ DMPAPER_DBL_JAPANESE_POSTCARD_ROTATED	, 1480, 2000 },// Double Japanese Postcard Rotated 148 x 200 mm
//	{ DMPAPER_A6_ROTATED			, 1480, 1050 },// A6 Rotated 148 x 105 mm
//	{ DMPAPER_JENV_KAKU2_ROTATED	,,},// Japanese Envelope Kaku #2 Rotated
//	{ DMPAPER_JENV_KAKU3_ROTATED	,,},// Japanese Envelope Kaku #3 Rotated
//	{ DMPAPER_JENV_CHOU3_ROTATED	,,},// Japanese Envelope Chou #3 Rotated
//	{ DMPAPER_JENV_CHOU4_ROTATED	,,},// Japanese Envelope Chou #4 Rotated
//	{ DMPAPER_B6_JIS				, 1280, 1820 },// B6 (JIS) 128 x 182 mm
//	{ DMPAPER_B6_JIS_ROTATED		, 1820, 1280 },// B6 (JIS) Rotated 182 x 128 mm
//	{ DMPAPER_12X11					, 3048, 2794 },// 12 x 11 in
//	{ DMPAPER_JENV_YOU4				,,},// Japanese Envelope You #4
//	{ DMPAPER_JENV_YOU4_ROTATED		,,},// Japanese Envelope You #4 Rotated
//	{ DMPAPER_P16K					, 1460, 2150 },// PRC 16K 146 x 215 mm
//	{ DMPAPER_P32K					, 970, 1510 },// PRC 32K 97 x 151 mm
//	{ DMPAPER_P32KBIG				, 970, 1510 },// PRC 32K(Big) 97 x 151 mm
//	{ DMPAPER_PENV_1				, 1020, 1650 },// PRC Envelope #1 102 x 165 mm
//	{ DMPAPER_PENV_2				, 1020, 1760 },// PRC Envelope #2 102 x 176 mm
//	{ DMPAPER_PENV_3				, 1250, 1760 },// PRC Envelope #3 125 x 176 mm
//	{ DMPAPER_PENV_4				, 1100, 2080 },// PRC Envelope #4 110 x 208 mm
//	{ DMPAPER_PENV_5				, 1100, 2200 },// PRC Envelope #5 110 x 220 mm
//	{ DMPAPER_PENV_6				, 1200, 2300 },// PRC Envelope #6 120 x 230 mm
//	{ DMPAPER_PENV_7				, 1600, 2300 },// PRC Envelope #7 160 x 230 mm
//	{ DMPAPER_PENV_8				, 1200, 3090 },// PRC Envelope #8 120 x 309 mm
//	{ DMPAPER_PENV_9				, 2290, 3240 },// PRC Envelope #9 229 x 324 mm
//	{ DMPAPER_PENV_10				, 3240, 4580 },// PRC Envelope #10 324 x 458 mm
//	{ DMPAPER_P16K_ROTATED			,,},// PRC 16K Rotated
//	{ DMPAPER_P32K_ROTATED			,,},// PRC 32K Rotated
//	{ DMPAPER_P32KBIG_ROTATED		,,},// PRC 32K(Big) Rotated
//	{ DMPAPER_PENV_1_ROTATED		, 1650, 1020 },// PRC Envelope #1 Rotated 165 x 102 mm
//	{ DMPAPER_PENV_2_ROTATED		, 1760, 1020 },// PRC Envelope #2 Rotated 176 x 102 mm
//	{ DMPAPER_PENV_3_ROTATED		, 1760, 1250 },// PRC Envelope #3 Rotated 176 x 125 mm
//	{ DMPAPER_PENV_4_ROTATED		, 2080, 1100 },// PRC Envelope #4 Rotated 208 x 110 mm
//	{ DMPAPER_PENV_5_ROTATED		, 2200, 1100 },// PRC Envelope #5 Rotated 220 x 110 mm
//	{ DMPAPER_PENV_6_ROTATED		, 2300, 1200 },// PRC Envelope #6 Rotated 230 x 120 mm
//	{ DMPAPER_PENV_7_ROTATED		, 2300, 1600 },// PRC Envelope #7 Rotated 230 x 160 mm
//	{ DMPAPER_PENV_8_ROTATED		, 3090, 1200 },// PRC Envelope #8 Rotated 309 x 120 mm
//	{ DMPAPER_PENV_9_ROTATED		, 3240, 2290 },// PRC Envelope #9 Rotated 324 x 229 mm
//	{ DMPAPER_PENV_10_ROTATED		, 4580, 3240 },// PRC Envelope #10 Rotated 458 x 324 mm
	{ DMPAPER_USER					,0, 0 }		   // ユーザー定義
};

static	void	get_prn_(				// プリンター情報取得
WCHAR		*nam,					  	// プリンター名
int			siz )						// 格納領域サイズ
{
	WCHAR			buf[120];
	WCHAR			*ptr;

	::GetProfileString( _T("windows"), _T("device"), _T(",,,"), buf, sizeof(buf) );
	ptr = buf;
	while ( ( *nam++ = *ptr++ ) != ',' && 0 < siz-- );
	*(nam-1) = '\0';
}

int		Ppr_Siz(						// 用紙サイズ生成
int			*wid,						// 幅（ピクセル）格納領域
int			*hgt,						// 高さ（ピクセル）格納領域
Ppr_def		*ppr,						// 指定用紙仕様
Mtx_def		*mtx )						// 座標変換マトリックス係数
{
	int					pcn;					// 用紙サイズ登録数
	int					suc = false;
	WCHAR				pnm[MAX_PNM];			// プリンター名
	SIZEL				siz;					// 用紙幅、高さ
	Dpn					bfr, aft;				// 座標変換前、後

	if ( ppr->wid > 0 && ppr->hgt > 0 ){
		suc = true;
		bfr.x = ppr->wid;
		bfr.y = ppr->hgt;
	}
	else if( ppr->pnm && ppr->pnm[0] ){
		get_prn_( (WCHAR *)pnm, sizeof(pnm) );
		if ( Get_Ppr_Siz( &siz, pnm, ppr->pnm ) ){
			suc = true;
			if ( ppr->orn == DMORIENT_LANDSCAPE ){
				bfr.x = siz.cy / 100;
				bfr.y = siz.cx / 100;
			}
			else{
				bfr.x = siz.cx / 100;
				bfr.y = siz.cy / 100;
			}
		}
	}
	else{
		pcn = memcnt(Ppr);
		while( --pcn ){
			if ( ppr->siz ==  Ppr[pcn].nam ){
				if ( ppr->orn == DMORIENT_LANDSCAPE ){
					bfr.x = Ppr[pcn].hgt;
					bfr.y = Ppr[pcn].wid;
				}
				else{
					bfr.x = Ppr[pcn].wid;
					bfr.y = Ppr[pcn].hgt;
				}
				suc = true;
				break;
			}
		}
		if ( !suc ){
			bfr.x = 0;
			bfr.y = 0;
		}
	}
	if ( suc ){
		Dpn_Cnv( &aft, &bfr, 1, mtx );		// 変換座標取得
		*wid = (int)( aft.x + 0.5 );
		*hgt = (int)( aft.y + 0.5 );
	}
	return( suc );
}
