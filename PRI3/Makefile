__start__: main.out
	./main.out

main.out: main.o filters.o
	gcc main.o filters.o -o main.out

main.o: main.c image.h filters.h
	gcc main.c -c -o main.o

filters.o: filters.h filters.c image.h
	gcc filters.c -c -o filters.o

remake : clean run

