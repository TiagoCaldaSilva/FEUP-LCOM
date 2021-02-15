// KBD Interrupt Number
#define KBD_IRQ 0x01

//TIMER Interrupt number
#define TIMER0_IRQ 0

//MOUSE Interrupt number
#define MOUSE_IRQ 0x0C

// KBC I/O Ports
#define KBC_OUT_BUF 0x60
#define KBC_IN_BUF 0x64

#define KBC_STATUS_REG 0x64
#define KBC_CMD_REG 0x64

// KBC OH between polling cycles
#define WAIT_KBC 20000

// KBC`s errors verification bits
#define KBC_STATUS_PARITY BIT(7)
#define KBC_STATUS_TIMEOUT BIT(6)
#define KBC_STATUS_AUX BIT(5)
#define KBC_STATUS_INH BIT(4)
#define KBC_IBF BIT(1)
#define KBC_OBF BIT(0)

/* 
    Command Values to be written @KBC_CMD_REG.
    Some commands might require some arguments which must be wirtten @port 0x60
*/
#define KBC_READ_CMD_BYTE 0x20
#define KBC_WRITE_CMD_BYTE 0x60
#define CHECK_KBC 0xAA
#define CHECK_KBC_INTERFACE 0xAB
#define DISABLE_KBD_INTERFACE 0xAD
#define ENABLE_KBD_INTERFACE 0xAE

#define DISABLE_MOUSE_INTERFACE BIT(5)
#define DISABLE_KEYBOARD_INTERFACE BIT(4)
#define ENABLE_ITR_MOUSE_OBF BIT(1)
#define ENABLE_ITR_KEYBOARD_OBF BIT(0)

#define ESC_BREAKCODE 0x81
#define TWO_BYTES_CODE 0xE0
#define BREAKCODE_DETECTION BIT(7)


// Mouse Commands
#define WRITE_BYTE_MOUSE 0xD4

#define RESET 0xFF
#define RESEND 0xFE
#define SET_DEFAULTS 0xF6

#define DIS_DATA_REPORTING 0xF5
#define EN_DATA_REPORTING 0xF4

#define SET_SAMPLE_RATE 0xF3
#define SET_REMOTE_MODE 0xF0
#define SET_STREAM_MODE 0xEA

#define READ_DATA_PACKET 0xEB
#define STATUS_REQUEST 0xE9
#define SET_RESOLUTION 0xE8

#define ACC_MODE 0xE7
#define LINEAR_MODE 0xE6

#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC

#define LEFT_BUTTON BIT(0)
#define RIGHT_BUTTON BIT(1)
#define MIDDLE_BUTTON BIT(2)
#define BIT3 BIT(3)
#define MSB_X BIT(4)
#define MSB_Y BIT(5)
#define X_OV BIT(6)
#define Y_OV BIT(7)
