#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void intHandler();
void intHandler2();

int main(){
	int answer;
	
	signal(SIGALRM, intHandler);
	signal(SIGINT, intHandler2);
	alarm(10);

	while(1){
		scanf("%d",&answer);
		if(answer==123){
			printf("Correct!\n");
			exit(0);
		}
	}
		
}

void intHandler(){
	printf("Fire!\n");
	exit(0);
}

void intHandler2(){
      	printf("\n10 sec reset...\n");
	alarm(10);
}



