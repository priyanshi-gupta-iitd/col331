// The I/O APIC manages hardware interrupts for an SMP system.
// http://www.intel.com/design/chipsets/datashts/29056601.pdf
// See also picirq.c.

#include "types.h"
#include "defs.h"
#include "traps.h"

#define IOAPIC  0xFEC00000   // Default physical address of IO APIC
                              //memory mapped io
#define REG_ID     0x00  // Register index: ID
#define REG_VER    0x01  // Register index: version
#define REG_TABLE  0x10  // Redirection table base

// The redirection table starts at REG_TABLE and uses
// two registers to configure each interrupt. (each hw irq has two registers one at offset 2*irq and other at 2*irq+1)
// The first (low) register in a pair contains configuration bits. (2*irq )
// The second (high) register contains a bitmask telling which (2*irq+1)
// CPUs can serve that interrupt.
#define INT_DISABLED   0x00010000  // Interrupt disabled

volatile struct ioapic *ioapic;

// IO APIC MMIO structure: write reg, then read or write data.
struct ioapic {
  uint reg; //register number
  uint pad[3];
  uint data;
}; //matches hardware layout

static uint ioapicread(int reg)
{
  ioapic->reg = reg;
  return ioapic->data;
}

static void ioapicwrite(int reg, uint data)
{
  ioapic->reg = reg;
  ioapic->data = data;
}

void ioapicinit(void)
{
  int i, id, maxintr;

  ioapic = (volatile struct ioapic*)IOAPIC; //mapping ioapic
  maxintr = (ioapicread(REG_VER) >> 16) & 0xFF; //find max irq
  id = ioapicread(REG_ID) >> 24; //verify id
  if(id != ioapicid)
    cprintf("ioapicinit: id isn't equal to ioapicid; not a MP\n");

  // Mark all interrupts edge-triggered, active high, disabled,
  // and not routed to any CPUs.
  for(i = 0; i <= maxintr; i++){ //disabling all interrupts for booting
    ioapicwrite(REG_TABLE+2*i, INT_DISABLED | (T_IRQ0 + i));
    ioapicwrite(REG_TABLE+2*i+1, 0); //off
  }
}

void ioapicenable(int irq, int cpunum) //enable one interrupt
{
  // Mark interrupt edge-triggered, active high,
  // enabled, and routed to the given cpunum,
  // which happens to be that cpu's APIC ID.
  ioapicwrite(REG_TABLE+2*irq, T_IRQ0 + irq); //set vector
  ioapicwrite(REG_TABLE+2*irq+1, cpunum << 24); //sending to cpu
}
