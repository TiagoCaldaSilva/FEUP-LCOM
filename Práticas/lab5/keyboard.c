#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>
#include "keyboard.h"


int hook_id_kbd = KBD_IRQ, error_raised, empty_buffer;
uint8_t data = 0;

int (kbd_subscribe_int)(uint8_t *bit_no) {
    *bit_no = hook_id_kbd;
    if(sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE ,&hook_id_kbd) != OK){
        printf("ERROR IN KEYBOARD_SUBSCRIBE_INT\n");
        return 1;
    }
    return 0;
}

int (kbd_unsubscribe_int)() {
    if(sys_irqrmpolicy(&hook_id_kbd) != OK)
    {
        printf("ERROR IN KEYBOARD_UNSUBSCRIBE_INT\n");
        return 1;
    }
    return 0;
}

int (kbd_read_status)(uint8_t *st) {
    if(util_sys_inb(STATUS_REG, st) != OK)
        return 1;
    return 0;
}

int (kbd_verifyINBF)(uint8_t *st) {
    if((*st & IN_BIT) >> 1)
        return 1;
    return 0;
}
int (kbd_verifyOUTBUF)(uint8_t *st) {
    if(!(*st & OUT_BIT))
        return 1;
    return 0;
}

void (kbc_ih)() {
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
