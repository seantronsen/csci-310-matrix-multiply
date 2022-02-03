/*
 * FILE     : matrix_multiplication_functions.h
 * AUTHOR   : Sean Tronsen
 * PURPOSE  : Contains the headers for matrix multiply operations
 * DATE     : November 16, 2021
 *
 * FUNCTIONS HEADER - IN USE
 */


#ifndef TRONSEN_CSCI_310_OPENMP_02_MATRIX_MULTIPLICATION_FUNCTIONS_H
#define TRONSEN_CSCI_310_OPENMP_02_MATRIX_MULTIPLICATION_FUNCTIONS_H

void initialize_matrix(double *matrix, int row, int col);
void initialize_matrix_to_zero(double *matrix, int row, int col);

void show_matrix(double *matrix, int row, int col);

void parallel_matrix_multiplication(double *A, double *B, double *C, int M, int N, int K);

void write_file(double *matrix, int row, int col, char *filename);

void show_matrix_from_file(char *filename);

void serial_matrix_multiplication(double *A, double *B, double *C, int M, int N, int K);

double calc_mm_gigaflops(int n, double t);

void dgemm(int n, double *A, double *B, double *C, int block_size);

void do_block(int n, int si, int sj, int sk, double *A, double *B, double *C, int block_size);

// void bijk(double *A, double *B, double *C, int n, int bsize);

#endif //TRONSEN_CSCI_310_OPENMP_02_MATRIX_MULTIPLICATION_FUNCTIONS_H
