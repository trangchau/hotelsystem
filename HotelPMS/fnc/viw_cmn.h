/*
	各ビューの共通関数
*/

#ifndef	viw_cmnH
#define	viw_cmnH

#include "..\stdafx.h"
#include "..\..\Common\bsc_def.h"
#include "..\..\Common\dte_mng.h"
#include "..\..\Common\lnk_mng.h"
#include "..\..\Custom\CustomStatic.h"
#include "..\..\pms_def.h"

#define	WM_VIEWNOTIFY		(WM_USER+16386)			// Notify message from each view to BookingStatus view

// Notify codes
#define	VWN_RSV_CREATED		1						// Created a new reservation event
#define	VWN_RSV_CHANGED		2						// Changed a reservation event(CHECKIN, CHECKOUT, STAY PERIOD....)
#define	VWN_RSV_DELETED		3						// Create a reservation event
#define	VWN_STS_CHANGED		4						// Changed a stay status event(checedin, checked out...)


#define	STS_CHG_CHECKIN		1
#define	STS_CHG_CHECKOUT	2

#define ON_WM_VIEWNOTIFY() \
	{ WM_VIEWNOTIFY, 0, 0, 0, AfxSig_lwl, \
		(AFX_PMSG)(AFX_PMSGW) \
		(static_cast< LRESULT (AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM) > \
		( &ThisClass :: OnViewNotify)) },
// ViewNotifyData
typedef	struct	{
	int			ntf;				// NotifyCode
	int			vwk;				// View kind of sender
	CWnd		*sdr;				// View of sender
	void		*usr;				// User definition data( Necessary to free memory )
}VIW_NTF;

// カスタムビュー種別(Index)定義
#define	VWK_SCH			0			// Searching reservation view
#define	VWK_RSV			1			// Editting reservation view
#define	VWK_BST			2			// Booking status view
#define	VWK_CIN			3			// Checkin operating view
#define	VWK_INH			4			// Inhouse operating view
#define	VWK_COT			5			// Checkout operating view
#define	VWK_NAD			6			// Night audit view
#define	VWK_HKP			7			// House keeping view

#define	RSV_EST_OLD		1			// past reservation ( All editing impossible -> departure date is less than today ) 
#define	RSV_EST_RSV		2			// reservaed and before checkin
#define	RSV_EST_INH		4			// Checked in and before checkout
#define	RSV_EST_COT		8			// Checked out  
#define	RSV_EST_NBP		16			// no selecting booking path  
#define	RSV_EST_NEW		32			// creating a new reservation  
#define	RSV_EST_SAM		128			// same as reserved person  
#define	RSV_EST_CXL		256			// Canceled 
#define	RSV_EST_NSW		512			// No Show  


#define	REQ_COL			RGB( 255, 255, 192)			// Background color of reqired item on editbox

#define	STC_NWF			RGB( 255, 255, 255)			// Font color of reservation of status
#define	STC_NWB			RGB(   0, 192,   0)			// Background color of reservation of status
#define	STC_RVF			RGB( 255, 255, 255)			// Font color of reservation of status
#define	STC_RVB			RGB(  96,  96, 255)			// Background color of reservation of status
#define	STC_IHF			RGB(   0,   0,   0)			// Font color of inhouse of status
#define	STC_IHB			RGB( 128, 255, 128)			// Background color of reservation of status
#define	STC_COF			RGB( 192, 192, 192)			// Font color of reservation of status
#define	STC_COB			RGB( 128, 128, 128)			// Background color of reservation of status
#define	STC_CXF			RGB( 255, 255, 255)			// Font color of reservation of status
#define	STC_CXB			RGB( 255,   0,   0)			// Background color of reservation of status
#define	STC_NSF			RGB( 255, 255, 255)			// Font color of reservation of status
#define	STC_NSB			RGB( 255,   0, 255)			// Background color of reservation of status
#define	STC_OLF			RGB( 192, 192, 192)			// Font color of reservation of status
#define	STC_OLB			RGB( 128, 128, 128)			// Background color of reservation of status

#define	STC_PSF			RGB(   0,   0,   0)			// Font color of reservation of status
#define	STC_PSB			RGB( 128, 255, 128)			// Background color of reservation of status
#define	STC_GPF			RGB(   0,   0,   0)			// Font color of reservation of status
#define	STC_GPB			RGB( 255, 255, 128)			// Background color of reservation of status

#define	GetMainFrm()		((CMainFrame *)((CHotelPMSApp *)AfxGetApp())->m_pMainWnd)
#define	SetTopOfChildren(x)	(x)->GetParent()->SetWindowPos( &CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);(x)->GetParent()->ShowWindow(SW_NORMAL);

#define	IDC_GRD_RSV		(8192)						// Searching reservation grid ID
#define	IDC_GRD_CIN		(IDC_GRD_RSV+1)				//  Checking in grid ID
#define	IDC_GRD_INH		(IDC_GRD_RSV+2)				//  Inhouse reservation grid ID
#define	IDC_GRD_COT		(IDC_GRD_RSV+3)				//  Checking out grid ID

#define	VIW_SND_SCH		1							// Send message to SearchView
#define	VIW_SND_BST		2							// Send message to BookingStatusView
#define	VIW_SND_NAD		3							// Send message to NightAuditView

typedef	int (*RCH_VIW_CBK)( int, CView *, LPVOID );					// Call back function on launch the view
typedef	int (*RCH_EXV_CBK)( int, CView *, LPVOID );					// Call back function on check existing view
typedef	int (*MST_SAV_FNC)( Csr, WCHAR **, CDatabase *, PMS_MNG * );// Save function of each master

#define	WND_GET_TXT(b,i,w)		(w)->GetDlgItem(i)->GetWindowText( b, memcnt(b) )
#define	CBX_GET_CHK(i,w)		((CButton *)(w)->GetDlgItem(i))->GetCheck()


#define	ADK_WNG					1					// Additional data of equipment (WING)
#define	ADK_FCE					2					// Additional data of equipment (FACEING)
#define	ADK_VIW					3					// Additional data of equipment (VIEW)
#define	ADK_SIZ					4					// Additional data of equipment (SIZE)

typedef	struct	{									// Definition structure of additional data of equipment
	WCHAR		*nam;								// Displaying name
	int			cod;								// Additional code
}EQP_ADT;

#define	DSC_DRK					1					// description_kind on deacriptions
#define	DSC_PRK					2					// price_kind on deacriptions
#define	DSC_PMT					3					// payment_method on payment_details
#define	DSC_DPK					4					// dsposit_kind on payment_details

typedef	struct	{									// Definition structure of additional data of equipment
	WCHAR		*nam;								// Displaying name
	int			cod;								// Additional code
}DSC_CDN;

#define	STS_CLN					1					// cleaning status on equipment_states
#define	STS_OCC					2					// occupied status on equipment_states
#define	STS_SAL					3					// sale status on equipment_states

typedef	struct	{									// Definition structure of roo status of equipment
	WCHAR		*nam;								// Displaying name
	int			cod;								// Additional code
}STS_CDN;

#define	EPL_KND					1					// cleaning employee kind

typedef	struct	{									// Definition structure of roo status of equipment
	WCHAR		*nam;								// Displaying name
	int			cod;								// Additional code
}EPL_CDN;

void	Viw_Clc_Dfs( POINT *, CFormView * );								// ウィンドウ既定サイズの取得
int		Get_Int_Dat( void *, int, CWnd *, DWORD );							// Editコントロールから数値データ取得
int		Get_Str_Dat( void *, int, CWnd *, DWORD );							// Editコントロールから文字列データ取得
int		Get_Day_Dat( void *, int, CWnd *, DWORD );							// Editコントロールから日付データ取得
CView	*Rnc_Ech_Viw( int, RCH_VIW_CBK, RCH_VIW_CBK, LPVOID );				// Launch each view
WCHAR	*Viw_Get_Rmn( Csr, int );											// getting room name
WCHAR	*Viw_Get_Rtn( Csr, int );											// getting room type name
int		Viw_Get_Wtx( WCHAR *, int, void *, DWORD, CWnd * );					// Getting window text and setting null status
int		Viw_Get_Int( int *, void *, DWORD, CWnd * );						// Getting window integer and setting null status
int		Viw_Get_Dte( Day *, int *, void *, DWORD, CWnd * );					// Getting window date text and setting null status
int		Viw_Get_Tim( Day *, int *, void *, DWORD, CWnd * );					// Getting window time text and setting null status
int		Viw_Get_Cbx( int *, void *, DWORD, CWnd * );						// Getting window checkbox status and setting null status
int		Viw_Scb_Eqt( CComboBox *, int, int, Day *, Day *, int, CWnd *, CDatabase *, PMS_MNG * );
WCHAR	*Edt_Num_Nml( WCHAR *, int, int );									// 整数の通常編集
WCHAR	*Edt_Dbl_Nml( WCHAR *, int, WCHAR *, double );						// 実数の通常編集
WCHAR	*Viw_Bkp_Nam( int, int, PMS_MNG * );								// Getting booking path name
WCHAR	*Viw_Crk_Nam( int, PMS_MNG * );										// Getting client rank name
WCHAR	*Viw_Sts_Ope( CCustomStatic *, int );								// Setting the status panel
int		Viw_Inf_Sts( RSV_INF *, PMS_MNG * );								// Making a status of reservation_infos
int		Viw_Dtl_Sts( RSV_INF *, Day *, Day *, Day *, int *, PMS_MNG * );	// Making a status of reservation_details
LRESULT	Viw_Snd_Ntf( int, int, void *, LPARAM, CWnd * );					// Send message to SearchView/BookingStatusView
LRESULT	Viw_Pst_Ntf( int, int, void *, LPARAM, CWnd * );					// Post message to SearchView/BookingStatusView
int		Viw_Get_Eqk( WCHAR **, int );										// Getting equipment kind by facility lind
int		Viw_Get_Eqa( EQP_ADT **, int );										// getting equipment infos additional data 
int		Viw_Get_Dsc( DSC_CDN **, int );										// getting descriptions each selecting data 
int		Viw_Ext_Dsc( DSC_CDN **, int, int );								// getting extend descriptions each selecting data 
int		Viw_Get_Sts( STS_CDN **, int );										// getting equipment_states each selecting data 
int		Viw_Get_Epl( EPL_CDN **, int );										// getting employee kind each selecting data 
WCHAR	*Viw_Get_Dsn( int, int );											// getting description names each selecting data 
void	Viw_Prc_Edt( WCHAR *, int, double, int, PMS_MNG * );				// editting price data
int		Viw_Ovl_Pod( Day *, Day *, Day *, Day * );							// confirm date period overlap
DWORD	Viw_Cfm_Dcd( CWnd * );												// cinfirmation dialog of discard data
DWORD	Viw_Cfm_Svc( Csr, MST_SAV_FNC, int, int, CWnd *, CDatabase *, PMS_MNG * );	// Confirm to save changed
BOOL	Viw_Ctl_Btn( CWnd *, MSG * );										// Form(Dialog) button control
void	Viw_Gen_Pwd( WCHAR *, int );										// creating password
int		Viw_Cre_Hsh( int *, WCHAR * );										// creating hash code
WCHAR	*Viw_Get_Gdr( int );												// getting gender name


#define	WIN_TXT_DBS(p,i,w)		Viw_Get_Wtx(DBS_WST_PTR(p),FLD_WST_SIZ(p),p,i,w)
#define	WIN_INT_DBS(p,i,w)		Viw_Get_Int(DBS_INT_PTR(p),p,i,w)
#define	WIN_CBX_DBS(p,i,w)		Viw_Get_Cbx(DBS_INT_PTR(p),p,i,w)
#define	WIN_DAY_DBS(p,e,i,w)	Viw_Get_Dte(DBS_DAY_PTR(p),e,p,i,w)
#define	WIN_TIM_DBS(p,e,i,w)	Viw_Get_Tim(DBS_TIM_PTR(p),e,p,i,w)

#endif
