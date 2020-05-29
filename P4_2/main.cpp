#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "main.h"
#include "rngutil.h"
#include <malloc.h>
#include <memory>
#include <memory.h>

struct Charge {
	float *x, *y, *z, *q; // Koordinaten und Stärke der Ladung
	// Pointer bei Anlage des Structs initialisieren (2048 = n, 2056 = lda (durch 8 teilbar))
	Charge(int n, int lda)
	{
		x = (float*)_mm_malloc(sizeof(float) * n * lda, 32);
		y = (float*)_mm_malloc(sizeof(float) * n * lda, 32);
		z = (float*)_mm_malloc(sizeof(float) * n * lda, 32);
		q = (float*)_mm_malloc(sizeof(float) * n * lda, 32);
	}
};

void CalculateElectricPotential(
	const int m,       // Anzahl der Ladungen
	const Charge chg, // Ladungsverteilung
	const float Rx, const float Ry, const float Rz, // Bezugspunkt
	float& phi  // Ausgabe: Elektrisches Potential
) {
	phi = 0.0f;
	int tile = 16;
	for (int i = 0; i < m; i++) {
		// Zugriff von chg[i].x auf chg.x[i] umgestellt, da man nun das offset eines jeden Pointers betrachtet und
		// nicht länger das offset des Structs
		const float dx = chg.x[i] - Rx;
		const float dy = chg.y[i] - Ry;
		const float dz = chg.z[i] - Rz;
		phi -= chg.q[i] / sqrtf(dx * dx + dy * dy + dz * dz);
	}

}

// ***********************************************************************
int main(int argc, char* argv[])
{
	const size_t n = 1 << 11;
	const size_t m = 1 << 11;

	double starttime, elapsedtime;
	// Puffer berechnen, wenn n nicht durch 8 teilbar
	int lda = m + (4 - m % 4);
	alignas(16) float* potential = (float*)_mm_malloc(sizeof(float) * n * lda, 16);
	Charge chg(n,lda);
	//float* potential = (float*)malloc(sizeof(float) * n * n);

	// Initializing array of charges
	RNGutil rng;
	printf("Initialization...");
	for (size_t i = 0; i < n; i++) {
		chg.x[i] = rng.Next();
		chg.y[i] = rng.Next();
		chg.z[i] = rng.Next();
		chg.q[i] = rng.Next();
	}
	printf(" complete.\n");

	// 5 runs for runtme measurements
	for (int runs = 0; runs < 5; runs++)
	{
		for (int i = 0; i < n * n; i++)
			potential[i] = 0.0f;

		starttime = wtime();
		for (int j = 0; j < n * n; j++) {
			const float Rx = (float)(j % n);
			const float Ry = (float)(j / n);
			const float Rz = 0.0f;
			CalculateElectricPotential(m, chg, Rx, Ry, Rz, potential[j]);
		}
		elapsedtime = wtime() - starttime;
		float sum = 0;
		for (int i = 0; i < n * n; i++)
			sum += potential[i];
		sum /= -3721903.f;
		// report elapsed time
		printf("Time Elapsed  % 03f Secs, Sum: % 03f \n", elapsedtime, sum);
	}

	//free(a);
	system("PAUSE");
	return 0;
}