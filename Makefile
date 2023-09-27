exe: main.o copy.y
	gcc -o exe main.o copy.o
main.o: main.c copy.h
	gcc -c main.c
copy.o: copy.c
	gcc -c copy.c
