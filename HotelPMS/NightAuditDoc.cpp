// HotelPMSDoc.cpp : CNightAuditDoc クラスの実装
//

#include "stdafx.h"
#include "HotelPMS.h"

#include "NightAuditDoc.h"
#include "NightAuditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNightAuditDoc

IMPLEMENT_DYNCREATE(CNightAuditDoc, CDocument)

BEGIN_MESSAGE_MAP(CNightAuditDoc, CDocument)
END_MESSAGE_MAP()


// CNightAuditDoc コンストラクション/デストラクション

CNightAuditDoc::CNightAuditDoc()
{
	// TODO: この位置に 1 度だけ呼ばれる構築用のコードを追加してください。

}

CNightAuditDoc::~CNightAuditDoc()
{
}

BOOL CNightAuditDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	SetTitle(_T("NightAudit"));

	return TRUE;
}




// CNightAuditDoc シリアル化

void CNightAuditDoc::Serialize(CArchive& ar)
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


// CNightAuditDoc 診断

#ifdef _DEBUG
void CNightAuditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNightAuditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CNightAuditDoc コマンド
