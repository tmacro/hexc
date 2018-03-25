#include "hexc.h"

// extern PyTypeObject AbstractHexType;

static PyModuleDef hexmodule = {
    PyModuleDef_HEAD_INIT,
    "hexc",
    "A module for manipulating 3 dimensional hexagonal maps.",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_hexc(void)
{
    PyObject* m;

    // if (PyType_Ready(&HexGridType) < 0)
    //     return NULL;

    if (PyType_Ready(&AbstractHexType) < 0)
        return NULL;
    
    if (PyType_Ready(&HexType) < 0)
        return NULL;

    if (PyType_Ready(&HexRingGenType) < 0)
        return NULL;
    
    if (PyType_Ready(&HexRangeGenType) < 0)
        return NULL;

    if (PyType_Ready(&StackType) < 0)
        return NULL;
    
    // if (PyType_Ready(&HexGridType) < 0)
    //     return NULL;

    m = PyModule_Create(&hexmodule);
    if (m == NULL)
        return NULL;

	Py_INCREF(&AbstractHexType);
	Py_INCREF(&HexType);
	Py_INCREF(&HexRingGenType);
	Py_INCREF(&HexRangeGenType);
	Py_INCREF(&StackType);
    PyModule_AddObject(m, "AbstractHex", (PyObject *)&AbstractHexType);
    PyModule_AddObject(m, "Hex", (PyObject *)&HexType);
    PyModule_AddObject(m, "HexRingGenerator", (PyObject *)&HexRingGenType);
    PyModule_AddObject(m, "HexRangeGenerator", (PyObject *)&HexRangeGenType);
    PyModule_AddObject(m, "Stack", (PyObject *)&StackType);
    // Py_INCREF(&HexGridType);
    // PyModule_AddObject(m, "Grid", (PyObject *)&HexGridType);

    return m;
}