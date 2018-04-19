// VariableRngBuf.h: CVariableRngBuf �N���X�̃C���^�[�t�F�C�X
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
	int		Create( DWORD );				// �o�b�t�@����
	void	Destroy( void );				// �o�b�t�@�j��
	int		ReadBuffer( void *, DWORD );	// �w��T�C�Y�ǂݍ���
	int		WriteBuffer( void *, DWORD );	// �w��T�C�Y��������
	DWORD	GetUsedSize();					// �g�p�ς݃o�C�g�T�C�Y�擾
	DWORD	GetFreeSize();					// �g�p�\�o�C�g�T�C�Y�擾
	bool	m_bCreated;						// �����σt���O
};

#endif // !defined(AFX_VARIABLERNGBUF_H__CE015FE4_D05B_4F00_A08C_CFE2E7D8A7BA__INCLUDED_)
