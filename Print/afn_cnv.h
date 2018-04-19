/*
	�s�񉉎Z���[�e�B���e�B��`
	Copyright(C) Arsnova Corp. 2003
*/

#ifndef AFN_CNV_H			/*  ���Y��`�m�F*/
#define AFN_CNV_H

typedef struct {					/*  �ꎟ�ϊ��s��  */
	double			xa, xb, xc;		/*  �w��ϊ��W��  */
	double			ya, yb, yc;		/*  �x��ϊ��W��  */
} Mtx_def;

typedef struct {					/*  �̈�        */
	double			sx, sy;			/*  ���������W  */
	double			ex, ey;			/*  �E������W  */
} Fld_def;

/* ----- �􉽗v�f��` ------------------------- */
typedef struct {					/*  ���W    */
	double			x, y;			/*  ���W�l  */
} Dpn;

void Set_Mtx(Mtx_def *,Fld_def *,Fld_def *,int);
void Get_Mtx(Mtx_def *,Fld_def *,Fld_def *,int);
void Dpn_Cnv(Dpn *,Dpn *,int,Mtx_def *);
void Trs_Mtx(Mtx_def *,double,double,Dpn *,Dpn *);
void Prd_Mtx(Mtx_def *,Mtx_def *,Mtx_def *);
void Unt_Mtx(Mtx_def *);

#endif
