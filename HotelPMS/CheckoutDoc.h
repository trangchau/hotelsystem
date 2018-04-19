// CheckoutDoc.h : CCheckoutDoc クラスのインターフェイス
//


#pragma once


class CCheckoutDoc : public CDocument
{
protected: // シリアル化からのみ作成します。
	CCheckoutDoc();
	DECLARE_DYNCREATE(CCheckoutDoc)

// 属性
public:

// 操作
public:

// オーバーライド
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 実装
public:
	virtual ~CCheckoutDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
};


