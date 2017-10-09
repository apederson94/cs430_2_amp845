#include "v3dm_lib.h"
#include "structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void skip_non_alphanum();
void intersection_sphere();
//MARK: - PARSER FUNCTIONS

//parses the object type from csv
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

//prases width and height fields from csv
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

//parses radius from csv
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

//parses position from csv
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

//parses color from csv
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

//parses the normal vector from csv
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

//comprehensive parser that combines all parser helper functions
Object* parse_csv(FILE *fh, Object *object, char character) {
  char new_char;
  if (character != EOF) {
    Object *object_next = malloc(sizeof(Object));
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
    object_next->prev = object;
    object->next = object_next;
    new_char = fgetc(fh);
    ungetc(new_char, fh);
    parse_csv(fh, object_next, new_char);
  } else {
    return object;
  }
}

//skips all non-alphanumeric characters in a file
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

//helper function to make sure parser was working correctly.
//TODO: REMOVE BEFORE TURNING IN
void print_all(Object *object) {
  Object *object1 = malloc(sizeof(Object));
  object1 = object;
  while (object1->next != NULL) {
    printf("KIND: %s\nRADIUS: %lf\nWIDTH: %lf\nHEIGHT: %lf\nPOSITIONX: %lf\nPOSITIONY: %lf\nPOSITIONZ: %lf\nNORMALX: %lf\nNORMALY: %lf\nNORMALZ: %lf\nCOLORR: %lf\nCOLORG: %lf\nCOLORB: %lf\n",
  object1->kind, object1->radius, object1->width, object1->height, object1->position.x, object1->position.y, object1->position.z, object1->normal.x, object1->normal.y, object1->normal.z, object1->color.r, object1->color.g, object1->color.b);
  object1 = object1->next;
  }
}

Object* rewind_linked_object_list(Object *object) {
  if (object->prev != NULL) {
    rewind_linked_object_list(object->prev);
  } else {
    return object;
  }
}

//returns camera width
double get_width(Object *object) {
  if (strcmp(object->kind, "CAMERA") == 0) {
    return object->width;
  } else if (object->next == NULL) {
    //TODO: ERROR HERE FOR NO CAMERA OBJECT IN LINKED LIST
  } else {
    get_width(object->next);
  }
}

//returns camera height
double get_height(Object *object) {
  if (strcmp(object->kind, "CAMERA") == 0) {
    return object->height;
  } else if (object->next == NULL) {
    //TODO: ERROR HERE FOR NO CAMERA OBJECT IN LINKED LIST
  } else {
    get_height(object->next);
  }
}

double argv_to_double(char const *str, int index, double result) {
  if (index == strlen(str)) {
    return result;
  } else {
    double updated_result = result;
    char character = str[index];
    updated_result *= 10;
    updated_result += character - '0';
    argv_to_double(str, index + 1, updated_result);
  }
}

void print_v3(Vector3 *a) {
  printf("x: %lf\ny: %lf\nz: %lf\n", a->x, a->y, a->z);
}

//MARK: -CALCULATIONS

void intersection_sphere(Vector3 *Rd, Vector3 *Ro, double Cx, double Cy, double Cz, Vector3 *result) {
  double b, c, t0, t1, x, y, z;
  b = -2 * (Rd->x + Rd->y + Rd->z);
  c = ((Cx * Cx) + (Cy * Cy) + (Cz * Cz));
  t0 = -b + (sqrt((b * b) - (4 * c)) / 2);
  t1 = -b - (sqrt((b * b) - (4 * c)) / 2);
  //printf("t0: %lf\nt1: %lf\n", t0, t1);
  if (!isnan(t0) && !isnan(t1)) {
    if (t0 >= 0 && t1 >= 0) {
      if (t0 < t1) {
        x = Ro->x + (Rd->x * t0);
        y = Ro->y + (Rd->y * t0);
        z = Ro->z + (Rd->z * t0);
        v3dm_assign(x, y, z, result);
      } else if (t1 < t0) {
        x = Ro->x + (Rd->x * t1);
        y = Ro->y + (Rd->y * t1);
        z = Ro->z + (Rd->z * t1);
        v3dm_assign(x, y, z, result);
      }
    } else {
      if (t1 < 0) {
        x = Ro->x + (Rd->x * t0);
        y = Ro->y + (Rd->y * t0);
        z = Ro->z + (Rd->z * t0);
        v3dm_assign(x, y, z, result);
      } else if (t0 < 0) {
        x = Ro->x + (Rd->x * t1);
        y = Ro->y + (Rd->y * t1);
        z = Ro->z + (Rd->z * t1);
        v3dm_assign(x, y, z, result);
      }
    }
  } else {
    if (isnan(t0) && !isnan(t1)) {
      x = Ro->x + (Rd->x * t1);
      y = Ro->y + (Rd->y * t1);
      z = Ro->z + (Rd->z * t1);
      v3dm_assign(x, y, z, result);
    } else if (isnan(t1) && !isnan(t0)) {
      x = Ro->x + (Rd->x * t0);
      y = Ro->y + (Rd->y * t0);
      z = Ro->z + (Rd->z * t0);
      v3dm_assign(x, y, z, result);
    } else {
      x = INFINITY;
      y = INFINITY;
      z = INFINITY;
      //printf("YO\n");
      v3dm_assign(x, y, z, result);
      //printf("YO\n");
    }
  }
}

void intersection_plane(Vector3 *Ro, Vector3 *Rd, struct Vector3 norm, Vector3 *result) {
  double dotRo, dotRd, d, t, x, y , z;
  Vector3 *origin, *normal;
  origin = malloc(sizeof(Vector3));
  normal = malloc(sizeof(Vector3));
  v3dm_assign(0, 0, 0, origin);
  d = v3dm_distanceFromPoint(normal, origin);
  if (isinf(d)) {
    x = INFINITY;
    y = INFINITY;
    z = INFINITY;
    return;
  }
  dotRo = v3dm_dot(&norm, Ro);
  dotRd = v3dm_dot(&norm, Rd);
  t = -(dotRo + d) / (dotRd);
  if (t >= 0) {
    x = Ro->x + (Rd->x * t);
    y = Ro->y + (Rd->y * t);
    z = Ro->z + (Rd->z * t);
    v3dm_assign(x, y, z, result);
  } else {
    x = INFINITY;
    y = INFINITY;
    z = INFINITY;
    v3dm_assign(x, y, z, result);
  }

}

Color* castARay(Object *object, Vector3 *Ro, Vector3 *Rd, Object *closest_obj, double closest_intersection) {
  Vector3 *intersection;
  double distance;
  Color *color;
  intersection = malloc(sizeof(Vector3));
  if (object->kind != NULL) {
    //printf("KIND: %s\n", object->kind);
    if (strcmp(object->kind, "SPHERE") == 0) {
      intersection_sphere(Rd, Ro, object->position.x, object->position.y, object->position.z, intersection);
      //printf("WTF\n");
    } else if (strcmp(object->kind, "PLANE") == 0) {
      intersection_plane(Ro, Rd, object->normal, intersection);
    } else if (strcmp(object->kind, "CAMERA") == 0) {
      castARay(object->next, Ro, Rd, closest_obj, closest_intersection);
    }
    //print_v3(intersection);
    if (!isinf(intersection->x)) {
      distance = v3dm_distanceFromPoint(Ro, intersection);
      if (isinf(closest_intersection)) {
        castARay(object->next, Ro, Rd, object, distance);
      } else  if (distance < closest_intersection) {
        castARay(object->next, Ro, Rd, object, distance);
      } else {
        castARay(object->next, Ro, Rd, closest_obj, closest_intersection);
      }
    } else {
      castARay(object->next, Ro, Rd, closest_obj, closest_intersection);
    }
  } else {
    if (isinf(closest_intersection)) {
      color = malloc(sizeof(Color));
      color->r = 0;
      color->g = 0;
      color->b = 0;
      return color;
    } else {
      printf("%lf\n", closest_obj->color.b);
      return &closest_obj->color;
    }
  }

}

double* render(double width, double height, double xRes, double yRes, Object *object) {
  double x, y, *colors;
  Vector3 *Pij, *Rd, *Ro;
  Color *color;
  Ro = malloc(sizeof(Vector3));
  Pij = malloc(sizeof(Vector3));
  Rd = malloc(sizeof(Vector3));
  colors = malloc(sizeof(double) * sizeof(double) * xRes * yRes * 3);
  v3dm_assign(0, 0, 0, Ro);
  int counter = 0;
  for (int i = 0; i < xRes; i++) {
    printf("%d\n", i);
    x = -(width/2) + (i * (width/xRes)) + (0.5 * (width/xRes));
    for (int j = 0; j < yRes; j++) {
      //TODO: array issues
      y = -(height/2) + (j * (height/yRes)) + (0.5 * (width/yRes));
      v3dm_assign(x, y, -1, Pij);
      v3dm_add(Pij, Ro, Rd);
      color = castARay(object, Ro, Rd, object, INFINITY);
      colors[counter] = color->r;
      counter += (int) sizeof(double);
      colors[counter] = color->g;
      counter += (int) sizeof(double);
      colors[counter] = color->b;
      counter += (int) sizeof(double);
    }
  }
  return colors;
}

//MARK: -MAIN FUNCTION
int main(int argc, char const *argv[]) {
  //variables for setup later
  FILE *input_file, *output_file;
  char character;
  double width, height, xRes, yRes, *colors;
  double index = 0;
  int i, j;

  //setting up some variables
  Object *object = malloc(sizeof(Object));
  Object *result_object = malloc(sizeof(Object));
  input_file = fopen(argv[3], "r");
  character = fgetc(input_file);
  xRes = argv_to_double(argv[1], index, xRes);
  yRes = argv_to_double(argv[2], index, yRes);


  //replacing character in file for parser to work correctly
  ungetc(character, input_file);

  //parsing to return object
  result_object = parse_csv(input_file, object, character);
  fclose(input_file);

  //rewinding linked_list then getting width & height from camera and rewinding again
  result_object = rewind_linked_object_list(result_object);
  width = get_width(result_object);
  height = get_height(result_object);
  result_object = rewind_linked_object_list(result_object);

  //rendering image
  printf("start rendering\n");
  colors = malloc(sizeof(xRes * yRes * sizeof(double) * 3));
  colors = render(width, height, xRes, yRes, result_object);
  printf("rendered\n");
  rewind_linked_object_list(object);

  //setting up image file
  output_file = fopen(argv[4], "w");
  fprintf(output_file, "%s\n%s\n%lf\n%lf\n%d\n", "P3", "Created by Austin Pederson's raycaster", xRes, yRes, 255);

  //writing to image file
  fwrite(colors, sizeof(double), 3 * sizeof(double) * width * height, output_file);
  fclose(output_file);

  return 0;
}
