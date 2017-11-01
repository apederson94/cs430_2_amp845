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
  double radius, width, height, theta;
  struct Vector3 position, normal;
  struct Object *prev, *next;
  struct Color diffuse_color, specular_color, color;
  struct Radial radial;
} Object;

typedef struct Pixel {
  struct Color color;
  struct Pixel *next, *prev;
} Pixel;

typedef struct Light {
  char *kind;
  struct Vector3 position, direction;
  struct Color color;
  struct Radial radial;
  double theta, angular;
  struct Light *prev, *next;
} Light;

typedef struct ObjectPlus {
  struct Object object;
  struct Vector3 intersection;
} ObjectPlus;

#endif
