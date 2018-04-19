// DatePickerDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "DatePickerDlg.h"

//#define	CRN_WID		280
//#define	CRN_HGT		180
#define	CRN_WID		220
#define	CRN_HGT		140

#define	IDC_CALENDER_LEFT		1
#define	IDC_CALENDER_RIGHT		2

// CDatePickerDlg ダイアログ

IMPLEMENT_DYNAMIC(CDatePickerDlg, CDialog)

CDatePickerDlg::CDatePickerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDatePickerDlg::IDD, pParent)
{
	m_Lft_cdr = NULL;
	m_Rgt_cdr = NULL;
	m_Ntf_wnd = NULL;
	m_Dul_mod = CDR_DLG_DUAL;
	memset( m_Def_dte, 0, sizeof(m_Def_dte) );
	Create(CDatePickerDlg::IDD,pParent);
}

CDatePickerDlg::~CDatePickerDlg()
{
	if ( m_Lft_cdr )	delete m_Lft_cdr;
	if ( m_Rgt_cdr )	delete m_Rgt_cdr;
}

void CDatePickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDatePickerDlg, CDialog)
	ON_USER_CalendarNOTIFY()
	ON_BN_CLICKED(IDOK, &CDatePickerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDatePickerDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

void	CDatePickerDlg::Initialize( int iMode, int iYear, int iMonth )
{
	RECT		rct[1];


	GetClientRect( rct );
	rct->left = 0;
	rct->top = 0;

	rct->right = rct->left + CRN_WID;
	rct->bottom = rct->top + CRN_HGT;

	m_Lft_cdr = new CCalendar( this );
	m_Lft_cdr->Create( iYear, iMonth, rct, 1024, IDC_CALENDER_LEFT, iMode==CDR_DLG_SINGLE ? 0: UCN_CSP_LEFT );
//	m_Lft_cdr->SetLanguage( m_Trc_mng->lng==LNG_JPN ? CLG_JAPANESE: CLG_ENGLISH );
	m_Lft_cdr->SetLanguage( CLG_ENGLISH );
	m_Lft_cdr->SetFont( _T("MS UI Gothic"), FALSE );
	m_Lft_cdr->SetSelMode( iMode==CDR_DLG_SINGLE ? UCN_MOD_SINGLE: UCN_MOD_MULTI );
	m_Lft_cdr->SetLimit( iYear, iMonth, iYear, iMonth+12 );

	if ( iMode==CDR_DLG_DUAL ){
		rct->left += CRN_WID;	rct->right += CRN_WID;
		m_Rgt_cdr = new CCalendar( this );
		m_Rgt_cdr->Create( 2018, 2, rct, 1024, IDC_CALENDER_RIGHT, UCN_CSP_RIGHT );
		m_Rgt_cdr->SetLanguage( CLG_ENGLISH );
		m_Rgt_cdr->SetFont( _T("MS UI Gothic"), FALSE );
		m_Rgt_cdr->SetSelMode( UCN_MOD_MULTI );
		m_Rgt_cdr->SetLimit( 2018, 1, 2018, 12 );
	}

	SetWindowPos( NULL, 0, 0, CRN_WID*(iMode==CDR_DLG_SINGLE ? 1: 2)+2, CRN_HGT, SWP_NOMOVE|SWP_NOZORDER	);
	m_Dul_mod = iMode;
}

void	CDatePickerDlg::SetNotifyWindow( CWnd *wnd )
{
	m_Ntf_wnd = wnd;
}

// CDatePickerDlg メッセージ ハンドラ

BOOL CDatePickerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//SetWindowPos( 

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

LRESULT CDatePickerDlg::OnCalendarNotify( WPARAM wParam, LPARAM lParam )
{
	TFCLNDRNTFY		*ntf = (TFCLNDRNTFY *)lParam;
	int				cnt;
	TFCNSELDATE		*sel;
	CCalendar		*cdr;
	DWORD			dte;

	switch( wParam ){
	case UCN_NEEDHOLIDAY:	//for ( idx=0; idx<memcnt(m_EditBuf->knd[ntf->iMonth-1]); idx++ ){
							//	ntf->pHoliday[idx] = (char)(m_EditBuf->knd[ntf->iMonth-1][idx] | 0x30);
							//}
							break;
	case UCN_INCREMENT:		if ( m_Dul_mod==CDR_DLG_DUAL ){
								if ( ntf->dwUser==IDC_CALENDER_LEFT )	m_Rgt_cdr->Increment();
								else									m_Lft_cdr->Increment();
							}
							break;
	case UCN_DECREMENT:		if ( m_Dul_mod==CDR_DLG_DUAL ){
								if ( ntf->dwUser==IDC_CALENDER_LEFT )	m_Rgt_cdr->Decrement();
								else									m_Lft_cdr->Decrement();
							}
							break;
	case UCN_SEL_DAY:		cdr = ntf->dwUser==IDC_CALENDER_LEFT ? m_Lft_cdr: m_Rgt_cdr;
							if ( sel = cdr->GetSelDays( &cnt ) ){
								dte = sel->iYear * 10000 + sel->iMonth * 100 + sel->iDay;
								m_Ntf_wnd->PostMessageW( WM_DATEPICKERNOTIFY, DPC_SELECTDATE, (LPARAM)dte );
							}
							break;
	case UCN_NEEDSELDAYS:	//*dtp = *m_Def_dte;
							//dtp->day = 1;
							//idx = Get_Dow( dtp );
							//ntf->pSelect[idx+m_Def_dte->day-1] = '1';
							ntf->iYear = m_Def_dte->yer;
							ntf->iMonth = m_Def_dte->mth;
							ntf->iDay = m_Def_dte->day;
							break;
	}

	return( TRUE );
}

void CDatePickerDlg::OnBnClickedOk()
{
	DestroyWindow();
	//OnOK();
}

void CDatePickerDlg::OnBnClickedCancel()
{
	DestroyWindow();
	//OnCancel();
}

void CDatePickerDlg::PostNcDestroy()
{

	delete	this;
//	CDialog::PostNcDestroy();
}


// Operations
void CDatePickerDlg::SetPosition( CWnd *par, CWnd *wnd )
{
	WINDOWPLACEMENT		wpc[1];
	RECT				rct[1], dtp[1];
	int					sx, sy;

	GetDesktopWindow()->GetClientRect( dtp );

	wnd->GetWindowPlacement( wpc );
	*rct = wpc->rcNormalPosition;
	par->ClientToScreen( rct );
	if ( rct->left + CRN_WID > dtp->right )	sx = rct->left - CRN_WID;
	else									sx = rct->left;
	if ( rct->top - CRN_HGT < 0 )			sy = rct->bottom;
	else									sy = rct->top - CRN_HGT;
	SetWindowPos( NULL, sx<0 ? 0: sx, sy, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOACTIVATE );
}

void CDatePickerDlg::SetDefaultDate( Day *dte )
{
	//if ( m_Lft_cdr )	m_Lft_cdr->SetReserveDate( dte->yer, dte->mth, dte->day );
	//if ( m_Rgt_cdr )	m_Rgt_cdr->SetReserveDate( dte->yer, dte->mth, dte->day );
	if ( m_Lft_cdr ){
		*m_Def_dte = *dte;
		//m_Rgt_cdr->SetReserveDate( dte->yer, dte->mth, dte->day );
		m_Lft_cdr->ResetSelect();
		m_Lft_cdr->ChangeYears( dte->yer, dte->mth );
		m_Lft_cdr->SetSelDays();
	}
	if ( m_Rgt_cdr )	m_Rgt_cdr->SetReserveDate( dte->yer, dte->mth, dte->day );
}




void CDatePickerDlg::SetLimit( int iMinYear, int iMinMonth, int iMaxYear, int iMaxMonth )
{
	if ( m_Lft_cdr )	m_Lft_cdr->SetLimit( iMinYear, iMinMonth, iMaxYear, iMaxMonth );
	if ( m_Rgt_cdr )	m_Rgt_cdr->SetLimit( iMinYear, iMinMonth, iMaxYear, iMaxMonth );
}

BOOL CDatePickerDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_POPUP;

	return CDialog::PreCreateWindow(cs);
}
