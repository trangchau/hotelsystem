/*
	Functions for inhouse view definition
*/

//#include "windows.h"
#include "..\..\Common\bsc_def.h"
#include "..\..\Common\dbs_mng.h"
#include "..\..\Common\dte_mng.h"
#include "dsc_sql.h"
#include "dsc_fnc.h"

static	WCHAR	*Grd_ttl[] =	{	{ _T("Name")		},
									{ _T("Abbreviation")},
									{ _T("S.E")			},
									{ _T("SalePeriod")	},
									{ _T("Kind")		},
									{ _T("Price")		} };

static	GRD_CLM	Grd_clm[] =		{	{ 218, GAL_LFT, GCT_NML,  12 },		// Name
									{ 120, GAL_LFT, GCT_NML,  12 },		// Abbreviation
									{  24, GAL_CTR, GCT_CHK,  12 },		// S.E
									{ 176, GAL_LFT, GCT_NML,  12 },		// SalePeriod
									{  80, GAL_LFT, GCT_NML,  12 },		// Kind
									{  96, GAL_RGT, GCT_NML,  12 } };	// Price

static	WCHAR	*Apl_ttl[] =	{	{ _T("Apl")			},
									{ _T("Description")	},
									{ _T("Price")		} };

static	GRD_CLM	Apl_clm[] =		{	{  24, GAL_CTR, GCT_CHK,  12 },		// Apl
									{ 154, GAL_LFT, GCT_NML,  12 },		// Description
									{  94, GAL_RGT, GCT_NML,  12 } };	// Price



CSimpleGrid		*Dsc_Mak_Grd(		// Create each grid
int			id,						// id of grid control
RECT		*rct,					// Rect of grid
CWnd		*wnd )					// 表示先ウィンドウ
{
	CSimpleGrid		*grd;
	GRD_DEF			gdf[1];
	int				idx;

	memset( gdf, 0, sizeof(gdf) );

	gdf->lnh = 20;
	gdf->vlc = id==IDC_GRD_PDT ? 10: 6;
	gdf->vcc = id==IDC_GRD_PDT ? memcnt(Grd_clm): memcnt(Apl_clm);
	gdf->hbc = RGB(128,255,255);
	gdf->hfc = RGB(0,0,0);
	gdf->dbc = RGB(255,255,255);
	gdf->dfc = RGB(0,0,0);
	gdf->sbc = RGB(0,0,0);
	gdf->sfc = RGB(255,255,255);
	gdf->cct = gdf->vcc;
	gdf->clm = id==IDC_GRD_PDT ? Grd_clm: Apl_clm;
	gdf->sel = id==IDC_GRD_PDT ? GSL_ROW: GSL_NON;
	gdf->hdr = TRUE;
	gdf->fsa = TRUE;

	grd = new CSimpleGrid();
	grd->Create( gdf, *rct, id, wnd );
	for ( idx=0; idx<(int)(id==IDC_GRD_PDT ? memcnt(Grd_ttl): memcnt(Apl_ttl)); idx++ ){
		grd->SetCellText( idx, 0, id==IDC_GRD_PDT ? Grd_ttl[idx]: Apl_ttl[idx] );
	}

	return( grd );
}

