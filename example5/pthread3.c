#include<pthread.h>
#include<math.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#define N 4
#define MAXLINE 100

double x[N] ={0, M_PI/6.,M_PI/3.,0.134};
double res[N];
int terms = 0;
pthread_mutex_t pmt;

int main(){

        sinx_taylor(N, 3);

        for(int i=0; i<N;i++){
                printf("sin(%.2f) by Taylor series = %f\n", x[i], res[i]);
                printf("sin(%.2f) = %f\n", x[i], sin(x[i]));
        }
        return 0;
}

void * Taylor_sinx(void *argument){
	
	
	int tid;
        tid = *((int*)argument);
   
	double value =x[tid];
        double number = x[tid] * x[tid] *x[tid];
        double denom = 6.;
        int sign = -1;


        for(int j=0;j<=terms; j++){
		value += (double)sign * number / denom;
                number *= x[tid] * x[tid];
                denom *= (2.*(double)j+2.)*(2.*(double)j+3.);
                sign *= -1;
	}
	pthread_mutex_lock(&pmt);
	res[tid] = value;
	pthread_mutex_unlock(&pmt);

}

void sinx_taylor(int num_elements, int term){
        int n, length, status;
	pthread_t threads[num_elements];
	int args[num_elements];
	terms = term;
	pthread_mutex_init(&pmt, NULL);

        for(int i=0;i<num_elements;i++){

                args[i] = i;
                pthread_create(&threads[i], NULL, Taylor_sinx, (void*) &args[i]);
                
        }

	for(int i=0;i<num_elements;i++)
		pthread_join(threads[i], NULL);
        
}
