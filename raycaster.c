#include "v3dm_lib.h"
#include "structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void skip_non_alphanum();
double intersection();

Pixel* render(int width, int height) {
  V3 Pij;
  V3 Rd;
  Color color;
  double x, y, half_width, half_height, width_d_res, height_d_res, iComponent,
  jComponent, pixelAdjustX, pixelAdjustY;
  half_width = -width/2;
  half_height = height/2;
  width_d_res = width/1024;
  height_d_res = height/1024;
  pixelAdjustX = 0.5 * width_d_res;
  pixelAdjustY = 0.5 * height_d_res;
  for (int i = 0; i < width; i++) {
    iComponent = i * (width_d_res) + pixelAdjustX
    for (int j = 0; j < height; j++) {
      jComponent = (j* height_d_res) + pixelAdjust y;
      x = half_width + iComponent;
      y = half_height + jComponent;
      v3dm_assign(x, y, , a)
      //v3dm_assign(Pij,...);
      //v3dm_assign(Rd,...);
    }
  }
  //color = castARay(Ro, Rd,...);
  //color.r *= 255;
  //color.g *= 255;
  //color.b *= 255;
}

Color castARay(Object *objects[]) {
  Object *obj = objects[0];
  double closest_t = INFINITY;
  double t;
  V3 *Ro, Rd;
  Object *closest_obj;
  while (obj != NULL) {
    t = intersection(Ro, Rd, obj);
    if (t < closest_t) {
      closest_t = t;
      closest_obj = obj;
    }
    obj = obj->next;
  }
  return obj->color;
}

//MARK: - PARSER FUNCTIONS
void parse_type(FILE *fh, Object *obj) {
  char *str = (char *) malloc(sizeof(sizeof(double)));
  char *character = malloc(sizeof(char));
  sprintf(character, "%c", fgetc(fh));
  while (*character != 44) {
    strcat(str, character);
    sprintf(character, "%c", fgetc(fh));
  }
  skip_non_alphanum(fh);
  if (strcmp(str, "sphere")) {
    obj->kind = "SPHERE";
  } else if (strcmp(str, "plane")) {
    obj->kind = "PLANE";
  }
}

void parse_field(FILE *fh, Object *obj) {
  char *str = (char *) malloc(sizeof(sizeof(double)));
  char *character = malloc(sizeof(char));
  sprintf(character, "%c", fgetc(fh));
  while (*character != 58) {
    strcat(str, character);
    sprintf(character, "%c", fgetc(fh));

  }
  skip_non_alphanum(fh);
  if (strcmp(str, "width")) {
    fscanf(fh, "%lf", &obj->width);
  } else if (strcmp(str, "height")) {
    fscanf(fh, "%lf", &obj->height);
  }
  skip_non_alphanum(fh);
}

void parse_radius(FILE *fh, Object *obj) {
  char *str = (char *) malloc(sizeof(sizeof(double)));
  char *character = malloc(sizeof(char));
  sprintf(character, "%c", fgetc(fh));
  while (*character != 58) {
    strcat(str, character);
    sprintf(character, "%c", fgetc(fh));
  }
  skip_non_alphanum(fh);
  if (strcmp(str, "radius")) {
    fscanf(fh, "%lf", &obj->radius);
  }
  skip_non_alphanum(fh);
}

void parse_position(FILE *fh, Object *obj) {
  char *str = (char *) malloc(sizeof(sizeof(double)));
  char *character = malloc(sizeof(char));
  sprintf(character, "%c", fgetc(fh));
  while (*character != 58) {
    strcat(str, character);
    sprintf(character, "%c", fgetc(fh));
  }
  if (strcmp(str, "position")) {
    skip_non_alphanum(fh);
    fscanf(fh, "%lf", &obj->position[0]);
    fgetc(fh);
    fscanf(fh, "%lf", &obj->position[1]);
    fgetc(fh);
    fscanf(fh, "%lf", &obj->position[2]);
  }
  skip_non_alphanum(fh);
}

void parse_color(FILE *fh, Object *obj) {
  char *str = (char *) malloc(sizeof(sizeof(double)));
  char *character = malloc(sizeof(char));
  sprintf(character, "%c", fgetc(fh));
  while (*character != 58) {
    strcat(str, character);
    sprintf(character, "%c", fgetc(fh));
  }
  skip_non_alphanum(fh);
  if (strcmp(str, "color")) {
    fscanf(fh, "%i", &obj->color.r);
    fgetc(fh);
    fscanf(fh, "%i", &obj->color.g);
    fgetc(fh);
    fscanf(fh, "%i", &obj->color.b);
  }
  skip_non_alphanum(fh);
}

void parse_normal(FILE *fh, Object *obj) {
  char *str = (char *) malloc(sizeof(double));
  char *character = malloc(sizeof(char));
  sprintf(character, "%c", fgetc(fh));
  while (*character != 58) {
    strcat(str, character);
    sprintf(character, "%c", fgetc(fh));
  }
  skip_non_alphanum(fh);
  if (strcmp(str, "normal")) {
    fscanf(fh, "%lf", &obj->normal[0]);
    fgetc(fh);
    fscanf(fh, "%lf", &obj->normal[1]);
    fgetc(fh);
    fscanf(fh, "%lf", &obj->normal[2]);
  }
  skip_non_alphanum(fh);
}

void skip_non_alphanum(FILE *fh){
  char character;
  character = fgetc(fh);
  //compares character to all non-alphanum characters and cycles through them until an alphanum character is found
  while ((character < 48) || (character > 57 && character < 65) || (character > 90 && character < 97) || (character > 122)) {
    character = fgetc(fh);
  }
  //replaces alphanum character that was pulled from the file
  ungetc(character, fh);
}

int main(int argc, char const *argv[]) {

  return 0;
}
