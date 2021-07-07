#include<stdio.h>

// print Celsius-Fahrenheit table

#include<stdio.h>
int main(void)
{
    float fahr, celsius;
    int lower, upper, step;
    lower = 0; // lower limit of temperature table
    upper = 300; // upper limit
    step = 20; // step size

    celsius = lower;
    while(celsius <= upper){
        fahr = (9.0/5.0) * celsius + 32;
        printf("%3.f%6.1f\n", celsius, fahr);
        celsius += step;
    }
    return 0;
}
