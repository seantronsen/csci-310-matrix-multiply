/*
 * FILE     : matrix_multiplication_serial_sweep_runner.cpp
 * AUTHOR   : Sean Tronsen
 * PURPOSE  : Contains the original serial version of the computations required for matrix multiplication
 *              minor modifications made for automation related to data collection
 * DATE     : November 7, 2021
 *
 */



#include <iostream>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include "matrix_multiplication_functions.h"

using namespace std;


int main() {
    /* related to fwrite and filename*/
    char data_file_name_01[] = "./serial_matrix_multiply_performance_time_data.csv";
    char data_file_name_02[] = "./serial_matrix_multiply_performance_flops_data.csv";
    FILE *d_fp_01, *d_fp_02;

    if ((d_fp_01 = fopen(data_file_name_01, "w")) == NULL) {
        cerr << "Unable to open file: " << data_file_name_01 << endl;
        exit(EXIT_FAILURE);

    }

    if ((d_fp_02 = fopen(data_file_name_02, "w")) == NULL) {
        cerr << "Unable to open file: " << data_file_name_02 << endl;
        exit(EXIT_FAILURE);

    }
    // setup csv header row
    fprintf(d_fp_01, "%s", "MATRIX_ROWS,MATRIX_COLS");
    fprintf(d_fp_01, ",%d", 1);
    fprintf(d_fp_01, "%s", "\n");

    fprintf(d_fp_02, "%s", "MATRIX_ROWS,MATRIX_COLS");
    fprintf(d_fp_02, ",%d", 1);
    fprintf(d_fp_02, "%s", "\n");

    int size_base = 1000;

    for (int i = 0; i < 8; i++) {
        int M, K, N;
        M = K = N = size_base + (500 * i);
        fprintf(d_fp_01, "%d", M);
        fprintf(d_fp_01, ",%d", N);
        fprintf(d_fp_02, "%d", M);
        fprintf(d_fp_02, ",%d", N);

        cout << endl;
        cout << "Data Initialization Started (" << M << " x " << K << ")..." << endl;
        double *A = new double[M * K];
        double *B = new double[K * N];
        double *C = new double[M * N];
        initialize_matrix(A, M, K);
        initialize_matrix(B, K, N);
        cout << "Computation Started.." << endl;
        auto start = chrono::steady_clock::now();

        serial_matrix_multiplication(A, B, C, M, N, K);

        auto end = chrono::steady_clock::now();
        double compute_time = chrono::duration<double>(end - start).count();
        double flops = calc_mm_gigaflops(M, compute_time);
        cout << "Duration : " << compute_time << " seconds" << endl;
        cout << "Total G_Flops: " << flops << endl;
        fprintf(d_fp_01, ",%f", compute_time);
        fprintf(d_fp_02, ",%f", flops);
        delete[] A;
        delete[] B;
        delete[] C;
        fprintf(d_fp_01, "%s", "\n");
        fprintf(d_fp_02, "%s", "\n");
    }
    cout << "Iterations complete..." << endl;
    fclose(d_fp_01);
    fclose(d_fp_02);
    cout << data_file_name_01 << " written..." << endl;
    cout << data_file_name_02 << " written..." << endl;
    exit(EXIT_SUCCESS);
}
