// VariableRngBuf.cpp: CVariableRngBuf �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include "VariableRngBuf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CVariableRngBuf::CVariableRngBuf()			// �R���X�g���N�^�[
{
	InitializeCriticalSection( m_Critical );
	m_dwReadIndex = m_dwWriteIndex = 0;
	m_pBuffer = NULL;
	m_bCreated = FALSE;
}

CVariableRngBuf::~CVariableRngBuf()			// �f�X�g���N�^�[
{
	EnterCriticalSection( m_Critical );
	if ( m_pBuffer )	free( m_pBuffer );
	m_pBuffer = NULL;
	m_bCreated = FALSE;
	LeaveCriticalSection( m_Critical );
	DeleteCriticalSection( m_Critical );
}

//---------------------------------------------------------------------
// ���[�J���t�@���N�V����

DWORD		CVariableRngBuf::RemainSize()	// �o�b�t�@�c�ʌv�Z
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

DWORD		CVariableRngBuf::UsedSize()		// �o�b�t�@�g�p�ʌv�Z
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

void	CVariableRngBuf::MoveData(			// �f�[�^�]��
void		*pExternal,						// �O���o�b�t�@
DWORD		*pBufferIndex,					// �C���f�b�N�X
DWORD		dwSize,							// �]���T�C�Y
int			bRead )							// �ǂݍ��ݎw��
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
// �p�u���b�N���\�b�h

int		CVariableRngBuf::Create(			// �o�b�t�@����
DWORD	dwBufferSize )						// �m�ۂ���o�b�t�@�[�T�C�Y
{
	if ( m_pBuffer = (char * )malloc( dwBufferSize ) ){
		m_dwBufferSize = dwBufferSize;
		m_bCreated = TRUE;
	}
	return( !!m_pBuffer );
}

void	CVariableRngBuf::Destroy( void )	// �o�b�t�@�j��
{
	EnterCriticalSection( m_Critical );
	if ( m_pBuffer )	free( m_pBuffer );
	m_pBuffer = NULL;
	m_bCreated = FALSE;
	m_dwReadIndex = m_dwWriteIndex = 0;
	LeaveCriticalSection( m_Critical );
}

int		CVariableRngBuf::ReadBuffer(		// �ǂݍ���
void		*pBuffer,						// �ǂݍ��݃o�b�t�@�|�C���^
DWORD		dwSize )						// �ǂݍ��݃T�C�Y
{
	int			chk = !!m_pBuffer;

	EnterCriticalSection( m_Critical );
	if ( chk && ( chk = UsedSize() >= dwSize ) ){
		MoveData( pBuffer, &m_dwReadIndex, dwSize, TRUE );
	}
	LeaveCriticalSection( m_Critical );
	return( chk );
}

int		CVariableRngBuf::WriteBuffer(		// ��������
void		*pBuffer,						// �������݃o�b�t�@�|�C���^
DWORD		dwSize )						// �������݃T�C�Y
{
	int			chk = !!m_pBuffer;

	EnterCriticalSection( m_Critical );
	if ( chk && ( chk = RemainSize() >= dwSize ) ){
		MoveData( pBuffer, &m_dwWriteIndex, dwSize, FALSE );
	}
	LeaveCriticalSection( m_Critical );
	return( chk );
}

DWORD	CVariableRngBuf::GetUsedSize()		// �g�p�ς݃o�C�g�T�C�Y�擾
{
	DWORD		dwSize = 0;

	if ( !!m_pBuffer ){
		EnterCriticalSection( m_Critical );
		dwSize = UsedSize();
		LeaveCriticalSection( m_Critical );
	}
	return( dwSize );
}

DWORD	CVariableRngBuf::GetFreeSize()		// �g�p�\�o�C�g�T�C�Y�擾
{
	DWORD		dwSize = 0;

	if ( !!m_pBuffer ){
		EnterCriticalSection( m_Critical );
		dwSize = RemainSize();
		LeaveCriticalSection( m_Critical );
	}
	return( dwSize );
}

