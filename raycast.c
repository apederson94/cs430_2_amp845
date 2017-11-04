#include "v3dm_lib.h"
#include "structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void skip_non_alphanum();
void intersection_sphere();

//TODO: CLAMP AT THE END, NOT THE BEGINNING.

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
  printf("%s\n", str);
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
      if (obj->color.r < 0.0) {
        perror("Error: Color values must be greater than 0.0. Please retry with correct color values.");
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
      if (obj->color.g < 0.0) {
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
      if (obj->color.b < 0.0) {
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

      if (obj->diffuse_color.r < 0.0) {
        perror("Error: diffuse_color values may only range from 0.0 to 1.0. Please retry with correct diffuse_color values.");
        exit(EXIT_FAILURE);
      } else {
        if (obj->diffuse_color.r > 1.0) {
          obj->diffuse_color.r = 1.0;
        }
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
      if (obj->diffuse_color.g < 0.0) {
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
      if (obj->diffuse_color.b < 0.0) {
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
      if (obj->specular_color.r < 0.0) {
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
      if (obj->specular_color.g < 0.0) {
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
      if (obj->specular_color.b < 0.0) {
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
Scene* parse_csv(FILE *fh, Scene *scene, char *character) {
  Object *new_obj = malloc(sizeof(Object));
  char *new_char = malloc(sizeof(char));
  char *str = malloc(10);
  if (*character != EOF) {
    Object *object_next = malloc(sizeof(Object));
    parse_type(fh, new_obj);
    if (strcmp(new_obj->kind, "CAMERA") == 0) {
      parse_field(fh, new_obj);
      parse_field(fh, new_obj);
      printf("%f, %f\n", new_obj->width, new_obj->height);
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
      printf("%f\n%f, %f, %f\n", new_obj->radius, new_obj->diffuse_color.r, new_obj->diffuse_color.g, new_obj->diffuse_color.b);
      printf("%f, %f, %f\n%f, %f, %f\n", new_obj->specular_color.r, new_obj->specular_color.g, new_obj->specular_color.b, new_obj->position.x, new_obj->position.y, new_obj->position.z);
      rewind_file(fh, str);
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
      printf("%f, %f, %f\n%f, %f, %f\n", new_obj->normal.x, new_obj->normal.y, new_obj->normal.z, new_obj->diffuse_color.r, new_obj->diffuse_color.g, new_obj->diffuse_color.b);
      printf("%f, %f, %f\n", new_obj->position.x, new_obj->position.y, new_obj->position.z);
      rewind_file(fh, str);
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

      new_light->color = new_obj->color;
      new_light->theta = new_obj->theta;
      new_light->radial.a2 = new_obj->radial.a2;
      new_light->radial.a1 = new_obj->radial.a2;
      new_light->radial.a0 = new_obj->radial.a0;
      new_light->position = new_obj->position;
      scene->light->next = new_light;
      new_light->prev = scene->light;
      scene->light = new_light;

      printf("%f, %f, %f\n", new_obj->color.r, new_obj->color.g, new_obj->color.b);
      printf("%f\n%f, %f, %f\n%f, %f, %f\n", new_light->theta, new_obj->radial.a2, new_obj->radial.a1, new_obj->radial.a0, new_obj->position.x, new_obj->position.y, new_obj->position.z);
    }
    skip_non_alphanum(fh);
    if (!(strcmp(new_obj->kind, "LIGHT") == 0)) {
      new_obj->prev = scene->object;
      scene->object->next = new_obj;
      scene->object = new_obj;
    }
    sprintf(new_char, "%c", fgetc(fh));
    ungetc(*new_char, fh);
    parse_csv(fh, scene, new_char);
  } else {
    return scene;
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
Scene* rewind_scene(Scene *scene) {
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

//clamps max value to 255.0 and minimum value to 0.0
void clamp(Color *color) {

  //clamping red
  if (color->r > 255.0) {
    color->r = 255;
  } else if (color->r < 0.0) {
    color->r = 0.0;
  }

  //clamping green
  if (color->g > 255.0) {
    color->g = 255;
  } else if (color->g < 0.0) {
    color->g = 0.0;
  }

  //clapming blue
  if (color->b > 255.0) {
    color->b = 255;
  } else if (color->b < 0.0) {
    color->b = 0.0;
  }
}

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

ObjectPlus* castARay_primitive(Object *object, Vector3 *Ro, Vector3 *Rd) {
  Vector3 *intersection = malloc(sizeof(Vector3));
  ObjectPlus *result = malloc(sizeof(ObjectPlus));
  double closest_distance, distance;
  closest_distance = NAN;
  Vector3 *closest_intersection = malloc(sizeof(Vector3));
  Color *color;
  intersection = malloc(sizeof(Vector3));
  v3dm_assign(NAN, NAN, NAN, intersection);
  while (object->kind != NULL) {
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
          closest_intersection = intersection;
          result->object = object;
          result->intersection = intersection;
        } else  if (distance < closest_distance) {
          closest_distance = distance;
          closest_intersection = intersection;
          result->object = object;
          result->intersection = intersection;
        }
      }
    }
    object = object->next;
  }
  return result;
}

//returns color of closest object in front of the camera, or black if no object is closest in front of the camera
Color* castARay(Object *object, Vector3 *Ro, Vector3 *Rd, Light *light) {
  Vector3 *intersection = malloc(sizeof(Vector3));
  Vector3 *hit = malloc(sizeof(Vector3));
  Color *final_color = malloc(sizeof(Color));
  ObjectPlus *result = malloc(sizeof(ObjectPlus));
  Vector3 *Ro2 = malloc(sizeof(Vector3));
  Vector3 *Rd2 = malloc(sizeof(Vector3));
  Vector3 *v = malloc(sizeof(Vector3));
  Vector3 *r = malloc(sizeof(Vector3));
  Vector3 *l = malloc(sizeof(Vector3));
  Vector3 *n = malloc(sizeof(Vector3));
  double n_dot_l;


  while (light != NULL) {
    result = castARay_primitive(object, Ro, Rd);
    Ro2 = result->intersection;
    object = result->object;
    v3dm_subtract(Ro2, Ro, v);
    v3dm_subtract(&light->position, Ro2, l);

    if (!isnan(object->normal.x) && !isnan(object->normal.y) && !isnan(object->normal.z)) {
      v3dm_reflect(l, &object->normal, r);
      n_dot_l = v3dm_dot(l, &object->normal);
    } else {
      v3dm_subtract(Ro2, &object->position, n);
      v3dm_reflect(l, n, r);
      n_dot_l = v3dm_dot(l, n);
    }

    v3dm_subtract(&light->position, intersection, Rd2);
    v3dm_unit(Rd2, Rd2);
    result = castARay_primitive(object, Ro2, Rd2);
    if (!isnan(result->intersection->x) && !isnan(result->intersection->y) && !isnan(result->intersection->z)) continue; //light source is OFF
    //light = light->next;


    //radial attenuation
    double dl = v3dm_pointToPointDistance(&light->position, intersection);
    double f_rad = 1.0;
    if (dl < INFINITY) {
      f_rad = 1/(light->radial.a2 * dl * dl + light->radial.a1 * dl + light->radial.a0);
    }

    //angular attenuation
    double f_ang = 1.0;
    Vector3 *vo = malloc(sizeof(Vector3));
    v3dm_subtract(intersection, &light->position, vo);
    if (strcmp(light->kind, "SPOT") == 0) {
      double vo_dot_vl = v3dm_dot(vo, &light->direction);
      double alpha = acos(vo_dot_vl);
      if (alpha > light->theta) {
        f_ang = 0.0;
      } else {
        f_ang = pow(vo_dot_vl, light->angular);
      }
    } else {
      f_ang = 1.0;
    }

    //calculate the diffuse reflection
    double Idiff_r, Idiff_g, Idiff_b;
    if (n_dot_l > 0.0) {
      Idiff_r = object->diffuse_color.r * light->color.r * (n_dot_l);
      Idiff_g = object->diffuse_color.g * light->color.g * (n_dot_l);
      Idiff_b = object->diffuse_color.b * light->color.b * (n_dot_l);
    } else {
      Idiff_r = 0.0;
      Idiff_g = 0.0;
      Idiff_b = 0.0;
    }


    //calculate the specular reflection
    double Ispec_r, Ispec_g, Ispec_b;
    if (v3dm_dot(v,r) > 0) {
      Ispec_r = object->specular_color.r * light->color.r * (v3dm_dot(v, r));
      Ispec_g = object->specular_color.g * light->color.g * (v3dm_dot(v, r));
      Ispec_b = object->specular_color.b * light->color.b * (v3dm_dot(v, r));
    } else {
      Ispec_r = 0.0;
      Ispec_g = 0.0;
      Ispec_b = 0.0;
    }


    final_color->r += f_ang *f_rad * (Idiff_r+Ispec_r);
    final_color->g += f_ang *f_rad * (Idiff_g+Ispec_g);
    final_color->b += f_ang *f_rad * (Idiff_b+Ispec_b);
    light = light->next;
  }

  if (final_color->r > 255.0) {
    final_color->r = 255.0;
  }
  if (final_color->g > 255.0) {
    final_color->g = 255.0;
  }
  if (final_color->b > 255.0) {
    final_color->b = 255.0;
  }

  return final_color;
}

//grabs color values of closest objects in front of camera and writes them to an array to return when complete
int* render(double width, double height, double xRes, double yRes, Scene *scene) {
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
      color = castARay(scene->object, Ro, Rd, scene->light);
      clamp(color);
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
  char *character = malloc(sizeof(char));
  double width, height, xRes, yRes;
  double index = 0;
  int *colors;

  //setting up some variables
  Object *object = malloc(sizeof(Object));
  Scene *scene = malloc(sizeof(Scene));
  Object *scene_obj = malloc(sizeof(Object));
  Light *scene_light = malloc(sizeof(Light));
  Light *light = malloc(sizeof(Scene));
  Object *result_object = malloc(sizeof(Object));
  input_file = fopen(argv[3], "r");
  sprintf(character, "%c", fgetc(input_file));

  xRes = argv_to_double(argv[1], index, xRes);
  yRes = argv_to_double(argv[2], index, yRes);
  scene->object = scene_obj;
  scene->light = scene_light;


  //replacing character in file for parser to work correctly
  ungetc(*character, input_file);

  //parsing to return object
  scene = parse_csv(input_file, scene, character);
  fclose(input_file);
  //rewinding linked_list then getting width & height from camera and rewinding again
  scene = rewind_scene(scene);

  //real world width & height values
  width = get_width(scene->object);
  height = get_height(scene->object);

  //rewinding linked list to be reused
  scene = rewind_scene(scene);

  //allocating memory for array to accept all color values
  colors = malloc(xRes * yRes * sizeof(double) * 3);
  colors = render(width, height, xRes, yRes, scene);


  //setting up image file
  output_file = fopen(argv[4], "w");
  fprintf(output_file, "%s%d\n%s\n%d\n%d\n%d\n", "P", 3, "#superfluous comment", (int) xRes, (int) yRes, 255);

  //writing to image file
  write_to_file(output_file, colors, xRes, yRes);
  fclose(output_file);
  //(output_file);

  return 0;
}
