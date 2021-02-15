#include "rtc.h"

int rtc_hook_id = RTC_IRQ;

int rtc_subscribe_int(uint8_t* bit_number) {

    *bit_number = rtc_hook_id;
    if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hook_id) != OK) return 1;
    return 0;
}

int rtc_unsubscribe_int() {
    if(sys_irqrmpolicy(&rtc_hook_id) != OK) return 1;
    return 0;
}

int rtc_enable_conf(bool bcd, rtc_int_t interrupt) {

    uint8_t rtc_conf;
    sys_outb(RTC_ADDR_REG, RTC_REGISTER_B);
    proj_util_sys_inb(RTC_DATA_REG, &rtc_conf);

    rtc_conf = bcd ? rtc_conf | interrupt | ~RTC_DM : rtc_conf | interrupt | RTC_DM;
    
    sys_outb(RTC_ADDR_REG, RTC_REGISTER_B);
    sys_outb(RTC_DATA_REG, rtc_conf);
    return 0;
}

int rtc_disable_conf(rtc_int_t interrupt) {

    uint8_t rtc_conf;
    sys_outb(RTC_ADDR_REG, RTC_REGISTER_B);
    proj_util_sys_inb(RTC_DATA_REG, &rtc_conf);

    rtc_conf = rtc_conf & ~interrupt;
    
    sys_outb(RTC_ADDR_REG, RTC_REGISTER_B);
    sys_outb(RTC_DATA_REG, rtc_conf);
    return 0;
}


void rtc_wait_valid_time() {
    uint8_t dummy_byte = 0x00;

    do {
        sys_outb(RTC_ADDR_REG, RTC_REGISTER_A);
        proj_util_sys_inb(RTC_DATA_REG, &dummy_byte);
    } while(RTC_REGISTER_A & RTC_UIP);
}

uint8_t rtc_read_value(rtc_time_value_t time) {
    uint8_t time_value;
    rtc_wait_valid_time();

    sys_outb(RTC_ADDR_REG, time);
    proj_util_sys_inb(RTC_DATA_REG, &time_value);
    return time_value;
}

void rtc_write_value(rtc_time_value_t time, uint8_t time_value) {
    rtc_wait_valid_time();

    sys_outb(RTC_ADDR_REG, time);
    sys_outb(RTC_DATA_REG, time_value);
}

uint8_t rtc_interrupt_handler() {

    uint8_t regC_data, value = 0x00;
    sys_outb(RTC_ADDR_REG, RTC_REGISTER_C);
    proj_util_sys_inb(RTC_DATA_REG, &regC_data);

    if (regC_data & RTC_UF) {
        value |= BIT(0);

    } if (regC_data & RTC_AF) {
        value |= BIT(1);

    } if (regC_data & RTC_PF) {
        value |= BIT(2);
    }
    return value;
}

void rtc_set_alarm(uint8_t time_value) {

    uint8_t curr_sec = rtc_read_value(RTC_SECONDS);

    curr_sec = (curr_sec + time_value) % 0x3C; // target second value mod 60

    rtc_write_value(RTC_SECONDS_ALARM, curr_sec);
    rtc_write_value(RTC_MINUTES_ALARM, IGNORE);
    rtc_write_value(RTC_HOURS_ALARM, IGNORE);
}

/*void rtc_show_date() {
    uint8_t year = rtc_read_value(RTC_YEAR),
            month = rtc_read_value(RTC_MONTH),
            day = rtc_read_value(RTC_DAY),
            hour = rtc_read_value(RTC_HOURS),
            minutes = rtc_read_value(RTC_MINUTES),
            seconds = rtc_read_value(RTC_SECONDS);
}*/
