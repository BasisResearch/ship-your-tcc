union S { unsigned :3; unsigned a:5; int b; };
int main(void) { static union S s={17}; return s.a!=17 || sizeof s!=4; }
