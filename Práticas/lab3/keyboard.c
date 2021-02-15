#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>
#include "keyboard.h"

int hook_id = KBD_IRQ, error_raised, empty_buffer, timer_counter = 0, timer_hook_id = TIMER_IRQ;
uint8_t data = 0;

int (kbd_subscribe_int)(uint8_t *bit_no)
{
    *bit_no = hook_id;
    if(sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE ,&hook_id) != OK){
        printf("ERROR IN KEYBOARD_SUBSCRIBE_INT\n");
        return 1;
    }
    return 0;
}

int (kbd_unsubscribe_int)()
{
    if(sys_irqrmpolicy(&hook_id) != OK)
    {
        printf("ERROR IN KEYBOARD_UNSUBSCRIBE_INT\n");
        return 1;
    }
    return 0;
}

int (kbd_read_status)(uint8_t *st){
    if(util_sys_inb(STATUS_REG, st) != OK)
        return 1;
    return 0;
}

int (kbd_verifyINBF)(uint8_t *st)
{
    if((*st & IN_BIT) >> 1)
        return 1;
    return 0;
}

int (kbd_verifyOUTBUF)(uint8_t *st)
{
    if(!(*st & OUT_BIT))
        return 1;
    return 0;
}

void (keyboard_interrupt_handler)()
{
    error_raised = 0;
    empty_buffer = 0;
    uint8_t st;
    if((kbd_read_status)(&st) != OK)
        error_raised++;
    else if((kbd_verifyOUTBUF)(&st) == OK)
    {
        if(util_sys_inb(OUT_BUF, &data) != OK)
            error_raised++;
        else if((st & PARITY_ERROR) >> 7)
            error_raised++;
        else if((st & TIMEOUT_ERROR) >> 6)
            error_raised++;
        else if((st & AUX_ERROR) >> 5)
            error_raised++;
    }
    else{
        empty_buffer++;
    }
}

int (keyboard_get_command_byte)(uint8_t *cmd)
{
    uint8_t st;
    int counter = 11;
    do{
        if((kbd_read_status)(&st) != OK)
            return 1;
        counter--;
    }while((kbd_verifyINBF)(&st) != OK && (counter > 0));
    if(counter == 0)
        return 1;
    if(sys_outb(IN_BUF, READ_CMD) != OK)
        return 1;

    if((kbd_read_status)(&st) != OK)
        return 1;
    while((kbd_verifyOUTBUF)(&st) != OK)
    {
        if((kbd_read_status)(&st) != OK)
            return 1;
        tickdelay(micros_to_ticks(WAIT_KBC));
    }

    if(util_sys_inb(OUT_BUF, cmd) != OK)
        return 1;
    return 0;
}

int (keyboard_write_command_byte)(uint8_t cmd)
{
    uint8_t st;
    int counter = 11;
    do{
        if((kbd_read_status)(&st) != OK)
            return 1;
        counter--;
    }while((kbd_verifyINBF)(&st) != OK && (counter > 0));
    if(counter == 0)
        return 1;
    if(sys_outb(IN_BUF, WRITE_CMD) != OK)
        return 1;

    if((kbd_read_status)(&st) != OK)
        return 1;

    if(sys_outb(OUT_BUF, cmd) != OK)
        return 1;
    return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no)
{
    *bit_no = timer_hook_id;
    if(sys_irqsetpolicy(TIMER_IRQ, IRQ_REENABLE, &timer_hook_id) != OK)
        return 1;
    return 0;
}

int (timer_unsubscribe_int)()
{
    if(sys_irqrmpolicy(&timer_hook_id) != OK)
        return 1;
    return 0;
}

void(timer_int_handler)()
{
    timer_counter++;
}

void (kbc_ih)()
{
    error_raised = 0;
    empty_buffer = 0;
    uint8_t st;
    if((kbd_read_status)(&st) != OK)
        error_raised++;
    else if((kbd_verifyOUTBUF)(&st) == OK)
    {
        if(util_sys_inb(OUT_BUF, &data) != OK)
            error_raised++;
        else if((st & PARITY_ERROR) >> 7)
            error_raised++;
        else if((st & TIMEOUT_ERROR) >> 6)
            error_raised++;
        else if((st & AUX_ERROR) >> 5)
            error_raised++;
    }
    else{
        empty_buffer++;
    }
}
