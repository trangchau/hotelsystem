/*
	—\–ñŒŸõˆ—ŠÖ”
*/

//#include "windows.h"
#include "..\..\Common\bsc_def.h"
#include "..\..\Common\lnk_mng.h"
#include "..\..\Common\dbs_mng.h"
#include "..\NightAuditView.h"
#include "..\HotelPMS.h"
#include "..\MainFrm.h"
#include "..\ChildFrm.h"
#include "..\..\pms_def.h"
#include "..\ReservationView.h"
#include "..\CheckinView.h"
#include "..\InhouseView.h"
#include "..\CheckoutView.h"
#include "nad_fnc.h"
#include "nad_sql.h"
#include "bst_sql.h"
#include "viw_cmn.h"

int		Nad_Get_Dat(				// getting worning data
CWnd		*wnd,					// parent window( CNigtAuditView )
CDatabase	*dbs,					// ODBC Databae object
PMS_MNG		*mng )					// Hotel Property Management System structure
{
	int				chk=TRUE, row=1;
	Csr				csr=NULL;
	NAD_WRN_LST		*wrn;
	WCHAR			*erm=NULL, buf[1024];
	CNightAuditView	*viw=(CNightAuditView *)wnd;
	CSimpleGrid		*grd=(CSimpleGrid *)viw->GetDlgItem(IDC_GRD_WRN);

	Del_All( viw->m_Wrn_rot );
	grd->ResetContent();

	if ( chk = Nad_Wrn_Lst( viw->m_Wrn_rot, viw->m_Ope_cod, viw->m_Brc_cod, viw->m_Fcl_cod, &erm, dbs, mng ) ){
		grd->AddRow( Cnt_Lnk( viw->m_Wrn_rot ) );
		while ( csr=Nxt_Lnk( viw->m_Wrn_rot, csr ) ){
			wrn = Rec_Of(NAD_WRN_LST, csr);
			grd->SetCellText( NAD_CLM_STS, row, wrn->sts==NAS_NCI ? _T("NoCheckin"): _T("NoCheckout") );
			swprintf_s( buf, memcnt(buf), _T("%d"), DBS_INT_VAL(wrn->rvn) );
			grd->SetCellText( NAD_CLM_RVN, row, buf );
			grd->SetCellText( NAD_CLM_ERN, row, EDT_DBF_WST(wrn->ern,_T("")) );
			grd->SetCellText( NAD_CLM_RMN, row, EDT_DBF_WST(wrn->rno,_T("")) );
			swprintf_s( buf, memcnt(buf), _T("%s %s"), EDT_DBF_WST(wrn->dfn,_T("")), EDT_DBF_WST(wrn->dln,_T("")) );
			grd->SetCellText( NAD_CLM_GNM, row, buf );
			swprintf_s( buf, memcnt(buf), _T("%s %s"), EDT_DBF_WST(wrn->rfn,_T("")), EDT_DBF_WST(wrn->rln,_T("")) );
			grd->SetCellText( NAD_CLM_RNM, row, buf );
			grd->SetCellText( NAD_CLM_OTR, row, _T("") );
			grd->SetData( NAD_CLM_DAT, row++, (int)(LONG_PTR)wrn );
		}
		grd->Redraw();
	}
	else{
		if ( erm ){
			viw->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
	}
	wnd->GetDlgItem(IDOK)->EnableWindow( !Cnt_Lnk(viw->m_Wrn_rot) );

	return( chk );
}


typedef	struct	{
	int			rii;		// reservation_info_id
	int			rdi;		// reservation_detail_id
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
	case VWK_RSV:	chk = ((CReservationView *)viw)->GetData( rpr->rii );				break;
	case VWK_CIN:	chk = ((CCheckinView *)viw)->GetData( rpr->rii );					break;
	case VWK_INH:	chk = ((CInhouseView *)viw)->GetData( rpr->rii, rpr->rdi, FALSE );	break;
	case VWK_COT:	chk = ((CCheckoutView *)viw)->GetData( rpr->rii, rpr->rdi );		break;
	}

	return( chk );
}

void	Nad_Rnc_Viw(				// Launch each view
int			vkd,					// View kind of target view
int			rii,					// reservation_info_id
int			rdi )					// reservation_detail_id
{
		RPR_		rpr[1];

		rpr->rii = rii;
		rpr->rdi = rdi;

		//switch( gid ){
		//case IDC_GRD_RSV:	vkd = VWK_RSV;	break;
		//case IDC_GRD_CIN:	vkd = VWK_CIN;	break;
		//case IDC_GRD_INH:	vkd = VWK_INH;	break;
		//case IDC_GRD_COT:	vkd = VWK_COT;	break;
		//}
		if ( vkd != -1 ){
			Rnc_Ech_Viw( vkd, exv_cbk, rvw_cbk, (LPVOID)(LONG_PTR)rpr );
		}
}

void	Nad_Ctx_Mnu(			// Open context menu on search view
CWnd		*wnd,				// Search view
int			msx,				// Mouse point x
int			msy,				// Mouse point y
int			row,				// row
int			gid )				// Grid ID
{
	CMenu			mnu, *pmn=NULL;
	POINT			pnt[1] = { msx, msy };
	DWORD			mid=0;
	int				flg;
	NAD_WRN_LST		*wrn;
	CSimpleGrid		*grd;
	CNightAuditView	*viw = (CNightAuditView *)wnd;

	switch( gid ){
	case IDC_GRD_WRN:	mid = IDR_MENU_NGT_ADT;	break;
	}

	if ( mid ){
		grd = (CSimpleGrid *)wnd->GetDlgItem(gid);
		viw->m_Rdn_wrn = wrn = grd ? (NAD_WRN_LST *)(LONG_PTR)grd->GetData( NAD_CLM_DAT, row ): NULL;
		viw->m_Rdn_rii = viw ? DBS_INT_VAL(wrn->rii): 0;
		viw->m_Rdn_rdi = viw ? DBS_INT_VAL(wrn->rdi): 0;
		mnu.LoadMenu(mid);
		pmn = mnu.GetSubMenu(0);

		// checkin/noshow/cancelreservation
		flg = MF_BYCOMMAND|(wrn->sts==NAS_NCI ? MF_ENABLED: MF_DISABLED|MF_GRAYED);
		pmn->EnableMenuItem( ID_NIGHTAUDIT_CHECKIN, flg );
		pmn->EnableMenuItem( ID_NIGHTAUDIT_NOSHOW, flg );
		pmn->EnableMenuItem( ID_NIGHTAUDIT_CANCELRESERVATION, flg );
		// checkout
		flg = MF_BYCOMMAND|(wrn->sts==NAS_NCO ? MF_ENABLED: MF_DISABLED|MF_GRAYED);
		pmn->EnableMenuItem( ID_NIGHTAUDIT_CHECKOUT, flg );

		// mouse on the panel
		wnd->ClientToScreen( pnt );
		pmn->TrackPopupMenu( TPM_TOPALIGN/*TPM_LEFTALIGN*/|TPM_RIGHTBUTTON, pnt->x, pnt->y, wnd, NULL );
	}
}

static	int	ntf_viw(				// notification to another views
int			rii,					// reservation_info_id
int			rdi,					// reservation_detail_id ( 0 when no_show )
CWnd		*wnd,					// parent window( CNigtAuditView )
PMS_MNG		*mng )					// Hotel Property Management System structure
{
	int				chk=TRUE, val, row, cnt=0;
	BST_ROM_STS		*sts;
	NAD_WRN_LST		*wrn;
	Lnk				rot[1], *csr=NULL;
	CNightAuditView	*viw = (CNightAuditView *)wnd;
	CSimpleGrid		*grd = (CSimpleGrid *)wnd->GetDlgItem(IDC_GRD_WRN);

	
	memset( rot, 0, sizeof(rot) );
	wrn = viw->m_Rdn_wrn;

	for ( row=1; chk && row<=grd->GetLineCount(FALSE); row++ ){
		wrn = (NAD_WRN_LST *)(LONG_PTR)grd->GetData( NAD_CLM_DAT, row );
		if ( rii==DBS_INT_VAL(wrn->rii) && ( !rdi || rdi==DBS_INT_VAL(wrn->rdi) ) ){
			if ( chk = !!( csr = Add_Lnk( rot, NULL, sizeof(BST_ROM_STS) ) ) ){
				sts = Rec_Of(BST_ROM_STS, csr);
				DBS_INT_CPY(sts->req->id,  wrn->rei);
				DBS_INT_CPY(sts->req->eii, wrn->eii);
				DBS_INT_SET(sts->cxl, &(val=1));
			}
		}
	}
	if ( chk ){
		Viw_Snd_Ntf( VIW_SND_BST, VWN_RSV_CHANGED, rot, cnt, wnd );
		Viw_Pst_Ntf( VIW_SND_SCH, VWN_RSV_CHANGED, NULL, rii, wnd );
	}

	Del_All( rot );

	return( chk );
}

int		Nad_Mak_Nsw(				// Make it NowShow
int			rii,					// reservation_info_id
int			rdi,					// reservation_detail_id
CWnd		*wnd,					// parent window( CNigtAuditView )
CDatabase	*dbs,					// ODBC Databae object
PMS_MNG		*mng )					// Hotel Property Management System structure
{
	int			chk=TRUE;
	WCHAR		*erm=NULL;
	Day			dte[1];

	if ( wnd->MessageBox( _T("Do you make it NoShow this reservation?"), NULL, MB_ICONQUESTION|MB_YESNO ) == IDYES ){
		Get_Dte( dte );
		if ( chk = Nad_Nsw_Exc( rii, dte, &erm, dbs, mng ) ){
			ntf_viw( rii, 0, wnd, mng );
			Nad_Get_Dat( wnd, dbs, mng );
		}
	}
	if ( erm ){
		wnd->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
		free( erm );
	}

	return( chk );
}


void	Nad_Cxl_Rsv(			// execute a reservation cancellation
int			rii,				// reservation_info_id
int			rdi,				// reservation_detail_id ( cancellation target )
CWnd		*wnd,				// parent window
CDatabase	*dbs,				// ODBC Database object
PMS_MNG		*mng )				// Hotel Property Management System structure
{
	WCHAR			*erm=NULL;
	int				chk;

	if ( wnd->MessageBox( _T("Are you sure to cancel this reservation?"), NULL, MB_ICONQUESTION|MB_YESNO )==IDYES ){
		if ( chk = Rsv_Idv_Cxl( rdi, rii, &erm, dbs, mng ) ){
			ntf_viw( rii, rdi, wnd, mng );
			Nad_Get_Dat( wnd, dbs, mng );
		}
		if ( erm ){
			wnd->MessageBox( erm, NULL, MB_ICONINFORMATION|MB_OK );
			free( erm );
		}
	}
}

int		Nad_Day_Cls(			// execute day close
CWnd		*wnd,				// parent window
CDatabase	*dbs,				// ODBC Database object
PMS_MNG		*mng )				// Hotel Property Management System structure
{
	int				chk;
	WCHAR			*erm=NULL;

	// sale up room charge
	if ( !( chk = Nad_Day_Cls( &erm, dbs, mng ) ) ){
		if ( erm ){
			wnd->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK );
			free( erm );
		}
	}

	return( chk );
}
