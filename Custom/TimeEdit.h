#pragma once

#include "CustomEdit.h"

class CTimeEdit : public CCustomEdit
{
	DECLARE_DYNAMIC(CTimeEdit)
private:

public:
	CTimeEdit();
	virtual ~CTimeEdit();

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
};


