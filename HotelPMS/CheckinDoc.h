// HotelPMSDoc.h : CCheckinDoc クラスのインターフェイス
//


#pragma once


class CCheckinDoc : public CDocument
{
protected: // シリアル化からのみ作成します。
	CCheckinDoc();
	DECLARE_DYNCREATE(CCheckinDoc)

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
	virtual ~CCheckinDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
};


