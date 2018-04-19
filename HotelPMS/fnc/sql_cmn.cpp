/*
	ホテルプロパティ管理システム
	検索ウィンドウ用データベースアクセス関数
*/

#include <afx.h>
#include <afxdb.h>
#include "..\..\Common\bsc_def.h"
#include "..\..\Common\sys_def.h"
#include "..\..\Common\dbs_mng.h"
#include "..\..\Common\cmn_fnc.h"
#include "..\..\Common\sys_log.h"

#include "..\..\pms_def.h"

#define	is_esc(x)	((x)==_T('%') || (x)==_T('_') || (x)==_T('[') )
#define	is_qrt(x)	((x)==_T('\'') )

static	WCHAR	*add_esc(			// エスケープ文字の追加
WCHAR		*dst,					// 格納領域
WCHAR		*src )					// 元文字列
{
	WCHAR		*ptr = dst;

	while( *src ){
		if ( is_qrt(*src) )	*ptr++ = _T('\'');
		*ptr++ = *src++;
	}
	*ptr = 0;
	return( dst );
}

static	void	sql_log(
int			mdn,			// モジュール番号
int			knd,			// ログ種別
SYS_MNG		*mng,			// システム管理構造
int			usr,			// ユーザー定義データ
WCHAR		*sql )			// ログフォーマット
{
	WCHAR		*buf, *src, *dst;

	if ( buf = (WCHAR *)malloc(wsizeof(wcslen(sql)*2+10))){
		src = sql;
		dst = buf;
		while( *src ){
			if ( *src++==_T('%') )	*dst++ = _T('%');
			*dst++ = *(src-1);
		}
		*dst = 0;
//		Sys_Log(TDN_MIN, LOG_CMN ,mng, CMN_CFL|CMN_INF, buf );
		free( buf );
	}
}

WCHAR	*SQL_Edt_Wst(				// wide char string edit ( escape sequence addition )
WCHAR			*dst,				// pointer of destination buffer
int				siz,				// size of destination buffer
WCHAR			*src )				// souurce string
{
	WCHAR		*ptr = dst;

	while( *src && siz-- ){
		if ( is_qrt(*src) )	*ptr++ = _T('\'');
		*ptr++ = *src++;
	}
	*ptr = 0;

	return( dst );
}

int		SQL_Get_Lst(				// 各種一覧データ取込実行
Csr				rot,				// 格納領域
WCHAR			*sql,				// ＳＱＬ文字列
Dbs_fld			*fld,				// フィールド定義
int				fct,				// フィールド定義数
void			*tmp,				// 一時データ格納領域
int				siz,				// 一時データ格納領域サイズ
WCHAR			**erm,				// エラーメッセージ格納領域
CDatabase		*dbc )				// データベース接続オブジェクト
{
	CRecordset	*rec;
	int			chk, rsl, cnt=0;

	Del_All(rot);

	rec = Dbs_Qry( dbc, sql, erm );

	if ( chk = !!rec ){
		if ( *erm )		free( *erm );
		*erm = NULL;
		while( ( rsl = Dbs_Get( fld, erm, fct, rec ) )==QRY_NML ){
			if ( !Add_Lnk( rot, tmp, siz ) ){
				chk = FALSE;
				break;
			}
			cnt++;
		}
		chk = rsl==QRY_END;
		Dbs_Cls(rec);
	}
	return( chk );
}

int		SQL_Get_MLs(				// 各種一覧データ取込実行
Csr				rot,				// 格納領域
WCHAR			*sql,				// ＳＱＬ文字列
Mlt_dbf			*mdf,				// フィールド定義
int				mfc,				// フィールド定義数
void			*tmp,				// 一時データ格納領域
int				siz,				// 一時データ格納領域サイズ
WCHAR			**erm,				// エラーメッセージ格納領域
CDatabase		*dbc )				// データベース接続オブジェクト
{
	CRecordset	*rec;
	int			chk, cnt=0;

	Del_All(rot);

	rec = Dbs_Qry( dbc, sql, erm );

	if ( chk = !!rec ){
		if ( *erm )		free( *erm );
		*erm = NULL;
		while( Dbs_MGt( mdf, erm, mfc, rec )==QRY_NML ){
			if ( !Add_Lnk( rot, tmp, siz ) ){
				chk = FALSE;
				break;
			}
			cnt++;
		}
		Dbs_Cls(rec);
	}
	return( chk );
}


void	SQL_Dsc_Cnd(	// Making a condition of description
WCHAR		*dst,		// storing area pointer
int			siz,		// storing area pointer size
DSC_RPT		*key,		// key data
int			ntv,		// Only native ( business_date is in alid_sale_period ) 
PMS_MNG		*mng )		// Hotel Property Management System
{
	int			pos;
	WCHAR		buf[40];

	pos = swprintf_s( dst, siz, _T("AND (branch_code IS NULL ") );
	if ( !Nul_Fld(key->brc,FLD_WST) ){
		pos += swprintf_s( dst+pos, siz-pos, _T("OR branch_code='%s' "), SQL_Edt_Wst(buf,memcnt(buf),DBS_WST_PTR(key->brc)) );
	}
	pos += swprintf_s( dst+pos, siz-pos, _T(") ") );
	pos += swprintf_s( dst+pos, siz-pos, _T("AND (facility_code IS NULL ") );
	if ( !Nul_Fld(key->fcc,FLD_WST) ){
		pos += swprintf_s( dst+pos, siz-pos, _T("OR facility_code='%s' "), SQL_Edt_Wst(buf,memcnt(buf),DBS_WST_PTR(key->fcc)) );
	}
	pos += swprintf_s( dst+pos, siz-pos, _T(") ") );

	if ( ntv ){
		pos += swprintf_s( dst+pos, siz-pos,
			_T("AND (valid_sale_period_start IS NULL OR valid_sale_period_start<='%s') "), Edt_Dte(buf, memcnt(buf), mng->bdt) );
		pos += swprintf_s( dst+pos, siz-pos,
			_T("AND (valid_sale_period_end IS NULL OR valid_sale_period_end>='%s' ) "), buf );
	}
	pos += swprintf_s( dst+pos, siz-pos, _T("AND sale_enable!=0 AND deleted_date IS NULL ") );

}

/*
WCHAR	*SQL_Edt_Dtm(				// editing date time for query string
WCHAR		*dst,					// Storing area
int			siz,					// Storing area size
Day			*dte )					// source datetime
{
	swprintf_s( dst, siz, _T("%4d-%02d-%02d %2d:%02d:%02d"), dte->yer, dte->mth, dte->day, dte->hor, dte->min, dte->sec );

	return( dst );
}
*/

static	WCHAR	*str_val(		// Making ansi string value 
char			*src )			// Source pointer of wide string
{
	char		*dst, *ptr;
	int			siz;

	siz = !!src ? (int)strlen( src ): 5;
	if ( !!src ){
		if ( dst = ptr = (char *)malloc( siz*2+8 ) ){
			*ptr++=_T('\'');
			while( *src ){
				if ( is_qrt(*src) )	*ptr++ = _T('\'');
				*ptr++ = *src++;
			}
			*ptr++ = _T('\'');
			*ptr = 0;
		}
	}
	else{
		if ( dst = (char *)malloc( siz ) ){
			strcpy_s( dst, siz, "NULL" );
		}
	}

	return( (WCHAR *)dst );
}



static	WCHAR	*wst_val(		// Making wide string value 
WCHAR			*src )			// Source pointer of wide string
{
	WCHAR		*dst, *ptr;
	int			siz;

	siz = !!src ? (int)wcslen( src ): 5;
	if ( !!src ){
		if ( dst = ptr = (WCHAR *)malloc( wsizeof((siz*2+8) ) ) ){
			*ptr++=_T('\'');
			while( *src ){
				if ( is_qrt(*src) )	*ptr++ = _T('\'');
				*ptr++ = *src++;
			}
			*ptr++ = _T('\'');
			*ptr = 0;
		}
	}
	else{
		if ( dst = (WCHAR *)malloc( wsizeof(siz) ) ){
			wcscpy_s( dst, siz, _T("NULL") );
		}
	}

	return( dst );
}

WCHAR		*SQL_Edt_Flv(		// Editting field data value
Dbs_fld			*fld )			// Tarhet field
{
	WCHAR		*dat, dbf[20], tbf[20];
	int			siz;
	Day			*dte;

	switch( fld->typ ){
	case FLD_INT:	siz = 20;
					if ( dat = (WCHAR *)malloc( siz*wsizeof(1) ) ){
						swprintf_s( dat, siz, Nul_Fld(fld->ptr,FLD_INT) ? _T("NULL"):_T("%d"), DBS_INT_VAL(fld->ptr) );
					}
					break;
	case FLD_SGL:
	case FLD_DBL:	siz = 128;
					if ( dat = (WCHAR *)malloc( siz*wsizeof(1) ) ){
						swprintf_s( dat, siz, Nul_Fld(fld->ptr,fld->typ) ? _T("NULL"):_T("%.40f"), DBS_DBL_VAL(fld->ptr) );
					}
					break;
	case FLD_AST:
	case FLD_STR:	dat = str_val( Nul_Fld(fld->ptr,FLD_STR) ? NULL: DBS_STR_PTR(fld->ptr) );
					break;
	case FLD_WST:	dat = wst_val( Nul_Fld(fld->ptr,FLD_WST) ? NULL: DBS_WST_PTR(fld->ptr) );
					break;
	case FLD_ADS:	break;
	case FLD_DAY:
	case FLD_DTM:
	case FLD_TIM:	siz = 40;
					if ( dat = (WCHAR *)malloc( siz * wsizeof(1) ) ){
						if ( Nul_Fld(fld->ptr,fld->typ) ){
							wcscpy_s( dat, siz, _T("NULL") );
						}
						else{
							dte = DBS_DAY_PTR(fld->ptr);
							swprintf_s( dbf, memcnt(dbf), _T("%4d-%02d-%02d"), dte->yer, dte->mth, dte->day );
							if ( fld->typ!=FLD_DAY ){
								swprintf_s( tbf, memcnt(tbf), _T("%02d:%02d:%02d"), dte->hor, dte->min, dte->sec );
							}
							swprintf_s( dat, siz, _T("'%s%s%s'"),
								( fld->typ==FLD_DAY || fld->typ==FLD_DTM ) ? dbf: _T(""),
								( fld->typ==FLD_DAY || fld->typ==FLD_TIM ) ? _T(""): _T(" "),
								( fld->typ==FLD_DTM || fld->typ==FLD_TIM ) ? tbf:_T("") );
						}
					}
					break;
	}
	return( dat );
}

int		SQL_Exc_Ins(			// Make inserting sql and execute to insert
WCHAR			*dst,			// Destination pointer of storing the sql
int				siz,			// Size of storinhg area
WCHAR			*tbn,			// Table name for inserting
WCHAR			**erm,			// Pointer of storing error message
Dbs_fld			*fld,			// Definition of fields
int				flc,			// Number of definition
CDatabase		*dbs )			// ODBC Database object
{
	int			i, pos, chk, id[DBF_SIZ_INT];
	WCHAR		*fvl, *err=_T("memory allocation failure on SQL_Mak_Ins");
	Dbs_fld		*ptr;
	Dbs_fld		idf[1] = { id, _T("new_id"), FLD_INT, FLD_INT_SIZ(id) };
	CRecordset	*rec;

	// Makeing SQL
	pos = swprintf_s( dst, siz, _T("INSERT INTO %s ("), tbn );
	for ( i=0, ptr=fld; i<flc; i++, ptr++ ){
		pos += swprintf_s( dst+pos, siz-pos, _T("%s%s"), !i ? _T(""):_T(","), ptr->fln );
	}
	pos += swprintf_s( dst+pos, siz-pos, _T(") VALUES( ") );
	for ( i=0, ptr=fld; i<flc; i++, ptr++ ){
		if ( chk = !!( fvl = SQL_Edt_Flv( ptr ) ) ){
			pos += swprintf_s( dst+pos, siz-pos, _T("%s%s"), !i ? _T(""):_T(","), fvl );
			free( fvl );
		}
		else{
			if ( !!erm && !!( *erm = (WCHAR *)malloc( ((siz=(int)wcslen(err))+1) * wsizeof(1) ) ) ){
				wcscpy_s( *erm, siz, err );
			}
			break;
		}
	}
	pos += swprintf_s( dst+pos, siz-pos, _T(")"), tbn );
	
	// Execute to insert
	if ( chk && !!( chk = Dbs_Upd( dbs, dst, erm )==QRY_NML ) ){
		//swprintf_s( dst, siz, _T("SELECT last_insert_id() as new_id FROM %s GROUP BY last_insert_id()"), tbn );
		//swprintf_s( dst, siz, _T("SELECT last_insert_id() as new_id") );
		swprintf_s( dst, siz, _T("SELECT MAX(id) as new_id from %s"), tbn );
		rec = Dbs_Qry( dbs, dst, erm );

		if ( chk = !!rec ){
			if ( *erm )		free( *erm );
			*erm = NULL;
			chk = Dbs_Get( idf, erm, 1, rec )==QRY_NML;
			Dbs_Cls(rec);
		}
	}

	return( chk ? DBS_INT_VAL(id): 0 );
}


int		SQL_Exc_Upd(			// Make update sql and execute to update
WCHAR			*dst,			// Destination pointer of storing the sql
int				siz,			// Size of storinhg area
WCHAR			*tbn,			// Table name for inserting
WCHAR			**erm,			// Pointer of storing error message
Dbs_fld			*fld,			// Definition of fields
int				flc,			// Number of definition
CDatabase		*dbs )			// ODBC Database object
{
	int			i, pos, chk, id;
	WCHAR		*fvl, *cma=_T("");
	WCHAR		*mer=_T("memory allocation failure on SQL_Mak_Ins"), *ier = _T("id field not found on update function");
	Dbs_fld		*ptr, *idf=NULL;

	// Makeing SQL
	pos = swprintf_s( dst, siz, _T("UPDATE %s SET "), tbn );
	for ( i=0, ptr=fld; i<flc; i++, ptr++ ){
		if ( !!wcscmp( ptr->fln, _T("id") ) ){
			if ( fvl = SQL_Edt_Flv(ptr) ){
				pos += swprintf_s( dst+pos, siz-pos, _T("%s%s=%s"), cma, ptr->fln, fvl );
				cma = _T(",");
				free( fvl );
			}
		}
		else{
			idf = ptr;
			id = DBS_INT_VAL(idf->ptr);
		}
	}
	if ( chk = !!idf ){
		if ( chk = !!( fvl = SQL_Edt_Flv(idf) ) ){
			pos += swprintf_s( dst+pos, siz-pos, _T(" WHERE id=%s"), fvl );
			free( fvl );
		}
	}
	if ( !fvl && !!erm && !!( *erm = (WCHAR *)malloc( wsizeof( siz=(int)wcslen(mer)+1 ) ) ) ){
		wcscpy_s( *erm, siz, mer );
	}
	else if ( !idf && !!erm && !!( *erm = (WCHAR *)malloc( wsizeof( siz=(int)wcslen(ier)+1 )  ) ) ){
		wcscpy_s( *erm, siz, ier );
	}

	if ( chk )	chk = Dbs_Upd( dbs, dst, erm )==QRY_NML;

	return( chk ? id: 0 );
}

