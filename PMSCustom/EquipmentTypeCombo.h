#pragma once

#include "..\Common\Lnk_Mng.h"
#include "..\Custom\CustomCombo.h"

// CEquipmentTypeCombo

typedef	struct	{
	int		eti;
	int		avl;
	WCHAR	nam[256];
}TYP_;

class CEquipmentTypeCombo : public CCustomCombo
{
	DECLARE_DYNAMIC(CEquipmentTypeCombo)
private:
	TYP_	*m_Eqp_typ;
	int		m_Eqp_cnt;
public:
	CEquipmentTypeCombo();
	virtual ~CEquipmentTypeCombo();
	void	ClearType();
	void	ResetAvalable();
	void	SetAvalable( int, int );
	void	AddType( int eti, int avl, WCHAR *nam );
	int		GetType( int * );
	void	SetType( int );


protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
};


