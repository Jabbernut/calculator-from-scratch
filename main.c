#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "arithmetic.h"
#include "mymacros.h"

short isnum(const char* s)
{

	unsigned i = 0;
	short hasdot = FALSE;

	if(s[i] == '-') {
		i++;
	}

	while(s[i] != 0)
	{
		
		if(s[i] == '.' && hasdot == FALSE)
		{
			hasdot = TRUE;
		}
		else if(isdigit(s[i]) == FALSE)
		{
			return FALSE;
		}

		i++;

	}

	return TRUE;

}

short alfastr(const char* s)
{

	short i = 0;

	while(s[i] != 0)
	{
		if(isalpha(s[i]) == FALSE)
			return FALSE;
	}

	return TRUE;

}

short opIndex(char* s)
{
	const char* operations[] = {"+", "-", "*", "/", "^", "sin", "cos", "tan", "asin", "acos", "atan", "logb", "exp", "ln", "lg", "sqrt"};
	short i = 0;

	for(i; i < 16; i++)
	{
		if(strcmp(s, operations[i]) == 0)
			return i;
	}
	return -1;

}
//these macros define the distance between '+' and the actual operation pointed to
#define ADD 0
#define SUB 1
#define MULT 2
#define DIV 3
#define PWR 4
#define SIN 5
#define COS 6
#define TAN 7
#define ASIN 8
#define ACOS 9
#define ATAN 10
#define LOG_BASE 11
#define EXP 12
#define LN 13
#define LG 14
#define SQRT 15

int main(void)
{

	printf("-------------------------------------------------\n\n");
	float stack[20]; char input[20]; char len = 0; char operation;
	
	float *a, *b;

	while(1 == 1)
	{
	
		scanf("%s", input);

		if(strcmp(input, "exit") == 0)
		{
			printf("\e[%uA\e[J", len+3);
			return 0;
		}
		else if(isnum(input) == TRUE && len < 20)
		{
			stack[len] = atof(input);
			printf("\e[2A\e[J%f\n\n", stack[len]);
			len++;
		}
		else if(len > 0)
		{
			b = stack + len - 1;
			operation = opIndex(input);
			
			if(len > 1 && ADD <= operation && operation <= LOG_BASE)
			{
				a = stack + len - 2;
				switch(operation)
				{
				
					case ADD:	*a = add(*a, *b); break;
				
					case SUB:	*a = sub(*a, *b); break;

					case MULT:	*a = mult(*a, *b); break;
	
					case DIV:	*a = mydiv(*a, *b); break;

					case PWR:	*a = ipow(*a, (int) *b); break;
					
					case SIN:	break;
					
					case COS:	break;

					case TAN:	break;

					case ASIN:	break;

					case ACOS:	break;

					case ATAN:	break;

					case LOG_BASE:	break;
	
					default:	break;
					
				}

				printf("\e[4A\e[J%f\n\n", *a);
				len--;
			}
			else
			{
				switch(operation)
				{
					case EXP: break;

					case LN:break;

					case LG:break;

					case SQRT: *b = mysqrt(*b); break;

					default: break;
				}

				printf("\e[3A\e[J%f\n\n", *b);
			}
		}
		else
		{
			printf("\e[A\e[J");
		}
	
	}
	

}
