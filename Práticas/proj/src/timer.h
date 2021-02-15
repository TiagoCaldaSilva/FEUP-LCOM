#include <lcom/lcf.h>
#include <stdint.h>

#include "i8254.h"
#include "utils.h"

#define DEFAULT_FPS 60 /**< @brief default framerate */

/**
 * @brief Subscribes timer interruptions
 * 
 * @param bit_number pic number line 
 * @return int 0 upon success, non-zero otherwise
 */
int (timer_subscribe_int)(uint8_t * bit_number);
/**
 * @brief Unsubscribes timer interruptions
 * 
 * @return int 0 upon success, non-zero otherwise
 */
int (timer_unsubscribe_int)();

#define CONFIG_MASK 0xF /**< @brief Mask to make the least 4 bits of timer configuration remain constant */

/**
 * @brief Sets the timer to a given frequency
 * 
 * @param timer timer whose frequency will be changed
 * @param freq 
 * @return int 0 upon success, non-zero otherwise
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

#define MSB_MASK 0xFF00 /**< @brief Most significant byte mask */
#define LSB_MASK 0x00FF /**< @brief Least significantbyte mask */

/**
 * @brief Returns the configuration of a given timer
 * 
 * @param timer timer whose configuration will be retrieved
 * @param st value with the current configuration
 * @return int 0 upon success, non-zero otherwise
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);
