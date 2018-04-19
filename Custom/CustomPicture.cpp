// Picture.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include <afxwin.h>
#include "CustomPicture.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomPicture

CCustomPicture::CCustomPicture()
{
	m_Init = TRUE;
	m_UserData = 0;
}

CCustomPicture::~CCustomPicture()
{
}


BEGIN_MESSAGE_MAP(CCustomPicture, CWnd)
	//{{AFX_MSG_MAP(CPicture)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPicture ���b�Z�[�W �n���h��

BOOL CCustomPicture::Create(
const RECT	&rect,
CWnd		*pParentWnd,
UINT		nID )
{
	m_Rect = rect;
	return CWnd::Create( NULL, "",
			WS_CHILD|WS_VISIBLE, rect, pParentWnd, nID, NULL );
}

BOOL CCustomPicture::PreCreateWindow(CREATESTRUCT& cs) 
{
//	cs.style |= WS_TABSTOP | 0x200;
//	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	return CWnd::PreCreateWindow(cs);
}

void CCustomPicture::SetBitmap(				// �r�b�g�}�b�v�̐ݒ�
UINT	nIDResource )					// ���\�[�X
{
	BITMAP		Bitmap[1];
	CDC			mdcImg, mdcMask;
	RECT		rct[1];

	m_Bitmap.DeleteObject();
	m_BitMask.DeleteObject();
	m_Bitmap.LoadBitmap( nIDResource );

	m_Bitmap.GetObject( sizeof(BITMAPINFOHEADER), Bitmap );
	m_Width = Bitmap->bmWidth;
	m_Height = Bitmap->bmHeight;

	m_BitMask.CreateBitmap( m_Width, m_Height, 1,1, NULL);
/*
	mdcImg.CreateCompatibleDC(NULL);
	mdcMask.CreateCompatibleDC(NULL);
	mdcImg.SelectObject( &m_Bitmap );
	mdcMask.SelectObject( &m_BitMask );

	col = mdcImg.GetPixel( 0, 0 );					//�Ұ�ނ̍�����̓_�̐F���擾
	mdcImg.SetBkColor( col );						//�֋X�I�ɂ��̐F��w�i�F�i���ߐF�j�Ƃ���
	mdcMask.BitBlt( 0, 0, m_Width, m_Height, &mdcImg, 0, 0, SRCCOPY );  
	mdcImg.SetBkColor( RGB( 0, 0, 0 ) );			//�w�i�F������
	mdcImg.SetTextColor( RGB( 255, 255, 255 ) );	//�O�i�F�𔒂�
	mdcImg.BitBlt( 0, 0, m_Width, m_Height, &mdcMask, 0, 0, SRCAND );
	mdcImg.DeleteDC();
	mdcMask.DeleteDC();
*/
	rct->left = rct->top = 0;
	rct->right = m_Width - 1;
	rct->bottom = m_Height - 1;
//	m_Rect.right = m_Rect.left + m_Width - 1;
//	m_Rect.bottom = m_Rect.top + m_Height - 1;
	MoveWindow( m_Rect.left, m_Rect.top, m_Width, m_Height, TRUE );
	InvalidateRect( rct, TRUE );
}

void CCustomPicture::PostNcDestroy() 
{
	m_Bitmap.DeleteObject();
	m_BitMask.DeleteObject();
	CWnd::PostNcDestroy();
}

void CCustomPicture::OnPaint() 
{
	CPaintDC	dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
	CDC			sDC;
	CGdiObject	*pBmp;
		
	sDC.CreateCompatibleDC( &dc );
	dc.SetBkColor( RGB( 255, 255, 255 ) );
	dc.SetTextColor( RGB( 0, 0, 0 ) );
//	pBmp = sDC.SelectObject( &m_BitMask );
//	dc.BitBlt( 0, 0, m_Width, m_Height, &sDC, 0, 0, SRCAND );
//	sDC.SelectObject( &m_Bitmap );
//	dc.BitBlt( 0, 0, m_Width, m_Height, &sDC, 0, 0, SRCPAINT );
	pBmp = sDC.SelectObject( &m_Bitmap );
	dc.BitBlt( 0, 0, m_Width, m_Height, &sDC, 0, 0, SRCCOPY );
	sDC.SelectObject( pBmp );
	sDC.DeleteDC();
	
	// �`��p���b�Z�[�W�Ƃ��� CWnd::OnPaint() ���Ăяo���Ă͂����܂���
}
