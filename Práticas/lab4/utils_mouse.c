#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

int (util_sys_inb)(int port, uint8_t *value) {

  uint32_t value32;
  if(!sys_inb(port, &value32)) {
    *value = (uint8_t)value32; return 0; 
  }
  return 1;
}

