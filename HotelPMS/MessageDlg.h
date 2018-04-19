#pragma once


// CMessageDlg ダイアログ

class CMessageDlg : public CDialog
{
	DECLARE_DYNAMIC(CMessageDlg)

public:
	CMessageDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CMessageDlg();

	void	SetMessage( WCHAR * );

// ダイアログ データ
	enum { IDD = IDD_DLG_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
};
