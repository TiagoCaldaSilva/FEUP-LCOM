// KBD Interrupt Number
#define KBD_IRQ 0x01    /**< @brief Keyboard irq line */
    
//MOUSE Interrupt number    
#define MOUSE_IRQ 0x0C  /**< @brief Mouse irq line */
   
// KBC I/O Ports    
#define KBC_OUT_BUF 0x60    /**< @brief KBC Output buffer port*/
#define KBC_IN_BUF 0x64 /**< @brief KBC Input buffer port */
    
#define KBC_STATUS_REG 0x64 /**< @brief KBC status register */
#define KBC_CMD_REG 0x64    /**< @brief KBC command register */
    
// KBC OH between polling cycles    
#define WAIT_KBC 20000  /**< @brief Overhead in microseconds required by KBC between polling cycles */
    
// KBC`s errors verification bits   
#define KBC_STATUS_PARITY BIT(7)    /**< @brief Parity Error Flag */
#define KBC_STATUS_TIMEOUT BIT(6)   /**< @brief Timeout Error Flag */
#define KBC_STATUS_AUX BIT(5)   /**< @brief Mouse Data Flag */
#define KBC_STATUS_INH BIT(4)   /**< @brief Inhibt KBC interrupts Flag */
#define KBC_IBF BIT(1)  /**< @brief KBC input buffer full */
#define KBC_OBF BIT(0)  /**< @brief KBC output buffer full */

/* 
    Command Values to be written @KBC_CMD_REG.
    Some commands might require some arguments which must be wirtten @port 0x60
*/
#define KBC_READ_CMD_BYTE 0x20  /**< @brief KBC Read Command Byte */
#define KBC_WRITE_CMD_BYTE 0x60 /**< @brief KBC Write Command Byte */
#define CHECK_KBC 0xAA 
#define CHECK_KBC_INTERFACE 0xAB
#define DISABLE_KBD_INTERFACE 0xAD
#define ENABLE_KBD_INTERFACE 0xAE

#define DISABLE_MOUSE_INTERFACE BIT(5)  /**< @brief Enable Mouse Interface */
#define DISABLE_KEYBOARD_INTERFACE BIT(4)   /**< @brief Disable Mouse Interface */
#define ENABLE_ITR_MOUSE_OBF BIT(1)
#define ENABLE_ITR_KEYBOARD_OBF BIT(0)

#define ESC_BREAKCODE 0x81  /**< @brief ESC breakcode */
#define TWO_BYTES_CODE 0xE0 /**< @brief ScanCode of 2 bytes identifier */
#define BREAKCODE_DETECTION BIT(7)  /**< @brief Breakcode Flag */


// Mouse Commands
#define WRITE_BYTE_MOUSE 0xD4 /**< @brief Send Byte to Mouse */

#define RESET 0xFF  /**< @brief Mouse Reset */
#define RESEND 0xFE /**< @brief For serial communications errors */
#define SET_DEFAULTS 0xF6   /**< @brief Set Default Values */

#define DIS_DATA_REPORTING 0xF5 /**< @brief Enable Data Reporting */
#define EN_DATA_REPORTING 0xF4  /**< @brief Disable Data Reporting */

#define SET_SAMPLE_RATE 0xF3    /**< @brief Sets state sampling rate */
#define SET_REMOTE_MODE 0xF0    /**< @brief Remote Set Mode */   
#define SET_STREAM_MODE 0xEA    /**< @brief Stream Set Mode */       

#define READ_DATA_PACKET 0xEB   /**< @brief Send data packet request */
#define STATUS_REQUEST 0xE9 /**< @brief Send data on events */

#define ACC_MODE 0xE7       /**< @brief Acceleration Mode */
#define LINEAR_MODE 0xE6    /**< @brief Linear Mode */

#define ACK 0xFA    /**< @brief Acknowledgement Byte */
#define NACK 0xFE   /**< @brief Non-acknowledgement Byte */
#define ERROR 0xFC  /**< @brief Error Byte */

// First Mouse Packet Byte Components
#define LEFT_BUTTON BIT(0)      /**< @brief Left Mouse Button Pressed */    
#define RIGHT_BUTTON BIT(1)     /**< @brief Right Mouse Button Pressed */
#define MIDDLE_BUTTON BIT(2)    /**< @brief Middle Mouse Button Pressed */    
#define MSB_X BIT(4)    /**< @brief Most significant bit of delta X*/
#define MSB_Y BIT(5)    /**< @brief Most significant bit of delta Y*/
#define X_OV BIT(6) /**< @brief Overflow in X axis Flag */
#define Y_OV BIT(7) /**< @brief Overflow in Y axis Flag */
