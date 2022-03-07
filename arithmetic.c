#include "errorMsg.h"
#include "splicing.h"
#include "mymacros.h"
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#define EPSILON 0.000005

unsigned long umult(unsigned a, unsigned b) {
	if(a < b)
		return umult(b, a);

	unsigned long ans = 0;
	unsigned short digit = 0;

	while(0 < b) {
		if(b&1 == 1)
			ans = uadd(ans, a << digit);
		b = b >> 1;
		digit++;
	}

	return ans;
}

unsigned long uadd(unsigned x, unsigned y) {

	unsigned tmp;
	while(y != 0)
	{

		tmp = (x&y) << 1;
		x = x^y;
		y = tmp;
	
	}

	return x;

}

unsigned usub(unsigned x, unsigned y)
{

	unsigned tmp;
	
	if(x < y)
	{
	
		errMsg("Result of unsigned subtraction is negative!");
		return UINT_MAX;
	
	}

	while(y != 0)
	{
	
		
		tmp = (~x) & y;
		x = x^y;
		y = tmp << 1;
	
	}

	return x;

}

unsigned udiv(unsigned a, unsigned b) {
	if(a < b)
		return 0;
	int offshoot = 0;
	
	while(b&1 == 0 && a&1 == 0) {
		b = b >> 1;
		a = a >> 1;
	}
	if(b == 1)
		return a;

	while(b < a>>1) {
		b = b<<1;
		offshoot++;
	}

	unsigned ans = 0;
	while(0 < a && 0 <= offshoot) {
		a = usub(a, b);
		ans = uadd(ans, 1 << offshoot);
		
		while(a < b && a != 0) {
			a = a << 1;
			offshoot--;
		}
	}

	if(b < a && offshoot == -1)
		return uadd(ans, 1);

	return ans;
}

float add(float x, float y)
{

	sploat ans, a, b;

	a.e = exponent(x);
	b.e = exponent(y);
	
	if(a.e < b.e)
		return add(y, x);

	a.s = sign(x); a.m = man(x);
	b.s = sign(y); b.m = man(y);

	if(a.s == 1 && b.s == 1){
		ans.s = 1;
	} else if(a.s == 1) {
		a.s = 0;
		return sub(y, assemble(a));
	} else if(b.s == 1) {
		b.s = 0;
		return sub(x, assemble(b));
	} else {
		ans.s = 0;
	}

	unsigned expdiff = usub(a.e, b.e);
	
	ans.e = a.e;
	
	if(expdiff != 0)
		ans.m = uadd(a.m, b.m >> expdiff);
	else
		ans.m = uadd(a.m, b.m);


	while(ans.m > 0xffffff)
	{
	
		ans.m = ans.m >> 1;
		ans.e = uadd(ans.e, 1);
	
	}

	if(ans.e > 0xff)
	{
		if(ans.s == 0)
		{
			errMsg("Result is too big!");
			return FLT_MAX;
		}
		else
		{
			errMsg("Result is too small!");
			return FLT_MIN;
		}


	}
	
	return assemble(ans);

}


float sub(float A, float B)
{
	
	sploat ans, a, b;

	a.s = sign(A); a.e = exponent(A); a.m = man(A);
	b.s = sign(B); b.e = exponent(B); b.m = man(B);

	if(a.s == 1 && b.s == 1) {
		a.s = 0;
		b.s = 0;
		return sub(assemble(b), assemble(a));
	} else if(a.s == 1) {
		b.s = 1;
		return add(A, assemble(b));
	} else if(b.s == 1) {
		b.s = 0;
		return add(A, assemble(b));
	}

	if(A < B) {
		ans.s = 1;
		ans.e = b.e;
		a.m = a.m >> usub(b.e, a.e);
		ans.m = usub(b.m, a.m);
	} else {
		ans.s = 0;
		ans.e = a.e;
		b.m = b.m >> usub(a.e, b.e);
		ans.m = usub(a.m, b.m);
	}

	while(ans.m <= 0x7fffff)
	{
		if(ans.e == 0)
			return 0;

		ans.m = ans.m << 1;
		ans.e = usub(ans.e, 1);
	}

	return assemble(ans);

}


float mult(float x, float y){
	sploat a, b, ans;
	a.s = sign(x); a.e = exponent(x); a.m = man(x);
	b.s = sign(y); b.e = exponent(y); b.m = man(y);

	if(a.m < b.m)
		return mult(y, x);
	
	//determine the sign
	if(a.s == 1 && b.s == 1) {
		ans.s = 0;
	} else if(a.s == 1 || b.s == 1) {
		ans.s = 1;
	} else {
		ans.s = 0;
	}

	//determining the exponent
	ans.e = uadd(a.e, b.e);
	if(ans.e < 127)
		return 0;
	ans.e = usub(ans.e, 127);

	//determine the mantissa
	ans.m = umult(a.m, b.m);

	//if mantissa is too big, then decrease it and take it out on the exponent.
	while(0xffffff < ans.m){
		ans.m = ans.m >> 1;
		ans.e++;
	}

	//now the exponent might be too big.
	if(ans.e > 0xff){
		if(ans.s == 1)
			return -FLT_MAX;
		else
			return FLT_MAX;
	}
	return assemble(ans);
}


float mydiv(float x, float y){
	sploat a, b, ans;
	a.s = sign(x); a.e = exponent(x); a.m = man(x);
	b.s = sign(y); b.e = exponent(y); b.m = man(y);

	//determine sign
	if(a.s == 1 && b.s == 1)
		ans.s = 0;
	else if(a.s == 1 || b.s == 1)
		ans.s = 1;
	else
		ans.s = 0;

	//determine exponent
	ans.e = uadd(a.e, 127);
	if(ans.e < b.e)
		return 0;
	ans.e = usub(ans.e, b.e);

	//determine mantissa
	while(a.m&1 == 1 && b.m&1 == 1)
	{
		a.m = a.m >> 1;
		b.m = b.m >> 1;
	}
	
	ans.m = 0;
	unsigned digit = 23;
	while(digit > 0 && a.m > 0){
		while(a.m < b.m){
			a.m = a.m << 1;
			if(digit == 0)
				break;
			digit = usub(digit, 1);
		}
		while(b.m <= a.m){
			a.m = usub(a.m, b.m);
			ans.m = uadd(ans.m, 1 << digit);
		}
	}

	//scaling mantissa correctly
	while(ans.m < 0x7fffff) {
		ans.m = ans.m << 1;
		ans.e = usub(ans.e, 1);
	}
	while(0xffffff < ans.m) {
		ans.m = ans.m >> 1;
		ans.e = uadd(ans.e, 1);
	}

	return assemble(ans);
}


float ipow(float b, int e) {
	float ans = 1;

	for(int i = 0; i < e; i++) {
		ans = mult(ans, b);
	}

	return ans;
}

float mysqrt(float n) {
	printf("sqrt(%f)\n", n);
	if(sign(n) == 1) {
		errMsg("Root is not real.");
		return n;
	}
	printf("root is real\n");
	
	sploat res;
	res.s = 0;
	res.e = exponent(n);
	res.m = MAX_MAN >> 1;
	printf("Mantissa = %u\n", res.m);

	unsigned delta = res.m >> 1;
	unsigned tmp;

	//exponent gets halved.
	if(res.e < MAX_EXP >> 1) {//checks if exponent is negative.
		printf("negative ");
		tmp = usub(MAX_EXP>>1, res.e) >> 1;
		res.e = uadd(res.e, tmp);
	} else {
		printf("positive ");
		tmp = usub(res.e, MAX_EXP>>1) >> 1;
		res.e = uadd(tmp, MAX_EXP>>1);
	}
	printf("exponent = %hu\n", res.e);

	float myans, myansSq;
	int i=0;
	for(delta; 0<delta; delta >> 1) {//this will iterate 22 times
		myans = assemble(res);
		printf("%d: %f² ", i, myans);
		myansSq = mult(myans, myans);

		if(myansSq < n) {
			printf("< ");
			res.m = uadd(res.m, delta);
		} else if(n < myansSq) {
			printf("> ");
			res.m = usub(res.m, delta);
		} else {
			printf("= ");
			break;
		}
		printf("%f\n", n);

		delta = delta >> 1;
		i++;
	}

	return myans;
}	
