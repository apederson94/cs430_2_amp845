#include "v3dm_lib.h"
#include "structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void skip_non_alphanum();
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

Pixel* rewind_linked_object_list(Pixel *pixel) {
  if (pixel->prev != NULL) {
    rewind_linked_object_list(pixel->prev);
  } else {
    return pixel;
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

double argv_to_double(char const *str, double index, double result) {
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

void write_pixels_to_file(FILE *fh, Pixel *pixel) {
  if (pixel->next != NULL) {
    fprintf(fh, "%lf\n", pixel->color->r);
    fprintf(fh, "%lf\n", pixel->color->g);
    fprintf(fh, "%lf\n", pixel->color->b);
    write_pixels_to_file(fh, pixel->next);
  } else {
    fprintf(fh, "%lf\n", pixel->color->r);
    fprintf(fh, "%lf\n", pixel->color->g);
    fprintf(fh, "%lf\n", pixel->color->b);
    return;
  }
}

//MARK: -CALCULATIONS

void intersection_sphere(Vector3 *Rd, Vecotr3 *Ro, double Cx, double Cy, double Cz, Vector3 *result) {
  double b, c, t0, t1;
  b = -2 * (Rd->x + Rd->y + Rd->z)
  c = ((Cx * Cx) + (Cy * Cy) + (Cz * Cz));
  t0 = -b + (sqrt((b * b) - (4 * c)) / 2)
  t1 = -b - (sqrt((b * b) - (4 * c)) / 2)
  if (t0 >= 0 && t1 >= 0) {
    if (t0 < t1) {
      result->x = Ro->x + (Rd->x * t0);
      result->y = Ro->y + (Rd->y * t0);
      result->z = Ro->z + (Rd->z * t0);
    } else if (t1 < t0) {
      result->x = Ro->x + (Rd->x * t1);
      result->y = Ro->y + (Rd->y * t1);
      result->z = Ro->z + (Rd->z * t1);
    } else {
      //TODO: ERROR THESE SHOULD NEVER BE EQUAL! could just return one tho (?)
    }
  } else if (t0 < 0 && t1 >= 0) {
    result->x = Ro->x + (Rd->x * t1);
    result->y = Ro->y + (Rd->y * t1);
    result->z = Ro->z + (Rd->z * t1);
  } else if (t1 < 0 && t0 >= 0) {
    result->x = Ro->x + (Rd->x * t0);
    result->y = Ro->y + (Rd->y * t0);
    result->z = Ro->z + (Rd->z * t0);
  } else {
    result->x = -1;
    result->y = -1;
    result->z = -1;
  }
}

void intersection_plane(Vector3 *Ro, Vector3 *Rd, Vector3 *norm, Vector3 *result) {
  double dotRo, dotRd, d, t;
  Vector3 origin;
  origin->x = 0;
  origin->y = 0;
  origin->z = 0;
  d = v3dm_distanceFromPoint(norm, origin);
  dotRo = v3dm_dot(norm, Ro);
  dotRd = v3dm_dot(norm, Rd);
  t = -(dotRo + d) / (dotRd);
  if (t >= 0) {
    result->x = Ro->x + (Rd->x * t);
    result->y = Ro->y + (Rd->y * t);
    result->z = Ro->z + (Rd->z * t);
  } else {
    result->x = -1;
    result->y = -1;
    result->z = -1;
  }

}

Color* castARay(Object *object, Vector3 *Ro, Vector3 *Rd, Object *closest_obj, double closest_intersection) {
  Vector3 intersection;
  double *distance;
  if (object->next != NULL) {
    if (strcmp(object->kind, "SPHERE") == 0) {
      intersection_sphere(Rd, Ro, object->position.x, object->position.y, object->position.z, intersection);
    } else if (strcmp(object->kind, "PLANE") == 0) {
      intersection_plane(Ro, Rd, object->normal, intersection);
    }
    distance = v3dm_distanceFromPoint(Ro, intersection);
    if (distance < closest_intersection) {
      castARay(object->next, Ro, Rd, object, distance);
    } else {
      castARay(object->next, Ro, Rd, closest_obj, closest_intersection);
    }
  } else {
    if (strcmp(object->kind, "SPHERE") == 0) {
      intersection_sphere(Rd, Ro, object->position.x, object->position.y, object->position.z, intersection);
    } else if (strcmp(object->kind, "PLANE") == 0) {
      intersection_plane(Ro, Rd, object->normal, intersection);
    }
    distance = v3dm_distanceFromPoint(Ro, intersection);
    if (distance < closest_intersection) {
      return object->color;
    } else {
      return closest_obj->color;
    }
  }
}

Pixel* render(double width, double height, double xRes, double yRes, Object *object) {
  double xComponent, yComponent, xAdjust, yAdjust, xAdd, yAdd, counter;
  Vector3 *Pij, *Rd, *Ro;
  Color color;
  Pixel *pixel;
  v3dm_assign(0, 0, 0, Ro);
  Pixel *pixels = malloc(sizeof(Pixel) * width * height);
  counter = 0;
  xAdjust = (width/xRes);
  xAdd = (0.5 * xAdjust) + xAdjust;
  xAdd += (-0.5 * width);
  yAdjust = height/yRes;
  yAdd = (0.5 * yAdjust) + yAdjust;
  yAdd += (-0.5 * height);
  for (int i = 0; i < width, i++) {
    for (int j = 0; j < height; j++) {
      v3dm_assign(xAdd, yAdd, -1, Pij);
      v3dm_add(Pij, Ro, Rd);
      color = castARay(object, Ro, Rd, object, INFINITY);
      pixel->color = color;
      pixels[counter];
      counter += sizeof(Pixel);
    }
    yAdd += yAdjust;
  }
  xAdd += xAdjust;
}

Pixel* create_pixel_linked_list(Pixel *image, double index, Pixel *current_pixel) {
  Pixel *new_pixel;
  if (index < sizeof(image)) {
    new_pixel = image[index];
    current_pixel->next = new_pixel;
    new_pixel->prev = current_pixel;
    create_pixel_linked_list(pixel->next, index + sizeof(Pixel), new_pixel);
  } else {
    return current_pixel;
  }
}

//MARK: -MAIN FUNCTION
int main(int argc, char const *argv[]) {
  //variables for setup later
  FILE *input_file, *output_file;
  Pixel *image, *pixel;
  char character;
  double width, height, xRes, yRes;
  double index = 0;

  //setting up some variables
  Object *object = malloc(sizeof(Object));
  Object *result = malloc(sizeof(Object));
  input_file = fopen(argv[3], "r");
  character = fgetc(input_file);
  xRes = argv_to_double(argv[1], index, xRes);
  yRes = argv_to_double(argv[2], index, yRes);


  //replacing character in file for parser to work correctly
  ungetc(character, input_file);

  //parsing to return object
  result = parse_csv(input_file, object, character);
  fclose(input_file);

  //rewinding linked_list then getting width & height from camera and rewinding again
  result = rewind_linked_object_list(result);
  width = get_width(result);
  height = get_height(result);
  result = rewind_linked_object_list(result);

  //rendering image
  image = render(Ro, width, height, xRes, yRes, object);
  rewind_linked_object_list(object);

  //setting up image file
  output_file = fopen(argv[4], "w");
  fprintf(output_file, "%s\n%s\n%lf\n%lf\n%d", "P3", "Created by Austin Pederson's raycaster",xRes, yRes, 255);

  //writing to image file
  index = sizeof(Pixel);
  pixel = image[0];
  pixel = create_pixel_linked_list(image, index, pixel);
  rewind_linked_pixel_list(pixel);
  write_pixels_to_file(output_file, pixel);
  fclose(output_file);

  return 0;
}
