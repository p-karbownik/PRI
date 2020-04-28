
#ifndef PRI3_IMAGE_H
#define PRI3_IMAGE_H
#include <stdint.h>

typedef struct BitMapFileHeader
{
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BMF_HEADER;

typedef struct BitMapInfoHeader
{
    uint32_t biSize; //rozmiar naglowka
    uint32_t biWidth; //szerokosc obrazu
    uint32_t biHeight; //wysokosc obrazu
    uint16_t biPlanes; //liczba warstw kolorow
    uint16_t biBitCount; //liczba bitow na piksel
    uint32_t biCompression; //algorytm kompresji
    uint32_t biSizeImage; //rozmiar samego rysunku
    uint32_t biXPelsPerMeter; //rozdzielczosc pozioma
    uint32_t biYPelsPerMeter; //rozdzielczosc pionowa
    uint32_t biClrUsed; //liczba kolorow w palecie
    uint8_t biClrImportant; // liczba waznych kolorow
    uint8_t biClrRotation; //flaga rotacja palety, 0 NIE 1 TAK
    uint16_t biReserved;
} BMI_HEADER;

typedef struct Pixel
{
    unsigned char B;
    unsigned char G;
    unsigned char R;
    unsigned char Reserved;
}Pixel;


typedef struct Image
{
    BMF_HEADER* ptr_to_BMF_HEADER;
    BMI_HEADER* ptr_to_BMI_HEADER;
    Pixel** pixMap;
}Image;

#endif //PRI3_IMAGE_H
