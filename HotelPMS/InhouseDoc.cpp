// InhouseDoc.cpp : CInhouseDoc クラスの実装
//

#include "stdafx.h"
#include "HotelPMS.h"

#include "InhouseDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInhouseDoc

IMPLEMENT_DYNCREATE(CInhouseDoc, CDocument)

BEGIN_MESSAGE_MAP(CInhouseDoc, CDocument)
END_MESSAGE_MAP()


// CInhouseDoc コンストラクション/デストラクション

CInhouseDoc::CInhouseDoc()
{
	// TODO: この位置に 1 度だけ呼ばれる構築用のコードを追加してください。

}

CInhouseDoc::~CInhouseDoc()
{
}

BOOL CInhouseDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetTitle(_T("InhouseOperation"));

	return TRUE;
}




// CInhouseDoc シリアル化

void CInhouseDoc::Serialize(CArchive& ar)
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


// CInhouseDoc 診断

#ifdef _DEBUG
void CInhouseDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CInhouseDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CInhouseDoc コマンド
