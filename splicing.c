//splicing.c

typedef union flint{ unsigned u; float f; } flint;

float assemble(sploat strukt)
{

	flint num;
	
	num.u = strukt.s << 31;
	num.u = num.u|(strukt.e << 23);
	num.u = num.u|(strukt.m & 0x7fffff);

	return num.f;

}

unsigned sign(float x)
{

	flint a;
	a.f = x;
	return a.u >> 31;

}

unsigned exponent(float x)
{

	flint a;
	a.f = x;
	return (a.u << 1) >> 24;

}

unsigned man(float x)
{

	flint a;
	a.f = x;
	return ((a.u << 9) >> 9) | 0x800000;

}


/*
 * num.u = 1 << 31 == 0x80000000
 * 0x80 << 23 == 0x40000000
 * num.u = num.u | 0x40000000 == 0xc0000000
 * 0xc00000 & 0x7fffff == 0x400000
 * num.u = num.u | 0x400000 == 0xc0400000
 * num.f == -3
 */
