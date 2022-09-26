#ifndef _CORE_H_
#define _CORE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <ctype.h>
#include <algorithm>
#define EPSILON 1e-4
#define GRAV_COE 9.8f
#define DEFAULT_AIRRESIST 0.4f
#define DEFAULT_RADIUS 1.0f
#define DEFAULT_ELASTIC 1.0f
#define DEFAULT_FRICTION 0.1f
#define DEFAULT_GRAVMULT 1.0f
#define DEFAULT_MASS 1.0f
#define TOLERANCE 0.0f

#endif