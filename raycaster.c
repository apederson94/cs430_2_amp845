#include "v3dm_lib.h"
#include "structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void skip_non_alphanum();
/*double intersection();

double intersection(V3 Pij, Object object) {
  double b, c;
  if (object.kind == "SPHERE") {
    b = -2*(Pij[0] + Pij[1] + Pij[2]);
    c = (Cx*Cx) + (Cy*Cy) + (Cz*Cz) + (r*r);
    t0 = -b + (sqrt((b*b) - (4 *c))/2);
    t1 = -b - (sqrt((b*b) - (4 *c))/2);
    if (t0 > t1) {
      return t1;
    } else if (t1 > t0) {
      return t0;
    }
  } else if (object.kind == "PLANE") {
    //TODO: Plane intersection
  }
  return -1;
}

//renders the image
Pixel* render(int width, int height, double xRes, double yRes, double focalLength, double r, Object *objects[]) {
  V3 Pij;
  V3 Rd, Ro;
  Pixel pixel;
  Pixel *pixels[] = malloc(sizeof(int) * width * height * 3);
  Color color;
  double x, y, half_width, half_height, width_d_res, height_d_res, iComponent,
  jComponent, pixelAdjustX, pixelAdjustY, t;
  half_width = -width/2;
  half_height = height/2;
  width_d_res = width/xRes;
  height_d_res = height/yRes;
  pixelAdjustX = width_d_res/2;
  pixelAdjustY = height_d_res/2;
  v3dm_assign(0, 0, 0, Ro);
  //loops over rows
  for (int i = 0; i < width; i++) {
    //x calculation
    iComponent = i * (width_d_res) + pixelAdjustX
    x = half_width + iComponent;
    //loops over columns
    for (int j = 0; j < height; j++) {
      //y calculation
      jComponent = (j* height_d_res) + pixelAdjust y;
      y = half_height + jComponent;

      v3dm_assign(x, y, -focalLength, Pij);
      t = 0;//intersection(Pij, );
      if (t != -1) {
        v3dm_unit(Pij, Rd);
        v3dm_scale(Rd, t, Rd);
        color = castARay(objects, Rd, Ro);
        color.r *= 255;
        color.g *= 255;
        color.b *= 255;
      } else {
        color.r = 0;
        color.g = 0;
        color.b = 0;
      }
      pixel.color = color;
      pixels[sizeof(int) * (i*width + j)] = pixel;
    }
  }
  return pixels;
}

Color castARay(Object *objects[], V3 Rd, V3 Ro) {
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
  return closest_obj->color;
}*/

//MARK: - PARSER FUNCTIONS
void parse_type(FILE *fh, Object *obj) {
  char *str = (char *) malloc(sizeof(sizeof(double)));
  char *character = malloc(sizeof(char));

  sprintf(character, "%c", fgetc(fh));
  while (*character != 44) {
    strcat(str, character);
    sprintf(character, "%c", fgetc(fh));
  }
  if (strcmp(str, "sphere") == 0) {
    obj->kind = "SPHERE";
  } else if (strcmp(str, "plane") == 0) {
    obj->kind = "PLANE";
  } else if (strcmp(str, "camera") == 0) {
    obj->kind = "CAMERA";
  }
  skip_non_alphanum(fh);
}

void parse_field(FILE *fh, Object *obj) {
  char *str = (char *) malloc(sizeof(char) * 9);
  char *character = malloc(sizeof(char));
  sprintf(character, "%c", fgetc(fh));
  while (*character != 58) {
    strcat(str, character);
    sprintf(character, "%c", fgetc(fh));
  }
  skip_non_alphanum(fh);
  if (strcmp(str, "width") == 0) {
    fscanf(fh, "%lf", &obj->width);
  } else if (strcmp(str, "height") == 0) {
    fscanf(fh, "%lf", &obj->height);
  }
  skip_non_alphanum(fh);
}

void parse_radius(FILE *fh, Object *obj) {
  char *str = (char *) malloc(sizeof(char) * 6);
  char *character = malloc(sizeof(char));
  sprintf(character, "%c", fgetc(fh));
  while (*character != 58) {
    strcat(str, character);
    sprintf(character, "%c", fgetc(fh));
  }
  skip_non_alphanum(fh);
  if (strcmp(str, "radius") == 0) {
    fscanf(fh, "%lf", &obj->radius);
  }
  fgetc(fh);
}

void parse_position(FILE *fh, Object *obj) {
  char *str = (char *) malloc(sizeof(char) * 8);
  char *character = malloc(sizeof(char));
  sprintf(character, "%c", fgetc(fh));
  while (*character != 58) {
    strcat(str, character);
    sprintf(character, "%c", fgetc(fh));
  }
  skip_non_alphanum(fh);
  if (strcmp(str, "position") == 0) {
    fscanf(fh, "%lf", &obj->position.x);
    fgetc(fh);
    fscanf(fh, "%lf", &obj->position.y);
    fgetc(fh);
    fscanf(fh, "%lf", &obj->position.z);
  }
  skip_non_alphanum(fh);
}

void parse_color(FILE *fh, Object *obj) {
  char *str = (char *) malloc(sizeof(char) * 5);
  char *character = malloc(sizeof(char));
  sprintf(character, "%c", fgetc(fh));
  while (*character != 58) {
    strcat(str, character);
    sprintf(character, "%c", fgetc(fh));
  }
  skip_non_alphanum(fh);
  if (strcmp(str, "color") == 0) {
    fscanf(fh, "%lf", &obj->color.r);
    fgetc(fh);
    fscanf(fh, "%lf", &obj->color.g);
    fgetc(fh);
    fscanf(fh, "%lf", &obj->color.b);
  }
  skip_non_alphanum(fh);
}

void parse_normal(FILE *fh, Object *obj) {
  char *str = (char *) malloc(sizeof(char) * 6);
  char *character = malloc(sizeof(char));
  sprintf(character, "%c", fgetc(fh));
  while (*character != 58) {
    strcat(str, character);
    sprintf(character, "%c", fgetc(fh));
  }
  skip_non_alphanum(fh);
  fscanf(fh, "%lf", &obj->normal.x);
  fgetc(fh);
  fscanf(fh, "%lf", &obj->normal.y);
  fgetc(fh);
  fscanf(fh, "%lf", &obj->normal.z);
  if (strcmp(str, "normal") == 0) {
    fscanf(fh, "%lf", &obj->normal.x);
    fgetc(fh);
    fscanf(fh, "%lf", &obj->normal.y);
    fgetc(fh);
    fscanf(fh, "%lf", &obj->normal.z);
  }
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
  FILE *fh;
  Pixel *image;
  int ctr = 0;
  Object *object = malloc(sizeof(Object));
  Object *objects[128];
  char character, *output;
  double xRes, yRes, width, height;
  xRes = 1024;
  yRes = 1024;
  width = 50;
  height = 50;
  fh = fopen(argv[3], "r");
  character = fgetc(fh);
  ungetc(character, fh);
  while(character != EOF) {
    parse_type(fh, object);
    if (strcmp(object->kind, "CAMERA") == 0) {
      parse_field(fh, object);
      parse_field(fh, object);
    } else if (strcmp(object->kind, "SPHERE") == 0) {
      parse_color(fh, object);
      parse_position(fh, object);
      parse_radius(fh, object);
    } else if (strcmp(object->kind, "PLANE") == 0) {
      parse_color(fh, object);
      parse_position(fh, object);
      parse_normal(fh, object);
    }
    character = fgetc(fh);
    ungetc(character, fh);
    objects[ctr] = object;
    ctr++;
  }
  //image = render(xRes, yRes, width, height, )
  return 0;
}
