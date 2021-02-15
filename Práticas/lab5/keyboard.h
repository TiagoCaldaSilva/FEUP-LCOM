#include "i8042.h"

int (kbd_subscribe_int)(uint8_t *bit_no);
int (kbd_unsubscribe_int)();

int (kbd_read_status)(uint8_t *st);

int (kbd_verifyINBF)(uint8_t *st);
int(kbd_verifyOUTBUF)(uint8_t *st);

void (kbc_ih)();
