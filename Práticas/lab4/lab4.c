// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "mouse.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


extern uint8_t data;
int  error_raised, pCounter, empty_buffer;
struct packet p;

int (mouse_test_packet)(uint32_t cnt)
{
    int r, ipc_status;
    uint8_t bit_no = 0;
    uint8_t garbage;

    message msg;

    if(send_command_toMouse(EN_DATA_REPORTING) != OK)
        return 1;

    if(mouse_subscribe_int(& bit_no))
        return 1;

    uint32_t irq_set = BIT(bit_no);
    

    pCounter = 0;

    while( cnt > 0 ) { // You may want to use a different condition 
    // Get a request message. 
        if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { // received notification 
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: // hardware interrupt notification 
                    if (msg.m_notify.interrupts & irq_set) {
                        pCounter %= 3;
                        mouse_ih();
                        if(error_raised != 0) return 1;
                        packetAssemble(data);
                        if(pCounter == 3)
                        {
                            mouse_print_packet(& p);
                            cnt--;
                        }
                    }
                    break;
                default:
                    break; // no other notifications expected: do nothing 19
            } 
        }
        else { } // received a standard message, not a notification 21:  no standard messages expected: do nothing 
    }


    if(mouse_unsubscribe_int() != OK)
        return 1;
    

    while(!output_buffer_full())
    {
        if(util_sys_inb(KBC_OUT_BUF, &garbage) != OK)
            return 1;
    }
    
    if(send_command_toMouse(DIS_DATA_REPORTING) != OK)
        return 1;

    return 0;
}

extern uint8_t timer_counter;
int (mouse_test_async)(uint8_t idle_time) {
    
    uint8_t mouse_bit_no, timer_bit_no;
    int ipc_status, r;
    message msg;

    send_command_toMouse(EN_DATA_REPORTING);

    if((timer_subscribe_int)(&timer_bit_no) != OK)//Subscribe a notification of interrupt
        return 1; 
    uint32_t timer_irqset = BIT(timer_bit_no); //Identifier used to comunicate kernel about an interrupt
    if(mouse_subscribe_int(& mouse_bit_no))
        return 1;
    uint32_t mouse_irqset = BIT(mouse_bit_no); //Identifier used to comunicate kernel about an interrupt
    pCounter = 0;

    while(timer_counter < (idle_time * sys_hz()))
    {
        // Get a request message.
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r); continue;
        }

        if (is_ipc_notify(ipc_status)) { // received notification 
            switch(_ENDPOINT_P(msg.m_source)){
                case (HARDWARE):

                    if (msg.m_notify.interrupts & mouse_irqset) {
                        mouse_ih();
                        if(error_raised != 0) return 1;
                        packetAssemble(data);
                        if(pCounter == 3)
                        {
                            mouse_print_packet(& p);
                            pCounter = 0;
                            timer_counter = 0;
                        }
                    }

                    else if (msg.m_notify.interrupts & timer_irqset && (pCounter == 0)) {
                        timer_int_handler();
                        if(timer_counter % sys_hz() == 0)
                            timer_print_elapsed_time();
                    }
                    break;

                default:
                    break;
            }
        }
    }
    if (timer_unsubscribe_int() != OK){
        printf("Timer unsubscribe failed\n");
        return 1;
    }

    if(mouse_unsubscribe_int() != OK)
        return 1;

    send_command_toMouse(DIS_DATA_REPORTING);

    while (!output_buffer_full()) { // OUTPUT BUFFER flush
        uint8_t garbage_data;
        if (util_sys_inb(KBC_OUT_BUF, &garbage_data) != OK) return 1;
    }
    return 0;
}

uint32_t x_sum = 0;
bool end = 0;
int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    int r, ipc_status;
    uint8_t bit_no = 0;
    uint8_t garbage;

    message msg;

    if(send_command_toMouse(EN_DATA_REPORTING) != OK)
        return 1;

    if(mouse_subscribe_int(& bit_no))
        return 1;

    uint32_t irq_set = BIT(bit_no);
    
    pCounter = 0;

    while( !end ) { // You may want to use a different condition 
    // Get a request message. 
        if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { // received notification 
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: // hardware interrupt notification 
                    if (msg.m_notify.interrupts & irq_set) {
                        pCounter %= 3;
                        mouse_ih();
                        if(error_raised != 0) return 1;
                        packetAssemble(data);
                        if(pCounter == 3){
                            mouse_print_packet(& p);
                            struct mouse_ev event = mouse_detect_ev(&p);
                            (gesture_handler)(event, x_len, tolerance);
                        }
                    }
                    break;
                default:
                    break; // no other notifications expected: do nothing 19
            } 
        }
        else { } // received a standard message, not a notification 21:  no standard messages expected: do nothing
    }

    if(mouse_unsubscribe_int() != OK)
        return 1;
    

    while(!output_buffer_full())
    {
        if(util_sys_inb(KBC_OUT_BUF, &garbage) != OK)
            return 1;
    }
    
    if(send_command_toMouse(DIS_DATA_REPORTING) != OK)
        return 1;

    return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {

    pCounter = 0;
    while(cnt > 0)
    {
        pCounter %= 3;
        if(pCounter == 0)
            if(send_command_toMouse(READ_DATA_PACKET) != OK)
                return 1;
        mouse_ih();
        if(error_raised != 0) return 1;
        if(empty_buffer != 0) tickdelay(micros_to_ticks(WAIT_KBC));
        packetAssemble(data);
        if(pCounter == 3)
        {
            mouse_print_packet(& p);
            cnt--;
        }
        tickdelay(micros_to_ticks(period / 1000));
    }

    if(send_command_toMouse(SET_STREAM_MODE) != OK)
        return 1;
    if(send_command_toMouse(DIS_DATA_REPORTING) != OK)
        return 1;
    if(writeCommandByte() != OK){
        return 1;
    }
    
    return 0;
}
