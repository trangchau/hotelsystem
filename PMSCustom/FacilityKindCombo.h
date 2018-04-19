#pragma once

#include "..\Common\Lnk_Mng.h"
#include "..\Custom\CustomCombo.h"

// CFacilityKindCombo

//typedef	struct	{
//	int		knd;
//	int		id;
//}KND_;

class CFacilityKindCombo : public CCustomCombo
{
	DECLARE_DYNAMIC(CFacilityKindCombo)
private:

public:
	CFacilityKindCombo();
	virtual ~CFacilityKindCombo();
	void	SetKind( int );
	int		GetKind();


protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
};


