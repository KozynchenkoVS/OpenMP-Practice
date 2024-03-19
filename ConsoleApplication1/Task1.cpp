#include <stdio.h>
#include <omp.h>
#include <set>
using namespace std;
double f(double y) { return(4.0 / (1.0 + y * y)); }
set<int> insert(int a, set<int> b) {
	if (b.size() <= 1) {
		b.insert(a);
	}
	else{
		b.erase(*b.rbegin());
		b.insert(a);
	}
	return b;
}
int main()
{
	double start_time, end_time, tick;
	const int nThreads = 8;
	float calculatedThreads;
	start_time = omp_get_wtime();
	float result[nThreads][3];
	set<int> iterations;
	double calculatedSum;
	double w, x, sum, pi;
	int i;
	int n = 100000000;
	w = 1.0 / n;
	sum = 0.0;
#pragma omp parallel private(iterations, calculatedSum) shared(w) num_threads(nThreads)
{
#pragma omp master
{
		calculatedThreads = (float)omp_get_num_threads();
}
	
#pragma omp for private(x) reduction(+:sum)
	for (i = 0; i < n; i++)
	{
		x = w * (i - 0.5);
		sum = sum + f(x);
		calculatedSum = sum;
		iterations = insert(i, iterations);
	}
	result[omp_get_thread_num()][0] = (float)*iterations.begin();
	result[omp_get_thread_num()][1] = (float)*iterations.rbegin();
	result[omp_get_thread_num()][2] = calculatedSum;
	iterations.clear();
}
	printf("Количество потоков: %lf \n", (float)calculatedThreads);
	for (i = 0; i < calculatedThreads; i++) {
		printf("Выполняющий узел: %lf \n", (float)i);
		printf("Начальный счётчик цикла: %lf \n", (float)result[i][0]);
		printf("Конечный счётчик цикла: %lf \n", (float)result[i][1]);
		printf("Последнее значение суммы: %lf \n", (float)result[i][2]);
	}
	pi = w * sum;
	end_time = omp_get_wtime();
	tick = omp_get_wtick();
	printf("pi = %f\n", pi);
	printf("Затраченное время %lf \n", end_time - start_time);
	printf("Точность таймера %lf \n", tick);
	return 0;
}