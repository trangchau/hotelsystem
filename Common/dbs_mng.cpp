/*
		データベースアクセス管理
*/

#include <afxdb.h>
#include "bsc_def.h"
#include "cmn_fnc.h"
#include "dbs_mng.h"

void		*Fld_Ptr(			// フィールドポインタ取得
void		*ptr,				// フィールドポインター
int			typ )				// フィールドタイプ
{
	int		nul = TRUE;

	switch( typ ){
	case	FLD_WSI:
	case	FLD_INT:	ptr = ((int *)ptr)+1;		break;	// 整数フィールド
	case	FLD_SGL:	ptr = ((float *)ptr)+1;		break;	// 単精度実数フィールド
	case	FLD_DBL:	ptr = ((double *)ptr)+1;	break;	// 倍精度実数フィールド
	case	FLD_STR:	ptr = ((char *)ptr)+1;		break;	// 文字列フィールド
	case	FLD_WST:	ptr = ((WCHAR *)ptr)+1;		break;	// WideChar文字列フィールド
	case	FLD_AST:	ptr = ((char *)ptr)+1;		break;	// ANSI文字列フィールド
	case	FLD_ADS:	ptr = ((char *)ptr)+1;		break;	// 文字列フィールド(strcat)
	case	FLD_DAY:										// 日付フィールド
	case	FLD_DTM:										// 日付・時刻型フィールド
	case	FLD_TIM:	ptr = ((Day *)ptr)+1;		break;	// 時刻型フィールド
	}

	return( ptr );
}

int			Nul_Fld(			// NULL検定
void		*ptr,				// フィールドポインター
int			typ )				// フィールドタイプ
{
	int		nul = TRUE;

	switch( typ ){
	case	FLD_WSI:
	case	FLD_INT:	nul = *((int *)(ptr)) == 0;		break;	// 整数フィールド
	case	FLD_SGL:	nul = *((float *)(ptr)) == 0;	break;	// 単精度実数フィールド
	case	FLD_DBL:	nul = *((double *)(ptr)) == 0;	break;	// 倍精度実数フィールド
	case	FLD_STR:	nul = *((char *)(ptr)) == 0;	break;	// 文字列フィールド
	case	FLD_WST:	nul = *((WCHAR *)(ptr)) == 0;	break;	// WideChar文字列フィールド
	case	FLD_AST:	nul = *((char *)(ptr)) == 0;	break;	// ANSI文字列フィールド
	case	FLD_ADS:	nul = *((char *)(ptr)) == 0;	break;	// 文字列フィールド(strcat)
	case	FLD_DAY:											// 日付フィールド
	case	FLD_DTM:											// 日付・時刻型フィールド
	case	FLD_TIM:	nul = ((Day *)ptr)->yer == 0;	break;	// 時刻型フィールド
	}

	return( nul );
}

void		Set_Nul(			// NULL設定
void		*ptr,				// フィールドポインター
int			typ,				// フィールドタイプ
int			nul )				// NULL or NOT
{	
	switch( typ ){
	case	FLD_WSI:
	case	FLD_INT:	*((int *)(ptr)) = !nul;						break;	// 整数フィールド
	case	FLD_SGL:	*((float *)(ptr)) = !nul;					break;	// 単精度実数フィールド
	case	FLD_DBL:	*((double *)(ptr)) = !nul;					break;	// 倍精度実数フィールド
	case	FLD_STR:	*((char *)(ptr)) = !nul;					break;	// 文字列フィールド
	case	FLD_WST:	*((WCHAR *)(ptr)) = !nul;					break;	// WideChar文字列フィールド
	case	FLD_AST:	*((char *)(ptr)) = !nul;					break;	// ANSI文字列フィールド
	case	FLD_ADS:	*((char *)(ptr)) = !nul;					break;	// 文字列フィールド(strcat)
	case	FLD_DAY:														// 日付フィールド
	case	FLD_DTM:														// 日付・時刻型フィールド
	case	FLD_TIM:	memset( ptr, nul ? 0: -1, sizeof(Day) );	break;	// 時刻型フィールド
	}
}

static	void	all_nul(		// NULL設定
Dbs_fld		*fld,				// 格納領域
int			fct,				// カラム数
int			nul )				// NULL or NOT
{	
	while( fct-- ){
		Set_Nul( fld->ptr, fld->typ, nul );
		fld++;
	}
}

int		Dbs_Cnc(				// データベースの接続
CDatabase	*dbc,				// データベースクラス
WCHAR		*dsn,				// データセット名称
WCHAR		*dbn,				// データベース名
WCHAR		*usr,				// データベースユーザーＩＤ
WCHAR		*pwd,				// ＤＢパスワード
WCHAR		**erm )				// エラーメッセージ格納場所
{
	WCHAR		cnc[1024];
	int			chk=TRUE;

	swprintf_s( cnc, memcnt(cnc),
		_T("DSN=%s;DBQ=%s;UID=%s;PWD=%s"), dsn, dbn, usr, pwd );
	*erm = NULL;
	
	try{
		chk = !!dbc->OpenEx( cnc, CDatabase::noOdbcDialog );
	}
	catch( CDBException *dbe ){
		*erm = (WCHAR *)malloc( wsizeof(dbe->m_strError.GetLength()+1) );
		if ( *erm )	wcscpy_s( *erm,
						dbe->m_strError.GetLength()+1, dbe->m_strError );
		dbe->Delete();
		chk = FALSE;
	}
	catch( CMemoryException *mee ){
		*erm = (WCHAR *)malloc( wsizeof(256) );
		if ( *erm )	mee->GetErrorMessage( *erm, 256, NULL );
		mee->Delete();
		chk = FALSE;
	}
	return( chk );
}

void	Dbs_Cut(				// データベースの切断
CDatabase	*dbc )				// データベースクラス
{
	dbc->Close();
}

CRecordset	*Dbs_Qry(			// データベース問合わせ実行
CDatabase	*dbc,				// データベースクラス
WCHAR		*sql,				// ＳＱＬ文
WCHAR		**erm )				// エラーメッセージ格納場所
{
	CRecordset		*rec;
	int				err=FALSE;

	*erm = NULL;

	try{
		rec = new CRecordset( dbc );
	}catch( CDBException *dbe ){
		*erm = (WCHAR *)malloc( wsizeof(dbe->m_strError.GetLength()+1) );
		if ( *erm )	wcscpy_s( *erm,
			dbe->m_strError.GetLength()+1, dbe->m_strError.GetString() );
		dbe->Delete();
		err = TRUE;
	}catch( CMemoryException *mee ){
		*erm = (WCHAR *)malloc( wsizeof(256) );
		if ( *erm )	mee->GetErrorMessage( *erm, 256, NULL );
		mee->Delete();
		err = TRUE;
	}
	
	if ( !err ){
		try{
			rec->Open( CRecordset::forwardOnly, sql, CRecordset::readOnly );
		}catch( CDBException *dbe ){
			*erm = (WCHAR *)malloc( wsizeof(dbe->m_strError.GetLength()+1) );
			if ( *erm )	wcscpy_s( *erm, 
							dbe->m_strError.GetLength()+1, dbe->m_strError );
			dbe->Delete();
			err = TRUE;
		}catch( CMemoryException *mee ){
			*erm = (WCHAR *)malloc( wsizeof(256) );
			if ( *erm )	mee->GetErrorMessage( *erm, 256, NULL );
			mee->Delete();
			err = TRUE;
		}
		if ( err ){
			delete	rec;
			rec = NULL;
		}
	}
	return( rec );
}
/*
int		Dbs_Get(				// フィールドデータ取得
Dbs_fld		*fld,				// 格納領域
int			*rct,				// レコード数格納領域
WCHAR		**erm,				// エラーメッセージ格納領域
int			fct,				// カラム数
CRecordset	*rec )				// レコードセットクラス
{
	int				chk;
	Day				*dpt;
	CDBVariant		val;

	all_nul( fld, fct, FALSE );

	if ( rct )	*rct = 0;

	chk = ( rec && !rec->IsEOF() ) ? QRY_NML: QRY_END;
	while( chk==QRY_NML && fct-- ){
		try{
			rec->GetFieldValue( fld->fln, val, DEFAULT_FIELD_TYPE );
		}catch( CDBException *dbe ){
			if ( !!erm ){
				*erm = (WCHAR *)malloc( wsizeof(dbe->m_strError.GetLength()+1) );
				if ( *erm )	wcscpy_s( *erm, 
								dbe->m_strError.GetLength()+1, dbe->m_strError );
			}
			dbe->Delete();
			chk = QRY_DBS;
		}catch( CMemoryException *mem ){
			mem->Delete();
			chk = QRY_MEM;
		}

		if ( chk==QRY_NML ){
			if ( rct )	++(*rct);
			switch ( val.m_dwType ){
			case DBVT_NULL:		Set_Nul( fld->ptr, fld->typ, TRUE );
								break;
			case DBVT_UCHAR:	if ( fld->typ==FLD_INT ){
									*((int *)Fld_Ptr(fld->ptr, fld->typ)) = val.m_chVal;
								}
								break;
			case DBVT_SHORT:	if ( fld->typ==FLD_INT ){
									*((int *)Fld_Ptr(fld->ptr, fld->typ)) = val.m_iVal;
								}
								break;
			case DBVT_LONG:		if ( fld->typ==FLD_INT ){
									*((int *)Fld_Ptr(fld->ptr, fld->typ)) = val.m_lVal;
								}
								break;
			case DBVT_SINGLE:	if ( fld->typ==FLD_SGL ){
									*((float *)Fld_Ptr(fld->ptr, fld->typ)) = val.m_fltVal;
								}
								break;
			case DBVT_DOUBLE:	if ( fld->typ==FLD_DBL ){
									*((float *)Fld_Ptr(fld->ptr, fld->typ)) = val.m_fltVal;
								}
								break;
			case DBVT_DATE:		dpt = (Day *)Fld_Ptr(fld->ptr, fld->typ);
								if ( fld->typ==FLD_DAY || fld->typ==FLD_DTM || fld->typ==FLD_TIM ){
									if ( fld->typ==FLD_DAY || fld->typ==FLD_DTM ){
										dpt->yer = val.m_pdate->year; 
										dpt->mth = val.m_pdate->month; 
										dpt->day = val.m_pdate->day;
									}
									if ( fld->typ==FLD_DTM || fld->typ==FLD_TIM ){
										dpt->hor = val.m_pdate->hour; 
										dpt->min = val.m_pdate->minute; 
										dpt->sec = val.m_pdate->second; 
									}
								}
								break;
			case DBVT_STRING:	if ( fld->typ==FLD_STR ){
									//*((char *)fld->ptr) = 0;	// NULL flag
									strcpy_s((char *)Fld_Ptr(fld->ptr, fld->typ), fld->siz, (char *)val.m_pstring->GetBuffer(fld->siz-1)  );
								}
								break;
			case DBVT_ASTRING:	if ( fld->typ==FLD_AST ){
									//*((char *)fld->ptr) = 0;	// NULL flag
									strcpy_s((char *)Fld_Ptr(fld->ptr, fld->typ), fld->siz, (char *)val.m_pstring->GetBuffer(fld->siz-1)  );
								}
								break;
			case DBVT_WSTRING:	if ( fld->typ==FLD_WST ){
									//*((WCHAR *)fld->ptr) = 0;	// NULL flag
									WStr_NCpy((WCHAR *)Fld_Ptr(fld->ptr, fld->typ),
											val.m_pstring->GetBuffer(0), fld->siz-1  );
									//wcsncpy_s((WCHAR *)Fld_Ptr(fld->ptr, fld->typ), fld->siz,
									//		val.m_pstring->GetBuffer(0), val.m_pstring->GetLength()  );
								}
								break;
			case DBVT_BOOL:
			case DBVT_BINARY:	break;
			}			
			
			++fld;
		}
	}
	if ( chk==QRY_NML )	rec->MoveNext();
	return( chk );
}
*/

static	int	get_fld(		// Getting each field data
WCHAR		**erm,				// エラーメッセージ格納領域
Dbs_fld		*fld,				// Target field
CRecordset	*rec )				// レコードセットクラス
{
	Day				*dpt;
	CDBVariant		val;
	int				chk=QRY_NML;

	try{
		rec->GetFieldValue( fld->fln, val, DEFAULT_FIELD_TYPE );
	}catch( CDBException *dbe ){
		if ( !!erm ){
			*erm = (WCHAR *)malloc( wsizeof(dbe->m_strError.GetLength()+1) );
			if ( *erm )	wcscpy_s( *erm, 
							dbe->m_strError.GetLength()+1, dbe->m_strError );
		}
		dbe->Delete();
		chk = QRY_DBS;
	}catch( CMemoryException *mem ){
		mem->Delete();
		chk = QRY_MEM;
	}

	if ( chk==QRY_NML ){
		switch ( val.m_dwType ){
		case DBVT_NULL:		Set_Nul( fld->ptr, fld->typ, TRUE );
							break;
		case DBVT_UCHAR:	if ( fld->typ==FLD_INT ){
								*((int *)Fld_Ptr(fld->ptr, fld->typ)) = val.m_chVal;
							}
							break;
		case DBVT_SHORT:	if ( fld->typ==FLD_INT ){
								*((int *)Fld_Ptr(fld->ptr, fld->typ)) = val.m_iVal;
							}
							break;
		case DBVT_LONG:		if ( fld->typ==FLD_INT || fld->typ==FLD_WSI ){
								*((int *)Fld_Ptr(fld->ptr, fld->typ)) = val.m_lVal;
							}
							break;
		case DBVT_SINGLE:	if ( fld->typ==FLD_SGL ){
								*((float *)Fld_Ptr(fld->ptr, fld->typ)) = val.m_fltVal;
							}
							break;
		case DBVT_DOUBLE:	if ( fld->typ==FLD_DBL ){
								*((double *)Fld_Ptr(fld->ptr, fld->typ)) = val.m_dblVal;
							}
							break;
		case DBVT_DATE:		dpt = (Day *)Fld_Ptr(fld->ptr, fld->typ);
							if ( fld->typ==FLD_DAY || fld->typ==FLD_DTM || fld->typ==FLD_TIM ){
								if ( fld->typ==FLD_DAY || fld->typ==FLD_DTM ){
									dpt->yer = val.m_pdate->year; 
									dpt->mth = val.m_pdate->month; 
									dpt->day = val.m_pdate->day;
								}
								if ( fld->typ==FLD_DTM || fld->typ==FLD_TIM ){
									dpt->hor = val.m_pdate->hour; 
									dpt->min = val.m_pdate->minute; 
									dpt->sec = val.m_pdate->second; 
								}
							}
							break;
		case DBVT_STRING:	if ( fld->typ==FLD_STR ){
								//*((char *)fld->ptr) = 0;	// NULL flag
								strcpy_s((char *)Fld_Ptr(fld->ptr, fld->typ), fld->siz, (char *)val.m_pstring->GetBuffer(fld->siz-1)  );
							}
							else if ( fld->typ==FLD_STI ){
								//*((int *)Fld_Ptr(fld->ptr, FLD_INT)) = atoi( val.m_pstring->GetBuffer() );
							}
							break;
		case DBVT_ASTRING:	if ( fld->typ==FLD_AST ){
								//*((char *)fld->ptr) = 0;	// NULL flag
								strcpy_s((char *)Fld_Ptr(fld->ptr, fld->typ), fld->siz, (char *)val.m_pstring->GetBuffer(fld->siz-1)  );
							}
							break;
		case DBVT_WSTRING:	if ( fld->typ==FLD_WST ){
								//*((WCHAR *)fld->ptr) = 0;	// NULL flag
								WStr_NCpy((WCHAR *)Fld_Ptr(fld->ptr, fld->typ),
										val.m_pstring->GetBuffer(0), fld->siz-1  );
								//wcsncpy_s((WCHAR *)Fld_Ptr(fld->ptr, fld->typ), fld->siz,
								//		val.m_pstring->GetBuffer(0), val.m_pstring->GetLength()  );
							}
							else if ( fld->typ==FLD_WSI ){
								*((int *)Fld_Ptr(fld->ptr, fld->typ)) = _wtoi( val.m_pstring->GetBuffer() );
							}
							break;
		case DBVT_BOOL:
		case DBVT_BINARY:	break;
		}			
		
	}

	return( chk );
}

int		Dbs_Get(				// フィールドデータ取得
Dbs_fld		*fld,				// 格納領域
WCHAR		**erm,				// エラーメッセージ格納領域
int			fct,				// カラム数
CRecordset	*rec )				// レコードセットクラス
{
	int				chk;

	all_nul( fld, fct, FALSE );

	chk = ( rec && !rec->IsEOF() ) ? QRY_NML: QRY_END;
	while( chk==QRY_NML && fct-- ){
		chk = get_fld( erm, fld, rec );
		++fld;
	}
	if ( chk==QRY_NML )	rec->MoveNext();
	return( chk );
}

int		Dbs_MGt(				// フィールドデータ取得
Mlt_dbf		*mfl,				// 格納領域
WCHAR		**erm,				// エラーメッセージ格納領域
int			mfc,				// カラム数
CRecordset	*rec )				// レコードセットクラス
{
	int				chk, idx, fct;
	Dbs_fld			*fld;

	for ( idx=0; idx<mfc; idx++ ){
		chk = ( rec && !rec->IsEOF() ) ? QRY_NML: QRY_END;
		if ( chk==QRY_NML ){
			all_nul( (mfl+idx)->fld, (mfl+idx)->flc, FALSE );
			fld = (mfl+idx)->fld;
			fct = (mfl+idx)->flc;
			while( chk==QRY_NML && fct-- ){
				chk = get_fld( erm, fld, rec );
				++fld;
			}
		}
	}
	if ( chk==QRY_NML )	rec->MoveNext();
	return( chk );
}

void	Dbs_Cls(				// 問合わせクローズ
CRecordset	*rec )				// レコードセットクラス
{
	if ( rec ){
		rec->Close();
		delete	rec;
	}
}

int		Dbs_Upd(				// 更新クエリー実行
CDatabase	*dbc,				// データベースクラス
WCHAR		*sql,				// 更新SQL
WCHAR		**erm )				// エラーメッセージ格納領域
{
	int			chk=QRY_NML;

	*erm = NULL;

	try {
		dbc->ExecuteSQL( sql );
	}catch( CDBException *dbe ){
		*erm = (WCHAR *)malloc( wsizeof(dbe->m_strError.GetLength()+1) );
		if ( *erm )	wcscpy_s( *erm, 
						dbe->m_strError.GetLength()+1, dbe->m_strError );
		dbe->Delete();
//		chk = !memcmp( *erm, "ORA-06510", 9 ) ? QRY_USR: QRY_DBS
		chk = QRY_DBS;
	}catch( CMemoryException *mee ){
		*erm = (WCHAR *)malloc( wsizeof(256) );
		if ( *erm )	mee->GetErrorMessage( *erm, 256, NULL );
		mee->Delete();
		chk = QRY_MEM;
	}
	return( chk );
}

int		Dbs_Trn(				// Transaction operator
WCHAR		**erm,				// Destination pointer for sroring error message
CDatabase	*dbc,				// Pointer of ODBC Database object
int			mod )				// Operation mode of transaction
{
	int			chk=FALSE, siz;
	WCHAR		*opn=_T("Unknown"), *pfx=_T(" transaction was failure");

	switch( mod ){
	case TRN_STT:	chk = dbc->BeginTrans();	opn=_T("Begin");		break;
	case TRN_CMT:	chk = dbc->CommitTrans();	opn=_T("Commit");		break;
	case TRN_RBK:	chk = dbc->Rollback();		opn=_T("Rollback");		break;
	}

	if ( !chk == !!erm ){
		siz = (int)wcslen(opn) + (int)wcslen(pfx) + 1;
		if ( !!( *erm = (WCHAR *)malloc( siz * wsizeof(1) ) ) ){
			swprintf_s( *erm, siz, _T("%s%s"), opn, pfx ); 
		}
	}

	return( chk );
}
