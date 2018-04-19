#pragma once

#include "..\Common\Lnk_Mng.h"
#include "..\Custom\CustomCombo.h"
#include "..\Common\lnk_mng.h"

// CBookingPathCombo

typedef	struct	{
	int		pth;
	int		agi;
	WCHAR	nam[256];
}AGT_;

class CBookingPathCombo : public CCustomCombo
{
	DECLARE_DYNAMIC(CBookingPathCombo)
private:
	AGT_	*m_Agt_dat;
public:
	CBookingPathCombo();
	virtual ~CBookingPathCombo();
	void	SetAgentData( Csr );
	void	SetItemIndex( int, int );
	void	GetItemIndex( int *, int * );	// getting a booking path index
	int		GetFixedRoot( Csr, int );		// getting fixsd booking path
	WCHAR	*GetPathName( int, int );		// getting booking path name

protected:
	DECLARE_MESSAGE_MAP()
};


