// RsvSchPanel.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include <afxwin.h>
#include "RsvSchPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKosuStatic

CRsvSchPanel::CRsvSchPanel()
{
}

CRsvSchPanel::~CRsvSchPanel()
{
}

BEGIN_MESSAGE_MAP(CRsvSchPanel, CStatic)
	//{{AFX_MSG_MAP(CRsvSchPanel)
	ON_WM_GRIDNOTIFY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKosuStatic ���b�Z�[�W �n���h��

// �O���b�h�C�x���g
LRESULT CRsvSchPanel::OnGridNotify( WPARAM wParam, LPARAM lParam )
{
	return( GetParent()->SendMessage( WM_GRIDNOTIFY, wParam, lParam ) ); 
}
