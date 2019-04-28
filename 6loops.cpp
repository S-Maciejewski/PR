#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "omp.h"

#define USE_MULTIPLE_THREADS true
#define MAXTHREADS 128
int NumThreads;
double start;

static const int ROWS = 1000;    // liczba wierszy macierzy
static const int COLUMNS = 1000; // liczba kolumn macierzy

float matrix_a[ROWS][COLUMNS]; // lewy operand
float matrix_b[ROWS][COLUMNS]; // prawy operand
float matrix_r[ROWS][COLUMNS]; // wynik

void initialize_matrices()
{
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLUMNS; j++)
        {
            matrix_a[i][j] = (float)rand() / RAND_MAX;
            matrix_b[i][j] = (float)rand() / RAND_MAX;
            matrix_r[i][j] = 0.0;
        }
}

void multiply_matrices_IJK_IIJJKK()
{
    int r = 10;
    for (int i = 0; i < ROWS; i += r)
        for (int j = 0; j < COLUMNS; j += r)
            for (int k = 0; k < COLUMNS; k += r)
#pragma omp parallel for
                for (int ii = i; ii < i + r; ii++)
                    for (int jj = j; jj < j + r; jj++)
                        for (int kk = k; kk < k + r; kk++)
                            matrix_r[ii][jj] += matrix_a[ii][kk] * matrix_b[kk][jj];
}

void numThreads()
{
    if (USE_MULTIPLE_THREADS)
    {
        SYSTEM_INFO SysInfo;
        GetSystemInfo(&SysInfo);
        NumThreads = SysInfo.dwNumberOfProcessors;
        if (NumThreads > MAXTHREADS)
            NumThreads = MAXTHREADS;
    }
    else
    {
        NumThreads = 1;
    }
    printf("liczba watkow  = %d\n\n", NumThreads);
}

int main(int argc, char *argv[])
{
    initialize_matrices();

    double start = (double)clock() / CLK_TCK;
    multiply_matrices_IJK_IIJJKK();
    double elapsed = (double)clock() / CLK_TCK;

    printf("Czas: %8.4f sec \n", elapsed - start);

    return 0;
}