#include <stdio.h>
#include <stdlib.h>
#include "../lib/utils.c"
#include "../lib/matrix_lib.c"


int main(int argc, char const *argv[])
{
  const int size = 2;
  matrix_t matrix_id     = identity_matrix(size);
  matrix_t matrix_a      = create_matrix(size);
  matrix_t matrix_result = create_matrix(size);
  matrix_a = randomize_matrix(matrix_a);


  printf("First property: A * I = A\n");

  print_matrix(matrix_a);
  printf("\n");

  print_matrix(matrix_id);
  printf("\n");

  matrix_result = par_multiply_matrix(matrix_a, matrix_id, 4);

  print_matrix(matrix_result);



  return EXIT_SUCCESS;
}