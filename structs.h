#ifndef STRUCT_H
#define  STRUCT_H

typedef struct Color {
  double r, g, b;
} Color;

typedef struct Object {
  char *kind;
  double radius, width, height;
  struct Vector3 position, normal;
  struct Object *prev, *next;
  struct Color diffuse_color, specular_color;
  struct Radial radial;
} Object;

typedef struct Pixel {
  struct Color color;
  struct Pixel *next, *prev;
} Pixel;

typedef struct Radial {
  int a0, a1, a2;
} Radial;

#endif
