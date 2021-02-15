#include "settings_menu.h"

extern vbe_mode_info_t vbe_info;
extern Sprite * mouse_cursor;
extern const char* mouse_cursor_xpm[];
extern const char * settings_xpm[];
extern const char * settings_back_xpm[];
extern const char * settings_playground_xpm[];

extern const char * nW0_xpm[];
extern const char * nW1_xpm[];
extern const char * nW2_xpm[];
extern const char * nW3_xpm[];
extern const char * nW4_xpm[];
extern const char * nW5_xpm[];
extern const char * nW6_xpm[];
extern const char * nW7_xpm[];
extern const char * nW8_xpm[];
extern const char * nW9_xpm[];

extern uint8_t mouse_bit_number;
extern uint8_t timer_bit_number;
extern uint8_t keyboard_bit_number;
extern int actual_time;
extern int actual_velocity;


Sprite * sprites[3], * numbers[10];
int actual_sprite, time_max = 45, time_min = 10, velocity_max = 45, velocity_min = 5, settings_prev_index = 0;
uint16_t settings_start_pos_x = 0, settings_start_pos_y = 0;
bool settings_prev_event;
bool (* buttons[])(bool) = {function_time, function_velocity};

int settings_menu_cursor_button(uint16_t x, uint16_t y){
	if(between(x, PLAYGROUND_X, PLAYGROUND_X + PLAYGROUND_WIDTH) && between(y, PLAYGROUND_Y, PLAYGROUND_Y + PLAYGROUND_HEIGHT)) return MOUSE_PLAYGROUND;
	else if(between(x, BACK_BT_X, BACK_BT_X + BACK_BT_WIDTH) && between(y, BACK_BT_Y, BACK_BT_Y + BACK_BT_HEIGHT)) return MOUSE_BACK;
	else if(between(y, TIME_BT_Y, TIME_BT_Y + BUTTON_HEIGHT)){ //All this buttons have the same y
		if(between(x, BUTTON_M_X, BUTTON_M_X + BUTTON_WIDTH)) return MOUSE_TIME_BT_M;
      	else if(between(x, BUTTON_P_X, BUTTON_P_X + BUTTON_WIDTH)) return MOUSE_TIME_BT_P;
  	}
	else if(between(y, VELO_BT_Y, VELO_BT_Y + BUTTON_HEIGHT)){ //All this buttons have the same y
		if(between(x, BUTTON_M_X, BUTTON_M_X + BUTTON_WIDTH)) return MOUSE_VELO_BT_M;
		else if(between(x, BUTTON_P_X, BUTTON_P_X + BUTTON_WIDTH)) return MOUSE_VELO_BT_P;
	}
  return DEFAULT;
}

void settings_load_sprites(){

    /*
     * Settings sprite
     */
    sprites[0] = create_sprite(settings_xpm, 0, 0, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites[1] = create_sprite(settings_back_xpm, 0, 0, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites[2] = create_sprite(settings_playground_xpm, 0, 0, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);

    /*
     * Numbers
     */
    numbers[0] = create_sprite(nW0_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    numbers[1] = create_sprite(nW1_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    numbers[2] = create_sprite(nW2_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    numbers[3] = create_sprite(nW3_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    numbers[4] = create_sprite(nW4_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    numbers[5] = create_sprite(nW5_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    numbers[6] = create_sprite(nW6_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    numbers[7] = create_sprite(nW7_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    numbers[8] = create_sprite(nW8_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    numbers[9] = create_sprite(nW9_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);

}

int settings_menu(char * video_mem, char * double_buffer){
    actual_sprite = 0;

    settings_load_sprites();
    draw_sprites(double_buffer); 
    mouse_cursor->x = vbe_info.XResolution / 2;
    mouse_cursor->y = vbe_info.YResolution / 2 - 20;
    draw_sprite_in(vbe_info.XResolution / 2, vbe_info.YResolution / 2 - 20,mouse_cursor, double_buffer);

    struct packet mouse_packet;
    message msg;
    int r, ipc_status, pCounter = 0;

    uint8_t mouse_data_byte, keyboard_data;
    uint32_t mouse_irq_set = BIT(mouse_bit_number), timer_irq_set = BIT(timer_bit_number), keyboard_irq_set = BIT(keyboard_bit_number);

    int end = DEFAULT;
    while(end != MOUSE_PLAYGROUND && end != MOUSE_BACK){
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
                                if(pCounter == 3) {     //Avoids the three bytes from not being processed
                                    end = settings_state_machine(detect_event(&mouse_packet), &mouse_packet, double_buffer);
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
                if(msg.m_notify.interrupts & timer_irq_set && pCounter == 0) {
                    memcpy(video_mem, double_buffer, vbe_info.XResolution * vbe_info.YResolution * ceil(vbe_info.BitsPerPixel / 8.0)); //Double buffering
                }
                default:
                    break;
            }
        }
    }

    // Free the space used by the sprites
    for (int i = 0; i < 3; i++) destroy_sprite(sprites[i]);
    for(int i = 0; i < 10; i++) destroy_sprite(numbers[i]);

    return end;
}

int settings_state_machine(enum mouse_event event, struct packet *pp, char * double_buffer){
    int index = settings_menu_cursor_button(mouse_cursor->x, mouse_cursor->y);
    if(index != settings_prev_index && index < MOUSE_TIME_BT_M){    //It means that the cursor is in a different button, the buffer should be updated
        clear_buffer(double_buffer, &vbe_info);
        actual_sprite = index;          //To track the atual sprite
        draw_sprites(double_buffer);
        settings_prev_index = index;    //To track the cursor position
    }
    switch(event){
        case MOUSE_MOVE:
            move_cursor(pp, double_buffer);
            break;
        case L_PRESSED:
            if(settings_prev_event != L_PRESSED){ settings_start_pos_x = mouse_cursor->x; settings_start_pos_y = mouse_cursor->y;}
            move_cursor(pp, double_buffer);
            break;
        case L_RELEASED:
            if(settings_prev_event == L_PRESSED){ //check if the position of the cursor is on a button and the initial press was done inside the corresponding button
                int index = settings_menu_cursor_button(mouse_cursor->x, mouse_cursor->y); 
                if(index >= MOUSE_TIME_BT_M && index <= MOUSE_VELO_BT_P) { //If the button pressed is one of the decrement/increment buttons of the menu
                    //index is bigger than 5 if the buttons are velocity buttons
					if(buttons[index >= 5](index == MOUSE_VELO_BT_P || index == MOUSE_TIME_BT_P)){ draw_sprites(double_buffer); move_cursor(pp, double_buffer);}
				}
				else if(index && index == settings_menu_cursor_button(settings_start_pos_x, settings_start_pos_y))
                    return index;  //function will return a value to change the menu
            }
            break;
        default:
            move_cursor(pp, double_buffer);
            break;
    }
    settings_prev_event = event;
    return DEFAULT;
}

bool function_time(bool add){
	if(add){ // add is true when it is supposed to increment the time
		if(actual_time + 1 > time_max) return false;    //Only increments the time if it is less than the max value
		else actual_time++;
	}else{
		if(actual_time - 1 < time_min) return false;    //Only decrements the time if it is bigger than the minimum value
		else actual_time--;
	}
	return true; //Returns true if the operation was a success
}
bool function_velocity(bool add){
	if(add){ //add is true when it is supposed to increment the velocity
		if(actual_velocity + 1 > velocity_max) return false; //Only increments the velocity if it is less than the max value
		else actual_velocity++;
	}else{
		if(actual_velocity - 1 < velocity_min) return false; //Only decrements the velocity if it is bigger than the minimum value
		else actual_velocity--;
	}
	return true; //Returns true if the operation was a success
}

void draw_sprites(char * double_buffer){
	draw_sprite(sprites[actual_sprite], double_buffer);
    draw_time(double_buffer);
    draw_velocity(double_buffer);
}
void draw_time(char * double_buffer){
    int temp[2];
    int temporary_time = actual_time;
    for(int i = 0; i < 2; i++){ //Get the digits of the time
        temp[1 - i] = temporary_time % 10;
        temporary_time /= 10;
    }
    for(int i = 0; i < 2; i++){ //Draw the obtained digits
        draw_sprite_in(NUMBERS_X + i * (numbers[temp[i]]->width + SPACE_BETWEEN_NUMBERS), TIME_N_Y, numbers[temp[i]], double_buffer);
    }
}
void draw_velocity(char * double_buffer){
    int temp[2];
    int temporary_velo = actual_velocity;
    for(int i = 0; i < 2; i++){ //Get the digits of the velocity
        temp[1 - i] = temporary_velo % 10;
        temporary_velo /= 10;
    }
    for(int i = 0; i < 2; i++){ //Draw the obtained digits
        draw_sprite_in(NUMBERS_X + i * (numbers[temp[i]]->width + SPACE_BETWEEN_NUMBERS), VELO_N_Y, numbers[temp[i]], double_buffer);
    }
}
