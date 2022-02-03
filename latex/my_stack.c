/*
 * FILE     : my_stack.c
 * AUTHOR   : Sean Tronsen (sttronsen@coastal.edu)
 * PURPOSE  : source code file for the associated my_stack.h header file - implementations of the prototyped functions
 * DATE     : October 11, 2021
 *
 */


#include <stddef.h>
#include "my_stack.h"
#include <stdlib.h>
#include <stdio.h>

// pre-def to avoid compiler error
typedef struct stackS stackS;


// function used to create a new stack structure
// execution first allocates the appropriate amount of memory
// on the heap. If the returned address is null, the program exits.
// the top is also assigned null before the stack pointer is returned.
stack newstack() {
    stack s = (stack) malloc(sizeof(stackS));
    if (s == NULL) {
        printf("Memory could not be allocated. \n");
        exit(0);
    }
    s->top = NULL;
    return s;
}

// simple if condition used to determine whether or not the stack is empty
// if top is null then return true (1 in C since dedicated booleans do not exist)
int isempty(const stack s) {
    if (!s->top) return 1;
    return 0;

}


// function used to push new items onto the stack
// first, memory is allocated on the heap for a new stack element
// then the members of said element are assigned the prior top and the
// passed in item arg as the contents
// the top of the stack is then set to the new elements address
void push(stack s, void *item) {
    stackelement *element = (stackelement *) malloc(sizeof(stackelement));
    element->contents = item;
    element->next = s->top;
    s->top = element;

}

// function used to pop elements off the stack (returns type-castable void pointer)
// checks if the stack is empty, if so then return null
// otherwise, get the pointer to the element and then the contents
// return the contents, but deallocate the stack element beforehand
void* pop(stack s) {
    if (isempty(s)) return NULL;
    stackelement* element = (s->top);
    s->top = element->next;
    void* item = element->contents;
    free(element);
    return item;
}


