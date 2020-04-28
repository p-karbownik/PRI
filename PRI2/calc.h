
#ifndef PRI2_CALC_H
#define PRI2_CALC_H
#include <stdio.h>
#include <string.h>

#define SIZE 72

void load(char *z, int amount);
void addSign(char *tab, int size);
void resetArray (char *array, int size);
void checkNumber(char *number, int size);
void reverseArray(char *tab, int length);
int isBigger(char *tab1, char *tab2);
void swap(char *a, char *b);
void subtraction(char number1[], char number2[]);
void multiply(char number1[], char number2[]);
void factorial(char number[]);
void addition(char *num1, char *num2);
void deleteZeros(char number[]);

int error;

#endif //PRI2_CALC_H
