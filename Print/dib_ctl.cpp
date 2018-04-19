/*
	ＷＩＮ３２ 印刷 ルーチン
	ビットマップ(DIB)制御関数(VCLの代替機能)

	File Name : Dib_Ctl.c
	Maked By S.Nagaoka ARSNOVA Co.,ltd '98/ 9/19
	At Borland C++ Builder Ver1.0

	Copyright(C) Arsnova Corp. 1987-1998
*/
#include <Windows.h>

static	long	WidthBytes(			// サイズ境界にあわせる
long	lgs )						// バイトレングス
{
	return( ( ( lgs + 31 ) / 32 ) * 4 );
}

static	void	InitializeBitmapInfoHeader(
HBITMAP				Bitmap,
BITMAPINFOHEADER	*BI,
int					Colors )
{
	BITMAP	BM;

	GetObject( Bitmap, sizeof(BITMAPINFOHEADER), &BM );

	BI->biSize = sizeof(BITMAPINFOHEADER);
	BI->biWidth = BM.bmWidth;
	BI->biHeight = BM.bmHeight;
	if ( Colors ){
		switch( Colors ){
		case 2:		BI->biBitCount = 1;
		case 16:	BI->biBitCount = 4;
		case 256:	BI->biBitCount = 8;
		}
	}
	else{
		BI->biBitCount = BM.bmBitsPixel * BM.bmPlanes;
	}
	BI->biPlanes = 1;
	BI->biXPelsPerMeter = 0;
	BI->biYPelsPerMeter = 0;
	BI->biClrUsed = 0;
	BI->biClrImportant = 0;
	BI->biCompression = BI_RGB;
	if ( BI->biBitCount==16 || BI->biBitCount==32 ){
		BI->biBitCount = 24;
	}
	BI->biSizeImage = WidthBytes(BI->biWidth * BI->biBitCount) * BI->biHeight;
}

static	void InternalGetDIBSizes(
HBITMAP	Bitmap,
int		*InfoHeaderSize,
DWORD	*ImageSize,
int		Colors )
{
	BITMAPINFOHEADER	BI;

	InitializeBitmapInfoHeader( Bitmap, &BI, Colors );
	if ( BI.biBitCount == 24 ){
		*InfoHeaderSize = sizeof(BITMAPINFOHEADER);
	}
	else{
		*InfoHeaderSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)
						*	(1 << BI.biBitCount);
	}
	*ImageSize = BI.biSizeImage;
}

void	GetDIBSizes_(
HBITMAP	Bitmap,
int		*InfoHeaderSize,
DWORD	*ImageSize )
{
  InternalGetDIBSizes( Bitmap, InfoHeaderSize, ImageSize, 0 );
}

static	int		InternalGetDIB(			// ビットマップビット取得
HBITMAP		Bitmap,
HPALETTE	Palette,
BITMAPINFO	*BitmapInfo,
char		*Bits,
int			Colors )
{
	HPALETTE	OldPal;
	HWND		Focus;
	HDC			DC;
	int			ret;

	InitializeBitmapInfoHeader(Bitmap, (BITMAPINFOHEADER *)BitmapInfo, Colors);
	OldPal = 0;
	Focus = GetFocus();
	DC = GetDC(Focus);

	if ( Palette ){
		OldPal = SelectPalette(DC, Palette, FALSE);
		RealizePalette(DC);
	}
	ret = GetDIBits(DC, Bitmap, 0, ((BITMAPINFOHEADER *)BitmapInfo)->biHeight,
										Bits, BitmapInfo, DIB_RGB_COLORS ) != 0;
	if ( OldPal )	SelectPalette( DC, OldPal, FALSE );
	ReleaseDC(Focus, DC);

	return( ret );
}

int		GetDIB_(
HBITMAP		Bitmap,
HPALETTE	Palette,
BITMAPINFO	*BitmapInfo,
char		*Bits )
{
	return( InternalGetDIB(Bitmap, Palette, BitmapInfo, Bits, 0) );
}
