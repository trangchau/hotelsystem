/*
	Functions for each element draw 
*/

#ifndef	fig_drwH
#define	fig_drwH

#include <afx.h>
#include "fig_mnp.h"

#define	SMK_SIZ		5						// 選択マークサイズ

int		Fig_Drw( HDC, HWND );				// Drauing all figure
void	Drw_Elm( Fig_mng *, Fig_elm * );	// 単一幾何図形描画


#endif
