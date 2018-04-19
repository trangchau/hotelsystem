/*
	予約状況処理関数
*/

#ifndef	bst_fncH
#define	bst_fncH

#include "..\stdafx.h"
#include "..\BookingStatusDoc.h"
#include "..\BookingStatusView.h"

HFONT	Bst_Cre_Fnt( WCHAR *, int, int, int, HDC );						// 文字列用フォント作成
int		Bst_Get_Fsz( int, HDC );										// 文字サイズ（水平ピクセル数取得）
int		Bst_Get_Rom( Csr, CBookingStatusView * );						// Getting list of room definition
int		Bst_Mak_Sts( CBookingStatusView *, BST_MNG * );					// Getting list of room use status
void	Bst_Pod_Chg( CBookingStatusView *, BST_MNG * );					// Displaying period change
void	Bst_Upd_Gph( Csr, CBookingStatusView *, BST_MNG * );			// graphics update from each view
void	Bst_Ctx_Mnu( CWnd *, int, int, LPVOID, PMS_MNG * );				// Open context menu on search view
void	Bst_Rch_Viw( DWORD, int, int );									// Launch each view
void	Bst_Rch_Crv( GPH_RDY * );										// Launch create a new reservation view
int		Bst_Rsv_Chg( BST_ROM_STS *, GPH_RDY *, CWnd *, CDatabase *, BST_MNG * );// Chainging a reservation from graph
void	Bst_Cin_Dir( BST_ROM_STS *, CWnd *, CDatabase *, BST_MNG * );	// Execute checkin direct
void	Bst_Cxl_Dir( BST_ROM_STS *, CWnd *, CDatabase *, BST_MNG * );	// Execute cancellation direct
void	Bst_Cxl_Rsv( CWnd *, CDatabase *, BST_MNG * );					// Cancel reservation
void	Bst_Upd_Sts( int, int, CWnd *, BST_MNG * );		 				// Booking status update
void	Bst_Dps_Dat( Csr );												// Room statis data casting

#endif
