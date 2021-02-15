#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>


#include "i8254.h"

#define mask 0x0F //mask for the first 4 less significant bits

int counter = 0;//, hook_id = TIMER0_IRQ;

//definir uma nova control_word e definir a frequência do timer
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  // freq must be higher that 18.2065 Hz -> ceil(18.2065) = 19
  // more details in https://wiki.osdev.org/Programmable_Interval_Timer
  uint8_t curr_conf; 
  if (freq > TIMER_FREQ || freq < 19 || timer_get_conf(timer, &curr_conf)) { return 1; }

  // We must maintain the 4 last bits, which provide info on operating and counting mode (BCD or Binary)
  uint8_t new_conf = (curr_conf & mask) | TIMER_LSB_MSB;

  // freq_factor (number of clock cycles in a second)
  uint16_t freq_factor = TIMER_FREQ / freq;

  //obter os valores correspondentes para inicializar o timer
  uint8_t lsb, msb;
  util_get_LSB(freq_factor, &lsb);
  util_get_MSB(freq_factor, &msb);

  //passar para a control word a nova configuração e inicializar o timer com os novos valores
  switch(timer) {
    case 0:
      new_conf |= TIMER_SEL0;
      break;
    case 1:
      new_conf |= TIMER_SEL1;
      break;
    case 2:
      new_conf |= TIMER_SEL2;
      break;
    default:
      return 1;
  }
  if(sys_outb(TIMER_CTRL, new_conf)|| sys_outb(TIMER_0 + timer, lsb) || sys_outb(TIMER_0 + timer, msb)){
    printf("LSB OR MSB FAILED\n");
  }
  return 0;
}


//subscrive a notification on every interrupt
int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id; //bit_no will be used is the mask 
  printf("Before setpolicy: ox%x\n", hook_id);
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)){
    printf("Sys_irqsetpolicy failed\n");
    return 0;
  }
  printf("After setpolicy: 0x%x\n", hook_id);
  return 1;
}


int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hook_id)){
    printf("Sys_irqrmpolicy failed\n");
    return 1;
  }
  return 0;
}


//increment the counter
void (timer_int_handler)() {
  counter++;
}


int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  uint8_t rbCommand = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));
  if (sys_outb(TIMER_CTRL, rbCommand) || util_sys_inb(TIMER_0 + timer, st)) { return 1; }

  return 0;
}


//field é uma variável que quer tomar vários valores
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  union timer_status_field_val conf;

  switch (field) {
    case tsf_all: //quer saber toda a informação sobre configuração do timer
      conf.byte = st; break;

    case tsf_initial: //apenas a forma que ele inicia (LSB ou MSB ou ambas)
      conf.in_mode = (st & TIMER_LSB_MSB) >> 4; break; // checking bits no. 4 and 5

    case tsf_mode: //apenas quer saber o mode
      st = (st & (BIT(3) | TIMER_SQR_WAVE)) >> 1; // checking bits no. 1, 2 and 3
      
      // We need to ensure compatibility with Intel Products
      // more details in: https://web.fe.up.pt/~pfs/aulas/lcom2020/labs/lab2/lab2_03.html#tab1
      
      if (st == 0x06 || st == 0x07) { st -= 0x04; }
      conf.count_mode = st; break;

    case tsf_base: //se está em binário ou bcd
      conf.bcd = st & TIMER_BCD; break; // checking bit no. 0

    default:
      return 1;
  }

  if (timer_print_config(timer, field, conf)) { return 1; }
  return 0;
}
