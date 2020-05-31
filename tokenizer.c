#include "tokenizer.h"
#include "linkedlist.h"
#include "talloc.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Value *tokenize()
{
    char charRead;
    Value *list = makeNull();
    charRead = (char)fgetc(stdin);

    char boolBuffer[3];
    bool boolFlag = false;

    char numberBuffer[300];
    bool numberFlag = false;
    int numberUsedIndex = 0;

    char symbolBuffer[300];
    bool symbolFlag = false;
    int symbolUsedIndex = 0;
    const char *symbols = "+-!$%&*/:<=>?~_^,'";

    char stringBuffer[300];
    bool stringFlag = false;
    int stringUsedIndex = 0;

    bool commentFlag = false;
    bool negativeFlag = false;
    bool dotFlag = false;

    while (charRead != EOF)
    {
        if (charRead == '\n')
        {
            if (dotFlag && !commentFlag)
            {
                printf("Error: . is present alone\n");
                texit(0);
            }
            if (commentFlag)
            {
                commentFlag = false;
            }
            if (boolFlag)
            {
                Value *val = talloc(sizeof(Value));
                val->type = BOOL_TYPE;
                val->s = talloc(sizeof(char) * (strlen(boolBuffer) + 1));
                strcpy(val->s, boolBuffer);
                list = cons(val, list);

                boolBuffer[0] = '\0';
                boolFlag = false;
            }
            else if (numberFlag)
            {
                if (strchr(numberBuffer, '.') == NULL)
                {
                    Value *val = talloc(sizeof(Value));
                    val->type = INT_TYPE;
                    val->i = atoi(numberBuffer);
                    list = cons(val, list);
                }
                else
                {
                    Value *val = talloc(sizeof(Value));
                    val->type = DOUBLE_TYPE;
                    val->d = strtof(numberBuffer, NULL);
                    list = cons(val, list);
                }
                numberBuffer[0] = '\0';
                numberUsedIndex = 0;
                numberFlag = false;
            }
            else if (symbolFlag) {
              Value *val = talloc(sizeof(Value));
              val->type = SYMBOL_TYPE;
              val->s = talloc(sizeof(char) * (strlen(symbolBuffer) + 1));
              strcpy(val->s, symbolBuffer);
              list = cons(val, list);
              symbolBuffer[0] = '\0';
              symbolFlag = false;
              symbolUsedIndex = 0;
            }
        }
        // Open paren checking
        else if (charRead == '(')
        {
            if (commentFlag)
            {
            }
            else if (stringFlag)
            {
                stringBuffer[stringUsedIndex] = charRead;
                stringUsedIndex++;
                stringBuffer[stringUsedIndex] = '\0';
            }
            else if (boolFlag)
            {
                printf("Error: ( is not a boolean\n");
                texit(0);
            }
            else
            {
                Value *val = talloc(sizeof(Value));
                val->type = OPEN_TYPE;
                char text[2];
                text[0] = charRead;
                text[1] = '\0';
                val->s = talloc(sizeof(char) * (strlen(text) + 1));
                strcpy(val->s, text);
                list = cons(val, list);
            }
        }

        // Close paren checking
        else if (charRead == ')')
        {
            if (commentFlag)
            {
            }
            else if (numberFlag)
            {
                if (strchr(numberBuffer, '.') == NULL)
                {
                    Value *val = talloc(sizeof(Value));
                    val->type = INT_TYPE;
                    val->i = atoi(numberBuffer);
                    list = cons(val, list);
                }
                else
                {
                    Value *val = talloc(sizeof(Value));
                    val->type = DOUBLE_TYPE;
                    val->d = strtof(numberBuffer, NULL);
                    list = cons(val, list);
                }
                numberBuffer[0] = '\0';
                numberUsedIndex = 0;
                numberFlag = false;

                Value *parenVal = talloc(sizeof(Value));
                parenVal->type = CLOSE_TYPE;
                char text[2];
                text[0] = charRead;
                text[1] = '\0';
                parenVal->s = talloc(sizeof(char) * (strlen(text) + 1));
                strcpy(parenVal->s, text);
                list = cons(parenVal, list);
            }
            else if (stringFlag)
            {
                stringBuffer[stringUsedIndex] = charRead;
                stringUsedIndex++;
                stringBuffer[stringUsedIndex] = '\0';
            }
            else if (symbolFlag)
            {
                Value *val = talloc(sizeof(Value));
                val->type = SYMBOL_TYPE;
                val->s = talloc(sizeof(char) * (strlen(symbolBuffer) + 1));
                strcpy(val->s, symbolBuffer);
                list = cons(val, list);
                symbolBuffer[0] = '\0';
                symbolFlag = false;
                symbolUsedIndex = 0;

                Value *parenVal = talloc(sizeof(Value));
                parenVal->type = CLOSE_TYPE;
                char text[2];
                text[0] = charRead;
                text[1] = '\0';
                parenVal->s = talloc(sizeof(char) * (strlen(text) + 1));
                strcpy(parenVal->s, text);
                list = cons(parenVal, list);
            }
            else if (boolFlag)
            {
                printf("Error: ) is not a boolean\n");
                texit(0);
            }
            else
            {
                Value *val = talloc(sizeof(Value));
                val->type = CLOSE_TYPE;
                char text[2];
                text[0] = charRead;
                text[1] = '\0';
                val->s = talloc(sizeof(char) * (strlen(text) + 1));
                strcpy(val->s, text);
                list = cons(val, list);
            }
        }

        // Checking if we are inside a string
        else if (charRead == '"')
        {
            if (commentFlag)
            {
            }
            else if (!stringFlag)
            {
                stringFlag = true;
            }
            else if (boolFlag)
            {
                printf("Error: \" is not a boolean\n");
                texit(0);
            }
            else
            {
                stringFlag = false;
                Value *val = talloc(sizeof(Value));
                val->type = STR_TYPE;
                stringBuffer[stringUsedIndex] = '\0';
                val->s = talloc(sizeof(char) * (strlen(stringBuffer) + 1));
                strcpy(val->s, stringBuffer);
                list = cons(val, list);
                stringUsedIndex = 0;
                stringBuffer[stringUsedIndex] = '\0';
            }
        }

        // Checking if any of predefined symbols
        else if (strchr(symbols, charRead) != NULL)
        {
            if (commentFlag)
            {
            }
            else if (symbolFlag)
            {
                symbolBuffer[symbolUsedIndex] = charRead;
                symbolUsedIndex++;
                symbolBuffer[symbolUsedIndex] = '\0';
            }
            else if (charRead == '-')
            {
                negativeFlag = true;
            }
            else if (stringFlag)
            {
                stringBuffer[stringUsedIndex] = charRead;
                stringUsedIndex++;
                stringBuffer[stringUsedIndex] = '\0';
            }
            else if (numberFlag)
            {
                printf("Error: cannot have %c following a number\n", charRead);
                texit(0);
            }
            else if (boolFlag)
            {
                printf("Error: %c is not a boolean\n", charRead);
                texit(0);
            }
            else
            {
                Value *val = talloc(sizeof(Value));
                val->type = SYMBOL_TYPE;
                char text[2];
                text[0] = charRead;
                text[1] = '\0';
                val->s = talloc(sizeof(char) * (strlen(text) + 1));
                strcpy(val->s, text);
                list = cons(val, list);
            }
        }

        // Cheking if we are inside of the ingeger/double
        else if (isdigit(charRead))
        {
            if (commentFlag)
            {
            }
            else if (negativeFlag)
            {
                numberFlag = true;
                numberBuffer[numberUsedIndex] = '-';
                numberUsedIndex++;
                numberBuffer[numberUsedIndex] = charRead;
                numberUsedIndex++;
                numberBuffer[numberUsedIndex] = '\0';
                negativeFlag = false;
            }
            else if (boolFlag)
            {
                printf("Error: %c is not a boolean\n", charRead);
                texit(0);
            }
            else if (stringFlag)
            {
                stringBuffer[stringUsedIndex] = charRead;
                stringUsedIndex++;
                stringBuffer[stringUsedIndex] = '\0';
            }
            else if (symbolFlag)
            {
                symbolBuffer[symbolUsedIndex] = charRead;
                symbolUsedIndex++;
                symbolBuffer[symbolUsedIndex] = '\0';
            }
            else if (!numberFlag)
            {
                numberFlag = true;
                numberBuffer[numberUsedIndex] = charRead;
                numberUsedIndex++;
                numberBuffer[numberUsedIndex] = '\0';
            }
            else
            {
                numberBuffer[numberUsedIndex] = charRead;
                numberUsedIndex++;
                numberBuffer[numberUsedIndex] = '\0';
            }
        }

        // Cheking if '.' is a part of the double or string or symbol
        else if (charRead == '.')
        {
            if (commentFlag)
            {
            }

            else if (stringFlag)
            {
                stringBuffer[stringUsedIndex] = charRead;
                stringUsedIndex++;
            }
            else if (symbolFlag)
            {
                printf("Error: . cannot follow symbol\n");
                texit(0);
            }
            else if (boolFlag)
            {
                printf("Error: . is not a boolean\n");
                texit(0);
            }
            else if (numberFlag)
            {
                numberBuffer[numberUsedIndex] = charRead;
                numberUsedIndex++;
                numberBuffer[numberUsedIndex] = '\0';
            }
            else
            {
                numberFlag = true;
                dotFlag = true;
                numberBuffer[numberUsedIndex] = charRead;
                numberUsedIndex++;
                numberBuffer[numberUsedIndex] = '\0';
            }
        }

        // Checking if # is a part of the string or the beginning of the boolean
        else if (charRead == '#')
        {
            if (commentFlag)
            {
            }

            else if (stringFlag)
            {
                stringBuffer[stringUsedIndex] = charRead;
                stringUsedIndex++;
                stringBuffer[stringUsedIndex] = '\0';
            }
            else if (!boolFlag)
            {
                boolFlag = true;
            }
        }

        // Checking if 't' and 'f' are part of the string or the boolean
        else if (charRead == 't' || charRead == 'f')
        {
            if (commentFlag)
            {
            }
            else if (symbolFlag)
            {
                symbolBuffer[symbolUsedIndex] = charRead;
                symbolUsedIndex++;
                symbolBuffer[symbolUsedIndex] = '\0';
            }
            else if (stringFlag)
            {
                stringBuffer[stringUsedIndex] = charRead;
                stringUsedIndex++;
                stringBuffer[stringUsedIndex] = '\0';
            }
            else if (boolFlag)
            {
                boolBuffer[0] = '#';
                boolBuffer[1] = charRead;
                boolBuffer[2] = '\0';

                Value *val = talloc(sizeof(Value));
                val->type = BOOL_TYPE;
                val->s = talloc(sizeof(char) * (strlen(boolBuffer) + 1));
                strcpy(val->s, boolBuffer);
                list = cons(val, list);

                boolBuffer[0] = '\0';
                boolFlag = false;
            }
            else
            {
                symbolFlag = true;
                symbolBuffer[symbolUsedIndex] = charRead;
                symbolUsedIndex++;
                symbolBuffer[symbolUsedIndex] = '\0';
            }
        }

        // Checking for variables and such
        else if (isalpha(charRead))
        {
            if (commentFlag)
            {
            }
            else if (stringFlag)
            {
                stringBuffer[stringUsedIndex] = charRead;
                stringUsedIndex++;
                stringBuffer[stringUsedIndex] = '\0';
            }
            else if (numberFlag)
            {
                printf("Error: Cannot have letter %c following number\n", charRead);
                texit(0);
            }
            else if (boolFlag)
            {
                printf("Error: invalid boolean type #%c\n", charRead);
                texit(0);
            }
            else if (!symbolFlag)
            {
                symbolFlag = true;
                symbolBuffer[symbolUsedIndex] = charRead;
                symbolUsedIndex++;
                symbolBuffer[symbolUsedIndex] = '\0';
            }
            else
            {
                symbolBuffer[symbolUsedIndex] = charRead;
                symbolUsedIndex++;
                symbolBuffer[symbolUsedIndex] = '\0';
            }
        }

        else if (charRead == ';')
        {
            if (stringFlag)
            {
                stringBuffer[stringUsedIndex] = charRead;
                stringUsedIndex++;
                stringBuffer[stringUsedIndex] = '\0';
            }
            else if (!commentFlag)
            {
                commentFlag = true;
            }
        }

        // if the character is a whitespace, add the previous item to the list(for string, integer, double, multi-char-symbol, boolean)
        else if (charRead == ' ')
        {
            if (commentFlag)
            {
            }
            else if (negativeFlag)
            {
                negativeFlag = false;
                Value *val = talloc(sizeof(Value));
                val->type = SYMBOL_TYPE;
                val->s = talloc(sizeof(char) + 1);
                strcpy(val->s, "-");
                list = cons(val, list);
            }
            else if (stringFlag)
            {
                stringBuffer[stringUsedIndex] = charRead;
                stringUsedIndex++;
            }
            else if (symbolFlag)
            {
                Value *val = talloc(sizeof(Value));
                val->type = SYMBOL_TYPE;
                val->s = talloc(sizeof(char) * (strlen(symbolBuffer) + 1));
                strcpy(val->s, symbolBuffer);
                list = cons(val, list);
                symbolBuffer[0] = '\0';
                symbolFlag = false;
                symbolUsedIndex = 0;
            }
            else if (numberFlag)
            {
                if (strchr(numberBuffer, '.') == NULL)
                {
                    Value *val = talloc(sizeof(Value));
                    val->type = INT_TYPE;
                    val->i = atoi(numberBuffer);
                    list = cons(val, list);
                }
                else
                {
                    Value *val = talloc(sizeof(Value));
                    val->type = DOUBLE_TYPE;
                    val->d = strtof(numberBuffer, NULL);
                    list = cons(val, list);
                }
                numberBuffer[0] = '\0';
                numberUsedIndex = 0;
                numberFlag = false;
            }

            else if (boolFlag)
            {
                Value *val = talloc(sizeof(Value));
                val->type = BOOL_TYPE;
                val->s = talloc(sizeof(char) * (strlen(boolBuffer) + 1));
                strcpy(val->s, boolBuffer);
                list = cons(val, list);

                boolBuffer[0] = '\0';
                boolFlag = false;
            }
        }
        else
        {
            printf("Error: Invalid character %c cannot be tokenized\n", charRead);
            texit(0);
        }
        charRead = (char)fgetc(stdin);
    }
    if (numberFlag)
    {
      if (strchr(numberBuffer, '.') == NULL)
      {
          Value *val = talloc(sizeof(Value));
          val->type = INT_TYPE;
          val->i = atoi(numberBuffer);
          list = cons(val, list);
      }
      else
      {
          Value *val = talloc(sizeof(Value));
          val->type = DOUBLE_TYPE;
          val->d = strtof(numberBuffer, NULL);
          list = cons(val, list);
      }
      numberBuffer[0] = '\0';
      numberUsedIndex = 0;
      numberFlag = false;
    }

    else if (boolFlag)
    {
      Value *val = talloc(sizeof(Value));
      val->type = BOOL_TYPE;
      val->s = talloc(sizeof(char) * (strlen(boolBuffer) + 1));
      strcpy(val->s, boolBuffer);
      list = cons(val, list);

      boolBuffer[0] = '\0';
      boolFlag = false;
    }
    
    else if (symbolFlag)
    {
      Value *val = talloc(sizeof(Value));
      val->type = SYMBOL_TYPE;
      val->s = talloc(sizeof(char) * (strlen(symbolBuffer) + 1));
      strcpy(val->s, symbolBuffer);
      list = cons(val, list);
      symbolBuffer[0] = '\0';
      symbolFlag = false;
      symbolUsedIndex = 0;
    }

    Value *revList = reverse(list);
    return revList;
}

// Displays the contents of the linked list as tokens, with type information
void displayTokens(Value *list)
{
    if (list == NULL)
    {
        printf("List is empty");
        return;
    }

    Value *temp = list;
    while (!isNull(temp))
    {
        switch (car(temp)->type)
        {
        case INT_TYPE:
            printf("%i:integer \n", car(temp)->i);
            break;

        case DOUBLE_TYPE:
            printf("%lf:double \n", car(temp)->d);
            break;

        case STR_TYPE:
            printf("\"%s\":string \n", car(temp)->s);
            break;

        case CONS_TYPE:
            printf("you're trying to print a cons cell.\n");
            break;

        case NULL_TYPE:
            printf("You're trying to print a null cell\n");
            break;

        case PTR_TYPE:
            printf("you're trying to print a pointer cell.\n");
            break;

        case OPEN_TYPE:
            printf("%s:open \n", car(temp)->s);
            break;

        case CLOSE_TYPE:
            printf("%s:close \n", car(temp)->s);
            break;

        case BOOL_TYPE:
            printf("%s:boolean \n", car(temp)->s);
            break;

        case SYMBOL_TYPE:
            printf("%s:symbol \n", car(temp)->s);
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