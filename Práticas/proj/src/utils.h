#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Wrapper function of sys_inb()
 * 
 * @param port in which the value will be written
 * @param value to write in the corresponding port
 * @return int 0 upon success, non-zero otherwise
 */
int proj_util_sys_inb(uint8_t port, uint8_t *value);

/**
 * @brief Determines if a value is inside a given range
 * 
 * @param value value to be analyzed
 * @param min lower limit of the interval
 * @param max upper limit of the interval
 * @return true if value is part of the interval
 * @return false if the value is not part of the interval
 */
bool between(uint16_t value, uint16_t min, uint16_t max);

/**
 * @brief Returns the greatest value between parameters
 * 
 * @param n1 number to be compared 
 * @param n2 number to be compared
 * @return int value with the greatest number
 */
int max(int n1, int n2);
/**
 * @brief Returns the smallest value between parameters
 * 
 * @param n1 number to be compared 
 * @param n2 number to be compared
 * @return int value with the smallest number
 */
int min(int n1, int n2);

/**
 * @brief Converts a decimal number to the corresponding hexadecimal
 * 
 * @param num Decimal number
 * @param result The value of num in hexadecimal
 */
void decimal_to_hex(int num, uint8_t * result);
