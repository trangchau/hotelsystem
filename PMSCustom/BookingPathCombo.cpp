// BookingPathCombo.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include <afxwin.h>
#include "BookingPathCombo.h"
#include "..\pms_def.h"

#define		wsizeof(x)	((x)*sizeof(WCHAR))

// CBookingPathCombo

IMPLEMENT_DYNAMIC(CBookingPathCombo, CCustomCombo)

CBookingPathCombo::CBookingPathCombo()
{
	m_Agt_dat = NULL;
}

CBookingPathCombo::~CBookingPathCombo()
{
	if ( !!m_Agt_dat ){
		free( m_Agt_dat );
	}
}

BEGIN_MESSAGE_MAP(CBookingPathCombo, CComboBox)
END_MESSAGE_MAP()

static	struct	{
	WCHAR	*nam;
	AGT_	dat[1];
}Fix_[] = {
	{ _T("Telephone"),		{ VIA_TELEPHONE, 0	}},	// Reservation via tel.
	{ _T("Fax"),			{ VIA_FAX,       0	}},	// Reservation via fax
	{ _T("email"),			{ VIA_EMAIL,     0	}},	// Reservation via direct email
	{ _T("Walk In"),		{ VIA_WALKIN,    0	}},	// Walkin checkin
	{ _T("Web Reservation"),{ VIA_WEBSITE,   0	}}	// Resrevation via direct WebSite
};

void	CBookingPathCombo::SetAgentData( Csr rot )
{
	Csr			csr=NULL;
	AGT_INF		*agt;
	int			idx, pos;

	if ( !!( m_Agt_dat = (AGT_ *)calloc( sizeof(AGT_), memcnt(Fix_) + Cnt_Lnk(rot) ) ) ){
		ResetContent();
		while( csr=Nxt_Lnk(rot, csr) ){
			agt = Rec_Of(AGT_INF, csr);
			idx = InsertString( -1, (WCHAR *)Fld_Ptr(agt->nam, FLD_WST) );
			(m_Agt_dat+idx)->pth = VIA_AGENT;
			(m_Agt_dat+idx)->agi = DBS_INT_VAL(agt->id);
			wcscpy_s( (m_Agt_dat+idx)->nam, memcnt((m_Agt_dat+idx)->nam), DBS_WST_PTR(agt->nam) );
		}
		for ( idx=0; idx<memcnt(Fix_); idx++ ){
			*(m_Agt_dat+(pos=InsertString( -1, Fix_[idx].nam ))) = *Fix_[idx].dat;
			wcscpy_s( (m_Agt_dat+pos)->nam, memcnt((m_Agt_dat+idx)->nam),  Fix_[idx].nam );
		}
	}
}

void	CBookingPathCombo::SetItemIndex(	// getting a booking path index
int				pth,						// Path kind
int				agi )						// id
{
	int			cnt, idx=-1;

	if ( !!m_Agt_dat ){
		if ( ( cnt = GetCount() ) != -1 ){
			for ( idx=0; idx<cnt; idx++ ){
				if ( (m_Agt_dat+idx)->pth == pth && (m_Agt_dat+idx)->agi==agi ){
					break;
				}
			}
		}
	}
	SetCurSel(idx<cnt ? idx: -1);
}

void	CBookingPathCombo::GetItemIndex(	// getting a booking path index
int				*pth,						// Path kind storing pointer
int				*agi )						// id storing pointer
{
	int			idx=GetCurSel();

	if ( idx>=0 ){
		*pth = (m_Agt_dat+idx)->pth;
		*agi = (m_Agt_dat+idx)->agi;
	}
	else{
		*pth = -1;
		*agi = 0;
	}
}

int		CBookingPathCombo::GetFixedRoot(	// getting fixsd booking path
Csr			dst,							// storing root
int			ufd )							// Unify DIRECT
{
	int				idx, drc=0;
	Csr				csr;

	for ( idx=0; idx<memcnt(Fix_); idx++ ){
		if ( !ufd || !((Fix_+idx)->dat->pth & VIA_DIRECT) || !drc++ ){
			if ( csr = Add_Lnk( dst, NULL, sizeof(DWORD) * 2 ) ){
				*Rec_Of(DWORD, csr) = drc==1 ? VIA_DIRECT: (Fix_+idx)->dat->pth;
				*(Rec_Of(DWORD, csr)+1) = ( drc==1 ? (DWORD)(LONG_PTR)_T("Direct"): (DWORD)(LONG_PTR)(Fix_+idx)->nam );
			}
			else{
				break;
			}
		}
	}
	if ( idx<memcnt(Fix_) )	Del_All( dst );

	return( idx>=memcnt(Fix_) );
}

WCHAR	*CBookingPathCombo::GetPathName(		// getting booking path name
int			pth,								// booking path
int			aii )								// agent_info_id
{
	int				cnt, idx=-1;
	WCHAR			*nam=NULL;

	if ( !!m_Agt_dat ){
		if ( ( cnt = GetCount() ) != -1 ){
			for ( idx=0; idx<cnt; idx++ ){
				if ( pth == (m_Agt_dat+idx)->pth && aii==(m_Agt_dat+idx)->agi ){
					nam = (m_Agt_dat+idx)->nam;
					break;
				}
			}
		}
	}

	return( nam );
}
