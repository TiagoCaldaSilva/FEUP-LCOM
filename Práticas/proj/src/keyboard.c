#include "keyboard.h"
#include "utils.h"

int keyboard_hook_id = KBD_IRQ;
const uint8_t alphabet_sc[ALPHABET_LENGTH] = { 0x1e, 0x30, 0x2e, 0x20, 0x12, 0x21, 0x22,
                                0x23, 0x17, 0x24, 0x25, 0x26, 0x32, 0x31,
                                0x18, 0x19, 0x10, 0x13, 0x1f, 0x14, 0x16,
                                0x2f, 0x11, 0x2d, 0x15, 0x2c };

int keyboard_susbcribe_int(uint8_t *bit_number){
    *bit_number  = keyboard_hook_id;
    if(sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id) != OK) return 1;
    return 0;
}

int keyboard_unsusbcribe_int(){
    if(sys_irqrmpolicy(&keyboard_hook_id) != OK) return 1;
    return 0;
}

int keyboard_read_status(uint8_t *st){
    if(proj_util_sys_inb(KBC_STATUS_REG, st) != OK) return 1;
    return 0;
}

bool keyboard_out_buffer(){
    int counter = 10;
    uint8_t st;
    do{
        if(keyboard_read_status(&st) != OK) return 1;
        if(st & KBC_OBF) return true;
        counter--;
        tickdelay(micros_to_ticks(WAIT_KBC));
    }while(counter > 0);
    return false;
}

bool keyboard_in_buffer(){
    int counter = 10;
    uint8_t st;
    do{
        if(keyboard_read_status(&st) != OK) return 1;
        if(st & KBC_IBF) return true;
        counter--;
        tickdelay(micros_to_ticks(WAIT_KBC));
    }while(counter > 0);
    return false;
}

int keyboard_interrupt_handler(uint8_t *data){
    uint8_t st;
    if(!keyboard_out_buffer()) return KBD_EMPTY_BUFFER;
    if(keyboard_read_status(&st) != OK) return KBD_INVALID_READING;
    if(proj_util_sys_inb(KBC_OUT_BUF, data) != OK) return KBD_INVALID_READING;
    //Checks if any errors have occurred
    if(st & KBC_STATUS_PARITY) return KBD_INVALID_READING;
    if(st & KBC_STATUS_TIMEOUT) return KBD_INVALID_READING;
    if(st & KBC_STATUS_AUX) return KBD_INVALID_READING;
    return KBD_VALID_READING;
}

int keyboard_recognizeScanCode(uint8_t code){
    for(int i = 0; i < ALPHABET_LENGTH; i++){
        if(alphabet_sc[i] == code) return i;
    }
    if(code == BACK_SPACE) return BACK_SPACE_KEY;
    else if(code == SPACE) return SPACE_KEY;
    else if(code == ENTER) return ENTER_KEY;
    else if(code == ESC) return ESC_KEY;
    return UNUSED_KEY;
}
