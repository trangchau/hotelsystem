// HotelPMSDoc.cpp : CHotelPMSDoc クラスの実装
//

#include "stdafx.h"
#include "HotelPMS.h"

#include "HotelPMSDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHotelPMSDoc

IMPLEMENT_DYNCREATE(CHotelPMSDoc, CDocument)

BEGIN_MESSAGE_MAP(CHotelPMSDoc, CDocument)
END_MESSAGE_MAP()


// CHotelPMSDoc コンストラクション/デストラクション

CHotelPMSDoc::CHotelPMSDoc()
{
	// TODO: この位置に 1 度だけ呼ばれる構築用のコードを追加してください。

}

CHotelPMSDoc::~CHotelPMSDoc()
{
}

BOOL CHotelPMSDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)

	return TRUE;
}




// CHotelPMSDoc シリアル化

void CHotelPMSDoc::Serialize(CArchive& ar)
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


// CHotelPMSDoc 診断

#ifdef _DEBUG
void CHotelPMSDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHotelPMSDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

