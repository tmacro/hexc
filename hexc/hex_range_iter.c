#include "hexc.h"

// Define class boilerplate functions
static PyObject *hex_range_iter_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	hexrangeiter_t *self;

	self = (hexrangeiter_t *)(type->tp_alloc(type, 0));
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
    Py_DECREF(self->hex);  // decref our stored hex
    Py_DECREF(self->center); // and our center point
	Py_TYPE(self)->tp_free((PyObject*)self); // SIGSEGV here
    
}

static int hex_range_iter_init(hexrangeiter_t *self, PyObject *args, PyObject *kwds)
{
    // Takes 2 args, the center of the ring as an AbstractHex, and the radius as an int
    static char *kwlist[] = {"center", "radius", NULL};
	if (! PyArg_ParseTupleAndKeywords(args, kwds, "Oi", kwlist, &self->center, &self->radius))
		return -1;

    Py_INCREF(self->center); //increc\f here because Py_Arg_Parse... returns borrowed references
	PyObject *radius = PyLong_FromLong(1L);
    PyObject *direction = PyObject_CallMethod(self->center, "direction", "i", 4);
    PyObject *scaled = PyNumber_Multiply(direction, radius);
	self->hex = PyNumber_Add(self->center, scaled); // No incref needed because Py_Number_Add returns a new reference
	Py_DECREF(radius); // decref all of our temp variables
    Py_DECREF(direction);
    Py_DECREF(scaled);
	return 0;
}

static PyObject *hex_range_iter_next(hexrangeiter_t *self)
{

    self->pos++; // pos is the index along a side of a hexagonal ring
    if (self->pos >= self->radius)  // its length is equal to the radius of the ring
    {                               // so check if we've run off the end and if so change direction
        self->pos = 0;
        self->dir++;
    }
    
    if (self->dir >= 6)             // If we've already done all 6 sides
    {
        self->dir = 0;              // reset our direction
        self->distance++;           // and increase our radius

        if (self->distance >= self->radius) // If we've reached our max radius
            return NULL;                    // return NULL to exit generator
                                            // Returning NULL in this case is enough. The next() builtin will raise the
                                            // StopIteration error for us.

        PyObject *radius = PyLong_FromLong(self->distance); // calculate our new starting position  v
        PyObject *direction = PyObject_CallMethod(self->center, "direction", "i", 4);
        PyObject *scaled = PyNumber_Multiply(direction, radius); 
        Py_DECREF(self->hex); // make sure to decref our unused hex from last call
        self->hex = PyNumber_Add(self->center, scaled);  // finish calc and save new hex            ^
        Py_DECREF(radius);  // decref all our temp variables
        Py_DECREF(direction);
        Py_DECREF(scaled);
    }

    PyObject *ret = self->hex; // grab a ref to our return value
    self->hex = PyObject_CallMethod(ret, "neighbor", "i", self->dir); // store the next hex in our sequence
       
    return (ret);
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
    0,            /* tp_alloc */
    hex_range_iter_new,                  /* tp_new */
};
