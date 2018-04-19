/*
		初期化ファイルアクセス関数
*/
#include <afx.h>
#include <stdio.h>
#include <windows.h>
#include "ini_inf.h"
#include "lnk_mng.h"
#include "get_tkn.h"
#include "cmn_fnc.h"

static	int		get_val_(			// 指定情報の取り込み
void	*dst,						// 格納領域
WCHAR	*sct,						// セクション名文字列
WCHAR	*key,						// キー文字列
int		typ,						// キータイプ
int		mcl,						// 複数カラム指定あり
int		mlt,						// 複数行指定あり
WCHAR	*ifn )						// 初期化ファイル名
{
	WCHAR	*tkn[MAX_TKN], sbf[256], **ptr;
	int		i, tkc, ibf, ret = VAL_SUC;

	switch( typ ){
	case	ITP_INT:
			if ( mcl ){
				ibf = GetPrivateProfileString(sct, key, _T("NONE"), sbf, sizeof(sbf), ifn);
				if ( !wcscmp( sbf, _T("NONE") ) )	ret = VAL_NON;
			}
			else{
				ibf = GetPrivateProfileInt(sct, key, 0, ifn);
				if ( ibf == 0 )	ret = VAL_NON;
			}
			break;
	case	ITP_STR:
			ibf = GetPrivateProfileString(sct, key, _T("NONE"), sbf, sizeof(sbf), ifn);
			if ( !wcscmp( sbf, _T("NONE") ) )	ret = VAL_NON;
			break;
	default:
			ret = VAL_NON;
	}

	if ( ret == VAL_SUC ){
/*
		if ( mlt ){
			switch( typ ){
			case	ITP_INT:
					if ( Add_Lnk( (Csr)dst, (char *)&ibf, sizeof(ibf) )==NULL ){
						ret = VAL_MEM;
					}
					break;
			case	ITP_STR:
					if ( Add_Lnk( (Csr)dst, sbf, strlen(sbf)+1 ) == NULL ){
						ret = VAL_MEM;
					}
					break;
			}
		}
		else{
*/
			switch( typ ){
			case	ITP_INT:
					if ( mcl ){
						tkc = Get_Tkn( tkn, sbf, _T(" \t,"), _T("") );
						if ( tkc ){
							for ( i=0; i<tkc; i++ ){
								ibf = _wtoi( tkn[i] );
								if ( !Add_Lnk( (Csr)dst, &ibf, sizeof(ibf) ) ){
									ret = VAL_MEM;
								}
							}
						}
					}
					else if ( mlt ){
						if ( !Add_Lnk( (Csr)dst, &ibf, sizeof(ibf) ) ){
							ret = VAL_MEM;
						}
					}
					else{
						*((int *)dst) = ibf;
					}
					break;
			case	ITP_STR:
					if ( mcl ){
						tkc = Get_Tkn( tkn, sbf, _T(" \t,"), _T("") );
						if ( tkc ){
							for ( i=0; i<tkc; i++ ){
								if ( !Add_Lnk( (Csr)dst,
										tkn[i], (int)wsizeof(wcslen(tkn[i])+1)) ){
									ret = VAL_MEM;
								}
							}
						}
					}
					else if ( mlt ){
						if ( !Add_Lnk( (Csr)dst, sbf, (int)wsizeof(wcslen(sbf)+1) ) ){
							ret = VAL_MEM;
						}
					}
					else{
						ptr = (WCHAR **)dst;
						*ptr = (WCHAR *)malloc( wsizeof(wcslen(sbf)+1) );
						if ( *ptr != NULL ){
							wcscpy_s( *ptr, wcslen(sbf)+wsizeof(1), sbf );
						}
						else{
							ret = VAL_MEM;
						}
					}
					break;
			}
/*
		}
*/
	}
	return( ret );
}

static	int		set_val_(			// 指定情報の書き込み
void	*src,						// データ領域
WCHAR	*sct,						// セクション名文字列
WCHAR	*key,						// キー文字列
int		typ,						// キータイプ
WCHAR	*ifn )						// 初期化ファイル名
{
	WCHAR	ibf[30], *ptr = *((WCHAR **)src);

	if ( typ == ITP_INT ){
		swprintf_s( ibf, memcnt(ibf), _T("%d"), *((int *)src) );
		ptr = (WCHAR *)&ibf;
	}
	return( WritePrivateProfileString( sct, key, ptr, ifn ) );
}

int		Get_Ini(					// プログラム情報の取り込み
Ini_inf	*dst,						// 格納領域
int		cnt,						// キー個数
WCHAR	*ifn )						// 初期化ファイル名
{
	int		i, sub, siz;
	WCHAR	*buf, *sct;
	int		chk = TRUE, mlt;
	Ini_inf	*sav = dst;
	Csr		rot;

	for ( i=0; i<cnt && chk==TRUE; i++ ){
		if ( dst->sct != NULL )		sct = dst->sct;
		if ( dst->mlt ){			// 複数データあり
			siz = ((int)wcslen(dst->key) + 10);
			buf = (WCHAR *)malloc( wsizeof(siz) );
			if ( buf != NULL ){
				mlt = VAL_SUC;
				sub=1;
				while( mlt == VAL_SUC ){
					swprintf_s( buf, siz, _T("%s%d"), dst->key, sub );
					if ( dst->mcl ){
						if ( !( rot = Add_Lnk( (Csr)dst->val, NULL, sizeof(Lnk) ) ) ){
							chk = FALSE;
							break;
						}
					}
					mlt = get_val_( dst->mcl ? Rec_Of(Lnk,rot): dst->val, sct, buf,
											dst->typ, dst->mcl, dst->mlt, ifn );
					sub++;
					if ( mlt!=VAL_SUC && mlt!=VAL_NON && dst->mcl ){
						Del_Lnk( (Csr)dst->val, rot );
					}
				}
				free( buf );
			}
			else{
				chk = FALSE;
			}
			if ( mlt == VAL_MEM ){
				chk = FALSE;
			}
		}
		else{						// 単一データのとき
			mlt = get_val_( dst->val, sct, dst->key, dst->typ,
													dst->mcl, dst->mlt, ifn );
			if ( mlt == VAL_MEM ){
				chk = FALSE;
			}
		}
		dst++;
	}
	if ( chk == FALSE ){
		Cst_Ini( sav, cnt );
	}
	return( chk );
}

static	int		dgt_cnt_(			// 整数桁数算出
int		src )
{
	int		dgt = 1;

	while( src ){
		src /= 10;
		if ( src )	dgt++;
	}
	return( dgt );
}

int		Set_Ini(					// プログラム情報の書き込み
Ini_inf	*src,						// 格納領域
int		cnt,						// キー個数
WCHAR	*ifn )						// 初期化ファイル名
{
	int		i, sub, siz, bsz;
	WCHAR	*buf, *sct, tmp[30], *ptr;
	int		chk = TRUE;
	Csr		csr, cse;

	for ( i=0; i<cnt && chk==TRUE; i++ ){
		if ( src->sct != NULL )		sct = src->sct;
		if ( src->mlt ){			// 複数行データあり
			bsz = (int)wcslen(src->key) + 10;
			buf = (WCHAR *)malloc( wsizeof(bsz) );
			if ( buf != NULL ){
				sub=1;
				for ( csr=NULL; (csr=Nxt_Lnk((Csr)(src->val), csr))!=NULL; ){
					swprintf_s( buf, bsz, _T("%s%d"), src->key, sub );
					if ( src->mcl ){
						for ( cse=NULL, ptr=NULL; cse=Nxt_Lnk( Rec_Of(Lnk, csr), cse ); ){
							if ( src->typ==ITP_INT ){
								swprintf_s( tmp, memcnt(tmp), _T("%s%d"), !!ptr ? _T(","): _T(""), *Rec_Of(int, cse) );
								chk = !!( ptr = Add_Str( ptr, tmp ) );
							}
							if ( src->typ==ITP_STR ){
								if ( !!ptr )	ptr = Add_Str( ptr, _T(",") );
								chk = !!( ptr = Add_Str( ptr, Rec_Of(WCHAR, cse) ) );
							}
						}
						if ( chk )	chk = set_val_( &ptr, sct, buf, ITP_STR, ifn );
						if ( ptr )	free( ptr );
					}
					else{
						if ( src->typ==ITP_STR ){
							ptr = (WCHAR *)(csr+1);
							chk = set_val_( &ptr, sct, buf, src->typ, ifn );
						}
						if ( src->typ==ITP_INT ){
							chk = set_val_( csr+1, sct, buf, src->typ, ifn );
						}
					}
					if ( !chk )		break;
					sub++;
				}
				free( buf );
			}
			else{
				chk = FALSE;
			}
		}
		else{
			if ( src->mcl ){			// 複数カラムデータあり
				for (csr=NULL,siz=0;(csr=Nxt_Lnk((Csr)(src->val),csr))!=NULL;){
					if ( src->typ == ITP_INT )
						siz += (int)dgt_cnt_( *((int *)(csr+1)) ) + 3;
					else
						siz += (int)strlen((char *)(csr+1)) + 3;
				}
				if ( !!siz ){
					buf = (WCHAR *)malloc( wsizeof(siz) );
					if ( buf != NULL ){
						sub = 0;
						*buf = 0;
						for ( csr=NULL; (csr=Nxt_Lnk((Csr)(src->val),csr))!=NULL; ){
							if ( src->typ == ITP_INT ){
								swprintf_s(tmp,siz,_T("%s%d"),sub ? _T(","):_T(""),*((int *)(csr+1)));
								wcscat_s( buf, siz, tmp );
							}
							else{
								if ( sub )	wcscat_s( buf, siz, _T(",") );
								wcscat_s( buf, siz, (WCHAR *)(csr+1) );
							}
							++sub;
						}
						chk = set_val_( &buf, sct, src->key, ITP_STR, ifn );
						free( buf );
					}
					else{
						chk = FALSE;
					}
				}
				else{
					csr = (Csr)(src->val);
				}
			}
			else{						// 単一データのとき
				chk = set_val_( src->val, sct, src->key, src->typ, ifn );
			}
		}
		src++;
	}
	return( chk );
}

void	Cst_Ini(					// プログラム情報の破棄
Ini_inf	*dst,						// 格納領域
int		cnt )						// キー個数
{
	int		i;
	char	**ptr;
	Csr		csr=NULL;

	for ( i=0; i<cnt; i++, dst++ ){
		if ( dst->mlt ){				// 複数行データあり
			if ( dst->mcl ){
				while( csr=Nxt_Lnk( (Csr)dst->val, csr ) ){
					Del_All( Rec_Of(Lnk, csr) );
				}
			}
			Del_All( (Csr)dst->val );
		}
		else if ( dst->mcl ){			// 複数カラム有り
			Del_All( (Csr)dst->val );
		}
		else{							// 単一データのとき
			if ( dst->typ == ITP_STR ){
				ptr = (char **)dst->val;
				if ( *ptr != NULL ){
					free( *ptr );
					*ptr = NULL;
				}
			}
		}
	}
}

