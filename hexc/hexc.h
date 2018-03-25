#ifndef HEXC_H
#define HEXC_H

#include <Python.h>
#include <structmember.h>
#include <stdlib.h>
#include <Judy.h>

#define GET_ATTR(obj, attr) PyObject_GetAttrString(obj, attr)
#define HASH_SIZE 45
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
typedef struct hex_s			hex_t;
typedef struct slice_s			slice_t;
typedef struct stack_s			stack_t;
typedef struct grid_s			grid_t;

typedef struct hexringiter_s	hexringiter_t;
typedef struct hexrangeiter_s	hexrangeiter_t;

struct abstracthex_s
{
	PyObject_HEAD
	int q;
	int r;
	int s;
	int z;
};

struct hex_s
{
	abstracthex_t	abstract;
	PyObject		*parent;
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
	int	dir;
	long pos;
	long distance;
	long radius;
	PyObject *hex;
	PyObject *center;
};

struct stack_s
{
	PyObject_HEAD
	long radius;
	long height;
	Pvoid_t chunk;
	PyObject *parent;
};

struct grid_s
{
	PyObject_HEAD
	PyObject **stacks;
};

// declare Python type objects
extern PyTypeObject AbstractHexType;
extern PyTypeObject HexRingGenType;
extern PyTypeObject HexRangeGenType;
extern PyTypeObject HexType;
// extern PyTypeObject SliceType;
extern PyTypeObject StackType;
// extern PyTypeObject GridType;

// declare some handy constants
// static int	DIRECTIONS[6][3] = {
// 	{1, 0, -1},
// 	{1, -1, 0},
// 	{0, -1, 1},
// 	{-1, 0, 1},
// 	{-1, 1, 0},
// 	{0, 1, -1}
// };
static int	DIRECTIONS[8][4] = {
	{ 1, -1,  0,  0}, 
	{ 1,  0, -1,  0},	//     __<--2	
	{ 0,  1, -1,  0},	// 3--/  \--1	FLAT TOP
	{-1,  1,  0,  0},	// 4--\__/--0
	{-1,  0,  1,  0},	// 5----^     	6/7 = UP/DOWN
	{ 0, -1,  1,  0},	//  2--/\--1
	{ 0,  0,  0,  1},	//  3-|  |-0 	POINTY TOP
	{ 0,  0,  0, -1}	//  4--\/--5
};

#endif