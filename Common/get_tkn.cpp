/*
	トークン抽出処理関数
*/

#include <afx.h>
#include "get_tkn.h"
#include "windows.h"

#define		is_kanji(x)	((((unsigned char(x)))>=0x80&&((unsigned char(x)))<=0xa0)||((unsigned char(x)))>=0xe0)

static	int		chk_sep_(			/* 分割文字列検査 */
WCHAR	chr,						/* 被検査文字列   */
WCHAR	*sep )						/* 分割文字列     */
{
	while( *sep ){
		if ( *sep==chr )	break;
		sep++;
	}
	return( *sep ? TRUE: FALSE );
}

int		Get_Tkn(					/* トークンの抽出 */
WCHAR	**tkn,						/* トークン列     */
WCHAR	*txt,						/* テキスト文字列 */
WCHAR	*sep,						/* 分割文字列     */
WCHAR	*omt )						/* 読み飛ばし文字 */
{
	int		tkc = 0;
	WCHAR	*dst, *src;

	while( *txt ){
		if ( tkc == MAX_TKN )	break;
		while( *txt && chk_sep_( *txt, omt ) )	txt++;
		while( *txt && ( chk_sep_( *txt, sep ) && *txt!='"' ) )	*txt++ = '\0';
		if ( *txt ){
			if ( *txt=='"' ){
				tkn[tkc++] = ++txt;
				while( *txt && *txt!='"' ){
					if ( is_kanji(*txt) )	txt++;
					txt++;
				}
				if ( *txt=='"' )	*txt++ = '\0';
			}
			else{
				tkn[tkc++] = txt;
				while( *txt ){
					if ( is_kanji(*txt) ){
						txt++;
					}
					else{
						if ( chk_sep_( *txt, omt ) ){
							for (dst=txt, src=txt+1; !!(*src); )	*dst++ = *src++;
							*dst = '\0';
							txt--;
							//*txt='\0';
						}
						else{
							if ( !!chk_sep_( *txt, sep ) )	break;
						}
					}
					txt++;
				}
			}
			if ( *txt )	*txt++= '\0';
		}
	}
	return( tkc );
}
