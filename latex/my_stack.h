/*
 * my_stack.h - functions for a stack implementation
 *
 * Author: clintf
 * Student Name:
 *
 * Course: CSCI 356
 * Version 1.0
 */

#ifndef MY_STACK_H_
#define MY_STACK_H_


struct stackelementS {
    void* contents;				// the stack item
    struct stackelementS* next; // pointer to the next item in stack
};


typedef struct stackelementS stackelement;

struct stackS {
    stackelement* top;
};

typedef struct stackS* stack;	// a stack is a pointer


/*
 * creates a stack
 * returns: a pointer to a stack
 */
stack newstack();

/*
 * checks the status of a stack
 * stack s: a stack to check for emptiness; must not be NULL
 * returns: value is > 0 iff stack has no elements
 */
int isempty (const stack s);

/*
 * pushes item onto stack
 * stack s: 	a stack to check for emptiness; must not be NULL
 * void* item:	a pointer to an item to be pushed onto stack
 * returns:		item is head of stack
 */
void push (stack s, void* item);

/*
 * pops head from stack
 * stack s: a stack to check for emptiness: must not be NULL
 * returns: item returned was head of stack and next element
 * 			is new head of stack
 */
void* pop (stack s);


#endif /* MY_STACK_H_ */