/* 6.10.9: the _Pragma operator; 6.10.6 #pragma. */
#define DO_PRAGMA(x) _Pragma(#x)
_Pragma("pack(1)")
struct s { char c; int i; };
DO_PRAGMA(pack())
#pragma STDC FP_CONTRACT ON
#define N 4
#pragma pack(N)
int x;
