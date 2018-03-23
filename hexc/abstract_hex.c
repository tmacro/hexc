#include "hexc.h"

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
	long sQ, sR, sS, oQ, oR, oS;
	PyObject *tmp = NULL;
	PyObject *result = NULL;

	try:
		switch (op) {
		case Py_EQ:
			tmp = PyObject_GetAttrString(self, "q");
			if (!tmp)
				goto except;
			sQ = PyLong_AsLong(tmp);
			Py_DECREF(tmp);
			tmp = PyObject_GetAttrString(self, "r");
			if (!tmp)
				goto except;
			sR = PyLong_AsLong(tmp);
			Py_DECREF(tmp);			
			tmp = PyObject_GetAttrString(self, "s");
			if (!tmp)
				goto except;
			sS = PyLong_AsLong(tmp);
			Py_DECREF(tmp);
			tmp = PyObject_GetAttrString(other, "q");
			if (!tmp)
				goto except;
			oQ = PyLong_AsLong(tmp);
			Py_DECREF(tmp);
			tmp = PyObject_GetAttrString(other, "r");
			if (!tmp)
				goto except;
			oR = PyLong_AsLong(tmp);
			Py_DECREF(tmp);
			tmp = PyObject_GetAttrString(other, "s");
			if (!tmp)
				goto except;
			oS = PyLong_AsLong(tmp);
			Py_DECREF(tmp);
			result = (	sQ == oQ
					&&	sR == oR
					&&	sS == oS) ? Py_True : Py_False;
			// result = (GETINT(self, "q") == GETINT(other, "q") && GETINT(self, "r") == GETINT(other, "r") && GETINT(self, "s") == GETINT(other, "s")) ? Py_True : Py_False;
			goto finally;
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
	except:
		Py_XDECREF(tmp);
		result = NULL;
	
	finally:
		Py_XINCREF(result);
		return result;
}

// Define abstract_hex math functions

static PyObject *abstract_hex_add(PyObject *self, PyObject *other)
{
	long q, r, s;
	PyObject *sQ, *sR, *sS, *oQ, *oR, *oS, *ret, *argList;
	sQ = sR = sS = oQ = oR = oS = ret = argList = NULL;
	goto try;

	try:
		if (!PyObject_HasAttrString(other, "q") || !PyObject_HasAttrString(other, "r") || !PyObject_HasAttrString(other, "s"))
		{
			PyErr_SetString(PyExc_TypeError, "Second arguement does not implement AbstractHex interface.");
			goto except;
		}
		sQ = GET_ATTR(self, "q");
		sR = GET_ATTR(self, "r");
		sS = GET_ATTR(self, "s");
		oQ = GET_ATTR(other, "q");
		oR = GET_ATTR(other, "r");
		oS = GET_ATTR(other, "s");

		q = PyLong_AsLong(sQ) + PyLong_AsLong(oQ);
		r = PyLong_AsLong(sR) + PyLong_AsLong(oR);
		s = PyLong_AsLong(sS) + PyLong_AsLong(oS);

		assert(!PyErr_Occurred());

		argList = Py_BuildValue("iii", q, r, s);
		ret = PyObject_CallObject((PyObject*)self->ob_type, argList);
		assert(ret);
		goto finally;

	except:
		Py_XDECREF(ret);
		Py_XDECREF(argList);
		assert(PyErr_Occurred());
		ret = NULL;

	finally:
		Py_XDECREF(argList);		
		return (ret);
}

static PyObject *abstract_hex_subtract(PyObject *self, PyObject *other)
{
	long q, r, s;
	PyObject *sQ, *sR, *sS, *oQ, *oR, *oS, *ret, *argList;
	sQ = sR = sS = oQ = oR = oS = ret = argList = NULL;
	goto try;

	try:
		if (!PyObject_HasAttrString(other, "q") || !PyObject_HasAttrString(other, "r") || !PyObject_HasAttrString(other, "s"))
		{
			PyErr_SetString(PyExc_TypeError, "Second arguement does not implement AbstractHex interface.");
			goto except;
		}
		sQ = GET_ATTR(self, "q");
		sR = GET_ATTR(self, "r");
		sS = GET_ATTR(self, "s");
		oQ = GET_ATTR(other, "q");
		oR = GET_ATTR(other, "r");
		oS = GET_ATTR(other, "s");

		q = PyLong_AsLong(sQ) - PyLong_AsLong(oQ);
		r = PyLong_AsLong(sR) - PyLong_AsLong(oR);
		s = PyLong_AsLong(sS) - PyLong_AsLong(oS);

		assert(!PyErr_Occurred());

		argList = Py_BuildValue("iii", q, r, s);
		ret = PyObject_CallObject((PyObject*)self->ob_type, argList);
		assert(ret);
		goto finally;

	except:
		Py_XDECREF(ret);
		Py_XDECREF(argList);
		assert(PyErr_Occurred());
		ret = NULL;

	finally:
		Py_XDECREF(argList);		
		return (ret);
}

static PyObject *abstract_hex_scale(PyObject *self, PyObject *z)
{
	long q, r, s, scale;
	PyObject *sQ, *sR, *sS, *ret, *argList;
	sQ = sR = sS = ret = argList = NULL;
	goto try;

	try:
		scale = PyLong_AsLong(z);
		sQ = GET_ATTR(self, "q");
		sR = GET_ATTR(self, "r");
		sS = GET_ATTR(self, "s");

		q = PyLong_AsLong(sQ) * scale;
		r = PyLong_AsLong(sR) * scale;
		s = PyLong_AsLong(sS) * scale;

		assert(!PyErr_Occurred());

		argList = Py_BuildValue("iii", q, r, s);
		ret = PyObject_CallObject((PyObject*)self->ob_type, argList);
		assert(ret);
		goto finally;

	except:
		Py_XDECREF(ret);
		Py_XDECREF(argList);
		assert(PyErr_Occurred());
		ret = NULL;

	finally:
		Py_XDECREF(argList);		
		return (ret);
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
	PyObject *sQ = NULL, *sR = NULL, *sS = NULL, *ret = NULL;
	goto try;

	try:
		sQ = GET_ATTR(self, "q");
		sR = GET_ATTR(self, "r");
		sS = GET_ATTR(self, "s");

		if (PyErr_Occurred() != NULL)
			goto except;

		len = (abs(PyLong_AsLong(sQ)) + abs(PyLong_AsLong(sR)) + abs(PyLong_AsLong(sS))) / 2;

		ret = Py_BuildValue("i", len);
		if (PyErr_Occurred())
			goto except;
		assert(ret);
		goto finally;

	except:
		Py_XDECREF(ret);
		assert(PyErr_Occurred());
		ret = NULL;

	finally:
		Py_XDECREF(sQ);
		Py_XDECREF(sR);
		Py_XDECREF(sS);
		return (ret);
}

static PyObject *abstract_hex_distance(PyObject *self, PyObject *hex2)
{
	PyObject *sub = NULL, *ret = NULL;
	goto try;

	try:
		sub = PyNumber_Subtract(self, hex2);
		assert(sub);
		if (PyErr_Occurred() != NULL)
			goto except;

		ret = PyObject_CallMethod(sub, "length", NULL);
		assert(ret);
		if (PyErr_Occurred() != NULL)
			goto except;
		goto finally;

	except:
		Py_XDECREF(ret);
		assert(PyErr_Occurred());
		ret = NULL;

	finally:
		Py_XDECREF(sub);
		return (ret);
}

static PyObject *abstract_hex_direction(PyObject *self, PyObject *direction)
{
	long dir = PyLong_AsLong(direction);
	PyObject *argList = NULL, *ret = NULL;

	try:
		argList = Py_BuildValue("iii", DIRECTIONS[dir][0], DIRECTIONS[dir][1], DIRECTIONS[dir][2]);
		if (argList == NULL || PyErr_Occurred() != NULL)
			goto except;
		assert(argList);
		ret = PyObject_CallObject((PyObject*)self->ob_type, argList);
		if (ret == NULL || PyErr_Occurred() != NULL)
			goto except;
		assert(ret);
		goto finally;

	except:
		Py_XDECREF(ret);
		ret = NULL;

	finally:
		Py_XDECREF(argList);
		return (ret);
}

static PyObject *abstract_hex_neighbor(PyObject *self, PyObject *direction)
{
	PyObject *dir = NULL, *ret = NULL;

	try:
		dir  = PyObject_CallMethod(self, "direction", "O", direction);
		if (dir == NULL || PyErr_Occurred() != NULL)
			goto except;
		assert(dir);
		ret = PyNumber_Add(self, dir);
		if (ret == NULL || PyErr_Occurred() != NULL)
			goto except;
		assert(ret);
		goto finally;

	except:
		Py_XDECREF(ret);
		ret = NULL;

	finally:
		Py_XDECREF(dir);
		return (ret);
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

static PyObject *abstract_hex_within(PyObject *self, PyObject *args, PyObject *kwargs)
{
	long radius = 1;
	static char *kwlist[] = {"radius", NULL};

	if (! PyArg_ParseTupleAndKeywords(args, kwargs, "|i", kwlist, &radius))
		return NULL;

	PyObject *argsList = Py_BuildValue("Oi", self, radius);
	PyObject *obj = PyObject_CallObject((PyObject*)&HexRangeGenType, argsList);
	Py_DECREF(argsList);
	return obj;
}

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
	"Return the ring of hexes, radius distance from this hex."
	},
	{"within", (PyCFunction)abstract_hex_within, METH_VARARGS | METH_KEYWORDS,
	"Return the hexes within radius distance inclusively."
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