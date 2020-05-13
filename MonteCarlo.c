#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main(int argc, char const *argv[])
{
        int i, count=0;
        double x, y;
        int samples;
        double pi;
        unsigned int seed;
        int threshold_omp = 100;

        samples = atoi (argv[1]);
        int nthreads = atoi (argv[2]);

        omp_set_num_threads(nthreads);
        double start = omp_get_wtime();

        #pragma omp parallel if (samples > threshold_omp) default(none) private(x,y,seed) shared(samples,count)
        {
                seed = 25234 + 17 * omp_get_thread_num();

                #pragma omp for reduction(+:count)
                for ( i = 0; i < samples; i++)
                {

                        x = (double) rand_r(&seed) / RAND_MAX;
                        y = (double) rand_r(&seed) / RAND_MAX;
                        if (x*x + y*y <= 1)
                        {
                                count++;
                        }
                }

        }
        double end = omp_get_wtime();
        printf("elapsed time: %.16g\n", end - start );

        pi = 4.0 * (double)count / (double)samples;
        printf("Count = %d, Samples = %d, Estimate of pi = %7.5f\n", count, samples, pi );
        return 0;
}