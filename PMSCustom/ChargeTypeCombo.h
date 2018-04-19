#pragma once

#include "..\Common\Lnk_Mng.h"
#include "..\Custom\CustomCombo.h"

// CChargeTypeCombo

typedef	struct	{
	int		ptc;			// Perticular code
	WCHAR	*nam;			// Perticular name
}CGT_;

class CChargeTypeCombo : public CCustomCombo
{
	DECLARE_DYNAMIC(CChargeTypeCombo)
private:
	int		m_Eqp_knd;				// Equipment kind

public:
	CChargeTypeCombo();
	virtual ~CChargeTypeCombo();
	void	SetEquipmentKind( int );


	void	SetItemIndex( int );
	int		GetItemIndex( void );	// getting a booking path index
	WCHAR	*GetItemName( int ptc );


protected:
	DECLARE_MESSAGE_MAP()
};


