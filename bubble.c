#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

void bubble_sort(int *, unsigned long, unsigned long chunkSize, int numThreads);
void imprimir_vetor(int *, unsigned long);

int main(int argc, char *argv[])
{
	struct timeval timevalA;
        struct timeval timevalB;

	int *vetor = NULL, numThreads;
	unsigned long tam, i = 0, chunkSize;

	if (argc != 3) {
		printf("%s elementos, necessita de 2\n", (argv[0]-1));
		exit(EXIT_FAILURE);
	}

	tam = atoi(argv[1]);
	numThreads = atoi(argv[2]);

	if(!(numThreads > 0)){
		printf("Número de threads deve ser > 0\n");
		exit(EXIT_FAILURE);
	}

	if (!(vetor = (int *) malloc(sizeof(int) * tam))) {
		printf("Erro ao alocar memória\n");
		exit(EXIT_FAILURE);
	}

	chunkSize = tam / numThreads;

	srand(time(NULL));

	for (i = 0; i < tam; i++) {
		*(vetor + i) = random() % 10000;
	}

	gettimeofday(&timevalA, NULL);
	bubble_sort(vetor, tam, chunkSize, numThreads);
	gettimeofday(&timevalB, NULL);

	printf("%lf\n", timevalB.tv_sec - timevalA.tv_sec + (timevalB.tv_usec - timevalA.tv_usec) / (double) 1000000);

	imprimir_vetor(vetor, tam);

	free(vetor);
	return EXIT_SUCCESS;
}

void bubble_sort(int *vetor, unsigned long tam, unsigned long chunkSize, int numThreads)
{
	unsigned long i, j, inicio;
	int aux;

	for (i = 0; i < tam; i++) {
		inicio = i % 2;
		#pragma omp parallel for schedule(static) num_threads(numThreads) shared(vetor, i) private(aux, j)
		for (j = inicio; j < tam - 1; j+=2) {
			if (vetor[j] > vetor[j + 1]) {
				aux = vetor[j];
				vetor[j] = vetor[j + 1];
				vetor[j + 1] = aux;
			}
		/*	if(j < tam -2 && vetor[j] > vetor[j + 2]) {
				aux = vetor[j];
				vetor[j] = vetor[j + 2];
				vetor[j + 2] = aux;
			}
			*/
		}
	}
}

void imprimir_vetor(int *vetor, unsigned long tam)
{
	unsigned long i;
	for (i = 0; i < tam; i++) {
		printf("%d\t", vetor[i]);
	}
	printf("\n");
}