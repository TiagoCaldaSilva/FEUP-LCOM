#include <machine/int86.h>

// INDEXED MODES
#define MODE_640X400_256C   0x100
#define MODE_640X480_256C   0x101
#define MODE_800X600_16C    0x102
#define MODE_800X600_256C   0x103
#define MODE_1024X678_16C   0x104
#define MODE_1024X678_256C  0x105
#define MODE_1280X1024_16C  0x106
#define MODE_1280X1024_256C 0x107

void (get_info)(uint16_t mode, vbe_mode_info_t *info);
void (get_pallet)(uint16_t *pallet);

void* (vg_init)(uint16_t mode);

void (paint_img)(uint16_t xi, uint16_t yi, xpm_image_t* img);
void clear_img(uint16_t xi_old, uint16_t yi_old, uint16_t speed, xpm_image_t* pixmap, bool horizontal, bool positive);
void (paint_pixel)(uint16_t x, uint16_t y, int number_bytes, uint32_t color);

int (vg_draw_hline)(uint16_t x, uint16_t y ,uint16_t len,uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (kbc_interrupt)(uint8_t breakcode);

void (get_info_vbe_specs)(vg_vbe_contr_info_t *info_p);
