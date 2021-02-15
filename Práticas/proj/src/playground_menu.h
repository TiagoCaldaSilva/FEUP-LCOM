#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "video_card.h"
#include "mouse.h"
#include "keyboard.h"
#include "timer.h"

#include "canvas.h"

#define PL_PLAY_MENU_BUTTON_SIZE 70 /**< @brief Size of the buttons in playground */    
#define PL_MENU_ACTUAL_SPRITES 2    /**< @brief Number of sprites required by playground */
#define PL_MENU_NUMBER_TOOLS 5      /**< @brief Number of tools available in playground */

#define PL_MINI_BOX_X 261   /**< @brief MiniBox x coordinate */
#define PL_MINI_BOX_Y 696   /**< @brief MiniBox y coordinate */
#define PL_MINI_BOX_WIDTH 875   /**< @brief MiniBox width */

#define PL_PALLET_X 157 /**< @brief Pallete x coordinate */
#define PL_PALLET_Y 29  /**< @brief Pallete y coordinate */
#define PL_PALLET_WIDTH 80  /**< @brief Pallete width */
#define PL_PALLET_HEIGHT 637    /**< @brief Pallete height */

#define PL_BACKGROUND_INDEX 0

#define PL_DEFAULT_XY 0

/* Mini Menu Macros */
#define PL_MINI_MENU_X 348  /**< @brief MiniMenu x coordinate */
#define PL_MINI_MENU_Y 704  /**< @brief MiniMenu y coordinate */
#define PL_MINI_MENU_WIDTH 701  /**< @brief MiniMenu width */
#define PL_MINI_MENU_FINAL_X 1049   /**< @brief MiniMenu final x coordinate  */
#define PL_MENU_SPEED 3 /**< @brief MiniMenu speed animation */

/**
 * @brief Tells whether a mouse is hovering the leave button or not
 * 
 */
enum playground_mouse_button_play_menu{
    PL_MOUSE_NOT_IN_BUTTON,
    PL_LEAVE_GAME
};

/**
 * @brief Identifies the current brush size in use
 * 
 */
enum playground_actual_menu{
    PL_BRUSH_SIZE = 0,
    PL_BRUSH_SIZE_SMALL = 18,
    PL_BRUSH_SIZE_MEDIUM,
    PL_BRUSH_SIZE_LARGE,
};

/**
 * @brief Identifiers of the states while playground is being executed
 * 
 */
enum playground_game_status{
    PL_PLAYING_BUTTONS,
    PL_PLAYING_TOOLS,
    PL_PLAYING_MENU,
    PL_PLAYING_DEFAULT,
};

/**
 * @brief Loads all the different playground sprites
 * 
 */
void playground_load_sprites();

/**
 * @brief Playground entry function
 * 
 * @param video_mem main video buffer
 * @param double_buffer secondary video buffer
 * @return int value with information of the next screen
 */
int playground_menu(void * video_mem, void * double_buffer);

/**
 * @brief Determines the flow of execution according to event
 * 
 * @param event current mouse event
 * @param pp mouse packet event for next event detection
 * @return int value with information of the next screen
 */
int playground_state_machine(enum mouse_event event, struct packet *pp);

/**
 * @brief Tells where the cursor is hovering 
 * 
 * @param x Cursor x coordinate
 * @param y Cursor y coordinate
 * @return int Value of the button in which the cursor is hovering
 */
int playground_cursor_button(uint16_t x, uint16_t y);

/**
 * @brief Behaves like a state machine determining which function should be called
 * 
 * @param pp mouse packet 
 * @param curr_tool current painting tool
 * @param curr_color current color
 * @param b_size current brush size
 */
void playground_paint_call(struct packet *pp, enum paint_tools curr_tool, uint32_t curr_color, enum brush_size b_size);

/**
 * @brief Sets the canvas to be ready to be used
 * 
 */
void playground_draw_canvas();

/**
 * @brief Sets the trail of the brush according to the current brush size
 * 
 * @param button Pressed button value 
 */
void playground_draw_brush_size(int button);

/**
 * @brief Draws all the required sprites by playground
 * 
 */
void playground_draw_actual_default();
/**
 * @brief Draws all the required sprites by playground if mouse is hovering in the buttons
 * 
 * @param index value of the hovered button
 */
void playground_draw_actual_mouse_in_buttons(int index);
/**
 * @brief Draws all the required sprites by playground if mouse is hovering in the buttons
 * 
 * @param index value of the hovered tool icon
 */
void playground_draw_actual_mouse_in_tools(int index);
/**
 * @brief Draws all the required sprites by playground if mouse is hovering in the buttons
 * 
 * @param index value of the hovered elemnts in menu
 */
void playground_draw_actual_mouse_in_menu(int index);

/**
 * @brief updates the contents of the main video buffer with the ones from the secondary video buffer 
 * 
 */
void playground_refresh_double_buffer();

#endif
