#include <windows.h>
#include <stdio.h>
#include <wincrypt.h>
#include <tchar.h>

#include "md5_cpt.h"

#define	wsizeof(x)		((x)*sizeof(WCHAR *))

int		md5Hashed(
char			*szPassword,
DWORD			dwLength,
HASH_UINT128*	u128 )
{
	int			chk;
	HCRYPTPROV	hProv = 0;
	HCRYPTHASH	hHash = 0;

	if ( chk = CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT ) ){
		if ( chk = CryptCreateHash( hProv, CALG_MD5, 0, 0, &hHash ) ){
			if ( chk = CryptHashData( hHash, (BYTE*)szPassword, dwLength, 0) ) {
				if ( chk = CryptHashData( hHash, (BYTE*)szPassword, dwLength, 0 ) ){
					DWORD dwRead = 16;
					if ( !( chk = CryptGetHashParam (hHash, HP_HASHVAL, (BYTE*)u128, &dwRead, 0 ) ) ){
						CryptDestroyHash(hHash);
						CryptReleaseContext(hProv, 0);
						return false;
					}
				}
			}
			CryptDestroyHash(hHash);
		}
		CryptReleaseContext( hProv, 0 );
	}

	return( chk );
}

int		md5_cpt(
HASH_UINT128	*dst,
WCHAR			*src,
DWORD			siz )
{
	return( md5Hashed( (char *)src, wsizeof(siz), dst ) );
}

/*
void _tmain(void){
	HASH_UINT128 hash;
	TCHAR* pass = _TEXT("abc");
	TCHAR hexs[34];
#ifdef UNICODE
	char sjis[256];
	WideCharToMultiByte(932, 0, pass, -1, sjis, sizeof(sjis), NULL, NULL);
	md5Hashed(sjis, (int)strlen(sjis), &hash);
#else
	md5Hashed(pass, (int)_tcslen(pass), &hash);
#endif
	hash.hex(hexs);
	_tprintf(_TEXT("%s\nmd5 %s\n"), pass,hexs);
}

int		md5_cpt(
HASH_UNIT128	*dst,
WCHAR			*src,
DWORD			siz )
{
	return( md5Hashed( (char *)src, wsizeof(siz), dst ) );
}
*/

#include "atlstr.h"

#include "wincrypt.h"

//
//	MD5ハッシュ計算（128ビット）
//
//対応：Windows 95 OSR2以降
//
bool	GetMD5Hash(const void* pData,DWORD dwLen,BYTE pcbHashData[16])
{
	bool		ret;
	HCRYPTPROV	hCryptProv;
	HCRYPTHASH	hHash;
	BYTE		pbHash[0x7f];
	DWORD		dwHashLen;

	::ZeroMemory(pcbHashData,16);
	if(pData == NULL || dwLen == 0)
		return	false;

	dwHashLen = 16;
	hHash = NULL;
	hCryptProv = NULL;
	ret = false;
	if(::CryptAcquireContext(&hCryptProv,NULL,NULL,PROV_RSA_FULL,CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET))
	{
		if(::CryptCreateHash(hCryptProv,CALG_MD5,0,0,&hHash))
		{
			if(::CryptHashData(hHash,(BYTE*)pData,dwLen,0))
			{
				if(::CryptGetHashParam(hHash,HP_HASHVAL,pbHash,&dwHashLen,0)) 
				{
					::memcpy_s(pcbHashData,dwHashLen,pbHash,dwHashLen);
					ret = true;
				}
			}
		}
	}

	if(hHash)
		::CryptDestroyHash(hHash);
	if(hCryptProv)
		::CryptReleaseContext(hCryptProv,0);

	if(ret == false)
		::ZeroMemory(pcbHashData,dwHashLen);

	return	ret;
}

void	Test(void)
{
	bool		ret;
	int			i;
	BYTE		pcbData[16];
	CAtlString	strTmp;
	CAtlString	strMessage;


	//"abc"という文字のMD5ハッシュを計算
	ret = GetMD5Hash("abc",3,pcbData);

	if(ret == false)
	{
		::MessageBox(NULL,_T("取得に失敗しました"),_T(""),MB_OK);
		return;
	}

	for(i = 0; i < 16; i++)
	{
		strTmp.Format(_T("%x"),pcbData[i]);
		strMessage += strTmp;
	}

	::MessageBox(NULL,strMessage,_T(""),MB_OK);
}

WCHAR		*Ecd_MD5(
WCHAR			*dst,
WCHAR			*src )
{
	int			siz;
	char		*chr;
	BYTE		hss[16];

	siz = WideCharToMultiByte( 932, 0, src, -1, NULL, 0, NULL, NULL );
	if ( chr = (char *)malloc( siz+1 ) ){
		WideCharToMultiByte( 932, 0, src, -1, chr, siz, NULL, NULL );
		GetMD5Hash( chr, (int)strlen(chr), hss );
		_stprintf_s( dst, 33, 
			_TEXT("%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x"),
				hss[0], hss[1], hss[2], hss[3], hss[4], hss[5], hss[6], hss[7],
				hss[8], hss[9], hss[10], hss[11], hss[12], hss[13], hss[14], hss[15]);
		free( chr );
	}

	return( dst );
}
