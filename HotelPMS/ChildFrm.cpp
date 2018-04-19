// ChildFrm.cpp : CChildFrame �N���X�̎���
//
#include "stdafx.h"
#include "HotelPMS.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include ".\fnc\viw_cmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_GETMINMAXINFO()
	ON_WM_NCACTIVATE()
	ON_WM_MOVING()
END_MESSAGE_MAP()


// CChildFrame �R���X�g���N�V����/�f�X�g���N�V����

CChildFrame::CChildFrame()
{
	m_Ini_upd = FALSE;
	m_Rsv_id = 0;
	m_Nsz_viw = FALSE;
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs ��ύX���āAWindow �N���X�܂��̓X�^�C����ύX���܂��B
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame �f�f

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame ���b�Z�[�W �n���h��

// �E�B���h�E�T�C�Y�v���C�x���g
void CChildFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	WINDOWPLACEMENT		wpl[1];

	if ( !m_Ini_upd ){
//		if ( m_Viw_knd==VWK_SCH ){
//			CMenu		*mnu;
//			mnu = GetSystemMenu( FALSE );
//			mnu->EnableMenuItem( SC_CLOSE, MF_BYCOMMAND|MF_GRAYED );
//			ModifyStyle( SW_MAXIMIZE, 0, FALSE );
//		}
		GetWindowPlacement( wpl );
		SetWindowPos( NULL, wpl->rcNormalPosition.left, 0, 0, 0, SWP_NOSIZE|SWP_NOZORDER );
	}
	else{	// �������̃r���[�́A�E�B���h�E���T�C�Y���֎~����
		GetWindowPlacement( wpl );
		lpMMI->ptMinTrackSize.x = m_Win_siz->x;
		lpMMI->ptMinTrackSize.y = m_Win_siz->y;
		if ( m_Nsz_viw ){
			lpMMI->ptMaxTrackSize.x = m_Win_siz->x;
			lpMMI->ptMaxTrackSize.y = m_Win_siz->y;
		}
		SetWindowPos( &wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
	}

	CMDIChildWnd::OnGetMinMaxInfo(lpMMI);
}

BOOL CChildFrame::OnNcActivate(BOOL bActive)
{
	CWnd		*pcr = ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->m_Dte_pcr;

	if ( !bActive ){
		if ( pcr )	pcr->ShowWindow( SW_HIDE );
	}
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	return CMDIChildWnd::OnNcActivate(bActive);
}

void CChildFrame::OnMoving(UINT fwSide, LPRECT pRect)
{
	CMDIChildWnd::OnMoving(fwSide, pRect);
	CWnd		*pcr = ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->m_Dte_pcr;

	if ( pcr )	pcr->ShowWindow( SW_HIDE );
}
