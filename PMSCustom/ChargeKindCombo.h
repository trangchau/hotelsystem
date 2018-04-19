#pragma once

#include "..\Common\Lnk_Mng.h"
#include "..\Custom\CustomCombo.h"

// CChargeKindCombo

typedef	struct	{
	int		cgk;			// charge kind code
	WCHAR	*nam;			// charge kind name
}CGK_;

class CChargeKindCombo : public CCustomCombo
{
	DECLARE_DYNAMIC(CChargeKindCombo)
private:

public:
	CChargeKindCombo();
	virtual ~CChargeKindCombo();
	void	SetData( void );


	void	SetItemIndex( int );
	int		GetItemIndex( void );	// getting a charge kind
	WCHAR	*GetItemName( int cgk );

protected:
	DECLARE_MESSAGE_MAP()
};


