#define P(x) _Pragma(x)
#define F(x) __LINE__ x
#define APPLY(x) F x
#line 300
APPLY(P("unknown in_frame") (
__LINE__))
#line 400
APPLY(P("unknown across_frame")) (
__LINE__)
#line 500
APPLY(P("unknown no_call")) end __LINE__
