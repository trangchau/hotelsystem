#pragma once

#include "..\Common\Lnk_Mng.h"
#include "..\Custom\CustomCombo.h"

// CClientRankCombo

typedef	struct	{
	int		knd;
	int		id;
}RNK_;

class CClientRankCombo : public CCustomCombo
{
	DECLARE_DYNAMIC(CClientRankCombo)
private:

public:
	CClientRankCombo();
	virtual ~CClientRankCombo();
	void	SetRank( int );
	int		GetRank();


protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
};


