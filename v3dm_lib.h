#include <math.h>

typedef double *V3;

static inline double v3dm_magnitude(V3 a) {
  return sqrt(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
}

static inline void v3dm_unit(V3 a, V3 b) {
  b = scale(a, 1/v3dm_magnitude(a), b);
}

static inline void v3dm_add(V3 a, V3 b, V3 c) {
  c[0] = a[0] + b[0];
  c[1] = a[1] + b[1];
  c[2] = a[2] + b[2];
}

static inline void v3dm_subtract(V3 a, V3 b, V3 c) {
  c[0] = a[0] - b[0];
  c[1] = a[1] - b[1];
  c[2] = a[2] - b[2];
}

static inline void v3dm_scale(V3 a, double b, V3 c) {
  c[0] = a[0] * b;
  c[1] = a[1] * b;
  c[2] = a[2] * b;
}

static inline double v3dm_dot(V3 a, V3 b) {
  return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
}

static inline void v3dm_project(V3 a, V3 b, V3 c) {
  v3dm_unit(a, c);
  v3dm_scale(c, v3dm_dot(b, c), c);
}

static inline void v3dm_distanceFromPoint(V3 a, V3 b) {
  V3 c;
  v3dm_project(a, b, c);
  return v3dm_magnitude(v3dm_subtract(b, c, c));
}

static inline void v3dm_cross(V3 a, V3 b, V3 c) {
  c[0] = (a[1] * b[2]) - (a[2] * b[1]);
  c[1] = (a[2] * b[0]) - (a[0] * b[2]);
  c[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

static inline void v3dm_reflect(V3 a, V3 n, V3 b) {
  V3 c;
  v3dm_unit(n, n);
  v3dm_scale(n, v3dm_dot(n, a), c);
  v3dm_scale(c, 2, c);
  v3dm_subtract(a, c, b);
}

static inline void v3dm_assign(double x, double y, double z, V3 a) {
  a[0] = x;
  a[1] = y;
  a[2] = z;
}
