#ifndef CANVAS_H
#define CANVAS_H

#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "utils.h"
#include "video_card.h"
#include "sprite.h"

#define CANVAS_X 266    /**< @brief Canvas x coordinate position */
#define CANVAS_Y 32 /**< @brief Canvas y coordinate position */
#define CANVAS_WIDHT 864    /**< @brief Canvas width */
#define CANVAS_HEIGHT 628   /**< @brief Canvas height */   

/**
 * @brief Identifies the current tool in use
 * 
 */
enum paint_tools {
    BRUSH = 8,
    BUCKET, 
    RUBBER,
    COLOR_PICKER,
    CLEANER,
    BRUSH_PRESSED,
    BUCKET_PRESSED,
    RUBBER_PRESSED,
    COLOR_PICKER_PRESSED,
    CLEANER_PRESSED
};

/**
 * @brief Identifies the current brush size in use
 * 
 */
enum brush_size {
    SMALL = 10, MEDIUM = 20, LARGE = 30
};

#define SMALL_BRUSH_SIZE 10 /**< @brief Small brush size */
#define MEDIUM_BRUSH_SIZE 20    /**< @brief Medium brush size */
#define LARGE_BRUSH_SIZE 30 /**< @brief Large brush size */

#define TOOLS_NORMAL_X 32   /**< @brief Initial coordinate x for painting tools when mouse is not hovering */
#define TOOLS_ZOOMED_X 28   /**< @brief Initial coordinate x for painting tools when mouse is hovering */

#define BRUSH_Y 150     /**< @brief Initial coordinate y for brush icon when mouse is not hovering */
#define BUCKET_Y 260    /**< @brief Initial coordinate y for bucket icon when mouse is not hovering */
#define RUBBER_Y 370    /**< @brief Initial coordinate y for rubber icon when mouse is not hovering */
#define COLOR_PICKER_Y 460  /**< @brief Initial coordinate y for color picker icon when mouse is not hovering */
#define CLEANER_Y 570   /**< @brief Initial coordinate y for cleaner icon when mouse is not hovering */
#define TOOLS_NORMAL_SIZE 100   /**< @brief Size of painting tools icons when mouse is hovering */
#define TOOLS_ZOOMED_SIZE 90    /**< @brief Size of painting tools icons when mouse is not hovering */

#define BRUSH_SIZE_Y 782    /**< @brief Initial coordinate y for brush size selection menu */
#define BRUSH_SIZE_HEIGHT 25    /**< @brief Height for brush size selection menu */
#define SMALL_SIZE_X 348    /**< @brief Initial coordinate x for the word SMALL in the brush size selection menu */
#define SMALL_SIZE_WIDTH 140    /**< @brief Width of word SMALL in brush size selection menu */
#define MEDIUM_SIZE_X 598   /**< @brief Initial coordinate x for the word MEDIUM in the brush size selection menu */
#define MEDIUM_SIZE_WIDTH 175   /**< @brief Width of word MEDIUM in brush size selection menu */
#define LARGE_SIZE_X 896    /**< @brief Initial coordinate x for the word LARGE in the brush size selection menu */
#define LARGE_SIZE_WIDTH 148    /**< @brief Width of word LARGE in brush size selection menu */

#define CANVAS_DEFAULT_COLOR 0x00FFFFFF     /**< @brief Canvas default color - white */

/**
 * @brief Fills the gaps between mouse movements with the correct color
 * 
 * @param x Starting point - x coordinate
 * @param y Starting point - y coordinate
 * @param delta_x Delta value for x axis 
 * @param delta_y Delta value for y axis 
 * @param color Color to fill the gaps
 * @param brush_s Current size of the brush
 * @param rubber true if current tool is the rubber, false if it is brush
 */
void canvas_fill_brush(uint16_t x, uint16_t y, int16_t delta_x, int16_t delta_y, uint32_t color, enum brush_size brush_s, bool rubber);
/**
 * @brief Called upon a event that requires painting and the current tool is the brush or the rubber
 * 
 * @param x Starting point - x coordinate
 * @param y Starting point - y coordinate
 * @param color Color to fill the gaps
 * @param brush_s Current size of the brush
 * @param rubber true if current tool is the rubber, false if it is brush
 */
void canvas_draw_brush(uint16_t x, uint16_t y, uint32_t color, enum brush_size brush_size, bool rubber);

/**
 * @brief Emulates the bucket tool, using the flood-fill algorithm in playground
 * 
 * @param x Starting point - x coordinate
 * @param y Starting point - y coordinate
 * @param color Color to fill the corresponding area
 * @param previous_color Color that determines if the pixel should be painted or not
 */
void canvas_bucket_fill(int x, int y, uint32_t color, uint32_t previous_color);
/**
 * @brief Emulates the bucket tool, using the flood-fill algorithm in Play Menu
 * 
 * @param x Starting point - x coordinate
 * @param y Starting point - y coordinate
 * @param color Color to fill the corresponding area
 * @param previous_color Color that determines if the pixel should be painted or not
 * @param blocker_x blocker x coordinate
 * @param blocker_y blocker y coordinate
 * @param blocker_width blocker width
 * @param blocker_height blocker height
 */
void canvas_bucket_fill_play_mode(int x, int y, uint32_t color, uint32_t previous_color, uint16_t blocker_x, uint16_t blocker_y, uint16_t blocker_width, uint16_t blocker_height);
/**
 * @brief Implementation of the flood-fill algorithm (Discussed more closely in the report)
 * 
 * @param x Starting point - x coordinate
 * @param y Starting point - y coordinate
 * @param previous_color Color that determines if the pixel should be painted or not
 * @param new_color The color in which the pixels should be painted
 */
void canvas_flood_fill(uint16_t x, uint16_t y, uint32_t previous_color, uint32_t new_color);
/**
 * @brief Paints All the pixels to the left to avoid stack Overflow in Playground
 * 
 * @param x Starting point - x coordinate
 * @param y Starting point - y coordinate
 * @param previous_color Color that determines if the pixel should be painted or not
 * @param new_color The color in which the pixels should be painted
 */
void canvas_flood_fill_left(uint16_t x, uint16_t y, uint32_t previous_color, uint32_t new_color);
/**
 * @brief Paints All the pixels to the right to avoid stack Overflow in Playground
 * 
 * @param x Starting point - x coordinate
 * @param y Starting point - y coordinate
 * @param previous_color Color that determines if the pixel should be painted or not
 * @param new_color The color in which the pixels should be painted
 */
void canvas_flood_fill_right(uint16_t x, uint16_t y, uint32_t previous_color, uint32_t new_color);

/**
 * @brief Paints All the pixels to the left to avoid stack Overflow in Play Menu
 * 
 * @param x Starting point - x coordinate
 * @param y Starting point - y coordinate
 * @param previous_color Color that determines if the pixel should be painted or not
 * @param new_color The color in which the pixels should be painted
 */
void canvas_flood_fill_left_play_mode(uint16_t x, uint16_t y, uint32_t previous_color, uint32_t new_color, uint16_t blocker_x, uint16_t blocker_y, uint16_t blocker_width, uint16_t blocker_height);
/**
 * @brief Paints All the pixels to the right to avoid stack Overflow in Play Menu
 * 
 * @param x Starting point - x coordinate
 * @param y Starting point - y coordinate
 * @param previous_color Color that determines if the pixel should be painted or not
 * @param new_color The color in which the pixels should be painted
 */
void canvas_flood_fill_right_play_mode(uint16_t x, uint16_t y, uint32_t previous_color, uint32_t new_color, uint16_t blocker_x, uint16_t blocker_y, uint16_t blocker_width, uint16_t blocker_height);

/**
 * @brief Retrieves the color of the point in the screen with coordinates x and y
 * 
 * @param x pixel's x coordinate 
 * @param y pixel's y coordinate 
 * @return uint32_t pixel's color in the point passed as parameter
 */
uint32_t canvas_pick_color(uint16_t x, uint16_t y);

/**
 * @brief Paints all the canvas to the default color 
 * 
 */
void canvas_cleaning();

#endif
