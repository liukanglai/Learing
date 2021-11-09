/**
 * simple.c
 *
 * A simple kernel module.
 *
 * To compile, run makefile by entering "make"
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

// A good reference: https://www.cnblogs.com/yangguang-it/p/11667772.html

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/list.h>  // link list
#include <linux/slab.h>  // for kmalloc
#include <linux/types.h> // list_head

struct birthday {
  int day;
  int month;
  int year;
  struct list_head list;
};

static LIST_HEAD(birthday_list);
/*
 * will create a list_head, in it: prev->next, next->prev.
 * same as struct birthday person, than INIT_LIST_HEAD(&person.list),
 * you can see, it's more easy.
 */
struct birthday *person; // can't put in simple_init, why?

struct birthday *assign_birthday(struct birthday *person, int day, int month,
                                 int year) {
  person = kmalloc(sizeof(*person),
                   GFP_KERNEL); // 这出错好久，注意不能返回空，赋值的这个 person
                                // 非那个person；若需，可在外部 malloc,
                                // 可要定义多个，只能在内
  person->day = day;
  person->month = month;
  person->year = year;
  return person;
}

/* This function is called when the module is loaded. */
int simple_init(void) {
  printk(KERN_INFO "Loading Module\n");

  int i = -1;
  while (++i < 5) {
    person = assign_birthday(person, 2 + i, 8 + i, 1995 + i);
    list_add_tail(&person->list,
                  &birthday_list); // to insert in the end, and conversely
                                   // list_add() is in head.
                                   // }
  }
  /*
  person = assign_birthday(person, 2, 8, 1995);
  person = kmalloc(sizeof(*person), GFP_KERNEL);
  person->day = 2;
  person->month = 8;
  person->year = 1995;
  INIT_LIST_HEAD(&person->list); // is redundance?
  */

  list_for_each_entry(
      person, &birthday_list,
      list) { // this person will assign in list_entry, so no need to define,
              // just a signal. 他怎么认识 list 啊，I define it in struct?
    printk(KERN_INFO "day: %d, month: %d, year: %d\n", person->day,
           person->month, person->year);
  }

  return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
  struct birthday *ptr;
  struct birthday *next;

  list_for_each_entry_safe(
      ptr, next, &birthday_list,
      list) { // as same, the ptr, next will assign inner...
    list_del(&ptr->list);
    kfree(ptr);
  }

  printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
