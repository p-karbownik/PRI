#include"calc.h"

void load(char *z, int amount)
{
    char *result;
    int i = 0;
    result = fgets(z, amount, stdin);
    if(result)
    {
        while (z[i] != '\n' && z[i] != '\0')
            i++;
        if(z[i] == '\n')
            z[i] = '\0';
        else
            while (getchar() != '\n')
                continue;
    }
}

void addSign(char *tab, int size)
{
    char temp[size];
    int i = 0;
    temp[0] = 0;
    for(i = 1; i < size; i++)
        temp[i] = tab[i - 1];
    temp[0] = '+';
    for(i = 0; i < size; i++)
        tab[i] = temp[i];
    if(tab[SIZE - 1] != 0)
    {
        printf("Podano za duza liczbe!!!\n");
        printf("Podaj nowa liczbe: ");
        load(tab, SIZE - 1);
        return;
    }
}

void resetArray (char *array, int size)
{
    int i = 0;
    for (i = 0; i < size; i++)
        array[i] = 0;
}

void checkNumber(char *number, int size)
{
    if(number[0] == 0)
    {
        printf("Nie podano liczby!!!\n");
        printf("Podaj ponownie liczbe: ");
        resetArray(number, SIZE);
        load(number, SIZE - 1);
        checkNumber(number, size);
    }
    else if(!((number[0] == '+') || (number[0] == '-') || ((number[0] >= '0') && (number[0] <= '9'))))
    {
        printf("Podana liczba zawiera niepoprawne znaki!!!\n");
        printf("Podaj ponownie liczbe: ");
        resetArray(number, SIZE);
        load(number, SIZE - 1);
        checkNumber(number, size);
    }
    else
    {
        int i;
        for (i = 1; i < size; i++)
        {
            if((number[i] >= '0') && (number[i] <= '9'))
                continue;
            else if(number[i] == 0)
                break;
            else
            {
                printf("Podana liczba zawiera niepoprawne znaki!!!\n");
                printf("Podaj ponownie liczbe: ");
                resetArray(number, SIZE);
                load(number, SIZE - 1);
                checkNumber(number, size);
            }
        }
    }
    if((number[0] >= '0') && (number[0] <= '9') && (number[0] != '-'))
        addSign(number, size);

    deleteZeros(number);
}

void reverseArray(char *tab, int length)
{
    char temp;
    int i;
    int lastIndex = length - 1;
    for(i = 0; i < lastIndex; i++, lastIndex--)
    {
        temp = tab[lastIndex];
        tab[lastIndex] = tab[i];
        tab[i] = temp;
    }
}

int isBigger(char *tab1, char *tab2)
{
    int number1_length = strlen(tab1);
    int number2_length = strlen(tab2);

    if(number1_length > number2_length)
        return 1;
    if(number1_length < number2_length)
        return 0;
    if(number1_length == number2_length)
    {
        int i = 1;
        int a_tab1;
        int b_tab2;
        for(i = 1; i < number1_length; i++)
        {
            a_tab1 = (int)(tab1[i] - '0');
            b_tab2 = (int)(tab2[i] - '0');
            if(a_tab1 == b_tab2)
                continue;
            if(a_tab1 > b_tab2)
                return 1;
            if(b_tab2 > a_tab1)
                return 0;
        }
        return 2;
    }
}

void swap(char *a, char *b)
{
    char c = 0;
    int i = 0;
    for(i = 0; i < SIZE; i++)
    {
        c = a[i];
        a[i] = b[i];
        b[i] = c;
    }
}

void subtraction(char number1[], char number2[])
{
    int number2_length = strlen(number2);
    int number1_length = strlen(number1);
    int instance = isBigger(number1, number2);
    char sign = number2[0];
    if(instance == 2)
    {
        int i = 2;
        number1[0] = sign;
        number1[1] = '0';
        for(i = 2; i < number1_length; i++)
        {
            number2[i] = 0;
        }
        return;
    }
    if(instance == 1)
    {
        number1[0] = 0;
        number2[0] = 0;

        reverseArray(number1, number1_length);
        reverseArray(number2, number2_length);

        int a_tab2;
        int b_tab1;
        int r = 0;
        int i = 0;


        for(i = 0; i < number1_length; i++)
        {
            a_tab2 = (int)(number1[i] - '0');
            b_tab1 = (int)(number2[i] - '0');

            if(number2[i] == 0)
                b_tab1 = 0;

            if(number1[i] == 0)
                a_tab2 = 0;

            a_tab2 -= r;
            r = 0;

            if(a_tab2 > b_tab1)
            {
                a_tab2 -= b_tab1;
                number1[i] = (char)(a_tab2 + '0');
            }

            else if(a_tab2 < b_tab1)
            {
                r = 1;
                a_tab2 += 10;
                a_tab2 -= b_tab1;
                number1[i] = (char)(a_tab2 + '0');
            }

            else if(a_tab2 == b_tab1)
            {
                if(number1[i + 1] != 0)
                {
                    number1[i] = '0';
                }
                else
                {
                    number1[i] = 0;
                }
            }
        }
        number1_length = strlen(number1) + 1;
        reverseArray(number1, number1_length);
        number1[0] = sign;
    }
    if(instance == 0)
    {
        if(number2[0] == '+')
        {
            number2[0] = '-';
            number1[0] = '-';
        }
        else
        {
            number2[0] = '+';
            number1[0] = '-';
        }
        subtraction(number2, number1);
        swap(number2, number1);
    }
}

void multiply(char number1[], char number2[])
{
    char sign;
    if(number1[0] == number2[0])
        sign = '+';
    else
        sign = '-';

    int number1_length = strlen(number1);
    int number2_length = strlen(number2);
    if (number1_length + 1 >= SIZE)
    {
        error = 1;
        return;
    }
    if (number2_length + 1 >= SIZE)
    {
        error = 1;
        return;
    }
    number1[0] = 0;
    number2[0] = 0;
    reverseArray(number1, number1_length);
    reverseArray(number2, number2_length);

    char temp[72];

    int i = 0;
    for(i = 0; i < 72; i++)
        temp[i] = 0;
    int j = 0;

    int a_2 = 0;
    int b_1= 0;
    int c_temp = 0;
    int r = 0;

    for(i = 0; i < number2_length - 1; i++)
    {
        if(number2[i] == 0)
            a_2 = 0;
        else
            a_2 = (int)(number2[i] - '0');
        for(j = 0; j < number1_length - 1; j++)
        {
            if(number1[j] == 0)
                b_1 = 0;
            else
                b_1 = (int)(number1[j] - '0');
            if(temp[j + i] == 0)
                c_temp = 0;
            else
                c_temp = (int)(temp[j + i] - '0');
            c_temp += a_2 * b_1;
            if(c_temp > 9)
            {
                r = c_temp / 10;
                c_temp %= 10;
                temp[j+i] = (char)(c_temp + '0');
            }
            else
            {
                temp[j+i] = (char)(c_temp + '0');
            }
            if((r != 0) && ((i + j + 1) >= (SIZE - 1)))
            {
                printf("Przekroczono zakres!!!\n");
                for(i = 0; i < SIZE; i++)
                {
                    number1[i] = 0;
                    number2[i] = 0;
                }
                error = 1;
                return;
            }
            if(r != 0)
            {
                if(temp[i + j + 1] == 0)
                    temp[i + j + 1] = (char)(r + '0');
                else
                {
                    int x = (int)(temp[i + j + 1] - '0');
                    x += r;
                    temp[i + j + 1] = (char)(x + '0');
                }
                r = 0;
            }
        }
    }
    for(i = 0; ; i++)
        if(temp[i] == 0)
        {
            temp[i] = sign;
            break;
        }
    for(i = 0; temp[i] != sign; i++);
    for(j = 0; i >= 0; j++, i--)
        number1[j] = temp[i];
}

void factorial(char number[])
{
    char temp[72];
    int i = 0;
    for(i = 0; i < 72; i++)
        temp[i] = number[i];
    char temp2[72];
    for(i = 0; i < 72; i++)
        temp2[i] = temp[i];
    if(( (number[0] == '-') || (number[0] == '+')) && (number[1] == '0') && (number[2] == 0))
    {
        number[0] = '+';
        number[1] = '1';
        number[2] = 0;
        return;
    }
    while(!(isBigger(temp, "+1") == 2))
    {
        if(error)
            return;
        char one[4] = "+1";
        subtraction(temp, one);
        for(i = 0; i < 72; i++)
            temp2[i] = temp[i];
        multiply(number, temp2);
        number[0] = '+';

    }
}

void addition(char *num1, char *num2)
{
    char sign;

    sign = num1[0];
    int number1_length = strlen(num1);
    int number2_length = strlen(num2);

    num1[0] = 0;
    num2[0] = 0;
    reverseArray(num1, number1_length);
    reverseArray(num2, number2_length);
    if(number1_length > number2_length)
        number2_length = number1_length;

    int i = 0;
    int r = 0;

    for(i = 0; i < number2_length; i++)
    {
        if ((i == (SIZE - 2)) && (r == 1))
        {
            printf("Przekroczono zakres!!!\n");
            error = 1;
            for(i = 0; i < SIZE; i++)
            {
                num1[i] = 0;
                num2[i] = 0;
            }
        }
        int a_tab1, b_tab2;
        if(num1[i] != 0)
            a_tab1 = (int)(num1[i] - '0');
        else
            a_tab1 = 0;
        if(num2[i] != 0)
            b_tab2 = (int)(num2[i] - '0');
        else
            b_tab2 = 0;
        b_tab2 += r;
        r = 0;
        if((a_tab1 + b_tab2) > 9)
        {
            r = 1;
            num2[i] = (char)(((a_tab1 + b_tab2) % 10) + '0');
        }
        else
        {
            num2[i] = (char)(a_tab1 + b_tab2 + '0');
        }
    }
    number2_length = strlen(num2) + 1;
    reverseArray(num2, number2_length);
    deleteZeros(num2);
    num2[0] = sign;
}

void deleteZeros(char number[])
{
    int i = 0;
    int j = 0;
    int isZero = 1;
    if(number[1] != '0')
        return;
    for(i = 1; number[i] != 0; i++)
    {
        if((number[i] >= '1') && (number[i] <= '9'))
        {
            isZero = 0;
            break;
        }
    }
    if(isZero)
    {
        number[0] = '+';
        number[1] = '0';
        for(i = 2; i < SIZE; i++)
            number[i] = 0;
    }
    else
    {
        for(i = 1; number[i] == '0'; i++);
        char temp[SIZE];
        for(j = 0; j < SIZE; j++)
            temp[j] = 0;
        temp[0] = number[0];
        for(j = 1; number[i] != 0; j++, i++)
            temp[j] = number[i];
        for(i = 1; i < SIZE; i++)
            number[i] = temp[i];
    }
}