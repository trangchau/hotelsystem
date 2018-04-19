/*
			日付操作関数定義
*/
#ifndef		dte_mngH
#define		dte_mngH

#define	is_kanji(x)	(((x)>=0x80&&(x)<=0xa0)||(x)>=0xe0)
#define	is_leap(x)	(((x)%400)==0||(((x)%4)==0&&((x)%100)!=0))
#define	day_dte(x)	(x)->yer,(x)->mth,(x)->day
#define	day_tim(x)	(x)->hor,(x)->min,(x)->sec

int		Get_Dte( Day * );						// システム日付取得
short	Get_Mxd( Day * );						// 最大日付取得
void	Add_Dte( Day *, int );					// 日数加算
int		Cmp_Dte( Day *, Day * );				// 日付比較
int		Cmp_Dtm( Day *, Day * );				// 日付時刻比較
int		Cmp_Tim( Day *, Day * );				// 時刻比較
int		Chk_Dte( Day * );						// 日付正当性検査
int		Str_Dte( Day *, WCHAR * );				// 文字列→日付変換
int		Str_Tim( Day *, WCHAR * );				// 文字列→時刻変換
int		Clc_Dys( Day *, Day * );				// 日数(泊数)計算
double	Clc_Tim( Day *, Day * );				// 経過時間計算
int		Clc_Age( Day * );						// 年齢計算
int		Get_Dow( Day * );						// 曜日計算
WCHAR 	*Edt_Dte( WCHAR *, int, Day * );		// 日付を文字列に編集
WCHAR 	*Edt_Tim( WCHAR *, int, Day * );		// 時刻を文字列に編集
WCHAR 	*Edt_Dtm( WCHAR *, int, Day * );		// 日付時刻を文字列に編集

#endif
