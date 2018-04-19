// HotelPMSDoc.h : CReservationDoc クラスのインターフェイス
//

#include "HotelPMSDoc.h"

#pragma once


class CReservationDoc : public CDocument
{
protected: // シリアル化からのみ作成します。
	CReservationDoc();
	DECLARE_DYNCREATE(CReservationDoc)

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
	virtual ~CReservationDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
};


