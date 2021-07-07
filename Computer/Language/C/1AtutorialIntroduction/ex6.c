#include<stdio.h>

// verify that the expression getchar() != EOF is 0 or 1.
// I can't verify it is 0!!! solved: input a Ctrl-d(linux) or Ctrl-z(windows)

int main()
{

    printf("%d\n", (getchar() != EOF));

}
