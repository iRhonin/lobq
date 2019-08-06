#define PY_SSIZE_T_CLEAN
#define Index(i, q) ((q->head + i) % q->capacity)

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
	int capacity;
	PyObject *ob_item;
	int head;
	int tail;
} QueueObject;

static int
valid_index(Py_ssize_t i, Py_ssize_t limit)
{
    /* The cast to size_t lets us use just a single comparison
       to check whether i is in the range: 0 <= i < limit */
    return (size_t) i < (size_t) limit;
}

static int
Queue_traverse(QueueObject *self, visitproc visit, void *arg)
{
    return 0;
}

static int
Queue_clear(QueueObject *self)
{
    return 0;
}

static void
Queue_dealloc(QueueObject *self)
{
    PyObject_GC_UnTrack(self);
    Queue_clear(self);
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static PyObject *
Queue_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    QueueObject *self;
    self = (QueueObject *) type->tp_alloc(type, 0);
    if (self != NULL) {
		self->head = -1;
		self->tail = 0;
    }
    return (PyObject *) self;
}

static int
Queue_init(QueueObject *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {"capacity", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "i", kwlist, 
			&self->capacity))
        return NULL;

	self->ob_item = PyTuple_New(self->capacity);
 	if(!self->ob_item)
		return -1;

    return 0;
}

static PyMemberDef Queue_members[] = {
    {"capacity", T_INT, offsetof(QueueObject, capacity), 0,
     "Queue capacity"},
    {NULL}  /* Sentinel */
};

static Py_ssize_t
Queue_size(QueueObject *self)
{
	if(self->head == -1)
		return 0;
	else if(self->head >= self->tail)
		return self->head - self->tail + 1;
	else
		return self->capacity;
}

static PyObject *
Queue_append(QueueObject *self, PyObject *args, PyObject *kwds)
{
//	assert(args != NULL);
//    if (!PyTuple_Check(args)) {
//        PyErr_SetString(PyExc_SystemError,
//            "Only lists are acceptable");
//		return NULL;//Py_BuildValue("O", &argc);
//    }
//
	if(Queue_size(self) == self->capacity)
	{
		self->tail = modulo(self->tail + 1, self->capacity);
	}

	Py_ssize_t argc = PyTuple_GET_SIZE(args);  
	assert(argc == 1);
    self->head = modulo(self->head + 1, self->capacity);
	PyObject * new_item = *((PyTupleObject *)args)->ob_item;
	Py_INCREF(new_item);
	PyTuple_SetItem(self->ob_item, self->head, new_item);
	//*self->ob_item[self->head] = malloc(sizeof(PyObject));
//    if (!PyArg_ParseTuple(args, "O", self->ob_item[self->head]))
		//return NULL;//Py_BuildValue("O", &argc);
	//	return -1;
	
		return Py_BuildValue("O", Py_None);
	return Py_BuildValue("O", PyTuple_GetItem(self->ob_item, self->head));
}

static PyObject *
Queue_peek(QueueObject *self, PyObject *args, PyObject *kwds)
{
//	char *time = 0;
//    static char *kwlist[] = {"time", NULL};
//    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s", kwlist, 
//		time)
//	) 
//		return NULL;
//
//	for(
//		Py_ssize_t i = self->tail;
//		i <= self->head;
//		i = modulo(i + 1, self->capacity)
//	) {
//		//if(!PyDict_Check(self->ob_item[i]))
//		//	return NULL;
//
//		if(!strcmp(
//			PyUnicode_AsUTF8(PyDict_GetItemString(self->ob_item[i], "time")),
//			time)
//		  )
//			return Py_BuildValue("O", &self->ob_item[i]);
//	}
	return Py_BuildValue("O", Py_None);
}

static PyObject *
Queue_item(QueueObject *self, Py_ssize_t i)
{
	if(!valid_index(i, Queue_size(self)))
	{
		PyErr_SetString(PyExc_IndexError, "queue index out of range");
        return NULL;
	}

	i = modulo(self->head - i, self->capacity);
	return Py_BuildValue("O", PyTuple_GetItem(self->ob_item, i));
}

static Py_ssize_t 
Queue_len(QueueObject *self)
{
	return Queue_size(self);
}

static PyObject *
Queue_echo(QueueObject *self, PyObject *args)
{
	PyObject * py_obj = Py_None;
	if(!PyArg_ParseTuple(args, "O", py_obj))
		return NULL;
	return Py_BuildValue("O", *py_obj);
}

static PyMethodDef Queue_methods[] = {
    {"append", (PyCFunctionWithKeywords) Queue_append, METH_VARARGS | METH_KEYWORDS},
    {"peek", (PyCFunctionWithKeywords) Queue_peek, METH_VARARGS | METH_KEYWORDS},
    {"echo", (PyCFunction) Queue_echo, METH_VARARGS},
    {NULL}  /* Sentinel */
};


static PySequenceMethods Queue_as_sequence = {
    .sq_length = (lenfunc)Queue_len,
	.sq_item = (ssizeargfunc)Queue_item,
};

static PyTypeObject QueueType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "fastqueue.Queue",
    .tp_basicsize = sizeof(QueueObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
    .tp_new = Queue_new,
    .tp_init = (initproc) Queue_init,
    .tp_dealloc = (destructor) Queue_dealloc,
    .tp_traverse = (traverseproc) Queue_traverse,
    .tp_clear = (inquiry) Queue_clear,
    .tp_members = Queue_members,
    .tp_methods = Queue_methods,
    .tp_as_sequence = &Queue_as_sequence,
};

static struct PyModuleDef fastqueue = {
    PyModuleDef_HEAD_INIT,
    "fastqueue",
    "A fast queue",
    -1,
};

PyMODINIT_FUNC
PyInit_fastqueue(void)
{
    PyObject *m;
    if (PyType_Ready(&QueueType) < 0)
        return NULL;

    m = PyModule_Create(&fastqueue);
    if (m == NULL)
        return NULL;

    Py_INCREF(&QueueType);
    PyModule_AddObject(m, "Queue", (PyObject *) &QueueType);

    return m;
}

