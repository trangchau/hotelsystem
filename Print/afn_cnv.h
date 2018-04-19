/*
	行列演算ユーティリティ定義
	Copyright(C) Arsnova Corp. 2003
*/

#ifndef AFN_CNV_H			/*  当該定義確認*/
#define AFN_CNV_H

typedef struct {					/*  一次変換行列  */
	double			xa, xb, xc;		/*  Ｘ列変換係数  */
	double			ya, yb, yc;		/*  Ｙ列変換係数  */
} Mtx_def;

typedef struct {					/*  領域        */
	double			sx, sy;			/*  左下隅座標  */
	double			ex, ey;			/*  右上隅座標  */
} Fld_def;

/* ----- 幾何要素定義 ------------------------- */
typedef struct {					/*  座標    */
	double			x, y;			/*  座標値  */
} Dpn;

void Set_Mtx(Mtx_def *,Fld_def *,Fld_def *,int);
void Get_Mtx(Mtx_def *,Fld_def *,Fld_def *,int);
void Dpn_Cnv(Dpn *,Dpn *,int,Mtx_def *);
void Trs_Mtx(Mtx_def *,double,double,Dpn *,Dpn *);
void Prd_Mtx(Mtx_def *,Mtx_def *,Mtx_def *);
void Unt_Mtx(Mtx_def *);

#endif
