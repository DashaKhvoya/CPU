#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "Stack.h"

static const size_t numb_of_reg = 5;
static const size_t size_of_memory = 1000000;
static const size_t vram_begin = 0;
static const size_t vram_end = 10000;
static const size_t vram_width = 80;
static const size_t vram_height = 80;

enum Arguments {
  MEMORY = 0x1,
  REGISTER = 0x2,
  IMMED = 0x4,
  LABEL = 0x8
};

void MainArguments(int argc, char* argv[], const char** input);

struct String {
  size_t size;
  char* buf;
};

void ReadBinFile(const char* input, String* string);

#ifdef DEBUG
  #define CPU_ASSERT_OK(cpu) Dump(cpu); if(IsOk(cpu) != 0) { assert(0); }
#else
  #define CPU_ASSERT_OK(cpu) ;
 #endif

struct CPU {
  Stack_t stk;
  Stack_t call_stk;

  char* ram;

  char* code;
  size_t pc;

  double registers[numb_of_reg];

  bool run;
};

void CpuConstruct(CPU* cpu, const char* command_file);
void ReadCommand(const char* command_file, CPU* cpu);
void Counter(const char* command_file);
void WriteVRAM(CPU *cpu);

int IsOk(const CPU* cpu);
void Dump(const CPU* cpu);

#define DEF_CMD(name, num, arg, code) void (CPU_##name)(CPU* cpu);

#include "commands.h"

#undef DEF_CMD
