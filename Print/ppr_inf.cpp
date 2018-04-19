/*
	�v�h�m�R�Q ��� ���[�`��
	����p�����擾�֐��t�@�C��
	Copyright(C) Arsnova Corp. 2003
*/

#include <afx.h>
#include <Windows.h>
#include <Winnt.h>
#include <Winspool.h>
#include <stdlib.h>
#include "..\Common\bsc_def.h"
#include "print.h"
#include "ppr_inf.h"

int		Get_Ppr_Nbr(			// �p���ԍ��擾
short		*nbr,				// �p���ԍ�
WCHAR		*pnm,				// �v�����^�[��
WCHAR		*ppr )				// �p����
{
	int					fnd = false;			// �T�|�[�g�L��
	WCHAR 				*out;					// �ԍ��A�p�����o�͐�
	WCHAR 				*wrk;					// ���[�N�|�C���^
	unsigned long		plc = 0;				// �^�[�Q�b�g�ʒu
	unsigned long		cnt;					// �ԍ��A�p������
	unsigned long 		i;

	/*	�T�|�[�g�p�����擾�A�T��	*/
	cnt = DeviceCapabilities( pnm, _T("LPT1"), DC_PAPERNAMES, NULL, NULL );
	out = (WCHAR *)calloc( cnt, wsizeof(MAX_PPR) );
	if ( out ){
		DeviceCapabilities( pnm, _T("LPT1"), DC_PAPERNAMES, out, NULL );
		wrk = out;
		for( i = 0; i < cnt; i++ ){
			if ( wcscmp( wrk, ppr ) == 0 ){
				fnd = true;
				break;
			}
			plc++;
			wrk += MAX_PPR;
		}
		free( out );
	}
	/*	�p���ԍ��擾�A�T��	*/
	if ( fnd ){
		cnt = DeviceCapabilities( pnm, _T("LPT1"), DC_PAPERS, NULL, NULL );
		if ( cnt >= plc + 1 ){
			out = (WCHAR *)calloc( cnt, wsizeof( sizeof(WORD) ) );
			if ( out ){
				DeviceCapabilities( pnm, _T("LPT1"), DC_PAPERS, out, NULL );
				wrk = out;
				wrk = (WCHAR *)((WORD *)wrk + plc);
				*nbr = *((WORD *)wrk );
				free( out );
			}
		}
		else	fnd = false;
	}
	return( fnd );
}

int		Get_Ppr_Nam(			// �p���ԍ�����p�����擾
WCHAR		*ppr,				// �p����
int			siz,				// Sizeof output buffer
short		nbr,				// �p���ԍ�
WCHAR		*pnm )				// �v�����^�[��
{
	int					fnd = false;			// �T�|�[�g�L��
	WCHAR 				*out;					// �ԍ��A�p�����o�͐�
	WORD				*wrk;					// ���[�N�|�C���^
	unsigned long		plc = 0;				// �^�[�Q�b�g�ʒu
	unsigned long		cnt;					// �ԍ��A�p������
	unsigned long 		i;

	/*	�p���ԍ��擾�A�T��	*/

	cnt = DeviceCapabilities( pnm, _T("LPT1"), DC_PAPERS, NULL, NULL );
	out = (WCHAR *)calloc( cnt, wsizeof( sizeof(WORD) ) );
	if ( out ){
		DeviceCapabilities( pnm, _T("LPT1"), DC_PAPERS, out, NULL );
		wrk = (WORD *)out;
		for( i = 0; i < cnt; i++ ){
			if ( *wrk == nbr ){
				fnd = true;
				break;
			}
			plc++;
			wrk++;
		}
		free( out );
	}
	/*	�T�|�[�g�p�����擾�A�T��	*/
	if ( fnd ){
		cnt = DeviceCapabilities( pnm, _T("LPT1"), DC_PAPERNAMES, NULL, NULL );
		if ( cnt >= plc + 1 ){
			out = (WCHAR *)calloc( cnt, wsizeof(MAX_PPR) );
			if ( out ){
				DeviceCapabilities( pnm, _T("LPT1"), DC_PAPERNAMES, out, NULL );
				wrk = (WORD *)out;
				wrk = (WORD *)((char *)wrk + ( MAX_PPR * plc ));
				wcscpy_s( ppr, siz, (WCHAR *)wrk );
			}
			free( out );
		}
	}
	return( fnd );
}

int		Get_Ppr_Siz(			// �p���T�C�Y�擾
SIZEL		*siz,				// �p�����A����
WCHAR		*pnm,				// �v�����^�[��
WCHAR		*ppr )				// �p����
{
	int	 				suc = false;
	int	 				err = ERROR_INSUFFICIENT_BUFFER;// �G���[�R�[�h
	unsigned long		byt = 0;
	HANDLE				prh;					// �v�����^�[�n���h��
	FORM_INFO_1			*dat = NULL;
	PRINTER_DEFAULTS	def;					// �v�����^�[�������

	def.pDatatype = NULL;
	def.pDevMode = NULL;
	def.DesiredAccess = PRINTER_ALL_ACCESS;
	if ( OpenPrinter( pnm, &prh, &def ) ){
		GetForm( prh, ppr, 1, (LPBYTE)dat, 0, &byt );
		if ( err == ERROR_INSUFFICIENT_BUFFER ){
			dat = (FORM_INFO_1 *)calloc( 1, byt );
			if ( dat ){
				if ( GetForm( prh, ppr, 1, (LPBYTE)dat, byt, &byt ) ){;
					siz->cx = ( dat->Size.cx );
					siz->cy = ( dat->Size.cy );
					suc = true;
				}
				free( dat );
			}
		}
		ClosePrinter( prh );
	}
	return ( suc );
}

int		Enm_Prn(				// �v�����^�[����
char		*ptr,				// �����ݐ�
int			siz )				// Size of output buffer
{
	int				suc;
	DWORD			prm, byt, cnt, i;
	LPBYTE			use, buf;

	prm = PRINTER_ENUM_CONNECTIONS|PRINTER_ENUM_LOCAL;
	byt = cnt = 0;
	EnumPrinters( prm, NULL, 4, NULL, 0, &byt, &cnt );

	if ( GetLastError() == ERROR_INSUFFICIENT_BUFFER ){
		if ( ( buf = ( LPBYTE )calloc( 1, byt ) ) != NULL ){
			suc = EnumPrinters( prm , NULL, 4, buf, byt, &byt, &cnt );
			if ( suc && ptr ){
				use = (LPBYTE)ptr;
				for ( i=0; i<cnt; i++ ){
					wcscpy_s( (WCHAR *)use, siz,
								(((PRINTER_INFO_4 *)buf) + i )->pPrinterName );
					use += MAX_PNM;
				}
			}
			free( buf );
		}
	}
	return( cnt );
}

int			Enm_Ppr(			// �p������
WCHAR		*ptr,				// �����ݐ�
WCHAR		*pnm )				// �v�����^�[��
{
	int 		cnt;

	cnt = DeviceCapabilities( pnm, _T("LPT1"), DC_PAPERNAMES, NULL, NULL );
	if ( ptr && cnt != -1 )
		DeviceCapabilities( pnm, _T("LPT1"), DC_PAPERNAMES, ptr, NULL );

	return( cnt );
}

