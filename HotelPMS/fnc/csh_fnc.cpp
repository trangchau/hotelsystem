/*
	Functions for inhouse view definition
*/

//#include "windows.h"
//#include <afxwin.h>
#include <math.h>
#include "..\resource.h"
#include "..\..\Common\bsc_def.h"
#include "..\..\Common\dbs_mng.h"
#include "..\..\Common\dte_mng.h"
#include "..\..\Common\cmn_fnc.h"
#include "..\..\Custom\SimpleGrid.h"
#include "..\..\PMSCustom\BookingPathCombo.h"
#include "..\CashierDlg.h"
#include "..\PaymentDlg.h"
#include "..\AddDepositDlg.h"
#include "..\TransferDlg.h"
#include "csh_sql.h"
#include "dsc_sql.h"
#include "viw_cmn.h"
#include "csh_fnc.h"

#define	SDK_POST		1		// post message
#define	SDK_SEND		2		// send message

#define	CLM_DAT			1		// Data keeping column

#define	CLM_SEL			0		// Sel
#define	CLM_NBR			1		// No.
#define	CLM_DTE			2		// Date
#define	CLM_DSC			3		// Description
#define	CLM_UNT			4		// Unit
#define	CLM_QTY			5		// Qty
#define	CLM_PRC			6		// Price
#define	CLM_ABR			7		// Price of abroad
//
//#define	CLM_TDC			0		// tax description
//#define	CLM_RTE			1		// tax rate
//#define	CLM_TXP			2		// tax price
//#define	CLM_TXA			3		// tax price abroad

static	WCHAR	*Grd_ttl[] =	{	{ _T("Sel")			},
									{ _T("No.")			},
									{ _T("Date")		},
									{ _T("Description")	},
									{ _T("Unit")		},
									{ _T("Qty")			},
									{ _T("Price")		},
									{ _T("Price")		}};

static	GRD_CLM	Grd_clm[] =		{	{  26, GAL_LFT, GCT_CHK,  20 },		// Sel
									{  32, GAL_LFT, GCT_NML,  20 },		// No.
									{  90, GAL_CTR, GCT_NML,  20 },		// Date
									{ 256, GAL_LFT, GCT_NML, 256 },		// Description
									{  84, GAL_RGT, GCT_CED,  12 },		// Unit
									{  64, GAL_RGT, GCT_CED,  12 },		// Qty
									{ 100, GAL_RGT, GCT_NML,  12 },		// Price
									{ 100, GAL_RGT, GCT_NML,  12 } };	// Price

static	WCHAR	*Tax_ttl[] =	{	{ _T("Description")	},
									{ _T("Rate")		},
									{ _T("Amount")		}};

static	GRD_CLM	Tax_clm[] =		{	{ 128, GAL_RGT, GCT_NML,  12 },		// Description
									{  64, GAL_RGT, GCT_CED,  12 },		// Rate
									{ 100, GAL_RGT, GCT_CED,  12 },		// Amount
									{ 100, GAL_RGT, GCT_NML,  12 } };	// Amount

static	WCHAR	*Dps_ttl[] =	{	{ _T("Currency")	},
									{ _T("Amount")		}};

static	GRD_CLM	Dps_clm[] =		{	{  96, GAL_RGT, GCT_NML,  12 },		// Currency
									{ 100, GAL_RGT, GCT_NML,  12 } };	// Amount


static	int		Pay_mtd[] =	{ PAYMENT_CASH, PAYMENT_CREDITCARD, PAYMENT_COUPON, PAYMENT_OTHER, PAYMENT_DEPOSIT };

static	void	dps_inh(			// dispose CSH_INH_DAT
CSH_INH_DAT		*inh )				// target of dispose
{
	Del_All( inh->dps );
	Del_All( inh->sal->dtl );
}

static	void	set_abr(			// setting abroad currncy
CSimpleGrid		*grd,				// pointer of grid
int				clm,				// column of grid
int				row,				// row of grid
double			val,				// value of amount
CRC_CVS			*ccv )				// currency_conversion
{
	WCHAR		*ptr;

	if ( ccv ){
		if ( ptr = Cma_Edt( val/**DBS_DBL_VAL(ccv->rte)*/, DBS_INT_VAL(ccv->adp) ) ){
			grd->SetCellText( clm, row, ptr );
			free( ptr );
		}
	}
	else{
		grd->SetCellText( clm, row, _T("") );
	}
}
static	void	apl_tax(			// application tax 
double		amt,					// amount
int			apl,					// tax_application of descriptions data
Csr			rot,					// root of tax data
int			rec,					// recursive call
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	//int			idx=0;
	double		prc;
	Csr			csr=NULL;
	CSH_TAX_DAT	*tax;

	while( csr=Nxt_Lnk( rot, csr ) ){
		tax = Rec_Of(CSH_TAX_DAT, csr);
		if ( !!( apl & DBS_INT_VAL(tax->tax->atr) ) ){
			prc = !Nul_Fld(tax->tax->prc, FLD_DBL) ? DBS_DBL_VAL(tax->tax->prc): 0;
			prc = Rnd_Dec( prc, mng->bsc->adp, CEDT_ROUND_UP );
			if ( DBS_INT_VAL(tax->tax->prk)==PRK_AMOUNT || DBS_INT_VAL(tax->tax->prk)==PRK_DISC_AMOUNT ){
				tax->ttl += prc;
			}
			else{
				prc = Rnd_Dec( amt, mng->bsc->adp, CEDT_ROUND_UP );
				tax->ttl += prc;
			}
			if ( !rec ){
				if ( DBS_INT_VAL(tax->tax->prk)==PRK_RATE || DBS_INT_VAL(tax->tax->prk)==PRK_DISC_RATE ){
					prc = Rnd_Dec( prc * ( DBS_DBL_VAL(tax->tax->prc) / 100.0 ), mng->bsc->adp, CEDT_ROUND_UP );
				}
				apl_tax( prc, DBS_INT_VAL(tax->tax->apl), rot, TRUE, mng );
			}
		}
	}
}


static	int	pst_ntf(			// post message to parent window
int			ncd,				// notify id
LPARAM		lpr,				// addtional data
CWnd		*csh,				// parent window
int			knd )				// post/send 
{
	CSH_NTF			*ntf;
	int				rsl=-1;

	ntf = (CSH_NTF *)calloc( 1, sizeof(CSH_NTF) );
	if ( ntf ){
		ntf->ntf = ncd;
		ntf->cid = GetWindowLong( csh->m_hWnd, GWL_ID );
		ntf->csh = (CCashierDlg *)csh;
		if ( SDK_SEND )		rsl = (int)::SendMessage( csh->GetParent()->m_hWnd, WM_CASHIERNOTIFY, (WPARAM)ntf, lpr );
		else				rsl = (int)::PostMessage( csh->GetParent()->m_hWnd, WM_CASHIERNOTIFY, (WPARAM)ntf, lpr );
	}
	return( rsl );
}

double	Csh_Clc_Ttl(				// total calculation
double		*sbt,					// sub total
double		*grt,					// grand total
Csr			dst,					// sales_details
Csr			rot,					// root of tax data
Csr			dpr,					// root of deposit master
CRC_CVS		*ccv,					// currency_conversion
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int			row=1, adp;
	double		amt, rte, dct=1.0;
	CSH_SAL_DTL	*dtl;
	CSH_TAX_DAT	*tax;
	Csr			csr=NULL, cse=NULL;

	if ( ccv )	rte=DBS_DBL_VAL(ccv->rte), adp=DBS_INT_VAL(ccv->adp);

	while( csr=Nxt_Lnk( rot, csr ) ){
		tax = Rec_Of(CSH_TAX_DAT, csr);
		tax->ttl = 0.0;
		tax->amt = 0.0;
	}
	*(sbt+0) = *(sbt+1) = *(grt+0) = *(grt+1) = 0.0;

	while ( csr=Nxt_Lnk( dst, csr ) ){
		dtl = Rec_Of(CSH_SAL_DTL, csr);
		amt = !Nul_Fld(dtl->dtl->slp, FLD_DBL) ? DBS_DBL_VAL(dtl->dtl->slp): 0;
		amt *= DBS_DBL_VAL(dtl->dtl->qty);
		amt = Rnd_Dec( amt, mng->bsc->adp, CEDT_ROUND_UP );
		*(sbt+0) += amt;
		if ( ccv )	*(sbt+1) += Rnd_Dec( amt * rte, adp, CEDT_ROUND_UP );
		apl_tax( amt, DBS_INT_VAL(dtl->apl), rot, FALSE, mng );
	}
	*(grt+0) = *(sbt+0);
	*(grt+1) = *(sbt+1);

	while ( csr = Nxt_Lnk( dpr, csr ) ){
		tax = Rec_Of(CSH_TAX_DAT, csr);
		if ( DBS_INT_VAL(tax->tax->dkd)==DRK_DISCOUNT ){
			if ( tax->ttl > 0.0 ){
				tax->amt = tax->ttl;
				if ( DBS_INT_VAL(tax->tax->prk)==PRK_DISC_RATE ){
					tax->amt = Rnd_Dec( *(sbt+0) * (tax->amt / 100), mng->bsc->adp, CEDT_ROUND_UP );
					dct = 1 - tax->ttl / 100.0;
				}
				else{
					dct = 1 - tax->amt / *(sbt+0);
				}
				if ( ccv ){
					tax->abr = Rnd_Dec( tax->amt * rte, adp, CEDT_ROUND_UP ) * -1;
					*(grt+1) += tax->abr;
				}
				tax->amt *= -1.0;
				*(grt+0) += tax->amt;
			}
		}
	}
	while ( csr=Nxt_Lnk( rot, csr ) ){
		tax = Rec_Of(CSH_TAX_DAT, csr);
		if ( DBS_INT_VAL(tax->tax->prk)==PRK_RATE || DBS_INT_VAL(tax->tax->prk)==PRK_DISC_RATE ){
			tax->amt = Rnd_Dec( tax->ttl * ((DBS_DBL_VAL(tax->tax->prc)*dct) / 100), mng->bsc->adp, CEDT_ROUND_UP );
		}
		else{
			tax->amt = tax->ttl;
		}
		if ( ccv )	tax->abr = Rnd_Dec( tax->amt * rte, adp, CEDT_ROUND_UP );
		*(grt+0) += tax->amt;
		if ( ccv )	*(grt+1) += tax->abr;
	}
	return( *(grt+0) );
}

double	Csh_Dsp_Ttl(				// total calculation and display
CSimpleGrid	*grd,					// displaying grid
Csr			dst,					// sales_details
Csr			rot,					// root of tax data 
Csr			dpr,					// root of deposit master
CRC_CVS		*ccv,					// currency_conversion
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int			row=1;
	double		sbt[2], grt[2], ttl;
	WCHAR		buf[40];
	Csr			csr=NULL;
	CSH_TAX_DAT	*tax;

	ttl = Csh_Clc_Ttl( sbt, grt, dst, rot, dpr, ccv, mng );

	Viw_Prc_Edt(buf, memcnt(buf), sbt[0], 0, mng ); 
	grd->SetCellText( CLM_TXP, row, buf );
	set_abr( grd, CLM_TXA, row++, sbt[1], ccv );
	for ( ; row<grd->GetLineCount(FALSE); row++ ){
		tax = (CSH_TAX_DAT *)(LONG_PTR)grd->GetData( CLM_DAT, row );
		if ( DBS_INT_VAL(tax->tax->dkd)!=DRK_DISCOUNT )	break;
		if ( tax->ttl > 0.0 ){
			if ( DBS_INT_VAL(tax->tax->prk)==PRK_DISC_RATE ){
				Viw_Prc_Edt(buf, memcnt(buf), tax->ttl, DBS_INT_VAL(tax->tax->prk), mng );
				grd->SetCellText( CLM_RTE, row, buf );
			}
			Viw_Prc_Edt(buf, memcnt(buf), tax->amt, 0, mng );
			grd->SetCellText( CLM_TXP, row, buf );
			set_abr( grd, CLM_TXA, row, tax->abr, ccv );
		}
		else{
			*buf = 0;
			grd->SetCellText( CLM_RTE, row, _T("") );
			grd->SetCellText( CLM_TXP, row, _T("") );
			grd->SetCellText( CLM_TXA, row, _T("") );
		}
	}
	while ( csr=Nxt_Lnk( rot, csr) ){
		tax = Rec_Of(CSH_TAX_DAT, csr);
		Viw_Prc_Edt(buf, memcnt(buf), tax->amt, 0, mng ); 
		grd->SetCellText( CLM_TXP, row, buf );
		set_abr( grd, CLM_TXA, row++, tax->abr, ccv );
	}
	Viw_Prc_Edt(buf, memcnt(buf), grt[0], 0, mng ); 
	grd->SetCellText( CLM_TXP, row, buf );
	set_abr( grd, CLM_TXA, row++, grt[1], ccv );

	return( ttl ); 
}

static	void	dsp_ttl(			// total calculation and display
CCashierDlg	*csh,					// target dialog
CRC_CVS		*ccv,					// currency_conversion
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	CSH_INH_DAT	*inh;
	Csr			csr=NULL, cse=NULL;

	inh = Rec_Of(CSH_INH_DAT, Idx_Lnk( csh->m_Inh_rot, csh->m_Tab_idx ) );
	csh->m_Bls_amt = Csh_Dsp_Ttl( csh->m_Tax_grd, inh->sal->dtl, csh->m_Tax_rot, csh->m_Dps_rot, ccv, mng );
}

static	void	dsp_dtl(			// displaying sale_detail
CSimpleGrid		*grd,				// target grid
CSH_SAL_DTL		*dtl,				// displaying data
int				row,				// display row
CRC_CVS			*ccv,				// currency_conversion
PMS_MNG			*mng )				// Hotel Property Management Segment
{
	WCHAR			buf[512], *ptr;
	double			amt;

	swprintf_s( buf, memcnt(buf), _T("%d"), row );
	grd->SetCellText( CLM_NBR, row, buf );
	grd->SetCellText( CLM_DTE, row, EDT_DBF_DAY(buf, memcnt(buf), dtl->dtl->sld) );
	ptr =  EDT_DBF_WST(dtl->dcn, _T(""));
	if ( DBS_INT_VAL(dtl->dkd)==DRK_SERVICE ){
		swprintf_s( buf, memcnt(buf), _T("%s (%s)"), ptr, DBS_WST_PTR(dtl->rmn) );
		ptr = buf;
	}
	grd->SetCellText( CLM_DSC, row, ptr );
	amt = !Nul_Fld(dtl->dtl->slp, FLD_DBL) ? DBS_DBL_VAL(dtl->dtl->slp): 0;
	Viw_Prc_Edt(buf, memcnt(buf), amt, 0, mng ); 
	grd->SetCellText( CLM_UNT, row, buf );
	ptr = Cma_Edt( DBS_DBL_VAL(dtl->dtl->qty), 0 );
	grd->SetCellText( CLM_QTY, row, ptr ? ptr: _T("") );
	if ( ptr )	free( ptr );
	amt = Rnd_Dec(amt * DBS_DBL_VAL(dtl->dtl->qty), mng->bsc->adp, CEDT_ROUND_UP);
	Viw_Prc_Edt(buf, memcnt(buf), amt, 0, mng ); 
	grd->SetCellText( CLM_PRC, row, buf );
	if ( ccv )	amt = Rnd_Dec(amt*DBS_DBL_VAL(ccv->rte), DBS_INT_VAL(ccv->adp), CEDT_ROUND_UP);
	set_abr( grd, CLM_ABR, row, amt, ccv );
}

static int 	dps_cmp(		// compeare the deposit data for ascending sort 
const void	*dpt,			// key data 1
const void	*spt )			// key data 2
{
	int		cmp;

	CSH_DPS_DAT	*dst = *Rec_Of(CSH_DPS_DAT *, *((Csr *)dpt ) );
	CSH_DPS_DAT	*src = *Rec_Of(CSH_DPS_DAT *, *((Csr *)spt ) );
	cmp = DBS_INT_VAL(dst->pmt->cci) - DBS_INT_VAL(src->pmt->cci);
	if ( !cmp )	cmp = DBS_INT_VAL(dst->pmt->mtd) - DBS_INT_VAL(src->pmt->mtd);
	return( cmp );
}

static	double	clc_exc(			// calculate exchange amount
double		amt,					// conversion amount
CRC_CVS		*dcv,					// destination currency
CRC_CVS		*scv,					// dource currency
//CSH_DPS_DAT	*dps,					// source currency
CRC_CVS		*ccb )					// bace currency
{
//	double		amt = dps->ttl;

	if ( !!DBS_INT_VAL(scv->isb) ){
		amt = Rnd_Dec( amt * DBS_DBL_VAL(dcv->rte), DBS_INT_VAL(dcv->adp), CEDT_ROUND_UP );
	}
	else if ( !!DBS_INT_VAL(dcv->isb) ){
		amt = Rnd_Dec( amt / DBS_DBL_VAL(scv->rte), DBS_INT_VAL(scv->adp), CEDT_ROUND_UP );
	}
	else{
		amt = Rnd_Dec( amt / DBS_DBL_VAL(scv->rte), DBS_INT_VAL(ccb->adp), CEDT_ROUND_UP );
		amt = Rnd_Dec( amt * DBS_DBL_VAL(dcv->rte), DBS_INT_VAL(dcv->adp), CEDT_ROUND_UP );
	}
	return( amt );
}

void	Csh_Dsp_Dps(				// displaying deposit
CSimpleGrid	*grd,					// displaying grid
Csr			rot,					// root of deposit
int			mtd,					// displaying payment_method
CRC_CVS		*ccv,					// peyment currency
CRC_CVS		*ccb )					// bace currency
{
	int			chk=TRUE, clm, row=1;
	Lnk			tmp[1], *csr=NULL, *cse=NULL;
	CSH_DPS_DAT	*dps, *ptr;
	WCHAR		*ept, buf[40], ebf[40];
	CRC_CVS		ctp[1];


	memset( tmp, 0, sizeof(tmp) );
	memset( ctp, 0, sizeof(ctp) );

	grd->ResetContent();

	while( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
		dps = Rec_Of(CSH_DPS_DAT, csr);
		for ( cse=NULL; cse=Nxt_Lnk( tmp, cse ); ){
			ptr = *Rec_Of(CSH_DPS_DAT *, cse);
			if ( DBS_INT_VAL(dps->pmt->cci) == DBS_INT_VAL(ptr->pmt->cci) ){
				if ( !mtd || DBS_INT_VAL(dps->pmt->mtd) == DBS_INT_VAL(ptr->pmt->mtd) ){
					break;
				}
			}
		}
		if ( !!cse ){
			ptr->ttl += DBS_DBL_VAL(dps->pmt->amt);
		}
		else{
			if ( chk = !!( cse = Add_Lnk( tmp, &dps, sizeof(CSH_DPS_DAT *) ) ) ){
				ptr = *Rec_Of(CSH_DPS_DAT *, cse);
				ptr->ttl = DBS_DBL_VAL(ptr->pmt->amt);
			}
		}
	}
	if ( chk ){
		Sot_Lnk( tmp, dps_cmp );
		grd->AddRow( Cnt_Lnk( tmp ) );
		while ( csr = Nxt_Lnk( tmp, csr ) ){
			ptr = *Rec_Of(CSH_DPS_DAT *, csr);
			clm = 0;
			grd->SetCellText( clm++, row, EDT_DBF_WST(ptr->crc,_T("")) );
			if ( mtd ){
				ept = Viw_Get_Dsn( DBS_INT_VAL(ptr->pmt->mtd), DSC_PMT );
				grd->SetCellText( clm++, row, ept ? ept: _T("Unknown") );
			}
			if ( !!( ept = Cma_Edt( ptr->ttl, DBS_INT_VAL(ptr->adp) ) ) ){
				grd->SetCellText( clm++, row, ept );
				free( ept );
			}
			if ( ccv && ccb ){
				DBS_INT_CPY(ctp->adp, ptr->adp); 
				DBS_DBL_CPY(ctp->rte, ptr->rte); 
				if ( row==1 ){
					grd->GetCellText( ebf, memcnt(ebf), clm, 0 );
					if ( ept = wcschr( ebf, _T('(') ) )		*ept = 0;
					swprintf_s( buf, memcnt(buf), _T("%s(%s)"), ebf, EDT_DBF_WST(ccv->crc,_T("")) );
					grd->SetCellText( clm, 0, buf );
				}
				ptr->bls = clc_exc( ptr->ttl, ccv, ctp, ccb );
				ptr->pay = 0.0;
				if ( !!( ept = Cma_Edt( ptr->bls - ptr->pay, DBS_INT_VAL(ccv->adp) ) ) ){
					grd->SetCellText( clm, row, ept );
					free( ept );
				}
				if ( row==1 ){
					grd->GetCellText( ebf, memcnt(ebf), ++clm, 0 );
					if ( ept = wcschr( ebf, _T('(') ) )		*ept = 0;
					swprintf_s( buf, memcnt(buf), _T("%s(%s)"), ebf, EDT_DBF_WST(ccv->crc,_T("")) );
					grd->SetCellText( clm, 0, buf );
				}
			}
			grd->SetData( CLM_DAT, row, (int)(LONG_PTR)ptr );
			++row;
		}
	}
	grd->Redraw();

	Del_All( tmp );
}

void	Csh_Dsp_Sal(				// displaying sale list 
CWnd		*wnd,					// target dialog
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int			row=0;
	WCHAR		ttl[50];
	CSH_INH_DAT	*inh;
	CSH_SAL_DAT	*sal;
	CSH_SAL_DTL	*dtl;
	Csr			csr=NULL;
	CCashierDlg	*csh = (CCashierDlg	*)wnd;

	inh = Rec_Of(CSH_INH_DAT, Idx_Lnk( csh->m_Inh_rot, csh->m_Tab_idx ) );
	sal = inh->sal;
	//sal = csh->m_Sal_dat+csh->m_Tab_idx;

	csh->m_Csh_grd->ResetContent();
	csh->m_Csh_grd->InputEnable( TRUE );
	if ( !!csh->m_Crc_cvs ){
		swprintf_s( ttl, memcnt(ttl), _T("Price(%s)"), DBS_WST_PTR(csh->m_Crc_cvs->crc) );
	}
	else{
		*ttl = 0;
	}
	csh->m_Csh_grd->SetCellText( CLM_ABR, 0, ttl );
		
	while ( csr=Nxt_Lnk( sal->dtl, csr ) ){
		dtl = Rec_Of(CSH_SAL_DTL, csr);
//		if ( DBS_INT_VAL(dtl->dkd)!=DRK_DEPOSIT ){
			csh->m_Csh_grd->AddRow( 1 );
			dsp_dtl( csh->m_Csh_grd, dtl, ++row, csh->m_Crc_cvs, mng );
			csh->m_Csh_grd->SetData( CLM_DAT, row, (int)(LONG_PTR)dtl );
			csh->m_Csh_grd->SetEnable( CLM_UNT, row, TRUE );
			csh->m_Csh_grd->SetEnable( CLM_QTY, row, TRUE );
//		}
	}
	dsp_ttl( csh, csh->m_Crc_cvs, mng );
	Csh_Dsp_Dps( csh->m_Dps_grd, inh->dps, FALSE, NULL, NULL );
	pst_ntf( CN_ROOMCHANGED, DBS_INT_VAL(inh->rdi), csh, SDK_POST );
}


CSimpleGrid		*Csh_Mak_Grd(		// Creating a cashier grid
int			id,						// id of grid control
RECT		*rct,					// Rect of grid
CWnd		*wnd )					// parent window
{
	CSimpleGrid		*grd;
	WCHAR			**ttl;
	GRD_DEF			gdf[1];
	GRD_CLM			*gcl;
	int				idx, vlc, vcc;

	memset( gdf, 0, sizeof(gdf) );

	switch( id ){
	case IDC_GRD_CSH:	vlc=10; gcl=Grd_clm; vcc=memcnt(Grd_clm); ttl=Grd_ttl; break;
	case IDC_GRD_CTX:	vlc= 8; gcl=Tax_clm; vcc=memcnt(Tax_clm); ttl=Tax_ttl; break;
	case IDC_GRD_DPS:	vlc= 4; gcl=Dps_clm; vcc=memcnt(Dps_clm); ttl=Dps_ttl; break;
	}

	gdf->lnh = 20;
	gdf->vlc = vlc;
	gdf->vcc = vcc;
	gdf->hbc = RGB(128,255,255);
	gdf->hfc = RGB(0,0,0);
	gdf->dbc = RGB(255,255,255);
	gdf->dfc = RGB(0,0,0);
	gdf->sbc = RGB(0,0,0);
	gdf->sfc = RGB(255,255,255);
	gdf->cct = gdf->vcc;
	gdf->clm = gcl;
	gdf->sel = GSL_NON;//id==IDC_GRD_CSH ? GSL_ROW: GSL_NON;
	gdf->hdr = id!=IDC_GRD_CTX;
	gdf->fsa = TRUE;

	grd = new CSimpleGrid();
	grd->Create( gdf, *rct, id, wnd );
	for ( idx=0; idx<vlc; idx++ ){
		grd->SetCellText( idx, 0, ttl[idx] );
	}

	return( grd );
}

void	Csh_Dsp_Tax(				// displaying tax definision
CSimpleGrid	*grd,					// displaying grid
Csr			trt,					// root of tax definition
Csr			drt,					// root of discount and depsit definition
int			dct,					// displaying discount
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int			row=1, cnt=0;
	double		amt;
	Csr			csr=NULL;
	WCHAR		buf[40];
	CSH_TAX_DAT	*tax;

	if ( dct ){
		while( csr=Nxt_Lnk( drt, csr ) ){
			tax = Rec_Of(CSH_TAX_DAT, csr);
			if ( DBS_INT_VAL(tax->tax->dkd)==DRK_DISCOUNT )	cnt++;
		}
	}
	grd->ResetContent();
	grd->AddRow( Cnt_Lnk( trt ) + cnt + 3 - ( grd->IsHeader() ? 1: 0 ) );
	grd->InputEnable( TRUE );
	grd->SetCellText( CLM_TDC, row, _T("Sub total") );
	row++;
	if ( dct ){
		while( csr=Nxt_Lnk( drt, csr ) ){
			tax = Rec_Of(CSH_TAX_DAT, csr);
			if ( DBS_INT_VAL(tax->tax->dkd)==DRK_DISCOUNT ){
				grd->SetCellText( CLM_TDC, row, EDT_DBF_WST(tax->tax->nam, _T("")) );
				grd->SetCellText( CLM_RTE, row, _T("") );
				grd->SetData( CLM_DAT, row, (int)(LONG_PTR)tax );
				grd->SetEnable( DBS_INT_VAL(tax->tax->prk)==PRK_DISC_RATE ? CLM_RTE: CLM_TXP, row, TRUE );
				row++;
			}
		}
	}
	while( csr=Nxt_Lnk( trt, csr ) ){
		tax = Rec_Of(CSH_TAX_DAT, csr);
		grd->SetCellText( CLM_TDC, row, EDT_DBF_WST(tax->tax->nam, _T("")) );
		amt = !Nul_Fld(tax->tax->prc, FLD_DBL) ? DBS_DBL_VAL(tax->tax->prc): 0;
		Viw_Prc_Edt(buf, memcnt(buf), amt, DBS_INT_VAL(tax->tax->prk), mng ); 
		grd->SetCellText( CLM_RTE, row, buf );
		grd->SetData( CLM_DAT, row, (int)(LONG_PTR)tax );
		row++;
	}
	grd->SetCellText( CLM_TDC, row++, _T("Total") );
	//grd->SetCellText( CLM_TDC, row++, _T("Deposit") );
	//grd->SetCellText( CLM_TDC, row++, _T("Balance") );
}

int		Csh_Get_Tax(				// Tax and additional data
WCHAR		**erm,					// error message storing pointer
CWnd		*wnd,					// parent window
CDatabase	*dbs,					// ODBC Database object
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int			chk, row=1;
//	double		amt;
	Lnk			rot[1], *csr=NULL, *cse;
//	WCHAR		buf[40];
	DSC_RPT		tmp[1]/*, *tax*/;
	CSH_TAX_DAT	*tax;
	CCashierDlg	*csh = (CCashierDlg *)wnd;

	memset( rot, 0, sizeof(rot) );
	memset( tmp, 0, sizeof(tmp) );

	DBS_WST_SET(tmp->opc, csh->m_Ope_cod );
	DBS_WST_SET(tmp->brc, csh->m_Brc_cod );
	DBS_WST_SET(tmp->fcc, csh->m_Fcl_cod );

	chk = Dsc_Get_Tax( rot, tmp, TRUE, erm, dbs, mng );
	while( chk && !!( csr=Nxt_Lnk( rot, csr ) ) ){
		if ( chk = !!( cse=Add_Lnk( csh->m_Tax_rot, NULL, sizeof(CSH_TAX_DAT) ) ) ){
			tax = Rec_Of(CSH_TAX_DAT, cse);
			*tax->tax = *Rec_Of(DSC_RPT, csr);
		}
	}
	if ( chk ){
		if ( chk = Dsc_Get_Dsc( csh->m_Dps_rot, tmp, TRUE, erm, dbs, mng ) ){
			Csh_Dsp_Tax( csh->m_Tax_grd, csh->m_Tax_rot, csh->m_Dps_rot, FALSE, mng );
		}
	}
	Del_All( rot );

	return( chk );
}

static	void	set_sal(			// sales_infos data set
SAL_INF		*dst,					// destination pointer
CCashierDlg	*csh,					// CCashierDlg
CSH_INH_DAT	*inh,					// inhouse room information
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	DBS_WST_SET(dst->opc, csh->m_Ope_cod);
	DBS_WST_SET(dst->brc, csh->m_Brc_cod);
	DBS_WST_SET(dst->fcc, csh->m_Fcl_cod);
	DBS_INT_CPY(dst->rii, inh->rii);
	DBS_INT_CPY(dst->rdi, inh->rdi);
	DBS_INT_CPY(dst->eii, inh->eii);
	if ( Nul_Fld(inh->slg, FLD_WST) ){
		DBS_WST_SET(dst->flg, mng->dfl);
	}
	else{
		DBS_INT_CPY(dst->flg, inh->slg);
	}
}

int		Csh_All_Sal(				// add new selecting products
Csr			prt,					// root of selecting products
CWnd		*wnd,					// parent window
CDatabase	*dbs,					// ODBC Database object
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int			chk=TRUE, val=0;
	Csr			csr=NULL, cse;
	CSH_INH_DAT	*inh;
	CSH_PDT_LST	*pdt;
	CSH_SAL_DAT	*sal;
	CSH_SAL_DTL	*dtl;
	CCashierDlg	*csh = (CCashierDlg *)wnd;

	inh = Rec_Of(CSH_INH_DAT, Idx_Lnk( csh->m_Inh_rot, csh->m_Tab_idx ) );
	sal = inh->sal;
//	sal = csh->m_Sal_dat+csh->m_Tab_idx;

	if ( Nul_Fld(sal->sal->id, FLD_INT) ){
		set_sal( sal->sal, csh, inh, mng );
	}
	DBS_INT_SET(sal->sal->pid, &val);		
		
	while( csr=Nxt_Lnk( prt, csr ) ){
		pdt = Rec_Of(CSH_PDT_LST, csr);
		if ( chk = !!( cse = Add_Lnk( sal->dtl, NULL, sizeof(CSH_SAL_DTL) ) ) ){
			dtl = Rec_Of(CSH_SAL_DTL, cse);
			DBS_WST_SET(dtl->dtl->opc, csh->m_Ope_cod);
			DBS_WST_CPY(dtl->dcn, pdt->pdt->nam);
			DBS_INT_CPY(dtl->apl, pdt->pdt->apl);
			DBS_INT_CPY(dtl->dkd, pdt->pdt->dkd);
			DBS_INT_CPY(dtl->prk, pdt->pdt->prk);
			DBS_DAY_SET(dtl->dtl->sld, mng->bdt);
			DBS_INT_CPY(dtl->dtl->dsi, pdt->pdt->id);
			DBS_DBL_SET(dtl->dtl->qty, &pdt->qty);
			DBS_DBL_CPY(dtl->dtl->nmp, pdt->pdt->prc);
			DBS_DBL_CPY(dtl->dtl->slp, pdt->pdt->prc);
			DBS_INT_SET(dtl->dtl->asi, &val);
			DBS_INT_CPY(dtl->dtl->eqi, inh->eii);
			DBS_INT_CPY(dtl->apl, pdt->pdt->apl);
		}
	}

	Csh_Dsp_Sal( csh, mng );

	return( chk );
}
/*
static	int	add_dps(				// adding a deposit into inhouse data
CSH_INH_DAT	*inh,					// inhouse data
double		amt,					// deposit amount
int			pmt,					// payment method
CRC_CVS		*ccv,					// currency data
CSH_TAX_DAT	*dsc,					// discount master
CCashierDlg	*csh,					// parent window
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	double		fvl;
	CSH_DPS_DAT	*dps;
	Csr			csr;
	
	if ( csr = Add_Lnk( inh->dps, NULL, sizeof(CSH_DPS_DAT) ) ){
		dps = Rec_Of(CSH_DPS_DAT, csr);
		DBS_INT_CPY(dps->rii, inh->rii);
		DBS_INT_CPY(dps->rdi, inh->rdi);
		DBS_WST_CPY(dps->crc, ccv->crc);
		DBS_INT_CPY(dps->adp, ccv->adp);
		DBS_WST_SET(dps->dtl->opc, csh->m_Ope_cod);
		DBS_INT_CPY(dps->dtl->sii, inh->sal->sal->id);
		DBS_DAY_SET(dps->dtl->sld, mng->bdt);
		DBS_INT_CPY(dps->dtl->dsi, dsc->tax->id);
		DBS_DBL_SET(dps->dtl->qty, &(fvl=0));
		DBS_DBL_SET(dps->dtl->nmp, &(fvl=0));
		DBS_DBL_SET(dps->dtl->slp, &(fvl=0));
		DBS_INT_SET(dps->dtl->asi, (int *)NULL);
		DBS_INT_SET(dps->dtl->eqi, (int *)NULL);
		DBS_WST_SET(dps->pmt->opc, csh->m_Ope_cod);
		DBS_INT_CPY(dps->pmt->sii, inh->sal->sal->id);
		DBS_DAY_SET(dps->pmt->pmd, mng->bdt);
		DBS_INT_CPY(dps->pmt->cci, ccv->id);
		DBS_DBL_CPY(dps->pmt->rte, ccv->rte);
		DBS_DBL_SET(dps->pmt->amt, &amt);
		DBS_INT_SET(dps->pmt->mtd, &pmt);
		//Sot_Lnk( inh->dps, dps_cmp );
	}
	return( !!csr );
}
*/

int		Csh_Add_Dps(				// adding a deposit
CWnd		*wnd,					// parent window
CDatabase	*dbs,					// ODBC Database object
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int				chk, idx;
//	Lnk				rot[1], *csr=NULL;
	Csr				csr=NULL;
	CSH_INH_DAT		*inh;
//	CSH_PDT_LST		*pdt;
	CSH_TAX_DAT		*dps;
	CCashierDlg		*csh = (CCashierDlg *)wnd;
	CAddDepositDlg	*dlg;

	idx = ((CTabCtrl *)csh->GetDlgItem(IDC_TAB_ROOM))->GetCurSel();
	inh = Rec_Of(CSH_INH_DAT, Idx_Lnk( csh->m_Inh_rot, idx ));

	//memset( rot, 0, sizeof(rot) );

	while( csr=Nxt_Lnk( csh->m_Dps_rot, csr ) ){
		dps = Rec_Of(CSH_TAX_DAT, csr);
		if ( DBS_INT_VAL(dps->tax->dkd)==DRK_DEPOSIT )	break;
	}

	if ( chk = !!csr ){
		if ( chk = !!( dlg = new CAddDepositDlg(wnd) ) ){
//			dlg->m_Inh_dat = inh;
			dlg->m_Dps_rot = inh->dps;
			dlg->m_Rsv_iid = DBS_INT_VAL(inh->rii);
			dlg->m_Rsv_did = DBS_INT_VAL(inh->rdi);
			dlg->m_Sal_iid = DBS_INT_VAL(inh->sal->sal->id);
			dlg->m_Dps_mst = dps;
			dlg->m_Ccv_rot = csh->m_Ccv_rot;
			dlg->m_Fst_nam = EDT_DBF_WST(inh->fsn,_T(""));
			dlg->m_Lst_nam = EDT_DBF_WST(inh->lsn,_T(""));
			dlg->m_Rom_nbr = EDT_DBF_WST(inh->rmn,_T(""));
			dlg->m_Ope_cod = csh->m_Ope_cod;
			dlg->m_Brc_cod = csh->m_Brc_cod;
			dlg->m_Fcl_cod = csh->m_Fcl_cod;
			dlg->m_pDbs_obj = csh->m_pDbs_obj;
			dlg->m_pPms_mng = csh->m_pPms_mng;
			dlg->m_Dir_ect = FALSE;
			if ( dlg->DoModal()==IDOK ){
				Csh_Dsp_Dps( csh->m_Dps_grd, inh->dps, FALSE, NULL, NULL );
			}
			delete dlg;
		}
	}
	else{
		csh->MessageBox( _T("Deposit is not registerd on master."), NULL, MB_ICONEXCLAMATION|MB_OK );
	}
	//Del_All( rot );

	return( chk );
}

void	Csh_Ecp_Pdt(				// edit to complete of unit price or quantity
DWORD		cid,					// control id
int			clm,					// column of grid
int			row,					// row of grid
WCHAR		*txt,					// editting text 
CWnd		*wnd,					// parent window
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	double		fvl;
	CSH_SAL_DTL	*dtl;
	CCashierDlg	*csh = (CCashierDlg *)wnd;
	Day			dte[1];

	Prg_Cma( txt );
	fvl = _wtof( txt );

	if ( !!( dtl = (CSH_SAL_DTL *)(LONG_PTR)csh->m_Csh_grd->GetData(CLM_DAT, row ) ) ){
		Get_Dte( dte );
		if ( clm==CLM_QTY ){
			*DBS_DBL_PTR(dtl->dtl->qty) = fvl;
		}
		else if ( clm==CLM_UNT ){
			*DBS_DBL_PTR(dtl->dtl->slp) = fvl;
		}
		if ( !Nul_Fld(dtl->dtl->id, FLD_INT) ){
			DBS_DTM_SET(dtl->dtl->mdt, dte);
		}
		dsp_dtl( csh->m_Csh_grd, dtl, row, csh->m_Crc_cvs, mng );
		dsp_ttl( csh, csh->m_Crc_cvs, mng );
	}
}

void	Csh_Ecp_Ttl(				// edit to complete of unit price or quantity
int			row,					// grid notification data
Csr			rot,					// root of sales_details
WCHAR		*txt,					// editting text 
CSimpleGrid	*grd,
CWnd		*wnd,					// parent window
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int			idx;
	double		fvl;
	CSH_TAX_DAT	*tax;
	CCashierDlg	*csh = (CCashierDlg *)wnd;

	Prg_Cma( txt );
	fvl = _wtof( txt );

	if ( tax = (CSH_TAX_DAT *)(LONG_PTR)grd->GetData( CLM_DAT, row ) ){
		tax->ttl = fvl;
		for ( idx=1; idx<grd->GetLineCount(FALSE); idx++ ){
			if ( idx!=row && !!( tax = (CSH_TAX_DAT *)(LONG_PTR)grd->GetData( CLM_DAT, idx ) ) ){
				if ( DBS_INT_VAL(tax->tax->dkd)==DRK_DISCOUNT )	tax->ttl = 0;
			}
		}
	}
}

/*
void	Csh_Edt_Cmp(				// edit to complete of unit price or quantity
DWORD		cid,					// control id
int			clm,					// column of grid
int			row,					// row of grid
WCHAR		*txt,					// editting text 
CWnd		*wnd,					// parent window
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int			idx;
	double		fvl;
	CSH_SAL_DTL	*dtl;
	CSH_TAX_DAT	*tax;
	CSH_INH_DAT	*inh;
	CCashierDlg	*csh = (CCashierDlg *)wnd;
	Day			dte[1];

	Prg_Cma( txt );
	fvl = _wtof( txt );

	if ( cid==IDC_GRD_CSH ){
		if ( !!( dtl = (CSH_SAL_DTL *)(LONG_PTR)csh->m_Csh_grd->GetData(CLM_DAT, row ) ) ){
			Get_Dte( dte );
			if ( clm==CLM_QTY ){
				*DBS_DBL_PTR(dtl->dtl->qty) = fvl;
			}
			else if ( clm==CLM_UNT ){
				*DBS_DBL_PTR(dtl->dtl->slp) = fvl;
			}
			if ( !Nul_Fld(dtl->dtl->id, FLD_INT) ){
				DBS_DTM_SET(dtl->dtl->mdt, dte);
			}
			dsp_dtl( csh->m_Csh_grd, dtl, row, csh->m_Crc_cvs, mng );
			dsp_ttl( csh, csh->m_Crc_cvs, mng );
		}
	}
	else{
		idx = ((CTabCtrl *)csh->GetDlgItem(IDC_TAB_ROOM))->GetCurSel();
		if ( idx!= -1 ){
			inh = Rec_Of(CSH_INH_DAT, Idx_Lnk(csh->m_Inh_rot, idx));
			if ( tax = (CSH_TAX_DAT *)(LONG_PTR)csh->m_Tax_grd->GetData( CLM_DAT, row ) ){
				tax->ttl = fvl;
				for ( idx=1; idx<csh->m_Tax_grd->GetLineCount(FALSE); idx++ ){
					if ( idx!=row && !!( tax = (CSH_TAX_DAT *)(LONG_PTR)csh->m_Tax_grd->GetData( CLM_DAT, idx ) ) ){
						if ( DBS_INT_VAL(tax->tax->dkd)==DRK_DISCOUNT )	tax->ttl = 0;
					}
				}
				Csh_Dsp_Ttl( csh->m_Tax_grd, inh->sal->dtl, csh->m_Tax_rot, csh->m_Dps_rot, csh->m_Crc_cvs, mng );
			}
		}
	}
}
*/

void	Csh_Dsp_Crt(				// display the currency rate
CRC_CVS			*ccv,				// currency data
CWnd			*wnd )				// displaying window
{
	WCHAR		buf[40];

	if ( ccv )	swprintf_s( buf, memcnt(buf), _T("%.12f"), DBS_DBL_VAL(ccv->rte) );
	else		*buf = 0;
	wnd->SetWindowText( buf );
}


LRESULT		Csh_Clm_Typ(			// required colmun type
DWORD		cid,					// control id
int			clm,					// column of grid
int			row,					// row of grid
CWnd		*wnd )					// displaying window
{
	LRESULT		rsl;
	CSH_SAL_DTL	*dtl;
	CSH_TAX_DAT	*tax;
	CCashierDlg	*csh = (CCashierDlg *)wnd;

	if ( cid==IDC_GRD_CSH ){
		dtl = (CSH_SAL_DTL *)(LONG_PTR)csh->m_Csh_grd->GetData( CLM_DAT, row );
		switch( clm ){
		case CLM_SEL:	rsl=GCT_CHK;
						break;
		case CLM_UNT:
		case CLM_QTY:	rsl = Nul_Fld(dtl->dtl->id, FLD_INT) ? GCT_CED: GCT_NML;
						break;
		default:		rsl=GCT_NML;
						break; 
		}
	}
	else{
		rsl=GCT_NML; 
		if ( tax = (CSH_TAX_DAT *)(LONG_PTR)csh->m_Tax_grd->GetData( CLM_DAT, row ) ){
			if ( DBS_INT_VAL(tax->tax->dkd)==DRK_DISCOUNT ){
				if ( ( DBS_INT_VAL(tax->tax->prk)==PRK_DISC_AMOUNT && clm==CLM_TXP )
					|| ( DBS_INT_VAL(tax->tax->prk)==PRK_DISC_RATE && clm==CLM_RTE ) ){
					rsl=GCT_CED;
				}
			}
		}
	}
	return( rsl );
}

static	int	pay_mnt(				// sale up partical products
CSH_SAL_DAT	*sal,					// sales_infos ( NULL if partical payment
double		*pay,					// content of payment
Csr			dpr,					// root of deposti payment 
CSH_INH_DAT	*inh,					// room information
CRC_CVS		*ccv,					// payment currency
CCashierDlg	*csh,					// CCashierDlg
CDatabase	*dbs,					// ODBC Database object
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int			chk=TRUE, idx=0, val;
	double		amt, sbt[2], grt[2];
	WCHAR		*erm=NULL;
	Day			dte[1];
	Lnk			dpd[1], prt[1], *csr=NULL, *cse;
	CSH_SAL_DTL	*dtl;
	CSH_TAX_DAT	*tax;
	CSH_DPS_DAT	*dps, *dpn;
	SAL_DTL		*sdt, tmp[1];
	PMT_DTL		*pdt;

	memset( prt, 0, sizeof(prt) );
	memset( dpd, 0, sizeof(dpd) );
	memset( tmp, 0, sizeof(tmp) );
	Get_Dte( dte );

	set_sal( sal->sal, csh, inh, mng );
	DBS_INT_SET(sal->sal->pid, &(val=1));
	if ( !Nul_Fld(sal->sal->id, FLD_INT) ){
		DBS_DTM_SET(sal->sal->mdt, dte);
	}
//	int				ivn[DBF_SIZ_INT];		// invoice_number

	Csh_Clc_Ttl( sbt, grt, sal->dtl, csh->m_Tax_rot, csh->m_Dps_rot, NULL, mng );

	while( csr = Nxt_Lnk( sal->dtl, csr ) ){
		dtl = Rec_Of(CSH_SAL_DTL, csr);
		//amt = DBS_DBL_VAL(dtl->dtl->slp) * DBS_DBL_VAL(dtl->dtl->qty);
		//amt = Rnd_Dec( amt, mng->bsc->adp, CEDT_ROUND_UP );
		//apl_tax( amt, DBS_INT_VAL(dtl->apl), csh->m_Tax_rot, FALSE, mng );
		if ( !Nul_Fld(dtl->dtl->id, FLD_INT) ){
			DBS_DTM_SET(dtl->dtl->mdt, dte);
		}
	}
	for ( idx=0, csr=NULL; chk && !!( csr=Nxt_Lnk( csh->m_Tax_rot, csr ) ); idx++ ){
		tax = Rec_Of(CSH_TAX_DAT, csr);
		if ( tax->amt > 0.0 ){
			if ( chk = !!( cse = Add_Lnk( sal->dtl, NULL, sizeof(CSH_SAL_DTL) ) ) ){
				dtl = Rec_Of(CSH_SAL_DTL, cse);
				sdt = dtl->dtl;
				DBS_WST_SET(sdt->opc, csh->m_Ope_cod);
				DBS_DAY_SET(sdt->sld, mng->bdt);
				DBS_INT_CPY(sdt->dsi, tax->tax->id);
				DBS_DBL_SET(sdt->qty, &(amt=1));
				DBS_DBL_SET(sdt->nmp, &tax->amt);
				DBS_DBL_SET(sdt->slp, &tax->amt);
				DBS_INT_SET(sdt->asi, &(val=0));
				DBS_INT_SET(sdt->eqi, &(val=0));
			}
		}
	}
	for ( idx=0, csr=NULL; chk && !!( csr=Nxt_Lnk( csh->m_Dps_rot, csr ) ); idx++ ){
		tax = Rec_Of(CSH_TAX_DAT, csr);
		if ( tax->amt != 0.0 && DBS_INT_VAL(tax->tax->dkd)==DRK_DISCOUNT ){
			if ( chk = !!( cse = Add_Lnk( sal->dtl, NULL, sizeof(CSH_SAL_DTL) ) ) ){
				dtl = Rec_Of(CSH_SAL_DTL, cse);
				sdt = dtl->dtl;
				DBS_WST_SET(sdt->opc, csh->m_Ope_cod);
				DBS_DAY_SET(sdt->sld, mng->bdt);
				DBS_INT_CPY(sdt->dsi, tax->tax->id);
				DBS_DBL_SET(sdt->qty, &(amt=1));
				DBS_DBL_SET(sdt->nmp, &tax->ttl);
				DBS_DBL_SET(sdt->slp, &tax->amt);
				DBS_INT_SET(sdt->asi, &(val=0));
				DBS_INT_SET(sdt->eqi, &(val=0));
			}
		}
	}
	for ( idx=0; chk && idx<memcnt(Pay_mtd); idx++ ){
		if ( *(pay+idx) > 0.0 ){
			if ( chk = !!( cse=Add_Lnk( prt, NULL, sizeof(PMT_DTL) ) ) ){
				pdt = Rec_Of(PMT_DTL, cse);
				DBS_WST_SET(pdt->opc, csh->m_Ope_cod);
				DBS_DAY_SET(pdt->pmd, mng->bdt);
				DBS_INT_CPY(pdt->cci, ccv->id);
				DBS_DBL_CPY(pdt->rte, ccv->rte);
				DBS_DBL_SET(pdt->amt, pay+idx);
				DBS_INT_SET(pdt->mtd, Pay_mtd+idx);
			}
		}
	}
	// making a history of deposit payment into deposit history
	for ( csr=NULL; chk && !!( csr=Nxt_Lnk( csh->m_Dps_rot, csr ) ); ){
		tax = Rec_Of(CSH_TAX_DAT, csr);
		if( DBS_INT_VAL(tax->tax->dkd) == DRK_DEPOSIT )	break;
	}
	if ( chk = !!csr ){
		for ( csr=NULL; chk && !!( csr=Nxt_Lnk( dpr, csr ) ); ){
			dps = *Rec_Of(CSH_DPS_DAT *, csr);
			if ( dps->pye > 0.0 ){
				if ( chk = !!( cse=Add_Lnk( dpd, NULL, sizeof(CSH_DPS_DAT) ) ) ){
					dpn = Rec_Of(CSH_DPS_DAT, cse);
					DBS_INT_CPY(dpn->rii, dps->rii);
					DBS_INT_CPY(dpn->rdi, dps->rdi);
					DBS_INT_CPY(dpn->adp, dps->adp);
					DBS_INT_CPY(dpn->cci, dps->cci);
					DBS_INT_CPY(dpn->isb, dps->isb);
					DBS_DBL_CPY(dpn->rte, dps->rte);
					DBS_WST_CPY(dpn->crc, dps->crc);
					DBS_WST_SET(dpn->dtl->opc, csh->m_Ope_cod);
					DBS_DAY_SET(dpn->dtl->sld, mng->bdt);
					DBS_INT_CPY(dpn->dtl->dsi, tax->tax->id);
					DBS_DBL_SET(dpn->dtl->qty, &(amt=1));
					DBS_DBL_SET(dpn->dtl->nmp, &(amt=0));
					DBS_DBL_SET(dpn->dtl->slp, &(amt=0));
					DBS_INT_SET(dpn->dtl->asi, &(val=0));
					DBS_INT_SET(dpn->dtl->eqi, &(val=0));

					DBS_WST_SET(dpn->pmt->opc, csh->m_Ope_cod);
					DBS_DAY_SET(dpn->pmt->pmd, mng->bdt);
					DBS_INT_CPY(dpn->pmt->cci, dps->cci);
					DBS_DBL_CPY(dpn->pmt->rte, dps->rte);
					DBS_DBL_SET(dpn->pmt->amt, &(amt = dps->pye*-1));
					DBS_INT_CPY(dpn->pmt->mtd, dps->pmt->mtd);
					DBS_INT_SET(dpn->pmt->dpk, &(val=DPK_PAYMENT));
				}
			}
		}
	}

	while ( chk && !!( csr=Nxt_Lnk( dpd, csr ) ) ){
		chk = !!Add_Lnk( inh->dps, Rec_Of(CSH_DPS_DAT, csr), sizeof(CSH_DPS_DAT) );
	}

	chk = Csh_Pay_Mnt( sal, prt, dpd, dte, &erm, dbs, mng );

	if ( !!erm ){
		csh->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
		free( erm );
	}

	Del_All( dpd );
	Del_All( prt );

	return( chk );
}

static	int	pay_dlg(				// payment dialog
CSH_SAL_DAT	*sal,					// sales_infos and sales_details
CCashierDlg	*csh,					// Cashier dialog
CSH_INH_DAT	*inh,					// inhouse room information
CDatabase	*dbs,					// ODBC Database object
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int			chk=FALSE;
	Csr			csr=NULL;
	double		pay[memcnt(Pay_mtd)];
	CPaymentDlg	*dlg;

	memset( pay, 0, sizeof(pay) );

	if ( dlg = new CPaymentDlg(csh) ){
		dlg->m_pDbs_obj = csh->m_pDbs_obj;
		dlg->m_pPms_mng = csh->m_pPms_mng;
		dlg->m_Tax_rot = csh->m_Tax_rot;
		dlg->m_Crc_cvs = csh->m_Crc_cvs;
		dlg->m_Ccv_rot = csh->m_Ccv_rot;
		dlg->m_Dps_rot = csh->m_Dps_rot;
		dlg->m_Pay_rot = sal->dtl;
		dlg->m_Dpd_rot = inh->dps;
		dlg->m_Chk_out = csh->m_Chk_out;
		//dlg->InitData();
		if ( dlg->DoModal() == IDOK ){
			pay[0] = dlg->m_Csh_amt;
			pay[1] = dlg->m_Cdt_amt;
			pay[2] = dlg->m_Cpn_amt;
			pay[3] = dlg->m_Otr_amt;
			pay[4] = dlg->m_Dps_amt;
			if ( chk = pay_mnt( sal, pay, dlg->m_Dps_pay, inh, dlg->m_Crc_cvs, csh, dbs, mng ) ){
				Csh_Dsp_Dps( csh->m_Dps_grd, inh->dps, FALSE, NULL, NULL );
			}
		}
		delete dlg;
	}
	return( chk );
}

int		Csh_All_Pay(				// all payment
CWnd		*wnd,					// parent window
CDatabase	*dbs,					// ODBC Database object
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int			chk=TRUE, idx, val, cnt;
	DWORD		ans=IDYES;
	Csr			csr;
	CSH_INH_DAT	*inh;
	CSH_SAL_DAT	sal[1];
	CCashierDlg	*csh = (CCashierDlg *)wnd;
	CTabCtrl	*tab = (CTabCtrl *)wnd->GetDlgItem(IDC_TAB_ROOM);

	memset( sal, 0, sizeof(sal) );

	idx = ((CTabCtrl *)csh->GetDlgItem(IDC_TAB_ROOM))->GetCurSel();
	inh = Rec_Of(CSH_INH_DAT, csr = Idx_Lnk( csh->m_Inh_rot, idx ));

	if ( !csh->m_Chk_out || pst_ntf( CN_CHECKOUTING, DBS_INT_VAL(inh->rdi), csh, SDK_SEND ) ){
		// reason of leave the deposit, sale up by new sales_infos
		*sal = *inh->sal;
		Set_Nul(sal->sal->id, FLD_INT, TRUE);
		Set_Nul(sal->sal->cdt, FLD_DTM, TRUE);
		Set_Nul(sal->sal->mdt, FLD_DTM, TRUE);
		DBS_INT_SET(sal->sal->ivn, &(val=0) );

		if ( csh->m_Bls_amt > 0.0 ){
			if ( chk = pay_dlg( sal, csh, inh, dbs, mng ) ){
				csh->m_Csh_grd->ResetContent();
				Del_All( inh->sal->dtl );
			}
		}
		if ( chk && csh->m_Chk_out ){
			if ( chk = pst_ntf( CN_CHECKOUT, DBS_INT_VAL(inh->rdi), csh, SDK_SEND ) ){
				Viw_Pst_Ntf( VIW_SND_BST, VWN_STS_CHANGED, (void *)(LONG_PTR)DBS_INT_VAL(inh->rei), STS_CHG_CHECKOUT, csh->GetParent() );
				Viw_Pst_Ntf( VIW_SND_SCH, VWN_RSV_CHANGED, 0, 0, csh->GetParent() );
				Viw_Pst_Ntf( VIW_SND_NAD, VWN_RSV_CHANGED, 0, 0, csh->GetParent() );
				tab->DeleteItem( idx );
				dps_inh( inh );
				Del_Lnk( csh->m_Inh_rot, csr );
				if ( idx >= ( cnt = Cnt_Lnk( csh->m_Inh_rot ) ) ){
					csh->m_Tab_idx = idx = cnt-1;
				}
				tab->SetCurSel( idx );
				if ( idx!=-1 )	Csh_Dsp_Sal( csh, mng );
				if( !cnt )	pst_ntf( CN_ALLCHECKOUT, 0, csh, SDK_POST );
			}
		}
	}

/*
	else{
		if ( csh->m_Bls_amt < 0.0 ){
			bls = fabs( csh->m_Bls_amt );
			bls = Rnd_Dec( bls, mng->bsc->adp, CEDT_ROUND_UP );
			bpr = Cma_Edt( bls, mng->bsc->adp );
			if ( ccv = csh->m_Crc_cvs ){
				bls = Rnd_Dec( bls * DBS_DBL_VAL(ccv->rte), DBS_INT_VAL(ccv->adp), CEDT_ROUND_UP );
				epr = Cma_Edt( bls, DBS_INT_VAL(ccv->adp) );
			}
			swprintf_s( buf, memcnt(buf),
				_T("Please refund %s%s%s%s%s%s to Client.\nDo you continue?"), 
					bpr ? bpr: _T("???????"), mng->bsc->cod, ccv ? _T("("): _T(""),
					ccv ? ( epr ? epr: _T("???????") ): _T(""), ccv ? DBS_WST_PTR(ccv->crc): _T(""), ccv ? _T(")"): _T("") );
			ans = csh->MessageBox( buf, NULL, MB_ICONQUESTION|MB_YESNO );
			if ( epr )	free( epr );
			if ( bpr )	free( bpr );
		}
	}
*/

	return( chk );
}

int		Csh_Cls_Sav(				// save on close
CWnd		*wnd,					// parent window
CDatabase	*dbs,					// ODBC Database object
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int			chk;
	WCHAR		*erm=NULL;
	Csr			csr=NULL;
	CCashierDlg	*csh = (CCashierDlg *)wnd;
	CSH_INH_DAT	*inh;
	Day			dte[1];

	Get_Dte( dte );

	while ( csr = Nxt_Lnk( csh->m_Inh_rot, csr ) ){
		inh = Rec_Of(CSH_INH_DAT, csr);
		if ( Nul_Fld(inh->sal->sal->id, FLD_INT) ){
			set_sal( inh->sal->sal, csh, inh, mng );
		}
	}
	if ( !( chk = Csh_Sav_Dat( csh->m_Inh_rot, FALSE, dte, &erm, dbs, mng ) ) ){
		if ( erm ){
			wnd->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
	}

	return( chk );
}

void	Csh_Prt_Pay(				// partical payment
CWnd		*wnd,					// parent window
CDatabase	*dbs,					// ODBC Database object
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int			idx, cnt=0;
	Lnk			rot[1], *csr=NULL;
	CSH_INH_DAT	*inh;
	CSH_SAL_DAT	sal[1];
	CSH_SAL_DTL	*dtl;
	CCashierDlg	*csh = (CCashierDlg *)wnd;

	memset( sal, 0, sizeof(sal) );
	memset( rot, 0, sizeof(rot) );

	idx = ((CTabCtrl *)csh->GetDlgItem(IDC_TAB_ROOM))->GetCurSel();
	inh = Rec_Of(CSH_INH_DAT, Idx_Lnk( csh->m_Inh_rot, idx ));

	for ( idx=1; idx<=csh->m_Csh_grd->GetLineCount( FALSE ); idx++ ){
		if ( csh->m_Csh_grd->GetCheck(CLM_SEL, idx) ){
			csr = Add_Lnk( rot, (CSH_SAL_DTL *)(LONG_PTR)csh->m_Csh_grd->GetData(CLM_DAT, idx), sizeof(CSH_SAL_DTL) );
			if ( !csr )	break;
			cnt++;
		}
	}
	if ( !!cnt &&  cnt < csh->m_Csh_grd->GetLineCount( FALSE ) ){
		*sal->dtl = *rot;
		if ( pay_dlg( sal, csh, inh, dbs, mng ) ){
			for ( idx=1; idx<csh->m_Csh_grd->GetLineCount(FALSE); idx++){
				if ( csh->m_Csh_grd->GetCheck(CLM_SEL, idx) ){
					dtl = (CSH_SAL_DTL *)(LONG_PTR)csh->m_Csh_grd->GetData(CLM_DAT, idx);
					csh->m_Csh_grd->DelRow(idx);
					Del_Lnk( inh->sal->dtl, Csr_Of(dtl) );
				}
			}
			csh->m_Csh_grd->Redraw();
		}
	}
	else if ( cnt ){
		Csh_All_Pay( wnd, dbs, mng );
	}

	Del_All( rot );
}

static	double	dsp_dfd(			// displaying PayFromDeposit
CWnd		*stc,					// window of displaying target
CSimpleGrid *grd,					// deposit grid
CRC_CVS		*ccv )					// payment currency
{
	int			row;
	double		amt=0.0;
	WCHAR		*ptr;
	CSH_DPS_DAT	*dps;

	for ( row=1; row<=grd->GetLineCount(FALSE); row++ ){
		dps = (CSH_DPS_DAT *)(LONG_PTR)grd->GetData( CLM_DAT, row );
		amt += dps->pay;
	}
	if ( ptr = Cma_Edt( amt, DBS_INT_VAL(ccv->adp) ) ){
		stc->SetWindowTextW( ptr );
		free( ptr );
	}
	return( amt );
}

int		Csh_Use_Dps(				// using a deposit
CSimpleGrid	*grd,					// grid of deposit
WCHAR		*txt,					// Inputted text
int			clm,					// clicked column
int			row,					// clicked row
CWnd		*wnd,					// parent window
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int			chk;
	double		amt;
	WCHAR		buf[40], *ptr;
	CRC_CVS		ctp[1];
	CSH_DPS_DAT	*dps;
	CPaymentDlg	*pay = (CPaymentDlg *)wnd;

	memset( ctp, 0, sizeof(ctp) );
	wcscpy_s( buf, memcnt(buf), txt );
	dps = (CSH_DPS_DAT *)(LONG_PTR)grd->GetData( CLM_DAT, row );
	amt = _wtof( Prg_Cma( buf ) );
	if ( chk = amt <= dps->bls ){
		DBS_INT_CPY(ctp->adp, dps->adp); 
		DBS_DBL_CPY(ctp->rte, dps->rte); 
		dps->pay = amt;
		dps->pye = clc_exc( amt, ctp, pay->m_Crc_cvs, pay->m_Crc_bse );
		if ( !!( ptr = Cma_Edt( dps->bls - dps->pay, DBS_INT_VAL(pay->m_Crc_cvs->adp) ) ) ){
			grd->SetCellText( CLM_BLE, row, ptr );
			free( ptr );
		}
//if ( !!( ptr = Cma_Edt( dps->pye, DBS_INT_VAL(dps->adp) ) ) ){
//	grd->SetCellText( 1, row, ptr );
//	free( ptr );
//}
		grd->SetCellText( clm, row, txt );
		pay->m_Dps_amt = dsp_dfd( pay->GetDlgItem(IDC_STATIC_PFDEPOSIT), grd, pay->m_Crc_cvs );
	}
	else{
		pay->MessageBox( _T("Can not input amount that is more than balance."),
											NULL, MB_ICONEXCLAMATION|MB_OK );
	}
	return( chk );
}

void	Csh_Chk_All(				// set all checkbox on Csh_grd
int			chk,					// check status
CWnd		*wnd,					// parent window
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int			row;
	CCashierDlg	*csh = (CCashierDlg *)wnd;

	for ( row=1; row<=csh->m_Csh_grd->GetLineCount(FALSE); row++ ){
		csh->m_Csh_grd->SetCheck( CLM_SEL, row, chk );
	}
}

static int 	tfr_cmp(		// compeare the products data for transfer (ascending sort) 
const void	*dpt,			// key data 1
const void	*spt )			// key data 2
{
	int		cmp;
	CSH_SAL_DTL	*dst = Rec_Of(CSH_SAL_DTL, *((Csr *)dpt ) );
	CSH_SAL_DTL	*src = Rec_Of(CSH_SAL_DTL, *((Csr *)spt ) );
	Day		sdt[1], ddt[1];

	// move to bottom (decending) if sales_date is null
	*sdt = *DBS_DTM_PTR(src->dtl->sdt);
	*ddt = *DBS_DTM_PTR(dst->dtl->sdt);
	if ( Nul_Fld(src->dtl->sdt, FLD_DTM) )	sdt->yer = 9999, sdt->mth=12, sdt->day=31;
	if ( Nul_Fld(dst->dtl->sdt, FLD_DTM) )	ddt->yer = 9999, sdt->mth=12, sdt->day=31;

	cmp = Cmp_Dte( ddt, sdt );
	if ( !cmp )	cmp = DBS_INT_VAL(dst->dtl->id) - DBS_INT_VAL(src->dtl->id);
	return( cmp );
}

static	int	trs_fer(				// execute the transfer
CSH_INH_DAT	*dst,					// destination room
CSH_INH_DAT	*src,					// source room
CCashierDlg	*csh,					// parent window
CDatabase	*dbs,					// ODBC Database object
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int			chk=TRUE, row;
	Csr			csr;
	CSH_SAL_DAT	*ssl=src->sal, *dsl=dst->sal;
	CSH_SAL_DTL	*dtl;
	CSimpleGrid	*grd=csh->m_Csh_grd;

	for ( row=1; chk && row<=grd->GetLineCount(FALSE); ){
		if ( !!grd->GetCheck( CLM_SEL, row ) ){
			dtl = (CSH_SAL_DTL *)(LONG_PTR)grd->GetData( CLM_DAT, row );
			if ( chk = !!( csr = Add_Lnk( dsl->dtl, dtl, sizeof(CSH_SAL_DTL) ) ) ){
				Del_Lnk( ssl->dtl, Csr_Of(dtl) );
				dtl = Rec_Of(CSH_SAL_DTL, csr);
				if ( !!DBS_INT_VAL(dtl->dtl->id) ){
					Set_Nul(dtl->dtl->mdt, FLD_DTM, FALSE);
				}
				DBS_INT_CPY(dtl->dtl->sii, dsl->sal->id);
			}
			if ( chk )	grd->DelRow( row );
		}
		else{
			row++;
		}
	}
	if ( chk )	chk = Sot_Lnk( dsl->dtl, tfr_cmp );
	if ( chk )	grd->Redraw();

	return( chk );
}

int		Csh_Trs_Far(
CWnd		*wnd,					// parent window
CDatabase	*dbs,					// ODBC Database object
PMS_MNG		*mng )					// Hotel Property Management Segment
{
	int				chk, idx, cnt=0;
	Lnk				rot[1], *csr=NULL;
	CSH_INH_DAT		*inh, *ptr;
	CTransferDlg	*dlg;
	CCashierDlg	*csh = (CCashierDlg *)wnd;

//	memset( sal, 0, sizeof(sal) );
	memset( rot, 0, sizeof(rot) );

	idx = ((CTabCtrl *)csh->GetDlgItem(IDC_TAB_ROOM))->GetCurSel();
	chk = !!( inh = Rec_Of(CSH_INH_DAT, Idx_Lnk( csh->m_Inh_rot, idx ) ) );
	while ( chk && !!( csr=Nxt_Lnk( csh->m_Inh_rot, csr ) ) ){
		ptr = Rec_Of(CSH_INH_DAT, csr);
		if ( DBS_INT_VAL(inh->rdi) != DBS_INT_VAL(ptr->rdi) ){
			chk = !!Add_Lnk( rot, &ptr, sizeof(CSH_INH_DAT *) );
		}
	}
	if ( chk ){
		for ( idx=1; idx<=csh->m_Csh_grd->GetLineCount(FALSE); idx++ ){
			if ( !!csh->m_Csh_grd->GetCheck( CLM_SEL, idx ) )	break;
		}
		if ( idx<=csh->m_Csh_grd->GetLineCount(FALSE) ){
			if ( chk = !!( dlg = new CTransferDlg(csh) ) ){
				dlg->m_Dst_rot = rot;
				if ( dlg->DoModal()==IDOK ){
					chk = trs_fer( dlg->m_Inh_dat, inh, csh, dbs, mng );
				}
				delete	dlg;
			}
		}
		else{
			csh->MessageBox( _T("Please select item which you want to transfer"), NULL, MB_ICONINFORMATION|MB_OK );
		}
	}

	Del_All( rot );
	return( chk );
}

void	Csh_Dps_Ihl(				// dispose the room list
CWnd		*wnd )					// detination pointer
{
	Csr			csr=NULL;
	CCashierDlg	*csh = (CCashierDlg *)wnd;

	csr = Nxt_Lnk( csh->m_Inh_rot, NULL );

	while( csr ){
		dps_inh ( Rec_Of(CSH_INH_DAT, csr) );
		csr = Del_Lnk( csh->m_Inh_rot, csr );
	}
}
