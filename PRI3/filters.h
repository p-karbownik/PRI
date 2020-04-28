
#ifndef PRI3_FILTERS_H
#define PRI3_FILTERS_H

#include "image.h"
#define WRONG_FILE_FORMAT -2
#define FILE_OPENING_FAIL -1
#define WRONG_COLOUR_DEPTH -3
#define ALLOCATION_ERROR -3
#define BMPID 0x4d42

int loadImage(char fileName[], Image** image);
int greyscale(char fileName[], Image** image);
int negative(char fileName[], Image** image);
int getMaximum(int tab[], int n);
void sort(unsigned char tab[], int left,int right);
int getMinimum(unsigned char tab[], int n);
int getMedian(unsigned char tab[],int n);
int medianFilter(char fileName[], Image** image, int frameWidth, int frameHeight);
int minimalFilter(char fileName[], Image** image, int frameWidth, int frameHeight);
int maximalFilter(char fileName[], Image** image, int frameWidth, int frameHeight);
int averagingFilter(char fileName[], Image** image, int frameWidth, int frameHeight);

#endif //PRI3_FILTERS_H
