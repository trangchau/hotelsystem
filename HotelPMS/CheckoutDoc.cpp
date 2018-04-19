// CheckoutDoc.cpp : CCheckoutDoc クラスの実装
//

#include "stdafx.h"
#include "HotelPMS.h"

#include "CheckoutDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCheckoutDoc

IMPLEMENT_DYNCREATE(CCheckoutDoc, CDocument)

BEGIN_MESSAGE_MAP(CCheckoutDoc, CDocument)
END_MESSAGE_MAP()


// CCheckoutDoc コンストラクション/デストラクション

CCheckoutDoc::CCheckoutDoc()
{
	// TODO: この位置に 1 度だけ呼ばれる構築用のコードを追加してください。

}

CCheckoutDoc::~CCheckoutDoc()
{
}

BOOL CCheckoutDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetTitle(_T("CheckoutOperation"));

	return TRUE;
}




// CCheckoutDoc シリアル化

void CCheckoutDoc::Serialize(CArchive& ar)
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


// CCheckoutDoc 診断

#ifdef _DEBUG
void CCheckoutDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCheckoutDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCheckoutDoc コマンド
