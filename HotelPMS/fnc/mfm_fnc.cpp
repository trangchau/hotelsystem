/*
	Functions for Main Frame
*/

//#include "windows.h"
#include "..\stdafx.h"
#include "..\HotelPMS.h"
#include "..\ReservationDoc.h"
#include "..\ReservationView.h"
#include "..\CheckinView.h"
#include "..\InhouseView.h"
#include "..\CheckoutView.h"
#include "..\MessageDlg.h"
#include "..\LoginDlg.h"
#include "..\ChangePwdDlg.h"
#include "..\..\Common\scl_ble.h"
#include "mfm_fnc.h"
#include "mfm_sql.h"

// BookingStatusToolBar Button definition
static UINT BASED_CODE Bst_btn[] = {
	ID_SEPARATOR, ID_SEPARATOR, ID_SEPARATOR, ID_SEPARATOR, ID_SEPARATOR, 
	ID_BOOKING_ADDBOOKING, ID_BOOKING_CUTBOOKING, ID_BOOKING_MOVEBOOKING, ID_BOOKING_CANCELBOOKING
};

// InfoToolBar Button definition
static UINT BASED_CODE ITB_btn[] = {
	ID_SEPARATOR, ID_SEPARATOR, ID_SEPARATOR, ID_SEPARATOR, ID_SEPARATOR,
	ID_VIEW_SEARCHVIEW, ID_VIEW_BOOKINGSTATUSVIEW, ID_FILE_NEWRESERVATION
};

// RoomDirectBar Button definition
static UINT BASED_CODE RDr_btn[] = {
	ID_SEPARATOR, TBP_ID_DCRCIN_EDIT, ID_SEPARATOR, TBP_ID_DCRINH_EDIT, ID_SEPARATOR, 
	TBP_ID_DCRCOT_EDIT, TBP_ID_DCRCIN_EDIT

};


static	int		cre_fnt_(		// フォントの生成
CFont		*fnt,				// フォントクラスポインタ
int			siz,				// Size of font height
int			bld )				// Bold size
{
	LOGFONT		logFont[1];

	memset( logFont, 0, sizeof(logFont) );

	logFont->lfHeight = siz;
	logFont->lfWeight = bld;;
	logFont->lfCharSet = SHIFTJIS_CHARSET;
	logFont->lfOutPrecision = OUT_TT_PRECIS;
	logFont->lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logFont->lfQuality = DRAFT_QUALITY;
	logFont->lfPitchAndFamily = FIXED_PITCH | FF_MODERN;
	wcscpy_s(logFont->lfFaceName, memcnt(logFont->lfFaceName), _T("MS UI Gothic") );
	return( !!fnt->CreateFontIndirect( logFont ) );
}

static	int		cre_cbx_(		// コンボボックス生成
CComboBox	*cbx,				// 作成対象コンボボックス
int			idx,				// バー内のインデックス
int			wid,				// コンボボックス幅
int			cid,				// コントロールＩＤ
CToolBar	*bar )				// 個別情報バーポインタ
{
	DWORD		cst = CBS_DROPDOWNLIST|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL;
	CRect		rect;
	int			chk;

	bar->SetButtonInfo( idx, cid, TBBS_SEPARATOR, wid );
	bar->GetItemRect( idx, &rect );
	rect.right = rect.left + wid;
	rect.top = 1;
	rect.bottom = rect.top + 300;
	chk = !!cbx->Create( cst, rect, bar, cid );
	if ( chk )	cbx->SetItemHeight( -1, CBX_HGT );
	return( chk );
}

static	int		cre_stc_(		// Create static for tool bar
CStatic		*stc,				// 作成対象コンボボックス
int			idx,				// バー内のインデックス
int			wid,				// コンボボックス幅
int			cid,				// コントロールＩＤ
DWORD		cst,				// Style of window
CToolBar	*bar )				// 個別情報バーポインタ
{
	CRect		rect;
	int			chk;

	bar->SetButtonInfo( idx, cid, TBBS_SEPARATOR, wid );
	bar->GetItemRect( idx, &rect );
	rect.top = 1;
	chk = !!stc->Create( _T(""), cst, rect, bar, cid );
	return( chk );
}

static	int		cre_edt_(		// エディットボックス生成
CCustomEdit	*edt,				// 作成対象エディットボックス
int			idx,				// バー内のインデックス
int			wid,				// エディットボックス幅
int			cid,				// コントロールＩＤ
CToolBar	*bar )				// 個別情報バーポインタ
{
	DWORD		cst = ES_LEFT|ES_WANTRETURN|ES_AUTOHSCROLL|WS_VISIBLE|WS_TABSTOP;
	CRect		rect;
	int			chk;

	//edt->m_dwExStyle = WS_EX_CLIENTEDGE;

	bar->SetButtonInfo( idx, cid, TBBS_SEPARATOR, wid );
	bar->GetItemRect( idx, &rect );
	rect.right = rect.left + wid;
	rect.top = 1;
//	rect.top = 1;
	rect.bottom = rect.top + 24;
	chk = !!edt->Create( cst, rect, bar, cid );
	return( chk );
}

static	int		cre_ded_(		// Create a CDatEdit box
CDateEdit	*edt,				// 作成対象エディットボックス
int			idx,				// バー内のインデックス
int			wid,				// エディットボックス幅
int			cid,				// コントロールＩＤ
CToolBar	*bar )				// 個別情報バーポインタ
{
	DWORD		cst = ES_LEFT|ES_WANTRETURN|ES_AUTOHSCROLL|WS_VISIBLE|WS_TABSTOP;
	CRect		rect;
	int			chk;

	//edt->m_dwExStyle = WS_EX_CLIENTEDGE;

	bar->SetButtonInfo( idx, cid, TBBS_SEPARATOR, wid );
	bar->GetItemRect( idx, &rect );
	rect.right = rect.left + wid;
	rect.top = 1;
//	rect.top = 1;
	rect.bottom = rect.top + 24;
	chk = !!edt->Create( cst, rect, bar, cid );
	return( chk );
}

int		Mfm_Cre_BsB(			// Create a booking status tool bar
CBookingStsToolBar	*bar,		// 個別情報バーポインタ
CWnd				*wnd )		// 親ウィンドウクラス
{
	int			chk;

	if ( chk = !!bar->CreateEx(wnd, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ){
		//if (chk = !!bar->LoadToolBar(IDR_TOOLBAR_BST)){
		if (chk = !!bar->LoadBitmap(IDB_BMP_BOOKINGSTS)){
			if ( !!bar->SetButtons( Bst_btn, memcnt(Bst_btn) ) ){
				bar->SetHeight( TOL_HGT );
			}
		}
	}
	if ( chk ){
		if ( chk = cre_ded_( &bar->m_Edt_pfr, 0, 90, TBP_PERIODFROM, bar ) ){
			if ( chk  = cre_stc_( &bar->m_Stc_btw, 1, 28, IDC_STATIC, WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE, bar ) ){
				if ( chk = cre_ded_( &bar->m_Edt_pto, 2, 90, TBP_PERIODTO, bar ) ){
					if ( chk = cre_stc_( &bar->m_Stc_bdt, 3, 160, TBP_ID_BDATE_STATIC, WS_VISIBLE|SS_RIGHT|SS_CENTERIMAGE, bar ) ){
						bar->m_Stc_btw.SetWindowText( _T("to") );
						/*
						bar->SetButtonStyle( 5, TBBS_CHECKGROUP );
						bar->SetButtonStyle( 6, TBBS_CHECKGROUP );
						bar->SetButtonStyle( 7, TBBS_CHECKGROUP );
						//bar->SetButtonStyle( 5, TBBS_CHECKGROUP );
						*/
						bar->SetButtonInfo( 5, ID_BOOKING_ADDBOOKING, BTNS_CHECKGROUP, 0  );
						bar->SetButtonInfo( 6, ID_BOOKING_CUTBOOKING, BTNS_CHECKGROUP, 1  );
						bar->SetButtonInfo( 7, ID_BOOKING_MOVEBOOKING, BTNS_CHECKGROUP, 2  );
						bar->SetButtonInfo( 8, ID_BOOKING_CANCELBOOKING, BTNS_BUTTON, 3 );
					}
				}
			}
		}
	}

	if ( chk )	bar->SendMessage(TB_CHECKBUTTON, ID_BOOKING_ADDBOOKING, TRUE);

	return( chk );
}

int		Mfm_Cre_ITB(			// Create a info tool bar
CInfoToolBar	*bar,			// 個別情報バーポインタ
CWnd			*wnd )			// 親ウィンドウクラス
{
	int			chk;

	if ( chk = !!bar->CreateEx(wnd, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) ){
		if ( chk = !!bar->LoadBitmap( IDB_BITMAP_INFOTOOLBAR ) ){
			if ( chk = !!bar->SetButtons( ITB_btn, memcnt(ITB_btn) ) ){
				bar->SetHeight( TOL_HGT );
			}
		}
	}
	if ( chk ){
		if ( chk = cre_stc_( &bar->m_Stc_bdt, 0, 96, TBP_ID_BDATE_STATIC, WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE, bar ) ){
			if ( chk = cre_stc_( &bar->m_Lgi_usr, 1, 160, TBP_ID_EMPNAME_STATIC, WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE, bar ) ){
				if ( chk = cre_cbx_( &bar->m_Cmb_brc, 2, 144, TBP_ID_BRANCH_COMBO, bar ) ){
					if ( chk = cre_cbx_( &bar->m_Cmb_fcc, 3, 144, TBP_ID_FACILITY_COMBO, bar ) ){
						if ( chk = cre_fnt_( &bar->m_Cmb_font, 12, FW_NORMAL ) ){
							//bar->m_Cmb_brc.SetFont( &bar->m_Cmb_font );
							//bar->m_Cmb_fcc.SetFont( &bar->m_Cmb_font );
							bar->m_Cmb_brc.SetFont( _T("MS UI Gothic"), 12, RGB(0,0,0) );
							bar->m_Cmb_fcc.SetFont( _T("MS UI Gothic"), 12, RGB(0,0,0) );
							//bar->m_Cmb_fcc.m_NotifyWnd = wnd;
							//bar->m_Cmb_brc.m_NotifyWnd = wnd;

							bar->m_Stc_bdt.SetFont( _T("MS UI Gothic"), 16, RGB(0,0,0) );
							bar->m_Lgi_usr.SetFont( _T("MS UI Gothic"), 14, RGB(0,0,0) );

/*
							bar->SetButtonInfo(4, ID_SEPARATOR, TBBS_SEPARATOR, 12);
							if ( chk = cre_fnt_( &bar->m_Stc_font, 16, FW_EXTRABOLD ) ){
								bar->m_Stc_bdt.SetFont( &bar->m_Stc_font );
							}
							if ( chk = cre_fnt_( &bar->m_Lgi_font, 14, FW_EXTRABOLD ) ){
								bar->m_Lgi_usr.SetFont( &bar->m_Lgi_font );
							}
*/
						}
					}
				}
			}
		}
	}
	return( chk );
}

int		Mfm_Cre_RDr(			// Create a room direct tool bar
CRoomDirectToolBar	*bar,			// 個別情報バーポインタ
CWnd			*wnd )			// 親ウィンドウクラス
{
	int			chk;

	if ( chk = !!bar->CreateEx(wnd, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) ){
		//if ( chk = !!bar->LoadBitmap( IDB_BITMAP_INFOTOOLBAR ) ){
			if ( chk = !!bar->SetButtons( RDr_btn, memcnt(RDr_btn) ) ){
				bar->SetHeight( TOL_HGT );
			}
		//}
	}
	if ( chk ){
		chk  = cre_stc_( &bar->m_Stc_cin, 0, 36, IDC_STATIC, WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE, bar );
		chk &= cre_edt_( &bar->m_Edt_cin, 1, 48, TBP_ID_DCRCIN_EDIT, bar );
		chk &= cre_stc_( &bar->m_Stc_inh, 2, 36, IDC_STATIC, WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE, bar );
		chk &= cre_edt_( &bar->m_Edt_inh, 3, 48, TBP_ID_DCRINH_EDIT, bar );
		chk &= cre_stc_( &bar->m_Stc_cot, 4, 36, IDC_STATIC, WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE, bar );
		chk &= cre_edt_( &bar->m_Edt_cot, 5, 48, TBP_ID_DCRCOT_EDIT, bar );
		if ( chk ){
			if ( chk = cre_fnt_( &bar->m_font, 12, FW_NORMAL ) ){
				bar->m_Stc_cin.SetFont( &bar->m_font );
				bar->m_Stc_inh.SetFont( &bar->m_font );
				bar->m_Stc_cot.SetFont( &bar->m_font );
				bar->m_Stc_cin.SetWindowText( _T("C/I") );
				bar->m_Stc_inh.SetWindowText( _T("I/H") );
				bar->m_Stc_cot.SetWindowText( _T("C/O") );
				bar->m_Edt_cin.SetFont( _T("MS UI Gothic"), 16, RGB(0,0,0) );
				bar->m_Edt_inh.SetFont( _T("MS UI Gothic"), 16, RGB(0,0,0) );
				bar->m_Edt_cot.SetFont( _T("MS UI Gothic"), 16, RGB(0,0,0) );
				bar->m_Edt_cin.SetLimitText( 16 );
				bar->m_Edt_inh.SetLimitText( 16 );
				bar->m_Edt_cot.SetLimitText( 16 );

				//bar->m_Cmb_fcc.m_NotifyWnd = wnd;
				//bar->m_Cmb_brc.m_NotifyWnd = wnd;
				//bar->SetButtonInfo(6, ID_SEPARATOR, TBBS_SEPARATOR, 12);
			}
		}
	}
	return( chk );
}


// ローカルスレッド
typedef	struct	{				// データベース接続スレッドパラメータ
	Ini_fil		*ini;			// プログラム初期化情報
	WCHAR		*pth;			// データベースパス
	int			rsl;			// 処理結果
	WCHAR		*erm;			// エラーメッセージ格納領域
	CDatabase	*dbs;			// データベースオブジェクト
	HANDLE		cmp;			// 完了イベント
}DBC_PRM;

static	DWORD WINAPI dbc_trd( LPVOID prm )
{
	DBC_PRM		*dcp = (DBC_PRM *)prm;
	DBS_INI		*dbs = dcp->ini->dbs;

	dcp->rsl = Dbs_Cnc( dcp->dbs, dbs->dsn, dbs->dsn, dbs->uid, dcp->pth/*,dbs->pwd*/, &dcp->erm );

	SetEvent( dcp->cmp );

	return( 0 );
}

static	void	set_tbc(		// Tool bar combobox setting
CMainFrame		*mfm )
{
	Csr				csr=NULL;
	CInfoToolBar	*bar=&mfm->m_Inf_tbr;
	BRC_INF			*brc;
	FCL_INF			*fcc;
	int				idx=0;

	bar->m_Cmb_brc.ResetContent();
	while( csr = Nxt_Lnk( mfm->m_Pms_mng->brl, csr) ){
		brc = Rec_Of(BRC_INF, csr);
		bar->m_Cmb_brc.InsertString( -1, (WCHAR *)Fld_Ptr(brc->nam, FLD_WST) );
		if ( !wcscmp( (WCHAR *)Fld_Ptr(brc->brc,FLD_WST), mfm->m_Pms_mng->brc ) ){
			bar->m_Cmb_brc.SetCurSel( idx );
			mfm->m_Pms_mng->bri = brc;
		}
		bar->m_Cmb_brc.SetItemData( idx, (int)(LONG_PTR)brc );
		idx++;
	}

	idx = 0;
	bar->m_Cmb_fcc.ResetContent();
	while( csr = Nxt_Lnk( mfm->m_Pms_mng->fcl, csr) ){
		fcc = Rec_Of(FCL_INF, csr);
		if ( !wcscmp( (WCHAR *)Fld_Ptr(fcc->brc,FLD_WST), mfm->m_Pms_mng->brc ) ){
			bar->m_Cmb_fcc.InsertString( -1, (WCHAR *)Fld_Ptr(fcc->nam, FLD_WST) );
		}
		if ( !wcscmp( (WCHAR *)Fld_Ptr(fcc->fcc,FLD_WST), mfm->m_Pms_mng->fcc ) ){
			bar->m_Cmb_fcc.SetCurSel( idx );	
			mfm->m_Pms_mng->fci = fcc;
		}
		bar->m_Cmb_fcc.SetItemData( idx, (int)(LONG_PTR)fcc );
		idx++;
	}
}

static	int	opt_ini(			// Optimizing the .ini file
WCHAR			*dbp,			// database path
Ini_fil			*ini )			// target .ini data
{
	int			chk=TRUE;
	WCHAR		buf[192];

	if ( !!wcslen(ini->dbs->pwd) ){
		if ( chk = Psh_Str( buf, memcnt(buf), ini->dbs->pwd ) ){
			if ( chk = Set_PIni( SCT_DBS, IKY_CSC, buf ) ){
				if ( chk = Set_PIni( SCT_DBS, IKY_DBP, _T("") ) ){
					chk = Set_PIni( ini->fnm );
				}
			}
		}
	}
	if ( chk ){
		Pop_Str( dbp, ini->dbs->scc );
	}

	return( chk );
}

int		Mfm_Ini_Prc(			// Initialize process of MainFrame
CMainFrame		*mfm )
{
	DBC_PRM			dcp[1];
	DWORD			tid, evt;
	HANDLE			trd;
	int				chk=TRUE;
	WCHAR			*msg=NULL, *dbe=NULL, buf[20];
	CLoginDlg		*lgi;
	CMessageDlg		*dlg;
	WINDOWPLACEMENT	wpc[1];

	switch( mfm->m_Ini_stg ){
	case IST_INI:	// 初期化ファイル取得
					if ( chk = !!( mfm->m_Ini_inf = Get_PIni( INI_FNM ) ) ){
						opt_ini( mfm->m_Dbs_pth, mfm->m_Ini_inf );
						wcscpy_s( mfm->m_Pms_mng->opc, _countof(mfm->m_Pms_mng->opc), mfm->m_Ini_inf->opc );
						// 前回終了時のウィンドウ位置・サイズを復元する
						mfm->GetWindowPlacement( wpc );
						wpc->rcNormalPosition.left = *Rec_Of(int, Idx_Lnk( mfm->m_Ini_inf->wpc, 0 ));
						wpc->rcNormalPosition.top = *Rec_Of(int, Idx_Lnk( mfm->m_Ini_inf->wpc, 1 ));
						wpc->rcNormalPosition.right = *Rec_Of(int, Idx_Lnk( mfm->m_Ini_inf->wpc, 2 ));// - rect.left + 1;
						wpc->rcNormalPosition.bottom = *Rec_Of(int, Idx_Lnk( mfm->m_Ini_inf->wpc, 3 ));// - rect.top + 1;
						if ( mfm->m_Ini_inf->fls )	wpc->showCmd = SW_SHOWMAXIMIZED;
						mfm->SetWindowPlacement( wpc );
						// set branch_code ans facility code when last use to current
						wcscpy_s(mfm->m_Pms_mng->brc, memcnt(mfm->m_Pms_mng->brc), mfm->m_Ini_inf->obc );
						wcscpy_s(mfm->m_Pms_mng->fcc, memcnt(mfm->m_Pms_mng->brc), mfm->m_Ini_inf->ofc );
wcscpy_s(mfm->m_Pms_mng->dfl, memcnt(mfm->m_Pms_mng->dfl), _T("VN") );
						// ステージ移行							
						mfm->m_Ini_stg = IST_DBC;
					}
					else{
						msg = _T("Failed to load initialize data.");
					}
					break;
	case IST_DBC:	// データベース接続
					dlg = new CMessageDlg( mfm );
					dlg->ShowWindow( SW_NORMAL );
					dlg->SetMessage( _T("Connecting to database...") );
					memset( dcp, 0, sizeof(dcp) );
					dcp->ini = mfm->m_Ini_inf;
					dcp->dbs = mfm->m_Dbs_obj;
					dcp->cmp = CreateEvent( NULL, FALSE, FALSE, NULL );
					dcp->pth = mfm->m_Dbs_pth;
					if ( !!( trd = ::CreateThread( NULL, 0, dbc_trd, dcp, 0, &tid ) ) ){
						evt = WaitForSingleObject( dcp->cmp, mfm->m_Ini_inf->dbs->cto );
						switch( evt ){
						case WAIT_TIMEOUT:	msg = _T("Connecting database was timeout");
											chk = FALSE;
											//TerminateThread( trd, 0 );
											//CloseHandle( trd );
											break;
						case WAIT_OBJECT_0:	if ( !( chk = dcp->rsl ) ){
												msg = dcp->erm ? dcp->erm: _T("Unknown error");
											}
											break;
						default:			msg = _T("System error");
											chk = FALSE;
											break;
						}
						if ( dcp->erm )	free( dcp->erm );
					}
					dlg->DestroyWindow();
#ifdef	_DEBUG
//wcscpy_s(mfm->m_Pms_mng->lgi->ucc, memcnt(mfm->m_Pms_mng->lgi->ucc), _T("EN") );  
#endif
					mfm->m_Ini_stg = IST_MST;
					break;
	case IST_MST:	// マスターデータ取得
					dlg = new CMessageDlg( mfm );
					dlg->ShowWindow( SW_NORMAL );
					dlg->SetMessage( _T("Getting each master datas...") );
					if ( !( chk = Get_Ech_Mst( &dbe, mfm->m_Dbs_obj, mfm->m_Pms_mng ) ) ){
						if ( !!dbe )	msg = dbe;
					}
					set_tbc( mfm );
					dlg->DestroyWindow();
					mfm->m_Ini_stg = IST_BDT;
					break;
	case IST_BDT:	// Getting business_date
					dlg = new CMessageDlg( mfm );
					dlg->ShowWindow( SW_NORMAL );
					dlg->SetMessage( _T("Getting business_date...") );
					if ( chk = Get_Bdt( mfm->m_Pms_mng->bdt, &dbe, mfm->m_Dbs_obj, mfm->m_Pms_mng ) ){
						mfm->m_Inf_tbr.m_Stc_bdt.SetWindowText(Edt_Dte(buf, memcnt(buf), mfm->m_Pms_mng->bdt));
					}
					else{
						if ( !!dbe )	msg = dbe;
					}
					dlg->DestroyWindow();
					mfm->m_Ini_stg = IST_LGI;
					break;
	case IST_LGI:	if ( chk = !!( lgi = new CLoginDlg(mfm) ) ){
						chk = lgi->DoModal() == IDOK;
						delete lgi;
					}
					if ( chk )	mfm->SetLoginUser( mfm->m_Pms_mng->lgi );
					mfm->m_Ini_stg = IST_SWD;
					break;
	case IST_SWD:	// 予約検索ウィンドウの作成
					((CHotelPMSApp *)AfxGetApp())->pDocTemplate[VWK_BST]->OpenDocumentFile(NULL);
					((CHotelPMSApp *)AfxGetApp())->pDocTemplate[VWK_SCH]->OpenDocumentFile(NULL);
					mfm->m_Ini_stg = IST_END;
					break;
	}
	if ( !!msg )	mfm->MessageBox( msg, NULL, MB_ICONSTOP|MB_OK );
	if ( !!dbe )	free( dbe );
	if ( chk && mfm->m_Ini_stg!=IST_END ){
		mfm->SetTimer( TEV_INI, TIM_INI, NULL );		// Go to next stage
	}
	else{
		if ( chk )	mfm->SetTimer( TEV_ITV, TIM_ITV, NULL );	// Lounch interval timer
	}
	return( chk );
}

int		Mfm_Itv_Prc(			// Interval process of MainFrame
CMainFrame		*mfm )
{
	int			chk=TRUE;
	WCHAR		*msg;
	Day			bdt[1];

	if ( ++mfm->m_GBd_cnt==GBD_CNT ){	// Getting business date
		if ( chk = Get_Bdt( bdt, &msg, mfm->m_Dbs_obj, mfm->m_Pms_mng ) ){
			if ( !!Cmp_Dte(mfm->m_Pms_mng->bdt, bdt) ){
				*mfm->m_Pms_mng->bdt = *bdt;
				mfm->SetBusinessDate( bdt ); 
			}
		}
		else{
			if ( msg ){
				mfm->MessageBox( msg, NULL, MB_ICONSTOP|MB_OK );
				free( msg );
			}
		}
		mfm->m_GBd_cnt = 0;
	}

	if ( chk ){
		if ( mfm->m_Ini_stg==IST_END ){
			mfm->SetTimer( TEV_ITV, TIM_ITV, NULL );
		}
		else{
			mfm->SetTimer( TEV_INI, TIM_INI, NULL );		// for after logout stage
		}
	}

	return( chk );
}

CView	*Mfm_Get_Bsv(	// Getting the BookingStatusView pointer
CMainFrame	*mfm )
{
	POSITION		tps, dps, vps;
	CDocTemplate	*tmp;
	CDocument		*doc;
	CView			*viw;
	int				fnd = FALSE;

	tps = ((CHotelPMSApp *)AfxGetApp())->GetFirstDocTemplatePosition();
	while( !fnd && !!tps ){
		tmp = ((CHotelPMSApp *)AfxGetApp())->GetNextDocTemplate( tps );
		dps = tmp->GetFirstDocPosition();
		while ( !fnd && !!dps ){
			doc = tmp->GetNextDoc( dps );
			vps = doc->GetFirstViewPosition();
			while ( !fnd && !!vps ){
				viw = doc->GetNextView(vps);
				fnd = ((CChildFrame *)viw->GetParent())->m_Viw_knd == VWK_BST;
			}
		}
	}

	return( fnd ? viw: NULL );
}

void		Mfm_Cre_Rsv(					// Create a new reservation
CMainFrame	*mfm )
{
	CMultiDocTemplate	*tmp=((CHotelPMSApp *)AfxGetApp())->pDocTemplate[VWK_RSV];
	CReservationDoc		*doc;
	CReservationView	*viw;
	POSITION			pos;

	doc = (CReservationDoc *)tmp->OpenDocumentFile(NULL);
	if ( !!( pos = doc->GetFirstViewPosition() ) ){
		viw = (CReservationView *)doc->GetNextView(pos);
		if ( !viw->CreateData( NULL ) ){
			doc->OnCloseDocument();
		}
	}
}

static	int		viw_opn(			// Confirm to exist views which are opening
void )
{
	CMultiDocTemplate	*tmp;
//	CDocument			*doc;
	POSITION			pos;
	int					exs=FALSE, idx;
	int					vwk[] = { /*VWK_SCH,*/VWK_RSV,/*VWK_BST,*/VWK_CIN,VWK_INH,VWK_COT,VWK_NAD,VWK_HKP };

	for ( idx=0; idx<memcnt(vwk); idx++ ){
		tmp = ((CHotelPMSApp *)AfxGetApp())->pDocTemplate[vwk[idx]];
		if ( exs = !!( pos = tmp->GetFirstDocPosition() ) ){
			break;
		}
	}

	return( exs );
}

static	int		viw_cls(			// Close to exist views which are opening
void )
{
	CMultiDocTemplate	*tmp;
	CDocument			*doc;
	POSITION			pos;
	int					exs=FALSE, idx;
	int					vwk[] = { VWK_SCH,VWK_RSV,VWK_BST,VWK_CIN,VWK_INH,VWK_COT,VWK_NAD,VWK_HKP };

	for ( idx=0; idx<memcnt(vwk); idx++ ){
		tmp = ((CHotelPMSApp *)AfxGetApp())->pDocTemplate[vwk[idx]];
		pos = tmp->GetFirstDocPosition();
		while( pos ){
			doc = (CDocument *)tmp->GetNextDoc(pos);
			doc->OnCloseDocument();
		}
	}

	return( exs );
}

int			Mfm_Log_Out(					// Exec logout
CMainFrame	*mfm,
int			fce )							// force logout
{
	DWORD		ans;

	if ( ( ans = !viw_opn() ? IDYES: IDNO ) == IDNO ){
		ans = mfm->MessageBox( 
			_T("The data will discard if you don't close each view.\n")
			_T("Do you continue?"), NULL, MB_ICONQUESTION|MB_YESNO );
	}
	if ( ans==IDYES ){
		viw_cls();
		mfm->SetLoginUser( NULL );
		mfm->m_Ini_stg = IST_LGI;
	}

	return( ans==IDYES );
}

int			Mfm_Chg_Pwd(					// Change password
CMainFrame	*mfm )
{
	CChangePwdDlg		*dlg;

	if ( dlg = new CChangePwdDlg( mfm ) ){
		dlg->DoModal();
		delete dlg;
	}

	return( TRUE );
}

static	int		exv_cbk(
int		vkd,
CView	*viw,
LPVOID	prm )
{
	DRL_PRM		*drl=(DRL_PRM *)(LONG_PTR)prm;

	return( ((CChildFrame *)(viw->GetParent()))->m_Rsv_id == DBS_INT_VAL(drl->rii) );
}

static	int		rvw_cbk(
int		vkd,
CView	*viw,
LPVOID	prm )
{
	int			chk=FALSE;
	DRL_PRM		*drl=(DRL_PRM *)(LONG_PTR)prm;

	switch( vkd ){
	case VWK_RSV:	chk = ((CReservationView *)viw)->GetData( DBS_INT_VAL(drl->rii)	);							break;
	case VWK_CIN:	chk = ((CCheckinView *)viw)->GetData( DBS_INT_VAL(drl->rii) );								break;
	case VWK_INH:	chk = ((CInhouseView *)viw)->GetData( DBS_INT_VAL(drl->rii), DBS_INT_VAL(drl->rdi), FALSE );break;
	case VWK_COT:	chk = ((CCheckoutView *)viw)->GetData( DBS_INT_VAL(drl->rii), DBS_INT_VAL(drl->rdi) );		break;
	}

	return( chk );
}

void		Mfm_Dir_Rch(					// Launch direct
CMainFrame	*mfm,
DWORD		cid )							// Inputted edit control id
{
	int				knd, cnt;
	WCHAR			buf[128], *erm=NULL, *nam=_T("?????");
	DRL_PRM			prm[1];

	mfm->m_wndRoomDirectBar.GetDlgItem(cid)->GetWindowText( buf, memcnt(buf) );

	if ( !!wcslen(buf) ){
		switch( cid ){
		case TBP_ID_DCRCIN_EDIT:	knd = VWK_CIN;	nam=_T("checkin");	break;
		case TBP_ID_DCRINH_EDIT:	knd = VWK_INH;	nam=_T("inhouse");	break;
		case TBP_ID_DCRCOT_EDIT:	knd = VWK_COT;	nam=_T("checkout");	break;
		}
		if ( ( cnt = Mfm_Get_Rsv( prm, buf, knd, &erm, mfm->m_Dbs_obj, mfm->m_Pms_mng ) ) != -1 ){
			if ( cnt==1 ){
				Rnc_Ech_Viw( knd, exv_cbk, rvw_cbk, prm );

			}
			else{
				swprintf_s( buf, memcnt(buf), _T("Room number not found in %s list."), nam );
				mfm->MessageBox( buf, NULL, MB_ICONEXCLAMATION|MB_OK );
			}
		}
		else{
			if ( erm ){
				mfm->MessageBox( erm, NULL, MB_ICONEXCLAMATION|MB_OK ); 
				free( erm );
			}
		}
		mfm->m_wndRoomDirectBar.GetDlgItem(cid)->SetWindowText( _T("") );
	}
}
