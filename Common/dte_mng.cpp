/*
				“ú•t‘€ìŠÖ”
*/
#include <afx.h>
#include <windows.h>
#include <math.h>
#include "bsc_def.h"
#include "lnk_mng.h"
#include "dte_mng.h"

int		Get_Dte(					// ƒVƒXƒeƒ€“ú•tæ“¾
Day			*dst )					// Ši”[—Ìˆæ
{
	SYSTEMTIME		sys[1];

	GetLocalTime( sys );

	dst->yer = (short)sys->wYear;
	dst->mth = (short)sys->wMonth;
	dst->day = (short)sys->wDay;
	dst->hor = (short)sys->wHour;
	dst->min = (short)sys->wMinute;
	dst->sec = (short)sys->wSecond;

	return( sys->wDayOfWeek );
}

short		Get_Mxd(		// Å‘å“ú•tæ“¾
Day			*dst )			// ”í‰ÁZ“ú•t
{
	static short	mxd[] = {31,28,31,30,31,30,31,31,30,31,30,31};

	mxd[1] = is_leap(dst->yer) ? (short)29: (short)28;
	return( mxd[dst->mth-1] );
}

void	Add_Dte(						// “ú”‰ÁZ
Day		*dst,							// ‘ÎÛƒf[ƒ^
int		dys )							// ‰ÁZ“ú”
{
	if ( dys >= 0 ){
		while( dys-- ){
			if ( ++dst->day > Get_Mxd( dst ) ){
				if ( ++dst->mth > 12 ){
					dst->mth=1;
					dst->yer++;
				}
				dst->day = 1;
			}
		}
	}
	else{
		while( dys++ ){
			if ( --dst->day < 1 ){
				if ( --dst->mth < 1 ){
					dst->mth=12;
					dst->yer--;
				}
				dst->day = Get_Mxd( dst );
			}
		}
	}
}

int		Cmp_Dte(						// “ú•t”äŠr
Day		*src,							// ”äŠr“ú•t
Day		*dst )							// ”í”äŠr“ú•t
{
	int			sdt, ddt;

	sdt = src->yer * 10000 + src->mth * 100 + src->day;
	ddt = dst->yer * 10000 + dst->mth * 100 + dst->day;

	return( sdt - ddt );
}

int		Cmp_Dtm(						// “ú•t”äŠr
Day		*src,							// ”äŠr“ú•t
Day		*dst )							// ”í”äŠr“ú•t
{
	int			sdt, ddt;

	sdt = src->yer * 10000 + src->mth * 100 + src->day;
	ddt = dst->yer * 10000 + dst->mth * 100 + dst->day;
	if ( sdt == ddt ){
		sdt = src->hor * 10000 + src->min * 100 + src->sec;
		ddt = dst->hor * 10000 + dst->min * 100 + dst->sec;
	}
	return( sdt - ddt );
}

int		Cmp_Tim(						// ”äŠr
Day		*src,							// ”äŠr“ú•t
Day		*dst )							// ”í”äŠr“ú•t
{
	int			sdt, ddt;

	sdt = src->hor * 10000 + src->min * 100 + src->sec;
	ddt = dst->hor * 10000 + dst->min * 100 + dst->sec;

	return( sdt - ddt );
}

int			Chk_Dte(					// “ú•t³“–«ŒŸ¸
Day			*dte )						// ŒŸ¸‘ÎÛ“ú•t
{
	int			chk;

	chk = dte->mth >= 1 && dte->mth <= 12;
	if ( chk )	chk = dte->day >= 1 && dte->day <= Get_Mxd( dte );
	return( chk );
}

int		Str_Dte(						// •¶š—ñ¨“ú•t•ÏŠ·
Day			*dst,						// Ši”[—Ìˆæ
WCHAR		*str )						// “ú•t•¶š—ñ
{
	Day		dte[1];
	int		chk;

	memset( dte, 0, sizeof(dte) );
	dte->yer = (short)_wtoi(str);
	dte->mth = (short)_wtoi(str+5);
	dte->day = (short)_wtoi(str+8);
	if ( ( chk = Chk_Dte( dte ) ) == TRUE )	*dst = *dte;
	return( chk );
}

int		Chk_Tim(
Day			*dte )						// ŒŸ¸‘ÎÛ“ú•t
{
	int			chk;

	chk  = dte->hor>=0 && dte->hor<=23;
	chk &= dte->min>=0 && dte->min<=59;
	chk &= dte->sec>=0 && dte->sec<=59;

	return( chk );
}

int		Str_Tim(						// •¶š—ñ¨•ÏŠ·
Day			*dst,						// Ši”[—Ìˆæ
WCHAR		*str )						// “ú•t•¶š—ñ
{
	Day		dte[1];
	int		chk, dct=0;
	WCHAR	*ptr=str;

	while( *ptr && ( ptr = wcschr( ptr, _T(':') ) ) ){
		dct++;
		ptr = ptr+1;
	}
	if ( chk = dct==2 ){
		memset( dte, 0, sizeof(dte) );
		dte->hor = (short)_wtoi(str);
		ptr = wcschr( str, _T(':') ) + 1;
		dte->min = (short)_wtoi(ptr);
		ptr = wcschr( ptr, _T(':') ) +1;
		dte->sec = (short)_wtoi(ptr);
		if ( ( chk = Chk_Tim( dte ) ) == TRUE )	*dst = *dte;
	}
	return( chk );
}

int		Clc_Dys(						// “ú”(”‘”)ŒvZ
Day		*sta,							// ŠJn“ú•t
Day		*end )							// I—¹“ú•t
{
	Day		std[1], edd[1];
	int		dys=0;

	if ( Cmp_Dte( sta, end ) > 0 )	*std = *end, *edd = *sta;
	else							*std = *sta, *edd = *end;

	while ( Cmp_Dte( std, end ) < 0 ){
		dys++;
		if ( ++std->day > Get_Mxd( std ) ){
			if ( ++std->mth > 12 )	std->mth=1,	std->yer++;
			std->day = 1;
		}
	}
	return( dys );
}

double	Clc_Tim(					// Œo‰ßŠÔŒvZ
Day			*sta,					// ŠJn
Day			*end )					// I—¹
{
	double	src, dst;

	src = (double)sta->hor * 60.0 + (double)sta->min;
	dst = (double)end->hor * 60.0 + (double)end->min;
	return( (dst-src) / 60.0 );
}

int		Clc_Age(						// ”N—îŒvZ
Day			*bth )						// ¶”NŒ“ú
{
	Day			tdy[1], btp[1];
	int			age;

	Get_Dte( tdy );
	*btp = *bth;
	Add_Dte( btp, -1 );
	if ( bth->mth==3 && bth->day==1 ){
		btp->day = (short)(is_leap( tdy->yer ) ? 29: 28);
	}
	if ( btp->mth > tdy->mth ){
		age = tdy->yer - btp->yer - 1;
	}
	else{
		if ( btp->day <= tdy->day )	age = tdy->yer - btp->yer;
		else						age = tdy->yer - btp->yer - 1;
	}
	return( age );
}

void	Clc_Mth(					// Œ“xZo
int			*yer,					// ”N“xŠi”[—Ìˆæ
int			*mth,					// Œ“xŠi”[—Ìˆæ
Day			*dte,					// w’è“ú•t
int			cdy )					// ’÷‚ß“ú
{
	if ( cdy==-1 || dte->day <= cdy ){
		*yer = dte->yer;
		*mth = dte->mth;
	}
	else{
		if ( dte->day > cdy ){
			*mth = dte->mth + 1;
			if ( *mth == 13 )	*mth = 1, *yer = dte->yer + 1;
			else				*yer = dte->yer;
		}
	}
}

int		Get_Dow(				// —j“úŒvZ
Day			*dte )				// ‘ÎÛ“ú•t
{
	int			yer, mth, day, six;

    if ( dte->mth == 1 || dte->mth == 2 )	yer = dte->yer - 1;
	else									yer = dte->yer;
    if ( dte->mth == 1 || dte->mth == 2 )	mth = 13;
	else							        mth = dte->mth;
    if ( dte->mth == 2)						day = dte->day + 31;
	else									day = dte->day;

	six = (int)(floor((double)yer*1.25)) - (int)(floor((double)yer/100.0))
		+ (int)(floor((double)yer/400.0)) + (int)(floor((double)mth*2.6+1.6)) + day;

    return( six % 7 );
}

WCHAR	*Edt_Dte(				// “ú•t•ÒW
WCHAR		*dst,				// Ši”[—Ìˆæ
int			siz,				// Ši”[—ÌˆæƒTƒCƒYi•¶š”j
 Day		*dte )				// •ÒW‘ÎÛ“ú•t
{
	swprintf_s( dst, siz, _T("%4d/%02d/%02d"), dte->yer, dte->mth, dte->day );
	return( dst );
}

WCHAR	*Edt_Tim(				// •ÒW
WCHAR		*dst,				// Ši”[—Ìˆæ
int			siz,				// Ši”[—ÌˆæƒTƒCƒYi•¶š”j
Day			*dte )				// •ÒW‘ÎÛ
{
	swprintf_s( dst, siz, _T("%d:%02d:%02d"), dte->hor, dte->min, dte->sec );
	return( dst );
}

WCHAR	*Edt_Dtm(				// •ÒW
WCHAR		*dst,				// Ši”[—Ìˆæ
int			siz,				// Ši”[—ÌˆæƒTƒCƒYi•¶š”j
Day			*dte )				// •ÒW‘ÎÛ
{
	swprintf_s( dst, siz, _T("%4d/%02d/%02d %d:%02d:%02d"), dte->yer, dte->mth, dte->day, dte->hor, dte->min, dte->sec );
	return( dst );
}
