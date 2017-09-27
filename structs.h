#ifndef STRUCT_H
#define  STRUCT_H

typedef struct Color {
  int r;
  int g;
  int b;
} Color;

typedef struct Object {
  char *kind;
  double *position, *normal, radius, width, height;
  Color color;
  struct Object *next;
} Object;

typedef struct Pixel {
  struct Color color;
} Pixel;

#endif
