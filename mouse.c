#include "types.h"
#include "defs.h"
#include "x86.h"
#include "traps.h"
#include "mouse.h"

void
mousewait_send(void)
{
  for(int i = 0; i < MOUSE_TIMEOUT_ITERATIONS; i++){
    if(!(inb(MOUSE_STATUS_PORT) & MOUSE_STATUS_INPUT_FULL))
      return;
  }
}

void
mousewait_recv(void)
{
  for(int i = 0; i < MOUSE_TIMEOUT_ITERATIONS; i++){
    if(inb(MOUSE_STATUS_PORT) & MOUSE_STATUS_OUTPUT_READY)
      return;
  }
}

void
mousecmd(uchar cmd)
{
  mousewait_send();
  outb(MOUSE_CMD_PORT, MOUSE_CMD_WRITE);
  mousewait_send();
  outb(MOUSE_DATA_PORT, cmd);
  mousewait_recv();
  inb(MOUSE_DATA_PORT);
}

void
mouseinit(void)
{
  uchar status;

  mousewait_send();
  outb(MOUSE_CMD_PORT, MOUSE_CMD_ENABLE);

  mousewait_send();
  outb(MOUSE_CMD_PORT, MOUSE_CMD_GET_STATUS);
  mousewait_recv();
  status = inb(MOUSE_DATA_PORT);

  status |= MOUSE_CTRL_ENABLE_IRQ12;
  mousewait_send();
  outb(MOUSE_CMD_PORT, MOUSE_CMD_SET_STATUS);
  mousewait_send();
  outb(MOUSE_DATA_PORT, status);

  mousecmd(MOUSE_DEFAULT_SETTINGS);
  mousecmd(MOUSE_ENABLE_STREAM);

  ioapicenable(IRQ_MOUSE, 0);
}

void
mouseintr(void)
{
  static uchar packet[3];
  static int packet_index = 0;
  // Mouse IRQ is routed to CPU0; interrupts are disabled in trap.

  while(inb(MOUSE_STATUS_PORT) & MOUSE_STATUS_OUTPUT_READY){
    packet[packet_index] = inb(MOUSE_DATA_PORT);
    if(packet_index == 0 && !(packet[0] & MOUSE_PACKET_SYNC)){
      packet_index = 0;
      continue;
    }
    packet_index++;
    if(packet_index == 3){
      if(packet[0] & MOUSE_BTN_LEFT)
        cprintf("LEFT\n");
      if(packet[0] & MOUSE_BTN_RIGHT)
        cprintf("RIGHT\n");
      if(packet[0] & MOUSE_BTN_MIDDLE)
        cprintf("MID\n");
      packet_index = 0;
    }
  }
}
