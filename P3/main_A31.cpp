#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "main.h"

// ***********************************************************************
int main(int argc, char* argv[])
{
	constexpr long int N{ 100'000'000 };
	double starttime, elapsedtime_init, elapsedtime_calc;
	float* a, * b, * c, * d;
	// -----------------------------------------------------------------------
	// allocate memory for the matrices
	a = (float*)malloc(sizeof(float) * N);
	if (!a) { printf("malloc a failed!\n"); exit(999); }
	b = (float*)malloc(sizeof(float) * N);
	if (!b) { printf("malloc b failed!\n"); exit(999); }
	c = (float*)malloc(sizeof(float) * N);
	if (!c) { printf("malloc c failed!\n"); exit(999); }
	d = (float*)malloc(sizeof(float) * N);
	if (!d) { printf("malloc d failed!\n"); exit(999); }
	// 5 runs for runtme measurements
	for (int runs = 0; runs < 5; runs++)
	{
		// initialize matrix a,b,c
		starttime = wtime();
		for (int i = 0; i < N; i++) {
			float x = i + 1.f;
			// Original
			//a[i] = 1. / (i + 1);
			//b[i] = 2. / (i + 1);
			//c[i] = 3. / (i + 1);
			a[i] = 1.f / x;
			b[i] = 2.f / x;
			c[i] = 3.f / x;
		}
		elapsedtime_init = wtime() - starttime;

		// get starting time
		starttime = wtime();

		float sinVal = sin(15.f);
		for (int i = 0; i < N; i++) {
			// Original
			//d[i] = ((a[i] / b[i]) / c[i]) * sin(15.0);
			d[i] = ((a[i] / b[i]) / c[i]) * sinVal;
		}

		elapsedtime_calc = wtime() - starttime;

		float sum = 0;
		for (int i = 0; i < N; i++) {
			sum += d[i];
		}

		sum /= 281474976710656.f;
		// report elapsed time
		printf("Time Elapsed (Init) % 03f Secs, \t time Elapsed (Calc) % 03f Secs, CheckSum % 03f\n", elapsedtime_init, elapsedtime_calc, sum);
	}
	free(a);
	free(b);
	free(c);
	free(d);

	system("PAUSE");
	return 0;

}