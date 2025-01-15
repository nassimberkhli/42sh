#include <stdio.h>

unsigned int number_digits_rec(unsigned int n)
{
    if (n >= 10)
    {
        n = n / 10;
        return 1 + number_digits_rec(n);
    }
    return 1;
}

int my_pow(int a, int b)
{
    if (a <= 0)
    {
        return 0;
    }
    if (b == 0)
    {
        return 1;
    }
    if (b % 2 == 0)
    {
        int block = my_pow(a, b / 2);
        return block * block;
    }
    else
    {
        int block = my_pow(a, (b - 1) / 2);
        return block * block * a;
    }
}

unsigned int digit(int n, int k)
{
    if (k <= 0)
    {
        return 0;
    }
    if (n <= 0)
    {
        return 0;
    }
    n = n / my_pow(10, k - 1);
    n = n % 10;
    return n;
}
int my_strlen(const char *str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

char *my_itoa(int value, char *s)
{
    int signe = 0;
    if (value < 0)
    {
        signe++;
        value = value * -1;
    }
    int len = number_digits_rec(value);
    if (signe == 0)
    {
        for (int i = 0; i < len; i++)
        {
            s[i] = digit(value, len - i) + '0';
        }
        s[len] = '\0';
    }
    else
    {
        s[0] = '-';
        for (int i = 0; i < len; i++)
        {
            s[i + 1] = digit(value, len - i) + '0';
        }
        s[len + 1] = '\0';
    }
    return s;
}
