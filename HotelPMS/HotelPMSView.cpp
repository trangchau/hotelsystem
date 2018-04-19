// HotelPMSView.cpp : CHotelPMSView クラスの実装
//

#include "stdafx.h"
#include "HotelPMS.h"

#include "HotelPMSDoc.h"
#include "HotelPMSView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHotelPMSView

IMPLEMENT_DYNCREATE(CHotelPMSView, CFormView)

BEGIN_MESSAGE_MAP(CHotelPMSView, CFormView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CHotelPMSView コンストラクション/デストラクション

CHotelPMSView::CHotelPMSView(): CFormView(CHotelPMSView::IDD)
{
	// TODO: 構築コードをここに追加します。

}

CHotelPMSView::~CHotelPMSView()
{
}

BOOL CHotelPMSView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CHotelPMSView 描画

void CHotelPMSView::OnDraw(CDC* /*pDC*/)
{
	CHotelPMSDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}


// CHotelPMSView 印刷

BOOL CHotelPMSView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CHotelPMSView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CHotelPMSView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}


// CHotelPMSView 診断

#ifdef _DEBUG
void CHotelPMSView::AssertValid() const
{
	CView::AssertValid();
}

void CHotelPMSView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHotelPMSDoc* CHotelPMSView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHotelPMSDoc)));
	return (CHotelPMSDoc*)m_pDocument;
}
#endif //_DEBUG


// CHotelPMSView メッセージ ハンドラ



//	オペレーション
int		CHotelPMSView::GetData( int rid )
{
	return( TRUE );
}

int		CHotelPMSView::CreateData( void )
{
	return( TRUE );
}
