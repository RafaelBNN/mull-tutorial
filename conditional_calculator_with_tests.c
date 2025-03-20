#include <stdio.h>

int compute(int num1, int num2)
{
    if (num1 >= num2)
        return num1 - num2;
    return num1 + num2;
}

int main()
{
    if (compute(3,5) != 8)
        return 1; // test failed

    if (compute(0,0) != 0)
        return 1; // test failed

    return 0; // all tests passed for that mutant
}