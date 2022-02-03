/*
 * FILE     : print_matrix.cpp
 * AUTHOR   : Sean Tronsen
 * PURPOSE  : Contains a short runner program used to print the matrix from a file - uses existing codebase
 * DATE     : November 7, 2021
 *
 *  CREATED AT REQUEST OF DR. JONES
 *
 */




#include <iostream>
#include <cstring>
#include "matrix_multiplication_functions.h"

using namespace std;


int main(int argc, char **argv) {


    // CHECK FOR FILENAME
    if (argc != 2) {
        cout << "An incorrect number of parameters were passed to the program" << endl;
        cout << "Use the program as shown below" << endl;
        cout << argv[0] << " <input_file>" << endl;
        exit(1);
    }
    string inFileName = argv[1];
    char in_file_name_as_char_arr[inFileName.length()];
    strcpy(in_file_name_as_char_arr, inFileName.c_str());


    // OUTPUT MATRIX TO CONSOLE -- ASSUMES MATRIX FILE EXISTS
    cout << "PRINTING MATRIX" << endl;
    cout << endl;
    show_matrix_from_file(in_file_name_as_char_arr);


}