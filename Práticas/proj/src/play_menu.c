#include "play_menu.h"

extern Sprite * mouse_cursor;
extern vbe_mode_info_t vbe_info;
extern const char * mouse_cursor_xpm[];
extern const char * background_xpm[];
extern const char * front_pressed_xpm[];
extern const char * front_not_pressed_xpm[];
extern const char * back_pressed_xpm[];
extern const char * back_not_pressed_xpm[];
extern const char * pause_pressed_xpm[];
extern const char * pause_not_pressed_xpm[];
extern const char * pause_leave_game_xpm[];
extern const char * pause_resume_xpm[];
extern const char * pause__xpm[];
extern const char * give_up_pressed_xpm[];
extern const char * give_up_not_pressed_xpm[];
extern const char * begin_play_xpm[];
extern const char * done_not_pressed_xpm[];
extern const char * done_pressed_xpm[];
extern const char * EndGame_xpm[];

extern const char * Brush_size_not_p_xpm[];
extern const char * Brush_size_small_p_xpm[];
extern const char * Brush_size_medium_p_xpm[];
extern const char * Brush_size_large_p_xpm[];

extern const char * clock_0_xpm[];
extern const char * clock_1_xpm[];
extern const char * clock_2_xpm[];
extern const char * clock_3_xpm[];
extern const char * clock_4_xpm[];
extern const char * clock_5_xpm[];
extern const char * clock_6_xpm[];
extern const char * clock_7_xpm[];

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

extern const char * caveman_icon_xpm[];
extern const char * cyborg_icon_xpm[];
extern const char * deadcyber_icon_xpm[];
extern const char * devil_icon_xpm[];
extern const char * knight_icon_xpm[];
extern const char * avo_icon_xpm[];
extern const char * mummy_icon_xpm[];
extern const char * ninja_icon_xpm[];
extern const char * pirate_icon_xpm[];
extern const char * santa_claus_icon_xpm[];
extern const char * spiderman_icon_xpm[];
extern const char * spy_icon_xpm[];
extern const char * superman_icon_xpm[];
extern const char * wolverine_icon_xpm[];

extern const char * n0_xpm[];
extern const char * n1_xpm[];
extern const char * n2_xpm[];
extern const char * n3_xpm[];
extern const char * n4_xpm[];
extern const char * n5_xpm[];
extern const char * n6_xpm[];
extern const char * n7_xpm[];
extern const char * n8_xpm[];
extern const char * n9_xpm[];

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

extern const char * traco_xpm[];
extern const char * RightAnswer_xpm[];
extern const char * WrongAnswer_xpm[];
extern const char * block_xpm[];

extern uint8_t mouse_bit_number;
extern uint8_t timer_bit_number;
extern uint8_t keyboard_bit_number;

extern int player1_name[];
extern int player2_name[];
extern int player1_name_size; 
extern int player2_name_size;
extern int actual_time;
extern int actual_velocity;


Sprite * sprites[7];
Sprite * sprites_pause_menu[4];
Sprite * sprites_painting_tools[10];
Sprite * clock_sprites[8];
Sprite * play_mini_menu[11];
Sprite * player1_icons[7];
Sprite * player2_icons[7];
Sprite * numbers[10];
Sprite * alphabet_sprites[ALPHABET_LENGTH + 1];  // + 1 because of the underscore used when it is time to guess the word
Sprite * chooseWord_menu, * blocker;

char * play_video_mem, * canvas_buf, * play_double_buffer;

int curr_player1_icon, curr_player2_icon, _actual_menu = DONE, score_1, score_2, game_word_size,
      word1_length, word2_length, numberWords, play_prev_index, actual_clock, guess_size, replace_size,
        actual_round, actual_game_status = CHOOSE_WORD, last_game_status = CHOOSE_WORD, actual_gv;

// Keep tracking of the current tools
int current_tool = BRUSH, current_color = 0x00000000, current_brush_size = MEDIUM;

int menus[] = {0, 2, 6}; //array with the index of the sprites of the different menus
int * word1, * word2, * gameWord, guessWord[9];
int ** words;
uint32_t ** replace_color_guess;
bool player1_draw, play_prev_event, play_again;
uint16_t play_start_pos_x, play_start_pos_y, first_guess_x, final_position_y = CANVAS_X + CANVAS_WIDHT;
uint8_t actual_gt, game_time = 0;

enum movement actual_movement = RIGHT, last_movement = UP;

int  (* cursor_button[2])(uint16_t, uint16_t) = {play_menu_cursor_button, pause_menu_cursor_button};
void (*draw_actual_menu[3])(int) = {draw_done, draw_brush_size, draw_give_up};
void (* draw_actual[5])(int) = {draw_pause_sprites, draw_actual_mouse_in_buttons, draw_actual_mouse_in_tools, draw_actual_mouse_in_menu, draw_actual_end};
void (* move_blocker_func[4])() = {move_blocker_down, move_blocker_up, move_blocker_right, move_blocker_left};

void load_sprites(){

    /*
     * Sprites of the play menu
     */
    blocker = create_sprite(block_xpm, CANVAS_X, CANVAS_Y, actual_velocity, actual_velocity);
    sprites[0] = create_sprite(background_xpm, 0, 0, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites[1] = create_sprite(front_not_pressed_xpm, FRONT_X, FRONT_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites[2] = create_sprite(back_not_pressed_xpm, BACK_X, BACK_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites[3] = create_sprite(pause_not_pressed_xpm, PAUSE_X, PAUSE_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites[4] = create_sprite(front_pressed_xpm, FRONT_X, FRONT_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites[5] = create_sprite(back_pressed_xpm, BACK_X, BACK_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites[6] = create_sprite(pause_pressed_xpm, PAUSE_X, PAUSE_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);

    /*
     * Sprites of the play mini menu
     */ 
    chooseWord_menu = create_sprite(begin_play_xpm, MINI_MENU_X, MINI_MENU_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    play_mini_menu[0] = create_sprite(done_not_pressed_xpm, MINI_MENU_X, MINI_MENU_Y, MENU_SPEED, STATIC_SPRITE_SPEED);
    play_mini_menu[1] = create_sprite(done_pressed_xpm, MINI_MENU_X, MINI_MENU_Y, MENU_SPEED, STATIC_SPRITE_SPEED);
    play_mini_menu[2] = create_sprite(Brush_size_not_p_xpm, MINI_MENU_X, MINI_MENU_Y, MENU_SPEED, STATIC_SPRITE_SPEED);
    play_mini_menu[3] = create_sprite(Brush_size_small_p_xpm, MINI_MENU_X, MINI_MENU_Y, MENU_SPEED, STATIC_SPRITE_SPEED);
    play_mini_menu[4] = create_sprite(Brush_size_medium_p_xpm, MINI_MENU_X, MINI_MENU_Y, MENU_SPEED, STATIC_SPRITE_SPEED);
    play_mini_menu[5] = create_sprite(Brush_size_large_p_xpm, MINI_MENU_X, MINI_MENU_Y, MENU_SPEED, STATIC_SPRITE_SPEED);
    play_mini_menu[6] = create_sprite(give_up_not_pressed_xpm, MINI_MENU_X, MINI_MENU_Y, MENU_SPEED, STATIC_SPRITE_SPEED);
    play_mini_menu[7] = create_sprite(give_up_pressed_xpm, MINI_MENU_X, MINI_MENU_Y, MENU_SPEED, STATIC_SPRITE_SPEED);
    play_mini_menu[8] = create_sprite(EndGame_xpm, MINI_MENU_X, MINI_MENU_Y, MENU_SPEED, STATIC_SPRITE_SPEED);
    play_mini_menu[9] = create_sprite(WrongAnswer_xpm, STATUS_ANSWER_X, STATUS_ANSWER_Y, MENU_SPEED, STATIC_SPRITE_SPEED);
    play_mini_menu[10] = create_sprite(RightAnswer_xpm, STATUS_ANSWER_X, STATUS_ANSWER_Y, MENU_SPEED, STATIC_SPRITE_SPEED);

    /*
     * Sprites of pause menu
     */
    sprites_pause_menu[0] = create_sprite(background_xpm, 0, 0, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED); //mudar
    sprites_pause_menu[1] = create_sprite(pause__xpm, MINI_BOX_X, MINI_BOX_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites_pause_menu[2] = create_sprite(pause_resume_xpm, MINI_BOX_X, MINI_BOX_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    sprites_pause_menu[3] = create_sprite(pause_leave_game_xpm, MINI_BOX_X, MINI_BOX_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    
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

    /*
     * Possible icons to player 1
     */
    player1_icons[0] = create_sprite(caveman_icon_xpm, AVATAR_1_ICON_X, AVATAR_ICON_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    player1_icons[1] = create_sprite(cyborg_icon_xpm, AVATAR_1_ICON_X, AVATAR_ICON_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    player1_icons[2] = create_sprite(deadcyber_icon_xpm, AVATAR_1_ICON_X, AVATAR_ICON_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    player1_icons[3] = create_sprite(devil_icon_xpm, AVATAR_1_ICON_X, AVATAR_ICON_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    player1_icons[4] = create_sprite(knight_icon_xpm, AVATAR_1_ICON_X, AVATAR_ICON_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    player1_icons[5] = create_sprite(avo_icon_xpm, AVATAR_1_ICON_X, AVATAR_ICON_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    player1_icons[6] = create_sprite(mummy_icon_xpm, AVATAR_1_ICON_X, AVATAR_ICON_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);

    /*
     * Possible icons to player 2
     */
    player2_icons[0] = create_sprite(ninja_icon_xpm, AVATAR_2_ICON_X, AVATAR_ICON_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    player2_icons[1] = create_sprite(pirate_icon_xpm, AVATAR_2_ICON_X, AVATAR_ICON_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    player2_icons[2] = create_sprite(santa_claus_icon_xpm, AVATAR_2_ICON_X, AVATAR_ICON_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    player2_icons[3] = create_sprite(spiderman_icon_xpm, AVATAR_2_ICON_X, AVATAR_ICON_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    player2_icons[4] = create_sprite(spy_icon_xpm, AVATAR_2_ICON_X, AVATAR_ICON_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    player2_icons[5] = create_sprite(superman_icon_xpm, AVATAR_2_ICON_X, AVATAR_ICON_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    player2_icons[6] = create_sprite(wolverine_icon_xpm, AVATAR_2_ICON_X, AVATAR_ICON_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);

    /*
     * Numbers
     */
    numbers[0] = create_sprite(n0_xpm, DEFAULT_XY, NUMBERS_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    numbers[1] = create_sprite(n1_xpm, DEFAULT_XY, NUMBERS_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    numbers[2] = create_sprite(n2_xpm, DEFAULT_XY, NUMBERS_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    numbers[3] = create_sprite(n3_xpm, DEFAULT_XY, NUMBERS_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    numbers[4] = create_sprite(n4_xpm, DEFAULT_XY, NUMBERS_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    numbers[5] = create_sprite(n5_xpm, DEFAULT_XY, NUMBERS_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    numbers[6] = create_sprite(n6_xpm, DEFAULT_XY, NUMBERS_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    numbers[7] = create_sprite(n7_xpm, DEFAULT_XY, NUMBERS_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    numbers[8] = create_sprite(n8_xpm, DEFAULT_XY, NUMBERS_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    numbers[9] = create_sprite(n9_xpm, DEFAULT_XY, NUMBERS_Y, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);

    /*
     *Letters sprites
     */
    alphabet_sprites[0] = create_sprite(a_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[1] = create_sprite(b_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[2] = create_sprite(c_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[3] = create_sprite(d_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[4] = create_sprite(e_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[5] = create_sprite(f_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[6] = create_sprite(g_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[7] = create_sprite(h_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[8] = create_sprite(i_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[9] = create_sprite(j_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[10] = create_sprite(k_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[11] = create_sprite(l_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[12] = create_sprite(m_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[13] = create_sprite(n_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[14] = create_sprite(o_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[15] = create_sprite(p_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[16] = create_sprite(q_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[17] = create_sprite(r_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[18] = create_sprite(s_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[19] = create_sprite(t_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[20] = create_sprite(u_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[21] = create_sprite(v_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[22] = create_sprite(w_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[23] = create_sprite(x_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[24] = create_sprite(y_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[25] = create_sprite(z_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    alphabet_sprites[26] = create_sprite(traco_xpm, DEFAULT_XY, DEFAULT_XY, STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);

    /*
     * Clock sprites
     */
    clock_sprites[0] = create_sprite(clock_0_xpm, CLOCK_X, CLOCK_Y,STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    clock_sprites[1] = create_sprite(clock_1_xpm, CLOCK_X, CLOCK_Y,STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    clock_sprites[2] = create_sprite(clock_2_xpm, CLOCK_X, CLOCK_Y,STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    clock_sprites[3] = create_sprite(clock_3_xpm, CLOCK_X, CLOCK_Y,STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    clock_sprites[4] = create_sprite(clock_4_xpm, CLOCK_X, CLOCK_Y,STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    clock_sprites[5] = create_sprite(clock_5_xpm, CLOCK_X, CLOCK_Y,STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    clock_sprites[6] = create_sprite(clock_6_xpm, CLOCK_X, CLOCK_Y,STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);
    clock_sprites[7] = create_sprite(clock_7_xpm, CLOCK_X, CLOCK_Y,STATIC_SPRITE_SPEED, STATIC_SPRITE_SPEED);

}

int play_menu_cursor_button(uint16_t x, uint16_t y) {
    if (between(x, FRONT_X, FRONT_X + PLAY_MENU_BUTTON_SIZE) && between(y, FRONT_Y, FRONT_Y + PLAY_MENU_BUTTON_SIZE)) return MOUSE_FRONT_BUTTON;
    else if (between(x, BACK_X, BACK_X + PLAY_MENU_BUTTON_SIZE) && between(y, BACK_Y, BACK_Y + PLAY_MENU_BUTTON_SIZE)) return MOUSE_BACK_BUTTON;
    else if (between(x, PAUSE_X, PAUSE_X + PLAY_MENU_BUTTON_SIZE) && between(y, PAUSE_Y, PAUSE_Y + PLAY_MENU_BUTTON_SIZE)) return MOUSE_PAUSE_BUTTON;
    else if (between(x, TOOLS_ZOOMED_X, TOOLS_ZOOMED_X + TOOLS_ZOOMED_SIZE) && between(y, BRUSH_Y + 3, BRUSH_Y + TOOLS_ZOOMED_SIZE)) return BRUSH_PRESSED;
    else if (between(x, TOOLS_ZOOMED_X, TOOLS_ZOOMED_X + TOOLS_ZOOMED_SIZE) && between(y, BUCKET_Y, BUCKET_Y + TOOLS_ZOOMED_SIZE)) return BUCKET_PRESSED;
    else if (between(x, TOOLS_ZOOMED_X, TOOLS_ZOOMED_X + TOOLS_ZOOMED_SIZE) && between(y, RUBBER_Y, RUBBER_Y + TOOLS_ZOOMED_SIZE)) return RUBBER_PRESSED;
    else if (between(x, TOOLS_ZOOMED_X, TOOLS_ZOOMED_X + TOOLS_ZOOMED_SIZE) && between(y, COLOR_PICKER_Y, COLOR_PICKER_Y + TOOLS_ZOOMED_SIZE)) return COLOR_PICKER_PRESSED;
    else if (between(x, TOOLS_ZOOMED_X, TOOLS_ZOOMED_X + TOOLS_ZOOMED_SIZE) && between(y, CLEANER_Y - 5, CLEANER_Y + TOOLS_ZOOMED_SIZE)) return CLEANER_PRESSED;
    else if(_actual_menu == BRUSH_SIZE && between(y, BRUSH_SIZE_Y, BRUSH_SIZE_Y + BRUSH_SIZE_HEIGHT)){  //All this buttons have the same y
        if(between(x, SMALL_SIZE_X, SMALL_SIZE_X + SMALL_SIZE_WIDTH)) return BRUSH_SIZE_SMALL;
        else if(between(x, MEDIUM_SIZE_X, MEDIUM_SIZE_X + MEDIUM_SIZE_WIDTH)) return BRUSH_SIZE_MEDIUM;
        else if(between(x, LARGE_SIZE_X, LARGE_SIZE_X + LARGE_SIZE_WIDTH)) return BRUSH_SIZE_LARGE;
    }
    else if(_actual_menu == GIVE_UP && between(x, GIVE_UP_X, GIVE_UP_X + GIVE_UP_WIDTH) && between(y, GIVE_UP_Y, GIVE_UP_Y + GIVE_UP_HEIGHT)) return GIVE_UP_PRESSED;
    else if(_actual_menu == DONE && between(x, DONE_X,DONE_X + DONE_WIDTH) && between(y, DONE_Y, DONE_Y + DONE_HEIGHT)) return DONE_PRESSED;
    return _MOUSE_NOT_IN_BUTTON;
}
int pause_menu_cursor_button(uint16_t x, uint16_t y){
    if(between(x, MINI_BOX_X, MINI_BOX_X + sprites_pause_menu[IN_BOX]->width) && between(y, MINI_BOX_Y, MINI_BOX_Y + sprites_pause_menu[IN_BOX]->height)){ //All this buttons are inside of the mini box
        if (between(x, RESUME_X, RESUME_X + RESUME_WIDTH) && between(y, RESUME_Y, RESUME_Y + RESUME_HEIGHT)) return RESUME_GAME;
        else if (between(x, LEAVE_X, LEAVE_X + LEAVE_WIDTH) && between(y, LEAVE_Y, LEAVE_Y + LEAVE_HEIGHT)) return LEAVE_GAME;
        else return IN_BOX;
    }
    return NOT_IN_BOX;
}

void draw_canvas(){
    for(int i = 0; i < CANVAS_HEIGHT; i++){
        for(int j = 0; j < CANVAS_WIDHT;j++){
            for(int k = 0; k < ceil(vbe_info.BitsPerPixel / 8.0); k++){
                play_double_buffer[(int)(ceil(vbe_info.BitsPerPixel / 8.0)) *((CANVAS_Y + i)*vbe_info.XResolution + (CANVAS_X + j)) + k] = canvas_buf[(int)(ceil(vbe_info.BitsPerPixel / 8.0)) *((CANVAS_Y + i)*vbe_info.XResolution + (CANVAS_X + j)) + k];
            }
        }
    }
}

void draw_give_up(int pressed){
    if(pressed == GIVE_UP_PRESSED) draw_sprite(play_mini_menu[7], play_double_buffer);
    else draw_sprite(play_mini_menu[6], play_double_buffer);
}
void draw_brush_size(int button){
    if(button == BRUSH_SIZE_SMALL) draw_sprite(play_mini_menu[3], play_double_buffer);
    else if(button == BRUSH_SIZE_MEDIUM) draw_sprite(play_mini_menu[4], play_double_buffer);
    else if(button == BRUSH_SIZE_LARGE) draw_sprite(play_mini_menu[5], play_double_buffer);
    else draw_sprite(play_mini_menu[2], play_double_buffer);
}
void draw_done(int pressed){
    if(pressed == DONE_PRESSED) draw_sprite(play_mini_menu[1], play_double_buffer);
    else draw_sprite(play_mini_menu[0], play_double_buffer);
}

void draw_actual_default(){
    draw_canvas();
    draw_sprite(player1_icons[curr_player1_icon], play_double_buffer);    draw_score_1(play_double_buffer);
    draw_sprite(player2_icons[curr_player2_icon], play_double_buffer);    draw_score_2(play_double_buffer);
    for (int i = 0; i < mouse_cursor->height; i++) {
        for (int j = 0; j < mouse_cursor->width; j++) {
            mouse_cursor->replace_color[i * mouse_cursor->width + j] = video_card_get_pixel_color(play_double_buffer, mouse_cursor->x + j, mouse_cursor->y + i );
        }
    }
}
void draw_pause_sprites(int index){
    draw_sprite(sprites[BACKGROUND_INDEX], play_double_buffer);           //draw the background

    for(int i = 0; i < MENU_NUMBER_TOOLS; i++){
        draw_sprite(sprites_painting_tools[i], play_double_buffer);
    }
    if(index)   draw_sprite(sprites_pause_menu[index], play_double_buffer); //if index == 0, the cursor isn't in the box
    else    draw_sprite(sprites_pause_menu[1], play_double_buffer);

    draw_actual_default();
}
void draw_actual_mouse_in_buttons(int index){
    draw_sprite(sprites[BACKGROUND_INDEX], play_double_buffer);           //draw the background
    
    for(int i = 1; i <= MENU_ACTUAL_SPRITES; i++){                              //draw the button sprites correctly
        if(i + MENU_ACTUAL_SPRITES != index)                                  //This means that the button that should be drawn is the pressed button
            draw_sprite(sprites[i], play_double_buffer);
    }
    for(int i = 0; i < MENU_NUMBER_TOOLS; i++){
        draw_sprite(sprites_painting_tools[i], play_double_buffer);
    }
    if(index) draw_sprite(sprites[index], play_double_buffer);   //Draw the pressed button
    draw_actual_menu[_actual_menu](0);
    draw_actual_default();
    draw_sprite(clock_sprites[actual_clock], play_double_buffer);
    draw_sprite(blocker, play_double_buffer);
}
void draw_actual_mouse_in_tools(int index){
    draw_sprite(sprites[BACKGROUND_INDEX], play_double_buffer);           //draw the background    
    
    for(int i = 0; i <= MENU_ACTUAL_SPRITES; i++){
        draw_sprite(sprites[i], play_double_buffer);
    }

    for(int i = 0; i < MENU_NUMBER_TOOLS; i++){                            //draw the tools sprites correctly
        if(i + 5 + 8 != index){                                             //This means that the button that should be drawn is the pressed button
            draw_sprite(sprites_painting_tools[i], play_double_buffer);
        }
    }
    draw_sprite(sprites_painting_tools[index - 8], play_double_buffer);        //Draw the pressed button
    draw_actual_menu[_actual_menu](0);
    draw_actual_default();   
    draw_sprite(clock_sprites[actual_clock], play_double_buffer); 
    draw_sprite(blocker, play_double_buffer);

}
void draw_actual_mouse_in_menu(int index){
    draw_sprite(sprites[BACKGROUND_INDEX], play_double_buffer);           //draw the background
   
    for(int i = 0; i <= MENU_ACTUAL_SPRITES; i++){
        draw_sprite(sprites[i], play_double_buffer);
    }

    for(int i = 0; i < MENU_NUMBER_TOOLS; i++){
        draw_sprite(sprites_painting_tools[i], play_double_buffer);
    }

    draw_actual_menu[_actual_menu](index);
    
    draw_actual_default();
    draw_sprite(clock_sprites[actual_clock], play_double_buffer);
    draw_sprite(blocker, play_double_buffer);

}
void draw_actual_end(int index){
    draw_sprite(sprites[BACKGROUND_INDEX], play_double_buffer);           //draw the background

    for(int i = 0; i < MENU_NUMBER_TOOLS; i++){
        draw_sprite(sprites_painting_tools[i], play_double_buffer);
    }

    draw_sprite( play_mini_menu[8], play_double_buffer);
    //Draw the name of the winner
    if(score_1 > score_2){ //The player 1 is the winner
        for(int i = 0; i < player1_name_size; i++){ //Draw the player 1 name
            draw_sprite_in(END_GAME_NAME_X + i * (SPACE_BETWEEN_LETTERS + alphabet_sprites[player1_name[i]]->width), END_GAME_NAME_Y,  alphabet_sprites[player1_name[i]], play_double_buffer);
        }
    }
    else{   //The player 2 is the winner
        for(int i = 0; i < player2_name_size; i++){ //Draw the name of the player 2
            draw_sprite_in(END_GAME_NAME_X + i * (SPACE_BETWEEN_LETTERS + alphabet_sprites[player2_name[i]]->width), END_GAME_NAME_Y,  alphabet_sprites[player2_name[i]], play_double_buffer);
        }
    }

    draw_actual_default();

}

int play_menu(void * video_mem, void * double_buffer, int player1_icon, int player2_icon){
    
    /*Initialize some variables to start the function*/

    uint32_t timer_counter = 0; int end = NOT_IN_BOX, valid_reading = KBD_VALID_READING, kbd_index = 0;
    int refresh_after_play = 0;
    uint8_t keyboard_data;

    play_video_mem = video_mem;
    play_double_buffer = double_buffer;

    decimal_to_hex(actual_time, &actual_gt);

    curr_player1_icon = player1_icon;
    curr_player2_icon = player2_icon;

    _actual_menu = DONE; score_1 = 0; score_2 = 0; word1_length = 0; word2_length = 0;
    numberWords = 0; play_prev_index = 0; actual_clock = 0; guess_size = 0; replace_size = 0;
    actual_round = 1; actual_game_status = CHOOSE_WORD; last_game_status = CHOOSE_WORD;

    current_tool = BRUSH; current_color = 0x00000000;current_brush_size = MEDIUM;

    player1_draw = true; play_again = false;
    final_position_y = CANVAS_X + CANVAS_WIDHT; //first final position
    actual_movement = RIGHT; last_movement = UP;
    game_time = 0;

    load_sprites();
    if(!readfromFile()){return END;} //If occur an error at the reading file moment, this menu ends

    canvas_buf = calloc(vbe_info.XResolution * vbe_info.YResolution * ceil(vbe_info.BitsPerPixel / 8.0),sizeof(char)); //Canvas auxiliar buffer
    for(int i = 0; i < vbe_info.YResolution; i++){ //Paint the canvas in the main buffer
        for(int j = 0; j < vbe_info.XResolution; j++){
            video_card_paint_pixel(canvas_buf, j, i, CANVAS_DEFAULT_COLOR);
        }
    }

    choose_word(); //Draw in the screen the sprites that should appear when it is supposed to choose the word of the game

    mouse_cursor->x = vbe_info.XResolution / 2;
    mouse_cursor->y = vbe_info.YResolution / 2 - 20;
    draw_sprite_in(vbe_info.XResolution / 2, vbe_info.YResolution / 2 - 20, mouse_cursor, play_double_buffer);

    struct packet mouse_packet;
    message msg;
    int r, ipc_status, pCounter = 0;
    uint8_t mouse_data_byte, rtc_bit_number;

    if(rtc_subscribe_int(&rtc_bit_number) != OK) return 1;
    rtc_enable_conf(false, RTC_ALARM);
    rtc_enable_conf(false, RTC_UPDATE);
    rtc_set_alarm(game_time);
    rtc_interrupt_handler();
    uint32_t mouse_irq_set = BIT(mouse_bit_number), timer_irq_set = BIT(timer_bit_number), keyboard_irq_set = BIT(keyboard_bit_number), rtc_irq_set = BIT(rtc_bit_number);

    while(end != LEAVE_GAME && end != END){
        if((r = driver_receive(ANY, &msg, &ipc_status)) != OK) continue;
        if(is_ipc_notify(ipc_status)){
            switch(_ENDPOINT_P(msg.m_source)){
                case HARDWARE:
                    if(msg.m_notify.interrupts & mouse_irq_set){
                        switch(mouse_interrupt_handler(&mouse_data_byte)){
                            case MOUSE_VALID_READING:
                                if(mouse_packet_assemble(&pCounter, &mouse_packet, &mouse_data_byte) != OK) return 1;
                                if(pCounter == 3) { //Avoids the three bytes from not being processed
                                    end = play_menu_state_machine(detect_event(&mouse_packet), &mouse_packet);
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
                                if(actual_game_status == GUESS_WORD && between(kbd_index, 0, ALPHABET_LENGTH)){ //Normal letter
                                    if(guess_add_letter(kbd_index)){ //guess_add_letter returns true if the player gives a correct guess
                                        if(player1_draw){score_1 += CORRECT_ANSWER_PAINTER; score_2 += CORRECT_ANSWER_GUESSER;} //Give the respective points (Player 1 is the painter if player1_draw is true)
                                        else{score_1 += CORRECT_ANSWER_GUESSER; score_2 += CORRECT_ANSWER_PAINTER;} //Give the respective points(Player 2 is the painter if player1_draw is false)
                                        player1_draw = !player1_draw; //Change the turn
                                        if(actual_round < MAX_ROUNDS || score_1 == score_2){ //Go to the next round
                                            actual_game_status = CHOOSE_WORD;
                                            last_game_status = GUESS_WORD;
                                            play_again = true;
                                            actual_round++;
                                        }
                                        else{ //The game reaches the end
                                            play_again = false;
                                            end = END;
                                        }
                                    }
                                }
                                else if(actual_game_status != CHOOSE_WORD && kbd_index == ESC_KEY){ //The esc key can be used to pause the game or to back to the game (if the game is already paused)
                                    if(actual_game_status == GAME_PAUSED){
                                        actual_game_status = last_game_status;
                                        last_game_status = GAME_PAUSED;
                                    }
                                    else{
                                        last_game_status = actual_game_status;
                                        actual_game_status = GAME_PAUSED;
                                    }
                                    //Draw the sprites of actual game status (after press the key esc)
                                    if(actual_game_status == GUESS_WORD) draw_guess();
                                    else{ draw_actual[actual_game_status](cursor_button[actual_game_status == GAME_PAUSED](mouse_cursor->x, mouse_cursor->y)); draw_sprite(mouse_cursor, double_buffer);}
                                }
                                else if(kbd_index == BACK_SPACE_KEY && actual_game_status == GUESS_WORD && guess_size) guess_remove_letter();
                                break;
                            case KBD_EMPTY_BUFFER:
                                continue;
                            case KBD_INVALID_READING:
                                return 1;
                        }
                    }
                    if (msg.m_notify.interrupts & rtc_irq_set) {
                        uint8_t st = rtc_interrupt_handler();
                        if (actual_game_status == GAME_PAUSED || actual_game_status == CHOOSE_WORD) {
                            uint8_t alarm_sec = (rtc_read_value(RTC_SECONDS_ALARM) + 0x01) % 0x3C; //Adds 1 second to the alarm per update interrupt if the game is paused
                            rtc_write_value(RTC_SECONDS_ALARM, alarm_sec); 
                        }
                        else if (st & BIT(1) && between(actual_game_status,PLAYING_BUTTONS, PLAYING_DEFAULT) && actual_game_status != GUESS_WORD) { //Alarm to change the game status
                            last_game_status = actual_game_status;
                            actual_game_status = GUESS_WORD;
                            draw_guess();
                            rtc_set_alarm(game_time);
                            game_time = 0;
                        }
                        else if(st & BIT(1) && actual_game_status == GUESS_WORD){ //Alarm to change the game status
                            player1_draw = !player1_draw;
                            if(actual_round < MAX_ROUNDS || score_1 == score_2){
                                actual_game_status = CHOOSE_WORD;
                                last_game_status = GUESS_WORD;
                                play_again = true;
                                actual_round++;
                            }
                            else{
                                play_again = false;
                                end = END;
                            }
                        }
                        else if (st & BIT(0)) { //Update interrupt
                            actual_clock = game_time % 0x08;
                            draw_sprite(clock_sprites[game_time % 0x08], double_buffer);
                            game_time++;
                        } 
                    }
                    if(end == END || (msg.m_notify.interrupts & timer_irq_set && pCounter == 0)) {
                        
                        if(end != END && between(actual_game_status, PLAYING_BUTTONS, PLAYING_DEFAULT) && (timer_counter % 4 == 0)) move_blocker(); //Moves the blocker if it is supposed to
                        refresh_menu_play_double_buffer(); //Double buffering
                        if(play_again){ //If the round is over
                            refresh_after_play++;
                            if(refresh_after_play % (5 * sys_hz()) == 0){ //Wait 5 seconds after the end of the round to prepare the next round
                                canvas_cleaning();
                                refresh_after_play = 0;
                                play_again = false;
                                choose_word();
                                draw_sprite(mouse_cursor, play_double_buffer);
                                guess_size = 0;
                            }
                        }
                        else if(end == END){ //If the game is in the end
                            sleep(5);
                            draw_actual[4](END);
                            refresh_menu_play_double_buffer();
                        }
                        timer_counter++;
                    }
                default:
                    break;
            }
        }
    }

    if(end == END) sleep(5);

    rtc_disable_conf(RTC_ALARM);
    rtc_disable_conf(RTC_UPDATE);
    if(rtc_unsubscribe_int() != OK) return 1;
    free(canvas_buf);
    /* free sprites */
    for(int i = 0; i < 7; i++){destroy_sprite(sprites[i]);}
    for(int i = 0; i < 4; i++){destroy_sprite(sprites_pause_menu[i]);}
    for(int i = 0; i < 10; i++){destroy_sprite(sprites_painting_tools[i]);}
    for(int i = 0; i < 8; i++){destroy_sprite(clock_sprites[i]);}
    for(int i = 0; i < 11; i++){destroy_sprite(play_mini_menu[i]);}
    for(int i = 0; i < 7; i++){destroy_sprite(player1_icons[i]);}
    for(int i = 0; i < 7; i++){destroy_sprite(player2_icons[i]);}
    for(int i = 0; i < 10; i++){destroy_sprite(numbers[i]);}
    free(canvas_buf);
    free(word1); free(word2); free(words); free(replace_color_guess);
    return 0;
}

int play_menu_state_machine(enum mouse_event event, struct packet *pp){
    int index = 0;
    if(actual_game_status != CHOOSE_WORD && actual_game_status != GUESS_WORD){
        index = cursor_button[actual_game_status == GAME_PAUSED](mouse_cursor->x, mouse_cursor->y);
        if(actual_game_status != CHOOSE_WORD && actual_game_status != GUESS_WORD){
            index = cursor_button[actual_game_status == GAME_PAUSED](mouse_cursor->x, mouse_cursor->y);
            if(index != play_prev_index){ 
                if(actual_game_status != GAME_PAUSED){
                    if(index >= 13 && index <= 17) {last_game_status = actual_game_status; actual_game_status = PLAYING_TOOLS;}
                    else if(index >= 18) {last_game_status = actual_game_status; actual_game_status = PLAYING_MENU;}
                    else { last_game_status = actual_game_status; actual_game_status = PLAYING_BUTTONS;}
                } 
                draw_actual[actual_game_status](index);
                play_prev_index = index;
            }
        } 
    }
    if(play_again) event = MOUSE_MOVE; //if the game is in countdown to restart, the mouse only moves
    switch(event){
        case MOUSE_MOVE:
            move_cursor(pp, play_double_buffer);
            break;
        case L_PRESSED:
            if(play_prev_event != L_PRESSED){ play_start_pos_x = mouse_cursor->x; play_start_pos_y = mouse_cursor->y;}
            move_cursor(pp, play_double_buffer); 
            if(actual_game_status != CHOOSE_WORD && actual_game_status != GUESS_WORD){
                if (actual_game_status != GAME_PAUSED && between(mouse_cursor->x, CANVAS_X, CANVAS_X + CANVAS_WIDHT) && between(mouse_cursor->y, CANVAS_Y, CANVAS_Y + CANVAS_HEIGHT) && (current_tool == BRUSH || current_tool == RUBBER))
                    play_menu_paint_call(pp, current_tool, current_color, current_brush_size);
            }
            break;
        case L_RELEASED:
            if(play_prev_event == L_PRESSED){ //check if the position of the cursor is on a button and the initial press was done inside a the corresponding button
                if(actual_game_status != CHOOSE_WORD && actual_game_status != GUESS_WORD){
                    index = cursor_button[actual_game_status == GAME_PAUSED](mouse_cursor->x, mouse_cursor->y);
                    if(actual_game_status == GAME_PAUSED){
                        if(index == LEAVE_GAME)
                            return index;
                        else if(index == RESUME_GAME){
                            actual_game_status = last_game_status; last_game_status = GAME_PAUSED; 
                            if(actual_game_status == GUESS_WORD) draw_guess();
                            else{draw_actual[actual_game_status](BACKGROUND_INDEX); draw_sprite(mouse_cursor, play_double_buffer);}
                        }
                    }
                    if(index && index == play_menu_cursor_button(play_start_pos_x, play_start_pos_y)){ //se é diferente d background e começou no botão
                        if(index == MOUSE_PAUSE_BUTTON){
                            last_game_status = actual_game_status;
                            actual_game_status = GAME_PAUSED;
                            draw_actual[actual_game_status](cursor_button[actual_game_status == GAME_PAUSED](mouse_cursor->x, mouse_cursor->y));
                            draw_sprite(mouse_cursor, play_double_buffer);
                        }
                        else if(index == MOUSE_FRONT_BUTTON){
                            _actual_menu = move_menu(true);
                        }
                        else if(index == MOUSE_BACK_BUTTON){
                            _actual_menu = move_menu(false);
                        }
                        else if(index >= 13 && index <= 17){
                            if(index == CLEANER_PRESSED) play_menu_paint_call(pp, CLEANER, current_color, current_brush_size);
                            else current_tool = index - 5;
                        }
                        else if(index >= BRUSH_SIZE_SMALL && index <= BRUSH_SIZE_LARGE)
                            current_brush_size = ((index % 18) + 1) * 10; //SMALL = 10, MEDIUM = 20, LARGE = 30
                        else if(index == GIVE_UP_PRESSED){
                            player1_draw = !player1_draw;
                            last_game_status = actual_game_status;
                            actual_game_status = CHOOSE_WORD;
                            canvas_cleaning();
                            choose_word();
                            draw_sprite(mouse_cursor, play_double_buffer);
                            rtc_set_alarm(actual_gt); game_time = 0;
                        }
                        else if(index == DONE_PRESSED){
                            last_game_status = actual_game_status;
                            actual_game_status = GUESS_WORD;
                            draw_guess();
                            rtc_set_alarm(actual_gt); game_time = 0; 
                        }
                        else return index;  //function called return a value to end the program
                    }
                    else if(current_tool == COLOR_PICKER || current_tool == BUCKET) play_menu_paint_call(pp, current_tool, current_color, current_brush_size);
                }else if(actual_game_status == CHOOSE_WORD && between(mouse_cursor->y, WORDS_Y, WORDS_Y + LETTER_HEIGHT)){
                    if(between(mouse_cursor->x, WORD1_X, WORD1_X + word1_length * LETTER_WIDTH + (word1_length - 1) * SPACE_BETWEEN_LETTERS)){
                        if(between(play_start_pos_x, WORD1_X, WORD1_X + word1_length * LETTER_WIDTH + (word1_length - 1) * SPACE_BETWEEN_LETTERS) &&
                            between(play_start_pos_y, WORDS_Y, WORDS_Y + LETTER_HEIGHT)){
                                gameWord = word1;
                                game_word_size = word1_length;
                                first_guess_x =  (game_word_size % 2) ? ((MINI_MENU_X + (MINI_MENU_WIDTH / 2)) - (game_word_size / 2 * (SPACE_BETWEEN_LETTERS + LETTER_WIDTH) + LETTER_WIDTH / 2)):((MINI_MENU_X + (MINI_MENU_WIDTH / 2)) - ((game_word_size / 2 - 1) * SPACE_BETWEEN_LETTERS + (game_word_size / 2) *LETTER_WIDTH));
                                actual_game_status = PLAYING_MENU;
                                draw_actual[actual_game_status](cursor_button[actual_game_status == GAME_PAUSED](mouse_cursor->x, mouse_cursor->y));
                                draw_sprite(mouse_cursor, play_double_buffer);
                                rtc_set_alarm(actual_gt); game_time = 0;
                            }
                    }
                    else if(actual_game_status == CHOOSE_WORD && between(mouse_cursor->x, WORD2_X, WORD2_X + word2_length * LETTER_WIDTH + (word2_length - 1) * SPACE_BETWEEN_LETTERS)){
                        if(between(play_start_pos_x,  WORD2_X, WORD2_X + word2_length * LETTER_WIDTH + (word2_length - 1) * SPACE_BETWEEN_LETTERS) &&
                            between(play_start_pos_y, WORDS_Y, WORDS_Y + LETTER_HEIGHT)){
                            gameWord = word2;
                            game_word_size = word2_length;
                            first_guess_x =  (game_word_size % 2) ? ((MINI_MENU_X + (MINI_MENU_WIDTH / 2)) - (game_word_size / 2 * (SPACE_BETWEEN_LETTERS + LETTER_WIDTH) + LETTER_WIDTH / 2)):((MINI_MENU_X + (MINI_MENU_WIDTH / 2)) - ((game_word_size / 2 - 1) * SPACE_BETWEEN_LETTERS + (game_word_size / 2) *LETTER_WIDTH));
                            actual_game_status = PLAYING_MENU;
                            draw_actual[actual_game_status](cursor_button[actual_game_status == GAME_PAUSED](mouse_cursor->x, mouse_cursor->y));
                            draw_sprite(mouse_cursor, play_double_buffer);
                            rtc_set_alarm(actual_gt); game_time = 0;
                        }
                    }
                }
            }
            break;
        default:
            move_cursor(pp, play_double_buffer);
            break;
    }
    
    play_prev_event = event;
    return _MOUSE_NOT_IN_BUTTON;
}

void play_menu_paint_call(struct packet *pp, enum paint_tools curr_tool, uint32_t curr_color, enum brush_size b_size) {
    erase_sprite(mouse_cursor, play_double_buffer);
    switch (curr_tool) {
        case BRUSH: //The brush will only draw if the cursor isn't in the area of the blocker
            if(!between(mouse_cursor->x, blocker->x - mouse_cursor->width, blocker->x + blocker->width) || !between(mouse_cursor->y, blocker->y - mouse_cursor->height, blocker->y + blocker->height)){
                for(int i = 0; i < mouse_cursor->height; i++){ //Update the mouse replace color
                    for(int j = 0; j < mouse_cursor->width; j++){
                        mouse_cursor->replace_color[i*mouse_cursor->height + j] = curr_color;
                    }
                }
                canvas_fill_brush(mouse_cursor->x, mouse_cursor->y, pp->delta_x, -(pp->delta_y), curr_color, b_size, false);
            }
            break;
        case RUBBER://The rubber will only draw if the cursor isn't in the area of the blocker
            if(!between(mouse_cursor->x, blocker->x - mouse_cursor->width, blocker->x + blocker->width) || !between(mouse_cursor->y, blocker->y - mouse_cursor->height, blocker->y + blocker->height)){
                canvas_fill_brush(mouse_cursor->x, mouse_cursor->y, pp->delta_x, -(pp->delta_y), CANVAS_DEFAULT_COLOR, MEDIUM, true);
            }
            break;
        case BUCKET://The bucket will only draw if the cursor isn't in the area of the blocker
            if(!between(mouse_cursor->x, blocker->x - mouse_cursor->width, blocker->x + blocker->width) || !between(mouse_cursor->y, blocker->y - mouse_cursor->height, blocker->y + blocker->height)){
                canvas_bucket_fill_play_mode(mouse_cursor->x, mouse_cursor->y, curr_color, mouse_cursor->replace_color[0], blocker->x, blocker->y, blocker->width, blocker->height);
            }
            break;
        case COLOR_PICKER: //Pick a color
            current_color = mouse_cursor->replace_color[0];
            break;
        case CLEANER: //Clean the canvas
            canvas_cleaning();
            for (int i = 0; i < blocker->height; i++) { //Update the replace color of the blocker
                for (int j = 0; j < blocker->width; j++) {
                    blocker->replace_color[i * blocker->width + j] = video_card_get_pixel_color(canvas_buf, blocker->x + j, blocker->y + i );
                }
            }
            break;
        default:
            break;
    }
    draw_sprite(mouse_cursor, play_double_buffer);

}

void draw_score_1(){
    int temp[3];
    int temporary_score = score_1;
    for(int i = 0; i < 3; i++){//Get the digits of the score of the player 1
        temp[i] = temporary_score % 10;
        temporary_score /= 10;
    }
    int where = 0;
    for(int i = 2; i >= 0; i--){//Draw the obtained digits
        draw_sprite_in(SCORE1_X + where * numbers[temp[i]]->width, NUMBERS_Y, numbers[temp[i]], play_double_buffer);
        where++;
    }

}

void draw_score_2(){
    int temp[3];
    int temporary_score = score_2;
    for(int i = 0; i < 3; i++){//Get the digits of the score of the player 2
        temp[i] = temporary_score % 10;
        temporary_score /= 10;
    }
    int where = 0;
    for(int i = 2; i >= 0; i--){//Draw the obtained digits
        draw_sprite_in(SCORE2_X + where * numbers[temp[i]]->width, NUMBERS_Y, numbers[temp[i]], play_double_buffer);
        where++;
    }
}

int move_menu(bool right){
    int prev_x, next_x, prev_x_old, next_x_old, xf, next_menu, n_bytes = ceil(vbe_info.BitsPerPixel / 8.0);
    next_menu = (right) ? ((_actual_menu + 1) % 3):((_actual_menu - 1 + 3) % 3);
    prev_x = (right) ? MINI_MENU_X:MINI_MENU_X + MINI_MENU_WIDTH;//If the movement is to the right the x used is MINI_MENU_X (the leftmost x of the sprite), otherwise is the biggest x
    next_x = (right) ? MINI_MENU_X:MINI_MENU_FINAL_X;//If the movement is to the right the x used is the beginning of the menu, otherwise is the final of the menu
    xf = (right) ? MINI_MENU_FINAL_X:MINI_MENU_X;//The final position depends of the type of movement
    while(prev_x != xf){
        prev_x_old = prev_x;
        next_x_old = next_x;
        if (right) { 
            clear_moved_menu(prev_x_old, MINI_MENU_Y, play_mini_menu[menus[_actual_menu]], right, MINI_MENU_FINAL_X, true);
            clear_moved_menu(next_x_old, MINI_MENU_Y, play_mini_menu[menus[next_menu]], right, MINI_MENU_X, false);
            prev_x += play_mini_menu[menus[_actual_menu]]->xspeed;
            next_x += play_mini_menu[menus[next_menu]]->xspeed;
            if (prev_x >= xf){//The movement is over
                draw_sprite_in(MINI_MENU_X, MINI_MENU_Y, play_mini_menu[menus[next_menu]], play_double_buffer);
                break;
            }
            menu_move_prev_right(prev_x, MINI_MENU_Y, MINI_MENU_FINAL_X, play_mini_menu[menus[_actual_menu]], n_bytes);//The previous sprite moves from the normal position of the menu to the right of the box
            menu_move_next_right(next_x, MINI_MENU_Y, MINI_MENU_X, play_mini_menu[menus[next_menu]], n_bytes);//The next sprite moves from the left of the box to the normal position of the menu
        }
        else {
            clear_moved_menu(prev_x_old, MINI_MENU_Y, play_mini_menu[menus[_actual_menu]], right, MINI_MENU_X, true);
            clear_moved_menu(next_x_old, MINI_MENU_Y, play_mini_menu[menus[next_menu]], right, MINI_MENU_FINAL_X, false);
            prev_x -= play_mini_menu[menus[_actual_menu]]->xspeed;
            next_x -= play_mini_menu[menus[next_menu]]->xspeed; 
            if (prev_x  <= xf){//The movement is over
                draw_sprite_in(MINI_MENU_X, MINI_MENU_Y, play_mini_menu[menus[next_menu]], play_double_buffer);
                break;
            }
            menu_move_prev_left(prev_x, MINI_MENU_Y, MINI_MENU_X, play_mini_menu[menus[_actual_menu]], n_bytes);//The previous sprite moves from the normal position of the menu to the left of the box
            menu_move_next_left(next_x, MINI_MENU_Y, MINI_MENU_FINAL_X, play_mini_menu[menus[next_menu]], n_bytes);//The next sprite moves from the right of the box to the normal position of the menu
        }
        refresh_menu_play_double_buffer();
    }
    uint8_t alarm_sec = (rtc_read_value(RTC_SECONDS_ALARM) + 0x01) % 0x3C; //Adds 1 second to the alarm because the game is "paused"
    rtc_write_value(RTC_SECONDS_ALARM, alarm_sec); 
    game_time--;
    return next_menu;
}

void clear_moved_menu(uint16_t x, uint16_t y, Sprite * sp, bool right, uint16_t limit, bool prev) { //Clears the menu with a part of the next menu
    int n_bytes = ceil(vbe_info.XResolution / 8.0);
    if (right){
        if(prev) draw_part_of(x, y, sprites[BACKGROUND_INDEX], (limit - x < sp->width) ? (limit - x):sp->width, sp->height, play_double_buffer, n_bytes);
        else {
            uint16_t width = (x - limit < sp->width) ? (x- limit):sp->width;
            draw_part_of(x - width, y, sprites[BACKGROUND_INDEX], width, sp->height, play_double_buffer, n_bytes);
        }
    }
    else{
        if(prev) draw_part_of(limit, y, sprites[BACKGROUND_INDEX], x - limit, sp->height, play_double_buffer, n_bytes);
        else draw_part_of(x, y, sprites[BACKGROUND_INDEX], limit - x, sp->height, play_double_buffer, n_bytes);
    }
}

void menu_move_prev_right(uint16_t x, uint16_t y, uint16_t limit, Sprite * sp, int number_bytes_per_pixel){
    uint16_t width = sp->width;
    if(limit - x < width) width = limit - x; //Adjusts the width if it is necessary to 

    char * board = play_double_buffer + ((vbe_info.XResolution * y) + x) * number_bytes_per_pixel;


    for(int i = 0; i < sp->height; i++){
        for(int j = 0; j < width; j++){
            sp->replace_color[i * sp->width + j] = video_card_get_pixel_color(play_double_buffer, x + j, y + i );
            for (int k = 0; k < number_bytes_per_pixel - 1; k++) {
                if(sp->map[i*sp->width + j] != xpm_transparency_color(XPM_8_8_8_8))
                    board[k] = sp->map[i*sp->width + j] >> (k * 8);
            }
            board += number_bytes_per_pixel;
        }
        board += (vbe_info.XResolution - width) * number_bytes_per_pixel;
    }
}
void menu_move_next_left(uint16_t x, uint16_t y, uint16_t limit, Sprite * sp, int number_bytes_per_pixel){
    uint16_t width = sp->width;
    if(limit - x < width) width = limit - x; //Adjusts the width if it is necessary to 

    char * board = play_double_buffer + ((vbe_info.XResolution * y) + x) * number_bytes_per_pixel;

    for(int i = 0; i < sp->height; i++){
        for(int j = 0; j < width; j++){
            sp->replace_color[i * sp->width + j] = video_card_get_pixel_color(play_double_buffer, x + j, y + i );
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
void menu_move_prev_left(uint16_t x, uint16_t y, uint16_t limit, Sprite * sp, int number_bytes_per_pixel){
    uint16_t width = sp->width;
    int initial_j;
    if(x - limit < width){//Adjusts the width if it is necessary to 
        width = (x - limit);
        x = limit;
        initial_j = width;
    }
    else{
        x -= width;
        initial_j = 0;//Draws the entire sprite
    }
    char * board = play_double_buffer + ((vbe_info.XResolution * y) + x) * number_bytes_per_pixel;
    for(int i = 0; i < sp->height; i++){
        for(int j = 0; j < width; j++){
            sp->replace_color[i * sp->width + j] = video_card_get_pixel_color(play_double_buffer, x + j, y + i );
            for (int k = 0; k < number_bytes_per_pixel - 1; k++) {
                if(sp->map[i*sp->width + j + (sp->width - width)] != xpm_transparency_color(XPM_8_8_8_8))
                    board[k] = sp->map[i*sp->width + j + (sp->width - width)] >> (k * 8);
            }
            board += number_bytes_per_pixel;
        }
        board += (vbe_info.XResolution - width) * number_bytes_per_pixel;
    }
}
void menu_move_next_right(uint16_t x, uint16_t y, uint16_t limit, Sprite * sp, int number_bytes_per_pixel){
    uint16_t width = sp->width;
    if(x - limit < width){ //Adjusts the width if it is necessary to 
        width = x - limit;
        x = limit;
    }else x = x - width;

    char * board = play_double_buffer + ((vbe_info.XResolution * y) + x) * number_bytes_per_pixel;

    for(int i = 0; i < sp->height; i++){
        for(int j = sp->width -  width; j < sp->width; j++){
            sp->replace_color[i * sp->width + j] = video_card_get_pixel_color(play_double_buffer, x + j, y + i );
            for (int k = 0; k < number_bytes_per_pixel - 1; k++) {
                if(sp->map[i*sp->width + j] != xpm_transparency_color(XPM_8_8_8_8))
                    board[k] = sp->map[i*sp->width + j] >> (k * 8);
            }
            board += number_bytes_per_pixel;
        }
        board += (vbe_info.XResolution - width) * number_bytes_per_pixel;
    }
}

void refresh_menu_play_double_buffer(){
    memcpy(play_video_mem, play_double_buffer, vbe_info.XResolution * vbe_info.YResolution * ceil(vbe_info.BitsPerPixel / 8.0)); //Double buffering
}

bool readfromFile(){
    FILE * file;
    file = fopen("/home/lcom/labs/g09/proj/src/dictionary.txt", "r");

    if(file == NULL){ //error
        return false;
    }
    char word[10];
    words = (int **)malloc(sizeof(int *));
    while(fscanf(file, "%s\n", word) != EOF){ //read the words from the files
        numberWords++;
        words = (int **)realloc(words, sizeof(int *) * numberWords);
        words[numberWords - 1] = (int *)malloc(sizeof(int) * 10);
        for(int i = 0; i < 10; i++){
            if(word[i] == '\0'){ words[numberWords - 1][i] = 0; break;}
            else if(word[i] == 32) words[numberWords][i] = 32;
            else words[numberWords - 1][i] = word[i] % 96;  //word[i] is the index + 1 of the sprite in the array alphabet_sprites. 0 when in the end of the word
        }
    }

    fclose(file);

    return true;
}

void choose_word(){
    int numberWord1 = rand() % numberWords; //Choose a random word from the words that were read from the file
    word1 = words[numberWord1];
    int numberWord2 = rand() % numberWords;
    while(numberWord1 == numberWord2){ numberWord2 = rand() % numberWords;} //The word2 should be different of the word1
    word2 = words[numberWord2];
    draw_choose_word();
    for(int i = 0; i < 10; i++){
        if(word1[i] > 26 || word1[i] < 1){word1_length = i; break;}
        else if(word1[i] == 32) continue;
        else draw_sprite_in( WORD1_X + i * (SPACE_BETWEEN_LETTERS + LETTER_WIDTH), WORDS_Y, alphabet_sprites[word1[i] - 1], play_double_buffer);
    }
    for(int i = 0; i < 10; i++){
        if(word2[i] > 26 || word2[i] < 1){word2_length = i; break;}
        else if(word2[i] == 32) continue;
        else draw_sprite_in( WORD2_X + i * (SPACE_BETWEEN_LETTERS + LETTER_WIDTH), WORDS_Y, alphabet_sprites[word2[i] - 1], play_double_buffer);
    }
}

bool draw_guess(){
    uint16_t x = first_guess_x;
    bool right_answer = guess_size == game_word_size;
    draw_guess_word();
    for(int i = 0; i < game_word_size; i++){ //draw the guess word
        if( i < guess_size) draw_sprite_in(x, GUESS_Y, alphabet_sprites[guessWord[i]], play_double_buffer);
        else draw_sprite_in(x, GUESS_Y, alphabet_sprites[ALPHABET_LENGTH], play_double_buffer); //underscore
        if(right_answer && guessWord[i] != gameWord[i] - 1) right_answer = false; //gameWord starts in 1, guessWord starts in 0 (because guessWord has the index of the letters and gameWord has index + 1)
        x += SPACE_BETWEEN_LETTERS + LETTER_WIDTH;
    }
    if(right_answer) draw_sprite(play_mini_menu[10], play_double_buffer);
    else draw_sprite(play_mini_menu[9], play_double_buffer);
    draw_sprite(mouse_cursor, play_double_buffer);
    return right_answer;
}

bool guess_add_letter(int index){
    bool result = false; //result would be true if the guess word is equal to the game word (the guess is a success)
    if(guess_size < game_word_size){
        guessWord[guess_size] = index; //Add the letter to the array of the guess word
        if(replace_size == guess_size){ //Allocates memory if it is necessary
            replace_color_guess = (uint32_t **)realloc(replace_color_guess, sizeof(uint32_t *) * (guess_size + 1));
            replace_size++;
            replace_color_guess[guess_size] = (uint32_t *)malloc(sizeof(alphabet_sprites[index]->map));
        }
        guess_size++;
        result = draw_guess(); //draw the sprites that should appear on the screen
        replace_color_guess[guess_size - 1] = alphabet_sprites[index]->replace_color; //update the replace color array of the guess word
    }
    return result;
}

void guess_remove_letter(){
    guess_size--;
    if(guessWord[guess_size] != SPACE_KEY){ //If the character is a space key, it isn't necessary to delete the sprite
        alphabet_sprites[guessWord[guess_size]]->replace_color = replace_color_guess[guess_size];//update the replace color of the sprite to call the function erase_sprite
        alphabet_sprites[guessWord[guess_size]]->x = first_guess_x + guess_size * (LETTER_WIDTH + SPACE_BETWEEN_LETTERS); //get the x position of the letter that is supposed to erase
        alphabet_sprites[guessWord[guess_size]]->y = GUESS_Y;//get the y position of the letter that is supposed to erase
        erase_sprite(mouse_cursor, play_double_buffer);
        erase_sprite(alphabet_sprites[guessWord[guess_size]], play_double_buffer);
        draw_sprite_in(first_guess_x + guess_size * (LETTER_WIDTH + SPACE_BETWEEN_LETTERS), GUESS_Y, alphabet_sprites[ALPHABET_LENGTH], play_double_buffer); //draw the underscore 
        draw_sprite_in(mouse_cursor->x, mouse_cursor->y, mouse_cursor, play_double_buffer);
    }

}

void draw_choose_word(){ //Draws the sprites that should appear on the screen when is time to choose the word to draw
    draw_sprite(sprites[BACKGROUND_INDEX], play_double_buffer);   //draw background
    draw_sprite(chooseWord_menu, play_double_buffer);
    draw_sprite(player1_icons[curr_player1_icon], play_double_buffer);
    draw_sprite(player2_icons[curr_player2_icon], play_double_buffer);
    draw_score_1();
    draw_score_2();
    for(int i = 0; i < MENU_NUMBER_TOOLS; i++){
        draw_sprite(sprites_painting_tools[i], play_double_buffer);
    }
}

void draw_guess_word(){ //Draws the sprites that should appear on the screen when is time to guess what is the word
    draw_sprite(sprites[BACKGROUND_INDEX], play_double_buffer);   //draw background
    draw_canvas();
    draw_sprite(player1_icons[curr_player1_icon], play_double_buffer);
    draw_sprite(player2_icons[curr_player2_icon], play_double_buffer);
    draw_score_1();
    draw_score_2();
    draw_sprite(clock_sprites[actual_clock], play_double_buffer);

    for(int i = 0; i < MENU_NUMBER_TOOLS; i++){
        draw_sprite(sprites_painting_tools[i], play_double_buffer);
    }
}

void move_blocker_right(){
    erase_sprite(blocker, play_double_buffer);    //Erase the block
    if(blocker->x + blocker->width == CANVAS_X + CANVAS_WIDHT){ //If it reaches the final position, is time to change the movement
        actual_movement = (last_movement == DOWN) ? UP:DOWN;
        final_position_y = (last_movement == DOWN) ? blocker->y - blocker->height:blocker->y + blocker->height;
        last_movement = RIGHT;
    }
    else if(blocker->x + blocker->width + blocker->xspeed >= CANVAS_X + CANVAS_WIDHT)//Keep the blocker inside of the trajetory and avoid to pass the final position
        blocker->x = CANVAS_X + CANVAS_WIDHT - blocker->width;
    else blocker->x += blocker->xspeed;
    
}

void move_blocker_left(){
    erase_sprite(blocker, play_double_buffer);    //Erase the block
    if(blocker->x == CANVAS_X){ //If it reaches the final position, is time to change the movement
        actual_movement = last_movement;
        final_position_y = (last_movement == DOWN) ? blocker->y + blocker->height:blocker->y - blocker->height;
        last_movement = LEFT;
    }
    else if(blocker->x - blocker->xspeed <= CANVAS_X)//Keep the blocker inside of the trajetory and avoid to pass the final position
        blocker->x = CANVAS_X;
    else blocker->x -= blocker->xspeed;
}

void move_blocker_up(){
    erase_sprite(blocker, play_double_buffer);    //Erase the block
    if(blocker->y == final_position_y){//If it reaches the final position, is time to change the movement
        actual_movement = (last_movement == RIGHT) ? LEFT:RIGHT;
        last_movement = UP;
    }
    else if(blocker->y - blocker->yspeed <= final_position_y)//Keep the blocker inside of the trajetory and avoid to pass the final position
        blocker->y = final_position_y;
    else blocker->y -= blocker->yspeed;
}

void move_blocker_down(){
    erase_sprite(blocker, play_double_buffer); //Erase the block
    if(blocker->y == final_position_y){ //If it reaches the final position, is time to change the movement
        actual_movement = (last_movement == RIGHT) ? LEFT:RIGHT;
        last_movement = DOWN;
    }
    else if(blocker->y + blocker->yspeed >= final_position_y) //Keep the blocker inside of the trajetory and avoid to pass the final position
        blocker->y = final_position_y;
    else blocker->y += blocker->yspeed;
}

void move_blocker(){
    move_blocker_func[actual_movement](); //Move thess blocker
    draw_sprite(blocker, play_double_buffer);
    for (int i = 0; i < blocker->height; i++) { //Update the replace color of the blocker with the values of the canvas
        for (int j = 0; j < blocker->width; j++) {
            blocker->replace_color[i * blocker->width + j] = video_card_get_pixel_color(canvas_buf, blocker->x + j, blocker->y + i );
        }
    }
    for (int i = 0; i < mouse_cursor->height; i++) { //Update the mouse replace color of the mouse
        for (int j = 0; j < mouse_cursor->width; j++) {
            if(between(mouse_cursor->x + j, blocker->x, blocker->x + blocker->width - 1) && between(mouse_cursor->y + i, blocker->y, blocker->y + blocker->height - 1))
                mouse_cursor->replace_color[i * mouse_cursor->width + j] = video_card_get_pixel_color(play_double_buffer, mouse_cursor->x + j, mouse_cursor->y + i );
            else if(between(mouse_cursor->x + j, CANVAS_X, CANVAS_X + CANVAS_WIDHT -1) && between(mouse_cursor->y + i, CANVAS_Y, CANVAS_Y + CANVAS_HEIGHT - 1))
                mouse_cursor->replace_color[i * mouse_cursor->width + j] = video_card_get_pixel_color(canvas_buf, mouse_cursor->x + j, mouse_cursor->y + i);
        }
    }
    if(between(mouse_cursor->x , CANVAS_X, CANVAS_X + CANVAS_WIDHT) && between(mouse_cursor->y , CANVAS_Y, CANVAS_Y + CANVAS_HEIGHT)){
        erase_sprite(mouse_cursor, play_double_buffer);
        draw_sprite(mouse_cursor, play_double_buffer);
    }
}
