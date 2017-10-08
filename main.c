#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void freeMatrix(int row, int col, double **matrix)
{
    for (int i = 0; i < (row * col); i++)
     {
         free(matrix[i]);
     }
     free(matrix);
}

void printMatrix(int row, int col, double **matrix)
{
    printf("\nRows: %d\n", row);
    printf("Columns: %d\n", col);
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
            printf("%.2lf ", matrix[i][j]);
        printf("\n");
    }
    printf("\n");

}

double **get_matrix(FILE *file, int row, int col)       //создает матрицу из файла
{
    double **matrix = (double**) malloc((row * col) * sizeof(double*));
    for (int i = 0; i < (row * col); i++)
    {
        matrix[i] = (double*) malloc((row * col) * sizeof(double));
    }

    double buff = 0.0;

    while (!feof(file))
    {
        for(int i = 0; i < row; i++)
        {
            for(int j = 0; j < col; j++)
            {

                fscanf(file, "%lf" , &buff);
                if(buff > (sizeof(double)-1))
                {
                    printf("\nElement is too big!\n");
                    exit(1);
                }
                matrix[i][j] = buff;
            }
        }
    }
    return matrix;
}

double **multiply(double **amatrix, double **bmatrix, int aRow, int bCol, int aCol)     //возвращает матрицу, являющуюся результатом умножения amatrix и bmatrix
{
    double **cmatrix = (double**) malloc((aRow * bCol) * sizeof(double*));
    for (int i = 0; i < (aRow * bCol); i++)
    {
        cmatrix[i] = (int*) malloc((aRow * bCol) * sizeof(int));
    }

    for(int i = 0; i < aRow; i++)
    {
        for(int j = 0; j < bCol; j++)
        {
            cmatrix[i][j] = 0;
            for(int k = 0; k < aCol; k++)
            {
                cmatrix[i][j] += amatrix[i][k] * bmatrix[k][j];
            }
        }
    }
    return cmatrix;
}

#define MAX_PATH_SIZE 100

int main()
{
    char filePath[MAX_PATH_SIZE];
    int N = 0;

    printf("Enter amount of files: ");
    scanf("%d", &N);

    if(N <= 1)
    {
        printf("\nNothing to multiply! Press enter to exit...\n");
        exit(0);
    }

    double **amatrix = NULL;
    double **bmatrix = NULL;
    double **cmatrix = NULL;

    int aRow = 0;       //количество строк для матрицы А (то же самое для матрицы В)
    int aCol = 0;       //количество столбцов для матрицы А (то же самое для матрицы В)
    int bRow = 0;
    int bCol = 0;

    for (int i = 0; i < N; i++)
    {
        printf("Enter the path of the %d file: ", i+1);
        scanf("%s", &filePath);

        FILE *file = fopen(filePath, "r");

        if (!file)
        {
            printf("\nCan't open file! Errno: %d\n", ENOENT);
            exit(1);
        }

        switch (i) {
        case 0:
            fscanf(file, "%d", &aRow);
            fscanf(file, "%d", &aCol);
            amatrix = get_matrix(file, aRow, aCol);
            break;
        default:
            fscanf(file, "%d", &bRow);
            fscanf(file, "%d", &bCol);

            if(aCol != bRow)    //проверка возможности умножения
            {
                printf("\nCan't multiply!\n");
                exit(1);
            }

            bmatrix = get_matrix(file, bRow, bCol);
            cmatrix = multiply(amatrix, bmatrix, aRow, bCol, aCol);
            amatrix = cmatrix;
            break;
        }

        printMatrix(aRow, aCol, amatrix);
        fclose(file);
    }
    printf("RESULT: \n");
    printMatrix(aRow, bCol, cmatrix);

    freeMatrix(aRow, aCol, amatrix);
    freeMatrix(bRow, bCol, bmatrix);
    freeMatrix(aRow, bCol, cmatrix);
}
