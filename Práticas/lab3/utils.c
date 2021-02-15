#include <lcom/lcf.h>
#include <stdint.h>

int cnt = 0;

int (util_sys_inb)(int port, uint8_t *value)
{
    uint32_t temp;
    #ifdef LAB3
    cnt++;
    #endif
    if(sys_inb(port, &temp) != OK)
        return 1;
    *value = (uint8_t)temp;
    return 0;
}
