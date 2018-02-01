#ifndef HEXC_H
#define HEXC_H

#include <Python.h>
#include <structmember.h>
#include <stdlib.h>

// #include "math_3d.h"

// declare Hex object structure
struct abstracthex_s;
struct hex_s;
struct slice_s;
struct stack_s;
struct grid_s;

struct hexringiter_s;
struct hexrangeiter_s;

typedef struct abstracthex_s	abstracthex_t;
typedef struct slice_s			slice_t;
typedef struct stack_s			stack_t;
typedef struct hex_s			hex_t;
typedef struct grid_s			grid_t;

typedef struct hexringiter_s	hexringiter_t;
typedef struct hexrangeiter_s	hexrangeiter_t;

struct abstracthex_s
{
	PyObject_HEAD
	int q;
	int r;
	int s;
};

struct hexringiter_s
{
	PyObject_HEAD
	int pos;
	int	dir;
	int radius;
	PyObject *hex;
};

struct hexrangeiter_s
{
	PyObject_HEAD
	int pos;
	int	dir;
	int distance;
	int radius;
	PyObject *hex;
	PyObject *center;
};

// declare Python type objects
extern PyTypeObject AbstractHexType;
extern PyTypeObject HexRingGenType;
extern PyTypeObject HexRangeGenType;
PyTypeObject HexType;
PyTypeObject SliceType;
PyTypeObject StackType;
PyTypeObject GridType;

// declare some handy constants
static int	DIRECTIONS[6][3] = {
	{1, 0, -1},
	{1, -1, 0},
	{0, -1, 1},
	{-1, 0, 1},
	{-1, 1, 0},
	{0, 1, -1}
};

#endif