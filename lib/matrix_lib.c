#include <stdio.h>
#include <time.h>
#include <omp.h>

typedef struct {
  int size;
  double * data; 
} matrix_t;


double ** create_matrix(int n)
{
  double ** matrix = (double **) malloc(n * sizeof(double *));
  if (matrix == NULL)
  {
    perror("Error allocating memory for matrix double pointer");
    exit(EXIT_FAILURE);
  }
  
  for (int i = 0; i < n; i++)
  {
    matrix[i] = (double *) calloc(n, sizeof(double));
    if (matrix[i] == NULL)
    {
      perror("Error allocating memory for matrix[i] double pointer");
      exit(EXIT_FAILURE);
    }
  } 
  return matrix;
}

double ** transpose(int n, double ** matrix)
{
  double ** transposed_matrix = create_matrix(n);

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      transposed_matrix[i][j] = matrix[j][i];

  return transposed_matrix;
}

double ** randomize_matrix(int n, double ** matrix ) 
{
  srand(time(NULL));

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      matrix[i][j] = ((double) rand() / RAND_MAX) * 100;

  return matrix;
}

void free_matrix(int n, double ** matrix)
{
  for (int i = 0; i < n; i++)
    free(matrix[i]);
  free(matrix);
}

void print_matrix(int n, double ** matrix)
{
  for (int i = 0; i < n; i++)
  {
    printf("[");
    for (int j = 0; j < n; j++)
    {
      printf(" %8.5f ", matrix[i][j]);
    }
    printf("]\n");
  }
}

double ** multiply_matrix(int n, double ** matrix_a, double ** matrix_b)
{
  double ** matrix_c = create_matrix(n);

  for (int i = 0; i < n; i++)
    for (int k = 0; k < n; k++)
      for (int j = 0; j < n; j++)
        matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];

  return matrix_c;
}

double ** multiply_matrix_transposed(int n, double ** matrix_a, double ** matrix_b)
{
  double ** matrix_c = create_matrix(n);
  double ** matrix_bt = transpose(n, matrix_b);

  for (int i = 0; i < n; i++)
    for (int k = 0; k < n; k++)
      for (int j = 0; j < n; j++)
        matrix_c[i][j] += matrix_a[i][k] * matrix_bt[j][k];

  free_matrix(n, matrix_bt);
  return matrix_c;
}

double ** par_multiply_matrix(int n, double ** matrix_a, double ** matrix_b, int num_threads)
{
  double ** matrix_result = create_matrix(n);
  double ** matrix_bt = transpose(n, matrix_b);

  omp_set_num_threads(num_threads);

  #pragma omp parallel for 
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    { 
      double result = 0.0;
      
      for (int k = 0; k < n; k++)
        matrix_result[i][j] += matrix_a[i][k] * matrix_bt[j][k];

      matrix_result[i][j] = result;
    }
  }

  free_matrix(n, matrix_bt);
  return matrix_result;
}


double ** identity_matrix(int n)
{
  double ** matrix = calloc(n, sizeof(double *));
  for (int i = 0; i < n; i++)
    matrix[i] = calloc(n, sizeof(double));

  for (int i = 0; i < n; i++)
    matrix[i][i] = 1;

  return matrix;
}
