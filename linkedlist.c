#include "linkedlist.h"
#include "talloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create a new NULL_TYPE value node.
Value *makeNull()
{
    Value *value = talloc(sizeof(Value));
    value->type = NULL_TYPE;
    value->c.car = NULL;
    value->c.cdr = NULL;
    return value;
}

// Create a new CONS_TYPE value node.
Value *cons(Value *newCar, Value *newCdr)
{
    Value *value = talloc(sizeof(Value));
    value->type = CONS_TYPE;
    value->c.car = newCar;
    value->c.cdr = newCdr;
    return value;
}

// Display the contents of the linked list to the screen in some kind of
// readable format
void display(Value *list)
{
    if (list == NULL)
    {
        return;
    }
    Value *temp = list;
    while (!isNull(temp))
    {
        switch (car(temp)->type)
        {
        case INT_TYPE:
            printf("%d ", car(temp)->i);
            break;

        case DOUBLE_TYPE:
            printf("%lf ", car(temp)->d);
            break;

        case STR_TYPE:
            printf("%s ", car(temp)->s);
            break;

        case CONS_TYPE:
            printf("you're trying to print a cons cell.\n");
            break;

        case NULL_TYPE:
            printf("You're trying to print a null cell\n");
            break;

        case PTR_TYPE:
            printf("You're trying to print a pointer cell\n");
            break;

        case OPEN_TYPE:
            printf("You're trying to print an open bracket cell\n");
            break;

        case CLOSE_TYPE:
            printf("You're trying to print a close bracket cell\n");
            break;

        case BOOL_TYPE:
            printf("You're trying to print a boolean cell\n");
            break;

        case SYMBOL_TYPE:
            printf("You're trying to print a symbol cell\n");
            break;

        case OPENBRACKET_TYPE:
            break;

        case CLOSEBRACKET_TYPE:
            break;

        case DOT_TYPE:
            break;

        case SINGLEQUOTE_TYPE:
            break;

        case VOID_TYPE:
          break;

        case CLOSURE_TYPE:
          break; 
        
        case PRIMITIVE_TYPE:
          break;

				case UNSPECIFIED_TYPE:
					break;
        }
        temp = cdr(temp);
    }
    printf("\n");
}

// Return a new list that is the reverse of the one that is passed in. All
// content within the list should be duplicated; there should be no shared
// memory whatsoever between the original list and the new one.
//
// FAQ: What if there are nested lists inside that list?
// ANS: There won't be for this assignment. There will be later, but that will
// be after we've got an easier way of managing memory.
Value *reverse(Value *list)
{
    if (isNull(list))
    {
        return list;
    }
    Value *head = makeNull();
    while (!isNull(list))
    {
        Value *value = talloc(sizeof(Value));
        switch (car(list)->type)
        {
        case INT_TYPE:
            value->i = car(list)->i;
            value->type = INT_TYPE;
            head = cons(value, head);
            break;

        case DOUBLE_TYPE:
            value->d = car(list)->d;
            value->type = DOUBLE_TYPE;
            head = cons(value, head);
            break;

        case STR_TYPE:
            value->s = talloc(sizeof(char) * (strlen(car(list)->s) + 1));
            strcpy(value->s, car(list)->s);
            value->type = STR_TYPE;
            head = cons(value, head);
            break;

        case CONS_TYPE:
            value->c = car(list)->c;
            value->type = CONS_TYPE;
            head = cons(value, head);
            break;

        case NULL_TYPE:
            printf("Invalid call.(NULL_TYPE)\n");
            free(value);
            break;

        case PTR_TYPE:
            printf("Invalid call.(PTR_TYPE)\n");
            free(value);
            break;

        case OPEN_TYPE:
            value->s = talloc(sizeof(char) * (strlen(car(list)->s) + 1));
            strcpy(value->s, car(list)->s);
            value->type = OPEN_TYPE;
            head = cons(value, head);
            break;

        case CLOSE_TYPE:
            value->s = talloc(sizeof(char) * (strlen(car(list)->s) + 1));
            strcpy(value->s, car(list)->s);
            value->type = CLOSE_TYPE;
            head = cons(value, head);
            break;

        case BOOL_TYPE:
            value->s = car(list)->s;
            value->type = BOOL_TYPE;
            head = cons(value, head);
            break;

        case SYMBOL_TYPE:
            value->s = talloc(sizeof(char) * (strlen(car(list)->s) + 1));
            strcpy(value->s, car(list)->s);
            value->type = SYMBOL_TYPE;
            head = cons(value, head);
            break;

        case OPENBRACKET_TYPE:
            break;

        case CLOSEBRACKET_TYPE:
            break;

        case DOT_TYPE:
            break;

        case SINGLEQUOTE_TYPE:
            break;

        case VOID_TYPE:
          break;

        case CLOSURE_TYPE:
          break; 
        
        case PRIMITIVE_TYPE:
          break;
				
				case UNSPECIFIED_TYPE:
					break;
        }
        list = cdr(list);
    }
    return head;
}

// Utility to make it less typing to get car value. Use assertions to make sure
// that this is a legitimate operation.
Value *car(Value *list)
{
    if (list == NULL)
    {
        exit(0);
    }
    return list->c.car;
}

// Utility to make it less typing to get cdr value. Use assertions to make sure
// that this is a legitimate operation.
Value *cdr(Value *list)
{
    if (list == NULL)
    {
        exit(0);
    }
    return list->c.cdr;
}

// Utility to check if pointing to a NULL_TYPE value. Use assertions to make sure
// that this is a legitimate operation.
bool isNull(Value *value)
{
    if (value == NULL)
    {
        exit(0);
    }
    return (value->type == NULL_TYPE);
}

// Measure length of list. Use assertions to make sure that this is a legitimate
// operation.
int length(Value *value)
{
    if (value == NULL)
    {
        exit(0);
    }
    int ctr = 0;
    Value *temp = value;
    while (!isNull(temp))
    {
        temp = cdr(temp);
        ctr++;
    }
    return ctr;
}