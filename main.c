#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_PATH_SIZE 100

void FreeMatrix(int Row, int Col, double **Matrix)
{
    for (int i = 0; i < (Row * Col); i++)
    {
         free(Matrix[i]);
    }
     free(Matrix);
}

void PrintMatrix(int Row, int Col, double **Matrix)
{
    printf("\nRows: %d\n", Row);
    printf("Columns: %d\n", Col);
    for (int i = 0; i < Row; i++)
    {
        for(int j = 0; j < Col; j++)
        {
            printf("%.2lf ", Matrix[i][j]);
        }
        puts("");
    }
}

//создает матрицу из файла
double **GetMatrix(FILE *file)
{
    fseek(file, 0, SEEK_SET);
    int Row = 0;
    int Col = 0;

    fscanf(file, "%d", &Row);
    fscanf(file, "%d", &Col);
    double **Matrix = (double**) malloc((Row * Col) * sizeof(double*));
    for (int i = 0; i < (Row * Col); i++)
    {
        Matrix[i] = (double*) malloc((Row * Col) * sizeof(double));
    }

    double Buff = 0.0;

    while (!feof(file))
    {
        for(int i = 0; i < Row; i++)
        {
            for(int j = 0; j < Col; j++)
            {

                fscanf(file, "%lf" , &Buff);
                if(Buff > (sizeof(double) - 1))
                {
                    printf("\nElement is too big!\n");
                    exit(NULL);
                }
                Matrix[i][j] = Buff;
            }
        }
    }
    return Matrix;
}

//возвращает матрицу, являющуюся результатом умножения aMatrix и bMatrix
double **multiply(double **aMatrix, double **bMatrix, int aRow, int bCol, int aCol)
{
    double **cMatrix = (double**) malloc((aRow * bCol) * sizeof(double*));
    for (int i = 0; i < (aRow * bCol); i++)
    {
        cMatrix[i] = (int*) malloc((aRow * bCol) * sizeof(int));
    }

    for(int i = 0; i < aRow; i++)
    {
        for(int j = 0; j < bCol; j++)
        {
            cMatrix[i][j] = 0;
            for(int k = 0; k < aCol; k++)       //лишнего цикла точно нет
            {
                cMatrix[i][j] += aMatrix[i][k] * bMatrix[k][j];
            }
        }
    }
    return cMatrix;
}

int main()
{
    char filePath[MAX_PATH_SIZE];
    int N = 0;

    printf("Enter amount of files: ");
    scanf("%d", &N);

    if (N <= 1)
    {
        printf("\nNothing to multiply! Press enter to exit...\n");
        exit(1);
    }

    int aRow = 0;       //количество строк для матрицы А (то же самое для матрицы В)
    int aCol = 0;       //количество столбцов для матрицы А (то же самое для матрицы В)
    int bRow = 0;
    int bCol = 0;

    double **aMatrix = NULL;
    double **bMatrix = NULL;
    double **cMatrix = NULL;

    for (int i = 0; i < N; i++)
    {
        printf("\nEnter the path of the %d file: ", i+1);
        scanf("%s", &filePath);

        FILE *file = fopen(filePath, "r");

        if (!file)
        {
            printf("\nCan't open file! Errno: %d\n", ENOENT);
            exit(1);
        }

        if (i == 0)
        {
            fscanf(file, "%d", &aRow);
            fscanf(file, "%d", &aCol);
            aMatrix = GetMatrix(file);
        }
        else
        {
            fscanf(file, "%d", &bRow);
            fscanf(file, "%d", &bCol);

            if(aCol != bRow)    //проверка возможности умножения
            {
                printf("\nCan't multiply!\n");
                exit(1);
            }

            bMatrix = GetMatrix(file);
            cMatrix = multiply(aMatrix, bMatrix, aRow, bCol, aCol);
            aMatrix = cMatrix;
        }

        PrintMatrix(aRow, aCol, aMatrix);
        fclose(file);
    }
    printf("\nRESULT: \n");
    PrintMatrix(aRow, bCol, cMatrix);

    FreeMatrix(aRow, aCol, aMatrix);
    FreeMatrix(bRow, bCol, bMatrix);
    FreeMatrix(aRow, bCol, cMatrix);

    return 0;
}
