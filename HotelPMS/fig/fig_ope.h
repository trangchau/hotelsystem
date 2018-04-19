/*
	Figure window operation processing functions
*/

#ifndef	fig_opeH
#define	fig_opeH

#include "fig_mnp.h"

//-- 操作司令
#define	OPE_CRT		1				// 生成操作
#define	OPE_MOV		2				// 移動操作
#define	OPE_CHG		4				// 変形操作
#define	OPE_CUT		8				// 切断操作

//-- 操作イベント
#define	EVT_SLS		10				// 範囲選択開始
#define	EVT_SLE		11				// 範囲選択完了
#define	EVT_CRS		12				// 作成開始
#define	EVT_CRE		13				// 作成完了
#define	EVT_CRI		14				// 作成完了前
#define	EVT_CGS		15				// 変形開始
#define	EVT_CGI		16				// 変形完了前
#define	EVT_CGE		17				// 変形完了F
#define	EVT_CTS		18				// 切断開始
#define	EVT_CTI		19				// 切断完了前
#define	EVT_CTE		20				// 切断完了
#define	EVT_MVS		21				// 移動開始
#define	EVT_MVI		22				// 移動完了前
#define	EVT_MVE		23				// 移動完了
#define	EVT_HIT		24				// 単一要素ヒット

#define	EVT_DRG		25				// Dragging the mouse
#define	EVT_RDN		26				// Right button down on the mouse

#define	SID_LFT		0x100			// 左
#define	SID_RIT		0x200 			// 右
#define	SID_UPR		0x400 			// 上
#define	SID_LWR		0x800 			// 下
#define	SID_SP		0x1000			// 始点
#define	SID_EP		0x2000			// 終点

//-- 操作詳細
#define	DTL_NON		0				// 変形指定なし
#define	CHG_TOP		1				// 頂点変形
#define	CHG_SID		2				// 辺変形
#define	CHG_CUT		3				// 切断

//-- 切断方向
#define	CUT_HRZ		1				// 水平分割
#define	CUT_VRT		2				// 垂直分割

#define	CHG_TOP		1			// 頂点変形
#define	CHG_SID		2			// 辺変形
#define	CHG_CUT		3			// 切断変形

#define	cur_set_(x)	((LONG)(LONG_PTR)LoadCursor(mng->ins,MAKEINTRESOURCE(x)))

Fig_elm	*Hit_Tst( Fig_mng * );						// 図形ヒット調査
int		Ope_Stt( Fig_mng * );						// 操作開始イベント処理
int		Ope_End( double, Fig_mng * );				// 操作終了イベント処理
int		Ope_Drg( int, int, int, int,  Fig_mng * );	// 操作終了イベント処理
void	Set_Ope(  Fig_mng *, int, int, int );		// 詳細操作モード設定
void	Rdn_Ope(  Fig_mng *, int, int, int );

#endif
