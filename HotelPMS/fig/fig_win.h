/*
	Graphic object make and displaying window
*/

#ifndef	fig_winH
#define	fig_winH

#include "fig_mnp.h"

#define	DRG_NON		0			// ドラッグなし状態
#define	DRG_STT		1			// ドラッグ開始状態
#define	DRG_ING		2			// ドラッグ中状態

HWND	Crt_Win( HINSTANCE, Fig_mng * );			// 描画ウィンドウ生成

#endif
