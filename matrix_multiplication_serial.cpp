/*
 * FILE     : matrix_multiplication_serial.cpp
 * AUTHOR   : Sean Tronsen
 * PURPOSE  : Contains the original serial version of the computations required for matrix multiplication
 *              minor modifications made for difference checking
 * DATE     : November 7, 2021
 *
 * SIMPLE CLI PROGRAM - IN USE
 */


#include <iostream>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include "matrix_multiplication_functions.h"

using namespace std;

int main(int argc, char **argv) {

    // check the number of args and react accordingly
    if (argc != 3) {
        cout << "An incorrect number of parameters were passed to the program" << endl;
        cout << "Use the program as shown below" << endl;
        cout << argv[0] << " <size_of_matrix> <output_file>" << endl;
        exit(1);
    }

    // define vars before attempting to assign
    int size;
    string outFileName = argv[2];
    // attempt to parse ints from string command args
    try {

        size = stoi(argv[1]);
        if (size < 1) {
            cout << "The size_of_matrix parameter must be an integer value greater than zero" << endl;
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
        cout << argv[0] << " <size_of_matrix> <output_file>" << endl;
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
/*

    cout << "MATRIX A" << endl;
    show_matrix(A, M, N);
    cout << endl;

    cout << "MATRIX B" << endl;
    show_matrix(B, M, N);
    cout << endl*/;


    cout << "Computation Started.." << endl;
    auto start = chrono::steady_clock::now();
    serial_matrix_multiplication(A, B, C, M, N, K);
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
