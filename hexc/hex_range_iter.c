#include "hexc.h"
#include <stdio.h>
#define GETINT(x,y) PyLong_AsLong(PyObject_GetAttr(x, Py_BuildValue("s", y)))
#define TOLONG(x) PyLong_AsLong(x)

// Define class boilerplate functions
static PyObject *hex_range_iter_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	hexrangeiter_t *self;

	self = (hexrangeiter_t *)type->tp_alloc(type, 0);
	if (self != NULL) {
		self->pos = 0;
		self->dir = 0;
        self->distance = 1;
		self->radius = 0;
		self->hex = NULL;
        self->center = NULL;
	}

	return (PyObject *)self;
}

static void hex_range_iter_dealloc(hexrangeiter_t *self)
{
    // Py_XDECREF(self->center);
    // Py_XDECREF(self->hex);
	Py_TYPE(self)->tp_free((PyObject*)self);
    puts("Here");
    
}

static int hex_range_iter_init(hexrangeiter_t *self, PyObject *args, PyObject *kwds)
{
	static char *kwlist[] = {"center", "radius", NULL};
	if (! PyArg_ParseTupleAndKeywords(args, kwds, "Oi", kwlist, &self->center, &self->radius))
		return -1;

	PyObject *radius = Py_BuildValue("i", 1);
    PyObject *direction = PyObject_CallMethod(self->center, "direction", "i", 4);
    PyObject *scaled = PyNumber_Multiply(direction, radius);
	self->hex = PyNumber_Add(self->center, scaled);
	Py_CLEAR(radius);
    Py_CLEAR(direction);
    Py_CLEAR(scaled);
	return 0;
}

static PyObject *hex_range_iter_next(hexrangeiter_t *self)
{
	/* Returning NULL in this case is enough. The next() builtin will raise the
	 * StopIteration error for us.
	*/
    while (1)
    {
        if (self->distance <= self->radius)
        {    
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
                    continue;
                }
            }
            self->dir = 0;
            self->distance++;
            PyObject *radius = Py_BuildValue("i", self->distance);    
            PyObject *direction = PyObject_CallMethod(self->center, "direction", "i", 4);
            PyObject *scaled = PyNumber_Multiply(direction, radius);
            self->hex = PyNumber_Add(self->center, scaled);
            Py_CLEAR(radius);
            Py_CLEAR(direction);
            Py_CLEAR(scaled);
            continue;
        }
        break;
    }
    puts("Here");
    // Py_CLEAR(self->hex);
    Py_CLEAR(self->center);
    return NULL;
}

PyTypeObject HexRangeGenType = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "HexRangeGenerator",             /* tp_name */
    sizeof(hexrangeiter_t),           /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor)hex_range_iter_dealloc,  /* tp_dealloc */
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
    (iternextfunc)hex_range_iter_next,   /* tp_iternext */
    0,                              /* tp_methods */
    0,                              /* tp_members */
    0,                              /* tp_getset */
    0,                              /* tp_base */
    0,                              /* tp_dict */
    0,                              /* tp_descr_get */
    0,                              /* tp_descr_set */
    0,                              /* tp_dictoffset */
    (initproc)hex_range_iter_init,                 /* tp_init */
    PyType_GenericAlloc,            /* tp_alloc */
    hex_range_iter_new,                  /* tp_new */
};
