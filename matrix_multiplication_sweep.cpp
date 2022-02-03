/*
 * FILE     : matrix_multiplication_sweep.cpp
 * AUTHOR   : Sean Tronsen
 * PURPOSE  : Contains an automated version of the parallelized computations required for matrix multiplication
 * DATE     : November 16, 2021
 *
 * MAIN USE FILE
 */



#include <iostream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <omp.h>
#include <cstring>
#include "matrix_multiplication_functions.h"

using namespace std;

#define A(i, j, col) A[j + col*i]
#define B(i, j, col) B[j + col*i]
#define C(i, j, col) C[j + col*i]

int main(int argc, char *argv[]) {


    // check the number of args and react accordingly
    if (argc != 8) {
        cout << "An incorrect number of parameters were passed to the program" << endl;
        cout << "Use the program as shown below" << endl;
        cout << argv[0]
             << " <min_size_matrix> <max_size_matrix> <size_step_matrix> <min_threads> <max_threads> <thread_step> <out_file_name>"
             << endl;
        exit(1);
    }

    // define vars before attempting to assign
    int min_size_matrix, max_size_matrix, size_step_matrix, min_threads, max_threads, thread_step;
    string out_file_name = argv[7];


    // attempt to parse ints from string command args
    try {
        min_size_matrix = stoi(argv[1]);
        if (min_size_matrix < 1) {
            cout << "The min_size_matrix parameter must be an integer value greater than zero" << endl;
            exit(1);
        }

        max_size_matrix = stoi(argv[2]);
        if (max_size_matrix < min_size_matrix) {
            cout << "The max_size_matrix parameter must be an integer value greater than or equal to min_size_matrix"
                 << endl;
            exit(1);
        }

        size_step_matrix = stoi(argv[3]);
        if (size_step_matrix < 1) {
            cout << "The size_step_matrix parameter must be an integer value greater than zero" << endl;
            exit(1);
        }


        min_threads = stoi(argv[4]);
        if (min_size_matrix < 1) {
            cout << "The min_threads parameter must be an integer value greater than zero" << endl;
            exit(1);
        }

        max_threads = stoi(argv[5]);
        if (max_threads < min_threads) {
            cout << "The max_threads parameter must be an integer value greater than or equal to min_threads" << endl;
            exit(1);
        }

        thread_step = stoi(argv[6]);
        if (thread_step < 1) {
            cout << "The thread_step parameter must be an integer value greater than zero" << endl;
            exit(1);
        }


        if (out_file_name.length() < 3) {
            cout << "The name of the output file must be larger than 3 characters" << endl;
            exit(1);
        }

    } catch (...) {
        // alert user of mistake if parse fails
        cout << "The program only accepts integer values larger than zero  as parameters" << endl;
        cout << "Use the program as shown below" << endl;
        cout << argv[0]
             << " <min_size_matrix> <max_size_matrix> <size_step_matrix> <min_threads> <max_threads> <thread_step> <out_file_name>"
             << endl;
        exit(1);
    }

    char data_file_name_01[] = "./sweep_matrix_multiply_performance_time_data.csv";
    char data_file_name_02[] = "./sweep_matrix_multiply_performance_flops_data.csv";

    FILE *d_fp_01, *d_fp_02;

    char out_file_name_c[out_file_name.length()];
    strcpy(out_file_name_c, out_file_name.c_str());


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
    fprintf(d_fp_02, "%s", "MATRIX_ROWS,MATRIX_COLS");
    int tx;
    for (tx = min_threads; tx <= max_threads; tx += thread_step) {
        fprintf(d_fp_01, ",%d", tx);
        fprintf(d_fp_02, ",%d", tx);
    }
    fprintf(d_fp_01, "%s", "\n");
    fprintf(d_fp_02, "%s", "\n");



    // start processing
    for (int i = min_size_matrix; i <= max_size_matrix; i += size_step_matrix) {
        int M, K, N;
        M = K = N = i;
        auto *A = new double[M * K];
        auto *B = new double[K * N];
        auto *C = new double[M * N];
        initialize_matrix(A, M, K);
        initialize_matrix(B, K, N);

        fprintf(d_fp_01, "%d", M);
        fprintf(d_fp_01, ",%d", N);
        fprintf(d_fp_02, "%d", M);
        fprintf(d_fp_02, ",%d", N);
        cout << endl;
        int t;
        for (t = min_threads; t <= max_threads; t += thread_step) {
            cout << "-------------------------------------------" << endl;

            // serial execution for prep
            cout << endl;
            cout << "Data Initialization Started (" << M << " x " << K << ")..." << endl;

            auto start = chrono::steady_clock::now();
            auto end = start;

            cout << "Data Initialization Completed" << endl;
            cout << "Computation Started.." << endl;


            // use serial version mm
            if (t == 1) {
                start = chrono::steady_clock::now();
                serial_matrix_multiplication(A, B, C, M, N, K);
                end = chrono::steady_clock::now();

            }
                // enter parallel - parallelized mm
            else {
                omp_set_num_threads(t);
                start = chrono::steady_clock::now();
                parallel_matrix_multiplication(A, B, C, M, N, K);
                end = chrono::steady_clock::now();
            }

            // resume serial
            double compute_time = chrono::duration<double>(end - start).count();
            double flops = calc_mm_gigaflops(i, compute_time);
            cout << "Duration : " << compute_time << " seconds" << endl;
            fprintf(d_fp_01, ",%f", compute_time);
            fprintf(d_fp_02, ",%f", flops);
            if (t == max_threads && i == max_size_matrix) {
                write_file(C, i, i, out_file_name_c);
            }
            cout << "Total G_Flops: " << flops << endl;

        }
        fprintf(d_fp_01, "%s", "\n");
        fprintf(d_fp_02, "%s", "\n");
        delete[] A;
        delete[] B;
        delete[] C;
    }

    cout << "Iterations complete..." << endl;
    fclose(d_fp_01);
    fclose(d_fp_02);
    cout << data_file_name_01 << " written..." << endl;
    exit(EXIT_SUCCESS);


}
