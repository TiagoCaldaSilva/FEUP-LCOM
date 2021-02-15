#include "avatar.h"

extern Sprite * mouse_cursor;
extern vbe_mode_info_t vbe_info;

extern const char * mouse_cursor_xpm[];

extern const char * avatar_background_xpm[];
extern const char * ready_green_xpm[];
extern const char * ready_red_xpm[];
extern const char * front_pressed_xpm[];
extern const char * front_not_pressed_xpm[];
extern const char * back_pressed_xpm[];
extern const char * back_not_pressed_xpm[];

extern const char * caveman_xpm[];
extern const char * cyborg_xpm[];
extern const char * deadcyber_xpm[];
extern const char * devil_xpm[];
extern const char * knight_xpm[];
extern const char * lego_xpm[];
extern const char * mummy_xpm[];
extern const char * ninja_xpm[];
extern const char * pirate_xpm[];
extern const char * santa_claus_xpm[];
extern const char * spiderman_xpm[];
extern const char * spy_xpm[];
extern const char * superman_xpm[];
extern const char * wolverine_xpm[];

extern const char * a_xpm[];
extern const char * b_xpm[];
extern const char * c_xpm[];
extern const char * d_xpm[];
extern const char * e_xpm[];
extern const char * f_xpm[];
extern const char * g_xpm[];
extern const char * h_xpm[];
extern const char * i_xpm[];
extern const char * j_xpm[];
extern const char * k_xpm[];
extern const char * l_xpm[];
extern const char * m_xpm[];
extern const char * n_xpm[];
extern const char * o_xpm[];
extern const char * p_xpm[];
extern const char * q_xpm[];
extern const char * r_xpm[];
extern const char * s_xpm[];
extern const char * t_xpm[];
extern const char * u_xpm[];
extern const char * v_xpm[];
extern const char * w_xpm[];
extern const char * x_xpm[];
extern const char * y_xpm[];
extern const char * z_xpm[];

extern uint8_t mouse_bit_number;
extern uint8_t timer_bit_number;
extern uint8_t keyboard_bit_number;

Sprite * player1_icons[7];
Sprite * player2_icons[7];
Sprite * background[7];
Sprite * alphabet_sprites[ALPHABET_LENGTH];
/**
 * player1_name and player2_name are arrays with the name letters' position in the alphabet_sprites array
 */
int player1_name[NAME_MAX_LENGTH]; int player1_name_size; int *player1_icon; bool player1_ready;
int player2_name[NAME_MAX_LENGTH]; int player2_name_size; int *player2_icon; bool player2_ready;

uint32_t ** replace_color_1; int replace1_size;
uint32_t ** replace_color_2; int replace2_size;

enum player_write who; //Represent which player is writing

char * avatar_video_mem;
char * avatar_double_buffer;

int select_avatar_prev_index;
enum mouse_event select_avatar_prev_event;
uint16_t select_avatar_start_x, select_avatar_start_y;

void select_avatar_load_sprites(){

    /*
     * Background sprites
     */
    background[0] = create_sprite(avatar_background_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    background[1] = create_sprite(ready_red_xpm, DEFAULT_XY, READY_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    background[2] = create_sprite(front_not_pressed_xpm, DEFAULT_XY, BACK_FRONT_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    background[3] = create_sprite(back_not_pressed_xpm, DEFAULT_XY, BACK_FRONT_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    background[4] = create_sprite(ready_green_xpm, DEFAULT_XY, READY_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    background[5] = create_sprite(front_pressed_xpm, DEFAULT_XY, BACK_FRONT_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    background[6] = create_sprite(back_pressed_xpm, DEFAULT_XY, BACK_FRONT_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);

    /*
     * Player 1 icons
     */
    player1_icons[0] = create_sprite(caveman_xpm, AVATAR1_X, AVATAR1_Y, AVATAR_SPEED_X, AVATAR_SPEED_Y);
    player1_icons[1] = create_sprite(cyborg_xpm, AVATAR1_X, AVATAR1_Y, AVATAR_SPEED_X, AVATAR_SPEED_Y);
    player1_icons[2] = create_sprite(deadcyber_xpm, AVATAR1_X, AVATAR1_Y, AVATAR_SPEED_X, AVATAR_SPEED_Y);
    player1_icons[3] = create_sprite(devil_xpm, AVATAR1_X, AVATAR1_Y, AVATAR_SPEED_X, AVATAR_SPEED_Y);
    player1_icons[4] = create_sprite(knight_xpm, AVATAR1_X, AVATAR1_Y, AVATAR_SPEED_X, AVATAR_SPEED_Y);
    player1_icons[5] = create_sprite(lego_xpm, AVATAR1_X, AVATAR1_Y, AVATAR_SPEED_X, AVATAR_SPEED_Y);
    player1_icons[6] = create_sprite(mummy_xpm, AVATAR1_X, AVATAR1_Y, AVATAR_SPEED_X, AVATAR_SPEED_Y);

    /*
     * Player 2 icons
     */
    player2_icons[0] = create_sprite(ninja_xpm, AVATAR2_X, AVATAR2_Y, AVATAR_SPEED_X, AVATAR_SPEED_Y);
    player2_icons[1] = create_sprite(pirate_xpm, AVATAR2_X, AVATAR2_Y, AVATAR_SPEED_X, AVATAR_SPEED_Y);
    player2_icons[2] = create_sprite(santa_claus_xpm, AVATAR2_X, AVATAR2_Y, AVATAR_SPEED_X, AVATAR_SPEED_Y);
    player2_icons[3] = create_sprite(spiderman_xpm, AVATAR2_X, AVATAR2_Y, AVATAR_SPEED_X, AVATAR_SPEED_Y);
    player2_icons[4] = create_sprite(spy_xpm, AVATAR2_X, AVATAR2_Y, AVATAR_SPEED_X, AVATAR_SPEED_Y);
    player2_icons[5] = create_sprite(superman_xpm, AVATAR2_X, AVATAR2_Y, AVATAR_SPEED_X, AVATAR_SPEED_Y);
    player2_icons[6] = create_sprite(wolverine_xpm, AVATAR2_X, AVATAR2_Y, AVATAR_SPEED_X, AVATAR_SPEED_Y);

    /*
     * Letters sprites
     */
    alphabet_sprites[0] = create_sprite(a_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[1] = create_sprite(b_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[2] = create_sprite(c_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[3] = create_sprite(d_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[4] = create_sprite(e_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[5] = create_sprite(f_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[6] = create_sprite(g_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[7] = create_sprite(h_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[8] = create_sprite(i_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[9] = create_sprite(j_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[10] = create_sprite(k_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[11] = create_sprite(l_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[12] = create_sprite(m_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[13] = create_sprite(n_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[14] = create_sprite(o_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[15] = create_sprite(p_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[16] = create_sprite(q_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[17] = create_sprite(r_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[18] = create_sprite(s_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[19] = create_sprite(t_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[20] = create_sprite(u_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[21] = create_sprite(v_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[22] = create_sprite(w_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[23] = create_sprite(x_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[24] = create_sprite(y_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[25] = create_sprite(z_xpm, DEFAULT_XY, PLAYER_NAME_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);

}

void draw_name_player1(){
    for(int i = 0; i < player1_name_size; i++){
        if(player1_name[i] == SPACE_KEY) continue;
        draw_sprite_in(PLAYER1_NAME_INITIAL_X + i * (SPACE_BETWEEN_LETTERS + LETTER_WIDTH), PLAYER_NAME_Y, alphabet_sprites[player1_name[i]], avatar_double_buffer);
    }
}
void draw_name_player2(){
    for(int i = 0; i < player2_name_size; i++){
        if(player2_name[i] == SPACE_KEY) continue;
        draw_sprite_in(PLAYER2_NAME_INITIAL_X + i * (SPACE_BETWEEN_LETTERS + LETTER_WIDTH), PLAYER_NAME_Y,alphabet_sprites[player2_name[i]], avatar_double_buffer);
    }
}
void (*draw_player_name[])() = {draw_name_player1, draw_name_player2};

void player1_add_letter(int index){
    if(player1_name_size < NAME_MAX_LENGTH){    //Only adds if the player name size if less than the name max size
        player1_name[player1_name_size] = index;
        erase_sprite(mouse_cursor, avatar_double_buffer);
        draw_sprite_in(PLAYER1_NAME_INITIAL_X + player1_name_size * (LETTER_WIDTH + SPACE_BETWEEN_LETTERS), PLAYER_NAME_Y, alphabet_sprites[index], avatar_double_buffer);
        draw_sprite(mouse_cursor, avatar_double_buffer);    //Avoids the mouse from getting behind the letters
        if(replace1_size == player1_name_size){     //Allocates memory if it is necessary
            replace_color_1 = (uint32_t **)realloc(replace_color_1, sizeof(uint32_t *) * (player1_name_size + 1));
            replace1_size++;
            replace_color_1[player1_name_size] = (uint32_t *)malloc(sizeof(alphabet_sprites[index]->map));
        }
        replace_color_1[player1_name_size] = alphabet_sprites[index]->replace_color;    //Update replace, this will be used if it is necessary to remove some letter
        player1_name_size++;
    }
}
void player2_add_letter(int index){
    if(player2_name_size < NAME_MAX_LENGTH){    //Only adds if the player name size if less than the name max size
        player2_name[player2_name_size] = index;
        erase_sprite(mouse_cursor, avatar_double_buffer);
        draw_sprite_in(PLAYER2_NAME_INITIAL_X + player2_name_size * (LETTER_WIDTH + SPACE_BETWEEN_LETTERS), PLAYER_NAME_Y,alphabet_sprites[index], avatar_double_buffer);
        draw_sprite(mouse_cursor, avatar_double_buffer);    //Avoids the mouse from getting behind the letters
        if(replace2_size == player2_name_size){     //Allocates memory if it is necessary
            replace_color_2 = (uint32_t **)realloc(replace_color_2, sizeof(uint32_t *) * (player2_name_size + 1));
            replace2_size++;
            replace_color_2[player2_name_size] = (uint32_t *)malloc(sizeof(alphabet_sprites[index]->map));
        }
        replace_color_2[player2_name_size] = alphabet_sprites[index]->map;      //Update replace, this will be used if it is necessary to remove some letter
        player2_name_size++;

    }
}
void (*player_add_letter[])(int) = {player1_add_letter, player2_add_letter};

void player1_remove_letter(){
    player1_name_size--;
    select_avatar_draw_actual_sprites(select_avatar_cursor_button(mouse_cursor->x, mouse_cursor->y));
    draw_sprite(mouse_cursor, avatar_double_buffer);
}
void player2_remove_letter(){
    player2_name_size--;
    select_avatar_draw_actual_sprites(select_avatar_cursor_button(mouse_cursor->x, mouse_cursor->y));
    draw_sprite_in(mouse_cursor->x, mouse_cursor->y, mouse_cursor, avatar_double_buffer);
}

int select_avatar_cursor_button(uint16_t x, uint16_t y){
    if(between(y, BACK_FRONT_Y, BACK_FRONT_Y + BACK_FRONT_WH)){                 //All this buttons have the same y
        if(between(x, BACK1_X, BACK1_X + BACK_FRONT_WH)) return MOUSE_BACK1;
        else if(between(x, FRONT1_X, FRONT1_X + BACK_FRONT_WH)) return MOUSE_FRONT1;
        else if(between(x, BACK2_X, BACK2_X + BACK_FRONT_WH)) return MOUSE_BACK2;
        else if(between(x, FRONT2_X, FRONT2_X + BACK_FRONT_WH)) return MOUSE_FRONT2;
    }
    else if(between(y, READY_Y, READY_Y + background[MOUSE_READY1]->height)){
        if(between(x, READY1_X, READY1_X + background[MOUSE_READY1]->width)) return MOUSE_READY1;
        else if(between(x, READY2_X, READY2_X + background[MOUSE_READY1]->width)) return MOUSE_READY2;
    }
    else if(between(y, PLAYER_NAME_Y, PLAYER_NAME_Y + LETTER_HEIGHT)){
        if(between(x, PLAYER1_NAME_INITIAL_X, PLAYER1_NAME_INITIAL_X + NAME_MAX_LENGTH * LETTER_WIDTH + (NAME_MAX_LENGTH - 1) * SPACE_BETWEEN_LETTERS))
            return NAME_PLAYER1;
        else if(between(x, PLAYER2_NAME_INITIAL_X, PLAYER2_NAME_INITIAL_X + NAME_MAX_LENGTH * LETTER_WIDTH + (NAME_MAX_LENGTH - 1) * SPACE_BETWEEN_LETTERS))
            return NAME_PLAYER2;  
    }
    return MOUSE_BG;
}

int select_avatar_menu(char * video_mem, char * double_buffer, int *final_player1_icon, int * final_player2_icon){
    player1_icon = final_player1_icon;
    player2_icon = final_player2_icon;
    avatar_video_mem = video_mem;
    avatar_double_buffer = double_buffer;

    /*Initialize variables to start the funtion*/
    replace1_size = 0; replace2_size = 0;
    player1_name_size = 0; player2_name_size = 0;
    player1_ready = false; player2_ready= false;
    who = NOBODY;
    select_avatar_prev_index = 0; select_avatar_prev_event = MOUSE_MOVE;


    uint32_t timer_counter = 0; int end = SELECT, valid_reading = KBD_VALID_READING;
    uint8_t keyboard_data; int kbd_index = 0;


    select_avatar_load_sprites();
    select_avatar_draw_actual_sprites(0);
    mouse_cursor->x = vbe_info.XResolution / 2;
    mouse_cursor->y = vbe_info.YResolution / 2 - 20;
    draw_sprite_in(vbe_info.XResolution / 2, vbe_info.YResolution / 2 - 20, mouse_cursor, avatar_double_buffer);

    struct packet mouse_packet;
    message msg;
    int r, ipc_status, pCounter = 0;
    uint8_t mouse_data_byte;

    uint32_t mouse_irq_set = BIT(mouse_bit_number), timer_irq_set = BIT(timer_bit_number), keyboard_irq_set = BIT(keyboard_bit_number);

    while(end != LEAVE && end != PLAY){
        if((r = driver_receive(ANY, &msg, &ipc_status)) != OK) continue;
        if(is_ipc_notify(ipc_status)){
            switch(_ENDPOINT_P(msg.m_source)){
                case HARDWARE:
                    if(msg.m_notify.interrupts & keyboard_irq_set){

                        valid_reading = keyboard_interrupt_handler(&keyboard_data);
                        if(keyboard_data == TWO_BYTES_CODE){
                            valid_reading = keyboard_interrupt_handler(&keyboard_data);
                        }

                        switch(valid_reading){
                            case KBD_VALID_READING:
                                kbd_index = keyboard_recognizeScanCode(keyboard_data);
                                if(who == NOBODY){
                                    if(kbd_index == ESC_KEY){ end = LEAVE; continue;}   //if no one is writing, the esc key can be used to back to the main menu
                                }
                                else{
                                    if(between(kbd_index, 0, ALPHABET_LENGTH)) player_add_letter[who](kbd_index);   //Normal letter
                                    else if(kbd_index == ESC_KEY) who = NOBODY;     //Stop writing
                                    else if(kbd_index == SPACE_KEY){    //Only adds a space key to the player name if the player name size is less than the name max length
                                        if(who == player_1 && player1_name_size < NAME_MAX_LENGTH){
                                            player1_name[player1_name_size] = SPACE_KEY;                                        
                                            player1_name_size++;
                                        }else if(who == player_2 && player2_name_size < NAME_MAX_LENGTH){
                                                player2_name[player2_name_size] = SPACE_KEY;                                        
                                                player2_name_size++;
                                        }
                                    }
                                    else if(kbd_index == BACK_SPACE_KEY){   //Only removes a letter if the size of the player name is different of 0
                                        if(who == player_1 && player1_name_size){
                                            player1_remove_letter();
                                        }
                                        else if(who == player_2 && player2_name_size){
                                            player2_remove_letter();
                                        }
                                    }
                                    else if(kbd_index == ENTER_KEY){    //If the player presses enter when is writing, it is ready to play
                                        if(who == player_1 && player1_name_size != 0){
                                            player1_ready = true;
                                            draw_sprite_in(READY1_X, READY_Y, background[4], avatar_double_buffer);
                                            who = NOBODY;
                                        }
                                        else if(who == player_2 && player2_name_size != 0){
                                            player2_ready = true;
                                            draw_sprite_in(READY2_X, READY_Y, background[4], avatar_double_buffer);
                                            who = NOBODY;
                                        }
                                    }
                                }
                                break;
                            case KBD_EMPTY_BUFFER:
                                continue;
                            case KBD_INVALID_READING:
                                return 1;
                        }
                    }
                    if(msg.m_notify.interrupts & mouse_irq_set){
                        switch(mouse_interrupt_handler(&mouse_data_byte)){
                            case MOUSE_VALID_READING:
                                if(mouse_packet_assemble(&pCounter, &mouse_packet, &mouse_data_byte) != OK) return 1;
                                if(pCounter == 3) {     //Avoids the three bytes from not being processed
                                    select_avatar_state_machine(detect_event(&mouse_packet), &mouse_packet);
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
                        refresh_avatar_double_buffer(); //Double buffering
                        timer_counter++;
                    }
                    break;
                default:
                    break;
            }
        }
        if(end == SELECT && player1_ready && player2_ready)
            end = PLAY;

    }
    if(end == PLAY) refresh_avatar_double_buffer(); //update both ready buttons 

    // Free the space used by the sprites
    for(int i = 0; i < 7; i++){ destroy_sprite(background[i]); destroy_sprite(player1_icons[i]); destroy_sprite(player2_icons[i]);}
    for(int i = 0; i < ALPHABET_LENGTH; i++){destroy_sprite(alphabet_sprites[i]);}
    free(replace_color_1); free(replace_color_2);
    return end;
}

void select_avatar_state_machine(enum mouse_event event, struct packet *pp){
    int index = select_avatar_cursor_button(mouse_cursor->x, mouse_cursor->y);
    if(index != select_avatar_prev_index){  //It means that the cursor is in a different button, the buffer should be updated
        select_avatar_draw_actual_sprites(index);
        select_avatar_prev_index = index;   //To track the cursor position
    }
    switch(event){
        case MOUSE_MOVE:
            move_cursor(pp, avatar_double_buffer);
            break;
        case L_PRESSED:
            if(select_avatar_prev_event != L_PRESSED){ select_avatar_start_x = mouse_cursor->x; select_avatar_start_y = mouse_cursor->y;}
            move_cursor(pp, avatar_double_buffer);
            break;
        case L_RELEASED:
            who = NOBODY; //default;
            if(select_avatar_prev_event == L_PRESSED){ //check if the position of the cursor is on a button and the initial press was done inside the corresponding button
                int index = select_avatar_cursor_button(mouse_cursor->x, mouse_cursor->y);
                switch(index){
                    case MOUSE_READY1:
                        if(player1_ready){
                            draw_sprite_in(READY1_X, READY_Y, background[1], avatar_double_buffer);
                            draw_sprite_in(mouse_cursor->x, mouse_cursor->y, mouse_cursor, avatar_double_buffer);

                        }
                        else if(player1_name_size != 0 && player1_name[0] != SPACE_KEY){    //It can only be ready if the first letter is different from a space
                            draw_sprite_in(READY1_X, READY_Y, background[4], avatar_double_buffer);
                            draw_sprite_in(mouse_cursor->x, mouse_cursor->y, mouse_cursor, avatar_double_buffer);
                        }
                        player1_ready = !player1_ready;
                        break;
                    case MOUSE_READY2:
                        if(player2_ready){
                            draw_sprite_in(READY2_X, READY_Y, background[1], avatar_double_buffer);
                            draw_sprite_in(mouse_cursor->x, mouse_cursor->y, mouse_cursor, avatar_double_buffer);
                        }
                        else if(player2_name_size != 0 && player2_name[0] != SPACE_KEY){    //It can only be ready if the first letter is different from a space
                            draw_sprite_in(READY2_X, READY_Y, background[4], avatar_double_buffer);
                            draw_sprite_in(mouse_cursor->x, mouse_cursor->y, mouse_cursor, avatar_double_buffer);
                        }
                        player2_ready = !player2_ready;
                        break;
                    case MOUSE_FRONT1:  //The first parameter tells to the function if the movement is to the right, the second if the movement is from the first player
                        if(!player1_ready) *player1_icon = move_sprite(true, true);
                        break;
                    case MOUSE_BACK1:
                        if(!player1_ready) *player1_icon = move_sprite(false, true);
                        break;
                    case MOUSE_FRONT2:
                        if(!player2_ready) *player2_icon = move_sprite(true, false);
                        break;
                    case MOUSE_BACK2:
                        if(!player2_ready) *player2_icon = move_sprite(false, false);
                        break;
                    case NAME_PLAYER1:
                        who = player_1;
                        break;
                    case NAME_PLAYER2:
                        who = player_2;
                        break;
                    default:
                        break;
                }
            }
            break;
        default:
            move_cursor(pp, avatar_double_buffer);
            break;
    }
    select_avatar_prev_event = event;
}

void refresh_avatar_double_buffer(){    //Double buffering
    memcpy(avatar_video_mem, avatar_double_buffer, vbe_info.XResolution * vbe_info.YResolution * ceil(vbe_info.BitsPerPixel / 8.0));
}

int move_sprite(bool right, bool player1){
    int prev_x, next_x, prev_x_old, next_x_old, xf, next_sprite, n_bytes = ceil(vbe_info.BitsPerPixel / 8.0);
    erase_sprite(mouse_cursor, avatar_double_buffer); //Give all the attention to the movement
    if(player1){
        next_sprite = (right) ? ((*player1_icon + 1) % 7):((*player1_icon - 1 + 7) % 7);
        prev_x = (right) ? AVATAR1_X:AVATAR1_X + player1_icons[*player1_icon]->width; //If the movement is to the right the x used is AVATAR1_X (the leftmost x of the sprite), otherwise is the biggest x
        next_x = (right) ? AVATAR_PLAYER1_INITIAL_X:AVATAR_PLAYER1_FINAL_X; //If the movement is to the right the x used is the beginning of the box, otherwise is the final of the box
        xf = (right) ? AVATAR_PLAYER1_FINAL_X:AVATAR_PLAYER1_INITIAL_X; //The final position depends of the type of movement
        while(prev_x != xf){
            prev_x_old = prev_x;
            next_x_old = next_x;
            if (right) { 
                clear_moved_avatar(prev_x_old, AVATAR1_Y, player1_icons[*player1_icon], right, AVATAR_PLAYER1_FINAL_X, true);
                clear_moved_avatar(next_x_old, AVATAR1_Y, player1_icons[*player1_icon], right, AVATAR_PLAYER1_INITIAL_X, false); 
                prev_x += player1_icons[*player1_icon]->xspeed;
                next_x += player1_icons[*player1_icon]->xspeed;
                if (prev_x >= AVATAR_PLAYER1_FINAL_X){ //The movement is over
                    draw_sprite_in(AVATAR1_X, AVATAR1_Y, player1_icons[next_sprite], avatar_double_buffer);
                    draw_sprite(mouse_cursor, avatar_double_buffer);
                    break;
                }
                move_prev_right(prev_x, AVATAR1_Y, AVATAR_PLAYER1_FINAL_X, player1_icons[*player1_icon], n_bytes); //The previous sprite moves from the normal position of the icon to the right of the box
                move_next_right(next_x, AVATAR1_Y, AVATAR_PLAYER1_INITIAL_X, player1_icons[next_sprite], n_bytes); //The next sprite moves from the beginning of the box to the normal position of the icon
            }
            else {
                clear_moved_avatar(prev_x_old, AVATAR1_Y, player1_icons[*player1_icon], right, AVATAR_PLAYER1_INITIAL_X, true);
                clear_moved_avatar(next_x_old, AVATAR1_Y, player1_icons[*player1_icon], right, AVATAR_PLAYER1_FINAL_X, false);
                prev_x -= player1_icons[*player1_icon]->xspeed;
                next_x -= player1_icons[*player1_icon]->xspeed; 
                if (prev_x  <= xf){ //The movement is over
                    draw_sprite_in(AVATAR1_X, AVATAR1_Y, player1_icons[next_sprite], avatar_double_buffer);
                    draw_sprite(mouse_cursor, avatar_double_buffer);
                    break;
                }
                move_prev_left(prev_x, AVATAR1_Y, AVATAR_PLAYER1_INITIAL_X, player1_icons[*player1_icon], n_bytes);//The previous sprite moves from the normal position of the icon to the beginning of the box
                move_next_left(next_x, AVATAR1_Y, AVATAR_PLAYER1_FINAL_X, player1_icons[next_sprite], n_bytes);//The next sprite moves from the end of the box to the normal position of the icon
            }
            refresh_avatar_double_buffer();
        }
    }else{ //Do the same for the second player
        next_sprite = (right) ? ((*player2_icon + 1) % 7):((*player2_icon - 1 + 7) % 7);
        prev_x = (right) ? AVATAR2_X:AVATAR2_X + player2_icons[*player2_icon]->width;
        next_x = (right) ? AVATAR_PLAYER2_INITIAL_X:AVATAR_PLAYER2_FINAL_X;
        xf = (right) ? AVATAR_PLAYER2_FINAL_X:AVATAR_PLAYER2_INITIAL_X;
        while(prev_x != xf){
            prev_x_old = prev_x;
            next_x_old = next_x;
            if (right) { 
                clear_moved_avatar(prev_x_old, AVATAR2_Y, player2_icons[*player2_icon], right, AVATAR_PLAYER2_FINAL_X, true);
                clear_moved_avatar(next_x_old, AVATAR2_Y, player2_icons[*player2_icon], right, AVATAR_PLAYER2_INITIAL_X, false);
                prev_x += player2_icons[*player2_icon]->xspeed;
                next_x += player2_icons[*player2_icon]->xspeed;
                if (prev_x >= AVATAR_PLAYER2_FINAL_X){
                    draw_sprite_in(AVATAR2_X, AVATAR2_Y, player2_icons[next_sprite], avatar_double_buffer);
                    draw_sprite(mouse_cursor, avatar_double_buffer);
                    break;
                }
                move_prev_right(prev_x, AVATAR2_Y, AVATAR_PLAYER2_FINAL_X, player2_icons[*player2_icon], n_bytes);
                move_next_right(next_x, AVATAR2_Y, AVATAR_PLAYER2_INITIAL_X, player2_icons[next_sprite], n_bytes);
            }
            else {
                clear_moved_avatar(prev_x_old, AVATAR2_Y, player2_icons[*player2_icon], right, AVATAR_PLAYER2_INITIAL_X, true);
                clear_moved_avatar(next_x_old, AVATAR2_Y, player2_icons[*player2_icon], right, AVATAR_PLAYER2_FINAL_X, false);                
                prev_x -= player2_icons[*player2_icon]->xspeed;
                next_x -= player2_icons[*player2_icon]->xspeed; 
                if (prev_x <= xf){
                    draw_sprite_in(AVATAR2_X, AVATAR2_Y, player2_icons[next_sprite], avatar_double_buffer);
                    draw_sprite(mouse_cursor, avatar_double_buffer);
                    break;
                }
                move_prev_left(prev_x, AVATAR2_Y, AVATAR_PLAYER2_INITIAL_X, player2_icons[*player2_icon], n_bytes);
                move_next_left(next_x, AVATAR2_Y, AVATAR_PLAYER2_FINAL_X, player2_icons[next_sprite], n_bytes);
            }
            refresh_avatar_double_buffer();
        }
    }
    return next_sprite;
}

void clear_moved_avatar(uint16_t x, uint16_t y, Sprite * sp, bool right, uint16_t limit, bool prev) { //Clears the icon sprite with a white rectangle
    if (right){ 
        if(prev) video_card_draw_rectangle(x, y, (limit - x < sp->width) ? (limit - x):sp->width, sp->height, WHITE, avatar_double_buffer);
        else {
            uint16_t width = (x - limit < sp->width) ? (x- limit):sp->width;
            video_card_draw_rectangle(x - width, y, width, sp->height, WHITE, avatar_double_buffer);
        }
    }
    else{
        if(prev) video_card_draw_rectangle(limit, y, x - limit, sp->height, WHITE, avatar_double_buffer);
        else video_card_draw_rectangle(x, y, limit - x, sp->height, WHITE, avatar_double_buffer);
    }
}

void move_prev_right(uint16_t x, uint16_t y, uint16_t limit, Sprite * sp, int number_bytes_per_pixel){
    uint16_t width = sp->width;
    if(limit - x < width) width = limit - x; //Adjusts the width if it is necessary to 

    char * board = avatar_double_buffer + ((vbe_info.XResolution * y) + x) * number_bytes_per_pixel;


    for(int i = 0; i < sp->height; i++){
        for(int j = 0; j < width; j++){
            for (int k = 0; k < number_bytes_per_pixel - 1; k++) {
                if(sp->map[i*sp->width + j] != xpm_transparency_color(XPM_8_8_8_8))
                    board[k] = sp->map[i*sp->width + j] >> (k * 8);
            }
            board += number_bytes_per_pixel;
        }
        board += (vbe_info.XResolution - width) * number_bytes_per_pixel;
    }
}
void move_next_left(uint16_t x, uint16_t y, uint16_t limit, Sprite * sp, int number_bytes_per_pixel){
    uint16_t width = sp->width;
    if(limit - x < width) width = limit - x; //Adjusts the width if it is necessary to 

    char * board = avatar_double_buffer + ((vbe_info.XResolution * y) + x) * number_bytes_per_pixel;

    for(int i = 0; i < sp->height; i++){
        for(int j = 0; j < width; j++){
            for (int k = 0; k < number_bytes_per_pixel - 1; k++) {
                if(sp->map[i*sp->width + j] != xpm_transparency_color(XPM_8_8_8_8))
                    board[k] = sp->map[i*sp->width + j] >> (k * 8);
            }
            board += number_bytes_per_pixel;
        }
        board += (vbe_info.XResolution - width) * number_bytes_per_pixel;
    }
}

//x used is the rightmost pixel of the sprite, draws the rightmost part of the sprite when it is necessary to
void move_prev_left(uint16_t x, uint16_t y, uint16_t limit, Sprite * sp, int number_bytes_per_pixel){
    uint16_t width = sp->width;
    int initial_j;
    if(x - limit < width){ //Adjusts the width if it is necessary to 
        width = (x - limit);
        x = limit;
        initial_j = width;
    }
    else{
        x -= width;
        initial_j = 0; //Draws the entire sprite
    }
    char * board = avatar_double_buffer + ((vbe_info.XResolution * y) + x) * number_bytes_per_pixel;

    for(int i = 0; i < sp->height; i++){
        for(int j = 0; j < width; j++){
            for (int k = 0; k < number_bytes_per_pixel - 1; k++) {
                if(sp->map[i*sp->width + j + (sp->width - width)] != xpm_transparency_color(XPM_8_8_8_8))
                    board[k] = sp->map[i*sp->width + j + (sp->width - width)] >> (k * 8);
            }
            board += number_bytes_per_pixel;
        }
        board += (vbe_info.XResolution - width) * number_bytes_per_pixel;
    }
}
void move_next_right(uint16_t x, uint16_t y, uint16_t limit, Sprite * sp, int number_bytes_per_pixel){
    uint16_t width = sp->width;
    if(x - limit < width){ //Adjusts the width if it is necessary to 
        width = x - limit;
        x = limit;
    }else x = x - width;

    char * board = avatar_double_buffer + ((vbe_info.XResolution * y) + x) * number_bytes_per_pixel;

    for(int i = 0; i < sp->height; i++){
        for(int j = sp->width -  width; j < sp->width; j++){
            for (int k = 0; k < number_bytes_per_pixel - 1; k++) {
                if(sp->map[i*sp->width + j] != xpm_transparency_color(XPM_8_8_8_8))
                    board[k] = sp->map[i*sp->width + j] >> (k * 8);
            }
            board += number_bytes_per_pixel;
        }
        board += (vbe_info.XResolution - width) * number_bytes_per_pixel;
    }
}

void select_avatar_draw_actual_sprites(int index){

    draw_sprite(background[0], avatar_double_buffer);
    draw_sprite(player1_icons[*player1_icon], avatar_double_buffer);
    draw_sprite(player2_icons[*player2_icon], avatar_double_buffer);

    if(player1_ready) draw_sprite_in(READY1_X, READY_Y, background[4], avatar_double_buffer);
    else draw_sprite_in(READY1_X, READY_Y, background[1], avatar_double_buffer);

    if(player2_ready) draw_sprite_in(READY2_X, READY_Y, background[4], avatar_double_buffer);
    else draw_sprite_in(READY2_X, READY_Y, background[1], avatar_double_buffer);

    draw_player_name[0](avatar_double_buffer); draw_player_name[1](avatar_double_buffer);

    if(index != MOUSE_BACK1) draw_sprite_in(BACK1_X, BACK_FRONT_Y,background[3], avatar_double_buffer);
    else draw_sprite_in(BACK1_X, BACK_FRONT_Y,background[6], avatar_double_buffer);

    if(index != MOUSE_FRONT1) draw_sprite_in(FRONT1_X, BACK_FRONT_Y,background[2], avatar_double_buffer);
    else draw_sprite_in(FRONT1_X, BACK_FRONT_Y,background[5], avatar_double_buffer);

    if(index != MOUSE_BACK2) draw_sprite_in(BACK2_X, BACK_FRONT_Y,background[3], avatar_double_buffer);
    else draw_sprite_in(BACK2_X, BACK_FRONT_Y,background[6], avatar_double_buffer);

    if(index != MOUSE_FRONT2) draw_sprite_in(FRONT2_X, BACK_FRONT_Y,background[2], avatar_double_buffer);
    else draw_sprite_in(FRONT2_X, BACK_FRONT_Y,background[5], avatar_double_buffer);
    
    for (int i = 0; i < mouse_cursor->height; i++) {
        for (int j = 0; j < mouse_cursor->width; j++) {
            mouse_cursor->replace_color[i * mouse_cursor->width + j] = video_card_get_pixel_color(avatar_double_buffer, mouse_cursor->x + j, mouse_cursor->y + i );
        }
    }
}
