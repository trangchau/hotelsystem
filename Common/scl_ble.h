/*
		�f�[�^�Í����E�������֐���`
*/
#ifndef	Scl_BleH
#define	Scl_BleH

int		Ecd_Siz( int );										// �Í����f�[�^�T�C�Y�v�Z
void	Ecd_Dat( unsigned long *, void *, int, char * );	// �f�[�^�Í���
void	Dcd_Dat( void *, unsigned long *, int, char * );	// �f�[�^������
int		Pop_Str( WCHAR *, WCHAR *);							// pop from stack
int		Psh_Str( WCHAR *, int, WCHAR *);					// push to stack

#endif
