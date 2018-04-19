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
#include "..\CheckinView.h"
#include "..\RoomSelectDlg.h"
#include "bst_sql.h"
#include "cin_sql.h"
#include "cin_fnc.h"

static	WCHAR	*Grd_ttl[] =	{	{ _T("C/I")	},
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
									{  84, GAL_LFT, GCT_EDT,  12 },		// R No.
									{  90, GAL_LFT, GCT_NML,  12 },		// Arrv.
									{  90, GAL_LFT, GCT_NML,  12 },		// Dept.
									{  48, GAL_CTR, GCT_NML,  12 } };	// Ad/Ch

#define	SEL_ROW_(x)				((CCheckinView *)wnd)->m_Sel_row
#define	CIN_INF_(x)				((CCheckinView *)wnd)->m_Cin_inf
#define	RDN_DTL_(x)				((CCheckinView *)wnd)->m_Rdn_dtl
#define	RDN_ROW_(x)				((CCheckinView *)wnd)->m_Rdn_row


#define	dis_cin		(RSV_EST_OLD|RSV_EST_INH|RSV_EST_COT|RSV_EST_CXL|RSV_EST_NSW)

CSimpleGrid		*Cin_Mak_Grd(		// 予約明細グリッド作成
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
	gdf->sel = GSL_NON;
	gdf->hdr = TRUE;
	gdf->fsa = TRUE;

	grd = new CSimpleGrid();
	grd->Create( gdf, *rct, id, wnd );
	for ( idx=0; idx<(int)memcnt(Grd_ttl); idx++ ){
		grd->SetCellText( idx, 0, Grd_ttl[idx] );
	}

	return( grd );
}

static	int		can_cin(						// Confirm possible to checking in
CIN_DTL_DAT	*dtl )								// 予約詳細データ
{
	int			cin;

	cin  = !(dis_cin&dtl->est);
	cin &= !Nul_Fld(dtl->eii, FLD_INT);

	return( cin );
}

static	void	dsp_dtl(						// 予約詳細表示
int			row,								// target row
CIN_DTL_DAT	*dtl,								// 予約詳細データ
CIN_INF_DAT	*inf,								// NULL when new creation
CWnd		*wnd,								// 表示先ウィンドウ
PMS_MNG		*mng )								// Property management System management structure
{
	WCHAR				buf[520];
	CSimpleGrid			*grd = (CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_CIN));

	dtl->smr  = !Nul_Fld(inf->cii, FLD_INT) && !Nul_Fld(dtl->dtl->cii, FLD_INT);
	dtl->smr &= DBS_INT_VAL(inf->cii) == DBS_INT_VAL(dtl->dtl->cii);

	dtl->est = Viw_Dtl_Sts( inf->rsv, dtl->dtl->dpt, dtl->dtl->cid, dtl->dtl->cod, dtl->dtl->cxl, mng );

	swprintf_s( buf, memcnt(buf), _T("%s %s"), EDT_DBF_WST(dtl->fnm,_T("")), EDT_DBF_WST(dtl->lnm,_T("")) );
	grd->SetCellText( CIN_CLM_NAM, row, buf );													// Name
	grd->SetCellText( CIN_CLM_STS, row, Viw_Sts_Ope( NULL, dtl->est ) );							// Status
	grd->SetCellText( CIN_CLM_RTP, row, EDT_DBF_WST(dtl->tnm,_T("")) );							// R.Typ
	grd->SetCellText( CIN_CLM_RNB, row, EDT_DBF_WST(dtl->rno,_T("")) );							// R.No.
	grd->SetCellText( CIN_CLM_ARV, row, EDT_DBF_DAY(buf, memcnt(buf), dtl->dtl->arv) );			// Arrv
	grd->SetCellText( CIN_CLM_DPT, row, EDT_DBF_DAY(buf, memcnt(buf), dtl->dtl->dpt) );			// Dept
	swprintf_s( buf, memcnt(buf), _T("%d/%d"), DBS_INT_VAL(dtl->dtl->psn), DBS_INT_VAL(dtl->dtl->chn) );
	grd->SetCellText( CIN_CLM_PSN, row, buf );													//Ad/Ch
	grd->SetEnable( 0, row, can_cin(dtl) );
}

static	void	dsp_dat(						// 予約情報表示
CIN_INF_DAT	*inf,								// NULL when new creation
CWnd		*wnd,								// 表示先ウィンドウ
PMS_MNG		*mng )								// Property management System management structure
{
	WCHAR				buf[520];
	CIN_DTL_DAT			*dtl;	
	Csr					csr=NULL;
	int					row=1, rdi=0;
	CSimpleGrid			*grd = (CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_CIN));

	inf->est = Viw_Inf_Sts( inf->rsv, mng );

	wnd->GetDlgItem(IDC_STATIC_BOOKINGID)->SetWindowText( EDT_DBF_INT(buf, memcnt(buf), inf->rsv->rvn, Edt_Num_Nml) );
	wnd->GetDlgItem(IDC_STATIC_EXTBOOKINGID)->SetWindowText( EDT_DBF_INT(buf, memcnt(buf), inf->rsv->ern, Edt_Num_Nml) );
	wnd->GetDlgItem(IDC_STATIC_CLIENTTYPE)->SetWindowText( DBS_INT_VAL(inf->ckd)==CLK_PERSONAL ? _T("Personal"): _T("Group") );
	wnd->GetDlgItem(IDC_STATIC_BOOKINGPATH)->SetWindowText( Viw_Bkp_Nam( DBS_INT_VAL(inf->rsv->bfr), DBS_INT_VAL(inf->rsv->aii), mng ) ); 
	wnd->GetDlgItem(IDC_STATIC_CLIENTRANK)->SetWindowText( Viw_Crk_Nam( DBS_INT_VAL(inf->crk), mng ) ); 
	wnd->GetDlgItem(IDC_STATIC_FIRSTARRIVAL)->SetWindowText( EDT_DBF_DAY(buf, memcnt(buf), inf->rsv->arv) );
	wnd->GetDlgItem(IDC_STATIC_LASTDEPARTURE)->SetWindowText( EDT_DBF_DAY(buf, memcnt(buf), inf->rsv->dpt) );
	swprintf_s( buf, memcnt(buf), _T("%s %s"), EDT_DBF_WST(inf->fnm, _T("") ), EDT_DBF_WST(inf->lnm, _T("") ) );
	wnd->GetDlgItem(IDC_STATIC_BOOKINGNAME)->SetWindowText( buf );
	wnd->GetDlgItem(IDC_STATIC_TELEPHONE1)->SetWindowText( EDT_DBF_WST(inf->tel1,_T("")) );
	wnd->GetDlgItem(IDC_STATIC_TELEPHONE2)->SetWindowText( EDT_DBF_WST(inf->tel2,_T("")) );

	((CCustomStatic *)(wnd->GetDlgItem(IDC_STATIC_CLIENTTYPE)))->SetBGColor( DBS_INT_VAL(inf->ckd)==CLK_PERSONAL ? STC_PSB: STC_GPB );
	((CCustomStatic *)(wnd->GetDlgItem(IDC_STATIC_CLIENTTYPE)))->SetFTColor( DBS_INT_VAL(inf->ckd)==CLK_PERSONAL ? STC_PSF: STC_GPF );

	grd->ResetContent();

	grd->AddRow( Cnt_Lnk(inf->dtl) );
	while( csr = Nxt_Lnk( inf->dtl, csr ) ){
		dtl = Rec_Of(CIN_DTL_DAT, csr);
		dsp_dtl( row, dtl, inf, wnd, mng );
		Set_Nul( dtl->dtl->mdt, FLD_DTM, TRUE );
		grd->SetData( 1, row++, (int)(LONG_PTR)dtl );
	}
	grd->Redraw();
	grd->SetSelectCell( 0, 1 );
	grd->InputEnable( TRUE );
}

int		Cin_Get_Dat(						// Getting the existing data
int				rid,						// resservation_infos id
CWnd			*wnd,						// Parent window
CDatabase		*dbs,						// ODBC Database object
PMS_MNG			*mng )						// Hotel Property Management Structure
{
	WCHAR			*erm=NULL;
	int				chk;

	memset( CIN_INF_(wnd), 0, sizeof(CIN_INF_DAT) );
	if ( chk = Cin_Get_Inf( CIN_INF_(wnd), rid, &erm, dbs, mng ) ){
		if ( erm )		free( erm );
		if ( chk = Cin_Get_Dtl( CIN_INF_(wnd)->dtl, rid, &erm, dbs, mng ) ){
			((CChildFrame *)(wnd->GetParent()))->m_Rsv_id = rid;
			dsp_dat( CIN_INF_(wnd), wnd, mng );
		}
	}
	if ( erm ){
		wnd->MessageBox(erm/*_T("Faild to get a reservation data!")*/, 0, MB_ICONSTOP|MB_OK );
		free( erm );
	}
	return( chk );
}

void		Cin_Cbx_Clk(					// checkin checkbox click
int				row,						// ckicked row
CWnd			*wnd,						// Parent window
PMS_MNG			*mng )						// Hotel Property Management Structure
{
	CSimpleGrid		*grd = (CSimpleGrid *)((CCheckinView *)wnd)->GetDlgItem(IDC_GRD_CIN);
	CIN_DTL_DAT		*dtl;
	int				chk;

	if ( !!( dtl = (CIN_DTL_DAT *)(LONG_PTR)grd->GetData( 1, row ) ) ){
		chk = grd->GetCheck( 0, row );
		if ( chk )	chk &= can_cin( dtl );
		grd->SetCheck( 0, row, chk );
		dtl->cin = chk;
	}
}

void		Cin_Chk_All(					// CheckAll checkbox click
CWnd			*wnd,						// Parent window
PMS_MNG			*mng )						// Hotel Property Management Structure
{
	CIN_DTL_DAT	*dtl;
	int			row;
	int			chk = ((CButton *)(wnd->GetDlgItem(IDC_CHECK_CHECKALL)))->GetCheck();
	CSimpleGrid	*grd = (CSimpleGrid *)((CCheckinView *)wnd)->GetDlgItem(IDC_GRD_CIN);

	for ( row=1; row<=grd->GetLineCount( FALSE ); row++ ){
		if ( !!( dtl = (CIN_DTL_DAT *)(LONG_PTR)grd->GetData( 1, row ) ) ){
			grd->SetCheck( 0, row, dtl->cin = (chk & can_cin( dtl )) );
		}
	}
}

void		Cin_Exc_Clk(					// Execute checkin button click
CWnd			*wnd,						// Parent window
CDatabase		*dbs,						// ODBC Database object
PMS_MNG			*mng )						// Hotel Property Management Structure
{
	WCHAR		*erm=NULL;
	Csr			csr=NULL;
	CIN_DTL_DAT	*dtl;
	int			chk;

	while ( csr=Nxt_Lnk( CIN_INF_(wnd)->dtl, csr ) ){
		if ( Rec_Of(CIN_DTL_DAT, csr)->cin )	break;
	}

	if ( !!csr ){
		if ( chk = Cin_Sav_Dat( wnd, dbs, mng ) ){
			if ( chk = Cin_Mlt_Cin( CIN_INF_(wnd)->dtl, &erm, dbs, mng ) ){
				for( csr=NULL; csr=Nxt_Lnk( CIN_INF_(wnd)->dtl, csr ); ){
					dtl = Rec_Of(CIN_DTL_DAT, csr);
					if ( dtl->cin ){
						Viw_Pst_Ntf( VIW_SND_BST, VWN_STS_CHANGED, (void *)(LONG_PTR)DBS_INT_VAL(dtl->rei), STS_CHG_CHECKIN, wnd );
						Viw_Pst_Ntf( VIW_SND_NAD, VWN_STS_CHANGED, (void *)(LONG_PTR)DBS_INT_VAL(dtl->rei), STS_CHG_CHECKIN, wnd );
						Viw_Pst_Ntf( VIW_SND_SCH, VWN_RSV_CHANGED, NULL, DBS_INT_VAL(dtl->dtl->rii), wnd );
					}
				}

				Del_All( CIN_INF_(wnd)->dtl );
				if ( chk = Cin_Get_Dtl( CIN_INF_(wnd)->dtl, DBS_INT_VAL(CIN_INF_(wnd)->rsv->id), &erm, dbs, mng ) ){
					dsp_dat( CIN_INF_(wnd), wnd, mng );
				}
			}
			if ( !!erm ){
				wnd->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
				free( erm );
			}
		}
	}
	else{
		wnd->MessageBox( _T("Checkin target not selected"), NULL, MB_ICONINFORMATION|MB_OK );
	}
}

void	Cin_Ctx_Mnu(			// Open context menu on checkin grid
CWnd		*wnd,				// Search view
int			msx,				// Mouse point x
int			msy,				// Mouse point y
int			row )				// Selected row
{
	CMenu		mnu, *pmn=NULL;
	POINT		pnt[1] = { msx, msy };

	if ( row > 0 ){
		RDN_ROW_(wnd) = row;
		RDN_DTL_(wnd) = Rec_Of(CIN_DTL_DAT, Idx_Lnk( CIN_INF_(wnd)->dtl, row-1 ) ); 
		mnu.LoadMenu(IDR_MENU_CIN_ROM);
		pmn = mnu.GetSubMenu(0);
		// mouse on the panel
		wnd->ClientToScreen( pnt );
		pmn->TrackPopupMenu( TPM_TOPALIGN/*TPM_LEFTALIGN*/|TPM_RIGHTBUTTON, pnt->x, pnt->y, wnd, NULL );
	}
}

static	int		rom_avl(				// confirmation room avalable on inner same reservation
CIN_DTL_DAT	*own,						// room user data
int			eii,						// equipment_info_id
Day			*usd,						// use start date
Day			*ued,						// use end date
CWnd		*wnd,						// Parent window
CDatabase	*dbs,						// ODBC Database object
PMS_MNG		*mng )						// Hotel Property management System Structure
{
	int			chk=TRUE, cnt, avl=FALSE;
	WCHAR		*erm=NULL;
	Csr			csr=NULL, cse=NULL;
	CIN_INF_DAT	*inf = CIN_INF_(wnd);
	CIN_DTL_DAT	*dtl;
	Day			*dsd, *ded;

	// At first confirm avalable on inner(own reservation) data 
	while ( csr=Nxt_Lnk( inf->dtl, csr ) ){
		dtl = Rec_Of(CIN_DTL_DAT, csr);
		if ( eii==DBS_INT_VAL(dtl->eii) ){
			if ( DBS_INT_VAL(dtl->dtl->id) != DBS_INT_VAL(own->dtl->id) ){
				dsd = DBS_DAY_PTR(dtl->usd);
				ded = DBS_DAY_PTR(dtl->ued);
				if ( ( Cmp_Dte( usd, dsd ) >= 0 && Cmp_Dte( usd, ded ) < 0 ) 
					|| ( Cmp_Dte( ued, dsd ) > 0 && Cmp_Dte( ued, ded ) <= 0 ) ){
						break;
				}
			}
		}
	}
	if ( !csr ){
		if ( chk = ( cnt = Rsv_Rom_Avl( eii, DBS_INT_VAL(own->dtl->id), usd, ued, &erm, dbs, mng ) ) != -1 ){
			avl = cnt==0;
		}
	}
	if ( erm ){
		wnd->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
		free( erm );
	}
	return( chk ? avl: -1 );
}

void		Cin_Rom_Nbr(				// Room number edit complete event
WCHAR		*txt,						// inputted text
int			row,						// row on the grid
CWnd		*wnd,						// Parent window
CDatabase	*dbs,						// ODBC Database object
PMS_MNG		*mng )						// Hotel Property management System Structure
{
	int			avl;
	Csr			csr=NULL;
	EQP_INF		*eqi;
	EQP_TYP		*eqt;
	CIN_DTL_DAT	*dtl;
	WCHAR		*erm=NULL, *err=NULL;
	CSimpleGrid	*grd=(CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_CIN));
	Day			*usd, *ued;

	dtl = (CIN_DTL_DAT *)(LONG_PTR)grd->GetData( CIN_CLM_DAT, row );

	if ( !!wcslen( txt ) ){
		while( csr=Nxt_Lnk( mng->eil, csr ) ){
			eqi = Rec_Of(EQP_INF, csr);
			if ( !wcscmp( DBS_WST_PTR(eqi->eco), txt ) ){
				break;
			}
		}
		if ( !!csr ){			// found!
			for( csr=NULL; csr=Nxt_Lnk( mng->etl, csr ); ){
				eqt = Rec_Of(EQP_TYP, csr);
				if ( DBS_INT_VAL(eqt->id)==DBS_INT_VAL(eqi->eti) ){
					break;
				}
			}
			eqt = !!csr ? eqt: NULL;
			usd=DBS_DAY_PTR(dtl->usd);
			ued=DBS_DAY_PTR(dtl->ued);
			if ( ( avl = rom_avl( dtl, DBS_INT_VAL(eqi->id), usd, ued, wnd, dbs, mng ) ) != -1 ){
				if ( avl ){
					DBS_INT_CPY(dtl->eii, eqi->id);
					DBS_WST_CPY(dtl->rno, eqi->eco);
					if ( eqt ){
						DBS_INT_CPY(dtl->eti, eqt->id);
						DBS_WST_CPY(dtl->tnm, eqt->nam);
					}
					grd->SetCellText( CIN_CLM_RNB, row, txt );
					grd->SetCellText( CIN_CLM_RTP, row, DBS_WST_PTR(eqt->nam) );
					Set_Nul(dtl->dtl->mdt, FLD_DTM, FALSE);
				}
				else{
					err = _T("Room is not avalable on this period");
				}
			}
		}
		else{
			err = _T("Room number not found");
		}
		if ( err ){
			wnd->MessageBox( err, NULL, MB_ICONEXCLAMATION|MB_OK );
		}
	}
	else{				// room is not avalable
		DBS_INT_SET(dtl->eii, (int *)NULL);
		DBS_WST_SET(dtl->rno, (WCHAR *)NULL);
		DBS_INT_SET(dtl->eti, (int *)NULL);
		DBS_WST_SET(dtl->tnm, (WCHAR *)NULL);
		grd->SetCellText( CIN_CLM_RNB, row, _T("") );
		grd->SetCellText( CIN_CLM_RTP, row, _T("") );
		Set_Nul(dtl->dtl->mdt, FLD_DTM, FALSE);
	}
}

void	Cin_Rom_Sel(				// Context menu of charge detail
CWnd			*wnd,				// Parent window
PMS_MNG			*mng )				// Hotel Property Management Structure
{
	int				row=RDN_ROW_(wnd);
	CIN_INF_DAT		*inf = CIN_INF_(wnd);
	CIN_DTL_DAT		*dtl;
	CRoomSelectDlg	*dlg;
//	CComboBox		*plc = (CComboBox *)(wnd->GetDlgItem(IDC_COMBO_RATEPLANNAME));
	CSimpleGrid		*grd = (CSimpleGrid *)(wnd->GetDlgItem(IDC_GRD_CIN));

	dtl = Rec_Of(CIN_DTL_DAT, Idx_Lnk( inf->dtl, row-1 ) );
	if ( dlg = new CRoomSelectDlg( wnd ) ){
		dlg->m_Bok_pth = Nul_Fld(inf->rsv->bfr, FLD_INT) ? 0: DBS_INT_VAL(inf->rsv->bfr);
		dlg->m_Agt_iid = Nul_Fld(inf->rsv->aii, FLD_INT) ? 0: DBS_INT_VAL(inf->rsv->aii);
		dlg->m_Req_eti = Nul_Fld(dtl->dtl->eti, FLD_INT) ? 0: DBS_INT_VAL(dtl->dtl->eti);
		dlg->m_Rsv_did = DBS_INT_VAL(dtl->dtl->id);
		dlg->m_Eqp_tid = !!dtl && Nul_Fld(dtl->eti, FLD_INT) ? 0: DBS_INT_VAL(dtl->eti);
		dlg->m_Equ_iid = !!dtl && Nul_Fld(dtl->eii, FLD_INT) ? 0: DBS_INT_VAL(dtl->eii);
		dlg->m_Use_std = DBS_DAY_PTR(dtl->usd);
		dlg->m_Use_edd = DBS_DAY_PTR(dtl->ued);
		wcscpy_s( dlg->m_Pln_nam, memcnt(dlg->m_Pln_nam), EDT_DBF_WST(dtl->rtn,_T("")) );

		if ( dlg->DoModal()==IDOK ){
			DBS_INT_CPY(dtl->eii, dlg->m_Eqp_inf->eqi->id);
			DBS_WST_CPY(dtl->rno, dlg->m_Eqp_inf->eqi->eco);
			DBS_INT_CPY(dtl->eti, dlg->m_Eqp_inf->eti );
			DBS_WST_CPY(dtl->tnm, dlg->m_Eqp_inf->tnm );
			grd->SetCellText( CIN_CLM_RNB, row, DBS_WST_PTR(dlg->m_Eqp_inf->eqi->eco) );
			grd->SetCellText( CIN_CLM_RTP, row, DBS_WST_PTR(dlg->m_Eqp_inf->tnm) );
			Set_Nul(dtl->dtl->mdt, FLD_DTM, FALSE);
		}
		delete	dlg;
	}
}

int		Cin_Sav_Dat(			// saving data
CWnd		*wnd,				// Parent window
CDatabase	*dbs,				// ODBC Database object
PMS_MNG		*mng )				// Hotel Property Management Structure
{
	int			chk, cnt=0, val=0;
	Lnk			rot[1], *csr=NULL, *csn;
	Day			dte[1];
	WCHAR		*erm=NULL;
	CIN_INF_DAT	*inf = CIN_INF_(wnd);
	CIN_DTL_DAT	*dtl;
	BST_ROM_STS	*sts;

	Get_Dte( dte );
	memset( rot, 0, sizeof(rot) );

	if ( chk = Cin_Sav_Dtl( inf->dtl, dte, &erm, dbs, mng ) ){
		Viw_Pst_Ntf( VIW_SND_SCH, VWN_RSV_CHANGED, NULL, DBS_INT_VAL(inf->rsv->id), wnd );
		Viw_Pst_Ntf( VIW_SND_NAD, VWN_RSV_CHANGED, NULL, DBS_INT_VAL(inf->rsv->id), wnd );
		while ( csr=Nxt_Lnk( inf->dtl, csr ) ){
			dtl = Rec_Of(CIN_DTL_DAT, csr);
			if ( !Nul_Fld( dtl->dtl->mdt, FLD_DTM ) ){
				Set_Nul( dtl->dtl->mdt, FLD_DTM, TRUE );
				if ( csn = Add_Lnk( rot, NULL, sizeof(BST_ROM_STS) ) ){
					sts = Rec_Of(BST_ROM_STS, csn);
					//*(sts->req) = *req->req;
					DBS_INT_CPY(sts->req->id, dtl->rei);
					DBS_WST_CPY(sts->req->opc, dtl->dtl->opc);
					DBS_INT_CPY(sts->req->rii, inf->rsv->id);
					DBS_INT_CPY(sts->req->rdi, dtl->dtl->id);
					DBS_INT_CPY(sts->req->eti, dtl->eti);
					DBS_INT_CPY(sts->req->eii, dtl->eii);
					DBS_DAY_CPY(sts->req->usd, dtl->usd);
					DBS_DAY_CPY(sts->req->ued, dtl->ued);
					DBS_INT_CPY(sts->rvn, inf->rsv->rvn);
					DBS_WST_CPY(sts->ern, inf->rsv->ern);
					DBS_INT_CPY(sts->bfr, inf->rsv->bfr);
					DBS_INT_CPY(sts->aii, inf->rsv->aii);
					DBS_INT_CPY(sts->eci, dtl->dtl->eci);
					DBS_INT_CPY(sts->esi, dtl->esi);
					DBS_INT_CPY(sts->lco, dtl->dtl->lco);
					DBS_WST_CPY(sts->fnm, dtl->fnm);
					DBS_WST_CPY(sts->lnm, dtl->lnm);
					DBS_DAY_CPY(sts->arv, dtl->dtl->arv);
					DBS_DAY_CPY(sts->dpt, dtl->dtl->dpt);
					DBS_INT_SET(sts->cin, &val);
					DBS_INT_SET(sts->cin, &val);
					DBS_WST_CPY(sts->rno, dtl->rno);
					DBS_WST_CPY(sts->rtn, dtl->tnm);
					cnt++;
				}
				else{
					break;
				}
			}
		}
		if ( !csr && !!cnt ){
			Viw_Snd_Ntf( VIW_SND_BST, VWN_RSV_CHANGED, rot, cnt, wnd );
		}
		else{
			Del_All( rot );
		}
	}
	else{
		if ( erm ){
			wnd->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
	}

	return( chk );
}

void		Cin_Dps_Cin(					// Dispose the checkin information data 
CIN_INF_DAT	*inf )							// target data pointer
{
	Del_All( inf->dtl );
}