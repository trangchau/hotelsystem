// HotelPMSView.cpp : CReservationView クラスの実装
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#include "ReservationDoc.h"
#include "ReservationView.h"
#include "DatePickerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CReservationView

IMPLEMENT_DYNCREATE(CReservationView, CFormView)

BEGIN_MESSAGE_MAP(CReservationView, CFormView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_BN_CLICKED(IDCANCEL, &CReservationView::OnBnClickedCancel)
//	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_RSV_DETAIL, &CReservationView::OnTcnSelchangeTabRsvDetail)
	ON_BN_CLICKED(IDOK, &CReservationView::OnBnClickedOk)
	ON_WM_GRIDNOTIFY()
	ON_WM_DATEPICKERNOTIFY()
	ON_WM_EDITNOTIFY()
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDIT_FIRSTNAME, &CReservationView::OnEnChangeEditFirstname)
	ON_BN_CLICKED(IDC_CHECK_SAMERESERVATION, &CReservationView::OnBnClickedCheckSamereservation)
	ON_EN_CHANGE(IDC_EDIT_ROOMNUMBER, &CReservationView::OnEnChangeEditRoomnumber)
	ON_EN_CHANGE(IDC_EDIT_ADULTNUMBER, &CReservationView::OnEnChangeEditAdultnumber)
	ON_EN_CHANGE(IDC_EDIT_CHILDNUMBER, &CReservationView::OnEnChangeEditChildnumber)
	ON_CBN_SELCHANGE(IDC_COMBO_BOOKINGPATH, &CReservationView::OnCbnSelchangeComboBookingpath)
	ON_BN_CLICKED(IDC_RADIO_GROUP, &CReservationView::OnBnClickedRadioGroup)
	ON_BN_CLICKED(IDC_RADIO_PERSONAL, &CReservationView::OnBnClickedRadioPersonal)
	ON_CBN_SELCHANGE(IDC_COMBO_ROOMTYPE, &CReservationView::OnCbnSelchangeComboRoomtype)
	ON_EN_CHANGE(IDC_EDIT_ROOMCHANGE, &CReservationView::OnEnChangeEditRoomchange)
	ON_EN_KILLFOCUS(IDC_EDIT_ARRIVALDATE, &CReservationView::OnEnKillfocusEditArrivaldate)
	ON_EN_KILLFOCUS(IDC_EDIT_DEPARTUREDATE, &CReservationView::OnEnKillfocusEditDeparturedate)
	ON_EN_CHANGE(IDC_EDIT_FAMILYNAME, &CReservationView::OnEnChangeEditFamilyname)
	ON_CBN_SELCHANGE(IDC_COMBO_RATEPLANNAME, &CReservationView::OnCbnSelchangeComboRateplanname)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_NIGHTS, &CReservationView::OnDeltaposSpinNights)
	ON_EN_KILLFOCUS(IDC_EDIT_NIGHTS, &CReservationView::OnEnKillfocusEditNights)
	ON_BN_CLICKED(IDC_CHECK_CHECKALL, &CReservationView::OnBnClickedCheckCheckall)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CReservationView::OnBnClickedButtonSearch)
	ON_EN_SETFOCUS(IDC_EDIT_ARRIVALDATE, &CReservationView::OnEnSetfocusEditArrivaldate)
	ON_EN_SETFOCUS(IDC_EDIT_DEPARTUREDATE, &CReservationView::OnEnSetfocusEditDeparturedate)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHDETAIL, &CReservationView::OnBnClickedButtonSearchdetail)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CReservationView::OnBnClickedButtonCancel)
END_MESSAGE_MAP()

// CReservationView コンストラクション/デストラクション

CReservationView::CReservationView(): CFormView(CReservationView::IDD)
{
	m_Dtl_grd = m_Rom_grd = NULL;
	m_Sel_row = -1;
	m_Ctm_row = -1;
	m_Edt_sts = 0;
	m_Opr_cod = NULL;
	m_Brc_cod = NULL;
	m_Fcl_cod = NULL;
	m_Dte_pcr = NULL;
	m_Fcs_edt = NULL;

	memset( m_Rsv_inf, 0, sizeof(m_Rsv_inf) );
	memset( m_Rte_pln, 0, sizeof(m_Rte_pln) );
	memset( m_Eqp_isi, 0, sizeof(m_Eqp_isi) );
}

CReservationView::~CReservationView()
{
	if ( !!m_Dtl_grd )	delete m_Dtl_grd;
	if ( !!m_Rom_grd )	delete m_Rom_grd;
	Rsv_Dps_Dtl( m_Rsv_inf->dtl );
	Rsv_Dps_Rte( m_Rte_pln );
	Del_All( m_Eqp_isi );
}

BOOL CReservationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CReservationView 描画

void CReservationView::OnDraw(CDC* /*pDC*/)
{
	CReservationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}


// CReservationView 印刷

BOOL CReservationView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CReservationView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CReservationView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}


// CReservationView 診断

#ifdef _DEBUG
void CReservationView::AssertValid() const
{
	CView::AssertValid();
}

void CReservationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CReservationDoc* CReservationView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CReservationDoc)));
	return (CReservationDoc*)m_pDocument;
}
#endif //_DEBUG

void CReservationView::SetupComponent()
{
	int				idx;
	WCHAR			*fnm = _T("MS UI Gothic");
	WINDOWPLACEMENT	wpc[1];

	struct	{
		CWnd	*wnd;
		int		lmt;
		DWORD	cid;
	}Cpn_[] = {
		{ m_Arv_dte,  -1, IDC_EDIT_FIRSTARRIVAL		},
		{ m_Dpt_dte,  -1, IDC_EDIT_LASTDEPARTURE	},
		{ m_Arv_dtl,  -1, IDC_EDIT_ARRIVALDATE		},
		{ m_Dpt_dtl,  -1, IDC_EDIT_DEPARTUREDATE	},
		{ m_Fst_nam, 255, IDC_EDIT_FIRSTNAME		},
		{ m_Fam_nam, 255, IDC_EDIT_FAMILYNAME		},
		{ m_Fst_dtl, 255, IDC_EDIT_FIRSTNAME_DETAIL	},
		{ m_Fam_dtl, 255, IDC_EDIT_FAMILYNAME_DETAIL},
		{ m_Inf_sts,  -1, IDC_STATIC_INFOSTS		},
		{ m_Dtl_sts,  -1, IDC_STATIC_DETAILSTS		},
		{ m_Arv_tim,  -1, IDC_EDIT_ARRIVALTIME		},
		{ m_Dpt_tim,  -1, IDC_EDIT_DEPARTURETIME	},
		{ m_Rsv_typ,  -1, IDC_STATIC_RESERVATIONTYPE},
		{ m_Bpt_ttl,  -1, IDC_STATIC_BOOKINGPATH	},
		{ m_Bok_pth,  -1, IDC_COMBO_BOOKINGPATH		},
		{ m_Cli_rnk,  -1, IDC_COMBO_CLIENTRANK		},
		{ m_Rsv_nte,1024, IDC_EDIT_NOTE				},
		{ m_Eqp_typ,  -1, IDC_COMBO_ROOMTYPE		},
		{ NULL,		   4, IDC_EDIT_ROOMNUMBER		},
		{ NULL,		   3, IDC_EDIT_NIGHTS			},
		{ NULL,		   2, IDC_EDIT_ADULTNUMBER		},
		{ NULL,		   2, IDC_SPIN_CHILDNUMBER		}
	};

	for ( idx=0; idx<memcnt(Cpn_); idx++ ){
		if ( !!Cpn_[idx].wnd )		Cpn_[idx].wnd->SubclassDlgItem(Cpn_[idx].cid, this);
		if ( Cpn_[idx].lmt!=-1 )	((CEdit *)GetDlgItem(Cpn_[idx].cid))->SetLimitText( Cpn_[idx].lmt );
	}

	// Font setting
	m_Fst_nam->SetFont(fnm, 20, RGB(0,0,0) );
	m_Fam_nam->SetFont(fnm, 20, RGB(0,0,0) );
	m_Fst_dtl->SetFont(fnm, 20, RGB(0,0,0) );
	m_Fam_dtl->SetFont(fnm, 20, RGB(0,0,0) );
	m_Dtl_sts->SetFont(fnm, 18, STC_COF );
	m_Inf_sts->SetFont(fnm, 18, STC_COF );

	// BKColor setting(Required item)
	m_Fst_nam->SetBGColor( REQ_COL );
	m_Arv_dtl->SetBGColor( REQ_COL );
	m_Dpt_dtl->SetBGColor( REQ_COL );
	m_Bok_pth->SetBGColor( REQ_COL );
	m_Cli_rnk->SetBGColor( REQ_COL );
	m_Rsv_typ->SetBackColor(REQ_COL);
	m_Bpt_ttl->SetBackColor(REQ_COL);
	m_Inf_sts->AutoSize = TRUE;
	m_Dtl_sts->AutoSize = TRUE;

	// reservation_details Grid
	GetDlgItem(IDC_EDIT_ROOMNUMBER)->GetWindowPlacement( wpc );
	wpc->rcNormalPosition.top = wpc->rcNormalPosition.bottom + 2;
	m_Dtl_grd = Rsv_Mak_Grd( IDC_GRD_DTL, &wpc->rcNormalPosition, this );
	m_Dtl_grd->InputEnable( TRUE );

	// reservation_equipments Grid
	GetDlgItem(IDC_STATIC_ROOMLIST)->GetWindowPlacement( wpc );
	m_Rom_grd = Rsv_Mak_Grd( IDC_GRD_ROM, &wpc->rcNormalPosition, this );

	// Editbox limittext ( Out of subclass component)
	//((CEdit *)GetDlgItem(IDC_EDIT_ROOMNUMBER))->SetLimitText(4);
	//((CEdit *)GetDlgItem(IDC_EDIT_NIGHTS))->SetLimitText(3);
	//((CEdit *)GetDlgItem(IDC_EDIT_ADULTNUMBER))->SetLimitText(2);
	//((CEdit *)GetDlgItem(IDC_SPIN_CHILDNUMBER))->SetLimitText(2);
}

void	CReservationView::ShowCalenderDlg( DWORD cid )
{
	WCHAR		buf[40];
	Day			dte[1];

	if ( m_Dte_pcr && !(m_Dte_pcr->IsWindowVisible() ) ){
		m_Fcs_edt = GetDlgItem(cid);
		m_Fcs_edt->GetWindowText( buf, memcnt(buf) );
		if ( !Str_Dte( dte, buf ) ){
			*dte = *m_pPms_mng->bdt;
		}

		m_Dte_pcr->SetNotifyWindow( this );
		m_Dte_pcr->SetDefaultDate( dte );
		m_Dte_pcr->SetPosition( this, GetDlgItem(cid) );
		m_Dte_pcr->SetLimit( m_pPms_mng->bdt->yer, m_pPms_mng->bdt->mth, 9999, 99 );
		//m_Dte_pcr->SetParent( this );
		m_Dte_pcr->ShowWindow( SW_NORMAL );
	}
}


// CReservationView メッセージ ハンドラ

void CReservationView::OnInitialUpdate()
{
	((CChildFrame *)GetParent())->m_Nsz_viw = TRUE;	// Non sizable
	((CChildFrame *)GetParent())->m_Viw_knd = VWK_RSV;

	CView::OnInitialUpdate();

	// ダイアログリソースのサイズにフィットさせる
	ResizeParentToFit(false);

	Viw_Clc_Dfs(((CChildFrame *)GetParent())->m_Win_siz, this);
	
	((CChildFrame *)GetParent())->m_Ini_upd = TRUE;

	m_pPms_mng = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->m_Pms_mng;
	m_pDbs_obj = ((CMainFrame *)AfxGetApp()->m_pMainWnd)->m_Dbs_obj;

	// setup the roomtype combobox

	SetupComponent();

	// Room type combobox initialize
	Csr			csr=NULL;
	CComboBox	*cbx = (CComboBox *)GetDlgItem(IDC_COMBO_ROOMTYPE);
	EQP_TYP		*eqt;

	m_Eqp_typ->ClearType();
	while( csr = Nxt_Lnk( m_pPms_mng->etl, csr ) ){
		eqt = Rec_Of(EQP_TYP, csr);
		m_Eqp_typ->AddType( DBS_INT_VAL(eqt->id), FALSE, DBS_WST_PTR(eqt->nam) );
	}
	// Minimize box to disable 
	((CChildFrame *)GetParent())->ModifyStyle( WS_MAXIMIZEBOX, 0, TRUE );

	m_Bok_pth->SetAgentData( m_pPms_mng->agl );
	m_Dte_pcr = ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->m_Dte_pcr;
}

// Search Button click event
void CReservationView::OnBnClickedButtonSearch()
{
	Rsv_Sch_Cli( NULL, this, m_pDbs_obj, m_pPms_mng );
}

// SearchrReservation_detail) Button click event
void CReservationView::OnBnClickedButtonSearchdetail()
{
	RSV_DTL_DAT		*dtl;

	if ( m_Sel_row != -1 ){
		dtl =  (RSV_DTL_DAT *)(LONG_PTR)m_Dtl_grd->GetData( 0, m_Sel_row );
		Rsv_Sch_Cli( dtl, this, m_pDbs_obj, m_pPms_mng );
	}
}

// グリッドイベント
LRESULT CReservationView::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	GRD_NTF			*ntf = (GRD_NTF *)wParam;
	CComboBox		*cbx;
	WCHAR			*txt;
	int				typ;
	LRESULT			rsl=TRUE;
	RSV_DTL_DAT		*dtl;

	if ( ntf ){
		switch( ntf->ntf ){
		case GN_EDITCOMPLETE:	if ( !!( txt = (WCHAR *)lParam) ){
									if ( ntf->cid == IDC_GRD_ROM ){
										if ( m_Sel_row != -1 ){
											dtl =  (RSV_DTL_DAT *)(LONG_PTR)m_Dtl_grd->GetData( 0, m_Sel_row );
											if ( ntf->clm==ROM_NBR_CLM ){
												// Edited the room No. columun
												Rsv_Rom_Nbr( txt, ntf->row, this, m_pDbs_obj, m_pPms_mng );
											}
											else{
												// Edited the use start date/ end date
												Rsv_Rom_Dte( txt, ntf->clm, ntf->row, dtl, this, m_pDbs_obj, m_pPms_mng );
											}
										}
									}
									free( txt );
								}
								break;
		case GN_LBUTTONCLICK:	if ( ntf->cid==IDC_GRD_DTL ){
									if ( ntf->row == 0 )	m_Sel_row=-1;
									if ( ntf->clm!=RSV_CHK_CLM ){
										rsl = Rsv_Chg_Dtl( ntf->row, this, m_pDbs_obj, m_pPms_mng );
									}
									else{
										rsl = Rsv_Chk_Clk( ntf->row, this, m_pDbs_obj, m_pPms_mng );
									}
								}
								break;
		case GN_LBUTTONDBLCLICK:if ( ntf->cid == IDC_GRD_DTL ){
									//rid = m_Rsv_grd->GetData( 0, ntf->row );
									//Rnc_Rsv_Viw( rid );
								}
								break;						// 左ボタンクリック
		case GN_RBUTTONCLICK:	if ( ntf->cid==IDC_GRD_ROM ){
									m_Ctm_row = ntf->row;
									Rsv_Ctx_Mnu( this, ntf->msx, ntf->msy );
								}
								break;
		case GN_QUERYCOMBODATA:	cbx = (CComboBox *)(LONG_PTR)lParam;
								break;
		case GN_COMBOCHANGED:	typ = m_Dtl_grd->GetData( ntf->clm, ntf->row );
								break;
		case GN_QUERYINPUTTYPE:	if ( ntf->cid==IDC_GRD_DTL ){
									switch( ntf->clm ){
									case RSV_CHK_CLM:rsl = GCT_CHK;	break;
									default:rsl = GCT_NML;			break;
									}
								}
								else{
									switch( ntf->clm ){
									case ROM_USD_CLM:	rsl = ntf->row==1 ? GCT_NML: GCT_DED;
														break;
									case ROM_UED_CLM:	rsl = ntf->row==m_Rom_grd->GetLineCount(FALSE) ? GCT_NML: GCT_DED;
														break;
									case ROM_NBR_CLM:	rsl = GCT_EDT;
														break;
									default:			rsl = GCT_NML;
														break;
									}
								}
								break;
		case GN_QUERYCOLORDATA:	break;
		}
		free( ntf );
	}

	return( rsl );
}

LRESULT CReservationView::OnEditNotify( WPARAM wParam, LPARAM lParam )
{
	DWORD		cid;

	if ( LOWORD(wParam)==EN_RBUTTONCLICK ){
		cid = HIWORD(wParam);
		if ( cid==IDC_EDIT_ARRIVALDATE || cid==IDC_EDIT_DEPARTUREDATE ){
			ShowCalenderDlg(cid);
		}
	}
	return( 0 );
}

void CReservationView::OnDatePickerNotify( WPARAM wParam, LPARAM lParam )
{
	WCHAR			buf[20];
	Day				dte[1];

	memset( dte, 0, sizeof(dte) );
	dte->yer = (int)(lParam/10000);
	dte->mth = (int)((lParam/100)%100);
	dte->day = (int)(lParam % 100);

	switch( wParam ){
	case DPC_SELECTDATE:	//swprintf_s( buf, memcnt(buf), _T("%4d/%02d/%02d"), lParam/10000, (lParam/100)%100, lParam % 100 );
							Edt_Dte( buf, memcnt(buf), dte );
							if ( m_Fcs_edt )	m_Fcs_edt->SetWindowText( buf );
							break;
	}
}

// Windows command operation
BOOL CReservationView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	RSV_DTL_DAT		*dtl=NULL;

	if ( m_Sel_row != -1 ){
		dtl =  (RSV_DTL_DAT *)(LONG_PTR)m_Dtl_grd->GetData( 0, m_Sel_row );
	}

//	IDD_DIALOG_CHARGEDETAIL
	if (pHandlerInfo == NULL){
		if (nCode == CN_COMMAND){
			switch ( nID ){
			case ID_RESERVATION_CHARGEDETAIL:	Rsv_Crg_Dtl( dtl, m_Ctm_row, this, m_pPms_mng );
												break;
			case ID_RESERVATION_SELECTROOM:		Rsv_Rom_Sel( dtl, m_Ctm_row, this, m_pPms_mng );
												break;
			}
		}
	}

	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// Client kind( personal ) Click
void CReservationView::OnBnClickedRadioGroup()
{
	Rsv_Sel_Clk( this );
}

// Client kind( group ) Click
void CReservationView::OnBnClickedRadioPersonal()
{
	Rsv_Sel_Clk( this );
}

// FirstName of resserved person change
void CReservationView::OnEnChangeEditFirstname()
{
	if ( ((CButton *)GetDlgItem(IDC_CHECK_SAMERESERVATION))->GetCheck() ){
		Cpy_Edt( m_Fst_dtl, m_Fst_nam );
	}
}

// FamilyName of resserved person change
void CReservationView::OnEnChangeEditFamilyname()
{
	if ( ((CButton *)GetDlgItem(IDC_CHECK_SAMERESERVATION))->GetCheck() ){
		Cpy_Edt( m_Fam_dtl, m_Fam_nam );
	}
}

// BookingPath Combobox select change
void CReservationView::OnCbnSelchangeComboBookingpath()
{
	RSV_DTL_DAT		*dtl;

	if ( m_Sel_row != -1 ){
		dtl =  (RSV_DTL_DAT *)(LONG_PTR)m_Dtl_grd->GetData( 0, m_Sel_row );
		Rsv_Bph_Chg( dtl, this, m_pDbs_obj, m_pPms_mng );
	}
}

// cancel selected button click
void CReservationView::OnBnClickedButtonCancel()
{
	Rsv_Cxl_Exc( this, m_pDbs_obj, m_pPms_mng );
}

// number of rooms edit change
void CReservationView::OnEnChangeEditRoomnumber()
{
	WCHAR		buf[20];

	if ( m_Dtl_grd ){
		GetDlgItem(IDC_EDIT_ROOMNUMBER)->GetWindowText( buf, memcnt(buf) );
		if ( _wtoi(buf)>0 ){
			Rsv_Rct_Chg( _wtoi( buf ), this, m_pPms_mng );
		}
		else{
			swprintf_s( buf, memcnt(buf), _T("%d"), m_Dtl_grd->GetLineCount( FALSE ) );
			GetDlgItem(IDC_EDIT_ROOMNUMBER)->SetWindowText( buf );
		}
	}
}

void CReservationView::OnBnClickedCheckCheckall()
{
	Rsv_Chk_All( this, m_pPms_mng );
}

// Arrival date set focus
void CReservationView::OnEnSetfocusEditArrivaldate()
{
//	ShowCalenderDlg(IDC_EDIT_ARRIVALDATE);
}

// Arrival date change
void CReservationView::OnEnKillfocusEditArrivaldate()
{
	RSV_DTL_DAT		*dtl;

	m_Dte_pcr->ShowWindow( SW_HIDE );
	if ( m_Sel_row != -1 && !!( dtl =  (RSV_DTL_DAT *)(LONG_PTR)m_Dtl_grd->GetData( 0, m_Sel_row ) ) ){
//		Rsv_Sty_Chg( dtl, this, m_pDbs_obj, m_pPms_mng );
		Rsv_Pod_Chg( RSV_CGK_ARV, dtl, this, m_pDbs_obj, m_pPms_mng );
	}
}

// Departure date set focus
void CReservationView::OnEnSetfocusEditDeparturedate()
{
//	ShowCalenderDlg(IDC_EDIT_DEPARTUREDATE);
}

// Departure date change
void CReservationView::OnEnKillfocusEditDeparturedate()
{
	RSV_DTL_DAT		*dtl;

	m_Dte_pcr->ShowWindow( SW_HIDE );
	if ( m_Sel_row != -1 && !!( dtl =  (RSV_DTL_DAT *)(LONG_PTR)m_Dtl_grd->GetData( 0, m_Sel_row ) ) ){
//		Rsv_Sty_Chg( dtl, this, m_pDbs_obj, m_pPms_mng );
		Rsv_Pod_Chg( RSV_CGK_DPT, dtl, this, m_pDbs_obj, m_pPms_mng );
	}
}

// Requested room type combobox change
void CReservationView::OnCbnSelchangeComboRoomtype()
{
	RSV_DTL_DAT			*dtl;

	if ( m_Sel_row != -1 && !!( dtl =  (RSV_DTL_DAT *)(LONG_PTR)m_Dtl_grd->GetData( 0, m_Sel_row ) ) ){
		Rsv_Rtp_Chg( dtl, this, m_pDbs_obj, m_pPms_mng );
	}
	
}

// Rate plan combobox change
void CReservationView::OnCbnSelchangeComboRateplanname()
{
	RSV_DTL_DAT		*dtl;

	if ( m_Sel_row != -1 && !!( dtl =  (RSV_DTL_DAT *)(LONG_PTR)m_Dtl_grd->GetData( 0, m_Sel_row ) ) ){
		Rsv_Sel_Pln( dtl, this, m_pDbs_obj, m_pPms_mng );
	}
}

// Adult number edit change
void CReservationView::OnEnChangeEditAdultnumber()
{
	//WCHAR		buf[20];

	//GetDlgItem(IDC_EDIT_ADULTNUMBER)->GetWindowText( buf, memcnt(buf) );
	//if ( _wtoi(buf)==0 ){
	//	GetDlgItem(IDC_EDIT_ADULTNUMBER)->SetWindowText( _T("1") );
	//}
}

// Child number edit change
void CReservationView::OnEnChangeEditChildnumber()
{
	//WCHAR		buf[20];

	//GetDlgItem(IDC_EDIT_CHILDNUMBER)->GetWindowText( buf, memcnt(buf) );
	//if ( _wtoi(buf)==0 ){
	//	GetDlgItem(IDC_EDIT_CHILDNUMBER)->SetWindowText( _T("0") );
	//}
}

// Same as reservation persn checkbox clicked
void CReservationView::OnBnClickedCheckSamereservation()
{
	Rsv_Sam_Rsv( this, m_pDbs_obj, m_pPms_mng );
}

// Nights spin change
void CReservationView::OnDeltaposSpinNights(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN		pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	RSV_DTL_DAT		*dtl;
	int				ngt = pNMUpDown->iPos + pNMUpDown->iDelta;
	WCHAR			buf[20];

	if ( m_Sel_row != -1 && !!( dtl =  (RSV_DTL_DAT *)(LONG_PTR)m_Dtl_grd->GetData( 0, m_Sel_row ) ) ){
//		Rsv_Ngt_Chg( ngt, dtl, this, m_pDbs_obj, m_pPms_mng );

		swprintf_s( buf, memcnt(buf), _T("%d"), ngt );
		GetDlgItem(IDC_EDIT_NIGHTS)->SetWindowTextW( buf );
		Rsv_Pod_Chg( RSV_CGK_NGT, dtl, this, m_pDbs_obj, m_pPms_mng );
	}
	*pResult = 0;
}

// Nights edit kill focus
void CReservationView::OnEnKillfocusEditNights()
{
	WCHAR			buf[20];
	RSV_DTL_DAT		*dtl;

	if ( m_Sel_row != -1 && !!( dtl =  (RSV_DTL_DAT *)(LONG_PTR)m_Dtl_grd->GetData( 0, m_Sel_row ) ) ){
		GetDlgItem(IDC_EDIT_NIGHTS)->GetWindowText( buf, memcnt(buf) );
//		Rsv_Ngt_Chg( _wtoi(buf), dtl, this, m_pDbs_obj, m_pPms_mng );
		Rsv_Pod_Chg( RSV_CGK_NGT, dtl, this, m_pDbs_obj, m_pPms_mng );
	}
}

// Room change edit change
void CReservationView::OnEnChangeEditRoomchange()
{
	RSV_DTL_DAT		*dtl;

	if ( m_Sel_row != -1 ){
		// Save current detail data
		if ( dtl =  (RSV_DTL_DAT *)(LONG_PTR)m_Dtl_grd->GetData( 0, m_Sel_row ) ){
			Rsv_Rom_Chg( dtl, this, m_pDbs_obj, m_pPms_mng );
		}
	}
}

// OK Button Click
void CReservationView::OnBnClickedOk()
{
	if ( m_Sel_row != -1 ){
		if ( Rsv_Sav_Dat( m_Rsv_inf, this, m_pDbs_obj, m_pPms_mng ) ){
			GetDocument()->OnCloseDocument();
		}
	}
}

// Cancel button click
void CReservationView::OnBnClickedCancel()
{
	GetDocument()->OnCloseDocument();
}

// On Close the window
void CReservationView::OnClose()
{

	CFormView::OnClose();
}

// No Clienr area dispose
void CReservationView::PostNcDestroy()
{

	CFormView::PostNcDestroy();
}

BOOL CReservationView::PreTranslateMessage(MSG* pMsg)
{
	return( Viw_Ctl_Btn(this, pMsg ) ? CFormView::PreTranslateMessage(pMsg): FALSE );
}

//	オペレーション
int		CReservationView::GetData( int rid )
{
	m_Opr_cod = m_pPms_mng->opc;
	m_Brc_cod = m_pPms_mng->brc;
	m_Fcl_cod = m_pPms_mng->fcc;
	return( Rsv_Get_Dat( rid, this, m_pDbs_obj, m_pPms_mng ) );
}

int		CReservationView::CreateData( CCI_PRM *prm )			// Create a new reservation
{
	m_Opr_cod = m_pPms_mng->opc;
	m_Brc_cod = m_pPms_mng->brc;
	m_Fcl_cod = m_pPms_mng->fcc;
	return( Rsv_Cre_Rsv( prm, this, m_pDbs_obj, m_pPms_mng ) );
}
