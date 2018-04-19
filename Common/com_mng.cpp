/*
	�q�r�|�Q�R�Q�b �ʐM���[�`��
*/

#include <windows.h>
#include <stdio.h>
#include "com_mng.h"
#include "bsc_def.h"

static	int		rng_ini_(
Rng_buf		*rng )							// �����O�o�b�t�@�Ǘ��\��
{
	int			siz;

	siz = rng->siz * ( rng->max + 1 );
	rng->red = rng->wrt = 0;
	return( ( rng->buf = calloc( 1, siz ) ) != NULL );
}

static	void	rng_cst_(
Rng_buf		*rng )							// �����O�o�b�t�@�Ǘ��\��
{
	if ( rng->buf )		free( rng->buf );
	rng->buf = NULL;
}

static	int		rng_add_(
Rng_buf		*rng,							// �����O�o�b�t�@�Ǘ��\��
void		*dat )							// �ǉ�����f�[�^
{
	int		red, max, chk = FALSE;
	void	*ptr;

	max = rng->max;
	red = rng->red == 0 ? max: rng->red - 1;

	if ( rng->wrt != red ){
		ptr = (void *)( (char *)rng->buf + ( rng->wrt * rng->siz ) );
		memcpy( ptr, dat, rng->siz );
		rng->wrt = rng->wrt==max ? 0: rng->wrt+1;
		chk = TRUE;
	}
	return( chk );
}

static	int		rng_red_(
void		*dat,							// �i�[�̈�
Rng_buf		*rng )							// �����O�o�b�t�@�Ǘ��\��
{
	int		chk = FALSE;
	void	*ptr;

	if ( rng->wrt != rng->red ){
		ptr = (void *)( (char *)rng->buf + ( rng->red * rng->siz ) );
		memcpy( dat, ptr, rng->siz );
		rng->red = rng->red == rng->max ? 0: rng->red+1;
		chk = TRUE;
	}
	return( chk );
}

static	int		ini_tot_(			// �^�C���A�E�g�l������
Com_ctl			*ctl )				// �������Ώ�
{
	COMMTIMEOUTS	tot;

	memset( &tot, 0, sizeof(tot) );
	tot.ReadTotalTimeoutConstant = ctl->def.rto;
	tot.WriteTotalTimeoutConstant = ctl->def.sto;
	return( SetCommTimeouts( ctl->hnd, &tot ) );
}

static	int		set_err_(			// �G���[��Ԑݒ�
DWORD			err )				// �������Ώ�
{
	int		ret = CER_UNW;

	if ( err & CE_BREAK )		ret = CER_BRK;
	if ( err & CE_FRAME )		ret = CER_FRM;
	if ( err & CE_IOE )			ret = CER_IOE;
	if ( err & CE_MODE )		ret = CER_MOD;
	if ( err & CE_OVERRUN )		ret = CER_OVR;
	if ( err & CE_RXOVER )		ret = CER_RBO;
	if ( err & CE_RXPARITY )	ret = CER_PTY;
	if ( err & CE_TXFULL )		ret = CER_SBO;

	return( ret );
}

static	int		rcv_prc_(			// ��M�������s
Com_dat			*dat,				// �i�[�̈�
Com_ctl			*ctl )				// ��M�Ώ�
{
	int					chk, lop, siz;
	unsigned	long	len;
	char				*buf, *tmp;
	DWORD				err;
	COMSTAT				sts;

	buf = dat->dat;
	if ( !!dat->ecr )	siz = 1;
	else				siz = dat->siz;
	dat->siz = 0;

	if ( tmp = (char *)malloc( siz+1 ) ){
		do{
			ctl->rov.Offset = 0;
			ctl->rov.OffsetHigh = 0;

			chk = ReadFile( ctl->hnd, tmp, siz, &len, &ctl->rov );
			if ( !chk ){
				if ( GetLastError() == ERROR_IO_PENDING ){
					if ( GetOverlappedResult( ctl->hnd, &ctl->rov, &len, TRUE ) ){
						ResetEvent( ctl->rov.hEvent );
						ClearCommError( ctl->hnd, &err, &sts );
						if ( err ){
							chk = set_err_( err );
						}
						else{
							if ( (int)len==siz ){
								chk = CER_NON;
								dat->siz += siz;
								memcpy( buf, tmp, siz );
								buf += siz;
							}
							else{
								chk = CER_TOT;
							}
						}
					}
				}
				else{
					chk = CER_IOE;
				}
			}
			else{
				ResetEvent( ctl->rov.hEvent );
				ClearCommError( ctl->hnd, &err, &sts );
				if ( err ){
					chk = set_err_( err );
				}
				else{
					if ( (int)len==siz ){
						chk = CER_NON;
						dat->siz += siz;
						memcpy( buf, tmp, siz );
						buf += siz;
					}
					else{
						chk = CER_TOT;
					}
				}
			}
			if ( !!dat->ecr )	lop = *tmp!=dat->ecr;
			else				lop = FALSE;
		}while( lop && chk==CER_NON );
		*buf++ = '\0';
		free( tmp );
	}
	else{
		chk = CER_MEM;
	}
	return( chk );
}

static	int		rcv_trd_(			// ��M�X���b�h�G���g���[�|�C���g
Com_ctl			*ctl )				// ��M�Ώ�
{
	int			chk, idx, cnt, siz, end = FALSE;
	char		ecr;
	Com_dat		*dat=NULL;

	cnt = memcnt( ctl->rev );

	while( !end ){
		idx = WaitForMultipleObjects( cnt, ctl->rev, FALSE, INFINITE );
		idx -= WAIT_OBJECT_0;
		if ( idx == RRQ_EVT ){
			EnterCriticalSection( &ctl->rra );
			chk = rng_red_( &dat, &ctl->rrb );
			LeaveCriticalSection( &ctl->rra );
			if ( chk ){
				ctl->def.rto = dat->tim;
				ini_tot_( ctl );
				siz = dat->siz;
				ecr = dat->ecr;
				free( dat );
				dat = NULL;
				if ( ( dat = (Com_dat *)calloc( 1, Cbf_Siz(1025) ) ) != NULL ){
					dat->ecr = ecr;
					dat->siz = siz;
					if ( ( chk = rcv_prc_( dat, ctl ) ) == CER_NON ){
						if ( ctl->def.evt ){
							EnterCriticalSection( &ctl->rqa );
							rng_add_( &ctl->rbf, &dat );
							LeaveCriticalSection( &ctl->rqa );
							siz = dat->siz;
							dat = NULL;
							ctl->def.evt( ctl->def.pot, CEV_RCV, siz, ctl->usr );
						}
						else{
							free( dat );
							dat = NULL;
						}
					}
				}
				else{
					chk = CER_MEM;
				}
				if ( chk != CER_NON ){
					PurgeComm( ctl->hnd, PURGE_RXCLEAR );
					if ( ctl->def.evt ){
						ctl->def.evt( ctl->def.pot, CEV_ERR, chk, ctl->usr );
					}
					free( dat );
					dat = NULL;
				}
			}
		}
		else{
			end = idx == RED_EVT;
			if ( end ){
				ResetEvent( ctl->rev[RED_EVT] );
			}
		}
	}
	ctl->rtm = TRUE;
	if ( dat )	free( dat );
	return( 0 );
}

static	int		snd_prc_(			// ���M�������s
Com_dat			*dat,				// �i�[�̈�
Com_ctl			*ctl,				// ���M�Ώ�
int				siz )				// ���M�v���o�C�g��
{
	int					chk;
	unsigned	long	*len;
	char				*buf;
	DWORD				err;
	COMSTAT				sts;

	ctl->sov.Offset = 0;
	ctl->sov.OffsetHigh = 0;

	buf = dat->dat;
	len = (unsigned long *)&dat->siz;

	chk = WriteFile( ctl->hnd, buf, siz, len, &ctl->sov );
	if ( !chk ){
		if ( GetLastError() == ERROR_IO_PENDING ){
			if ( GetOverlappedResult(ctl->hnd, &ctl->sov, len, TRUE ) ){
				ResetEvent( ctl->sov.hEvent );
				ClearCommError( ctl->hnd, &err, &sts );
				if ( err ){
					chk = set_err_( err );
				}
				else{
					if ( siz == (int)*len ){
						chk = CER_NON;
					}
					else{
						chk = CER_TOT;
					}
				}
			}
		}
		else{
			chk = CER_IOE;
		}
	}
	else{
		ResetEvent( ctl->sov.hEvent );
		ClearCommError( ctl->hnd, &err, &sts );
		if ( err ){
			chk = set_err_( err );
		}
		else{
			if ( siz == (int)*len ){
				chk = CER_NON;
			}
			else{
				chk = CER_TOT;
			}
		}
	}
	return( chk );
}

static	int		snd_trd_(			// ���M�X���b�h�G���g���[�|�C���g
Com_ctl			*ctl )				// ���M�Ώ�
{
	int			chk, siz, idx, cnt, end = FALSE;
	Com_dat		*dat;

	cnt = memcnt( ctl->sev );

	while( !end ){
		idx = WaitForMultipleObjects( cnt, ctl->sev, FALSE, INFINITE );
		idx -= WAIT_OBJECT_0;
		if ( idx == SRQ_EVT ){
			EnterCriticalSection( &ctl->sqa );
			chk = rng_red_( &dat, &ctl->sbf );
			LeaveCriticalSection( &ctl->sqa );
			if ( chk ){
				ctl->def.sto = dat->tim;
				ini_tot_( ctl );
				siz = dat->siz;
				if ( ( chk = snd_prc_( dat, ctl, siz ) ) == CER_NON ){
					if ( ctl->def.evt ){
						ctl->def.evt( ctl->def.pot, CEV_SND, siz, ctl->usr );
					}
				}
				if ( chk != CER_NON ){
					PurgeComm( ctl->hnd, PURGE_TXCLEAR );
					if ( ctl->def.evt ){
						ctl->def.evt( ctl->def.pot, CEV_ERR, chk, ctl->usr );
					}
				}
				free( dat );
			}
		}
		else{
			end = idx == SED_EVT;
			if ( end ){
				ResetEvent( ctl->sev[SED_EVT] );
			}
		}
	}
	ctl->stm = TRUE;
	return( 0 );
}

static	int		ini_dcb_(			// �f�o�C�X�R���g���[���u���b�N������
Com_ctl			*ctl )				// �������Ώ�
{
	DCB		dcb;
	int		sts;

	if ( ( sts = GetCommState( ctl->hnd, &dcb ) ) == TRUE ){
		dcb.BaudRate = ctl->def.brd;
		dcb.fParity = ctl->def.pty != 'N';
		switch( ctl->def.pty ){
		case 'E':	dcb.Parity = EVENPARITY;	break;
		case 'O':	dcb.Parity = ODDPARITY;		break;
		case 'M':	dcb.Parity = MARKPARITY;	break;
		case 'N':	dcb.Parity = NOPARITY;		break;
		}
		switch( ctl->def.stp ){
		case 1:		dcb.StopBits = ONESTOPBIT;		break;
		case 15:	dcb.StopBits = ONE5STOPBITS;	break;
		case 2:		dcb.StopBits = TWOSTOPBITS;		break;
		}
		dcb.ByteSize = (BYTE)ctl->def.dtl;
		sts = SetCommState( ctl->hnd, &dcb );
	}
	return( sts );
}

static	int		ini_buf_(			// �ʐM���䏉����
Com_ctl			*ctl )				// �������Ώ�
{
	int		chk = CER_MEM;

	ctl->rbf.max = RQU_MAX;
	ctl->rbf.siz = sizeof(Com_dat *);

	ctl->sbf.max = SQU_MAX;
	ctl->sbf.siz = sizeof(Com_dat *);

	ctl->rrb.max = RRQ_MAX;
	ctl->rrb.siz = sizeof(Com_dat *);

	if ( rng_ini_( &ctl->rbf ) ){
		if ( rng_ini_( &ctl->sbf ) ){
			if ( rng_ini_( &ctl->rrb ) ){
				chk = TRUE;
			}
			else{
				rng_cst_( &ctl->sbf );
				rng_cst_( &ctl->rbf );
			}
		}
		else{
			rng_cst_( &ctl->sbf );
		}
	}
	return( chk );
}

static	int		ini_evt_(			// �ʐM����C�x���g������
Com_ctl			*ctl )				// �������Ώ�
{
	int			chk;

	chk  = ( ctl->rev[RRQ_EVT] = CreateSemaphore(NULL,0,RQU_MAX,NULL) ) != NULL;
	chk &= ( ctl->rev[RED_EVT] = CreateEvent(NULL, TRUE, FALSE, NULL) ) != NULL;
	chk &= ( ctl->sev[SRQ_EVT] = CreateSemaphore(NULL,0,SQU_MAX,NULL) ) != NULL;
	chk &= ( ctl->sev[SED_EVT] = CreateEvent(NULL, TRUE, FALSE, NULL) ) != NULL;
	if ( !chk ){
		if ( ctl->rev[RRQ_EVT] ){
			CloseHandle( ctl->rev[RRQ_EVT] );
			ctl->rev[RRQ_EVT] = NULL;
		}
		if ( ctl->rev[RED_EVT] ){
			CloseHandle( ctl->rev[RED_EVT] );
			ctl->rev[RED_EVT] = NULL;
		}
		if ( ctl->rev[SRQ_EVT] ){
			CloseHandle( ctl->rev[SRQ_EVT] );
			ctl->rev[SRQ_EVT] = NULL;
		}
		if ( ctl->rev[SED_EVT] ){
			CloseHandle( ctl->rev[SED_EVT] );
			ctl->rev[SED_EVT] = NULL;
		}
	}
	if ( chk ){
		chk  = (ctl->rov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) != NULL;
		chk &= (ctl->sov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) != NULL;
		if ( !chk ){
			if ( ctl->rov.hEvent ){
				CloseHandle( ctl->rov.hEvent );
				ctl->rov.hEvent = NULL;
			}
			if ( ctl->sov.hEvent ){
				CloseHandle( ctl->sov.hEvent );
				ctl->sov.hEvent = NULL;
			}
		}
	}
	return( chk );
}

static	int		trm_trd_(			// �X���b�h���I��������
Com_ctl			*ctl )				// �I�����Ώ�
{
	int		cnt=0, chk=TRUE, max;

	if ( !ctl->rtm )	SetEvent( ctl->rev[RED_EVT] );
	if ( !ctl->stm )	SetEvent( ctl->sev[SED_EVT] );

	max = ( ctl->def.rto + ctl->def.sto ) / 100 + TRE_RTY;

	while( !( ctl->rtm & ctl->stm ) ){
		Sleep( 100 );
//		if ( ++cnt == max ){
//			chk = FALSE;
//			break;
//		}
	}
	if ( !ctl->rtm )	TerminateThread( ctl->rtd, 0 );
	if ( !ctl->stm )	TerminateThread( ctl->std, 0 );

	ctl->rtm = ctl->stm = FALSE;

	CloseHandle( ctl->rtd );
	CloseHandle( ctl->std );

	return( chk );
}

static	int		ini_ctl_(			// �ʐM���䏉����
Com_ctl			*ctl )				// �������Ώ�
{
	int		chk;

	ctl->rtd = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)rcv_trd_, ctl, 0, (LPDWORD)&ctl->rid );
	if ( ctl->rtd )	ctl->rtm = FALSE;
	ctl->std = CreateThread( NULL, 0,
		(LPTHREAD_START_ROUTINE)snd_trd_, ctl, 0, (LPDWORD)&ctl->sid );
	if ( ctl->std )	ctl->stm = FALSE;
	chk = ctl->rtd && ctl->std;
	if ( !chk ){
		trm_trd_( ctl );
	}
	if ( chk ){
		InitializeCriticalSection( &ctl->rra );
		InitializeCriticalSection( &ctl->rqa );
		InitializeCriticalSection( &ctl->sqa );
	}
	return( chk );
}

HANDLE		Com_Opn(				// �ʐM����̃I�[�v��
Com_def		*def,					// �ʐM��`�f�[�^
void		*usr )					// ���[�U�[��`�f�[�^
{
	HANDLE			hnd;
	DWORD			sts = CER_OPN;
	char			dnm[10];
	Com_ctl			*ctl;

	ctl = (Com_ctl *)calloc( 1, sizeof(Com_ctl) );

	if ( ctl ){
		ctl->def = *def;
		ctl->rtm = TRUE;
		ctl->stm = TRUE;
		ctl->end = FALSE;
		ctl->usr = usr;
		sprintf_s( dnm, sizeof(dnm), "COM%d", def->pot );
		hnd = CreateFile( dnm,
						GENERIC_READ|GENERIC_WRITE,
						0,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,
						NULL);

		if ( hnd != NULL ){
			ctl->hnd = hnd;
			if ( ini_dcb_( ctl ) && ini_tot_( ctl ) && ini_buf_( ctl ) ){
				if ( ini_evt_( ctl ) && ini_ctl_( ctl ) ){
					sts = CER_NON;
					EscapeCommFunction( hnd, SETDTR );
					EscapeCommFunction( hnd, SETRTS );
				}
				PurgeComm( ctl->hnd, PURGE_RXCLEAR | PURGE_TXCLEAR );
			}
			if ( sts != CER_NON ){
				CloseHandle( hnd );
				ctl->hnd = NULL;
			}
		}
		if ( sts != CER_NON ){
			free( ctl );
			ctl = NULL;
		}
	}
	return( (HANDLE)ctl );
}

void	Com_Cls(						// �ʐM����̃N���[�Y
HANDLE		hnd )						// �ʐM����n���h��
{
	Com_ctl		*ctl;
	void		*tmp;
	int			i;

	if ( hnd ){
		ctl = (Com_ctl *)hnd;
		if ( ctl->hnd ){
			ctl->end = TRUE;
			trm_trd_( ctl );
			PurgeComm( ctl->hnd, PURGE_TXABORT | PURGE_RXABORT );
//			ResetEvent( ctl->rev[RRQ_EVT] );
//			ResetEvent( ctl->sev[SRQ_EVT] );
			if ( ctl->rov.hEvent )	CloseHandle( ctl->rov.hEvent );
			if ( ctl->sov.hEvent )	CloseHandle( ctl->sov.hEvent );
			for ( i=0; i<memcnt(ctl->rev); i++ ){
				if ( ctl->rev[i] )		CloseHandle( ctl->rev[i] );
			}
			for ( i=0; i<memcnt(ctl->sev); i++ ){
				if ( ctl->sev[i] )		CloseHandle( ctl->sev[i] );
			}
			DeleteCriticalSection( &ctl->rra );
			DeleteCriticalSection( &ctl->rqa );
			DeleteCriticalSection( &ctl->sqa );

			while( rng_red_( &tmp, &ctl->rbf ) )	free( tmp );
			while( rng_red_( &tmp, &ctl->rrb ) )	free( tmp );
			while( rng_red_( &tmp, &ctl->sbf ) )	free( tmp );

			rng_cst_( &ctl->rbf );
			rng_cst_( &ctl->rrb );
			rng_cst_( &ctl->sbf );

			CloseHandle( ctl->hnd );
			free( ctl );
		}
	}
}

int			Com_Snd(					// �f�[�^�̑��M
HANDLE		hnd,						// �ʐM����n���h��
char		*dat,						// �f�[�^�|�C���^
int			lng,						// �f�[�^�����O�X
int			tim )						// ���M�^�C���A�E�g�l(ms)
{
	Com_ctl		*ctl;
	int			chk, ret = CER_OPN;
	Com_dat		*tmp;

	if ( hnd ){
		ctl = (Com_ctl *)hnd;
		if ( ctl->hnd && !ctl->end ){
			if ( ( tmp = (Com_dat *)calloc( 1, Cbf_Siz(lng) ) ) != NULL ){
				tmp->siz = lng;
				tmp->tim = tim;
				memcpy( tmp->dat, dat, lng );
				EnterCriticalSection( &ctl->sqa );
				chk = rng_add_( &ctl->sbf, &tmp );
				LeaveCriticalSection( &ctl->sqa );
				if ( !chk ){
					ret = CER_SBO;
					free( tmp );
				}
				else{
					ReleaseSemaphore( ctl->sev[SRQ_EVT], 1, NULL );
					ret = CER_NON;
				}
			}
			else{
				ret = CER_MEM;
			}
		}
	}
	return( ret );
}

int			Com_Rcv(					// �f�[�^�̎�M�v��
HANDLE		hnd,						// �ʐM����n���h��
char		ecr,						// ��M�I�[����
int			siz,						// ��M�T�C�Y
int			tim )						// ��M�^�C���A�E�g�l(ms)
{
	Com_ctl		*ctl;
	int			chk, ret = CER_OPN;
	Com_dat		*tmp;

	if ( hnd ){
		ctl = (Com_ctl *)hnd;
		if ( ctl->hnd && !ctl->end ){
			if ( ( tmp = (Com_dat *)calloc( 1, Cbf_Siz(1) ) ) != NULL ){
				tmp->siz = siz;
				tmp->tim = tim;
				tmp->ecr = ecr;
				EnterCriticalSection( &ctl->rra );
				chk = rng_add_( &ctl->rrb, &tmp );
				LeaveCriticalSection( &ctl->rra );
				if ( !chk ){
					ret = CER_RBO;
					free( tmp );
				}
				else{
					ReleaseSemaphore( ctl->rev[RRQ_EVT], 1, NULL );
					ret = CER_NON;
				}
			}
			else{
				ret = CER_MEM;
			}
		}
	}
	return( ret );
}

int			Com_Get(					// ��M�f�[�^�̎擾
void		*dat,						// �i�[�̈�
HANDLE		hnd )						// �ʐM����n���h��
{
	Com_ctl		*ctl;
	int			chk, ret = CER_OPN;
	Com_dat		*tmp;

	if ( hnd ){
		ctl = (Com_ctl *)hnd;
		if ( ctl->hnd && !ctl->end ){
			EnterCriticalSection( &ctl->rqa );
			chk = rng_red_( &tmp, &ctl->rbf );
			LeaveCriticalSection( &ctl->rqa );
			if ( chk ){
				memcpy( dat, tmp->dat, tmp->siz+1 );
				free( tmp );
				ret = CER_NON;
			}
			else{
				ret = CER_NDT;
			}
		}
	}
	return( ret );
}

void		Com_Rts(					// �q�s�r�̑���
HANDLE		hnd,						// �ʐM����n���h��
int			set )						// SET/RESET���
{
	Com_ctl		*ctl;

	if ( hnd ){
		ctl = (Com_ctl *)hnd;
		if ( ctl->hnd && !ctl->end ){
			EscapeCommFunction( ctl->hnd, set ? SETRTS:CLRRTS );
		}
	}
}

void		Com_Dtr(					// �c�s�q�̑���
HANDLE		hnd,						// �ʐM����n���h��
int			set )						// SET/RESET���
{
	Com_ctl		*ctl;

	if ( hnd ){
		ctl = (Com_ctl *)hnd;
		if ( ctl->hnd && !ctl->end ){
			EscapeCommFunction( ctl->hnd, set ? SETDTR:CLRDTR );
		}
	}
}

char		*Edt_Cmf(			// �ʐM�t���[�������O�p�ɕҏW
char		*dst,				// �i�[�̈�
char		*src )				// ���t���[��
{
	unsigned char		*spt=(unsigned char *)src, *dpt = (unsigned char *)dst;
	char				*chr, tmp[10];
	
	while( *spt ){
		if ( is_kanji(*spt) ){
			*dpt++ = *spt++;
			*dpt++ = *spt++;
		}
		else if ( *spt < ' ' ){
			if ( *spt!=CR || *(spt+1) ){		// ������CR�͍폜
				*dpt++ = '[';
				switch( *spt ){
				case ENQ:	chr="ENQ";								break;
				case ACK:	chr="ACK";								break;
				case NAK:	chr="NAK";								break;
				case STX:	chr="STX";								break;
				case ETX:	chr="ETX";								break;
				case EOT:	chr="EOT";								break;
				case DLE:	chr="DLE";								break;
				case DC1:	chr="DC1";								break;
				case DC4:	chr="DC4";								break;
				case CR:	chr="CR";								break;
				default:	sprintf_s( tmp, sizeof(tmp), "%02X", *spt );
							chr=tmp;								break;
				}
				strcpy_s( (char *)dpt, 10, (char *)chr );
				dpt += strlen(chr);
				*dpt++ = ']';
			}
			spt++;
		}
		else{
			*dpt++ = *spt++;
		}
	}
	*dpt = '\0';
	return( dst );
}

char		*Get_Cen(			// �ʐM�G���[���b�Z�[�W�擾
int			err )				// �G���[�R�[�h
{
	char			*msg;

	switch( err ){
	case CER_OVR:	msg = "�I�[�o�[�����G���[";						break;
	case CER_PTY:	msg = "�p���e�B�G���[";							break;
	case CER_FRM:	msg = "�t���[�~���O�G���[";						break;
	case CER_BRK:	msg = "�u���[�N�L�����N�^�[���o";				break;
	case CER_IOE:	msg = "�h�^�n�G���[";							break;
	case CER_RBO:	msg = "��M�o�b�t�@�I�[�o�[�t���[";				break;
	case CER_SBO:	msg = "���M�o�b�t�@�I�[�o�[�t���[";				break;
	case CER_MOD:	msg = "���[�h�G���[";							break;
	case CER_OPN:	msg = "�I�[�v�����I�[�v��";						break;
	case CER_TOT:	msg = "�^�C���A�E�g�G���[";						break;
	case CER_MEM:	msg = "�������[�s���G���[";						break;
	case CER_NDT:	msg = "�f�[�^�����G���[";						break;
	default:		msg = "�s���ȃG���[";							break;
	}
	return( msg );
}

