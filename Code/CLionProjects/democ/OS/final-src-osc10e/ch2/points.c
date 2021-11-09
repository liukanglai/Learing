#include <stdio.h>
#include <stdlib.h>

struct birthday {
  int day;
};

void assign(struct birthday *me, int day) { me->day = day; }

int main(void) {
  struct birthday *me;
  me = malloc(sizeof(*me));
  assign(me, 1);
  printf("%d", me->day);
  return 0;
}
