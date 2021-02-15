#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"

int16_t two_comp(bool sign, uint8_t value);

int mouse_subscribe_int(uint8_t *bit_no);
int mouse_unsubscribe_int();

void (mouse_ih)();

int readStatus(uint8_t *st);
int input_buffer_clean();
int output_buffer_full();

int send_command_toMouse(uint8_t cmd);
int verifyACKbyte();
void packetAssemble(uint8_t data);
int readCommandByte(uint8_t *cmd);
int writeCommandByte();
int writeCommandByteInit(uint8_t mask);

int (timer_subscribe_int)(uint8_t *bit_no);
int (timer_unsubscribe_int)();

struct mouse_ev (mouse_detect_ev)(struct packet * pp);
void (gesture_handler)(struct mouse_ev event, uint8_t x_len, uint8_t tolerance);
