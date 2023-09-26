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

// double index_m(matrix_t matrix, int row, int col)
// {
//   return matrix.data[row * matrix.size + col];
// }

void set_m(matrix_t matrix, int row, int col, double value)
{
  matrix.data[row * matrix.size + col] = value;
}


matrix_t transpose(int n, matrix_t matrix)
{
  matrix_t transposed_matrix = create_matrix(n);

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      index_m(transposed_matrix, i, j) = index_m(matrix, j, i);

  return transposed_matrix;
}

matrix_t randomize_matrix(int n, matrix_t matrix) 
{
  srand(time(NULL));

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      index_m(matrix, i, j) = ((double) rand() / RAND_MAX) * 100;

  return matrix;
}

void free_matrix(int n, matrix_t matrix)
{
  free(matrix.data);
}

void print_matrix(int n, matrix_t matrix)
{
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

matrix_t multiply_matrix(int n, matrix_t matrix_a, matrix_t matrix_b)
{
  matrix_t matrix_c = create_matrix(n);

  for (int i = 0; i < n; i++)
    for (int k = 0; k < n; k++)
      for (int j = 0; j < n; j++)
        index_m(matrix_c, i, j) += index_m(matrix_a, i, k) * index_m(matrix_b, k, j);

  return matrix_c;
}

// double ** multiply_matrix_transposed(int n, double ** matrix_a, double ** matrix_b)
// {
//   double ** matrix_c = create_matrix(n);
//   double ** matrix_bt = transpose(n, matrix_b);

//   for (int i = 0; i < n; i++)
//     for (int k = 0; k < n; k++)
//       for (int j = 0; j < n; j++)
//         matrix_c[i][j] += matrix_a[i][k] * matrix_bt[j][k];

//   free_matrix(n, matrix_bt);
//   return matrix_c;
// }

matrix_t par_multiply_matrix(int n, matrix_t matrix_a, matrix_t matrix_b, int num_threads)
{
  matrix_t matrix_result = create_matrix(n);
  matrix_t matrix_bt = transpose(n, matrix_b);

  omp_set_num_threads(num_threads);

  #pragma omp parallel for 
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    { 
      double result = 0.0;
      
      for (int k = 0; k < n; k++)
        index_m(matrix_result, i, j) += index_m(matrix_a, i, k) * index_m(matrix_bt, j, k);

      index_m(matrix_result, i, j) = result;
    }
  }

  free_matrix(n, matrix_bt);
  return matrix_result;
}


// double ** identity_matrix(int n)
// {
//   double ** matrix = calloc(n, sizeof(double *));
//   for (int i = 0; i < n; i++)
//     matrix[i] = calloc(n, sizeof(double));

//   for (int i = 0; i < n; i++)
//     matrix[i][i] = 1;

//   return matrix;
// }
