#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "vbe.h"

/**
 * @brief Fills the vbe_info struct with all the graphic mode information
 * 
 * @param mode value for graphic mode
 * @param vbe_info struct which contains the info about the graphic mode
 * @return int 0 upon sucess, non-zero otherwise
 */
int video_card_get_info(uint16_t mode, vbe_mode_info_t * vbe_info);

/**
 * @brief Initializes video card in the desired graphic mode
 * 
 * @param mode value for graphic mode
 * @param vbe_info struct which contains the info about the graphic mode
 * @return void* containg the adress of the initial pixel
 */
void * video_card_init(uint16_t mode, vbe_mode_info_t *vbe_info);

/**
 * @brief Uses the function 07h from VBE to use page flipping (NOT WORKING)
 *        We decided to staw with double buffering with a dynamic allocated buffer making a full copy in every frame
 *          
 * @param vbe_info struct which contains the info about the graphic mode
 * @return int 0 upon sucess, non-zero otherwise
 */
int video_card_page_flip(vbe_mode_info_t *vbe_info);

/**
 * @brief Retrieves the color of the pixel in the position (x, y) in the given buffer
 * 
 * @param buffer where the color should be read
 * @param x position in the screen's plane
 * @param y position in the screen's plane
 * @return uint32_t value of the color in the buffer
 */
uint32_t video_card_get_pixel_color(char* buffer, uint16_t x, uint16_t y);
/**
 * @brief Paints the pixel in the position (x, y) in the given buffer
 * 
 * @param buffer where the pixel should be changed
 * @param x position in the screen's plane
 * @param y position in the screen's plane
 * @param color value for the pixel's new color
 */
void video_card_paint_pixel(char* buffer, uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Clears the given buffer
 * 
 * @param buffer where all the content should be deleted
 * @param vbe_info struct which contains the info about the graphic mode
 * @return int 0 upon sucess, non-zero otherwise
 */
int clear_buffer(void *buffer, vbe_mode_info_t * vbe_info);

/**
 * @brief draws a reactangle of a given color with a given color
 * 
 * @param x Starting coordinate in x axis
 * @param y Starting coordinate in y axis
 * @param width width of the rectangle 
 * @param height height of the rectangle  
 * @param color color of the rectangle
 * @param buffer buffer where rectangle should be drawn
 * @return int 0 upon sucess, non-zero otherwise
 */
int video_card_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, char * buffer);
