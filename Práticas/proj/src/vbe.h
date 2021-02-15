/* VBE MACROS */

#define VBE_FUNCTION(n) 0x4F00 | n  /**< @brief VBE function param */    
#define MODE(n) BIT(14) | n   /**< @brief VBE frame buffer mode */
#define VBE_INTNO 0x10    /**< @brief graphics interrupt number in protected mode */

#define VBE_CONTROLLER_INFO 0     /**< @brief VBE function 00h */
#define VBE_MODE_INFO 1   /**< @brief VBE function 01h */
#define VBE_SET_MODE 2    /**< @brief VBE function 02h */
#define VBE_DISPLAY_START 7   /**< @brief VBE function 03h */

#define DEFAULT_MODE 0x14C    /**< @brief graphic mode used in proj */
#define DEFAULT_NUMBER_BYTES_PER_PIXEL 4    /**< @brief number of bytes per pixel of DEFAULT_MODE */
