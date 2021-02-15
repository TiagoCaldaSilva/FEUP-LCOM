#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

#include "utils.h"

#define RTC_ADDR_REG 0x70   /**< @brief  RTC Address register */
#define RTC_DATA_REG 0x71   /**< @brief  RTC Data register */
    
#define RTC_IRQ 0x08    /**< @brief RTC IRQ line */
    
#define RTC_REGISTER_A 0x0A /**< @brief RTC register A */
#define RTC_REGISTER_B 0x0B /**< @brief RTC register B */
#define RTC_REGISTER_C 0x0C /**< @brief RTC register C */
#define RTC_REGISTER_D 0x0D /**< @brief RTC register D */

#define IGNORE 0xFF /**< @brief Ignore value for alarms */

// Bits of register A   
#define RTC_UIP BIT(7)  /**< @brief Update in Progress Flag */

// Bits of register B   
#define RTC_SET_INT BIT(7)  /**< @brief Set interrupts Flag */
#define RTC_PIE BIT(6)  /**< @brief Periodic Interrupt Enabled Flag */
#define RTC_AIE BIT(5)  /**< @brief Alarm Interrupt Enabled Flag */
#define RTC_UIE BIT(4)  /**< @brief Update Interrupt Enabled Flag */
#define RTC_SQWE BIT(3) /**< @brief Square Wave Flag */
#define RTC_DM BIT(2)   /**< @brief BCD/Binary Flag */
#define RTC_24_12 BIT(1)    /**< @brief 24/12 format Flag */
    
// Bits of Register C   
#define RTC_PF BIT(6)   /**< @brief Periodic Interrupt Flag */
#define RTC_AF BIT(5)   /**< @brief Alarm Interrupt Flag */
#define RTC_UF BIT(4)   /**< @brief Update Interrupt Flag */

/**
 * @brief Contains the value for acessing the correponding RTC register
 * 
 */
typedef enum rtc_time_value {
    RTC_SECONDS = 0x00,
    RTC_SECONDS_ALARM = 0x01,
    RTC_MINUTES = 0x02,
    RTC_MINUTES_ALARM = 0x03,
    RTC_HOURS = 0x04,
    RTC_HOURS_ALARM = 0x05,
    RTC_DAY_OF_WEEK = 0x06,
    RTC_DAY = 0x07,
    RTC_MONTH = 0x08,
    RTC_YEAR = 0x09
} rtc_time_value_t;

/**
 * @brief Contains the value for setting the bit of the correponding RTC interrupt type
 * 
 */
typedef enum rtc_interrupt {
    RTC_PERIODIC = 0x40,
    RTC_ALARM = 0x20,
    RTC_UPDATE = 0x10
} rtc_int_t;


/**
 * @brief Subscribes RTC interruptions
 * 
 * @param bit_number pic number line 
 * @return int 0 upon success, non-zero otherwise
 */
int rtc_subscribe_int(uint8_t *bit_no);
/**
 * @brief Unsubscribes RTC interruptions
 * 
 * @return int 0 upon success, non-zero otherwise
 */
int rtc_unsubscribe_int();

/**
 * @brief Enables Interrupts of the type passed as interrupt
 * 
 * @param bcd True for BCD, False for Binary
 * @param interrupt Interrupt type
 * @return int 0 upon success, non-zero otherwise
 */
int rtc_enable_conf(bool bcd, rtc_int_t interrupt);
/**
 * @brief Disables Interrupts of the type passed as interrupt
 * 
 * @param interrupt Interrupt type
 * @return int 0 upon success, non-zero otherwise
 */
int rtc_disable_conf(rtc_int_t interrupt);

/**
 * @brief Makes sure none of the register are operation while an update is in progress
 * 
 */
void rtc_wait_valid_time();

/**
 * @brief Read the value of the register passed as time
 * 
 * @param time register to be accessed
 * @return uint8_t value present in the resgister
 */
uint8_t rtc_read_value(rtc_time_value_t time);
/**
 * @brief Writes a value to the register passed as time
 * 
 * @param time register to be accessed
 * @param time_value value to be written
 */
void rtc_write_value(rtc_time_value_t time, uint8_t time_value);

/**
 * @brief Handles an rtc interrupt
 * 
 * @return uint8_t Containing info about the interrupt Flags enabled
 */
uint8_t rtc_interrupt_handler();

/**
 * @brief Sets an alarm for the next time_value seconds
 * 
 * @param time_value value in seconds
 */
void rtc_set_alarm(uint8_t time_value);

/**
 * @brief Shows the current system date based in the values of the RTC Registers
 * 
 */
void rtc_show_date();
