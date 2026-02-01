// Per-CPU state
struct cpu {
  uchar apicid;                // Local APIC ID
};

extern struct cpu cpus[NCPU]; //array of cpu with NCPU cpus
extern int ncpu;  //active cpus at boot