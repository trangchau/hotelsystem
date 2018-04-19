/*
	Functions for inhouse view definition
*/

//#include "windows.h"
#include "..\..\Common\bsc_def.h"
#include "..\..\Common\dbs_mng.h"
#include "..\..\Common\dte_mng.h"
#include "..\..\Custom\SimpleGrid.h"
#include "..\..\PMSCustom\BookingPathCombo.h"
#include "inh_sql.h"
#include "inh_fnc.h"

static	WCHAR	*Grd_ttl[] =	{	{ _T("CS")	},
									{ _T("Name")		},
									{ _T("Status")		},
									{ _T("R.Typ")		},
									{ _T("R.No.")		},
									{ _T("Arrv")		},
									{ _T("Dept")		},
									{ _T("Ad/Ch")		} };

static	GRD_CLM	Grd_clm[] =		{	{  26, GAL_CTR, GCT_CHK,  20 },		// New
									{ 192, GAL_LFT, GCT_NML,  20 },		// Name
									{  96, GAL_CTR, GCT_NML,  20 },		// Status
									{ 144, GAL_LFT, GCT_NML, 256 },		// R.Typ
									{  64, GAL_LFT, GCT_NML,  12 },		// R No.
									{  84, GAL_LFT, GCT_NML,  12 },		// Arrv.
									{  84, GAL_LFT, GCT_NML,  12 },		// Dept.
									{  48, GAL_CTR, GCT_NML,  12 } };	// Ad/Ch


CSimpleGrid		*Inh_Mak_Grd(		// 予約明細グリッド作成
int			id,						// id of grid control
RECT		*rct,					// Rect of grid
CWnd		*wnd )					// 表示先ウィンドウ
{
	CSimpleGrid		*grd;
	GRD_DEF			gdf[1];
	int				idx;

	memset( gdf, 0, sizeof(gdf) );

	gdf->lnh = 20;
	gdf->vlc = 10;
	gdf->vcc = memcnt(Grd_clm);
	gdf->hbc = RGB(128,255,255);
	gdf->hfc = RGB(0,0,0);
	gdf->dbc = RGB(255,255,255);
	gdf->dfc = RGB(0,0,0);
	gdf->sbc = RGB(0,0,0);
	gdf->sfc = RGB(255,255,255);
	gdf->cct = gdf->vcc;
	gdf->clm = Grd_clm;
	gdf->sel = GSL_ROW;
	gdf->hdr = TRUE;
	gdf->fsa = TRUE;

	grd = new CSimpleGrid();
	grd->Create( gdf, *rct, id, wnd );
	for ( idx=0; idx<(int)memcnt(Grd_ttl); idx++ ){
		grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}

	return( grd );
}

void		Inh_Dtl_Dsp(						// 予約詳細表示
CWnd		*wnd,								// 表示先ウィンドウ
INH_DTL_DAT	*dtl,								// 予約詳細データ
INH_INF_DAT	*rsv,								// NULL when new creation
PMS_MNG		*mng )								// Property management System management structure
{
}

void		Inh_Dsp_Dat(						// 予約情報表示
CWnd		*wnd,								// 表示先ウィンドウ
CSimpleGrid	*grd,								// Grid pointer of displaying reservation detail
INH_INF_DAT	*cin,								// NULL when new creation
Csr			rot,								// 予約詳細婚
PMS_MNG		*mng )								// Property management System management structure
{
	WCHAR				buf[520];
	INH_DTL_DAT			*dtl;	
	Csr					csr=NULL;
	int					idx=0, rdi=0;

	cin->est = Viw_Inf_Sts( cin->rsv, mng );

	wnd->GetDlgItem(IDC_STATIC_BOOKINGID)->SetWindowText( EDT_DBF_INT(buf, memcnt(buf), cin->rsv->rvn, Edt_Num_Nml) );
	wnd->GetDlgItem(IDC_STATIC_EXTBOOKINGID)->SetWindowText( EDT_DBF_INT(buf, memcnt(buf), cin->rsv->ern, Edt_Num_Nml) );
	wnd->GetDlgItem(IDC_STATIC_CLIENTTYPE)->SetWindowText( DBS_INT_VAL(cin->ckd)==CLK_PERSONAL ? _T("Personal"): _T("Group") );
	wnd->GetDlgItem(IDC_STATIC_BOOKINGPATH)->SetWindowText( Viw_Bkp_Nam( DBS_INT_VAL(cin->rsv->bfr), DBS_INT_VAL(cin->rsv->aii), mng ) ); 
	wnd->GetDlgItem(IDC_STATIC_CLIENTRANK)->SetWindowText( Viw_Crk_Nam( DBS_INT_VAL(cin->crk), mng ) ); 
	wnd->GetDlgItem(IDC_STATIC_FIRSTARRIVAL)->SetWindowText( EDT_DBF_DAY(buf, memcnt(buf), cin->rsv->arv) );
	wnd->GetDlgItem(IDC_STATIC_LASTDEPARTURE)->SetWindowText( EDT_DBF_DAY(buf, memcnt(buf), cin->rsv->dpt) );
	swprintf_s( buf, memcnt(buf), _T("%s %s"), EDT_DBF_WST(cin->fnm, _T("") ), EDT_DBF_WST(cin->lnm, _T("") ) );
	wnd->GetDlgItem(IDC_STATIC_BOOKINGNAME)->SetWindowText( buf );
	wnd->GetDlgItem(IDC_STATIC_TELEPHONE1)->SetWindowText( EDT_DBF_WST(cin->tel1,_T("")) );
	wnd->GetDlgItem(IDC_STATIC_TELEPHONE2)->SetWindowText( EDT_DBF_WST(cin->tel2,_T("")) );

	((CCustomStatic *)(wnd->GetDlgItem(IDC_STATIC_CLIENTTYPE)))->SetBGColor( DBS_INT_VAL(cin->ckd)==CLK_PERSONAL ? STC_PSB: STC_GPB );
	((CCustomStatic *)(wnd->GetDlgItem(IDC_STATIC_CLIENTTYPE)))->SetFTColor( DBS_INT_VAL(cin->ckd)==CLK_PERSONAL ? STC_PSF: STC_GPF );

	grd->ResetContent();

	if ( rot ){
		grd->AddRow( Cnt_Lnk(rot) );
		while( csr = Nxt_Lnk( rot, csr ) ){
			dtl = Rec_Of(INH_DTL_DAT, csr);
			Inh_Dtl_Dsp( wnd, dtl, cin, mng );
			swprintf_s( buf, memcnt(buf), _T("%s %s"), EDT_DBF_WST(dtl->fnm,_T("")), EDT_DBF_WST(dtl->lnm,_T("")) );
			grd->SetCellText( 1, idx+1, buf );		// Name
			// Status
			grd->SetCellText( 3, idx+1, EDT_DBF_WST(dtl->tnm,_T("")) );							// R.Typ
			grd->SetCellText( 4, idx+1, EDT_DBF_WST(dtl->rno,_T("")) );							// R.No.
			grd->SetCellText( 5, idx+1, EDT_DBF_DAY(buf, memcnt(buf), dtl->dtl->arv) );		// Arrv
			grd->SetCellText( 6, idx+1, EDT_DBF_DAY(buf, memcnt(buf), dtl->dtl->dpt) );		// Dept
			swprintf_s( buf, memcnt(buf), _T("%d/%d"), DBS_INT_VAL(dtl->dtl->psn), DBS_INT_VAL(dtl->dtl->chn) );
			grd->SetCellText( 7, idx+1, buf );												//Ad/Ch

			grd->SetData( 0, idx+1, idx );
			idx++;
		}
		grd->SetSelectCell( 0, 1 );
	}
	else{
		grd->AddRow( 1 );
		grd->SetSelectCell( 0, 1 );
		grd->SetCellText( 0, 1, _T("*") );
		//Rsv_Dtl_Dsp( wnd, NULL, NULL, mng );
	}
}
