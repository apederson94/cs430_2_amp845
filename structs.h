#ifndef STRUCT_H
#define  STRUCT_H

typedef struct Color {
  double r, g, b;
} Color;

typedef struct Vector3 {
  double x, y, z;
} Position;

typedef struct Object {
  char *kind;
  double radius, width, height;
  struct Vector3 position, normal;
  struct Object *prev, *next;
  struct Color color;
} Object;

typedef struct Pixel {
  struct Color color;
} Pixel;

#endif
