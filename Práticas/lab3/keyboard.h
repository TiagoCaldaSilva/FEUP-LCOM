#include "i8042.h"

int (kbd_subscribe_int)(uint8_t *bit_no);

int (kbd_unsubscribe_int)();

int (kbd_read_status)(uint8_t *st);

int (kbd_verifyINBF)(uint8_t *st);

int(kbd_verifyOUTBUF)(uint8_t *st);

void (keyboard_interrupt_handler)();

int (keyboard_get_command_byte)(uint8_t *cmd);

int (keyboard_write_command_byte)(uint8_t cmd);

int (timer_subscribe_int)(uint8_t *bit_no);

int (timer_unsubscribe_int)();

void(timer_int_handler)();

void(kbc_ih)();
