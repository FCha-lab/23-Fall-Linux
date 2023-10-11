#include<stdio.h>
#include<stdlib.h>
#define _USE_MATH_DEFINES
#include<math.h>
#include<sys/types.h>
#include<sys/wait.h>
#define N 4
#define MAXLINE 100

int main(){
	double x[N] ={0, M_PI/6.,M_PI/3.,0.134};
	double res[N];

	sinx_taylor(N, 3, x, res);

	for(int i=0; i<N;i++){
		printf("sin(%.2f) by Taylor series = %f\n", x[i], res[i]);
		printf("sin(%.2f) = %f\n", x[i], sin(x[i]));
	}
	return 0;
}


void sinx_taylor(int num_elements, int terms, double *x, double *result){
	int n, length, fd[num_elements][2], status;
	pid_t pid[num_elements];
	char throw[MAXLINE], catch[MAXLINE];

	for(int i=0;i<num_elements;i++){
		
		pipe(fd[i]);
		if((pid[i]=fork()) == 0){
			double value =x[i];
                	double number = x[i] * x[i] *x[i];
                	double denom = 6.;
                	int sign = -1;
			close(fd[i][0]);
			for(int j=0;j<=terms; j++){
                        	value += (double)sign * number / denom;
                        	number *= x[i] * x[i];
                        	denom *= (2.*(double)j+2.)*(2.*(double)j+3.);
                        	sign *= -1;
                	}
                	sprintf(throw, "%f", value);
                	length = strlen(throw)+1;
                	write(fd[i][1], throw, length);	
			exit(0);
		}

	}
	for(int i=0;i<num_elements;i++){
		double value;
		while(wait(&status) != pid[i])
			continue;
		close(fd[i][1]);
                n = read(fd[i][0], catch, MAXLINE);
                value = atof(catch);
                result[i] = value;
	}
}

