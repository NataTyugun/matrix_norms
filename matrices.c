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

/*обусловленность 1 по первой норме*/
static PyObject* cond_1(PyObject* self, PyObject* args) {
    PyObject* list;
    int i, j, n, m, k;

    if(!PyArg_ParseTuple(args,"O", &list)) {
        return NULL;
    }

    n = PyList_GET_SIZE(list);
    m = PyList_GET_SIZE(PySequence_Fast_GET_ITEM(list, 0));

    double sum_1 = 0;
    double max_1 = 0;
    double elem;

    /*норма для исходной матрицы*/
    for (i=0; i<m; i++ ){
        for ( j=0; j<n; j++ ){
            PyObject* tempi = PySequence_Fast_GET_ITEM(list, j);
            PyObject* tempj = PySequence_Fast_GET_ITEM(tempi, i);
            elem = PyFloat_AsDouble(tempj);
            sum_1 += abs(elem);
        }
        if (max_1 < sum_1){
            max_1 = sum_1;
        }
        sum_1 = 0;
    }

    /*вычисление обратной матрицы*/

    if (n != m) {
        PyErr_SetString(PyExc_TypeError, "The matrix is not square");/*проверка на квадратность*/
        return NULL;
    }

    const double EPS = 1E-9;
    double det = 1;

    PyObject* a = PyList_New(n);

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

    /*считаем норму для обратной матрицы*/
    double sum_2 = 0;
    double max_2 = 0;

    for (i=0; i<m; i++ ){
        for ( j=0; j<n; j++ ){
            PyObject* tempi = PySequence_Fast_GET_ITEM(NEW_MATRIX, j);
            PyObject* tempj = PySequence_Fast_GET_ITEM(tempi, i);
            elem = PyFloat_AsDouble(tempj);
            sum_2 += abs(elem);
        }
        if (max_2 < sum_2){
            max_2 = sum_2;
        }
        sum_2 = 0;
    }

    return PyFloat_FromDouble(max_1*max_2);
}

/*обусловленность 2 по Евклидовой норме*/
static PyObject* cond_2(PyObject* self, PyObject* args) {
    PyObject* list;
    int i, j, n, m, k;

    if(!PyArg_ParseTuple(args,"O", &list)) {
        return NULL;
    }

    n = PyList_GET_SIZE(list);
    m = PyList_GET_SIZE(PySequence_Fast_GET_ITEM(list, 0));

    /*вычисляем норму для исходной матрицы*/
    double sum_1 = 0;
    double elem;

    for (i=0; i<m; i++ ){
        PyObject* tempi = PySequence_Fast_GET_ITEM(list, i);
        for ( j=0; j<n; j++ ){
            PyObject* tempj = PySequence_Fast_GET_ITEM(tempi, j);
            elem = PyFloat_AsDouble(tempj);
            sum_1 += (abs(elem) * abs(elem));
        }
    }

    /*вычисляем обратную матрицу*/
    if (n != m) {
        PyErr_SetString(PyExc_TypeError, "The matrix is not square");
        return NULL;
    }

    const double EPS = 1E-9;
    double det = 1;

    PyObject* a = PyList_New(n);

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

    double sum_2 = 0;

    /*вычисляем норму обратной матрицы*/
    for (i=0; i<m; i++ ){
        PyObject* tempi = PySequence_Fast_GET_ITEM(NEW_MATRIX, i);
        for ( j=0; j<n; j++ ){
            PyObject* tempj = PySequence_Fast_GET_ITEM(tempi, j);
            elem = PyFloat_AsDouble(tempj);
            sum_2 += (abs(elem) * abs(elem));
        }
    }

    return PyFloat_FromDouble(sqrt(sum_1)*sqrt(sum_2));
}

/*обусловленноть 3 по норме бесконечность*/
static PyObject* cond_3(PyObject* self, PyObject* args) {
    PyObject* list;
    int i, j, n, m, k;

    if(!PyArg_ParseTuple(args,"O", &list)) {
        return NULL;
    }

    n = PyList_GET_SIZE(list); /* количество строк */
    m = PyList_GET_SIZE(PySequence_Fast_GET_ITEM(list, 0)); /* количество столбцов */

    double sum_1 = 0;
    double max_1 = 0;
    double elem;

    for (i=0; i<n; i++ ){
        PyObject* tempi = PySequence_Fast_GET_ITEM(list, i); /* перебираем строки */
        for ( j=0; j<m; j++ ){ /* перебираем элементы в строке */
            PyObject* tempj = PySequence_Fast_GET_ITEM(tempi, j);
            elem = PyFloat_AsDouble(tempj);
            sum_1 += abs(elem);
        }
        if (max_1 < sum_1){
            max_1 = sum_1;
        }
        sum_1 = 0;
    }

/*вычисляем обратную матрицу*/
    if (n != m) {
        PyErr_SetString(PyExc_TypeError, "The matrix is not square");
        return NULL;
    }

    const double EPS = 1E-9;
    double det = 1;

    PyObject* a = PyList_New(n);

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

    /*вычисляем норму для обратной матрицы*/
    double sum_2 = 0;
    double max_2 = 0;

    for (i=0; i<n; i++ ){
        PyObject* tempi = PySequence_Fast_GET_ITEM(NEW_MATRIX, i);
        for ( j=0; j<m; j++ ){
            PyObject* tempj = PySequence_Fast_GET_ITEM(tempi, j);
            elem = PyFloat_AsDouble(tempj);
            sum_2 += abs(elem);
        }
        if (max_2 < sum_2){
            max_2 = sum_2;
        }
        sum_2 = 0;
    }

    return PyFloat_FromDouble(max_1*max_2);
}

/*------------------------------------------------------------------------------------------*/

/* Описывает методы модуля */
static PyMethodDef ownmod_methods[] = {
    {
    "infinity",          /* название функции внутри python*/
     infinity_norm,        /* функция C */
     METH_VARARGS,   /* макрос, поясняющий, что функция у нас с аргументами */
    },

    {
    "first",
     first_norm,
     METH_VARARGS,
    },

    {
    "euclidean",
     euclidean_norm,
     METH_VARARGS,
    },

    {
    "inversion",
     inversion,
     METH_VARARGS,
    },

    {
    "cond_1",
     cond_1,
     METH_VARARGS,
    },

    {
    "cond_2",
     cond_2,
     METH_VARARGS,
    },

    {
    "cond_3",
     cond_3,
     METH_VARARGS,
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
