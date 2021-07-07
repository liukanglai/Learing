#include<stdio.h>

// print the table in reverse order

int main(void)
{
    float fahr, celsius;
    int lower, upper, step;
    lower = 0; // lower limit of temperature table
    upper = 300; // upper limit
    step = 20; // step size

    printf("  F    C\n");
    printf("--- ------\n");

    fahr = upper;
    while(fahr >= lower){
        celsius = (5.0/9.0) * (fahr-32);
        printf("%3.f %6.1f\n", fahr, celsius);
        fahr = fahr - step; // fahr -= step;
    }
}
