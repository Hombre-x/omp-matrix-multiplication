#include <stdio.h>
#include <stdlib.h>
#include "../lib/utils.c"
#include "../lib/matrix_lib.c"


int main(int argc, char const *argv[])
{

  // This loop represents the sizes of the matrices
  for (int s = 128; s <= 1024; s *= 2)
  {
    printf("==================================================\n");
    printf("Initiating test with matrix size %d...\n\n", s);

    int n = s;

    matrix_t matrix_a = create_matrix(n);
    matrix_t matrix_b = create_matrix(n);
    matrix_t matrix_c = create_matrix(n);


    const int threads[5] = {1, 2, 4, 8, 12};
    double total_time[5] = {0.0};

    matrix_a = randomize_matrix(n, matrix_a);
    matrix_b = randomize_matrix(n, matrix_b);


    // Begin test using threads
    double start_time = 0.0, end_time = 0.0;
    
    for (int i = 0; i < 5; i++)
    {
      printf("Initiating test with %2d thread(s)...\n", threads[i]);

      start_time = omp_get_wtime();

      par_multiply_matrix(n, matrix_a, matrix_b, threads[i]);

      end_time = omp_get_wtime();

      total_time[i] = ((double) (end_time - start_time));
    }

    printf("\nResults:\n");
    for (int i = 0; i < 5; i++)
      printf("Total time with %2d thread(s) and size %d: %0.7f ms \n", threads[i], s,total_time[i] * 1000.0);

    free_matrix(n, matrix_a);
    free_matrix(n, matrix_b);
    free_matrix(n, matrix_c);

    printf("==================================================\n\n");
  }  

  return EXIT_SUCCESS;
}
