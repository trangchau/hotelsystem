// HotelPMSDoc.cpp : CReservationDoc クラスの実装
//

#include "stdafx.h"
#include "HotelPMS.h"

#include "ReservationDoc.h"
#include "ReservationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CReservationDoc

IMPLEMENT_DYNCREATE(CReservationDoc, CDocument)

BEGIN_MESSAGE_MAP(CReservationDoc, CDocument)
END_MESSAGE_MAP()


// CReservationDoc コンストラクション/デストラクション

CReservationDoc::CReservationDoc()
{
	// TODO: この位置に 1 度だけ呼ばれる構築用のコードを追加してください。

}

CReservationDoc::~CReservationDoc()
{
}

BOOL CReservationDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetTitle(_T("ReservationInformation"));

	return TRUE;
}




// CReservationDoc シリアル化

void CReservationDoc::Serialize(CArchive& ar)
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


// CReservationDoc 診断

#ifdef _DEBUG
void CReservationDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CReservationDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CReservationDoc コマンド
