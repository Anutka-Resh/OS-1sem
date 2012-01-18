#include <stdio.h>

extern void mult(double, int);

extern void print(double f)
{
       printf("%lf", f);
}


int main()
{
    mult(2.71828, 17);
    return 0;
}
