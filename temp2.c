#include "value.h"
#include "parser.h"
#include "linkedlist.h"
#include "interpreter.h"
#include "talloc.h"
#include "tokenizer.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>


Value *lookUpSymbol(Value *tree, Frame *frame) {
	Frame *current = frame;
	
	while (current != NULL) {
		Value *binding = current->bindings;
		while(binding->type != NULL_TYPE) {
			Value *currentBinding = car(binding);
			Value *symbol = car(currentBinding);

			if(symbol->type == NULL_TYPE) {
				break;
			}
			else {
				Value *value = cdr(currentBinding);
				if (!strcmp(symbol->s, tree->s)) {
					return value;
				}
			}
		  binding = cdr(binding);
		}
    current = current->parent;
	}
  printf("Evaluation error: symbol '%s' not found.\n", tree->s);
  texit(0);
  return makeNull();
}

Value *lookUpBinding(Value *tree, Frame *frame) {
	Frame *current = frame;
	
	while (current != NULL) {
		Value *binding = current->bindings;
		while(binding->type != NULL_TYPE) {
			Value *currentBinding = car(binding);
			Value *symbol = car(currentBinding);

			if(symbol->type == NULL_TYPE) {
				break;
			}
			else {
				if (!strcmp(symbol->s, tree->s)) {
					return currentBinding;
				}
			}
		  binding = cdr(binding);
		}
    current = current->parent;
	}
  printf("Evaluation error: symbol '%s' not found.\n", tree->s);
  texit(0);
  return makeNull();
}

void printCons(Value *list) {
  while (!isNull(list)) {
    if (list->type != CONS_TYPE) {
      if (list->type == INT_TYPE) {
        printf(". %i", list->i);
      }
      else if (list->type == DOUBLE_TYPE) {
        printf(". %lf", list->d);
      }
      else if (list->type == STR_TYPE) {
        printf(". \"%s\"", list->s);
      }
      else if (list->type == BOOL_TYPE) {
        printf(". %s", list->s);
      }
      else if (list->type == SYMBOL_TYPE) {
        printf(". %s", list->s);
      }
      return;
    }
    else if (car(list)->type == INT_TYPE) {
			if (cdr(list)->type == NULL_TYPE) {
        printf("%i", car(list)->i);
      }
      else {
        printf("%i ", car(list)->i);
      }
    }
    else if (car(list)->type == DOUBLE_TYPE) {
      if (cdr(list)->type == NULL_TYPE) {
        printf("%lf", car(list)->d);
      }
      else {
        printf("%lf ", car(list)->d);
      }
    }
    else if (car(list)->type == STR_TYPE) {
      if (cdr(list)->type == NULL_TYPE) {
        printf("\"%s\"", car(list)->s);
      }
      else {
        printf("\"%s\" ", car(list)->s);
      }
      printf("\"%s\"", car(list)->s);
    }
    else if (car(list)->type == BOOL_TYPE) {
			if (cdr(list)->type == NULL_TYPE) {
				printf("%s", car(list)->s);
			}
      else {
        printf("%s ", car(list)->s);
      }
    }
    else if (car(list)->type == SYMBOL_TYPE) {
			if (cdr(list)->type == NULL_TYPE) {
        printf("%s", car(list)->s);
      }
      else {
        printf("%s ", car(list)->s);
      }
    }
    else if (car(list)->type == NULL_TYPE) {
      printf("() ");
    }
    else if (car(list)->type == CONS_TYPE) {
			printf("(");
      printCons(car(list));
		  printf(") ");
    }
    list = cdr(list);
  }
}

Value *evalIf(Value *args, Frame *frame) {
  Value *conditional;
  Value *trueArg;
  Value *falseArg;
  if (isNull(car(args))) {
    printf("Evaluation error: no consequent following an if.\n");
    texit(0);
  }

	else if (isNull(cdr(args))) {
		printf("Evaluation error: no consequent following an if.\n");
    texit(0);
	}
	
  else if (isNull(car(cdr(args)))) {
    printf("Evaluation error: no consequent following an if.\n");
    texit(0);
  }
  else if (isNull(cdr(cdr(args)))) {
    printf("Evaluation error: no consequent following an if.\n");
    texit(0);
  }
	else if (isNull(car(cdr(cdr(args))))) {
		printf("Evaluation error: no consequent following an if.\n");
    texit(0);
	}
	if (car(args)->type == BOOL_TYPE) {
		conditional = car(args);
	} 
  
  else if (car(args)->type == CONS_TYPE) {
    Value *symbol = eval(car(args), frame);

    if (symbol->type == BOOL_TYPE) {
      conditional = symbol;
    }
    else {
      printf("Non-conditional in if statement.\n");
      texit(0);
    }
  }

  else if (car(args)->type == SYMBOL_TYPE) {
    Value *symbol = lookUpSymbol(car(args), frame);

    if (symbol->type == BOOL_TYPE) {
      conditional = symbol;
    }
    else {
      printf("Non-conditional in if statement.\n");
      texit(0);
    }
  }
  else {
    printf("Invalid conditional in if statement.\n");
    texit(0);
  }

  if (isNull(cdr(args))) {
    printf("Incomplete if statement.\n");
    texit(0);
  }
  else if (isNull(car(cdr(args)))) {
    printf("Incomplete if statement.\n");
    texit(0);
  }
  else if (isNull(cdr(cdr(args)))) {
    printf("Incomplete if statement.\n");
    texit(0);
  }
  else if (isNull(car(cdr(cdr(args))))) {
    printf("Incomplete if statement.\n");
    texit(0);
  }
  trueArg = car(cdr(args));
  falseArg = car(cdr(cdr(args)));
  if (!strcmp(conditional->s, "#t")) {
    return eval(trueArg, frame);
  } 
  else if (!strcmp(conditional->s, "#f")) {
    return eval(falseArg, frame);
  }
  else {
    printf("Invalid boolean. Something went horribly wrong and the BOOL_TYPE does not have a #t or #f.\n");
    texit(0);
  }
  return makeNull();
}

bool checkBindings(Value *symbol, Value *bindings) {
  while (!isNull(bindings)) {
    if (!strcmp(car(car(bindings))->s, symbol->s)) {
      return true;
    }
    bindings = cdr(bindings);
  }
  return false;
} 

bool checkParams(Value *param, Value *newParams) {
  while (!isNull(newParams)) {
    if (!strcmp(car(newParams)->s, param->s)) {
      return true;
    }
    newParams = cdr(newParams);
  }
  return false;
} 


Value *evalEach(Value *args, Frame *frame) {
  Value *result = makeNull();
  Value *current = args;
  while (!isNull(current)) {
    result = cons(eval(car(current), frame), result);
    current = cdr(current);
  }
  return result;
}


Value *evalLet(Value *args, Frame *frame) {
  if (isNull(args)) {
    printf("Invalid arguments for let: No args.\n");
    texit(0);
  }
	else if (isNull(cdr(args))) {
		printf("Evaluation error: no args following the bindings in let.\n");
    texit(0);
	}

  Value *pairs = car(args);
  Value *expr = cdr(args);
	
  if (pairs->type != CONS_TYPE && pairs->type != NULL_TYPE) {
    printf("Evaluation error: bad form in let.\n");
    texit(0);
  }

  Frame *newFrame = talloc(sizeof(Frame));
	newFrame->parent = frame;
  Value *bindings = makeNull();

  while (!isNull(pairs) && pairs->type != NULL_TYPE) {
    Value *pair = car(pairs);
    if (isNull(pair)) {
      printf("Evaluation error: null binding in let.\n");
      texit(0);
    }
    else if (pair->type != CONS_TYPE) {
      printf("Evaluation error: bad form in let\n");
      texit(0);
    }
    else if (car(pair)->type != SYMBOL_TYPE) {
      printf("Evaluation error: left side of a let pair doesn't have a variable.\n");
      texit(0);
    }
    else if (isNull(cdr(pair))) {
      printf("Evaluation error: Value associated with symbol is null.\n");
      texit(0);
    }
    Value *symbol = car(pair);
    if (checkBindings(symbol, bindings)) {
      printf("Evaluation error: duplicate variable in let\n");
      texit(0);
    }
    Value *value = eval(car(cdr(pair)), frame);
    Value *newCell = cons(symbol, value);
    bindings = cons(newCell, bindings);
    pairs = cdr(pairs);
	}

	newFrame->bindings = bindings;

	while (cdr(expr)->type != NULL_TYPE){
		eval(car(expr), newFrame);
		expr = cdr(expr);
	}
  // Checking for repeats in bindings
	
  return eval(car(expr), newFrame);
}


Value *evalDefine(Value *args, Frame *frame) {
  if (isNull(args)) {
    printf("Evaluation error: no args following define.\n");
    texit(0);
  }
  else if (isNull(car(args))) {
		printf("Error: no arguments in define\n");
		texit(0);
	}
  else if (isNull(cdr(args))) {
    printf("Evaluation error: no value following the symbol in define.\n");
    texit(0);
  }
	else if (frame == NULL) {
		printf("Error: no frame found\n");
		texit(0);
	}
  else if (car(args)->type != SYMBOL_TYPE) {
    printf("Evaluation error: define must bind to a symbol.\n");
    texit(0);
  }
  Value *var = car(args);
  Value *expr = eval(car(cdr(args)), frame);
	Value *value = cons(var, expr);
  Value *toBeAdded = cons(value, frame->bindings);
  frame->bindings = toBeAdded;
  Value *returnVal = talloc(sizeof(Value));
  returnVal->type = VOID_TYPE;
  return returnVal;
}

Value *evalLambda(Value *args, Frame *frame) {
  if (isNull(args)) {
    printf("Evaluation error: no args following lambda.\n");
    texit(0);
  }
  else if (isNull(cdr(args))) {
    printf("Evaluation error: no code in lambda following parameters.\n");
    texit(0);
  }
	else if (frame == NULL) {
		printf("Error: no frame found in lambda.\n");
		texit(0);
	}
  Value *params = car(args);
  Value *body = car(cdr(args));
  Value *value = talloc(sizeof(Value));
  value->type = CLOSURE_TYPE;
  value->cl.frame = frame;
  value->cl.functionCode = body;
  Value *paramNames = makeNull();
  while (!isNull(params)) {
    if (car(params)->type != SYMBOL_TYPE) {
      printf("Evaluation error: formal parameters for lambda must be symbols.\n");
      texit(0);
    }
    if (checkParams(car(params), paramNames)) {
      printf("Evaluation error: duplicate identifier in lambda.\n");
      texit(0);
    }
    paramNames = cons(car(params), paramNames);
    params = cdr(params);
  }
  value->cl.paramNames = paramNames;
  return value;
}

Value *evalLetStar(Value *args, Frame *frame) {
  if (isNull(args)) {
    printf("Invalid arguments for let*: No args.\n");
    texit(0);
  }
	else if (isNull(cdr(args))) {
		printf("Evaluation error: no args following the bindings in let*.\n");
    texit(0);
	}

  Value *pairs = car(args);
  Value *expr = cdr(args);
	
  if (pairs->type != CONS_TYPE && pairs->type != NULL_TYPE) {
    printf("Evaluation error: bad form in let*.\n");
    texit(0);
  }

  while (!isNull(pairs) && pairs->type != NULL_TYPE) {
    Value *pair = car(pairs);
    if (isNull(pair)) {
      printf("Evaluation error: null binding in let*.\n");
      texit(0);
    }
    else if (pair->type != CONS_TYPE) {
      printf("Evaluation error: bad form in let*\n");
      texit(0);
    }
    else if (car(pair)->type != SYMBOL_TYPE) {
      printf("Evaluation error: left side of a let* pair doesn't have a variable.\n");
      texit(0);
    }
    else if (isNull(cdr(pair))) {
      printf("Evaluation error: Value associated with symbol is null.\n");
      texit(0);
    }
    Value *symbol = car(pair);
    Value *bindings = makeNull();
    Frame *newFrame = talloc(sizeof(Frame));
    newFrame->parent = frame;
    newFrame->bindings = bindings;
    Value *value = eval(car(cdr(pair)), newFrame);
    Value *newCell = cons(symbol, value);
    bindings = cons(newCell, bindings);
    newFrame->bindings = bindings;
    frame = newFrame;
    pairs = cdr(pairs);
	}

  while (cdr(expr)->type != NULL_TYPE){
		eval(car(expr), frame);
		expr = cdr(expr);
	}
  return eval(car(expr), frame);
}

Value *evalLetrec(Value *args, Frame *frame) {
  if (isNull(args)) {
    printf("Invalid arguments for let: No args.\n");
    texit(0);
  }
	else if (isNull(cdr(args))) {
		printf("Evaluation error: no args following the bindings in let.\n");
    texit(0);
	}

  Value *pairs = args->c.car;
  Value *expr = cdr(args);
	
  if (pairs->type != CONS_TYPE && pairs->type != NULL_TYPE) {
    printf("Evaluation error: bad form in let.\n");
    texit(0);
  }

  Frame *newFrame = talloc(sizeof(Frame));
	newFrame->parent = frame;

  Value *bindings = makeNull();
	Value *valuesList = makeNull();

  while (!isNull(pairs) && pairs->type != NULL_TYPE) {
		if (car(car(pairs))->type != SYMBOL_TYPE) {
			printf("Evaluation error: Invalid binding in let.\n");
      texit(0);
		}
		valuesList = cons(car(cdr(car(pairs))), valuesList);
		Value *newBinding = makeNull();
		newBinding = cons(car(car(pairs)), newBinding);
		bindings = cons(newBinding, bindings);
		pairs = cdr(pairs);
	}

	bindings = reverse(bindings);
	newFrame->bindings = bindings;

	valuesList = reverse(valuesList);
	valuesList = evalEach(valuesList, newFrame);
Value *first =car(cdr((valuesList)))->cl.paramNames;
printf("valuesList %s\n", car(first)->s);

	Value *pairsName = bindings;
	Value *pairsValue = valuesList;
	while(pairsName->type != NULL_TYPE) { 
			car(pairsName)->c.cdr = cons(car(pairsValue), makeNull());
			pairsName = cdr(pairsName);
			pairsValue = cdr(pairsValue);
	}
	pairs = cdr(args);
	while (cdr(pairs)->type != NULL_TYPE) {
			pairs = cdr(pairs);
	}
	return eval(car(pairs), newFrame);
}

Value *evalSet(Value *args, Frame *frame) {
  if (isNull(args)) {
    printf("Evaluation error: no args following set!.\n");
    texit(0);
  }
  else if (isNull(car(args))) {
		printf("Error: no arguments in set!\n");
		texit(0);
	}
  else if (isNull(cdr(args))) {
    printf("Evaluation error: no value following the symbol in set!.\n");
    texit(0);
  }
	else if (frame == NULL) {
		printf("Error: no frame found.\n");
		texit(0);
	}
  else if (car(args)->type != SYMBOL_TYPE) {
    printf("Evaluation error: set! must bind to a symbol.\n");
    texit(0);
  }
  Value *var = car(args);
  Value *expr = eval(car(cdr(args)), frame);
  Value *bindings = frame->bindings;

  Value *binding = lookUpBinding(var, frame);
  binding->c.cdr = expr;

  Value *returnVal = talloc(sizeof(Value));
  returnVal->type = VOID_TYPE;
  return returnVal;
}

Value *evalBegin(Value *args, Frame *frame) {
    assert(args->type == NULL_TYPE || args->type == CONS_TYPE);
    Value *current = args;
    Value *result = talloc(sizeof(Value));
    result->type = VOID_TYPE;
    if (current->type == NULL_TYPE){
			return result;
		}
    while (cdr(current)->type != NULL_TYPE) {
			Value *curValue = eval(car(current),frame);
			current = cdr(current); 
   }
   return eval(car(current),frame); 
}

Value *evalAnd(Value *args, Frame *frame) {
    Value *result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;
    if (length(args) == 0) {
        result->s = "#t";
        return result;
    }
    Value *current = args;
    assert(current->type == CONS_TYPE);
    while (cdr(current)->type != NULL_TYPE) {
        Value *current_value = eval(car(current), frame);
        if (current_value->type == BOOL_TYPE && (!strcmp(current_value->s, "#f"))) {
            result->s = "#f";
            return result;
        }
        current = cdr(current);
    }
    return eval(car(current),frame);
}

Value *evalOr(Value *args, Frame *frame) {
    Value *result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;
    if (length(args) == 0) {
        result->s = "#f";
        return result;
    }
    Value *current = args;
    assert(current->type == CONS_TYPE);
    while (cdr(current)->type != NULL_TYPE) {
        Value *current_value = eval(car(current), frame);
        if (!(current_value->type == BOOL_TYPE && (!strcmp(current_value->s, "#f")))) {
            return current_value;
        }
        current = cdr(current);
    }
    return eval(car(current),frame);
}


Value *evalAdd(Value *args) {
	Value *result = talloc(sizeof(Value));
	result->type = INT_TYPE;
	double result_value = 0;
	Value *current = args;
	while(!isNull(current)) {
		Value *current_value = car(current);
		if (current_value->type == DOUBLE_TYPE) {
			result->type = DOUBLE_TYPE;
			result_value += current_value->d;
		}
		else if (current_value->type == INT_TYPE) {
			result_value += current_value->i;			
		}
		else {
			printf("Evaluation Error: Unexpected argument\n");
			texit(0);
		}
		current = cdr(current);
	}
	
	if (result->type == INT_TYPE) {
		result->i = (int) result_value;
	}
	else if (result->type == DOUBLE_TYPE) {
		result->d = result_value;
	}
	return result;
}

Value *evalSub(Value *args) {
    Value *result = talloc(sizeof(Value));
    result->type = car(args)->type;
    double result_value;
    
    if (result->type == INT_TYPE) {
        if (length(args) == 1) {
            result->i = 0 - car(args)->i;
            return result;
        } else {
            result_value = car(args)->i;
        } 
    } else if (result->type == DOUBLE_TYPE) {
        if (length(args) == 1) {
            result->d = 0 - car(args)->d;
            return result;
        } else {
            result_value = car(args)->d;
        }
    } else {
        printf("Evaluation Error: Wrong arguments for subtraction. \n");
        texit(0);
    }

    Value *current = cdr(args); 
    while(!isNull(current)) {
        Value *current_value = car(current);
        if (current_value->type != INT_TYPE) {
            if (current_value->type == DOUBLE_TYPE) {
                result->type = DOUBLE_TYPE;
                result_value -= current_value->d;
            } else {
                printf("Evaluation Error: Wrong arguments for subtraction. \n");
                texit(0);
            }
        } else {
            result_value -= current_value->i;    
        }
        current = cdr(current);
    }
    
    if (result->type == INT_TYPE) {
        result->i = (int) result_value * -1;
    } else {
        result->d = result_value  * -1;
    }
    return result;
}

Value *evalLess(Value *args) {
    Value *result = talloc(sizeof(Value));
    if (length(args) < 2) {
        printf("Evaluation Error: Wrong number of arguments for equal.\n");
        texit(0);
    }
    result->type = BOOL_TYPE; 
    result->s = "#t";
    double smallest;
    
    if (car(args)->type == INT_TYPE) {
        smallest = car(args)->i;
    } else if (car(args)->type == DOUBLE_TYPE) {
        smallest = car(args)->d;
    } else {
        printf("Evaluation Error: Wrong type of arguments for <.\n");
        texit(0);
    }

    Value *current = cdr(args); 
    while (!isNull(current)) {
        Value *current_value = car(current);
        if (current_value->type != INT_TYPE) {
            if (current_value->type == DOUBLE_TYPE) {
                if (smallest > current_value->d) {
                    smallest = current_value->d;
                } else {
                    result->s = "#f";
                    return result;
                }
            } else {
								printf("Evaluation Error: Wrong number of arguments for <.\n");
								texit(0);
            }
        } else {
            if (smallest > current_value->i) {
                smallest = current_value->i;
            } else {
                result->s = "#f";
                return result;
            }    
        }
        current = cdr(current);
    }
    return result;    
}

Value *evalGreater(Value *args) {
    Value *result = talloc(sizeof(Value));
    if (length(args) < 2) {
        printf("Evaluation Error: Wrong number of arguments for equal.\n");
        texit(0);
    }
    result->type = BOOL_TYPE; 
    result->s = "#t";
    double largest;
    
    if (car(args)->type == INT_TYPE) {
        largest = car(args)->i;
    } else if (car(args)->type == DOUBLE_TYPE) {
        largest = car(args)->d;
    } else {
        printf("Evaluation Error: Wrong type of arguments for <.\n");
        texit(0);
    }

    Value *current = cdr(args); 
    while (!isNull(current)) {
        Value *current_value = car(current);
        if (current_value->type != INT_TYPE) {
            if (current_value->type == DOUBLE_TYPE) {
                if (largest < current_value->d) {
                    largest = current_value->d;
                } else {
                    result->s = "#f";
                    return result;
                }
            } else {
								printf("Evaluation Error: Wrong number of arguments for <.\n");
								texit(0);
            }
        } else {
            if (largest < current_value->i) {
                largest = current_value->i;
            } else {
                result->s = "#f";
                return result;
            }    
        }
        current = cdr(current);
    }
    return result;    
}


Value *evalEqual(Value *args) {
    if (length(args) != 2) {
        printf("Evaluation Error: Wrong number of arguments for equal.\n");
        texit(0);
    }
    Value *result = talloc(sizeof(Value));
    result->type = BOOL_TYPE;
    bool resultBool = true;
    
    Value *first = car(args);
    Value *second = car(cdr(args));
    
    switch (first->type) {
        case BOOL_TYPE:
            resultBool = (second->type == BOOL_TYPE &&
                          !strcmp(first->s, second->s) );
            break;
        case SYMBOL_TYPE:
            resultBool = (second->type == SYMBOL_TYPE &&
                         !strcmp(first->s, second->s));
            break;
        case INT_TYPE:
            resultBool = (second->type == INT_TYPE &&
                          first->i == second->i);
            break;
        case DOUBLE_TYPE:
            resultBool = (second->type == DOUBLE_TYPE &&
                          first->d == second->d);
            break;
        case STR_TYPE:
            resultBool = (second->type == STR_TYPE &&
                         !strcmp(first->s, second->s));
            break;
        case NULL_TYPE:
            resultBool = second->type == NULL_TYPE;
            break;
        case CONS_TYPE:
            resultBool = (second->type == CONS_TYPE &&
                          &first->c == &second->c);
            break;
        case CLOSURE_TYPE:
            resultBool = (second->type == CLOSURE_TYPE &&
                          &first->cl == &second->cl);
            break;
        case PRIMITIVE_TYPE:
            resultBool = (second->type == PRIMITIVE_TYPE &&
                          &first->pf == &second->pf);
            break;

				case UNSPECIFIED_TYPE:
						resultBool = (second->type == UNSPECIFIED_TYPE &&
                          &first == &second);
        default:
            resultBool = (second->type == CLOSURE_TYPE &&
                          &first == &second);
            break;
    }
    if (resultBool) {
        result->s = "#t";
    } else {
        result->s = "#f";
    }
    return result;
}

Value *evalNull(Value *args) {
  if (isNull(args)) {
    printf("Evaluation Error: Null? has no arguments.\n");
    texit(0);
  }
  else if (!isNull(cdr(args))) {
    printf("Evaluation Error: Null? has too many arguments.\n");
    texit(0);
  }
  Value *toBeChecked = car(args);
  Value *result = talloc(sizeof(Value));
  result->type = BOOL_TYPE;
  if (toBeChecked->type == CONS_TYPE) {
    if (isNull(car(toBeChecked))) {
      result->s = "#t";
    }
    else {
      result->s = "#f";
    }
  }
  else if(isNull(toBeChecked)) {
    result->s = "#t";
  }
  else {
    result->s = "#f";
  }
  return result;
}


Value *evalCar(Value *args) {
  if (isNull(args)) {
    printf("Evaluation Error: car has no arguments.\n");
    texit(0);
  }
  else if (!isNull(cdr(args))) {
    printf("Evaluation Error: car has too many arguments.\n");
    texit(0);
  }
  else if (car(args)->type != CONS_TYPE) {
    printf("Evaluation error: cannot get cdr of non-list.\n");
		texit(0);
  }
  Value *firstArg = car(args);
  if (car(firstArg)->type != CONS_TYPE) {
    return car(firstArg);
  }
  return car(car(firstArg));
}

Value *evalCdr(Value *args) {
  if (isNull(args)) {
    printf("Evaluation Error: cdr has no arguments.\n");
    texit(0);
  }
  else if (!isNull(cdr(args))) {
    printf("Evaluation Error: cdr has too many arguments.\n");
    texit(0);
  }
	else if (args->type != CONS_TYPE) {
		printf("Evaluation Error: cannot get cdr of non-list. \n");
		texit(0);
	}
  Value *firstArg = car(args);
  if (car(firstArg)->type != CONS_TYPE) {
    return cdr(firstArg);
  }
  return cdr(car(firstArg));
}


Value *evalCons(Value *args) {
  if (isNull(args)) {
    printf("Evaluation Error: cons has no arguments.\n");
    texit(0);
  }
  else if (isNull(cdr(args))) {
    printf("Evaluation Error: cons has too few arguments.\n");
    texit(0);
  }
	else if (length(args) != 2) {
		printf("Evaluation Error: cons requires only two arguements.\n");
    texit(0);
	}

  Value *secondParam = car(args);
  Value *firstParam = car(cdr(args));
  Value *result;

  result = cons(firstParam, secondParam);
  	
  return result;
}


Value *apply(Value *function, Value *args) {
  if (function->type == CLOSURE_TYPE) {
    Frame *frame = talloc(sizeof(Frame));
    frame->parent = function->cl.frame;
    Value *newBindings = makeNull();
    Value *params = function->cl.paramNames;
    while (!isNull(args)) {
      if (isNull(params)) {
        printf("Evaluation Error: Too many arguments to function\n");
        texit(0);
      }
      Value *value = cons(car(params), car(args));
      newBindings = cons(value, newBindings);
      params = cdr(params);
      args = cdr(args);
    }
    if (!isNull(params)) {
      printf("Evaluation Error: Too few arguments to function\n");
      texit(0);
    }
    frame->bindings = newBindings;
    return eval(function->cl.functionCode, frame);
  }
  else if (function->type == PRIMITIVE_TYPE) {
    return (function->pf)(args);
  }
	else if (function->type == UNSPECIFIED_TYPE) {
    return function;
  }
  else {
    printf("Evaluation Error: Trying to execute non-function.\n");
    texit(0);
  }
  return makeNull();
  
}

void bind(char *name, Value *(*function)(struct Value *), Frame *frame) {
  // Add primitive functions to top-level bindings list
  Value *value = talloc(sizeof(Value));
  value->type = PRIMITIVE_TYPE;
  value->pf = function;
  Value *primitive = talloc(sizeof(Value));
  primitive->type = SYMBOL_TYPE;
  primitive->s = name;
  Value *newBinding = cons(primitive, value);
  frame->bindings = cons(newBinding, frame->bindings);
  return;
}


void evaluationError() {
  printf("Error: Unrecognized special form. Please only use if and let for now.\n");
  texit(0);
}

Value *eval(Value *expr, Frame *frame) {
	Value *tree = expr;

  switch (tree->type)  {
     case INT_TYPE: {
        return tree;
        break;
     }
		 case DOUBLE_TYPE: {
        return tree;
        break;
     }
     case BOOL_TYPE: {
        return tree;
        break;
     }  
     case SYMBOL_TYPE: {
        return lookUpSymbol(tree, frame);
        break;
     }
     case STR_TYPE: {
        return tree;
        break;
     }  
     case CONS_TYPE: {
        Value *first = car(tree);
        Value *args = cdr(tree);

        if (!strcmp(first->s,"if")) {
            return evalIf(args,frame);
        }
				
				else if (!strcmp(first->s,"let")) {
            return evalLet(args,frame);
        }

        else if (!strcmp(first->s, "quote")) {
            if (!isNull(cdr(args))) {
              printf("Evaluation error: multiple arguments to quote\n");
              texit(0);
            }
						Value *current = car(args);
						return current;
        }
        
        else if (!strcmp(first->s, "let*")) {
          return evalLetStar(args, frame);
        }

        else if (!strcmp(first->s, "letrec")) {
          return evalLetrec(args, frame);
        }

        else if (!strcmp(first->s, "set!")) {
          return evalSet(args, frame);
        }

        else if (!strcmp(first->s, "begin")) {
          return evalBegin(args, frame);
        }

        else if (!strcmp(first->s, "and")) {
          return evalAnd(args, frame);
        }

        else if (!strcmp(first->s, "or")) {
          return evalOr(args, frame);
        }

        else if (!strcmp(first->s, "define")) {
          return evalDefine(args, frame);
        }

        else if (!strcmp(first->s, "lambda")) {
          return evalLambda(args, frame);
        }

        else {
          // If not a special form, evaluate the first, evaluate the args, then
          // apply the first to the args.
          Value *evaledOperator = eval(first, frame);
          Value *evaledArgs = evalEach(args, frame);
          return apply(evaledOperator,evaledArgs);
        }
        break;
      }
      default: {
        break;
      }
    }    
    return NULL;
}


void interpret(Value *tree) {
  Frame *frame = talloc(sizeof(Frame));
	frame->bindings = makeNull();
	frame->parent = NULL;

	bind("+", evalAdd,frame);
	bind("null?", evalNull,frame);
	bind("car", evalCar,frame);
	bind("cdr", evalCdr,frame);
	bind("cons", evalCons,frame);
	bind("-", evalSub,frame);
	bind("<", evalLess,frame);
	bind(">", evalGreater,frame);
	bind("=", evalEqual,frame);

	Value *current = tree;
	while (current != NULL && current->type == CONS_TYPE) {
		Value *result = eval(car(current), frame);
		
		if (result->type == INT_TYPE) {
			printf("%i\n", result->i);
		}
		else if (result->type == DOUBLE_TYPE) {
      printf("%lf\n", result->d);
    }
    else if (result->type == STR_TYPE) {
      printf("\"%s\"\n", result->s);
    }
    else if (result->type == BOOL_TYPE) {
      printf("%s\n", result->s);
    }
		else if (result->type == CONS_TYPE) {
      printf("(");
			printCons(result);
      printf(")");
			printf("\n");
		}
		else if (result->type == NULL_TYPE) {
      printf("(");

      printf(")\n");
		}
    else if (result->type == VOID_TYPE) {
      
    }
    else if (result->type == CLOSURE_TYPE) {
      printf("#<procedure>\n");
    }
		else if (result->type == UNSPECIFIED_TYPE) {
      printf("#<unspecified>\n");
    }
    else {
      printf("%s\n", result->s);
    }
	  current = cdr(current);
	}
}
