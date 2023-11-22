#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<fcntl.h>

int child_id;

void intHandler(){
        kill(child_id, SIGINT);
}

void intHandler2(){
        printf("아직 구구단이 덜 끝났습니다...\n");
}

void intHandler3(){
        kill(-getppid(), SIGKILL);
}

void print_gugu(){
	for(int i =1;i<=20;i++){
               	printf("%d단 시작\n", i);
              	for(int j=1;j<=20;j++){
                 	printf("%d * %d = %d\n", i, j, i*j);
                }
                printf("\n");
		if(i==9){
			signal(SIGINT, intHandler3);
		}
		sleep(1);
	}
}

int main(){
	int pid;

	if((pid=fork())==0){
		signal(SIGINT, intHandler2);
		print_gugu();
	}else{
		child_id=pid;
		signal(SIGINT, intHandler);
		wait();

	}
}
