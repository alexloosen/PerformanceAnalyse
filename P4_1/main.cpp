#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "main.h"

// Neue Wurzelberechnung mit Vektorlänge 4 Bytes für Floats
#pragma omp declare simd simdlen(4)
float* testSqrt(float *a, float* b, int N)
{
	float sum = 0;
	for (int i = 0; i < N; i++) {
		b[i] = 1.f / sqrt(a[i]);
	}
	return b;
}
// ***********************************************************************
int main(int argc, char* argv[])
{
	constexpr int N{ 250'000'000 };
	double starttime, elapsedtime;
	float* a, * b;
	constexpr float AngleConst = 360.f / 3.14159f;
	// -----------------------------------------------------------------------
	// allocate memory for the matrices
	a = (float*)malloc(sizeof(float) * N);
	b = (float*)malloc(sizeof(float) * N);
	if (!a) { printf("malloc a failed!\n"); exit(999); }
	
	// initialize array a
	for (int i = 0; i < N; i++) {
		a[i] = (N-i) * AngleConst;
	}
	
	// 5 runs for runtme measurements
	for (int runs = 0; runs < 5; runs++)
	{
		// get starting time
		starttime = wtime();
		
		float sum = 0;
		// Alte Berechnung
		//for (int i = 0; i < N; i++) {
		//	sum += 1./sqrt(a[i]);
		//	sum += b[i];
		//}

		// Wurzelberechnung ausgelagert (Strip-Mining) und mitgeteilt, dass es dabei kein Pointer-Aliasing gibt
		// Laut Bericht wird hier vektorisiert, aber nicht parallelisiert, wegen unzureichender Rechenarbeit
		#pragma ivdep
		b = testSqrt(a, b, N);
		// Hier muss dann nurnoch aufsummiert werden
		// Laut Bericht kann hier weder vektorisiert noch parallelisiert werden
		// Gründe: 1. unzureichende Rechenarbeit, 2. Floating Point Modell precise statt fast gewählt
		for (int i = 0; i < N; i++) {
			sum += b[i];
		}

		sum /= 1214.145874;
		elapsedtime = wtime() - starttime;
		// report elapsed time
		printf("Time Elapsed  % 03f Secs, Sum: % 03f \n", elapsedtime, sum);
	}
	free(a);
	system("PAUSE");
	return 0;
}