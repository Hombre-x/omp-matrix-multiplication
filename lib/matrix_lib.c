#include <stdio.h>
#include <time.h>
#include <omp.h>

#define index_m(matrix, row, col) (matrix.data[(row) * matrix.size + (col)])

typedef struct {
  int size;
  double * data; 
} matrix_t;


matrix_t create_matrix(int n)
{
  double * data = (double *) malloc(n * n * sizeof(double));
  if (data == NULL)
  {
    perror("Error allocating memory for matrix double pointer");
    exit(EXIT_FAILURE);
  }

  matrix_t matrix = {n, data};

  return matrix;
}


void set_m(matrix_t matrix, int row, int col, double value)
{
  matrix.data[row * matrix.size + col] = value;
}


matrix_t transpose(matrix_t matrix)
{
  int n = matrix.size;
  matrix_t transposed_matrix = create_matrix(n);

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      index_m(transposed_matrix, i, j) = index_m(matrix, j, i);

  return transposed_matrix;
}

matrix_t randomize_matrix(matrix_t matrix) 
{
  srand(time(NULL));

  int n = matrix.size;

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      index_m(matrix, i, j) = ((double) rand() / RAND_MAX) * 100;

  return matrix;
}

void free_matrix(matrix_t matrix)
{
  free(matrix.data);
}

void print_matrix(matrix_t matrix)
{
  int n = matrix.size;

  for (int i = 0; i < n; i++)
  {
    printf("[");
    for (int j = 0; j < n; j++)
    {
      printf(" %8.5f ", index_m(matrix, i, j));
    }
    printf("]\n");
  }
}

matrix_t multiply_matrix(matrix_t matrix_a, matrix_t matrix_b)
{
  int n = matrix_a.size;
  matrix_t matrix_c = create_matrix(n);

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      for (int k = 0; k < n; k++)
        index_m(matrix_c, i, j) += index_m(matrix_a, i, k) * index_m(matrix_b, k, j);

  return matrix_c;
}

matrix_t par_multiply_matrix(matrix_t matrix_a, matrix_t matrix_b, int num_threads)
{
  int n = matrix_a.size;
  matrix_t matrix_c = create_matrix(n);
  matrix_t matrix_bt = transpose(matrix_b);

  omp_set_num_threads(num_threads);

  #pragma omp parallel for 
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    { 
      double result = 0.0;
      
      for (int k = 0; k < n; k++)
        result += index_m(matrix_a, i, k) * index_m(matrix_bt, j, k);

      index_m(matrix_c, i, j) = result;
    }
  }

  free_matrix(matrix_bt);

  return matrix_c;
}


matrix_t identity_matrix(int n)
{
  double * data = calloc(n * n, sizeof(double));
  if (data == NULL)
  {
    perror("Error allocating memory for matrix double pointer");
    exit(EXIT_FAILURE);
  }

  matrix_t matrix = {n, data};

  for (int i = 0; i < n; i++)
    index_m(matrix, i, i) = 1;

  return matrix;
}
