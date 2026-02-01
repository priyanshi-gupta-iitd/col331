#include "types.h" //data typedefs
#include "defs.h" //global header file, has all definitions
#include "param.h" //gives max number of cpus
#include "mmu.h" 
#include "x86.h" //injects x86 assembly 
#include "proc.h" //contains struct of cpu

// Must be called with interrupts disabled
int cpuid() {
  return mycpu()-cpus; //subtracting pointers to get CPU id
}

// Must be called with interrupts disabled to avoid the caller being
// rescheduled between reading lapicid and running through the loop.
 
struct cpu* mycpu(void) //returns pointer to current cpu
{
  int apicid, i; 

  if(readeflags()&FL_IF) panic("mycpu called with interrupts enabled\n");

  apicid = lapicid(); //function to get LAPIC id
  // APIC IDs are not guaranteed to be contiguous. Maybe we should have
  // a reverse map, or reserve a register to store &cpus[i].
  for (i = 0; i < ncpu; ++i) {
    if (cpus[i].apicid == apicid) //if APIC ID matches
      return &cpus[i];
  }
  panic("unknown apicid\n");
}