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
  free(character);
  skip_non_alphanum(fh);
}

//moves pointer in file back str(len) positions
void rewind_file(FILE *fh, char *str) {
  fseek(fh, -strlen(str), SEEK_CUR);
}

//checks the string to see if it is alphanum or not
int check_str(FILE *fh, char *str) {
  char *c = malloc(sizeof(char));
  for (int i = 0; i < strlen(str); i++) {
    *c = str[i];
    if ((*c < '0' || *c > '9') && (*c != '.' && *c != '-')) {
      return 0;
    }
  }
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
  free(character);
  free(tmp);
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
  free(character);
  free(tmp);
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
      if (obj->color.r < 0.0) {
        perror("Error: Color values must be greater than 0.0. Please retry with correct color values.");
        exit(EXIT_FAILURE);
      } else {
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
      if (obj->color.g < 0.0) {
        perror("Error: Color values may only range from 0.0 to 1.0. Please retry with the correct color values.");
        exit(EXIT_FAILURE);
      } else {
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
      if (obj->color.b < 0.0) {
        perror("Error: Color values may only range from 0.0 to 1.0. Please retry with the correct color values.");
        exit(EXIT_FAILURE);
      } else {
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

      if (obj->diffuse_color.r < 0.0) {
        perror("Error: diffuse_color values may only range from 0.0 to 1.0. Please retry with correct diffuse_color values.");
        exit(EXIT_FAILURE);
      } else {
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
      if (obj->diffuse_color.g < 0.0) {
        perror("Error: diffuse_color values may only range from 0.0 to 1.0. Please retry with the correct diffuse_color values.");
        exit(EXIT_FAILURE);
      } else {
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
      if (obj->diffuse_color.b < 0.0) {
        perror("Error: diffuse_color values may only range from 0.0 to 1.0. Please retry with the correct diffuse_color values.");
        exit(EXIT_FAILURE);
      } else {
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
      if (obj->specular_color.r < 0.0) {
        perror("Error: specular_color values may only range from 0.0 to 1.0. Please retry with correct specular_color values.");
        exit(EXIT_FAILURE);
      } else {
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
      if (obj->specular_color.g < 0.0) {
        perror("Error: specular_color values may only range from 0.0 to 1.0. Please retry with the correct specular_color values.");
        exit(EXIT_FAILURE);
      } else {
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
      if (obj->specular_color.b < 0.0) {
        perror("Error: specular_color values may only range from 0.0 to 1.0. Please retry with the correct specular_color values.");
        exit(EXIT_FAILURE);
      } else {
        skip_non_alphanum(fh);
      }
    } else {
      perror("Error: specular_color values must be of numeric format. Please retry with numeric values.");
      exit(EXIT_FAILURE);
    }
  } else {
    rewind_file(fh, str);
  }
  free(character);
  free(tmp);
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
      double d;
      sscanf(tmp, "%lf", &d);
      obj->theta = d;
    } else {
      perror("Error: Theta values must be of nuermic format. Please try again with numeric values.");
      exit(EXIT_FAILURE);
    }
  } else {
    obj->theta = 0;
    rewind_file(fh, str);
  }
  free(character);
  free(tmp);
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
  free(character);
  free(tmp);
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
  free(character);
  free(tmp);
  skip_non_alphanum(fh);
}

//TODO: PLANE specular_color POTENTIALLY WRONG
//comprehensive parser that combines all parser helper functions
Scene* parse_csv(FILE *fh, Scene *scene, char *character, Object *objects, Light *lights, int *obj_size, int *l_size) {
  int l_counter = 0;
  int obj_counter = 0;
  Object *new_obj = malloc(sizeof(Object));
  char *str = malloc(100);
  while (*character != EOF) {
    parse_type(fh, new_obj);
    if (strcmp(new_obj->kind, "CAMERA") == 0) {
      parse_field(fh, new_obj);
      parse_field(fh, new_obj);
      skip_non_alphanum(fh);
    } else if (strcmp(new_obj->kind, "SPHERE") == 0) {
      while (strcmp(str, "light") != 0 && strcmp(str, "plane") != 0 && strcmp(str, "camera") != 0 && *character != EOF) {
        sprintf(character, "%c", fgetc(fh));
        strcat(str, character);
        if (strcmp(str, "radius") == 0) {
          rewind_file(fh, str);
          strcpy(str, "");
          parse_radius(fh, new_obj);
        } else if (strcmp(str, "color") == 0) {
          rewind_file(fh, str);
          strcpy(str, "");
          parse_color(fh, new_obj);
        } else if (strcmp(str, "diffuse_color") == 0) {
          rewind_file(fh, str);
          strcpy(str, "");
          parse_color(fh, new_obj);
        } else if (strcmp(str, "specular_color") == 0) {
          rewind_file(fh, str);
          strcpy(str, "");
          parse_color(fh, new_obj);
        } else if (strcmp(str, "position") == 0) {
          rewind_file(fh, str);
          strcpy(str, "");
          parse_position(fh, new_obj);
        }
      }
      if (*character != EOF && strcmp(new_obj->kind, "SPHERE") == 0) {
        rewind_file(fh, str);
        strcpy(str, "");
        /*printf("%s, r: %f\ncr: %f, cg: %f, cb: %f\ndcr: %f, dcg: %f, dcb: %f\nscr: %f, scg: %f, scb: %f\nx: %f, y: %f, z: %f\n", new_obj->kind, new_obj->radius,
        new_obj->color.r, new_obj->color.g, new_obj->color.b, new_obj->diffuse_color.r, new_obj->diffuse_color.g, new_obj->diffuse_color.b, new_obj->specular_color.r,
        new_obj->specular_color.g, new_obj->specular_color.b, new_obj->position.x, new_obj->position.y, new_obj->position.z);*/
      }
    } else if (strcmp(new_obj->kind, "PLANE") == 0) {
      while (strcmp(str, "light") != 0 && strcmp(str, "sphere") != 0 && strcmp(str, "camera") != 0 && *character != EOF) {
        sprintf(character, "%c", fgetc(fh));
        strcat(str, character);
        if (strcmp(str, "normal") == 0) {
          rewind_file(fh, str);
          strcpy(str, "");
          parse_normal(fh, new_obj);
        } else if (strcmp(str, "color") == 0) {
          rewind_file(fh, str);
          strcpy(str, "");
          parse_color(fh, new_obj);
        } else if (strcmp(str, "diffuse_color") == 0) {
          rewind_file(fh, str);
          strcpy(str, "");
          parse_color(fh, new_obj);
        } else if (strcmp(str, "specular_color") == 0) {
          rewind_file(fh, str);
          strcpy(str, "");
          parse_color(fh, new_obj);
        } else if (strcmp(str, "position") == 0) {
          rewind_file(fh, str);
          strcpy(str, "");
          parse_position(fh, new_obj);
        }
      }
      if (*character != EOF && strcmp(new_obj->kind, "PLANE") == 0) {
        rewind_file(fh, str);
        strcpy(str, "");
        /*printf("%s, nx: %f, ny: %f, nz: %f\ncr: %f, cg: %f, cb: %f\ndcr: %f, dcg: %f, dcb: %f\nscr: %f, scg: %f, scb: %f\nx: %f, y: %f, z: %f\n",
        new_obj->kind, new_obj->normal.x, new_obj->normal.y, new_obj->normal.z, new_obj->color.r, new_obj->color.g, new_obj->color.b, new_obj->diffuse_color.r,
        new_obj->diffuse_color.g, new_obj->diffuse_color.b, new_obj->specular_color.r, new_obj->specular_color.g, new_obj->specular_color.b, new_obj->position.x,
        new_obj->position.y, new_obj->position.z);*/
      }
    } else if (strcmp(new_obj->kind, "LIGHT") == 0) {
      Light *new_light = malloc(sizeof(Light));
      while (strcmp(str, "camera") != 0 && strcmp(str, "plane") != 0 && strcmp(str, "sphere") != 0 && *character != EOF) {
        sprintf(character, "%c", fgetc(fh));
        strcat(str, character);
        if (strcmp(str, "theta") == 0) {
          rewind_file(fh, str);
          strcpy(str, "");
          parse_theta(fh, new_obj);;
        } else if (strcmp(str, "color") == 0) {
          rewind_file(fh, str);
          strcpy(str, "");
          parse_color(fh, new_obj);
        } else if (strcmp(str, "radial-a2") == 0) {
          rewind_file(fh, str);
          strcpy(str, "");
          parse_radial(fh, new_obj);
        } else if (strcmp(str, "radial-a1") == 0) {
          rewind_file(fh, str);
          strcpy(str, "");
          parse_radial(fh, new_obj);
        } else if (strcmp(str, "radial-a0") == 0) {
          rewind_file(fh, str);
          strcpy(str, "");
          parse_radial(fh, new_obj);
        } else if (strcmp(str, "position") == 0) {
          rewind_file(fh, str);
          strcpy(str, "");
          parse_position(fh, new_obj);
        }
      }
      if (*character != EOF && strcmp(new_obj->kind, "LIGHT") == 0) {
        rewind_file(fh, str);
        strcpy(str, "");
        /*printf("%s, cr: %f, cg: %f, cb: %f\na2: %f, a1: 5f, a0: %f\nth: %f, ang: %f\ndirx: %f, diry: %f, dirz: %f\n",
        new_obj->kind, new_obj->color.r, new_obj->color.g, new_obj->color.b, new_obj->radial.a2, new_obj->radial.a1, new_obj->radial.a0,
        new_obj->theta, new_obj->direction.x, new_obj->direction.y, new_obj->direction.z);*/
      } else if (*character == EOF) {
        rewind_file(fh, character);
        strcpy(str, "");
      }

      new_light->position = new_obj->position;
      new_light->color = new_obj->color;
      new_light->radial.a0 = new_obj->radial.a0;
      new_light->radial.a1 = new_obj->radial.a1;
      new_light->radial.a2 = new_obj->radial.a2;
      new_light->theta = new_obj->theta;
      new_light->angular = new_obj->angular;
      new_light->direction = new_obj->direction;

      if (new_light->theta == 0.0) {
        new_light->kind = "POINT";
      } else {
        new_light->kind = "SPOT";
      }
      lights[l_counter] = *new_light;
      l_counter += 1;
    }
    skip_non_alphanum(fh);
    if (!(strcmp(new_obj->kind, "LIGHT") == 0)) {
      objects[obj_counter] = *new_obj;
      obj_counter += 1;
    }
    //sprintf(character, "%c", fgetc(fh));
    //ungetc(*character, fh);
  }
  *obj_size = obj_counter;
  *l_size = l_counter;
  return scene;
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
/*Scene* rewind_scene(Scene *scene) {
  if (scene->object->prev->kind != NULL && scene->light->prev->kind != NULL) {
    scene->object = scene->object->prev;
    scene->light = scene->light->prev;
    //print("%s, %s", scene->object->kind, scene->light->kind);
    rewind_scene(scene);
  } else if (scene->object->prev->kind != NULL) {
    scene->object = scene->object->prev;
    //printf("%s\n", scene->object->kind);
    rewind_scene(scene);
  } else if (scene->light->prev->kind != NULL){
    scene->light = scene->light->prev;
    rewind_scene(scene);
  } else {
    return scene;
  }
}*/

//returns camera width recursively
double get_width(Object *objects, int obj_size) {
  for(int i = 0; i < obj_size; i++) {
    if(strcmp(objects[i].kind, "CAMERA") == 0) {
      return objects[i].width;
    }
  }
  perror("Error: No camera found in input file. Please use an input file containing a camera object.");
  exit(EXIT_FAILURE);
}

//returns camera height recursively
double get_height(Object *objects, int obj_size) {
  for (int i = 0; i < obj_size; i++) {
    if(strcmp(objects[i].kind, "CAMERA") == 0) {
      return objects[i].height;
    }
  }
  perror("Error: No camera found in input file. Please use an input file containing a camera object.");
  exit(EXIT_FAILURE);
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

//clamps max value to 255.0 and minimum value to 0.0
void clamp(Color *color) {

  //clamping red
  if (color->r > 1.0) {
    color->r = 1.0;
  } else if (color->r < 0.0) {
    color->r = 0.0;
  } else if (isnan(color->r)) {
    color->r = 0.0;
  }

  //clamping green
  if (color->g > 1.0) {
    color->g = 1.0;
  } else if (color->g < 0.0) {
    color->g = 0.0;
  } else if (isnan(color->g)) {
    color->g = 0.0;
  }

  //clapming blue
  if (color->b > 1.0) {
    color->b = 1.0;
  } else if (color->b < 0.0) {
    color->b = 0.0;
  } else if (isnan(color->b)) {
    color->b = 0.0;
  }
}

//calculates sphere intersections and returns closest one that is in front of the camera
void intersection_sphere(Vector3 *Rd, Vector3 *Ro, double Cx, double Cy, double Cz, double radius, Vector3 *result) {
  double b, c, t0, t1, x, y, z;
  Vector3 _subtracted;
  Vector3 *subtracted = &_subtracted;
  v3dm_assign(Cx, Cy, Cz, subtracted);
  v3dm_subtract(Ro, subtracted, subtracted);
  b = 2 * ((Rd->x * subtracted->x) + (Rd->y * subtracted->y) + (Rd->z * subtracted->z));
  c = (subtracted->x * subtracted->x) + (subtracted->y * subtracted->y) + (subtracted->z * subtracted->z) - (radius * radius);
  t0 += (-b - (sqrt((b*b) - (4*c))))/2.0;
  t1 += (-b + (sqrt((b*b) - (4*c))))/2.0;
  if (!isnan(t0) && !isnan(t1) && !isinf(t0) && !isinf(t1)) {
    if (t0 >= 0.0 && t1 >= 0.0) {
      if (t0 < t1) {
        v3dm_scale(Rd, t0, result);
        v3dm_add(Ro, result, result);

      } else if (t1 <= t0) {
        v3dm_scale(Rd, t1, result);
        v3dm_add(Ro, result, result);

      }
    } else if (t1 <= 0.0) {
      v3dm_scale(Rd, t0, result);
      v3dm_add(Ro, result, result);

    } else if (t0 <= 0.0) {
      v3dm_scale(Rd, t1, result);
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

void castARay_primitive(Object *objects, Vector3 *Ro, Vector3 *Rd, int j, ObjectPlus *result, int obj_size) {
  double closest_distance, distance;
  closest_distance = NAN;
  Vector3 _closest_intersection;
  Vector3 *closest_intersection = &_closest_intersection;
  Color *color;
  Object object;
  //TODO: normal loops, plane first loop, weird data loop, repeat: found in castARay from casting a ray with new_object
  //printf("INNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN\n");
  for(int x = 0;  x < obj_size; x++) {
    object = objects[x];
    //printf("%s\n", object.kind);
    if (strcmp(object.kind, "CAMERA") != 0) {
      //printf("position: %f, %f, %f\n", object.position.x, object.position.y, object.position.z);
      if (strcmp(object.kind, "SPHERE") == 0) {
        //printf("1\n");
        intersection_sphere(Rd, Ro, object.position.x, object.position.y, object.position.z, object.radius, result->intersection);
        //printf("2\n");
      } else if (strcmp(object.kind, "PLANE") == 0) {
        //printf("3\n");
        intersection_plane(Ro, Rd, object.normal, object.position, result->intersection);
        //printf("4\n");
      }
      if (!isnan(result->intersection->x) && !isnan(result->intersection->y) && !isnan(result->intersection->z)) {
        //printf("5\n");
        distance = v3dm_pointToPointDistance(Ro, result->intersection);
        if (isnan(closest_distance)) {
          //printf("6\n");
          closest_distance = distance;
          v3dm_assign(result->intersection->x, result->intersection->y, result->intersection->z, closest_intersection);
          result->object = object;
          result->valid = 1;
        } else if (distance < closest_distance) {
          //printf("7\n");
          closest_distance = distance;
          v3dm_assign(result->intersection->x, result->intersection->y, result->intersection->z, closest_intersection);
          result->object = object;
          result->valid = 1;
        }
      }
      //printf("8\n");
    }
    //printf("9\n");
  }
  //TODO: CHANGE FINAL_COLOR
  //TODO: SPHERE NOT SHOWING UP IN IMAGE EVEN THOUGH REPORTED CLOSEST OBJ
  v3dm_assign(closest_intersection->x, closest_intersection->y, closest_intersection->z, result->intersection);
  //printf("10\n");
  return;
}

//returns color of closest object in front of the camera, or black if no object is closest in front of the camera
void castARay(Object *objects, Vector3 *Ro, Vector3 *Rd, Light *lights, int i, int j, Color *final_color, int l_size, int obj_size) {
  Vector3 _intersection;
  Vector3 *intersection = &_intersection;
  Vector3 _hit;
  Vector3 *hit = &_hit;
  ObjectPlus _result;
  ObjectPlus *result = &_result;
  Vector3 _Ro2;
  Vector3 *Ro2 = &_Ro2;
  Vector3 _Rd2;
  Vector3 *Rd2 = &_Rd2;
  Vector3 _v;
  Vector3 *v = &_v;
  Vector3 _r;
  Vector3 *r = &_r;
  Vector3 _l;
  Vector3 *l = &_l;
  Vector3 _n;
  Vector3 *n = &_n;
  Color _light_color;
  Color *light_color = &_light_color;
  Light light;
  struct Object new_objects[1];
  result->intersection = &_intersection;
  result->valid = 0;
  double epsilon;
  //result->object = NULL;
  double n_dot_l;

  for (int x = 0; x < l_size; x++) {
    light = lights[x];
    v3dm_unit(&light.direction, &light.direction);
    //printf("x: %d\n", x);
    castARay_primitive(objects, Ro, Rd, j, result, obj_size);
    //printf("result: %f, %f, %f\n", result->intersection->x, result->intersection->y, result->intersection->z);

    //no intersection found
    if (result->valid == 0) {
      //TODO: CHANGE BACK TO BLACK LATER, CURRENTLY DEBUG CODE TO FIGURE OUT WHAT'S GOING ON WITH SPHERE
      //HALF THE IMAGE IS COMING BACK ALL FUCKY LIKE THIS
      final_color->r = 0.5;
      final_color->g = 0.1;
      final_color->b = 0.1;
      return;
    }

    Ro2 = result->intersection;
    new_objects[0] = result->object;
    v3dm_subtract(Ro2, Ro, v);
    v3dm_unit(v, v);
    if (!isnan(Ro2->x) && !isnan(Ro2->y) && !isnan(Ro2->z)) {
      v3dm_subtract(&light.position, Ro2, l);
      v3dm_unit(l, l);

      if (!isnan(new_objects[0].normal.x) && !isnan(new_objects[0].normal.y) && !isnan(new_objects[0].normal.z)) {
        v3dm_reflect(l, &new_objects[0].normal, r);
        n_dot_l = v3dm_dot(l, &new_objects[0].normal);

      } else {
        epsilon = 0.1 * new_objects[0].radius;
        v3dm_subtract(Ro2, &new_objects[0].position, n);
        v3dm_unit(n, n);
        v3dm_reflect(l, n, r);
        v3dm_unit(r, r);
        n_dot_l = v3dm_dot(l, n);
      }
      //TODO: RD2 SOMETIMES NAN
      v3dm_subtract(&light.position, Ro2, Rd2);
      v3dm_unit(Rd2, Rd2);
      v3dm_unit(Ro2, Ro2);
      v3dm_assign(Ro2->x + epsilon, Ro2->y + epsilon, Ro2->z + epsilon, Ro2);
      //printf("Rd2: %f, %f, %f\n", Rd2->x, Rd2->y, Rd2->z);
      //TODO: LIGHTING SETUP

      castARay_primitive(new_objects, Ro2, Rd2, j, result, 1);
      //toggling light here maybe??
      if (result->valid == 1) {
        final_color->r = 0.0;
        final_color->g = 0.0;
        final_color->b = 0.0;
        return;
      }
    } else {
      final_color->r = 0.0;
      final_color->g = 0.0;
      final_color->b = 0.5;
      return;
    }

    light_color = &light.color;

    //TODO: toggling light for shadow


    //radial attenuation
    double dl = v3dm_pointToPointDistance(&light.position, intersection);
    double f_rad = 1.0;
    if (dl < 1000) {
      f_rad = 1/(light.radial.a2 * dl * dl + light.radial.a1 * dl + light.radial.a0);
    }
    //angular attenuation
    double f_ang = 1.0;
    Vector3 *vo = malloc(sizeof(Vector3));
    v3dm_subtract(intersection, &light.position, vo);
    v3dm_unit(vo, vo);
    if (strcmp(light.kind, "SPOT") == 0) {
      double vo_dot_vl = v3dm_dot(vo, &light.direction);
      double alpha = acos(vo_dot_vl);
      if (alpha > light.theta) {
        f_ang = 0.0;
      } else {
        f_ang = pow(vo_dot_vl, light.angular);
      }
    } else {
      f_ang = 1.0;
    }
    //calculate the diffuse reflection
    double Idiff_r, Idiff_g, Idiff_b;
    if (n_dot_l > 0.0) {
      //TODO: NEVER ENTERS HERE
      Idiff_r = new_objects[0].diffuse_color.r * light_color->r * (n_dot_l);
      Idiff_g = new_objects[0].diffuse_color.g * light_color->g * (n_dot_l);
      Idiff_b = new_objects[0].diffuse_color.b * light_color->b * (n_dot_l);
    } else {
      Idiff_r = 0.0;
      Idiff_g = 0.0;
      Idiff_b = 0.0;
    }

    //calculate the specular reflection
    double Ispec_r, Ispec_g, Ispec_b;
    if (v3dm_dot(v,r) > 0) {
      //TODO: NEVER ENTERS HERE
      Ispec_r = new_objects[0].specular_color.r * light_color->r * pow(v3dm_dot(v, r), 20);
      Ispec_g = new_objects[0].specular_color.g * light_color->g * pow(v3dm_dot(v, r), 20);
      Ispec_b = new_objects[0].specular_color.b * light_color->b * pow(v3dm_dot(v, r), 20);
      printf("color: %f, %f, %f\n", new_objects[0].diffuse_color.r, new_objects[0].diffuse_color.g, new_objects[0].diffuse_color.b);
    } else {
      Ispec_r = 0.0;
      Ispec_g = 0.0;
      Ispec_b = 0.0;
    }

    final_color->r += f_ang *f_rad * (Idiff_r+Ispec_r);
    final_color->g += f_ang *f_rad * (Idiff_g+Ispec_g);
    final_color->b += f_ang *f_rad * (Idiff_b+Ispec_b);
  }
  //final_color->r = new_objects[0].diffuse_color.r;
  //final_color->g = new_objects[0].diffuse_color.g;
  //final_color->b = new_objects[0].diffuse_color.b;
  return;
}

//grabs color values of closest objects in front of camera and writes them to an array to return when complete
int* render(double width, double height, double xRes, double yRes, Scene *scene, int l_size, int obj_size) {
  double x, y;
  int *colors;
  Vector3 *Pij, *Rd, *Ro;
  Color _color;
  Color *color = &_color;
  Ro = malloc(sizeof(Vector3));
  Pij = malloc(sizeof(Vector3));
  Rd = malloc(sizeof(Vector3));
  color = malloc(sizeof(Color));
  colors = malloc(sizeof(int) * xRes * yRes * 3);
  v3dm_assign(0, 0, 0, Ro);
  int counter = 0;
  for (int j = 0; j < yRes; j++) {
    y =  (height/2) - (j * (height/yRes)) - (0.5 * (height/yRes));
    //printf("%d\n", j);
    for (int i = 0; i < xRes; i++) {
      x = -(width/2) + (i * (width/xRes)) + (0.5 * (width/xRes));
      //TODO: FIND OUT WHY THIS WORKS MAYBE IF I HAVE TIME; x = y, y = -x was solution
      v3dm_assign(x, y, -1, Pij);
      v3dm_add(Pij, Ro, Rd);
      v3dm_unit(Rd, Rd);

      //TODO: RD IS SOMETIMES NAN.
      //TODO: SPHERE INTERSECTIONS AREN'T WORKING AND HALF OF IMAGE IS NOT USED
      //here
      castARay(scene->objects, Ro, Rd, scene->lights, i, j, color, l_size, obj_size);
      clamp(color);
      colors[counter] = (int) (color->r * 255);
      counter++;
      colors[counter] = (int) (color->g * 255);
      counter++;
      colors[counter] = (int) (color->b * 255);
      counter++;
    }
  }
  return colors;
}

//writes all data in the colors array to a file
void write_to_file(FILE *fh, int *array, int xRes, int yRes) {
  for (int i = 0; i < xRes * yRes * 3; i++) {
    fprintf(fh, "%d\n", array[i]);
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

//MARK: -MAIN FUNCTION
int main(int argc, char const *argv[]) {
  printf("PROGRAM START\nINITIAL CONDITIONS CHECK\n");

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

  printf("CHECKS PASSED\nSETTING UP VARIABLES\n");

  //variables for setup later
  FILE *input_file, *output_file;
  char *character = malloc(sizeof(char));
  double width, height, xRes, yRes;
  double index = 0;
  int *colors;

  //setting up some variables
  int l_size, obj_size;
  Object *object = malloc(sizeof(Object));
  struct Object objects[128];
  struct Light lights[128];
  Scene *scene = malloc(sizeof(Scene));
  Light *light = malloc(sizeof(Scene));
  Object *result_object = malloc(sizeof(Object));
  input_file = fopen(argv[3], "r");
  sprintf(character, "%c", fgetc(input_file));

  xRes = argv_to_double(argv[1], index, xRes);
  yRes = argv_to_double(argv[2], index, yRes);
  scene->objects = objects;
  scene->lights = lights;


  //replacing character in file for parser to work correctly
  ungetc(*character, input_file);

  printf("VARIABLE SETUP COMPLETE\n");

  printf("PARSING FILE\n");

  //parsing to return object
  scene = parse_csv(input_file, scene, character, objects, lights, &obj_size, &l_size);
  fclose(input_file);

  printf("FILE PARSED\n");

  //rewinding linked_list then getting width & height from camera and rewinding again
  //scene = rewind_scene(scene);

  //real world width & height values
  width = get_width(scene->objects, obj_size);
  height = get_height(scene->objects, obj_size);

  //rewinding linked list to be reused
  //scene = rewind_scene(scene);

  printf("RENDERING\n");

  //allocating memory for array to accept all color values
  colors = malloc(xRes * yRes * sizeof(double) * 3);
  colors = render(width, height, xRes, yRes, scene, l_size, obj_size);

  printf("RENDER COMPLETE\n");

  printf("WRITING TO FILE\n");
  //setting up image file
  output_file = fopen(argv[4], "w");
  fprintf(output_file, "%s%d\n%s\n%d\n%d\n%d\n", "P", 3, "#superfluous comment", (int) xRes, (int) yRes, 255);

  printf("WRITE COMPLETE\n");

  //writing to image file
  write_to_file(output_file, colors, xRes, yRes);
  fclose(output_file);

  printf("PROGRAM FINISHED RUNNING\n");

  return 0;
}
