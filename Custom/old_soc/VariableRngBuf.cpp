// VariableRngBuf.cpp: CVariableRngBuf クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include "VariableRngBuf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CVariableRngBuf::CVariableRngBuf()			// コンストラクター
{
	InitializeCriticalSection( m_Critical );
	m_dwReadIndex = m_dwWriteIndex = 0;
	m_pBuffer = NULL;
	m_bCreated = FALSE;
}

CVariableRngBuf::~CVariableRngBuf()			// デストラクター
{
	EnterCriticalSection( m_Critical );
	if ( m_pBuffer )	free( m_pBuffer );
	m_pBuffer = NULL;
	m_bCreated = FALSE;
	LeaveCriticalSection( m_Critical );
	DeleteCriticalSection( m_Critical );
}

//---------------------------------------------------------------------
// ローカルファンクション

DWORD		CVariableRngBuf::RemainSize()	// バッファ残量計算
{
	DWORD		dwSize;

	if ( m_dwReadIndex > m_dwWriteIndex ){
		dwSize = m_dwReadIndex - m_dwWriteIndex;
	}
	else{
		dwSize = m_dwBufferSize - m_dwWriteIndex + m_dwReadIndex;
	}
	return( dwSize );
}

DWORD		CVariableRngBuf::UsedSize()		// バッファ使用量計算
{
	DWORD		dwSize;

	if ( m_dwReadIndex <= m_dwWriteIndex ){
		dwSize = m_dwWriteIndex - m_dwReadIndex;
	}
	else{
		dwSize = m_dwBufferSize - m_dwReadIndex + m_dwWriteIndex;
	}
	return( dwSize );
}

void	CVariableRngBuf::MoveData(			// データ転送
void		*pExternal,						// 外部バッファ
DWORD		*pBufferIndex,					// インデックス
DWORD		dwSize,							// 転送サイズ
int			bRead )							// 読み込み指示
{
	DWORD		siz=dwSize, rmn=0;

	if ( ( (*pBufferIndex) + dwSize ) > m_dwBufferSize ){
		rmn = ( (*pBufferIndex) + dwSize ) - m_dwBufferSize;
		siz = m_dwBufferSize - (*pBufferIndex);
	}
	::CopyMemory( bRead ? pExternal: m_pBuffer+(*pBufferIndex),
			bRead ? m_pBuffer+(*pBufferIndex): pExternal, siz );
	(*pBufferIndex) += siz;
	if ( (*pBufferIndex) >= m_dwBufferSize )	(*pBufferIndex) -= m_dwBufferSize;
	if ( rmn ){
		::CopyMemory( bRead ? ((char *)pExternal)+siz: m_pBuffer+(*pBufferIndex),
				bRead ? m_pBuffer+(*pBufferIndex): ((char *)pExternal)+siz, rmn );
		(*pBufferIndex) += rmn;
	}
}

//---------------------------------------------------------------------
// パブリックメソッド

int		CVariableRngBuf::Create(			// バッファ生成
DWORD	dwBufferSize )						// 確保するバッファーサイズ
{
	if ( m_pBuffer = (char * )malloc( dwBufferSize ) ){
		m_dwBufferSize = dwBufferSize;
		m_bCreated = TRUE;
	}
	return( !!m_pBuffer );
}

void	CVariableRngBuf::Destroy( void )	// バッファ破棄
{
	EnterCriticalSection( m_Critical );
	if ( m_pBuffer )	free( m_pBuffer );
	m_pBuffer = NULL;
	m_bCreated = FALSE;
	m_dwReadIndex = m_dwWriteIndex = 0;
	LeaveCriticalSection( m_Critical );
}

int		CVariableRngBuf::ReadBuffer(		// 読み込み
void		*pBuffer,						// 読み込みバッファポインタ
DWORD		dwSize )						// 読み込みサイズ
{
	int			chk = !!m_pBuffer;

	EnterCriticalSection( m_Critical );
	if ( chk && ( chk = UsedSize() >= dwSize ) ){
		MoveData( pBuffer, &m_dwReadIndex, dwSize, TRUE );
	}
	LeaveCriticalSection( m_Critical );
	return( chk );
}

int		CVariableRngBuf::WriteBuffer(		// 書き込み
void		*pBuffer,						// 書き込みバッファポインタ
DWORD		dwSize )						// 書き込みサイズ
{
	int			chk = !!m_pBuffer;

	EnterCriticalSection( m_Critical );
	if ( chk && ( chk = RemainSize() >= dwSize ) ){
		MoveData( pBuffer, &m_dwWriteIndex, dwSize, FALSE );
	}
	LeaveCriticalSection( m_Critical );
	return( chk );
}

DWORD	CVariableRngBuf::GetUsedSize()		// 使用済みバイトサイズ取得
{
	DWORD		dwSize = 0;

	if ( !!m_pBuffer ){
		EnterCriticalSection( m_Critical );
		dwSize = UsedSize();
		LeaveCriticalSection( m_Critical );
	}
	return( dwSize );
}

DWORD	CVariableRngBuf::GetFreeSize()		// 使用可能バイトサイズ取得
{
	DWORD		dwSize = 0;

	if ( !!m_pBuffer ){
		EnterCriticalSection( m_Critical );
		dwSize = RemainSize();
		LeaveCriticalSection( m_Critical );
	}
	return( dwSize );
}

