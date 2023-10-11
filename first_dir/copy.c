#include<stdio.h>
#include"copy.h"


//copy.h의 메서드 정의
void copy(char from[], char to[]){
	int i;
	i = 0;
	while((to[i] = from[i]) != ' 0')
		++i;
	//to의 문자열에 from문자열을 넣는다. ' 0', 즉 문자열의 끝이 나올 때 까지

}
