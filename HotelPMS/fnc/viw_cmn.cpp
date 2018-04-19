/*
	各ビューの共通関数
*/

#include	"..\stdafx.h"
#include	"..\HotelPMS.h"
#include	"..\MainFrm.h"
#include	"..\ChildFrm.h"
#include	"..\ReservationView.h"
#include	"..\CheckinView.h"
#include	"..\InhouseView.h"
#include	"..\CheckoutView.h"
#include	"..\..\Common\bsc_def.h"
#include	"..\..\Common\dbs_mng.h"
#include	"..\..\PMSCustom\EquipmentTypeCombo.h"
#include	"..\..\pms_def.h"
#include	"ecg_sql.h"
#include	"viw_cmn.h"

#define	is_new(x)				(!!Nul_Fld((x)->id,FLD_INT))

void	Viw_Clc_Dfs(						// ウィンドウ既定サイズの取得
POINT			*siz,						// 格納領域
CFormView		*viw )						// 対象のビュー
{
	WINDOWPLACEMENT		wpc[1];
	RECT				rct[1];

	viw->GetWindowPlacement( wpc );
	*rct = wpc->rcNormalPosition;
	rct->left = rct->right - rct->left + GetSystemMetrics( SM_CXFRAME ) * 2;
	rct->top = rct->bottom - rct->top + GetSystemMetrics( SM_CYFRAME ) * 2 + GetSystemMetrics( SM_CYCAPTION );
	siz->x = rct->left;
	siz->y = rct->top;
}

static	int	get_edt(		// エディットコントロールからの文字列取得
CString		*dst,			// 格納領域
CWnd		*viw,			// 取得元ビュー
DWORD		cid )			// コントロールID
{
	viw->GetDlgItem(cid)->GetWindowText( *dst );
	*dst = dst->Trim();
	return( dst->GetLength()==0 );
}

int		Get_Int_Dat(		// Editコントロールから数値データ取得
void		*dst,			// 格納領域
int			siz,			// 格納領域サイズ
CWnd		*wnd,			// 取得元ビュー
DWORD		cid )			// コントロールID
{
	CString		buf;
	int			nul;

	Set_Nul( dst, FLD_INT, nul = get_edt( &buf, wnd, cid ) );
	*((int *)Fld_Ptr(dst,FLD_INT)) = _wtoi( buf );

	return( nul );
}

int		Get_Str_Dat(		// Editコントロールから文字列データ取得
void		*dst,			// 格納領域
int			siz,			// 格納領域サイズ
CWnd		*wnd,			// 取得元ビュー
DWORD		cid )			// コントロールID
{
	CString		buf;
	int			nul;

	Set_Nul( dst, FLD_WST, nul = get_edt( &buf, wnd, cid ) );
	wcscpy_s( (WCHAR *)Fld_Ptr(dst,FLD_WST), siz, buf.GetBuffer( siz ) );

	return( nul );
}

int		Get_Day_Dat(		// Editコントロールから日付データ取得
void		*dst,			// 格納領域
int			siz,			// 格納領域サイズ
CWnd		*wnd,			// 取得元ビュー
DWORD		cid )			// コントロールID
{
	CString		buf;
	int			chk, nul;

	Set_Nul( dst, FLD_DAY, nul = get_edt( &buf, wnd, cid ) );
	chk = !!nul || Str_Dte( (Day *)Fld_Ptr(dst,FLD_DAY), buf.GetBuffer(0) );

	return( chk ? nul: -1 );
}

WCHAR	*Edt_Num_Nml(		// 整数の通常編集
WCHAR		*dst,			// 格納領域
int			siz,			// 格納領域サイズ
int			dat )			// ＤＢフィールドポインタ
{
	swprintf_s( dst, siz, _T("%d"), dat );
	return( dst );
}

WCHAR	*Edt_Dbl_Nml(		// 実数の通常編集
WCHAR		*dst,			// 格納領域
int			siz,			// 格納領域サイズ
WCHAR		*fmt,			// 格納領域
double		dat )			// ＤＢフィールドポインタ
{
	swprintf_s( dst, siz, fmt, dat );
	return( dst );
}


static	int		sch_exv(			// Searching already exsiting view
int			vkd,					// View kind of target view
RCH_EXV_CBK	exv,					// Call back function on check existing view
LPVOID		cbp )					// Call back parameter
{
	CMultiDocTemplate	*tmp=((CHotelPMSApp *)AfxGetApp())->pDocTemplate[vkd];
	CDocument			*doc;
	CFormView			*viw;
	POSITION			pos;
	int					ext=FALSE;

	pos = tmp->GetFirstDocPosition();
	while( pos ){
		doc = (CDocument *)tmp->GetNextDoc(pos);
		if ( !!( pos = doc->GetFirstViewPosition() ) ){
			viw = (CFormView *)doc->GetNextView(pos);
			//if ( ((CChildFrame *)(viw->GetParent()))->m_Rsv_id == rid ){
			if ( ext = exv( vkd, viw, cbp ) ){
				// if found, set top of Z order with founded window
				//viw->GetParent()->SetWindowPos( &CWnd::wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
				SetTopOfChildren( viw );
				break;
			}
		}
	}

	return( ext );
}

CView		*Rnc_Ech_Viw(			// Launch each view
int			vkd,					// Kind of target view
RCH_EXV_CBK	exv,					// Call back function on check existing view
RCH_VIW_CBK	cbk,					// Call back function on open new document
LPVOID		cbp )					// Call back parameter
{
	CMultiDocTemplate	*tmp;
	CDocument			*doc;
	CView				*viw=NULL;
	POSITION			pos;
	int					chk=TRUE;

	if ( vkd != -1 ){
		if ( tmp=((CHotelPMSApp *)AfxGetApp())->pDocTemplate[vkd] ){
			if ( !sch_exv( vkd, exv, cbp ) ){
				// New document open when not exist
				doc = tmp->OpenDocumentFile(NULL);
				if ( !!( pos = doc->GetFirstViewPosition() ) ){
					viw = doc->GetNextView( pos );
					chk = cbk( vkd, viw, cbp );
					if ( !chk )		doc->OnCloseDocument();
				}
			}
		}
	}

	return( viw );
}

WCHAR		*Viw_Get_Rmn(				// getting room name
Csr				rot,					// root of equipment_infos
int				eii )					// equipment_info_id
{
	Csr			csr=NULL;
	EQP_INF		*eqp;
	WCHAR		*rnm=NULL;

	while( csr=Nxt_Lnk( rot, csr ) ){
		eqp = Rec_Of(EQP_INF, csr);
		if ( DBS_INT_VAL(eqp->id) == eii ){
			rnm = (WCHAR *)Fld_Ptr(eqp->eco, FLD_WST);
			break;
		}
	}

	return( rnm );
}

WCHAR		*Viw_Get_Rtn(				// getting room type name
Csr				rot,					// root of equipment_infos
int				eti )					// equipment_info_id
{
	Csr			csr=NULL;
	EQP_TYP		*eqt;
	WCHAR		*rtn=NULL;

	while( csr=Nxt_Lnk( rot, csr ) ){
		eqt = Rec_Of(EQP_TYP, csr);
		if ( DBS_INT_VAL(eqt->id) == eti ){
			rtn = DBS_WST_PTR(eqt->nam);
			break;
		}
	}

	return( rtn );
}

int		Viw_Get_Wtx(						// Getting window text and setting null status
WCHAR		*dst,							// destination of storeing area
int			siz,							// Storeing area size(Character count)
void		*ptr,							// data pointer
DWORD		cid,							// Contorol ID
CWnd		*wnd )							// Parent window
{
	WCHAR		buf[2048];
	int			nul;

	WND_GET_TXT(buf, cid, wnd);
	wcscpy_s(dst, siz,  buf );
	Set_Nul(ptr,FLD_WST,nul=wcslen(buf)==0);

	return( nul);
}

int		Viw_Get_Int(						// Getting window integer and setting null status
int			*dst,							// destination of storeing area
void		*ptr,							// data pointer
DWORD		cid,							// Contorol ID
CWnd		*wnd )							// Parent window
{
	WCHAR		buf[256];
	int			nul;

	WND_GET_TXT(buf, cid, wnd);
	*dst = _wtoi(buf);
	Set_Nul(ptr,FLD_WST,nul=wcslen(buf)==0);

	return( nul);
}


int		Viw_Get_Dte(						// Getting window text and setting null status
Day			*dst,							// destination of storeing area
int			*err,							// error on convert string to date
void		*ptr,							// data pointer
DWORD		cid,							// Contorol ID
CWnd		*wnd )							// Parent window
{
	WCHAR		buf[256];
	int			nul;

	WND_GET_TXT(buf, cid, wnd);
	nul = wcslen(buf)==0;
	if ( !nul )	*err = !Str_Dte(dst, buf);
	else		*err = 0;
	Set_Nul(ptr,FLD_DAY,nul);

	return( nul);
}

int		Viw_Get_Tim(						// Getting window text and setting null status
Day			*dst,							// destination of storeing area
int			*err,							// error on convert string to date
void		*ptr,							// data pointer
DWORD		cid,							// Contorol ID
CWnd		*wnd )							// Parent window
{
	WCHAR		buf[256];
	int			nul;

	WND_GET_TXT(buf, cid, wnd);
	nul = wcslen(buf)==0;
	if ( !nul )	*err = !Str_Tim(dst, buf);
	else		*err = 0;
	Set_Nul(ptr,FLD_TIM,nul);

	return( nul);
}


int		Viw_Get_Cbx(						// Getting window checkbox status and setting null status
int			*dst,							// destination of storeing area
void		*ptr,							// data pointer
DWORD		cid,							// Contorol ID
CWnd		*wnd )							// Parent window
{
	*dst = CBX_GET_CHK(cid, wnd);
	Set_Nul(ptr,FLD_INT,FALSE);

	return( FALSE);
}

int		Viw_Scb_Eqt(				// Set equipment type combobox
CComboBox			*cbx,			// Destination combobox pointer
int					pth,			// booking path( reservation_route)
int					agi,			// id of agant_infos
Day					*std,			// use start date
Day					*edd,			// use end date
int					pst,			// getting past data( no edit data)
CWnd				*wnd,			// Parent window
CDatabase			*dbc,			// Database ODBC object
PMS_MNG				*mng )			// HotelPMS system management segment
{
	int			chk;
	Lnk			rot[1], *csr=NULL;
	WCHAR		*erm=NULL, *msg=NULL;
	ECG_AVL_EQT	*eqt;
	ECG_SQL_PRM	prm[1];
	CEquipmentTypeCombo	*etc = (CEquipmentTypeCombo *)cbx;

	memset( rot, 0, sizeof(rot) );

	prm->bfr = pth;
	prm->agi = agi;
	prm->opc = mng->opc;
	prm->brc = mng->brc;
	prm->fcc = mng->fcc;
	prm->std = std;
	prm->edd = edd;

	etc->ResetAvalable();

	if ( !!pth ){
		if ( chk = Ecg_Avl_Eqt( rot, prm, pst, &erm, dbc, mng ) ){
			while( csr = Nxt_Lnk( rot, csr ) ){
				eqt = Rec_Of(ECG_AVL_EQT, csr);
				etc->SetAvalable( DBS_INT_VAL(eqt->eti), TRUE );
			}
			if ( !Cnt_Lnk( rot ) )		msg=_T("There are no price settings on this BookingPath");
		}
		if ( !pst && ( !!msg || !!erm ) ){
			wnd->MessageBox( !!erm ? erm: msg, NULL, MB_ICONEXCLAMATION|MB_OK );
			if ( erm )		free( erm );
		}
		Del_All( rot );
	}

	return( chk );
}

static	struct	{
	int		cod;
	WCHAR	*nam;
}Bph_[] = {
	{ VIA_TELEPHONE,	_T("Telephone"),		},	// Reservation via tel.
	{ VIA_FAX,			_T("Fax"),				},	// Reservation via fax
	{ VIA_EMAIL,		_T("email"),			},	// Reservation via direct email
	{ VIA_WALKIN,		_T("Walk In"),			},	// Walkin checkin
	{ VIA_WEBSITE,		_T("Web Reservation"),	}	// Resrevation via direct WebSite
};

WCHAR		*Viw_Bkp_Nam(				// Getting booking path name
int			bph,						// booking path
int			aii,						// agent_info_id
PMS_MNG		*mng )						// Hotel Property Management System Structure
{
	Csr					csr=NULL;
	int					idx;
	WCHAR				*rsl=_T("Unknown");

	if ( bph==VIA_AGENT ){
		while( csr=Nxt_Lnk( mng->agl, csr ) ){
			if ( DBS_INT_VAL(Rec_Of(AGT_INF, csr)->id)==aii ){
				rsl = DBS_WST_PTR(Rec_Of(AGT_INF, csr)->nam);
				break;
			}
		}
	}
	else{
		for ( idx=0; idx<memcnt(Bph_); idx++ ){
			if ( (Bph_+idx)->cod == bph ){
				rsl = (Bph_+idx)->nam;
				break;
			}
		}
	}
	return( rsl );
}

static	struct	{
	int		cod;
	WCHAR	*nam;
}Rnk_[] = {
	{ CLL_VIP,		_T("Very Important Person(VIP)")	},	// Very Important Person
	{ CLL_DOMINANT,	_T("Dominan")						},	// Dominant Person
	{ CLL_GENERAL,	_T("General")						},	// General Person
	{ CLL_UG,		_T("Undesirable Guest(UG)")			}	// Undesirable Guest
};

WCHAR		*Viw_Crk_Nam(				// Getting client rank name
int			crk,						// client_lank
PMS_MNG		*mng )						// Hotel Property Management System Structure
{
	Csr					csr=NULL;
	int					idx;
	WCHAR				*rsl=_T("Unknown");

	for ( idx=0; idx<memcnt(Rnk_); idx++ ){
		if ( (Rnk_+idx)->cod == crk ){
			rsl = (Rnk_+idx)->nam;
			break;
		}
	}

	return( rsl );
}

WCHAR		*Viw_Sts_Ope(				// Setting the status panel
CCustomStatic	*pnl,					// Panel pointer
int				est )					// Status of reservation detail
{
	COLORREF	fcl, bcl;
	WCHAR		*str=_T("Unknown");

	if ( !!( est & RSV_EST_NEW ) ){
		fcl=STC_NWF;	bcl=STC_NWB;	str = _T(" Creating ");
	}
	else if ( !!( est & RSV_EST_NSW ) ){
		fcl=STC_NSF;	bcl=STC_NSB;	str = _T(" No Show ");
	}
	else if ( !!( est & RSV_EST_CXL ) ){
		fcl=STC_CXF;	bcl=STC_CXB;	str = _T(" Canceled ");
	}
	else if ( !!( est & RSV_EST_OLD ) ){
		fcl=STC_OLF;	bcl=STC_OLB;	str = _T(" Passed ");
	}
	else if ( !!( est & RSV_EST_INH ) ){
		fcl=STC_IHF;	bcl=STC_IHB;	str = _T(" Inhouse ");
	}
	else if ( !!( est & RSV_EST_COT ) ){
		fcl=STC_COF;	bcl=STC_COB;	str = _T(" Checked out ");
	}
	else if ( !!( est & RSV_EST_RSV ) ){
		fcl=STC_RVF;	bcl=STC_RVB;	str = _T(" Reserved ");
	}

	if ( pnl ){
		pnl->SetBGColor( bcl );
		pnl->SetFTColor( fcl );
		pnl->SetText( str );
	}

	return( str );
}

int			Viw_Inf_Sts(			// Making a editing status of reservation_infos
RSV_INF		*inf,					// reservation_infos data
PMS_MNG		*mng )					// Hotel Property Management System Structure
{
	int			sts=0;
	Csr			csr=NULL;

	sts |= RSV_EST_RSV;
	sts |= ( Cmp_Dte( DBS_DAY_PTR(inf->dpt), mng->bdt ) < 0 ) ? RSV_EST_OLD: 0;
	sts |= DBS_INT_VAL(inf->cxl) ? RSV_EST_CXL: 0;		// canceled 
	sts |= DBS_INT_VAL(inf->nsw) ? RSV_EST_NSW: 0;		// No Show

	switch( DBS_INT_VAL(inf->cis) ){
	case CIS_BEFORE_CHECKIN:						break;
	case CIS_INHOUSE:			sts |= RSV_EST_INH;	break;
	case CIS_AFTER_CHECKOUT:	sts |= RSV_EST_COT;	break;
	}

	return( sts );
}
int			Viw_Dtl_Sts(			// Making a editing status of reservation_details
RSV_INF		*rif,					// reservation_infos data
Day			*dpt,					// departure_date of reservation_details
Day			*cid,					// check_in_date of reservation_details
Day			*cod,					// check_out_date of reservation_details
int			*cxl,
PMS_MNG		*mng )					// Hotel Property Management System Structure
{
	int			sts=0;
	Csr			csr=NULL;

	sts |= RSV_EST_RSV;
	sts |= ( Cmp_Dte( DBS_DAY_PTR(dpt), mng->bdt ) < 0 ) ? RSV_EST_OLD: 0;
	sts |= !!DBS_INT_VAL(rif->nsw) ? RSV_EST_NSW: 0;
	if ( !Nul_Fld(cid, FLD_DTM) )	sts |= Nul_Fld(cod, FLD_DTM) ? RSV_EST_INH: RSV_EST_COT;
	sts |= !!DBS_INT_VAL(cxl) ? RSV_EST_CXL: 0;

	return( sts );
}

static	LRESULT	viw_ntf(
int			snd,					// Send notify destination
int			ntf,					// Notify Code
void		*usr,					// User definition data
int			sms,					// SendMessage
LPARAM		lpr,					// LPARAM of windows
CWnd		*src )					// sender
{
	LRESULT		rsl=FALSE;
	VIW_NTF		*vwn;
	CView		*tgt=NULL;

	switch( snd ){
	case VIW_SND_SCH:	tgt=GetMainFrm()->m_Sch_viw;	break;
	case VIW_SND_BST:	tgt=GetMainFrm()->m_Bst_viw;	break;
	case VIW_SND_NAD:	tgt=GetMainFrm()->m_Nad_viw;	break;
	}

	if ( rsl = !!( vwn=(VIW_NTF *)calloc( 1, sizeof(VIW_NTF) ) ) ){
		vwn->ntf = ntf;
		vwn->usr = usr;
		vwn->sdr = src;
		vwn->vwk = ((CChildFrame *)src->GetParent())->m_Viw_knd;
		if ( tgt ){
			if ( snd )	rsl = tgt->SendMessage( WM_VIEWNOTIFY, (WPARAM)(LONG_PTR)vwn, lpr );
			else		rsl = tgt->PostMessage( WM_VIEWNOTIFY, (WPARAM)(LONG_PTR)vwn, lpr );
		}
		else{
			free( vwn );
		}
	}

	return( rsl );
}

LRESULT		Viw_Snd_Ntf(			// Send message to BookingStatusView
int			snd,					// Send notify destination
int			ntf,					// Notify Code
void		*usr,					// User definition data					
LPARAM		lpr,					// LPARAM of windows
CWnd		*src )					// sender
{
	return( viw_ntf( snd, ntf, usr, TRUE, lpr, src ) );
}

LRESULT		Viw_Pst_Ntf(			// Post message to BookingStatusView
int			snd,					// Send notify destination
int			ntf,					// Notify Code
void		*usr,					// User definition data					
LPARAM		lpr,					// LPARAM of windows
CWnd		*src )					// sender
{
	return( viw_ntf( snd, ntf, usr, FALSE, lpr, src ) );
}

int	Viw_Get_Eqk(					// Getting equipment kind by facility lind
WCHAR	**fcn,						// Facility name ( NULL if no need )
int		fck )						// Facility kind
{
	int			knd=0;
	WCHAR		*nam=_T("Unknown");

	switch( fck ){
	case FCK_HOTEL:		knd = EQK_ROOM;		nam=_T("Room");		break;
	case FCK_SPA:		knd = EQK_LOCKER;	nam=_T("Locker");	break;
	case FCK_SPORTS:	knd = EQK_LOCKER;	nam=_T("Locker");	break;
	case FCK_SHOP:		knd = EQK_LOCKER;	nam=_T("Locker");	break;
	case FCK_RESTAURANT:knd = EQK_TABLE;	nam=_T("Table");	break;
	case FCK_CASINO:	knd = EQK_TABLE;	nam=_T("Table");	break;
	}

	if ( fcn )	*fcn = nam;
	return( knd );
}

static	EQP_ADT	Adt_wng[] = {	{ _T("North Wing"),		EQA_NORTH_WING			},
								{ _T("East Wing"),		EQA_EAST_WING			},
								{ _T("West Wing"),		EQA_WEST_WING			},
								{ _T("South Wing"),		EQA_SOUTH_WING			} };

static	EQP_ADT	Adt_fce[] =	{	{ _T("North Facing"),	EQA_NORTH_DIRECTION		},
								{ _T("East Facing"),	EQA_EAST_DIRECTION		},
								{ _T("West Facing"),	EQA_WEST_DIRECTION		},
								{ _T("South Facing"),	EQA_SOUTH_DIRECTION		} };

static	EQP_ADT	Adt_viw[] =	{	{ _T("Ocean View"),		EQA_OCEAN_VIEW			},
								{ _T("Mountain View"),	EQA_MOUNTAIN_VIEW		},
								{ _T("City View"),		EQA_CITY_VIEW			} };

static	EQP_ADT	Adt_siz[] =	{	{ _T("Large"),			EQA_LARGE_SIZE			},
								{ _T("Regular"),		EQA_NORMAL_SIZE			},
								{ _T("Small"),			EQA_SMALL_SIZE			} };

int		Viw_Get_Eqa(			// getting equipment infos additional data 
EQP_ADT		**dst,				// destination pointer 
int			knd )				// kind of addditional data
{
	int			cnt=-1;

	switch( knd ){
	case ADK_WNG:	*dst=Adt_wng;	cnt=memcnt(Adt_wng);	break;
	case ADK_FCE:	*dst=Adt_fce;	cnt=memcnt(Adt_fce);	break;
	case ADK_VIW:	*dst=Adt_viw;	cnt=memcnt(Adt_viw);	break;
	case ADK_SIZ:	*dst=Adt_siz;	cnt=memcnt(Adt_siz);	break;
	}

	return( cnt );
}

//static	int	

static	DSC_CDN	Dsc_knd[] = {	{ _T("Merchandise"),	DRK_MERCHANDISE		},
								{ _T("Service"),		DRK_SERVICE			},
								{ _T("Tax"),			DRK_TAX				},	
								{ _T("Additional Fee"),	DRK_ADDITIONAL		},
								{ _T("Deposit"),		DRK_DEPOSIT			},
								{ _T("Discount"),		DRK_DISCOUNT		},
								{ _T("Extra Service"),	DRK_EXTRA_SERVICE	},
								{ _T("Transfer"),		DRK_TRANSFER		}};

static	DSC_CDN	Prc_knd[] = {	{ _T("Amount"),			PRK_AMOUNT			},
								{ _T("Rate"),			PRK_RATE			},
								{ _T("Amount discount"),PRK_DISC_AMOUNT		},	
								{ _T("Rate discount"),	PRK_DISC_RATE		}};

static	DSC_CDN	Pmt_mth[] = {	{ _T("Cash"),			PAYMENT_CASH		},	
								{ _T("Creditcard"),		PAYMENT_CREDITCARD	},
								{ _T("Transfer"),		PAYMENT_TRANSFER	},
								{ _T("Coupon"),			PAYMENT_COUPON		},
								{ _T("Other"),			PAYMENT_OTHER		}};

static	DSC_CDN	Dps_knd[] = {	{ _T("Deposit"),		DPK_DEPOSIT	},	
								{ _T("Payment"),		DPK_PAYMENT	},
								{ _T("Refund"),			DPK_REFUND	}};

int		Viw_Get_Dsc(			// getting descriptions each selecting data 
DSC_CDN		**dst,				// destination pointer 
int			knd )				// kind of addditional data
{
	int			cnt=-1;

	switch( knd ){
	case DSC_DRK:	*dst=Dsc_knd;	cnt=memcnt(Dsc_knd);	break;
	case DSC_PRK:	*dst=Prc_knd;	cnt=memcnt(Prc_knd);	break;
	case DSC_PMT:	*dst=Pmt_mth;	cnt=memcnt(Pmt_mth);	break;
	case DSC_DPK:	*dst=Dps_knd;	cnt=memcnt(Dps_knd);	break;
	}

	return( cnt );
}

int		Viw_Ext_Dsc(			// getting extend descriptions each selecting data 
DSC_CDN		**dst,				// destination pointer 
int			knd,				// kind of addditional data
int			drk )				// description_kind
{
	int			idx=-1, idy;
	DSC_CDN		*ptr;

	*dst = ptr = (DSC_CDN *)calloc( 3, sizeof(DSC_CDN) );

	if ( ptr ){
		switch(drk){
		case DRK_MERCHANDISE:
		case DRK_SERVICE:
		case DRK_DEPOSIT:
		case DRK_ADDITIONAL:
		case DRK_TRANSFER:
		case DRK_EXTRA_SERVICE:	ptr[0].cod=PRK_AMOUNT;
								break;
		case DRK_TAX:			ptr[0].cod=PRK_AMOUNT;
								ptr[1].cod=PRK_RATE;
								break;
		case DRK_DISCOUNT:		ptr[0].cod=PRK_DISC_AMOUNT;
								ptr[1].cod=PRK_DISC_RATE;	
								break;
		}
		for ( idx=0; ptr->cod!=0; idx++, ptr++ ){
			for ( idy=0; idy<memcnt(Prc_knd); idy++ ){
				if ( Prc_knd[idy].cod == ptr->cod ){
					ptr->nam = Prc_knd[idy].nam;
					break;
				}
			}
		}
	}
	return( idx );		
}

static	STS_CDN	Cln_sts[] = {	{ _T("Clean"),			CLS_CLEAN		},
								{ _T("Dirty"),			CLS_DIRTY		},
								{ _T("Inspected"),		CLS_INSPECTED	},
								{ _T("Pickup"),			CLS_PICKUP		}};

static	STS_CDN	Occ_sts[] = {	{ _T("Vacant"),			USS_VACANT		},
								{ _T("Occupied"),		USS_OCCUPIED	}};

static	STS_CDN	Sal_sts[] = {	{ _T(""),				RSS_ENABLE		},
								{ _T("OutOfOrder"),		RSS_OUTOFORDER	},
								{ _T("OutOfService"),	RSS_OUTOFSERVICE}};

int		Viw_Get_Sts(			// getting equipment_states each selecting data 
STS_CDN		**dst,				// destination pointer 
int			knd )				// kind of addditional data
{
	int			cnt=-1;

	switch( knd ){
	case STS_CLN:	*dst=Cln_sts;	cnt=memcnt(Cln_sts);	break;
	case STS_OCC:	*dst=Occ_sts;	cnt=memcnt(Occ_sts);	break;
	case STS_SAL:	*dst=Sal_sts;	cnt=memcnt(Sal_sts);	break;
	}

	return( cnt );
}

static	EPL_CDN	Epl_knd[] = {	{ _T("Proper"),		EPK_PROPER	},
								{ _T("FullTime"),	EPK_FULLTIME},
								{ _T("PartTime"),	EPK_PARTTIME},
								{ _T("Other"),		EPK_OTHER	}};

int		Viw_Get_Epl(			// getting employee kind each selecting data 
EPL_CDN		**dst,				// destination pointer 
int			knd )				// kind of addditional data
{
	int			cnt=-1;

	switch( knd ){
	case EPL_KND:	*dst=Epl_knd;	cnt=memcnt(Epl_knd);	break;
	}

	return( cnt );
}

WCHAR		*Viw_Get_Dsn(		// getting description names each selecting data 
int			cod,				// code 
int			knd )				// kind of addditional data
{
	DSC_CDN		*dcn;
	WCHAR		*ptr=NULL;
	int			idx, cnt = Viw_Get_Dsc( &dcn, knd );

	if ( cnt ){
		for ( idx=0; idx<cnt; idx++ ){
			if ( (dcn+idx)->cod == cod ){
				ptr = (dcn+idx)->nam;
				break;
			}
		}
	}

	return( ptr );
}


DWORD	Viw_Cfm_Dcd(			// cinfirmation dialog of discard data
CWnd		*wnd )				// Parent window
{
	return( wnd->MessageBox( 
		_T("Are you sure you want to discard changes?"), NULL, MB_ICONQUESTION|MB_YESNO ) );
}

DWORD	Viw_Cfm_Svc(			// Confirm to save changed
Csr			rot,				// root of saving data
MST_SAV_FNC	fnc,				// Save function
int			msg,				// Displaying save changes
int			fce,				// Displaying force discard changes
CWnd		*wnd,				// Parent window
CDatabase	*dbs,				// ODBC Database object
PMS_MNG		*mng )				// Hotel Property Management System Structure
{
	int			ans=IDYES;
	WCHAR		*erm=NULL;

	if ( fce ){
		ans = Viw_Cfm_Dcd( wnd );
	}
	else{
		if ( !msg || ( ans = wnd->MessageBox( 
			_T("Do you save the changes?"), NULL, MB_ICONQUESTION|MB_YESNOCANCEL ) )==IDYES ){
			if ( !fnc( rot, &erm, dbs, mng ) ){
				if ( erm ){
					wnd->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
					free( erm );
					ans = IDCANCEL;
				}
			}
			else{
			}
		}
	}

	return( ans );
}

void	Viw_Prc_Edt(			// editting price data
WCHAR		*dst,				// destination of storing area
int			siz,				// size of storing area
double		prc,				// price data
int			prk,				// price kind ( No add a postfix '%' or ' ' if 0 )
PMS_MNG		*mng )				// Hotel Property Management System Structure
{
	WCHAR		*ptr;

	*dst = 0;
	if ( ptr = Cma_Edt( prc, mng->bsc->adp ) ){
		swprintf_s( dst, siz, _T("%s%s"), ptr, !prk ? _T(""): ( prk==PRK_RATE || prk==PRK_DISC_RATE ) ? _T("%"): _T(" ") );
		free( ptr );
	}
}

int		Viw_Ovl_Pod(			// confirm date period overlap
Day			*sst,				// source period start date
Day			*sed,				// source period end date
Day			*dst,				// destinate period start date
Day			*ded )				// destinate period end date
{
	int			ovl;

	if( ( !sst && !sed ) || ( !dst && !ded ) ){
		ovl = TRUE;
	}
	else if ( !!sst && !!sed && !!dst && !!ded ){
		ovl = ( Cmp_Dte( sst, dst ) >= 0 && Cmp_Dte( sst, ded ) <= 0 ) 
			|| ( Cmp_Dte( sed, dst ) >= 0 && Cmp_Dte( sed, ded ) <= 0 );
	}
	else if ( ( !!sst && !sed && !!dst && !!ded ) || ( !!sst && !!sed && !dst && !!ded ) || ( !!sst && !sed && !dst && !!ded ) ){
		ovl = Cmp_Dte( sst, ded ) <= 0;
	}
	else if ( ( !!sst && !!sed && !!dst && !ded ) || ( !sst && !!sed && !!dst && !!ded ) || ( !sst && !!sed && !!dst && !ded ) ) {
		ovl = Cmp_Dte( sed, dst ) >= 0;
	}
	else{
		ovl = TRUE;
	}

	return( ovl );
}

BOOL Viw_Ctl_Btn(			// Form(Dialog) button control
CWnd		*wnd,			// target eindow
MSG			*pMsg)			// windows message on PreTranslateMessaage
{
	int		ret=TRUE;
	DWORD	idc;

	// To prevent that dialog is closed when 2 keys which below pushed
	if( WM_KEYDOWN == pMsg->message ){
		switch( pMsg->wParam ){
		case VK_RETURN:	idc = ::GetWindowLong( wnd->GetFocus()->m_hWnd, GWL_ID );
						ret = idc==IDOK || idc==IDCANCEL;
						break;
		case VK_ESCAPE:	ret=FALSE;
						break;
		default:		break;
		}
	}

	return( ret );
}

//static	WCHAR gen_str[] =	_T("ABCDEFGHIJKLMNOPQRSTUVWXYZ\"#$%\'*+,-./:;=<>()[]{}")
//							_T("?@\\^_`|~!0123456789abcdefghijklmnopqrstuvwxyz");
static	WCHAR gen_str[] =	_T("ABCDEFGHIJKLMNOPQRSTUVWXYZ#$%*=<>()[]{}")
							_T("?@\\^_|~!0123456789abcdefghijklmnopqrstuvwxyz");

void	Viw_Gen_Pwd(		// Generate hash code
WCHAR			*str,		// destination pointer
int				len )		// creating string size
{
	int			idx, gen_idx, gen_len=(int)wcslen(gen_str);

	for ( idx=0; idx<len; idx++, str++ ) {
		gen_idx = rand() % gen_len;
		*str = gen_str[gen_idx];
	}
    *str = '\0';
}


int		Viw_Cre_Hsh(		// creating hash code
int				*dst,		// storeing pointer
WCHAR			*src)		// source string
{
	int			rsl = TRUE, hsh = 0, idx;
	WCHAR		*cur;

	for (cur = src; *cur; cur++) {
		if (!isalnum(*cur) && *cur!='-' && *cur!='_')
			rsl = FALSE;
	}
	if (rsl) {
		for (cur = src, idx = 5; *cur; cur++, idx++) {
			hsh = (hsh + *cur % 0x80 * idx) % 0xffffffff;
		}
		*dst = (int)hsh;
	}
	return (rsl);
}

WCHAR	*Viw_Get_Gdr(						// getting gender name
int			gdr )							// gender code
{
	WCHAR			*nam=NULL;

	switch( gdr ){
	case GDR_MALE:		nam = _T("Male");	break;
	case GDR_FEMALE:	nam = _T("Female");	break;
	case GDR_VOID:		nam = _T("");		break;
	}

	return( nam );
}
