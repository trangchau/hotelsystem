// ChargeKindCombo.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include <afxwin.h>
#include "ChargeKindCombo.h"
#include "..\pms_def.h"

#define		wsizeof(x)	((x)*sizeof(WCHAR))

// CChargeKindCombo

IMPLEMENT_DYNAMIC(CChargeKindCombo, CCustomCombo)

static	CGK_	Cgk_[] =	{
	{ CGK_ONCE,			_T("Once")		},
	{ CGK_TIME,			_T("Time")		},
	{ CGK_EXTEND_TIME,	_T("Ext.Time")	}
};



CChargeKindCombo::CChargeKindCombo()
{
}

CChargeKindCombo::~CChargeKindCombo()
{
}

BEGIN_MESSAGE_MAP(CChargeKindCombo, CComboBox)
END_MESSAGE_MAP()

void	CChargeKindCombo::SetData( void )
{
	int			idx;

	ResetContent();
	for ( idx=0; idx<memcnt(Cgk_); idx++ ){
		SetItemData( InsertString( -1, Cgk_[idx].nam ), Cgk_[idx].cgk );
	}
}

void	CChargeKindCombo::SetItemIndex(	// gstting a charge kind
int				cgk )					// charge kind
{
	int			cnt=GetCount(), idx;

	SetCurSel( -1 );
	for ( idx=0; idx<cnt; idx++ ){
		if ( GetItemData(idx) == cgk ){
			SetCurSel(idx);
			break;
		}
	}
}

int		CChargeKindCombo::GetItemIndex()	// getting a charge kind
{
	int			idx=GetCurSel();
	return( idx==-1 ? -1: (int)GetItemData(idx) );
}

WCHAR	*CChargeKindCombo::GetItemName( int cgk )	// getting a charge kind name
{
	int			idx;
	WCHAR		*nam=_T("Unknown");

	for ( idx=0; idx<memcnt(Cgk_); idx++ ){
		if ( Cgk_[idx].cgk == cgk ){
			nam = Cgk_[idx].nam;
			break;
		}
	}

	return( nam );
}
