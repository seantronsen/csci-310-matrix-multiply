/*
 * FILE     : student.h
 * AUTHOR   : Sean Tronsen (sttronsen@coastal.edu)
 * PURPOSE  : file houses the student struct and associated typedef - also contains output prototype function
 * DATE     : October 11, 2021
 *
 */
#ifndef TRONSEN_HW_PROG_02_STUDENT_H
#define TRONSEN_HW_PROG_02_STUDENT_H

// student struct
struct studentS {
    char name[75];
    short grade;
};

typedef struct studentS* student;



int student_driver();


void print_student_stats(student s, int high_score);

#endif //STUDENT_H
