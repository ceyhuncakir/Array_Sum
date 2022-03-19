#include <iostream>
#include <omp.h>
#include <fstream>
#include <chrono>

void readArray(char *fileName, double **a, int *n);
double sumArray(double *a, int numValues, int amount_of_threads);

int main(int argc, char *argv[]) {

  int howMany;
  double sum;
  double *a;
  int amount_of_threads = 8;

  readArray(argv[1], &a, &howMany);

  double start_parallel = omp_get_wtime();
  sum = sumArray(a, howMany, amount_of_threads);
  double end_parallel = omp_get_wtime();

  std::cout << "total time taken with parallel: " << (end_parallel - start_parallel) << std::endl;
  std::cout << "total sum with parallel: " << sum << std::endl;
  std::cout << "used threads:" << amount_of_threads << std::endl;

  free(a);

  return 0;
}

void readArray(char *fileName, double **a, int *n)
{
    int count, howMany;
    double *tempA;
    std::ifstream fin(fileName);
    if (!fin.is_open())
    {
        std::cout << std::endl << "*** Unable to open input file ''" << fileName << "'" << std::endl << std::endl;
        exit(1);
    }

    fin >> howMany;
    tempA = (double *)calloc(howMany, sizeof(double));
    if (tempA == NULL)
    {
        std::cout << std::endl << "*** Unable to allocate " << howMany << "-length array" << std::endl << std::endl;
        exit(1);
    }

    for (count = 0; count < howMany; count++)
        fin >> tempA[count];

    fin.close();

    *n = howMany;
    *a = tempA;
}

double sumArray(double *a, int numValues, int amount_of_threads)
{
    int i;
    double result = 0.0;

    #pragma omp parallel for reduction (+:result) num_threads(amount_of_threads)
      for (i = 0; i < numValues; i++)
      {
        // alleen om te debuggen of er de juiste hoeveel aantal threads geactiveerd worden
        //int tid = omp_get_thread_num();
        //printf("Hello world from omp thread %d\n", tid);
        result += a[i];
      }

      return result;
}
