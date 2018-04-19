// EquipmentTypeCombo.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include <afxwin.h>
#include "EquipmentTypeCombo.h"
#include "..\pms_def.h"

#define		wsizeof(x)	((x)*sizeof(WCHAR))

// CEquipmentTypeCombo

IMPLEMENT_DYNAMIC(CEquipmentTypeCombo, CCustomCombo)

static	WCHAR	*edt_rnm(
WCHAR			*buf,
int				siz,
int				avl,
WCHAR			*nam )
{
	swprintf_s( buf, siz, _T("%s%s%s"), avl ?_T(""): _T("("), nam, avl ?_T(""): _T(")") );
	return( buf );
}


CEquipmentTypeCombo::CEquipmentTypeCombo()
{
	m_Eqp_typ = NULL;
	m_Eqp_cnt = 0;
}

CEquipmentTypeCombo::~CEquipmentTypeCombo()
{
	if ( m_Eqp_typ )	free( m_Eqp_typ );
}

BEGIN_MESSAGE_MAP(CEquipmentTypeCombo, CComboBox)
END_MESSAGE_MAP()

void	CEquipmentTypeCombo::ClearType(  )
{
	ResetContent();
	free( m_Eqp_typ );
	m_Eqp_typ = NULL;
	m_Eqp_cnt = 0;
}

void	CEquipmentTypeCombo::AddType( int eti, int avl, WCHAR *nam )
{
	TYP_		*tmp;
	WCHAR		buf[290];

	if ( tmp = (TYP_ *)calloc( m_Eqp_cnt+1, sizeof(TYP_) ) ){
		if ( m_Eqp_typ ){
			memcpy( tmp, m_Eqp_typ, sizeof(TYP_) * m_Eqp_cnt );
		}
		(tmp+m_Eqp_cnt)->eti = eti;
		(tmp+m_Eqp_cnt)->avl = avl;
		wcscpy_s((tmp+m_Eqp_cnt)->nam, memcnt((tmp+m_Eqp_cnt)->nam), nam );
		if ( m_Eqp_typ )	free( m_Eqp_typ );
		m_Eqp_typ = tmp;
		InsertString( -1, edt_rnm( buf, memcnt(buf), avl, nam ) );
		m_Eqp_cnt++;
	}
}

void	CEquipmentTypeCombo::ResetAvalable()
{
	int			idx;
	WCHAR		buf[290];

	ResetContent();

	for ( idx=0; idx<m_Eqp_cnt; idx++ ){
		(m_Eqp_typ+idx)->avl = FALSE;
		InsertString( -1, edt_rnm( buf, memcnt(buf), FALSE, (m_Eqp_typ+idx)->nam ) );
	}
}


void	CEquipmentTypeCombo::SetAvalable( int eti, int avl )
{
	int			idx;
	WCHAR		buf[290];

	for ( idx=0; idx<m_Eqp_cnt; idx++ ){
		if ( (m_Eqp_typ+idx)->eti == eti ){
			(m_Eqp_typ+idx)->avl = avl;
			DeleteString( idx );
			InsertString( idx, ( buf, memcnt(buf), avl, (m_Eqp_typ+idx)->nam ) );
		}
	}
}


int		CEquipmentTypeCombo::GetType( int *avl )
{
	int			idx = GetCurSel();

	if( !!avl )	*avl = idx==-1 ? 0: (m_Eqp_typ+idx)->avl;
	return( idx==-1 ? 0: (m_Eqp_typ+idx)->eti );
}

void	CEquipmentTypeCombo::SetType( int eti )
{
	int			idx;

	if ( eti == -1 ){
		SetCurSel(-1);
	}
	else{
		for ( idx=0; idx<m_Eqp_cnt; idx++ ){
			if ( (m_Eqp_typ+idx)->eti == eti )	SetCurSel( idx );
		}
	}
}

void CEquipmentTypeCombo::PreSubclassWindow()
{
	CCustomCombo::PreSubclassWindow();
}
