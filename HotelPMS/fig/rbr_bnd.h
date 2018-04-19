/*
	図形作成・操作ルーチン
	図形影（ラバーバンド）処理関数
*/

#ifndef	rbr_bndH
#define	rbr_bndH

#include "fig_mnp.h"

#define	RBR_LIN		1				// 直線ラバーバンド
#define	RBR_RCT		2				// 矩形ラバーバンド
#define	RBR_CCL		3				// 円ラバーバンド（未サポート）

#define	RBR_COL		RGB(255,255,0)	// ラバーバンドの色

void	Rbr_Bnd( HDC, Fig_mng * );	// 要素の影描画

#endif
