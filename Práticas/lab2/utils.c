#include <lcom/lcf.h>

#include <stdint.h>
 /*
//utiliza a mask FF para ler o byte menos significativo
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  
  //val &= 0x00FF;
  *lsb = (uint8_t)val;
  return 0;
}


//right shif no valor para associar o byte mais significativo
int(util_get_MSB)(uint16_t val, uint8_t *msb) {

  *msb = (uint8_t)(val >> 8);
  return 0;
}


//sys_inb trabalha apenas com words
//sys_inb lê no adresso port
//return 0 , se sys_inb tiver sucesso
int (util_sys_inb)(int port, uint8_t *value) {
  
  uint32_t value32;

  if(!sys_inb(port, &value32)) { *value = (uint8_t)value32; return 0; }
  return 1;
}
*/

#define LSB_MASK 0x00FF
#define MSB_MASK 0xFF00

int (util_sys_inb)(int port, uint8_t *value)
{
    uint32_t temp_value;
    if(sys_inb(port, &temp_value) != OK)
        return 1;
    *value = (uint8_t)temp_value;
    return 0;
}

int (util_get_LSB)(uint16_t value, uint8_t *lsb)
{
  *lsb = value & LSB_MASK;
  return 0;
}

int (util_get_MSB)(uint16_t value, uint8_t *msb)
{
  *msb = (value & MSB_MASK) >> 8;
  return 0;
}
