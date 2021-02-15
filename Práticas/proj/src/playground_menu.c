#include "playground_menu.h"

extern Sprite * mouse_cursor;
extern vbe_mode_info_t vbe_info;
extern const char * mouse_cursor_xpm[];
extern const char * playground_xpm[];

extern const char * Brush_size_not_p_xpm[];
extern const char * Brush_size_small_p_xpm[];
extern const char * Brush_size_medium_p_xpm[];
extern const char * Brush_size_large_p_xpm[];

extern const char * paint_brush_xpm[];
extern const char * paint_brush_pressed_xpm[];
extern const char * paint_bucket_xpm[];
extern const char * paint_bucket_pressed_xpm[];
extern const char * eraser_xpm[];
extern const char * eraser_pressed_xpm[];
extern const char * color_picker_xpm[];
extern const char * color_picker_pressed_xpm[];
extern const char * cleaning_xpm[];
extern const char * cleaning_pressed_xpm[];

extern uint8_t mouse_bit_number;
extern uint8_t timer_bit_number;
extern uint8_t keyboard_bit_number;

Sprite * background;
Sprite * sprites_painting_tools[10];
Sprite * play_mini_menu[4];
char * play_video_mem, * canvas_buf, * play_double_buffer;
int playground_prev_index = 0;
// Keep tracking of the current tools
int playground_current_tool = BRUSH, playground_current_color = 0x00000000, playground_current_brush_size = MEDIUM;
int playground_replace_size = 0, playground_actual_game_status = PL_PLAYING_DEFAULT, playground_last_game_status = PL_PLAYING_DEFAULT;
bool playground_prev_event;
uint16_t playground_start_pos_x, playground_start_pos_y;

void (* playground_draw_actual[3])(int) = {playground_draw_actual_mouse_in_buttons, playground_draw_actual_mouse_in_tools, playground_draw_actual_mouse_in_menu};

void playground_load_sprites(){

    /*
     * Sprites of the play menu
     */
    background = create_sprite(playground_xpm, 0, 0, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);

    /*
     * Sprites of the play mini menu
     */ 
    play_mini_menu[0] = create_sprite(Brush_size_not_p_xpm, PL_MINI_MENU_X, PL_MINI_MENU_Y, PL_MENU_SPEED, STATIC_SPRITE_SPEED);
    play_mini_menu[1] = create_sprite(Brush_size_small_p_xpm, PL_MINI_MENU_X, PL_MINI_MENU_Y, PL_MENU_SPEED, STATIC_SPRITE_SPEED);
    play_mini_menu[2] = create_sprite(Brush_size_medium_p_xpm, PL_MINI_MENU_X, PL_MINI_MENU_Y, PL_MENU_SPEED, STATIC_SPRITE_SPEED);
    play_mini_menu[3] = create_sprite(Brush_size_large_p_xpm, PL_MINI_MENU_X, PL_MINI_MENU_Y, PL_MENU_SPEED, STATIC_SPRITE_SPEED);
 
    /*
     * Sprites of painting tools
     */
    sprites_painting_tools[0] = create_sprite(paint_brush_xpm, TOOLS_NORMAL_X, BRUSH_Y + 10, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites_painting_tools[1] = create_sprite(paint_bucket_xpm, TOOLS_NORMAL_X, BUCKET_Y + 5, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites_painting_tools[2] = create_sprite(eraser_xpm, TOOLS_NORMAL_X, RUBBER_Y + 5, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites_painting_tools[3] = create_sprite(color_picker_xpm, TOOLS_NORMAL_X, COLOR_PICKER_Y + 5, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites_painting_tools[4] = create_sprite(cleaning_xpm, TOOLS_NORMAL_X, CLEANER_Y + 5, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites_painting_tools[5] = create_sprite(paint_brush_pressed_xpm, TOOLS_ZOOMED_X, BRUSH_Y + 8, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites_painting_tools[6] = create_sprite(paint_bucket_pressed_xpm, TOOLS_ZOOMED_X, BUCKET_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites_painting_tools[7] = create_sprite(eraser_pressed_xpm, TOOLS_ZOOMED_X, RUBBER_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites_painting_tools[8] = create_sprite(color_picker_pressed_xpm, TOOLS_ZOOMED_X, COLOR_PICKER_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites_painting_tools[9] = create_sprite(cleaning_pressed_xpm, TOOLS_ZOOMED_X, CLEANER_Y - 5, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
}

int playground_cursor_button(uint16_t x, uint16_t y) {
    if (between(x, TOOLS_ZOOMED_X, TOOLS_ZOOMED_X + TOOLS_ZOOMED_SIZE) && between(y, BRUSH_Y + 3, BRUSH_Y + TOOLS_ZOOMED_SIZE)) return BRUSH_PRESSED;
    else if (between(x, TOOLS_ZOOMED_X, TOOLS_ZOOMED_X + TOOLS_ZOOMED_SIZE) && between(y, BUCKET_Y, BUCKET_Y + TOOLS_ZOOMED_SIZE)) return BUCKET_PRESSED;
    else if (between(x, TOOLS_ZOOMED_X, TOOLS_ZOOMED_X + TOOLS_ZOOMED_SIZE) && between(y, RUBBER_Y, RUBBER_Y + TOOLS_ZOOMED_SIZE)) return RUBBER_PRESSED;
    else if (between(x, TOOLS_ZOOMED_X, TOOLS_ZOOMED_X + TOOLS_ZOOMED_SIZE) && between(y, COLOR_PICKER_Y, COLOR_PICKER_Y + TOOLS_ZOOMED_SIZE)) return COLOR_PICKER_PRESSED;
    else if (between(x, TOOLS_ZOOMED_X, TOOLS_ZOOMED_X + TOOLS_ZOOMED_SIZE) && between(y, CLEANER_Y - 5, CLEANER_Y + TOOLS_ZOOMED_SIZE)) return CLEANER_PRESSED;
    else if(between(y, BRUSH_SIZE_Y, BRUSH_SIZE_Y + BRUSH_SIZE_HEIGHT)){
        if(between(x, SMALL_SIZE_X, SMALL_SIZE_X + SMALL_SIZE_WIDTH)) return PL_BRUSH_SIZE_SMALL;
        else if(between(x, MEDIUM_SIZE_X, MEDIUM_SIZE_X + MEDIUM_SIZE_WIDTH)) return PL_BRUSH_SIZE_MEDIUM;
        else if(between(x, LARGE_SIZE_X, LARGE_SIZE_X + LARGE_SIZE_WIDTH)) return PL_BRUSH_SIZE_LARGE;
    }
    return PL_MOUSE_NOT_IN_BUTTON;
}

void playground_draw_canvas(){
    for(int i = 0; i < CANVAS_HEIGHT; i++){
        for(int j = 0; j < CANVAS_WIDHT;j++){
            for(int k = 0; k < ceil(vbe_info.BitsPerPixel / 8.0); k++){
                play_double_buffer[(int)(ceil(vbe_info.BitsPerPixel / 8.0)) *((CANVAS_Y + i)*vbe_info.XResolution + (CANVAS_X + j)) + k] = canvas_buf[(int)(ceil(vbe_info.BitsPerPixel / 8.0)) *((CANVAS_Y + i)*vbe_info.XResolution + (CANVAS_X + j)) + k];
            }
        }
    }
}

void playground_draw_brush_size(int button){
    if(button == PL_BRUSH_SIZE_SMALL) draw_sprite(play_mini_menu[1], play_double_buffer);
    else if(button == PL_BRUSH_SIZE_MEDIUM) draw_sprite(play_mini_menu[2], play_double_buffer);
    else if(button == PL_BRUSH_SIZE_LARGE) draw_sprite(play_mini_menu[3], play_double_buffer);
    else draw_sprite(play_mini_menu[PL_BRUSH_SIZE], play_double_buffer);
}

void playground_draw_actual_default(){
    playground_draw_canvas();
    for (int i = 0; i < mouse_cursor->height; i++) {
        for (int j = 0; j < mouse_cursor->width; j++) {
            mouse_cursor->replace_color[i * mouse_cursor->width + j] = video_card_get_pixel_color(play_double_buffer, mouse_cursor->x + j, mouse_cursor->y + i );
        }
    }
}
void playground_draw_actual_mouse_in_buttons(int index){
    draw_sprite(background, play_double_buffer);           //draw the background

    for(int i = 0; i < PL_MENU_NUMBER_TOOLS; i++){
        draw_sprite(sprites_painting_tools[i], play_double_buffer);
    }

    playground_draw_brush_size(0);
    playground_draw_actual_default();
}
void playground_draw_actual_mouse_in_tools(int index){
    draw_sprite(background, play_double_buffer);           //draw the background    
    
    for(int i = 0; i < PL_MENU_NUMBER_TOOLS; i++){
        if(i + 5 + 8 != index){
            draw_sprite(sprites_painting_tools[i], play_double_buffer);
        }
    }
    draw_sprite(sprites_painting_tools[index - 8], play_double_buffer);
    playground_draw_brush_size(0);
    playground_draw_actual_default();   
}
void playground_draw_actual_mouse_in_menu(int index){
    draw_sprite(background, play_double_buffer);           //draw the background

    for(int i = 0; i < PL_MENU_NUMBER_TOOLS; i++){
        draw_sprite(sprites_painting_tools[i], play_double_buffer);
    }

    playground_draw_brush_size(index);
    
    playground_draw_actual_default();
}

int playground_menu(void * video_mem, void * double_buffer){
    int end = PL_MOUSE_NOT_IN_BUTTON, valid_reading = KBD_VALID_READING, kbd_index = 0;
    int timer_counter = 0;
    uint8_t keyboard_data;
    play_video_mem = video_mem;
    play_double_buffer = double_buffer;

    playground_load_sprites();
    mouse_cursor->x = vbe_info.XResolution / 2;
    mouse_cursor->y = vbe_info.YResolution / 2 - 20;

    draw_sprite(background, play_double_buffer);
    for(int i = 0; i < PL_MENU_NUMBER_TOOLS; i++){
        draw_sprite(sprites_painting_tools[i], play_double_buffer);
    }
    playground_draw_brush_size(0);
    canvas_buf = calloc(vbe_info.XResolution * vbe_info.YResolution * ceil(vbe_info.BitsPerPixel / 8.0),sizeof(char));
    for(int i = 0; i < vbe_info.YResolution; i++){
        for(int j = 0; j < vbe_info.XResolution; j++){
            video_card_paint_pixel(canvas_buf, j, i, CANVAS_DEFAULT_COLOR);
        }
    }
    playground_draw_actual_default();   
    draw_sprite_in(vbe_info.XResolution / 2, vbe_info.YResolution / 2 - 20, mouse_cursor, play_double_buffer);

    struct packet mouse_packet;
    message msg;
    int r, ipc_status, pCounter = 0;
    uint8_t mouse_data_byte;

    uint32_t mouse_irq_set = BIT(mouse_bit_number), timer_irq_set = BIT(timer_bit_number), keyboard_irq_set = BIT(keyboard_bit_number);

    while(end != PL_LEAVE_GAME){
        if((r = driver_receive(ANY, &msg, &ipc_status)) != OK) continue;
        if(is_ipc_notify(ipc_status)){
            switch(_ENDPOINT_P(msg.m_source)){
                case HARDWARE:
                    if(msg.m_notify.interrupts & mouse_irq_set){
                        switch(mouse_interrupt_handler(&mouse_data_byte)){
                            case MOUSE_VALID_READING:
                                if(mouse_packet_assemble(&pCounter, &mouse_packet, &mouse_data_byte) != OK) return 1;
                                if(pCounter == 3) {
                                    end = playground_state_machine(detect_event(&mouse_packet), &mouse_packet);
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
                    if(msg.m_notify.interrupts & keyboard_irq_set){
                        valid_reading = keyboard_interrupt_handler(&keyboard_data);
                        if(keyboard_data == TWO_BYTES_CODE){
                            valid_reading = keyboard_interrupt_handler(&keyboard_data);
                        }
                        switch(valid_reading){
                            case KBD_VALID_READING:
                                kbd_index = keyboard_recognizeScanCode(keyboard_data);
                                if(kbd_index == ESC_KEY){
                                    end = PL_LEAVE_GAME;
                                }
                                break;
                            case KBD_EMPTY_BUFFER:
                                continue;
                            case KBD_INVALID_READING:
                                return 1;
                        }
                    }
                    if(msg.m_notify.interrupts & timer_irq_set && pCounter == 0) {
                        playground_refresh_double_buffer();
                        timer_counter++;
                    }
                default:
                    break;
            }
        }
    }

    free(canvas_buf);
    destroy_sprite(background);
    for(int i = 0; i < 10; i++){destroy_sprite(sprites_painting_tools[i]);}
    for(int i = 0; i < 4; i++){destroy_sprite(play_mini_menu[i]);}
    return 0;
}

int playground_state_machine(enum mouse_event event, struct packet *pp){
    int index = playground_cursor_button(mouse_cursor->x, mouse_cursor->y);
    if(index != playground_prev_index){ 
        if(index >= 13 && index <= 17) {playground_last_game_status = playground_actual_game_status; playground_actual_game_status = PL_PLAYING_TOOLS;}
        else if(index >= 18) {playground_last_game_status = playground_actual_game_status; playground_actual_game_status = PL_PLAYING_MENU;}
        else { playground_last_game_status = playground_actual_game_status; playground_actual_game_status = PL_PLAYING_BUTTONS;}
        playground_draw_actual[playground_actual_game_status](index);
        playground_prev_index = index;
    } 
    switch(event){
        case MOUSE_MOVE:
            move_cursor(pp, play_double_buffer);
            break;
        case L_PRESSED:
            if(playground_prev_event != L_PRESSED){ playground_start_pos_x = mouse_cursor->x; playground_start_pos_y = mouse_cursor->y;}
            move_cursor(pp, play_double_buffer); 
            if (between(mouse_cursor->x, CANVAS_X, CANVAS_X + CANVAS_WIDHT) && between(mouse_cursor->y, CANVAS_Y, CANVAS_Y + CANVAS_HEIGHT) && (playground_current_tool == BRUSH || playground_current_tool == RUBBER))
                playground_paint_call(pp, playground_current_tool, playground_current_color, playground_current_brush_size);
            break;
        case L_RELEASED:
            if(playground_prev_event == L_PRESSED){ //check if the position of the cursor is on a button and the initial press was done inside a the corresponding button
                index = playground_cursor_button(mouse_cursor->x, mouse_cursor->y);
                if(index && index == playground_cursor_button(playground_start_pos_x, playground_start_pos_y)){ //se é diferente d background e começou no botão
                    if(index >= 13 && index <= 17){
                        if(index == CLEANER_PRESSED) playground_paint_call(pp, CLEANER, playground_current_color, playground_current_brush_size);
                        else playground_current_tool = index - 5;
                    }
                    else if(index >= PL_BRUSH_SIZE_SMALL && index <= PL_BRUSH_SIZE_LARGE)
                        playground_current_brush_size = ((index % 18) + 1) * 10; //SMALL = 10, MEDIUM = 20, LARGE = 30
                }
                else if(playground_current_tool == COLOR_PICKER || playground_current_tool == BUCKET) playground_paint_call(pp, playground_current_tool, playground_current_color, playground_current_brush_size);
            }
            break;
        default:
            move_cursor(pp, play_double_buffer);
            break;
    }
    
    playground_prev_event = event;
    return PL_MOUSE_NOT_IN_BUTTON;
}

void playground_paint_call(struct packet *pp, enum paint_tools curr_tool, uint32_t curr_color, enum brush_size b_size) {
    erase_sprite(mouse_cursor, play_double_buffer);
    switch (curr_tool) {
        case BRUSH:
            for(int i = 0; i < mouse_cursor->height; i++){
                for(int j = 0; j < mouse_cursor->width; j++){
                    mouse_cursor->replace_color[i*mouse_cursor->height + j] = curr_color;
                }
            }
            canvas_fill_brush(mouse_cursor->x, mouse_cursor->y, pp->delta_x, -(pp->delta_y), curr_color, b_size, false);
            break;
        case RUBBER:

            canvas_fill_brush(mouse_cursor->x, mouse_cursor->y, pp->delta_x, -(pp->delta_y), CANVAS_DEFAULT_COLOR, MEDIUM, true);
            break;
        case BUCKET:
            canvas_bucket_fill(mouse_cursor->x, mouse_cursor->y, curr_color, mouse_cursor->replace_color[0]);
            break;
        case COLOR_PICKER:
            playground_current_color = mouse_cursor->replace_color[0];
            break;
        case CLEANER:
            canvas_cleaning();
            break;
        default:
            break;
    }
    draw_sprite(mouse_cursor, play_double_buffer);
}

void playground_refresh_double_buffer(){
    memcpy(play_video_mem, play_double_buffer, vbe_info.XResolution * vbe_info.YResolution * ceil(vbe_info.BitsPerPixel / 8.0));
}
