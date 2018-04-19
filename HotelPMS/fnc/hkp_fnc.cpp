/*
	Functions for checkin view definition
*/

//#include "windows.h"
#include "..\..\Common\bsc_def.h"
#include "..\..\Common\dbs_mng.h"
#include "..\..\Common\dte_mng.h"
#include "..\..\Custom\SimpleGrid.h"
#include "..\..\PMSCustom\BookingPathCombo.h"
#include "..\ChildFrm.h"
#include "..\HouseKeepingView.h"
#include "bst_sql.h"
#include "hkp_sql.h"
#include "hkp_fnc.h"

static	WCHAR	*Grd_ttl[] =	{	{ _T("RoomNo")		},
									{ _T("Sale Sts.")	},
									{ _T("Cln.Sts.")	},
									{ _T("Occ.Sts.")	},
									{ _T("GuestName")	},
									{ _T("Use Period")	}};

static	GRD_CLM	Grd_clm[] =		{	{  96, GAL_LFT, GCT_NML,  20 },		// RoomNo.
									{  96, GAL_LFT, GCT_CMB,  20 },		// Sale Status
									{  72, GAL_LFT, GCT_CMB,  20 },		// Clean Status
									{  72, GAL_LFT, GCT_CMB,  20 },		// Occupied Status
									{ 180, GAL_LFT, GCT_NML,  20 },		// GuestName
									{ 176, GAL_LFT, GCT_NML,  20 } };	// GuestName

static	WCHAR	*Rch_ttl[] =	{	{ _T("Sel")			},
									{ _T("Cur.Room")	},
									{ _T("Nxt.Room")	},
									{ _T("GuestName")	},};

static	GRD_CLM	Rch_clm[] =		{	{  32, GAL_LFT, GCT_CHK,  12 },		// Select
									{  96, GAL_LFT, GCT_NML,  12 },		// current room number
									{  96, GAL_LFT, GCT_NML,  12 },		// next room number
									{ 192, GAL_LFT, GCT_NML,  12 }};	// guest name

static	int		Sal_sts[] =		{ RSS_ENABLE, RSS_OUTOFORDER, RSS_OUTOFSERVICE };

CSimpleGrid		*Hkp_Mak_Grd(		// 予約明細グリッド作成
int			id,						// id of grid control
RECT		*rct,					// Rect of grid
CWnd		*wnd )					// 表示先ウィンドウ
{
	CSimpleGrid		*grd;
	GRD_DEF			gdf[1];
	int				idx, cnt= id==IDC_GRD_HKP ? memcnt(Grd_clm): memcnt(Rch_clm);

	memset( gdf, 0, sizeof(gdf) );

	gdf->lnh = 20;
	gdf->vlc = id==IDC_GRD_HKP ? 10: 5;
	gdf->vcc = id==IDC_GRD_HKP ? memcnt(Grd_clm): memcnt(Rch_clm);
	gdf->hbc = RGB(128,255,255);
	gdf->hfc = RGB(0,0,0);
	gdf->dbc = RGB(255,255,255);
	gdf->dfc = RGB(0,0,0);
	gdf->sbc = RGB(0,0,0);
	gdf->sfc = RGB(255,255,255);
	gdf->cct = gdf->vcc;
	gdf->clm = id==IDC_GRD_HKP ? Grd_clm: Rch_clm;
	gdf->sel = GSL_NON;
	gdf->hdr = TRUE;
	gdf->fsa = TRUE;

	grd = new CSimpleGrid();
	grd->Create( gdf, *rct, id, wnd );
	for ( idx=0; idx<cnt; idx++ ){
		grd->SetCellText( idx, 0, id==IDC_GRD_HKP ? Grd_ttl[idx]: Rch_ttl[idx] );
	}

	return( grd );
}

static	WCHAR	*get_sts(						// getting status name
int			dst,								// gettting target code
int			cnt,								// name table count
STS_CDN		*cdn )								// name table
{
	int			idx;
	WCHAR		*nam = _T("Unknown");

	for ( idx=0; idx<cnt; idx++ ){
		if ( (cdn+idx)->cod == dst ){
			nam = (cdn+idx)->nam;
			break;
		}
	}
	return( nam );
}

static	int		get_sal(						// getting sale status
HKP_DTL_DAT		*dtl )							// house keeping status
{
	int		idx;

	if		( !DBS_INT_VAL(dtl->ise) )	idx = 1;		// RSS_OUTOFORDER
	else if ( !DBS_INT_VAL(dtl->sse) )	idx = 2;		// RSS_OUTOFSERVICE
	else								idx = 0;		// RSS_ENABLE

	return( idx );
}

static	void	dsp_hkl(						// displaying housekeeping on one line
CSimpleGrid			*grd,							// target grid
HKP_DTL_DAT			*dtl,							// detail of data
int					row,							// row of grid
CHouseKeepingView	*viw,
PMS_MNG				*mng )							// Property management System management structure
{
	WCHAR		buf[520], db1[40], db2[40], *fnm, *lnm, *nam;

	grd->SetCellText( HKP_CLM_RMN, row, EDT_DBF_WST(dtl->rmn, _T("")) );
	nam = get_sts( Sal_sts[get_sal(dtl)], viw->m_Sal_cnt,viw-> m_Sal_sts );		
	grd->SetCellText( HKP_CLM_SST, row, nam );
	nam = get_sts( DBS_INT_VAL(dtl->sts->cln), viw->m_Cln_cnt, viw->m_Cln_sts );		
	grd->SetCellText( HKP_CLM_CST, row, nam );
	nam = get_sts( DBS_INT_VAL(dtl->sts->ocp), viw->m_Occ_cnt, viw->m_Occ_sts );		
	grd->SetCellText( HKP_CLM_OST, row, nam );
	fnm = !Nul_Fld(dtl->fnm2, FLD_WST) ? DBS_WST_PTR(dtl->fnm2): ( !Nul_Fld(dtl->fnm1, FLD_WST) ? DBS_WST_PTR(dtl->fnm1): _T("") );
	lnm = !Nul_Fld(dtl->fnm2, FLD_WST) ? DBS_WST_PTR(dtl->lnm2): ( !Nul_Fld(dtl->lnm1, FLD_WST) ? DBS_WST_PTR(dtl->lnm1): _T("") );
	swprintf_s( buf, memcnt(buf), _T("%s %s"), fnm, lnm );	
	grd->SetCellText( HKP_CLM_GNM, row, buf );
	if ( !Nul_Fld(dtl->rei, FLD_INT) ){
		swprintf_s( buf, memcnt(buf), _T("%s-%s"),
			Edt_Dte(db1, memcnt(db1), DBS_DAY_PTR(dtl->usd) ), Edt_Dte(db2, memcnt(db2), DBS_DAY_PTR(dtl->ued) ) );
	}
	else{
		*buf = 0;
	}
	grd->SetCellText( HKP_CLM_POD, row, buf );
}

static	void	dsp_hkp(						// displaying housekeeping data
CSimpleGrid	*grd,								// target grid
Csr			rot,								// root of data
CWnd		*wnd,								// parent window
PMS_MNG		*mng )								// Property management System management structure
{
	int			row=1;
	Csr			csr=NULL;
	HKP_DTL_DAT	*dtl;

	grd->ResetContent();
	grd->AddRow( Cnt_Lnk( rot ) );

	while ( csr = Nxt_Lnk( rot, csr ) ){
		dtl = Rec_Of(HKP_DTL_DAT, csr);
		dsp_hkl( grd, dtl, row, (CHouseKeepingView *)wnd, mng );
		grd->SetData( HKP_CLM_DAT, row++, (int)(LONG_PTR)dtl );
	}
	grd->Redraw();
	grd->InputEnable( TRUE );
}

static	void	dsp_rch(						// displaying room change data
CSimpleGrid		*grd,							// target grid
Csr				rot,							// root of data
PMS_MNG		*mng )								// Property management System management structure
{
	int			row=1;
	Csr			csr=NULL;
	WCHAR		buf[520], *fnm, *lnm;
	HKP_RCH_DAT	*rch;

	grd->ResetContent();
	grd->AddRow( Cnt_Lnk( rot ) );

	while ( csr = Nxt_Lnk( rot, csr ) ){
		rch = Rec_Of(HKP_RCH_DAT, csr);
		grd->SetCellText( HKP_CCL_CRM, row, EDT_DBF_WST(rch->rmn1, _T("")) );
		grd->SetCellText( HKP_CCL_NRM, row, EDT_DBF_WST(rch->rmn2, _T("")) );
		fnm = !Nul_Fld(rch->fnm2, FLD_WST) ? DBS_WST_PTR(rch->fnm2): DBS_WST_PTR(rch->fnm1);
		lnm = !Nul_Fld(rch->fnm2, FLD_WST) ? DBS_WST_PTR(rch->lnm2): DBS_WST_PTR(rch->lnm1);
		swprintf_s( buf, memcnt(buf), _T("%s %s"), fnm, lnm );	
		grd->SetCellText( HKP_CCL_GNM, row, buf );
		grd->SetData( HKP_CLM_DAT, row++, (int)(LONG_PTR)rch );
	}
	grd->Redraw();
	grd->InputEnable( TRUE );
}

int		Hkp_Get_Dat(							// getting and displaying house keeping data
CWnd		*wnd,								// parent window
CDatabase	*dbs,								// ODBC Database object
PMS_MNG		*mng )								// Property management System management structure
{
	int					chk;
	WCHAR				*erm=NULL;
	CHouseKeepingView	*viw = (CHouseKeepingView *)wnd;
	
	if ( chk = Hkp_Get_Dat( viw->m_Hkp_rot, &erm, dbs, mng ) ){
		dsp_hkp( viw->m_Hkp_grd, viw->m_Hkp_rot, wnd, mng );
	}
	else{
		if ( erm ){
			wnd->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
	}

	return( chk );
}

int		Hkp_Get_Rch(							// getting and displaying room chane data
CWnd		*wnd,								// parent window
CDatabase	*dbs,								// ODBC Database object
PMS_MNG		*mng )								// Property management System management structure
{
	int					chk;
	WCHAR				*erm=NULL;
	CHouseKeepingView	*viw = (CHouseKeepingView *)wnd;
	
	if ( chk = Hkp_Get_Rch( viw->m_Rch_rot, &erm, dbs, mng ) ){
		dsp_rch( viw->m_Rch_grd, viw->m_Rch_rot, mng );
	}
	else{
		if ( erm ){
			wnd->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
	}

	return( chk );
}

int		Hkp_Set_Cmb(						// setting combobox data
CComboBox	*cbx,							// combobox of target
int			clm,							// column of grid
int			row,							// row of grid
CWnd		*wnd )							// parent window
{
	int					chk, cnt, knd, idx, cur, now;
	STS_CDN				*sts;
	HKP_DTL_DAT			*dtl;
	CHouseKeepingView	*viw = (CHouseKeepingView *)wnd;

	if ( chk = !!( dtl = (HKP_DTL_DAT *)(LONG_PTR)viw->m_Hkp_grd->GetData( HKP_CLM_DAT, row ) ) ){
		switch( clm ){
		case HKP_CLM_CST:	knd = STS_CLN;	now = DBS_INT_VAL(dtl->sts->cln);	break;
		case HKP_CLM_OST:	knd = STS_OCC;	now = DBS_INT_VAL(dtl->sts->ocp);	break;
		case HKP_CLM_SST:	knd = STS_SAL;	now = Sal_sts[get_sal( dtl )];		break;
		default:			knd = -1;		break;
		}

		if ( chk = knd != -1 ){
			cnt = Viw_Get_Sts( &sts, knd );
			cbx->ResetContent();
			for ( idx=0; idx<cnt; idx++ ){
				cbx->SetItemData( cur = cbx->InsertString( -1, (sts+idx)->nam ), (sts+idx)->cod );
				if ( (sts+idx)->cod == now )	cbx->SetCurSel( cur );
			}
		}
	}

	return( chk );
}

int		Hkp_Get_Cmb(						// getting combobox data
int			idx,							// selecting index of combobox
int			clm,							// column of grid
int			row,							// row of grid
CWnd		*wnd,							// parent window
CDatabase	*dbs,							// ODBC Database object
PMS_MNG		*mng )							// Property management System management structure
{
	int					chk, cnt, knd, *dst;
	WCHAR				*erm=NULL;
	STS_CDN				*sts;
	HKP_DTL_DAT			*dtl, tmp[1];
	CHouseKeepingView	*viw = (CHouseKeepingView *)wnd;

	if ( chk = !!( dtl = (HKP_DTL_DAT *)(LONG_PTR)viw->m_Hkp_grd->GetData( HKP_CLM_DAT, row ) ) ){
		*tmp = *dtl;
		switch( clm ){
		case HKP_CLM_CST:	knd = STS_CLN;	dst = DBS_INT_PTR(tmp->sts->cln);	break;
		case HKP_CLM_OST:	knd = STS_OCC;	dst = DBS_INT_PTR(tmp->sts->ocp);	break;
		case HKP_CLM_SST:	knd = STS_SAL;	dst = NULL;							break;
		default:			knd = -1;		break;
		}

		if ( chk = knd != -1 ){
			cnt = Viw_Get_Sts( &sts, knd );
			if ( clm==HKP_CLM_SST ){
				switch( Sal_sts[idx] ){
				case RSS_ENABLE:		*DBS_INT_PTR(tmp->ise) = TRUE;	*DBS_INT_PTR(tmp->sse) = TRUE;	break;
				case RSS_OUTOFORDER:	*DBS_INT_PTR(tmp->ise) = FALSE;	*DBS_INT_PTR(tmp->sse) = TRUE;	break;
				case RSS_OUTOFSERVICE:	*DBS_INT_PTR(tmp->ise) = TRUE;	*DBS_INT_PTR(tmp->sse) = FALSE;	break;
				}
			}
			else{
				*dst = (sts+idx)->cod;
			}
			if ( chk = Hkp_Chg_Sts( tmp, knd, &erm, dbs, mng ) ){
				*dtl = *tmp;
				dsp_hkl( viw->m_Hkp_grd, dtl, row, viw, mng );
			}
			else{
				if ( erm ){
					wnd->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
					free( erm );
				}
			}
		}
	}

	return( chk );
}
