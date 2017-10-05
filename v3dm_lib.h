#include <math.h>

typedef struct Vector3 {
  double x, y, z;
} Vecotr3;

void scale();

static inline double v3dm_magnitude(Vector3 a) {
  return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

static inline void v3dm_unit(Vector3 a, Vector3 b) {
  scale(a, 1/v3dm_magnitude(a), b);
}

static inline void v3dm_add(Vector3 a, Vector3 b, Vector3 c) {
  c.x = a.x + b.x;
  c.y = a.y + b.y;
  c.z = a.z + b.z;
}

static inline void v3dm_subtract(Vector3 a, Vector3 b, Vector3 c) {
  c.x = a.x - b.x;
  c.y = a.y - b.y;
  c.z = a.z - b.z;
}

static inline void v3dm_scale(Vector3 a, double b, Vector3 c) {
  c.x = a.x * b;
  c.y = a.y * b;
  c.z = a.z * b;
}

static inline double v3dm_dot(Vector3 a, Vector3 b) {
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

static inline void v3dm_project(Vector3 a, Vector3 b, Vector3 c) {
  v3dm_unit(a, c);
  v3dm_scale(c, v3dm_dot(b, c), c);
}

static inline double v3dm_distanceFromPoint(Vector3 a, Vector3 b) {
  Vector3 c;
  v3dm_project(a, b, c);
  v3dm_subtract(b, c, c);
  return v3dm_magnitude(c);
}

static inline void v3dm_cross(Vector3 a, Vector3 b, Vector3 c) {
  c.x = (a.y * b.z) - (a.z * b.y);
  c.y = (a.z * b.x) - (a.x * b.z);
  c.z = (a.x * b.y) - (a.y * b.x);
}

static inline void v3dm_reflect(Vector3 a, Vector3 n, Vector3 b) {
  Vector3 c;
  v3dm_unit(n, n);
  v3dm_scale(n, v3dm_dot(n, a), c);
  v3dm_scale(c, 2, c);
  v3dm_subtract(a, c, b);
}

static inline void v3dm_assign(double x, double y, double z, Vector3 a) {
  a.x = x;
  a.y = y;
  a.z = z;
}
