#include "hexc.h"

// Define class boilerplate functions
static PyObject *hex_range_iter_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	hexrangeiter_t *self;

	self = (hexrangeiter_t *)(type->tp_alloc(type, 0));
	if (self != NULL) {
		self->pos = -1;
		self->dir = -1;
		self->distance = 0;
		self->radius = 0;
		self->hex = NULL;
		self->center = NULL;
	}

	return (PyObject *)self;
}

static void hex_range_iter_dealloc(hexrangeiter_t *self)
{
	Py_XDECREF(self->hex);	// decref our stored hex
	Py_XDECREF(self->center); // and our center point
	Py_TYPE(self)->tp_free((PyObject*)self); // SIGSEGV here	
}

static int hex_range_iter_init(hexrangeiter_t *self, PyObject *args, PyObject *kwds)
{
	// Takes 2 args, the center of the ring as an AbstractHex, and the radius as an int
	static char *kwlist[] = {"center", "radius", NULL};
	if (! PyArg_ParseTupleAndKeywords(args, kwds, "Oi", kwlist, &self->center, &self->radius))
		return -1;

	Py_INCREF(self->center); //increcf here because Py_Arg_Parse... returns borrowed references
	return 0;
}

static PyObject *get_hex_ring_start(PyObject *center, long radius)
{
	PyObject *rad;
	PyObject *direction;
	PyObject *scaled;
	PyObject *ret;

	rad = PyLong_FromLong(radius);
	direction = PyObject_CallMethod(center, "direction", "i", 4);
	scaled = PyNumber_Multiply(direction, rad);
	ret = PyNumber_Add(center, scaled);
	Py_DECREF(rad);
	Py_DECREF(direction);
	Py_DECREF(scaled);
	return (ret);
}

static PyObject *hex_range_iter_next(hexrangeiter_t *self)
{
	if (self->dir == -1)
	{
		self->dir = 6;
		Py_INCREF(self->center);
		return self->center;
	}

	if (self->pos >= self->distance)	// its length is equal to the radius of the ring
	{								// so check if we've run off the end and if so change direction
		// printf("pos: %i / dist: %i / dir: %i\n", self->pos, self->dir, self->distance); 
		self->pos = 0;
		self->dir++;
	}

	if (self->dir >= 6 || (self->dir == 5 && self->pos >= self->distance - 1))// If we've already done all 6 sides or this is the first call
	{
		self->dir = 0;				// reset our direction
		self->pos = -1;				// our position along the side
		self->distance++;			// and increase our radius

		if (self->distance > self->radius) // If we've reached our max radius
			return NULL;					// return NULL to exit generator
											// Returning NULL in this case is enough. The next() builtin will raise the
											// StopIteration error for us.
		Py_XDECREF(self->hex); // make sure to decref our unused hex from last call
		self->hex = get_hex_ring_start(self->center, self->distance);
		// Py_INCREF(self->hex);
		// PyObject *radius = PyLong_FromLong(self->distance); // calculate our new starting position	v
		// PyObject *direction = PyObject_CallMethod(self->center, "direction", "i", 4);
		// PyObject *scaled = PyNumber_Multiply(direction, radius); 
		// self->hex = PyNumber_Add(self->cen/ter, scaled);	// finish calc and save new hex				^
		// Py_DECREF(radius);								// we don't need an incref because PyNumber_Add return a new reference
		// Py_DECREF(direction); // decref our temp variables
		// Py_DECREF(scaled);

	} 
	else
	{
		PyObject *tmp = self->hex;		
		self->hex = PyObject_CallMethod(self->hex, "neighbor", "i", self->dir); // simple move along the side in our direction
		Py_XDECREF(tmp);
	}

	self->pos++;
	Py_INCREF(self->hex); // incref here because we're storing a ref and we also want to give the caller a new ref
	return (self->hex);
}

PyTypeObject HexRangeGenType = {
	PyVarObject_HEAD_INIT(&PyType_Type, 0)
	"HexRangeGenerator",			 /* tp_name */
	sizeof(hexrangeiter_t),			/* tp_basicsize */
	0,								/* tp_itemsize */
	(destructor)hex_range_iter_dealloc,	/* tp_dealloc */
	0,								/* tp_print */
	0,								/* tp_getattr */
	0,								/* tp_setattr */
	0,								/* tp_reserved */
	0,				 /* tp_repr */
	0,								/* tp_as_number */
	0,								/* tp_as_sequence */
	0,								/* tp_as_mapping */
	0,								/* tp_hash */
	0,								/* tp_call */
	0,								/* tp_str */
	0,								/* tp_getattro */
	0,								/* tp_setattro */
	0,								/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,			 /* tp_flags */
	0,								/* tp_doc */
	0,								/* tp_traverse */
	0,								/* tp_clear */
	0,								/* tp_richcompare */
	0,								/* tp_weaklistoffset */
	PyObject_SelfIter,				/* tp_iter */
	(iternextfunc)hex_range_iter_next,	/* tp_iternext */
	0,								/* tp_methods */
	0,								/* tp_members */
	0,								/* tp_getset */
	0,								/* tp_base */
	0,								/* tp_dict */
	0,								/* tp_descr_get */
	0,								/* tp_descr_set */
	0,								/* tp_dictoffset */
	(initproc)hex_range_iter_init,				 /* tp_init */
	0,			/* tp_alloc */
	hex_range_iter_new,					/* tp_new */
};
