#pragma once

#include "CustomEdit.h"

class CDateEdit : public CCustomEdit
{
	DECLARE_DYNAMIC(CDateEdit)
private:

	void		EditInputtedData();
public:
	CDateEdit();
	virtual ~CDateEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
public:
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
protected:
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};


