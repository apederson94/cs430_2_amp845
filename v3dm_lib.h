#include <math.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Vector3 {
  double x, y, z;
} Vector3;


static inline void v3dm_assign(double x, double y, double z, Vector3 *a) {
  a->x = x;
  a->y = y;
  a->z = z;
}

static inline double v3dm_magnitude(Vector3 *a) {
  return sqrt(a->x*a->x + a->y*a->y + a->z*a->z);
}

static inline void v3dm_scale(Vector3 *a, double b, Vector3 *c) {
  c->x = a->x * b;
  c->y = a->y * b;
  c->z = a->z * b;
}

static inline void v3dm_unit(Vector3 *a, Vector3 *b) {
//TODO: check 0
  v3dm_scale(a, 1/v3dm_magnitude(a), b);
}

static inline void v3dm_add(Vector3 *a, Vector3 *b, Vector3 *c) {
  c->x = a->x + b->x;
  c->y = a->y + b->y;
  c->z = a->z + b->z;
}

static inline void v3dm_subtract(Vector3 *a, Vector3 *b, Vector3 *c) {
  c->x = a->x - b->x;
  c->y = a->y - b->y;
  c->z = a->z - b->z;
}

static inline double v3dm_dot(Vector3 *a, Vector3 *b) {
  return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

static inline void v3dm_project(Vector3 *a, Vector3 *b, Vector3 *c) {
  Vector3 *unit_a, *unit_b;
  double mag_a, scalar;
  unit_a = malloc(sizeof(Vector3));
  unit_b = malloc(sizeof(Vector3));
  mag_a = v3dm_magnitude(a);
  v3dm_unit(a, unit_a);
  v3dm_unit(b, unit_b);
  scalar = mag_a * v3dm_dot(unit_a, unit_b);
  v3dm_scale(unit_b, scalar, c);
  free(unit_a);
  free(unit_b);
}

static inline double v3dm_pointToPointDistance(Vector3 *a, Vector3 *b) {
  return sqrt((b->x - a->x)*(b->x - a->x) + (b->y - a->y)*(b->y - a->y) + (b->z - a->z)*(b->z - a->z));
}

static inline double v3dm_pointToPlaneDistance(struct Vector3 normal, struct Vector3 point_on, Vector3 *point_off) {
  double dot, result;
  Vector3 *tmp = malloc(sizeof(Vector3));
  v3dm_subtract(point_off, &point_on, tmp);
  dot = v3dm_dot(&normal, tmp);
  result = dot/v3dm_magnitude(&normal);
  return result;
}

static inline void v3dm_cross(Vector3 *a, Vector3 *b, Vector3 *c) {
  c->x = (a->y * b->z) - (a->z * b->y);
  c->y = (a->z * b->x) - (a->x * b->z);
  c->z = (a->x * b->y) - (a->y * b->x);
}

static inline void v3dm_reflect(Vector3 *a, Vector3 *n, Vector3 *b) {
  Vector3 *c, *n_unit;
  v3dm_unit(n, n_unit);
  v3dm_scale(n_unit, 2 * v3dm_dot(a, n), c);
  v3dm_subtract(c, a, b);

}
