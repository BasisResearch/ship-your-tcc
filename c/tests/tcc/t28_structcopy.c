/* struct copy (assignment, arrays of structs, nested) with floats/doubles */
#include <stdio.h>

struct vec { double x, y, z; };
struct particle { struct vec pos, vel; float mass; int id; char tag[6]; };

struct vec vadd(struct vec a, struct vec b) { struct vec r = {a.x + b.x, a.y + b.y, a.z + b.z}; return r; }
struct vec vscale(struct vec a, double s) { a.x *= s; a.y *= s; a.z *= s; return a; }
double vdot(const struct vec *a, const struct vec *b) { return a->x * b->x + a->y * b->y + a->z * b->z; }

int main(void)
{
    struct particle ps[4], tmp;
    int i, step;
    for (i = 0; i < 4; i++) {
        struct vec p = {i, i * 0.5, -i}, v = {0.1 * i, 1, 0.25};
        ps[i].pos = p; ps[i].vel = v; ps[i].mass = 1.0f + i; ps[i].id = i;
        sprintf(ps[i].tag, "p%d", i);
    }
    for (step = 0; step < 10; step++)
        for (i = 0; i < 4; i++) {
            ps[i].pos = vadd(ps[i].pos, vscale(ps[i].vel, 0.1));
            ps[i].vel = vscale(ps[i].vel, 1.0 - 0.01 * ps[i].mass);
        }
    tmp = ps[0]; ps[0] = ps[3]; ps[3] = tmp;   /* struct swap */
    for (i = 0; i < 4; i++)
        printf("%s id=%d m=%g pos=(%.9f,%.9f,%.9f) |v|^2=%.12f\n", ps[i].tag, ps[i].id, ps[i].mass,
               ps[i].pos.x, ps[i].pos.y, ps[i].pos.z, vdot(&ps[i].vel, &ps[i].vel));
    {
        struct vec a[3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, b[3];
        for (i = 0; i < 3; i++) b[2 - i] = a[i];
        printf("%g %g %g\n", b[0].x, b[1].y, b[2].z);
        struct vec *p = &b[1];
        *p = vadd(*p, a[0]);
        printf("%g %g %g\n", p->x, p->y, p->z);
    }
    return 0;
}
