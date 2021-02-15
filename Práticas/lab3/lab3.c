#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "keyboard.h"

extern int cnt, empty_buffer, error_raised, timer_counter;
extern uint8_t data;

int main(int argc, char *argv[]) {

  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("C:/Users/user/Desktop/FEUP/2_Ano/1_Semestre/LCOM/shared/g09/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("C:/Users/user/Desktop/FEUP/2_Ano/1_Semestre/LCOM/shared/g09/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  uint8_t bit_no;
  message msg;
  int ipc_status, r;
  if(kbd_subscribe_int(&bit_no) != OK)
    return 1;
  uint32_t irq_set = BIT(bit_no); //Identifier used to comunicate kernel about an interrupt

  while(data != ESC_BREAKCODE)
  {
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r); continue;
    }
    if(is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set){
            (keyboard_interrupt_handler)();
            if(error_raised != 0){
              return 1;
            }
            else if(empty_buffer != 0){
              break;
            }
            else{
              uint8_t pack[2];
              int length = 0;
              pack[length++] = data;
              if(data == TWO_BYTES_CODE){
              (keyboard_interrupt_handler)();
              if(error_raised != 0)
                return 1;
                pack[length] = data;
              }
              bool make = true;
              if((data & BREAK_BIT) >> 7)
                make = false;
                kbd_print_scancode(make, length, pack);
                }
              }
              break;
        default:
          break;
      }

    }
    else{}
  }
  if(kbd_unsubscribe_int() != OK)
    return 1;
  kbd_print_no_sysinb(cnt);
  return 0;
}

int(kbd_test_poll)() {
  while(data != ESC_BREAKCODE)
  {
    (keyboard_interrupt_handler)();
    if(error_raised != 0)
      return 1;
    else if(empty_buffer)
      tickdelay(micros_to_ticks(WAIT_KBC));
    else{
      uint8_t pack[2];
      int length = 0;
      pack[length++] = data;
      if(data == TWO_BYTES_CODE){
        (keyboard_interrupt_handler)();
        if(error_raised != 0)
          return 1;
        pack[length++] = data;
      }
      bool make = true;
      if((data & BREAK_BIT) >> 7)
        make = false;
      kbd_print_scancode(make, length, pack);
      }
  }
  uint8_t cmd;
  if((keyboard_get_command_byte)(&cmd) != OK)
      return 1;
  if((keyboard_write_command_byte)(cmd & ENABLE_BIT) != OK)
      return 1;
  kbd_print_no_sysinb(cnt);
  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  uint8_t timer_bit_no, kbd_bit_no;
  message msg;
  int r, ipc_status;
  if((timer_subscribe_int)(&timer_bit_no) != OK)
    return 1;
  if((kbd_subscribe_int)(&kbd_bit_no) != OK)
    return 1;
  uint32_t timer_irq_set = BIT(timer_bit_no), kbd_irq_set = BIT(kbd_bit_no);
  while(data != ESC_BREAKCODE && timer_counter < (n * 60))
  {
    if((r = driver_receive(ANY, &msg, &ipc_status)) != OK){
      printf("driver_receive failed with: %d", r); continue;
    }
    if(is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          if (msg.m_notify.interrupts & kbd_irq_set){
            timer_counter = 0;
            kbc_ih();
            if(error_raised != 0)
              return 1;
            else if(empty_buffer != 0)
              break;
            uint8_t pack[2];
            int length = 0;
            pack[length++] = data;
            if(data == TWO_BYTES_CODE){
              kbc_ih();
              if(error_raised != 0)
                return 1;
              pack[length++] = data;
            }
            bool make = true;
            if((data & BREAK_BIT) >> 7)
              make = false;
            kbd_print_scancode(make, length, pack);
          }
          if(msg.m_notify.interrupts & timer_irq_set){
            timer_int_handler();
            if((timer_counter % 60) == 0)
              timer_print_elapsed_time();
          }
          break;
        default:
          break;
      }
    }else{}
  }

  if((timer_unsubscribe_int)() != OK)
    return 1;
  if((kbd_unsubscribe_int)() != OK)
    return 1;
  kbd_print_no_sysinb(cnt);
  return 0;
}
