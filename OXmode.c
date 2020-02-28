#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <stdio.h>

double max(double *arr, int len)
{
	int i;
	double temp = -INFINITY;
	for (i = 0; i < len; i++)
	{
		if (arr[i] > temp)
		{
			temp = arr[i];
		}
	}
	return temp;
}

void arange_1_step(double start, double stop, int len, double *out)
{
	int i;
	for (i = 0; i < len; i++)
	{
		out[i] = start + i;
	}
}



int OXmode(double *fps, const int fps_len, const double fcO, const double fcX, const double dz, double *fvsO, const int fvsO_len, double* fvsX, const int fvsX_len, double *vhsO, double *vhsX, double fe)
{
	// O-trace forward model
	int i;
	for (i = 0; i < fvsO_len; i++)
	{
		double fv = fvsO[i];
		if (fv < fcO + 0.025)
		{
			double vh = 0;
			double n2b = 1;
			int j;
			for (j = 0; j < fps_len; j++)
			{
				double fp = fps[j];
				double X = fp*fp/(fv*fv);
				double n2t = 1 - X;
				if (n2t > 0)
				{
					vh = vh + dz/((sqrt(n2b)+sqrt(n2t))/2.0);
					n2b = n2t;
				}
				else
				{
					vh = vh + 0.2 * sqrt(n2b)/(n2b - n2t);
					break;
				}
			}
			vhsO[i] = vh;
		}
		else
		{
			break;
		}
	}
	// X-trace forward model
	for (i = 0; i < fvsX_len; i++)
	{
		double fv = fvsX[i];
		if (fv < fcX + 0.025)
		{
			double vh = 0;
			double n2b = 1;
			double fxb = 1;
			int j;
			for (j = 0; j < fps_len; j++)
			{
				double fp = fps[j];
				double X = fp*fp/(fv*fv);
				double Y = fe/fv;
				double n2t = 1 - X*(1 - X)/(1 - X - Y*Y);
				double fxt = n2t + X*((1 - X)/(1 - X-Y*Y))*((1 - X)/(1 - X-Y*Y)) + (X*Y/(1 - X - Y*Y))*(X*Y/(1 - X - Y*Y));
				if (n2t > 0)
				{
					vh = vh + dz/(  (sqrt(n2b)/fxb + sqrt(n2t)/fxt)/2.0  );
					n2b = n2t;
					fxb = fxt;
				}
				else
				{
					vh = vh + 2.0*dz*sqrt(n2b)/(n2b - n2t);
					break;
				}
			}
			vhsX[i] = vh;
		}
		else
		{
			break;
		}
	}
	return 1;
}

