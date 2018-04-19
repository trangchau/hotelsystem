// HouseKeepingDoc.h : CHouseKeepingDoc クラスのインターフェイス
//


#pragma once


class CHouseKeepingDoc : public CDocument
{
protected: // シリアル化からのみ作成します。
	CHouseKeepingDoc();
	DECLARE_DYNCREATE(CHouseKeepingDoc)

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
	virtual ~CHouseKeepingDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
};


