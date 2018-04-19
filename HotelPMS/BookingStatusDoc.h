// BookingStatusDoc.h : CBookingStatusDoc クラスのインターフェイス
//


#pragma once


class CBookingStatusDoc : public CDocument
{
protected: // シリアル化からのみ作成します。
	CBookingStatusDoc();
	DECLARE_DYNCREATE(CBookingStatusDoc)

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
	virtual ~CBookingStatusDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
};


