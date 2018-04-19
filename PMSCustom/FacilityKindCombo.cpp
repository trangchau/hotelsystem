// FacilityKindCombo.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include <afxwin.h>
#include "FacilityKindCombo.h"
#include "..\pms_def.h"

#define		wsizeof(x)	((x)*sizeof(WCHAR))

// CFacilityKindCombo

IMPLEMENT_DYNAMIC(CFacilityKindCombo, CCustomCombo)

CFacilityKindCombo::CFacilityKindCombo()
{
}

CFacilityKindCombo::~CFacilityKindCombo()
{
}

BEGIN_MESSAGE_MAP(CFacilityKindCombo, CComboBox)
END_MESSAGE_MAP()

static	struct	{
	WCHAR	*nam;
	int		cod;
}Fix_[] = {
	{ _T("Hotel"),		FCK_HOTEL		},	
	{ _T("SPA"),		FCK_SPA			},	
	{ _T("Sports"),		FCK_SPORTS		},	
	{ _T("Shop"),		FCK_SHOP		},	
	{ _T("Restaurant"),	FCK_RESTAURANT	},	
	{ _T("CASINO"),		FCK_CASINO		}	
};

void	CFacilityKindCombo::SetKind( int knd )
{
	int			idx;

	for ( idx=0; idx<memcnt(Fix_); idx++ ){
		if ( knd==Fix_[idx].cod ){
			SetCurSel( idx );
			break;
		}
	}
}

int		CFacilityKindCombo::GetKind( void )
{
	int			knd=-1, idx = GetCurSel();

	if ( idx >= 0 )	knd = Fix_[idx].cod;

	return( knd );
}

void CFacilityKindCombo::PreSubclassWindow()
{
	int			idx;

	ResetContent();
	for ( idx=0; idx<memcnt(Fix_); idx++ ){
		InsertString( -1, Fix_[idx].nam );
	}

	CCustomCombo::PreSubclassWindow();
}
