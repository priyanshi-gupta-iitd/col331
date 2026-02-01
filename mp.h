// See MultiProcessor Specification Version 1.[14]

struct mp {             // floating pointer
  uchar signature[4];           // "_MP_", to identify this as MP info
  void *physaddr;               // phys addr of MP config table, somewhere in physical memory placed by BIOS(not a C pointer)
                              //We do not make it struct mpconf* as we will recast it later, generic pointer for now
  uchar length;                 // 1, size of this struct 
  uchar specrev;                // [14] , specification version
  uchar checksum;               // for the struct ,all bytes must add up to 0
  uchar type;                   // MP system config type
  uchar imcrp;  //tells if system can switch from PIC to APIC
  uchar reserved[3]; //unused to make struct 16 byte aligned
};

//Since i am compiling with -m32 flage , the environment is 32 bit , so void * or any other pointer would be 4 bytes , thus the struct mp is 16 bit 


//Main configuration table, consists full description of multiprocessor system
struct mpconf {         // configuration table header
  uchar signature[4];           // "PCMP"
  ushort length;                // total table length
  uchar version;                // [14]
  uchar checksum;               // all bytes must add up to 0
  uchar product[20];            // product id, vendor string not important for OS
  uint *oemtable;               // OEM table pointer
  ushort oemlength;             // OEM table length
  ushort entry;                 // entry count after this struct in memory 
  uint *lapicaddr;              // address of local APIC
  ushort xlength;               // extended table length
  uchar xchecksum;              // extended table checksum
  uchar reserved;
};

//texhnically a cpu
struct mpproc {         // processor table entry
  uchar type;                   // entry type (0)
  uchar apicid;                 // local APIC id , used to send interrupts
  uchar version;                // local APIC version
  uchar flags;                  // CPU flags
    #define MPBOOT 0x02           // This proc is the bootstrap processor.
  uchar signature[4];           // CPU signature
  uint feature;                 // feature flags from CPUID instruction
  uchar reserved[8];
};

//aligned at multiples of 4

//technically ioapic
struct mpioapic {       // I/O APIC table entry
  uchar type;                   // entry type (2)
  uchar apicno;                 // I/O APIC id
  uchar version;                // I/O APIC version
  uchar flags;                  // I/O APIC flags
  uint *addr;                  // I/O APIC address (memory address)
};

// Table entry types
#define MPPROC    0x00  // One per processor
#define MPBUS     0x01  // One per bus
#define MPIOAPIC  0x02  // One per I/O APIC
#define MPIOINTR  0x03  // One per bus interrupt source
#define MPLINTR   0x04  // One per system interrupt source

//PAGEBREAK!
// Blank page.

/*
after struct mpconf memory is like this, each entry is of above mentioned types
+--------------------+
| struct mpconf      |  <-- header
+--------------------+
| entry #1           |  (mpproc / mpioapic / etc.)
+--------------------+
| entry #2           |
+--------------------+
| entry #3           |
+--------------------+
| ...                |
 */