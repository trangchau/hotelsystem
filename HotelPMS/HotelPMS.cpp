// HotelPMS.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
//

#include "stdafx.h"
#include "HotelPMS.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "HotelPMSDoc.h"
#include "HotelPMSView.h"
#include "SearchDoc.h"
#include "SearchView.h"
#include "ReservationDoc.h"
#include "ReservationView.h"
#include "BookingStatusDoc.h"
#include "BookingStatusView.h"
#include "CheckinDoc.h"
#include "CheckinView.h"
#include "InhouseDoc.h"
#include "InhouseView.h"
#include "CheckoutDoc.h"
#include "CheckoutView.h"
#include "NightAuditDoc.h"
#include "NightAuditView.h"
#include "HouseKeepingDoc.h"
#include "HouseKeepingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHotelPMSApp

BEGIN_MESSAGE_MAP(CHotelPMSApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CHotelPMSApp::OnAppAbout)
	// �W���̃t�@�C����{�h�L�������g �R�}���h
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// �W���̈���Z�b�g�A�b�v �R�}���h
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CHotelPMSApp �R���X�g���N�V����

CHotelPMSApp::CHotelPMSApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}


// �B��� CHotelPMSApp �I�u�W�F�N�g�ł��B

CHotelPMSApp theApp;


// CHotelPMSApp ������

BOOL CHotelPMSApp::InitInstance()
{
	// �A�v���P�[�V���� �}�j�t�F�X�g�� visual �X�^�C����L���ɂ��邽�߂ɁA
	// ComCtl32.dll Version 6 �ȍ~�̎g�p���w�肷��ꍇ�́A
	// Windows XP �� InitCommonControlsEx() ���K�v�ł��B�����Ȃ���΁A�E�B���h�E�쐬�͂��ׂĎ��s���܂��B
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// �A�v���P�[�V�����Ŏg�p���邷�ׂẴR���� �R���g���[�� �N���X���܂߂�ɂ́A
	// �����ݒ肵�܂��B
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// OLE ���C�u���������������܂��B
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// �W��������
	// �����̋@�\���g�킸�ɍŏI�I�Ȏ��s�\�t�@�C����
	// �T�C�Y���k���������ꍇ�́A�ȉ�����s�v�ȏ�����
	// ���[�`�����폜���Ă��������B
	// �ݒ肪�i�[����Ă��郌�W�X�g�� �L�[��ύX���܂��B
	// TODO: ��Ж��܂��͑g�D���Ȃǂ̓K�؂ȕ������
	// ���̕������ύX���Ă��������B
	SetRegistryKey(_T("�A�v���P�[�V���� �E�B�U�[�h�Ő������ꂽ���[�J�� �A�v���P�[�V����"));
	LoadStdProfileSettings(10);  // �W���� INI �t�@�C���̃I�v�V���������[�h���܂� (MRU ���܂�)
	// �A�v���P�[�V�����p�̃h�L�������g �e���v���[�g��o�^���܂��B�h�L�������g �e���v���[�g
	//  �̓h�L�������g�A�t���[�� �E�B���h�E�ƃr���[���������邽�߂ɋ@�\���܂��B

	memset( pDocTemplate, 0, sizeof( pDocTemplate) );
	
	pDocTemplate[VWK_SCH] = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSearchDoc),
		RUNTIME_CLASS(CChildFrame), // �J�X�^�� MDI �q�t���[��
		RUNTIME_CLASS(CSearchView));
	AddDocTemplate(pDocTemplate[VWK_SCH]);

	pDocTemplate[VWK_RSV] = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CReservationDoc),
		RUNTIME_CLASS(CChildFrame), // �J�X�^�� MDI �q�t���[��
		RUNTIME_CLASS(CReservationView));
	AddDocTemplate(pDocTemplate[VWK_RSV]);

	pDocTemplate[VWK_BST] = new CMultiDocTemplate(
		IDR_BOOKINGSTS,
		RUNTIME_CLASS(CBookingStatusDoc),
		RUNTIME_CLASS(CChildFrame), // �J�X�^�� MDI �q�t���[��
		RUNTIME_CLASS(CBookingStatusView));
	AddDocTemplate(pDocTemplate[VWK_BST]);

	pDocTemplate[VWK_CIN] = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCheckinDoc),
		RUNTIME_CLASS(CChildFrame), // �J�X�^�� MDI �q�t���[��
		RUNTIME_CLASS(CCheckinView));
	AddDocTemplate(pDocTemplate[VWK_CIN]);

	pDocTemplate[VWK_INH] = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CInhouseDoc),
		RUNTIME_CLASS(CChildFrame), // �J�X�^�� MDI �q�t���[��
		RUNTIME_CLASS(CInhouseView));
	AddDocTemplate(pDocTemplate[VWK_INH]);

	pDocTemplate[VWK_COT] = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCheckoutDoc),
		RUNTIME_CLASS(CChildFrame), // �J�X�^�� MDI �q�t���[��
		RUNTIME_CLASS(CCheckoutView));
	AddDocTemplate(pDocTemplate[VWK_COT]);

	pDocTemplate[VWK_NAD] = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CNightAuditDoc),
		RUNTIME_CLASS(CChildFrame), // �J�X�^�� MDI �q�t���[��
		RUNTIME_CLASS(CNightAuditView));
	AddDocTemplate(pDocTemplate[VWK_NAD]);

	pDocTemplate[VWK_HKP] = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CHouseKeepingDoc),
		RUNTIME_CLASS(CChildFrame), // �J�X�^�� MDI �q�t���[��
		RUNTIME_CLASS(CHouseKeepingView));
	AddDocTemplate(pDocTemplate[VWK_HKP]);

	pDocTemplate[9] = new CMultiDocTemplate(
		IDR_HotelPMSTYPE,
		RUNTIME_CLASS(CHotelPMSDoc),
		RUNTIME_CLASS(CChildFrame), // �J�X�^�� MDI �q�t���[��
		RUNTIME_CLASS(CHotelPMSView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate[9]);

	// ���C�� MDI �t���[�� �E�B���h�E���쐬���܂��B
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// �ڔ��������݂���ꍇ�ɂ̂� DragAcceptFiles ���Ăяo���܂��B
	//  MDI �A�v���P�[�V�����ł́A���̌Ăяo���́Am_pMainWnd ��ݒ肵������ɔ������Ȃ���΂Ȃ�܂���B
	// �h���b�O/�h���b�v �I�[�v���������܂��B
	m_pMainWnd->DragAcceptFiles();

	// DDE Execute open ���g�p�\�ɂ��܂��B
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// DDE�Afile open �ȂǕW���̃V�F�� �R�}���h�̃R�}���h ���C������͂��܂��B
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// �ŏ��̐V�K�t�@�C���E�B���h�E��\�����Ȃ�����B
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	
	// �R�}���h ���C���Ŏw�肳�ꂽ�f�B�X�p�b�` �R�}���h�ł��B�A�v���P�[�V������
	// /RegServer�A/Register�A/Unregserver �܂��� /Unregister �ŋN�����ꂽ�ꍇ�AFalse ��Ԃ��܂��B
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// ���C�� �E�B���h�E�����������ꂽ�̂ŁA�\���ƍX�V���s���܂��B
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}



// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �_�C�A���O�����s���邽�߂̃A�v���P�[�V���� �R�}���h
void CHotelPMSApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CHotelPMSApp ���b�Z�[�W �n���h��

