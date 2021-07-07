#include<stdio.h>

// print Fahrenheit-Celsius table for fahr = 0, 20, ..., 300

int main(void)
{
    int fahr, celsius;
    int lower, upper, step;
    lower = 0; // lower limit of temperature table
    upper = 300; // upper limit
    step = 20; // step size

    fahr = lower;
    while(fahr <= upper){
        celsius = 5 * (fahr - 32) / 9;
        printf("%d\t%d\n", fahr, celsius);
        fahr = fahr + step; // fahr += step;
    }
}

/* 

//floating-point version:

main(void)
{
    float fahr, celsius;
    int lower, upper, step;
    lower = 0; // lower limit of temperature table
    upper = 300; // upper limit
    step = 20; // step size

    fahr = lower;
    while(fahr <= upper){
        celsius = (5.0/9.0) * (fahr-32);
        printf("%3.f%6.1f\n", fahr, celsius);
        fahr = fahr + step; // fahr += step;
    }
}

//loop version:

main()
{
    int fahr;

    for(fahr = 0; fahr <= 300; fahr += 20){
        printf("%3d %6.1f\n", fahr, (5.0 / 9.0) * (fahr -32));
    }
}


// avoid "magic numbers", use Symbolic constants(usually use upper case):

#define LOWER 0
#define UPPER 300
#define STEP 20

main()
{
    int fahr;

    for(fahr = LOWER; fahr <= UPPER; fahr += STEP){
        printf("%3d %6.1f\n", fahr, (5.0 / 9.0) * (fahr -32));
    }
}



*/
