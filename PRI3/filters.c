#include <stdio.h>
#include "image.h"
#include "filters.h"

#include <stdlib.h>
#include <string.h>

int loadImage(char fileName[], Image** image)
{
    int i, j;
    short padding;
    Pixel** pixMap;
    FILE* ptr_to_file = fopen(fileName, "rb");

    if(ptr_to_file == NULL)
        return  FILE_OPENING_FAIL;

    *image = (Image*) malloc(sizeof(Image));

    (*image)->ptr_to_BMF_HEADER = (BMF_HEADER*) malloc(sizeof(BMF_HEADER)); //przydziela pamiec naglowkowi pliku

    fread(&((*image)->ptr_to_BMF_HEADER->bfType), 2, 1, ptr_to_file);

    if((*image)->ptr_to_BMF_HEADER->bfType != BMPID) //Sprawdza poprawnosc pliku
    {
        free((*image)->ptr_to_BMF_HEADER);
        return WRONG_FILE_FORMAT;
    }

    /*loading file header and bitmap information header to memory*/
    fread(&((*image)->ptr_to_BMF_HEADER->bfSize), 4, 1, ptr_to_file);
    fread(&((*image)->ptr_to_BMF_HEADER->bfReserved1), 2, 1, ptr_to_file);
    fread(&((*image)->ptr_to_BMF_HEADER->bfReserved2), 2, 1, ptr_to_file);
    fread(&((*image)->ptr_to_BMF_HEADER->bfOffBits), 4, 1, ptr_to_file);


    (*image)->ptr_to_BMI_HEADER = (BMI_HEADER*) malloc(sizeof(BMI_HEADER));

    fread(&(*image)->ptr_to_BMI_HEADER->biSize, 4, 1, ptr_to_file);
    fread(&(*image)->ptr_to_BMI_HEADER->biWidth, 4, 1, ptr_to_file);
    fread(&(*image)->ptr_to_BMI_HEADER->biHeight, 4, 1, ptr_to_file);
    fread(&(*image)->ptr_to_BMI_HEADER->biPlanes, 2, 1, ptr_to_file);
    fread(&(*image)->ptr_to_BMI_HEADER->biBitCount, 2, 1, ptr_to_file);
    fread(&(*image)->ptr_to_BMI_HEADER->biCompression, 4, 1, ptr_to_file);
    fread(&(*image)->ptr_to_BMI_HEADER->biSizeImage, 4, 1, ptr_to_file);
    fread(&(*image)->ptr_to_BMI_HEADER->biXPelsPerMeter, 4, 1, ptr_to_file);
    fread(&(*image)->ptr_to_BMI_HEADER->biYPelsPerMeter, 4, 1, ptr_to_file);
    fread(&(*image)->ptr_to_BMI_HEADER->biClrUsed, 4, 1, ptr_to_file);
    fread(&(*image)->ptr_to_BMI_HEADER->biClrImportant, 1, 1, ptr_to_file);
    fread(&(*image)->ptr_to_BMI_HEADER->biClrRotation, 1, 1, ptr_to_file);
    fread(&(*image)->ptr_to_BMI_HEADER->biReserved, 2, 1, ptr_to_file);

    if(((*image)->ptr_to_BMI_HEADER->biBitCount != 16) && ((*image)->ptr_to_BMI_HEADER->biBitCount != 24) && ((*image)->ptr_to_BMI_HEADER->biBitCount != 32))
    {
        free((*image)->ptr_to_BMI_HEADER);
        free((*image)->ptr_to_BMF_HEADER);
        free((*image));

        return WRONG_COLOUR_DEPTH;
    }

    pixMap = (Pixel**) malloc(sizeof(Pixel*) * (*image)->ptr_to_BMI_HEADER->biHeight);

    for(i = 0; i < (*image)->ptr_to_BMI_HEADER->biHeight; i++)
        pixMap[i] = (Pixel*) malloc(sizeof(Pixel) * (*image)->ptr_to_BMI_HEADER->biWidth);

    fseek(ptr_to_file, (*image)->ptr_to_BMF_HEADER->bfOffBits,SEEK_SET);
    /* loading pixel array to memory */
    switch ((*image)->ptr_to_BMI_HEADER->biBitCount)
    {
        case 16:
            padding = (((*image)->ptr_to_BMI_HEADER->biWidth * 2 ) % 4) ? (4 - ((*image)->ptr_to_BMI_HEADER->biWidth * 2 ) % 4) : 0;

            for(i = 0; i < (*image)->ptr_to_BMI_HEADER->biHeight; i++, fseek(ptr_to_file, padding, SEEK_CUR))
                for(j = 0; j < (*image)->ptr_to_BMI_HEADER->biWidth; j++)
                {
                    uint16_t pixel;
                    uint16_t b, g, r, alpha;

                    fread(&pixel, sizeof(uint16_t), 1, ptr_to_file);

                    r = pixel >> 10;
                    g = pixel >> 5;
                    b = pixel;
                    alpha = pixel >> 15;

                    r &= 0x001fu;
                    g &= 0x001fu;
                    b &= 0x001fu;
                    alpha = pixel & 0x0001u;

                    pixMap[i][j].B = (unsigned char) b;
                    pixMap[i][j].R = (unsigned char) r;
                    pixMap[i][j].G = (unsigned char) g;
                    pixMap[i][j].Reserved = (unsigned char) alpha;

                    pixel = 0;
                }
                break;

        case 24:
            padding = ((*image)->ptr_to_BMI_HEADER->biWidth * 3 ) % 4 ? 4 - ((*image)->ptr_to_BMI_HEADER->biWidth * 3 ) % 4 : 0;

            for(i = 0; i < (*image)->ptr_to_BMI_HEADER->biHeight; i++, fseek(ptr_to_file, padding, SEEK_CUR))
                for(j = 0; j < (*image)->ptr_to_BMI_HEADER->biWidth; j++)
                {
                    pixMap[i][j].B = fgetc(ptr_to_file);
                    pixMap[i][j].G = fgetc(ptr_to_file);
                    pixMap[i][j].R = fgetc(ptr_to_file);
                }
        break;

        case 32:
            for(i = 0; i < (*image)->ptr_to_BMI_HEADER->biHeight; i++)
                for(j = 0; j <(*image)->ptr_to_BMI_HEADER->biWidth; j++)
                {
                     pixMap[i][j].Reserved = fgetc(ptr_to_file);
                     pixMap[i][j].B = fgetc(ptr_to_file);
                     pixMap[i][j].G = fgetc(ptr_to_file);
                     pixMap[i][j].R = fgetc(ptr_to_file);
                }
            break;
    }

    (*image)->pixMap = pixMap;
    fclose(ptr_to_file);

    return 0;
}

int greyscale(char fileName[], Image** image)
{
    FILE *ptr_to_source_file, *ptr_to_destination_file;
    char *newFileName;
    long j = 0;
    int i = 0;
    int padding = 0;
    uint32_t dataOffset = (*image)->ptr_to_BMF_HEADER->bfOffBits;
    uint32_t height = (*image)->ptr_to_BMI_HEADER->biHeight;
    uint32_t width = (*image)->ptr_to_BMI_HEADER->biWidth;
    Pixel** pixMap = (*image)->pixMap;
    uint16_t bitCount = (*image)->ptr_to_BMI_HEADER->biBitCount;
    char grey;
    char reserved;

    ptr_to_source_file = fopen(fileName, "rb");

    if(ptr_to_source_file == NULL)
        return FILE_OPENING_FAIL;

    newFileName = (char*) malloc((strlen(fileName) + strlen("grayscale_") + 1) * sizeof(char));

    if(newFileName == NULL)
    {
        fclose(ptr_to_source_file);
        return ALLOCATION_ERROR;
    }

    strcpy(newFileName, "grayscale_");
    strcat(newFileName, fileName);

    ptr_to_destination_file = fopen(newFileName, "wb");

    if(ptr_to_destination_file == NULL)
    {
        fclose(ptr_to_source_file);
        free(newFileName);

        return FILE_OPENING_FAIL;
    }

    /*copying the file and bitmap information headers */
    fseek(ptr_to_source_file, 0, SEEK_END);
    j = ftell(ptr_to_source_file);
    fseek(ptr_to_source_file, 0, SEEK_SET);

    for(i = 0; i < j; i++)
        fputc(fgetc(ptr_to_source_file), ptr_to_destination_file);

    fseek(ptr_to_destination_file, dataOffset, SEEK_SET);
    /*copying modified pixel array */
    switch (bitCount)
    {
        case 16:
            padding = (width * 2) % 4 ? 4 - (width * 2) % 4 : 0;
            for(i = 0; i < height; i++, fseek(ptr_to_destination_file, padding, SEEK_CUR))
                for(j = 0; j < width; j++)
                {
                    uint16_t red, blue, green, pixel;
                    red = blue = green = ((pixMap[i][j].R + pixMap[i][j].G + pixMap[i][j].B) / 3);

                    red = red << 10;
                    green = green << 5;
                    blue = blue;

                    pixel = blue + green + red + (((uint16_t) pixMap[i][j].Reserved) << 15);

                    fwrite(&pixel, 2, 1, ptr_to_destination_file);
                }
            break;
        case 24:
            padding = (width * 3) % 4 ? 4 - (width * 3) % 4 : 0;
            for(i = 0; i < height; i++, fseek(ptr_to_destination_file, padding, SEEK_CUR))
                for(j = 0; j < width; j++)
                {
                    grey = (char) (0.229 * pixMap[i][j].R + 0.587 * pixMap[i][j].G + 0.114 * pixMap[i][j].B); /*the formula is used in YUV */
                    for(int k = 0; k < 3; k++)
                        fputc(grey, ptr_to_destination_file);
                }
            break;

        case 32:
            for(i = 0; i < height; i++)
                for(j = 0; j < width; j++)
                {
                    grey = (char) (0.229 * pixMap[i][j].R + 0.587 * pixMap[i][j].G + 0.114 * pixMap[i][j].B);
                    reserved = pixMap[i][j].Reserved;

                    fputc(reserved, ptr_to_destination_file);
                    for(int k = 0; k < 3; k++)
                        fputc(grey, ptr_to_destination_file);
                }
            break;
    }

    fclose(ptr_to_source_file);
    fclose(ptr_to_destination_file);
    free(newFileName);

    return 0;
}

int negative(char fileName[], Image** image)
{
    FILE *ptr_to_source_file, *ptr_to_destination_file;
    char *newFileName;
    char red;
    char green;
    char blue;
    char reserved;
    int j = 0;
    int i = 0;
    uint32_t dataOffset = (*image)->ptr_to_BMF_HEADER->bfOffBits;
    uint32_t height = (*image)->ptr_to_BMI_HEADER->biHeight;
    uint32_t width = (*image)->ptr_to_BMI_HEADER->biWidth;
    Pixel** pixMap = (*image)->pixMap;
    uint16_t bitCount = (*image)->ptr_to_BMI_HEADER->biBitCount;
    int padding;

    ptr_to_source_file = fopen(fileName, "rb");

    if(ptr_to_source_file == NULL)
        return FILE_OPENING_FAIL;

    newFileName = (char*) malloc((strlen(fileName) + strlen("negative_") + 1) * sizeof(char));

    if(newFileName == NULL)
    {
        fclose(ptr_to_source_file);
        return ALLOCATION_ERROR;
    }

    strcpy(newFileName, "negative_");
    strcat(newFileName, fileName);

    ptr_to_destination_file = fopen(newFileName, "wb");

    if(ptr_to_destination_file == NULL)
    {
        fclose(ptr_to_source_file);
        free(newFileName);

        return FILE_OPENING_FAIL;
    }

    /*copying the file and bitmap information headers */

    fseek(ptr_to_source_file, 0, SEEK_END);
    j = ftell(ptr_to_source_file);
    fseek(ptr_to_source_file, 0, SEEK_SET);

    for(i = 0; i < j; i++)
        fputc(fgetc(ptr_to_source_file), ptr_to_destination_file);

    fseek(ptr_to_destination_file, dataOffset, SEEK_SET);

    switch (bitCount)
    {
        case 16:
            padding = (width * 2) % 4 ? 4 - (width * 2) % 4 : 0;
            for (i = 0; i < height; i++, fseek(ptr_to_destination_file, padding, SEEK_CUR))
                for (j = 0; j < width; j++) {
                    uint16_t red, blue, green, pixel;

                    red = (31 - pixMap[i][j].R) << 10;
                    green = (31 - pixMap[i][j].G) << 5;
                    blue = (31 - pixMap[i][j].B);

                    pixel = green + red + blue + (pixMap[i][j].Reserved << 15);
                    fwrite(&pixel, 2, 1, ptr_to_destination_file);
                }
            break;

        case 24:
            padding = (width * 3) % 4 ? 4 - (width * 3) % 4 : 0;
            for (i = 0; i < height; i++, fseek(ptr_to_destination_file, padding, SEEK_CUR))
                for (j = 0; j < width; j++) {
                    red = 255 - pixMap[i][j].R;
                    blue = 255 - pixMap[i][j].B;
                    green = 255 - pixMap[i][j].G;
                    fputc(blue, ptr_to_destination_file);
                    fputc(green, ptr_to_destination_file);
                    fputc(red, ptr_to_destination_file);
                }
            break;
        case 32:
            for(i = 0; i < height; i++)
                for(j = 0; j < width; j++)
                {
                    red = 255 - pixMap[i][j].R;
                    blue = 255 - pixMap[i][j].B;
                    green = 255 - pixMap[i][j].G;
                    reserved = pixMap[i][j].Reserved;
                    fputc(reserved, ptr_to_destination_file);
                    fputc(blue, ptr_to_destination_file);
                    fputc(green, ptr_to_destination_file);
                    fputc(red, ptr_to_destination_file);
                }
            break;
    }

    free(newFileName);
    fclose(ptr_to_destination_file);
    fclose(ptr_to_source_file);

    return 0;
}

int getMaximum(int tab[], int n)
{
    int result = tab[0], i;

    for(i = 1; i < n; i++)
    {
        if(result < tab[i])
        {
            result = tab[i];
        }
    }

    return result;
}

void sort(unsigned char tab[], int left,int right)
{
    int i = left, change;
    int j = right;
    int x = tab[(left + right) / 2];

    do
    {
        while(tab[i] < x)
        {
            i++;
        }
        while(tab[j]>x)
        {
            j--;
        }
        if(i<=j)
        {
            change = tab[i];
            tab[i] = tab[j];
            tab[j] = change;
            i++;
            j--;
        }
    }while(i <= j);

    if(left < j)
    {
        sort(tab, left, j);
    }
    if(right > i)
    {
        sort(tab, i, right);
    }
}

int getMinimum(unsigned char tab[], int n)
{
    int result = tab[0], i;

    for (i = 1; i < n; i++)
    {
        if (result > tab[i])
        {
            result = tab[i];
        }
    }
    return result;
}

int getMedian(unsigned char tab[],int n)
{
    if(n % 2 == 1)
        return (tab[(n + 1) / 2]);

    else
        return ((tab[n / 2] + tab[(n / 2) + 1]) / 2);
}

int medianFilter(char fileName[], Image** image, int frameWidth, int frameHeight) {
    FILE *ptr_to_source_file, *ptr_to_destination_file;
    char *newFileName;

    int i, j, k, m, n;
    i = j = k = m = n = 0;
    uint32_t dataOffset = (*image)->ptr_to_BMF_HEADER->bfOffBits;
    uint32_t height = (*image)->ptr_to_BMI_HEADER->biHeight;
    uint32_t width = (*image)->ptr_to_BMI_HEADER->biWidth;
    Pixel **pixMap = (*image)->pixMap;
    uint16_t bitCount = (*image)->ptr_to_BMI_HEADER->biBitCount;

    ptr_to_source_file = fopen(fileName, "rb");

    if (ptr_to_source_file == NULL)
        return FILE_OPENING_FAIL;

    newFileName = (char *) malloc((strlen(fileName) + strlen("median_") + 1) * sizeof(char));

    if (newFileName == NULL) {
        fclose(ptr_to_source_file);

        return ALLOCATION_ERROR;
    }

    strcpy(newFileName, "median_");
    strcat(newFileName, fileName);

    ptr_to_destination_file = fopen(newFileName, "wb");

    if (ptr_to_destination_file == NULL) {
        fclose(ptr_to_source_file);
        free(newFileName);

        return FILE_OPENING_FAIL;
    }

    /*copying the file and bitmap information headers */

    fseek(ptr_to_source_file, 0, SEEK_END);
    j = ftell(ptr_to_source_file);
    fseek(ptr_to_source_file, 0, SEEK_SET);

    for (i = 0; i < j; i++)
        fputc(fgetc(ptr_to_source_file), ptr_to_destination_file);

    fseek(ptr_to_destination_file, dataOffset, SEEK_SET);

    unsigned char *tabB = malloc((2 * frameWidth + 1) * (2 * frameHeight + 1) * sizeof(unsigned char));
    unsigned char *tabR = malloc((2 * frameWidth + 1) * (2 * frameHeight + 1) * sizeof(unsigned char));
    unsigned char *tabG = malloc((2 * frameWidth + 1) * (2 * frameHeight + 1) * sizeof(unsigned char));

    int padding;

    switch (bitCount)
    {
        case 16:
            padding = (width * 2) % 4 ? 4 - (width * 2) % 4 : 0;
            for (i = 0; i < height; i++, fseek(ptr_to_destination_file, padding, SEEK_CUR)) {
                for (j = 0; j < width; j++) {
                    n = 0;
                    for (k = i - frameHeight; k < i + frameHeight; k++) {
                        for (m = j - frameWidth; m < j + frameWidth; m++) {
                            if (k >= 0 && k < height && m >= 0 && m < width) {
                                tabB[n] = pixMap[k][m].B;
                                tabR[n] = pixMap[k][m].R;
                                tabG[n] = pixMap[k][m].G;
                                n++;
                            }
                        }
                    }

                    sort(tabB, 0, n - 1);
                    sort(tabG, 0, n - 1);
                    sort(tabR, 0, n - 1);

                    uint16_t blue = (uint16_t) getMedian(tabB, n);
                    uint16_t green = (uint16_t) getMedian(tabG, n);
                    uint16_t red = (uint16_t) getMedian(tabR, n);
                    uint16_t alpha = pixMap[i][j].Reserved;

                    red = red << 10;
                    green = green << 5;

                    uint16_t pixel = green + red + blue + (alpha << 15);
                    uint16_t *wsk_pixel = &pixel;

                    fwrite(wsk_pixel, 2, 1, ptr_to_destination_file);
                }
            }
            break;

        case 24 :
            padding = (width * 3) % 4 ? 4 - (width * 3) % 4 : 0;
            for (i = 0; i < height; i++, fseek(ptr_to_destination_file, padding, SEEK_CUR)) {
                for (j = 0; j < width; j++) {
                    n = 0;
                    for (k = i - frameHeight; k < i + frameHeight; k++) {
                        for (m = j - frameWidth; m < j + frameWidth; m++) {
                            if (k >= 0 && k < height && m >= 0 && m < width) {
                                tabB[n] = pixMap[k][m].B;/*wpisanie kolorow do tablicy pomocniczej*/
                                tabR[n] = pixMap[k][m].R;
                                tabG[n] = pixMap[k][m].G;
                                n++;
                            }
                        }
                    }

                    sort(tabB, 0, n - 1);/*sortowaie aby znalesc mediane*/
                    sort(tabG, 0, n - 1);
                    sort(tabR, 0, n - 1);

                    fputc((char) (getMedian(tabB, n)), ptr_to_destination_file);/*podmiana danych w pliku*/
                    fputc((char) (getMedian(tabG, n)), ptr_to_destination_file);
                    fputc((char) (getMedian(tabR, n)), ptr_to_destination_file);
                }
            }
            break;

        case 32:
            for (i = 0; i < height; i++) {
                for (j = 0; j < width; j++) {
                    n = 0;
                    for (k = i - frameHeight; k < i + frameHeight; k++) {
                        for (m = j - frameWidth; m < j + frameWidth; m++) {
                            if (k >= 0 && k < height && m >= 0 && m < width) {
                                tabB[n] = pixMap[k][m].B;/*wpisanie kolorow do tablicy pomocniczej*/
                                tabR[n] = pixMap[k][m].R;
                                tabG[n] = pixMap[k][m].G;
                                n++;
                            }
                        }
                    }
                    sort(tabB, 0, n - 1);/*sortowaie aby znalesc mediane*/
                    sort(tabG, 0, n - 1);
                    sort(tabR, 0, n - 1);
                    char reserved = pixMap[i][j].Reserved;
                    fputc(reserved, ptr_to_destination_file);
                    fputc((char) (getMedian(tabB, n)), ptr_to_destination_file);/*podmiana danych w pliku*/
                    fputc((char) (getMedian(tabG, n)), ptr_to_destination_file);
                    fputc((char) (getMedian(tabR, n)), ptr_to_destination_file);
                }
            }
            break;
    }

    free(tabG);
    free(tabB);
    free(tabR);
    free(newFileName);

    fclose(ptr_to_source_file);
    fclose(ptr_to_destination_file);

    return 0;
}

int minimalFilter(char fileName[], Image** image, int frameWidth, int frameHeight)
{
    FILE *ptr_to_source_file, *ptr_to_destination_file;
    char *newFileName;

    int i, j, k, m, n;
    i = j = k = m = n = 0;
    uint32_t dataOffset = (*image)->ptr_to_BMF_HEADER->bfOffBits;
    uint32_t height = (*image)->ptr_to_BMI_HEADER->biHeight;
    uint32_t width = (*image)->ptr_to_BMI_HEADER->biWidth;
    Pixel** pixMap = (*image)->pixMap;
    uint16_t bitCount = (*image)->ptr_to_BMI_HEADER->biBitCount;

    ptr_to_source_file = fopen(fileName, "rb");

    if(ptr_to_source_file == NULL)
        return FILE_OPENING_FAIL;

    newFileName = (char *) malloc((strlen(fileName) + strlen("minimal_") + 1) * sizeof(char));

    if (newFileName == NULL) {
        fclose(ptr_to_source_file);

        return ALLOCATION_ERROR;
    }

    strcpy(newFileName, "minimal_");
    strcat(newFileName, fileName);

    ptr_to_destination_file = fopen(newFileName, "wb");

    if (ptr_to_destination_file == NULL) {
        fclose(ptr_to_source_file);
        free(newFileName);

        return FILE_OPENING_FAIL;
    }

    fseek(ptr_to_source_file, 0, SEEK_END);
    j = ftell(ptr_to_source_file);
    fseek(ptr_to_source_file, 0, SEEK_SET);

    for(i = 0; i < j; i++)
        fputc(fgetc(ptr_to_source_file), ptr_to_destination_file);

    fseek(ptr_to_destination_file, dataOffset, SEEK_SET);

    unsigned char *tabB = malloc((2 * frameWidth + 1) * (2 * frameHeight + 1) * sizeof(unsigned char));
    unsigned char *tabR = malloc((2 * frameWidth + 1) * (2 * frameHeight + 1) * sizeof(unsigned char));
    unsigned char *tabG = malloc((2 * frameWidth + 1) * (2 * frameHeight + 1) * sizeof(unsigned char));

    int padding;

    switch (bitCount)
    {
        case 16:
            padding = (width * 2) % 2 ? 2 - (width * 2) % 4 : 0;
            for(i = 0; i < height; i++, fseek(ptr_to_destination_file, padding, SEEK_CUR))
            {
                for(j = 0; j < width; j++)
                {
                    n = 0;
                    for(k = i - frameHeight; k < i + frameHeight; k++)
                    {
                        for(m = j - frameWidth; m < j + frameWidth; m++)
                        {
                            if(k >= 0 && k < height && m >= 0 && m < width)
                            {
                                tabB[n] = pixMap[k][m].B;
                                tabR[n] = pixMap[k][m].R;
                                tabG[n] = pixMap[k][m].G;
                                n++;
                            }
                        }
                    }

                    uint16_t blue = (uint16_t) getMinimum(tabB, n);
                    uint16_t red = (uint16_t) getMinimum(tabR, n);
                    uint16_t green = (uint16_t) getMinimum(tabG, n);
                    uint16_t alpha = pixMap[i][j].Reserved;

                    red = red << 10;
                    green = green << 5;

                    uint16_t pixel = green + red + blue + (alpha << 15);
                    uint16_t *wsk_pixel = &pixel;

                    fwrite(wsk_pixel, 2, 1, ptr_to_destination_file);
                }
            }
            break;

        case 24:
            padding = (width * 3 ) % 4 ? 4 - (width * 3 ) % 4 : 0;
            for(i = 0; i < height; i++, fseek(ptr_to_destination_file, padding, SEEK_CUR))
            {
                for(j = 0; j < width; j++)
                {
                    n = 0;
                    for(k = i - frameHeight; k < i + frameHeight; k++)
                    {
                        for(m = j - frameWidth; m < j + frameWidth; m++)
                        {
                            if(k >= 0 && k < height && m >= 0 && m < width)
                            {
                                tabB[n] = pixMap[k][m].B;/*wpisanie do tablicy kolorow*/
                                tabR[n] = pixMap[k][m].R;
                                tabG[n] = pixMap[k][m].G;
                                n++;
                            }
                        }
                    }
                    fputc((char)(getMinimum(tabB,n)), ptr_to_destination_file);/*nadpisaie kolorem o najmniejszej skladowej*/
                    fputc((char)(getMinimum(tabG,n)), ptr_to_destination_file);
                    fputc((char)(getMinimum(tabR,n)), ptr_to_destination_file);
                }
            }
            break;

        case 32:
            for(i = 0; i < height; i++)
            {
                for(j = 0; j < width; j++)
                {
                    n = 0;
                    for(k = i - frameHeight; k < i + frameHeight; k++)
                    {
                        for(m = j - frameWidth; m < j + frameWidth; m++)
                        {
                            if(k >= 0 && k < height && m >= 0 && m < width)
                            {
                                tabB[n] = pixMap[k][m].B;/*wpisanie do tablicy kolorow*/
                                tabR[n] = pixMap[k][m].R;
                                tabG[n] = pixMap[k][m].G;
                                n++;
                            }
                        }
                    }
                    fputc(pixMap[i][j].Reserved, ptr_to_destination_file);
                    fputc(getMinimum(tabB, n), ptr_to_destination_file);/*nadpisanie kolorem o najmniejszej skladowej*/
                    fputc(getMinimum(tabG, n), ptr_to_destination_file);
                    fputc(getMinimum(tabR, n), ptr_to_destination_file);
                }
            }
            break;
        }

    free(tabB);
    free(tabG);
    free(tabR);

    free(newFileName);

    fclose(ptr_to_destination_file);
    fclose(ptr_to_source_file);

    return 0;
}

int maximalFilter(char fileName[], Image** image, int frameWidth, int frameHeight)
{
    FILE *ptr_to_source_file, *ptr_to_destination_file;
    char *newFileName;
    int i, j, k, m, n;
    i = j = k = m = n = 0;
    uint32_t dataOffset = (*image)->ptr_to_BMF_HEADER->bfOffBits;
    uint32_t height = (*image)->ptr_to_BMI_HEADER->biHeight;
    uint32_t width = (*image)->ptr_to_BMI_HEADER->biWidth;
    Pixel** pixMap = (*image)->pixMap;
    uint16_t bitCount = (*image)->ptr_to_BMI_HEADER->biBitCount;

    ptr_to_source_file = fopen(fileName, "rb");

    if(ptr_to_source_file == NULL)
        return FILE_OPENING_FAIL;

    newFileName = (char *) malloc((strlen(fileName) + strlen("maximal_") + 1) * sizeof(char));

    if (newFileName == NULL) {
        fclose(ptr_to_source_file);

        return ALLOCATION_ERROR;
    }

    strcpy(newFileName, "maximal_");
    strcat(newFileName, fileName);

    ptr_to_destination_file = fopen(newFileName, "wb");

    if (ptr_to_destination_file == NULL) {
        fclose(ptr_to_source_file);
        free(newFileName);

        return FILE_OPENING_FAIL;
    }
    /*copying the file and bitmap information headers */

    fseek(ptr_to_source_file, 0, SEEK_END);
    j = ftell(ptr_to_source_file);

    fseek(ptr_to_source_file, 0, SEEK_SET);
    for(i = 0; i < j; i++)
        fputc(fgetc(ptr_to_source_file), ptr_to_destination_file);

    fseek(ptr_to_destination_file, dataOffset, SEEK_SET);

    int *tabB = malloc((2 * frameWidth + 1) * (2 * frameHeight + 1) * sizeof(int));/*przydzielnie pamieic dla tablic kolorow pomocniczej*/
    int *tabR = malloc((2 * frameWidth + 1) * (2 * frameHeight + 1) * sizeof(int));
    int *tabG = malloc((2 * frameWidth + 1) * (2 * frameHeight + 1) * sizeof(int));
    int padding = 0;

    switch (bitCount)
    {
        case 16:
            padding = (width * 2) % 4 ? 4 - (width * 2) % 4 : 0;
            for(i = 0; i < height; i++, fseek(ptr_to_destination_file, padding, SEEK_CUR))
            {
                for(j = 0; j < width; j++)
                {
                    n = 0;
                    for(k = i - frameHeight; k < i + frameHeight; k++)
                    {
                        for(m = j - frameWidth; m < j + frameWidth; m++)
                        {
                            if(k >= 0 && k < height && m >= 0 && m < width)
                            {
                                tabB[n] = pixMap[k][m].B;
                                tabR[n] = pixMap[k][m].R;
                                tabG[n] = pixMap[k][m].G;
                                n++;
                            }
                        }
                    }
                    uint16_t blue = (uint16_t) getMaximum(tabB, n);
                    uint16_t green = (uint16_t) getMaximum(tabG, n);
                    uint16_t red = (uint16_t) getMaximum(tabR, n);
                    uint16_t alpha = pixMap[i][j].Reserved;

                    red = red << 10;
                    green = green << 5;
                    uint16_t pixel = green + red + blue + (alpha << 15);
                    uint16_t *wsk_pixel = &pixel;

                    fwrite(wsk_pixel, 2, 1, ptr_to_destination_file);
                }
            }
            break;

        case 24:
            padding = (width * 3 ) % 4 ? 4 - (width * 3 ) % 4 : 0;
            for(i = 0; i < height; i++, fseek(ptr_to_destination_file, padding, SEEK_CUR))
            {
                for(j = 0; j < width; j++)
                {
                    n = 0;
                    for(k = i - frameHeight; k < i + frameHeight; k++)
                    {
                        for(m = j - frameWidth; m < j + frameWidth; m++)
                        {
                            if(k >= 0 && k < height && m >= 0 && m < width)
                            {
                                tabB[n] = pixMap[k][m].B;
                                tabR[n] = pixMap[k][m].R;
                                tabG[n] = pixMap[k][m].G;
                                n++;
                            }
                        }
                    }
                    fputc((char)(getMaximum(tabB,n)), ptr_to_destination_file);
                    fputc((char)(getMaximum(tabG,n)), ptr_to_destination_file);
                    fputc((char)(getMaximum(tabR,n)), ptr_to_destination_file);
                }
            }
            break;

        case 32:
            for(i = 0; i < height; i++)
            {
                for(j = 0; j < width; j++)
                {
                    n = 0;
                    for(k = i - frameHeight; k < i + frameHeight; k++)
                    {
                        for(m = j - frameWidth; m < j + frameWidth; m++)
                        {
                            if(k >= 0 && k < height && m >= 0 && m < width)
                            {
                                tabB[n] = pixMap[k][m].B;
                                tabR[n] = pixMap[k][m].R;
                                tabG[n] = pixMap[k][m].G;
                                n++;
                            }
                        }
                    }
                    fputc((char)pixMap[i][j].Reserved, ptr_to_destination_file);
                    fputc((char)(getMaximum(tabB,n)), ptr_to_destination_file);
                    fputc((char)(getMaximum(tabG,n)), ptr_to_destination_file);
                    fputc((char)(getMaximum(tabR,n)), ptr_to_destination_file);
                }
            }
            break;
    }
    free(tabB);
    free(tabG);
    free(tabR);
    free(newFileName);

    fclose(ptr_to_source_file);
    fclose(ptr_to_destination_file);
    return 0;
}

int averagingFilter(char fileName[], Image** image, int frameWidth, int frameHeight)
{
    FILE *ptr_to_source_file, *ptr_to_destination_file;
    char *newFileName;
    int i, j, k, m, rSum, gSum, bSum, denominator;
    i = j = k = m = rSum = gSum = bSum = denominator = 0;
    uint32_t dataOffset = (*image)->ptr_to_BMF_HEADER->bfOffBits;
    uint32_t height = (*image)->ptr_to_BMI_HEADER->biHeight;
    uint32_t width = (*image)->ptr_to_BMI_HEADER->biWidth;
    Pixel** pixMap = (*image)->pixMap;
    uint16_t bitCount = (*image)->ptr_to_BMI_HEADER->biBitCount;

    ptr_to_source_file = fopen(fileName, "rb");
    if(ptr_to_source_file == NULL)
        return FILE_OPENING_FAIL;

    newFileName = (char *) malloc((strlen(fileName) + strlen("averaging_") + 1) * sizeof(char));

    if (newFileName == NULL) {
        fclose(ptr_to_source_file);

        return ALLOCATION_ERROR;
    }

    strcpy(newFileName, "averaging_");
    strcat(newFileName, fileName);

    ptr_to_destination_file = fopen(newFileName, "wb");

    if (ptr_to_destination_file == NULL) {
        fclose(ptr_to_source_file);
        free(newFileName);

        return FILE_OPENING_FAIL;
    }

    /*copying the file and bitmap information headers */

    fseek(ptr_to_source_file, 0, SEEK_END);
    j = ftell(ptr_to_source_file);
    fseek(ptr_to_source_file, 0, SEEK_SET);
    for(i = 0; i < j; i++)
        fputc(fgetc(ptr_to_source_file), ptr_to_destination_file);


    fseek(ptr_to_destination_file, dataOffset, SEEK_SET);
    int *tabB = malloc((2 * frameWidth + 1) * (2 * frameHeight + 1) * sizeof(int));
    int *tabR = malloc((2 * frameWidth + 1) * (2 * frameHeight + 1) * sizeof(int));
    int *tabG = malloc((2 * frameWidth + 1) * (2 * frameHeight + 1) * sizeof(int));

    int padding = 0;

    switch (bitCount)
    {
        case 16:
            padding = (width * 2) % 4 ? 4 - (width * 2) % 4 : 0;
            for(i = 0; i < height; i++, fseek(ptr_to_destination_file, padding, SEEK_CUR))
            {
                for (j = 0; j < width; j++)
                {
                    denominator = 0;
                    rSum = 0;
                    gSum = 0;
                    bSum = 0;
                    for (k = i - frameHeight; k < i + frameHeight; k++)
                    {
                        for (m = j - frameWidth; m < j + frameWidth; m++)
                        {
                            if (k >= 0 && k < height && m >= 0 && m < width)
                            {
                                denominator++;
                                rSum += pixMap[k][m].R;
                                gSum += pixMap[k][m].G;
                                bSum += pixMap[k][m].B;
                            }
                        }
                    }
                    if (denominator != 0)
                    {
                        uint16_t blue = (uint16_t) (bSum / denominator);
                        uint16_t green = (uint16_t) (gSum / denominator);
                        uint16_t red = (uint16_t) (rSum / denominator);
                        uint16_t alpha = pixMap[i][j].Reserved;
                        red = red << 10;
                        green = green << 5;
                        uint16_t pixel = green + red + blue + (alpha << 15);
                        uint16_t *wsk_pixel = &pixel;
                        fwrite(wsk_pixel, 2, 1, ptr_to_destination_file);
                    }
                }
            }
            break;

        case 24:
            padding = (width * 3 ) % 4 ? 4 - (width * 3 ) % 4 : 0;
            for(i = 0; i < height; i++, fseek(ptr_to_destination_file, padding, SEEK_CUR))
            {
                for(j = 0; j < width; j++)
                {
                    denominator = 0;
                    rSum  = 0;
                    gSum = 0;
                    bSum = 0;
                    for(k = i - frameHeight; k < i + frameHeight; k++)
                    {
                        for(m = j - frameWidth; m < j + frameWidth; m++)
                        {
                            if(k >= 0 && k < height && m >= 0 && m < width)
                            {
                                denominator++;
                                rSum += pixMap[k][m].R;/*sumowanie*/
                                gSum += pixMap[k][m].G;
                                bSum += pixMap[k][m].B;
                            }
                        }
                    }
                    if(denominator != 0)
                    {
                        fputc((char)(bSum / denominator), ptr_to_destination_file);/*nadpisanie kolorow*/
                        fputc((char)(gSum / denominator), ptr_to_destination_file);
                        fputc((char)(rSum / denominator), ptr_to_destination_file);
                    }
                }
            }
            break;

        case 32:
            for(i = 0; i < height; i++)
            {
                for(j = 0; j < width; j++)
                {
                    denominator = 0;
                    rSum  = 0;
                    gSum = 0;
                    bSum = 0;
                    for(k = i - frameHeight; k < i + frameHeight; k++)
                    {
                        for(m = j - frameWidth; m < j + frameWidth; m++)
                        {
                            if(k >= 0 && k < height && m >= 0 && m < width)
                            {
                                denominator++;
                                rSum += pixMap[k][m].R;/*sumowanie*/
                                gSum += pixMap[k][m].G;
                                bSum += pixMap[k][m].B;
                            }
                        }
                    }
                    if(denominator != 0)
                    {
                        fputc(pixMap[i][j].Reserved, ptr_to_destination_file);
                        fputc((char)(bSum / denominator), ptr_to_destination_file);/*nadpisanie kolorow*/
                        fputc((char)(gSum / denominator), ptr_to_destination_file);
                        fputc((char)(rSum / denominator), ptr_to_destination_file);
                    }

                }
            }
            break;
    }

    free(tabG);
    free(tabB);
    free(tabR);
    free(newFileName);

    fclose(ptr_to_source_file);
    fclose(ptr_to_destination_file);

    return 0;
}