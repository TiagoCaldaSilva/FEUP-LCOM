#include "video_card.h"

int video_card_get_info(uint16_t mode, vbe_mode_info_t * vbe_info){
    mmap_t map;
    lm_alloc(sizeof(vbe_mode_info_t), &map);

    reg86_t r86;
    memset(&r86, 0, sizeof(r86));

    r86.ax = VBE_FUNCTION(VBE_MODE_INFO);
    r86.cx = MODE(mode);
    r86.intno = VBE_INTNO;
    r86.es = PB2BASE(map.phys);
    r86.di = PB2OFF(map.phys);

    if(sys_int86(&r86) != OK) return 1;

    *vbe_info = *(vbe_mode_info_t *)map.virt;

    lm_free(&map);

    return 0;
}

void * video_card_init(uint16_t mode, vbe_mode_info_t *vbe_info){
    if(video_card_get_info(mode, vbe_info) != OK) return NULL;

    static void *video_mem;

    struct minix_mem_range mr;
    unsigned int vram_base = vbe_info->PhysBasePtr;
    unsigned int vram_size = vbe_info->XResolution * vbe_info->YResolution * ceil(vbe_info->BitsPerPixel / 8.0);
    mr.mr_base = (phys_bytes) vram_base;	
    mr.mr_limit = mr.mr_base + vram_size; 

    int r;
    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) return NULL;

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(video_mem == MAP_FAILED) return NULL;

    reg86_t r86;
    memset(&r86, 0, sizeof(r86));

    r86.ax = VBE_FUNCTION(VBE_SET_MODE);
    r86.bx = MODE(mode);
    r86.intno = VBE_INTNO;
    if( sys_int86(&r86) != OK ) return NULL;

    return video_mem;
}

// Not working properly so we've decided to stay with double buffering with full copy
int video_card_page_flip(vbe_mode_info_t *vbe_info) {

    reg86_t r86;
    memset(&r86, 0, sizeof(r86));

    r86.ax = VBE_FUNCTION(VBE_DISPLAY_START);
    r86.bh = 0x00; r86.bh = 0x00;
    r86.cx = 0x00; r86.dx = vbe_info->YResolution;
    r86.intno = VBE_INTNO;


    if( sys_int86(&r86) != OK ) return 1;
    return 0;
}

extern vbe_mode_info_t vbe_info;
uint32_t video_card_get_pixel_color(char* buffer, uint16_t x, uint16_t y) {
    uint8_t number_bytes_per_pixel = ceil(vbe_info.BitsPerPixel / 8); //Obtain the number of bytes per pixel
    char* canvas = buffer + ((vbe_info.XResolution * y) + x) * number_bytes_per_pixel; //Get the memory position of the pixel
    uint32_t color = 0x00; uint8_t aux_color = 0x00;

    for (int i = number_bytes_per_pixel - 1; i >= 0; i--) { //Get the color of that position
        aux_color = canvas[i]; color = (color << 8) | aux_color;
    }
    return color;
}

void video_card_paint_pixel(char* buffer, uint16_t x, uint16_t y, uint32_t color) {
    uint8_t number_bytes_per_pixel = ceil(vbe_info.BitsPerPixel / 8);
    char* canvas = buffer + ((vbe_info.XResolution * y) + x) * number_bytes_per_pixel;

    // There's no need to use color masks because 0x14C works with 32 bits colors
    for (int i = 0; i < number_bytes_per_pixel - 1; i++) 
        canvas[i] = color >> (i*8);
}

int clear_buffer(void *buffer, vbe_mode_info_t * vbe_info){
  memset(buffer, 0, vbe_info->XResolution * vbe_info->YResolution * ceil(vbe_info->BitsPerPixel / 8.0));
  return 0;
}

int video_card_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, char * buffer){
    
    for(int j = y; j < height + y; j++){ //Number of lines
        for(int i = 0; i < width; i++) //number of pixels that should color per line
            video_card_paint_pixel(buffer, x + i, j, color);
    }
    return 0;
}
