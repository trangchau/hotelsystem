// ChildFrm.h : CChildFrame クラスのインターフェイス
//

#pragma once


class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// 属性
public:
	int				m_Viw_knd;						// ビュー種別
	int				m_Ini_upd;						// 初期表示完了フラグ
	POINT			m_Win_siz[1];					// 規程のウィンドウサイズ

	int				m_Rsv_id;						// reservation_info_id of be editting
	int				m_Nsz_viw;						// Non sizable view

// 操作
public:

// オーバーライド
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 実装
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
};
