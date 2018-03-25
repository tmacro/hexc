#include "hexc.h"

// Define class boilerplate functions
static PyObject *concrete_hex_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	hex_t *self;

	self = (hex_t *)type->tp_alloc(type, 0);
	if (self != NULL) {
		((abstracthex_t*)self)->q = 0;
		((abstracthex_t*)self)->r = 0;
		((abstracthex_t*)self)->s = 0;
		((abstracthex_t*)self)->z = 0;
        self->parent = NULL;
	}

	return (PyObject *)self;
}

static void concrete_hex_dealloc(hex_t *self)
{
	if (self->parent)
    	Py_XDECREF(self->parent);
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static int concrete_hex_init(hex_t *self, PyObject *args, PyObject *kwds)
{
	static char *kwlist[] = {"q", "r", "s", "z", "parent", NULL};
	
	if (! PyArg_ParseTupleAndKeywords(args, kwds, "ii|iiO", kwlist, 
        &((abstracthex_t*)self)->q, &((abstracthex_t*)self)->r,
		&((abstracthex_t*)self)->s, &((abstracthex_t*)self)->z,
		&self->parent))
		return -1;

	if (((abstracthex_t*)self)->q + ((abstracthex_t*)self)->r + ((abstracthex_t*)self)->s !=0 && ((abstracthex_t*)self)->s == 0)
		((abstracthex_t*)self)->s = -((abstracthex_t*)self)->q - ((abstracthex_t*)self)->r;
	else if (((abstracthex_t*)self)->q + ((abstracthex_t*)self)->r + ((abstracthex_t*)self)->s !=0 && ((abstracthex_t*)self)->s != 0)
	{
		PyErr_SetString(PyExc_TypeError, "Invalid hexagonal coordinates!");
		return -1;
	}
    
    if (self->parent == NULL)
	{
        self->parent = Py_None;
    	Py_INCREF(self->parent);
	}
	return 0;
}

static PyMemberDef concrete_hex_members[] = {
	{"q", T_INT, offsetof(abstracthex_t, q), 0,
	 "q"},
	{"r", T_INT, offsetof(abstracthex_t, r), 0,
	 "r"},
	{"s", T_INT, offsetof(abstracthex_t, s), 0,
	 "s"},
	{"z", T_INT, offsetof(abstracthex_t, z), 0,
	 "z"},
    {"parent", T_OBJECT, offsetof(hex_t, parent), 0,
    "parent"},
	{NULL}	/* Sentinel */
};

static PyObject *concrete_hex_repr(abstracthex_t *obj)
{
	return PyUnicode_FromFormat("Hex(q:\%i, r:\%i, s:\%i, z:\%i)",
									obj->q, obj->r, obj->s, obj->z);
}

static PyObject *concrete_hex_compare(PyObject *self, PyObject *other, int op)
{
	long sQ, sR, sS, sZ, oQ, oR, oS, oZ;
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

			tmp = PyObject_GetAttrString(self, "z");
			if (!tmp)
				goto except;
			sZ = PyLong_AsLong(tmp);
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

			tmp = PyObject_GetAttrString(other, "z");
			if (!tmp)
				goto except;
			oZ = PyLong_AsLong(tmp);
			Py_DECREF(tmp);

			result = (	sQ == oQ
					&&	sR == oR
					&&	sS == oS
					&&	sZ == oZ) ? Py_True : Py_False;
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

static PyObject *concrete_hex_add(PyObject *self, PyObject *other)
{
	long q, r, s, z, sQ, sR, sS, sZ, oQ, oR, oS, oZ;	
	PyObject *ret = NULL, *argList = NULL, *tmp = NULL, *parent = NULL;
	goto try;

	try:
		if (!PyObject_HasAttrString(other, "q") || !PyObject_HasAttrString(other, "r") || !PyObject_HasAttrString(other, "s"))
		{
			PyErr_SetString(PyExc_TypeError, "Second arguement does not implement AbstractHex interface.");
			goto except;
		}

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

		tmp = PyObject_GetAttrString(self, "z");
		if (!tmp)
			goto except;
		sZ = PyLong_AsLong(tmp);
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

		tmp = PyObject_GetAttrString(other, "z");
		if (!tmp)
			goto except;
		oZ = PyLong_AsLong(tmp);
		Py_DECREF(tmp);

		parent = PyObject_GetAttrString(self, "parent");
		if (!parent)
			goto except;

		assert(!PyErr_Occurred());

		q = sQ + oQ;
		r = sR + oR;
		s = sS + oS;
		z = sZ + oZ;

		argList = Py_BuildValue("iiiiO", q, r, s, z, parent);
		if (!argList)
			goto except;
		ret = PyObject_CallObject((PyObject*)self->ob_type, argList);
		Py_DECREF(argList);		
		Py_DECREF(parent);
		if(!ret)
			goto except;
		assert(ret);
		goto finally;

	except:
		Py_XDECREF(tmp);
		Py_XDECREF(ret);
		Py_XDECREF(argList);
		Py_XDECREF(parent);
		assert(PyErr_Occurred());
		ret = NULL;

	finally:
		return (ret);
}

static PyObject *concrete_hex_subtract(PyObject *self, PyObject *other)
{
	long q, r, s, z, sQ, sR, sS, sZ, oQ, oR, oS, oZ;
	PyObject *ret = NULL, *argList = NULL, *tmp = NULL, *parent = NULL;
	goto try;

	try:
		if (!PyObject_HasAttrString(other, "q") || !PyObject_HasAttrString(other, "r") || !PyObject_HasAttrString(other, "s"))
		{
			PyErr_SetString(PyExc_TypeError, "Second arguement does not implement AbstractHex interface.");
			goto except;
		}

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

		tmp = PyObject_GetAttrString(self, "z");
		if (!tmp)
			goto except;
		sZ = PyLong_AsLong(tmp);
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

		tmp = PyObject_GetAttrString(other, "z");
		if (!tmp)
			goto except;
		oZ = PyLong_AsLong(tmp);
		Py_DECREF(tmp);

		parent = PyObject_GetAttrString(self, "parent");
		if (!parent)
			goto except;

		assert(!PyErr_Occurred());

		q = sQ - oQ;
		r = sR - oR;
		s = sS - oS;
		z = sZ - oZ;

		argList = Py_BuildValue("iiiiO", q, r, s, z, parent);
		if (!argList)
			goto except;
		ret = PyObject_CallObject((PyObject*)self->ob_type, argList);
		Py_DECREF(argList);		
		Py_DECREF(parent);
		if(!ret)
			goto except;
		assert(ret);
		goto finally;

	except:
		Py_XDECREF(tmp);
		Py_XDECREF(ret);
		Py_XDECREF(argList);
		Py_XDECREF(parent);
		assert(PyErr_Occurred());
		ret = NULL;

	finally:
		return (ret);
}

static PyObject *concrete_hex_scale(PyObject *self, PyObject *scalar)
{
	long q, r, s, z, sQ, sR, sS, sZ, scale;	
	PyObject *ret = NULL, *argList = NULL, *tmp = NULL, *parent = NULL;
	goto try;

	try:
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

		tmp = PyObject_GetAttrString(self, "z");
		if (!tmp)
			goto except;
		sZ = PyLong_AsLong(tmp);
		Py_DECREF(tmp);

		scale = PyLong_AsLong(scalar);

		parent = PyObject_GetAttrString(self, "parent");
		if (!parent)
			goto except;

		assert(!PyErr_Occurred());

		q = sQ * scale;
		r = sR * scale;
		s = sS * scale;
		z = sZ * scale;

		argList = Py_BuildValue("iiiiO", q, r, s, z, parent);
		if (!argList)
			goto except;
		ret = PyObject_CallObject((PyObject*)self->ob_type, argList);
		Py_DECREF(argList);	
		Py_DECREF(parent);	
		if(!ret)
			goto except;
		assert(ret);
		goto finally;

	except:
		Py_XDECREF(tmp);
		Py_XDECREF(ret);
		Py_XDECREF(argList);
		Py_XDECREF(parent);	
		assert(PyErr_Occurred());
		ret = NULL;

	finally:
		return (ret);
}

// define struct for mathmatical operations tp_as_number
static PyNumberMethods concrete_hex_num_meths[] = {
			&concrete_hex_add,		// nb_add;
			&concrete_hex_subtract,	// nb_subtract;
			&concrete_hex_scale,	// nb_multiply;
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

static PyObject *concrete_hex_length(PyObject *self)
{
	long len;
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

static PyObject *concrete_hex_distance(PyObject *self, PyObject *hex2)
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

static PyObject *concrete_hex_direction(PyObject *self, PyObject *direction)
{
	long dir = PyLong_AsLong(direction);
	PyObject *argList = NULL, *ret = NULL, *parent = NULL;

	try:
		parent = PyObject_GetAttrString(self, "parent");
		if (parent == NULL || PyErr_Occurred() != NULL)
			goto except;
		
		argList = Py_BuildValue("iiii", DIRECTIONS[dir][0], DIRECTIONS[dir][1],
										DIRECTIONS[dir][2], DIRECTIONS[dir][3]);
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
		Py_XDECREF(parent);
		Py_XDECREF(argList);	
		ret = NULL;
		return (ret);

	finally:
		Py_DECREF(argList);
		Py_DECREF(parent);
		return (ret);
}

static PyObject *concrete_hex_neighbor(PyObject *self, PyObject *direction)
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

static PyObject *concrete_hex_around(PyObject *self, PyObject *args, PyObject *kwargs)
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

static PyObject *concrete_hex_within(PyObject *self, PyObject *args, PyObject *kwargs)
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

static PyObject *concrete_hex_get(PyObject *self)
{
	PyObject	*value = NULL, *parent = NULL;
	int			res;
	
	try:
		// if (!PyArg_ParseTuple(args, "O", &value))
			// goto except;
		
		parent = PyObject_GetAttrString(self, "parent");
		if (parent == NULL || PyErr_Occurred() != NULL)
		{
			PyErr_SetString(PyExc_TypeError, "Hex is not associated with any Stack!");
			goto except;
		}

		res = PyObject_RichCompareBool(parent, Py_None, Py_EQ);	// return 1 if true, 0 if false and -1 on error
		if (res == 1)
		{
			PyErr_SetString(PyExc_TypeError, "Hex is not associated with any Stack!");			
			goto except; 
		}
		else if (res == -1)
			goto except;

		value = PyObject_CallMethod(parent, "get", "O", self);
		if (value == NULL || PyErr_Occurred() != NULL)
			goto except;
		Py_DECREF(parent);
		goto finally;

	except:
		Py_XDECREF(value);
		Py_XDECREF(parent);
		value = NULL;

	finally:
		return value;
}

static PyObject *concrete_hex_set(PyObject *self, PyObject *args)
{
	PyObject	*value = NULL, *ret = NULL, *parent = NULL;
	int			res;
	
	try:
		if (!PyArg_ParseTuple(args, "O", &value))
			goto except;
		
		parent = PyObject_GetAttrString(self, "parent");
		if (parent == NULL || PyErr_Occurred() != NULL)
		{
			PyErr_SetString(PyExc_TypeError, "Hex is not associated with any Stack!");
			goto except;
		}

		res = PyObject_RichCompareBool(parent, Py_None, Py_EQ);	// return 1 if true, 0 if false and -1 on error
		if (res == 1)
		{
			PyErr_SetString(PyExc_TypeError, "Hex is not associated with any Stack!");			
			goto except; 
		}
		else if (res == -1)
			goto except;

		ret = PyObject_CallMethod(parent, "set", "OO", self, value); // None is returned, to save some trouble dont decref and return the same one
		if (ret == NULL || PyErr_Occurred() != NULL)
			goto except;
		Py_DECREF(parent);
		goto finally;

	except:
		Py_XDECREF(ret);
		ret = NULL;

	finally:
		return ret;
}

static PyMethodDef concrete_hex_methods[] = {
	{"length", (PyCFunction)concrete_hex_length, METH_NOARGS,
	"Distance from 0, 0, 0"
	},
	{"distance", (PyCFunction)concrete_hex_distance, METH_O,
	"Distance between 2 AbstractHexes"
	},
	{"direction", (PyCFunction)concrete_hex_direction, METH_O,
	"Returns the coordinates associated with a given direction."
	},
	{"neighbor", (PyCFunction)concrete_hex_neighbor, METH_O,
	"Return the neighboring hex in the given direction."
	},
	{"around", (PyCFunction)concrete_hex_around, METH_VARARGS | METH_KEYWORDS,
	"Return the ring of hexes, radius distance from this hex."
	},
	{"within", (PyCFunction)concrete_hex_within, METH_VARARGS | METH_KEYWORDS,
	"Return the hexes within radius distance inclusively.",
	},
	{"get", (PyCFunction)concrete_hex_get, METH_VARARGS,
	"If a parent Stack is assigned return the associated value at this position."
	},
	{"set", (PyCFunction)concrete_hex_set, METH_VARARGS,
	"If a parent Stack is assigned set the value at this position."
	},
    {NULL}  /* Sentinel */
};

// Object description
PyTypeObject HexType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"hexc.Hex",					/* tp_name */
	sizeof(hex_t),				/* tp_basicsize */
	0,							/* tp_itemsize */
	(destructor)concrete_hex_dealloc,	/* tp_dealloc */
	0,							/* tp_print */
	0,							/* tp_getattr */
	0,							/* tp_setattr */
	0,							/* tp_reserved */
	concrete_hex_repr,			/* tp_repr */
	concrete_hex_num_meths,		/* tp_as_number */
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
	(richcmpfunc)&concrete_hex_compare,	/* tp_richcompare */
	0,							/* tp_weaklistoffset */
	0,							/* tp_iter */
	0,							/* tp_iternext */
	concrete_hex_methods,		/* tp_methods	Hex_methods goes here */
	concrete_hex_members,		/* tp_members */
	0,							/* tp_getset */
	0,							/* tp_base */
	0,							/* tp_dict */
	0,							/* tp_descr_get */
	0,							/* tp_descr_set */
	0,							/* tp_dictoffset */
	(initproc)concrete_hex_init,/* tp_init */
	0,							/* tp_alloc */
	concrete_hex_new,			/* tp_new */
};