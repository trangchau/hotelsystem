// HotelPMSDoc.h : CNightAuditDoc クラスのインターフェイス
//

#include "HotelPMSDoc.h"

#pragma once


class CNightAuditDoc : public CDocument
{
protected: // シリアル化からのみ作成します。
	CNightAuditDoc();
	DECLARE_DYNCREATE(CNightAuditDoc)

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
	virtual ~CNightAuditDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
};


