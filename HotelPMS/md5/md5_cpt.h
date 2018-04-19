#ifndef	md5_cptH
#define	md5_cptH

union HASH_UINT128{
	UINT64	u64[2];
	UINT32	u32[4];
	BYTE	u8[16];
	void hex(TCHAR* s){
		_stprintf_s(s, 33, _TEXT("%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x%0.2x"), u8[0], u8[1], u8[2], u8[3], u8[4], u8[5], u8[6], u8[7], u8[8], u8[9], u8[10], u8[11], u8[12], u8[13], u8[14], u8[15]);
	}
};

int		md5Hashed(char* szPassword, DWORD dwLength, HASH_UINT128* u128);
int		md5_cpt( HASH_UINT128 *, WCHAR *, DWORD );
bool	GetMD5Hash(const void* pData,DWORD dwLen,BYTE pcbHashData[16]);
WCHAR	*Ecd_MD5( WCHAR *, WCHAR * );

#endif
