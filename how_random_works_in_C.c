#include <stdio.h>
#include <math.h> // compile with -lm

double m=pow(2.0,31.0);
double a =1103515245;
double c=12345;

double random(double x)
{
    return (x*a + c)%m;
}

int main()
{

}
