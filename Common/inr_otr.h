/*
	内部データ⇔外部Ｉ／Ｆデータ変換 定義
*/

#ifndef	inr_otrH
#define	inr_otrH

void	Inr_Otr( ORG_EQP *, INR_EQP * );			// 内部データ→外部データ変換
void	Otr_Inr( INR_EQP *, ORG_EQP * ); 			// 外部データ→内部データ変換	
INR_BRG	*Get_Brg( BRG_INF * );						// 内輪情報取得

#endif

