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
  while (*character != ',') {
    strcat(str, character);
    sprintf(character, "%c", fgetc(fh));
  }
  if (strcmp(str, "sphere") == 0) {
    obj->kind = "SPHERE";
  } else if (strcmp(str, "plane") == 0) {
    obj->kind = "PLANE";
  } else if (strcmp(str, "camera") == 0) {
    obj->kind = "CAMERA";
  } else if (strcmp(str, "light") == 0) {
    obj->kind = "LIGHT";
  } else {
    perror("Error: Object must be of type camera, sphere, plane, or light. Please try again with only these types of objects.");
    exit(EXIT_FAILURE);
  }
  skip_non_alphanum(fh);
}

void rewind_file(FILE *fh, char *str) {
  fseek(fh, -strlen(str), SEEK_CUR);
}

int check_str(FILE *fh, char *str) {
  char *c = malloc(sizeof(char));
  for (int i = 0; i < strlen(str); i++) {
    *c = str[i];
    if ((*c < '0' || *c > '9') && (*c != '.' && *c != '-')) {
      return 0;
    }
  }
  //rewind_file(fh, str);
  return 1;
}

//parses width and height fields from csv
void parse_field(FILE *fh, Object *obj) {
  char *str = (char *) malloc(sizeof(char) * 9);
  char *character = malloc(sizeof(char));
  char *tmp = malloc(100);
  int check = 0;
  sprintf(character, "%c", fgetc(fh));
  while (*character != 58) {
    strcat(str, character);
    sprintf(character, "%c", fgetc(fh));
  }
  skip_non_alphanum(fh);
  if (strcmp(str, "width") == 0) {
    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]\n", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->width);
      if (obj->width <= 0.0) {
        perror("Error: Camera width must be greater than zero. Please try again with value greater than zero.");
      }
    } else {
      perror("Error: Camera width must be a numeric value. Please try again with a numeric value.");
      exit(EXIT_FAILURE);
    }
  } else if (strcmp(str, "height") == 0) {
    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]\n", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->height);
      if (obj->height <= 0.0) {
        perror("Error: Camera height must be greater than zero. Please try again with a value greater than zero.");
      }
    } else {
      perror("Error: Camera height must be a numeric value. Please try again with a numeric value.");
      exit(EXIT_FAILURE);
    }
  }
  skip_non_alphanum(fh);
}

//parses radius from csv
void parse_radius(FILE *fh, Object *obj) {
  char *str = (char *) malloc(sizeof(char) * 6);
  char *character = malloc(sizeof(char));
  char *tmp = malloc(100);
  int check = 0;
  sprintf(character, "%c", fgetc(fh));
  while (*character != 58) {
    strcat(str, character);
    sprintf(character, "%c", fgetc(fh));
  }
  skip_non_alphanum(fh);
  if (strcmp(str, "radius") == 0) {
    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->radius);
      if (obj->radius < 0) {
        perror("Error: Sphere radius must be a positive value. Please try again with a positive radius.");
      }
    } else {
      perror("Error: Sphere radius must be a numeric value. Please try again with a numeric value.");
      exit(EXIT_FAILURE);
    }

  }
  skip_non_alphanum(fh);
}

//parses position from csv
void parse_position(FILE *fh, Object *obj) {
  char *str = (char *) malloc(sizeof(char) * 8);
  char *character = malloc(sizeof(char));
  char *tmp = malloc(100);
  int check = 0;
  sprintf(character, "%c", fgetc(fh));
  while (*character != 58) {
    strcat(str, character);
    sprintf(character, "%c", fgetc(fh));
  }
  skip_non_alphanum(fh);
  if (strcmp(str, "position") == 0) {
    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->position.x);
      skip_non_alphanum(fh);
    } else {
      perror("Error: All x positions must be in numeric format. Please retry with numeric values.");
      exit(EXIT_FAILURE);
    }

    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->position.y);
      skip_non_alphanum(fh);
    } else {
      perror("Error: All y positions must be in numeric format. Please retry with numeric values.");
      exit(EXIT_FAILURE);
    }

    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->position.z);
      skip_non_alphanum(fh);
    } else {
      perror("Error: All z positions must be in numeric format. Please retry with numeric values.");
      exit(EXIT_FAILURE);
    }
  }
  skip_non_alphanum(fh);
}

//parses color from csv
void parse_color(FILE *fh, Object *obj) {
  char *str = (char *) malloc(sizeof(char) * 5);
  char *character = malloc(sizeof(char));
  char *tmp = malloc(100);
  int check = 0;
  while (*character != 58) {
    sprintf(character, "%c", fgetc(fh));
    strcat(str, character);
  }

  if (strcmp(str, "color:") == 0) {
    skip_non_alphanum(fh);
    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->color.r);
      if (obj->color.r < 0.0 || obj->color.r > 1.0) {
        perror("Error: Color values may only range from 0.0 to 1.0. Please retry with correct color values.");
        exit(EXIT_FAILURE);
      } else {
        obj->color.r *= 255;
        skip_non_alphanum(fh);
      }
    } else {
      perror("Error: Color values must be of numeric format. Please retry with numeric values.");
      exit(EXIT_FAILURE);
    }

    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->color.g);
      if (obj->color.g < 0.0 || obj->color.g > 1.0) {
        perror("Error: Color values may only range from 0.0 to 1.0. Please retry with the correct color values.");
        exit(EXIT_FAILURE);
      } else {
        obj->color.g *= 255;
        skip_non_alphanum(fh);
      }
    } else {
      perror("Error: Color values must be of numeric format. Please retry with numeric values.");
      exit(EXIT_FAILURE);
    }

    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->color.b);
      if (obj->color.b < 0.0 || obj->color.b > 1.0) {
        perror("Error: Color values may only range from 0.0 to 1.0. Please retry with the correct color values.");
        exit(EXIT_FAILURE);
      } else {
        obj->color.b *= 255;
        skip_non_alphanum(fh);
      }
    } else {
      perror("Error: Color values must be of numeric format. Please retry with numeric values.");
      exit(EXIT_FAILURE);
    }
  } else if (strcmp(str, "diffuse_color:") == 0) {
    skip_non_alphanum(fh);
    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->diffuse_color.r);

      if (obj->diffuse_color.r < 0.0 || obj->diffuse_color.r > 1.0) {
        perror("Error: diffuse_color values may only range from 0.0 to 1.0. Please retry with correct diffuse_color values.");
        exit(EXIT_FAILURE);
      } else {
        obj->diffuse_color.r *= 255;
        skip_non_alphanum(fh);
      }
    } else {
      perror("Error: diffuse_color values must be of numeric format. Please retry with numeric values.");
      exit(EXIT_FAILURE);
    }

    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->diffuse_color.g);
      if (obj->diffuse_color.g < 0.0 || obj->diffuse_color.g > 1.0) {
        perror("Error: diffuse_color values may only range from 0.0 to 1.0. Please retry with the correct diffuse_color values.");
        exit(EXIT_FAILURE);
      } else {
        obj->diffuse_color.g *= 255;
        skip_non_alphanum(fh);
      }
    } else {
      perror("Error: diffuse_color values must be of numeric format. Please retry with numeric values.");
      exit(EXIT_FAILURE);
    }

    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->diffuse_color.b);
      if (obj->diffuse_color.b < 0.0 || obj->diffuse_color.b > 1.0) {
        perror("Error: diffuse_color values may only range from 0.0 to 1.0. Please retry with the correct diffuse_color values.");
        exit(EXIT_FAILURE);
      } else {
        obj->diffuse_color.b *= 255;
        skip_non_alphanum(fh);
      }
    } else {
      perror("Error: diffuse_color values must be of numeric format. Please retry with numeric values.");
      exit(EXIT_FAILURE);
    }
  } else if (strcmp(str, "specular_color:") == 0) {
    skip_non_alphanum(fh);
    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->specular_color.r);
      if (obj->specular_color.r < 0.0 || obj->specular_color.r > 1.0) {
        perror("Error: specular_color values may only range from 0.0 to 1.0. Please retry with correct specular_color values.");
        exit(EXIT_FAILURE);
      } else {
        obj->specular_color.r *= 255;
        skip_non_alphanum(fh);
      }
    } else {
      perror("Error: specular_color values must be of numeric format. Please retry with numeric values.");
      exit(EXIT_FAILURE);
    }

    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->specular_color.g);
      if (obj->specular_color.g < 0.0 || obj->specular_color.g > 1.0) {
        perror("Error: specular_color values may only range from 0.0 to 1.0. Please retry with the correct specular_color values.");
        exit(EXIT_FAILURE);
      } else {
        obj->specular_color.g *= 255;
        skip_non_alphanum(fh);
      }
    } else {
      perror("Error: specular_color values must be of numeric format. Please retry with numeric values.");
      exit(EXIT_FAILURE);
    }

    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->specular_color.b);
      if (obj->specular_color.b < 0.0 || obj->specular_color.b > 1.0) {
        perror("Error: specular_color values may only range from 0.0 to 1.0. Please retry with the correct specular_color values.");
        exit(EXIT_FAILURE);
      } else {
        obj->specular_color.b *= 255;
        skip_non_alphanum(fh);
      }
    } else {
      perror("Error: specular_color values must be of numeric format. Please retry with numeric values.");
      exit(EXIT_FAILURE);
    }
  } else {
    rewind_file(fh, str);
  }
  skip_non_alphanum(fh);
}

//parses theta from csv
void parse_theta(FILE *fh, Object *obj) {
  char *str = malloc(sizeof(char) * 6);
  char *character = malloc(sizeof(char));
  char *tmp = malloc(100);
  int check = 0;
  sprintf(character, "%c", fgetc(fh));
  while (*character != ':') {
    strcat(str, character);
    sprintf(character, "%c", fgetc(fh));
  }
  skip_non_alphanum(fh);
  if (strcmp(str, "theta") == 0) {
    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->theta);
    } else {
      perror("Error: Theta values must be of nuermic format. Please try again with numeric values.");
      exit(EXIT_FAILURE);
    }
  } else {
    obj->theta = 0;
    rewind_file(fh, str);
  }
  skip_non_alphanum(fh);
}

//parses radial from csv
void parse_radial(FILE *fh, Object *obj) {
  char *str = malloc(sizeof(char) * 10);
  char *character = malloc(sizeof(char));
  char *tmp = malloc(100);
  int check = 0;
  sprintf(character, "%c", fgetc(fh));
  while (*character != ':') {
    strcat(str, character);
    sprintf(character, "%c", fgetc(fh));
  }
  skip_non_alphanum(fh);

  if (strcmp(str, "radial-a2") == 0) {
    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->radial.a2);
    } else {
      perror("Error: radial-a2 values must be of numeric format. Please try again with numeric values.");
      exit(EXIT_FAILURE);
    }
  } else if (strcmp(str, "radial-a1") == 0) {
    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->radial.a1);
    } else {
      perror("Error: radial-a1 values must be of numeric format. Please try again with numeric values.");
      exit(EXIT_FAILURE);
    }
  } else if (strcmp(str, "radial-a0") == 0) {
    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->radial.a0);
    } else {
      perror("Error: radial-a0 values must be of numeric format. Please try again with numeric values.");
      exit(EXIT_FAILURE);
    }
  }
  skip_non_alphanum(fh);
}

//parses the normal vector from csv
void parse_normal(FILE *fh, Object *obj) {
  char *str = (char *) malloc(sizeof(char) * 6);
  char *character = malloc(sizeof(char));
  int check = 0;
  char *tmp = malloc(100);
  sprintf(character, "%c", fgetc(fh));
  while (*character != 58) {
    strcat(str, character);
    sprintf(character, "%c", fgetc(fh));
  }
  skip_non_alphanum(fh);
  if (strcmp(str, "normal") == 0) {
    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->normal.x);
    } else {
      perror("Error: Normal values must be of numeric type. Please try again with numeric values.");
      exit(EXIT_FAILURE);
    }

    skip_non_alphanum(fh);
    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->normal.y);
    } else {
      perror("Error: Normal values must be of numeric type. Please try again with numeric values.");
      exit(EXIT_FAILURE);
    }

    skip_non_alphanum(fh);
    fscanf(fh, "%[a-z | A-Z | 0-9 | . | -]", tmp);
    check = check_str(fh, tmp);
    if (check == 1) {
      sscanf(tmp, "%lf", &obj->normal.z);
    } else {
      perror("Error: Normal values must be of numeric type. Please try again with numeric values.");
      exit(EXIT_FAILURE);
    }
  }
  skip_non_alphanum(fh);
}

//comprehensive parser that combines all parser helper functions
Object* parse_csv(FILE *fh, Object *object, char character) {
  char new_char;
  if (character != EOF) {
    Object *object_next = malloc(sizeof(Object));
    parse_type(fh, object);
    printf("%s\n", object->kind);
    if (strcmp(object->kind, "CAMERA") == 0) {
      parse_field(fh, object);
      parse_field(fh, object);
      printf("%lf, %lf\n", object->width, object->height);
    } else if (strcmp(object->kind, "SPHERE") == 0) {
      parse_radius(fh, object);
      parse_color(fh, object);
      parse_color(fh, object);
      parse_color(fh, object);
      parse_position(fh, object);
      printf("%lf\n%lf, %lf, %lf\n", object->radius, object->diffuse_color.r, object->diffuse_color.g, object->diffuse_color.b);
      printf("%lf, %lf, %lf\n%lf, %lf, %lf\n", object->specular_color.r, object->specular_color.g, object->specular_color.b, object->position.x, object->position.y, object->position.z);
    } else if (strcmp(object->kind, "PLANE") == 0) {
      parse_normal(fh, object);
      parse_color(fh, object);
      parse_color(fh, object);
      parse_color(fh, object);
      parse_position(fh, object);
      printf("%lf, %lf, %lf\n%lf, %lf, %lf\n", object->normal.x, object->normal.y, object->normal.z, object->diffuse_color.r, object->diffuse_color.g, object->diffuse_color.b);
      printf("%lf, %lf, %lf\n", object->position.x, object->position.y, object->position.z);
    } else if (strcmp(object->kind, "LIGHT") == 0) {
      parse_color(fh, object);
      parse_theta(fh, object);
      parse_radial(fh, object);
      parse_radial(fh, object);
      parse_radial(fh, object);
      parse_position(fh, object);
      printf("%lf, %lf %lf\n", object->color.r, object->color.g, object->color.b);
      printf("%lf\n%lf, %lf, %lf\n%lf, %lf, %lf\n", object->theta, object->radial.a2, object->radial.a1, object->radial.a0, object->position.x, object->position.y, object->position.z);
    }
    skip_non_alphanum(fh);
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
  if (character == EOF) {
    ungetc(character, fh);
    return;
  }

  //compares character to all non-alphanum characters and cycles through them until an alphanum character is found
  while ((character < '0' && character != '-') || (character > '9' && character < 'A') || (character > 'Z' && character < 'a') || (character > 'z')) {
    if (character == EOF) {
      ungetc(character, fh);
      return;
    }
    character = fgetc(fh);
  }
  //replaces alphanum character that was pulled from the file
  ungetc(character, fh);
}

//rewinds linked list and returns first object in list recursively
Object* rewind_linked_object_list(Object *object) {
  if (object->prev != NULL) {
    rewind_linked_object_list(object->prev);
  } else {
    return object;
  }
}

//returns camera width recursively
double get_width(Object *object) {
  if (strcmp(object->kind, "CAMERA") == 0) {
    return object->width;
  } else if (object->next == NULL) {
    perror("Error: No camera found in input file. Please use an input file containing a camera object.");
    exit(EXIT_FAILURE);
  } else {
    get_width(object->next);
  }
}

//returns camera height recursively
double get_height(Object *object) {
  if (strcmp(object->kind, "CAMERA") == 0) {
    return object->height;
  } else if (object->next == NULL) {
    perror("Error: No camera found in input file. Please use an input file containing a camera object.");
    exit(EXIT_FAILURE);
  } else {
    get_height(object->next);
  }
}

//converts argv[x] to a double value recursively
double argv_to_double(char const *str, int index, double result) {
  if (index == strlen(str)) {
    return result;
  } else {
    double updated_result = result;
    char character = str[index];
    if (character > 57 || character < 48) {
      perror("Error: Width and height must be numeric values only. Please retry with different width and height inputs.");
      exit(EXIT_FAILURE);
    }

    //multiply result by 10 to shift values left
    updated_result *= 10;
    //character - '0' returns numerical value of any number 0-9
    updated_result += character - '0';
    argv_to_double(str, index + 1, updated_result);
  }
}

//MARK: -CALCULATIONS

//calculates sphere intersections and returns closest one that is in front of the camera
void intersection_sphere(Vector3 *Rd, Vector3 *Ro, double Cx, double Cy, double Cz, double radius, Vector3 *result) {
  double b, c, t0, t1, x, y, z;
  Vector3 *subtracted = malloc(sizeof(Vector3));
  v3dm_assign(Cx, Cy, Cz, subtracted);
  v3dm_subtract(Ro, subtracted, subtracted);
  b = 2 * ((Rd->x * subtracted->x) + (Rd->y * subtracted->y) + (Rd->z * subtracted->z));
  c = (subtracted->x * subtracted->x) + (subtracted->y * subtracted->y) + (subtracted->z * subtracted->z) - (radius * radius);
  t0 = (-b - (sqrt((b*b) - (4*c))))/2;
  t1 =(-b + (sqrt((b*b) - (4*c))))/2;
  if (!isnan(t0) && !isnan(t1) && !isinf(t0) && !isinf(t1)) {
    if (t0 <= 0 && t1 <= 0) {
      if (t0 > t1) {
        v3dm_scale(Rd, t0, result);
        v3dm_add(Ro, result, result);
      } else if (t1 >= t0) {
        v3dm_scale(Rd, t1, result);
        v3dm_add(Ro, result, result);
      }
    } else if (t1 > 0) {
      v3dm_scale(Rd, t0, result);
      v3dm_add(Ro, result, result);
    } else if (t0 > 0) {
      v3dm_scale(Rd, t0, result);
      v3dm_add(Ro, result, result);
    }
  } else {
    v3dm_assign(NAN, NAN, NAN, result);
  }
}

//calculates plane intersection and returns closest intersection in front of the camera
void intersection_plane(Vector3 *Ro, Vector3 *Rd, struct Vector3 norm, struct Vector3 point, Vector3 *result) {
  double dotRo, dotRd, d, t;
  Vector3 *tmp;
  d = v3dm_pointToPlaneDistance(norm, point, Ro);
  if (!isnan(d) && d > 0) {
    dotRo = v3dm_dot(&norm, Ro);
    dotRd = v3dm_dot(&norm, Rd);
    if (dotRd != 0) {
      t = -(dotRo + d) / (dotRd);
      if (t > 0) {
        v3dm_scale(Rd, t, result);
        v3dm_add(Ro, result, result);
        return;
      }
    }
  }
  v3dm_assign(NAN, NAN, NAN, result);
}

//returns color of closest object in front of the camera, or black if no object is closest in front of the camera
Color* castARay_non(Object *object, Vector3 *Ro, Vector3 *Rd) {
  Vector3 *intersection;
  double closest_distance, distance;
  closest_distance = NAN;
  Color *color;
  intersection = malloc(sizeof(Vector3));
  v3dm_assign(NAN, NAN, NAN, intersection);
  for (int i = 0; object->kind != NULL; i++) {
    if (strcmp(object->kind, "CAMERA") != 0) {
      if (strcmp(object->kind, "SPHERE") == 0) {
        intersection_sphere(Rd, Ro, object->position.x, object->position.y, object->position.z, object->radius, intersection);

      } else if (strcmp(object->kind, "PLANE") == 0) {
        intersection_plane(Ro, Rd, object->normal, object->position, intersection);
      }

      if (!isnan(intersection->x)) {
        distance = v3dm_pointToPointDistance(Ro, intersection);
        if (isnan(closest_distance)) {
          closest_distance = distance;
          color = &object->color;
        } else  if (distance < closest_distance) {
          closest_distance = distance;
          color = &object->color;
        }
      }
    }
    object = object->next;
  }
  return color;
}

//grabs color values of closest objects in front of camera and writes them to an array to return when complete
int* render(double width, double height, double xRes, double yRes, Object *object) {
  double x, y;
  int *colors;
  Vector3 *Pij, *Rd, *Ro;
  Color *color;
  Ro = malloc(sizeof(struct Vector3));
  Pij = malloc(sizeof(Vector3));
  Rd = malloc(sizeof(Vector3));
  color = malloc(sizeof(Color));
  colors = malloc(sizeof(int) * xRes * yRes * 3 * 4);
  v3dm_assign(0, 0, 0, Ro);
  int counter = 0;
  for (int i = 0; i < xRes; i++) {
    x = -(width/2) + (i * (width/xRes)) + (0.5 * (width/xRes));
    for (int j = 0; j < yRes; j++) {
      y = -(height/2) + (j * (height/yRes)) + (0.5 * (width/yRes));
      v3dm_assign(x, y, -1, Pij);
      v3dm_add(Pij, Ro, Rd);
      v3dm_unit(Rd, Rd);
      color = castARay_non(object, Ro, Rd);
      //print_color(*color);
      colors[counter] = (int) color->r;
      counter += (int) sizeof(int);
      colors[counter] = (int) color->g;
      counter += (int) sizeof(int);
      colors[counter] = (int) color->b;
      counter += (int) sizeof(int);
    }
  }
  return colors;
}

//writes all data in the colors array to a file
void write_to_file(FILE *fh, int *array, int xRes, int yRes) {
  for (int i = 0; i < sizeof(int) * xRes * yRes * 3; i+= 4) {
    fprintf(fh, "%d\n", (int) array[i]);
  }
}

//checks the extension on a file against a passed value
int checkExtension(char *fileName, char *extension) {
  char character, ext_char;
  for (int i = 1; i < strlen(extension); i++) {
    ext_char = extension[strlen(extension) - i];
    character = fileName[strlen(fileName) - i];
    if (!(character == ext_char)) {
      return -1;
    }
  }
}

//finds all lights, classifies them, and then returns a linked list of lights recursively
Light* find_lights(Object *obj, Light *light) {
  Light *new_light = malloc(sizeof())
  if (strcmp(obj->kind, "LIGHT")) {
    new_light->position = obj->position;
    new_light->direction = obj->direction;
    new_light->color = obj->color;
    new_light->radial = obj->radial;
    new_light->theta = obj->theta;
    new_light->angular = obj->angular;
    light->next = new_light;
    new_light->prev = light;
    if (new_light->theta == NULL || new_light->theta == 0) {
      new_light->kind = "POINT";
    } else if (angular != 0) {
      new_light->kind = "SPOT";
    }
    if (obj->next == NULL) {
      return new_light;
    }
  } else if (obj->next == NULL && light->kind == NULL) {
    perror("Error: No lighting data found in input file. Please try again with a light source in the input file.");
    exit(EXIT_FAILURE);
  } else if (obj->next == NULL && light->kind != NULL) {
    return light;
  } else {
    find_lights(obj->next, light);
  }
}

//MARK: -MAIN FUNCTION
int main(int argc, char const *argv[]) {
  //initial check variables
  int test_index = 4;
  char *test_str;
  if (sizeof(argv[3]) >= sizeof(argv[4])) {
    test_str = (char *) malloc(sizeof(argv[3]));
  } else {
    test_str = (char *) malloc(sizeof(argv[4]));
  }
  test_str = (char *) argv[3];
  char *test_char;
  char *ext = (char *) malloc(sizeof(char) * 4);

  //initial checks
  if (argc != 5) {
    perror("Error: This program requires 4 arguments. Please retry with the correct number of arguments");
    exit(EXIT_FAILURE);
  }

  if (!fopen(argv[3], "r")) {
    perror("Error: Please select a different input file, as this one does not exist.");
    exit(EXIT_FAILURE);
  }

  if (checkExtension(test_str, ".csv") == -1) {
    perror("Error: Please choose a new input file with a .csv extension.");
    exit(EXIT_FAILURE);
  }
  test_str = (char *) argv[4];
  if (checkExtension(test_str, ".ppm") == -1) {
    perror("Error: Please choose a new output file with a .ppm extension.");
    exit(EXIT_FAILURE);
  }

  //variables for setup later
  FILE *input_file, *output_file;
  char character;
  double width, height, xRes, yRes;
  double index = 0;
  int *colors;

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

  //real world width & height values
  width = get_width(result_object);
  height = get_height(result_object);

  //rewinding linked list to be reused
  result_object = rewind_linked_object_list(result_object);

  //allocating memory for array to accept all color values
  colors = malloc(xRes * yRes * sizeof(double) * 3);
  colors = render(width, height, xRes, yRes, result_object);

  //setting up image file
  output_file = fopen(argv[4], "w");
  fprintf(output_file, "%s%d\n%s\n%d\n%d\n%d\n", "P", 3, "#superfluous comment", (int) xRes, (int) yRes, 255);

  //writing to image file
  write_to_file(output_file, colors, xRes, yRes);
  fclose(output_file);
  //(output_file);

  return 0;
}
