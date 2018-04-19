#pragma once

#include "CustomEdit.h"
#include "..\common\cmn_fnc.h"

class CCommaEdit : public CCustomEdit
{
	DECLARE_DYNAMIC(CCommaEdit)
private:

public:
	CCommaEdit();
	virtual ~CCommaEdit();
	int		DecimalPlacess;
	int		RoundMethod;

	void	SetValue( double dValue, int bNull );
	int		GetValue( double *dValue );

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
protected:
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
//	LRESULT OnSetText( WPARAM wParam, LPARAM lParam );

public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


