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
struct birthday *person[5]; // can't put in simple_init, why?

/* This function is called when the module is loaded. */
int simple_init(void) {
  printk(KERN_INFO "Loading Module\n");

  // The following code can't put outside, why?
  int i = 4;
  while (i--) {
    person[i] = kmalloc(sizeof(*person[i]), GFP_KERNEL);
    person[i]->day = 2;
    person[i]->month = 8;
    person[i]->year = 1995;
  }
  INIT_LIST_HEAD(&person[0]->list);

  while (++i < 5) {
    list_add_tail(&person[i]->list, &birthday_list);
  }

  list_for_each_entry(person[0], &birthday_list, list) {
    printk(KERN_INFO "day: %d, month: %d, year: %d\n", person[0]->day,
           person[0]->month, person[0]->year);
  }

  return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
  list_for_each_entry_safe(person[0], person[1], &birthday_list, list) {
    list_del(&person[0]->list);
    kfree(person[0]);
  }

  printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
