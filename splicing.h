typedef struct splicedflt{
	char s;
	unsigned short e;
	unsigned m;
}sploat;

//finished
//assemble a sploat into a float.
float assemble(sploat);

//finished
//returns sign, exponent and mantissa of a float.
unsigned sign(float);
unsigned exponent(float);
unsigned man(float);


#include "splicing.c"
