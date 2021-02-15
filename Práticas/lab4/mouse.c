#include "mouse.h"

uint8_t data;
int error_raised, hook_id, timer_hook_id, empty_buffer;
uint8_t timer_counter = 0;
extern int pCounter;
extern struct packet p;

int16_t two_comp(bool sign, uint8_t value) {
    int16_t res;
    if(sign)
        res = 0xFF00 | value;
    else
        res = value;
    return res;
}

int mouse_subscribe_int(uint8_t *bit_no)
{
    hook_id = MOUSE_IRQ;
    *bit_no = hook_id;
    if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK)
    {
        printf("Sys_irqsetpoicy error.\n");
        return 1;
    }
    return 0;
}

int mouse_unsubscribe_int()
{
    if(sys_irqrmpolicy(&hook_id) != OK){
        printf("Sys_irqrmpolicy error.\n");
        return 1;
    }
    return 0;
}

int readStatus(uint8_t *st)
{
    if(util_sys_inb(KBC_STATUS_REG, st) != OK)
        return 1;
    return 0;
}

void (mouse_ih)()
{
    error_raised = 0;
    empty_buffer = 0;
    uint8_t st;
    if((readStatus)(&st) != OK){
        printf("READING STATUS ERROR.\n");
        error_raised++;}
    if((st & KBC_OBF) != 0)
        empty_buffer++;
    else if((st & KBC_STATUS_PARITY) >> 7){
        printf("PARITY ERROR\n");
        error_raised++;
    }
    else if((st & KBC_STATUS_TIMEOUT) >> 6){
        printf("TIMEOUT ERROR\n");
        error_raised++;
    }
    util_sys_inb(KBC_OUT_BUF, &data);
}

int input_buffer_clean()
{
    int counter = 10;
    uint8_t st;
    while(counter > 0)
    {
        readStatus(&st);
        if((st & KBC_IBF) >> 1)
        {
            tickdelay(micros_to_ticks(WAIT_KBC));
            counter--;
        }
        else
            counter = -1;
    }

    if(counter == 0)
        return 1;
    return 0;
}

int output_buffer_full()
{
    uint8_t st;
    readStatus(&st);
    if(st & KBC_OBF)
        return 0;
    return 1;
}

int send_command_toMouse(uint8_t cmd)
{
    do
    {
        if(input_buffer_clean() != OK)
            return 1;

        sys_outb(KBC_IN_BUF, WRITE_BYTE_MOUSE);

        if(input_buffer_clean() != OK)
            return 1;
        sys_outb(KBC_OUT_BUF, cmd);
    }while(verifyACKbyte() == OK);
    return 0;
}

int verifyACKbyte()
{
    uint8_t ack;
    while(output_buffer_full() != OK){
        tickdelay(micros_to_ticks(WAIT_KBC));
    }
    if(util_sys_inb(KBC_OUT_BUF, &ack) != OK)
        return 1;
    return (ack == ACK);
}

void packetAssemble(uint8_t data)
{
    p.bytes[pCounter] = data;
    switch(pCounter){
        case 0:
            if((data & BIT3) == 0){
                pCounter--;
                break;
            }
            p.lb = (data & LEFT_BUTTON);
            p.rb = (data & RIGHT_BUTTON) >> 1;
            p.mb = (data & MIDDLE_BUTTON) >> 2;
            p.x_ov = (data & X_OV) >> 6;
            p.y_ov = (data & Y_OV) >> 7;
            pCounter++;
            break;
        case 1:
            p.delta_x = two_comp((p.bytes[0] & MSB_X) >> 4, data);
            pCounter++;
            break;
        case 2: 
            p.delta_y = two_comp((p.bytes[0] & MSB_Y) >> 5, data);
            pCounter++;
    }
}

int readCommandByte(uint8_t *cmd)
{
    if(input_buffer_clean() != OK)
        return 1;
    if(sys_outb(KBC_CMD_REG, KBC_READ_CMD_BYTE) != OK)
        return 1;
    if(util_sys_inb(KBC_OUT_BUF, cmd) != OK)
        return 1;
    return 0;
}

int writeCommandByte()
{
    if(input_buffer_clean() != OK)
        return 1;
    if(sys_outb(KBC_STATUS_REG, KBC_WRITE_CMD_BYTE) != OK)
        return 1;
    if(sys_outb(KBC_OUT_BUF, minix_get_dflt_kbc_cmd_byte()) != OK)
        return 1;
    return 0;
} 

int writeCommandByteInit(uint8_t mask)
{
    uint8_t cmd;
    if(readCommandByte(&cmd) != OK)
        return 1;
    if(input_buffer_clean() != OK)
        return 1;
    if(sys_outb(KBC_STATUS_REG, KBC_WRITE_CMD_BYTE) != OK)
        return 1;
    if(sys_outb(KBC_OUT_BUF, cmd & mask) != OK)
        return 1;
    return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no)
{
    timer_hook_id = TIMER0_IRQ;
    *bit_no = timer_hook_id;
    if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id) != OK)
        return 1;
    return 0;
}

int (timer_unsubscribe_int)()
{
    if(sys_irqrmpolicy(&timer_hook_id) != OK){
        printf("Sys_irqrmpolicy error.\n");
        return 1;
    }
    return 0;
}

void(timer_int_handler)()
{
    timer_counter++;
}

bool lb_pressed = 0, rb_pressed = 0;
struct mouse_ev (mouse_detect_ev)(struct packet * pp){
    struct mouse_ev event;
    event.delta_x = pp->delta_x;
    event.delta_y = pp->delta_y;

    if( pp->lb && !pp->rb && !pp->mb){
        event.type = LB_PRESSED;
        lb_pressed = 1;
    }
    else if(!pp->lb && pp->rb && !pp->mb){
	    event.type = RB_PRESSED;
        rb_pressed = 1;
    }
    else if(!pp->lb && !pp->rb && !pp->mb && pp->delta_x == 0 && pp->delta_y == 0){
        if(lb_pressed){
            event.type = LB_RELEASED;
            lb_pressed = 0;
        }
        else if(rb_pressed){
            event.type = RB_RELEASED;
            rb_pressed = 1;
        }
        else{
            event.type = BUTTON_EV;
        }
    }
    else if(pp->delta_x != 0 || pp->delta_y != 0){
        event.type = MOUSE_MOV;
    }
    else{
        event.type = BUTTON_EV;
        lb_pressed = 0;
        rb_pressed = 0;
    }
    return event;
}

enum current_state{
    LB_P, RB_P, INIT, READY1, READY2, HALF2
};

enum current_state state = INIT;
extern bool end;
extern uint32_t x_sum;
void (gesture_handler)(struct mouse_ev event, uint8_t x_len, uint8_t tolerance)
{
    switch(state){
        case INIT:
            if(event.type == LB_PRESSED)
                state = LB_P;
            break;
        case LB_P:
            if(event.type == LB_PRESSED){
                if((event.delta_x < -tolerance) || (event.delta_y < -tolerance)){
                    state = INIT;
                    x_sum = 0;
                }
                else if(abs(event.delta_y / event.delta_x) < 1){
                    state = INIT;
                    x_sum = 0;
                }
                else{
                    x_sum += event.delta_x;
                    if(x_sum >= x_len)
                        state = READY1;
                }
            }
            else if(event.type == LB_RELEASED)
                state = INIT;
            else if(event.type == BUTTON_EV && event.delta_x == 0 && event.delta_y == 0)
                break;
            else if(event.type == RB_PRESSED && event.delta_x == 0 && event.delta_y == 0)
                break;
            else if (event.type == RB_RELEASED)
                break;
            else{
                state = INIT;
                    x_sum = 0;
            }
            break;
        case READY1:
            if(event.type == LB_PRESSED){
                if((event.delta_x < -tolerance) || (event.delta_y < -tolerance)){
                    state = INIT;
                    x_sum = 0;
                }
                else if(abs(event.delta_y / event.delta_x) < 1){
                    state = INIT;
                    x_sum = 0;
                }
            }
            else if(event.type == LB_RELEASED){
                state = HALF2;
                x_sum = 0;
            }
            else{
                state = INIT;
                x_sum = 0;
            }
            break;
        case HALF2:
            if(event.type == RB_PRESSED && abs(event.delta_x) <= tolerance && abs(event.delta_y) <= tolerance)
                state = RB_P;
            else if(event.type == LB_PRESSED && event.delta_x >= -tolerance <= tolerance && event.delta_y >= -tolerance){
                state = READY1;
            }
            else if(event.type == RB_RELEASED)
                break;
            else if(event.type == BUTTON_EV && abs(event.delta_x) <= tolerance && abs(event.delta_y) <= tolerance)
                break;
            else if(abs(event.delta_x) <= tolerance && abs(event.delta_y) <= tolerance)
                break;
            else{
                state = INIT;
                x_sum = 0;
            }
            break;
        case RB_P:
            if(event.type == RB_PRESSED){
                if((event.delta_x < -tolerance) || (event.delta_y > tolerance)){
                    state = INIT;
                    x_sum = 0;
                }
                else if(abs(event.delta_y / event.delta_x) < 1){
                    state = INIT;
                    x_sum = 0;
                }
                else{
                    x_sum += event.delta_x;
                    if(x_sum >= x_len)
                        state = READY2;
                    }
            }
            else if(event.type == RB_RELEASED)
                state = HALF2;
            else if(event.type == BUTTON_EV && abs(event.delta_x) <= tolerance && abs(event.delta_y) <= tolerance)
                break;
            else{ 
                state = INIT;
                x_sum = 0;
            }
            break;
        case READY2:
            if(event.type == RB_PRESSED){
                if((event.delta_x < -tolerance) || (event.delta_y > tolerance)){
                    state = INIT;
                    x_sum = 0;
                }
                else if(abs(event.delta_y / event.delta_x) < 1){
                    state = INIT;
                    x_sum = 0;
                }
            }
            else if(event.type == RB_RELEASED)
                end = true;
            else if(event.type == BUTTON_EV && abs(event.delta_x) <= tolerance && abs(event.delta_y) <= tolerance)
                break;
            else{
                state = INIT;
                x_sum = 0;
            }
            break;
        default:
            break;
    }
}
