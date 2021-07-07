// count blanks, tabs, and newlines
#include<stdio.h>

int main()
{
    int c, nb, nt, nl;

    nb = nt = nl = 0;

    while((c = getchar()) != EOF){
        if(c == '\n') 
            nl++;
        if(c == '\t') 
            nt++;
        if(c == ' ') 
            nb++;
    }
    printf("blanks:%d tabs:%d newline:%d\n", nb, nt, nl);
    return 0;
}
