#pragma once

#include <glm/glm.hpp>

#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#define GL(x) x;                                                               \
              {                                                                \
                  int gl_error = glGetError();                                 \
                  if (gl_error != GL_NO_ERROR) {                               \
                      fprintf(stderr, "OpenGL error: %d near '%s:%d'\n",       \
                              gl_error, __FILE__, __LINE__);                   \
                      exit(gl_error);                                          \
                  }                                                            \
              }
#else
#define GL(x) x
#endif

void sleep(int ms);

glm::vec3 arc_ball_mapping(glm::vec2 screen_vec, glm::ivec2 screen_size);
