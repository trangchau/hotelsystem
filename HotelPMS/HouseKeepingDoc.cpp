// HouseKeepingDoc.cpp : CHouseKeepingDoc クラスの実装
//

#include "stdafx.h"
#include "HotelPMS.h"

#include "HouseKeepingDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHouseKeepingDoc

IMPLEMENT_DYNCREATE(CHouseKeepingDoc, CDocument)

BEGIN_MESSAGE_MAP(CHouseKeepingDoc, CDocument)
END_MESSAGE_MAP()


// CHouseKeepingDoc コンストラクション/デストラクション

CHouseKeepingDoc::CHouseKeepingDoc()
{
	// TODO: この位置に 1 度だけ呼ばれる構築用のコードを追加してください。

}

CHouseKeepingDoc::~CHouseKeepingDoc()
{
}

BOOL CHouseKeepingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetTitle(_T("HouseKeepingOperation"));

	return TRUE;
}




// CHouseKeepingDoc シリアル化

void CHouseKeepingDoc::Serialize(CArchive& ar)
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


// CHouseKeepingDoc 診断

#ifdef _DEBUG
void CHouseKeepingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHouseKeepingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CHouseKeepingDoc コマンド
