/*
 * FILE     : matrix_multiplication_parallel.cpp
 * AUTHOR   : Sean Tronsen
 * PURPOSE  : Contains the parallelized version of the computations required for matrix multiplication
 * DATE     : November 7, 2021
 *
 * SIMPLE CLI PROGRAM - IN USE
 */


#include <iostream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <omp.h>
#include <cstring>
#include "matrix_multiplication_functions.h"

#define A(i, j, col) A[j + col*i]
#define B(i, j, col) B[j + col*i]
#define C(i, j, col) C[j + col*i]


using namespace std;

// the driver function - execution entry point
int main(int argc, char *argv[]) {


    // check the number of args and react accordingly
    if (argc != 4) {
        cout << "An incorrect number of parameters were passed to the program" << endl;
        cout << "Use the program as shown below" << endl;
        cout << argv[0] << " <size_of_matrix> <number_of_threads> <out_file_name>" << endl;
        exit(1);
    }

    // define vars before attempting to assign
    int size, num_threads;
    string out_file_name = argv[3];
    // attempt to parse ints from string command args
    try {

        size = stoi(argv[1]);
        num_threads = stoi(argv[2]);
        if (size < 1) {
            cout << "The size_of_matrix parameter must be an integer value greater than zero" << endl;
            exit(1);
        }
        if (num_threads < 1) {
            cout << "The number_of_threads parameter must be an integer value greater than zero" << endl;
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
        cout << argv[0] << " <size_of_matrix> <number_of_threads> <out_file_name>" << endl;
        exit(1);
    }

    int M, K, N;
    M = K = N = size;


    /* related to fwrite and filename*/
    string file_name_starter = "./parallel_matrix_multiply__result_data_";
    time_t seconds;
    seconds = time(NULL);
    file_name_starter += to_string(seconds);
    char file_name_char_array[file_name_starter.length()];
    strcpy(file_name_char_array, file_name_starter.c_str());


    // set the number of threads - based on CLI value
    omp_set_num_threads(num_threads);
    cout << "Data Initialization Started.." << endl;
    double *A = new double[M * K];
    double *B = new double[K * N];
    double *C = new double[M * N];
    initialize_matrix(A, M, K);
    initialize_matrix(B, K, N);
    cout << "Data Initialization Completed" << endl;
    cout << "Computation Started.." << endl;
    auto start = chrono::steady_clock::now();

    parallel_matrix_multiplication(A, B, C, M, N, K);

    auto end = chrono::steady_clock::now();
    double compute_time = chrono::duration<double>(end - start).count();
    cout << "Duration : " << compute_time << " seconds" << endl;
    char out_file_name_c[out_file_name.length()];
    strcpy(out_file_name_c, out_file_name.c_str());
    write_file(C, M, N, out_file_name_c);

    double flops = calc_mm_gigaflops(size, compute_time);
    cout << "Total G_Flops: " << flops << endl;
    show_matrix_from_file(out_file_name_c);
    delete[] A;
    delete[] B;
    delete[] C;
}
