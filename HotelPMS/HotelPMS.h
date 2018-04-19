// HotelPMS.h : HotelPMS アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CHotelPMSApp:
// このクラスの実装については、HotelPMS.cpp を参照してください。
//

class CHotelPMSApp : public CWinApp
{
public:
	CHotelPMSApp();

	CMultiDocTemplate	*pDocTemplate[10];



// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CHotelPMSApp theApp;