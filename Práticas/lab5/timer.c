#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"


int hook_id = TIMER_IRQ;
int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id; //bit_no will be used is the mask 
  return (sys_irqsetpolicy(TIMER_IRQ, IRQ_REENABLE, &hook_id));
}

int (timer_unsubscribe_int)() {
  return (sys_irqrmpolicy(&hook_id));
}

//increment the counter
int counter_timer = 0;
void (timer_int_handler)() {
  counter_timer++;
}
