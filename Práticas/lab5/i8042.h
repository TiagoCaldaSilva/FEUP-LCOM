#define KBD_IRQ 1
#define TIMER_IRQ 0
#define ESC_BREAKCODE 0x81
#define TWO_BYTES_CODE 0xE0
#define IN_BUF 0x64
#define OUT_BUF 0x60
#define STATUS_REG 0x64

#define IN_BIT BIT(1)
#define OUT_BIT BIT(0)

#define PARITY_ERROR BIT(7)
#define TIMEOUT_ERROR BIT(6)
#define AUX_ERROR BIT(5)
#define BREAK_BIT BIT(7)

#define READ_CMD 0x20
#define WRITE_CMD 0x60
#define WAIT_KBC 20000

#define ENABLE_BIT BIT(0)
