/*
	予約検索処理関数
*/

//#include "windows.h"
#include "..\..\Common\bsc_def.h"
#include "..\..\Common\lnk_mng.h"
#include "..\..\pms_def.h"
#include "sch_fnc.h"
#include "sch_sql.h"
#include "viw_cmn.h"
#include "..\..\Custom\SimpleGrid.h"
#include "..\..\Common\dbs_mng.h"
#include "..\HotelPMS.h"
#include "..\MainFrm.h"
#include "..\ChildFrm.h"
#include "..\ReservationView.h"
#include "..\CheckinView.h"
#include "..\InhouseView.h"
#include "..\CheckoutView.h"
#include "..\SearchDoc.h"
#include "..\SearchView.h"
#include "..\AddDepositDlg.h"

#define	GKD_RSV			1			// 予約検索結果グリッド
#define	GKD_ROM			2			// 部屋検索結果グリッド

static	WCHAR	*Tab_ttl[] =	{	{ _T("Reservation")	},
									{ _T("CheckIn")		},
									{ _T("InHouse")		},
									{ _T("CheckOut")	} };

static	WCHAR	*Rsv_txt[] =	{	{ _T("RSV ID")		},
									{ _T("Ext.RsvID")	},
									{ _T("Kind")		},
									{ _T("Rsv.Name")	},
									{ _T("Status")		},
									{ _T("RoomNo")		},
									{ _T("Psn.Name")	},
									{ _T("Arrival")		},
									{ _T("Departure")	},
									{ _T("Amount")		} };

static	GRD_CLM	Rsv_clm[] =		{	{  80, GAL_LFT, GCT_NML,  20 },
									{  80, GAL_LFT, GCT_NML,  20 },
									{  40, GAL_CTR, GCT_NML,  20 },
									{ 192, GAL_LFT, GCT_NML, 256 },
									{  96, GAL_LFT, GCT_NML,  12 },
									{  96, GAL_LFT, GCT_NML,  12 },
									{ 192, GAL_LFT, GCT_NML, 256 },
									{  90, GAL_CTR, GCT_NML,  12 },
									{  90, GAL_CTR, GCT_NML,  12 }, 
									{ 112, GAL_RGT, GCT_NML,  12 } }; 

static	WCHAR	*Rom_txt[] =	{	{ _T("RoomNo")		},
									{ _T("Psn.Name")	},
									{ _T("RSV ID")		},
									{ _T("Ext.RsvID")	},
									{ _T("Rsv.Name")	},
									{ _T("Arrival")		},
									{ _T("Departure")	},
									{ _T("Amount")		} };

static	GRD_CLM	Rom_clm[] =		{	{  96, GAL_LFT, GCT_NML,  20 },
									{ 192, GAL_LFT, GCT_NML, 256 },
									{  80, GAL_LFT, GCT_NML,  20 },
									{  80, GAL_LFT, GCT_NML,  20 },
									{ 192, GAL_LFT, GCT_NML, 256 },
									{  90, GAL_CTR, GCT_NML,  12 },
									{  90, GAL_CTR, GCT_NML,  12 }, 
									{ 112, GAL_RGT, GCT_NML,  12 } }; 
// 各タブのコンポーネント
static	DWORD	rsv_ids[] =		{	IDC_STATIC_RSV_ID,
									IDC_STATIC_RSV_NAME,
									IDC_STATIC_RSV_ARRIVALFR,
									IDC_STATIC_RSV_ARRIVALTO,      
									IDC_STATIC_RSV_DEPARTUREFR,    
									IDC_STATIC_RSV_DEPARTURETO,    
									IDC_STATIC_RSV_BOOKINGFR,
									IDC_EDIT_RSV_ID,
									IDC_EDIT_RSV_NAME,
									IDC_EDIT_RSV_ARRIVALFR,
									IDC_EDIT_RSV_ARRIVALTO,
									IDC_EDIT_RSV_DEPARTUREFR,
									IDC_EDIT_RSV_DEPARTURETO,
									IDC_COMBO_RSV_BOOKIGFROM,
									IDC_CHECK_RSV_RSV,
									IDC_CHECK_RSV_CXL,
									IDC_CHECK_RSV_NSW };

static	DWORD	cin_ids[] =		{	IDC_STATIC_CIN_ROOMNO,
									IDC_STATIC_CIN_ID,
									IDC_STATIC_CIN_NAME,
									IDC_STATIC_CIN_BOOKINGFR,
									IDC_EDIT_CIN_ROOMNO,
									IDC_EDIT_CIN_ID,
									IDC_EDIT_CIN_NAME,
									IDC_COMBO_CIN_BOOKIGFROM };

static	DWORD	inh_ids[] =		{	IDC_STATIC_INH_ID,        
									IDC_STATIC_INH_NAME,
									IDC_STATIC_INH_ARRIVALFR,
									IDC_STATIC_INH_ARRIVALTO,
									IDC_STATIC_INH_DEPARTUREFR,
									IDC_STATIC_INH_DEPARTURETO,
									IDC_STATIC_INH_BOOKINGFR,
									IDC_EDIT_INH_ID,
									IDC_EDIT_INH_NAME,
									IDC_EDIT_INH_ARRIVALFR,
									IDC_EDIT_INH_ARRIVALTO,
									IDC_EDIT_INH_DEPARTUREFR,
									IDC_EDIT_INH_DEPARTURETO,
									IDC_COMBO_INH_BOOKIGFROM };

static	DWORD	cot_ids[] =		{	IDC_STATIC_COT_ROOMNO,
									IDC_STATIC_COT_ID,
									IDC_STATIC_COT_NAME,
									IDC_STATIC_COT_BOOKINGFR,
									IDC_EDIT_COT_ROOMNO,
									IDC_EDIT_COT_ID,
									IDC_EDIT_COT_NAME,
									IDC_COMBO_COT_BOOKIGFROM };

typedef	struct	CTLDEF	{			// グリッド作成定義
	CSimpleGrid	**grd;				// 格納領域
	int			shi;				// 検索種別インデックス
	int			knd;				// グリッド種別
	DWORD		id;					// グリッドID
	DWORD		par;				// 親ウィンドウ
	int			idc;				// 関連コンポーネントID数
	DWORD		*ids;				// 関連コンポーネントID
}CTL_DEF;

CTL_DEF	_Grd[]	=	{	{ NULL, SHI_RSV, GKD_RSV, IDC_GRD_RSV, IDC_STATIC_RSVPANEL, memcnt(rsv_ids), rsv_ids },
						{ NULL, SHI_CIN, GKD_RSV, IDC_GRD_CIN, IDC_STATIC_CINPANEL, memcnt(cin_ids), cin_ids },
						{ NULL, SHI_INH, GKD_ROM, IDC_GRD_INH, IDC_STATIC_INHPANEL, memcnt(inh_ids), inh_ids },
						{ NULL, SHI_COT, GKD_ROM, IDC_GRD_COT, IDC_STATIC_COTPANEL, memcnt(cot_ids), cot_ids } };

#define	SCH_DEF_BDT				1			// default is business date

static	int	get_shi(				// Getting search index from grid id
int				gid )				// Grid id
{
	int			shi = -1;

	switch( gid ){
	case IDC_GRD_RSV:	shi = SHI_RSV;	break;
	case IDC_GRD_CIN:	shi = SHI_CIN;	break;
	case IDC_GRD_INH:	shi = SHI_INH;	break;
	case IDC_GRD_COT:	shi = SHI_COT;	break;
	}

	return( shi );
}

static	int	vwk_shi(				// Getting search index from view kind
int				gid )				// Grid id
{
	int			shi = -1;

	switch( gid ){
	case VWK_RSV:	shi = SHI_RSV;	break;
	case VWK_CIN:	shi = SHI_CIN;	break;
	case VWK_INH:	shi = SHI_INH;	break;
	case VWK_COT:	shi = SHI_COT;	break;
	}

	return( shi );
}

static	CTL_DEF	*get_cdf(			// コントロール定義テーブルの取得
int			shi )					// 検索対象
{
	CTL_DEF		*ptr=NULL;
	int			idx;

	for ( idx=0; idx<memcnt(_Grd); idx++ ){
		if ( _Grd[idx].shi == shi ){
			ptr = _Grd+idx;
			break;
		}
	}

	return( ptr );
}

// グリッド作成
static	CSimpleGrid	*mak_grd(		// グリッド作成
int		knd,						// グリッド種別
DWORD	id,							// グリッドID
CWnd	*par,						// 親ウィンドウ
RECT	&rct )						// 作成位置
{
	CSimpleGrid		*grd;
	GRD_DEF			gdf[1];
	int				idx;

	memset( gdf, 0, sizeof(gdf) );

	gdf->lnh = 20;
	gdf->vlc = 20;
	gdf->vcc = knd==GKD_RSV ? memcnt(Rsv_clm): memcnt(Rom_clm);
	gdf->hbc = RGB(128,255,255);
	gdf->hfc = RGB(0,0,0);
	gdf->dbc = RGB(255,255,255);
	gdf->dfc = RGB(0,0,0);
	gdf->sbc = RGB(0,0,0);
	gdf->sfc = RGB(255,255,255);
	gdf->cct = gdf->vcc;
	gdf->clm = knd==GKD_RSV ? Rsv_clm: Rom_clm;
	gdf->sel = GSL_ROW;
	gdf->hdr = TRUE;
	gdf->fsa = TRUE;

	grd = new CSimpleGrid();
	grd->Create( gdf, rct, id, par );
	for ( idx=0; idx<(int)(knd==GKD_RSV ? memcnt(Rsv_txt): memcnt(Rom_txt)); idx++ ){
		grd->SetCellText( idx, 0, knd==GKD_RSV ? Rsv_txt[idx]: Rom_txt[idx] );
	}

	return( grd );
}

typedef	struct	{
	DWORD		cid;
	int			def;
}_Def;

static	void	set_def(			// setting default search option on individual tab
int			shi,					// search kind
CWnd		*wnd,					// parent window
//CDatabase	*dbs,					// ODBC Database object
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int				chk, idx, cnt;
	CTL_DEF			*ctl = get_cdf( shi );
	_Def			*def;
	WCHAR			buf[50];
	CSearchView		*viw = (CSearchView *)wnd;

	_Def	Def_rsv[] = {
					{ IDC_EDIT_RSV_ID,			0 },
					{ IDC_EDIT_RSV_NAME,		0 },
					{ IDC_EDIT_RSV_ARRIVALFR,	SCH_DEF_BDT },
					{ IDC_EDIT_RSV_ARRIVALTO,	0 },
					{ IDC_EDIT_RSV_DEPARTUREFR,	0 },
					{ IDC_EDIT_RSV_DEPARTURETO,	0 },
					{ IDC_COMBO_RSV_BOOKIGFROM,	0 } };
	_Def	Def_cin[] = {
					{ IDC_EDIT_CIN_ROOMNO,		0 },
					{ IDC_EDIT_CIN_ID,			0 },
					{ IDC_EDIT_CIN_NAME,		0 },
					{ IDC_COMBO_CIN_BOOKIGFROM,	0 } };
	_Def	Def_inh[] = {
					{ IDC_EDIT_INH_ID,			0 },
					{ IDC_EDIT_INH_NAME,		0 },
					{ IDC_EDIT_INH_ARRIVALFR,	0 },
					{ IDC_EDIT_INH_ARRIVALTO,	0 },
					{ IDC_EDIT_INH_DEPARTUREFR,	0 },
					{ IDC_EDIT_INH_DEPARTURETO,	0 },
					{ IDC_COMBO_INH_BOOKIGFROM,	0 } };
	_Def	Def_cot[] =	{
					{ IDC_EDIT_COT_ROOMNO,		0 },
					{ IDC_EDIT_COT_ID,			0 },
					{ IDC_EDIT_COT_NAME,		0 },
					{ IDC_COMBO_COT_BOOKIGFROM,	0 } };

	switch( shi ){
	case	SHI_RSV:	def = Def_rsv;	cnt=memcnt(Def_rsv);	break;
	case	SHI_CIN:	def = Def_cin;	cnt=memcnt(Def_cin);	break;
	case	SHI_INH:	def = Def_inh;	cnt=memcnt(Def_inh);	break;
	case	SHI_COT:	def = Def_cot;	cnt=memcnt(Def_cot);	break;
	}
	if ( def ){
		for ( idx=0, chk=TRUE; idx<cnt; idx++ ){
			*buf = 0;
			if ( (def+idx)->def==SCH_DEF_BDT ){
				Edt_Dte( buf, memcnt(buf), mng->bdt );
			}
			viw->GetDlgItem(ctl->par)->GetDlgItem((def+idx)->cid)->SetWindowText( buf );
		}
	}
	if ( shi==SHI_RSV ){
		((CButton *)(viw->GetDlgItem(ctl->par)->GetDlgItem(IDC_CHECK_RSV_RSV)))->SetCheck( TRUE );
		((CButton *)(viw->GetDlgItem(ctl->par)->GetDlgItem(IDC_CHECK_RSV_CXL)))->SetCheck( FALSE );
		((CButton *)(viw->GetDlgItem(ctl->par)->GetDlgItem(IDC_CHECK_RSV_NSW)))->SetCheck( FALSE );
	}
}

// 各検索種別タブ用パネル作成
void	Mak_Sch_Pnl(
CWnd		*wnd,
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int				idx, idy;
	WINDOWPLACEMENT	wpc[1];
	RECT			rct[1];
	CSearchView		*dlg = (CSearchView *)wnd;
	CSimpleGrid		**grd[] = { &dlg->m_Rsv_grd, &dlg->m_Cin_grd, &dlg->m_Inh_grd, &dlg->m_Cot_grd };

	// タブの初期化
	for ( idx=0; idx<memcnt(Tab_ttl); idx++ ){
	    dlg->m_Knd_tab.InsertItem(idx, Tab_ttl[idx] );
	}

	// グリッドの生成
	for ( idx=0; idx<memcnt(_Grd); idx++ ){
		_Grd[idx].grd = grd[idx];
		dlg->GetDlgItem(_Grd[idx].par)->GetClientRect(rct);
		rct->left += 8;
		rct->top += 46;
		*_Grd[idx].grd = mak_grd( _Grd[idx].knd, _Grd[idx].id, dlg->GetDlgItem(_Grd[idx].par), rct[0] );
		// 関連コンポーネントの親ウィンドウ書き換え
		for ( idy=0; idy<_Grd[idx].idc; idy++ ){
			dlg->GetDlgItem(_Grd[idx].ids[idy] )->SetParent( dlg->GetDlgItem( _Grd[idx].par ) );
		}
		set_def( _Grd[idx].shi, wnd, mng );
	}

	dlg->GetDlgItem(IDC_TAB_KIND)->GetClientRect(rct);

	for ( idx=0; idx<memcnt(_Grd); idx++ ){
		dlg->GetDlgItem((_Grd+idx)->par)->GetWindowPlacement( wpc );
		wpc->rcNormalPosition.right = wpc->rcNormalPosition.left + rct->right - 4;
		dlg->GetDlgItem((_Grd+idx)->par)->SetWindowPlacement( wpc );
	}
	//GetDlgItem(IDC_STATIC_GRIDPANEL)->SetParent(&m_Knd_tab);
}


static	int	get_bfr(		// CBookingPathCombコントロールからIDデータ取得
void		*dst,			// 格納領域
int			siz,			// 格納領域サイズ
CWnd		*wnd,			// 取得元ビュー
DWORD		cid )			// コントロールID
{
	int					pth, agi, nul;
	CBookingPathCombo	*cbx = (CBookingPathCombo *)wnd->GetDlgItem(cid);

	cbx->GetItemIndex( &pth, &agi );

	if ( nul = ( pth == -1 ) ){
		Set_Nul(dst, FLD_INT, TRUE);
		Set_Nul((int *)dst+DBF_SIZ_INT, FLD_INT, TRUE);
	}
	else{
		DBS_INT_SET(dst, &pth);
		DBS_INT_SET((int *)dst+DBF_SIZ_INT, &agi);
	}

	return( nul );
}

typedef	struct	{					// 
		void	*ptr;
		int		siz;
		DWORD	cid;									// inputtable control id
		int		(*fnc)( void *, int, CWnd *, DWORD );	// Process function
		DWORD	sid;									// static control id
		int		def;									// default kind
}_Get;

static	int		get_opt(			// Getting search options from screen
SCH_OPT		*dst,					// Pointer of storing destination
WCHAR		**enm,					// Error message storeing pointer
int			shi,					// Kind of tab
CSearchView	*viw )					// Parent window
{
	int			chk=FALSE, idx, cnt;
	CWnd		*par=NULL;
	CTL_DEF		*ptr;
	_Get		*get=NULL;


	_Get	Get_rsv[] = {
					{ dst->bid,  0,						IDC_EDIT_RSV_ID,			Get_Int_Dat, IDC_STATIC_RSV_ID,			0 },
					{ dst->nam,  FLD_WST_SIZ(dst->nam),	IDC_EDIT_RSV_NAME,			Get_Str_Dat, IDC_STATIC_RSV_NAME,		0 },
					{ dst->arvf, 0,						IDC_EDIT_RSV_ARRIVALFR,		Get_Day_Dat, IDC_STATIC_RSV_ARRIVALFR,	SCH_DEF_BDT },
					{ dst->arvt, 0,						IDC_EDIT_RSV_ARRIVALTO,		Get_Day_Dat, IDC_STATIC_RSV_ARRIVALTO,	0 },
					{ dst->dptf, 0,						IDC_EDIT_RSV_DEPARTUREFR,	Get_Day_Dat, IDC_STATIC_RSV_DEPARTUREFR,0 },
					{ dst->dptt, 0,						IDC_EDIT_RSV_DEPARTURETO,	Get_Day_Dat, IDC_STATIC_RSV_DEPARTURETO,0 },
					{ dst->bfr,  0,						IDC_COMBO_RSV_BOOKIGFROM,	get_bfr,     IDC_STATIC_RSV_BOOKINGFR,	0 } };
	_Get	Get_cin[] = {
					{ dst->rmn,  FLD_WST_SIZ(dst->rmn),	IDC_EDIT_CIN_ROOMNO,		Get_Str_Dat, IDC_STATIC_CIN_ROOMNO,		0 },
					{ dst->bid,  0,						IDC_EDIT_CIN_ID,			Get_Int_Dat, IDC_STATIC_INH_ID,			0 },
					{ dst->nam,  FLD_WST_SIZ(dst->nam),	IDC_EDIT_CIN_NAME,			Get_Str_Dat, IDC_STATIC_CIN_NAME,		0 },
					{ dst->bfr,  0,						IDC_COMBO_CIN_BOOKIGFROM,	get_bfr,     IDC_STATIC_CIN_BOOKINGFR,	0 } };
	_Get	Get_inh[] = {
					{ dst->bid,  0,						IDC_EDIT_INH_ID,			Get_Int_Dat, IDC_STATIC_INH_ID,			0 },
					{ dst->nam,  FLD_WST_SIZ(dst->nam),	IDC_EDIT_INH_NAME,			Get_Str_Dat, IDC_STATIC_INH_NAME,		0 },
					{ dst->arvf, 0,						IDC_EDIT_INH_ARRIVALFR,		Get_Day_Dat, IDC_STATIC_INH_ARRIVALFR,	0 },
					{ dst->arvt, 0,						IDC_EDIT_INH_ARRIVALTO,		Get_Day_Dat, IDC_STATIC_INH_ARRIVALTO,	0 },
					{ dst->dptf, 0,						IDC_EDIT_INH_DEPARTUREFR,	Get_Day_Dat, IDC_STATIC_INH_DEPARTUREFR,0 },
					{ dst->dptt, 0,						IDC_EDIT_INH_DEPARTURETO,	Get_Day_Dat, IDC_STATIC_INH_DEPARTURETO,0 },
					{ dst->bfr,  0,						IDC_COMBO_INH_BOOKIGFROM,	get_bfr,     IDC_STATIC_INH_BOOKINGFR,	0 } };
	_Get	Get_cot[] =	{
					{ dst->rmn,  FLD_WST_SIZ(dst->rmn),	IDC_EDIT_COT_ROOMNO,		Get_Str_Dat, IDC_STATIC_COT_ROOMNO,		0 },
					{ dst->bid,  0,						IDC_EDIT_COT_ID,			Get_Int_Dat, IDC_STATIC_COT_ID,			0 },
					{ dst->nam,  FLD_WST_SIZ(dst->nam),	IDC_EDIT_COT_NAME,			Get_Str_Dat, IDC_STATIC_COT_NAME,		0 },
					{ dst->bfr,  0,						IDC_COMBO_COT_BOOKIGFROM,	get_bfr,     IDC_STATIC_COT_BOOKINGFR,	0 } };

	if ( chk = !!( ptr = get_cdf( shi ) ) ){
		par = viw->GetDlgItem(ptr->par);
		if ( chk = !!par ){
			switch( shi ){
			case	SHI_RSV:	get = Get_rsv;	cnt=memcnt(Get_rsv);	break;
			case	SHI_CIN:	get = Get_cin;	cnt=memcnt(Get_cin);	break;
			case	SHI_INH:	get = Get_inh;	cnt=memcnt(Get_inh);	break;
			case	SHI_COT:	get = Get_cot;	cnt=memcnt(Get_cot);	break;
			}
			if ( get ){
				for ( idx=0, chk=TRUE; idx<cnt; idx++ ){
					chk &= (get+idx)->fnc( (get+idx)->ptr, (get+idx)->siz, par, (get+idx)->cid ) != -1;
					if ( !chk ){
						if ( *enm = (WCHAR *)malloc( wsizeof(80) ) ){
							viw->GetDlgItem(ptr->par)->GetDlgItem((get+idx)->sid)->GetWindowText( *enm, 80 );
						}
						break;
					}
				}
			}
		}
	}
	return( chk );
}

void	Sch_Tab_Chg(
int			tix,
CWnd		*wnd )
{
	int				idx;
	CSearchView		*dlg = (CSearchView *)wnd;

	for ( idx=0; idx<memcnt(_Grd); idx++ ){
		dlg->GetDlgItem(_Grd[idx].par)->ShowWindow(idx==tix ? SW_NORMAL: SW_HIDE);
	}
}

static	int		is_dsp(				// filtering display data
int				est,				// target data
CSearchView		*viw,				// search view
DWORD			pid )				// parent window control id
{
	int			dsp=TRUE;
	int			rsv = ((CButton *)viw->GetDlgItem(pid)->GetDlgItem(IDC_CHECK_RSV_RSV))->GetCheck();
	int			cxl = ((CButton *)viw->GetDlgItem(pid)->GetDlgItem(IDC_CHECK_RSV_CXL))->GetCheck();
	int			nsw = ((CButton *)viw->GetDlgItem(pid)->GetDlgItem(IDC_CHECK_RSV_NSW))->GetCheck();

	if ( ( est & RSV_EST_CXL ) )		dsp = !!cxl;
	else if ( ( est & RSV_EST_NSW ) )	dsp = !!nsw;
	else								dsp = !!rsv;

	return( dsp );
}

static	void	rsv_lst(			// Diaplaying reservation list 
CSimpleGrid		*grd,				// Grid pointer for display
Csr				rot,				// Root of searched data
int				shi,				// Target of search
CWnd			*wnd,				// Parent view
PMS_MNG			*mng )				// Hotel Property Management System Structure
{
	WCHAR		buf[128];
	int			row=1, dct=0, rvn=0, rwc, sts;
	Csr			csr=NULL;
	SCH_RSV_LST	*rsv;
	CSearchView	*viw = (CSearchView *)wnd;
	CTL_DEF		*ctl = get_cdf( shi );

	
	if ( shi==SHI_RSV ){
		while( csr=Nxt_Lnk( rot, csr ) ){
			rsv = Rec_Of(SCH_RSV_LST, csr);
			sts = Viw_Dtl_Sts( rsv->rsv, rsv->dpt, rsv->cid, rsv->cod, rsv->cxl, mng );
			if ( is_dsp( sts, viw, ctl->par ) )	dct++;
		}
	}
	else{
		dct = Cnt_Lnk( rot );
	}

	grd->ResetContent();
	grd->AddRow(dct);


//	rwc = grd->GetLineCount( FALSE );
//	if ( dct > rwc )	grd->AddRow( dct - rwc );
//	else				while( rwc > dct )	grd->DelRow( rwc-- );

	//if  ( !!( dct = Cnt_Lnk( rot ) ))	grd->AddRow(dct);
	while( csr= Nxt_Lnk(rot, csr) ){
		rsv = Rec_Of(SCH_RSV_LST, csr);
		sts = Viw_Dtl_Sts( rsv->rsv, rsv->dpt, rsv->cid, rsv->cod, rsv->cxl, mng );

		if ( shi!=SHI_RSV || is_dsp( sts, viw, ctl->par ) ){
			if ( rvn!=DBS_INT_VAL(rsv->rsv->rvn) ){
				swprintf( buf, memcnt(buf), _T("%d"), DBS_INT_VAL(rsv->rsv->rvn) );
				grd->SetCellText( 0, row, buf );
				grd->SetCellText( 1, row, EDT_DBF_WST(rsv->rsv->ern,_T("")) );
				if ( !Nul_Fld(rsv->ckd, FLD_INT) ){
					wcscpy_s( buf, memcnt(buf),
						DBS_INT_VAL(rsv->ckd)==CLK_PERSONAL ? _T("Psn"): _T("Grp") );
				}
				grd->SetCellText( 2, row, buf );
				swprintf_s( buf, memcnt(buf), _T("%s %s"), EDT_DBF_WST(rsv->fnm,_T("")), EDT_DBF_WST(rsv->lnm,_T("")) );
				grd->SetCellText( 3, row, buf );
				rvn=DBS_INT_VAL(rsv->rsv->rvn);
			}
			else{
				grd->SetCellText( 0, row, _T("") );
				grd->SetCellText( 1, row, _T("") );
				grd->SetCellText( 2, row, _T("") );
				grd->SetCellText( 3, row, _T("") );
			}
			grd->SetCellText( 4, row, Viw_Sts_Ope( NULL, sts ) );
			grd->SetCellText( 5, row, EDT_DBF_WST(rsv->rno,_T("-")) );
			if ( Nul_Fld(rsv->dci, FLD_INT) ){
				wcscpy_s( buf, memcnt(buf), _T("--<No inputted yet>--") );
			}
			else{
				swprintf_s( buf, memcnt(buf), _T("%s %s"), EDT_DBF_WST(rsv->dfn,_T("")), EDT_DBF_WST(rsv->dln,_T("")) );
			}
			grd->SetCellText( 6, row, buf );
			EDT_DBF_DAY(buf, memcnt(buf), rsv->rsv->arv );
			grd->SetCellText( 7, row, buf );
			EDT_DBF_DAY(buf, memcnt(buf), rsv->rsv->dpt );
			grd->SetCellText( 8, row, buf );
			// Keep reservation_inf_id to cell
			grd->SetData( 0, row, DBS_INT_VAL(rsv->rsv->id) );
			grd->SetData( 1, row, DBS_INT_VAL(rsv->rdi) );
			grd->SetData( 2, row, (int)(LONG_PTR)rsv );
			row++;
		}
	}
	grd->Redraw();
}

static	void	inh_lst(			// Displaying inhouse/checkouttoday list
CSimpleGrid		*grd,				// Grid pointer for display
Csr				rot,				// Root of searched data
int				shi,				// Target of search
CWnd			*wnd,				// Parent view
PMS_MNG			*mng )				// Hotel Property Management System Structure
{
	WCHAR		buf[128];
	int			row=1, dct, rvn=0, rwc;
	Csr			csr=NULL;
	SCH_RSV_LST	*rsv;
	CSearchView	*viw = (CSearchView *)wnd;

	dct = Cnt_Lnk( rot );
	rwc = grd->GetLineCount( FALSE );
	if ( dct > rwc )	grd->AddRow( dct - rwc );
	else				while( rwc > dct )	grd->DelRow( rwc-- );

//	grd->ResetContent();
//	if  ( !!( dct = Cnt_Lnk( rot ) ))	grd->AddRow(dct);
	while( csr= Nxt_Lnk(rot, csr) ){
		rsv = Rec_Of(SCH_RSV_LST, csr);
		// RoomNo	
		grd->SetCellText( 0, row, Nul_Fld(rsv->rno, FLD_WST) ?  _T("-"): (WCHAR *)Fld_Ptr(rsv->rno, FLD_WST) );
		// Psn.Name
		swprintf_s( buf, memcnt(buf), _T("%s %s"), EDT_DBF_WST(rsv->dfn,_T("")), EDT_DBF_WST(rsv->dln,_T("")) );
		grd->SetCellText( 1, row, buf );

		if ( rvn!=DBS_INT_VAL(rsv->rsv->rvn) ){
			// RSV ID	
			swprintf_s( buf, memcnt(buf), _T("%d"), DBS_INT_VAL(rsv->rsv->rvn) );
			grd->SetCellText( 2, row, buf );
			// Ext.RSV ID	
			grd->SetCellText( 3, row, EDT_DBF_WST(rsv->rsv->ern,_T("")) );
			// Rsv.Name
			swprintf_s( buf, memcnt(buf), _T("%s %s"), EDT_DBF_WST(rsv->fnm,_T("")), EDT_DBF_WST(rsv->lnm,_T("")) );
			grd->SetCellText( 4, row, buf );
			rvn = DBS_INT_VAL(rsv->rsv->rvn);
		}
		else{
			grd->SetCellText( 2, row, _T("") );
			grd->SetCellText( 3, row, _T("") );
			grd->SetCellText( 4, row, _T("") );
		}
		// Arrival
		grd->SetCellText( 5, row, EDT_DBF_DAY(buf, memcnt(buf), rsv->arv ) );
		// Departure
		grd->SetCellText( 6, row, EDT_DBF_DAY(buf, memcnt(buf), rsv->dpt ) );
		grd->SetData( 0, row, DBS_INT_VAL(rsv->rsv->id) );
		grd->SetData( 1, row, DBS_INT_VAL(rsv->rdi) );
		grd->SetData( 2, row, (int)(LONG_PTR)rsv );
		row++;
	}
	grd->Redraw();
}
						
int		Sch_Sch_Exc(				// 各一覧取得実行
Csr			rot,					// Root of data
int			shi,					// 検索対象
CWnd		*wnd,
PMS_MNG		*mng )					// Hotel Property Management System Structure
{
	int			chk;
	CTL_DEF		*ptr;
	SCH_OPT		opt[1];
	WCHAR		*msg=NULL, buf[128];
	CSearchView	*viw = (CSearchView *)wnd;

	Del_All( rot );
	
	memset( opt, 0, sizeof(opt) );

	if ( chk = get_opt( opt, &msg, shi, viw ) ){
		if ( chk = Sch_Rsv_Lst( rot, 0, shi, opt, &msg, viw->m_pDbs_obj, viw->m_pPms_mng ) ){
			if ( chk = !!( ptr = get_cdf( shi ) ) ){
				switch( shi ){
				case	SHI_RSV:	rsv_lst( *ptr->grd, rot, shi, viw, mng );	break;
				case	SHI_CIN:	rsv_lst( *ptr->grd, rot, shi, viw, mng );	break;
				case	SHI_INH:	inh_lst( *ptr->grd, rot, shi, viw, mng );	break;
				case	SHI_COT:	inh_lst( *ptr->grd, rot, shi, viw, mng );	break;
				}
			}
		}
		else{
			if ( msg )	viw->MessageBox( msg, 0, MB_ICONSTOP|MB_OK );
		}
	}
	else{
		if ( msg ){
			swprintf_s( buf, memcnt(buf), _T("Ordering of %s is wrong"), msg );
			viw->MessageBox( buf, 0, MB_ICONSTOP|MB_OK );
		}
	}

	if ( msg )		free( msg );

	return( chk );
}

typedef	struct	{
	int			rii;		// reservation_info_id
	int			rdi;		// reservation_detail_id
	int			csh;		// raunch a casiher
}RPR_;

static	int		exv_cbk(
int		vkd,
CView	*viw,
LPVOID	prm )
{
	return( ((CChildFrame *)(viw->GetParent()))->m_Rsv_id == (int)(LONG_PTR)prm );
}

static	int		rvw_cbk(
int		vkd,
CView	*viw,
LPVOID	prm )
{
	int			chk=FALSE;
	RPR_		*rpr=(RPR_ *)(LONG_PTR)prm;

	switch( vkd ){
	case VWK_RSV:	chk = ((CReservationView *)viw)->GetData( rpr->rii );					break;
	case VWK_CIN:	chk = ((CCheckinView *)viw)->GetData( rpr->rii );						break;
	case VWK_INH:	chk = ((CInhouseView *)viw)->GetData( rpr->rii, rpr->rdi, rpr->csh );	break;
	case VWK_COT:	chk = ((CCheckoutView *)viw)->GetData( rpr->rii, rpr->rdi );			break;
	}

	return( chk );
}

void	Sch_Rnc_Viw(				// Launch each view
int			gid,					// View kind of target view
int			rii,					// reservation_info_id
int			rdi,					// reservation_detail_id
int			csh )					// direct raunch a cashier on inhouse view 
{
		int			vkd = -1;
		RPR_		rpr[1];

		rpr->rii = rii;
		rpr->rdi = rdi;
		rpr->csh = csh;

		switch( gid ){
		case IDC_GRD_RSV:	vkd = VWK_RSV;	break;
		case IDC_GRD_CIN:	vkd = VWK_CIN;	break;
		case IDC_GRD_INH:	vkd = VWK_INH;	break;
		case IDC_GRD_COT:	vkd = VWK_COT;	break;
		}
		if ( vkd != -1 ){
			Rnc_Ech_Viw( vkd, exv_cbk, rvw_cbk, (LPVOID)(LONG_PTR)rpr );
		}
}

Csr		Rsv_Get_Rot(			// Getting data root
int			shi,				// Target of search
CWnd		*wnd )
{
	Csr			rot=NULL;
	CSearchView	*viw = (CSearchView *)wnd;

	switch( shi ){
	case SHI_RSV:	rot = viw->m_Rsv_rot;	break;
	case SHI_CIN:	rot = viw->m_Cin_rot;	break;
	case SHI_INH:	rot = viw->m_Inh_rot;	break;
	case SHI_COT:	rot = viw->m_Cot_rot;	break;
	}

	return( rot );
}

void	Sch_Ctx_Mnu(			// Open context menu on search view
CWnd		*wnd,				// Search view
int			msx,				// Mouse point x
int			msy,				// Mouse point y
int			row,				// row
int			gid )				// Giod ID
{
	CMenu		mnu, *pmn=NULL;
	POINT		pnt[1] = { msx, msy };
	DWORD		mid=0;
	int			shi, rii=0;
	CTL_DEF		*cdf;
	CSimpleGrid	*grd;
	CSearchView	*viw = (CSearchView *)wnd;

	switch( shi = get_shi( gid ) ){
	case SHI_RSV:	mid = 0;				break;
	case SHI_CIN:	mid = IDR_MENU_SCH_CIN;	break;
	case SHI_INH:	mid = IDR_MENU_SCH_INH;	break;
	case SHI_COT:	mid = IDR_MENU_SCH_COT;	break;
	}

	if ( mid && !!( cdf = get_cdf( shi ) ) ){
		grd = (CSimpleGrid *)((wnd->GetDlgItem(cdf->par))->GetDlgItem(gid));
		viw->m_Rdn_rii = grd ? grd->GetData( 0, row ): 0;
		viw->m_Rdn_rdi = grd ? grd->GetData( 1, row ): 0;
		viw->m_Rdn_rsv = grd ? (SCH_RSV_LST *)(LONG_PTR)grd->GetData( 2, row ): NULL;
		mnu.LoadMenu(mid);
		pmn = mnu.GetSubMenu(0);
		// mouse on the panel
		wnd->GetDlgItem(cdf->par)->ClientToScreen( pnt );
		pmn->TrackPopupMenu( TPM_TOPALIGN/*TPM_LEFTALIGN*/|TPM_RIGHTBUTTON, pnt->x, pnt->y, wnd, NULL );
	}
}

void	Sch_Add_Rsv(			// added new reservation notify from ReservationView
int			rii,				// reservation_info_id
int			vwk,				// Target of search
CWnd		*wnd,				// Parent window
CDatabase	*dbs,				// ODBC Database object
PMS_MNG		*mng )				// Hotel Property Management System Structure
{
	Lnk			rot[1], *csr;
	WCHAR		*erm=NULL;
	int			chk;
	int			shi = vwk_shi( vwk );
	CSearchView	*viw = (CSearchView *)wnd;

	memset( rot, 0, sizeof(rot) );
	if ( chk = Sch_Rsv_Lst( rot, rii, shi, NULL, &erm, dbs, mng ) ){
		if ( !!( csr=Nxt_Lnk( rot,  NULL ) ) ){
		}
	}
	Del_All( rot );
}

void	Sch_Upd_Rsv(			// updated a reservation notify from ReservationView
int			rii,				// reservation_info_id
int			vwk,				// Target of search
CWnd		*wnd,				// Parent window
CDatabase	*dbs,				// ODBC Database object
PMS_MNG		*mng )				// Hotel Property Management System Structure
{
	Lnk			rot[1], *csr;
	WCHAR		*erm=NULL;
	int			chk;
	int			shi = vwk_shi( vwk );
	CSearchView	*viw = (CSearchView *)wnd;

	memset( rot, 0, sizeof(rot) );
	if ( chk = Sch_Rsv_Lst( rot, rii, shi, NULL, &erm, dbs, mng ) ){
		if ( !!( csr=Nxt_Lnk( rot,  NULL ) ) ){
		}
	}
	Del_All( rot );
}

int		Sch_Mng_Dps(				// managemnt a deposit
CWnd		*wnd,					// parent window
CDatabase	*dbs,					// ODBC Database object
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int				chk;
	Csr				*csr=NULL;
	CAddDepositDlg	*dlg;
	CSearchView		*viw = (CSearchView *)wnd;

	if ( chk = !!( dlg = new CAddDepositDlg(wnd) ) ){
//		dlg->m_Dps_rot = inh->dps;
		dlg->m_Rsv_iid = DBS_INT_VAL(viw->m_Rdn_rsv->rsv->id);
		dlg->m_Rsv_did = DBS_INT_VAL(viw->m_Rdn_rsv->rdi);
		dlg->m_Eqp_iid = DBS_INT_VAL(viw->m_Rdn_rsv->eii);
		dlg->m_Fst_nam = EDT_DBF_WST(viw->m_Rdn_rsv->dfn,_T(""));
		dlg->m_Lst_nam = EDT_DBF_WST(viw->m_Rdn_rsv->dln,_T(""));
		dlg->m_Rom_nbr = EDT_DBF_WST(viw->m_Rdn_rsv->rno,_T(""));
		dlg->m_Ope_cod = mng->opc;
		dlg->m_Brc_cod = mng->brc;
		dlg->m_Fcl_cod = mng->fcc;
		dlg->m_pDbs_obj = dbs;
		dlg->m_pPms_mng = mng;
		dlg->m_Dir_ect = TRUE;
		if ( dlg->DoModal()==IDOK ){
		}
		delete dlg;
	}

	return( chk );
}

void	Sch_Set_Def(				// setting default search option 
CWnd		*wnd,					// parent window
CDatabase	*dbs,					// ODBC Database object
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int				idx;
	CSearchView		*viw = (CSearchView *)wnd;
	
	if ( ( idx = ((CTabCtrl *)viw->GetDlgItem(IDC_TAB_KIND))->GetCurSel() ) != -1 ){
		set_def( (_Grd+idx)->shi, wnd, mng );
	}
}
