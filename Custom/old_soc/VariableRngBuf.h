// VariableRngBuf.h: CVariableRngBuf クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VARIABLERNGBUF_H__CE015FE4_D05B_4F00_A08C_CFE2E7D8A7BA__INCLUDED_)
#define AFX_VARIABLERNGBUF_H__CE015FE4_D05B_4F00_A08C_CFE2E7D8A7BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVariableRngBuf  
{
private:
	CRITICAL_SECTION	m_Critical[1];
	char				*m_pBuffer;
	DWORD				m_dwReadIndex;
	DWORD				m_dwWriteIndex;
	DWORD				m_dwBufferSize;
	DWORD				RemainSize();
	DWORD				UsedSize();
	void				MoveData( void *, DWORD *, DWORD, int );
public:
	CVariableRngBuf();
	virtual ~CVariableRngBuf();
	int		Create( DWORD );				// バッファ生成
	void	Destroy( void );				// バッファ破棄
	int		ReadBuffer( void *, DWORD );	// 指定サイズ読み込み
	int		WriteBuffer( void *, DWORD );	// 指定サイズ書き込み
	DWORD	GetUsedSize();					// 使用済みバイトサイズ取得
	DWORD	GetFreeSize();					// 使用可能バイトサイズ取得
	bool	m_bCreated;						// 生成済フラグ
};

#endif // !defined(AFX_VARIABLERNGBUF_H__CE015FE4_D05B_4F00_A08C_CFE2E7D8A7BA__INCLUDED_)
