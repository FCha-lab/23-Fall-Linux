#include<stdio.h>
#include<string.h>
#include"copy.h"

#define MAXLINE 100
char line[MAXLINE]; //입력 줄
char longest[MAXLINE]; // 가장 긴 줄

//입력한 문자열 강누데 가장 긴 줄을 프린트하는 프로그램
int main(){
	int len;
	int max;
	max = 0;
	while(fgets(line, MAXLINE, stdin) != NULL){
		len = strlen(line);
		if(len > max){
			max = len;
			copy(line, longest);
			//copy.h에서 구현되는 메서드
		}
	}

	if(max > 0) // 입력한 문자열 중 가장 긴 값이 존재하는 지 확인
		printf("%s", longest);

	return 0;
}
