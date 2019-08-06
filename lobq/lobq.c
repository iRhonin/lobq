#define PY_SSIZE_T_CLEAN
#define Index(i, q) ((q->head + i) % q->cap)

#include <Python.h>
#include "structmember.h"

int
modulo(int a, int b) {
    int m = a % b;
    if (m < 0) {
      m = (b < 0) ? m - b : m + b;
    }
    return m;
}

typedef struct {
    PyObject_HEAD
    int cap;
    PyObject *ob_item;
    int head;
    int tail;
} LobQ;

static int
valid_index(Py_ssize_t i, Py_ssize_t limit)
{
    /* The cast to size_t lets us use just a single comparison
       to check whether i is in the range: 0 <= i < limit */
    return (size_t) i < (size_t) limit;
}



static int
LobQ_clear(LobQ *self)
{
    return 0;
}

static void
LobQ_dealloc(LobQ *self)
{
    PyObject_GC_UnTrack(self);
    LobQ_clear(self);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *
LobQ_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    LobQ *self = (LobQ *) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->head = -1;
        self->tail = 0;
    }
    return (PyObject*) self;
}

static int
LobQ_init(LobQ *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"cap", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "i", kwlist, 
            &self->cap))
        return NULL;

    self->ob_item = PyTuple_New(self->cap);
    if(!self->ob_item)
        return -1;

    return 0;
}

static PyMemberDef LobQ_members[] = {
    {"cap", T_INT, offsetof(LobQ, cap), 0,
     "LobQ cap"},
    {NULL}  /* Sentinel */
};

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

static PyObject *
LobQ_item(LobQ *self, Py_ssize_t i)
{
    if(!valid_index(i, LobQ_size(self)))
    {
        PyErr_SetString(PyExc_IndexError, "queue index out of range");
        return NULL;
    }

    i = modulo(self->head - i, self->cap);
    printf("i: %d\n", i);
    return Py_BuildValue("O", PyTuple_GetItem(self->ob_item, i));
}

static int
LobQ_traverse(LobQ *self, visitproc visit, void *arg)
{
    Py_ssize_t i;

    for (i = LobQ_size(self); --i > 0; )
        Py_VISIT(self->ob_item[i]);
    return 0;    
}

static PyObject *
LobQ_append(LobQ *self, PyObject *args, PyObject *kwds)
{
    if(LobQ_size(self) == self->cap)
    {
        Py_DECREF(LobQ_item(self, 1));
        self->tail = modulo(self->tail + 1, self->cap);
    }

    Py_ssize_t argc = PyTuple_GET_SIZE(args);  
    assert(argc == 1);
    self->head = modulo(self->head + 1, self->cap);
    PyObject * new_item = *((PyTupleObject *)args)->ob_item;
    Py_INCREF(new_item);
    PyTuple_SetItem(self->ob_item, self->head, new_item);
    return Py_BuildValue("O", Py_None);
}

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
        //if(!PyDict_Check(self->ob_item[i]))
        //	return NULL;
        PyObject *item = LobQ_item(self, i);
        if(!strcmp(
            PyUnicode_AsUTF8(PyDict_GetItemString(item, "time")),
            time)
          )
            return Py_BuildValue("O", item);
    }
    Py_INCREF(Py_None);
    return Py_BuildValue("O", Py_None);
}

static Py_ssize_t 
LobQ_len(LobQ *self)
{
    return LobQ_size(self);
}

static PyObject *
LobQ_echo(LobQ *self, PyObject *args)
{
    PyObject * py_obj = Py_None;
    if(!PyArg_ParseTuple(args, "O", py_obj))
        return NULL;

    return Py_BuildValue("O", *py_obj);
}

static PyMethodDef LobQ_methods[] = {
    {"append", (PyCFunctionWithKeywords) LobQ_append, METH_VARARGS | METH_KEYWORDS},
    {"peek", (PyCFunctionWithKeywords) LobQ_peek, METH_VARARGS | METH_KEYWORDS},
    {"echo", (PyCFunction) LobQ_echo, METH_VARARGS},
    {NULL}  /* Sentinel */
};


static PySequenceMethods LobQ_as_sequence = {
    .sq_length = (lenfunc)LobQ_len,
    .sq_item = (ssizeargfunc)LobQ_item,
};

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
    .tp_clear = (inquiry) LobQ_clear,
    .tp_members = LobQ_members,
    .tp_methods = LobQ_methods,
    .tp_as_sequence = &LobQ_as_sequence,
};

static struct PyModuleDef lobq = {
    PyModuleDef_HEAD_INIT,
    "lobq",
    "fast queue",
    -1,
};

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

    return m;
}

