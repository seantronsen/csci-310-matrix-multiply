/*
 * FILE     : student.c
 * AUTHOR   : Sean Tronsen (sttronsen@coastal.edu)
 * PURPOSE  : main driver for the requested student stack program - also contains output function for student
 * DATE     : October 14, 2021
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "student.h"
#include "my_stack.h"


typedef struct studentS studentS;

int main(int argc, char *argv[]) {
    printf("Student Record Program\n");
    printf("Enter students individually and complete processing by entering a blank line\n\n");
    stack student_stack = newstack();

    int high_score = 0; // used for short math
    while (1) {
        student student_i = (student) malloc(sizeof(studentS)); // set up struct in scope

        //  get the name
        char name_holder[75];
        strcpy(name_holder, ""); // assign empty string
        printf("Enter student name:");

        getchar();
        fgets(name_holder, sizeof(name_holder), stdin);
        if (strcmp(name_holder, "\n") == 0) goto out_loop; // check for newline only - exit loop if exists
        if (sscanf(name_holder, "%s\n", name_holder) != 1) { // check value
            printf("Please enter a series of alphabetical characters. \n");
            continue;
        }
        strcpy(student_i->name, name_holder);                   // assign the value

        // get the grade
        int x;
        printf("Enter associated grade (numeric value): ");
        if (scanf("%d", &x) != 1 || (x < 0 || x > 100)) { // get user input and check value
            printf("\nPlease enter a numeric value from 0 - 100\n");
            continue;
        }
        student_i->grade = x;    // assign the value



        if (student_i->grade > high_score) high_score = student_i->grade; // check for high score
        push(student_stack, student_i);                                   // push onto the stack
    }

    out_loop: // exit loop label
    if (!isempty(student_stack)) { // only if the user entered anything
        printf("NAME | GRADE | DISTANCE\n");
        student student_z;
        student_z = pop(student_stack); // grab the first item
        while (student_z != NULL) {     // continue while students remain
            print_student_stats(student_z, high_score);
            free(student_z);                // deallocate the student
            student_z = pop(student_stack); // grab any future items
        }
    }
    return 0;
}

// short function to print the stats
void print_student_stats(student s, int high_score) {
    printf("%s | %d | %d\n", s->name, s->grade, high_score - s->grade);
}
