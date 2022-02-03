/*
 * FILE     : matrix_multiplication_serial_blocked.cpp
 * AUTHOR   : Sean Tronsen
 * PURPOSE  : Contains the DGEMM version of the matrix multiply algorithm
 * DATE     : November 7, 2021
 *
 */


// code provided by assignment handout via Dr. Jones

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include "matrix_multiplication_functions.h"

using namespace std;

int main(int argc, char **argv) {

    // check the number of args and react accordingly
    if (argc != 4) {
        cout << "An incorrect number of parameters were passed to the program" << endl;
        cout << "Use the program as shown below" << endl;
        cout << argv[0] << " <size_of_matrix> <block_size> <output_file>" << endl;
        exit(1);
    }

    // define vars before attempting to assign
    int size, block_size;
    string outFileName = argv[3];
    // attempt to parse ints from string command args
    try {

        size = stoi(argv[1]);
        if (size < 1) {
            cout << "The size_of_matrix parameter must be an integer value greater than zero" << endl;
            exit(1);
        }

        block_size = stoi(argv[2]);
        if (size < 1) {
            cout << "The block_size parameter must be an integer value greater than zero" << endl;
            exit(1);
        }

        if ((size % block_size) != 0) {
            cout
                    << "The size parameter must be divisible by the block_size parameter such that no remainder results from the operation."
                    << endl;
            cout << "In other words, (size_of_matrix % block_size) must be equal to zero" << endl;
            exit(1);
        }

        if (outFileName.length() < 3) {
            cout << "The name of the output file must be larger than 3 characters" << endl;
            exit(1);
        }


    } catch (...) {
        // alert user of mistake if parse fails
        cout << "The program only accepts integer values larger than zero  as parameters" << endl;
        cout << "Use the program as shown below" << endl;
        cout << argv[0] << " <size_of_matrix> <block_size> <output_file>" << endl;
        exit(1);
    }


    int M, K, N;
    M = K = N = size;
    cout << "Data Initialization Started.." << endl;
    auto *A = new double[M * K];
    auto *B = new double[K * N];
    auto *C = new double[M * N];
    initialize_matrix(A, M, K);
    initialize_matrix(B, K, N);
    initialize_matrix_to_zero(C, M, N);

/*

    cout << "MATRIX A" << endl;
    show_matrix(A, M, N);
    cout << endl;

    cout << "MATRIX B" << endl;
    show_matrix(B, M, N);
    cout << endl;


    cout << "MATRIX C" << endl;
    show_matrix(C, M, N);
    cout << endl;
*/


    cout << "Computation Started.." << endl;
    auto start = chrono::steady_clock::now();



  dgemm(size, A, B, C, block_size);
 //   bijk(A, B, C, M, block_size);



    auto end = chrono::steady_clock::now();
    double compute_time = chrono::duration<double>(end - start).count();
    cout << "Duration : " << compute_time << " seconds" << endl;
    char outFileNameC[outFileName.length()];
    strcpy(outFileNameC, outFileName.c_str());
    write_file(C, M, N, outFileNameC);
    show_matrix_from_file(outFileNameC);
    double flops = calc_mm_gigaflops(size, compute_time);
    cout << "Total G_Flops: " << flops << endl;
    delete[] A;
    delete[] B;
    delete[] C;
}




