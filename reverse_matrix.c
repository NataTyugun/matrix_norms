//обратная матрица(для поиска обусловленности матрицы)

#include<stdio.h>
int main()
{
    int matrix[20][20], r,a;
    int i, j, k, m;
    scanf_s("%d", &m); //ввести сначала размерность матрицы, затем в той же строке элементы по порядку
    for(i = 0; i < m; i++)
    {
        for(j = 0; j < m; j++)
        {
            scanf_s("%i", &matrix[i][j]);
        }
    }
    for(i = 0; i < m; i++)
    {
        for(j = m; j < 2*m; j++)
        {
            if(i == (j - m))
                matrix[i][j] = 1;
            else
                matrix[i][j] = 0;
        }
    }
    for(i = 0; i < m; i++)
    {
        for(j = 0; j < m; j++)
        {
            if(i != j)
            {
                r = matrix[j][i]/matrix[i][i];
                for(k = 0; k < 2*m; k++)
                {
                    matrix[j][k] -= r * matrix[i][k];
                }
            }
        }
    }
    for(i = 0; i < m; i++)
    {
        a = matrix[i][i];
        for(j = 0; j < 2*m; j++)
        {
            matrix[i][j] /= a;
        }
    }
    for(i = 0; i < m; i++)
    {
        for(j = m; j < 2*m; j++)
        {
            printf("%d ", matrix[i][j]);

        }
    }
    return 0;
}
