// copy its input to its output, replacing each string of one or more blanks by a single blank.
#include<stdio.h>

int main(void)
{
    int c, preblank = 0;

    while((c = getchar()) != EOF){
        if(c == ' ' || c == '\t'){ // \t...
            if(!preblank){
                putchar(' ');
                preblank = 1;
            }
        }
        else{
            preblank = 0;
            putchar(c);
        } 

    }
    return 0;
}

