#include "mouse.h"
#include "utils.h"

int mouse_hook_id = MOUSE_IRQ;

Sprite *mouse_cursor;
extern vbe_mode_info_t vbe_info;
extern uint32_t* double_buffer;
uint16_t mouse_prev_x = MOUSE_DEFAULT_X, mouse_prev_y = MOUSE_DEFAULT_Y;
uint16_t prev_data;
bool lb_pressed = 0, rb_pressed = 0;

int mouse_subscribe_int(uint8_t * bit_number){
    *bit_number = mouse_hook_id;
    if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != OK) return 1;
    return 0;
}

int mouse_unsubscribe_int(){
    if(sys_irqrmpolicy(&mouse_hook_id) != OK) return 1;
    return 0;
}

int mouse_output_buffer_flush(){ //Used to clear the output buffer in the end of the driver receive while
    uint8_t out_buffer_garbage_byte;
    while(mouse_out_buffer() != MOUSE_EMPTY_BUFFER){
        if(proj_util_sys_inb(KBC_OUT_BUF, &out_buffer_garbage_byte) != OK) return 1;
    }
    return 0;
}

int mouse_read_status(uint8_t *st){
    if (proj_util_sys_inb(KBC_STATUS_REG, st) != OK) return 1;
    else return 0;
}

int mouse_out_buffer(){
    int counter = 10;
    uint8_t st = 0x00;
    do {
        if(mouse_read_status(&st) != OK) return MOUSE_INVALID_READING;
        if (st & KBC_OBF) return MOUSE_VALID_READING;
        counter--;
        tickdelay(micros_to_ticks(WAIT_KBC));
    } while(counter > 0);
    return MOUSE_EMPTY_BUFFER;
}

int mouse_in_buffer(){
    int counter = 10;
    uint8_t st;
    do {
        if(mouse_read_status(&st) != OK) return MOUSE_INVALID_READING;
        if(st & KBC_IBF) return MOUSE_VALID_READING;
        counter--;
        tickdelay(micros_to_ticks(WAIT_KBC));
    } while(counter > 0);
    return MOUSE_EMPTY_BUFFER;
}

int mouse_send_command(uint8_t cmd){
    uint8_t ack_byte;
    do {
        if(mouse_in_buffer() != MOUSE_EMPTY_BUFFER) return 1;
        if(sys_outb(KBC_IN_BUF, WRITE_BYTE_MOUSE) != OK) return 1;
        if(sys_outb(KBC_OUT_BUF, cmd) != OK) return 1;

        switch(mouse_out_buffer()) {
            case MOUSE_VALID_READING:
                if(proj_util_sys_inb(KBC_OUT_BUF, &ack_byte) != OK) return 1;
                break;
            case MOUSE_INVALID_READING:
                return 1;
            case MOUSE_EMPTY_BUFFER:
                break;

            default:
                break;
        }
    } while(ack_byte != ACK);   //The cicle only ends when the acknowledgment byte was received
    return 0;
}

int mouse_interrupt_handler(uint8_t *data){
    uint8_t st;
    if(mouse_read_status(&st) != OK) return MOUSE_INVALID_READING;
	if (!(st & KBC_OBF)) return MOUSE_EMPTY_BUFFER;
	if(proj_util_sys_inb(KBC_OUT_BUF, data) != OK) return MOUSE_INVALID_READING;
    if(st & (KBC_STATUS_PARITY | KBC_STATUS_TIMEOUT)) return MOUSE_INVALID_READING; //Checks if any errors have occurred
    return MOUSE_VALID_READING;
}

int mouse_packet_assemble(int *pCounter, struct packet *p, uint8_t *data){ //Processes the data given by the mouse
    switch(*pCounter){
        case 0:
            if(!(*data & BIT(3))) return 0;
            p->lb = *data & LEFT_BUTTON;
            p->rb = *data & RIGHT_BUTTON;
            p->mb = *data & MIDDLE_BUTTON;
            p->x_ov = *data & X_OV;
            p->y_ov = *data & Y_OV;
            prev_data = *data;
            break;
        case 1:
            p->delta_x = (prev_data & MSB_X) ? (signal_mask | *data) : *data;
            break;
        case 2:
            p->delta_y = (prev_data & MSB_Y) ? (signal_mask | *data) : *data;
            break;
    }
    p->bytes[*pCounter] = *data;
    *pCounter = (*pCounter + 1);
    return 0;
}

void move_cursor(struct packet *pp, char *buffer){ //Processes the data given by the mouse, moves the respective sprite
    mouse_prev_x = mouse_cursor->x; mouse_prev_y = mouse_cursor->y;
    int16_t final_x = mouse_cursor->x + pp->delta_x, final_y = mouse_cursor->y - pp->delta_y;
    if (final_x + mouse_cursor->width > vbe_info.XResolution){ final_x = vbe_info.XResolution - mouse_cursor->width;}
    if (final_x < 0) final_x = 0;
    if (final_y + mouse_cursor->height > vbe_info.YResolution){ final_y = vbe_info.YResolution - mouse_cursor->height;}
    if (final_y < 0) final_y = 0;

    erase_sprite(mouse_cursor, buffer); //Delete the sprite from the past position
    mouse_cursor->x = final_x;
    mouse_cursor->y = final_y;
    draw_sprite(mouse_cursor, buffer);  //Draws the sprite in the actual position
}

int detect_event(struct packet *pp){    //Function that detects the events in order to use that information in the menus state machines
    if( pp->lb && !pp->rb && !pp->mb){
        lb_pressed = true;
        return L_PRESSED;
    }
    else if(!pp->lb && pp->rb && !pp->mb){
	    rb_pressed = true;
        return R_PRESSED;
    }
    else if(!pp->lb && !pp->rb && !pp->mb && pp->delta_x == 0 && pp->delta_y == 0){
        if(lb_pressed){
            lb_pressed = false;
            return L_RELEASED;
        }
        else if(rb_pressed){
            rb_pressed = 1;
            return R_RELEASED;
        }
        else return BUTTON_EVENT;
    }
    else if(pp->delta_x != 0 || pp->delta_y != 0) return MOUSE_MOVE;
    else{
        lb_pressed = 0;
        rb_pressed = 0;
        return BUTTON_EVENT;
    }
}
