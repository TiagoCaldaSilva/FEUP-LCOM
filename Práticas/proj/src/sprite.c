
#include "sprite.h"

extern vbe_mode_info_t vbe_info;

Sprite *create_sprite(const char *pic[], int x, int y, int xspeed, int yspeed) {
    //allocate space for the "object"
    Sprite *sp = (Sprite *) malloc (sizeof(Sprite));
    xpm_image_t img;
    if(sp == NULL) return sp;

    // read the sprite pixmap
    xpm_load(pic, XPM_8_8_8_8, &img);
    sp->map = (uint32_t *)img.bytes;
    if( sp->map == NULL ) {
        free(sp);
        return NULL;
    }

    sp->x = x; sp->y = y;
    sp->xspeed = xspeed; sp->yspeed = yspeed;
    sp->width = img.width; sp->height = img.height;
    sp->replace_color = (uint32_t *)malloc(sizeof(uint32_t) * img.width * img.height); //This has the colors to use when the sprite is removed
    return sp;
}

void destroy_sprite(Sprite *sp) {
    if(sp == NULL) return;
    if(sp->map)
        free(sp->map);
    if(sp->replace_color)
        free(sp->replace_color);
    free(sp);
    sp = NULL;
}

void draw_sprite(Sprite *sp, char *buffer) {
    uint8_t number_bytes_per_pixel = ceil(vbe_info.BitsPerPixel / 8.0);//Obtain the number of bytes per pixel

    sp->replace_color = (uint32_t *)calloc(sp->width * sp->height, number_bytes_per_pixel);//Reset the values of replace color

    char * board = buffer + ((vbe_info.XResolution * sp->y) + sp->x) * number_bytes_per_pixel;//Get the memory position of the pixel

    for (int i = 0; i < sp->height; i++) {
        for (int j = 0; j < sp->width; j++) {
            sp->replace_color[i * sp->width + j] = video_card_get_pixel_color(buffer, sp->x + j, sp->y + i );//Update the value of the replace color 
            for (int k = 0; k < number_bytes_per_pixel - 1; k++) {
                if(sp->map[i*sp->width + j] != xpm_transparency_color(XPM_8_8_8_8))//Draws only if the color is different of the transparency color
                    board[k] = sp->map[i*sp->width + j] >> (k * 8);
            }
            board += number_bytes_per_pixel;
        }
        board += (vbe_info.XResolution - sp->width) * number_bytes_per_pixel;
    }
}

void erase_sprite(Sprite *sp, char *buffer){
    uint8_t number_bytes_per_pixel = ceil(vbe_info.BitsPerPixel / 8.0);//Obtain the number of bytes per pixel

    char * board = buffer + ((vbe_info.XResolution * sp->y) + sp->x) * number_bytes_per_pixel;//Get the memory position of the pixel
    for (int i = 0; i < sp->height; i++) {
        for (int j = 0; j < sp->width; j++) {
            for (int k = 0; k < number_bytes_per_pixel - 1; k++) {
                board[k] = sp->replace_color[i * sp->width + j]  >> (k * 8); //Erase the sprite
            }

            board += number_bytes_per_pixel;
        }
        board += (vbe_info.XResolution -  sp->width) * number_bytes_per_pixel;
    }
}

// RipOff of animate Sprite
void swap_sprite(char * double_buffer, Sprite * sprites[], int index) {
    draw_sprite(sprites[index], double_buffer);
}

void draw_sprite_in(uint16_t x, uint16_t y, Sprite *sp, char * buffer){
    uint8_t number_bytes_per_pixel = ceil(vbe_info.BitsPerPixel / 8.0);//Obtain the number of bytes per pixel

    char * board = buffer + ((vbe_info.XResolution * y) + x) * number_bytes_per_pixel;//Get the memory position of the pixel
    for(int i = 0; i < sp->height; i++){
        for(int j = 0; j < sp->width; j++){
            sp->replace_color[i * sp->width + j] = video_card_get_pixel_color(buffer, x + j, y + i ); //Update the value of the replace color 
            for (int k = 0; k < number_bytes_per_pixel - 1; k++) {
                if(sp->map[i*sp->width + j] != xpm_transparency_color(XPM_8_8_8_8)) //Draws only if the color is different of the transparency color
                    board[k] = sp->map[i*sp->width + j] >> (k * 8);
            }
            board += number_bytes_per_pixel;
        }
        board += (vbe_info.XResolution - sp->width) * number_bytes_per_pixel;

    }
}

void draw_part_of(uint16_t pos_x, uint16_t pos_y, Sprite * bk, uint16_t max_x, uint16_t max_y, char * buffer, int n_bytes){
    for(int y = 0; y < max_y; y++){
        for(int x = 0; x < max_x; x++){
            video_card_paint_pixel(buffer, pos_x + x, pos_y + y, bk->map[(pos_y + y) * bk->width + (pos_x + x)]);
        }
    }
}
