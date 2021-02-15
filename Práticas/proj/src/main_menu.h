#ifndef MENU_H
#define MENU_H

#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "video_card.h"
#include "mouse.h"
#include "timer.h"
#include "keyboard.h"

#define PLAY_BUTTON_Y 406   /**< @brief Play Button y coordinate */
#define SETTINGS_BUTTON_Y 523   /**< @brief Settings Button y coordinate  */
#define EXIT_BUTTON_Y 643   /**< @brief Exit Button y coordinate  */
#define MAIN_MENU_BUTTON_X 386  /**< @brief Buttons x coordinate */
#define MAIN_MENU_BUTTON_WIDTH 384  /**< @brief Buttons width */
#define MAIN_MENU_BUTTON_HEIGHT 102 /**< @brief Buttons height */

/**
 * @brief Identifiers of the buttons where mouse can hover
 * 
 */
enum mouse_button_main_menu {
    MOUSE_NOT_IN_BUTTON,
    MOUSE_PLAY_BUTTON,
    MOUSE_SETTINGS_BUTTON,
    MOUSE_EXIT_BUTTON,
};

/**
 * @brief Identifiers for the next screen
 * 
 */
enum main_menu_next_screen {
    NOT_IN_BREAK_CONDITION,
    PLAY_SCREEN, 
    SETTINGS_SCREEN, 
    EXIT
};

/**
 * @brief Main Menu entry function
 * 
 * @param video_mem main video buffer
 * @param double_buffer secondary video buffer
 * @return int value with information of the next screen
 */
int main_menu(void * video_mem, void * double_buffer);

/**
 * @brief Tells where the cursor is hovering 
 * 
 * @param x Cursor x coordinate
 * @param y Cursor y coordinate
 * @return int Value of the button in which the cursor is hovering
 */
int main_menu_cursor_button(uint16_t x, uint16_t y);
/**
 * @brief Determines the flow of execution according to event
 * 
 * @param event current mouse event
 * @param pp mouse packet event for next event detection
 * @param double_buffer secondary video buffer, where changes should be made 
 * @param sprites sprites that should be drawn to the screen
 * @return int value with information of the next screen
 */
int main_menu_state_machine(enum mouse_event event, struct packet *pp, char * double_buffer, Sprite * sprites[]);

#endif
