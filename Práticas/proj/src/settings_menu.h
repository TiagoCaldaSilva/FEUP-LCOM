#ifndef SETTINGS_H
#define SETTINGS_H

#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "video_card.h"
#include "mouse.h"
#include "timer.h"
#include "keyboard.h"

// Settings Menu MACROS
#define PLAYGROUND_X 376  /**< @brief Playground Button x coordinate */
#define PLAYGROUND_Y 620  /**< @brief Playground Button y coordinate */
#define PLAYGROUND_WIDTH 400  /**< @brief Playground Button width */
#define PLAYGROUND_HEIGHT 110 /**< @brief Playground Button height */
#define BACK_BT_X 38  /**< @brief Back Button x coordinate */
#define BACK_BT_Y 30  /**< @brief Back Button y coordinate */
#define BACK_BT_WIDTH 130 /**< @brief Back Button width */
#define BACK_BT_HEIGHT 76 /**< @brief Back Button height */
#define BUTTON_WIDTH 70 /**< @brief Button width */
#define BUTTON_HEIGHT 70  /**< @brief Button height */
#define BUTTON_M_X 429  /**< @brief  */
#define BUTTON_P_X 653  /**< @brief  */
#define TIME_BT_Y 218 /**< @brief Time Button y coordinate */
#define VELO_BT_Y 488 /**< @brief Velocity Button y coordinate */
#define NUMBERS_X 537 /**< @brief Numbers x coordinate */
#define SPACE_BETWEEN_NUMBERS 8 /**< @brief Space between number sprites */
#define TIME_N_Y 218  /**< @brief Time value number coordinate y */
#define VELO_N_Y 488  /**< @brief Velocity value number coordinate y */
#define DEFAULT_XY 0 

/**
 * @brief Identifiers of the buttons where mouse can hover
 * 
 */
enum mouse_button_settings_menu{
  DEFAULT,
  MOUSE_BACK,
  MOUSE_PLAYGROUND,
  MOUSE_TIME_BT_M,
  MOUSE_TIME_BT_P,
  MOUSE_VELO_BT_M,
  MOUSE_VELO_BT_P
};

/**
 * @brief Tells where the cursor is hovering 
 * 
 * @param x Cursor x coordinate
 * @param y Cursor y coordinate
 * @return int Value of the button in which the cursor is hovering
 */
int settings_menu_cursor_button(uint16_t x, uint16_t y);

/**
 * @brief Loads all the sprites required by the settings menu
 * 
 */
void settings_load_sprites();

/**
 * @brief Settings Menu entry function
 * 
 * @param video_mem main video buffer
 * @param double_buffer secondary video buffer
 * @param game_time value of time in seconds for each turn
 * @param game_velocity value in which the square blocker shoul move
 * @return int value with information of the next screen
 */
int settings_menu(char * video_mem, char * double_buffer);

/**
 * @brief Determines the flow of execution according to event
 * 
 * @param event current mouse event 
 * @param pp mouse packet 
 * @param double_buffer buffer where changes should happen
 * @return int value with information of the next screen
 */
int settings_state_machine(enum mouse_event event, struct packet *pp, char * double_buffer);

/**
 * @brief Changes the value of the time for each round
 * 
 * @param add True if increasing, otherwise false
 * @return true if the operation was succeful
 * @return false if the operation was not succeful
 */
bool function_time(bool add);
/**
 * @brief Changes the value of velocity for the square blocker in play menu
 * 
 * @param add True if increasing, otherwise false
 * @return true if the operation was succeful
 * @return false if the operation was not succeful
 */
bool function_velocity(bool add);

/**
 * @brief draws sprites in the buffer passed as parameter
 * 
 * @param double_buffer buffer where the sprites should be drawn
 */
void draw_sprites(char * double_buffer);
/**
 * @brief draws the value of time
 * 
 * @param double_buffer buffer where the sprites should be drawn
 */
void draw_time(char * double_buffer);
/**
 * @brief draws the value of the velocity
 * 
 * @param double_buffer buffer where the sprites should be drawn
 */
void draw_velocity(char * double_buffer);

#endif
