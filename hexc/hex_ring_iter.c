#include "hexc.h"
#define GETINT(x,y) PyLong_AsLong(PyObject_GetAttr(x, Py_BuildValue("s", y)))
#define TOLONG(x) PyLong_AsLong(x)

// Define class boilerplate functions
static PyObject *iter_ring_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	hexringiter_t *self;

	self = (hexringiter_t *)type->tp_alloc(type, 0);
	if (self != NULL) {
		self->pos = 0;
		self->dir = 0;
		self->radius = 0;
		self->hex = NULL;
	}

	return (PyObject *)self;
}

static void iter_ring_dealloc(hexringiter_t *self)
{
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static int iter_ring_init(hexringiter_t *self, PyObject *args, PyObject *kwds)
{
	static char *kwlist[] = {"center", "radius", NULL};
	if (! PyArg_ParseTupleAndKeywords(args, kwds, "Oi", kwlist, &self->hex, &self->radius))
		return -1;

	PyObject *rad = Py_BuildValue("i", self->radius);
	self->hex = PyNumber_Add(self->hex, 
					PyNumber_Multiply(
						PyObject_CallMethod(self->hex, "direction", "i", 4), 
						rad
						));
	Py_INCREF(self->hex);						
	Py_DECREF(rad);
	return 0;
}

static PyObject *iter_ring_next(hexringiter_t *self)
{
	/* Returning NULL in this case is enough. The next() builtin will raise the
	 * StopIteration error for us.
	*/
	if (self->dir < 6)
	{
		if (self->pos < self->radius)
		{
			PyObject *ret = self->hex;
			self->hex = PyObject_CallMethod(ret, "neighbor", "i", self->dir);
			self->pos++;
			return (ret);
		}
		else
		{
			self->pos = 0;
			self->dir++;
			return iter_ring_next(self);
		}
	}
	Py_CLEAR(self->hex);
	return NULL;
}

PyTypeObject HexRingGenType = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "HexRingGenerator",             /* tp_name */
    sizeof(hexringiter_t),           /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)iter_ring_dealloc,  /* tp_dealloc */
    0,                              /* tp_print */
    0,                              /* tp_getattr */
    0,                              /* tp_setattr */
    0,                              /* tp_reserved */
    0,                 /* tp_repr */
    0,                              /* tp_as_number */
    0,                              /* tp_as_sequence */
    0,                              /* tp_as_mapping */
    0,                              /* tp_hash */
    0,                              /* tp_call */
    0,                              /* tp_str */
    0,                              /* tp_getattro */
    0,                              /* tp_setattro */
    0,                              /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,             /* tp_flags */
    0,                              /* tp_doc */
    0,                              /* tp_traverse */
    0,                              /* tp_clear */
    0,                              /* tp_richcompare */
    0,                              /* tp_weaklistoffset */
    PyObject_SelfIter,              /* tp_iter */
    (iternextfunc)iter_ring_next,   /* tp_iternext */
    0,                              /* tp_methods */
    0,                              /* tp_members */
    0,                              /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    (initproc)iter_ring_init,                 /* tp_init */
    0,					            /* tp_alloc */
    iter_ring_new,                  /* tp_new */
};
