// HotelPMS.cpp : アプリケーションのクラス動作を定義します。
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
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 標準の印刷セットアップ コマンド
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CHotelPMSApp コンストラクション

CHotelPMSApp::CHotelPMSApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}


// 唯一の CHotelPMSApp オブジェクトです。

CHotelPMSApp theApp;


// CHotelPMSApp 初期化

BOOL CHotelPMSApp::InitInstance()
{
	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// OLE ライブラリを初期化します。
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));
	LoadStdProfileSettings(10);  // 標準の INI ファイルのオプションをロードします (MRU を含む)
	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。

	memset( pDocTemplate, 0, sizeof( pDocTemplate) );
	
	pDocTemplate[VWK_SCH] = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSearchDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CSearchView));
	AddDocTemplate(pDocTemplate[VWK_SCH]);

	pDocTemplate[VWK_RSV] = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CReservationDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CReservationView));
	AddDocTemplate(pDocTemplate[VWK_RSV]);

	pDocTemplate[VWK_BST] = new CMultiDocTemplate(
		IDR_BOOKINGSTS,
		RUNTIME_CLASS(CBookingStatusDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CBookingStatusView));
	AddDocTemplate(pDocTemplate[VWK_BST]);

	pDocTemplate[VWK_CIN] = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCheckinDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CCheckinView));
	AddDocTemplate(pDocTemplate[VWK_CIN]);

	pDocTemplate[VWK_INH] = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CInhouseDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CInhouseView));
	AddDocTemplate(pDocTemplate[VWK_INH]);

	pDocTemplate[VWK_COT] = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCheckoutDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CCheckoutView));
	AddDocTemplate(pDocTemplate[VWK_COT]);

	pDocTemplate[VWK_NAD] = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CNightAuditDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CNightAuditView));
	AddDocTemplate(pDocTemplate[VWK_NAD]);

	pDocTemplate[VWK_HKP] = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CHouseKeepingDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CHouseKeepingView));
	AddDocTemplate(pDocTemplate[VWK_HKP]);

	pDocTemplate[9] = new CMultiDocTemplate(
		IDR_HotelPMSTYPE,
		RUNTIME_CLASS(CHotelPMSDoc),
		RUNTIME_CLASS(CChildFrame), // カスタム MDI 子フレーム
		RUNTIME_CLASS(CHotelPMSView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate[9]);

	// メイン MDI フレーム ウィンドウを作成します。
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// 接尾辞が存在する場合にのみ DragAcceptFiles を呼び出します。
	//  MDI アプリケーションでは、この呼び出しは、m_pMainWnd を設定した直後に発生しなければなりません。
	// ドラッグ/ドロップ オープンを許可します。
	m_pMainWnd->DragAcceptFiles();

	// DDE Execute open を使用可能にします。
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// DDE、file open など標準のシェル コマンドのコマンド ラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 最初の新規ファイルウィンドウを表示しなくする。
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	
	// コマンド ラインで指定されたディスパッチ コマンドです。アプリケーションが
	// /RegServer、/Register、/Unregserver または /Unregister で起動された場合、False を返します。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}



// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
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

// ダイアログを実行するためのアプリケーション コマンド
void CHotelPMSApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CHotelPMSApp メッセージ ハンドラ

