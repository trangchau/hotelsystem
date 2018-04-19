// ToolBarCombo.cpp : インプリメンテーション ファイル
//

#include <afxwin.h>
#include "ToolBarCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBarCombo

CToolBarCombo::CToolBarCombo()
{
	m_NotifyWnd = NULL;
}

CToolBarCombo::~CToolBarCombo()
{
}


BEGIN_MESSAGE_MAP(CToolBarCombo, CComboBox)
	//{{AFX_MSG_MAP(CToolBarCombo)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarCombo メッセージ ハンドラ

void CToolBarCombo::OnSelchange() 
{
	UINT	nID = GetWindowLong(m_hWnd, GWL_ID);

	if ( m_NotifyWnd ){
		m_NotifyWnd->PostMessage( WM_COMMAND, MAKELPARAM( nID, CN_COMMAND), (LPARAM)m_hWnd );
	}
}
