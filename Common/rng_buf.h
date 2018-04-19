/*
	リングバッファ管理
*/

#ifndef	rng_bufH
#define	rng_bufH

typedef	DWORD	RBF_HDL;		// リングバッファハンドル

RBF_HDL	Cre_Rbf( DWORD, int, int );			// リングバッファ生成(第３引数は保存の要否 TRUE/FALSE）
void	Cst_Rbf( RBF_HDL );					// リングバッファ破棄
bool	Wrt_Rbf( void *, RBF_HDL );			// リングバッファ書き込み
bool	Red_Rbf( void *, RBF_HDL );			// リングバッファ読み込み
bool	Ref_Rbf( void *, int, RBF_HDL );	// リングバッファ参照(デキュー無しの取得)
int		Cnt_Rbf( RBF_HDL );					// バッファリング数取得
HANDLE	Rbf_Smp( RBF_HDL );					// セマフォハンドル取得
int		Sav_Rbf( WCHAR * );					// リングバッファ保存
int		Rst_Rbf( WCHAR * );					// リングバッファ復元
RBF_HDL	Get_Rbf( RBF_HDL );					// 復旧リングバッファ取得
RBF_HDL	Xch_Rbf( RBF_HDL, RBF_HDL );		// リングバッファ入れ替え取得
void	Rbf_Prg( void );					// 復旧リングバッファ破棄

#endif
