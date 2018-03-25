#include "hexc.h"


// static void HexMap_dealloc(HexMap* self)
// {
// 	Word_t Bytes;
// 	JSLFA(Bytes, self->JMap)
//     Py_TYPE(self)->tp_free((PyObject*)self);
// }

// static PyObject *HexMap_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
// {
//     HexMap *self;

//     self = (HexMap*)type->tp_alloc(type, 0);
//     if (self != NULL) {
// 		self->JMap = (PWord_t)NULL;
//     }

//     return (PyObject *)self;
// }

// static int HexMap_init(HexMap *self, PyObject *args, PyObject *kwds)
// {
// 	return 0;
// }

// -----------------


// Define class boilerplate functions
static PyObject *stack_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	stack_t *self;

	self = (stack_t *)type->tp_alloc(type, 0);
	if (self != NULL) {
		self->chunk = (PWord_t)NULL;
        self->radius = 0;
        self->height = 0;
        self->parent = (PyObject*)NULL;
		
	}

	return (PyObject *)self;
}

// static int  free_chunk(void *trash, void *value)
// {
//     Py_XDECREF(value);
//     return MAP_OK;
// }

static void stack_dealloc(stack_t *self)
{
    // hashmap_iterate(self->chunk, &free_chunk, NULL);
    // hashmap_free(self->chunk);
    self->chunk = NULL;
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static int stack_init(stack_t *self, PyObject *args, PyObject *kwds)
{
	static char *kwlist[] = {"radius", "height", "parent", NULL};

	if (! PyArg_ParseTupleAndKeywords(args, kwds, "ii|O!", kwlist, &self->radius, &self->height, &StackType, &self->parent))
		return -1;
	if (self != NULL) {
		self->chunk = (PWord_t)NULL;
    }
	return 0;
}


unsigned char *hash_hex(long q, long r, long s, long z)
{
	unsigned char *ret;
	char buf[HASH_SIZE];
	size_t size = HASH_SIZE;

	bzero(buf, HASH_SIZE);
	snprintf(buf, size, "%0+11li%0+11li%0+11li%0+11li", q, r, s, z);
	ret = (unsigned char*)malloc(sizeof(char) * HASH_SIZE);
	bzero(ret, HASH_SIZE);
	// printf("hash %lu long\n", strlen(buf));
	memcpy(ret, buf, HASH_SIZE);
	return (ret);
}

static PyObject *stack_set(stack_t *self, PyObject *args, PyObject *kwds)
{
	static char *kwlist[] = {"coord", "value", "overwrite", NULL};
    
	PyObject 		*coord;
	PyObject    	*value;
	int             overwrite = 1;
	unsigned char	*hash;
	long			q, r, s, z;
	PyObject		*tmp;
	PWord_t 		ptr;
	
	
	if (! PyArg_ParseTupleAndKeywords(args, kwds, "OO|p", kwlist, &coord, &value, &overwrite))
        return -1;

	tmp = PyObject_GetAttrString(coord, "q");
	q = PyLong_AsLong(tmp);
	Py_DECREF(tmp);
	tmp = PyObject_GetAttrString(coord, "r");
	r = PyLong_AsLong(tmp);	
	Py_DECREF(tmp);
	tmp = PyObject_GetAttrString(coord, "s");
	s = PyLong_AsLong(tmp);	
	Py_DECREF(tmp);
	tmp = PyObject_GetAttrString(coord, "z");
	z = PyLong_AsLong(tmp);	
	Py_DECREF(tmp);
	hash = hash_hex(q, r, s, z);
	// printf("setting %i, %i, %i to  %p using key %s\n", q, r, s, value, hash);
	JSLI(ptr, self->chunk, hash);	
	free(hash);
	*ptr = (Word_t)value;	
	// hashmap_put(self->chunk, hash, (any_t)value);
	Py_INCREF(value);

	Py_RETURN_NONE;
}

static PyObject *stack_get(stack_t *self, PyObject *args)
{
	PyObject		*coord;
	Pvoid_t			value = NULL;
	PyObject		*tmp;
	unsigned char	*hash;
	int				err;
	long 			q, r, s, z;
	// PWord_t			Value;

	if (! PyArg_ParseTuple(args, "O", &coord))
		return -1;

	tmp = PyObject_GetAttrString(coord, "q");
	q = PyLong_AsLong(tmp);
	Py_DECREF(tmp);
	tmp = PyObject_GetAttrString(coord, "r");
	r = PyLong_AsLong(tmp);	
	Py_DECREF(tmp);
	tmp = PyObject_GetAttrString(coord, "s");
	s = PyLong_AsLong(tmp);	
	Py_DECREF(tmp);
	tmp = PyObject_GetAttrString(coord, "z");
	z = PyLong_AsLong(tmp);	
	Py_DECREF(tmp);
	hash = hash_hex(q, r, s, z);
	// printf("getting %i, %i, %i to  %p using key %s\n", q, r, s, value, hash);
	JSLG(value, self->chunk, hash);
	free(hash);
	
	// err = hashmap_get(self->chunk, hash, &value);
	if (!value)
		Py_RETURN_NONE;
	return Py_BuildValue("O", *(PyObject**)value);
}

static PyObject *stack_rem(stack_t *self, PyObject *args)
{
	PyObject		*coord;
 	char			*hash;
	int				err;
	long 			q, r, s;
	PyObject		*tmp;
	

	// if (! PyArg_ParseTuple(args, "O", &coord))
	// 	return -1;

	// tmp = PyObject_GetAttrString(coord, "q");
	// q = PyLong_AsLong(tmp);
	// Py_DECREF(tmp);
	// tmp = PyObject_GetAttrString(coord, "r");
	// r = PyLong_AsLong(tmp);	
	// Py_DECREF(tmp);
	// tmp = PyObject_GetAttrString(coord, "s");
	// s = PyLong_AsLong(tmp);	
	// Py_DECREF(tmp);
	// hash = hash_hex(q, r, s);
	// err = hashmap_remove(self->chunk, hash);
	// free(hash);
	// if (err == MAP_MISSING)
	Py_RETURN_NONE;
	// Py_RETURN_TRUE;
}

static PyObject *stack_repr(stack_t *obj)
{
	return PyUnicode_FromFormat("Stack(radius:\%i, height:\%i)",
									obj->radius, obj->height);
}

static PyMemberDef stack_members[] = {
	{"radius", T_INT, offsetof(stack_t, radius), 0,
	 "radius"},
	{"height", T_INT, offsetof(stack_t, height), 0,
	 "height"},
	{NULL}	/* Sentinel */
};

static PyMethodDef stack_methods[] = {
    {"set", (PyCFunction)stack_set, METH_VARARGS | METH_KEYWORDS,
    "set a position on the hex grid to a value"
    },
	{"get", (PyCFunction)stack_get, METH_VARARGS | METH_KEYWORDS,
	"get the value at position on the hex grid return None if position does not exist"
	},
	{"rem", (PyCFunction)stack_rem, METH_VARARGS | METH_KEYWORDS,
	"remove the value at position and return True if it exists return None otherwise"
	},
    {NULL}  /* Sentinel */
};

PyTypeObject StackType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"hexc.Stack",				/* tp_name */
	sizeof(stack_t),			/* tp_basicsize */
	0,							/* tp_itemsize */
	(destructor)stack_dealloc,	/* tp_dealloc */
	0,							/* tp_print */
	0,							/* tp_getattr */
	0,							/* tp_setattr */
	0,							/* tp_reserved */
	stack_repr,					/* tp_repr */
	0,							/* tp_as_number */
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
	"3D hexagonal map",			/* tp_doc */
	0,							/* tp_traverse */
	0,							/* tp_clear */
	0,							/* tp_richcompare */
	0,							/* tp_weaklistoffset */
	0,							/* tp_iter */
	0,							/* tp_iternext */
	stack_methods,				/* tp_methods	Hex_methods goes here */
	stack_members,				/* tp_members */
	0,							/* tp_getset */
	0,							/* tp_base */
	0,							/* tp_dict */
	0,							/* tp_descr_get */
	0,							/* tp_descr_set */
	0,							/* tp_dictoffset */
	(initproc)stack_init,		/* tp_init */
	0,							/* tp_alloc */
	stack_new,					/* tp_new */
};