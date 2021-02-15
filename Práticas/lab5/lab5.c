// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include <math.h>

// Any header files included below this line should have been created by you
#include "keyboard.h"
#include "video_card.h"
/*
extern vbe_mode_info_t mode_info;
*/
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  vg_init(mode);
  sleep(delay);
  if(vg_exit() != OK)
    return 1;
  return 0;
}
/*
extern uint8_t data;
int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
    adress = vg_init(0x105);
    uint16_t xi_old = xi, yi_old = yi;
    if((xi == xf) && (yi == yf)){vg_exit(); return 1;}

    xpm_image_t img;
    img.bytes = xpm_load(xpm, XPM_INDEXED, &img);

    if(xi > info.XResolution) xi = info.XResolution - img.width;
    if(xf > info.XResolution) xf = info.XResolution - img.width;
    if(yi > info.YResolution) yi = info.YResolution - img.height;
    if(yf > info.YResolution) yf = info.YResolution - img.height;

    my_paint_img(img, xi, yi);

    uint8_t int_per_fr = sys_hz() / fr_rate;
    uint8_t counter_int = 0;
    bool horizontal = (yi == yf), positive = ((xi < xf) || (yi < yf)), finished = false;
    uint8_t t_bit_no;
    if(my_timer_subscribe_int(&t_bit_no) != OK) return 1;

    uint8_t k_bit_no;
    if(my_keyboard_subscribe_int(&k_bit_no) != OK) return 1;

    message msg;
    int r, ipc_status;
    uint32_t t_irq_set = BIT(t_bit_no), k_irq_set = BIT(k_bit_no);

    while(data != ESC_BREAKCODE){
        if((r = driver_receive(ANY, &msg, &ipc_status)) != OK){
            printf("driver_receive failed with %d", r);
            continue;
        }

        if(is_ipc_notify(ipc_status)){

            switch(_ENDPOINT_P(msg.m_source)){
                case HARDWARE:
                    if(msg.m_notify.interrupts & k_irq_set){
                        my_k_handler();
                        if(data == TWO_BYTES_CODE) my_k_handler();
                    }
                    if((msg.m_notify.interrupts & t_irq_set) && !finished){

                        counter_int++;
                        if(speed > 0){
                            if(counter_int % int_per_fr == 0){
                                if(horizontal){
                                    xi_old = xi;
                                    if(positive){xi += speed;xi = (xi > xf) ? xf:xi;}
                                    else{xi -= speed; xi = (xi < xf) ? xf:xi;}
                                }else{
                                    yi_old = yi;
                                    if(positive){yi += speed; yi = (yi > yf) ? yf:yi;}
                                    else{yi -= speed; yi = (yi < yf) ? yf:yi;}
                                }
                                finished = ((xi == xf) && (yi == yf));
                                
                                my_clear_img(xi_old, yi_old, speed, img, horizontal, positive);
                                my_paint_img(img, xi, yi);
                            }
                        }else{
                            if(counter_int  % (abs(speed) * int_per_fr) == 0){
                                if(horizontal){
                                  xi_old = xi;
                                    if(positive) xi++;
                                    else xi--;
                                }else{
                                    yi_old = yi;
                                    if(positive) yi++;
                                    else yi--;
                                }
                                finished = ((xi == xf) && (yi == yf));
                                my_clear_img(xi_old, yi_old, 1, img, horizontal, positive);
                                my_paint_img(img, xi, yi);
                            }
                        }
                    }
            }
        }
    }
    if(my_keyboard_unsubscribe_int() != OK) return 1;
    if(my_timer_unsubscribe_int() != OK) return 1;

    vg_exit();
    return 0;
}*/

char * adress;
extern int empty_buffer, error_raised, timer_counter;
extern uint8_t data;
vbe_mode_info_t mode_info;
int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  adress = vg_init(mode);
  uint16_t len = width;
  if((x + width) > mode_info.XResolution)
      len = mode_info.XResolution - x;
  if(mode == 0x110)
    color = color & 0x7FFF;
  for(int j = y; j < height + y && j < mode_info.YResolution; j++){
    if(vg_draw_hline(x, j, len , color) != OK)
      return 1;
  }
  
  kbc_interrupt(ESC_BREAKCODE);
  vg_exit(); 
  return 0;
}

uint32_t getRGBmask(uint8_t size){
  uint32_t mask = 0;
  for(int i = 0; i < size; i++)
    mask |= (1 << i);
  return mask;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  adress = vg_init(mode);
  uint32_t color;
  uint16_t width = mode_info.XResolution / no_rectangles, height = mode_info.YResolution / no_rectangles;

  for(unsigned int row = 0; row < no_rectangles; row++) {
    for(unsigned int col = 0; col < no_rectangles; col++) {

      // Direct Mode
      if(mode_info.RedMaskSize != 0){ 
        uint8_t R = (((first >> (mode_info.RedFieldPosition)) & getRGBmask(mode_info.RedMaskSize)) + col * step) % (1 << mode_info.RedMaskSize); 
        uint8_t G = (((first >> (mode_info.GreenFieldPosition)) & getRGBmask(mode_info.GreenMaskSize)) + row * step) % (1 << mode_info.GreenMaskSize); 
        uint8_t B = (((first >> (mode_info.BlueFieldPosition)) & getRGBmask(mode_info.BlueMaskSize)) + (col + row) * step) % (1 << mode_info.BlueMaskSize);
        color = (R << mode_info.RedFieldPosition) | (G << mode_info.GreenFieldPosition)| B << mode_info.BlueFieldPosition;
        if (mode == 0x110)  color &= 0x7FFF;
      }
      else{
        color = (first + (row * no_rectangles + col) * step) % (1 << mode_info.BitsPerPixel);
      }
      if(vg_draw_rectangle(width * col, height * row, width,  height, color) != OK)
        return 1;
    }
  }

  kbc_interrupt(ESC_BREAKCODE);
  vg_exit(); 
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    adress = vg_init(0x105);

    // video mem
    xpm_image_t pixmap;

    pixmap.bytes = xpm_load(xpm, XPM_INDEXED, &pixmap);
    paint_img(x, y, &pixmap);

    kbc_interrupt(ESC_BREAKCODE);
    vg_exit();
    return 0;
}


extern int counter_timer;
int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
    adress = vg_init(0x105);

    xpm_image_t pixmap;
    pixmap.bytes = xpm_load(xpm, XPM_INDEXED, &pixmap);

    if (xi > mode_info.XResolution) xi = mode_info.XResolution;
    if (xf > mode_info.XResolution) xf = mode_info.XResolution;
    if (yi > mode_info.XResolution) yi = mode_info.YResolution;
    if (yf > mode_info.XResolution) yf = mode_info.YResolution;

    paint_img(xi, yi, &pixmap);
    
    if (xi != xf && yi != yf) { vg_exit(); return 1; }
    
    bool horizontal = (yf == yi);

    bool finished = false;

    bool positive_side = yi < yf || xi < xf;

    uint16_t xi_old = xi, yi_old = yi;
    
    // Positive speed means that speed represents the pixels displaced per frame
    // Negative speed means the number of frames needed to move a pixel 
    uint8_t interrupts_per_frames = sys_hz() / fr_rate;
    uint8_t counter_int = 0;
    uint8_t timer_bit_no, kbd_bit_no;
    message msg;
    int r, ipc_status;
    
    if((timer_subscribe_int)(&timer_bit_no) != OK) return 1;
    if((kbd_subscribe_int)(&kbd_bit_no) != OK) return 1;

    uint32_t timer_irq_set = BIT(timer_bit_no), kbd_irq_set = BIT(kbd_bit_no);

    while(data != ESC_BREAKCODE) {

        if((r = driver_receive(ANY, &msg, &ipc_status)) != OK){
            printf("driver_receive failed with: %d", r); continue;
        }
        if(is_ipc_notify(ipc_status)){
          switch(_ENDPOINT_P(msg.m_source)){
            case HARDWARE:
              if (msg.m_notify.interrupts & kbd_irq_set) {
                  kbc_ih();
                  if(data == TWO_BYTES_CODE) kbc_ih();
              }
              if(msg.m_notify.interrupts & timer_irq_set) {
                  counter_int++;
                  if(counter_int % interrupts_per_frames == 0){
                      if(!finished){
                          if (speed > 0) {
                              if(!finished){
                                if (horizontal) {
                                  xi_old = xi;
                                  if (positive_side) { 
                                    xi += speed;
                                    if (xi > xf) xi = xf;
                                  }
                                  else {
                                    xi -= speed;
                                    if (xi < xf) xi = xf;
                                  }
                              } else {
                                  yi_old = yi;
                                  if (positive_side) { 
                                    yi += speed;
                                    if (yi > yf) yi = yf;
                                  }
                                  else {
                                    yi -= speed;
                                    if (yi < yf) yi = yf;
                                  }
                              }
                              finished = (xi == xf && yi == yf);
                              clear_img(xi_old, yi_old, speed, &pixmap, horizontal, positive_side);
                              paint_img(xi, yi, &pixmap);
                            }  
                          } else {
                              if ((counter_int / interrupts_per_frames) % abs(speed) == 0) {
                                  if (horizontal) {
                                      xi_old = xi;
                                      if (positive_side) xi++;
                                      else xi--;
                                  } else {
                                      yi_old = yi;
                                      if (positive_side) yi++;                            
                                      else yi--; 
                                  }
                                  clear_img(xi_old, yi_old, 1, &pixmap, horizontal, positive_side);
                                  paint_img(xi, yi, &pixmap);
                                  finished = (xi == xf && yi == yf);
                              }
                          }
                      }
                  }
              }
              break;
            default:
              break;
          }
        } 
    }

    if(kbd_unsubscribe_int() != OK)
      return 1;
    if(timer_unsubscribe_int() != OK)
      return 1;
    vg_exit();
    return 0;
}


int(video_test_controller)() {
  vg_vbe_contr_info_t video_card_info;
  get_info_vbe_specs(&video_card_info);
  if (vg_display_vbe_contr_info(&video_card_info) != OK) return 1;
  return 0;
}
