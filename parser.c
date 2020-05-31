#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

bool stringFlag = false;

// Helper function for parse that adds a token to the tree
Value *addToParseTree(Value *tree, int *depth, Value *token)
{
    if (token->type != CLOSE_TYPE)
    {
        if (token->type == OPEN_TYPE)
        {
            *depth = *depth + 1;
        }
        tree = cons(token, tree);
    }
    else
    {
        *depth = *depth - 1;
        Value *tempList = makeNull();
        while (car(tree)->type != OPEN_TYPE & cdr(tree)->type != NULL_TYPE)
        {
            tempList = cons(car(tree), tempList);
            tree = cdr(tree);
        }
        // Removing the ( from the tree
        tree = cdr(tree);
        tree = cons(tempList, tree);
    }
    return tree;
}

// Helper function to handle any syntax errors
void syntaxError(int depth)
{
    if (depth < 0)
    {
        printf("Syntax error: too many close parentheses.\n");
        texit(0);
    }
    else
    {
        printf("Syntax error: not enough close parentheses.\n");
        texit(0);
    }
}

Value *reverseTree(Value *tree) 
{
    Value *current = tree;
    Value *reversed = makeNull();
    while (!isNull(current))
    {
        reversed = cons(car(current), reversed);
        current = cdr(current);
    }
    return reversed;
}


// Takes a list of tokens from a Racket program, and returns a pointer to a
// parse tree representing that program.
Value *parse(Value *tokens) 
{
    Value *tree = makeNull();
    int depth = 0;
    Value *current = tokens;
    assert(current != NULL && "Error (parse): null pointer");
    while (current->type != NULL_TYPE)
    {
        Value *token = car(current);
        tree = addToParseTree(tree, &depth, token);
        current = cdr(current);
    }
    if (depth != 0)
    {
        syntaxError(depth);
    }

    return reverseTree(tree);
}

// Prints the tree to the screen in a readable fashion. It should look just like
// Racket code; use parentheses to indicate subtrees.
void printTree(Value *tree) 
{
  Value *stack = makeNull();
  Value *current = tree;
  assert(current != NULL && "Error (printTree): null pointer");
  bool firstEntry = true;
  while (firstEntry || (current->type != NULL_TYPE || !isNull(stack)))
  {
    firstEntry = false;
    if (current->type == NULL_TYPE)
    {
      printf(") ");
      current = car(stack);
      stack = cdr(stack);
      current = cdr(current);
    }
    else if (car(current)->type == BOOL_TYPE)
    {
      if (stringFlag)
      {
        printf(" ");
        stringFlag = false;
      }
      if (isNull(cdr(current)) & !isNull(stack))
      {
          current = car(stack);
          stack = cdr(stack);
          printf("%s) ", car(current)->s);
      }
      else
      {
          printf("%s ", car(current)->s);
      }
      current = cdr(current);
    }

    else if (car(current)->type == CONS_TYPE)
    {
        printf("(");
        stack = cons(current, stack);
        current = car(current);
    }

    else if (car(current)->type == SYMBOL_TYPE)
    {
        if (stringFlag)
        {
          printf(" ");
          stringFlag = false;
        }
        if (isNull(cdr(current)) & !isNull(stack)) 
        {
            printf("%s) ", car(current)->s);
            current = car(stack);
            stack = cdr(stack);
        }
        else
        {
          printf("%s ", car(current)->s);
        }
        current = cdr(current);
    }

    else if (car(current)->type == INT_TYPE)
    {
        if (stringFlag)
        {
          printf(" ");
          stringFlag = false;
        }
        if (isNull(cdr(current)) & !isNull(stack))
        {
            printf("%i) ", car(current)->i);
            current = car(stack);
            stack = cdr(stack);
        }
        else
        {
          printf("%i ", car(current)->i);
        }
        current = cdr(current);
    }

    else if (car(current)->type == DOUBLE_TYPE)
    {
        if (stringFlag)
        {
          printf(" ");
          stringFlag = false;
        }
        printf("%lf ", car(current)->d);
        if (isNull(cdr(current)) & !isNull(stack))
        {
            current = car(stack);
            stack = cdr(stack);
            printf(") ");
        }
        current = cdr(current);
    }

    else if (car(current)->type == STR_TYPE)
    {
        stringFlag = true;
        printf("\"%s\"", car(current)->s);
        if (isNull(cdr(current)) & !isNull(stack))
        {
            current = car(stack);
            stack = cdr(stack);
            printf(") ");
        }
        current = cdr(current);
    }
    else if (car(current)->type == NULL_TYPE)
    {
      printf("()");
      if (isNull(cdr(current)) & !isNull(stack)) 
        {
            printf(")");
            current = car(stack);
            stack = cdr(stack);
        }
      current = cdr(current);
    }
    
  }
}