#include <iostream>
#include <omp.h>
#include <fstream>
#include <chrono>

void readArray(char *fileName, double **a, int *n);
double sumArray(double *a, int numValues);

int main(int argc, char *argv[]) {

  int howMany;
  double sum;
  double *a;

  readArray(argv[1], &a, &howMany);

  double start_parallel = omp_get_wtime();
  sum = sumArray(a, howMany);
  double end_parallel = omp_get_wtime();

  std::cout << "total time taken with parallel: " << (end_parallel - start_parallel) << std::endl;
  std::cout << "total sum with parallel: " << sum;

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

double sumArray(double *a, int numValues)
{
    int i;
    double result = 0.0;

    #pragma omp parallel for reduction (+:result)
      for (i = 0; i < numValues; i++)
      {
        result += a[i];
      }

      return result;
}
