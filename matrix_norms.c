#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include <math.h>

/*норма бесконечность*/
static PyObject* infinity_norm(PyObject *self, PyObject *args){
    PyObject* list;
    int i, j, n, m;

    /* преобразуем тип данных */
    /* PyArg_ParseTuple - обработка аргументов функции и трасформация типы данных Python в типы данных C.
    Первый агрумент PyObject, кортеж переданных функции аргументов,  второй - тип переменных в которые мы хотим преобразовать наши аргументы
    ("O" - произвольный агрумент, то есть будет сохраняться объект Python (без какого-либо преобразования) в указателе объекта C). Далее идут сами
    переменныке в которые мы сохраняем преобразованные значения. Также осуществляется проверка, корректно ли отработала функция. */

    if(!PyArg_ParseTuple(args,"O", &list)) {
        return NULL; /* проверка */
    }

    n = PyList_GET_SIZE(list); /* количество строк */
    m = PyList_GET_SIZE(PySequence_Fast_GET_ITEM(list, 0)); /* количество столбцов */

    double sum = 0;
    double max = 0;
    double elem;

    for (i=0; i<n; i++ ){
        PyObject* tempi = PySequence_Fast_GET_ITEM(list, i); /* перебираем строки */
        for ( j=0; j<m; j++ ){ /* перебираем элементы в строке */
            PyObject* tempj = PySequence_Fast_GET_ITEM(tempi, j);
            elem = PyFloat_AsDouble(tempj);
            sum += abs(elem);
        }
        if (max < sum){
            max = sum;
        }
        sum = 0;
    }
    return PyFloat_FromDouble(max);
}

/*норма первая*/
static PyObject* first_norm(PyObject *self, PyObject *args){
    PyObject* list;
    int i, j, n, m;

    /* преобразуем тип данных */
    /* PyArg_ParseTuple - обработка аргументов функции и трасформация типы данных Python в типы данных C.
    Первый агрумент PyObject, кортеж переданных функции аргументов,  второй - тип переменных в которые мы хотим преобразовать наши аргументы
    ("O" - произвольный агрумент, то есть будет сохраняться объект Python (без какого-либо преобразования) в указателе объекта C). Далее идут сами
    переменныке в которые мы сохраняем преобразованные значения. Также осуществляется проверка, корректно ли отработала функция. */
    if(!PyArg_ParseTuple(args,"O", &list)) {
        return NULL; /* проверка */
    }

    n = PyList_GET_SIZE(list); /* количество строк */
    m = PyList_GET_SIZE(PySequence_Fast_GET_ITEM(list, 0)); /* количество столбцов */

    double sum = 0;
    double max = 0;
    double elem;

    for (i=0; i<m; i++ ){
        for ( j=0; j<n; j++ ){ /* перебираем элементы в строке */
            PyObject* tempi = PySequence_Fast_GET_ITEM(list, j);  /* перебираем строки */
            PyObject* tempj = PySequence_Fast_GET_ITEM(tempi, i);
            elem = PyFloat_AsDouble(tempj);
            sum += abs(elem);
        }
        if (max < sum){
            max = sum;
        }
        sum = 0;
    }
    return PyFloat_FromDouble(max);
}

/* Евклидова норма */
static PyObject* euclidean_norm(PyObject *self, PyObject *args){
    PyObject* list;
    int i, j, n, m;

    /* преобразуем тип данных */
    /* PyArg_ParseTuple - обработка аргументов функции и трасформация типы данных Python в типы данных C.
    Первый агрумент PyObject, кортеж переданных функции аргументов,  второй - тип переменных в которые мы хотим преобразовать наши аргументы
    ("O" - произвольный агрумент, то есть будет сохраняться объект Python (без какого-либо преобразования) в указателе объекта C). Далее идут сами
    переменныке в которые мы сохраняем преобразованные значения. Также осуществляется проверка, корректно ли отработала функция. */
    if(!PyArg_ParseTuple(args,"O", &list)) {
        return NULL; /* проверка */
    }

    n = PyList_GET_SIZE(list); /* количество строк */
    m = PyList_GET_SIZE(PySequence_Fast_GET_ITEM(list, 0)); /* количество столбцов */

    double sum = 0;
    double elem;

    for (i=0; i<m; i++ ){
        PyObject* tempi = PySequence_Fast_GET_ITEM(list, i);  /* перебираем строки */
        for ( j=0; j<n; j++ ){ /* перебираем элементы в строке */
            PyObject* tempj = PySequence_Fast_GET_ITEM(tempi, j);
            elem = PyFloat_AsDouble(tempj);
            sum += (abs(elem) * abs(elem));
            }
        }
    return PyFloat_FromDouble(sqrt(sum));
}

/*------------------------------------------------------------------------------------------*/

/* Описывает методы модуля */
static PyMethodDef ownmod_methods[] = {
    {
    "infinity",          /* название функции внутри python*/
     infinity_norm,        /* функция C */
     METH_VARARGS,   /* макрос, поясняющий, что функция у нас с аргументами */
     "infinity_norm_docs" /* документация для функции внутри python */
    },

    {
    "first",          /* название функции внутри python*/
     first_norm,        /* функция C */
     METH_VARARGS,   /* макрос, поясняющий, что функция у нас с аргументами */
     "first_norm_docs" /* документация для функции внутри python */
    },

    {
    "euclidean",          /* название функции внутри python*/
     euclidean_norm,        /* функция C */
     METH_VARARGS,   /* макрос, поясняющий, что функция у нас с аргументами */
     "euclidean_norm_docs" /* документация для функции внутри python */
    },

    { NULL, NULL, 0, NULL } /*Сколько бы элементов структуры
                            у вас не было, этот нулевой элемент должен быть всегда,
                            и при этом быть последним*/
};

/* Описываем наш модуль */
static PyModuleDef matrix_norms = {
    PyModuleDef_HEAD_INIT,   /* обязательный макрос */
    "matrix_norms",          /* my_plus.__name__*/
    "matrix_norms documentation", /* my_plus.__doc__ */
    -1,
    ownmod_methods  /* сюда передаем методы модуля */
};

/* в названии функции обязательно должен быть префикс PyInit */
PyMODINIT_FUNC PyInit_matrix_norms(void) {
      PyObject* module;
      /* создаем модуль */
      module = PyModule_Create(&matrix_norms);
      /* если все корректно, то эта проверка не проходит */
      if (module == NULL)
          return NULL;
      return module;
}
