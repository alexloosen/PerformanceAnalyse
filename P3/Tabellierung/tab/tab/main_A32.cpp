#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "main.h"

int factorial_opt(int n, int factorials[]) { // n! 
// ToDo: Implementieren Sie die Funktion, 
// so dass der gesuchte Fakultätswert aus einem lokalem array geladen wird.
// Für Fakultätswerte außerhalb des Wertebereichs soll 0 zurückgegeben werden.
	if (n < 12) {
		return factorials[n];
	}
	return 0;
}

int factorial(int n) {
	int i, f = 1;
	for (i = 2; i <= n; i++)
	{
		f *= i;
	}
	return f;
}

void init_factorials(int factorials[])
{
	for (int i = 0; i < 12; i++)
	{
		factorials[i] = factorial(i);
	}
}

// ***********************************************************************
int main(int argc, char* argv[])
{
	int factorials[12];

	constexpr int N{ 100'000 };
	double starttime, elapsedtime;
	int* a;
	// -----------------------------------------------------------------------
	// allocate memory for the matrices
	a = (int*)malloc(sizeof(int) * N);
	if (!a) { printf("malloc a failed!\n"); exit(999); }

	starttime = wtime();
	init_factorials(factorials);
	elapsedtime = wtime() - starttime;
	// report elapsed time
	printf("Time Elapsed  % 03f Secs for initialization, \n", elapsedtime);

	// 5 runs for runtme measurements
	for (int runs = 0; runs < 5; runs++)
	{
		// initialize array a
		for (int i = 0; i < N; i++) {
			a[i] = i % 10;
		}

		// get starting time
		starttime = wtime();

		// Kommentieren Sie jeweils den Teil aus, den Sie nicht messen möchten

		// Original-Berechnung (Baseline)
		//for (int i = 0; i < N; i++) {
		//	a[i] = factorial(a[i]);
		//	a[i] += factorial(1 + a[i]);
		//}
		// Optimierte Berechnung
		for (int i = 0; i < N; i++)
		{
			a[i] = factorial_opt(a[i], factorials);
			a[i] += factorial_opt(1 + a[i], factorials);
		}

		elapsedtime = wtime() - starttime;
		// report elapsed time
		printf("Time Elapsed  % 03f Secs, \n", elapsedtime);
	}
	// return a value from matrix a
	// just here to make sure matrix calc doesn’t get optimized away.
	system("PAUSE");
	return a[20];

}