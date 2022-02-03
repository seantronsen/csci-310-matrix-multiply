/*
 * FILE     : matrix_multiplication_functions.cpp
 * AUTHOR   : Sean Tronsen
 * PURPOSE  : Contains the source code for matrix multiply operations
 * DATE     : November 16, 2021
 *
 * FUNCTIONS IMPORTABLE - IN USE
 */



#include <omp.h>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include "matrix_multiplication_functions.h"


#define A(i, j, col) A[j + col*i]
#define B(i, j, col) B[j + col*i]
#define C(i, j, col) C[j + col*i]


using namespace std;

// The method below was provided - it is used to populate a single dimensional
// array  that is disguised as a matrix
void initialize_matrix(double *matrix, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            matrix[j + i * col] = (double) rand() / RAND_MAX;
        }
    }
}


void initialize_matrix_to_zero(double *matrix, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            matrix[j + i * col] = 0.0;
        }
    }
}


// the below method was provided despite being unused
// I took the liberty of adding the parallel pragma anyways
void show_matrix(double *matrix, int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%2.8f\t", matrix[j + i * col]);
        }
        cout << endl;
    }
}


void serial_matrix_multiplication(double *A, double *B, double *C, int M, int N, int K) {
    double my_sum;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            my_sum = 0;
            for (int k = 0; k < K; k++) {
                my_sum += A(i, k, K) * B(k, j, N);
            }
            C(i, j, N) = my_sum;
        }
    }
}


// the below method is used to perform the matrix multiplication (dot product calc)
//
// the my_sum variable had to be localized in initialization where total_time was "globalized"
// in terms of the function scope
// some simple math operations were added to calc the time inside a critical section
// and lastly the parallel pragma was added
//
// note that the parallel and parallel for pragmas are split in order to calc the total parallel time
void parallel_matrix_multiplication_with_log(double *A, double *B, double *C, int M, int N, int K) {
    double total_time = 0;
    int threads_alloc = 0;
    for (int i = 0; i < M; i++) {
#pragma omp parallel
        {

#pragma omp sections
            {
#pragma omp section
                {
                    threads_alloc = omp_get_num_threads();
                }

            }

            double start_time = omp_get_wtime();
#pragma omp for
            for (int j = 0; j < N; j++) {
                double my_sum = 0;
                for (int k = 0; k < K; k++) {
                    my_sum += A(i, k, K) * B(k, j, N);
                }
                C(i, j, N) = my_sum;
            }

            double total_thread_time = omp_get_wtime() - start_time;
#pragma omp critical
            {
                total_time += total_thread_time;
            }
        }
    }
    cout << "Total time in parallel (" << threads_alloc << " thread(s)) : " << total_time / threads_alloc << endl;
}


void parallel_matrix_multiplication(double *A, double *B, double *C, int M, int N, int K) {
    for (int i = 0; i < M; i++) {
#pragma omp parallel for
        for (int j = 0; j < N; j++) {
            double my_sum = 0;
            for (int k = 0; k < K; k++) {
                my_sum += A(i, k, K) * B(k, j, N);
            }
            C(i, j, N) = my_sum;
        }

    }
}


void write_file(double *matrix, int row, int col, char *filename) {
    FILE *fp;
    if ((fp = fopen(filename, "w")) == nullptr) {
        cerr << "Unable to open file: " << filename << " ..." << endl;
        exit(EXIT_FAILURE);
    }
    fwrite(&row, sizeof(int), 1, fp);
    fwrite(&col, sizeof(int), 1, fp);
    for (int i = 0; i < row; i++) {
        fwrite(&matrix[i * col], col * sizeof(double), 1, fp);
    }
    fclose(fp);
    cout << filename << " written..." << endl;
}

void show_matrix_from_file(char *filename) {
    FILE *fp;
    int meta_data[2];
    if ((fp = fopen(filename, "r")) == nullptr) {
        cerr << "Unable to open file: " << filename << " ..." << endl;
        exit(EXIT_FAILURE);
    }
    if (fread(&meta_data, sizeof(meta_data), 1, fp) != 1) {
        cerr << "Issue reading metadata from file...";
        exit(EXIT_FAILURE);
    }
    int row = meta_data[0], col = meta_data[1];
    double matrix[row * col];

    for (int i = 0; i < row; i++) {
        fread(&matrix[i * col], col * sizeof(double), 1, fp);
    }
    fclose(fp);
    show_matrix(matrix, row, col);
}

double calc_mm_gigaflops(int n, double t) {
    return ((pow(n, 3) / t) / pow(10, 9));
}


// initial code provided by assignment handout via Dr. Jones
void do_block(int n, int si, int sj, int sk, double *A, double *B, double *C, int block_size) {
    for (int i = si; i < si + block_size; ++i)
        for (int j = sj; j < sj + block_size; ++j) {
            double cij = C[i + j * n];/* cij = C[i][j] */
            for (int k = sk; k < sk + block_size; k++)
                cij += A[i + k * n] * B[k + j * n];/* cij+=A[i][k]*B[k][j] */
            C[i + j * n] = cij;/* C[i][j] = cij */
        }
}


// initial code provided by assignment handout via Dr. Jones
void dgemm(int n, double *A, double *B, double *C, int block_size) {
    for (int sj = 0; sj < n; sj += block_size)
        for (int si = 0; si < n; si += block_size)
            for (int sk = 0; sk < n; sk += block_size)
                do_block(n, si, sj, sk, A, B, C, block_size);
}

/*  IMPORTANT NOTE - THIS ENTIRE FUNCTION WAS TAKEN FROM A PUBLISHED STUDY FOR ERROR TESTING
 * THE TEXT CAN BE LOCATED HERE https://csapp.cs.cmu.edu/public/waside/waside-blocking.pdf
 *
 * The instructor, Dr. Jones, was alerted of its use during office hours
 *
 * */


void bijk(double *A, double *B, double *C, int n, int bsize) {
    int i, j, k, kk, jj;
    double sum;
    int en = bsize * (n / bsize);

/* Amount that fits evenly into blocks */


    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            C[i + j * n] = 0.0;

    for (kk = 0; kk < en; kk += bsize) {
        for (jj = 0; jj < en; jj += bsize) {
            for (i = 0; i < n; i++) {
                for (j = jj; j < jj + bsize; j++) {
                    sum = C[i + j * n];
                    for (k = kk; k < kk + bsize; k++) {
                        sum += A[i + k * n] * B[k + j * n];
                    }
                    C[i + j * n] = sum;
                }
            }
        }
    }
}


