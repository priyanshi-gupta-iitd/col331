// Mouse controller constants
#define MOUSE_DATA_PORT    0x60
#define MOUSE_STATUS_PORT  0x64
#define MOUSE_CMD_PORT     0x64

#define MOUSE_CMD_WRITE          0xD4
#define MOUSE_CMD_ENABLE         0xA8
#define MOUSE_CMD_GET_STATUS     0x20
#define MOUSE_CMD_SET_STATUS     0x60

#define MOUSE_DEFAULT_SETTINGS   0xF6
#define MOUSE_ENABLE_STREAM      0xF4

#define MOUSE_STATUS_OUTPUT_READY 0x01
#define MOUSE_STATUS_INPUT_FULL   0x02
#define MOUSE_CTRL_ENABLE_IRQ12   0x02
#define MOUSE_PACKET_SYNC         0x08
#define MOUSE_TIMEOUT_ITERATIONS  100000

#define MOUSE_BTN_LEFT           0x01
#define MOUSE_BTN_RIGHT          0x02
#define MOUSE_BTN_MIDDLE         0x04
