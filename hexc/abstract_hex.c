#include "hexc.h"
#define GETINT(x,y) PyLong_AsLong(PyObject_GetAttr(x, Py_BuildValue("s", y)))
#define TOLONG(x) PyLong_AsLong(x)

#define hex_to_vec(h) ((vec3_t){GETINT(h, 'q'), GETINT(h, 'r'), GETINT(h, 's')})

// Define class boilerplate functions
static PyObject *abstract_hex_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	abstracthex_t *self;

	self = (abstracthex_t *)type->tp_alloc(type, 0);
	if (self != NULL) {
		self->q = 0;
		self->r = 0;
		self->s = 0;
	}

	return (PyObject *)self;
}

static void abstract_hex_dealloc(abstracthex_t *self)
{
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static int abstract_hex_init(abstracthex_t *self, PyObject *args, PyObject *kwds)
{
	static char *kwlist[] = {"q", "r", "s", NULL};

	if (! PyArg_ParseTupleAndKeywords(args, kwds, "ii|i", kwlist, &self->q, &self->r, &self->s))
		return -1;

	if (self->q + self->r + self->s !=0 && self->s == 0)
		self->s = -self->q - self->r;
	else if (self->q + self->r + self->s !=0 && self->s != 0)
	{
		PyErr_SetString(PyExc_TypeError, "Invalid hexagonal coordinates!");
		return -1;
	}
	return 0;
}

static PyMemberDef abstract_hex_members[] = {
	{"q", T_INT, offsetof(abstracthex_t, q), 0,
	 "q"},
	{"r", T_INT, offsetof(abstracthex_t, r), 0,
	 "r"},
	{"s", T_INT, offsetof(abstracthex_t, s), 0,
	 "s"},
	{NULL}	/* Sentinel */
};

static PyObject *abstract_hex_repr(abstracthex_t *obj)
{
	return PyUnicode_FromFormat("AbstractHex(q:\%i, r:\%i, s:\%i)",
									obj->q, obj->r, obj->s);
}

static PyObject *abstract_hex_compare(PyObject *self, PyObject *other, int op)
{
	PyObject *result = NULL;

	// if (UndefinedObject_Check(other)) {
	//	result = Py_NotImplemented;
	// }
	// else {
		switch (op) {
		case Py_EQ:
			result = (GETINT(self, "q") == GETINT(other, "q") && GETINT(self, "r") == GETINT(other, "r") && GETINT(self, "s") == GETINT(other, "s")) ? Py_True : Py_False;
			break;
		// case Py_LT:
		//	result = Py_False;
		//	break;
		// case Py_LE:
		//	result = (LargestObject_Check(other)) ? Py_True : Py_False;
		//	break;
		// case Py_NE:
		//	result = (LargestObject_Check(other)) ? Py_False : Py_True;
		//	break;
		// case Py_GT:
		//	result = (LargestObject_Check(other)) ? Py_False : Py_True;
		//	break;
		// case Py_GE:
		//	result = Py_True;
		//	break;
		}
	// }

	Py_XINCREF(result);
	return result;
}

// Define abstract_hex math functions

static PyObject *abstract_hex_add(PyObject *self, PyObject *Hex2)
{
	long q, r, s;

	q = GETINT(self, "q") + GETINT(Hex2, "q");
	r = GETINT(self, "r") + GETINT(Hex2, "r");
	s = GETINT(self, "s") + GETINT(Hex2, "s");

	PyObject *argList = Py_BuildValue("iii", q, r, s);
	PyObject *obj = PyObject_CallObject((PyObject*)self->ob_type, argList);
	Py_DECREF(argList);
	
	return (obj);
}

static PyObject *abstract_hex_subtract(PyObject *self, PyObject *Hex2)
{
	long q, r, s;


	q = GETINT(self, "q") - GETINT(Hex2, "q");
	r = GETINT(self, "r") - GETINT(Hex2, "r");
	s = GETINT(self, "s") - GETINT(Hex2, "s");

	PyObject *argList = Py_BuildValue("iii", q, r, s);
	PyObject *obj = PyObject_CallObject((PyObject*)self->ob_type, argList);
	Py_DECREF(argList);
	return (obj);
}

static PyObject *abstract_hex_scale(PyObject *self, PyObject *z)
{
	long q, r, s, scale;
	scale = TOLONG(z);

	q = GETINT(self, "q") * scale;
	r = GETINT(self, "r") * scale;
	s = GETINT(self, "s") * scale;

	PyObject *argList = Py_BuildValue("iii", q, r, s);
	PyObject *obj = PyObject_CallObject((PyObject*)self->ob_type, argList);
	Py_DECREF(argList);
	return (obj);
}

// define struct for mathmatical operations tp_as_number
static PyNumberMethods abstract_hex_num_meths[] = {
			&abstract_hex_add,		// nb_add;
			&abstract_hex_subtract,	// nb_subtract;
			&abstract_hex_scale,	// nb_multiply;
			0,						// nb_remainder;
			0, 						//_divmod;
			0, 						//_power;
			0, 						//_negative;
			0, 						//_positive;
			0, 						//_absolute;
			0, 						//_bool;
			0, 						//_invert;
			0, 						//_lshift;
			0, 						//_rshift;
			0, 						//_and;
			0, 						//_xor;
			0, 						//_or;
			0, 						//_int;
			0, 						//_reserved;
			0, 						//_float;
			0, 						//_inplace_add;
			0, 						//_inplace_subtract;
			0, 						//_inplace_multiply;
			0, 						//_inplace_remainder;
			0, 						//_inplace_power;
			0, 						//_inplace_lshift;
			0, 						//_inplace_rshift;
			0, 						//_inplace_and;
			0, 						//_inplace_xor;
			0, 						//_inplace_or;
			0, 						//_floor_divide;
			0, 						//_true_divide;
			0, 						//_inplace_floor_divide;
			0, 						//_inplace_true_divide;
			0,						//	nb_index;
			0, 						//_matrix_multiply;
			0, 						//_inplace_matrix_multiply;
};

static PyObject *abstract_hex_length(PyObject *self)
{
	long q, r, s, len;

	q = GETINT(self, "q");
	r = GETINT(self, "r");
	s = GETINT(self, "s");	

	len = (abs(q) + abs(r) + abs(s)) / 2;
	return Py_BuildValue("i", len);
}

static PyObject *abstract_hex_distance(PyObject *self, PyObject *hex2)
{
	return PyObject_CallMethod(PyNumber_Subtract(self, hex2), "length", NULL);
	// return abstract_hex_length(abstract_hex_subtract(self, hex2));
}

static PyObject *abstract_hex_direction(PyObject *self, PyObject *direction)
{
	long dir = TOLONG(direction);
	PyObject *argList = Py_BuildValue("iii", DIRECTIONS[dir][0], DIRECTIONS[dir][1], DIRECTIONS[dir][2]);
	PyObject *obj = PyObject_CallObject((PyObject*)self->ob_type, argList);
	Py_DECREF(argList);	
	return obj;
}

static PyObject *abstract_hex_neighbor(PyObject *self, PyObject *direction)
{
	return PyNumber_Add(self, PyObject_CallMethod(self, "direction", "O", direction));
	// return abstract_hex_add(self, abstract_hex_direction(self, direction));
}

static PyObject *abstract_hex_around(PyObject *self, PyObject *args, PyObject *kwargs)
{
	long radius = 1;
	static char *kwlist[] = {"radius", NULL};

	if (! PyArg_ParseTupleAndKeywords(args, kwargs, "|i", kwlist, &radius))
		return NULL;

	PyObject *argsList = Py_BuildValue("Oi", self, radius);
	PyObject *obj = PyObject_CallObject((PyObject*)&HexRingGenType, argsList);
	Py_DECREF(argsList);
	return obj;
}

// static int abstract_hex_init(abstracthex_t *self, PyObject *args, PyObject *kwds)

static PyMethodDef abstract_hex_methods[] = {
	{"length", (PyCFunction)abstract_hex_length, METH_NOARGS,
	"Distance from 0, 0, 0"
	},
	{"distance", (PyCFunction)abstract_hex_distance, METH_O,
	"Distance between 2 AbstractHexes"
	},
	{"direction", (PyCFunction)abstract_hex_direction, METH_O,
	"Returns the coordinates associated with a given direction."
	},
	{"neighbor", (PyCFunction)abstract_hex_neighbor, METH_O,
	"Return the neighboring hex in the given direction."
	},
	{"around", (PyCFunction)abstract_hex_around, METH_VARARGS | METH_KEYWORDS,
	"Return the ring of hexes, radius distance for this."
	},
    {NULL}  /* Sentinel */
};

// Object description
PyTypeObject AbstractHexType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"hexc.AbstractHex",			/* tp_name */
	sizeof(abstracthex_t),		/* tp_basicsize */
	0,							/* tp_itemsize */
	(destructor)abstract_hex_dealloc,	/* tp_dealloc */
	0,							/* tp_print */
	0,							/* tp_getattr */
	0,							/* tp_setattr */
	0,							/* tp_reserved */
	abstract_hex_repr,			/* tp_repr */
	abstract_hex_num_meths,		/* tp_as_number */
	0,							/* tp_as_sequence */
	0,							/* tp_as_mapping */
	0,							/* tp_hash	*/
	0,							/* tp_call */
	0,							/* tp_str */
	0,							/* tp_getattro */
	0,							/* tp_setattro */
	0,							/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT |
		Py_TPFLAGS_BASETYPE,	/* tp_flags */
	"A container for manipulating hexagonal coordinates",/* tp_doc */
	0,							/* tp_traverse */
	0,							/* tp_clear */
	(richcmpfunc)&abstract_hex_compare,	/* tp_richcompare */
	0,							/* tp_weaklistoffset */
	0,							/* tp_iter */
	0,							/* tp_iternext */
	abstract_hex_methods,		/* tp_methods	Hex_methods goes here */
	abstract_hex_members,		/* tp_members */
	0,							/* tp_getset */
	0,							/* tp_base */
	0,							/* tp_dict */
	0,							/* tp_descr_get */
	0,							/* tp_descr_set */
	0,							/* tp_dictoffset */
	(initproc)abstract_hex_init,/* tp_init */
	0,							/* tp_alloc */
	abstract_hex_new,			/* tp_new */
};