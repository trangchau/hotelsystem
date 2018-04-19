// HotelPMSDoc.cpp : CCheckinDoc クラスの実装
//

#include "stdafx.h"
#include "HotelPMS.h"

#include "CheckinDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCheckinDoc

IMPLEMENT_DYNCREATE(CCheckinDoc, CDocument)

BEGIN_MESSAGE_MAP(CCheckinDoc, CDocument)
END_MESSAGE_MAP()


// CCheckinDoc コンストラクション/デストラクション

CCheckinDoc::CCheckinDoc()
{
	// TODO: この位置に 1 度だけ呼ばれる構築用のコードを追加してください。

}

CCheckinDoc::~CCheckinDoc()
{
}

BOOL CCheckinDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetTitle(_T("Checkin"));

	return TRUE;
}




// CCheckinDoc シリアル化

void CCheckinDoc::Serialize(CArchive& ar)
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


// CCheckinDoc 診断

#ifdef _DEBUG
void CCheckinDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCheckinDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCheckinDoc コマンド
