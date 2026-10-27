/* tcc retains GNU comma deletion in C11 mode. Metadata payloads must never
   be mistaken for punctuation while removing the comma before ##. */
#define V(x,...) x,##__VA_ARGS__
#line 44
static int empty[] = {V(1)};
#line 144
static int present[] = {V(1, 2)};
#line 244
static int token_line[] = {V(__LINE__\
)};
int main(void)
{
    return sizeof(empty)/sizeof(int) != 1 || empty[0] != 1
        || sizeof(present)/sizeof(int) != 2 || present[1] != 2
        || sizeof(token_line)/sizeof(int) != 1 || token_line[0] != 244;
}
