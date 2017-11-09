#ifndef STRUCT_H
#define  STRUCT_H

typedef struct Color {
  double r, g, b;
} Color;

typedef struct Radial {
  double a0, a1, a2;
} Radial;

typedef struct Object {
  char *kind;
  double radius, width, height, theta, angular;
  struct Vector3 position, normal, direction;
  struct Color diffuse_color, specular_color, color;
  struct Radial radial;
} Object;

typedef struct Light {
  char *kind;
  struct Vector3 position, direction;
  struct Color color;
  struct Radial radial;
  double theta, angular;
} Light;

typedef struct ObjectPlus {
  struct Object object;
  struct Vector3 *intersection;
  int valid;
} ObjectPlus;

typedef struct Scene {
  struct Object *objects;
  struct Light *lights;
} Scene;

#endif
