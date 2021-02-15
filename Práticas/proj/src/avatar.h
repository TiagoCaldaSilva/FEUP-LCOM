#ifndef AVATAR_H
#define AVATAR_H

#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "utils.h"
#include "video_card.h"
#include "mouse.h"
#include "keyboard.h"

#define AVATAR_WIDTH 256    /**< @brief Avatar Width */
#define AVATAR_HEIGHT 256   /**< @brief Avatar Height */

#define AVATAR1_X 160   /**< @brief Left Avatar x position in avatar menu */
#define AVATAR1_Y 260   /**< @brief Left Avatar y position in avatar menu */
#define AVATAR2_X 740   /**< @brief Right Avatar x position in avatar menu */
#define AVATAR2_Y 260   /**< @brief Right Avatar y position in avatar menu */

#define AVATAR_SPEED_X 3    /**< @brief Avatar animation speed in the x axis */
#define AVATAR_SPEED_Y 0    /**< @brief Avatar animation speed in the y axis */

#define READY1_X 50 /**< @brief Avatar Left Ready button x coordinate */
#define READY2_X 626    /**< @brief Avatar Right Ready button x coordinate */
#define READY_Y 766 /**< @brief Avatar Ready button y coordinate */

/* Avatar Animation Macros */
#define DEFAULT_XY 0   
#define BACK1_X 39  
#define BACK2_X 617 
#define FRONT1_X 467    
#define FRONT2_X 1043   
#define BACK_FRONT_Y 355    
#define BACK_FRONT_WH 70    
#define BACKGROUND_INDEX 0  
#define DELTA_X_AVATAR 51   
#define AVATAR_PLAYER1_FINAL_X 476  
#define AVATAR_PLAYER1_INITIAL_X 100    
#define AVATAR_PLAYER2_INITIAL_X 676    
#define AVATAR_PLAYER2_FINAL_X 1052 

#define LETTER_WIDTH 28 /**< @brief width of a letter sprite */
#define LETTER_HEIGHT 50    /**< @brief height of a letter sprite */
#define SPACE_BETWEEN_LETTERS 1 /**< @brief number of pixels between sprite letters */

#define PLAYER1_NAME_INITIAL_X 194  /**< @brief Initial coordinate in the x axis for the name of player1 */
#define PLAYER2_NAME_INITIAL_X 770  /**< @brief Initial coordinate in the x axis for the name of player2 */
#define PLAYER_NAME_Y 688   /**< @brief Initial coordinate in the y axis for the name of bothe players */

#define NAME_MAX_LENGTH 11  /**< @brief Maximum Player name Lenght */

/**
 * @brief Identifier of the states of avatar_menu_state_machine
 * 
 */
enum mouse_button_avatar_menu{
    MOUSE_BG,
    MOUSE_READY1,
    MOUSE_READY2,
    MOUSE_BACK1,
    MOUSE_FRONT1,
    MOUSE_BACK2,
    MOUSE_FRONT2,
    NAME_PLAYER1,
    NAME_PLAYER2
};

/**
 * @brief Identifiers of the states that determine which player is writing
 * 
 */
enum player_write{
    player_1,
    player_2,
    NOBODY
};

/**
 * @brief Identifiers for the button pressed
 * 
 */
enum selection{
    PLAY,
    LEAVE,
    SELECT
};

/**
 * @brief Loads all the different avatar menu sprites
 * 
 */
void select_avatar_load_sprites();

/**
 * @brief adds a letter to the player 1 name
 * 
 * @param index According to the letter
 */
void player1_add_letter(int index);
/**
 * @brief adds a letter to the player 2 name
 * 
 * @param index According to the letter
 */
void player2_add_letter(int index);
/**
 * @brief removes a letter to the player 1 name
 * 
 */
void player1_remove_letter();
/**
 * @brief adds a letter to the player 2 name
 * 
 */
void player2_remove_letter();

/**
 * @brief Tells where the cursor is hovering 
 * 
 * @param x Cursor x coordinate
 * @param y Cursor y coordinate
 * @return int Value of the button in which the cursor is hovering
 */
int select_avatar_cursor_button(uint16_t x, uint16_t y);

/**
 * @brief Select Avatar Menu entry function 
 * 
 * @param video_mem main video buffer
 * @param double_buffer secondary video buffer
 * @param final_player1_icon value of the player 1 icon
 * @param final_player2_icon value of the player 2 icon
 * @return int value with information of the next screen
 */
int select_avatar_menu(char * video_mem, char * double_buffer, int *final_player1_icon, int * final_player2_icon);

/**
 * @brief Determines the flow of execution according to event
 * 
 * @param event current mouse event
 * @param pp mouse packet event for next event detection
 */
void select_avatar_state_machine(enum mouse_event event, struct packet *pp);

/**
 * @brief Moves the avatar sprite
 * 
 * @param right true if it should move to the right, false otherwise
 * @param player1 true if the avatar moved is in player1 space, false otherwise 
 * @return int with the value of the sprite that should appear next
 */
int move_sprite(bool right, bool player1);

/**
 * @brief Cleans the avatar that was moved out of sight
 * 
 * @param x Initial coordinate of the x axis where the sprite passed as sp should be drawn
 * @param y Initial coordinate of the y axis where the sprite passed as sp should be drawn
 * @param sp sprite to be moved
 * @param right true if it should move to the right, false otherwise
 * @param limit coordinate of the limit where sprite should disapear
 * @param prev true if the sprite to move was already being shown, otherwise false
 */
void clear_moved_avatar(uint16_t x, uint16_t y, Sprite * sp, bool right, uint16_t limit, bool prev);

/**
 * @brief updates the contents of the main video buffer with the ones from the secondary video buffer 
 * 
 */
void refresh_avatar_double_buffer();

/**
 * @brief Moves the previous avatar sprite to the right
 * 
 * @param x Initial coordinate of the x axis where the sprite passed as sp should be drawn
 * @param y Initial coordinate of the y axis where the sprite passed as sp should be drawn
 * @param limit coordinate of the limit where sprite should disapear
 * @param sp sprite to be moved
 * @param number_bytes_per_pixel number of bytes per pixel (In 0x14C is 4)
 */
void move_prev_right(uint16_t x, uint16_t y, uint16_t limit, Sprite * sp, int number_bytes_per_pixel);
/**
 * @brief Moves the next avatar sprite to the left
 * 
 * @param x Initial coordinate of the x axis where the sprite passed as sp should be drawn
 * @param y Initial coordinate of the y axis where the sprite passed as sp should be drawn
 * @param limit coordinate of the limit where sprite should disapear
 * @param sp sprite to be moved
 * @param number_bytes_per_pixel number of bytes per pixel (In 0x14C is 4)
 */
void move_next_left(uint16_t x, uint16_t y, uint16_t limit, Sprite * sp, int number_bytes_per_pixel);
/**
 * @brief Moves the previous avatar sprite to the left
 * 
 * @param x Initial coordinate of the x axis where the sprite passed as sp should be drawn
 * @param y Initial coordinate of the y axis where the sprite passed as sp should be drawn
 * @param limit coordinate of the limit where sprite should disapear
 * @param sp sprite to be moved
 * @param number_bytes_per_pixel number of bytes per pixel (In 0x14C is 4)
 */
void move_prev_left(uint16_t x, uint16_t y, uint16_t limit, Sprite * sp, int number_bytes_per_pixel);
/**
 * @brief Moves the next avatar sprite to the right
 * 
 * @param x Initial coordinate of the x axis where the sprite passed as sp should be drawn
 * @param y Initial coordinate of the y axis where the sprite passed as sp should be drawn
 * @param limit coordinate of the limit where sprite should disapear
 * @param sp sprite to be moved
 * @param number_bytes_per_pixel number of bytes per pixel (In 0x14C is 4)
 */
void move_next_right(uint16_t x, uint16_t y, uint16_t limit, Sprite * sp, int number_bytes_per_pixel);

/**
 * @brief Draws the sprites required for avatar menu
 * 
 * @param index tells where the mouse cursor is hovering 
 */
void select_avatar_draw_actual_sprites(int index);

#endif
