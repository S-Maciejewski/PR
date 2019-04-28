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

void multiply_matrices_JKI()
{
#pragma omp parallel for 
    for (int j = 0; j < COLUMNS; j++)
        for (int k = 0; k < COLUMNS; k++)
            for (int i = 0; i < ROWS; i++)
                matrix_r[i][j] += matrix_a[i][k] * matrix_b[k][j];
}

void multiply_matrices_IKJ_sequentially() 
{
    for (int i = 0; i < ROWS; i++)
        for (int k = 0; k < COLUMNS; k++)   
            for (int j = 0; j < COLUMNS; j++)
                matrix_r[i][j] += matrix_a[i][k] * matrix_b[k][j];
}

void numThreads() {
    if (USE_MULTIPLE_THREADS) {
        SYSTEM_INFO SysInfo;
        GetSystemInfo(&SysInfo);
        NumThreads = SysInfo.dwNumberOfProcessors;
        if (NumThreads > MAXTHREADS) NumThreads = MAXTHREADS;
    } else {
        NumThreads = 1;
    }
    printf("liczba watkow  = %d\n\n", NumThreads);
}

int main(int argc, char *argv[])
{
    initialize_matrices();

    double start = (double)clock() / CLK_TCK;
    multiply_matrices_JKI();
    // multiply_matrices_IKJ_sequentially();
    double elapsed = (double)clock() / CLK_TCK;

    printf("Czas: %8.4f sec \n", elapsed - start);

    return 0;
}