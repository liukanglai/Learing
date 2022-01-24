// Time:2021-12-02 15:03:08
#include <stdio.h>
#include <string.h>

int main(void) {
  int a[2][2][2] = {1, 2, 3, 4, 5, 6, 7, 8};
  printf("%d", ***(a + 1));
  return 0;
}
