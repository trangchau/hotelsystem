// ClientRankCombo.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include <afxwin.h>
#include "ClientRankCombo.h"
#include "..\pms_def.h"

#define		wsizeof(x)	((x)*sizeof(WCHAR))

// CClientRankCombo

IMPLEMENT_DYNAMIC(CClientRankCombo, CCustomCombo)

CClientRankCombo::CClientRankCombo()
{
}

CClientRankCombo::~CClientRankCombo()
{
}

BEGIN_MESSAGE_MAP(CClientRankCombo, CComboBox)
END_MESSAGE_MAP()

static	struct	{
	WCHAR	*nam;
	int		cod;
}Fix_[] = {
	{ _T("Very Important Person(VIP)"),	CLL_VIP		},	// Very Important Person
	{ _T("Dominan"),					CLL_DOMINANT},	// Dominant Person
	{ _T("General"),					CLL_GENERAL	},	// General Person
	{ _T("Undesirable Guest(UG)"),		CLL_UG		}	// Undesirable Guest
};

void	CClientRankCombo::SetRank( int cll )
{
	int			idx;

	for ( idx=0; idx<memcnt(Fix_); idx++ ){
		if ( cll==Fix_[idx].cod ){
			SetCurSel( idx );
			break;
		}
	}
}

int		CClientRankCombo::GetRank( void )
{
	int			cll=-1, idx = GetCurSel();

	if ( idx >= 0 )	cll = Fix_[idx].cod;

	return( cll );
}

void CClientRankCombo::PreSubclassWindow()
{
	int			idx;

	ResetContent();
	for ( idx=0; idx<memcnt(Fix_); idx++ ){
		InsertString( -1, Fix_[idx].nam );
	}

	CCustomCombo::PreSubclassWindow();
}
