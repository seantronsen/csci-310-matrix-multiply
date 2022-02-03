/*
 * FILE     : my_diff.cpp
 * AUTHOR   : Sean Tronsen
 * PURPOSE  : Contains utility function used on the CLI for checking equality between the matrix multiplication result files
 *              made primarily for comparing the results of serial and parallel computations on equally sized matrices
 * DATE     : November 7, 2021
 *
 *
 */


#include <iostream>
#include <cmath>

using namespace std;

int main(int argc, char **argv) {

    if (argc != 3) {
        cout << "Incorrect number of arguments. Use as follows: " << endl;
        cout << argv[0] << " <file_name_01> <file_name_02>" << endl;
        exit(EXIT_FAILURE);
    }

    string file_name_01 = argv[1];
    string file_name_02 = argv[2];

    int result = system(("diff " + file_name_01 + " " + file_name_02).c_str());

    if (result == 0) {
        cout << "The files provided have identical contents" << endl;
        // exit(EXIT_SUCCESS); -- removal of exit command at request of Dr. Jones
    }

    cout << "Checking each file's contents..." << endl;

    FILE *fp_01, *fp_02;
    int file_meta_01[2], file_meta_02[2];

    // open file 01 and read meta
    if ((fp_01 = fopen(file_name_01.c_str(), "r")) == NULL) {
        cerr << "Unable to open file: " << file_name_01 << " ..." << endl;
        exit(EXIT_FAILURE);
    }

    if (fread(&file_meta_01, sizeof(file_meta_01), 1, fp_01) != 1) {
        cerr << "Issue reading metadata from file " << file_name_01 << "..." << endl;
        exit(EXIT_FAILURE);
    }


    // open file 02 and read meta
    if ((fp_02 = fopen(file_name_02.c_str(), "r")) == NULL) {
        cerr << "Unable to open file: " << file_name_02 << " ..." << endl;
        exit(EXIT_FAILURE);
    }

    if (fread(&file_meta_02, sizeof(file_meta_02), 1, fp_02) != 1) {
        cerr << "Issue reading metadata from file " << file_name_02 << "..." << endl;
        exit(EXIT_FAILURE);
    }



    // begin checking the equality of each file's contents
    if (file_meta_01[0] != file_meta_02[0] || file_meta_01[1] != file_meta_02[1]) {
        cerr << "Array dimensions differ between the files..." << endl;
        exit(EXIT_FAILURE);
    }

    int row = file_meta_01[0], col = file_meta_01[1];
    double *A = new double[row * col];
    double *B = new double[row * col];

    for (int i = 0; i < row; i++) {
        fread(&A[i * col], col * sizeof(double), 1, fp_01);
        fread(&B[i * col], col * sizeof(double), 1, fp_02);
    }
    fclose(fp_01);
    fclose(fp_02);
    double TTSE = 0;
    double AVGPRE = 0;
    double num_different_elements = 0;

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            double holder_01 = A[j + i * col];
            double holder_02 = B[j + i * col];
            TTSE += pow(holder_01 - holder_02, 2.0);
            AVGPRE += abs((holder_01 - holder_02) / holder_01);
            if (holder_01 != holder_02) {
                num_different_elements++;
            }
        }
    }
    AVGPRE = ((1.0 / (row * col)) * AVGPRE);

    cout << "TOTAL ELEMENTS: " << (double) (row * col) << " | TOTAL DIFFERENT ELEMENTS: " << num_different_elements
         << endl;
    cout << "FILE CONTENT EQUALITY (PERCENT MATCH AS FRACTION): "
         << (num_different_elements == 0 ? 100 : ((row * col) / num_different_elements)) << " / 100" << endl;
    cout << "TOTAL SUM OF THE SQUARED ERROR: " << TTSE << endl;
    cout << "AVERAGE PERCENT RELATIVE ERROR (decimal): " << AVGPRE << endl;

    exit(EXIT_SUCCESS);


}

