// HotelPMSDoc.cpp : CSearchDoc クラスの実装
//

#include "stdafx.h"
#include "HotelPMS.h"

#include "SearchDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSearchDoc

IMPLEMENT_DYNCREATE(CSearchDoc, CDocument)

BEGIN_MESSAGE_MAP(CSearchDoc, CDocument)
END_MESSAGE_MAP()


// CSearchDoc コンストラクション/デストラクション

CSearchDoc::CSearchDoc()
{
	// TODO: この位置に 1 度だけ呼ばれる構築用のコードを追加してください。

}

CSearchDoc::~CSearchDoc()
{
}

BOOL CSearchDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetTitle(_T("ReservationSearch"));

	return TRUE;
}




// CSearchDoc シリアル化

void CSearchDoc::Serialize(CArchive& ar)
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


// CSearchDoc 診断

#ifdef _DEBUG
void CSearchDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSearchDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSearchDoc コマンド
