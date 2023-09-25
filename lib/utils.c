#include <time.h>
#include <omp.h>

double measure_time(void * procedure(void *), void * args, void * result)
{
  double start_time = 0.0, end_time = 0.0;
  double elapsed_time = 0.0;

  start_time = omp_get_wtime();

  result = procedure(args);

  end_time = omp_get_wtime();
  
  elapsed_time = ((double) (end_time - start_time));


  return elapsed_time;
}
