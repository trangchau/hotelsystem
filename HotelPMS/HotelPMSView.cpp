// HotelPMSView.cpp : CHotelPMSView �N���X�̎���
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
	// �W������R�}���h
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CHotelPMSView �R���X�g���N�V����/�f�X�g���N�V����

CHotelPMSView::CHotelPMSView(): CFormView(CHotelPMSView::IDD)
{
	// TODO: �\�z�R�[�h�������ɒǉ����܂��B

}

CHotelPMSView::~CHotelPMSView()
{
}

BOOL CHotelPMSView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

// CHotelPMSView �`��

void CHotelPMSView::OnDraw(CDC* /*pDC*/)
{
	CHotelPMSDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B
}


// CHotelPMSView ���

BOOL CHotelPMSView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// ����̈������
	return DoPreparePrinting(pInfo);
}

void CHotelPMSView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ����O�̓��ʂȏ�����������ǉ����Ă��������B
}

void CHotelPMSView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �����̌㏈����ǉ����Ă��������B
}


// CHotelPMSView �f�f

#ifdef _DEBUG
void CHotelPMSView::AssertValid() const
{
	CView::AssertValid();
}

void CHotelPMSView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHotelPMSDoc* CHotelPMSView::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHotelPMSDoc)));
	return (CHotelPMSDoc*)m_pDocument;
}
#endif //_DEBUG


// CHotelPMSView ���b�Z�[�W �n���h��



//	�I�y���[�V����
int		CHotelPMSView::GetData( int rid )
{
	return( TRUE );
}

int		CHotelPMSView::CreateData( void )
{
	return( TRUE );
}
