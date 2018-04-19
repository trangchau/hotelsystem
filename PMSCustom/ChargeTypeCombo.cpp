// ChargeTypeCombo.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include <afxwin.h>
#include "ChargeTypeCombo.h"
#include "..\pms_def.h"

#define		wsizeof(x)	((x)*sizeof(WCHAR))

// CChargeTypeCombo

IMPLEMENT_DYNAMIC(CChargeTypeCombo, CCustomCombo)

static	CGT_	Cgt_[] =	{
	{ CGP_HOTEL_STAY,			_T("Stay")			},
	{ CGP_HOTEL_DAYUSE,			_T("Day Use")		},
	{ CGP_HOTEL_RESIDENCE,		_T("Regidence")		},
	{ CGP_HOTEL_EARLY_CHECKIN,	_T("E C/I(Time)")	},
	{ CGP_HOTEL_LATE_CHECKOUT,	_T("L C/O(Time)")	},
	{ CGP_GOLF_NORMAL,			_T("Normal")		},
	{ CGP_GOLF_RESERVED,		_T("Reservaed")		},
	{ CGP_GOLF_MEMBER,			_T("Member")		},
	{ CGP_GOLF_VISITOR,			_T("Visitor")		},
	{ CGP_LOCKER_NORMAL,		_T("Normal")		},
	{ CGP_MASSERGE_NORMAL,		_T("Normal")		}
};



CChargeTypeCombo::CChargeTypeCombo()
{
	m_Eqp_knd = -1;
}

CChargeTypeCombo::~CChargeTypeCombo()
{
}

BEGIN_MESSAGE_MAP(CChargeTypeCombo, CComboBox)
END_MESSAGE_MAP()

void	CChargeTypeCombo::SetEquipmentKind( int knd )
{
	Csr			csr=NULL;
	int			idx;

	if ( m_Eqp_knd != knd ){
		m_Eqp_knd = knd;
		ResetContent();
		for ( idx=0; idx<memcnt(Cgt_); idx++ ){
			if ( !!( Cgt_[idx].ptc & (knd<<8) ) ){
				SetItemData( InsertString( -1, Cgt_[idx].nam ), Cgt_[idx].ptc );
			}
		}
	}
}

void	CChargeTypeCombo::SetItemIndex(	// getting a booking path index
int				ptc )					// perticular
{
	int			cnt=GetCount(), idx;

	SetCurSel( -1 );
	for ( idx=0; idx<cnt; idx++ ){
		if ( GetItemData(idx) == ptc ){
			SetCurSel(idx);
			break;
		}
	}
}

int		CChargeTypeCombo::GetItemIndex()	// getting a particular
{
	int			idx=GetCurSel();
	return( idx==-1 ? -1: (int)GetItemData(idx) );
}

WCHAR	*CChargeTypeCombo::GetItemName( int ptc )	// getting a charge type name
{
	int			idx;
	WCHAR		*nam=_T("Unknown");

	for ( idx=0; idx<memcnt(Cgt_); idx++ ){
		if ( Cgt_[idx].ptc == ptc ){
			nam = Cgt_[idx].nam;
			break;
		}
	}

	return( nam );
}
