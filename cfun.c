#include <stddef.h>
double ten(double x)
{
	return x*1*2.2;
}
void test_function(double *in, size_t len, double *out)
{
	size_t i;
	for (i = 0; i < len; i++)
	{
		out[i] = ten(in[i]) * 2.0;
	}
}
