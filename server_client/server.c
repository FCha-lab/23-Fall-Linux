#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <pthread.h>
#define MAXLINE 256

pthread_mutex_t pmt;
pthread_t threads[2];
pthread_t waitthreads[2];
int clientfd1, clientfd2, clientfd3, clientfd4, n;
//통신종료 요청에 대한 결과
int result=-1;

void* TaskCode1(void* argument);
void* TaskCode2(void* argument);

int main(){
	
	pthread_mutex_init(&pmt, NULL);


	if(access("client1fifo1", F_OK) != -1)
        	unlink("client1fifo1");
	if(access("client1fifo2", F_OK) != -1)
        	unlink("client1fifo2");
    	if(access("client2fifo1", F_OK) != -1)
        	unlink("client2fifo1");
    	if(access("client2fifo2", F_OK) != -1)
        	unlink("client2fifo2");
	
	
	if(mkfifo("client1fifo1", 0666) == -1){
		perror("mkfifo");
		exit(1);
	}
	if(mkfifo("client1fifo2", 0666) == -1) {
		perror("mkfifo");
		exit(2);
	}

	if (mkfifo("client2fifo1", 0666) == -1) {
		perror("mkfifo");
		exit(1);
	}
	if (mkfifo("client2fifo2", 0666) == -1) {
		perror("mkfifo");
		exit(2);
	}

	clientfd1 = open("client1fifo1", O_WRONLY); // 1번 클라이언트에게 보낼 정보
	clientfd2 = open("client1fifo2", O_RDONLY); // 1번 클라이언트가 보낸 정보

	clientfd3 = open("client2fifo1", O_WRONLY); // 2번 클라이언트에게 보낼 정보
	clientfd4 = open("client2fifo2", O_RDONLY); // 2번 클라이언트가 보낸 정보

	if(clientfd1 == -1 || clientfd2 == -1 || clientfd1 == -1 || clientfd2 == -1) {
		perror("open");
		exit(3);
	}

	printf("* Server starts *\n");
	int args[2];
	for (int i = 0; i < 2; ++i) {
			args[i] = i;
			pthread_create(&threads[i], NULL, TaskCode1, (void*)&args[i]);
		}

		for (int i = 0; i < 2; ++i) {
			args[i] = i;
			pthread_create(&waitthreads[i], NULL, TaskCode2, (void*)&args[i]);
		}

		for (int i = 0; i < 2; ++i) {
			pthread_join(waitthreads[i], NULL);
		}



	printf("* Server starts *\n");

}

void* TaskCode1(void* argument) {
	int tid;
	char msg1[MAXLINE];
	char msg2[MAXLINE];
	tid = *((int*)argument);
	if (tid == 0) {
		//1번 클라이언트 -> 2번 클라이언트
		int from;
		int to;
		from = clientfd2;
		to = clientfd3;
		while (1) {
			n = read(from, msg1, MAXLINE);
			pthread_mutex_lock(&pmt);
			if (result == 0) {
				pthread_mutex_unlock(&pmt);
				break;
			}
			result = strcmp(msg1, "-1\n");
			if (result == 0) {
				pthread_mutex_unlock(&pmt);
				break;
			}
			pthread_mutex_unlock(&pmt);
			n = write(to, msg1, strlen(msg1) + 1);
		}
	}
	else {
		//2번 클라이언트 -> 1번 클라이언트
		int from;
		int to;
		from = clientfd4;
		to = clientfd1;
		while (1) {
			n = read(from, msg2, MAXLINE);
			pthread_mutex_lock(&pmt);
			if (result == 0) {
				pthread_mutex_unlock(&pmt);
				break;
			}
			result = strcmp(msg2, "-1\n");
			if (result == 0) {
				pthread_mutex_unlock(&pmt);
				break;
			}
			pthread_mutex_unlock(&pmt);
			n = write(to, msg2, strlen(msg2) + 1);
		}
	}
}


void* TaskCode2(void* argument) {

	int tid;
	char resultMsg[MAXLINE]= "-1\n";
	tid = *((int*)argument);

	pthread_join(threads[tid], NULL);
	switch (tid) {
	case 0:
		n = write(clientfd1, resultMsg, strlen(resultMsg) + 1);
		break;
	case 1:
		n = write(clientfd3, resultMsg, strlen(resultMsg) + 1);
		break;
	}
}


