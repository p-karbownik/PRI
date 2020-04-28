__start__: calc.out
	./calc.out

calc.out: main.o calc.o
	gcc main.o calc.o -o calc.out

main.o: main.c calc.h
	gcc main.c -c -o main.o

calc.o: calc.h calc.c
	gcc calc.c -c -o calc.o

remake : clean run

