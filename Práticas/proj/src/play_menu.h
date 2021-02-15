#ifndef PLAY_MENU_H
#define PLAY_MENU_H

#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "video_card.h"
#include "mouse.h"
#include "keyboard.h"
#include "timer.h"
#include "rtc.h"

#include "canvas.h"

#define FRONT_X 1066    /**< @brief Front Arrow Button x coordinate */
#define FRONT_Y 690 /**< @brief Front Arrow Button y coordinate */
#define BACK_X 261  /**< @brief Back Arrow Button x coordinate */
#define BACK_Y 690  /**< @brief Back Arrow Button y coordinate */
#define PAUSE_X 1066    /**< @brief Pause Button x coordinate */
#define PAUSE_Y 769 /**< @brief Pause Button x coordinate */

#define PLAY_MENU_BUTTON_SIZE 70    /**< @brief Play Menu Buttons size  */
#define MENU_ACTUAL_SPRITES 3   /**< @brief Number of sprites required by Play Menu */
#define MENU_NUMBER_TOOLS 5 /**< @brief Number of tools available in playground */

#define CLOCK_X 3   /**< @brief Clock x coordinate */
#define CLOCK_Y 21  /**< @brief Clock y coordinate */

#define MINI_BOX_X 261  /**< @brief MiniBox x coordinate */
#define MINI_BOX_Y 696  /**< @brief MiniBox y coordinate */
#define MINI_BOX_WIDTH 875  /**< @brief MiniBox width */

#define LEAVE_X 801 /**< @brief Leave Button x coordinate */
#define LEAVE_Y 786 /**< @brief Leave Button y coordinate */
#define LEAVE_WIDTH 236 /**< @brief Leave Button width */
#define LEAVE_HEIGHT 25 /**< @brief Leave Button height */
#define RESUME_X 387    /**< @brief Resume Button x coordinate */
#define RESUME_Y 786    /**< @brief Resume Button y coordinate */
#define RESUME_WIDTH 151    /**< @brief Resume Button width */
#define RESUME_HEIGHT 25    /**< @brief Resume Button height */

#define PL_PALLET_X 157 /**< @brief Pallete x coordinate */
#define PL_PALLET_Y 29  /**< @brief Pallete y coordinate */
#define PL_PALLET_WIDTH 80  /**< @brief Pallete width */
#define PL_PALLET_HEIGHT 637    /**< @brief Pallete height */

#define BACKGROUND_INDEX 0

#define AVATAR_1_ICON_X 30  /**< @brief Player 1 avatar icon x coordinate */
#define AVATAR_2_ICON_X 142 /**< @brief Player 2 avatar icon x coordinate */
#define AVATAR_ICON_Y 697   /**< @brief Player avatar icons y coordinate */

#define SCORE1_X 32     /**< @brief Player 1 Score x coordinate */
#define SCORE2_X 144    /**< @brief Player 2 Score x coordinate */
#define NUMBERS_Y 785   /**< @brief Players' Score y coordinate */

#define DEFAULT_XY 0

/* Mini Menu Macros */
#define MINI_MENU_X 348  /**< @brief MiniMenu x coordinate */
#define MINI_MENU_Y 704  /**< @brief MiniMenu y coordinate */
#define MINI_MENU_WIDTH 701  /**< @brief MiniMenu width */
#define MINI_MENU_FINAL_X 1049   /**< @brief MiniMenu final x coordinate  */
#define MENU_SPEED 3 /**< @brief MiniMenu speed animation */

#define GIVE_UP_X 461   /**< @brief Give Up Button x coordinate */
#define GIVE_UP_Y 715   /**< @brief Give Up Button y coordinate */
#define GIVE_UP_WIDTH 475   /**< @brief Give Up Button width */
#define GIVE_UP_HEIGHT 100  /**< @brief Give Up Button height */

#define DONE_X 461 /**< @brief Done Button x coordinate */
#define DONE_Y 715  /**< @brief Done Button y coordinate */
#define DONE_WIDTH 475  /**< @brief Done Button width */
#define DONE_HEIGHT 100 /**< @brief Done Button height */

#define WORD1_X 368 /**< @brief Word 1 x coordinate */
#define WORD2_X 798 /**< @brief Word 2 x coordinate */
#define WORDS_Y 770 /**< @brief Words y coordinate */

#define LETTER_WIDTH 28 /**< @brief Letter sprites width */
#define LETTER_HEIGHT 50    /**< @brief Letter sprites height */
#define SPACE_BETWEEN_LETTERS 1 /**< @brief number of pixels between Letter sprites */

#define END_GAME_NAME_X 618 /**< @brief End of the game name x coordinate */
#define END_GAME_NAME_Y 757 /**< @brief End of the game name y coordinate */

#define GUESS_Y 724 /**< @brief Guess y coordinate */
#define STATUS_ANSWER_X 276 /**< @brief Status Answer x coordinate */ 
#define STATUS_ANSWER_Y 772 /**< @brief Status Answer y coordinate */

#define CORRECT_ANSWER_PAINTER 45   /**< @brief Score for Painter */
#define CORRECT_ANSWER_GUESSER 90   /**< @brief Score for Guesser */

#define MAX_ROUNDS 3 /**< @brief Number of max rounds per game */

/**
 * @brief Tells if some button was pressed in play menu that changed the contents of MiniMenu
 * 
 */
enum mouse_button_play_menu{
    _MOUSE_NOT_IN_BUTTON,
    MOUSE_FRONT_BUTTON = 4,
    MOUSE_BACK_BUTTON = 5,
    MOUSE_PAUSE_BUTTON = 6
};

/**
 * @brief Tells if some button was pressed in play menu while game was paused
 * 
 */
enum pause_buttons{
    NOT_IN_BOX,
    IN_BOX,
    RESUME_GAME,
    LEAVE_GAME
};

/**
 * @brief Tells if some button was pressed in play menu 
 * 
 */
enum actual_menu{
    DONE = 0,
    BRUSH_SIZE = 1,
    GIVE_UP = 2,
    BRUSH_SIZE_SMALL = 18,
    BRUSH_SIZE_MEDIUM,
    BRUSH_SIZE_LARGE,
    DONE_PRESSED,
    GIVE_UP_PRESSED,
    END
};

/**
 * @brief Identifiers of the states while play menu is being executed
 * 
 */
enum game_status{
    GAME_PAUSED,
    PLAYING_BUTTONS,
    PLAYING_TOOLS,
    PLAYING_MENU,
    PLAYING_DEFAULT,
    CHOOSE_WORD,
    GUESS_WORD,
};

/**
 * @brief Tells the movement of the Square Blocker
 * 
 */
enum movement{
    DOWN,
    UP,
    RIGHT,
    LEFT
};

/**
 * @brief Loads all the different play menu sprites
 * 
 */
void load_sprites();

/**
 * @brief Tells where the cursor is hovering 
 * 
 * @param x Cursor x coordinate
 * @param y Cursor y coordinate
 * @return int Value of the button in which the cursor is hovering
 */
int play_menu_cursor_button(uint16_t x, uint16_t y);
/**
 * @brief Tells where the cursor is hovering 
 * 
 * @param x Cursor x coordinate
 * @param y Cursor y coordinate
 * @return int Value of the button in which the cursor is hovering
 */
int pause_menu_cursor_button(uint16_t x, uint16_t y);

/**
 * @brief Sets the canvas to be ready to be used
 * 
 */
void draw_canvas();

/**
 * @brief Draws the Give Up button according the value passed as parameter
 * 
 * @param pressed value telling if button is pressed or not
 */
void draw_give_up(int pressed);
/**
 * @brief Sets the trail of the brush according to the current brush size
 * 
 * @param button Pressed button value 
 */
void draw_brush_size(int button);
/**
 * @brief Draws the Done button according the value passed as parameter
 * 
 * @param pressed value telling if button is pressed or not
 */
void draw_done(int pressed);

/**
 * @brief Draws all the required sprites by play menu
 * 
 */
void draw_actual_default();
/**
 * @brief Draws all the required sprites by play menu if the game is paused
 * 
 * @param index value of the hovered button
 */
void draw_pause_sprites(int index);
/**
 * @brief Draws all the required sprites by play menu if mouse is hovering in the buttons
 * 
 * @param index value of the hovered button
 */
void draw_actual_mouse_in_buttons(int index);
/**
 * @brief Draws all the required sprites by play menu if mouse is hovering in the buttons
 * 
 * @param index value of the hovered tool icon
 */
void draw_actual_mouse_in_tools(int index);
/**
 * @brief Draws all the required sprites by play menu if mouse is hovering in the buttons
 * 
 * @param index value of the hovered elemnts in menu
 */
void draw_actual_mouse_in_menu(int index);
/**
 * @brief Draws all the required sprites by play menu if the game ends
 * 
 * @param index value of the hovered button
 */
void draw_actual_end(int index);

/**
 * @brief Play MenueEntry function
 * 
 * @param video_mem main video buffer
 * @param double_buffer secondary video buffer
 * @param player1_icon Player 1 avatar icon
 * @param player2_icon Player 2 avatar icon
 * @return int value with information of the next screen
 */
int play_menu(void * video_mem, void * double_buffer, int player1_icon, int player2_icon);
/**
 * @brief Determines the flow of execution according to event
 * 
 * @param event current mouse event
 * @param pp mouse packet event for next event detection
 * @return int value with information of the next screen
 */
int play_menu_state_machine(enum mouse_event event, struct packet *pp);
/**
 * @brief Behaves like a state machine determining which function should be called
 * 
 * @param pp mouse packet 
 * @param curr_tool current painting tool
 * @param curr_color current color
 * @param b_size current brush size
 */
void play_menu_paint_call(struct packet *pp, enum paint_tools curr_tool, uint32_t curr_color, enum brush_size b_size);

/**
 * @brief Draws player 1 score sprite
 * 
 */
void draw_score_1();
/**
 * @brief Draws player 2 score sprite
 * 
 */
void draw_score_2();

/**
 * @brief Moves the menu in a given direction
 * 
 * @param right true if it should move to the right, false otherwise
 * @return int Value of the next menu
 */
int move_menu(bool right);
/**
 * @brief Cleans the contents of previous menu
 * 
 * @param x Initial coordinate of the x axis where the sprite passed as sp should be drawn
 * @param y Initial coordinate of the y axis where the sprite passed as sp should be drawn
 * @param sp sprite to be moved
 * @param right true if it should move to the right, false otherwise
 * @param limit coordinate of the limit where sprite should disapear
 * @param prev true if the sprite to move was already being shown, otherwise false
 */
void clear_moved_menu(uint16_t x, uint16_t y, Sprite * sp, bool right, uint16_t limit, bool prev);
/**
 * @brief Moves the previous menu set of sprite to the right
 * 
 * @param x Initial coordinate of the x axis where the sprite passed as sp should be drawn
 * @param y Initial coordinate of the y axis where the sprite passed as sp should be drawn
 * @param limit coordinate of the limit where sprite should disapear
 * @param sp sprite to be moved
 * @param number_bytes_per_pixel number of bytes per pixel (In 0x14C is 4)
 */
void menu_move_prev_right(uint16_t x, uint16_t y, uint16_t limit, Sprite * sp, int number_bytes_per_pixel);
/**
 * @brief Moves the next menu set of sprite to the left
 * 
 * @param x Initial coordinate of the x axis where the sprite passed as sp should be drawn
 * @param y Initial coordinate of the y axis where the sprite passed as sp should be drawn
 * @param limit coordinate of the limit where sprite should disapear
 * @param sp sprite to be moved
 * @param number_bytes_per_pixel number of bytes per pixel (In 0x14C is 4)
 */
void menu_move_next_left(uint16_t x, uint16_t y, uint16_t limit, Sprite * sp, int number_bytes_per_pixel);
/**
 * @brief Moves the previous menu set of sprite to the left
 * 
 * @param x Initial coordinate of the x axis where the sprite passed as sp should be drawn
 * @param y Initial coordinate of the y axis where the sprite passed as sp should be drawn
 * @param limit coordinate of the limit where sprite should disapear
 * @param sp sprite to be moved
 * @param number_bytes_per_pixel number of bytes per pixel (In 0x14C is 4)
 */
void menu_move_prev_left(uint16_t x, uint16_t y, uint16_t limit, Sprite * sp, int number_bytes_per_pixel);
/**
 * @brief Moves the next menu set of sprite to the right
 * 
 * @param x Initial coordinate of the x axis where the sprite passed as sp should be drawn
 * @param y Initial coordinate of the y axis where the sprite passed as sp should be drawn
 * @param limit coordinate of the limit where sprite should disapear
 * @param sp sprite to be moved
 * @param number_bytes_per_pixel number of bytes per pixel (In 0x14C is 4)
 */
void menu_move_next_right(uint16_t x, uint16_t y, uint16_t limit, Sprite * sp, int number_bytes_per_pixel);

/**
 * @brief updates the contents of the main video buffer with the ones from the secondary video buffer 
 * 
 */
void refresh_menu_play_double_buffer();

/**
 * @brief Adds a letter sprite to the guess word
 * 
 * @param index Value of the position of the letter in the array of characters
 * @return true if the insertion was succeful
 * @return false if the insertion was not succeful
 */
bool guess_add_letter(int index);
/**
 * @brief Removes the last letter sprite from the guess word
 * 
 */
void guess_remove_letter();

/**
 * @brief Reads from a text file a set of words
 * 
 * @return true if the reading was succeful
 * @return false if the reading was not succeful
 */
bool readfromFile();

/**
 * @brief Chooses a word from the set of words, randomly
 * 
 */
void choose_word();
/**
 * @brief Draws the choosen word in the screen
 * 
 */
void draw_choose_word();

/**
 * @brief Draws the guess word in the screen
 * 
 */
void draw_guess_word();
/**
 * @brief Draws the correct awnser icon
 * 
 * @return true if the awnser given is correct
 * @return false if the answer given is wrong
 */
bool draw_guess();

/**
 * @brief Moves the blocker right
 * 
 */
void move_blocker_right();
/**
 * @brief Moves the blocker left
 * 
 */
void move_blocker_left();
/**
 * @brief Moves the blocker up
 * 
 */
void move_blocker_up();
/**
 * @brief Moves the blocker down
 * 
 */
void move_blocker_down();

/**
 * @brief Moves the blocker
 * 
 */
void move_blocker();

#endif
