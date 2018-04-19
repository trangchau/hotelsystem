/*
		初期化ファイルアクセス関数定義
*/
#ifndef Ini_InfH
#define Ini_InfH

//#include <windows.h>
#include "bsc_def.h"

#define	ITP_INT		1			// 整数
#define	ITP_STR		2			// 文字列

#define	VAL_SUC		0			// 正常終了
#define	VAL_NON		1			// 値なし
#define	VAL_MEM		2			// メモリー不足

typedef	struct {				// 格納テーブル定義
	WCHAR	*sct;				// セクション文字列
	WCHAR	*key;				// キー文字列
	int		mcl;				// 複数カラム情報あり
	int		mlt;				// 複数行情報あり
	char	typ;				// キーの属性
	void	*val;				// 格納変数の位置
}Ini_inf;

int		Get_Ini( Ini_inf *, int, WCHAR * );		// プログラム情報の取り込み
int		Set_Ini( Ini_inf *, int, WCHAR * );		// プログラム情報の書き込み
void	Cst_Ini( Ini_inf *, int );				// プログラム情報の破棄

#endif
