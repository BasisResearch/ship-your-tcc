/* C11 6.7.9p19-21: descendants accumulate within one selected member,
   switching members discards that entire subtree, a braced replacement
   resets implicit descendants. No read of an inactive member or padding. */
union U { long first; struct { int x, y; int a[4]; } s; int *p; };
struct S { int before; union U u[2]; int after; };
static int target = 19;
static struct S data = {
  .before = 7, .u[0].s.x = 8, .u[1].s.y = 9,
  .u[0].first = 42, .u[0].s.a[2] = 3, .u[0].s.a[0] = 5,
  .u[1].s.x = 1, .u[1].s.x = 2, .after = 6
};
static union U ptr = { .s.y = 7, .p = &target };
static union U discarded_ptr = { .p = &target, .s.y = 4 };
union N { int first; struct { union U u; int z; } s; };
static union N nested = { .s.z = 11, .s.u.s.y = 2, .first = 6,
                         .s.u.s.a[3] = 4 };
static union U implicit;
int main(void) {
  union U local = { .s.x=1, .s.y=2, .s={.a[1]=7}, .s.y=3 };
  union U same = { .s.a={1,2,3,4}, .s.a[2]=8, .s.a[2]=9 };
  union U back = { .s.y=7, .p=&target, .s.x=4 };
  union U zero = { .s.y=8, .s={0} };
  union U null = { .p = 0 };
  union U values[3] = { [2].s.y=3, [0].s.a[1]=4, [2].s.x=5 };
  int bad = data.before!=7 || data.after!=6;
  bad |= data.u[0].s.x || data.u[0].s.y || data.u[0].s.a[0]!=5 ||
         data.u[0].s.a[1] || data.u[0].s.a[2]!=3 || data.u[0].s.a[3];
  bad |= data.u[1].s.x!=2 || data.u[1].s.y!=9 || data.u[1].s.a[0];
  bad |= ptr.p!=&target || *ptr.p!=19 || discarded_ptr.s.x || discarded_ptr.s.y!=4;
  bad |= nested.s.z || nested.s.u.s.y || nested.s.u.s.a[3]!=4;
  bad |= local.s.x || local.s.y!=3 || local.s.a[1]!=7 || local.s.a[0];
  bad |= same.s.a[0]!=1 || same.s.a[1]!=2 || same.s.a[2]!=9 || same.s.a[3]!=4;
  bad |= back.s.x!=4 || back.s.y || zero.s.x || zero.s.y || zero.s.a[0];
  bad |= null.p!=0 || implicit.first;
  bad |= values[0].s.a[1]!=4 || values[0].s.x || values[1].first ||
         values[2].s.x!=5 || values[2].s.y!=3;
  return bad;
}
