#include "canvas.h"

extern char * double_buffer;
extern char * canvas_buf;
extern const char * Large_brush_xpm[];
extern const char * Medium_brush_xpm[];
extern const char * Small_brush_xpm[];

void canvas_draw_brush(uint16_t x, uint16_t y, uint32_t color, enum brush_size brush_size, bool rubber) {

    Sprite * small = create_sprite(Small_brush_xpm, 0, 0, 0, 0);
    Sprite * medium = create_sprite(Medium_brush_xpm, 0, 0, 0, 0);
    Sprite * large = create_sprite(Large_brush_xpm, 0, 0, 0, 0);

// Checks if the mouse position is inside the correct coordinates and calls the respective function
    // Accounting for the canvas borders
    Sprite * actual_sprite = (brush_size == SMALL) ? small:(brush_size == MEDIUM) ? medium:large;
    uint8_t real_size_x = (x + brush_size >= CANVAS_X + CANVAS_WIDHT) ? (CANVAS_X + CANVAS_WIDHT) - x : brush_size;
    uint8_t real_size_y = (y + brush_size >= CANVAS_Y + CANVAS_HEIGHT) ? (CANVAS_Y + CANVAS_HEIGHT) - y : brush_size;
    for (uint8_t i = 0; i < real_size_y; i++) {
        for (uint8_t j = 0; j < real_size_x; j++) {
            if(actual_sprite->map[i * actual_sprite->width + j] != xpm_transparency_color(XPM_8_8_8_8)){
                if (rubber){
                    video_card_paint_pixel(double_buffer, x + j, y + i, CANVAS_DEFAULT_COLOR);
                    video_card_paint_pixel(canvas_buf, x + j, y + i, CANVAS_DEFAULT_COLOR);
                }
                else {
                    video_card_paint_pixel(double_buffer, x + j, y + i, color);
                    video_card_paint_pixel(canvas_buf, x + j, y + i, color);
                }
            }
        }
    }
    destroy_sprite(small); destroy_sprite(medium); destroy_sprite(large); 
}

void canvas_fill_brush(uint16_t x, uint16_t y, int16_t delta_x, int16_t delta_y, uint32_t color, enum brush_size brush_s, bool rubber) {
    
    int32_t number_of_deltas_to_fill = max(abs(delta_x), abs(delta_y));

    // this deltas allow a smoother trasition in order to avoid teleportation from the mouse
    int16_t real_delta_x = ((float) delta_x / number_of_deltas_to_fill);
    int16_t real_delta_y = ((float) delta_y / number_of_deltas_to_fill);

    // Draws the inital position before start filling the deltas
    do {
        if(x < CANVAS_X || x > CANVAS_X + CANVAS_WIDHT || y < CANVAS_Y || y > CANVAS_Y + CANVAS_HEIGHT) {break;}
        canvas_draw_brush(x, y, color, brush_s, rubber);

        x += real_delta_x; y += real_delta_y;
        number_of_deltas_to_fill--;
    } while (number_of_deltas_to_fill > 0);
}


/* This function uses the flood fill algorithm */
void canvas_bucket_fill(int x, int y, uint32_t color, uint32_t previous_color) {

    // Checks if the mouse position is inside the correct coordinates
    if (between(x, CANVAS_X, CANVAS_X + CANVAS_WIDHT) && between(y, CANVAS_Y, CANVAS_Y + CANVAS_HEIGHT)) {
        canvas_flood_fill(x, y, previous_color, color);
    } 

}

void canvas_bucket_fill_play_mode(int x, int y, uint32_t color, uint32_t previous_color, uint16_t blocker_x, uint16_t blocker_y, uint16_t blocker_width, uint16_t blocker_height) {

    // Checks if the mouse position is inside the correct coordinates
    if (between(x, CANVAS_X, CANVAS_X + CANVAS_WIDHT) && between(y, CANVAS_Y, CANVAS_Y + CANVAS_HEIGHT)) {
        canvas_flood_fill_left_play_mode(x, y, previous_color, color, blocker_x, blocker_y, blocker_width,blocker_height);
        canvas_flood_fill_right_play_mode(x + 1, y, previous_color, color, blocker_x, blocker_y, blocker_width,blocker_height);

    } 

}

/**
 * Flood-Fill Algorithm implementation (Used to Emulate the bucket paint)
 * Adapted from GeeksToGeeks.coom for more info, please visit:
 * https://www.geeksforgeeks.org/flood-fill-algorithm-implement-fill-paint/ 
*/
void canvas_flood_fill(uint16_t x, uint16_t y, uint32_t previous_color, uint32_t new_color) {
    // Checks if the mouse position is inside the correct coordinates
    if (between(x, CANVAS_X, CANVAS_X + CANVAS_WIDHT) && between(y, CANVAS_Y, CANVAS_Y + CANVAS_HEIGHT)) {
        canvas_flood_fill_left(x, y, previous_color, new_color);
        canvas_flood_fill_right(x + 1, y, previous_color, new_color);

    } 
}

void canvas_flood_fill_left(uint16_t x, uint16_t y, uint32_t previous_color, uint32_t new_color) {
    // Base Cases
    if (x < CANVAS_X || x >= CANVAS_X + CANVAS_WIDHT || y < CANVAS_Y || y >= CANVAS_Y + CANVAS_HEIGHT)return ;
    if (video_card_get_pixel_color(canvas_buf, x, y) != previous_color) return;
    if (video_card_get_pixel_color(canvas_buf, x, y) == new_color) return;
    // Sets the new color to the corresponding pixel
    video_card_paint_pixel(double_buffer, x, y, new_color);
    video_card_paint_pixel(canvas_buf, x, y, new_color);
    // Recursive approach
    canvas_flood_fill_left(x-1, y, previous_color, new_color);
    canvas_flood_fill_left(x, y+1, previous_color, new_color); // baixo
    canvas_flood_fill_left(x, y-1, previous_color, new_color);
}

void canvas_flood_fill_right(uint16_t x, uint16_t y, uint32_t previous_color, uint32_t new_color) {
    // Base Cases
    if (x < CANVAS_X || x >= CANVAS_X + CANVAS_WIDHT || y < CANVAS_Y || y >= CANVAS_Y + CANVAS_HEIGHT)return ;
    if (video_card_get_pixel_color(canvas_buf, x, y) != previous_color) return;
    if (video_card_get_pixel_color(canvas_buf, x, y) == new_color) return;
    // Sets the new color to the corresponding pixel
    video_card_paint_pixel(double_buffer, x, y, new_color);
    video_card_paint_pixel(canvas_buf, x, y, new_color);
    // Recursive approach
    if (video_card_get_pixel_color(canvas_buf, x - 1, y) == previous_color) canvas_flood_fill_left(x - 1, y, previous_color, new_color);
    canvas_flood_fill_right(x+1, y, previous_color, new_color);
    canvas_flood_fill_right(x, y+1, previous_color, new_color);
    canvas_flood_fill_right(x, y-1, previous_color, new_color);
}

void canvas_flood_fill_left_play_mode(uint16_t x, uint16_t y, uint32_t previous_color, uint32_t new_color, uint16_t blocker_x, uint16_t blocker_y, uint16_t blocker_width, uint16_t blocker_height) {
    // Base Cases
    if (x < CANVAS_X || x >= CANVAS_X + CANVAS_WIDHT || y < CANVAS_Y || y >= CANVAS_Y + CANVAS_HEIGHT)return ;
    if(between(x, blocker_x, blocker_x + blocker_width) && between(y, blocker_y, blocker_y + blocker_height)) return;
    if (video_card_get_pixel_color(canvas_buf, x, y) != previous_color) return;
    if (video_card_get_pixel_color(canvas_buf, x, y) == new_color) return;
    // Sets the new color to the corresponding pixel
    video_card_paint_pixel(double_buffer, x, y, new_color);
    video_card_paint_pixel(canvas_buf, x, y, new_color);
    // Recursive approach
    canvas_flood_fill_left_play_mode(x-1, y, previous_color, new_color, blocker_x, blocker_y, blocker_width,blocker_height);
    canvas_flood_fill_left_play_mode(x, y+1, previous_color, new_color, blocker_x, blocker_y, blocker_width,blocker_height);
    canvas_flood_fill_left_play_mode(x, y-1, previous_color, new_color, blocker_x, blocker_y, blocker_width,blocker_height);
}

void canvas_flood_fill_right_play_mode(uint16_t x, uint16_t y, uint32_t previous_color, uint32_t new_color, uint16_t blocker_x, uint16_t blocker_y, uint16_t blocker_width, uint16_t blocker_height) {
    // Base Cases
    if (x < CANVAS_X || x >= CANVAS_X + CANVAS_WIDHT || y < CANVAS_Y || y >= CANVAS_Y + CANVAS_HEIGHT)return ;
    if(between(x, blocker_x, blocker_x + blocker_width) && between(y, blocker_y, blocker_y + blocker_height)) return;
    if (video_card_get_pixel_color(canvas_buf, x, y) != previous_color) return;
    if (video_card_get_pixel_color(canvas_buf, x, y) == new_color) return;
    // Sets the new color to the corresponding pixel
    video_card_paint_pixel(double_buffer, x, y, new_color);
    video_card_paint_pixel(canvas_buf, x, y, new_color);
    // Recursive approach
    if (video_card_get_pixel_color(canvas_buf, x - 1, y) == previous_color) canvas_flood_fill_left_play_mode(x - 1, y, previous_color, new_color, blocker_x, blocker_y, blocker_width,blocker_height);
    canvas_flood_fill_right_play_mode(x+1, y, previous_color, new_color, blocker_x, blocker_y, blocker_width,blocker_height);
    canvas_flood_fill_right_play_mode(x, y+1, previous_color, new_color, blocker_x, blocker_y, blocker_width,blocker_height);
    canvas_flood_fill_right_play_mode(x, y-1, previous_color, new_color, blocker_x, blocker_y, blocker_width,blocker_height);
}

/* Color picker */
uint32_t canvas_pick_color(uint16_t x, uint16_t y) {
    return video_card_get_pixel_color(double_buffer, x, y);
}

void canvas_cleaning() {
    for (int i = CANVAS_Y; i < CANVAS_Y + CANVAS_HEIGHT; i++) {
        for (int j = CANVAS_X; j < CANVAS_X + CANVAS_WIDHT; j++) {
            video_card_paint_pixel(double_buffer, j, i, CANVAS_DEFAULT_COLOR);
            video_card_paint_pixel(canvas_buf, j, i, CANVAS_DEFAULT_COLOR);
        }
    }
}
