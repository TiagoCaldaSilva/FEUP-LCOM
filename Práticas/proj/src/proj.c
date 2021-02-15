#ifndef PROJ_
#define PROJ_

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

// Any header files included below this line should have been created by you
#include "main_menu.h"
#include "avatar.h"
#include "play_menu.h"
#include "settings_menu.h"
#include "playground_menu.h"
#include "utils.h"
#include "sprites/cursor.h"
#include "sprites/main_menu/background.xpm"
#include "sprites/main_menu/exit_b.xpm"
#include "sprites/main_menu/play_b.xpm"
#include "sprites/main_menu/settings_b.xpm"
#include "sprites/play_menu/paint_tools/paint_brush.xpm"
#include "sprites/play_menu/paint_tools/paint_brush_pressed.xpm"
#include "sprites/play_menu/paint_tools/paint_bucket.xpm"
#include "sprites/play_menu/paint_tools/paint_bucket_pressed.xpm"
#include "sprites/play_menu/paint_tools/eraser.xpm"
#include "sprites/play_menu/paint_tools/eraser_pressed.xpm"
#include "sprites/play_menu/paint_tools/color_picker.xpm"
#include "sprites/play_menu/paint_tools/color_picker_pressed.xpm"
#include "sprites/play_menu/paint_tools/cleaning.xpm"
#include "sprites/play_menu/paint_tools/cleaning_pressed.xpm"
#include "sprites/play_menu/paint_tools/Large_brush.xpm"
#include "sprites/play_menu/paint_tools/Medium_brush.xpm"
#include "sprites/play_menu/paint_tools/Small_brush.xpm"
#include "sprites/play_menu/background.xpm"
#include "sprites/play_menu/back_pressed.xpm"
#include "sprites/play_menu/back_not_pressed.xpm"
#include "sprites/play_menu/front_not_pressed.xpm"
#include "sprites/play_menu/front_pressed.xpm"
#include "sprites/play_menu/pause_pressed.xpm"
#include "sprites/play_menu/pause_not_pressed.xpm"
#include "sprites/play_menu/clock/clock_0.xpm"
#include "sprites/play_menu/clock/clock_1.xpm"
#include "sprites/play_menu/clock/clock_2.xpm"
#include "sprites/play_menu/clock/clock_3.xpm"
#include "sprites/play_menu/clock/clock_4.xpm"
#include "sprites/play_menu/clock/clock_5.xpm"
#include "sprites/play_menu/clock/clock_6.xpm"
#include "sprites/play_menu/clock/clock_7.xpm"
#include "sprites/play_menu/pause_leave_game.xpm"
#include "sprites/play_menu/pause_.xpm"
#include "sprites/play_menu/pause_resume.xpm"
#include "sprites/select_avatar/background.xpm"
#include "sprites/select_avatar/caveman.xpm"
#include "sprites/select_avatar/cyborg.xpm"
#include "sprites/select_avatar/deadcyber.xpm"
#include "sprites/select_avatar/devil.xpm"
#include "sprites/select_avatar/knight.xpm"
#include "sprites/select_avatar/lego.xpm"
#include "sprites/select_avatar/mummy.xpm"
#include "sprites/select_avatar/ninja.xpm"
#include "sprites/select_avatar/pirate.xpm"
#include "sprites/select_avatar/ready_green.xpm"
#include "sprites/select_avatar/ready_red.xpm"
#include "sprites/select_avatar/santa-claus.xpm"
#include "sprites/select_avatar/spiderman.xpm"
#include "sprites/select_avatar/spy.xpm"
#include "sprites/select_avatar/superman.xpm"
#include "sprites/select_avatar/wolverine.xpm"
#include "sprites/select_avatar/keys/a.xpm"
#include "sprites/select_avatar/keys/b.xpm"
#include "sprites/select_avatar/keys/c.xpm"
#include "sprites/select_avatar/keys/d.xpm"
#include "sprites/select_avatar/keys/e.xpm"
#include "sprites/select_avatar/keys/f.xpm"
#include "sprites/select_avatar/keys/g.xpm"
#include "sprites/select_avatar/keys/h.xpm"
#include "sprites/select_avatar/keys/i.xpm"
#include "sprites/select_avatar/keys/j.xpm"
#include "sprites/select_avatar/keys/k.xpm"
#include "sprites/select_avatar/keys/l.xpm"
#include "sprites/select_avatar/keys/m.xpm"
#include "sprites/select_avatar/keys/n.xpm"
#include "sprites/select_avatar/keys/o.xpm"
#include "sprites/select_avatar/keys/p.xpm"
#include "sprites/select_avatar/keys/q.xpm"
#include "sprites/select_avatar/keys/r.xpm"
#include "sprites/select_avatar/keys/s.xpm"
#include "sprites/select_avatar/keys/t.xpm"
#include "sprites/select_avatar/keys/u.xpm"
#include "sprites/select_avatar/keys/v.xpm"
#include "sprites/select_avatar/keys/w.xpm"
#include "sprites/select_avatar/keys/x.xpm"
#include "sprites/select_avatar/keys/y.xpm"
#include "sprites/select_avatar/keys/z.xpm"
#include "sprites/play_menu/avatares_icones/avo.xpm"
#include "sprites/play_menu/avatares_icones/caveman.xpm"
#include "sprites/play_menu/avatares_icones/cyborg.xpm"
#include "sprites/play_menu/avatares_icones/devil.xpm"
#include "sprites/play_menu/avatares_icones/knight.xpm"
#include "sprites/play_menu/avatares_icones/lego.xpm"
#include "sprites/play_menu/avatares_icones/spy.xpm"
#include "sprites/play_menu/avatares_icones/mummy.xpm"
#include "sprites/play_menu/avatares_icones/ninja.xpm"
#include "sprites/play_menu/avatares_icones/pirate.xpm"
#include "sprites/play_menu/avatares_icones/santa-claus.xpm"
#include "sprites/play_menu/avatares_icones/spiderman.xpm"
#include "sprites/play_menu/avatares_icones/superman.xpm"
#include "sprites/play_menu/avatares_icones/wolverine.xpm"
#include "sprites/play_menu/Numbers/0.xpm"
#include "sprites/play_menu/Numbers/1.xpm"
#include "sprites/play_menu/Numbers/2.xpm"
#include "sprites/play_menu/Numbers/3.xpm"
#include "sprites/play_menu/Numbers/4.xpm"
#include "sprites/play_menu/Numbers/5.xpm"
#include "sprites/play_menu/Numbers/6.xpm"
#include "sprites/play_menu/Numbers/7.xpm"
#include "sprites/play_menu/Numbers/8.xpm"
#include "sprites/play_menu/Numbers/9.xpm"
#include "sprites/play_menu/Menus/Brush_size_large_p.xpm"
#include "sprites/play_menu/Menus/Brush_size_medium_p.xpm"
#include "sprites/play_menu/Menus/Brush_size_small_p.xpm"
#include "sprites/play_menu/Menus/Brush_size_not_p.xpm"
#include "sprites/play_menu/Menus/give_up_not_pressed.xpm"
#include "sprites/play_menu/Menus/give_up_pressed.xpm"
#include "sprites/play_menu/Menus/begin_play.xpm"
#include "sprites/play_menu/Menus/done_not_pressed.xpm"
#include "sprites/play_menu/Menus/done_pressed.xpm"
#include "sprites/play_menu/Menus/EndGame.xpm"
#include "sprites/play_menu/Numbers/traco.xpm"
#include "sprites/settings/settings.xpm"
#include "sprites/settings/settings_back.xpm"
#include "sprites/settings/settings_playground.xpm"
#include "sprites/settings/numbers_BIG_white/nW0.xpm"
#include "sprites/settings/numbers_BIG_white/nW1.xpm"
#include "sprites/settings/numbers_BIG_white/nW2.xpm"
#include "sprites/settings/numbers_BIG_white/nW3.xpm"
#include "sprites/settings/numbers_BIG_white/nW4.xpm"
#include "sprites/settings/numbers_BIG_white/nW5.xpm"
#include "sprites/settings/numbers_BIG_white/nW6.xpm"
#include "sprites/settings/numbers_BIG_white/nW7.xpm"
#include "sprites/settings/numbers_BIG_white/nW8.xpm"
#include "sprites/settings/numbers_BIG_white/nW9.xpm"
#include "sprites/play_menu/RightAnswer.xpm"
#include "sprites/play_menu/WrongAnswer.xpm"
#include "sprites/play_menu/block.xpm"
#include "sprites/play_menu/playground.xpm"


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

// Globals
char * video_mem;
char * double_buffer;
Sprite * mouse_cursor;
vbe_mode_info_t vbe_info; // This struct contains usefull info for a lot of functions, that's why it is declared as global
extern int player1_name[NAME_MAX_LENGTH];
extern int player2_name[NAME_MAX_LENGTH];
extern int player1_name_size;
extern int player2_name_size;
int actual_velocity = 10;
int actual_time = 15;
uint8_t mouse_bit_number, timer_bit_number, keyboard_bit_number;

int (proj_main_loop)(int UNUSED(argc), char *UNUSED(argv[])) {
    srand(time(NULL));
    timer_set_frequency(0, DEFAULT_FPS);
    video_mem = video_card_init(DEFAULT_MODE, &vbe_info);
    double_buffer = malloc(vbe_info.XResolution * vbe_info.YResolution * ceil(vbe_info.BitsPerPixel / 8.0));

    mouse_cursor = create_sprite(mouse_cursor_xpm, vbe_info.XResolution / 2, vbe_info.YResolution / 2 - 20, 3, 3);
    if(mouse_send_command(EN_DATA_REPORTING) != OK) return 1;
    if(mouse_subscribe_int(&mouse_bit_number) != OK) return 1;
    if(timer_subscribe_int(&timer_bit_number) != OK) return 1;
    if(keyboard_susbcribe_int(&keyboard_bit_number) != OK) return 1;


	  int next_step, player1_icon = 0, player2_icon = 0;
    while ((next_step = main_menu(video_mem, double_buffer)) != EXIT) {
        switch(next_step) {
          case PLAY_SCREEN:
              if(!select_avatar_menu(video_mem, double_buffer, &player1_icon, &player2_icon)){
                play_menu(video_mem, double_buffer, player1_icon, player2_icon);
              }
              break;
          case SETTINGS_SCREEN:
              while(settings_menu(video_mem, double_buffer) != MOUSE_BACK){ playground_menu(video_mem, double_buffer); }
              break;
          default:
              next_step = EXIT; // exits the loop and ending program execution
        }
    }


    if(keyboard_unsusbcribe_int() != OK) return 1;
    if(timer_unsubscribe_int() != OK) return 1;
    if(mouse_unsubscribe_int() != OK) return 1;
    if(mouse_output_buffer_flush() != OK) return 1;
    if(mouse_send_command(DIS_DATA_REPORTING) != OK) return 1;
	  vg_exit();
	  free(double_buffer);
    return 0;
}

#endif
