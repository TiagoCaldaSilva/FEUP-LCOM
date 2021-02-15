#include "timer.h"

int timer_hook_id = TIMER_IRQ;

int (timer_subscribe_int)(uint8_t * bit_number){
    *bit_number = timer_hook_id;
    if(sys_irqsetpolicy(TIMER_IRQ, IRQ_REENABLE, &timer_hook_id) != OK) return 1;
    return 0;
}

int (timer_unsubscribe_int)(){
    if(sys_irqrmpolicy(&timer_hook_id) != OK) return 1;
    return 0;
}


int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t rbCommand = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));

  if (sys_outb(TIMER_CTRL, rbCommand) != OK || proj_util_sys_inb(TIMER_0 + timer, st) != OK)  return 1; 
  return 0;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
	uint8_t curr_conf; 
	if (freq > TIMER_FREQ || freq < 19 || timer_get_conf(timer, &curr_conf)) { return 1; }
	uint8_t new_conf = (curr_conf & CONFIG_MASK) | TIMER_LSB_MSB;

	// freq_factor (number of clock cycles in a second)
	uint16_t freq_factor = TIMER_FREQ / freq;

	//Get the corresponding value to initialize the timer
	uint8_t lsb = freq_factor & LSB_MASK, msb = (freq_factor & MSB_MASK) >> 8;

	//Pass to the control word the new configuration and initialize the timer with the new values
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
	if (sys_outb(TIMER_CTRL, new_conf) != OK || sys_outb(TIMER_0 + timer, lsb) != OK || sys_outb(TIMER_0 + timer, msb)) return 1;
	return 0;
}
