#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "filters.h"

#define BMPID 0x4d42

int main()
{
    char fileName[30];
    Image* image;
    int i = 0;
    for(i = 0; i < 30; i++)
        fileName[i] = 0;
    printf("Podaj nazwe pliku: ");
    scanf("%29s", fileName);

    while( getchar() != '\n');
    while(loadImage(fileName, &image))
    {
        printf("Ponownie podaj nazwe pliku: ");
        scanf("%29s", fileName);
        while(getchar() != '\n');
    }
    char option = 0;
    int inputStatus = 0;
    while((option != 'K') && (option != 'k'))
    {
        printf("Dostepne filtry: \n");
        if(image->ptr_to_BMI_HEADER->biBitCount == 8)
        {
            printf("Jedyna dostepna opcja to: negatyw\nCzy chcesz uzyc?(T/N): ");
            do
            {
                option = getchar();
                while(getchar() != '\n');
                if((option == 'n') || (option == 't') || (option == 'N') || (option == 'T'))
                    inputStatus = 0;
                else
                    inputStatus = 1;
                if(inputStatus)
                {
                    printf("Niepoprawnie wprowadzony wybor!!!\n");
                    printf("Czy chcesz uzyc? (T/N): ");
                }

            }while(inputStatus);
            if(option == 'T' || option == 't')
            {
                negative(fileName, &image);
            }
            break;
        }
        else
        {
            printf("Skala szarosc -> 1\n");
            printf("Negatyw -> 2\n");
            printf("Filtr medianowy -> 3\n");
            printf("Filtr usredniajacy - > 4\n");
            printf("Filtr minimalny -> 5\n");
            printf("Filtr maksymalny -> 6\n");
            printf("Koniec -> K\n");
            printf("Wybieram opcje: ");
            option = getchar();
            while(getchar() != '\n');
            if(option == '1')
            {
                if(greyscale(fileName, &image) == 0)
                    printf("Wykonano pomyslnie\n\n");
            }
            else if(option == '2')
            {
                if(negative(fileName, &image) == 0)
                    printf("Wykonano pomyslnie\n\n");

            }
            else if((option == '3') || (option == '4') || (option == '5') || (option == '6'))
            {
                int width = 3;
                int height = 3;
                char temp_option = 0;
                printf("Domyslny rozmiar okna 3x3. Czy chcesz zmienic?(T/N): ");
                do
                {
                    temp_option = getchar();
                    while(getchar() != '\n');
                    if((temp_option == 'n') || (temp_option == 't') || (temp_option == 'N') || (temp_option == 'T'))
                        inputStatus = 0;
                    else
                        inputStatus = 1;
                    if(inputStatus)
                    {
                        printf("Niepoprawnie wprowadzony wybor!!!\n");
                        printf("Czy chcesz uzyc? (T/N): ");
                    }
                }while(inputStatus);
                if((temp_option == 'T') || (temp_option == 't'))
                {
                    printf("Podaj wysokosc (wartosci od 1 do 20): ");
                    do
                    {
                        while(!(scanf("%d", &height)))
                        {
                            while(getchar() != '\n');
                            printf("Podano niepoprawna wartosc!\n");
                            printf("Podaj ponownie wartosc wysokosci: ");
                        }
                        while(getchar() != '\n');
                        if((height > 0) && (height <= 20))
                        {
                            inputStatus = 0;
                        }
                        else
                        {
                            printf("Podano liczbe z poza zakresu\nPodaj ponownie wysokosc:");
                            inputStatus = 1;
                        }
                    }while(inputStatus);
                    printf("Podaj szerokosc (wartosci od 1 do 20): ");
                    do
                    {
                        while(!(scanf("%d", &width)))
                        {
                            while(getchar() != '\n');
                            printf("Podano niepoprawna wartosc!\n");
                            printf("Podaj ponownie wartosc szerokosci: ");
                        }
                        while(getchar() != '\n');
                        if((width > 0) && (width <= 20))
                        {
                            inputStatus = 0;
                        }
                        else
                        {
                            printf("Podano liczbe z poza zakresu\nPodaj ponownie szerokosc: ");
                            inputStatus = 1;
                        }
                    }while(inputStatus);
                }
                if(option == '3')
                {
                    if(medianFilter(fileName, &image, width, height) == 0)
                        printf("Wykonano pomyslnie\n\n");
                }

                else if(option == '4')
                {
                    if(averagingFilter(fileName, &image, width, height) == 0)
                        printf("Wykonano pomyslnie");
                }

                else if(option == '5')
                {
                    if(minimalFilter(fileName, &image, width, height) == 0)
                        printf("Wykonano pomyslnie\n\n");
                }

                else if(option == '6')
                {
                    if(maximalFilter(fileName, &image, width, height) == 0)
                        printf("Wykonano pomyslnie\n\n");
                }
            }
            else if((option == 'k') || (option == 'K'));
            else
            {
                printf("Nie ma takiej opcji! Wybierz inna:\n");
            }
        }
    }

    for(i = 0; i < image->ptr_to_BMI_HEADER->biHeight; i++)
        free(image->pixMap[i]);
    free(image->ptr_to_BMF_HEADER);
    free(image->ptr_to_BMI_HEADER);
    free(image->pixMap);
    free(image);

    return 0;
}