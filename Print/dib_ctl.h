/*
	ＷＩＮ３２ 印刷 ルーチン
	ビットマップ(DIB)制御関数(VCLの代替機能)

	File Name : dib_ctl.h
	Maked By S.Nagaoka ARSNOVA Co.,ltd '98/ 9/19
	At Borland C++ Builder Ver1.0

	Copyright(C) Arsnova Corp. 1987-1998
*/

#ifndef	dib_ctlH
#define	dib_ctlH

void	GetDIBSizes_( HBITMAP, int *, DWORD * );
int		GetDIB_( HBITMAP, HPALETTE, BITMAPINFO *, char * );

#endif
