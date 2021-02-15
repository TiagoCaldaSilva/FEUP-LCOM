#include "main_menu.h"

extern Sprite * mouse_cursor;
extern vbe_mode_info_t vbe_info;
extern const char* initial_screen_xpm[];
extern const char* mouse_cursor_xpm[];
extern const char* exit_pressed_xpm[];
extern const char* play_pressed_xpm[];
extern const char* settings_pressed_xpm[];

extern uint8_t mouse_bit_number;
extern uint8_t timer_bit_number;
extern uint8_t keyboard_bit_number;

uint16_t main_menu_start_pos_x = 0, main_menu_start_pos_y = 0;
bool main_menu_prev_event;
int main_menu_prev_index = 0;

int main_menu_cursor_button(uint16_t x, uint16_t y) {
    if (between(x, MAIN_MENU_BUTTON_X, MAIN_MENU_BUTTON_X + MAIN_MENU_BUTTON_WIDTH) && between(y, PLAY_BUTTON_Y, PLAY_BUTTON_Y + MAIN_MENU_BUTTON_HEIGHT)) return MOUSE_PLAY_BUTTON;
    else if (between(x, MAIN_MENU_BUTTON_X, MAIN_MENU_BUTTON_X + MAIN_MENU_BUTTON_WIDTH) && between(y, SETTINGS_BUTTON_Y, SETTINGS_BUTTON_Y + MAIN_MENU_BUTTON_HEIGHT)) return MOUSE_SETTINGS_BUTTON;
    else if (between(x, MAIN_MENU_BUTTON_X, MAIN_MENU_BUTTON_X + MAIN_MENU_BUTTON_WIDTH) && between(y, EXIT_BUTTON_Y, EXIT_BUTTON_Y + MAIN_MENU_BUTTON_HEIGHT)) return MOUSE_EXIT_BUTTON;
    else return MOUSE_NOT_IN_BUTTON;
}

//Returns which menu shoul appear in the end of this function
int main_menu(void * video_mem, void * double_buffer){

    Sprite * sprites[4];
    sprites[0] = create_sprite(initial_screen_xpm, 0, 0, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites[1] = create_sprite(play_pressed_xpm, 0, 0, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites[2] = create_sprite(settings_pressed_xpm, 0, 0, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites[3] = create_sprite(exit_pressed_xpm, 0, 0, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    
    mouse_cursor->x = vbe_info.XResolution / 2;
    mouse_cursor->y = vbe_info.YResolution / 2 - 20;
    draw_sprite(sprites[1], double_buffer);
    draw_sprite_in(vbe_info.XResolution / 2, vbe_info.YResolution / 2 - 20, mouse_cursor, double_buffer);


    struct packet mouse_packet;
    message msg;
    int r, ipc_status, pCounter = 0;
    uint8_t mouse_data_byte, keyboard_data;

    uint32_t mouse_irq_set = BIT(MOUSE_IRQ), timer_irq_set = BIT(TIMER_IRQ), keyboard_irq_set = BIT(keyboard_bit_number);
    int end = NOT_IN_BREAK_CONDITION;
    while(end == NOT_IN_BREAK_CONDITION){
        if((r = driver_receive(ANY, &msg, &ipc_status)) != OK) continue;
        if(is_ipc_notify(ipc_status)){
            switch(_ENDPOINT_P(msg.m_source)){
                case HARDWARE:
                    if(msg.m_notify.interrupts & keyboard_irq_set){
                        keyboard_interrupt_handler(&keyboard_data);
                        if(keyboard_data == TWO_BYTES_CODE)
                            keyboard_interrupt_handler(&keyboard_data);
                    }
                    if(msg.m_notify.interrupts & mouse_irq_set){
                        switch(mouse_interrupt_handler(&mouse_data_byte)){
                            case MOUSE_VALID_READING:
                                if(mouse_packet_assemble(&pCounter, &mouse_packet, &mouse_data_byte) != OK) return 1;
                                if(pCounter == 3) { //Avoids the three bytes from not being processed
                                    end = main_menu_state_machine(detect_event(&mouse_packet), &mouse_packet, double_buffer, sprites);
                                    pCounter = 0;
                                }
                                break;
                            case MOUSE_INVALID_READING:
                                return 1;
                            case MOUSE_EMPTY_BUFFER:
                                break;
                            default:
                                break;
                        }
                    }
                    if(msg.m_notify.interrupts & timer_irq_set && pCounter == 0) {  //Double buffering
                        memcpy(video_mem, double_buffer, vbe_info.XResolution * vbe_info.YResolution * ceil(vbe_info.BitsPerPixel / 8.0));
                    }
                    break;
                default:
                    break;
            }
        }
    }

    // Free the space used by the sprites
    for (int i = 0; i < 4; i++) 
        destroy_sprite(sprites[i]);
    return end;
}

int main_menu_state_machine(enum mouse_event event, struct packet *pp, char * double_buffer, Sprite * sprites[]){
    int index = main_menu_cursor_button(mouse_cursor->x, mouse_cursor->y);
    if(index != main_menu_prev_index){  //It means that the cursor is in a different button, the buffer should be updated
        clear_buffer(double_buffer, &vbe_info);
        swap_sprite(double_buffer, sprites, index);
        main_menu_prev_index = index;   //To track the cursor position
    }
    switch(event){
        case MOUSE_MOVE:
            move_cursor(pp, double_buffer);
            break;
        case L_PRESSED:
            if(main_menu_prev_event != L_PRESSED){ main_menu_start_pos_x = mouse_cursor->x; main_menu_start_pos_y = mouse_cursor->y;}
            move_cursor(pp, double_buffer);
            break;
        case L_RELEASED:
            if(main_menu_prev_event == L_PRESSED){ //Check if the position of the cursor is on a button and the initial press was done inside a the corresponding button
                int index = main_menu_cursor_button(mouse_cursor->x, mouse_cursor->y); 
                if(index && index == main_menu_cursor_button(main_menu_start_pos_x, main_menu_start_pos_y))
                    return index;  //function called return a value to end the program and open the next menu
            }
            break;
        default:
            move_cursor(pp, double_buffer);
            break;
    }
    main_menu_prev_event = event;
    return NOT_IN_BREAK_CONDITION;
}
