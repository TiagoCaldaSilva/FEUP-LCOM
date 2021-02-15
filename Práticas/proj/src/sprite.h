#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include <math.h>
#include "video_card.h"

#ifndef PROJ_SPRITE_H
#define PROJ_SPRITE_H

#define STATIC_SPRITE_SPEED 0  /**< @brief Speed of a static sprite */
#define WHITE 0xFFFFFF  

/**
 * @brief Sprite Struct
 * 
 */
typedef struct {
    int x, y;           // current position
    int width, height;  // dimensions
    int xspeed, yspeed; // current speed
    uint32_t *map, *replace_color;    // the pixmaps
    int current_sprite, number_of_pixmaps;
} Sprite;

/**
 * @brief Create a sprite object
 * 
 * @param pic pixmap containig the shape of the sprite
 * @param x Initial x coordinate
 * @param y Initial y coordinate
 * @param xspeed Initial speed in the x axis
 * @param yspeed Initial speed in the y axis
 * @return Sprite* Returns NULL on invalid pixmap
 */
Sprite *create_sprite(const char *pic[], int x, int y, int xspeed, int yspeed);
/**
 * @brief Destroys the sprite passed as parameter
 * 
 * @param sp Sprite which will be destroyed
 */
void destroy_sprite(Sprite *sp);

/**
 * @brief Draws the sprite in the given buffer in the position by sprite.x and sprite.y
 * 
 * @param sp Sprite to be drawn
 * @param buffer Buffer where the sprite passed as parameter should drawn
 */
void draw_sprite(Sprite *sp, char *buffer);
/**
 * @brief Draws the sprite in the given buffer in a different position from sprite.x and sprite.y
 * 
 * @param x Starting Draw x coordinate
 * @param y Starting Draw y coordinate
 * @param sp Sprite to be drawn
 * @param buffer Buffer where the sprite passed as parameter should drawn
 */
void draw_sprite_in(uint16_t x, uint16_t y, Sprite *sp, char * buffer);
/**
 * @brief Erases the sprite from a given buffer
 * 
 * @param sp Sprite to be erased
 * @param buffer Buffer where the sprite passed as parameter should erased
 */
void erase_sprite(Sprite *sp, char *buffer);

/**
 * @brief Swaps Sprites thus creating animations effects
 * 
 * @param double_buffer Buffer where the sprite passed as parameter should be swaped
 * @param sprites Sprites to be swapped
 * @param index position of the swapped sprites in the array passed as parameter
 */
void swap_sprite(char * double_buffer, Sprite * sprites[], int index);

/**
 * @brief Same as draw_sprites but allows drawing beyond the screen borders or even not the whole sprite
 * 
 * @param pos_x Starting Draw x coordinate
 * @param pos_y Starting Draw y coordinate
 * @param bk Background Sprite
 * @param max_x x coordinate limit
 * @param max_y y coordinate limit
 * @param buffer Buffer where the sprite passed as parameter should drawn
 * @param n_bytes number of bytes per pixel (In 0x14C is 4)
 */
void draw_part_of(uint16_t pos_x, uint16_t pos_y, Sprite * bk, uint16_t max_x, uint16_t max_y, char * buffer, int n_bytes);

#endif
