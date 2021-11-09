#include <stdio.h>
int main(void) {
  typedef struct person {
    int age;
    int sex;
  } person1;

  person1 *me;
  me = NULL;

  printf("%p\n%p\n", &me->age, &me->sex);

  return 0;
}
