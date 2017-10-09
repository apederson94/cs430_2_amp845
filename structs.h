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
  struct Color color;
} Object;

typedef struct Pixel {
  struct Color color;
  struct Pixel *next, *prev;
} Pixel;

#endif
