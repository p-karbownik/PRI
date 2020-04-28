#include "calc.h"

int main()
{
    char number1[SIZE];
    char number2[SIZE];
    char memory[SIZE];
    char option = ' ';
    char ch;

    int memoryState = 0;
    int inputState = 0;
    int i = 0;
    for (i = 0; i < SIZE; i++)
        memory[i] = 0;

    while(option != 'K')
    {
        printf("1 ---> DODAWANIE\n");
        printf("2 ---> ODEJMOWANIE\n");
        printf("3 ---> MNOZENIE\n");
        printf("4 ---> SILNIA\n");
        printf("K ---> KONIEC\n");
        printf("Wybieram opcje: ");

        for (i = 0; i < SIZE; i++)
        {
            number1[i] = 0;
            number2[i] = 0;
        }
        option = getchar();
        while ((ch = getchar()) != '\n');
        if(option == '1')
        {
            if(memoryState)
            {
                printf("Liczba zapisana w pamieci: %s\n", memory);
                printf("Podaj liczbe: ");
                load(number1, SIZE - 1);
                checkNumber(number1, SIZE);
                if(memory[0] == number1[0])
                {
                    addition(number1, memory);
                }
                else
                {
                    if(number1[0] == '+' && memory[0] == '-')
                    {
                        memory[0] = '+';
                        subtraction(number1, memory);
                        for(i = 0; i < SIZE; i++)
                            memory[i] = number1[i];
                    }
                    else
                    {
                        number1[0] = '+';
                        subtraction(memory, number1);
                    }
                }
                if(error)
                {
                    error = 0;
                    printf("Blad\n");
                    memoryState = 0;
                }
                else
                {
                    printf("Wynik: %s\n", memory);

                    printf("Czy chcesz skorzystac z wyniku w nastepnym dzialaniu? (T/N): ");
                    do
                    {
                        option = getchar();
                        while((ch = getchar()) != '\n');
                        if(inputState)
                        {
                            printf("Nie poprawnie wprowadzony wybor!!!\n");
                            printf("Czy chcesz skorzystac z wyniku w nastepnym dzialaniu? (T/N): ");
                        }
                        if((option == 'n') || (option == 't') || (option == 'N') || (option == 'T'))
                            inputState = 0;
                        else
                            inputState = 1;
                    }while(inputState);

                    if ((option == 'T') || (option == 't'))
                        memoryState = 1;
                    else
                    {
                        memoryState = 0;
                        for(i = 0; i < SIZE; i++)
                            memory[i] = 0;
                    }
                }
            }
            else
            {
                printf("Podaj pierwsza liczbe: ");
                load(number1, SIZE - 1);
                checkNumber(number1, SIZE);
                printf("Podaj druga liczbe: ");
                load(number2, SIZE - 1);
                checkNumber(number2, SIZE);
                if(number1[0] == number2[0])
                    addition(number1, number2);
                else
                {
                    if((number2[0] == '+') && (number1[0] == '-'))
                    {
                        number1[0] = '+';
                        subtraction(number2, number1);
                    }
                    else
                    {
                        number2[0] = '+';
                        subtraction(number1, number2);
                        for(i = 0; i < SIZE; i++)
                            number2[i] = number1[i];
                    }

                }
                if(error)
                {
                    error = 0;
                    printf("Blad\n");
                    memoryState = 0;
                }
                else
                {
                    printf("Wynik: %s\n", number2);
                    printf("Czy chcesz skorzystac z wyniku w nastepnym dzialaniu? (T/N): ");

                    do
                    {
                        option = getchar();
                        while((ch = getchar()) != '\n');
                        if(inputState)
                        {
                            printf("Nie poprawnie wprowadzony wybor!!!\n");
                            printf("Czy chcesz skorzystac z wyniku w nastepnym dzialaniu? (T/N): ");
                        }
                        if((option == 'n') || (option == 't') || (option == 'N') || (option == 'T'))
                            inputState = 0;
                        else
                            inputState = 1;
                    }while(inputState);

                    if ((option == 'T') || (option == 't'))
                    {
                        memoryState = 1;
                        for (i = 0; i < SIZE; i++)
                            memory[i] = number2[i];
                    }
                    else
                    {
                        memoryState = 0;
                        for(i = 0; i < SIZE; i++)
                            memory[i] = 0;
                    }
                }
            }
        }

        else if(option == '2')
        {
            if(memoryState)
            {
                printf("Liczba zapisana w pamieci: %s\n", memory);
                printf("Podaj liczbe: ");
                load(number1, SIZE - 1);
                checkNumber(number1, SIZE);
                printf("Czy liczba zapisana w pamieci ma byc odjemna? (T/N): ");
                do
                {
                    option = getchar();
                    while((ch = getchar()) != '\n');
                    if(inputState)
                    {
                        printf("Nie poprawnie wprowadzony wybor!!!\n");
                        printf("Czy liczba zapisana w pamieci ma byc odjemna? (T/N): ");
                    }
                    if((option == 'n') || (option == 't') || (option == 'N') || (option == 'T'))
                        inputState = 0;
                    else
                        inputState = 1;
                }while(inputState);
                if((option == 'T') || (option == 't'))
                {
                    if(memory[0] == number1[0])
                    {
                        subtraction(memory, number1);
                    }
                    else
                    {
                        if((memory[0] == '+') && (number1[0] == '-'))
                        {
                            number1[0] = '+';
                            addition(number1, memory);
                        }
                        else
                        {
                            number1[0] = '-';
                            addition(number1, memory);
                        }
                    }
                }
                else
                {
                    for(i = 0; i < SIZE; i++)
                    {
                        ch = memory[i];
                        memory[i] = number1[i];
                        number1[i] = ch;
                    }
                    if(memory[0] == number1[0])
                    {
                        subtraction(memory, number1);
                    }
                    else
                    {
                        if((memory[0] == '+') && (number1[0] == '-'))
                        {
                            number1[0] = '+';
                            addition(number1, memory);
                        }
                        else
                        {
                            number1[0] = '-';
                            addition(number1, memory);
                        }
                    }
                }
                if(error)
                {
                    error = 0;
                    printf("Blad\n");
                    memoryState = 0;
                }
                else
                {
                    printf("Wynik: %s\n", memory);

                    printf("Czy chcesz skorzystac z wyniku w nastepnym dzialaniu? (T/N): ");
                    do
                    {
                        option = getchar();
                        while((ch = getchar()) != '\n');
                        if(inputState)
                        {
                            printf("Nie poprawnie wprowadzony wybor!!!\n");
                            printf("Czy chcesz skorzystac z wyniku w nastepnym dzialaniu? (T/N): ");
                        }
                        if((option == 'n') || (option == 't') || (option == 'N') || (option == 'T'))
                            inputState = 0;
                        else
                            inputState = 1;
                    }while(inputState);

                    if ((option == 'T') || (option == 't'))
                        memoryState = 1;
                    else
                    {
                        memoryState = 0;
                        for(i = 0; i < SIZE; i++)
                            memory[i] = 0;
                    }
                }
            }
            else
            {
                printf("Podaj odjemna: ");
                load(number1, SIZE - 1);
                checkNumber(number1, SIZE);
                printf("Podaj odjemnik: ");
                load(number2, SIZE - 1);
                checkNumber(number2, SIZE);
                if(number1[0] == number2[0])
                    subtraction(number1, number2);
                else
                {
                    if((number1[0] == '+') && (number2[0] == '-'))
                    {
                        number2[0] = '+';
                        addition(number2, number1);
                    }
                    else
                    {
                        number2[0] = '-';
                        addition(number2, number1);
                    }
                }
                if(error)
                {
                    error = 0;
                    printf("Blad");
                    memoryState = 0;
                }
                else
                {
                    printf("Wynik: %s\n", number1);
                    printf("Czy chcesz skorzystac z wyniku w nastepnym dzialaniu? (T/N): ");

                    do
                    {
                        option = getchar();
                        while((ch = getchar()) != '\n');
                        if(inputState)
                        {
                            printf("Nie poprawnie wprowadzony wybor!!!\n");
                            printf("Czy chcesz skorzystac z wyniku w nastepnym dzialaniu? (T/N): ");
                        }
                        if((option == 'n') || (option == 't') || (option == 'N') || (option == 'T'))
                            inputState = 0;
                        else
                            inputState = 1;
                    }while(inputState);

                    if ((option == 'T') || (option == 't'))
                    {
                        memoryState = 1;
                        for (i = 0; i < SIZE; i++)
                            memory[i] = number1[i];
                    }
                    else
                    {
                        memoryState = 0;
                        for(i = 0; i < SIZE; i++)
                            memory[i] = 0;
                    }
                }
            }
        }

        else if(option == '3')
        {
            if(memoryState)
            {
                printf("Liczba zapisana w pamieci: %s\n", memory);
                printf("Podaj liczbe: ");
                load(number1, SIZE - 1);
                checkNumber(number1, SIZE);
                multiply(memory, number1);
                if(error)
                {
                    printf("Blad\n");
                    error = 0;
                    memoryState = 0;
                }
                else
                {
                    printf("Wynik: %s\n", memory);

                    printf("Czy chcesz skorzystac z wyniku w nastepnym dzialaniu? (T/N): ");
                    do
                    {
                        option = getchar();
                        while((ch = getchar()) != '\n');
                        if(inputState)
                        {
                            printf("Nie poprawnie wprowadzony wybor!!!\n");
                            printf("Czy chcesz skorzystac z wyniku w nastepnym dzialaniu/ (T/N): ");
                        }
                        if((option == 'n') || (option == 't') || (option == 'N') || (option == 'T'))
                            inputState = 0;
                        else
                            inputState = 1;
                    }while(inputState);

                    if ((option == 'T') || (option == 't'))
                        memoryState = 1;
                    else
                    {
                        memoryState = 0;
                        for(i = 0; i < SIZE; i++)
                            memory[i] = 0;
                    }
                }
            }
            else
            {
                printf("Podaj pierwsza liczbe: ");
                load(number1, SIZE - 1);
                checkNumber(number1, SIZE);
                printf("Podaj druga liczbe: ");
                load(number2, SIZE - 1);
                checkNumber(number2, SIZE);
                multiply(number2, number1);
                if(error)
                {
                    error = 0;
                    printf("Blad!!!\n");
                    memoryState = 0;
                }
                else
                {
                    printf("Wynik: %s\n", number2);
                    printf("Czy chcesz skorzystac z wyniku w nastepnym dzialaniu? (T/N): ");

                    do
                    {
                        option = getchar();
                        while((ch = getchar()) != '\n');
                        if(inputState)
                        {
                            printf("Nie poprawnie wprowadzony wybor!!!\n");
                            printf("Czy chcesz skorzystac z wyniku w nastepnym dzialaniu/ (T/N): ");
                        }
                        if((option == 'n') || (option == 't') || (option == 'N') || (option == 'T'))
                            inputState = 0;
                        else
                            inputState = 1;
                    }while(inputState);

                    if ((option == 'T') || (option == 't'))
                    {
                        memoryState = 1;
                        for (i = 0; i < SIZE; i++)
                            memory[i] = number2[i];
                    }
                    else
                    {
                        memoryState = 0;
                        for(i = 0; i < SIZE; i++)
                            memory[i] = 0;
                    }
                }
            }

        }

        else if(option == '4')
        {
            if(memoryState)
            {
                printf("Liczba zapisana w pamieci: %s\n", memory);
                if((memory[0] == '-') && (memory[1] != '0'))
                    error = 1;
                factorial(memory);
                if(error)
                {
                    error = 0;
                    printf("Blad!!!\n");
                    memoryState = 0;
                }
                else
                {
                    printf("Wynik: %s\n", memory);
                    printf("Czy chcesz skorzystac z wyniku w nastepnym dzialaniu? (T/N): ");
                    do
                    {
                        option = getchar();
                        while((ch = getchar()) != '\n');
                        if(inputState)
                        {
                            printf("Nie poprawnie wprowadzony wybor!!!\n");
                            printf("Czy chcesz skorzystac z wyniku w nastepnym dzialaniu/ (T/N): ");
                        }
                        if((option == 'n') || (option == 't') || (option == 'N') || (option == 'T'))
                            inputState = 0;
                        else
                            inputState = 1;
                    }while(inputState);

                    if ((option == 'T') || (option == 't'))
                        memoryState = 1;
                    else
                    {
                        memoryState = 0;
                        for(i = 0; i < SIZE; i++)
                            memory[i] = 0;
                    }
                }

            }
            else
            {
                printf("Podaj liczbe: ");
                do
                {
                    load(number1, SIZE - 1);
                    checkNumber(number1, SIZE);
                    if((number1[0] == '-') && (number1[1] != '0'))
                    {
                        inputState = 1;
                        printf("Wpisano ujemna liczbe!!!\nPodaj liczbe dodatnia: ");
                    }
                    else
                        inputState = 0;
                }while(inputState);

                factorial(number1);

                if(error)
                {
                    error = 0;
                    printf("Blad!!!\n");
                    memoryState = 0;
                }

                else
                {
                    printf("Wynik: %s\n", number1);
                    printf("Czy chcesz skorzystac z wyniku w nastepnym dzialaniu? (T/N): ");

                    do
                    {
                        option = getchar();
                        while((ch = getchar()) != '\n');
                        if(inputState)
                        {
                            printf("Nie poprawnie wprowadzony wybor!!!\n");
                            printf("Czy chcesz skorzystac z wyniku w nastepnym dzialaniu? (T/N): ");
                        }
                        if((option == 'n') || (option == 't') || (option == 'N') || (option == 'T'))
                            inputState = 0;
                        else
                            inputState = 1;
                    }while(inputState);

                    if ((option == 'T') || (option == 't'))
                    {
                        memoryState = 1;
                        for (i = 0; i < SIZE; i++)
                            memory[i] = number1[i];
                    }
                    else
                    {
                        memoryState = 0;
                        for(i = 0; i < SIZE; i++)
                            memory[i] = 0;
                    }
                }
            }

        }

        else if (option != 'K')
        {
            printf("Nie ma takiej opcji, wybierz jedna z podanych");
        }

        printf("\n\n\n");

    }
}
