#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<signal.h>
#include<time.h>
#define MAXLINE 256

pthread_mutex_t pmt;
pthread_t threads[2];
char sendMsg[MAXLINE];
char receiveMsg[MAXLINE];
time_t currentTime;
struct tm *timeInfo;
//통신종료 요청에 대한 결과
int fd1, fd2, n;
int result = -1;

/*
clientfd1 = open("./client1fifo1", O_WRONLY); // 1번 클라이언트에게 보낼 정보
clientfd2 = open("./client1fifo2", O_RDONLY); // 1번 클라이언트가 보낸 정보

clientfd3 = open("./client2fifo1", O_WRONLY); // 2번 클라이언트에게 보낼 정보
clientfd4 = open("./client2fifo2", O_RDONLY); // 2번 클라이언트가 보낸 정보
*/


void* TaskCode1(void* argument);
void* TaskCode2(void* argument);
void sigHandler();

int main(){

	signal(SIGHUP, sigHandler);
	signal(SIGPIPE, sigHandler);
	signal(SIGINT, sigHandler);

	pthread_mutex_init(&pmt, NULL);

	fd1 = open("./client2fifo1", O_RDONLY);
	fd2 = open("./client2fifo2", O_WRONLY);

	if(fd1 == -1 || fd2 == -1){
		perror("open");
		exit(1);
	}

	printf("[Client] starts\n");
	printf("Type \"-1\" will finish the chatting!\n");

	int args[2];
	
	args[0] = 0;
	args[1] = 1;
	pthread_create(&threads[0], NULL, TaskCode1, (void*)&args[0]);
	pthread_create(&threads[1], NULL, TaskCode2, (void*)&args[1]);

	for (int i = 0; i < 2; ++i)
		pthread_join(threads[i], NULL);


	printf("Finished the chatting!\n");
}

void* TaskCode1(void* argument) {

	while (1) {
		pthread_mutex_lock(&pmt);
                if (result == 0) {
                        pthread_mutex_unlock(&pmt);
			printf("Client 1 exit the chat!\n");
                        break;
                }
                result = strcmp(receiveMsg, "-1\n");
                if (result == 0) {
                        pthread_mutex_unlock(&pmt);
			printf("Client 1 exit the chat!\n");
                        break;
                }
                pthread_mutex_unlock(&pmt);
		n = read(fd1, receiveMsg, MAXLINE);
		pthread_mutex_lock(&pmt);
		if (result == 0) {
			pthread_mutex_unlock(&pmt);
			printf("Client 1 exit the chat!\n");
			break;
		}
		result = strcmp(receiveMsg, "-1\n");
		if (result == 0) {
			pthread_mutex_unlock(&pmt);
			printf("Client 1 exit the chat!\n");
			break;
		}
		pthread_mutex_unlock(&pmt);
		sleep(2);

		time(&currentTime);
		timeInfo = localtime(&currentTime);

        printf("[%02d:%02d]Client 1 -> %s",timeInfo->tm_hour, timeInfo->tm_min, receiveMsg);
	}

}
void* TaskCode2(void* argument) {
	
	while (1) {
		pthread_mutex_lock(&pmt);
		if (result == 0) {
			pthread_mutex_unlock(&pmt);
			printf("Client 1 exit the chat!\n");
			break;
		}
		result = strcmp(receiveMsg, "-1\n");
		if (result == 0) {
			pthread_mutex_unlock(&pmt);
			printf("Client 1 exit the chat!\n");
			break;
		}
		pthread_mutex_unlock(&pmt);
		fgets(sendMsg, MAXLINE, stdin);
		write(fd2, sendMsg, strlen(sendMsg) + 1);
		pthread_mutex_lock(&pmt);
                if (result == 0) {
                        pthread_mutex_unlock(&pmt);
                        printf("Client 1 exit the chat!\n");
                        break;
                }
                result = strcmp(sendMsg, "-1\n");
                if (result == 0) {
                        pthread_mutex_unlock(&pmt);
                        printf("Finishing chat!\n");
                        break;
                }
                pthread_mutex_unlock(&pmt);
	}
}

void sigHandler(){
        strcpy(sendMsg, "-1\n");
        write(fd2, sendMsg, strlen(sendMsg) + 1);

        exit(0);
}

