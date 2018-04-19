// BookingStatusDoc.cpp : CBookingStatusDoc クラスの実装
//

#include "stdafx.h"
#include "HotelPMS.h"

#include "BookingStatusDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBookingStatusDoc

IMPLEMENT_DYNCREATE(CBookingStatusDoc, CDocument)

BEGIN_MESSAGE_MAP(CBookingStatusDoc, CDocument)
END_MESSAGE_MAP()


// CBookingStatusDoc コンストラクション/デストラクション

CBookingStatusDoc::CBookingStatusDoc()
{
	// TODO: この位置に 1 度だけ呼ばれる構築用のコードを追加してください。

}

CBookingStatusDoc::~CBookingStatusDoc()
{
}

BOOL CBookingStatusDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetTitle(_T("BookingStatus"));

	return TRUE;
}




// CBookingStatusDoc シリアル化

void CBookingStatusDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 格納するコードをここに追加してください。
	}
	else
	{
		// TODO: 読み込むコードをここに追加してください。
	}
}


// CBookingStatusDoc 診断

#ifdef _DEBUG
void CBookingStatusDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBookingStatusDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBookingStatusDoc コマンド
