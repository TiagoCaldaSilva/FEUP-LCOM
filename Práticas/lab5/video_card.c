#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Device Header Files
#include "keyboard.h"
#include "video_card.h"
#include "timer.h"

vbe_mode_info_t mode_info;
char * adress;

void (get_info)(uint16_t mode, vbe_mode_info_t *info){
    mmap_t map;
    lm_alloc(sizeof(vbe_mode_info_t), &map);

    reg86_t r;
    memset(&r, 0, sizeof(r));

    r.ax = 0x4F01; // VBE call, function 01 -- get VBE mode
    r.cx = 1<<14 | mode;
    r.intno = 0x10;
    r.es = PB2BASE(map.phys);
    r.di = PB2OFF(map.phys);
    if( sys_int86(&r) != OK ) {
        printf("set_vbe_mode: sys_int86() failed \n");
    }
    *info = *(vbe_mode_info_t *)map.virt;
    lm_free(&map);
}

void (get_pallet)(uint16_t *pallet){
    reg86_t r;
    memset(&r, 0, sizeof(r));

    r.ax = 0x4F09; // VBE call, function 01 -- get VBE mode
    r.intno = 0x10;
    r.bl = 0x01;
    if( sys_int86(&r) != OK ) {
        printf("set_vbe_mode: sys_int86() failed \n");
    }
    pallet = &r.ax;
}

extern uint8_t data;
int kbc_interrupt(uint8_t breakcode) {
    uint8_t bit_no;
    message msg;
    int ipc_status, r;
    if(kbd_subscribe_int(&bit_no) != OK)
        return 1;
    uint32_t irq_set = BIT(bit_no); //Identifier used to comunicate kernel about an interrupt
    while(data != breakcode)
    {
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r); continue;
        }
        if(is_ipc_notify(ipc_status)) {
            switch(_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set){
                        kbc_ih();
                        if(data == TWO_BYTES_CODE) kbc_ih();
                        break;
                }
                default:
                    break;
            }
        }
    }
    if(kbd_unsubscribe_int() != OK)
        return 1;
 
    return 0;
}

vbe_mode_info_t mode_info;
void * (vg_init)(uint16_t mode){

    get_info(mode, &mode_info);

    static void *video_mem;       // frame-buffer VM address 

    struct minix_mem_range mr;
    unsigned int vram_base = (phys_bytes) mode_info.PhysBasePtr;  // VRAM's physical addresss
    unsigned int vram_size;
    if(mode_info.BitsPerPixel % 8 != 0){
        vram_size = mode_info.XResolution * mode_info.YResolution * ((mode_info.BitsPerPixel / 8) +1);
    }
    else{
        vram_size = mode_info.XResolution * mode_info.YResolution * (mode_info.BitsPerPixel / 8);
    }

    int _r;				    

    // Allow memory mapping 

    mr.mr_base = vram_base;	
    mr.mr_limit = mr.mr_base + vram_size;  

    if( OK != (_r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", _r);

    // Map memory 

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(video_mem == MAP_FAILED){
        panic("couldn't map video memory");
        return NULL;
    }

    reg86_t r;
    memset(&r, 0, sizeof(r));	    
    r.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
    r.bx = BIT(14) | mode; // set bit 14: linear framebuffer
    r.intno = 0x10;
    if( sys_int86(&r) != OK ) {
        printf("set_vbe_mode: sys_int86() failed \n");
    }

    return video_mem;
}

uint32_t get_mask(int k) {
    uint32_t mask = (0xFF << (8 * k));
    return mask;
}

extern char * adress;
void paint_img(uint16_t xi, uint16_t yi, xpm_image_t* img) {
    if (xi + img->width > mode_info.XResolution) img->width = mode_info.XResolution - xi;
    if (yi + img->height > mode_info.YResolution) img->height = mode_info.YResolution - yi;
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            paint_pixel(xi + j, yi + i, 1, img->bytes[i*img->width + j]);
        }
    }
}

void clear_img(uint16_t xi_old, uint16_t yi_old, uint16_t speed, xpm_image_t* pixmap, bool horizontal, bool positive) {
    if (horizontal) {
        if (positive) vg_draw_rectangle(xi_old, yi_old, speed, pixmap->height, xpm_transparency_color(XPM_INDEXED));
        else vg_draw_rectangle(xi_old + pixmap->width - speed, yi_old, speed, pixmap->height, xpm_transparency_color(XPM_INDEXED)); 
    }
    else { 
        if (positive) vg_draw_rectangle(xi_old, yi_old, pixmap->width, speed, xpm_transparency_color(XPM_INDEXED));
        else vg_draw_rectangle(xi_old, yi_old + pixmap->height - speed, pixmap->width, speed, xpm_transparency_color(XPM_INDEXED)); 
    }
}

void paint_pixel(uint16_t x, uint16_t y, int number_bytes, uint32_t color) {
    char* init_address = adress + (y*mode_info.XResolution + x) * number_bytes;
    for(int k = 0; k < number_bytes; k++) {
        init_address[k] = (color & get_mask(k)) >> (8 * k);
    }
}

int (vg_draw_hline)(uint16_t x, uint16_t y ,uint16_t len, uint32_t color){
    int number_bytes = mode_info.BitsPerPixel / 8;
    if(mode_info.BitsPerPixel % 8) number_bytes++;

    for(int i = 0; i < len; i++) //number of pixeis that should color
        paint_pixel(x+i, y, number_bytes, color);

    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    for(int j = y; j < height + y && j < mode_info.YResolution; j++){
        if(vg_draw_hline(x, j, width , color) != OK)
            return 1;
    }
    return 0;
}

void (get_info_vbe_specs)(vg_vbe_contr_info_t *info_p){
    mmap_t map;
    lm_alloc(512, &map);

    info_p->VBESignature[0] = 'V';
    info_p->VBESignature[1] = 'B';
    info_p->VBESignature[2] = 'E';
    info_p->VBESignature[3] = '2';
    map.virt = info_p;

    reg86_t r;
    memset(&r, 0, sizeof(r));

    r.ax = 0x4F00; // VBE call, function 00 -- get VBE mode
    r.intno = 0x10;
    r.es = PB2BASE(map.phys);
    r.di = PB2OFF(map.phys);
    if( sys_int86(&r) != OK ) { printf("INVALID KERNEL CALL!\n"); }

    *info_p = *(vg_vbe_contr_info_t *)map.virt;
    lm_free(&map);
}

/*void my_clear_img(uint16_t x, uint16_t y, uint16_t speed, xpm_image_t xpm, bool horizontal, bool positive){
    if(horizontal){
        if(positive) my_vg_draw_rectangle(x, y, speed, xpm.height, xpm_transparency_color(XPM_INDEXED));
        else my_vg_draw_rectangle(x + (xpm.width - speed), y, speed, xpm.height, xpm_transparency_color(XPM_INDEXED));
    }
    else{
        if(positive) my_vg_draw_rectangle(x, y, xpm.width, speed, xpm_transparency_color(XPM_INDEXED));
        else my_vg_draw_rectangle(x, y + (xpm.height - speed), xpm.width, speed, xpm_transparency_color(XPM_INDEXED));
    }
}*/
