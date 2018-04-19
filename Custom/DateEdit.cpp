// DateEdit.cpp : �����t�@�C��
//

#include <afxwin.h>
#include "DateEdit.h"

#include "..\Common\bsc_def.h"
#include "..\Common\dte_mng.h"

#define		wsizeof(x)	((x)*sizeof(WCHAR))


// CDateEdit

IMPLEMENT_DYNAMIC(CDateEdit, CCustomEdit)

CDateEdit::CDateEdit()
{
}

CDateEdit::~CDateEdit()
{
}

BEGIN_MESSAGE_MAP(CDateEdit, CCustomEdit)
	ON_WM_KILLFOCUS()
	ON_WM_RBUTTONUP()
	ON_WM_SETFOCUS()
	ON_WM_CHAR()
	ON_WM_KEYUP()
END_MESSAGE_MAP()



// CDateEdit ���b�Z�[�W �n���h��

#define	is_delimitter(x)		((x)==_T('/') || (x)==_T('.') || (x)==_T('-') || (x)==VK_DELETE || (x)==VK_BACK )

static	int	is_numeric( WCHAR *str )
{
	while ( *str ){
		if ( *str<_T('0') || *str>_T('9') )	break;
		str++;
	}
	return( *str==0 );
}

void CDateEdit::EditInputtedData()
{
	WCHAR		buf[40], yer[10], mth[10], day[10], *tkn[3], *ptr;
	Day			dte[1], crn[1];
	int			tkc;

	Get_Dte( crn );

	GetWindowText( buf, memcnt(buf) );
	if ( wcslen(buf) > 0 ){
		if ( is_numeric( buf ) ){
			switch( wcslen( buf ) ){
			case	2:	day[0]=buf[0]; day[1]=buf[1]; day[2]=0;
						swprintf_s( buf, memcnt(buf), _T("%4d/%02d/%02d"), crn->yer, crn->mth, _wtoi(day) );
						break;
			case	4:	mth[0]=buf[0]; mth[1]=buf[1]; mth[2]=0;
						day[0]=buf[2]; day[1]=buf[3]; day[2]=0;
						swprintf_s( buf, _T("%4d/%02d/%02d"), crn->yer, _wtoi(mth), _wtoi(day) );
						break;
			case	8:	yer[0]=buf[0]; yer[1]=buf[1]; yer[2]=buf[2]; yer[3]=buf[3]; yer[4]=0; 
						mth[0]=buf[4]; mth[1]=buf[5]; mth[2]=0;
						day[0]=buf[6]; day[1]=buf[7]; day[2]=0;
						swprintf_s( buf, memcnt(buf), _T("%4d/%02d/%02d"), _wtoi(yer), _wtoi(mth), _wtoi(day) );
						break;
			default:	break;
			}
		}
		else{
			for ( tkc=1, ptr=tkn[0]=buf; *ptr; ptr++ ){
				if ( is_delimitter(*ptr) ){
					if ( !!*(ptr+1) ){
						if ( ++tkc<=3 ){
							*ptr = 0;
							tkn[tkc-1] = ptr+1;
						}
						else{
							break;
						}
					}
					else{
						break;
					}
				}
			}
			if ( !*(ptr) && tkc>1){
				if ( tkc==2 ){
					swprintf_s( buf, memcnt(buf), _T("%4d/%02d/%02d"), crn->yer, _wtoi(tkn[0]), _wtoi(tkn[1]) );
				}
				else{
					swprintf_s( buf, memcnt(buf), _T("%4d/%02d/%02d"), _wtoi(tkn[0]), _wtoi(tkn[1]), _wtoi(tkn[2]) );
				}
			}
		}
		if ( Str_Dte( dte, buf ) ){
			swprintf_s( buf, memcnt(buf), _T("%4d/%02d/%02d"), dte->yer, dte->mth, dte->day );
			SetWindowText( buf );
		}
	}
}


void CDateEdit::OnKillFocus(CWnd* pNewWnd)
{
	EditInputtedData();
	CCustomEdit::OnKillFocus(pNewWnd);

}

void CDateEdit::OnRButtonUp(UINT nFlags, CPoint point)
{
	//


	CCustomEdit::OnRButtonUp(nFlags, point);
}

void CDateEdit::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
{
	SetLimitText( 10 );

	CCustomEdit::CalcWindowRect(lpClientRect, nAdjustType);
}

void CDateEdit::OnSetFocus(CWnd* pOldWnd)
{
	SetLimitText( 10 );
	CCustomEdit::OnSetFocus(pOldWnd);

	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
}

void CDateEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ( ( nChar>=_T('0') && nChar<=_T('9') ) || is_delimitter(nChar) ){
		CCustomEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else{
		nChar = 0;
	}
}

void CDateEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int		cid = ::GetWindowLong( m_hWnd, GWL_ID );

	if ( nChar==VK_RETURN ){
		EditInputtedData();
		::PostMessage( GetParent()->m_hWnd,
			WM_EDITNOTIFY, MAKEWPARAM(WM_KEYUP,cid), CCustomEdit::m_UserData );
	}

	CCustomEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}
