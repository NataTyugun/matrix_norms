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

    if(!PyArg_ParseTuple(args,"O", &list)) {
        return NULL;
    }

    n = PyList_GET_SIZE(list);
    m = PyList_GET_SIZE(PySequence_Fast_GET_ITEM(list, 0));

    double sum = 0;
    double max = 0;
    double elem;

    for (i=0; i<m; i++ ){
        for ( j=0; j<n; j++ ){
            PyObject* tempi = PySequence_Fast_GET_ITEM(list, j);
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

    if(!PyArg_ParseTuple(args,"O", &list)) {
        return NULL;
    }

    n = PyList_GET_SIZE(list);
    m = PyList_GET_SIZE(PySequence_Fast_GET_ITEM(list, 0));

    double sum = 0;
    double elem;

    for (i=0; i<m; i++ ){
        PyObject* tempi = PySequence_Fast_GET_ITEM(list, i);
        for ( j=0; j<n; j++ ){
            PyObject* tempj = PySequence_Fast_GET_ITEM(tempi, j);
            elem = PyFloat_AsDouble(tempj);
            sum += (abs(elem) * abs(elem));
            }
        }
    return PyFloat_FromDouble(sqrt(sum));
}

/*обратная матрица*/
static PyObject* inversion(PyObject* self, PyObject* args) {
    PyObject *list;

    if(!PyArg_ParseTuple(args,"O", &list)) {
        return NULL;
    }

    int n, m;
    n = PyList_GET_SIZE(list);
    m = PyList_GET_SIZE(PySequence_Fast_GET_ITEM(list, 0));

    /*проверка на квадратность*/
    if (n != m) {
        PyErr_SetString(PyExc_TypeError, "The matrix is not square");
        return NULL;
    }

    const double EPS = 1E-9;
    double det = 1;

    PyObject* a = PyList_New(n);
    long i,j,k;

    for( i=0; i<n; i++ ) {
        PyObject* row = PyList_GetItem(list, i);
        PyObject* r = PyList_New(n);

        for( j=0; j<n; j++ )
        PyList_SetItem(r, j, PyList_GetItem(row, j));
        PyList_SetItem(a, i, r);
    }

    PyObject* NEW_MATRIX = PyList_New(n);
    for( i=0; i<n; i++ ) {
        PyObject* r = PyList_New(n);
        for( j=0; j<n; j++ )
            if( i == j ) {
                PyList_SetItem(r, j, PyFloat_FromDouble(1));
            }
            else {
                PyList_SetItem(r, j, PyFloat_FromDouble(0));
            }
            PyList_SetItem(NEW_MATRIX, i, r);
        }

    double temp1, temp2, temp3;

    for(i=0; i<n; i++) {
        k = i;
        for(j=i+1; j<n; j++) {
            temp1 = PyFloat_AsDouble( PyList_GetItem(PyList_GetItem(a, j), i) );
            temp2 = PyFloat_AsDouble( PyList_GetItem(PyList_GetItem(a, k), i) );
            if (fabs(temp1) > fabs(temp2)) k = j;
        }

        temp1 = PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(a, k), i));
        if(fabs(temp1) < EPS) {
            PyErr_SetString(PyExc_ValueError, "The discriminant is zero");
            return NULL;
        }

        if(i != k) {
            det = -det;
            for(j=0; j<n; j++) {
                temp1 = PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(a, i), j));
                temp2 = PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(a, k), j));
                PyList_SetItem(PyList_GetItem(a, i), j, PyFloat_FromDouble(temp2));
                PyList_SetItem(PyList_GetItem(a, k), j, PyFloat_FromDouble(temp1));

                temp1 = PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(NEW_MATRIX, i), j));
                temp2 = PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(NEW_MATRIX, k), j));
                PyList_SetItem(PyList_GetItem(NEW_MATRIX, i), j, PyFloat_FromDouble(temp2));
                PyList_SetItem(PyList_GetItem(NEW_MATRIX, k), j, PyFloat_FromDouble(temp1));
            }
        }

        temp1 = PyFloat_AsDouble( PyList_GetItem(PyList_GetItem(a, i), i));
        det *= temp1;

        for(j=i; j<n; j++) {
            temp2 = PyFloat_AsDouble( PyList_GetItem(PyList_GetItem(a, i), j));
            PyList_SetItem( PyList_GetItem(a, i), j, PyFloat_FromDouble(temp2/temp1));
        }

        for(j=0; j<n; j++) {
            temp2 = PyFloat_AsDouble( PyList_GetItem(PyList_GetItem(NEW_MATRIX, i), j));
            PyList_SetItem( PyList_GetItem(NEW_MATRIX, i), j, PyFloat_FromDouble(temp2/temp1));
        }

        for( j=i+1; j<n; j++ ) {
            temp1 = PyFloat_AsDouble( PyList_GetItem(PyList_GetItem(a, j), i) );
            if( fabs(temp1) > EPS ) {
                for( k=0; k<n; k++ ) {
                    temp2 = PyFloat_AsDouble( PyList_GetItem(PyList_GetItem(a, i), k) );
                    temp3 = PyFloat_AsDouble( PyList_GetItem(PyList_GetItem(a, j), k) );
                    PyList_SetItem( PyList_GetItem(a, j), k, PyFloat_FromDouble(temp3-temp2*temp1) );

                    temp2 = PyFloat_AsDouble( PyList_GetItem(PyList_GetItem(NEW_MATRIX, i), k) );
                    temp3 = PyFloat_AsDouble( PyList_GetItem(PyList_GetItem(NEW_MATRIX, j), k) );
                    PyList_SetItem( PyList_GetItem(NEW_MATRIX, j), k, PyFloat_FromDouble(temp3-temp2*temp1) );
                }
            }
        }
    }

    for(i = n-1; i > 0; i--) {
        for(int k = 0; k < i; k++) {
            temp1 = PyFloat_AsDouble( PyList_GetItem(PyList_GetItem(a, k), i) );
            if (fabs(temp1) > EPS) {
                for (j = i-1; j < n; j++) {
                    temp2 = PyFloat_AsDouble( PyList_GetItem(PyList_GetItem(a, i), j) );
                    temp3 = PyFloat_AsDouble( PyList_GetItem(PyList_GetItem(a, k), j) );
                    PyList_SetItem( PyList_GetItem(a, k), j, PyFloat_FromDouble(temp3-temp2*temp1) );
                }

                for (int j = 0; j < n; j++){
                    temp2 = PyFloat_AsDouble( PyList_GetItem(PyList_GetItem(NEW_MATRIX, i), j) );
                    temp3 = PyFloat_AsDouble( PyList_GetItem(PyList_GetItem(NEW_MATRIX, k), j) );
                    PyList_SetItem( PyList_GetItem(NEW_MATRIX, k), j, PyFloat_FromDouble(temp3-temp2*temp1) );
                }
            }
        }
    }

   return NEW_MATRIX;
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
