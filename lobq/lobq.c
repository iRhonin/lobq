#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include "structmember.h"


//  Mod operator
inline int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}


// LobQ Object
typedef struct {
    PyObject_HEAD // This should be in the begining of every python object
    PyTupleObject *ob_item; // Internal array
    int cap; // Max queue size 
    int head; // Index of head in the internal array
    int tail; // Like above
} LobQ;


// Check index valid
static int
valid_index(Py_ssize_t i, Py_ssize_t limit)
{
    /* The cast to size_t lets us use just a single comparison
       to check whether i is in the range: 0 <= i < limit
       becuase i could be negetive.*/
    return (size_t) i < (size_t) limit;
}


//  __del__(self)
static void
LobQ_dealloc(LobQ *self)
{
    PyObject_GC_UnTrack(self); // disable GC for self
    Py_TYPE(self->ob_item)->tp_free(self->ob_item); // Free intrenal array
    Py_TYPE(self)->tp_free(self);
}


//  __new__(cls)
static PyObject *
LobQ_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    LobQ *self = (LobQ *) type->tp_alloc(type, 0);
    if (self != NULL) // Nothing to do yet
    {
    }
    return (PyObject*) self;
}


//  __init__(self, cap)
static int
LobQ_init(LobQ *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"cap", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "i", kwlist, 
            &self->cap)) // Parsing the input
        return 0;

    self->ob_item = (PyObject*) PyTuple_New(self->cap); // Initialzing interal array
    if(!self->ob_item)
        return -1;

    self->head = -1;
    self->tail = 0;
    return 0;
}


//  Member registry
static PyMemberDef LobQ_members[] = {
    {"cap", T_INT, offsetof(LobQ, cap), 0, "LobQ cap"}, // self.cap
    {NULL}  // Sentinel
};


// __len__(self)
static Py_ssize_t
LobQ_size(LobQ *self)
{
    if(self->head == -1)
        return 0;
    else if(self->head >= self->tail)
        return self->head - self->tail + 1;
    else
        return self->cap;
}


// __getitem__(self, i)
static PyObject *
LobQ_item(LobQ *self, Py_ssize_t i)
{
    if(!valid_index(i, LobQ_size(self)))
    {
        PyErr_SetString(PyExc_IndexError, "Index out of range");
        return NULL;
    }

    i = mod(self->head - i, self->cap);
    PyObject *item = PyTuple_GetItem(self->ob_item, i);
    return Py_BuildValue("O", item);
}


//  GC use this dunction to detecte cyclic refrences
static int
LobQ_traverse(LobQ *self, visitproc visit, void *arg)
{
    Py_VISIT(self->ob_item);
    return 0;    
}


// append(self, item)
static PyObject *
LobQ_append(LobQ *self, PyObject *args)
{
    // Validating args
    if(!PyTuple_Check(args) || PyTuple_Size(args) != 1)
    {
        PyErr_SetString(PyExc_TypeError, "You can only pass a signle object");
        return NULL;
    }
    
    //  When the queue is full, drop last object
    if(LobQ_size(self) == self->cap)
    {
        Py_XDECREF(LobQ_item(self, self->tail));
        self->tail = mod(self->tail + 1, self->cap);
    }

    self->head = mod(self->head + 1, self->cap);
    Py_ssize_t index = self->head;

    PyObject * new_item = *((PyTupleObject *)args)->ob_item;
    Py_INCREF(new_item);

    PyTuple_SetItem(self->ob_item, index, new_item);
    Py_RETURN_NONE;
}


// peek(self, time)
static PyObject *
LobQ_peek(LobQ *self, PyObject *args, PyObject *kwds)
{
    char *time = "";
    static char *kwlist[] = {"time", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s", kwlist, 
        &time)
    ) 
        return NULL;

    for(Py_ssize_t i = LobQ_size(self);	--i >= 0;) 
    {
        PyObject *item = LobQ_item(self, i);
        if(!strcmp(
            PyUnicode_AsUTF8(PyDict_GetItemString(item, "time")),
            time)
          )
            return Py_BuildValue("O", item);
    }
    Py_RETURN_NONE;
}


// Registering append and peek methods
static PyMethodDef LobQ_methods[] = {
    {"append", (PyCFunctionWithKeywords) LobQ_append, METH_VARARGS},
    {"peek", (PyCFunctionWithKeywords) LobQ_peek, METH_VARARGS | METH_KEYWORDS},
    {NULL}  /* Sentinel */
};


// registring __len__ and __getitem__ methods
static PySequenceMethods LobQ_as_sequence = {
    .sq_length = (lenfunc)LobQ_size,
    .sq_item = (ssizeargfunc)LobQ_item,
};


// Defining LobQ Type
static PyTypeObject LobQType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "lobq.LobQ",
    .tp_basicsize = sizeof(LobQ),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    .tp_new = LobQ_new,
    .tp_init = (initproc) LobQ_init,
    .tp_dealloc = (destructor) LobQ_dealloc,
    .tp_traverse = (traverseproc) LobQ_traverse,
    .tp_members = LobQ_members,
    .tp_methods = LobQ_methods,
    .tp_as_sequence = &LobQ_as_sequence,
};


// Defining lobq module
static struct PyModuleDef lobq = {
    PyModuleDef_HEAD_INIT,
    "lobq",
    "A fast queue for LOB items",
    -1,
};


// Initializing lobq and registering LobQ type
PyMODINIT_FUNC
PyInit_lobq(void)
{
    PyObject *m;
    if (PyType_Ready(&LobQType) < 0)
        return NULL;

    m = PyModule_Create(&lobq);
    if (m == NULL)
        return NULL;

    Py_INCREF(&LobQType);
    PyModule_AddObject(m, "LobQ", (PyObject *) &LobQType);

    if (!PyEval_ThreadsInitialized())
        PyEval_InitThreads();
    
    return m;
}

