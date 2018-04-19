/*
	図形作成・操作ルーチン
	短点マーク処理関数
*/

#ifndef	fig_mrkH
#define	fig_mrkH

#include "fig_mnp.h"

//-- マーク追加指定
#define	MRK_LFT		1				// 左側マーク付加指定(兼始点)
#define	MRK_RIT		2				// 右側マーク付加指定(兼終点)
#define	MRK_TOP		4				// 上側マーク付加指定
#define	MRK_BTM		8				// 下側マーク付加指定

#define	MRK_DEF		0				// システム規定短点形状

#define	tmk_siz(x)	(sizeof(Trm_mrk)+sizeof(Dpn)*((x)-1))
#define	nxt_tmk(x)	((Trm_mrk *)(((char *)x)+tmk_siz((x)->pct)))

int		Add_Mrk( void *, Fig_elm *, Fig_mng * );	// 短点形状付加
int		Ent_Mrk( Trm_mrk *, Fig_mng * );			// ユーザー定義端点形状登録
double	Mrk_Siz( Mtx_def * );						// 直線用マークサイズの取得

#endif
